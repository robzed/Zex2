// *************************************MODULE HEADER*********************************
// * FILENAME           : dust.cpp
// * MODULE TITLE       : dust
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 00:00:09 on 20th March 2000
// * FIRST AUTHOR       : Stuart Ball
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
/*
 * $Log: dust.cpp,v $
 * Revision 1.2  2003/09/20 12:56:57  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:35:44  stu_c
 * First Imported.
 *
 * Revision 1.11  2002/09/08 02:02:09  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.10  2002/07/28 17:13:53  stu_c
 * Exhausts and further particle work
 *
 * Revision 1.9  2002/05/31 23:51:08  stu
 * Removed all warnings from the code
 *
 * Revision 1.8  2002/05/05 22:27:53  stu
 * Further developement - hull temperature now correct for multiple suns and atmosphere.

All calls to add_to_kill_list check the return
 *
 * Revision 1.7  2002/04/13 15:32:07  stu
 * Rotation, Alycians, behaviour
 *
 * Revision 1.6  2002/02/10 21:46:16  stu
 * fast dyn objects
 *
 * Revision 1.5  2001/12/14 19:42:08  stu
 * Docking
 *
 * Revision 1.4  2001/12/04 23:35:08  stu
 * Motherships work
 *
 * Revision 1.3  2001/11/02 19:31:06  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:28:45  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:11:45  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:05  rob
 * First Imported.
 *
 * Revision 1.9  2001/09/21 19:58:55  rob
 * No asteriods
 *
 * Revision 1.8  2001/09/18 20:42:28  stu
 * More Sim/ hud numbers print
 *
 * Revision 1.7  2001/09/10 21:41:55  stu
 * 100901
More Sim, AP broken.
 *
 * Revision 1.6  2001/06/23 20:06:22  stu
 * 0.66 checkin
 *
 * Revision 1.5  2001/06/03 03:17:36  stu
 * OCB structure now contains a sub structure called:
ZObjectControl
Which contains all the control data for an object.

Added accessor in access_ocbs which returns a pointer to type ZObjectControl
 *
 * Revision 1.4  2001/04/29 20:57:57  stu
 * Z Clipping
 *
 * Revision 1.3  2001/04/02 17:28:45  rob
 * made various variables switch on and off-able from forth. These include dust, asteriods, hud, zbuffer.
 *
 * Revision 1.2  2001/04/01 18:38:34  stu
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2001/01/01 21:13:35  rob
 * First Imported.
 *
 * Revision 1.7  2000/12/11 19:49:00  stu
 * Light fix in 3d_top - calc_normal
 *
 * Revision 1.6  2000/12/10 03:35:27  stu
 * Calls to kill replaced with either calls to add_to_kill_list OR
i_know_what_im_doing_kill

Dust now self lit.
 *
 * Revision 1.5  2000/12/03 23:52:44  rob
 * changed constants for object type to get uniformity
 *
 * Revision 1.4  2000/11/26 22:58:46  stu
 * Various.
 *
 * Revision 1.3  2000/11/16 20:21:54  rob
 * orbit
 *
 * Revision 1.2  2000/10/08 21:45:42  stu
 * Engine conversion to 64 bit. Bitch of a job.
 *
 * Revision 1.1  2000/09/20 23:48:17  stu
 * HUD/AP/Scaling
 *
 * Revision 1.0.0.1  2000/08/21 22:01:59  stu
 * First Imported.
 *
 */


// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "3d_structures_def.h"  
  #include "processor.h"
  #include "zex_misc.h"
  #include "access_ocbs.h"
  #include "error_handler.h"
#endif


#include "load_objects.h"
#include "angular_stuff.h"
#include "obj_control_misc_routines.h"
#include	"objects_controls.h"	//controller definitions
#include "dust.h"
#include "physics.h"
#include "3d_top.h"
#include "object_control_top.h"
#include "camera_control.h"

// ***********************************************************************************
// * CONSTANTS 
// *
 #define DUST_BOX_SIZE 50000


// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

int DUST_flag=1;		// t=dust on, f=dust off

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *




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

int can_gen_dust=1;
int kill_dust_called;

//еееееееееееееееееееееееееееееееееееееееееееееее
void handle_dust(int it)	//check if too far behind (or infront!) and regen if so
{
//extern	DynObjectsFixedSize *ocb_ptr;
//extern _3D ship_rotated_normal; //from engine
extern int number_of_stars;


//see if dust is visible and if not, kill it



if (is_in_frustrum(it,FALSE)==0)
  {
  kill_dust_called++;
  if (add_to_kill_list(it)!=0)
	{
	  MyDebugStr(__LINE__, __FILE__,"add_to_kill_list failed!");
	}
    if (number_of_stars>0) number_of_stars--;

  }


//int dust_distance,camera_z;
//int diff_angle,angle_to_dust,behind;
//float	zex_x,zex_y,it_x,it_y, it_z;
//
//
// int diff_x,diff_y,diff_z,dead,zex_z;
////find out if dust is out of box
//zex_x=(*ocb_ptr).object_list[0].Oworldx;
//zex_y=(*ocb_ptr).object_list[0].Oworldy;
//zex_z=(*ocb_ptr).object_list[0].Oworldz;
//
//it_x=(*ocb_ptr).object_list[it].Oworldx;
//it_y=(*ocb_ptr).object_list[it].Oworldy;
//it_z=(*ocb_ptr).object_list[it].Oworldz;
//
//
//diff_x=abs_nobranch(it_x-zex_x);
//diff_y=abs_nobranch(it_y-zex_y);
//diff_z=abs_nobranch(it_z-zex_z);
//
//dead=0;
//if (diff_x>DUST_BOX_SIZE/2) dead=1;
//if (diff_y>DUST_BOX_SIZE/2) dead=1;
//if (diff_z>DUST_BOX_SIZE/2) dead=1;
//
//
// if (dead==1) 
//  {
//   kill(it);
//   if (number_of_stars>0) number_of_stars--;
//
//  }


}


int gen_dust_called;
int debug_count=0;

void gen_dust()	//gen a star in front of zex
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
//extern int thrust;
extern int number_of_stars,global_show_status;
//extern int camera_mode;
//extern _3D camera_pos;
//int camera_z,camera_distance,distance_from_cam;
//float screen_x,screen_y,world_x,world_y,z;
int obj_targetted;
load_dyn_object_pb the_params;
//_3D point_in_frustrum;
int camera_object;
int it;
//float dust_random_x,dust_random_y;
float relative_velocity;
//float ship_dir_x,ship_dir_y;
if (global_show_status!=0) return;
if (can_gen_dust==0) return;

#if TEST_SYSTEM == 1
return;
#endif

return;

if(!DUST_flag) return;


camera_object=get_main_camera_object();
relative_velocity=(*ocb_ptr).object_list[camera_object].Dyn_OCB_control_data.current_relative_velocity;


obj_targetted=get_object_targetted(camera_object);
if (obj_targetted==-1) return;


if (relative_velocity>2000)
{
 return;
}

if (relative_velocity<-2000)
{
 return;
}

//debug_count--;
//if (debug_count>0) return;
//debug_count=500;

 while (number_of_stars<50)

 {





 gen_dust_called++;

 clear_pb(&the_params);

 it=find_vacant_dynamic_object_slot();

 if (it!=-1)
 {   
// float dust_x,dust_y,dust_z;
// int temp;
    number_of_stars++;

        the_params.world_x=(*ocb_ptr).object_list[0].Oworldx+RangedRdm(0,20000)-10000;
        the_params.world_y=(*ocb_ptr).object_list[0].Oworldy+RangedRdm(0,20000)-10000;
        the_params.world_z=(*ocb_ptr).object_list[0].Oworldz+RangedRdm(0,20000)-10000;
//offset dust box to in front of ship
      the_params.world_x+=ship_rotated_normal.x*25000;
      the_params.world_y+=ship_rotated_normal.y*25000;
      the_params.world_z+=ship_rotated_normal.z*25000;
	
	the_params.controller_ref=DUST;
	the_params.object_category=SCENARY;
		
	the_params.mass=1;
	load_dyn_object(DUST,it,&the_params,-1,1,-1,DUST_BIT);


    set_object_abs_vel_vector(it,get_object_abs_vel_vector(obj_targetted));

 
  }	//end of it!=-1
  else return;	 //couldn't gen dust - no spare slots

 }	//end of while num_of_stars

}


void kill_all_dust()
{
int i;
extern	DynObjectsFixedSize *ocb_ptr;
extern int number_of_stars;


for (i=1;i<MAX_NUMBER_OF_DYN_OBJECTS-1;i++)
   {
    if ((*ocb_ptr).object_list[i].in_use==1 && (*ocb_ptr).object_list[i].Dyn_OCB_control_data.controller==DUST) 
     {
      if (add_to_kill_list(i)!=0)
	  {
		 MyDebugStr(__LINE__, __FILE__,"add_to_kill_list failed!");
	  }
	 }    
   }
number_of_stars=0;

}


void gen_dust_for_end()	//gen a star in front of camera
{
//extern	DynObjectsFixedSize *ocb_ptr;
//extern _3D ship_rotated_normal; //from engine
//extern int thrust,number_of_stars;
//extern int global_show_status;
extern _3D camera_pos;
//int camera_z;

load_dyn_object_pb the_params;

int it;
//float dust_random_x,dust_random_y;
//float ship_dir_x,ship_dir_y;
//oif (global_show_status!=0) return;
 
it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in star
	the_params.world_x=(-camera_pos.x)+(RangedRdm(0,12000)-6000);	//dustPos[i].x;
	the_params.world_y=(-camera_pos.y)+(RangedRdm(0,12000)-6000);	//dustPos[i].y;
//	the_params.world_z=RangedRdm(0,8000)-3300;	//dustPos[i].z;
	the_params.world_z= camera_pos.z+400;	//dustPos[i].z;
//	the_params.Omovedx=-RangedRdm(1,22);	//vicious
	the_params.delta_rot_x=RangedRdm(5,20);
	the_params.delta_rot_z=RangedRdm(5,20);
	the_params.rot_z=RangedRdm(0,359);
	
	the_params.controller_ref=DUST_FOR_END;
	the_params.attack_mode=0;

	the_params.mass=1;
	the_params.object_category=SCENARY;
		
//	if(RangedRdm(0,4)>2)
	load_dyn_object(DUST,it,&the_params,-1,1,-1,!DUST_BIT);	//the object, the position (-1=next free)
//	else load_dyn_object(dust,it,&the_params);


 }
}



void gen_dust_for_warphole()	//gen a star in front of camera
{
//extern	DynObjectsFixedSize *ocb_ptr;
//extern _3D ship_rotated_normal; //from engine
//extern int thrust,number_of_stars,global_show_status;
extern _3D camera_pos;
//int camera_z;

load_dyn_object_pb the_params;

int it;
//float dust_random_x,dust_random_y;
//float ship_dir_x,ship_dir_y;
//oif (global_show_status!=0) return;
 
it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in star
	the_params.world_x=(-camera_pos.x)+(RangedRdm(0,12000)-6000);	//dustPos[i].x;
	the_params.world_y=(-camera_pos.y)+(RangedRdm(0,12000)-6000);	//dustPos[i].y;
//	the_params.world_z=RangedRdm(0,8000)-3300;	//dustPos[i].z;
	the_params.world_z= camera_pos.z+400;	//dustPos[i].z;
//	the_params.Omovedx=-RangedRdm(1,22);	//vicious
	the_params.rot_z=RangedRdm(0,359);
	
	the_params.controller_ref=DUST_FOR_HOLE;
	the_params.attack_mode=0;

	the_params.mass=1;
	the_params.object_category=SCENARY;
        the_params.number_of_missiles=0;
//	if(RangedRdm(0,4)>2)
	load_dyn_object(DUST,it,&the_params,-1,1,-1,DUST_BIT);	//the object, the position (-1=next free)
//	else load_dyn_object(dust,it,&the_params);


 }
}


void populate_dust_for_start()
{
int i;
extern	DynObjectsFixedSize *ocb_ptr;
//extern _3D ship_rotated_normal; //from engine
//extern int thrust;
extern int number_of_stars;
//extern int global_show_status;
//extern _3D camera_pos;
//float screen_x,screen_y,world_x,world_y,z;
load_dyn_object_pb the_params;

int it;
//float dust_random_x,dust_random_y;


return;

if(!DUST_flag) return;

for (i=0;i<70;i++)
     {
     
clear_pb(&the_params);

it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in star
    number_of_stars++;	


	the_params.delta_rot_y=RangedRdm(0,20)-10;
	the_params.delta_rot_z=RangedRdm(0,20)-10;
	the_params.delta_rot_x=RangedRdm(0,20)-10;	
	the_params.controller_ref=DUST;
	the_params.object_category=SCENARY;
	load_dyn_object(DUST,it,&the_params,-1,1,-1,DUST_BIT);	//the object, the position (-1=next free)
	}

//now set up position

  (*ocb_ptr).object_list[it].Oworldx=(*ocb_ptr).object_list[0].Oworldx+RangedRdm(0,10000)-5000;
  (*ocb_ptr).object_list[it].Oworldy=(*ocb_ptr).object_list[0].Oworldy+RangedRdm(0,10000)-5000;
  (*ocb_ptr).object_list[it].Oworldz=(*ocb_ptr).object_list[0].Oworldz+RangedRdm(0,10000)-5000;

     
     }	//end of for i
}



