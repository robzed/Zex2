/* game_pause.cpp
// SB 190199

// $Log: game_pause.cpp,v $
// Revision 1.4  2003/09/28 10:36:08  robp
// Signed/Unsigned comparison fixes from last night, plus collision fix.
//
// Revision 1.3  2003/09/26 19:20:47  robp
// Alteration for C++ const means internal unless specified extern. Made header extern so that C compiler wouldn't complain.
//
// Revision 1.2  2003/09/20 12:57:09  robp
// Removed nested comments
//
// Revision 1.1.1.1  2003/09/05 22:35:44  stu_c
// First Imported.
//
// Revision 1.9  2002/09/08 02:19:25  stu_c
// Precompiled header adjustments
//
// Revision 1.8  2002/07/28 17:13:14  stu_c
// Exhausts and further particle work
//
// Revision 1.7  2002/05/31 23:56:02  stu
// Removed all warnings from the code
//
// Revision 1.6  2002/02/24 03:40:14  stu
// Some compiler warnings fixed
//
// Revision 1.5  2001/12/29 01:12:32  stu
// Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
//
// Revision 1.4  2001/11/02 19:32:59  rob
// Build variants for interface lib and carbon.
Coffee fixes after changes for X.
//
// Revision 1.3  2001/10/24 21:42:49  stu
// First set of source to build on pb for carbon.
//
// Revision 1.2  2001/10/22 21:29:10  rob
// Carbon warning changes
//
// Revision 1.1  2001/10/21 00:57:44  stu
// Initial port work
//
// Revision 1.0.0.1  2001/10/17 20:46:05  rob
// First Imported.
//
// Revision 1.3  2001/06/03 00:25:21  stu
// Fix for dialogs not showing. Also affects finder and Anvil console commands
//
// Revision 1.2  2001/05/28 01:55:27  stu
// 280501
//
// Revision 1.1.1.1  2001/01/01 21:13:33  rob
// First Imported.
//
// Revision 1.2  2000/10/08 21:46:04  stu
// Engine conversion to 64 bit. Bitch of a job.
//
// Revision 1.1  2000/09/20 23:52:39  stu
// HUD/AP/Scaling
//
// Revision 1.0.0.1  2000/08/21 21:58:26  stu
// First Imported.
//
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h" 
  #include "LS_Standard.h"
  #include "options.h"
  #include "game_defs.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <mactypes.h>
    #include <files.h>
    #include <events.h>
    #include <timer.h>
    #include <memory.h>
  #endif

  #include "Error_handler.h" 
  #include "zex_misc.h"
  #include "3d_structures_def.h"
  #include "processor.h"

#endif //pch


#include "file_io.h"
#include "pictures.h"
#include	"splat_control.h"
#include	"txtsplat.h"

#include "ship_control.h"
#include "game_pause.h"
#include "object_control_top.h"
#include "camera_control.h"
#include "dlp.h"
#include "main_sel_screen.h"
#include "keyboard.h"

//proto
void render_frame(int width, int height);
int handle_message_mouse(LSRAW* button_down, LSRAW* button_up);
void setup_screenQS(LSRAW* the_picture, LSRAW* the_button);
int handle_pause_mouseQS(LSRAW* button_down, LSRAW* button_up);
int handle_diff_mouse(LSRAW* button_down, LSRAW* button_up);
int handle_train_mouse(LSRAW* button_down, LSRAW* button_up);

int button_y1_array[3] = {10,47,82};

int quicksave_available,execute_quicksave;
extern int game_running;

void do_game_pause()
{
//extern FSSpec Zex_FSSpec;
extern int zex_alive,death_frame_counter;
extern _3D light_normal;
//extern Ptr screenaddr;
//extern int screenwidth;
//extern int thrust;
extern int freeze;
//extern int ask_for_load
//extern int done_load_flag
extern int user_abort;
extern int tutorial;

_3D light_normal_save;

Handle prefs_screen_H;
Handle button_data_H;
Handle button_down_data_H;
//extern CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;

//FSSpec the_file;
LSRAW *prefs_screen;
LSRAW *button;
LSRAW *button_down;
//int load_result;
int wait_flag,player_click;
    light_normal_save=light_normal;
 
if (1)	//used to be quicksave in zex 1 - gone in 2! 
{ 
    prefs_screen_H=GetZexPicture ('RCZ ',139);	//in_game_prefs.rcz     
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"\pin_game_prefs.RCZ",&the_file);
//	prefs_screen_H=read_file(the_file);
	HLock(prefs_screen_H);
	prefs_screen = (LSRAW*)*prefs_screen_H;
	
    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up   
	HLock(button_data_H);
	button = (LSRAW*)*button_data_H;
	
    button_down_data_H=GetZexPicture ('RCZ ',134);	//prefs_button_down1   
	HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;

	light_normal.z=1.3;	//dim the scene
    freeze=1;	//in 3d_top
    render_frame(monitor_w,monitor_h);	//in this file - calls all the needed 3d stuff
    
    run_dlp();
//    run_dlp_text();
//    do_control_panel_control();
    setup_screen(prefs_screen,button);
Show_it();	//splat 3d portion of window
    

    render_frame(monitor_w,monitor_h);	//in this file - calls all the needed 3d stuff

    run_dlp();
//    run_dlp_text();
//    do_control_panel_control();
    setup_screen(prefs_screen,button);
Show_it();	//splat 3d portion of wind

//¥¥¥main title screen display loop
    ShowCursor();
    wait_flag=0;
	while (wait_flag==0)	//until mouse control says play
    {


     player_click=handle_pause_mouse(button_down,button);
     Show_it();	//splat 3d portion of window
     if (player_click==1) 
     {
     wait_flag=1;	 //continue		 
     freeze=0;	//unfreeze engine
     }
    else
     if (player_click==2)	//abort - leave engine frozen
        {
        wait_flag=1;
        zex_alive=0;
        user_abort=1;
        death_frame_counter=1;
        }
    }

    if (tutorial==0) HideCursor();
//    HideCursor();
    DisposeHandle(prefs_screen_H);
    DisposeHandle(button_data_H);
    DisposeHandle(button_down_data_H);
    
//    enable_inputs();
    light_normal=light_normal_save;	//restore the lighting
 }	//end of cant quicksave
}

int do_game_restore()
{
//extern FSSpec Zex_FSSpec;
//extern int zex_alive,death_frame_counter;
extern _3D light_normal;
//extern Ptr screenaddr;
//extern int screenwidth;
//extern int thrust;
extern int freeze;
//extern int ask_for_load,done_load_flag,user_abort;

_3D light_normal_save;

Handle prefs_screen_H;
Handle button_data_H;
Handle button_down_data_H;
//extern CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;

//FSSpec the_file;
LSRAW *prefs_screen;
LSRAW *button;
LSRAW *button_down;
//int load_result;
int wait_flag,player_click,return_val=0;
    light_normal_save=light_normal;
 
 
 
    prefs_screen_H=GetZexPicture ('RCZ ',146);	//restore/quit     
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"\pin_game_prefs.RCZ",&the_file);
//	prefs_screen_H=read_file(the_file);
	HLock(prefs_screen_H);
	prefs_screen = (LSRAW*)*prefs_screen_H;
	
    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up   
	HLock(button_data_H);
	button = (LSRAW*)*button_data_H;
	
    button_down_data_H=GetZexPicture ('RCZ ',134);	//prefs_button_down1   
	HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;

	light_normal.z=1.3;	//dim the scene
    freeze=1;	//in 3d_top
    render_frame(monitor_w,monitor_h);	//in this file - calls all the needed 3d stuff
    
    run_dlp();
//    run_dlp_text();
//    do_control_panel_control();
    setup_screen(prefs_screen,button);
Show_it();	//splat 3d portion of window
    

    render_frame(monitor_w,monitor_h);	//in this file - calls all the needed 3d stuff

//    render_frame(640,390);	  //in this file - calls all the needed 3d stuff
    run_dlp();
//    run_dlp_text();
//    do_control_panel_control();
    setup_screen(prefs_screen,button);
Show_it();	//splat 3d portion of wind

//¥¥¥main title screen display loop
    ShowCursor();
    wait_flag=0;
	while (wait_flag==0)	//until mouse control says play
    {


     player_click=handle_pause_mouse(button_down,button);
     Show_it();	//splat 3d portion of window
     if (player_click==1) 
     {
     wait_flag=1;	 //continue		 
     return_val=1;	//yes do restore
     }
    else
     if (player_click==2)	//abort - leave engine frozen
        {
        wait_flag=1;
        return_val=0;
        }
    }

    HideCursor();
    DisposeHandle(prefs_screen_H);
    DisposeHandle(button_data_H);
    DisposeHandle(button_down_data_H);
    
//    enable_inputs();
    light_normal=light_normal_save;	//restore the lighting
return return_val;
}




void do_game_dead()
{
//extern FSSpec Zex_FSSpec;
//extern int zex_alive,death_frame_counter;
extern _3D light_normal;
//extern Ptr screenaddr;
//extern int screenwidth;
//extern int thrust;
extern int freeze,showing_stats;
extern	DynObjectsFixedSize *ocb_ptr;


_3D light_normal_save;
UnsignedWide start_micros;
//UnsignedWide end_micros;

Handle prefs_screen_H;
//Handle button_data_H;
//Handle button_down_data_H;
//CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;

//FSSpec the_file;
LSRAW *prefs_screen;
int button_state;


    light_normal_save=light_normal;
  
    prefs_screen_H=GetZexPicture ('RCZ ',2009);	//deaded.rcz     
	HLock(prefs_screen_H);
	prefs_screen = (LSRAW*)*prefs_screen_H;
	

	light_normal.z=1.3;	//dim the scene
showing_stats=1;	//stop everything firing
     freeze=1;	 //in 3d_top   
    render_frame(monitor_w,monitor_h);	//in this file - calls all the needed 3d stuff
    print_crunched_trans_pict(prefs_screen,screen_cent_x-150,100);
    Show_it();
    render_frame(monitor_w,monitor_h);	//in this file - calls all the needed 3d stuff
    print_crunched_trans_pict(prefs_screen,screen_cent_x-150,100);
//    render_frame(640,390);	  //in this file - calls all the needed 3d stuff
    Show_it();


//wait for fire to come up
    button_state=1;
    while (button_state==1) button_state=game_fire_button();
    //and down
    button_state=0;
    while (button_state==0) 
    {
    Microseconds(&start_micros);	//equalise

    

     render_frame(monitor_w,monitor_h);	//in this file - calls all the needed 3d stuff
     print_crunched_trans_pict(prefs_screen,screen_cent_x-150,100);
//    render_frame(640,390);	  //in this file - calls all the needed 3d stuff
 //  Show_it();
    run_camera_control();;
     do_object_control();
   Show_it();

    button_state=game_fire_button();
    }
showing_stats=0;	//stop everything firing
    freeze=0;	//in 3d_top
    DisposeHandle(prefs_screen_H);
    light_normal=light_normal_save;	//restore the lighting
    (*ocb_ptr).object_list[0].invincible_count=250;	//zex is invincible whilst this is >0

}


//displays a dialog with just OK
void do_game_alert(char * the_string)
{
//extern FSSpec Zex_FSSpec;
//extern int zex_alive,death_frame_counter;
extern _3D light_normal;
//extern Ptr screenaddr;
//extern int screenwidth;
//extern int thrust;
extern int freeze;
//extern int ask_for_load,done_load_flag,user_abort;

_3D light_normal_save;

Handle prefs_screen_H;
Handle button_data_H;
Handle button_down_data_H;
//extern CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;

//FSSpec the_file;
LSRAW *prefs_screen;
LSRAW *button;
LSRAW *button_down;
//int load_result;
int wait_flag,player_click;
int prefs_x,prefs_y;

    light_normal_save=light_normal;
  
    prefs_screen_H=GetZexPicture ('RCZ ',144);	//message     
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"\pin_game_prefs.RCZ",&the_file);
//	prefs_screen_H=read_file(the_file);
	HLock(prefs_screen_H);
	prefs_screen = (LSRAW*)*prefs_screen_H;
	
    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up   
	HLock(button_data_H);
	button = (LSRAW*)*button_data_H;
	
    button_down_data_H=GetZexPicture ('RCZ ',134);	//prefs_button_down1   
	HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;

	light_normal.z=1.3;	//dim the scene
    freeze=1;	//in 3d_top
    render_frame(monitor_w,monitor_h);	   //in this file - calls all the needed 3d stuff
    
    run_dlp();
//    run_dlp_text();
//    do_control_panel_control();
//    setup_screen(prefs_screen,button);
//    prefs_x=250; prefs_y=100;
   prefs_x=screen_cent_x-70;
   prefs_y=screen_cent_y-140;

//    prefs_x=250; prefs_y=100;
    
	print_crunched_trans_pict(prefs_screen,prefs_x,prefs_y);
//	print_crunched_trans_pict(button,0+prefs_x, button_y1_array[0]+prefs_y);

   Show_it();	//splat 3d portion of window
    

    render_frame(monitor_w,monitor_h);	   //in this file - calls all the needed 3d stuff

//    render_frame(640,390);	  //in this file - calls all the needed 3d stuff
    run_dlp();
//    run_dlp_text();
//    do_control_panel_control();
//    setup_screen(prefs_screen,button);
//    prefs_x=250; prefs_y=100;
   prefs_x=screen_cent_x-70;
   prefs_y=screen_cent_y-140;
   
	print_crunched_trans_pict(prefs_screen,prefs_x,prefs_y);

    Show_it();	//splat 3d portion of wind

//¥¥¥main title screen display loop
    ShowCursor();
    wait_flag=0;
	while (wait_flag==0)	//until mouse control says play
    {


     player_click=handle_message_mouse(button_down,button);
     Show_it();	//splat 3d portion of window
     if (player_click==1) 
     {
     wait_flag=1;	 //continue		 
     freeze=0;	//unfreeze engine
     }
    }

    HideCursor();
    DisposeHandle(prefs_screen_H);
    DisposeHandle(button_data_H);
    DisposeHandle(button_down_data_H);
    
//    enable_inputs();
    light_normal=light_normal_save;	//restore the lighting
}



void setup_screen(LSRAW* the_picture, LSRAW* the_button)
{
extern int button_y1_array[3];

int prefs_x,prefs_y;
   prefs_x=screen_cent_x-70;
   prefs_y=screen_cent_y-140;
   
//    prefs_x=250; prefs_y=100;

	print_crunched_trans_pict(the_picture,prefs_x,prefs_y);
	print_crunched_trans_pict(the_button,0+prefs_x, button_y1_array[0]+prefs_y);
	print_crunched_trans_pict(the_button,0+prefs_x, button_y1_array[1]+prefs_y);
//	print_crunched_trans_pict(the_button,0+prefs_x, button_y1_array[2]+prefs_y);	//load gam removed - SB 4/4/99

//	Show_it();

}

void setup_screenQS(LSRAW* the_picture, LSRAW* the_button)
{
extern int button_y1_array[3];

int prefs_x,prefs_y;
    prefs_x=100; prefs_y=100;

	print_crunched_trans_pict(the_picture,prefs_x,prefs_y);
	print_crunched_trans_pict(the_button,0+prefs_x+80, button_y1_array[0]+prefs_y);
	print_crunched_trans_pict(the_button,0+prefs_x+80, button_y1_array[1]+prefs_y);
	print_crunched_trans_pict(the_button,0+prefs_x+80, button_y1_array[2]+prefs_y);	//load gam removed - SB 4/4/99

//	Show_it();

}


int handle_pause_mouse(LSRAW* button_down, LSRAW* button_up)
{
//extern	  DSpContextReference		      mDisplayContext;
extern int button_y1_array[3];
//extern Ptr screenaddr;
//extern int screenwidth;

//CGrafPtr the_drawing_buffer;

int prefs_x,prefs_y;

int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i;
extern int old_button_state;
//extern int quit;

   prefs_x=screen_cent_x-70;
   prefs_y=screen_cent_y-140;
   
//    prefs_x=250; prefs_y=100;
//copy bg for draw_bg
//	GetMouse(&mouse_xy);
    ZexGetMouse (&mouse_xy);
    
//    DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
    button_state=game_fire_button();
    
//first draw all buttons as off
    for (i=0; i<2;i++)
        print_crunched_trans_pict(button_up,prefs_x,button_y1_array[i]+prefs_y);
//now draw the down one
    for (i=0; i<2;i++)
    {
     if (mouse_xy.v>button_y1_array[i]+prefs_y && mouse_xy.v<button_y1_array[i]+36+prefs_y )	//controls
     {
     print_crunched_trans_pict(button_down,prefs_x+0,button_y1_array[i]+prefs_y);
     return_val=i+1;
     }
    }
//check for keyboard shortcuts
if (check_key(0x35) && TickCount()>last_pause_frame) { real_return_val=1; 
                     last_pause_frame=TickCount()+20;
                      goto end; }	//ESC key=resume
                      
//if (check_key(0xf)) { real_return_val=1; goto end; }	  //R key=resume
if (check_key(0x0)) { real_return_val=2; goto end; }	//A key
//if (check_key(0x0C)) { real_return_val=7; goto end; }	//Q key



if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;
old_button_state=button_state;

end:


return real_return_val;
}




int handle_pause_mouseQS(LSRAW* button_down, LSRAW* button_up)
{
//extern	  DSpContextReference		      mDisplayContext;
extern int button_y1_array[3];
//extern Ptr screenaddr;
//extern int screenwidth;

//CGrafPtr the_drawing_buffer;

int prefs_x,prefs_y;

int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i;
extern int old_button_state;
//extern int quit;

   prefs_x=screen_cent_x-70;
   prefs_y=screen_cent_y-140;
//	SetPort((GrafPtr)the_drawing_buffer);

//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;

//copy bg for draw_bg
//	GetMouse(&mouse_xy);
    ZexGetMouse (&mouse_xy);
    
    button_state=game_fire_button();
    
//first draw all buttons as off
    for (i=0; i<3;i++)
        print_crunched_trans_pict(button_up,prefs_x+80,button_y1_array[i]+prefs_y);
//now draw the down one
    for (i=0; i<3;i++)
    {
     if (mouse_xy.v>button_y1_array[i]+prefs_y && mouse_xy.v<button_y1_array[i]+36+prefs_y )	//controls
     {
     print_crunched_trans_pict(button_down,prefs_x+80,button_y1_array[i]+prefs_y);
     return_val=i+1;
     }
    }
//check for keyboard shortcuts
if (check_key(0x35) && TickCount()>last_pause_frame) { real_return_val=1; 
                         last_pause_frame=TickCount()+20;
                      goto end; }	//ESC key=resume
                      
//if (check_key(0xf)) { real_return_val=1; goto end; }	  //R key=resume
if (check_key(0x0)) { real_return_val=2; goto end; }	//A key
//if (check_key(0x0C)) { real_return_val=7; goto end; }	//Q key



if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;
old_button_state=button_state;

end:


return real_return_val;
}

int handle_message_mouse(LSRAW* button_down, LSRAW* button_up)
{
//extern	  DSpContextReference		      mDisplayContext;
//extern int button_y1_array[3];
//extern Ptr screenaddr;
//extern int screenwidth;
#define button_x 74
#define button_y 100

//CGrafPtr the_drawing_buffer;

int prefs_x,prefs_y;

int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val;
//int i;
extern int old_button_state;
//extern int quit;
   prefs_x=screen_cent_x-70;
   prefs_y=screen_cent_y-140;

    ZexGetMouse (&mouse_xy);
    
    
//    DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
    button_state=game_fire_button();
    
//first draw all buttons as off
        print_crunched_trans_pict(button_up,prefs_x+button_x,button_y+prefs_y);
     if (mouse_xy.v>button_y+prefs_y && mouse_xy.v<button_y+36+prefs_y )	//controls
     {
     print_crunched_trans_pict(button_down,prefs_x+button_x,button_y+prefs_y);
     return_val=1;
     }
    

if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;

old_button_state=button_state;

//end:


return real_return_val;
}

// **********************************************************************************

//called from level 4 to see if we are registered and can proceed
void check_balls()
{
extern Zprefs gzex_prefs;
extern _3D light_normal;
extern int zex_alive, user_abort,death_frame_counter;

Handle prefs_screen_H;
Handle button_data_H=0;
Handle button_down_data_H=0;
//extern CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;

//FSSpec the_file;
LSRAW *prefs_screen;
//LSRAW *button;
//LSRAW *button_down;
int wait_flag,button_state;
int prefs_x,prefs_y;


if ( gzex_prefs.registered==1)	//yes
{
//else show not registered screen and quit game
   prefs_screen_H=GetZexPicture ('RCZ ',5001);	//registered.rcz     
	HLock(prefs_screen_H);
	prefs_screen = (LSRAW*)*prefs_screen_H;
	

//	light_normal.z=1.3;	//dim the scene
//    freeze=1;	//in 3d_top

//¥¥¥main title screen display loop
    ShowCursor();
    button_state=1;
    while (button_state==1) button_state=game_fire_button();
    
    wait_flag=0;
	while (wait_flag==0)	//until mouse control says play
    {
    //and down
    wait_flag=game_fire_button();
    


    render_frame(monitor_w,monitor_h);	//in this file - calls all the needed 3d stuff

    run_dlp();

    prefs_x=120; prefs_y=60;


	print_crunched_trans_pict(prefs_screen,prefs_x,prefs_y);
//     do_object_control();

        Show_it();	//splat 3d portion of wind

    }

        wait_flag=1;
        
    HideCursor();
    DisposeHandle(prefs_screen_H);
    DisposeHandle(button_data_H);
    DisposeHandle(button_down_data_H);
}
else
{
//else show not registered screen and quit game
   prefs_screen_H=GetZexPicture ('RCZ ',5000);	//no passage.rcz     
	HLock(prefs_screen_H);
	prefs_screen = (LSRAW*)*prefs_screen_H;
	

	light_normal.z=1.3;	//dim the scene
//    freeze=1;	//in 3d_top

//¥¥¥main title screen display loop
    ShowCursor();
    button_state=1;
    while (button_state==1) button_state=game_fire_button();
    
    wait_flag=0;
	while (wait_flag==0)	//until mouse control says play
    {
    //and down
    wait_flag=game_fire_button();
    


    render_frame(monitor_w,monitor_h);	//in this file - calls all the needed 3d stuff

//    render_frame(640,390);	  //in this file - calls all the needed 3d stuff
    run_dlp();
    prefs_x=120; prefs_y=60;

	print_crunched_trans_pict(prefs_screen,prefs_x,prefs_y);
//     do_object_control();

        Show_it();	//splat 3d portion of wind

    }

        wait_flag=1;
        zex_alive=0;
        user_abort=1;
        death_frame_counter=1;
        
    HideCursor();
    DisposeHandle(prefs_screen_H);
    DisposeHandle(button_data_H);
    DisposeHandle(button_down_data_H);
}

}

//sets globals int difficulty,tutorial;
void get_level()
{
extern int difficulty,tutorial,training_mission,next_level;
Handle picture_H;
LSRAW* the_picture;

//Handle training_picture_H;
//LSRAW* training_picture;

Handle button_data_H;
Handle button_down_data_H;
LSRAW *button;
LSRAW *button_down;
int player_click;
//int player_clickd;


game_running=0; 
    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up   
	HLock(button_data_H);
	button = (LSRAW*)*button_data_H;
	
    button_down_data_H=GetZexPicture ('RCZ ',133);	//prefs_button_down   
	HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;


 picture_H=GetZexPicture ('RCZ ',3202);	//diff pict   
 if (picture_H==0) report_error("Resource missing: RCZ 3202","\p",4);
 HLock(picture_H);

// training_picture_H=GetZexPicture ('RCZ ',3204);	  //train pict   
// if (training_picture_H==0) report_error("Resource missing: RCZ 3204","\p",4);
// HLock(training_picture_H);
// training_picture=(LSRAW*)*training_picture_H;
     fade_main_screen();
 
 the_picture = (LSRAW*)*picture_H;
    erase_zex_rect(monitor_w,monitor_h);	//in pictures
 
 print_crunched_pict(the_picture,screen_cent_x-190,screen_cent_y-120);
 Show_it();
    erase_zex_rect(monitor_w,monitor_h);	//in pictures
 print_crunched_pict(the_picture,screen_cent_x-190,screen_cent_y-120);
 Show_it();
 
player_click=0;
while (player_click==0)
{


   #if ZEX_PROJECT_BUILDER==1
	  thread_yield();
   #endif

 player_click=handle_diff_mouse(button_down,button);

// if (player_click==1) //tutorial
// { 
////see if tutorial or training mission
//    print_crunched_pict(training_picture,150,120);
//    Show_it();
//    erase_zex_rect(640,480);	  //in pictures
//    print_crunched_pict(training_picture,150,120);
//    Show_it();
// 
//    player_clickd=0;
//    while (player_clickd==0)
//     {
//      player_clickd=handle_train_mouse(button_down,button);
//      if (player_clickd==1)
//      {
//      tutorial=1; 
//      difficulty=16;
//      training_mission=0; 
//      }
//      else
//      if (player_clickd==2)	  //training miss 1
//      {
//      difficulty=16;
//      training_mission=1;
//      }
//      else
//      if (player_clickd==3)	  //training miss 2
//      {
//      difficulty=16;
//      training_mission=2;
//      }
//      else
//      if (player_clickd==4)	  //training miss 3
//      {
//      difficulty=16;
//      training_mission=3;
//      }
//      Show_it();
//     }
//  
// }	  //end of tutorial selected
// 
// else	//difficulty rating
 difficulty=32;	// fix the billions of credits bug!
 //difficulty=16 for easy, 8 for medium and 1 for hard
 if (player_click==2) { game_running=0; next_level=6; tutorial=0; training_mission=0; difficulty=16;
 show_text_in_loading_screen("Zex: Easy setting\n");	
 }
 else
 if (player_click==3) { game_running=0; next_level=6; tutorial=0; training_mission=0;  difficulty=8;
  show_text_in_loading_screen("Zex: Medium setting\n");	
}
 else
 if (player_click==4) { game_running=0; next_level=6; tutorial=0; training_mission=0;  difficulty=1;
  show_text_in_loading_screen("Zex: Hard setting\n");	
}
 
 Show_it();
}

 DisposeHandle(picture_H);
// DisposeHandle(training_picture_H);
}

#define FRIG 27
static int button_y2_array[4] = 
    {10+FRIG,47+FRIG,82+FRIG,119+FRIG};
int handle_diff_mouse(LSRAW* button_down, LSRAW* button_up)
{
//extern	  DSpContextReference		      mDisplayContext;
//extern int button_y1_array[3];
//extern Ptr screenaddr;
//extern int screenwidth;

//CGrafPtr the_drawing_buffer;

int prefs_x,prefs_y;

int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i;
extern int old_button_state;
//extern int quit;
prefs_x=screen_cent_x-220;
prefs_y=screen_cent_y-100;

//    prefs_x=120; prefs_y=140;

    ZexGetMouse (&mouse_xy);
    
    button_state=game_fire_button();
    
//first draw all buttons as off
    for (i=0; i<4;i++)
        print_crunched_pict(button_up,prefs_x+80,button_y2_array[i]+prefs_y);
//now draw the down one
    for (i=0; i<4;i++)
    {
     if (mouse_xy.v>button_y2_array[i]+prefs_y && mouse_xy.v<button_y2_array[i]+36+prefs_y )	//controls
     {
     print_crunched_trans_pict(button_down,prefs_x+80,button_y2_array[i]+prefs_y);
     return_val=i+1;
     }
    }
                      


if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;
old_button_state=button_state;



return real_return_val;
}


int handle_train_mouse(LSRAW* button_down, LSRAW* button_up)
{
//extern	  DSpContextReference		      mDisplayContext;
//extern int button_y1_array[3];
//extern Ptr screenaddr;
//extern int screenwidth;

//CGrafPtr the_drawing_buffer;

int prefs_x,prefs_y;

int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i;
extern int old_button_state;
//extern int quit;

    prefs_x=120; prefs_y=140;

    ZexGetMouse (&mouse_xy);
    
    button_state=game_fire_button();
    
//first draw all buttons as off
    for (i=0; i<4;i++)
        print_crunched_pict(button_up,prefs_x+80,button_y2_array[i]+prefs_y);
//now draw the down one
    for (i=0; i<4;i++)
    {
     if (mouse_xy.v>button_y2_array[i]+prefs_y && mouse_xy.v<button_y2_array[i]+36+prefs_y )	//controls
     {
     print_crunched_trans_pict(button_down,prefs_x+80,button_y2_array[i]+prefs_y);
     return_val=i+1;
     }
    }
                      


if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;
old_button_state=button_state;



return real_return_val;
}
