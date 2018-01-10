// Zex 2 - 3D Space Trading Game with in-cockpit view 
// Copyright (C) 1998-2018  Stuart Ball and Robert Probin
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

// *************************************MODULE HEADER*********************************
// * FILENAME           : Autopilot.cpp
// * MODULE TITLE       : Autopilot
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 08:47:17 on 25th August 2001
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
$Log: Autopilot.cpp,v $
Revision 1.3  2003/09/28 17:30:08  robp
Changed files from .c to .cpp and removed spaces out of a couple of filenames.

Revision 1.2  2003/09/27 20:41:53  robp
Fixed float->int warnings.

Revision 1.1.1.1  2003/09/05 22:36:19  stu_c
First Imported.

Revision 1.20  2002/09/08 02:10:19  stu_c
Precompiled header adjustments

Revision 1.19  2002/08/11 22:06:30  stu_c
behaviour_aggressive started, falcons from motherships.

Revision 1.18  2002/05/31 23:53:22  stu
Removed all warnings from the code

Revision 1.17  2002/04/20 18:37:10  stu
Furhter work for AP and control at time>1

Revision 1.16  2002/04/16 22:24:43  stu
Nuetral motherships NPC generation

Revision 1.15  2002/04/14 15:58:26  stu
Further work

Revision 1.14  2002/03/19 23:19:04  stu
Work so npc's don't affect time/messages - get_main_camera_object etc.

Revision 1.13  2002/02/24 03:40:51  stu
(Some) compiler warnings cleaned up

Revision 1.12  2002/02/21 22:46:12  stu
Removed old scene code from docked, now displays outside scene

Revision 1.11  2002/02/17 06:18:45  stu
Added radar targetting

Revision 1.10  2002/02/16 19:00:41  stu
Radar range decrease key and rationalisation of dlp colours

Revision 1.9  2002/01/28 23:24:27  stu
TOok out a fade_out_music

Revision 1.8  2002/01/27 18:08:02  stu
Development of AP for high time multipliers

Revision 1.7  2001/12/29 01:11:26  stu
Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.

Revision 1.6  2001/12/14 19:43:03  stu
Docking

Revision 1.5  2001/11/24 19:27:56  stu
RA_line_len in place of sqrt

Revision 1.4  2001/11/05 19:58:35  stu
More OSX work

Revision 1.3  2001/11/02 19:32:10  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.2  2001/10/27 22:38:45  stu
zplatform included

Revision 1.1  2001/10/22 21:29:14  rob
Carbon warning changes

Revision 1.0.0.1  2001/10/17 20:46:07  rob
First Imported.

Revision 1.7  2001/10/04 22:48:02  stu
LAst set of changes before 2.2

Revision 1.6  2001/09/26 23:02:33  stu
Dunno? Lurkers.

Revision 1.5  2001/09/23 18:58:43  stu
AP work, improved number print, engines.

Revision 1.4  2001/09/18 20:41:04  stu
More Sim/ hud numbers print

Revision 1.3  2001/09/10 21:43:32  stu
100901
More Sim, AP broken.

Revision 1.2  2001/09/04 21:47:59  stu
ADded BREAK for combat AP so we def, really, don't hit hte target.

Revision 1.1  2001/09/04 20:32:03  stu
Initial issue

*/

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  
  #include "3d_structures_def.h"
  #include "access_ocbs.h"
  #include "real_time.h"
  #include "processor.h"
#endif //pch


#include "matrix_primitives.h"
#include "sds.h"
#include "physics.h"
#include "autopilot.h"
#include "Zsound.h"
#include "dlp.h"
#include "objects_controls.h"
#include "camera_control.h"
#include "objects_misc.h"


// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

extern double real_frame_time_in_secs;
extern DynObjectsFixedSize *ocb_ptr;
double cos_to_target;
double AP_targ_x;
double rot_to_target,s_to_target;
vector temp_vector,temp_vector1, norm_vector,v_vector;
int debug_orcs_pitch, debug_orcs_roll, debug_orcs_yaw;
float debug_brake_time,debug_calcT;
vector debug_target_vector,debug_calc_targ_vector,debug_heading_vector;

int debug_distance_to_target;

int camera_object_quat_x,camera_object_quat_y,camera_object_quat_z,camera_object_quat_w;
int camera_quat_x,camera_quat_y,camera_quat_z,camera_quat_w;

extern int time_multiplier;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *


module_private void run_AP_steer(int the_object);
module_private void run_AP_thrust(int the_object);
module_private void ap_calc_dampers(int the_object);
void run_AP_calcs(int the_object);
void hyper_transport(int object);


int time_to_targ;
int calc_time_to_targ;

void run_flight_computations(int the_object)
{
//very simple, calculate relative and absolute velocities. Used by AP et al.
	double temp;
	int sign;
	//vector temp_vect;
    double dist_x,dist_y,dist_z;
    float distance_to_target=0;
    ZObject *source_object;
       //double current_x,current_y,current_z;
       //double targ_x,targ_y,targ_z;

    source_object=&(*ocb_ptr).object_list[the_object];
    
    
    if(get_main_camera_object()==the_object)
    {
      camera_object_quat_x=STATIC_CAST_TO_INT(source_object->Qor.x*10000000);
      camera_object_quat_y=STATIC_CAST_TO_INT(source_object->Qor.y*10000000);
      camera_object_quat_z=STATIC_CAST_TO_INT(source_object->Qor.z*10000000);
      camera_object_quat_w=STATIC_CAST_TO_INT(source_object->Qor.w*10000000);

      camera_quat_x=STATIC_CAST_TO_INT((*ocb_ptr).object_list[2].Qor.x*10000000);
      camera_quat_y=STATIC_CAST_TO_INT((*ocb_ptr).object_list[2].Qor.y*10000000);
      camera_quat_z=STATIC_CAST_TO_INT((*ocb_ptr).object_list[2].Qor.z*10000000);
      camera_quat_w=STATIC_CAST_TO_INT((*ocb_ptr).object_list[2].Qor.w*10000000);

    
    }
    
    
    sign=1;
    	
	//if object targetted, get distance and compare to last_distance. If less than, sign = - else +
    if (get_object_targetted(the_object)!=-1)	    //check something is targetted
    {
    //if the PA has calculated a projected position for the object then we need ot use that
//       if(source_object->Dyn_OCB_control_data.AP_flags&AP_CALCD_FLAG)
//       {
       
//        current_x=source_object->Oworldx;
//        current_y=source_object->Oworldy;
//        current_z=source_object->Oworldz;
//        
//        targ_x=source_object->Dyn_OCB_control_data.AP_target_end_position_in_m.x/ZEX_UNIT_SIZE_IN_METERS;
//        targ_y=source_object->Dyn_OCB_control_data.AP_target_end_position_in_m.y/ZEX_UNIT_SIZE_IN_METERS;
//        targ_z=source_object->Dyn_OCB_control_data.AP_target_end_position_in_m.z/ZEX_UNIT_SIZE_IN_METERS;
//        
//        distance_to_target=get_distance_to_from_point(current_x,current_y,current_z,targ_x,targ_y,targ_z);
//       }
//       else
//       {
        distance_to_target=get_distance_to_from(get_object_targetted(the_object),the_object);
        debug_distance_to_target=STATIC_CAST_TO_INT(distance_to_target);

//       }
       if(source_object->Dyn_OCB_control_data.last_distance_to_target < distance_to_target) sign =-1;
 
       source_object->Dyn_OCB_control_data.last_distance_to_target=distance_to_target;
   }
   dist_x=source_object->rel_velocity_vector.x;
   dist_y=source_object->rel_velocity_vector.y;
   dist_z=source_object->rel_velocity_vector.z;	//this is in !ZEX UNITS PER SECOND! so 
								//needs no decoupling for time!!!

    temp=dist_x*dist_x+dist_y*dist_y+dist_z*dist_z;
    temp=sqrt(temp);
    
    temp*=sign;
    	    
//	if(sign<0) temp=-temp;

	source_object->Dyn_OCB_control_data.current_relative_velocity=temp*(ZEX_UNIT_SIZE_IN_METERS);	//convert to metres
//calc time_to_target_in_secs_as_crow_flies. d/v
       	source_object->Dyn_OCB_control_data.time_to_target_in_secs_as_crow_flies
       	=
       	(distance_to_target*ZEX_UNIT_SIZE_IN_METERS)/source_object->Dyn_OCB_control_data.current_relative_velocity;


        time_to_targ=STATIC_CAST_TO_INT(source_object->Dyn_OCB_control_data.time_to_target_in_secs_as_crow_flies);
	    
		if (the_object==get_main_camera_object() &&
		    source_object->Dyn_OCB_control_data.command_autopilot_engaged>AP_STEER	//AP is on, 1 is steer
		)
		{
		 if (time_to_targ<60)
		 {
		  set_time_multiplier_to_2();
		 }
         else
         if (time_to_targ<120)
		 {
		  set_time_multiplier_to_5();
		 }
         else
         if (time_to_targ<180)
		 {
		  set_time_multiplier_to_10();
		 }
		}
		
	dist_x=source_object->abs_velocity_vector.x;
	dist_y=source_object->abs_velocity_vector.y;
	dist_z=source_object->abs_velocity_vector.z;	//this is in !ZEX UNITS PER SECOND! so 
								//needs no decoupling for time!!!

    temp=dist_x*dist_x+dist_y*dist_y+dist_z*dist_z;
    temp=sqrt(temp);


	source_object->Dyn_OCB_control_data.current_abs_velocity=temp*(ZEX_UNIT_SIZE_IN_METERS);	//convert to metres

}

/*
How does the autopilot work?
This file contains an autopilot (AP) for objects in Zex.

For the AP to work, an object must be targetted. When an object is targetted
the AP has to work out a predicted course for the object. It also has to calculate
how long it is going to take to fly to the object. From this information it can predict
where it needs to steer the object to hit the object at that time.

All data is local to the object; i.e. in the OCB
The AP controls the object via ORCS_thrust_up, ORCS _extern_roll etc.


HOW IT WORKS:
Lets assume the controller (player or NPC) has just selected the AP
HEADING CALCULATION:
1. Get current target pos in metres.
2. Set timer_1 for 1 second.
3. When timer expires get target position. Calculate a vector for the target.
4. Calculate how long at max thrust it would take to get to the target in it's current position (needs radar)
5. Extrapolate targets vector over that time to get its end position. This gives us a rough heading.
6. Get ship heading in that direction.

THRUST CALCULATION
Calculate required velocity as follows:
1. Get distance to target (requires radar)
2. Calculate time to target at current velocity (time_to_target)
3. Calculate time to stop. (stop_time)
4. If stop_time>time_to_target accelerate else decelerate.



*/

void run_AP(int object)
{

ZObject *source_object;

source_object=&(*ocb_ptr).object_list[object];

if(source_object->Dyn_OCB_control_data.power_flags==(AC_GOOD+DC_GOOD))	//check power
  {
   	if (get_object_targetted(object)!=-1)	//check something is targetted
  	   {
  	     if(source_object->Dyn_OCB_control_data.command_autopilot_engaged>AP_STEER_CUE)	//AP is on, 1 is steer
  	     {
  	       run_AP_calcs(object); 	          
  	       run_AP_steer(object); 	          
  	       run_AP_thrust(object);
               hyper_transport(object);
         }
       }
  }

}

// ------------------ALL checks OK, AP is engaged and available


//This function drags the object really quickly if hypertransport mode
void hyper_transport(int object)
{
ZObject *source_object=&(*ocb_ptr).object_list[object];
int object_targetted=source_object->object_targetted;
int ap_mode=source_object->Dyn_OCB_control_data.command_autopilot_engaged;	//AP is on, 2 is steer transport, 3 is steer
ZObject *target_object=&(*ocb_ptr).object_list[object_targetted];
double target_radius, distance_to;
float accel=source_object->Dyn_OCB_control_data.ht_accel;

              //now calculate stop point
              distance_to=get_distance_to_from(object_targetted ,object);
              target_radius=target_object->radius;

              if ((distance_to>target_radius*5) && (target_radius>50000))
              {
                //now if the player object, then we create a drag AP object which will drag the player at very high speed
               //towards the object
               if ((object==get_main_camera_object()) && (ap_mode==AP_HYPERTRANSPORT)
               && source_object->Dyn_OCB_control_data.ap_ht_engage_timer<0.1)
               {
                 vector target_v;
                 if (check_for_object_near()==-1)
                 {
                   //calc vector to target
                   get_vector_to(&target_v, object_targetted ,object);   
                   //add on some big numbers
                   source_object->Oworldx+=target_v.x*distance_to*0.65*time_in_secs; //0.005 max
                   source_object->Oworldy+=target_v.y*distance_to*0.65*time_in_secs;
                   source_object->Oworldz+=target_v.z*distance_to*0.65*time_in_secs;
                   if (accel<0.150) 
                   {
                     accel+=0.000001;
                     source_object->Dyn_OCB_control_data.ht_accel=accel;
                   }
                 }
                 else
                 {
                  if (object==get_main_camera_object())
                  {

                   add_to_text_display("AP> Hypertransport disengaged - mass detected.",DLP_GREEN);  
                   add_to_text_display("AP> Changing to steer cue mode.",DLP_GREEN);  
                  } 
                   engage_ap(object,AP_STEER_CUE);
                 }
               }
              }
              else //not within the HT parameter envelope
              {
               if (ap_mode==AP_HYPERTRANSPORT)
               {
                  if (object==get_main_camera_object())
                  {
                   add_to_text_display("AP> Hypertransport disengaged - too close.",DLP_GREEN);  
                   add_to_text_display("AP> Changing to steer cue.",DLP_GREEN);  
                  }
                 engage_ap(object,AP_STEER_CUE);
               }
              }
}


void run_AP_calcs(int the_object)
{
ZObject *source_object;
_3D target_position,our_current_position;
int targetted_object;
//int loop;
//float brake_distance;
//double distance_to_target_in_m;
int ap_mode;
float max_thrust, recalculate_time;
vector target_vv;
sds_mount_data_type the_mount_info;

double f,m;
//double a; //Can you guess what it is yet?
double fr;	//reverse thrust available
//double v,t,t1;   // these are, uh, velocity and time


source_object=&(*ocb_ptr).object_list[the_object];

//calculate some vars
ap_mode=source_object->Dyn_OCB_control_data.command_autopilot_engaged;	//AP is on, 2 is steer transport, 3 is steer

 recalculate_time=3;	//secs
 max_thrust=1;
 
if (ap_mode==AP_COMBAT)
{
 recalculate_time=0.5;	//secs
 max_thrust=1;
}
else
if ((ap_mode==AP_TRANSPORT) || (ap_mode==AP_HYPERTRANSPORT))
{
float ht_engage_time=source_object->Dyn_OCB_control_data.ap_ht_engage_timer;
 
 recalculate_time=6;	//secs
 max_thrust=0.5;

 if (ht_engage_time>0.1 && ap_mode==AP_HYPERTRANSPORT)
 {
   ht_engage_time-=time_in_secs;
   if (ht_engage_time<0.1)
   {
   extern int flash_screen;

                  if (the_object==get_main_camera_object())
                  {
	             add_to_text_display("AP> Hypertransport Engaged.",DLP_GREEN);  
                     play_zsound(explosion4, sound_high_pri, c1_chan1, sound_vol_7);
                  }
                   flash_screen=15;
                   decrement_ship_attrib_data(the_object, HTRNV);
   }
   else
   {
    sds_attrib_data_type my_attrib;
    get_ship_attrib_data(the_object, HTRNV, &my_attrib);
    if (my_attrib.the_value==0)
    {
              if (the_object==get_main_camera_object())
              {

	           add_to_text_display("AP> Hypertransport not available. No HT",DLP_GREEN);  
	           add_to_text_display("AP> Shells. Switching AP to Transport.",DLP_GREEN);  
              }
              engage_ap(the_object,AP_TRANSPORT);

    }
   }
 }
 
 source_object->Dyn_OCB_control_data.ap_ht_engage_timer=ht_engage_time;
 
}

if (ap_mode<AP_TRANSPORT)
{
      get_position_in_m(source_object->object_targetted,&target_position);	//do this now for steering
      source_object->Dyn_OCB_control_data.AP_target_end_position_in_m=target_position;
}

//alright, this is AP MKIII
//The problem has always been how do we know where the object will be...
//MKIII cures the problem (I think) by using half power when calculating initial flight time
//the using that time to predict where the object will be.
//we then use full power to calculate the time and again predict where the object will be
//(As we know we can hit it along that trajectory)

     if ((source_object->Dyn_OCB_control_data.AP_flags&AP_CALCD_FLAG)==0)
     {
      get_position_in_m(source_object->object_targetted,&target_position);	//do this now for steering
      source_object->Dyn_OCB_control_data.AP_target_end_position_in_m=target_position;
       
       get_position_in_m( the_object, &our_current_position);

       targetted_object=source_object->object_targetted;
       target_vv=(*ocb_ptr).object_list[targetted_object].abs_velocity_vector;	//in zex units per second
       //get total reverse thrust
         get_ship_mount_data (the_object, MREV1V, &the_mount_info);
         fr=the_mount_info.the_value*max_thrust;
         get_ship_mount_data (the_object, MREV2V, &the_mount_info);
         fr+=the_mount_info.the_value*max_thrust;
       //get total main thrust
         get_ship_mount_data (the_object, MTHR1V, &the_mount_info);
         f=the_mount_info.the_value*max_thrust;
         get_ship_mount_data (the_object, MTHR2V, &the_mount_info);
         f+=the_mount_info.the_value*max_thrust;
       // get mass of object
        m=get_mass_in_kg(the_object);
       
////1/2 power calcs
//        distance_to_target_in_m=get_distance_apart(target_position,our_current_position);	//as both of these are in m, so is the result
//       
//      
//        //calc time to target inc braking time
//      //f=ma, a=f/m, f=main engines (Newtons), m is mass (KGs), a is acceleration - m/s/s
// 
//         //calc max accel (in m/s/s)
//        a=f/m;
//        a/=2;
//         //this is the friggy part, how to calc how long to get to the object
//         //OK, lets calculate the velocity when we get there -- v=sqr(2ad)
//         //then calculate time from v=at, so t=v/a. Taadaah.
//         //Test 1. We have 1kg being pushed by 1N. We want to know how long it takes to travel 20 metres.
//         //v=sqr(2*1*20)=6.32m/s at 20m
//         //t=v/a. =6.32/1 = 6.32s
//         //
//         //test 2. We have 1kg being pushed by 2N. We want to know how long it takes to travel 20 metres.
//         //v=sqr(2*2*20)=8.94 m/s at 20m
//         //t=v/a. =8.94/2 = 4.47s
//         //
//         //test 3. We have 1kg being pushed by 3N. We want to know how long it takes to travel 20 metres.
//         //v=sqr(2*3*20)=10.95 m/s at 20m
//         //t=v/a. =10.95/3 = 3.65s
//         //this all seems about right
//         //See http://www.faqs.org/faqs/space/math/ for more...
//         v=sqrt(2*a*distance_to_target_in_m);
//         //now this v assumes our current velocity is 0
// 	//if our current v is <0 then the time will increase, if >0 then 
// 	if (source_object->Dyn_OCB_control_data.current_relative_velocity<0)
// 	v-=source_object->Dyn_OCB_control_data.current_relative_velocity;
//         t=v/a;	//in seconds to reach the target in it's current location, 
//         //however...
//         //if our reverse thrusters were equal in power to our main thrusters then we could just multiply t by 2
//         //assume our main thrusters are 20000N and our reverse thrusters are 15000N then we want to multiply by
//         //2*(main/reverse). I'm not sure this is correct
//
////         t=t*(2*(f/fr));         
//         t=t*2;
//         //now t is my best guess at how long it'll take us to get there.
//         //find out where target will be at the end of this time and store it in target_position for next iteration
////         target_position.x+=target_vv.x*t*ZEX_UNIT_SIZE_IN_METERS;
////         target_position.y+=target_vv.y*t*ZEX_UNIT_SIZE_IN_METERS;
////         target_position.z+=target_vv.z*t*ZEX_UNIT_SIZE_IN_METERS;
                  

//full power calcs, we now know we can hit the target along this trajectory as we managed it on half power
//        distance_to_target_in_m=get_distance_apart(target_position,our_current_position);	//as both of these are in m, so is the result
      
      
        //calc time to target inc braking time
      //f=ma, a=f/m, f=main engines (Newtons), m is mass (KGs), a is acceleration - m/s/s
 
         //calc max accel (in m/s/s)
//        a=f/m;
         //this is the friggy part, how to calc how long to get to the object
         //OK, lets calculate the velocity when we get there -- v=sqr(2ad)
         //then calculate time from v=at, so t=v/a. Taadaah.
         //Test 1. We have 1kg being pushed by 1N. We want to know how long it takes to travel 20 metres.
         //v=sqr(2*1*20)=6.32m/s at 20m
         //t=v/a. =6.32/1 = 6.32s
         //
         //test 2. We have 1kg being pushed by 2N. We want to know how long it takes to travel 20 metres.
         //v=sqr(2*2*20)=8.94 m/s at 20m
         //t=v/a. =8.94/2 = 4.47s
         //
         //test 3. We have 1kg being pushed by 3N. We want to know how long it takes to travel 20 metres.
         //v=sqr(2*3*20)=10.95 m/s at 20m
         //t=v/a. =10.95/3 = 3.65s
         //this all seems about right
         //See http://www.faqs.org/faqs/space/math/ for more...
//         v=sqrt(2*a*distance_to_target_in_m);
         //now this v assumes our current velocity is 0
 	 //if our current v is <0 then the time will increase, if >0 then 
// 	if (source_object->Dyn_OCB_control_data.current_relative_velocity<0)
// 	v-=source_object->Dyn_OCB_control_data.current_relative_velocity;
//         t=v/a;	//in seconds to reach the target in it's current location, 
         //however...
         //if our reverse thrusters were equal in power to our main thrusters then we could just multiply t by 2
         //assume our main thrusters are 20000N and our reverse thrusters are 15000N then we want to multiply by
         //2*(main/reverse). I'm not sure this is correct

//         t=t*(2*(f/fr));         
//         t=t*2;
         //now t is my best guess at how long it'll take us to get there.
         //find out where target will be at the end of this time and store it in target_position for next iteration
//         target_position.x+=target_vv.x*t*ZEX_UNIT_SIZE_IN_METERS;
//         target_position.y+=target_vv.y*t*ZEX_UNIT_SIZE_IN_METERS;
//         target_position.z+=target_vv.z*t*ZEX_UNIT_SIZE_IN_METERS;



//        }	//end of loop - have we looped 4 times?
      //yes, so we have a good approx of where the target will end up and how long it's going to take us to get there
      //calc brake distance
//      distance_to_target_in_m=get_distance_apart(target_position,our_current_position);	//as both of these are in m, so is the result

//      brake_distance=distance_to_target_in_m/2;	//(2*(f/fr));	//asumming equal brakes
//      brake_distance=distance_to_target_in_m-brake_distance;	  //where we should start our braking
//      source_object->Dyn_OCB_control_data.calculated_time_to_target_in_secs
//      =source_object->Dyn_OCB_control_data.time_to_target_in_secs_as_crow_flies;

//      source_object->Dyn_OCB_control_data.AP_brake_time=t/2;   
//      source_object->Dyn_OCB_control_data.AP_brake_distance=distance_to_target_in_m/2;   
      
//      debug_brake_time=source_object->Dyn_OCB_control_data.AP_brake_time;
      
      source_object->Dyn_OCB_control_data.AP_flags|=AP_CALCD_FLAG;   
      
      source_object->Dyn_OCB_control_data.AP_target_end_position_in_m=target_position;
     }	//end of CALCD_FLAG not being set

}













module_private void run_AP_steer(int the_object)
{
double diff_x,diff_y,diff_z;	//heading difference components
//vector targ_vel_vector,target_velocity_vector;
vector our_current_position;

vector target_vector,heading_vector,s_vector;
float yaw_thrust, roll_thrust, pitch_thrust;
int did_course;

_3D target_position;
//vector target_vv;


ZObject *source_object;
//sds_mount_data_type the_mount_info;
int ap_mode;
float rot_divider,max_thrust;
float recalculate_time,lockout_time;

rot_divider=1;

source_object=&(*ocb_ptr).object_list[the_object];



//debug line - remove
     get_position_in_m(source_object->object_targetted,&target_position);	//do this now for steering
      source_object->Dyn_OCB_control_data.AP_target_end_position_in_m=target_position;
//end debug

//calculate some vars
ap_mode=source_object->Dyn_OCB_control_data.command_autopilot_engaged;	//AP is on, 2 is steer transport, 3 is steer

 recalculate_time=3;	//secs
 lockout_time=5;	//default
if (ap_mode==AP_COMBAT)
{
 recalculate_time=0.5;	//secs
 max_thrust=1;
 rot_divider=1;
 lockout_time=1;
}
else
if ((ap_mode==AP_TRANSPORT) || (ap_mode==AP_HYPERTRANSPORT))
{
 recalculate_time=3;	//secs
 max_thrust=0.8;
 rot_divider=1.2;
 
}
else
if (ap_mode==AP_STEER)
{
 max_thrust=0.5;
 rot_divider=1;
}

if (ap_mode>=AP_STEER)
source_object->Dyn_OCB_control_data.command_ORCS_damper|=ORCS_DAMPER_ROT;

get_position_in_m( the_object, &our_current_position);
//get_position_in_m(source_object->object_targetted,&target_vector);


source_object->Dyn_OCB_control_data.ORCS_extern_yaw=0;
source_object->Dyn_OCB_control_data.ORCS_extern_pitch=0;
source_object->Dyn_OCB_control_data.ORCS_extern_roll=0;

//if(source_object->Dyn_OCB_control_data.calculated_time_to_target_in_secs>0)
//{
//      source_object->Dyn_OCB_control_data.calculated_time_to_target_in_secs-=time_in_secs;
//      debug_calcT=source_object->Dyn_OCB_control_data.calculated_time_to_target_in_secs;
//}
//steering commands
      debug_target_vector=source_object->Dyn_OCB_control_data.AP_target_end_position_in_m;

     //get vector to target
        Vsub (&source_object->Dyn_OCB_control_data.AP_target_end_position_in_m, &our_current_position,  &target_vector);
        Vnormalise(&target_vector);
        
        debug_calc_targ_vector=target_vector;
      //get our heading vector
        heading_vector= source_object->n_translated;
        debug_heading_vector=heading_vector;
        
        v_vector = source_object->v_translated;
        s_vector = source_object->s_translated;
      // now, check if vectors are aligned
      diff_x=heading_vector.x-target_vector.x;
      diff_y=heading_vector.y-target_vector.y;
      diff_z=heading_vector.z-target_vector.z;

/*
      if (time_multiplier>10)
	  {
       yaw_thrust=source_object->Dyn_OCB_control_data.rotational_yaw*3; ///(time_multiplier/3);
       pitch_thrust=source_object->Dyn_OCB_control_data.rotational_pitch*3; ///(time_multiplier/3);
       roll_thrust=source_object->Dyn_OCB_control_data.rotational_roll*3; ///(time_multiplier/3);
      }
	  else
*/
	  {
       yaw_thrust=source_object->Dyn_OCB_control_data.rotational_yaw;
       pitch_thrust=source_object->Dyn_OCB_control_data.rotational_pitch;
       roll_thrust=source_object->Dyn_OCB_control_data.rotational_roll;
      }
	  
      //now, we need to pitch up if the target is behind us.
      //To find out if the target is behind us we need to find the angle between the two vectors.
      //this is simply the arccos of the dot product, although we're probably quite happy with the cosine so don't need the arcos bit.
      cos_to_target=Vdotproduct(&heading_vector, &target_vector);
      if(cos_to_target<0) 
      {
        source_object->Dyn_OCB_control_data.ORCS_extern_pitch=-pitch_thrust;
        goto end_steer;	//just go for max pitch and good bye.
      }

//      source_object->Dyn_OCB_control_data.AP_flags&=~AP_LOCKED; //clear locked flag, can't apply thrust until this is true


      //now, the object is in front of us so we aim to get rot to target=1 or -1.
      rot_to_target=Vdotproduct(&v_vector, &target_vector);	//for pitch
      s_to_target=Vdotproduct(&s_vector, &target_vector); //for roll
      
	  did_course=FALSE;
      //course adjustment
      //do yaw
      if(s_to_target>0.07)
      {
      source_object->Dyn_OCB_control_data.AP_thr_lock_out_timer=lockout_time;	//set thrust lockout timer
      source_object->Dyn_OCB_control_data.ORCS_extern_yaw=(-yaw_thrust*(fabs(s_to_target)))/rot_divider;
      did_course=TRUE;
	  }
      else
      if(s_to_target<-0.07)      
      {
      source_object->Dyn_OCB_control_data.AP_thr_lock_out_timer=lockout_time;	    //set thrust lockout timer
      source_object->Dyn_OCB_control_data.ORCS_extern_yaw=(yaw_thrust*(fabs(s_to_target)))/rot_divider;
      did_course=TRUE;
	  }


      //do roll
      if(s_to_target<-0.07) 
      {
      source_object->Dyn_OCB_control_data.AP_thr_lock_out_timer=lockout_time;	    //set thrust lockout timer
      if(rot_to_target>0)
      source_object->Dyn_OCB_control_data.ORCS_extern_roll=(-roll_thrust*(fabs(s_to_target)))/rot_divider;
      else
      source_object->Dyn_OCB_control_data.ORCS_extern_roll=(roll_thrust*(fabs(s_to_target)))/rot_divider;
      
	  did_course=TRUE;
	  source_object->Dyn_OCB_control_data.AP_thr_lock_out_timer=lockout_time;	    //set thrust lockout timer
      
      }
      else
      if(s_to_target>0.07) 
      {
      source_object->Dyn_OCB_control_data.AP_thr_lock_out_timer=lockout_time;	    //set thrust lockout timer
      if(rot_to_target>0)      
      source_object->Dyn_OCB_control_data.ORCS_extern_roll=(roll_thrust*(fabs(s_to_target)))/rot_divider;
      else
      source_object->Dyn_OCB_control_data.ORCS_extern_roll=(-roll_thrust*(fabs(s_to_target)))/rot_divider;
      
	  did_course=TRUE;
	  source_object->Dyn_OCB_control_data.AP_thr_lock_out_timer=lockout_time;	    //set thrust lockout timer
      
      }
//   
//      else   
      //do pitch if we are roughly aligned in roll
      if (rot_to_target>0.07)
        {
         source_object->Dyn_OCB_control_data.AP_thr_lock_out_timer=lockout_time;	//set thrust lockout timer
         source_object->Dyn_OCB_control_data.ORCS_extern_pitch=(pitch_thrust*(fabs(rot_to_target)))/rot_divider;
        did_course=TRUE;
        }
      else
      if (rot_to_target<-0.07)
        {
         source_object->Dyn_OCB_control_data.AP_thr_lock_out_timer=lockout_time;	//set thrust lockout timer
         source_object->Dyn_OCB_control_data.ORCS_extern_pitch=(-pitch_thrust*(fabs(rot_to_target)))/rot_divider;
         did_course=TRUE;
		  }
      
            
      
      //fine adjustment
      //do yaw
	  if (!did_course)
	  {
       if(s_to_target>0.005)
       {
       source_object->Dyn_OCB_control_data.ORCS_extern_yaw=(-yaw_thrust*(fabs(s_to_target)))/(rot_divider*2);
       }
       else
       if(s_to_target<-0.005)      
       {
       source_object->Dyn_OCB_control_data.ORCS_extern_yaw=(yaw_thrust*(fabs(s_to_target)))/(rot_divider*2);
       }


       //do pitch if we are roughly aligned in roll
       if (rot_to_target>0.005)
        {
         source_object->Dyn_OCB_control_data.ORCS_extern_pitch=(pitch_thrust*(fabs(rot_to_target)))/(rot_divider*2);
        
        }
       else
       if (rot_to_target<-0.005)
        {
         source_object->Dyn_OCB_control_data.ORCS_extern_pitch=(-pitch_thrust*( fabs(rot_to_target) ))/(rot_divider*2);
        }
      
      } //end of !did_course
	  
      if(source_object->Dyn_OCB_control_data.AP_thr_lock_out_timer<1.0)
      {

		  #if AP_DEBUG_MESSAGES==1 //can only printf to OSX syscon

	     if (source_object->Dyn_OCB_control_data.AP_flags&AP_THRUST_LOCK_OUT_FLAG)
		 {
		  #if ZEX_PROJECT_BUILDER==1 //can only printf to OSX syscon
          fprintf (stderr, "AP: Object %i is locked onto its target \n",the_object); 
          #endif
		 }

          #endif //AP_DEBUG_MESSAGES
		  
		  
         source_object->Dyn_OCB_control_data.AP_flags|=AP_LOCKED;	//we can apply thrust 
         source_object->Dyn_OCB_control_data.AP_flags&=~AP_THRUST_LOCK_OUT_FLAG;
      }
      else
      {

		#if AP_DEBUG_MESSAGES==1 //can only printf to OSX syscon
		#if ZEX_PROJECT_BUILDER==1 //can only printf to OSX syscon
	     if ((source_object->Dyn_OCB_control_data.AP_flags&AP_LOCKED)==AP_LOCKED)
		 {
          fprintf (stderr, "AP: Object %i has lost lock on its target \n",the_object); 
		 }
		#endif
		#endif //AP_DEBUG_MESSAGES

	  source_object->Dyn_OCB_control_data.AP_flags&=~AP_LOCKED; 
      source_object->Dyn_OCB_control_data.AP_flags&=~AP_BRAKING_PHASE_FLAG;
      source_object->Dyn_OCB_control_data.AP_flags|=AP_THRUST_LOCK_OUT_FLAG;
	  

      }
      
end_steer:   
     
     if(source_object->Dyn_OCB_control_data.AP_thr_lock_out_timer>0)
     source_object->Dyn_OCB_control_data.AP_thr_lock_out_timer-=time_in_secs;
     
     
     
     if(get_main_camera_object()==the_object)
     { 
      debug_orcs_pitch=STATIC_CAST_TO_INT(source_object->Dyn_OCB_control_data.ORCS_extern_pitch*1000);
      debug_orcs_roll=STATIC_CAST_TO_INT(source_object->Dyn_OCB_control_data.ORCS_extern_roll*1000);
      debug_orcs_yaw=STATIC_CAST_TO_INT(source_object->Dyn_OCB_control_data.ORCS_extern_yaw*1000);
     }
     
 return;
     
}




float thrust_error;

int debug_time_to_target,debug_time_to_stop;

module_private void run_AP_thrust(int the_object)
{
ZObject *source_object;
sds_mount_data_type the_mount_info;
vector our_current_position_in_m,target_position_in_m;
float distance_to_target_in_m;
float max_thrust,fr,f,m,a;
// float t;
int ap_mode;
float time_to_target,time_to_stop;
float recalculate_time,radius_mult;

source_object=&(*ocb_ptr).object_list[the_object];

//calculate some vars
ap_mode=source_object->Dyn_OCB_control_data.command_autopilot_engaged;	//AP is on, 2 is steer transport, 3 is steer



if (ap_mode==AP_COMBAT)
{
max_thrust=1;
recalculate_time=0.3;
}
else
if((ap_mode==AP_TRANSPORT) || (ap_mode==AP_HYPERTRANSPORT))
{
max_thrust=0.8;
recalculate_time=1.5;
}
else
return;	//off, steer_cue, steer



        if ((source_object->Dyn_OCB_control_data.AP_flags&AP_DROP_TO_STEER_FLAG))
        {
        if((source_object->Dyn_OCB_control_data.AP_flags&AP_LOCKED)==0) return;	//wait until we're synched

            source_object->Dyn_OCB_control_data.command_ORCS_damper=ORCS_DAMPER_ROT+ORCS_DAMPER_LIN;	 //switch on linear dampers for flight
            
            if(source_object->Dyn_OCB_control_data.current_relative_velocity<-20)
            {
              source_object->Dyn_OCB_control_data.ORCS_thrust_down=0;
              source_object->Dyn_OCB_control_data.ORCS_thrust_up=max_thrust/3;
            }
            else
            if(source_object->Dyn_OCB_control_data.current_relative_velocity>20)
            {
              source_object->Dyn_OCB_control_data.ORCS_thrust_down=max_thrust/3;
              source_object->Dyn_OCB_control_data.ORCS_thrust_up=0;
            }
            else
            {
              if (the_object==get_main_camera_object())
              {
                add_to_text_display("AP> Arrived. Dampers on",DLP_GREEN);
                set_time_multiplier_to_1();
                fade_out_music();		//kill AP music
              }
              engage_ap(the_object,AP_STEER_CUE); 
            }
         return;
        }



       if(source_object->Dyn_OCB_control_data.AP_timer_1<recalculate_time)
       {
         source_object->Dyn_OCB_control_data.AP_timer_1+=time_in_secs;
         return;
       }

ap_calc_dampers(the_object);

source_object->Dyn_OCB_control_data.AP_timer_1=0;
source_object->Dyn_OCB_control_data.ORCS_thrust_up=0;
source_object->Dyn_OCB_control_data.ORCS_thrust_down=0;


        get_position_in_m(source_object->object_targetted,&target_position_in_m);	//do this now for steering
        get_position_in_m( the_object, &our_current_position_in_m);
        distance_to_target_in_m=get_distance_apart(target_position_in_m,our_current_position_in_m);	//as both of these are in m, so is the result
//calc max time multiplier
        
		if (the_object==get_main_camera_object())
		{

		 if (distance_to_target_in_m<3e9) //3 mill km's
		 {		 
		 if (time_multiplier>1000)
		  {
	           add_to_text_display("AP> Approach. Reducing Tempmult",DLP_GREEN);  
		   set_time_multiplier_to_1000();
		  }
		 }

                 if (distance_to_target_in_m<5e8) //5 hundred thou km's
		 {
		  if (time_multiplier>200)
		  {
	       add_to_text_display("AP> Approach. Reducing Tempmult",DLP_GREEN);  
		   set_time_multiplier_to_200();
		  }
		 }
		}
		
		
//calc distance to stop at...

        if (get_mass_in_kg(source_object->object_targetted)>500000000) //planet?
        {
         radius_mult=AP_RADIUS_STOP_PLANET;
        }
        else
        {
         radius_mult=AP_RADIUS_STOP; //16;
        }
		
		
        if (ap_mode<AP_COMBAT) //modes above transport do not stop (i.e. combat)
        {
          if (distance_to_target_in_m<(get_object_radius_in_metres(source_object->object_targetted)*radius_mult))
          {
            if(!(source_object->Dyn_OCB_control_data.AP_flags&AP_DROP_TO_STEER_FLAG))	//only do this once
            {
              source_object->Dyn_OCB_control_data.AP_flags|=AP_DROP_TO_STEER_FLAG;
		    if (the_object==get_main_camera_object())
		    {
                     add_to_text_display("AP> Final phase. All dampers on",DLP_GREEN);
                    }
			
             source_object->Dyn_OCB_control_data.command_ORCS_damper=ORCS_DAMPER_ROT+ORCS_DAMPER_LIN;	 //switch on linear dampers for flight
            }
          return;
         }
        }	



       //get total reverse thrust
         get_ship_mount_data (the_object, MREV1V, &the_mount_info);
         fr=the_mount_info.the_value*max_thrust;
         get_ship_mount_data (the_object, MREV2V, &the_mount_info);
         fr+=the_mount_info.the_value*max_thrust;
       //get total main thrust
         get_ship_mount_data (the_object, MTHR1V, &the_mount_info);
         f=the_mount_info.the_value*max_thrust;
         get_ship_mount_data (the_object, MTHR2V, &the_mount_info);
         f+=the_mount_info.the_value*max_thrust;
         f*=max_thrust;
         fr*=max_thrust;
         
       // get mass of object
        m=get_mass_in_kg(the_object);
 


//calc time to brake given current velocity

        

// calc reverse acceleration
        a=fr/m;
//        v=sqrt(2*a*distance_to_target_in_m);
        time_to_stop=(source_object->Dyn_OCB_control_data.current_relative_velocity*ZEX_UNIT_SIZE_IN_METERS)/a;	     //time to brake to target
        
        
//        a=f/m;
//        v=sqrt(2*a*distance_to_target_in_m);
//        time_to_target=v/a;	//time to brake to target


//calc actual time to target at current vel
        time_to_target=source_object->Dyn_OCB_control_data.time_to_target_in_secs_as_crow_flies; 
       if(time_to_target<0) 
       {
         time_to_target=INFINITY;
       }
//      else	  //only do if we are travelling towards the object..
//if v>current velocity then we do not need to start braking
        if ((time_to_stop>(time_to_target *2)) && (time_to_stop>=0))
        {
            source_object->Dyn_OCB_control_data.AP_flags|=AP_BRAKE_FLAG;
        }
        else
        {
            source_object->Dyn_OCB_control_data.AP_flags&=~AP_BRAKE_FLAG;
        }
          
		if (get_main_camera_object()==the_object)
		{
         debug_time_to_target=STATIC_CAST_TO_INT(time_to_target);
         debug_time_to_stop=STATIC_CAST_TO_INT(time_to_stop);
        }
        
// -----------------------------------------------------------------




if((source_object->Dyn_OCB_control_data.AP_flags&AP_BRAKE_FLAG))	//apply reverse thrust
  {
    source_object->Dyn_OCB_control_data.ORCS_thrust_down=max_thrust/3;
    source_object->Dyn_OCB_control_data.ORCS_thrust_up=0;
    source_object->Dyn_OCB_control_data.AP_flags|=AP_BRAKING_PHASE_FLAG; //set we have braked
  }
else
  {
  if(!(source_object->Dyn_OCB_control_data.AP_flags&AP_BRAKING_PHASE_FLAG))	//we haven't braked yet
   {
    if(!(source_object->Dyn_OCB_control_data.AP_flags&AP_THRUST_LOCK_OUT_FLAG))	//and we are synched
    {
     source_object->Dyn_OCB_control_data.ORCS_thrust_down=0;
     source_object->Dyn_OCB_control_data.ORCS_thrust_up=max_thrust; //so we can apply fwd thrust
    }
   }
  }



}


module_private void ap_calc_dampers(int the_object)
{
vector vv,heading_vector;
ZObject *source_object;

source_object=&(*ocb_ptr).object_list[the_object];



   //if heading vector is aligned with velocity vector, switch off lin dampers
   vv=source_object->rel_velocity_vector;
   heading_vector=source_object->n_translated;
   
   if ((vv.x<0.2) && (vv.y<0.2) && (vv.z<0.2)) 
   {
     if ((vv.x>-0.2) && (vv.y>-0.2) && (vv.z>-0.2))
     vv=(vector)(*ocb_ptr).object_list[the_object].n_translated;	//no velocity
   }
   
   Vnormalise(&vv);


   thrust_error=Vdotproduct(&heading_vector, &vv);
   
   if ((thrust_error>0.9995))
   {
   source_object->Dyn_OCB_control_data.command_ORCS_damper=ORCS_DAMPER_ROT;	//switch off linear dampers for flight
   }
   else
   {
   source_object->Dyn_OCB_control_data.command_ORCS_damper=ORCS_DAMPER_ROT+ORCS_DAMPER_LIN;	//switch on linear dampers for flight
   source_object->Dyn_OCB_control_data.AP_flags&=~AP_BRAKING_PHASE_FLAG; //alow thrust if we have lost synch

//      source_object->Dyn_OCB_control_data.AP_flags&=~AP_CALCD_FLAG;	  //if lin damps on then we need to recalculate 
   }
}

void disengage_AP(int object)	
{
ZObject *source_object;

source_object=&(*ocb_ptr).object_list[object];

 source_object->Dyn_OCB_control_data.ORCS_extern_pitch=0;
 source_object->Dyn_OCB_control_data.ORCS_extern_roll=0;
 source_object->Dyn_OCB_control_data.ORCS_extern_yaw=0;
source_object->Dyn_OCB_control_data.ORCS_thrust_up=0;
source_object->Dyn_OCB_control_data.ORCS_thrust_down=0;
source_object->Dyn_OCB_control_data.command_autopilot_engaged=0;       //go to off
source_object->Dyn_OCB_control_data.command_ORCS_damper=ORCS_DAMPER_ROT+ORCS_DAMPER_LIN;	//switch linear dampers on
source_object->Dyn_OCB_control_data.AP_flags=0;
if (object==get_main_camera_object())
{
fade_out_music();		//kill AP slowdown music
}

}


void engage_ap(int the_object,int the_mode) //mode =3=transport, 4=combat
{
ZObject *source_object;

 source_object=&(*ocb_ptr).object_list[the_object];
        source_object->Dyn_OCB_control_data.ORCS_extern_pitch=0;
        source_object->Dyn_OCB_control_data.ORCS_extern_roll=0;
        source_object->Dyn_OCB_control_data.ORCS_extern_yaw=0;
        source_object->Dyn_OCB_control_data.ORCS_thrust_up=0;
        source_object->Dyn_OCB_control_data.ORCS_thrust_down=0;
        source_object->Dyn_OCB_control_data.AP_pull_up_timer=0;
        source_object->Dyn_OCB_control_data.AP_flags=0;
        source_object->Dyn_OCB_control_data.AP_roll_lock_out=0;
        source_object->Dyn_OCB_control_data.AP_brake_time=0;
        source_object->Dyn_OCB_control_data.AP_timer_1=0;
        source_object->Dyn_OCB_control_data.AP_thr_lock_out_timer=0;
        source_object->Dyn_OCB_control_data.AP_flags=0;
        source_object->Dyn_OCB_control_data.command_autopilot_engaged=the_mode;
        source_object->Dyn_OCB_control_data.ap_ht_engage_timer=HYPERTRANSPORT_DELAY;
        if (the_mode==AP_HYPERTRANSPORT)
        {
          add_to_text_display("AP> Running Hypertransport parameter checks...",DLP_GREEN);
          source_object->Dyn_OCB_control_data.ht_accel=0.0000001; //starting acceleration
        }
}







int find_nearest_planet(int object)
{
extern int dynamic_object_index;

int nearest_planet;
double  nearest_distance, dist;
register ZObject * current_object_ptr;
int active_object_counter, object_counter;

nearest_distance=nearest_planet=-1;

active_object_counter=0;
object_counter=0;


while (active_object_counter<dynamic_object_index)
{
    current_object_ptr=&ocb_ptr->object_list[object_counter];

    if ((current_object_ptr->in_use==1))
    {
     if(current_object_ptr->Dyn_OCB_control_data.controller==PLANET_CONTROL 
        || current_object_ptr->Dyn_OCB_control_data.controller==SUN)
        {
        	dist=get_distance_to_from(object,object_counter);
        	if(nearest_distance==-1 || dist<nearest_distance)
        	  {
        	  	nearest_distance=dist; nearest_planet=object_counter;
        	  }
        }
     active_object_counter+=1;
     
    }
    
object_counter++;

//¥ do next object
}	//end of while
return nearest_planet;
} 


//returns 0 if not aligned, else !0
int is_AP_aligned(int object)
{
ZObject *source_object;

 source_object=&(*ocb_ptr).object_list[object];

 return source_object->Dyn_OCB_control_data.AP_flags&AP_LOCKED;

}