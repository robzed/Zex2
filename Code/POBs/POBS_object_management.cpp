// *************************************MODULE HEADER*********************************
// * FILENAME           : POBS_object_management.cpp
// * MODULE TITLE       : Load and Unload objects from the system automatically.
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 9th January 2001
// * FIRST AUTHOR       : Rob
// *
// * COPYRIGHT (c)        Lightsoft 2002
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************


// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

  #if  (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <fp.h>
    #include <types.h>
    #include <math.h>
  #endif
  
  #include "3d_structures_def.h"  
  #include "Error_handler.h"
  #include "access_ocbs.h"
  #include "zex_misc.h"
#endif //pch

#include "POBS.h"
#include "camera_control.h"
#include "Galaxy.h"
#include "object_control_top.h"
#include "fpoly4.h"
#include "genpoly.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "load_game.h"


// ***********************************************************************************
// * CONSTANTS 
// *

#define module_private static

#define MAX_POBS_ENTRIES 500

// ***********************************************************************************
// * TYPE DEFINITIONS
// *

typedef struct {


// common entries
	   double x_in_m;
	   double y_in_m;
	   double z_in_m;
	   double mass_in_kg;
	   double diameter_in_m;
           double rotation_time_in_sec;
	   int galactic_id;
	   int POB_type;
	   int seed;
	   int flags;
	   int dynamic_ocb_id;
           float red,green,blue;
//	   int static_ocb_id;		// the source model

// both planets/moons and motherships, not suns
           double population;		// both planets/moons and motherships
           int supply_rating;		// both planets/moons and motherships
           int supply_type;		// motherships
           
// motherships only
           int allegiance;		// motherships only
           int alien_ships_docked;
           int human_ships_docked;
           int neutral_ships_docked;
           
// suns only
           int sun_number;		// suns only

// creates linked list of objects
            int orbit_parent;		// -1 = none
            int first_orbiting_child;	// e.g. first moon or spacestation around a planet, or first spacestation around a moon
            int	next_linked_object;	// -1 = none, e.g. previous moon if this is a moon
            int	previous_linked_object;	// -1 = none
           
       } POBs_data_structure;

// ***********************************************************************************
// * DATA STRUCTURE DECLARATIONS 
// *
       
POBs_data_structure POBs_data[MAX_POBS_ENTRIES];

#define default_flags(id) (POBs_data[id].flags) = 0;

// loaded into dynamic ocbs currently?
#define  is_POB_loaded(id) ((POBs_data[id].flags) &   0x0001)
#define set_POB_loaded(id) ((POBs_data[id].flags) |=  0x0001)
#define clr_POB_loaded(id) ((POBs_data[id].flags) &= ~0x0001)

#define  is_POB_always_loaded(id) ((POBs_data[id].flags) &   0x0002)
#define set_POB_always_loaded(id) ((POBs_data[id].flags) |=  0x0002)
#define clr_POB_always_loaded(id) ((POBs_data[id].flags) &= ~0x0002)

// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

int number_of_active_POBs;
int private_galactic_id_store;
int POBs_big_switch;		// to turn on and off

int constructing_linked_list=0;		// boolean = private, means we are in the primary solar system load phase


// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

// general functions
int next_POB(int last_POB);
Boolean is_inside_active_circle(int id);
void check_POBS_for_object_placement(void);
int generate_new_POB(void);
module_private void real_load_object(int POBs_id);
module_private void real_unload_object(int POBs_id);
void check_galactic_id_is_POBs_index(int gal_id);
void get_random_colour(float *red, float *green, float *blue);
int check_if_POB_is_near(int id);

// from POBS_game_data_accessors.c
void construct_mothership_supply_ratings(void);
double POBs_calc_distance_from_suns(int gal_id);
double calculate_a_planet_moon_population(int gal_id);
int find_planet_of_group(int gal_id);
int count_moons_around_planet(int gal_id);
int count_local_earth_sized_objects_around_planet(int gal_id);
int mothership_supply_rating_calculation(int gal_id);
int mothership_supply_type_calculation(int gal_id);
int planet_moon_supply_type_calculation(int gal_id);
int planet_moon_supply_rating_calculation(int gal_id);
double calculate_mothership_population(int gal_id);
void setup_docked_ships_count(int galactic_id);
void set_up_a_mothership_supply_rating(int gal_id);


// planet structure linked list functions
int find_last_in_this_link_level(int gal_id);
void link_inserted_object(int gal_id, int gal_id_it_orbits);
module_private void link_new_mothership(int gal_id);
module_private void link_new_moon(int gal_id);
module_private void link_new_planet(int gal_id);
module_private void finish_linking_new_system(void);
module_private void link_new_system(void);
module_private void no_linkage(int gal_id);


// ***********************************************************************************
// * ACTUAL CODE
// *


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----


void service_POBS(void)
{
if(POBs_big_switch)
  {
  check_POBS_for_object_placement();
  }
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: POBs_init               | AUTHOR(s): Rob Probin   | DATE STARTED: 20 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

// hack
int robs_dyn_slot_count=0;
int robs_dyn_slot=-123456;		// temporary hack

void POBs_init(void)
{
number_of_active_POBs=0;
POBs_big_switch=1;
private_galactic_id_store = -1;	// galactic id's start from -2 ... -1 is non-value

link_new_system();

// hack
robs_dyn_slot_count=0;
robs_dyn_slot=-123456;
}




// +---------------------------------------------------------------+-------------------------+-----------------------
// | TITLE: inform_POBs_we_have_finished_constructing_solar_system | AUTHOR(s): Rob Probin   | DATE STARTED: 17 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void inform_POBs_we_have_finished_constructing_solar_system(void)
{
finish_linking_new_system();
construct_mothership_supply_ratings();
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 20 Jan 02
// +
// | DESCRIPTION: checks POBS for movement over the miniblock size
// +----------------------------------------------------------------ROUTINE HEADER----

int next_POB(int last_POB)
{
last_POB++;
if(last_POB>=number_of_active_POBs)
  {
  last_POB=0;
  }
  
return last_POB;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 20 Jan 02
// +
// | DESCRIPTION:  check current camera position vs. object position
// +----------------------------------------------------------------ROUTINE HEADER----

Boolean is_inside_active_circle(int id)
{
_3D camera_pos, other_pos;

if(is_POB_loaded(id))
  {
  get_position_in_m(POBs_data[id].dynamic_ocb_id,&other_pos); // get position from dynamic OCBs via our copy of dynamic id
  }
else	// object is not loaded
  {
  other_pos.x = POBs_data[id].x_in_m;
  other_pos.y = POBs_data[id].y_in_m;
  other_pos.z = POBs_data[id].z_in_m;
  }
  
  get_position_in_m(get_main_camera_object(),&camera_pos); // get zex position

#define IN_BAND 5e10	// 50 000 000km


  if(fabs(camera_pos.x-other_pos.x) > IN_BAND) return FALSE;	// if outside band, return false to question
  if(fabs(camera_pos.y-other_pos.y) > IN_BAND) return FALSE;
  if(fabs(camera_pos.z-other_pos.z) > IN_BAND) return FALSE;
  
  return TRUE;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int check_if_POB_is_near(int id)
{

  if(is_inside_active_circle(id))
    {
    if(!is_POB_loaded(id))			// we only load one per frame...
      {
      real_load_object(id);
      return TRUE;				// only load one object per frame
      }
    }
  else // unload if not
    {
    if(!is_POB_always_loaded(id))
       {
       real_unload_object(id);
       }
    }

return FALSE;
} // check_if_POB_is_near()

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 20 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void check_POBS_for_object_placement(void)
{
static int frame_skipper=0;
static int id=0;
int count;

if(frame_skipper) { frame_skipper--; return; } // don't scan if we loaded on previous frame
if(number_of_active_POBs==0) return;	// nothing to do then return

for(count=0; count<50; count++)		// only search some of the objects per parse
  {
  if(check_if_POB_is_near(id)) { frame_skipper=1; return; } // only load one per alternate frame
  id=next_POB(id);
  }
      
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 20 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void check_all_POBS(void)
{
int id;

if(number_of_active_POBs==0) return;	// nothing to do then return

for(id=0; id < number_of_active_POBs; id++)
  {
  check_if_POB_is_near(id);
  }
  
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int get_new_galactic_id(void)
{
private_galactic_id_store--;	  // galactic id's start from -2 ... -1 is non-value
return private_galactic_id_store;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

// 
// store all planets
// kill all objects
// store player coordinates/camera coordinates
void POBS_unload_scene(void)
{
int id;

if(number_of_active_POBs==0) return;	// nothing to do then return

for(id=0; id < number_of_active_POBs; id++)
  {
  real_unload_object(id);
  }

}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----
void POBS_load_scene(void)
{
int id;

if(number_of_active_POBs==0) return;	// nothing to do then return

for(id=0; id < number_of_active_POBs; id++)
  {
  if(is_POB_always_loaded(id))
    {
    real_load_object(id);
    }
  else
    {
    check_if_POB_is_near(id);    
    }
  }
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int generate_new_POB(void)
{
if(number_of_active_POBs>=MAX_POBS_ENTRIES) { MyDebugStr(__LINE__,"POBS_object_managment.c","Exceeded Maximum POBs"); }
number_of_active_POBs++;		 	 // new object
return number_of_active_POBs-1;		// copy of POB
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 20 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----


// +--------------------------------+-------------------------+-----------------------
// | TITLE: get_random_colour       | AUTHOR(s): Rob Probin   | DATE STARTED: 10 Feb 02
// +
// | DESCRIPTION: Return a random colour, which is not washed out (most of the time)
// +----------------------------------------------------------------ROUTINE HEADER----

void get_random_colour(float *red, float *green, float *blue)
{
  *red=RangedRdm(0,256)/255.0;
  *green=RangedRdm(0,256)/255.0;
  *blue=RangedRdm(0,256)/255.0;

  if(*blue+*red+*green < 0.6)
    {
           switch(RangedRdm(0,3))
             {
             case 0:
                *red+=.5; if(*red>1) *red=1;
                break;
             case 1:
                *green+=.5; if(*green>1) *green=1;
                break;
             default:
                *blue+=.5; if(*blue>1) *blue=1;
                break;
           }
    }
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 20 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

enum { POBs_planet,
       POBs_moon,
       POBs_sun,
       POBs_spacestation,
       POBs_player_ship,
       POBs_player_vv,
       POBs_other
      };

void POBs_load_sun(double x_in_m, double y_in_m, double z_in_m, double sun_diameter_in_m, double sun_mass_in_kg, int seed, int sun_number)
{
int index;

index=generate_new_POB();

POBs_data[index].x_in_m = x_in_m;
POBs_data[index].y_in_m = y_in_m;
POBs_data[index].z_in_m = z_in_m;
POBs_data[index].diameter_in_m = sun_diameter_in_m;
POBs_data[index].mass_in_kg = sun_mass_in_kg;
POBs_data[index].seed = seed;
POBs_data[index].sun_number = sun_number;

POBs_data[index].POB_type = POBs_sun;

default_flags(index);
set_POB_always_loaded(index);
set_POB_loaded(index);

POBs_data[index].galactic_id=index;

POBs_data[index].dynamic_ocb_id=load_sun(x_in_m, y_in_m, z_in_m, sun_diameter_in_m, sun_mass_in_kg, seed, sun_number, POBs_data[index].galactic_id);

no_linkage(index);
}




// +--------------------------------+-------------------------+-----------------------
// | TITLE: POBs_load_planet        | AUTHOR(s): Rob Probin   | DATE STARTED: 20 Jan 02
// +
// | DESCRIPTION: Load a planet into the persistant object module
// +----------------------------------------------------------------ROUTINE HEADER----


void POBs_load_planet(double x_in_m, double y_in_m, double z_in_m, int seed, double planet_diameter_in_m, double planet_mass_in_kg, double rotation_time_in_sec)
{
int index;
float red,green,blue;

index=generate_new_POB();

POBs_data[index].x_in_m = x_in_m;
POBs_data[index].y_in_m = y_in_m;
POBs_data[index].z_in_m = z_in_m;
POBs_data[index].diameter_in_m = planet_diameter_in_m;
POBs_data[index].mass_in_kg = planet_mass_in_kg;
POBs_data[index].rotation_time_in_sec = rotation_time_in_sec;
POBs_data[index].seed = seed;

POBs_data[index].POB_type = POBs_planet;

default_flags(index);
set_POB_always_loaded(index);
set_POB_loaded(index);

POBs_data[index].galactic_id=index;

//set blobbed colour
       get_random_colour(&red,&green,&blue);
       POBs_data[index].red=red;
       POBs_data[index].green=green;
       POBs_data[index].blue=blue;


POBs_data[index].dynamic_ocb_id=load_planet(x_in_m, y_in_m, z_in_m, seed, POBs_data[index].galactic_id, index, planet_diameter_in_m, planet_mass_in_kg, rotation_time_in_sec);
set_blobbed_object_colour(POBs_data[index].dynamic_ocb_id,red,green,blue);			

if(robs_dyn_slot_count!=3)
  {
  robs_dyn_slot=POBs_data[index].dynamic_ocb_id;
  robs_dyn_slot_count++;
  }


  
link_new_planet(index);
}



// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 20 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void POBs_load_moon(double x_in_m, double y_in_m, double z_in_m, int seed, double planet_diameter_in_m, double planet_mass_in_kg, double rotation_time_in_sec)
{
int index;
float red,green,blue;

index=generate_new_POB();

POBs_data[index].x_in_m = x_in_m;
POBs_data[index].y_in_m = y_in_m;
POBs_data[index].z_in_m = z_in_m;
POBs_data[index].diameter_in_m = planet_diameter_in_m;
POBs_data[index].mass_in_kg = planet_mass_in_kg;
POBs_data[index].rotation_time_in_sec = rotation_time_in_sec;
POBs_data[index].seed = seed;

POBs_data[index].POB_type = POBs_moon;

default_flags(index);
clr_POB_always_loaded(index);
clr_POB_loaded(index);

POBs_data[index].galactic_id=index;

//set blobbed colour
       get_random_colour(&red,&green,&blue);
       POBs_data[index].red=red;
       POBs_data[index].green=green;
       POBs_data[index].blue=blue;


link_new_moon(index);
}



// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 20 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void POBs_load_spacestation(double x_in_m, double y_in_m, double z_in_m, int seed, int allegiance)
{
int index;
float red,green,blue;

index=generate_new_POB();

POBs_data[index].x_in_m = x_in_m;
POBs_data[index].y_in_m = y_in_m;
POBs_data[index].z_in_m = z_in_m;
POBs_data[index].diameter_in_m = 200;	// random figures
POBs_data[index].mass_in_kg = 10000;	// random figures
POBs_data[index].seed = seed;
POBs_data[index].allegiance = allegiance;

POBs_data[index].POB_type = POBs_spacestation;

default_flags(index);
clr_POB_always_loaded(index);
clr_POB_loaded(index);

POBs_data[index].galactic_id=index;

//set blobbed colour
       get_random_colour(&red,&green,&blue);
       POBs_data[index].red=red;
       POBs_data[index].green=green;
       POBs_data[index].blue=blue;

link_new_mothership(index);

// if we are NOT constructing a linked list, we need to fill out the other details...
if(! constructing_linked_list) { set_up_a_mothership_supply_rating(index); }
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 12 Feb 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----
void POBs_load_player_ship(double x_in_m, double y_in_m, double z_in_m)
{
int index;

index=generate_new_POB();

POBs_data[index].x_in_m = x_in_m;
POBs_data[index].y_in_m = y_in_m;
POBs_data[index].z_in_m = z_in_m;

POBs_data[index].POB_type = POBs_player_ship;

default_flags(index);
set_POB_always_loaded(index);
set_POB_loaded(index);

POBs_data[index].galactic_id=index;

POBs_data[index].dynamic_ocb_id=load_player_ship(x_in_m, y_in_m, z_in_m, POBs_data[index].galactic_id);

no_linkage(index);
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 12 Feb 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----
void POBs_load_player_vv(double x_in_m, double y_in_m, double z_in_m)
{
int index;

index=generate_new_POB();

POBs_data[index].x_in_m = x_in_m;
POBs_data[index].y_in_m = y_in_m;
POBs_data[index].z_in_m = z_in_m;

POBs_data[index].POB_type = POBs_player_vv;

default_flags(index);
set_POB_always_loaded(index);
set_POB_loaded(index);

POBs_data[index].galactic_id=index;

POBs_data[index].dynamic_ocb_id=load_player_vv(x_in_m, y_in_m, z_in_m, POBs_data[index].galactic_id);

no_linkage(index);
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 12 Feb 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----
void POBs_load_camera(double x_in_m, double y_in_m, double z_in_m)
{
int index;

index=generate_new_POB();

POBs_data[index].x_in_m = x_in_m;
POBs_data[index].y_in_m = y_in_m;
POBs_data[index].z_in_m = z_in_m;

POBs_data[index].POB_type = POBs_player_vv;

default_flags(index);
set_POB_always_loaded(index);
set_POB_loaded(index);

POBs_data[index].galactic_id=index;

POBs_data[index].dynamic_ocb_id=load_camera(x_in_m, y_in_m, z_in_m, POBs_data[index].galactic_id);

no_linkage(index);
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----


module_private void real_load_object(int POBs_id)
{
double x_in_m, y_in_m, z_in_m, diameter_in_m, mass_in_kg, rotation_time_in_sec;
int seed, galactic_id, type;

if(POBs_id >= number_of_active_POBs || POBs_id < 0)
  {
  MyDebugStr(__LINE__,"POBs_object_management.c","Invalid POB specified");
  return;
  }
  
if(is_POB_loaded(POBs_id))				// only load if not loaded
  {
  return;
  }  


// get the data so we can actually load the object
x_in_m = POBs_data[POBs_id].x_in_m;
y_in_m = POBs_data[POBs_id].y_in_m;
z_in_m = POBs_data[POBs_id].z_in_m;
diameter_in_m = POBs_data[POBs_id].diameter_in_m;
mass_in_kg = POBs_data[POBs_id].mass_in_kg;
seed = POBs_data[POBs_id].seed;
galactic_id = POBs_data[POBs_id].galactic_id;
type = POBs_data[POBs_id].POB_type;
rotation_time_in_sec=POBs_data[POBs_id].rotation_time_in_sec;

if(type == POBs_moon)	// we only know how to load moons at the moment
  {
  POBs_data[POBs_id].dynamic_ocb_id=load_moon(x_in_m, y_in_m, z_in_m, seed, galactic_id, POBs_id, diameter_in_m, mass_in_kg, rotation_time_in_sec);

  set_blobbed_object_colour(POBs_data[POBs_id].dynamic_ocb_id,POBs_data[POBs_id].red,POBs_data[POBs_id].green,POBs_data[POBs_id].blue);			
  
  }
else if(type == POBs_spacestation)	// we only know how to load moons at the moment
  {
  int allegiance;
  
  allegiance = POBs_data[POBs_id].allegiance;
  POBs_data[POBs_id].dynamic_ocb_id=load_mothership(x_in_m, y_in_m, z_in_m, galactic_id, allegiance);
  }
else if(type == POBs_planet)
  {  
  POBs_data[POBs_id].dynamic_ocb_id=load_planet(x_in_m, y_in_m, z_in_m, seed, POBs_data[POBs_id].galactic_id, POBs_id, diameter_in_m, mass_in_kg, rotation_time_in_sec);
  
  set_blobbed_object_colour(POBs_data[POBs_id].dynamic_ocb_id,POBs_data[POBs_id].red,POBs_data[POBs_id].green,POBs_data[POBs_id].blue);			
  }
else if(type == POBs_sun)
  {
  int sun_number;
  
  sun_number = POBs_data[POBs_id].sun_number;
  
  POBs_data[POBs_id].dynamic_ocb_id=load_sun(x_in_m, y_in_m, z_in_m, diameter_in_m, mass_in_kg, seed, sun_number, POBs_data[POBs_id].galactic_id);
  }
else if(type == POBs_player_ship)
  {
  POBs_data[POBs_id].dynamic_ocb_id=load_player_ship(x_in_m, y_in_m, z_in_m, POBs_data[POBs_id].galactic_id);
  }
else if(type == POBs_player_vv)
  {
  POBs_data[POBs_id].dynamic_ocb_id=load_player_vv(x_in_m, y_in_m, z_in_m, POBs_data[POBs_id].galactic_id);
  }
else
  {
  MyDebugStr(__LINE__,"pobs_object_man.c","Cannot yet load that object");
  }

  set_POB_loaded(POBs_id);

}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

module_private void real_unload_object(int POBs_id)
{
int dynocb;
_3D object_pos;

if(POBs_id >= number_of_active_POBs)
  {
  MyDebugStr(__LINE__,"POBs_object_management.c","Invalid POB specified");
  return;
  }
  
if(! is_POB_loaded(POBs_id))				// only unload if loaded
  {
  return;
  }  
  
//if(POBs_data[POBs_id].POB_type == POBs_moon)	// we only know how to unload moons at the moment
//  {
  dynocb = POBs_data[POBs_id].dynamic_ocb_id;
  get_position_in_m(dynocb, &object_pos);
  POBs_data[POBs_id].x_in_m = object_pos.x;
  POBs_data[POBs_id].y_in_m = object_pos.y;
  POBs_data[POBs_id].z_in_m = object_pos.z;
  
  if (add_to_kill_list(dynocb)!=0)
  {
	  MyDebugStr(__LINE__,"POBs_object_management.c","add_to_kill_list failed!");
  }
  
  clr_POB_loaded(POBs_id);
//  }
//else
//  {
//  MyDebugStr(__LINE__,"pobs_object_man.c","Cannot yet load that object");
//  }

}

/*
 * NAME:        my_planet
 *
 * DESCRIPTION: This is used to force all objects around what used to be Earth. And in order
 *              to discover what object this is we store it in a static variable.
 */
int my_planet(void)		// hack!
{


#if TEST_SYSTEM == 1
  return 0;
#endif

if(robs_dyn_slot!=-123456)
  {
  return robs_dyn_slot;
  }
else
  {
  report_error("Objects without solar system: make_solar_system","\p",-123456);
  return 0;
  }

} // end of my_planet()



// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 27 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void check_galactic_id_is_POBs_index(int gal_id)
{
if(gal_id < 0) 	// galactic id's start from -2 ... -1 is generated as default outside this system
  {
  MyDebugStr(__LINE__,"POBS_object_management.c","Attempted to use non-POBs object Galactic ID for POBs function");
  }
  
if(gal_id >= number_of_active_POBs) 
  { 
  MyDebugStr(__LINE__,"POBS_object_management.c","Galactic ID referenced for non-existing POBS object");
  }
}


 

// ***************************************************************************************8
// ***************************************************************************************8
// **
// ** PLANET STRUCTURE LINKED LIST
// **

// +--------------------------------+-------------------------+-----------------------
// | TITLE: link_new_system         | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION: We maintain a linked list of planets and moons to allow the game to
// | find it's way around the system. 
// +----------------------------------------------------------------ROUTINE HEADER----

int last_moon_slot_link_list_constructor = -1;
int last_planet_slot_link_list_constructor = -1;
int last_submoon_mothership_slot_link_list_constructor = -1;


module_private void link_new_system(void)
{
// we are linking a new system
constructing_linked_list=1;

// set up constructors to "no object"
last_moon_slot_link_list_constructor=-1;
last_planet_slot_link_list_constructor=-1;
last_submoon_mothership_slot_link_list_constructor=-1;
}


// +----------------------------------+-------------------------+-----------------------
// | TITLE: finish_linking_new_system | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

module_private void finish_linking_new_system(void)
{
constructing_linked_list=0;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: link_new_planet         | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----


module_private void link_new_planet(int gal_id)
{
if(! constructing_linked_list) {  MyDebugStr(__LINE__,"POBs_object_man.c","Illegal planet link - no insert coded"); return; }

if(last_planet_slot_link_list_constructor != -1)
  {
  // fill out "next slot" in last planet (if there is one)
  POBs_data[last_planet_slot_link_list_constructor].next_linked_object = gal_id;
  }
  
POBs_data[gal_id].previous_linked_object = last_planet_slot_link_list_constructor;	// fill out "last slot" in current planet
POBs_data[gal_id].orbit_parent = -1;		// it doesn't orbit a single object (binary suns...)
POBs_data[gal_id].first_orbiting_child = -1;	// patch in temporary no-object for next moon
POBs_data[gal_id].next_linked_object = -1;	// patch in temporary no-object for next planet

last_planet_slot_link_list_constructor = gal_id;
last_moon_slot_link_list_constructor=-1;			// new planets means new chain for moons
last_submoon_mothership_slot_link_list_constructor=-1;		// and new chain for moon orbiting motherships
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: link_new_moon           | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

module_private void link_new_moon(int gal_id)
{
if(! constructing_linked_list) {  MyDebugStr(__LINE__,"POBs_object_man.c","Illegal moon link - no insert coded"); return; }

if(last_moon_slot_link_list_constructor != -1)
  {
  POBs_data[last_moon_slot_link_list_constructor].next_linked_object = gal_id;  	// fill out "next slot" in last moon (if there is one)
  }
else // first child
  {
  POBs_data[last_planet_slot_link_list_constructor].first_orbiting_child = gal_id;	// we are first child of planet
  }
  
POBs_data[gal_id].previous_linked_object = last_moon_slot_link_list_constructor;	// fill out "last slot" in current planet
POBs_data[gal_id].orbit_parent = last_planet_slot_link_list_constructor;
POBs_data[gal_id].first_orbiting_child = -1;	// patch in temporary no-object for next moon
POBs_data[gal_id].next_linked_object = -1;	// patch in temporary no-object for next planet

last_moon_slot_link_list_constructor = gal_id;
last_submoon_mothership_slot_link_list_constructor=-1;		// new moons means new chain for moon orbiting motherships
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: link_new_mothership     | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

module_private void link_new_mothership(int gal_id)
{
if(! constructing_linked_list) {  link_inserted_object(gal_id, get_galactic_id(my_planet())); return; }

// what are we actually orbiting?
if(last_moon_slot_link_list_constructor != -1)	// orbiting a moon
  {
  
  if(last_submoon_mothership_slot_link_list_constructor != -1)
    {
    POBs_data[last_submoon_mothership_slot_link_list_constructor].next_linked_object = gal_id;    // fill out "next slot" in last moon (if there is one)
    }
  else // first child
    {
    POBs_data[last_moon_slot_link_list_constructor].first_orbiting_child = gal_id;	// fill in first child slot
    }

  POBs_data[gal_id].previous_linked_object = last_submoon_mothership_slot_link_list_constructor;
  POBs_data[gal_id].orbit_parent = last_moon_slot_link_list_constructor;
  POBs_data[gal_id].first_orbiting_child = -1;	// nothing orbits spacestations/motherships - they are too small...    
  POBs_data[gal_id].next_linked_object = -1;	// patch in temporary no-object for next planet
  
  last_submoon_mothership_slot_link_list_constructor = gal_id;
  }
else		// orbiting a planet
  {
  link_new_moon(gal_id);		// spacestations orbiting planets are treated as moons for this purposes here
  }
    
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: no_linkage              | AUTHOR(s): Rob Probin   | DATE STARTED: 23 Mar 02
// +
// | DESCRIPTION:  For objects that do not have any linkage (currently Suns, Zex, etc)
// +----------------------------------------------------------------ROUTINE HEADER----

module_private void no_linkage(int gal_id)
{
  POBs_data[gal_id].previous_linked_object = -1;
  POBs_data[gal_id].orbit_parent = -1;
  POBs_data[gal_id].first_orbiting_child = -1;
  POBs_data[gal_id].next_linked_object = -1;
}

// +----------------------------------+-------------------------+-----------------------
// | TITLE: link_inserted_object      | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION: Used for inserting an orbiting object outside the normal solar system 
// | load order, and correctly patching it in.
// +----------------------------------------------------------------ROUTINE HEADER----

void link_inserted_object(int gal_id, int gal_id_it_orbits)
{
int current_last_object;

if(POBs_data[gal_id_it_orbits].first_orbiting_child != -1)
  {
  current_last_object = find_last_in_this_link_level(POBs_data[gal_id_it_orbits].first_orbiting_child);
  POBs_data[current_last_object].next_linked_object = gal_id;
  }
else // this object has no orbiting object children ... yet
  {
  POBs_data[gal_id_it_orbits].first_orbiting_child = gal_id;
  current_last_object = -1;
  }

POBs_data[gal_id].previous_linked_object = current_last_object;  
POBs_data[gal_id].orbit_parent = gal_id_it_orbits;
POBs_data[gal_id].first_orbiting_child = -1;	// nothing orbits spacestations/motherships - they are too small...    
POBs_data[gal_id].next_linked_object = -1;	// patch no-object for next planet  



}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: unlink_object           | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION: Used when deleting an orbiting object - to stop missing links 
// +----------------------------------------------------------------ROUTINE HEADER----

#if 0	// not tested
void unlink_object(int gal_id)
{
int object_it_orbits;
int next_object;
int last_object;

object_it_orbits=POBs_data[gal_id].orbit_parent;
next_object=POBs_data[gal_id].next_linked_object;
last_object=POBs_data[gal_id].previous_linked_object;

if(POBs_data[object_it_orbits].first_orbiting_child==gal_id)
  { // we are in parent first child entry
  if(last_object != -1)
    { // must be first in list if parent's first child
    MyDebugStr(__LINE__,"POBs_object_man.c","Illegal linkage in POBs");
    }
  POBs_data[object_it_orbits].first_orbiting_child = next_object; // patch parent
  }

// patch along the string
  if(last_object != -1)
    {
    POBs_data[last_object].next_linked_object = next_object;        // now patch next linked object
    }
    
  if(next_object != -1)
    {
    POBs_data[next_object].previous_linked_object = last_object;    // now patch next linked object
    }

}

#endif



// +-------------------------------------+-------------------------+-----------------------
// | TITLE: find_last_in_this_link_level | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION: Finds the last object in the POBs linked object tree on this level.
// |              gal_id passed in must be of a valid object.
// +----------------------------------------------------------------ROUTINE HEADER----

int find_last_in_this_link_level(int gal_id)
{

while(POBs_data[gal_id].next_linked_object != -1) 
  {
  gal_id = POBs_data[gal_id].next_linked_object;	// keep going until last one
  }

return gal_id;
}


// **
// ** End of planet structure linked list
// **
// ***************************************************************************************8
// ***************************************************************************************8



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~   
//   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   
//    ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   
// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   
//  ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   
//   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   
//    ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   
//
//
// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   
//  ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   
//   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   
//    ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   
// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   
//  ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~   
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



#define DO_POBS_ERROR_CHECKING 1


// +----------------------------------------+-------------------------+-----------------------
// | TITLE: POBs_get_number_of_ships_docked | AUTHOR(s): Rob Probin   | DATE STARTED: 17 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_get_number_of_ships_docked(int allegiance, int galactic_id)
{
#if DO_POBS_ERROR_CHECKING
if(galactic_id >= number_of_active_POBs || galactic_id < 0)
  {
  MyDebugStr(__LINE__,"POBs_game_data_accessors.c","Invalid POB specified");
  return 0;
  }

if(POBs_data[galactic_id].POB_type != POBs_spacestation)
  {
  MyDebugStr(__LINE__,"POBS_game_data_accessors.c","Can only get number of ships docked on a mothership");
  return 0;
  }
#endif

if(allegiance == HUMAN)
  {
  return POBs_data[galactic_id].human_ships_docked;
  }
else if(allegiance == -1)
  {
  return POBs_data[galactic_id].human_ships_docked + POBs_data[galactic_id].alien_ships_docked + POBs_data[galactic_id].neutral_ships_docked;
  }
else 
  {
  return POBs_data[galactic_id].alien_ships_docked;
  }

}




// +---------------------------------------+-------------------------+-----------------------
// | TITLE: POBs_get_allegiance            | AUTHOR(s): Rob Probin   | DATE STARTED: 17 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_get_allegiance(int galactic_id) //returns HUMAN, ALIEN, NEUTRAL
{
#if DO_POBS_ERROR_CHECKING

if(galactic_id >= number_of_active_POBs || galactic_id < 0)
  {
  MyDebugStr(__LINE__,"POBs_game_data_accessors.c","Invalid POB specified");
  return 0;
  }

if(POBs_data[galactic_id].POB_type != POBs_spacestation)
  {
  MyDebugStr(__LINE__,"POBS_game_data_accessors.c","We currently only store allegiance for motherships - needs modification?");
  return 0;
  }
#endif

return POBs_data[galactic_id].allegiance;
}

// +------------------------------------------+-------------------------+-----------------------
// | TITLE: POBs_supply_rating                | AUTHOR(s): Rob Probin   | DATE STARTED: 17 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_get_supply_rating(int galactic_id) //0-100
{
#if DO_POBS_ERROR_CHECKING

if(galactic_id >= number_of_active_POBs || galactic_id < 0)
  {
  MyDebugStr(__LINE__,"POBs_game_data_accessors.c","Invalid POB specified");
  return 0;
  }
#endif

return POBs_data[galactic_id].supply_rating;
}


// +----------------------------------------+-------------------------+-----------------------
// | TITLE: POBs_supply_type                | AUTHOR(s): Rob Probin   | DATE STARTED: 17 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_get_supply_type(int galactic_id) //1-5=agri, 6-10=tech
{
#if DO_POBS_ERROR_CHECKING

if(galactic_id >= number_of_active_POBs || galactic_id < 0)
  {
  MyDebugStr(__LINE__,"POBs_game_data_accessors.c","Invalid POB specified");
  return 0;
  }
#endif

return POBs_data[galactic_id].supply_type;
}


// +----------------------------------------+-------------------------+-----------------------
// | TITLE: tell_POBs_ship_launched         | AUTHOR(s): Rob Probin   | DATE STARTED: 17 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void tell_POBs_ship_launched(int allegiance, int galactic_id)
{
#if DO_POBS_ERROR_CHECKING
if(galactic_id >= number_of_active_POBs || galactic_id < 0)
  {
  MyDebugStr(__LINE__,"POBs_game_data_accessors.c","Invalid POB specified");
  return;
  }

if(POBs_data[galactic_id].POB_type != POBs_spacestation)
  {
  MyDebugStr(__LINE__,"POBS_game_data_accessors.c","Can only launch from a mothership?");
  return;
  }
#endif

if(allegiance==HUMAN)
  {
  POBs_data[galactic_id].human_ships_docked--;
  }
else 
  {
  POBs_data[galactic_id].alien_ships_docked--;
  }

}



// +----------------------------------------+-------------------------+-----------------------
// | TITLE: tell_POBs_ship_docked         | AUTHOR(s): Rob Probin   | DATE STARTED: 17 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void tell_POBs_ship_docked(int allegiance, int galactic_id)
{
#if DO_POBS_ERROR_CHECKING
if(galactic_id >= number_of_active_POBs || galactic_id < 0)
  {
  MyDebugStr(__LINE__,"POBs_game_data_accessors.c","Invalid POB specified");
  return;
  }

if(POBs_data[galactic_id].POB_type != POBs_spacestation)
  {
  MyDebugStr(__LINE__,"POBS_game_data_accessors.c","Can only dock in a mothership?");
  return;
  }
#endif

if(allegiance==HUMAN)
  {
  POBs_data[galactic_id].human_ships_docked++;
  }
  else
  {
  POBs_data[galactic_id].alien_ships_docked++;
  }

}



// +--------------------------------------------+-------------------------+-----------------------
// | TITLE: construct_mothership_supply_ratings | AUTHOR(s): Rob Probin   | DATE STARTED: 17 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void construct_mothership_supply_ratings(void)
{
int gal_id;
  
for(gal_id=0; gal_id < number_of_active_POBs; gal_id++)
  {
  if(POBs_data[gal_id].POB_type == POBs_planet || POBs_data[gal_id].POB_type == POBs_moon)
    {
    POBs_data[gal_id].population = calculate_a_planet_moon_population(gal_id);
    POBs_data[gal_id].supply_type = planet_moon_supply_type_calculation(gal_id);

    // dependant up planet/moon population
    POBs_data[gal_id].supply_rating = planet_moon_supply_rating_calculation(gal_id);
    }
  }
 
   
for(gal_id=0; gal_id < number_of_active_POBs; gal_id++)
  {
  if(POBs_data[gal_id].POB_type == POBs_spacestation)
    {
    set_up_a_mothership_supply_rating(gal_id);
    }
  }
   
     
}


// +--------------------------------------------+-------------------------+-----------------------
// | TITLE: set_up_a_mothership_supply_rating   | AUTHOR(s): Rob Probin   | DATE STARTED: 23 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void set_up_a_mothership_supply_rating(int gal_id)
{
    // these are based on planet stats
    POBs_data[gal_id].supply_rating =  mothership_supply_rating_calculation(gal_id);
    POBs_data[gal_id].supply_type = mothership_supply_type_calculation(gal_id);
    
    // these are based on supply rating of the mothership
    POBs_data[gal_id].population = calculate_mothership_population(gal_id);
    setup_docked_ships_count(gal_id);
}


// +---------------------------------+-------------------------+-----------------------
// | TITLE: setup_docked_ships_count | AUTHOR(s): Rob Probin   | DATE STARTED: 17 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void setup_docked_ships_count(int galactic_id)
{
// neutral motherships are biased towards - they get more general traders

if(POBs_data[galactic_id].allegiance == HUMAN)
  {
  POBs_data[galactic_id].human_ships_docked = RangedRdm(0,20+POBs_data[galactic_id].supply_rating);
  POBs_data[galactic_id].alien_ships_docked = 0;
  POBs_data[galactic_id].neutral_ships_docked = RangedRdm(0,10+POBs_data[galactic_id].supply_rating);
  }
else
  {
  POBs_data[galactic_id].human_ships_docked = 0;
  POBs_data[galactic_id].alien_ships_docked = RangedRdm(0,20+POBs_data[galactic_id].supply_rating);
  POBs_data[galactic_id].neutral_ships_docked = RangedRdm(0,10+POBs_data[galactic_id].supply_rating);
  }
  
}
  


// +--------------------------------+-------------------------+-----------------------
// | TITLE: POBs_is_inhabited       | AUTHOR(s): Rob Probin   | DATE STARTED: 17 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_is_inhabited(int gal_id)
{
return POBs_data[gal_id].population != 0;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: POBs_what_is_population | AUTHOR(s): Rob Probin   | DATE STARTED: 17 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

double POBs_what_is_population(int gal_id)
{
return POBs_data[gal_id].population;
}


// +---------------------------------+-------------------------+-----------------------
// | TITLE: update_supply_ratings    | AUTHOR(s): Rob Probin   | DATE STARTED: 17 Mar 02
// +
// | DESCRIPTION:  When a planet, moon or mothership is added or removed the ratings need
// |               updating.
// +----------------------------------------------------------------ROUTINE HEADER----

#if 0 // not tested
void update_supply_ratings(void)
{

}
#endif



// +--------------------------------+-------------------------+-----------------------
// | TITLE: POBs_is_this_a_moon     | AUTHOR(s): Rob Probin   | DATE STARTED: 27 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_is_this_a_moon(int gal_id)
{
return POBs_data[gal_id].POB_type==POBs_moon;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: POBs_is_this_a_planet   | AUTHOR(s): Rob Probin   | DATE STARTED: 23 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_is_this_a_planet(int gal_id)
{
return POBs_data[gal_id].POB_type==POBs_planet;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: POBs_is_this_a_moon     | AUTHOR(s): Rob Probin   | DATE STARTED: 27 Jan 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_is_this_a_planet_or_moon(int gal_id)
{
return (POBs_data[gal_id].POB_type==POBs_moon) || (POBs_data[gal_id].POB_type==POBs_planet);
}


// +----------------------------------+-------------------------+-----------------------
// | TITLE: POBs_is_this_a_mothership | AUTHOR(s): Rob Probin   | DATE STARTED: 23 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_is_this_a_mothership(int gal_id)
{
return POBs_data[gal_id].POB_type==POBs_spacestation;
}


// +----------------------------------+-------------------------+-----------------------
// | TITLE: get_POBs_diameter         | AUTHOR(s): Rob Probin   | DATE STARTED: 23 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

double get_POBs_diameter(int gal_id)
{
return POBs_data[gal_id].diameter_in_m;
}

// +----------------------------------+-------------------------+-----------------------
// | TITLE: get_POBs_rotation         | AUTHOR(s): Rob Probin   | DATE STARTED: 19 Apr 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

double get_POBs_rotation(int gal_id)
{
return POBs_data[gal_id].rotation_time_in_sec;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 3 Feb 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

char POBs_get_planet_or_moon_colour(int dyn_slot)
{
int gal_id;

gal_id=get_galactic_id(dyn_slot);
check_galactic_id_is_POBs_index(gal_id);

return convert_to_palette(STATIC_CAST_TO_INT(POBs_data[gal_id].red*255),STATIC_CAST_TO_INT(POBs_data[gal_id].green*255),STATIC_CAST_TO_INT(POBs_data[gal_id].blue*255));
}




// +---------------------------------+-------------------------+-----------------------
// | TITLE: POBs_is_this_a_rock_moon | AUTHOR(s): Rob Probin   | DATE STARTED: 13 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_is_this_a_rock_moon(int gal_id)
{
return (POBs_data[gal_id].diameter_in_m < 5e5)         // 500Km
         && ((POBs_data[gal_id].POB_type == POBs_moon) || (POBs_data[gal_id].POB_type == POBs_planet));	
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 13 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_is_this_a_gas_giant(int gal_id)	// neptune is 49500km
{
return (POBs_data[gal_id].diameter_in_m > 30e6) && (POBs_data[gal_id].POB_type != POBs_sun);	// 30,000Km
}


// +------------------------------------+-------------------------+-----------------------
// | TITLE: POBs_is_this_a_giant_planet | AUTHOR(s): Rob Probin   | DATE STARTED: 23 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_is_this_a_giant_planet(int gal_id)	// neptune is 49500km
{
return (POBs_data[gal_id].diameter_in_m >= 15e6) && (POBs_data[gal_id].POB_type != POBs_sun);	// 30,000Km
}


// +----------------------------------------------+-------------------------+-----------------------
// | TITLE: POBs_is_this_a_hypermassive_gas_giant | AUTHOR(s): Rob Probin   | DATE STARTED: 23 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_is_this_a_hypermassive_gas_giant(int gal_id)	// neptune is 49500km
{
return (POBs_data[gal_id].diameter_in_m >= 80e6) && (POBs_data[gal_id].POB_type != POBs_sun);	// 30,000Km
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 13 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_is_this_a_sun(int gal_id)
{
return POBs_data[gal_id].POB_type == POBs_sun;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 13 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_is_this_earthish_size(int gal_id)
{
return (POBs_data[gal_id].diameter_in_m > 6e6 &&	// 6000Km
        POBs_data[gal_id].diameter_in_m < 15e6);	// 15000Km
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: POBs_object_parent      | AUTHOR(s): Rob Probin   | DATE STARTED: 23 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_object_parent(int gal_id)
{
return POBs_data[gal_id].orbit_parent;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: POBs_object_first_child | AUTHOR(s): Rob Probin   | DATE STARTED: 23 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_object_first_child(int gal_id)
{
return POBs_data[gal_id].first_orbiting_child;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: POBs_object_next        | AUTHOR(s): Rob Probin   | DATE STARTED: 23 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_object_next(int gal_id)
{
return POBs_data[gal_id].next_linked_object;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: POBs_object_previous    | AUTHOR(s): Rob Probin   | DATE STARTED: 23 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int POBs_object_previous(int gal_id)
{
return POBs_data[gal_id].previous_linked_object;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 15 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

double POBs_calc_distance_from_suns(int gal_id)
{
double dist;

dist=POBs_data[gal_id].x_in_m*POBs_data[gal_id].x_in_m;
dist+=POBs_data[gal_id].y_in_m*POBs_data[gal_id].y_in_m;
dist+=POBs_data[gal_id].z_in_m*POBs_data[gal_id].z_in_m;
dist=sqrt(dist);


// assume suns are at 0,0,0
return dist;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 13 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----
#define AU 150e9		// 150 million km

double calculate_a_planet_moon_population(int gal_id)
{
double to_sun;
double size, population;

size=RangedRdm(1,10) * pow(10,RangedRdm(3,10));

if(POBs_is_this_a_gas_giant(gal_id)) { size/=1000; }
if(POBs_is_this_a_giant_planet(gal_id)) { size/=1000; }	// also covers gas giants...
if(POBs_is_this_a_rock_moon(gal_id)) { size/=1000000; }

if(POBs_is_this_earthish_size(gal_id)) { size*=1000; }

// lets check distance from sun
to_sun = POBs_calc_distance_from_suns(gal_id) - (0.15*AU);	// adjust for binary suns manually
if(to_sun < (0.3*AU))  
    { /* close to sun */ size/=1000; }
else if(to_sun < (1.3*AU))  
    { /* sweet spot */ size*=1000; }
else if(to_sun > (1.8*AU))
    { /* further away is bad */
    to_sun -= 1.8 * AU;
    to_sun /= 0.3 * AU;
    to_sun *= 1.5;
    size /= to_sun;
    }

modf(size, &population);	// get integral (no half people)

if(population < 0) population=0;
return population;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: find_planet_of_group    | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION: Pass in gal_id of planet, moon or mothership and returns planet gal_id
// |
// | Relies on the fact that planets have no parent.
// +----------------------------------------------------------------ROUTINE HEADER----

int find_planet_of_group(int gal_id)
{
while(POBs_data[gal_id].orbit_parent != -1)
  {
  gal_id=POBs_data[gal_id].orbit_parent;
  }
  
return gal_id;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION: Pass in gal_id of planet or moon, and get number of moons. 
// +----------------------------------------------------------------ROUTINE HEADER----

int count_moons_around_planet(int gal_id)
{
int count;

gal_id = find_planet_of_group(gal_id);

gal_id = POBs_data[gal_id].first_orbiting_child;

count=0;
while(gal_id != -1)
  {
  if(POBs_data[gal_id].POB_type == POBs_moon) /* not spacestation */ { count++; }
  gal_id = POBs_data[gal_id].next_linked_object;
  }
  
return count;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION: gal_id can be planet or moon. 
// +----------------------------------------------------------------ROUTINE HEADER----

int count_local_earth_sized_objects_around_planet(int gal_id)
{
int count;

gal_id = find_planet_of_group(gal_id);
if(POBs_is_this_earthish_size(gal_id)) { count++; }

gal_id = POBs_data[gal_id].first_orbiting_child;

count=0;
while(gal_id != -1)
  {
  if(POBs_data[gal_id].POB_type == POBs_moon) /* not spacestation */ 
    {
    if(POBs_is_this_earthish_size(gal_id))
      {
      count++; 
      }
    }
  gal_id = POBs_data[gal_id].next_linked_object;
  }
  
return count;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int mothership_supply_rating_calculation(int gal_id)
{
int rating;
int parent_gal_id;

parent_gal_id = POBs_data[gal_id].orbit_parent;


#if 1	// assuming all based on orbiting planet

// 100% of orbiting parent
rating = POBs_data[parent_gal_id].supply_rating;

#else	// split it up between various moons.

// 50% of orbiting parent
rating = POBs_data[parent_gal_id].supply_rating / 2 ;

if(POBs_data[parent_gal_id].POB_type == POBs_planet)
  {
  }
else	// must be a moon
  {
  // 25% of nearest out - or 0 if none
  if(POBs_data[parent_gal_id].next_linked_object != -1)
    {
    rating += POBs_data[POBs_data[parent_gal_id].next_linked_object].supply_rating / 4;
    }

  // 25% of nearest in - or planet if non
  if(POBs_data[parent_gal_id].next_linked_object == -1)
    {
    
    }
  }

#endif

if(rating < 0) rating=0;
if(rating > 100) rating=100;

return rating;
}



// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int mothership_supply_type_calculation(int gal_id)
{
return POBs_data[POBs_data[gal_id].orbit_parent].supply_type;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

int planet_moon_supply_type_calculation(int gal_id)
{
return RangedRdm(1,11);
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----


int planet_moon_supply_rating_calculation(int gal_id)
{
int rating;
double to_sun;

rating = RangedRdm(0,30);		// between 0 and 29

if(POBs_is_this_a_rock_moon(gal_id)) { rating -= 20; }
if(POBs_is_this_a_gas_giant(gal_id)) { rating -= 20; }

rating+=count_moons_around_planet(gal_id);
rating+=count_local_earth_sized_objects_around_planet(gal_id);

// lets check distance from sun
to_sun=POBs_calc_distance_from_suns(gal_id) - (0.15*AU);	// adjust for binary suns manually
if(to_sun < (0.3*AU))  
    { /* close to sun */ rating-=4; }
else if(to_sun < (1.3*AU))  
    { /* sweet spot */ rating+=10; }
else if(to_sun > (1.8*AU))
    { /* further away is bad */
    to_sun -= 1.8 * AU;
    to_sun /= 0.3 * AU;
    rating -= STATIC_CAST_TO_INT(to_sun);
    }

rating += STATIC_CAST_TO_INT(2 * log10(POBs_data[gal_id].population ));
rating += STATIC_CAST_TO_INT(2 * log10(POBs_data[gal_id].population * 2));

if(rating < 0) rating=0;
if(rating > 100) rating=100;

return rating;
}




// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

double calculate_mothership_population(int gal_id)
{
int population;

population=RangedRdm(40,500);
population += POBs_data[gal_id].supply_rating;	// motherships with higher supply ratings have higher populations


if(population <= 0) population=1;
return population;
}



// ***********************************************************************************
// * DESIGN NOTES
// *

// THINGS THAT NEED INVESTIGATION:-
//   use of galactic id in load planet (source and destination use)
//   use of inhabited in load planet  (source & destination use)


//
// To do list & times
//
// structure setup [1 hour]   SUN
// automatic loading of objects [4 hours] SUN
// initial blocking [2 hours] SUN/MON
// automatic unloading of objects [4 hours] MON/TUE
// change solar setup over to pobs [2 hours] TUE/WED
// loading/unloading testing [1 hour] WED

// scene reload (e.g. from docking) [3 hours] FRI
// scene flush (for going into a static scene) [1 hour] SAT
// scene init (for starting the game) [1 hour] SAT
//
// low LOD object controller [2 hours] SAT
// buy.c accessors [3 hours] SAT/SUN
// galactic id generator [1 hour] SUN (+ve is index to pobs object, -ve is non-pobs object)


// ==========
// POBs files
// ==========
// (POBs = Persistant Object control Blocks)
//
// General notes
// =============
//
// Keeps game context of any object which needs to be 
// persistant.
//
// POBs is the only system which loads large objects
//
// Some smaller system load objects: e.g. missiles, asteriods, 
// etc.
//
// "object management"
// ==================
//
// handles the loading and unloading objects from the 
// dynamic OCBs (object control blocks)
//
// keeps tabs of camera position (camera object more precisly)
// and loads and unloads objects around that. So that it does
// not need to use loads of squareroots, all game objects
// are put into a sector (aka miniblock) and the 9 including 
// and surrounding the player are loaded. 
// 
// objects are killed (from the dynamic ocbs at least) when 
// they stray too far away (3 or 5 complete sectors are killed
// at a time). However object from new blocks are loaded one
// per frame to reduce load on the system.
//
// When an object is loaded, the POB's object manager takes
// the model from the static OCBs and loads it into the dynamic
// OCBs. Apart from causing the display engine to show the object
// this also causes the engine to run the object controller to 
// run the object.
//
// All live objects in the game have a POB entry.
//
// If the object is not "live" in the dynamic ocbs then a 
// low LOD (level of detail) controller is called. This allows 
// an object to maintain a crude amount of control whilst out
// of view of the player.
//
//
// key game data accessors
// =======================
//
// handles the accessing of data about an object via
// either an galactic id OR some other undetermined method.
//
//
// Supplies info to to "buy.c" to provide:
//			 supply_rating 0-100 inc. (how close), 
//         tech rating 1-9. 
//
//		 also names of planets, and people type
//
//		 seeds for generation
//
//
// see buy.c design doc by stu
//
//
// game service data
// ---------------
//
// Dishes out galactic id's. A galactic id is a unique number
// for every object instance, whether it is a "real" object or
// more simply a temporary object. A galactic id of zero
// is a special number - something like no POBs entry - used
// for objects that have no persistant game entry (e.g. an
// asteroid).
// 
//

