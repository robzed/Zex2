// *************************************MODULE HEADER*********************************
// * FILENAME           : solar_system_objects.cpp
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * DATE STARTED       : 09:18:00 on 23rd Septemeber 2001
// * FIRST AUTHOR       : Rob Probin
// *
// * COPYRIGHT (c)        2001 Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************

#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <QuickDraw.h>
    #include <memory.h>
    #include <timer.h>
    #include <textutils.h>
  #endif

  #include "options.h"
  #include "3d_structures_def.h"
  #include	"zex_misc.h"
  #include "Error_handler.h"
  #include "game_defs.h"
  #include "access_ocbs.h"
#endif  //pch

#include "Galaxy.h"
#include "objects_controls.h"
//#include "monster_control_misc.ext"
#include "file_io.h"
#include "pictures.h"
#include	"splat_control.h"
#include	"txtsplat.h"

#include "ship_control.h"
#include "game_pause.h"
#include "object_control_top.h"
#include "camera_control.h"
#include "dlp.h"
#include "inventory.h"
#include    "fpoly4.h"
#include "genpoly.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "planet_generator.h"
#include "instrum.h"
#include "obj_control_misc_routines.h"
#include "load_objects.h"
#include "Zsound.h"
#include "physics.h"
#include "light_control.h"
#include "texture_management.h"
#include "physics_gravity.h"
#include "objects_misc.h"
#include "olg_text.h"
#include "ogl.h"
#include "static_object_support.h"
#include "planet_generator.h"

#include "POBS.h"
#include "object_manipulation.h"

// ***********************************************************************************
// * CONSTANTS 
// *


// ***********************************************************************************
// * TYPE DEFINITIONS
// *


// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

extern	DynObjectsFixedSize *ocb_ptr;
extern StatObjectsFixedSize *ocb_Sptr;

int sun_slot1;	//we can cope with up to 2 suns
int sun_slot2;	//these are used for lens flares

extern int OGL_active;


// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *


// ***********************************************************************************
// * ACTUAL CODE
// *



//load a solar system into OCB's
void set_up_solar_system()
{
double min_dist;

//#define ZEX_SEED 0x69006931
//#define ZEX_SEED 0x69326731 //sun behind planet
#define ZEX_SEED 0x69117731 //quite nice
//#define ZEX_SEED 0x66117731 //quite nice
//#define ZEX_SEED 0x68325741
//#define ZEX_SEED 0x39e6f791


#if ZEX_CORE_CARBON==0
qd.randSeed=ZEX_SEED;
#else
Zsrand(ZEX_SEED);
#endif

//        init_zex_random(); this is done in "init_name_generator"
        init_name_generator();

	min_dist=load_suns();
	my_make_planets(min_dist); 

        inform_POBs_we_have_finished_constructing_solar_system();
}


int load_sun(double x_in_m, double y_in_m, double z_in_m, double sun_diameter_in_m, double sun_mass_in_kg, int seed, int sun_number, short galactic_id)
{
load_dyn_object_pb the_params;
int static_slot,dynamic_slot;

double scale_factor;
register ZObject * current_object_ptr;

//load sun(s)
//a sun is a bloody, nooo, a very, very, big disk that always faces the player
//the disk object STR&
	static_slot=get_static_object_slot('SUN&');	//go fetch fido (the object)
	if (static_slot==-1) report_error("Cant find static sun slot","\p",-1);
	clear_pb(&the_params);
        dynamic_slot=find_vacant_dynamic_object_slot();
	if (dynamic_slot==-1) report_error("Cant find dynamic slot for sun in set_up_solar_system","\p",-1);
                
	the_params.world_x=x_in_m/ZEX_UNIT_SIZE_IN_METERS;
	the_params.world_y=y_in_m/ZEX_UNIT_SIZE_IN_METERS;
	the_params.world_z=z_in_m/ZEX_UNIT_SIZE_IN_METERS;
//set up light position as per the sun
	
//	set_light_position(the_params.world_x,the_params.world_y,the_params.world_z);

	if (sun_number==1) 
	{
	set_light_position(0,the_params.world_x,the_params.world_y,the_params.world_z);
	set_light_default_brightness(0);
	}
	else 
	{
	set_light_position(1,the_params.world_x,the_params.world_y,the_params.world_z);
	set_light_default_brightness(1);
	}
	the_params.controller_ref=SUN;
	the_params.shield_value=1000;
	the_params.mass=sun_mass_in_kg/1000;
	the_params.object_category=MASSIVE_SCENARY;
     
	scale_factor=(sun_diameter_in_m/ZEX_UNIT_SIZE_IN_METERS)/ (ocb_Sptr->object_list[static_slot].radius);	// static radius in zex units
	
	load_dyn_object(static_slot,dynamic_slot,&the_params,galactic_id,scale_factor,-1,!DUST_BIT);	//the object, the position (-1=next free)
											// and no standard orbit
	current_object_ptr=&ocb_ptr->object_list[dynamic_slot];
         gen_planet_name(seed, current_object_ptr->Dyn_OCB_control_data.name_str,0);

	if(sun_number==1)
	  {
	  sun_slot1=dynamic_slot;
	  sun_slot2=-1;
	  }
	else
	  {
	sun_slot2=dynamic_slot;
	  }	

return dynamic_slot;		
}



int load_planet(double x_in_m, double y_in_m, double z_in_m, int seed, short galactic_id, int POBs_id, double planet_diameter_in_m, double planet_mass_in_kg, double rotation_time_in_secs)
{
int static_slot,dyn_slot;

register ZObject * current_object_ptr;
load_dyn_object_pb the_params;
dynamic_texture * my_tex_ptr;
_ZPoly *current_poly;
int number_of_polys_in_object,i,err;
float scale_factor;

            Ptr scratch_mem_ptr;

//calc section rect
          my_tex_ptr=get_memory_for_dyn_tex();
          if (my_tex_ptr==0)   
          
          {
          add_to_text_display("Zex Exec> WARNING: get_memory_for_dyn_tex failed in load_planet",DLP_WHITE);
          return 0;
          }
//we use PNTZ, a dummy rectangle as our planet.
//we poke the address of our gen on the fly texture into all the polys
//[lanets live between 30 and 120 thousand Z
          scratch_mem_ptr=NewPtr((256*256)+(128*128)+300);  // (1024*1024+300);
           if (scratch_mem_ptr==0)
          {
          add_to_text_display("Zex Exec> WARNING: load_planet couldn't get scratch memory. Graphic not loaded",DLP_WHITE);
          return 0;
          }

        static_slot=get_static_object_slot('PNT3');	// the center object
        if (static_slot==-1) report_error ("map_control:load_planet couldn't find PNT3","\p",-5);
        
        dyn_slot=find_vacant_dynamic_object_slot();
        if (dyn_slot==-1) report_error ("map_control:load_planet couldn't find empty slot","\p",-6);


          clear_pb(&the_params);
          the_params.control_use_1=seed;
          
	  the_params.world_x=x_in_m/ZEX_UNIT_SIZE_IN_METERS;
	  the_params.world_y=y_in_m/ZEX_UNIT_SIZE_IN_METERS;
	  the_params.world_z=z_in_m/ZEX_UNIT_SIZE_IN_METERS;		//RangedRdm(2000,6000);
	
//this rotated the planet	the_params.delta_rot_y=1;


        the_params.delta_rot_z=planet_mass_in_kg * UNIT_RADIUS * 3.142 / rotation_time_in_secs;

	the_params.mass = planet_mass_in_kg/1000;		// make into metric tonnes for ocb

	  the_params.object_category=MASSIVE_SCENARY;
	  the_params.controller_ref=PLANET_CONTROL;
		
	  the_params.no_trivial_rejection=1;	//controlled when not in view


	scale_factor=(planet_diameter_in_m/ZEX_UNIT_SIZE_IN_METERS)/ (2*(ocb_Sptr->object_list[static_slot].radius));	// static radius in zex units

	load_dyn_object(static_slot,dyn_slot,&the_params,galactic_id,scale_factor,-1,!DUST_BIT);	//the object, the position (-1=next free)
										// parent_slot=-1 ... planets come under a different system

        set_rotation_around_world_x(dyn_slot, 270);

        
	if(sun_slot2==-1)	// only one sun in this universe
		{
          	  set_stable_orbit_vector(dyn_slot,sun_slot1);
		}
	else
		{
		// need to orbit around double mass
		DOUBLE_set_stable_orbit_vector(dyn_slot,sun_slot1,sun_slot2);
		}

	current_object_ptr=&ocb_ptr->object_list[dyn_slot];
//set up texture
//get mem ofr otexture
          current_object_ptr->dynamic_tex_ptr=my_tex_ptr;
          
          gen_planet_name(seed, current_object_ptr->Dyn_OCB_control_data.name_str,1);

          if (current_object_ptr!=0)
            {
            
            //fill the texture with robs gfx

            if (scratch_mem_ptr!=0)
            {
             flat_planet_gen(seed,POBs_id,(unsigned char*) scratch_mem_ptr,(unsigned char*) my_tex_ptr->tex_data);
              #if OGL==1
             if (OGL_active==1)
             {
                 err=aglGetError();
                 if (err!=0) report_error ("load_planet(1): AGL error.","\p",err);
                 make_ogl_dynamic_texture(my_tex_ptr);
              }
              #endif
            }
            
            
            //galaxy_gen(seed, my_tex_ptr->tex_data);
            //poke the texture into the object - we assume all polys are covered (they are)
            number_of_polys_in_object=current_object_ptr->number_of_polys;
            current_poly=current_object_ptr->polys_ptr;
            for (i=0;i<number_of_polys_in_object;i++)
              {
//              (*current_poly).Ptexture=my_tex_ptr->tex_data;
                            
              #if OGL==1
              (*current_poly).Pid=my_tex_ptr->name;
              #endif
              

              (*current_poly).draw_code=0;	//not transparent
              current_poly++;
              }
            }

            DisposePtr(scratch_mem_ptr);

return dyn_slot;

} // end of load_planet

int load_moon(double x_in_m, double y_in_m, double z_in_m, int seed, short galactic_id,int POBs_id, double planet_diameter_in_m, double planet_mass_in_kg, double rotation_time_in_secs)
{
int static_slot,dyn_slot;

register ZObject * current_object_ptr;
load_dyn_object_pb the_params;
dynamic_texture * my_tex_ptr;
_ZPoly *current_poly;
int number_of_polys_in_object,i,err;
float scale_factor;

            Ptr scratch_mem_ptr;

//calc section rect
          my_tex_ptr=get_memory_for_dyn_tex();
          if (my_tex_ptr==0)   
          
          {
          add_to_text_display("Zex Exec> WARNING: get_memory_for_dyn_tex failed in load_moon",DLP_WHITE);
          return 0;
          }
//we use PNTZ, a dummy rectangle as our planet.
//we poke the address of our gen on the fly texture into all the polys
//[lanets live between 30 and 120 thousand Z
          scratch_mem_ptr=NewPtr((256*256)+(128*128)+300);  // (1024*1024+300);
           if (scratch_mem_ptr==0)
          {
          add_to_text_display("Zex Exec> WARNING: load_moon couldn't get scratch memory. Graphic not loaded",DLP_WHITE);
          return 0;
          }

        static_slot=get_static_object_slot('PNT3');	// the center object
        if (static_slot==-1) report_error ("load_moon:load_moon couldn't find PNT3","\p",-5);
        
        dyn_slot=find_vacant_dynamic_object_slot();
        if (dyn_slot==-1) report_error ("load_moon:load_moon couldn't find empty slot","\p",-6);


          clear_pb(&the_params);
          the_params.control_use_1=seed;
          
	  the_params.world_x=x_in_m/ZEX_UNIT_SIZE_IN_METERS;
	  the_params.world_y=y_in_m/ZEX_UNIT_SIZE_IN_METERS;
	  the_params.world_z=z_in_m/ZEX_UNIT_SIZE_IN_METERS;		//RangedRdm(2000,6000);
	
//this rotated the planet	the_params.delta_rot_y=1;



        the_params.delta_rot_z=planet_mass_in_kg * UNIT_RADIUS * 3.142 / rotation_time_in_secs;
        

	the_params.mass = planet_mass_in_kg/1000;		// make into metric tonnes for ocb
	  the_params.object_category=MASSIVE_SCENARY;
	  the_params.controller_ref=PLANET_CONTROL;
		
	  the_params.no_trivial_rejection=1;	//controlled when not in view

	scale_factor=(planet_diameter_in_m/ZEX_UNIT_SIZE_IN_METERS)/ (2*(ocb_Sptr->object_list[static_slot].radius));	// static radius in zex units

	load_dyn_object(static_slot,dyn_slot,&the_params,galactic_id,scale_factor,-1,!DUST_BIT);	//the object, the position (-1=next free)
										// parent_slot=-1 ... planets come under a different system

        set_rotation_around_world_x(dyn_slot, 270);
										
	if(sun_slot2==-1)	// only one sun in this universe
		{
          	  set_stable_orbit_vector(dyn_slot,sun_slot1);
		}
	else
		{
		// need to orbit around double mass
		DOUBLE_set_stable_orbit_vector(dyn_slot,sun_slot1,sun_slot2);
		}

	current_object_ptr=&ocb_ptr->object_list[dyn_slot];
//set up texture
//get mem ofr otexture
          current_object_ptr->dynamic_tex_ptr=my_tex_ptr;
          
          gen_planet_name(seed, current_object_ptr->Dyn_OCB_control_data.name_str,2);

          if (current_object_ptr!=0)
            {
            
            //fill the texture with robs gfx

            if (scratch_mem_ptr!=0)
            {
             flat_planet_gen(seed,POBs_id,(unsigned char*) scratch_mem_ptr,(unsigned char*) my_tex_ptr->tex_data);
              #if OGL==1
             if (OGL_active==1)
             {
                 err=aglGetError();
                 if (err!=0) report_error ("load_moon(1): AGL error.","\p",err);
                 make_ogl_dynamic_texture(my_tex_ptr);
              }
              #endif
            }
            
            
            //galaxy_gen(seed, my_tex_ptr->tex_data);
            //poke the texture into the object - we assume all polys are covered (they are)
            number_of_polys_in_object=current_object_ptr->number_of_polys;
            current_poly=current_object_ptr->polys_ptr;
            for (i=0;i<number_of_polys_in_object;i++)
              {
//              (*current_poly).Ptexture=my_tex_ptr->tex_data;
                            
              #if OGL==1
              (*current_poly).Pid=my_tex_ptr->name;
              #endif
              

              (*current_poly).draw_code=0;	//not transparent
              current_poly++;
              }
            }

            DisposePtr(scratch_mem_ptr);

return dyn_slot;

} // end of load_moon



//Load a mothership
int load_mothership(double x_in_m, double y_in_m, double z_in_m, short galactic_id, int allegiance)
{
int ms1_slot=0,ms2_slot, ms3_slot, dyn_slot;
int the_return_slot=-1;
int center_bolt_id;

load_dyn_object_pb the_params;

fprintf (stderr, "\nload_mothership: Location x:%.0f, y:%.0f, z:%.0f, gal_id %d, allegiance=%d\n",x_in_m, y_in_m, z_in_m,galactic_id,allegiance);

//Load the two objects - center peice and arm. Arms are position +-1250 units from center piece in X
if ((allegiance==0) || (allegiance>1))	//Anything but RMparian
{
        ms1_slot=get_static_object_slot('MS02');	// the center object
        if (ms1_slot==-1) report_error ("load_mothership: couldn't find MS02","\p",-15);
        ms2_slot=get_static_object_slot('MS01');	// the arm
        if (ms2_slot==-1) report_error ("load_mothership: couldn't find MS01","\p",-16);

//load center object at x,y,z        
        dyn_slot=find_vacant_dynamic_object_slot();
        if (dyn_slot==-1) report_error ("load_mothership: couldn't find empty dyn slot","\p",-17);

        the_return_slot=dyn_slot;	//return the cetner object
	  clear_pb(&the_params);
	  the_params.world_x=x_in_m/ZEX_UNIT_SIZE_IN_METERS;
	  the_params.world_y=y_in_m/ZEX_UNIT_SIZE_IN_METERS;
	  the_params.world_z=z_in_m/ZEX_UNIT_SIZE_IN_METERS;		//RangedRdm(2000,6000);

	the_params.controller_ref=MOTHERSHIP_CONTROL;	//mothership

       the_params.control_use_1_float=RangedRdm(5,20);      //time to launch first ship

	the_params.mass=35000;	//basic ship weighs 1 tonne
        the_params.shield_value=10000;
	the_params.object_category=allegiance;

	 the_params.unique_id=get_object_unique_id();
	 center_bolt_id=dyn_slot;	//for bolts
	 

//load it
	load_dyn_object(ms1_slot,dyn_slot,&the_params,galactic_id,1,-1,!DUST_BIT);	//the object, the position (-1=next free)
fprintf (stderr, "load_mothership: Loaded Centre object x:%.0f, y:%.0f, z:%.0f\n",the_params.world_x, the_params.world_y, the_params.world_z);

//now left arm
        dyn_slot=find_vacant_dynamic_object_slot();
        if (dyn_slot==-1) report_error ("load_mothership: couldn't find empty dyn slot","\p",-17);
//no need to clear the PB, but we do need to tell load_dyn_object the PB is good
      the_params.init_check='chek';
 
	  the_params.world_x=x_in_m/ZEX_UNIT_SIZE_IN_METERS;
      the_params.world_x-=2500;
	  the_params.world_y=y_in_m/ZEX_UNIT_SIZE_IN_METERS;
	  the_params.world_z=z_in_m/ZEX_UNIT_SIZE_IN_METERS;		//RangedRdm(2000,6000);

	the_params.delta_rot_x=960000;
	the_params.controller_ref=MOTHERSHIP_PART;	//mothership
        the_params.shield_value=10000;

	 the_params.unique_id=get_object_unique_id();

//load it
	load_dyn_object(ms2_slot,dyn_slot,&the_params,galactic_id,1,-1,!DUST_BIT);	//the object, the position (-1=next free)
fprintf (stderr, "load_mothership: Loaded left arm x:%.0f, y:%.0f, z:%.0f\n",the_params.world_x, the_params.world_y, the_params.world_z);
//connect it to center object

            (*ocb_ptr).object_list[dyn_slot].bolted_to=center_bolt_id;
            (*ocb_ptr).object_list[dyn_slot].bolt_x_offset=-2500;
            (*ocb_ptr).object_list[dyn_slot].bolt_y_offset=0;
            (*ocb_ptr).object_list[dyn_slot].bolt_z_offset=0;
//now right arm
        dyn_slot=find_vacant_dynamic_object_slot();
        if (dyn_slot==-1) report_error ("load_mothership: couldn't find empty dyn slot","\p",-17);
//no need to clear the PB, but we do need to tell load_dyn_object the PB is good
      the_params.init_check='chek';
 
	  the_params.world_x=x_in_m/ZEX_UNIT_SIZE_IN_METERS;
      the_params.world_x+=2500;
	  the_params.world_y=y_in_m/ZEX_UNIT_SIZE_IN_METERS;
	  the_params.world_z=z_in_m/ZEX_UNIT_SIZE_IN_METERS;		//RangedRdm(2000,6000);

	the_params.delta_rot_x=-960000;
	the_params.controller_ref=MOTHERSHIP_PART;	//mothership

	 the_params.unique_id=get_object_unique_id();

//load it
	load_dyn_object(ms2_slot,dyn_slot,&the_params,galactic_id,1,-1,!DUST_BIT);	//the object, the position (-1=next free)
//connect it to cetner object
            (*ocb_ptr).object_list[dyn_slot].bolted_to=center_bolt_id;
            (*ocb_ptr).object_list[dyn_slot].bolt_x_offset=2500;
            (*ocb_ptr).object_list[dyn_slot].bolt_y_offset=0;
            (*ocb_ptr).object_list[dyn_slot].bolt_z_offset=0;
fprintf (stderr, "load_mothership: Loaded right arm x:%.0f, y:%.0f, z:%.0f\n",the_params.world_x, the_params.world_y, the_params.world_z);

}	//end of earth mothership
else
if ((allegiance==1) )	//alien (we take neutral for now too)
{
        ms3_slot=get_static_object_slot('TUB1');	// the center tube
        if (ms1_slot==-1) report_error ("load_mothership: couldn't find TUB1","\p",-18);
        ms1_slot=get_static_object_slot('ST01');	// the small ring
        if (ms1_slot==-1) report_error ("load_mothership: couldn't find ST01","\p",-19);
        ms2_slot=get_static_object_slot('ST02');	// the large ring
        if (ms2_slot==-1) report_error ("load_mothership: couldn't find ST02","\p",-20);


//load center object at x,y,z        
        dyn_slot=find_vacant_dynamic_object_slot();
        if (dyn_slot==-1) report_error ("load_mothership: couldn't find empty dyn slot","\p",-21);

        the_return_slot=dyn_slot;	//return the cetner object
	  clear_pb(&the_params);
	  the_params.world_x=x_in_m/ZEX_UNIT_SIZE_IN_METERS;
	  the_params.world_y=y_in_m/ZEX_UNIT_SIZE_IN_METERS;
	  the_params.world_z=z_in_m/ZEX_UNIT_SIZE_IN_METERS;		//RangedRdm(2000,6000);

	the_params.controller_ref=MOTHERSHIP_CONTROL;	//mothership
    the_params.control_use_1_float=RangedRdm(2,10);      //time to launch first ship

	the_params.mass=30000;	//basic ship weighs 1 tonne
        the_params.shield_value=10000;
	the_params.object_category=ALIEN;

	 the_params.unique_id=get_object_unique_id();
	 center_bolt_id=dyn_slot;	//for bolts
	 

//load it
	load_dyn_object(ms1_slot,dyn_slot,&the_params,galactic_id,50,-1,!DUST_BIT);	//the object, the position (-1=next free)
fprintf (stderr, "load_mothership: Loaded docking bay x:%.0f, y:%.0f, z:%.0f\n",the_params.world_x, the_params.world_y, the_params.world_z);

//****now the tube****
        dyn_slot=find_vacant_dynamic_object_slot();
        if (dyn_slot==-1) report_error ("load_mothership: couldn't find empty dyn slot","\p",-22);
//no need to clear the PB, but we do need to tell load_dyn_object the PB is good
      the_params.init_check='chek';
	the_params.controller_ref=MOTHERSHIP_PART;	//mothership
 
	  the_params.world_x=x_in_m/ZEX_UNIT_SIZE_IN_METERS;
	  the_params.world_y=y_in_m/ZEX_UNIT_SIZE_IN_METERS;
      the_params.world_y+=55000;
	  the_params.world_z=z_in_m/ZEX_UNIT_SIZE_IN_METERS;		//RangedRdm(2000,6000);

	//the_params.delta_rot_x=12000;
	

	 the_params.unique_id=get_object_unique_id();

//load it
	load_dyn_object(ms3_slot,dyn_slot,&the_params,galactic_id,40,-1,!DUST_BIT);	//the object, the position (-1=next free)
//connect it to center object
fprintf (stderr, "load_mothership: Loaded tube x:%.0f, y:%.0f, z:%.0f\n",the_params.world_x, the_params.world_y, the_params.world_z);

            (*ocb_ptr).object_list[dyn_slot].bolted_to=center_bolt_id;
            (*ocb_ptr).object_list[dyn_slot].bolt_x_offset=0;
            (*ocb_ptr).object_list[dyn_slot].bolt_y_offset=55000;
            (*ocb_ptr).object_list[dyn_slot].bolt_z_offset=0;
//****now big ring at top - ST02****
        dyn_slot=find_vacant_dynamic_object_slot();
        if (dyn_slot==-1) report_error ("load_mothership: couldn't find empty dyn slot","\p",-23);
//no need to clear the PB, but we do need to tell load_dyn_object the PB is good
      the_params.init_check='chek';
	the_params.controller_ref=MOTHERSHIP_PART;	//mothership

    	the_params.delta_rot_y=982000;
 
	  the_params.world_x=x_in_m/ZEX_UNIT_SIZE_IN_METERS;
	  the_params.world_y=y_in_m/ZEX_UNIT_SIZE_IN_METERS;
      the_params.world_y-=90000;
	  the_params.world_z=z_in_m/ZEX_UNIT_SIZE_IN_METERS;		//RangedRdm(2000,6000);
	

	 the_params.unique_id=get_object_unique_id();

//load it
	load_dyn_object(ms2_slot,dyn_slot,&the_params,galactic_id,50,-1,!DUST_BIT);	//the object, the position (-1=next free)
fprintf (stderr, "load_mothership: Loaded top ring x:%.0f, y:%.0f, z:%.0f\n",the_params.world_x, the_params.world_y, the_params.world_z);
//connect it to cetner object
            (*ocb_ptr).object_list[dyn_slot].bolted_to=center_bolt_id;
            (*ocb_ptr).object_list[dyn_slot].bolt_x_offset=0;
            (*ocb_ptr).object_list[dyn_slot].bolt_y_offset=90000;
            (*ocb_ptr).object_list[dyn_slot].bolt_z_offset=0;
 
}//end of alien mship
   
else	//not a known allegiance - fatal error
{
 report_error ("load_mothership: Unknown allegiance!","\pAllegiance=",allegiance);
}
fprintf (stderr, "\n\n");

return the_return_slot;
}


