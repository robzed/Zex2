/*

Camera_conrol.cpp

 * $Log: Camera_control.cpp,v $
 * Revision 1.3  2003/09/27 20:41:37  robp
 * Fixed float->int warnings.
 *
 * Revision 1.2  2003/09/20 12:57:13  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:35:14  stu_c
 * First Imported.
 *
 * Revision 1.4  2002/09/08 02:17:43  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.3  2001/11/02 19:32:56  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:29:16  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:08:48  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:04  rob
 * First Imported.
 *
 * Revision 1.6  2001/09/04 20:37:00  stu
 * Sim stuff
 *
 * Revision 1.5  2001/07/08 20:24:49  stu
 * 0.66 release 080701
 *
 * Revision 1.4  2001/06/24 22:34:40  stu
 * Removed lots of obsolete calls and added
set_main_camera_object
 *
 * Revision 1.3  2001/06/24 21:45:00  stu
 * Removed lots of obsolete calls and added
set_main_camera_object
 *
 * Revision 1.2  2001/05/28 01:52:46  stu
 * 280501
 *
 * Revision 1.1.1.1  2001/01/01 21:13:34  rob
 * First Imported.
 *
 * Revision 1.1  2000/10/08 21:43:50  stu
 * Engine conversion to 64 bit. Bitch of a job.
 *
 * Revision 1.0.0.1  2000/08/21 22:00:03  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"
  #include "3d_structures_def.h"
  #include	"zex_misc.h"
#endif //pch

#include "camera_control.h"
#include "angular_stuff.h"
#include "matrix_primitives.h"
#include "physics.h"
#include "dlp.h"


int camera_mode;	//0=top down, 1=above behind
int camera_object_slot=0;
int camera_view=0;       //0=front, !0=panning

camera_type camera;


void reset_camera()
{
camera_mode=1;
camera_object_slot=0;
camera_set_front_view();
}

float camera_pan_force;
int camera_pan_force_debug;
int camera_switched_view=0;

void run_camera_control()
{
extern _3D camera_pos;
extern	DynObjectsFixedSize *ocb_ptr;
//double ship_x,offset_x;


//new cam code
  (*ocb_ptr).object_list[camera_object_slot].eng_invisible=1;
 
//  ship_x=();
  camera_pos.x=-(*ocb_ptr).object_list[camera_object_slot].Oworldx;
//  ship_x=(;
  camera_pos.y=-(*ocb_ptr).object_list[camera_object_slot].Oworldy;
 camera_pos.z=-(*ocb_ptr).object_list[camera_object_slot].Oworldz;
 //reset_angular_torque(2);
 camera_pan_force_debug=STATIC_CAST_TO_INT(camera_pan_force);
 
 if (camera_switched_view) camera_switched_view--;

}

// ---------------------------------------------------------
// ---------------------------------------------------------
//   CAMERA   SPECIFIC   
//
// ---------------------------------------------------------
// ---------------------------------------------------------

int get_main_camera_object(void)
{
return camera_object_slot;
}



void camera_set_front_view()
{
 camera_view=1;
 camera_pan_force=0;
 add_to_text_display("OBC> Front view.",DLP_GREEN);
 camera_switched_view=2; //for has_camera_switched_view used in fast object controller
}

void camera_set_left_view()
{

 camera_view=2;
  camera_pan_force=500;
 add_to_text_display("OBC> Left view.",DLP_GREEN);
 camera_switched_view=2; //for has_camera_switched_view used in fast object controller

}

void camera_set_right_view()
{
 camera_view=3;
  camera_pan_force=-500;
 add_to_text_display("OBC> Right view.",DLP_GREEN);
 camera_switched_view=2; //for has_camera_switched_view used in fast object controller
}

void camera_set_rear_view()
{
 camera_view=4;
  camera_pan_force=-200000;
 add_to_text_display("OBC> Rear view.",DLP_GREEN);
 camera_switched_view=2; //for has_camera_switched_view used in fast object controller

}

int has_camera_switched_view() //called from has_camera_switched_view used in fast object controller to switch the stars orientation 
{
 return camera_switched_view;
}


int camera_get_view() //returns TRUE=locked front, FALSE=free
{
 return camera_view;
}

float camera_get_pan() //returns camera pan force
{
 return camera_pan_force;
}


//set the camera object. If object is alive then returns 0 else -1
int set_main_camera_object(int dynamic_slot)
{
extern	DynObjectsFixedSize *ocb_ptr;

if(ocb_ptr->object_list[dynamic_slot].in_use==0) return -1;
if(dynamic_slot<0) return -1;
if(dynamic_slot>MAX_NUMBER_OF_DYN_OBJECTS) return -1;

//make old camera object visible
if(camera_object_slot>=0 && camera_object_slot<MAX_NUMBER_OF_DYN_OBJECTS)
(*ocb_ptr).object_list[camera_object_slot].eng_invisible=0;

camera_object_slot=dynamic_slot;
return 0;
}






