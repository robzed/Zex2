// *************************************MODULE HEADER*********************************
// * FILENAME           : Untitled 1
// * MODULE TITLE       : Untitled 1
// * PROJECT            : Zex
// * DATE STARTED       : 19:22:04 on 15th September 1999
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
 * $Log: command_and_control.c,v $
 * Revision 1.1.1.1  2003/09/05 22:37:11  stu_c
 * First Imported.
 *
 * Revision 1.12  2002/09/08 01:57:50  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.11  2002/08/31 19:25:31  stu_c
 * Callsigns
 *
 * Revision 1.10  2002/04/28 06:23:43  stu
 * This checkin adds more to the HUD and brings out some sim variables to the player.
 *
 * Revision 1.9  2002/04/13 15:33:15  stu
 * Rotation, Alycians, behaviour
 *
 * Revision 1.8  2002/03/16 00:42:58  stu
 * Continued development
 *
 * Revision 1.7  2002/01/02 19:29:01  rob
 * Added key setup code
 *
 * Revision 1.6  2001/12/29 01:12:14  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.5  2001/12/27 19:48:12  stu
 * Additional development work for docking
 *
 * Revision 1.4  2001/11/06 20:11:13  rob
 * Key input changes
 *
 * Revision 1.3  2001/11/02 19:32:54  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:28:13  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:09:30  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:08  rob
 * First Imported.
 *
 * Revision 1.3  2001/06/23 20:07:06  stu
 * 0.66 checkin
 *
 * Revision 1.2  2001/06/03 03:16:45  stu
 * OCB structure now contains a sub structure called:
ZObjectControl
Which contains all the control data for an object.

Added accessor in access_ocbs which returns a pointer to type ZObjectControl
 *
 * Revision 1.1.1.1  2001/01/01 21:13:39  rob
 * First Imported.
 *
 * Revision 1.1  2000/10/19 19:07:06  stu
 * Relative velocities, turbulence, planet names.
 *
 * Revision 1.0.0.1  2000/08/21 22:11:06  stu
 * First Imported.
 *
 */

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"
  #include "options.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <NumberFormatting.h>
  #endif
  #include "3d_structures_def.h"
  #include "Error_handler.h"
  #include	"zex_misc.h"
#endif //pch


#include "ship_control.h"
#include	"load_objects.h"
#include	"objects_controls.h"
#include "object_control_top.h"
#include "explode.h"
#include "Zsound.h"
#include "dlp.h"
#include "instrum.h"
#include "ship_kit.h"
#include	"drawing.h"
#include "pictures.h"
#include "game_pause.h"
#include "weapons_damage.h"
#include "camera_control.h"
#include "game_defs.h"
#include "obj_control_misc_routines.h"
#include "light_control.h"
#include "txtsplat.h"
#include "cargo.h"
#include "command_and_control.h"
#include	"splat_control.h"
#include "dlp.h"
#include "keyboard.h"


// ***********************************************************************************
// * CONSTANTS 
// *

#define DX 200
#define DY 100

// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

//we manage upto 4 wingmen
int wingmen_under_command[4];	//-1 if empty else ocb slot #
int command_slot_dialog;
// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *
void render_frame(int width, int height);
void handle_command(LSRAW* cc_pict, LSRAW* command_dialog_pict);
void print_available_info(int x, int y);
void copystr (char *to, char*from);
int process_cc_mouse();
int make_wingman(int slot, int from_where);
int check_under_command(int slot);
void print_current_info(int x, int y);
void release_wingman(int slot);

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

void init_wingmen_under_command()
{
int i;
for (i=0;i<4;i++) wingmen_under_command[i]=-1;
command_slot_dialog=-1;
}

void do_command_and_control()
{
Handle cc_H;
LSRAW *cc_pict;
Handle command_dialog_H;
LSRAW *command_dialog_pict;

_3D light_normal_save;
extern int freeze;
extern _3D light_normal;
//int quit;
//extern int game_running;

command_slot_dialog=-1;

   cc_H=GetZexPicture ('RCZ ',3114);	//Command&Control pilot list
	HLock(cc_H);
	cc_pict = (LSRAW*)*cc_H;

   command_dialog_H=GetZexPicture ('RCZ ',3115);	//Command&Control dialog     
	HLock(command_dialog_H);
	command_dialog_pict = (LSRAW*)*command_dialog_H;

    freeze=1;	//in 3d_top
    light_normal_save=light_normal;
	light_normal.z=1.3;	//dim the scene


ShowCursor();
handle_command(cc_pict,command_dialog_pict);

HideCursor();


    DisposeHandle(cc_H);
    DisposeHandle(command_dialog_H);
    light_normal=light_normal_save;	//restore the lighting
     freeze=0;	//unfreeze engine

}


int done_available_list=0;
int available_slots[5];

void handle_command(LSRAW* cc_pict, LSRAW* command_dialog_pict)
{
int quit;
int prefs_x,prefs_y;
//int old_mode;
int i;
int available_slots_index;
extern	DynObjectsFixedSize *ocb_ptr;
extern int last_pause_frame;
int time_entered;


time_entered=TickCount();	//for K key check - wait 20 ticks


//build list of available pilots
if (done_available_list==0)
     {

	available_slots_index=0;   
	for (i=0;i<5;i++)   available_slots[i]=-1;
	
//scan slots looking for available pilots
      for (i=1;i<MAX_NUMBER_OF_DYN_OBJECTS-1;i++)
       {
          if ((*ocb_ptr).object_list[i].in_use==1)
             {
              if ((*ocb_ptr).object_list[i].Dyn_OCB_control_data.allegiance==HUMAN
                   && (*ocb_ptr).object_list[i].Dyn_OCB_control_data.craft_type==TYPE_FALCON
                   && check_under_command(i)==0)
                 {
                 available_slots[available_slots_index]=i;
                 available_slots_index++;
                 if (available_slots_index>4) break;
                 }
             }
       }

     done_available_list=1;
     }
     

quit=0;
while(quit==0)
{

 if (command_slot_dialog==-1)	//show selector box
 {
 	 //Get_back_buffer();
     render_frame(monitor_w,monitor_h);	//in this file - calls all the needed 3d stuff
 
       prefs_x=DX; prefs_y=DY;
  	  print_crunched_translucent_pict(cc_pict,prefs_x,prefs_y);
  	  print_available_info(prefs_x,prefs_y);
  	  print_current_info (prefs_x,prefs_y);
 //    run_camera_control();;
 //     do_object_control();
 
     Show_it();	//splat 3d portion of window
     
 
     render_frame(monitor_w,monitor_h);	//in this file - calls all the needed 3d stuff
 
  	  print_crunched_translucent_pict(cc_pict,prefs_x,prefs_y);
  	  print_available_info(prefs_x,prefs_y);
  	  print_current_info (prefs_x,prefs_y);
 //     run_camera_control();;
 //     do_object_control();
 	 
    Show_it();	 //splat 3d portion of wind
 //check for keyboard input
    poll_input_device();
    if(gCommandControl && TickCount()>time_entered+20) //gCommandControl is ISp input
    {
     quit=1;
     gCommandControl=0;
     last_pause_frame=TickCount()+20;
    }
 //check for mouse clicks
    quit|=process_cc_mouse();
 
 
 }	 //end of show_command_dialog_for_callsign==0
 
}	//end of while quit

done_available_list=0;
}




void run_control_dialog()
 {
  // OSType ob_type; 
  // Byte *bp;
   char the_string[5];
   int prefs_x,prefs_y;
   extern LSRAW *order_dialog;
  // extern	DynObjectsFixedSize *ocb_ptr;

//#define DX 0
//#define DY 0
       prefs_x=0; prefs_y=0;
  	  print_crunched_translucent_pict(order_dialog,prefs_x,prefs_y);
  	  //draw text
  	      SplatText_Small_Transparent("COMMANDS FOR ",prefs_x+10,prefs_y+3,246);
	//   ob_type =(*ocb_ptr).object_list[command_slot_dialog].Dyn_OCB_control_data.callsign;
	//   bp = (Byte *)&ob_type;
       //the_string[0]=bp[0];
       //the_string[1]=bp[1];
       //the_string[2]=bp[2];
       //the_string[3]=bp[3];
       the_string[4]=0;
  	      SplatText_Small_Transparent(the_string,prefs_x+72,prefs_y+3,246);
          prefs_y+=13;
  	      SplatText_Small_Transparent("1 REPORT STATUS",prefs_x+10,prefs_y,246);
  	      SplatText_Small_Transparent("2 HOLD POSITION",prefs_x+100,prefs_y,246);
  	      SplatText_Small_Transparent("3 FOLLOW ME",prefs_x+200,prefs_y,246);
  	      SplatText_Small_Transparent("4 STAND DOWN",prefs_x+300,prefs_y,246);
  	      SplatText_Small_Transparent("5 CANCEL COMMAND",prefs_x+400,prefs_y,246);

//check for command keys - 1 to 9
    
//check for keyboard input
    if (isPressed(0x12))	//1 key
     {
     //report status
//     (*ocb_ptr).object_list[command_slot_dialog].command=SEND_STATUS;
     command_slot_dialog=-1;
     
     }
     else
    if (isPressed(0x13))	//2 key
     {
     //report status
//     (*ocb_ptr).object_list[command_slot_dialog].command=HOLD_POSITION;
     command_slot_dialog=-1;
     }
     else
    if (isPressed(0x14))	//3 key
     {
     //report status
//     (*ocb_ptr).object_list[command_slot_dialog].command=FOLLOW_ME;
     command_slot_dialog=-1;
     }
     else
    if (isPressed(0x15))	//4 key
     {
     //report status
//     (*ocb_ptr).object_list[command_slot_dialog].command=STAND_DOWN;

     release_wingman(command_slot_dialog);	//remove from our list of pilots under command
     command_slot_dialog=-1;
     }
     else
    if (isPressed(0x17))	//5 key - cancel
     {
     command_slot_dialog=-1;
     }

    
     
 }




//return 1 to quit, else 0
int process_cc_mouse()
{
Point mouse_xy;
int topline,i;

if (Button()==0)
return 0;
//button is down
     ZexGetMouse (&mouse_xy);
if (mouse_xy.h<DX || mouse_xy.h>DX+260 || mouse_xy.v<DY || mouse_xy.v>DY+220) 
    {
      play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
      return 0;
    }
else	//this is button down
{
//in top half of box?
topline=-1;
for (i=0;i<5;i++)
   {
   if (mouse_xy.v>DY+20+(i*20) && mouse_xy.v<DY+39+(i*20))  { topline=i; break; }
   }
   
if (topline!=-1)
   {
   //yes topline
      if(available_slots[topline]!=-1)
          {
          //a ship was available, copy to wingmen slots if one is free
                if (make_wingman(available_slots[topline],topline)==-1) return 1;
                

          }
   }

//try bottom of box, which is pilots under command
topline=-1;
for (i=0;i<4;i++)
   {
   if (mouse_xy.v>DY+140+(i*20) && mouse_xy.v<DY+159+(i*20))  { topline=i; break; }
   }
  
if (topline!=-1)
   {
   //yes topline
      if(wingmen_under_command[topline]!=-1)
          {
           //a wingman was found, store its ocb in 
                command_slot_dialog=wingmen_under_command[topline];
                play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
                     gCommandControl=0;
                     return 1;	//quit

          }
   }
   
}	//end of button down
return 0;
}


//returns 1 if OCB is already under command
int check_under_command(int slot)
{
int i;
//find an empt slot
for (i=0;i<4;i++)
 {
  if (wingmen_under_command[i]==slot) return 1;
 }
 
return 0; 

}

int make_wingman(int slot, int from_where)
{
int i;
extern int rank;
extern	DynObjectsFixedSize *ocb_ptr;

//check we are a higher rank than the wingman
if (rank>(*ocb_ptr).object_list[slot].Dyn_OCB_control_data.rank)
{
//find an empt slot
for (i=0;i<4;i++)
 {
  if (wingmen_under_command[i]==-1)
     {
     wingmen_under_command[i]=slot;
     (*ocb_ptr).object_list[slot].Dyn_OCB_control_data.controller=WINGMAN_FOR_GAME;
//     (*ocb_ptr).object_list[slot].Dyn_OCB_control_data.control_use_9=0;	//init wingman
//     (*ocb_ptr).object_list[slot].command=FOLLOW_ME;

     available_slots[from_where]=-1;
     MyDebugStr(__LINE__,__FILE__,"Wingman return values from this function need debugging");
     return -1;
     }
 }
return 0;
}	//end of rank check
else
{
Handle small_dialog_H;
LSRAW *small_dialog_pict;
int prefs_x,prefs_y;

   small_dialog_H=GetZexPicture ('RCZ ',3117);	//Command&Control dialog     
	HLock(small_dialog_H);
	small_dialog_pict = (LSRAW*)*small_dialog_H;

   while (Button()==1);
   play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
//show no can do box
     render_frame(monitor_w,monitor_h);	//in this file - calls all the needed 3d stuff
 
      prefs_x=256; prefs_y=100;
  	  print_crunched_translucent_pict(small_dialog_pict,prefs_x,prefs_y);
	   SplatText_Transparent ("You can't command",prefs_x+7,prefs_y+20,246);
	   SplatText_Transparent ("that pilot because",prefs_x+7,prefs_y+32,246);
	   SplatText_Transparent ("your rank is too low.",prefs_x+7,prefs_y+44,246);

 
     Show_it();	//splat 3d portion of window
   DisposeHandle(small_dialog_H);
   while (Button()==0);

 return -1;
}
}
#define colour 246

void print_available_info(int x, int y)
{
int i;
char the_string[256];
extern	DynObjectsFixedSize *ocb_ptr;
 // OSType ob_type; 
  Byte *bp;

	for (i=0;i<5;i++)   
	 {
	 if (available_slots[i]!=-1)
	   {
	   //draw ID
	   //ob_type =(*ocb_ptr).object_list[available_slots[i]].Dyn_OCB_control_data.callsign;
	   //bp = (Byte *)&ob_type;
       the_string[0]=bp[0];
       the_string[1]=bp[1];
       the_string[2]=bp[2];
       the_string[3]=bp[3];
       //draw craft type
       the_string[4]=' ';
       the_string[5]='F';
       the_string[6]='a';
       the_string[7]='l';
       the_string[8]='c';
       the_string[9]='o';
       the_string[10]='n';
       the_string[11]=0;
	   SplatText_Transparent (the_string,x+10,y+24,colour);

       fill_rank((*ocb_ptr).object_list[available_slots[i]].Dyn_OCB_control_data.rank,&the_string[0]);
	   SplatText_Transparent (the_string,x+150,y+24,colour);
       
       }
//	else
//	SplatText_Transparent ("None",x+10,y+24,246);
      y+=20;
     }
//RGBForeColor(&rgb
//TextFace(0);
//TextSize(12);
// MoveTo(x+10,y+30);
// DrawString("\pHello");
}




void print_current_info(int x, int y)
{
int i;
char the_string[256];
//  OSType ob_type; 
  Byte *bp;
extern	DynObjectsFixedSize *ocb_ptr;

	for (i=0;i<4;i++)   
	 {
	 if (wingmen_under_command[i]!=-1)
	  {
	   if ((*ocb_ptr).object_list[wingmen_under_command[i]].in_use==0 ||
	   (*ocb_ptr).object_list[wingmen_under_command[i]].Dyn_OCB_control_data.controller!=WINGMAN_FOR_GAME)
	   {
	    wingmen_under_command[i]=-1;
	   }
	   else
	   {
	    //draw ID
	    //ob_type =(*ocb_ptr).object_list[wingmen_under_command[i]].Dyn_OCB_control_data.callsign;
	    //bp = (Byte *)&ob_type;
        the_string[0]=bp[0];
        the_string[1]=bp[1];
        the_string[2]=bp[2];
        the_string[3]=bp[3];
        //draw craft type
        the_string[4]=' ';
        the_string[5]='F';
        the_string[6]='a';
        the_string[7]='l';
        the_string[8]='c';
        the_string[9]='o';
        the_string[10]='n';
        the_string[11]=0;
	    SplatText_Transparent (the_string,x+10,y+144,colour);

        fill_rank((*ocb_ptr).object_list[wingmen_under_command[i]].Dyn_OCB_control_data.rank,&the_string[0]);
	    SplatText_Transparent (the_string,x+150,y+144,colour);
        }
       }
//	else
//	SplatText_Transparent ("None",x+10,y+24,246);
      y+=20;
     }
//RGBForeColor(&rgb
//TextFace(0);
//TextSize(12);
// MoveTo(x+10,y+30);
// DrawString("\pHello");
}





void fill_rank (int rank, char * string)
{

             if (rank==ENS) 
             {
             copystr (string,"Ensign");
             }
             else
             if (rank==LT) 
             {
             copystr(string,"Lieutenant");
             }
             else
             if (rank==LTCR) 
             {
             copystr(string,"LT Commander");
             }
             else
             if (rank==CDR) 
             {
             copystr(string,"Commander");
             }
             else
             if (rank==CAPT) 
             {
             copystr(string,"Captain");
             }
             else
             if (rank==RADM) 
             {
             copystr(string,"Rear Admiral");
             }
             else
             if (rank==VADM) 
             {
             copystr(string,"Vice Admiral");
             }
             else
             if (rank==ADM) 
             {
             copystr(string,"Admiral");
             }
             else
             if (rank==FADM) 
             {
             copystr(string,"Fleet Admiral");
             }
}


//run through OCB's and change any wingmen for game to npc_human
void 
stand_down_wingmen()
{
int i;
extern	DynObjectsFixedSize *ocb_ptr;

      for (i=1;i<MAX_NUMBER_OF_DYN_OBJECTS-1;i++)
       {
          if ((*ocb_ptr).object_list[i].in_use==1)
             {
              if ((*ocb_ptr).object_list[i].Dyn_OCB_control_data.controller==WINGMAN_FOR_GAME)
                 {
                 (*ocb_ptr).object_list[i].Dyn_OCB_control_data.controller=NPC_CONTROL;	//revert to normal control
//                      (*ocb_ptr).object_list[i].Dyn_OCB_control_data.current_mode=1;	//back to normal mode
                 }
             }
       }
//now clear the under_command array
	for (i=0;i<4;i++)   
	 wingmen_under_command[i]=-1;

}

void release_wingman(int slot)
{
int i;
 for (i=0;i<4;i++)
 {
 if (wingmen_under_command[i]==slot) wingmen_under_command[i]=-1;
 
 }
}

