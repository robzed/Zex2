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

//input_control.c
//SB 22/12/98
/*
$Log: input_control.c,v $
Revision 1.0.0.1  2001/10/17 20:46:06  rob
First Imported.

Revision 1.4  2001/09/18 20:42:19  stu
More Sim/ hud numbers print

Revision 1.3  2001/09/08 10:32:17  rob
Added variable to track how long it takes to resume isp

Revision 1.2  2001/01/02 18:55:49  stu
mouse coordinates with multi res

Revision 1.1.1.1  2001/01/01 21:13:36  rob
First Imported.

Revision 1.2  2000/09/20 23:55:35  stu
HUD/AP/Scaling

Revision 1.1  2000/09/13 20:37:06  stu
Changes for hud targetting and sun corona effects.

Revision 1.0.0.1  2000/08/21 22:03:33  stu
First Imported.

Revision 1.3  2000/08/20 22:24:49  stu
Removed old control panel and associated code.

Revision 1.2  2000/08/20 21:20:45  stu
Added log to header

*/
#include "platform.h"
#include <timer.h>


#include <InputSprocket.h>
#include <agl.h>

#include "input_control.ext"
#include "Error_handler.ext"
#include	"splat_control.ext"


#define thrustUp 0
#define thrustDown 1

#define roll 2
#define pitch 3
#define yaw 4

#define up_thrust 5
#define down_thrust 6

#define cannon 7
#define laser 8
#define missile 9
#define retro 10
#define orbit_stab 11
#define radar_range 12
#define AskForDock 13
#define do_eject 14
#define vel_limiter 15
#define target 16
#define time_accelup 17
#define time_acceldown 18
#define autopilotup 19
#define autopilotdown 20
#define COMMAND 21
#define GM 22
#define show_inventory 23
#define abort 24
#define music_up 25
#define music_down 26
#define sound_up 27
#define sound_down 28
#define console 29


#define NUMBER_OF_INPUTS 30	//for array sizing

extern int gShowConsole;

//0=not isping, 1=we are
UInt32 gISPState=0;

//Our controller positions
UInt32 gThrustUp;
UInt32 gThrustDown;
UInt32	gVertUpThrust;
UInt32  gVertDownThrust;
UInt32 gRoll;	//was gTurn
UInt32 gPitch;
UInt32 gYaw;
UInt32 gThrust;

UInt32 gCannon;
UInt32 gMissile;
UInt32 gLaser;
UInt32 gRetro;
UInt32 gEject;	//as it says
UInt32 gAbort;
UInt32 gMusic_up;
UInt32 gMusic_down;
UInt32 gSound_up;
UInt32 gSound_down;
UInt32 gRadarRangeInput;
UInt32 gOrbitStab;	//orbit stabilisation
UInt32 gShowInventory;	//show the inventory screen
UInt32 gAskForDock;	//ask mothership for dock
UInt32 gvel_limiter_input;	//velocity limiter
UInt32 gCommandControl;	//Auto Pilot
UInt32 gGM;	//Galactic Map
UInt32 gConsole;
UInt32 gTarget;
UInt32 gTimeaccelup;
UInt32 gTimeacceldown;
UInt32 gAutopilotUp;
UInt32 gAutopilotDown;

ISpElementReference	gInputElements[NUMBER_OF_INPUTS];

//routine samples the inputs and sets the relevant gVars.
//called every frame from main game loop
void poll_input_device()
{
//only console nad abort keys processed if console on
ISpElement_GetSimpleState (gInputElements[console], &gConsole);  // orbit stabilisation
ISpElement_GetSimpleState (gInputElements[abort], &gAbort);  // 0 or 1

if (gConsole==0)
{

ISpElement_GetSimpleState (gInputElements[roll], &gRoll);	//0 to 0xffffffff
ISpElement_GetSimpleState (gInputElements[pitch], &gPitch);	//0 to 0xffffffff
ISpElement_GetSimpleState (gInputElements[yaw], &gYaw);	//0 to 0xffffffff
ISpElement_GetSimpleState (gInputElements[thrustUp], &gThrustUp); //0 to 0xffffffff
ISpElement_GetSimpleState (gInputElements[thrustDown], &gThrustDown); //0 to 0xffffffff
ISpElement_GetSimpleState (gInputElements[cannon], &gCannon); //0 or 1
ISpElement_GetSimpleState (gInputElements[laser], &gLaser); //0 or 1
ISpElement_GetSimpleState (gInputElements[missile], &gMissile); //0 or 1
ISpElement_GetSimpleState (gInputElements[retro], &gRetro);  //0 or 1
ISpElement_GetSimpleState (gInputElements[music_up], &gMusic_up);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[music_down], &gMusic_down);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[sound_up], &gSound_up);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[sound_down], &gSound_down);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[radar_range], &gRadarRangeInput);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[orbit_stab], &gOrbitStab);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[show_inventory], &gShowInventory);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[do_eject], &gEject);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[AskForDock], &gAskForDock);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[vel_limiter], &gvel_limiter_input);  // velocity limiter
ISpElement_GetSimpleState (gInputElements[COMMAND], &gCommandControl);  // commandeering box
ISpElement_GetSimpleState (gInputElements[GM], &gGM);  // velocity limiter
ISpElement_GetSimpleState (gInputElements[up_thrust], &gVertUpThrust);  // commandeering box
ISpElement_GetSimpleState (gInputElements[down_thrust], &gVertDownThrust);  // velocity limiter
ISpElement_GetSimpleState (gInputElements[target], &gTarget);  // velocity limiter
ISpElement_GetSimpleState (gInputElements[time_accelup], &gTimeaccelup);  // time accelerator
ISpElement_GetSimpleState (gInputElements[time_acceldown], &gTimeacceldown);  // time accelerator
ISpElement_GetSimpleState (gInputElements[autopilotup], &gAutopilotUp);  // time accelerator
ISpElement_GetSimpleState (gInputElements[autopilotdown], &gAutopilotDown);  // time accelerator
}

}

void init_input_device()
{

InitNeeds();
init_ip_globs();
}


init_ip_globs()
{
gThrustUp=0;
gThrustDown=0;
gRoll=0x7fffffff;
gPitch=0x7fffffff;
gYaw=0x7fffffff;
gCannon=0;
gMissile=0;
gLaser=0;
gRetro=0;
gAbort=0;
gMusic_up=0;
gMusic_down=0;
gSound_up=0;
gSound_down=0;
gRadarRangeInput=0;
gOrbitStab=0;
gShowInventory=0;
gEject=0;
gAskForDock=0;
gvel_limiter_input=0;
gTarget=0;
gCommandControl=0;
gConsole=0;
gVertUpThrust=0;
gVertDownThrust=0;
gTimeaccelup=0;
gTimeacceldown=0;
gAutopilotUp=0;
gAutopilotDown=0;

}





OSStatus InitNeeds()
{
#define kNumNeeds NUMBER_OF_INPUTS
#define kIconSuiteID_Cannon 129
#define   kIconSuiteID_YThrustUp 130
#define   kIconSuiteID_Roll 131
#define kIconSuiteID_Laser 132
#define kIconSuiteID_Missile 133
#define kIconSuiteID_Retro 134
#define kIconSuiteID_Abort 135
#define kIconSuiteID_mvol_up 136
#define kIconSuiteID_mvol_down 137
#define kIconSuiteID_svol_up 138
#define kIconSuiteID_svol_down 139
#define   kIconSuiteID_YThrustDown 140
#define   kIconSuiteID_YRadarRange 141
#define   kIconSuiteID_zoom_in 143
#define   kIconSuiteID_zoom_out 142	//I buggered these up a bit
#define   kIconSuiteID_MissileToggle 145
#define   kIconSuiteID_ShowInventory 146
#define   kIconSuiteID_Eject 147
#define   kIconSuiteID_Dock 148
#define   kIconSuiteID_CameraMode 149
#define   kIconSuiteID_vel_limiter 150
#define   kIconSuiteID_Command 152
#define   kIconSuiteID_GM 153
#define   kIconSuiteID_Console 153

int theError;

extern ISpElementReference	gInputElements[NUMBER_OF_INPUTS];

ISpNeed myNeeds[kNumNeeds] =
{
 {
  "\pForward Thrust",     //human name
  kIconSuiteID_YThrustUp, //icon
  0,0,	//player and group
 kISpElementKind_Button,  //kind
  kISpElementLabel_Axis_Throttle,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },

  {
  "\pReverse Thrust",     //human name
  kIconSuiteID_YThrustDown, //icon
  0,0,	//player and group
 kISpElementKind_Button,  //kind
  kISpElementLabel_Axis_Throttle,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },


  
 {
  "\pRoll",     //human name
  kIconSuiteID_Roll, //icon
  0,0,	//player and group
 kISpElementKind_Axis,  //kind
  kISpElementLabel_Axis_Roll,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },

 {
  "\pPitch",     //human name
  kIconSuiteID_Roll, //icon
  0,0,	//player and group
 kISpElementKind_Axis,  //kind
  kISpElementLabel_Axis_Pitch,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },

 {
  "\pYaw",     //human name
  kIconSuiteID_Roll, //icon
  0,0,	//player and group
 kISpElementKind_Axis,  //kind
  kISpElementLabel_Axis_Yaw,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },


  {
  "\pVertical Thruster",	//vert thrust
  kIconSuiteID_Roll,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },
  
  {
  "\pVertical Reverse Thruster",	//vert down thrust
  kIconSuiteID_Roll,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },
  
  {
  "\pCannon",
  kIconSuiteID_Cannon,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

  {
  "\pLaser",
  kIconSuiteID_Laser,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },


  {
  "\pMissile",
  kIconSuiteID_Missile,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },


  {
  "\pRetro Thrust",
  kIconSuiteID_Retro,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

  {
  "\pOrbital Stabilisers",
  kIconSuiteID_MissileToggle,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

  {
  "\pRadar Range Toggle",
  kIconSuiteID_YRadarRange,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

  {
  "\pRequest Mothership To Dock",
  kIconSuiteID_Dock,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

  {
  "\pEject",
  kIconSuiteID_Eject,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

 {
  "\pDampers",     //engine control
  kIconSuiteID_vel_limiter, //icon
  0,0,	//player and group
 kISpElementKind_Button,  //kind
  kISpElementLabel_Btn_Fire,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },

 {
  "\pTarget",     //Target item in sights
  kIconSuiteID_vel_limiter, //icon
  0,0,	//player and group
 kISpElementKind_Button,  //kind
  kISpElementLabel_Btn_Fire,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },
  
 {
  "\pTime Multiplier Increase",     //accelerated time
  kIconSuiteID_vel_limiter, //icon
  0,0,	//player and group
 kISpElementKind_Button,  //kind
  kISpElementLabel_Btn_Fire,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },

 {
  "\pTime Multiplier Decrease",     //accelerated time
  kIconSuiteID_vel_limiter, //icon
  0,0,	//player and group
 kISpElementKind_Button,  //kind
  kISpElementLabel_Btn_Fire,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },
 {
  "\pAutopilot Mode Up",     //accelerated time
  kIconSuiteID_vel_limiter, //icon
  0,0,	//player and group
 kISpElementKind_Button,  //kind
  kISpElementLabel_Btn_Fire,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  }, 
 {
  "\pAutopilot Mode Down",     //accelerated time
  kIconSuiteID_vel_limiter, //icon
  0,0,	//player and group
 kISpElementKind_Button,  //kind
  kISpElementLabel_Btn_Fire,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  }, 

 {
  "\pCommand And Control",     //ommand and control
  kIconSuiteID_Command, //icon
  0,0,	      //player and group
 kISpElementKind_Button,  //kind
  kISpElementLabel_Btn_Fire,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },

 {
  "\p System Map",     //solar system map
  kIconSuiteID_GM, //icon
  0,0,	      //player and group
 kISpElementKind_Button,  //kind
  kISpElementLabel_Btn_Fire,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },

  {
  "\pShow Ship Status Screen",
  kIconSuiteID_ShowInventory,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

  {
  "\pPause Game",
  kIconSuiteID_Abort,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },


  {
  "\pMusic Volume Up",
  kIconSuiteID_mvol_up,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },
  {
  "\pMusic Volume Down",
  kIconSuiteID_mvol_down,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },


  {
  "\pSound Volume Up",
  kIconSuiteID_svol_up,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },
  
  {
  "\pSound Volume Down",
  kIconSuiteID_svol_down,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },



  {
  "\pConsole",	//toggle display panel
  kIconSuiteID_Console,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

  };

	//¥	Setup the input sprocket elements
	theError = ISpElement_NewVirtualFromNeeds(kNumNeeds, myNeeds, gInputElements, 0);
	if (theError)
		report_error("Could not create ISp virtual controls from needs.","\p",theError);

	//¥	Init InputSprocket and tell it our needs
	theError = ISpInit(NUMBER_OF_INPUTS, myNeeds, gInputElements, 'LsZx', '0011', 0, 128, 0);
	if (theError)
		report_error("Could not initialize Input SprocketÏ.","\p",theError);

	//¥	Turn on the keyboard and mouse handlers
//	  ISpDevices_ActivateClass (kISpDeviceClass_SpeechRecognition);
//take_mouse_keyboard_as_input();
// return result;
return 0;
}

void take_mouse_keyboard_as_input()
{
if (gISPState==1)
	ISpDevices_ActivateClass (kISpDeviceClass_Keyboard);
//	ISpDevices_ActivateClass (kISpDeviceClass_Mouse);
}


void untake_mouse_keyboard_as_input()
{
if (gISPState==1)
	ISpDevices_DeactivateClass (kISpDeviceClass_Keyboard);
//	ISpDevices_DeactivateClass (kISpDeviceClass_Mouse);
}

extern int gCheckWatchDog;
void configure_input_device()
{
UInt32 old_state=gISPState;

   gCheckWatchDog=0;	//disable the watchdog

if (gISPState==0)
{
 enable_inputs();
}

ISpConfigure(0);

if (old_state==0)
{
 disable_inputs();
}
   gCheckWatchDog=1;	//reenable the watchdog

}




void SetKeyboardMouseActivation(Boolean active)
{
enum { kSimple = 100 };
ISpDeviceReference buffer[kSimple];
UInt32 count;

ISpDevices_ExtractByClass(kISpDeviceClass_Mouse, kSimple, &count, buffer);
if (active) 
   { ISpDevices_Activate(count,buffer); }
else 
   { ISpDevices_Deactivate(count, buffer); }

ISpDevices_ExtractByClass(kISpDeviceClass_Keyboard,kSimple,&count, buffer);
if (active) 
   { ISpDevices_Activate(count,buffer); }
else 
   { ISpDevices_Deactivate(count, buffer); }
}


void kill_inputs()
{
gISPState=0;
//untake_mouse_keyboard_as_input();
//ISpSuspend();
//ISpStop();
ISpShutdown();
}


int time_to_resume_isp=0;
UnsignedWide start_micros,end_micros;

void enable_inputs()
{
gISPState=1;



Microseconds(&start_micros);
ISpResume();

Microseconds(&end_micros);
time_to_resume_isp=(end_micros.lo-start_micros.lo);

 take_mouse_keyboard_as_input();

}

void disable_inputs()
{
gISPState=0;
 untake_mouse_keyboard_as_input();
ISpSuspend();

}


int game_fire_button()
{
extern UInt32 gCannon;
extern UInt32 gMissile;
extern UInt32 gLaser;
    poll_input_device();
return ( gCannon || gLaser || gMissile || Button());
}

void ZexGetMouse (Point *the_point)
{
extern	  AGLDrawable aglWindow;

DSpGetMouse (the_point);
SetPort((GrafPort*) aglWindow);
GlobalToLocal (the_point);
//correct for our splat offsets
//the_point->h-=(off_640_x+win_off_x);
//(*the_point).v-=(off_480_y+win_off_y);

}




short isPressed(unsigned short k )
// k =  any keyboard scan code, 0-127
{
	unsigned char km[16];

	GetKeys( (unsigned long *) km);
	return ( ( km[k>>3] >> (k & 7) ) & 1);
}

