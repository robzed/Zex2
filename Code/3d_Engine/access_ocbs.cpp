// *************************************MODULE HEADER*********************************
// * FILENAME           : access_ocbs.c
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * DATE STARTED       : Sun 08:56:11 on 8th Oct 2000
// * FIRST AUTHOR       : Rob Probin
// *
// * COPYRIGHT (c)        Lightsoft
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
  #include "LS_Standard.h"
  #include "3d_structures_def.h"
  #include "access_ocbs.h"
  #include "Error_handler.h"
#endif

#include "Physics.h"
#include "objects_controls.h"



// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

extern DynObjectsFixedSize *ocb_ptr;


//given a dyn object ptr, convert it to slot number
int ptr_to_dynslot(ZObject* which_object)
{
  return which_object-&ocb_ptr->object_list[0]; //note ptr arithmetic, not byte!
}

#if ACCESSORS_DEBUG 
void check_slot(int object_slot_number)
{
 if (object_slot_number<0 || object_slot_number>=MAX_NUMBER_OF_DYN_OBJECTS)
   {
//   Debugger();
   report_error ("OCB_ACESSORS. Bad slot number:","",object_slot_number);
   }
}

void check_static_slot(int object_slot_number)
{
 if (object_slot_number<0 || object_slot_number>=MAX_NUMBER_OF_STAT_OBJECTS)
   {
//   Debugger();
   report_error("OCB_ACESSORS. Bad static slot number:","",object_slot_number);
   }
}
#endif


double get_mass_in_kg(int object_slot_number)
{

#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif

return (ocb_ptr->object_list[object_slot_number].mass)*1000;
}

void set_mass_in_kg(int object_slot_number, double mass)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
ocb_ptr->object_list[object_slot_number].mass = mass/1000;
}

void set_has_children(int object_slot_number)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
ocb_ptr->object_list[object_slot_number].kill_flags|=HAS_CHILDREN;
}

void get_velocity_in_mps(int object_slot_number,_3D *velocity_ptr)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
velocity_ptr->x = ocb_ptr->object_list[object_slot_number].abs_velocity_vector.x*ZEX_UNIT_SIZE_IN_METERS;
velocity_ptr->y = ocb_ptr->object_list[object_slot_number].abs_velocity_vector.y*ZEX_UNIT_SIZE_IN_METERS;
velocity_ptr->z = ocb_ptr->object_list[object_slot_number].abs_velocity_vector.z*ZEX_UNIT_SIZE_IN_METERS;
}


float get_ND_velocity_in_mps(int object_slot_number)
{
vector velocity;
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
velocity.x = ocb_ptr->object_list[object_slot_number].abs_velocity_vector.x*ZEX_UNIT_SIZE_IN_METERS;
velocity.y = ocb_ptr->object_list[object_slot_number].abs_velocity_vector.y*ZEX_UNIT_SIZE_IN_METERS;
velocity.z = ocb_ptr->object_list[object_slot_number].abs_velocity_vector.z*ZEX_UNIT_SIZE_IN_METERS;
return sqrt((velocity.x*velocity.x)+(velocity.y*velocity.y)+(velocity.z*velocity.z));
}


void get_rel_velocity_in_mps(int object_slot_number,vector *velocity_ptr)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
velocity_ptr->x = ocb_ptr->object_list[object_slot_number].rel_velocity_vector.x*ZEX_UNIT_SIZE_IN_METERS;
velocity_ptr->y = ocb_ptr->object_list[object_slot_number].rel_velocity_vector.y*ZEX_UNIT_SIZE_IN_METERS;
velocity_ptr->z = ocb_ptr->object_list[object_slot_number].rel_velocity_vector.z*ZEX_UNIT_SIZE_IN_METERS;
}


void set_velocity_in_mps(int object_slot_number,_3D velocity)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
ocb_ptr->object_list[object_slot_number].abs_velocity_vector.x = velocity.x/ZEX_UNIT_SIZE_IN_METERS;
ocb_ptr->object_list[object_slot_number].abs_velocity_vector.y = velocity.y/ZEX_UNIT_SIZE_IN_METERS;
ocb_ptr->object_list[object_slot_number].abs_velocity_vector.z = velocity.z/ZEX_UNIT_SIZE_IN_METERS;
}


void get_position_in_m(int object_slot_number,_3D *position_ptr)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
position_ptr->x = ocb_ptr->object_list[object_slot_number].Oworldx*ZEX_UNIT_SIZE_IN_METERS;
position_ptr->y = ocb_ptr->object_list[object_slot_number].Oworldy*ZEX_UNIT_SIZE_IN_METERS;
position_ptr->z = ocb_ptr->object_list[object_slot_number].Oworldz*ZEX_UNIT_SIZE_IN_METERS;
}

void get_raw_position(int object_slot_number,_3D *position_ptr)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
position_ptr->x = ocb_ptr->object_list[object_slot_number].Oworldx;
position_ptr->y = ocb_ptr->object_list[object_slot_number].Oworldy;
position_ptr->z = ocb_ptr->object_list[object_slot_number].Oworldz;
}

void set_position_in_m(int object_slot_number,_3D position)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
ocb_ptr->object_list[object_slot_number].Oworldx = position.x/ZEX_UNIT_SIZE_IN_METERS;
ocb_ptr->object_list[object_slot_number].Oworldy = position.y/ZEX_UNIT_SIZE_IN_METERS;
ocb_ptr->object_list[object_slot_number].Oworldz = position.z/ZEX_UNIT_SIZE_IN_METERS;
}

void set_object_position(int object_slot_number, double x, double y, double z)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
ocb_ptr->object_list[object_slot_number].Oworldx = x;
ocb_ptr->object_list[object_slot_number].Oworldy = y;
ocb_ptr->object_list[object_slot_number].Oworldz = z;
}

int get_object_in_use(int object_slot_number)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
return (ocb_ptr->object_list[object_slot_number].in_use)==1;
}

void set_object_in_use(int object_slot_number)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
ocb_ptr->object_list[object_slot_number].in_use=1;
}

void set_object_not_in_use(int object_slot_number)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
ocb_ptr->object_list[object_slot_number].in_use=0;
}


//sets blobbed colour. Components in range 0-1.
void set_blobbed_object_colour(int object_slot_number, float red, float green, float blue)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
	 (*ocb_ptr).object_list[object_slot_number].blobbed_colour_red=red;
	 (*ocb_ptr).object_list[object_slot_number].blobbed_colour_green=green;
	 (*ocb_ptr).object_list[object_slot_number].blobbed_colour_blue=blue;

}




int get_object_targetted(int object_slot_number)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
 return ocb_ptr->object_list[object_slot_number].object_targetted;
}


float get_total_fuel_consumption(int object_slot_number)
{
double return_val;
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
return_val = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.current_total_fuel_consumption;
return return_val;
}


float get_engine_temperature_in_C(int object_slot_number)
{
double return_val;
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
return_val = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.eng_temperature_in_C;
return return_val;
}


void set_engine_temperature_in_C(int object_slot_number, float the_temperature_in_C)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.eng_temperature_in_C=the_temperature_in_C;
}

void set_hull_temperature_in_C(int object_slot_number, float the_temperature_in_C)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.hull_temperature_in_C=the_temperature_in_C;
}


float get_hull_temperature_in_C(int object_slot_number)
{
double return_val;
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
return_val = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.hull_temperature_in_C;
return return_val;
}


int get_object_category( int object_slot_number)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
 return ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.allegiance;
}

int get_galactic_id(int object_slot_number)
{
#if ACCESSORS_DEBUG
check_slot(object_slot_number);
#endif
  return ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.galactic_id;
}

int get_object_controller(int object_slot_number)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
 return ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.controller;
}

ZObjectControl* get_object_control_data(int object_slot_number)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
 return &(ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data);
}

int get_object_control_use_1(int object_slot_number)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
 return ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.control_use_1;
}



// This function checks whether an object is dust or
// the hud unit vector, and returns non-zero if not (i.e. this is a real
// object that should be effected by the forces of the universe
// or whether it is a on-board computer generated object. )
int object_is_real(int object_slot_number)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif

if(object_slot_number == 1 ) { return FALSE; }	    // if it is the HUD vector unit, ignore

if(ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.controller==DUST)
	{ return FALSE; }
	

return TRUE;			// it passed all the tests, it is real
}


void set_object_targetted(int object_slot_number, int slot_targetted)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
 ocb_ptr->object_list[object_slot_number].object_targetted=slot_targetted;
}


//new 221100
vector get_object_abs_vel_vector(int object)
{
#if ACCESSORS_DEBUG 
check_slot(object); 
#endif

  return ocb_ptr->object_list[object].abs_velocity_vector;
}

float get_object_radius_in_metres(int object)
{
#if ACCESSORS_DEBUG 
check_slot(object); 
#endif

  return ocb_ptr->object_list[object].radius*ZEX_UNIT_SIZE_IN_METERS;
}


void set_object_abs_vel_vector(int object, vector the_vect)
{
#if ACCESSORS_DEBUG 
check_slot(object); 
#endif

  ocb_ptr->object_list[object].abs_velocity_vector=the_vect;
}


// ---------------------------------------------------------

int get_object_Oid(int object)
{
#if ACCESSORS_DEBUG 
check_slot(object); 
#endif

return (ocb_ptr->object_list[object].Oid);
}


void get_translated_normal(int object_slot_number,_3D *trans_normal_ptr)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
trans_normal_ptr->x = ocb_ptr->object_list[object_slot_number].n_translated.x;
trans_normal_ptr->y = ocb_ptr->object_list[object_slot_number].n_translated.y;
trans_normal_ptr->z = ocb_ptr->object_list[object_slot_number].n_translated.z;
}





// ---------------------------------------------------------
// ---------------------------------------------------------
//   STATIC SLOT ACCESSORS   
//
// do these belong here?
// ---------------------------------------------------------
// ---------------------------------------------------------

extern	StatObjectsFixedSize *ocb_Sptr;


int STATIC_get_object_Oid(int object)
{
#if ACCESSORS_DEBUG 
check_slot(object); 
#endif

return (ocb_Sptr->object_list[object].Oid);
}


int STATIC_get_object_in_use(int object)
{
#if ACCESSORS_DEBUG 
check_slot(object); 
#endif

return (ocb_Sptr->object_list[object].in_use)==1;
}


float STATIC_get_object_radius_in_metres(int object)
{
#if ACCESSORS_DEBUG 
check_static_slot(object); 
#endif

return ocb_Sptr->object_list[object].radius*ZEX_UNIT_SIZE_IN_METERS;
}

