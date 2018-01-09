// *************************************MODULE HEADER*********************************
// * FILENAME           : engines.cpp
// * MODULE TITLE       : Engines
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 22:22:41 on 15th August 2001
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
$Log: engines.cpp,v $
Revision 1.4  2003/09/27 20:41:55  robp
Fixed float->int warnings.

Revision 1.3  2003/09/22 21:04:17  stu_c
Zex: Weapons: Adjustments to temp rise and fall in laser bay. Rebuild required.

Revision 1.2  2003/09/22 20:45:33  stu_c
Zex:Weapons: laser bay temperature and overheat (in progress)

Revision 1.1.1.1  2003/09/05 22:36:21  stu_c
First Imported.

Revision 1.19  2002/09/08 02:10:34  stu_c
Precompiled header adjustments

Revision 1.18  2002/07/28 17:17:42  stu_c
Exhaust particles and further work

Revision 1.17  2002/05/05 22:25:39  stu
Further developement - hull temperature now correct for multiple suns and atmosphere.

All calls to add_to_kill_list check the return

Revision 1.16  2002/04/29 01:39:57  stu
External temperature calcs

Revision 1.15  2002/04/28 06:21:10  stu
This checkin adds more to the HUD and brings out some sim variables to the player.

Revision 1.14  2002/04/20 18:36:55  stu
Furhter work for AP and control at time>1

Revision 1.13  2002/04/13 15:29:55  stu
Rotation, Alycians, behaviour

Revision 1.12  2002/02/21 22:48:50  stu
Call to reset_angular_torque replaces three lines

Revision 1.11  2002/01/27 18:09:55  stu
ORCS bug fix for time multipliers

Revision 1.10  2002/01/20 17:02:05  stu
rotational foces now clamped to 0 above / below CLAMP_RANGE

Revision 1.9  2001/12/29 01:11:31  stu
Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.

Revision 1.8  2001/12/14 19:43:36  stu
Docking

Revision 1.7  2001/11/24 19:29:45  stu
*** empty log message ***

Revision 1.6  2001/11/05 22:54:50  stu
Removal of debugging code for frame #1 crash on PB

Revision 1.5  2001/11/05 22:45:36  stu
Object thrust initialised correctly so engines doesn't crash on frame #1

Revision 1.4  2001/11/05 19:57:29  stu
More OSX work

Revision 1.3  2001/11/02 19:32:11  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.2  2001/10/27 22:38:59  stu
zplatform included

Revision 1.1  2001/10/22 21:29:07  rob
Carbon warning changes

Revision 1.0.0.1  2001/10/17 20:46:07  rob
First Imported.

Revision 1.5  2001/09/26 23:01:17  stu
ORCS rotational dmapers no longer use unfeasably large amounts of fuel. FIxed bug whereby ORCS would consume fuel with all dampers off.

Revision 1.4  2001/09/23 18:59:51  stu
AP work, improved number print, engines.

Revision 1.3  2001/09/18 20:42:48  stu
More Sim/ hud numbers print

Revision 1.2  2001/09/10 21:43:44  stu
100901
More Sim, AP broken.

Revision 1.1  2001/09/04 20:32:26  stu
Initial issue

*/

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #include "3d_structures_def.h"
  #include "processor.h"
  #include "access_ocbs.h"
  #include "real_time.h"
#endif //pch


#include "sds.h"
#include "physics.h"
#include "game_constants.h"
#include "game_defs.h"
#include "engines.h"
#include "Zsound.h"
#include "camera_control.h"

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

int can_consume_fuel=1;

int camera_object_total_fuel;
int camera_object_total_fuel_consumption;
int camera_object_orcs_fuel_consumption;
int camera_object_orcs_rot_fuel_consumption;
int camera_object_orcs_lin_fuel_consumption;



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *
module_private void set_all_thrusters_to_zero(int object);
module_private double run_ORCS(int object);
module_private void run_engine_temperature(int the_object);
module_private void run_hull_temperature(int the_object);
void run_laser_bay_temperature(int the_object);

float calc_temperature(_3D* position1, _3D* sun_pos_in_m);
void set_wanted_temperature_in_C(int the_object,float wanted_temperature);



// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE              : 
// | AUTHOR(s)          :
// | DATE STARTED       : 
// +----------------------------------------------------------------------------------
// | DESCRIPTION:  
// | 
// | 
// |
// |
// +----------------------------------------------------------------------------------
// | PARAM IN   : none
// |
// | PARAM OUT  : none
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial                Descrip.
// |
// |
// +----------------------------------------------------------------------------------
// | ADDITIONAL NOTES:
// | 

double total_fuel_consumption;
double temp_fuel_consumption;
#define ENG_ACCEL_IN_NEWTONS_PER_SECOND 20000	//fixed in this version, might be SDS data later

void run_engines(int the_object)
{
ZObject *source_object;
extern	DynObjectsFixedSize *ocb_ptr;
sds_mount_data_type the_mount_info;
vector d;
float thrust=0;
float main_accel,reverse_accel;	//engine thrust accel for this frame
float actual_thrust;
int engine2_in_use, engine2_rev_in_use;
float main_thrust1,main_thrust2,rev_thrust1,rev_thrust2;
double orcs_cons;   //Note: orcs=orientation control systems



run_engine_temperature(the_object);
run_hull_temperature(the_object);
run_laser_bay_temperature(the_object);

source_object=&(*ocb_ptr).object_list[the_object];

//we make a shortcut and assume both engines are the same
get_ship_mount_data (the_object, MTHR1V, &the_mount_info);
main_accel=(ENG_ACCEL_IN_NEWTONS_PER_SECOND/the_mount_info.the_value);
if(main_accel>1.0) main_accel=1.0;
main_accel*=time_in_secs;

get_ship_mount_data (the_object, MREV1V, &the_mount_info);
reverse_accel=(ENG_ACCEL_IN_NEWTONS_PER_SECOND/the_mount_info.the_value);
if(reverse_accel>1.0) reverse_accel=1.0;
reverse_accel*=time_in_secs;	//how much thrust can we accelerate the engine by this frame


//check out control inputs
temp_fuel_consumption=0;


get_ship_mount_data (the_object, MTHR2V, &the_mount_info);
if((the_mount_info.the_bits&3)==2)
engine2_in_use=1;
else
engine2_in_use=0;

get_ship_mount_data (the_object, MREV2V, &the_mount_info);
if((the_mount_info.the_bits&3)==2)
engine2_rev_in_use=1;
else
engine2_rev_in_use=0;

//no crash here

//change for SDS 110801

//handle main engine(s)
//calcilated demanded thrust
main_thrust1=(float)source_object->Dyn_OCB_control_data.command_mthr1/(float)256;	//0-1
main_thrust2=(float)source_object->Dyn_OCB_control_data.command_mthr2/(float)256;	//0-1
rev_thrust1=(float)source_object->Dyn_OCB_control_data.command_mrev1/(float)256;	//0-1
rev_thrust2=(float)source_object->Dyn_OCB_control_data.command_mrev2/(float)256;	//0-1

if(source_object->Dyn_OCB_control_data.fuel_tank_value_in_KGS>0)
{
	if (main_thrust1>0.1)	//player/controller
	{
	 thrust+=main_thrust1;
	}

	if ((main_thrust2>0.1) && (engine2_in_use==1))	//player/controller
	{
	 thrust+=main_thrust2;
	}
	
	if (source_object->Dyn_OCB_control_data.ORCS_thrust_up>0.1)
	{
	 thrust+=source_object->Dyn_OCB_control_data.ORCS_thrust_up;
	}

//reverse
	if (rev_thrust1>0.1)	//player/controller
	{
	 thrust-=rev_thrust1;
	}

	if ((rev_thrust2>0.1) && (engine2_rev_in_use==1))	//player/controller
	{
	 thrust-=rev_thrust2;
	}
	
	if (source_object->Dyn_OCB_control_data.ORCS_thrust_down>0.1)
	{
	 thrust-=source_object->Dyn_OCB_control_data.ORCS_thrust_down;
	}

	
}


//now calculate actual thrust as rate of change of thrust output from engines
//ok here

if (thrust>=0)
{
  if(source_object->Dyn_OCB_control_data.actual_main_thrust<thrust)
  {
  source_object->Dyn_OCB_control_data.actual_main_thrust+=main_accel;
  if(source_object->Dyn_OCB_control_data.actual_main_thrust>thrust) source_object->Dyn_OCB_control_data.actual_main_thrust=thrust;

  }
  else
  if(source_object->Dyn_OCB_control_data.actual_main_thrust>thrust)
  {
  source_object->Dyn_OCB_control_data.actual_main_thrust-=main_accel;
   if(source_object->Dyn_OCB_control_data.actual_main_thrust<thrust) source_object->Dyn_OCB_control_data.actual_main_thrust=thrust;
  }
}
else
if (thrust<0)
{
  if(source_object->Dyn_OCB_control_data.actual_main_thrust>thrust)
  {
   source_object->Dyn_OCB_control_data.actual_main_thrust-=reverse_accel;
   if(source_object->Dyn_OCB_control_data.actual_main_thrust<thrust) source_object->Dyn_OCB_control_data.actual_main_thrust=thrust;
  }
  else
  if(source_object->Dyn_OCB_control_data.actual_main_thrust<thrust)
  {
   source_object->Dyn_OCB_control_data.actual_main_thrust+=reverse_accel;
   if(source_object->Dyn_OCB_control_data.actual_main_thrust>thrust) source_object->Dyn_OCB_control_data.actual_main_thrust=thrust;
  }
}

//OK here

actual_thrust=source_object->Dyn_OCB_control_data.actual_main_thrust;
//now apply the main engine/reverse engine thrust
if(actual_thrust>0)	//main engines firing?
{

   get_ship_mount_data (the_object, MTHR1V, &the_mount_info);
   d.x=0; d.y=0; d.z=1;	//direction of force
   apply_a_force(the_object,the_mount_info.the_value*actual_thrust /*newton*/ , the_mount_info.the_position   /*point*/ , d /*direction*/);
   temp_fuel_consumption+=source_object->Dyn_OCB_control_data.main_thrust_consumption_per_second_in_KGS*actual_thrust;

   if(engine2_in_use==1)
   {
   get_ship_mount_data (the_object, MTHR2V, &the_mount_info);
    d.x=0; d.y=0; d.z=1;	//direction of force
    apply_a_force(the_object,the_mount_info.the_value*actual_thrust /*newton*/ , the_mount_info.the_position   /*point*/ , d /*direction*/);
    temp_fuel_consumption+=source_object->Dyn_OCB_control_data.main_thrust_consumption_per_second_in_KGS*actual_thrust;
   }
}
else //reverse engines firing
{
   actual_thrust=-actual_thrust;
   get_ship_mount_data (the_object, MREV1V, &the_mount_info);
   d.x=0; d.y=0; d.z=-1;	//direction of force
   apply_a_force(the_object,the_mount_info.the_value*actual_thrust /*newton*/ , the_mount_info.the_position   /*point*/ , d /*direction*/);
   temp_fuel_consumption+=source_object->Dyn_OCB_control_data.reverse_thrust_consumption_per_second_in_KGS*actual_thrust;


   if(engine2_rev_in_use==1)
   {
    get_ship_mount_data (the_object, MREV2V, &the_mount_info);
    d.x=0; d.y=0; d.z=-1;	 //direction of force
    apply_a_force(the_object,the_mount_info.the_value*actual_thrust /*newton*/ , the_mount_info.the_position   /*point*/ , d /*direction*/);
    temp_fuel_consumption+=source_object->Dyn_OCB_control_data.reverse_thrust_consumption_per_second_in_KGS*actual_thrust;
   }
}


//crash here

//vertical thrust up
if ((source_object->Dyn_OCB_control_data.command_vthr1>0.1) 
     && (source_object->Dyn_OCB_control_data.fuel_tank_value_in_KGS)>0)
  {
   get_ship_mount_data (the_object, VTHR1V, &the_mount_info);
   d.x=0; d.y=1; d.z=0;	//direction of force
   apply_a_force(the_object,the_mount_info.the_value /*newton*/ , the_mount_info.the_position   /*point*/ , d /*direction*/);
   temp_fuel_consumption+=source_object->Dyn_OCB_control_data.reverse_thrust_consumption_per_second_in_KGS;
  }


if ((source_object->Dyn_OCB_control_data.command_vthr2>0.1) 
     && (source_object->Dyn_OCB_control_data.fuel_tank_value_in_KGS)>0)
  {
   get_ship_mount_data (the_object, MREV2V, &the_mount_info);
   d.x=0; d.y=1; d.z=0;	//direction of force
   apply_a_force(the_object,the_mount_info.the_value /*newton*/ ,  the_mount_info.the_position   /*point*/ , d /*direction*/);
   temp_fuel_consumption+=source_object->Dyn_OCB_control_data.reverse_thrust_consumption_per_second_in_KGS;
  }


//vertical thrust down
if ((source_object->Dyn_OCB_control_data.command_rvth1>0.1) 
     && (source_object->Dyn_OCB_control_data.fuel_tank_value_in_KGS)>0)
  {
   get_ship_mount_data (the_object, RVTH1V, &the_mount_info);
   d.x=0; d.y=-1; d.z=0;	//direction of force
   apply_a_force(the_object,the_mount_info.the_value /*newton*/ ,  the_mount_info.the_position   /*point*/ , d /*direction*/);
   temp_fuel_consumption+=source_object->Dyn_OCB_control_data.reverse_thrust_consumption_per_second_in_KGS;
  }


if ((source_object->Dyn_OCB_control_data.command_rvth2>0.1) 
     && (source_object->Dyn_OCB_control_data.fuel_tank_value_in_KGS)>0)
  {
   get_ship_mount_data (the_object, RVTH2V, &the_mount_info);
   d.x=0; d.y=-1; d.z=0;	//direction of force
   apply_a_force(the_object,the_mount_info.the_value /*newton*/ , the_mount_info.the_position   /*point*/ , d /*direction*/);
   temp_fuel_consumption+=source_object->Dyn_OCB_control_data.reverse_thrust_consumption_per_second_in_KGS;
  }
orcs_cons=run_ORCS(the_object);
temp_fuel_consumption+=orcs_cons;

temp_fuel_consumption+=0.000001; //quiescent power consumption

total_fuel_consumption=temp_fuel_consumption;
//now subtract this fuel usage from the fuel tank
if(can_consume_fuel)
{
 source_object->Dyn_OCB_control_data.fuel_tank_value_in_KGS-=total_fuel_consumption*time_in_secs;
}

source_object->Dyn_OCB_control_data.current_total_fuel_consumption=total_fuel_consumption;

run_engine_sound(the_object);

//for sim_pane
if (get_main_camera_object()==the_object)
{
 camera_object_total_fuel=STATIC_CAST_TO_INT(source_object->Dyn_OCB_control_data.fuel_tank_value_in_KGS*100000000);
 camera_object_total_fuel_consumption=STATIC_CAST_TO_INT(total_fuel_consumption*100000000);
 camera_object_orcs_fuel_consumption=STATIC_CAST_TO_INT(orcs_cons*100000000);
}
set_all_thrusters_to_zero(the_object);

}



// -----------------------------------------------------------------
// ORCS - Orientation Control SystemS

module_private double run_ORCS(int object)	 //returns orcs fuel consumption in KG's
{
ZObject *source_object;
vector p,d,drag;
//vector hv,vv,Vdiff;
float pitch_bleed,roll_bleed,yaw_bleed;
//float cons;
int have_yaw,have_pitch,have_roll;
float yaw_thrust, roll_thrust, pitch_thrust;
float temp_thrust,f;
double ORCS_fuel_consumption;
double ORCS_rot_fuel_consumption, ORCS_lin_fuel_consumption;
extern	DynObjectsFixedSize *ocb_ptr;


sds_mount_data_type the_mount_info;




ORCS_lin_fuel_consumption=0;
ORCS_rot_fuel_consumption=0;


source_object=&(*ocb_ptr).object_list[object];


       //get total main thrust
         get_ship_mount_data (object, MTHR1V, &the_mount_info);
         f=the_mount_info.the_value;
         get_ship_mount_data (object, MTHR2V, &the_mount_info);
         f+=the_mount_info.the_value;
         f/=3;

yaw_thrust=source_object->Dyn_OCB_control_data.rotational_yaw;
pitch_thrust=source_object->Dyn_OCB_control_data.rotational_pitch;
roll_thrust=source_object->Dyn_OCB_control_data.rotational_roll;


if ((source_object->Dyn_OCB_control_data.command_ORCS_damper&ORCS_DAMPER_LIN) 
    && (source_object->Dyn_OCB_control_data.fuel_tank_value_in_KGS)>0)	 //on
{
 
 //LINEAR DAMPERS
 //if object targetted
	if (get_object_targetted(object)!=-1)
	{
    double channel_decrease;
	
 //drag on vel in 
 drag.x=(*ocb_ptr).object_list[object].rel_velocity_vector.x/24;
 drag.y=(*ocb_ptr).object_list[object].rel_velocity_vector.y/24;
 drag.z=(*ocb_ptr).object_list[object].rel_velocity_vector.z/24;
 
 channel_decrease=drag.x*time_in_secs;
 if ( fabs(channel_decrease) < fabs( (*ocb_ptr).object_list[object].abs_velocity_vector.x) )
 {
  (*ocb_ptr).object_list[object].abs_velocity_vector.x-=channel_decrease;
 }
 else
 {
   (*ocb_ptr).object_list[object].abs_velocity_vector.x=0.0;
 } 


 channel_decrease=drag.y*time_in_secs;
 if ( fabs(channel_decrease) < fabs( (*ocb_ptr).object_list[object].abs_velocity_vector.y) )
 {
  (*ocb_ptr).object_list[object].abs_velocity_vector.y-=channel_decrease;
 }
 else
 {
   (*ocb_ptr).object_list[object].abs_velocity_vector.y=0.0;
 } 
 
 
 channel_decrease=drag.z*time_in_secs;
 if ( fabs(channel_decrease) < fabs( (*ocb_ptr).object_list[object].abs_velocity_vector.z) )
 {
  (*ocb_ptr).object_list[object].abs_velocity_vector.z-=channel_decrease;
 }
 else
 {
   (*ocb_ptr).object_list[object].abs_velocity_vector.z=0.0;
 } 

 
// (*ocb_ptr).object_list[object].abs_velocity_vector.y-=drag.y*time_in_secs;
// (*ocb_ptr).object_list[object].abs_velocity_vector.z-=drag.z*time_in_secs;

// if (drag.x > 0.1 || drag.x <-0.1)
 drag.x=fabs(drag.x);
 drag.y=fabs(drag.y);
 drag.z=fabs(drag.z);
 
 if(drag.x>f) drag.x=f;
 if(drag.y>f) drag.y=f;
 if(drag.z>f) drag.z=f;
 
 ORCS_lin_fuel_consumption+=fabs(drag.x);	//DAMPERS NOT FREE ANYMORE

// if (drag.y > 0.1 || drag.y <-0.1)
 ORCS_lin_fuel_consumption+=fabs(drag.y);	//DAMPERS NOT FREE ANYMORE

// if (drag.z > 0.1 || drag.z <-0.1)
 ORCS_lin_fuel_consumption+=fabs(drag.z);	//DAMPERS NOT FREE ANYMORE

  }
}



//reset current angular torque
   reset_angular_torque(object);

//rotational dampers
//center pitch channel
//calc bleedoff

//the rotational channels are designed to reduce by half in one second



//limit inputs
   if (source_object->Dyn_OCB_control_data.ORCS_extern_pitch>pitch_thrust) 
      source_object->Dyn_OCB_control_data.ORCS_extern_pitch=pitch_thrust;

   if (source_object->Dyn_OCB_control_data.ORCS_extern_roll>roll_thrust) 
      source_object->Dyn_OCB_control_data.ORCS_extern_roll=roll_thrust;

   if (source_object->Dyn_OCB_control_data.ORCS_extern_yaw>yaw_thrust) 
      source_object->Dyn_OCB_control_data.ORCS_extern_yaw=yaw_thrust;


   if (source_object->Dyn_OCB_control_data.ORCS_extern_pitch<-pitch_thrust) 
      source_object->Dyn_OCB_control_data.ORCS_extern_pitch=-pitch_thrust;

   if (source_object->Dyn_OCB_control_data.ORCS_extern_roll<-roll_thrust) 
      source_object->Dyn_OCB_control_data.ORCS_extern_roll=-roll_thrust;

   if (source_object->Dyn_OCB_control_data.ORCS_extern_yaw<-yaw_thrust) 
      source_object->Dyn_OCB_control_data.ORCS_extern_yaw=-yaw_thrust;



   have_yaw=0;
   have_roll=0;
   have_pitch=0;
      
   if (1 /*get_time_multiplier()<=5*/ )
   {
//handle yaw
   temp_thrust=yaw_thrust;

   if (source_object->Dyn_OCB_control_data.ORCS_extern_yaw!=0)
   {
     source_object->Dyn_OCB_control_data.yaw_channel+=source_object->Dyn_OCB_control_data.ORCS_extern_yaw*real_frame_time_in_secs;
     have_yaw=1;
    }

   if (source_object->Dyn_OCB_control_data.command_yaw<0)	//left
   {
    have_yaw=1;	
	source_object->Dyn_OCB_control_data.yaw_channel+=yaw_thrust *real_frame_time_in_secs;
   }
   else //no left input
   if (source_object->Dyn_OCB_control_data.command_yaw>0)	//right
   {
    have_yaw=1;	
	source_object->Dyn_OCB_control_data.yaw_channel-=yaw_thrust *real_frame_time_in_secs;
   }
   
   
   
   
//two opposing thrusters
//   if ((source_object->Dyn_OCB_control_data.yaw_channel>0.01) || (source_object->Dyn_OCB_control_data.yaw_channel<-0.01))
   if (have_yaw==1)   ORCS_rot_fuel_consumption+=fabs(temp_thrust /* *source_object->Dyn_OCB_control_data.yaw_channel */ );


   p.x=0; p.y=0; p.z=1;	//position of force
   d.x=1; d.y=0; d.z=0;	//direction of force
   apply_a_force(object, source_object->Dyn_OCB_control_data.yaw_channel, p /*oint*/ , d /*irection*/);   

   p.x=0; p.y=0; p.z=-1;	//position of force
   d.x=-1; d.y=0; d.z=0;	//direction of force
   apply_a_force(object, source_object->Dyn_OCB_control_data.yaw_channel, p /*oint*/ , d /*irection*/);   

   


//handle roll
   temp_thrust=roll_thrust;
   
   if (source_object->Dyn_OCB_control_data.ORCS_extern_roll!=0)
   {
     source_object->Dyn_OCB_control_data.roll_channel+=(source_object->Dyn_OCB_control_data.ORCS_extern_roll*real_frame_time_in_secs);

    }



   if (source_object->Dyn_OCB_control_data.command_roll<0)	//left
   {
    have_roll=1;	
	source_object->Dyn_OCB_control_data.roll_channel-=roll_thrust *real_frame_time_in_secs;
   }
   else //no left input
   if (source_object->Dyn_OCB_control_data.command_roll>0)	//right
   {
    have_roll=1;	
	source_object->Dyn_OCB_control_data.roll_channel+=roll_thrust *real_frame_time_in_secs;
   }
   
//two opposing thrusters
//   if ((source_object->Dyn_OCB_control_data.roll_channel>0.01) || (source_object->Dyn_OCB_control_data.roll_channel<-0.01))
   if (have_roll==1)   ORCS_rot_fuel_consumption+=fabs(temp_thrust /* *source_object->Dyn_OCB_control_data.yaw_channel */ );

//   {
//   ORCS_rot_fuel_consumption+=fabs(temp_thrust*source_object->Dyn_OCB_control_data.roll_channel);

   p.x=1; p.y=0; p.z=0;	//position of force
   d.x=0; d.y=1; d.z=0;	//direction of force
   apply_a_force(object,source_object->Dyn_OCB_control_data.roll_channel /*newton*/,  p   /*point*/ , d /*direction*/);

   p.x=-1; p.y=0; p.z=0;	//position of force
   d.x=0; d.y=-1; d.z=0;	//direction of force
   apply_a_force(object,source_object->Dyn_OCB_control_data.roll_channel /*newton*/ , p   /*point*/ , d /*direction*/);

//   }
   

//handle pitch
   temp_thrust=pitch_thrust;

   if (source_object->Dyn_OCB_control_data.ORCS_extern_pitch!=0)
   {
     source_object->Dyn_OCB_control_data.pitch_channel+=(source_object->Dyn_OCB_control_data.ORCS_extern_pitch *real_frame_time_in_secs);

    }



   if (source_object->Dyn_OCB_control_data.command_pitch<0)	//left arrow
   {
    have_pitch=1;	
	source_object->Dyn_OCB_control_data.pitch_channel+=pitch_thrust *real_frame_time_in_secs;
   }
   else //no left input
   if (source_object->Dyn_OCB_control_data.command_pitch>0)	//left arrow
   {
    have_pitch=1;	
	source_object->Dyn_OCB_control_data.pitch_channel-=pitch_thrust *real_frame_time_in_secs;
   }
   
//two opposing thrusters
//   if ((source_object->Dyn_OCB_control_data.pitch_channel>0.01) || (source_object->Dyn_OCB_control_data.pitch_channel<-0.01) )
   if (have_pitch==1)   ORCS_rot_fuel_consumption+=fabs(temp_thrust /* *source_object->Dyn_OCB_control_data.yaw_channel */ );

//   {
//   ORCS_rot_fuel_consumption+=fabs(temp_thrust*source_object->Dyn_OCB_control_data.pitch_channel);

   p.x=0; p.y=0; p.z=1;	//position of force
   d.x=0; d.y=-1; d.z=0;	//direction of force
   apply_a_force(object, source_object->Dyn_OCB_control_data.pitch_channel,p /*oint*/ , d /*irection*/);   

   p.x=0; p.y=0; p.z=-1;	//position of force
   d.x=0; d.y=1; d.z=0;	//direction of force
   apply_a_force(object, source_object->Dyn_OCB_control_data.pitch_channel, p /*oint*/ , d /*irection*/);   
//   }


//end of new
}	//end of time multiplier<=5

//rotational dampers
#define CLAMP_RANGE 0.01
   if (source_object->Dyn_OCB_control_data.pitch_channel>-CLAMP_RANGE &&
   source_object->Dyn_OCB_control_data.pitch_channel<CLAMP_RANGE)
   source_object->Dyn_OCB_control_data.pitch_channel=0.0;

   if (source_object->Dyn_OCB_control_data.roll_channel>-CLAMP_RANGE &&
   source_object->Dyn_OCB_control_data.roll_channel<CLAMP_RANGE)
   source_object->Dyn_OCB_control_data.roll_channel=0.0;

   if (source_object->Dyn_OCB_control_data.yaw_channel>-CLAMP_RANGE &&
   source_object->Dyn_OCB_control_data.yaw_channel<CLAMP_RANGE)
   source_object->Dyn_OCB_control_data.yaw_channel=0.0;
   
   

if(source_object->Dyn_OCB_control_data.command_ORCS_damper&ORCS_DAMPER_ROT)
{
float temp_max;

  temp_max=fabs(source_object->Dyn_OCB_control_data.pitch_channel);
  if (temp_max>1) temp_max=1;	//do not allow overdrive
  
  
  
  pitch_bleed=fabs(source_object->Dyn_OCB_control_data.pitch_channel)*time_in_secs;	//real_frame_time_in_secs;

  if (pitch_bleed>fabs(source_object->Dyn_OCB_control_data.pitch_channel))
  pitch_bleed=fabs(source_object->Dyn_OCB_control_data.pitch_channel);

  if (source_object->Dyn_OCB_control_data.pitch_channel>0.01) 
  {	
    ORCS_rot_fuel_consumption+=temp_max*pitch_thrust;	//pitch_bleed;
    source_object->Dyn_OCB_control_data.pitch_channel-=pitch_bleed;
  }
  else
  if (source_object->Dyn_OCB_control_data.pitch_channel<-0.01) 
  {
   ORCS_rot_fuel_consumption+=temp_max*pitch_thrust;	//pitch_bleed;
  source_object->Dyn_OCB_control_data.pitch_channel+=pitch_bleed;

  }


  temp_max=fabs(source_object->Dyn_OCB_control_data.roll_channel);
  if (temp_max>1) temp_max=1;	//do not allow overdrive

  roll_bleed=fabs(source_object->Dyn_OCB_control_data.roll_channel)*time_in_secs;	//real_frame_time_in_secs;

  if (roll_bleed>fabs(source_object->Dyn_OCB_control_data.roll_channel))
  roll_bleed=fabs(source_object->Dyn_OCB_control_data.roll_channel);

 //center roll channel
 if (source_object->Dyn_OCB_control_data.roll_channel>0.01) 
 {
   ORCS_rot_fuel_consumption+=temp_max*roll_thrust;	//roll_bleed;
  source_object->Dyn_OCB_control_data.roll_channel-=roll_bleed;
  }
 else
 if (source_object->Dyn_OCB_control_data.roll_channel<-0.01) 
 {
   ORCS_rot_fuel_consumption+=temp_max*roll_thrust;	//roll_bleed;
 source_object->Dyn_OCB_control_data.roll_channel+=roll_bleed;
 }


  temp_max=fabs(source_object->Dyn_OCB_control_data.yaw_channel);
  if (temp_max>1) temp_max=1;	//do not allow overdrive

yaw_bleed=fabs(source_object->Dyn_OCB_control_data.yaw_channel)*time_in_secs;	//real_frame_time_in_secs;

  if (yaw_bleed>fabs(source_object->Dyn_OCB_control_data.yaw_channel))
  yaw_bleed=fabs(source_object->Dyn_OCB_control_data.yaw_channel);

//center yaw channel
if (source_object->Dyn_OCB_control_data.yaw_channel>0.01) 
 {
   ORCS_rot_fuel_consumption+=temp_max*yaw_thrust;	//yaw_bleed;
  source_object->Dyn_OCB_control_data.yaw_channel-=yaw_bleed;
 }
 else
 if (source_object->Dyn_OCB_control_data.yaw_channel<-0.01) 
 {
   ORCS_rot_fuel_consumption+=temp_max*yaw_thrust;	//yaw_bleed;
 source_object->Dyn_OCB_control_data.yaw_channel+=yaw_bleed;

 }
}	//end of rot damp
else
   ORCS_rot_fuel_consumption+=0;

if (get_main_camera_object()==object)
{
 
  camera_object_orcs_rot_fuel_consumption=STATIC_CAST_TO_INT(ORCS_rot_fuel_consumption*100000000*STANDARD_FUEL_USAGE_PER_NEWTONSECOND_IN_KGS);
  camera_object_orcs_lin_fuel_consumption=STATIC_CAST_TO_INT(ORCS_lin_fuel_consumption*100000000*STANDARD_FUEL_USAGE_PER_NEWTONSECOND_IN_KGS);

}
ORCS_fuel_consumption=ORCS_lin_fuel_consumption+ORCS_rot_fuel_consumption;
return ORCS_fuel_consumption*STANDARD_FUEL_USAGE_PER_NEWTONSECOND_IN_KGS;


}


// -----------------------------------------------------------------

module_private void set_all_thrusters_to_zero(int object)
{
ZObject *source_object;
extern	DynObjectsFixedSize *ocb_ptr;

source_object=&(*ocb_ptr).object_list[object];

source_object->Dyn_OCB_control_data.command_mthr1=0;
source_object->Dyn_OCB_control_data.command_mthr2=0;

source_object->Dyn_OCB_control_data.command_mrev1=0;
source_object->Dyn_OCB_control_data.command_mrev2=0;

source_object->Dyn_OCB_control_data.command_vthr1=0;
source_object->Dyn_OCB_control_data.command_vthr2=0;

source_object->Dyn_OCB_control_data.command_rvth1=0;
source_object->Dyn_OCB_control_data.command_rvth2=0;

source_object->Dyn_OCB_control_data.command_pitch=0;
source_object->Dyn_OCB_control_data.command_roll=0;
source_object->Dyn_OCB_control_data.command_yaw=0;

}


int is_engine1_available (int the_object)
{
sds_mount_data_type the_mount_info;

get_ship_mount_data (the_object, MTHR1V, &the_mount_info);
if((the_mount_info.the_bits&3)==2)
return 1;
else
return 0;
}

int is_engine2_available (int the_object)
{
sds_mount_data_type the_mount_info;

get_ship_mount_data (the_object, MTHR2V, &the_mount_info);
if((the_mount_info.the_bits&3)==2)
return 1;
else
return 0;
}

int is_engine2_fitted (int the_object)
{
sds_mount_data_type the_mount_info;

get_ship_mount_data (the_object, MTHR2V, &the_mount_info);
if((the_mount_info.the_bits&3)==0)
return 0;
else
return 1;	//is fitted
}

int is_rev1_available (int the_object)
{
sds_mount_data_type the_mount_info;

get_ship_mount_data (the_object, MREV1V, &the_mount_info);
if((the_mount_info.the_bits&3)==2)
return 1;
else
return 0;
}

int is_rev2_available (int the_object)
{
sds_mount_data_type the_mount_info;

get_ship_mount_data (the_object, MREV2V, &the_mount_info);
if((the_mount_info.the_bits&3)==2)
return 1;
else
return 0;
}

void run_engine_temperature(int the_object)
{
 float current_temp,current_flow;
 float onepc;
 ZObject *source_object;
 extern	DynObjectsFixedSize *ocb_ptr;

 if (the_object==get_main_camera_object())
 {

  //engine gains temperature at rate of 30C per max thrust/sec
  //engine loses heat at a rate of 1% per 15 C, so max temp is roughly 1500C
  current_flow=get_total_fuel_consumption(the_object);
  
  if (current_flow>0.0)
  {
    source_object=&(*ocb_ptr).object_list[the_object];
    current_temp=get_engine_temperature_in_C(the_object);
    onepc=current_temp/100.0;

    //add on 
    current_temp+=10.0*(current_flow/source_object->Dyn_OCB_control_data.main_thrust_consumption_per_second_in_KGS);
   //lose
    current_temp-=onepc*(current_temp/1500.0);
    if (current_temp<20.0) current_temp=20.0;
    set_engine_temperature_in_C(the_object,current_temp);
  }
 }
}


void run_laser_bay_temperature(int the_object)
{
 float current_temp,current_flow;
 ZObject *source_object;
 extern	DynObjectsFixedSize *ocb_ptr;


  //engine gains temperature at rate of 30C per max thrust/sec
  //laser loses heat at a rate of 1% per 15 C, so max temp is roughly 1500C
  current_flow=get_total_fuel_consumption(the_object);
  
    source_object=&(*ocb_ptr).object_list[the_object];
    current_temp=get_laser_bay_temperature_in_C(the_object);

    //add on 
    if (source_object->Dyn_OCB_control_data.laser_trigger_slot!=-1)
    {
      current_temp+=LASER_TEMP_RISE_RATE*time_in_secs;
    }
    else
    {
      current_temp-=LASER_TEMP_COOL_RATE*time_in_secs;
    }
   //lose
    //current_temp-=(current_temp/33.0)*(current_temp/1500.0);
    
    if (current_temp<20.0) current_temp=20.0;
    if (current_temp>MAX_LASER_TEMP) current_temp=MAX_LASER_TEMP;
    set_laser_bay_temperature_in_C(the_object,current_temp);
}

void run_hull_temperature(int the_object)
{
/*
The coldest place in the Solar System is Triton, a moon of Neptune, some 2,800 million miles from Earth. 
The spacecraft Voyager 2 was launched on 20 August 1977 and reached Neptune and Triton in August 1989 
after a fantastic journey through space. It was known that the atmosphere and surface of Triton contained 
nitrogen and methane and it had even been suggested that the surface might consist of oceans of liquid 
nitrogen (which boils at 77 K) with methane "icebergs"! This would have been spectacular but the reality 
was equally remarkable. At the temperature of Triton (-235deg.C or 38 K) and at low pressure, nitrogen 
freezes like water ice.

The temperature of intergalactic molecules of cyanogen (C2N2) has also been measured directly as 2.73 K, 
in thermal equilibrium with the cosmic background radiation (this was first discovered 50 years ago, 
but no explanation was then available). This is the lowest natural temperature but is available only in deep 
space. Lower temperatures require intelligence (or at least physicists).

Absolute zero is -273.15deg.C, reaching absolute zero is forbidden by the third law of thermodynamics.

Deep space is defined as 2.4AU (about 223 million miles) by the 2001 Mars obyssey mission.
1Km is 0.62 of a mile, so deep space as far as Zex is concerned is 372 million km's from a star.

*/
 
 double temperature;
 _3D position_in_m;
 _3D sun_pos_in_m;
 extern int sun_slot1, sun_slot2;
 float sun1_temp;
 float at_temperature=-271;
 extern int ship_buffet;
 
 if (the_object==get_main_camera_object())
 {
   //we only do this for the camera object
   //if buffet then we do that, which will top out at about +3000 degrees C (Death of the ship)
   
   //then we have a function which varies temperature as a result of distance to nearest star
   //which starts at 5000 degrees C at the surface and goes down to about -270
   //We'll probably use a square law thingy
   
   if (ship_buffet!=0)
   {
    //calc temp from buffet
	at_temperature=ship_buffet/5;
   }
   
   if (sun_slot1!=-1)
   {
	 get_position_in_m(sun_slot1,&sun_pos_in_m);
   }
   else
   {
 	 set_wanted_temperature_in_C(the_object,at_temperature); //No Sun!!
	return;
   }
   
   
   get_position_in_m(the_object,&position_in_m);
   temperature=calc_temperature(&position_in_m, &sun_pos_in_m);

   
  sun1_temp=temperature;

   if (sun_slot2!=-1)
   {
	 get_position_in_m(sun_slot2,&sun_pos_in_m);
     temperature=calc_temperature(&position_in_m, &sun_pos_in_m);
	 
	 if (temperature>sun1_temp) //we're going with sun 2
	 {
       if (at_temperature>temperature)
       set_wanted_temperature_in_C(the_object,at_temperature);
       else	
       set_wanted_temperature_in_C(the_object,temperature); 
	 }
	 else //we're going with sun1
	 {
       if (at_temperature>sun1_temp)
       set_wanted_temperature_in_C(the_object,at_temperature);
       else	
       set_wanted_temperature_in_C(the_object,sun1_temp); 	 
	 }
   }
   else //no sun2
   {
    if (at_temperature>sun1_temp)
    set_wanted_temperature_in_C(the_object,at_temperature); 
    else	
    set_wanted_temperature_in_C(the_object,sun1_temp); //No Sun!!
	return;
   }
   
 }
}






void set_wanted_temperature_in_C(int the_object,float wanted_temperature)
{
float temperature,diff;

//This somehwat simplified function takes care of the fact that a mass can't instantly change its temperature

    temperature= get_hull_temperature_in_C(the_object);
	//is waneted temp lower than current?
	if (wanted_temperature<temperature)
	{
	 diff=(temperature-wanted_temperature)/256;
//	 if (diff>30) diff=30;
	 temperature-=diff;
	}
	else //wanted > current
	{
	 diff=(wanted_temperature-temperature)/256;
//	 if (diff>30) diff=30;
	 temperature+=diff;
	}
    set_hull_temperature_in_C(the_object,temperature);

}



float calc_temperature(_3D* position1_in_m, _3D* sun_pos_in_m)
{
   float temperature;
   double dist;
   
   position1_in_m->x-=sun_pos_in_m->x;   position1_in_m->y-=sun_pos_in_m->y;  position1_in_m->z-=sun_pos_in_m->z;
//   position_in_m.x=fabs(poisioitn_in_m.x);
//   position_in_m.y=fabs(poisioitn_in_m.y);
//   position_in_m.z=fabs(poisioitn_in_m.z);
   
   dist=position1_in_m->x*position1_in_m->x;
   dist+=position1_in_m->y*position1_in_m->y;
   dist+=position1_in_m->z*position1_in_m->z;
   dist=fabs(dist);
   
   dist=sqrt(dist);  //distance in metres to 0,0,0
   dist/=1000;	//km's
   if (dist>372e6) dist=372e6;
   //we have two bands:
   //1. From the sun to about 10 million kilometres (mercury varies 27 to 41 million km's
   //2. From 10 million kms out. Here the temeperature is (in zex) about 0 degrees C
   if (dist<10e6) //range of 0 to 5000 degrees C
   {
    temperature=(5000-((dist/10e6)*5000));
   }
   else //greater than 10e6 - range 0 to -270 degrees C
   {
    //now temperature is ((distance/372e6)*50270)-270
    temperature=(270-((dist/362e6)*270))-270;
   }

return temperature;
}
