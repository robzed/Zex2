/* docked.cpp
// SB 300199
// $Log: docked.cpp,v $
// Revision 1.4  2003/09/28 17:29:55  robp
// Changed files from .c to .cpp and removed spaces out of a couple of filenames.
//
// Revision 1.3  2003/09/26 19:20:46  robp
// Alteration for C++ const means internal unless specified extern. Made header extern so that C compiler wouldn't complain.
//
// Revision 1.2  2003/09/20 12:57:14  robp
// Removed nested comments
//
// Revision 1.1.1.1  2003/09/05 22:35:43  stu_c
// First Imported.
//
// Revision 1.32  2002/09/08 02:17:56  stu_c
// Precompiled header adjustments
//
// Revision 1.31  2002/08/31 19:26:39  stu_c
// Callsigns
//
// Revision 1.30  2002/07/28 17:14:42  stu_c
// Exhausts and further particle work
//
// Revision 1.29  2002/07/04 22:45:40  stu_c
// Just removed the conditionals around yield_to_thread
//
// Revision 1.28  2002/05/31 23:55:51  stu
// Removed all warnings from the code
//
// Revision 1.27  2002/04/21 14:45:39  stu
// Further work
//
// Revision 1.26  2002/04/13 15:30:09  stu
// Rotation, Alycians, behaviour
//
// Revision 1.25  2002/03/10 03:40:48  stu
// Additional work on trading - we can buy and sell on different motehrships.
//
// Revision 1.24  2002/03/10 02:48:17  stu
// Fix for suns/particles not being rendered
//
// Revision 1.23  2002/02/24 03:42:15  stu
// (Some) compiler warnings cleaned up
//
// Revision 1.22  2002/02/23 18:52:45  stu
// Can now undock - yay!
//
// Revision 1.21  2002/02/21 22:44:22  stu
// Removed old scene code; now show view out of mothership
//
// Revision 1.20  2002/02/12 23:57:54  rob
// Kill kill list always resets kill list
Added new dock code to dock.
//
// Revision 1.19  2002/02/02 23:10:44  stu
// i_know_what_im_doing_kill param changes
//
// Revision 1.18  2002/01/27 18:04:44  stu
// Unused variables removed
//
// Revision 1.17  2002/01/02 19:25:53  rob
// Added key setup code
//
// Revision 1.16  2002/01/02 13:30:08  stu
// Continuing development for trading/static screens
//
// Revision 1.15  2001/12/31 20:00:55  stu
// More dev work; runs lots faster than the old version. Using code from main_menu screen
//
// Revision 1.14  2001/12/29 22:59:22  stu
// More dev work; now actually displays a falcon and runs OK.
//
// Revision 1.13  2001/12/29 01:12:52  stu
// Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
//
// Revision 1.12  2001/12/28 23:12:37  stu
// More dev work
//
// Revision 1.11  2001/12/27 19:47:51  stu
// Additional development work for docking
//
// Revision 1.10  2001/12/14 19:42:16  stu
// Docking
//
// Revision 1.9  2001/12/04 22:57:04  rob
// sin tab proto removed
//
// Revision 1.8  2001/11/24 19:29:54  stu
// *** empty log message ***
//
// Revision 1.7  2001/11/02 23:21:12  stu
// Further OS X work. Enabled syscon logging.
//
// Revision 1.6  2001/11/02 19:33:08  rob
// Build variants for interface lib and carbon.
Coffee fixes after changes for X.
//
// Revision 1.5  2001/10/27 22:39:27  stu
// zplatform included
//

 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"
  #include "options.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #ifndef __ZEX_PCH__
      #include <Carbon/Carbon.h>
    #endif
  #else
    #include <memory.h>
    #include <timer.h>
    #include <files.h>
    #include <NumberFormatting.h>
    #include <sound.h>
  #endif
  #include "3d_structures_def.h"
  #include "game_defs.h"
  #include "Error_handler.h"
  #include "zex_misc.h"
  #include "real_time.h"
#endif //pch

#include "Zsound.h"
#include "dust.h"
#include "pictures.h"
#include	"splat_control.h"
#include "load_objects.h"
#include "objects_controls.h"
#include "fpoly4.h"
#include "ship_control.h"
#include "drawing.h"
#include "buy.h"
#include "inventory.h"
#include	"ship_kit.h"
#include "instrum.h"
#include "file_io.h"
#include "light_control.h"
#include "obj_control_misc_routines.h"
#include	"txtsplat.h"
#include "docked.h"
#include "command_and_control.h"
#include "3d_top.h"
#include	"object_control_top.h"
#include "ogl.h"
#include "texture_management.h"
#include "static_object_support.h"
#include "access_ocbs.h"
#include "eclipse2.h"
#include "processor.h"
#include "keyboard.h"
#include "camera_control.h"
#include "object_manipulation.h"
#include "z_init.h"
#include "sds.h"
#include "dlp.h"
#include "explode.h"
#include "autopilot.h"
#include "physics.h"
#include "main_sim.h"
#include "hud.h"

            #define textcol 246	//white

#include "POBs.h"

int comms_will_fail,docked_at_time;
int docked_at_shields;

void render_frame(int width, int height);
void setup_docked_screen(LSRAW* the_picture, LSRAW* the_button, LSRAW* the_button_off);
int handle_docked_mouse(LSRAW* button_down, LSRAW* button_up);

void show_ratings();
void process_sleeping();
void do_3d_for_docked_menu(void);
void init_3d_scene(int mothership_slot);
void set_up_scene_for_launch();

extern DSpContextReference		mDisplayContext;


const RGBColor	rgbBlack	= { 0x0000, 0x0000, 0x0000 };
const RGBColor	rgbWhite	= { 0xFFFF, 0xFFFF, 0xFFFF };
const RGBColor	rgbRed		= { 0xFFFF, 0x0000, 0x0000 };
const RGBColor	rgbOrange	= { 0xFFFF, 0x7777, 0x7777 };
const RGBColor	rgbBlue		= { 0x0000, 0x0000, 0xFFFF };
const RGBColor	rgbDarkBlue		= { 0x0000, 0x0000, 0x3333 };
const RGBColor	rgbYellow	= { 0xFFFF, 0xFFFF, 0x0000 };
const RGBColor	rgbPurple	= { 0xFFFF, 0x0000, 0xFFFF };
const RGBColor	rgbGreen	= { 0x0000, 0xFFFF, 0x0000 };
const RGBColor	rgbLightBlue	= { 0x0000, 0xFFFF, 0xFFFF };
const RGBColor	rgbGrey	= { 0x7777, 0x7777, 0x7777 };
const RGBColor	rgbDarkGrey	= { 0x4444, 0x4444, 0x4444 };

int fastdock_in_progress,fastdock_fade_value,docked_at;
int mothership_cat;

void check_if_docked(void)
{
ZObject * MS_object_ptr;
extern	DynObjectsFixedSize *ocb_ptr;

 if (docked_at!=-1)	//we have docked
  {
  
  mothership_cat=get_object_category(docked_at);
  
  //kill the docking sound the mothership makes
  
    MS_object_ptr=&ocb_ptr->object_list[docked_at]; //ptr to mothership
    eclipse_kill_looped_sound(MS_object_ptr->Dyn_OCB_control_data.control_use_2); //Sound ref
    //also kill the engine sound of the object being docked
	eclipse_kill_looped_sound((*ocb_ptr).object_list[get_main_camera_object()].Dyn_OCB_control_data.eclipse_engine_sound_ref);
	
    //I'm going to check myself silly here, just in case...
	if (MS_object_ptr->in_use!=1) //I can't see this happening but it's better than a once in a lifetime crash
	                              //if it should get destroyed between the time we send the docked message and we dock
	{
	 reset_docked();
	 return;
	}
	
	
   do_docked(mothership_cat);
  }
}


void reset_docked(void)
{
 docked_at=-1;
}

void player_has_docked_at(int docked_at_object)
{
 docked_at=docked_at_object;
}




//returns 1 if quit, else 0
void do_docked(int mothership_cat)	 //0 for earth, 1 for alien
{
//extern int music_volume;

extern _3D light_normal;
//extern int retro;
extern int do_launch,zex_alive;
extern int tractor,showing_stats;
extern int launch_timer;
//extern kit_desc ship_items[ITEMS_COUNT];
//extern int hull_strength;
//extern int do_next_level;
//extern int can_do_next_level;
//extern int camera_mode;
extern int fastdock_in_progress,fastdock_fade_value;
extern int training_mission,tutorial;

#if PORTABLE_FILESYSTEM
ZexPicture* button_data_pic_obj;
ZexPicture* select_data_pic_obj;
ZexPicture* button_off_pic_obj;
ZexPicture* really_quit_pic_obj;
ZexPicture* quit_pic_obj;
#else
Handle button_data_H;
Handle select_data_H;
Handle button_off_H;
Handle really_quit_H;
Handle quit_H;
#endif

LSRAW *the_picture;
LSRAW *button;
LSRAW *button_off;
LSRAW *really_quitp;
LSRAW *quitp;

#if PORTABLE_FILESYSTEM
ZexPicture* button_down_data_pic_obj;
#else
Handle button_down_data_H;
#endif

LSRAW *button_down;
int wait_flag,player_click;

int really_quit=0;
//int old_hull_strength;
UnsignedWide start_micros;
//UnsignedWide end_micros;
//int saved_light_amplitude;
int ms_shields;
int sleeping_flag;
Rect top_sleep_rect = {0,0,0,639};
Rect bottom_sleep_rect= {479,0,480,639};


//Extract players kit from their ship


//copy_players_object();
copy_kit_from_ship(0);

//first do busy network
//show_text_in_loading_screen("Zex: do_docked\n");
//eclipse_kill_all_looped_sounds();

fastdock_in_progress=0; fastdock_fade_value=100;

kill_all_dust();

//stand down any wingmen
stand_down_wingmen();	//in C&C
set_black_level(0); //0=black, 32000=white

sleeping_flag=0;

//now do mothership damage
ms_shields=docked_at_shields;
if (docked_at_shields<700 && RangedRdm(0,docked_at_shields)<150) comms_will_fail=2;	//cant connect because of damage to mship

//if we are within 1 minute of the last dock, then the network will be busy <G>
//if (TickCount()<docked_at_time+(60*60)) comms_will_fail=1;
//else	//this prevents the guy docking every minute reestting the timer
docked_at_time=TickCount();	//we also use this to determine whether if failed, we can now connect (5 mins)

//(*ocb_ptr).object_list[docked_at].Dyn_OCB_control_data.mship_ptr->visited=1;
setup_stock(docked_at);

tractor=0;	//not tractoring anymore
do_launch=0;	//zex needs this. Set to 1 if relaunch ship
//SetNormalPriority();
set_time_multiplier_to_1();

    #if PORTABLE_FILESYSTEM
//    if (training_mission==0 && tutorial==0)
    select_data_pic_obj= new ZexPicture ('RCZ ',2002);	//docked_bg   2002
//	else
//    select_data_H=GetZexPicture ('RCZ ',2114);	//docked_bg no next mission   
    the_picture = select_data_pic_obj->GetPictureRef();

    button_data_pic_obj= new ZexPicture ('RCZ ',132);	//prefs_button_up   
    button = button_data_pic_obj->GetPictureRef();

    button_off_pic_obj= new ZexPicture ('RCZ ',140);	//prefs_button_up_off   
    button_off = button_off_pic_obj->GetPictureRef();

    really_quit_pic_obj= new ZexPicture ('RCZ ',3200);	//really quit grafic   
    really_quitp = really_quit_pic_obj->GetPictureRef();

    quit_pic_obj= new ZexPicture ('RCZ ',3201);	//quit grafic   
    quitp = quit_pic_obj->GetPictureRef();

    button_down_data_pic_obj= new ZexPicture ('RCZ ',133);	//prefs_button_down   
    button_down = button_down_data_pic_obj->GetPictureRef();
    #else
//    if (training_mission==0 && tutorial==0)
    select_data_H=GetZexPicture ('RCZ ',2002);	//docked_bg   2002
//	else
//    select_data_H=GetZexPicture ('RCZ ',2114);	//docked_bg no next mission   
	
	HLock(select_data_H);
	the_picture = (LSRAW*)*select_data_H;

    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up   
	HLock(button_data_H);
	button = (LSRAW*)*button_data_H;

    button_off_H=GetZexPicture ('RCZ ',140);	//prefs_button_up_off   
	HLock(button_off_H);
	button_off = (LSRAW*)*button_off_H;

    really_quit_H=GetZexPicture ('RCZ ',3200);	//really quit grafic   
	HLock(really_quit_H);
	really_quitp = (LSRAW*)*really_quit_H;

    quit_H=GetZexPicture ('RCZ ',3201);	//quit grafic   
	HLock(quit_H);
	quitp = (LSRAW*)*quit_H;


    button_down_data_H=GetZexPicture ('RCZ ',133);	//prefs_button_down   
	HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;

    #endif
/*
//Set up 3d scene
    Goutside=0;

    POBS_unload_scene();
    kill_kill_list();
    reset_3D();	//reset engine vars (important bug fix to stop crash when changing levels 6/5/99 SB)
    reset_objects();	//clear out ocb's
	
	
	reset_camera();
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
//set up lighting
	set_light_position(0,-300,0,3200);
	set_light_default_brightness(0);

	  
	  ship_slot=get_static_object_slot('FALC');
		
      if (ship_slot==-1) report_error("Internal Failure during dock. FALC missing","\p",-4);
        
        if (mothership_cat==HUMAN)
	    {
        bg_slot=get_static_object_slot('SHT1');
	    }
	    else
	    {
        bg_slot=get_static_object_slot('SHT1');
	    }
      if (bg_slot==-1) report_error("Internal Failure during dock. Background missing","\p",-4);
        
    docked_load_ship(ship_slot,0,0,0,0,1);	//zex is at slot 0
    docked_load_ship(ship_slot,1,-400,-350,3000,1);	//Falcon, zex is at slot 0
    
//now render the scene
	ship_y=-170;	
	ship_x=0;

*/
     unfreeze_physics();
init_3d_scene(docked_at);


//erase
	set_poly_clipping(0,monitor_w,0,monitor_h);	//set poly clip rect - sb 181298	
    erase_zex_rect(monitor_w, monitor_h);
    setup_docked_screen(the_picture,button,button_off);
	Show_it();
    wait_flag=0;
    ShowCursor();
    
//¥¥¥main title screen display loop
while (wait_flag==0)	//until mouse control says play
    {
    int hour,old_secs=0;

get_system_ticks();	//time for ZexTickCount
update_microsecs_timer();
update_real_time();	//tod clock

	set_poly_clipping(off_640_x,off_640_x+370,off_480_y,off_480_y+480);	//set poly clip rect - sb 181298	
	  
	  do_3d_for_docked_menu();
	  
	set_poly_clipping(0,monitor_w,0,monitor_h);	//set poly clip rect - sb 181298	

//drae time
//if seconds have changed, redraw
    if (get_TOD_secs()!=old_secs)
	{
     old_secs=get_TOD_secs();
	 set_poly_clipping(off_640_x+505,off_640_x+590,off_480_y+453,off_480_y+463);	//set poly clip rect - sb 181298	
	 erase_zex_rect_rgb(0.185,0.185,0.185);
 	 set_poly_clipping(0,monitor_w,0,monitor_h);	//set poly clip rect - sb 181298	
	 draw_real_time(off_640_x+506,off_480_y+463,textcol);            
    }  
	
      if (really_quit>0) 
	  {
	   really_quit--;
       print_crunched_pict(really_quitp,off_640_x+438, off_480_y+326); //Overwrite quit with really quit
	  }
	  else
       print_crunched_pict(quitp,off_640_x+438, off_480_y+326); //Overwrite really quit with quit
	  
      
      Microseconds(&start_micros);	 //equalise
//      if (RangedRdm(1,100)>95 && docked_at_shields<500)
       if (training_mission==0 && tutorial==0)
       { 
        hour=get_TOD_hours();
        if (hour>20 || hour<7)
          light_normal.z=3.9;	//dim the scene
          else
      	  light_normal.z=0.55;	//normal
       }
       else
      	  light_normal.z=0.55;	//normal

if (sleeping_flag==0)
{
     player_click=handle_docked_mouse(button_down,button);

     if (player_click==1)	//sleep
     {
//     fade_out_music();
     sleeping_flag=1;
     }
     else
     if (player_click==5)	//save game 
     {
    save_game();
    setup_docked_screen(the_picture,button,button_off);
    ShowCursor();
     }
     else

     if (player_click==6)	//abort game, change to Really? and prime
     {
     if (really_quit==0)
     {
      really_quit=200;     
     }
     else
      {
       fade_and_play_zex_resource(131);
       HideCursor();
       wait_flag=1;
	   zex_alive=0;
      }
     }
     else
     if (player_click==4)	//buy 
     {
     buy(docked_at);	//file buy.c
    setup_docked_screen(the_picture,button,button_off);
//    setup_docked_screen(the_picture,button,button_off);
//    setup_docked_screen(the_picture,button,button_off);
     }

     else
     if (player_click==2)	//leave dock
     {
     docked_at=-1;
	 wait_flag=1;	//local terminate loop variable     
     do_launch=1;	//tell zex to carry on...
     showing_stats=0;	//reenable monster control
    launch_timer=400;	//for 100 frames disable docking. Dec'd in ship_control


     set_up_scene_for_launch();   
     copy_kit_to_ship(0);
     update_mass(0);
	 
//    shields=1000;	//shields recharge when docked
     goto get_out_no_render;
     }	//end of launch
     else
     if (player_click==3)	//inventory 
     {
     do_inventory();	//file buy.c
    setup_docked_screen(the_picture,button,button_off);
    setup_docked_screen(the_picture,button,button_off);
//    setup_docked_screen(the_picture,button,button_off);
     }
     
 }	//end of sleeping flag==0    
 
 if (sleeping_flag>0)
     {
     if (sleeping_flag<240)	//rectangles
       {
       RGBForeColor(&rgbBlack);
       top_sleep_rect.bottom=sleeping_flag;
       PaintRect(&top_sleep_rect);
       bottom_sleep_rect.top=479-sleeping_flag;
       PaintRect(&bottom_sleep_rect);
       
       sleeping_flag+=10;
       }
     else
     if (sleeping_flag>=240 && sleeping_flag<299)	//D'OH!
     {
     process_sleeping();
     sleeping_flag=300;	//exit sleep
//     sleeping_flag++;
//       top_sleep_rect.bottom=240;
//       PaintRect(&top_sleep_rect);
//       bottom_sleep_rect.top=240;
//       PaintRect(&bottom_sleep_rect);

     }
     
     else if (sleeping_flag==300)
      {
      setup_docked_screen(the_picture,button,button_off);
      setup_docked_screen(the_picture,button,button_off);
      sleeping_flag=0;
      }
     }
     
     Show_it();	//splat 3d portion of window


    }
get_out_no_render:

set_poly_clipping(0,monitor_w,0,monitor_h);
erase_zex_rect(monitor_w, monitor_h);     


#if PORTABLE_FILESYSTEM
delete select_data_pic_obj;
delete button_data_pic_obj;
delete button_off_pic_obj;
delete really_quit_pic_obj;
delete quit_pic_obj;
delete button_down_data_pic_obj;
#else
DisposeHandle(select_data_H);
DisposeHandle(button_data_H);
DisposeHandle(button_off_H);
DisposeHandle(really_quit_H);
DisposeHandle(quit_H);
DisposeHandle(button_down_data_H);
#endif

return;
}


void set_up_scene_for_launch()
{
extern	DynObjectsFixedSize *ocb_ptr;
int camera_object=get_main_camera_object();

    HideCursor();
	    
     play_level_music();

      (*ocb_ptr).object_list[camera_object].Dyn_OCB_control_data.docked_object_slot=-1;	//deff. not docked
      (*ocb_ptr).object_list[camera_object].Dyn_OCB_control_data.docked=0;	     //deff. not docked
      (*ocb_ptr).object_list[camera_object].Dyn_OCB_control_data.request_dock=0;	//deff. not docked
     play_zsound_always(launch, sound_high_pri, c1_chan1, sound_vol_7);

     if (mothership_cat==HUMAN)
     {
       (*ocb_ptr).object_list[camera_object].Oworldx+=100000;
     }
     (*ocb_ptr).object_list[camera_object].Oworldy-=165000;
	 (*ocb_ptr).object_list[camera_object].abs_velocity_vector.y=-18000;
	 
     (*ocb_ptr).object_list[docked_at].Dyn_OCB_control_data.control_use_4=camera_object;	//set launch object
     (*ocb_ptr).object_list[docked_at].Dyn_OCB_control_data.control_use_1_float=5.0;	//set launch timer - 5 seconds whilst shields are down
//set up player orientation to up
	 reset_rotation_for_object(camera_object);
	 set_rotation_around_world_x(camera_object,90);
	 do_target(camera_object);
//And we are now outside the mothership so cancel colldetect
   (*ocb_ptr).object_list[camera_object].crashable=1;
	 
}


  
void init_3d_scene(int mothership_slot)
{
extern	DynObjectsFixedSize *ocb_ptr;
vector p,d;
int camera_object=get_main_camera_object();

//make camera object non-collidable   

//we are not docking   
   (*ocb_ptr).object_list[camera_object].Dyn_OCB_control_data.request_dock=0;
//And we are now inside the mothership so cancel colldetect
   (*ocb_ptr).object_list[camera_object].crashable=0;

  set_ship_thruster (camera_object, MREV1V, 0);
  set_ship_thruster (camera_object, MREV2V, 0);
  set_ship_thruster (camera_object, MTHR1V, 0);
  set_ship_thruster (camera_object, MTHR2V, 0);
    set_ship_thruster (camera_object, VTHR1V, 0);
    set_ship_thruster (camera_object, VTHR2V, 0);
    set_ship_thruster (camera_object, RVTH1V, 0);
    set_ship_thruster (camera_object, RVTH2V, 0);
	disengage_AP(camera_object);
//	engage_ap(camera_object,AP_STEER);
 (*ocb_ptr).object_list[camera_object].Dyn_OCB_control_data.command_ORCS_damper=ORCS_DAMPER_ROT+ORCS_DAMPER_LIN;	//all on


//reset current angular torque
   reset_angular_torque(camera_object);

//rotate around pitch if human mship or yaw if alien
if (mothership_cat==HUMAN)
{
//move away from mothership
   ocb_ptr->object_list[camera_object].Oworldx-=100000;
//rotate around x   
   p.x=0; p.y=0; p.z=1;	//position of force
   d.x=0; d.y=-1; d.z=0;	//direction of force
   apply_initial_force(camera_object, 1 /*Neweton*/ , 0.1 /*second*/ ,p /*oint*/ , d /*irection*/);   

   p.x=0; p.y=0; p.z=-1;	//position of force
   d.x=0; d.y=1; d.z=0;	//direction of force
   apply_initial_force(camera_object, 1 /*Neweton*/ , 0.1 /*second*/ ,p /*oint*/ , d /*irection*/);   
}
else
{
//move away up the mothership
	 reset_rotation_for_object(camera_object);
	 set_rotation_around_world_x(camera_object,0);

   ocb_ptr->object_list[camera_object].Oworldy+=100000;
//rotate around y   
   p.x=0; p.y=0; p.z=1;	//position of force
   d.x=-1; d.y=0; d.z=0;	//direction of force
   apply_initial_force(camera_object, 1 /*Neweton*/ , 0.1 /*second*/ ,p /*oint*/ , d /*irection*/);   

   p.x=0; p.y=0; p.z=-1;	//position of force
   d.x=1; d.y=0; d.z=0;	//direction of force
   apply_initial_force(camera_object, 1 /*Neweton*/ , 0.1 /*second*/ ,p /*oint*/ , d /*irection*/);   
}



}



void do_3d_for_docked_menu(void)
{

//   update_microsecs_timer();
//   update_real_time();	//tod clock
   #if ZEX_PROJECT_BUILDER==1
	  thread_yield();
   #endif

   run_periodic_sound_tasks();
   kill_kill_list();

   
//    poll_input_device();
//   apply_control_inputs();
//     set_ship_rotational_thruster (get_main_camera_object(), ROTYAWV, -5);    
//     set_ship_rotational_thruster (0, ROTROLLV, -127);    
//     set_ship_rotational_thruster (0, ROTPITCHV, -1);    
//    object_systems_sim(get_main_camera_object());	//handle all ships systems/engines/fuel etc

   run_camera_control();	//in camera.c

   Show_it();	//splat 3d portion of window

//	set_poly_clipping(off_640_x,off_640_x+356,off_480_y,off_480_y+480);	//set poly clip rect - sb 181298	
  render_frame(monitor_w,monitor_h);	//in this file - calls all the needed 3d stuff

  run_dlp();
// run_cwp();	//cpanel.c


// run_hud(0);	//display hud symbology


//if (in_wormhole==0) show_nav();	//show directional arrow

//run_console();

  do_object_control();

  explode_explode_list();
  reset_explode_list();	//as it says


}

void process_sleeping()
{
extern GDHandle				mZexGDH;
extern int sleep_timer;

//CGrafPtr the_drawing_buffer;
GDHandle	saveGDH;
GrafPtr		savePort;
int quit;
int button_down;
int rand_val;

rand_val=RangedRdm(0,100);

button_down=0;

while (Button());


if (sleep_timer > MAX_AWAKE-(38*60*60*4) )	 //been awake for less than 4 hours?
{
quit=0;
while (quit==0)
 {
//    GetPort (&savePort);
//	saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//	SetGDevice(mZexGDH);
//	RGBBackColor (&rgbBlack);
//	EraseRect(&the_drawing_buffer->portRect);
    update_microsecs_timer();
    update_real_time();	//tod clock
    erase_zex_rect(monitor_w,monitor_h);	//in pictures

//	SetGDevice(saveGDH);	//these are here so I can SEE them!
//	SetPort(savePort);
	if (rand_val>50)
	SplatText_Transparent ("You are not tired enough to sleep.",off_640_x+208,off_480_y+184,textcol);
	else
	SplatText_Transparent ("You try hard to fall asleep, but you simply are not tired.",off_640_x+140,off_480_y+184,textcol);
	
	draw_real_time(off_640_x+280,off_480_y+200,textcol);            
	sleep_timer+=38*60;

	SplatText_Transparent ("Click Mouse Button To Continue.",off_640_x+220,off_480_y+284,textcol);
        
if (Button()==1) button_down=1;

if (button_down==1) 
     {
     if (Button()==0) quit=1;
     }
     
    Show_it();
  }

}
else
{
//do sleep, show time incrementing very fast
quit=0;
while (quit==0)
 {
    GetPort (&savePort);
	saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
	SetGDevice(mZexGDH);
	RGBBackColor (&rgbBlack);
//	EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect(monitor_w,monitor_h);	//in pictures

	SetGDevice(saveGDH);	//these are here so I can SEE them!
	SetPort(savePort);
	add_a_minute_to_real_time();
	SplatText_Transparent ("You are asleep.",off_640_x+270,off_480_y+184,textcol);
        draw_real_time(off_640_x+280,off_480_y+200,textcol);            
        sleep_timer+=38*60;

        if (sleep_timer > MAX_AWAKE ) quit=1;
	SplatText_Transparent ("Click Mouse Button To Wake Up.",off_640_x+220,off_480_y+284,textcol);
        
if (Button()==1) button_down=1;

if (button_down==1) 
     {
     if (Button()==0) quit=1;
     }
     
    Show_it();
  }
}

	set_poly_clipping(0,monitor_w,0,monitor_h);	//set poly clip rect - sb 181298	
    erase_zex_rect(monitor_w, monitor_h);

}


extern int old_button_state;
int button_y2_array[6] = 
    {135,	//next mission
    172,	//Leave dock
    209,	//show status
    246,	//buy
    285,	//save
    322,	//quit
    };
    
void setup_docked_screen(LSRAW* the_picture, LSRAW* the_button, LSRAW* the_button_off)
{
extern int button_y2_array[6];
//extern Ptr screenaddr;
//extern int screenwidth;
//extern GDHandle				mZexGDH;

//CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;
   
         
	print_crunched_pict(the_picture,off_640_x,off_480_y);
	
	print_crunched_pict(the_button_off,off_640_x+380, off_480_y+button_y2_array[0]);
	
	print_crunched_pict(the_button,off_640_x+380, off_480_y+button_y2_array[1]);
	print_crunched_pict(the_button,off_640_x+380, off_480_y+button_y2_array[2]);
	print_crunched_pict(the_button,off_640_x+380, off_480_y+button_y2_array[3]);
	print_crunched_pict(the_button,off_640_x+380, off_480_y+button_y2_array[5]);
	print_crunched_pict(the_button_off,off_640_x+380, off_480_y+button_y2_array[4]);

        show_ratings();             


//	Show_it();
	 SplatText_Transparent ("Time:",off_640_x+383,off_480_y+463,textcol);
	 draw_real_time(off_640_x+507,off_480_y+463,textcol);            


}

void show_ratings()
{
//int temp;
unsigned char number_string[12]; 
//galactic_stats the_stats;
extern int rank,promotion_points,kills,tutorial;
extern int training_mission;

             #define textcol 246

if (training_mission!=0 || tutorial!=0)
{
//             SplatText_Transparent_Fade ("Stats not available in training missions.",390,380,textcol);

}
else
{
//         get_callsign_and_name(cs_string,docked_at);
//             #define textcol 246
//             SplatText_Transparent_Fade ((char *)cs_string,380,115,textcol);

//show stats
//             get_galactic_stats(&the_stats);
//
//
//             SplatText_Transparent_Fade ("Motherships:",376,380,textcol);
//             SplatText_Transparent_Fade ("Supply Rating:",376,393,textcol);
//             SplatText_Transparent_Fade ("Power Rating:",376,406,textcol);
//             
//             #define textcol 225	  //earth green
//             NumToString(the_stats.number_of_earth_mships,number_string);
//             number_string[number_string[0]+1]=0;	//quick p to c hack
//             number_string[0]=' ';
//             SplatText_Transparent_Fade ((char *)number_string,500,380,textcol);
//
//             #define textcol 215	  //alien red
//             NumToString(the_stats.number_of_alien_mships,number_string);
//             number_string[number_string[0]+1]=0;	//quick p to c hack
//             number_string[0]=' ';
//             SplatText_Transparent_Fade ((char *)number_string,560,380,textcol);
//
//             
//             #define textcol 225	  //earth green
//             NumToString(the_stats.total_earth_supply_rating,number_string);
//             number_string[number_string[0]+1]=0;	//quick p to c hack
//             number_string[0]=' ';
//             SplatText_Transparent_Fade ((char *)number_string,500,393,textcol);
//
//             #define textcol 215	  //alien red
//             NumToString(the_stats.total_alien_supply_rating,number_string);
//             number_string[number_string[0]+1]=0;	//quick p to c hack
//             number_string[0]=' ';
//             SplatText_Transparent_Fade ((char *)number_string,560,393,textcol);
//
//             #define textcol 225	  //earth green
//             NumToString(the_stats.total_earth_power_rating,number_string);
//             number_string[number_string[0]+1]=0;	//quick p to c hack
//             number_string[0]=' ';
//             SplatText_Transparent_Fade ((char *)number_string,500,406,textcol);
//
//             #define textcol 215	  //alien red
//             NumToString(the_stats.total_alien_power_rating,number_string);
//             number_string[number_string[0]+1]=0;	//quick p to c hack
//             number_string[0]=' ';
//             SplatText_Transparent_Fade ((char *)number_string,560,406,textcol);

             #define textcol 246	//white
             //print rank
             SplatText_Transparent ("Rank:",off_640_x+383,off_480_y+424,textcol);

             if (rank==ENS) SplatText_Transparent (" Ensign",off_640_x+500,off_480_y+424,textcol);
             else
             if (rank==LT) SplatText_Transparent (" Lieutenant",off_640_x+500,off_480_y+424,textcol);
             else
             if (rank==LTCR) SplatText_Transparent (" LT Commander",off_640_x+500,off_480_y+424,textcol);
             else
             if (rank==CDR) SplatText_Transparent (" Commander",off_640_x+500,off_480_y+424,textcol);
             else
             if (rank==CAPT) SplatText_Transparent (" Captain",off_640_x+500,off_480_y+424,textcol);
             else
             if (rank==RADM) SplatText_Transparent (" Rear Admiral",off_640_x+500,off_480_y+424,textcol);
             else
             if (rank==VADM) SplatText_Transparent (" Vice Admiral",off_640_x+500,off_480_y+424,textcol);
             else
             if (rank==ADM) SplatText_Transparent (" Admiral",off_640_x+500,off_480_y+424,textcol);
             else
             if (rank==FADM) SplatText_Transparent (" Fleet Admiral",off_640_x+500,off_480_y+424,textcol);

             SplatText_Transparent ("Promotion Points:",off_640_x+383,off_480_y+437,textcol);
             
             NumToString(promotion_points,number_string);
             number_string[number_string[0]+1]='/';	//quick p to c hack
             
             if (rank==ENS)
             {
             number_string[number_string[0]+2]='5';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]=0;	//quick p to c hack
             }
             else
             if (rank==LT)
             {
             number_string[number_string[0]+2]='2';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]=0;	//quick p to c hack
             }
            
             else
             if (rank==LTCR)
             {
             number_string[number_string[0]+2]='1';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
            

             else
             if (rank==CDR)
             {
             number_string[number_string[0]+2]='2';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (rank==CAPT)
             {
             number_string[number_string[0]+2]='4';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (rank==RADM)
             {
             number_string[number_string[0]+2]='5';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (rank==VADM)	//as high as it goes baby
             {
             number_string[number_string[0]+2]='7';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (rank==ADM)	//as high as it goes baby
             {
             number_string[number_string[0]+2]='9';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (rank==FADM)	//as high as it goes baby
             {
             number_string[number_string[0]+2]='N';	//quick p to c hack
             number_string[number_string[0]+3]='/';	//quick p to c hack
             number_string[number_string[0]+4]='A';	//quick p to c hack
             number_string[number_string[0]+5]=0;	//quick p to c hack
             }




             number_string[0]=' ';
             SplatText_Transparent ((char *)number_string,off_640_x+500,off_480_y+437,textcol);

             SplatText_Transparent ("Kills:",off_640_x+383,off_480_y+450,textcol);
             
             NumToString(kills,number_string);
             number_string[number_string[0]+1]=0;	//quick p to c hack

             number_string[0]=' ';
             SplatText_Transparent ((char *)number_string,off_640_x+500,off_480_y+450,textcol);
//display time
             
 }
}

int handle_docked_mouse(LSRAW* button_down, LSRAW* button_up)
{
//extern	  DSpContextReference		          mDisplayContext;
extern int button_y2_array[6];
//extern int music_volume;
//extern int sound_volume;
extern int training_mission;
int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i,k;
//unsigned char number_string[2] = {0,0};
extern int old_button_state;
//extern int quit;
//copy bg for draw_bg
//	GetMouse(&mouse_xy);
    ZexGetMouse (&mouse_xy);
//    DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
    button_state=Button();
//first draw all buttons as off
    if (training_mission==0) k=0; else k=1;
    for (i=k; i<6;i++)
    if (training_mission==0)	//if we can go to next mission
	print_crunched_pict(button_up,off_640_x+380, off_480_y+button_y2_array[i]);
    else
    {
    if(i!=4)	print_crunched_pict(button_up,off_640_x+380,off_480_y+ button_y2_array[i]);
    }
//        print_crunched_pict(button_up,380,button_y2_array[i]);
//now draw the down one
    for (i=k; i<6;i++)
    {
     if (i==4 && training_mission==1) i=5;
     if (mouse_xy.v>off_480_y+button_y2_array[i] && mouse_xy.v<off_480_y+button_y2_array[i]+36 )	//controls
     {
     print_crunched_pict(button_down,off_640_x+380,off_480_y+button_y2_array[i]);
     return_val=i+1;
     }
    }
////check for keyboard shortcuts
//if (check_key(0x2d)) { real_return_val=3; goto end; }	//N key
//if (check_key(0x01)) { real_return_val=1; goto end; }	//S key
//if (check_key(0x08)) { real_return_val=6; goto end; }	//C key
//if (check_key(0x0C)) { real_return_val=7; goto end; }	//Q key


if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;
old_button_state=button_state;

//end:
//process real_return_val
//if (real_return_val==1)
//{
//HideCursor();
//
//GraphicsPaused();
//FlushEvents(-1,0);	//flush all
//
//configure_input_device();
//FlushEvents(-1,0);	//flush all
//
//GraphicsActive();
//    setup_main_screen(the_picture,button_up);
//    setup_main_screen(the_picture,button_up);
//    setup_main_screen(the_picture,button_up);
//
//    ShowCursor();
//}
//
//if (real_return_val==6)
//{
//	  Show_it();
//     display_wait();	  //
//    do_creds_screen();
//
//    setup_main_screen(the_picture,button_up);
//    setup_main_screen(the_picture,button_up);
//    setup_main_screen(the_picture,button_up);
//
//real_return_val=6;
//}
//
////quit game
//if (real_return_val==7)
//{
//quit=1;
//}

return real_return_val;
}


void docked_load_ship(int static_obj, int dyn_slot, float x, float y, float z, int scale)
{
load_dyn_object_pb the_params;
//int drot_z;
//extern int dynamic_object_index;

clear_pb(&the_params);	//saves loads of zero code
//    reset_objects();
//set up ship for title screen
	the_params.world_x=x;
	the_params.world_y=y;
	the_params.world_z=z;
	the_params.normal.z =  -1;	//objects are ALWAYS defined facing right
	the_params.shield_value=1000;
  	the_params.mass=100;
	    the_params.object_category=HUMAN;
	load_dyn_object(static_obj,dyn_slot,&the_params,-1,scale,-1,!DUST_BIT);	//the object, the position (-1=next free)
}

//load_animated_ship(int static_obj, unsigned long frame1_id, unsigned long frame2_id, 
//                   unsigned long frame3_id, unsigned long frame4_id)
//{
//load_dyn_object_pb the_params;
//int drot_z;
//extern int dynamic_object_index;
//anim_pb anim;
//    kill(0);	  //clear out old ship
//
////    reset_objects();
//    anim.object_id[0]=frame1_id;
//    anim.object_id[1]=frame2_id;
//    anim.object_id[2]=frame3_id;
//    anim.object_id[3]=frame4_id;
//
////set up ship for title screen
//	  the_params.world_x=00;
//	  the_params.world_y=0;
//	  the_params.world_z=0;
//	  the_params.Omovedx=0;
//	  the_params.Omovedy=0;
//	  the_params.Omovedz=0;
//	  the_params.delta_rot_x=0;
//	  the_params.delta_rot_y=0;
//	  drot_z=RangedRdm(0,8)-4;
//	  if (drot_z==0) drot_z=4;
//	  
//	  the_params.delta_rot_z=drot_z;
//	  the_params.rot_x=0;
//	  the_params.rot_y=0;
//	  the_params.rot_z=0;
//	  the_params.controller_ref=0;
//	  the_params.control_use_1=0;
//	  the_params.control_use_2=0;
//	  the_params.control_use_3=0;
//	  the_params.control_use_4=0;
//	  the_params.control_use_5=0;
//	  the_params.wanted_velocity=0;
//	   the_params.attack_mode=0;
//	  the_params.normal.x =  0;	//objects are ALWAYS defined facing right
//	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	  the_params.normal.z =  -1;	//objects are ALWAYS defined facing right
//	  the_params.shield_value=1000;
//	  the_params.mass=100;
//	  the_params.no_trivial_rejection=0;
//	      the_params.object_category=zex_object;
//
////	load_dyn_object(static_obj,0,&the_params);	//the object, the position (-1=next free)
//    load_animated_object (&anim,4,3,1,1,&the_params);
//
//}

