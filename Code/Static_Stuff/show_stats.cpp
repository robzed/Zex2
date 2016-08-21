// *************************************MODULE HEADER*********************************
// * FILENAME           : show_stats.cpp
// * MODULE TITLE       : show_stats.c
// * PROJECT            : Zex
// * DATE STARTED       : 14:21:40 on 21st June 1999
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
/* ***********************************************************************************
// $Log: show_stats.cpp,v $
// Revision 1.4  2003/09/28 17:29:38  robp
// Changed files from .c to .cpp and removed spaces out of a couple of filenames.
//
// Revision 1.3  2003/09/26 19:20:49  robp
// Alteration for C++ const means internal unless specified extern. Made header extern so that C compiler wouldn't complain.
//
// Revision 1.2  2003/09/20 12:57:15  robp
// Removed nested comments
//
// Revision 1.1.1.1  2003/09/05 22:37:13  stu_c
// First Imported.
//
// Revision 1.13  2002/09/08 02:20:40  stu_c
// Precompiled header adjustments
//
// Revision 1.12  2002/07/28 17:12:45  stu_c
// Exhausts and further particle work
//
// Revision 1.11  2002/04/13 15:33:03  stu
// Rotation, Alycians, behaviour
//
// Revision 1.10  2002/02/02 23:11:33  stu
// i_know_what_im_doing_kill param changes
//
// Revision 1.9  2001/12/29 01:12:47  stu
// Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
//
// Revision 1.8  2001/12/27 19:48:32  stu
// Additional development work for docking
//
// Revision 1.7  2001/12/04 22:57:38  rob
// sin tab proto removed
//
// Revision 1.6  2001/11/24 19:30:03  stu
// *** empty log message ***
//
// Revision 1.5  2001/11/02 19:33:04  rob
// Build variants for interface lib and carbon.
Coffee fixes after changes for X.
//
// Revision 1.4  2001/10/24 21:41:01  stu
// First set of source to build on pb for carbon.
//
// Revision 1.3  2001/10/23 21:54:31  stu
// More carbon work
//
// Revision 1.2  2001/10/22 21:29:24  rob
// Carbon warning changes
//
// Revision 1.1  2001/10/21 01:13:36  stu
// Initial porting work
//
// Revision 1.0.0.1  2001/10/17 20:46:08  rob
// First Imported.
//
// Revision 1.6  2001/09/04 22:42:39  stu
// SDS
//
// Revision 1.5  2001/06/23 20:11:06  stu
// 0.66 checkin
//
// Revision 1.4  2001/06/03 03:22:32  stu
// OCB structure now contains a sub structure called:
ZObjectControl
Which contains all the control data for an object.

Added accessor in access_ocbs which returns a pointer to type ZObjectControl
//
// Revision 1.3  2001/05/28 01:54:18  stu
// 280501
//
// Revision 1.2  2001/04/01 18:24:29  stu
// *** empty log message ***
//
// Revision 1.1.1.1  2001/01/01 21:13:39  rob
// First Imported.
//
// Revision 1.7  2000/12/10 03:37:11  stu
// Calls to kill replaced with either calls to add_to_kill_list OR
i_know_what_im_doing_kill

Old lighting calls removed
//
// Revision 1.6  2000/12/03 23:52:29  rob
// changed constants for object type to get uniformity
//
// Revision 1.5  2000/11/28 22:30:28  stu
// Playing time to months, weeks, days etc.
Crash fix.
//
// Revision 1.4  2000/11/16 20:45:18  rob
// orbit
//
// Revision 1.3  2000/09/20 23:52:15  stu
// HUD/AP/Scaling
//
// Revision 1.2  2000/08/27 19:43:08  rob
// removed day night light movement code
//
// Revision 1.1  2000/08/27 01:35:24  stu
// Hud changes
//
// Revision 1.0.0.1  2000/08/21 22:11:29  stu
// First Imported.
//
 */

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "options.h"
  #include "LS_Standard.h"
  #include "game_defs.h"

#include "Error_handler.h"
#include "zex_misc.h"

#endif //pch

#include "Zsound.h"
#include "show_stats.h"
#include "pictures.h"
#include "dust.h"
#include	"splat_control.h"
#ifndef __3d_structures__  //might be from pch
#include "3d_structures_def.h"
#endif
#include "load_objects.h"
#include "objects_controls.h"

#include "fpoly4.h"
#include "ship_control.h"
#include "zex_misc.h"
#include "txtsplat.h"
#include "camera_control.h"



#include "object_control_top.h"

#include "dlp.h"
#include "drawing.h"
#include "ogl.h"
#include "keyboard.h"


// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

int handle_stats_mouse(LSRAW* button_down, LSRAW* button_up);

int accumulated_game_time;	//from load game else 0
int start_time;	//time this session started
int stop_time;
int showing_stats;	//if 1 stops all monster controllers from firing
extern int frame_counter;
static int reached_end_of_epilogue;
static int epilogue_colour,epilogue_colour_delta;
//credits vars
extern int real_frame_counter;

extern int user_abort;
extern int docked_at;
extern int score,credits,start_time;
extern int do_launch;
extern int ship_z_ang;
extern int launch_timer;


int score_this_level,credits_this_level,time_this_level,monsters_killed_this_level;
int dock_fee;
// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

void render_frame(int width, int height);

void ShowWhyWeDied();

void setup_stats_screen(LSRAW* logo,int dock_fee);




void show_stats()
{
//Handle button_data_H;
//LSRAW *button;
#if PORTABLE_FILESYSTEM
ZexPicture *logo_pic_obj;
#else
Handle logo_H;
#endif
LSRAW *logo;
//Handle button_down_data_H;
//LSRAW *button_down;
//extern Ptr screenaddr;
//extern int screenwidth;

//extern int zex_alive;
extern	DynObjectsFixedSize *ocb_ptr;
extern int thrust;
//extern int zex_alive;
//extern int current_level;
//extern int	dynamic_object_index;
extern int disable_camera_control;
extern DSpContextReference		mDisplayContext;
int dust_count=0;

UnsignedWide start_micros;
//UnsignedWide end_micros;

//CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;
load_dyn_object_pb the_params;
//int i; 
int fade_percent;
extern int dock_fee,game_over;

ogl_inactive();

if (docked_at!=-1) credits=credits-dock_fee;

disable_camera_control=0;

clear_dlp();	
init_dlp();	//clear it all

frame_counter=0;
reached_end_of_epilogue=0;
epilogue_colour=226;
epilogue_colour_delta=1;
//real_frame_counter=0;

//make zex dead so we dont get shot at
//copy zex
	the_params.world_x=(*ocb_ptr).object_list[0].Oworldx;
	the_params.world_y=(*ocb_ptr).object_list[0].Oworldy;
	the_params.world_z=-200;
	the_params.delta_rot_x=0;
	the_params.delta_rot_y=0;
	the_params.delta_rot_z=0;
	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=270;
	the_params.controller_ref=zex;
	the_params.control_use_1=(*ocb_ptr).object_list[0].Dyn_OCB_control_data.control_use_1;
	the_params.control_use_2=(*ocb_ptr).object_list[0].Dyn_OCB_control_data.control_use_2;
	the_params.control_use_3=(*ocb_ptr).object_list[0].Dyn_OCB_control_data.control_use_3;
	the_params.control_use_4=0;
	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	the_params.shield_value=(*ocb_ptr).object_list[0].Dyn_OCB_control_data.shield_value;
	the_params.mass=100;
	the_params.dq=0;
	the_params.object_category=HUMAN;
//	the_params.laser_cat=(*ocb_ptr).object_list[0].Dyn_OCB_control_data.laser_category;
//	the_params.cannon_cat=(*ocb_ptr).object_list[0].Dyn_OCB_control_data.cannon_category;
//	the_params.missile_cat=(*ocb_ptr).object_list[0].Dyn_OCB_control_data.missile_category;
//	the_params.number_of_missiles=(*ocb_ptr).object_list[0].Dyn_OCB_control_data.number_of_missiles;
	 the_params.unique_id=0;
i_know_what_im_doing_kill(zex, TRUE);

while (game_fire_button()==1);

showing_stats=1;	//stop everything firing

stop_time=TickCount();

    #if PORTABLE_FILESYSTEM
    if (game_over==1)
    logo_pic_obj = new ZexPicture('RCZ ',143);	//zex logo with game over over it.    
    else
    logo_pic_obj = new ZexPicture('RCZ ',130);	//zex logo.    
    
    logo = logo_pic_obj->GetPictureRef();
    #else
    if (game_over==1)
    logo_H=GetZexPicture ('RCZ ',143);	//zex logo with game over over it.    
    else
    logo_H=GetZexPicture ('RCZ ',130);	//zex logo.    
    
    HLock (logo_H);
    logo=(LSRAW*)*logo_H;
    #endif


//set up drawing env.
    set_poly_clipping(0,monitor_w-1,0,monitor_h-1);	//set poly clip rect - sb 181298
    DSpContext_FadeGamma ( mDisplayContext, 100,
                               0);
                               
    kill_all_dust();
    
    while(game_fire_button()==0)
    {

      Microseconds(&start_micros);	 //equalise
//    (*ocb_ptr).object_list[0].Omovedy=0;
//    (*ocb_ptr).object_list[0].Omovedx=0;	  //stop zex from moving
    thrust=0;	//just in case


    run_camera_control();;
    render_frame(monitor_w-1,monitor_h-1);	//calls all the needed 3d stuff

//print the strings
    setup_stats_screen(logo,dock_fee);

dust_count--;
if (dust_count<0 && game_over==1)
 {
 dust_count=3;	//every 300 frames, gen a new dust
 gen_dust_for_end();
 } 

    do_object_control();
       Show_it();
    

    }	//end of gcannon
    



//now we do a fade out
    fade_percent=100;
    while(fade_percent>10)
    {

      Microseconds(&start_micros);	 //equalise
//     (*ocb_ptr).object_list[0].Omovedy=0;
//    (*ocb_ptr).object_list[0].Omovedx=0;	  //stop zex from moving
    thrust=0;	//just in case
    DSpContext_FadeGamma ( mDisplayContext, fade_percent,
                               0);
    fade_percent-=5;

    erase_zex_rect(monitor_w,monitor_h);	//in pictures

    run_camera_control();;
    render_frame(monitor_w-1,monitor_h-1);	//calls all the needed 3d stuff

//print the strings
//    setup_stats_screen(logo);

dust_count--;
if (dust_count<0 && game_over==1)
 {
 dust_count=8;	//every 300 frames, gen a new dust
 gen_dust_for_end();
 } 

    do_object_control();
       Show_it();
    

    }	//end of while fade-percent

//get balck screen


//	reset_objects();	
	
	
//    setup_stats_screen(button,logo);
//    setup_stats_screen(button,logo);
//    setup_stats_screen(button,logo);
//    while (handle_stats_mouse(button_down, button)==0);

    #if PORTABLE_FILESYSTEM
    delete logo_pic_obj;
    #else
    DisposeHandle(logo_H);
    #endif
  while (game_fire_button()==1);


       Show_it();

//now we have to load zex back in
     the_params.parent=-1;
	load_dyn_object(zex,0,&the_params,-1,1,-1,!DUST_BIT);	  //the object, the position (-1=next free)

    DSpContext_FadeGamma ( mDisplayContext, 100,
                               0);

}


void ShowWhyWeDied()
{
extern char end_of_level_reason_string[512];	//we can print this if eolc is not zero
char the_char=-1;
int count=0;
int text_x=0;

  while(the_char!=0 && count<255)
  {
    the_char=end_of_level_reason_string[count];
    count++;
  }
//calc text x - center the string
//the font is 8 pixels wide
  text_x=(count>>1)*8;
  text_x=screen_cent_x-text_x;
  if (text_x<0) text_x=0;

//       SplatTextLarge_Transparent (end_of_level_reason_string,120,160);
       SplatText_Transparent (end_of_level_reason_string,text_x,20,10);;
}



char epilogue[] =
"Incoming message. Callsign not known...\n\n\
And so it was that Mankind succumbed to the evil that was the R'mparians.\n\
Children cried, women prayed and men fought but it was to no avail. Eventually the\n\
R'mparians wore down Earth's resources. Slowly but surely over a period exceeding\n\
one hundred Earth years, as more Aliens arrived, the Human population dwindled.\n\
\nEventually, in 2112 the last few Humans were brought before the Grand R'mparian\n\
'Zandar'. Medieval torture and gross humiliation preceded the final Human death.\n\n\
Zex had failed.";

void setup_stats_screen(LSRAW* logo,int dock_fee)
{
extern int game_over,end_of_level_code;

unsigned char number_string[12];
char number_string1[12];
unsigned int alive_time, years, months, weeks, days, hours,mins,secs;
int i;
extern int game_over;
char line_str[256];
 char the_char;
 int linepos;
 int ypos=280;
	print_crunched_trans_pict(logo,off_640_x+160, 0);
//now print the epilogue text



if (game_over==1 && user_abort!=1)	//we died for real and didn't abort
{
   i=0;
   next_line: linepos=0;
   copy_line:
   //copy each line then splat it
    the_char=epilogue[i];
    if (the_char==0)
    {
    reached_end_of_epilogue=1;
    }
    
    if (i==frame_counter) goto eof;
    line_str[linepos]=the_char;
    i++;
    linepos++;
    
    if (the_char==13) goto eol;
    goto copy_line;
    
   eol:
    line_str[linepos-1]=0;	//delete the cr
    line_str[linepos]=0;
    SplatText_Transparent ((char *)line_str,0,ypos,246);
    ypos+=14;
    goto next_line;
    
   eof:
    line_str[linepos]=0;
    SplatText_Transparent ((char *)line_str,0,ypos,216);
    
    if (reached_end_of_epilogue==0)
    frame_counter++;
        
   //	   print_crunched_trans_pict(button,260, 400);
   //	   SplatTextLarge_Transparent ("OK",320,400,255);
   //show vital stats
}

real_frame_counter++;
//epilogue_colour+=epilogue_colour_delta;
//if (epilogue_colour>230) epilogue_colour_delta=-epilogue_colour_delta;
//else
//if (epilogue_colour<226) epilogue_colour_delta=-epilogue_colour_delta;

       if (end_of_level_code!=0 && game_over==1) ShowWhyWeDied();

#define SCOREY 150
#define CREDY 190
#define TIMEY 230
//#define SCOREX 120
#define SCOREX 10
#define DATAX 120

	SplatTextLarge_Transparent ("RANK",off_640_x+46,SCOREY);
	SplatTextLarge_Transparent ("CREDITS",off_640_x+0,CREDY);
	SplatTextLarge_Transparent ("TIME",off_640_x+54,TIMEY);

//NumToString(score,number_string);
//number_string[number_string[0]+1]=0;	//quick p to c hack
//number_string[0]=' ';
//SplatTextLarge_Transparent ((char *)number_string,150,SCOREY);


//    NumToString((int)(score-score_this_level),number_string);
//    number_string[number_string[0]+1]=0;	  //quick p to c hack
//    number_string[0]=' ';
//    SplatTextLarge_Transparent ((char *)number_string,360,SCOREY);

NumToString(credits,number_string);
number_string[number_string[0]+1]=0;	//quick p to c hack
number_string[0]=' ';
SplatTextLarge_Transparent ((char *)number_string,off_640_x+150,CREDY);

//    NumToString((int)(credits-credits_this_level),number_string);
//    number_string[number_string[0]+1]=0;	  //quick p to c hack
//    number_string[0]=' ';
//    SplatTextLarge_Transparent ((char *)number_string,360,CREDY);

alive_time=(stop_time-start_time)+accumulated_game_time;
years=alive_time/(12*4*7*24*60*60*60);
alive_time-=years*(12*4*7*24*60*60*60);

months=alive_time/(4*7*24*60*60*60);
alive_time-=months*(4*7*24*60*60*60);

weeks=alive_time/(7*24*60*60*60);
alive_time-=weeks*(7*24*60*60*60);

days=alive_time/(24*60*60*60);
alive_time-=days*(24*60*60*60);

hours=alive_time/(60*60*60);
alive_time-=hours*(60*60*60);

mins=alive_time/(60*60);
alive_time-=mins*(60*60);

secs=alive_time/60;

//271100 - stop crashing if alive for > 99 hours
if(months>99) 
{
 months=99;
 weeks=3;
 days=6;
 hours=23;
 mins=59;
 secs=59;
}

#define TIMEX 173
num_to_string99_suppress_leading_zero(months,number_string1);	//months
SplatTextLarge_Transparent ((char *)number_string1,off_640_x+TIMEX,TIMEY);
	if(months!=1)
	SplatTextLarge_Transparent ("MONTHS",off_640_x+TIMEX+48,TIMEY);
	else
	SplatTextLarge_Transparent ("MONTH",off_640_x+TIMEX+48,TIMEY);

num_to_string99_suppress_leading_zero(weeks,number_string1);	//weeks
SplatTextLarge_Transparent ((char *)number_string1,off_640_x+TIMEX,TIMEY+40);
	if(weeks!=1)
	SplatTextLarge_Transparent ("WEEKS",off_640_x+TIMEX+48,TIMEY+40);
	else
	SplatTextLarge_Transparent ("WEEK",off_640_x+TIMEX+48,TIMEY+40);
	
num_to_string99_suppress_leading_zero(days,number_string1);	//days
SplatTextLarge_Transparent ((char *)number_string1,off_640_x+TIMEX,TIMEY+80);
	if(days!=1)
	SplatTextLarge_Transparent ("DAYS",off_640_x+TIMEX+48,TIMEY+80);
	else
	SplatTextLarge_Transparent ("DAY",off_640_x+TIMEX+48,TIMEY+80);
	
//	
num_to_string99_suppress_leading_zero(hours,number_string1);	//hours
SplatTextLarge_Transparent ((char *)number_string1,off_640_x+TIMEX,TIMEY+120);
	if(hours!=1)
	SplatTextLarge_Transparent ("HOURS",off_640_x+TIMEX+48,TIMEY+120);
	else
	SplatTextLarge_Transparent ("HOURS",off_640_x+TIMEX+48,TIMEY+120);
	
num_to_string99_suppress_leading_zero(mins,number_string1);	//mins
SplatTextLarge_Transparent ((char *)number_string1,off_640_x+TIMEX,TIMEY+160);
	if(mins!=1)
	SplatTextLarge_Transparent ("MINUTES",off_640_x+TIMEX+48,TIMEY+160);
	else
	SplatTextLarge_Transparent ("MINUTE",off_640_x+TIMEX+48,TIMEY+160);
	



if (game_over==0)
{

  if (docked_at!=-1) 
  {

  
  SplatText_Transparent ("DOCKING TOLL:",248,440,33);
  //calc docking toll
    NumToString((int)(dock_fee),number_string);
    number_string[number_string[0]+1]=0;	//quick p to c hack
    number_string[0]=' ';
    SplatText_Transparent ((char *)number_string,332,440,33);
  
  
  }
//  if (can_do_next_level==1) SplatText_Transparent ("MISSION COMPLETE",230,126,228);

}


SplatText_Transparent ("PRESS FIRE TO CONTINUE",off_640_x+225,off_480_y+460,33);


//	Show_it();

}


int handle_stats_mouse(LSRAW* button_down, LSRAW* button_up)
{
//extern	  DSpContextReference		          mDisplayContext;

//extern Ptr screenaddr;
//extern int screenwidth;

//CGrafPtr the_drawing_buffer;


int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val;
//int i;
//unsigned char number_string[2] = {0,0};
extern int old_button_state;
//extern int quit;
//copy bg for draw_bg

    ZexGetMouse (&mouse_xy);
//    DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
    button_state=Button();
        print_crunched_pict(button_up,260,400);
//now draw the down one
     if (mouse_xy.v>400 && mouse_xy.v<400+36 )	  //controls
     {
     print_crunched_pict(button_down,260,400);
     return_val=1;
     }
    
////check for keyboard shortcuts
if (check_key(0x24)) { real_return_val=1; goto end; }	//return key
if (check_key(0x4c)) { real_return_val=1; goto end; }	//enter key
//if (check_key(0x08)) { real_return_val=6; goto end; }	//C key
//if (check_key(0x0C)) { real_return_val=7; goto end; }	//Q key


if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;
old_button_state=button_state;

end:
Show_it();
return real_return_val;
}

