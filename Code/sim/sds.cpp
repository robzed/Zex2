// *************************************MODULE HEADER*********************************
// * FILENAME           : mount.c
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 22:08:25 on 9th July 2001
// * FIRST AUTHOR       : Stu
// *
// * COPYRIGHT (c)        LS
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************
/* CVS bits
$Log: sds.c,v $
Revision 1.12  2002/09/08 02:09:42  stu_c
Precompiled header adjustments

Revision 1.11  2002/03/10 03:41:27  stu
Additional work on trading - we can buy and sell on different motehrships.

Revision 1.10  2002/01/27 18:06:38  stu
Unused varaibles removed

Revision 1.9  2002/01/02 13:30:03  stu
Continuing development for trading/static screens

Revision 1.8  2001/12/29 01:11:22  stu
Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.

Revision 1.7  2001/12/27 19:44:56  stu
Added #if SDS_LOG==1 to control logging of debug essages

Revision 1.6  2001/12/04 23:35:46  stu
Motherships work

Revision 1.5  2001/11/24 19:29:39  stu
*** empty log message ***

Revision 1.4  2001/11/05 20:02:20  stu
More OSX work

Revision 1.3  2001/11/02 19:32:06  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.2  2001/10/27 22:38:29  stu
zplatform included

Revision 1.1  2001/10/22 21:29:01  rob
Carbon warning changes

Revision 1.0.0.1  2001/10/17 20:46:07  rob
First Imported.

Revision 1.1  2001/09/04 20:34:52  stu
Initial issue

*/

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <MacTypes.h>
    #include <fp.h>
  #endif

  #include "3d_structures_def.h"
  #include "zex_misc.h"
  #include "processor.h"
  #include "real_time.h"
  #include "access_ocbs.h"
#endif //pch

#include "dlp.h"
#include "instrum.h"
#include "physics.h"
#include "txtsplat.h"
#include "matrix_primitives.h"
//#include "matrix_primitives.int"
#include "3d_top.h"
#include "fcircle.h"
#include "Zsound.h"
#include "objects_controls.h"
#include "angular_stuff.h"
#include "ship_control.h"
#include "sds.h"


// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *


// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *


module_private mount_pb_type the_mounting_points[MAX_MOUNTS];
module_private int mount_count=0;


// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

//this function initialises the mount info structure
//called from init and coffee
//exported to coffee
void init_ship_data (void)
{
 mount_count=0;
 #if SDS_LOG==1
   show_text_in_loading_screen("SDS: Initialising Ships Data \n");	
 #endif
}

//this function loads a new mount_pb into the next slot of the mount_info array
//and updates the index of mount pb's
//exported to coffee
void load_ship_data (mount_pb_type* mount_pb)
{


if (mount_pb==0)
{
   show_text_in_loading_screen("SDS: load_ship_data(mount_pb_type* mount_pb) passed NULL ptr from Coffee!\n");	
}


if(mount_count<MAX_MOUNTS)
 {
 
 #if ZEX_PROJECT_BUILDER==1 && PB_PRINT_TO_LOG_ENABLED //can only printf to OSX syscon
  #if SDS_LOG==1
 id=mount_pb->id;
 //make id a c string
 id_string[0]=id>>24;
 id_string[1]=id>>16;
 id_string[2]=id>>8;
 id_string[3]=id;
 id_string[4]=0;
 fprintf (stderr,"SDS: Loading %s\n",id_string);
 #endif
 #endif
 
  the_mounting_points[mount_count]=(*mount_pb);
  mount_count++;
 }
else
 {
   show_text_in_loading_screen("SDS: load_ship_data(mount_pb_type* mount_pb) No free data slots!\n");	
   add_to_text_display("EXEC mount_load is out of slots - can't load mount!",1);
 }
 
}

//this function updates all static definitions.
//it also updates all currently loaded dynamic ocbs so objects do not need to be dismissed
//and summoned again 
//exported to coffee
void sds_update(void)
{
	
}


//this function is called from load_object to see if mounting point info exists
//for a given id
//returns either 0 or a pointer to the mounting point info
mount_pb_type* mount_exists (unsigned long the_id)
{
//search the mounts for an id
int n;
	for (n=0;n<mount_count;n++)
		{
			if(the_mounting_points[n].id==the_id)
			return &the_mounting_points[n];
		}
return 0;
}



// ---------------------------------------------------------
// ---------------------------------------------------------
//   SDS ACCESSORS   
// ---------------------------------------------------------
// ---------------------------------------------------------

extern DynObjectsFixedSize *ocb_ptr;


UInt32 get_main_thrust_eng_1(int object_slot_number)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
return ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mthr1>>3;
	
}

UInt32 get_main_thrust_eng_2(int object_slot_number)
{
#if ACCESSORS_DEBUG 
check_slot(object_slot_number); 
#endif
return ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mthr2>>3;
	
}


UInt32 get_main_thrust_total(int object_slot_number)
{
 UInt32 th1, th2;

 #if ACCESSORS_DEBUG 
 check_slot(object_slot_number); 
 #endif

 th1 = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mthr1>>3;

 th2 = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mthr2>>3;
	
 return th1+th2;

}


//will set bits to ff if selector not recognised - i.e. an error
float get_ship_non_status_value (int object_slot_number, unsigned long selector)
{

 #if ACCESSORS_DEBUG 
 check_slot(object_slot_number); 
 #endif
 
 switch (selector)
        {
         //32 bit values


         case MASSV:
          {
           return ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mass_tonnes;
           break;
          }

         case FUELCAPV:
          {
           return ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_fuel_capacity_kg;
           break;
          }

         case ROTPITCHV:
          {
           return ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.rotational_pitch;
           break;
          }

         case ROTROLLV:
          {
           return ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.rotational_roll;
           break;
          }

         case ROTYAWV:
          {
           return ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.rotational_yaw;
           break;
          }

         case CARGOCAPV:
          {
           return ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.cargo_capacity_tonnes;
           break;
          }

         case HULLCLASSV:
          {
           return ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.hull_class;
           break;
          }


         default :
           return 0;
           break;
   }
}



//will set bits to ff if selector not recognised - i.e. an error
void get_ship_mount_data (int object_slot_number, unsigned long selector, sds_mount_data_type* return_data)
{

 #if ACCESSORS_DEBUG 
 check_slot(object_slot_number); 
 #endif
 
 switch (selector)
        {
         //32 bit values


         case MTHR1V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].mthr1;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mthr1>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mthr1&7;
           break;
          }

         case MTHR2V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].mthr2;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mthr2>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mthr2&7;
           break;
          }
          
         case MREV1V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].mrev1;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mrev1>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mrev1&7;
           break;
          }
 
         case MREV2V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].mrev2;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mrev2>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mrev2&7;
           break;
          }


         case VTHR1V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].vthr1;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_vthr1>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_vthr1&7;
           break;
          }

         case VTHR2V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].vthr2;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_vthr2>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_vthr2&7;
           break;
          }


         case RVTH1V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].rvth1;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_rvth1>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_rvth1&7;
           break;
          }

         case RVTH2V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].rvth2;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_rvth2>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_rvth2&7;
           break;
          }


         case GUN01V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].gun01;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_gun01>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_gun01&7;
           break;
          }

         case GUN02V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].gun02;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_gun02>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_gun02&7;
           break;
          }
         case GUN03V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].gun03;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_gun03>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_gun03&7;
           break;
          }
         case GUN04V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].gun04;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_gun04>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_gun04&7;
           break;
          }


         case LAS01V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].las01;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_las01>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_las01&7;
           break;
          }

         case LAS02V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].las02;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_las02>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_las02&7;
           break;
          }
         case LAS03V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].las03;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_las03>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_las03&7;
           break;
          }
         case LAS04V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].las04;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_las04>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_las04&7;
           break;
          }


         case MIS01V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].mis01;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mis01>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mis01&7;
           break;
          }

         case MIS02V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].mis02;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mis02>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mis02&7;
           break;
          }
         case MIS03V:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].mis03;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mis03>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_mis03&7;
           break;
          }


         case EJECTV:
          {
           return_data->the_position = ocb_ptr->object_list[object_slot_number].eject;
           return_data->the_value = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_eject>>3;
           return_data->the_bits = ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.bk_eject&7;
           break;
          }

         default :
           return_data->the_value=-1;
           return_data->the_bits=0xff;
           break;
   }
}


//note: this routine does not test the selector. Beware! GIGO.
void get_ship_attrib_data (int object_slot_number, unsigned long selector, sds_attrib_data_type * return_data)
{
UInt8 * sds_data_ptr;

 #if ACCESSORS_DEBUG 
 check_slot(object_slot_number); 
 #endif


           sds_data_ptr =  & ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.shields1_class;	//start of 8 bit data
           sds_data_ptr += selector - SHIELD1V; //calc offset

 return_data->the_value=*sds_data_ptr>>3;
 return_data->the_bits=*sds_data_ptr&7;
        

}

//note: this routine does not test the selector. Beware! GIGO.
void decrement_ship_attrib_data (int object_slot_number, unsigned long selector)
{
UInt8 * sds_data_ptr;
UInt8 value;
UInt8 bits;

 #if ACCESSORS_DEBUG 
 check_slot(object_slot_number); 
 #endif


           sds_data_ptr =  & ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.shields1_class;	//start of 8 bit data
           sds_data_ptr += selector - SHIELD1V; //calc offset
           value=*sds_data_ptr>>3;
           bits=*sds_data_ptr&7;
           if (value>0)
           {
            value--;
            value<<=3;
            value|=bits;
            *sds_data_ptr=value;
           }
        

}



// ------- Interface to ship control items ------------- //

void set_ship_thruster (int object_slot_number, unsigned long selector, UInt8 magnitude)	//selector=one of MTHR1V etc	
{
UInt8 * sds_data_ptr;

 #if ACCESSORS_DEBUG 
 check_slot(object_slot_number); 
 #endif
           sds_data_ptr =  & ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.command_mthr1;	
           sds_data_ptr += selector; //calc offset

           *sds_data_ptr=magnitude;        
}


void set_ship_rotational_thruster (int object_slot_number, unsigned long selector, SInt8 magnitude)     //selector = ROTPITCHV, ROTROLLV, ROTYAWV
{
SInt8 * sds_data_ptr;

 #if ACCESSORS_DEBUG 
 check_slot(object_slot_number); 
 #endif
           sds_data_ptr =  & ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.command_pitch;
           sds_data_ptr += selector-ROTPITCHV; //calc offset

           *sds_data_ptr=magnitude;        
}


void set_ship_item_fired (int object_slot_number, UInt32 the_bit)	//the_bit = one of COMMAND_LASER1_FIRED etc
{
UInt32 * sds_data_ptr;

 #if ACCESSORS_DEBUG 
 check_slot(object_slot_number); 
 #endif


           sds_data_ptr =  & ocb_ptr->object_list[object_slot_number].Dyn_OCB_control_data.command_fired_flags;	//start of 8 bit animation data
           *sds_data_ptr=*sds_data_ptr|the_bit;
}



//misc cumulative functions used in sim
float get_reverse_thruster_power_in_newtons (int the_object)
{
sds_mount_data_type the_mount_info;
float total_thrust;

get_ship_mount_data (the_object, MREV1V, &the_mount_info);
total_thrust=the_mount_info.the_value;
get_ship_mount_data (the_object, MREV2V, &the_mount_info);
total_thrust+=the_mount_info.the_value;
return total_thrust;
}

float get_main_thruster_power_in_newtons (int the_object)
{
sds_mount_data_type the_mount_info;
float total_thrust;

get_ship_mount_data (the_object, MTHR1V, &the_mount_info);
total_thrust=the_mount_info.the_value;
get_ship_mount_data (the_object, MTHR2V, &the_mount_info);
total_thrust+=the_mount_info.the_value;
return total_thrust;
}


// ------------Following routines are for when docked. They copy the pertinent data out of the OCB to 
// A local data structure. We copy back to the OCB when the player launches
//All transactions are done through this structure 
ZObjectControl player_kit;

void copy_kit_from_ship(int object_number)
{
 player_kit=ocb_ptr->object_list[object_number].Dyn_OCB_control_data;
}

void copy_kit_to_ship(int object_number)
{
 ocb_ptr->object_list[object_number].Dyn_OCB_control_data=player_kit;
}


//note: this routine does not test the selector. Beware! GIGO.
void get_kit_attrib_data (unsigned long selector, sds_attrib_data_type * return_data)
{
UInt8 * sds_data_ptr;


           sds_data_ptr =  & player_kit.shields1_class;	//start of 8 bit data
           sds_data_ptr += selector - SHIELD1V; //calc offset
 return_data->the_value=*sds_data_ptr>>3;
 return_data->the_bits=*sds_data_ptr&7;

}