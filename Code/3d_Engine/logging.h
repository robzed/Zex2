/*
 *  logging.h
 *  zex22
 *
 *  Created by Stuart Ball on Sun Jul 21 2002.
 *  Copyright (c) 2002 Lightsoft (Games). All rights reserved.
 *
 */

#define __LOGGING__
//I STRONGLY RECOMMEND YOU CLEAN AND REBUILD THE PROJECT IF THIS FILE IS MODIFED.
//A LOT OF IDE'S DO NOT TRACK DEPENDENCIES WELL (INCLUDING ANVIL & PB)
//For example on PB changing this file will not cause all files that should be compiled to be
//compiled!!!!!!!!

//This next lot for debugging primarily...
//These should all be set to 1 for release

#define ENG_DEBUG_EXPORT 1	//various debugging variables are updated in 3d_top
#define RELEASE 0	        //DO set this to 1 for release - if 0 it shows useful debug/cheat stuff in log
#define RT_PRIORITY 0	        //Use real time thread if 1 - should be 1 for release
#define CAPTURE_DISPLAYS 0      //if 1, we capture the display on OS X -- should be 1 for release
#define DESKTOP_BLANKING 0      //if 1 we blank the desktop out to black - should be 1 for release
#define ALIGN_OGL_WIN_CENTRE 0  //aligns ogl window to centre of display, otherwise top/right so I can see the run log

#define NPCS_ON 0               //MS's launch NPC's if 1

//Logging control macros

  #define PB_PRINT_TO_LOG_ENABLED 1 //GLOBAL ON/OFF
//Edit these to suit your debugging requirements. All major systems logging should be defined here
  #define AP_DEBUG_MESSAGES 0   // Autopilot log
  #define ENG_DEBUG_MESSAGES 0 //3D engine log
  #define ECLIPSE_DEBUG_MESSAGES 0 //Eclipse Log
  #define DOCKED_DEBUG_MESSAGES 1 //Docked log
  #define EXEC_DEBUG_MESSAGES 1   //Boot/quit messages
  #define TRADE_LOG 1  //Trading log
  #define SDS_LOG  0   //Ships Data System log
  #define MUSIC_LOG 0  //player pro error logging
  #define LOD_LOG  1
  #define SHOW_OCB_SIZE 1 //should be mod 1024
  #define BUBBLE_LOG 1        //AI bubble killing logging
  #define MS_DOCKING_LOG 1 //
  #define TRADER_LOG 0
  #define MS_LAUNCH_LOG 0  //motherships log what they launch
  #define KILL_CHILD_LOG 1  //children being killed are logged
  #define DYNLOAD_LOG 1     //load_dyn_object logs
//weapons logging
  #define SHOW_VIRTUAL_TRIGGERS 0
  #define EXTREME_LASER_LOGGING 0 //will generate a message for every laser particle created and killed!
  
  #define HEARTBEAT_LOG 1
  
  #if EXEC_DEBUG_MESSAGES
   #define INIT_LOG 1
   #define SPLAT_CONTROL_LOG 1
   #define OGL_LOG 0
  #else
   #define INIT_LOG 0
   #define SPLAT_CONTROL_LOG 0
   #define OGL_LOG 0
  #endif


