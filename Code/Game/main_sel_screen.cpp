// *************************************MODULE HEADER*********************************
// * FILENAME           : main_sel_screen.cpp
// * MODULE TITLE       : main_sel_screen.cpp
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 01:11:06 on 31st March 2000
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
// $Log: main_sel_screen.cpp,v $
// Revision 1.12  2003/10/15 17:37:00  stu_c
// Zex: now boots to game, added VBL sync (via OGL)
//
// Revision 1.11  2003/10/08 20:15:42  robp
// Altered picture load based on new file load.
//
// Revision 1.10  2003/09/30 21:26:32  robp
// Changes to force the sound sub-system to build - maybe not work. No effect on build code unless PORTABLE_FILESYSTEM is enabled.
//
// Revision 1.9  2003/09/28 17:29:46  robp
// Changed files from .c to .cpp and removed spaces out of a couple of filenames.
//
// Revision 1.8  2003/09/28 10:36:08  robp
// Signed/Unsigned comparison fixes from last night, plus collision fix.
//
// Revision 1.7  2003/09/27 21:52:42  robp
// Fixed places where we were loading an unsigned with -1.
//
// Revision 1.5  2003/09/26 19:20:49  robp
// Alteration for C++ const means internal unless specified extern. Made header extern so that C compiler wouldn't complain.
//
// Revision 1.4  2003/09/24 21:39:39  robp
// More work on Portable file-system. No work should effect current operation on Zex. When it is up and running as the standard system, we can remove the #if conditional compilation. Currently the sound needs work, plus one ZGetResource before we can debug. Then rest of the file-system can be completed.
//
// Revision 1.3  2003/09/22 21:04:34  stu_c
// Zex: Weapons: Adjustments to temp rise and fall in laser bay. Rebuild required.
//
// Revision 1.2  2003/09/20 12:57:10  robp
// Removed nested comments
//
// Revision 1.1.1.1  2003/09/05 22:35:46  stu_c
// First Imported.
//
// Revision 1.19  2002/09/08 02:35:46  stu_c
// Pre compiled header support
//
// Revision 1.18  2002/09/07 15:57:05  rob_c
// Get rid of screen overriden in window mode in main screen key setting.
//
// Revision 1.17  2002/07/28 17:11:48  stu_c
// Exhausts and further particle work
//
// Revision 1.16  2002/05/31 23:56:58  stu
// Removed all warnings from the code
//
// Revision 1.15  2002/04/13 15:32:39  stu
// Rotation, Alycians, behaviour
//
// Revision 1.14  2002/02/17 06:21:12  stu
// Added radar targetting
//
// Revision 1.13  2002/02/02 23:11:20  stu
// i_know_what_im_doing_kill param changes
//
// Revision 1.12  2002/01/27 18:13:35  stu
// Dunno
//
// Revision 1.11  2002/01/02 19:22:07  rob
// added key setup code
//
// Revision 1.10  2002/01/02 13:30:25  stu
// Continuing development for trading/static screens
//
// Revision 1.9  2001/12/31 20:01:20  stu
// Now works; displays 3d ships.
//
// Revision 1.8  2001/12/29 01:12:59  stu
// Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
//
// Revision 1.7  2001/12/04 22:56:37  rob
// sin tab proto removed
//
// Revision 1.6  2001/11/24 21:19:52  rob
// Extra
//
// Revision 1.5  2001/11/12 20:22:05  stu
// Added call to eclipse_kill_looped_sounds on netry.
//
// Revision 1.4  2001/11/02 23:20:56  stu
// Further OS X work. Enabled syscon logging.
//
// Revision 1.3  2001/11/02 19:33:12  rob
// Build variants for interface lib and carbon.
Coffee fixes after changes for X.
//
// Revision 1.2  2001/10/22 21:28:09  rob
// Carbon warning changes
//
// Revision 1.1  2001/10/21 00:58:13  stu
// Initial port work
//
// Revision 1.0.0.1  2001/10/17 20:46:05  rob
// First Imported.
//
// Revision 1.9  2001/07/29 10:34:03  rob
// render_frame_no_erase prototype removed
//
// Revision 1.8  2001/06/24 21:42:09  rob
// changes to kill call
//
// Revision 1.7  2001/06/24 03:36:41  stu
// Was calling i_know_what_im_doing_kill on a dead object.
//
// Revision 1.6  2001/06/13 18:21:24  rob
// clear pb added
//
// Revision 1.5  2001/06/03 00:25:03  stu
// Fix for dialogs not showing. Also affects finder and Anvil console commands
//
// Revision 1.4  2001/05/28 01:53:33  stu
// 280501
//
// Revision 1.3  2001/04/01 18:46:41  stu
// *** empty log message ***
//
// Revision 1.2  2001/01/02 18:56:58  stu
// switching to finder
//
// Revision 1.1.1.1  2001/01/01 21:13:33  rob
// First Imported.
//
// Revision 1.7  2000/12/10 03:36:28  stu
// Calls to kill replaced with either calls to add_to_kill_list OR
i_know_what_im_doing_kill

Old lighting calls removed
//
// Revision 1.6  2000/12/03 23:52:19  rob
// changed constants for object type to get uniformity
//
// Revision 1.5  2000/11/16 20:16:52  rob
// orbit
//
// Revision 1.4  2000/10/08 21:48:46  stu
// Engine conversion to 64 bit. Bitch of a job.
//
// Revision 1.3  2000/09/20 23:50:12  stu
// HUD/AP/Scaling
//
// Revision 1.2  2000/08/27 16:27:04  stu
// Changes for movement decoupling in time.
//
// Revision 1.1  2000/08/27 01:33:05  stu
// Hud changes
//
// Revision 1.0.0.1  2000/08/21 21:58:44  stu
// First Imported.
//
 */

//do_main_sel_screen
//SB 1801298
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"
  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
    #include <AGL/agl.h>
  #else
    #include <files.h>
    #include <memory.h>
    #include <events.h>
    #include <timer.h>
    #include <sound.h>
    #include <agl.h>
  #endif
  #include "zex_misc.h"
  #include "Error_handler.h"
  #include "game_defs.h"
#endif //pch




#include "options.h"
#include "file_io.h"
#include "do_intro.h"

#include "drawing.h"
#include "pictures.h"
#include	"txtsplat.h"
#include "ship_control.h"
#include "main_sel_screen.h"
#ifndef __3d_structures__  //might be from pch
#include "3d_structures_def.h"
#endif
#include "load_objects.h"
#include "objects_controls.h"

#include "fpoly4.h"
#include "splat_control.h"
#include "object_control_top.h"

#include "game_pause.h"
#include "matrix_primitives.h"
#include "Physics.h"
#include "obj_control_misc_routines.h"
#include "keyboard.h"
#include "Eclipse2.h"
#include "light_control.h"
#include "static_object_support.h"
#include "docked.h"
#include "Zsound.h"
#include "key_setting.h"
#include "camera_control.h"
#include "object_manipulation.h"
#include "texture_management.h"
#include "z_init.h"
#include "load_game.h"
#include "galaxy.h"
#include "processor.h"
#include "object_control_top.h"
#include "access_ocbs.h"

//proto
void render_frame(int width, int height);
//void render_frame_no_erase(int width, int height);
void render_frame_colour_bg(int width, int height, int colour);

void setup_main_screen(LSRAW* the_picture, LSRAW* the_button);
void load_ship(int static_obj);
void load_animated_ship(int static_obj, unsigned long frame1_id, unsigned long frame2_id, 
                   unsigned long frame3_id, unsigned long frame4_id);

void setup_tables();	//actually register game
void fade_main_screen();
void fade_up_main_screen();
void do_3d_for_main_menu(void);

void draw_mouse_pointer();
int handle_mouse(LSRAW* button_down, LSRAW* button_up, 
                 LSRAW* the_picture,LSRAW* arrow_up,LSRAW* arrow_down,
                 LSRAW* arrow_up_down,LSRAW* arrow_down_down,
                 LSRAW* checkbox_off,LSRAW* checkbox_on);
                                  
void get_bg (short x,short y);
static int last_click_time;
static int button_has_been_up=0;

extern Zprefs gzex_prefs;
extern	  DSpContextReference		      mDisplayContext;

void do_creds_screen();

static int vbl_altered;
static double frame_time=0;

void do_main_sel_screen()
{
extern Zprefs gzex_prefs;
extern int music_volume;
extern int sound_volume;
extern int freeze,done_load_flag,start_time;
extern int accumulated_game_time;

//FSSpec the_file;
//Handle the_file_data_H;
//Handle object_data_H;
#if PORTABLE_FILESYSTEM
ZexPicture* button_data_pic_obj;
ZexPicture* select_data_pic_obj;

LSRAW *the_picture;
LSRAW *button;

ZexPicture* button_down_data_pic_obj;
LSRAW *button_down;
ZexPicture* arrow_down_data_pic_obj;
ZexPicture* arrow_up_data_pic_obj;
LSRAW *arrow_down;
LSRAW *arrow_up;
ZexPicture* arrow_up_down_data_pic_obj;
LSRAW *arrow_up_down;
ZexPicture* arrow_down_down_data_pic_obj;
LSRAW *arrow_down_down;

ZexPicture* checkbox_off_data_pic_obj;
LSRAW *checkbox_off;
ZexPicture* checkbox_on_data_pic_obj;
LSRAW *checkbox_on;
#else
Handle button_data_H;
Handle select_data_H;

LSRAW *the_picture;
LSRAW *button;

Handle button_down_data_H;
LSRAW *button_down;
Handle arrow_down_data_H;
Handle arrow_up_data_H;
LSRAW *arrow_down;
LSRAW *arrow_up;
Handle arrow_up_down_data_H;
LSRAW *arrow_up_down;
Handle arrow_down_down_data_H;
LSRAW *arrow_down_down;

Handle checkbox_off_data_H;
LSRAW *checkbox_off;
Handle checkbox_on_data_H;
LSRAW *checkbox_on;
#endif

int wait_flag;

int change_ship_counter,ship_counter;
//load_dyn_object_pb the_params;
//Handle mouse_grafic_H;
typedef struct
{
 int anim,slot;
 unsigned long f1_id,f2_id,f3_id,f4_id;
} loader_info;


int player_click,load_result;
extern int Goutside;
int ship_slot;
int is_faded;

//еееEND of vars

//Kill off all looped sounds
eclipse_kill_all_looped_sounds();


is_faded=0;

last_click_time=TickCount()+2;
vbl_altered=0;

freeze=0;	//unfreeze engine in case we quit a previous game
done_load_flag=0;
change_ship_counter=300;
ship_counter=0;


//load in main picture to the 3 buffers

//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"\pprefs_bg.RCZ",&the_file);
//	select_data_H=read_file(the_file);
#if PORTABLE_FILESYSTEM
    #if OGL==1
    select_data_pic_obj= new ZexPicture ('RCZ ',147);	//prefs_bg_ogl   
    #else
    select_data_pic_obj = new ZexPicture ('RCZ ',131);	//prefs_bg   
    #endif
    the_picture = select_data_pic_obj->GetPictureRef();

    button_data_pic_obj = new ZexPicture ('RCZ ',132);	//prefs_button_up   
    button = button_data_pic_obj->GetPictureRef();

    button_down_data_pic_obj = new ZexPicture ('RCZ ',133);	//prefs_button_down   
    button_down = button_down_data_pic_obj -> GetPictureRef();

    arrow_down_data_pic_obj = new ZexPicture ('RCZ ',135);	//prefs_button_down   
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"\parrow_down.RCZ",&the_file);
//	arrow_down_data_H=read_file(the_file);
    arrow_down = arrow_down_data_pic_obj->GetPictureRef();

    arrow_up_data_pic_obj = new ZexPicture ('RCZ ',137);	//prefs_button_down   
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"\parrow_up.RCZ",&the_file);
//	arrow_up_data_H=read_file(the_file);
    arrow_up = arrow_up_data_pic_obj->GetPictureRef();

    arrow_up_down_data_pic_obj = new ZexPicture ('RCZ ',138);	//prefs_button_down   
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"\parrow_up_down.RCZ",&the_file);
//	arrow_up_down_data_H=read_file(the_file);
    arrow_up_down = arrow_up_down_data_pic_obj->GetPictureRef();

    arrow_down_down_data_pic_obj = new ZexPicture ('RCZ ',136);	//prefs_button_down   
    arrow_down_down = arrow_down_down_data_pic_obj->GetPictureRef();

    checkbox_off_data_pic_obj = new ZexPicture ('RCZ ',141);	//prefs_checkbox off   
    checkbox_off = checkbox_off_data_pic_obj->GetPictureRef();

    checkbox_on_data_pic_obj= new ZexPicture ('RCZ ',142);	//prefs_checkbox on   
    checkbox_on = checkbox_on_data_pic_obj ->GetPictureRef();
#else
    #if OGL==1
    select_data_H=GetZexPicture ('RCZ ',147);	//prefs_bg_ogl   
    #else
    select_data_H=GetZexPicture ('RCZ ',131);	//prefs_bg   
    #endif
	HLock(select_data_H);
	the_picture = (LSRAW*)*select_data_H;

    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up   
	HLock(button_data_H);
	button = (LSRAW*)*button_data_H;

    button_down_data_H=GetZexPicture ('RCZ ',133);	//prefs_button_down   
	HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;

    arrow_down_data_H=GetZexPicture ('RCZ ',135);	//prefs_button_down   
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"\parrow_down.RCZ",&the_file);
//	arrow_down_data_H=read_file(the_file);
	HLock(arrow_down_data_H);
	arrow_down = (LSRAW*)*arrow_down_data_H;

    arrow_up_data_H=GetZexPicture ('RCZ ',137);	//prefs_button_down   
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"\parrow_up.RCZ",&the_file);
//	arrow_up_data_H=read_file(the_file);
	HLock(arrow_up_data_H);
	arrow_up = (LSRAW*)*arrow_up_data_H;

    arrow_up_down_data_H=GetZexPicture ('RCZ ',138);	//prefs_button_down   
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"\parrow_up_down.RCZ",&the_file);
//	arrow_up_down_data_H=read_file(the_file);
	HLock(arrow_up_down_data_H);
	arrow_up_down = (LSRAW*)*arrow_up_down_data_H;

    arrow_down_down_data_H=GetZexPicture ('RCZ ',136);	//prefs_button_down   
	HLock(arrow_down_down_data_H);
	arrow_down_down = (LSRAW*)*arrow_down_down_data_H;
    
    checkbox_off_data_H=GetZexPicture ('RCZ ',141);	//prefs_checkbox off   
	HLock(checkbox_off_data_H);
	checkbox_off = (LSRAW*)*checkbox_off_data_H;

    checkbox_on_data_H=GetZexPicture ('RCZ ',142);	//prefs_checkbox on   
	HLock(checkbox_on_data_H);
	checkbox_on = (LSRAW*)*checkbox_on_data_H;
        
#endif
    

    Goutside=0;
    reset_3D();	//reset engine vars (important bug fix to stop crash when changing levels 6/5/99 SB)
	reset_objects();	//clear out ocb's
	reset_camera();
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
//set up lighting
	set_light_position(0,-300,0,3200);
	set_light_default_brightness(0);
	load_object ("\pJaegerV1bTXT.3DMF",0,'FALC',no_colldet,0,IS_NOT_TRANSLUCENT);	
	load_object ("\pshuttleV2.3dmf",1,'SHT1',no_colldet,0,IS_NOT_TRANSLUCENT);	
	load_ogl_textures();

    ship_slot=get_static_object_slot('SHT1');
	
    docked_load_ship(ship_slot,0,0,0,0,1);	//zex is at slot 0
    docked_load_ship(ship_slot,1,-400,0,3000,7);	//SHuttle, zex is at slot 0
	
	 set_main_camera_object( 0 );
     unfreeze_physics();
     frame_time=0;
	

    setup_main_screen(the_picture,button);

/*
 DSpContext_FadeGamma ( mDisplayContext, 0,
                               0);	//black
    setup_main_screen(the_picture,button);
    setup_main_screen(the_picture,button);
 
 DSpContext_FadeGamma ( mDisplayContext, 100,
                               0);
*/


    wait_flag=0;
    ShowCursor();
	
SetHighPriority();

	
//еееmain title screen display loop
	while (wait_flag==0)	//until mouse control says play
     {
      update_microsecs_timer();
	  thread_yield();

      if (Button()==0) button_has_been_up=1;
      
	set_poly_clipping(off_640_x,off_640_x+356,off_480_y,off_480_y+418);	//set poly clip rect - sb 181298	
	  
	  do_3d_for_main_menu();
	  
	set_poly_clipping(0,monitor_w,0,monitor_h);	//set poly clip rect - sb 181298	
      
//#endif	//of showships
     
     player_click=handle_mouse(button_down,button,the_picture,arrow_up,arrow_down,
                               arrow_up_down,arrow_down_down,checkbox_off,checkbox_on);
                 
     if(player_click==1)
       {
       ShowCursor();
       do_key_setting();

       fade_main_screen();
       setup_main_screen(the_picture,button);
       ShowCursor();
       }
                   
                               
     if (player_click==2)	//load game 
     {
//     load_result=load_game();	//1=ok, 0=canx
     load_result=1;
     if (load_result==1) 
     {
     fade_main_screen();
     is_faded=1;
     wait_flag=1;
     done_load_flag=1;
//     start_time=TickCount();
     }
     else done_load_flag=0;
     
     if (is_faded==0)
      {
       setup_main_screen(the_picture,button);
       setup_main_screen(the_picture,button);
//    setup_main_screen(the_picture,button);
      }
       ShowCursor();
     }


     if (player_click==5)	//register game 
     {
     fade_main_screen();
     setup_tables();	//1=ok, 0=canx
 DSpContext_FadeGamma ( mDisplayContext, 0,
                               0);	//black

    setup_main_screen(the_picture,button);
    setup_main_screen(the_picture,button);
      fade_up_main_screen();
    set_poly_clipping(off_640_x+0,off_640_x+356,off_480_y+0,off_480_y+419);	//set poly clip rect - sb 181298

//    setup_main_screen(the_picture,button);
    ShowCursor();

     }
     if (player_click==3 || player_click==7) 
     {
     if (player_click==3) 
     {
     get_level();	//get difficulty level
     }
     else
     fade_main_screen();
     
     is_faded=1;
     start_time=TickCount();
     wait_flag=1;	//play level, quit game
     accumulated_game_time=0;	//reset accum time on new game
     }
     
     
     if (player_click==6 || player_click==4)	//been doing the story/hiscore screen
     {
 //        load_object ("\pzex_title.z",0,'ZEXS',no_colldet,'T010',IS_NOT_TRANSLUCENT);
         ship_counter=0;	//reset back to zex on next frrame
 //        load_ship(loader_list[ship_counter].slot);  
         set_poly_clipping(off_640_x+0,off_640_x+356,off_480_y+0,off_480_y+419);	//fixes not clipping correctly in < 0.88

     }
     
     if (vbl_altered==1)
     {
     vbl_altered=0;
    setup_main_screen(the_picture,button);
    setup_main_screen(the_picture,button);
     
     }
//     do_object_control();
        MoveTo(off_640_x+0,off_480_y+10);
        TextFont(21);
        TextSize(9);
        TextFace(normal);
        RGBForeColor(&rgbBlue);

        if ( gzex_prefs.registered==1)
        {
        DrawString(VERS_STRING);
	}
        else
        {
        DrawString(VERS_STRING);
        DrawString("\p Unreg.");
	}

Show_it(); 

	}	//end of while

SetNormalPriority();

//fade_out_music();	

#if PORTABLE_FILESYSTEM
    delete select_data_pic_obj;
    delete button_data_pic_obj;
    delete button_down_data_pic_obj;

    delete arrow_up_data_pic_obj;
    delete arrow_down_data_pic_obj;
    delete arrow_up_down_data_pic_obj;

    delete arrow_down_down_data_pic_obj;
    delete checkbox_off_data_pic_obj;
    delete checkbox_on_data_pic_obj ;
#else
	 DisposeHandle(select_data_H);
     DisposeHandle(button_data_H);
     DisposeHandle(button_down_data_H);
     DisposeHandle(arrow_up_data_H);
     DisposeHandle(arrow_down_data_H);
     DisposeHandle(arrow_up_down_data_H);
     DisposeHandle(arrow_down_down_data_H);
     DisposeHandle(checkbox_off_data_H);
     DisposeHandle(checkbox_on_data_H);
#endif

    
    HideCursor();
     display_wait();	//

gzex_prefs.sound_volume=sound_volume;
gzex_prefs.music_volume=music_volume;
write_prefs();
freeze=0;	//game is not frozen
}

static int model_rot=0;
static int spin_phase=0;
static int ship_counter=0;

void do_3d_for_main_menu(void)
{
int ship_slot=0;
int scale=1;
#define textcol 246
    

//fiddle with objects in scene
//    reset_rotation_for_object(1);
    frame_time+=real_frame_time_in_secs;
	
	if (frame_time>(1/30))
	{
	frame_time=0;
	
	
	model_rot+=1;
	if (model_rot>359) 
	{
	 model_rot=0;
	 spin_phase++;
	 if (spin_phase>2) 
	 {	//update model
	   spin_phase=0;
	   ship_counter++;
	   if (ship_counter>1) ship_counter=0;
	   
	   if (ship_counter==0)
	   {
	      ship_slot=get_static_object_slot('SHT1');
		  scale=7;
	   }
	   else
	   if (ship_counter==1)
	   {
	      ship_slot=get_static_object_slot('FALC');
		  scale=1;
		}
	   
	
	   if (ship_slot==-1) report_error("Fatal Failure during main menu. Ship missing","\p",-4);
       i_know_what_im_doing_kill(1,TRUE);
	   docked_load_ship(ship_slot,1,-400,0,3000,scale);	//SHuttle, zex is at slot 0
	 }
	}
	
	reset_rotation_for_object(1);

    if (spin_phase==0)
	set_rotation_around_world_y(1,model_rot);
	else
	if (spin_phase==1)
	set_rotation_around_world_x(1,model_rot);
	else
	if(spin_phase==2)
	set_rotation_around_world_z(1,model_rot);
    }
	
	run_camera_control();
    render_frame(monitor_w,monitor_h);
	
			
      if (ship_counter==0)
      {
        SplatText_Transparent ("Shuttle",off_640_x+156,off_480_y+60,textcol);
        SplatText_Transparent ("Basic trading craft",off_640_x+10,off_480_y+80,textcol);
        SplatText_Transparent ("Main Thrust:     2000N/s x 2",off_640_x+10,off_480_y+100,textcol);
        SplatText_Transparent ("Unloaded Mass:   1.5 Tonnes",off_640_x+10,off_480_y+120,textcol);
        SplatText_Transparent ("Cargo Capacity:  5 Tonnes",off_640_x+10,off_480_y+140,textcol);
        SplatText_Transparent ("Laser Mount:     Front only, not military power feed.",off_640_x+10,off_480_y+160,textcol);
        SplatText_Transparent ("Cannons:         None",off_640_x+10,off_480_y+180,textcol);
        SplatText_Transparent ("Missiles:        None",off_640_x+10,off_480_y+200,textcol);
        SplatText_Transparent ("Allegiance:      Earth",off_640_x+10,off_480_y+220,textcol);
      }
	  else
      if (ship_counter==1)
      {
        SplatText_Transparent ("Falcon",off_640_x+156,off_480_y+60,textcol);
        SplatText_Transparent ("Multi-role navy craft",off_640_x+10,off_480_y+80,textcol);
        SplatText_Transparent ("Main Thrust:     20000N/s x 2",off_640_x+10,off_480_y+100,textcol);
        SplatText_Transparent ("Unloaded Mass:   3 Tonnes",off_640_x+10,off_480_y+120,textcol);
        SplatText_Transparent ("Cargo Capacity:  2",off_640_x+10,off_480_y+140,textcol);
        SplatText_Transparent ("Laser Mount:     Front, rear. Military power feeds.",off_640_x+10,off_480_y+160,textcol);
        SplatText_Transparent ("Cannons:         4",off_640_x+10,off_480_y+180,textcol);
        SplatText_Transparent ("Missiles:        3",off_640_x+10,off_480_y+200,textcol);
        SplatText_Transparent ("Allegiance:      Earth",off_640_x+10,off_480_y+220,textcol);
      }
//      else
//      if (ship_counter==1)
//      {
//        SplatText_Transparent_Fade ("FALCON",off_640_x+136,off_480_y+100,textcol);
//        SplatText_Transparent_Fade ("VELOCITY:   -100/200+",off_640_x+20,off_480_y+200,textcol);
//        SplatText_Transparent_Fade ("CANNONS:    Yes",off_640_x+20,off_480_y+220,textcol);
//        SplatText_Transparent_Fade ("LASER:      Yes",off_640_x+20,off_480_y+240,textcol);
//        SplatText_Transparent_Fade ("MISSILES:   Not as standard",off_640_x+20,off_480_y+260,textcol);
//        SplatText_Transparent_Fade ("PILOT:      Yes",off_640_x+20,off_480_y+280,textcol);
//        SplatText_Transparent_Fade ("ALLEGIANCE: Earth",off_640_x+20,off_480_y+300,textcol);
//        SplatText_Transparent_Fade ("Standard Earth attack craft.",off_640_x+20,off_480_y+320,textcol);
//
//        
//      }




//
//#define showships 1
////#define textcol 181
//#define textcol 246
//
//#if showships==1
//      if (ship_counter==0)
//      {
//        SplatText_Transparent_Fade ("ZEX",off_640_x+156,off_480_y+100,textcol);
//        SplatText_Transparent_Fade ("Upgradeable attack craft",off_640_x+80,off_480_y+120,textcol);
//        SplatText_Transparent_Fade ("VELOCITY:   -100/1000+",off_640_x+20,off_480_y+200,textcol);
//        SplatText_Transparent_Fade ("CANNONS:    Yes",off_640_x+20,off_480_y+220,textcol);
//        SplatText_Transparent_Fade ("LASER:      Yes",off_640_x+20,off_480_y+240,textcol);
//        SplatText_Transparent_Fade ("MISSILES:   3",off_640_x+20,off_480_y+260,textcol);
//        SplatText_Transparent_Fade ("PILOT:      Yes",off_640_x+20,off_480_y+280,textcol);
//        SplatText_Transparent_Fade ("ALLEGIANCE: Earth",off_640_x+20,off_480_y+300,textcol);
//        SplatText_Transparent_Fade ("Look after this!",off_640_x+20,off_480_y+320,textcol);
//
//        
//      }
//      else
//      if (ship_counter==1)
//      {
//        SplatText_Transparent_Fade ("FALCON",off_640_x+136,off_480_y+100,textcol);
//        SplatText_Transparent_Fade ("VELOCITY:   -100/200+",off_640_x+20,off_480_y+200,textcol);
//        SplatText_Transparent_Fade ("CANNONS:    Yes",off_640_x+20,off_480_y+220,textcol);
//        SplatText_Transparent_Fade ("LASER:      Yes",off_640_x+20,off_480_y+240,textcol);
//        SplatText_Transparent_Fade ("MISSILES:   Not as standard",off_640_x+20,off_480_y+260,textcol);
//        SplatText_Transparent_Fade ("PILOT:      Yes",off_640_x+20,off_480_y+280,textcol);
//        SplatText_Transparent_Fade ("ALLEGIANCE: Earth",off_640_x+20,off_480_y+300,textcol);
//        SplatText_Transparent_Fade ("Standard Earth attack craft.",off_640_x+20,off_480_y+320,textcol);
//
//        
//      }

      
}

int old_button_state=0;
//int button_y_array[7] = 
//    {133,170,207,244,281,318,354};
int button_y_array[7] = 
    {113,150,187,224,261,298,334};
    

int handle_mouse(LSRAW* button_down, LSRAW* button_up, 
                 LSRAW* the_picture,LSRAW* arrow_up,LSRAW* arrow_down,
                 LSRAW* arrow_up_down,LSRAW* arrow_down_down,
                 LSRAW* checkbox_off,LSRAW* checkbox_on)
{

extern int button_y_array[7];
extern int music_volume;
extern int sound_volume, Use_SpeedLimit;
extern int Use_VBL; // Change_res;

int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i,mvolume;
unsigned char number_string[2] = {0,0};
extern int old_button_state,last_click_time;
extern int quit;
extern int gMusicPlaying;
//copy bg for draw_bg
//	GetMouse(&mouse_xy);
    ZexGetMouse (&mouse_xy);
//    mouse_xy.h-=off_640_x;
//    mouse_xy.v+=off_480_y;
    
//    DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
    button_state=Button();
//    button_state=game_fire_button();
//draw lores checkbox
//draw speedlimit checkbox
    if (Use_SpeedLimit==0)
    print_crunched_trans_pict(checkbox_off,off_640_x+415,off_480_y+456);
    else
    print_crunched_trans_pict(checkbox_on,off_640_x+415,off_480_y+456);
    
//draw vbl checkbox
    if (Use_VBL==0)
    print_crunched_trans_pict(checkbox_off,off_640_x+380,off_480_y+456);
    else
    print_crunched_trans_pict(checkbox_on,off_640_x+380,off_480_y+456);

    #if OGL==0
//draw change res checkbox
    if (Change_res==0)
    print_crunched_trans_pict(checkbox_off,off_640_x+380,436);
    else
    print_crunched_trans_pict(checkbox_on,off_640_x+380,436);
    #endif
//draw sound and music arrows
    print_crunched_trans_pict(arrow_up,off_640_x+380,off_480_y+393);
    print_crunched_trans_pict(arrow_up,off_640_x+380,off_480_y+415);

    print_crunched_trans_pict(arrow_down,off_640_x+415,off_480_y+394);
    print_crunched_trans_pict(arrow_down,off_640_x+415,off_480_y+416);
//draw volumes
//music
    mvolume=music_volume;
    mvolume=mvolume>>3;
    number_string[0]=mvolume+0x30;	//make ascii 1-8
//    SplatText_Fade ((char *)number_string,off_640_x+403,419,textcol,255);

//sounds
    mvolume=sound_volume;
    mvolume=mvolume>>3;	//div 8 (0-63)
    number_string[0]=mvolume+0x30;	//make ascii 1-8
//    SplatText_Fade ((char *)number_string,off_640_x+403,396,textcol,255);
//draw arrow under mouse
//d8 - check for keys
//low res vid
//     #if OGL==0
//    if (mouse_xy.h>415 && mouse_xy.h<415+120 && mouse_xy.v>435 && mouse_xy.v<435+16)
//    {
//       if (Button() && button_has_been_up)
//       {
//       button_has_been_up=0;
////       Use_Mode320=Use_Mode320^1;
//       }
//    }  
//    #endif
//Use speed limiter
    if (mouse_xy.h>off_640_x+415 && mouse_xy.h<off_640_x+415+140 && mouse_xy.v>off_480_y+456 && mouse_xy.v<off_480_y+456+16)
    {
       if (Button() && button_has_been_up)
       {
       button_has_been_up=0;
       Use_SpeedLimit=Use_SpeedLimit^1;
       }
    }  

//Use vbl
      #if OGL==0

    if (mouse_xy.h>off_640_x+250 && mouse_xy.h<off_640_x+380+16 && mouse_xy.v>off_480_y+456 && mouse_xy.v<off_480_y+456+16)
    {
       if (Button() && button_has_been_up)
       {
       button_has_been_up=0;
       Use_VBL=Use_VBL^1;
       vbl_altered=1;
       }
    }  

//Change res
    if (mouse_xy.h>off_640_x+180 && mouse_xy.h<off_640_x+380+16 && mouse_xy.v>off_480_y+435 && mouse_xy.v<off_480_y+435+16)
    {
       if (Button() && button_has_been_up)
       {
       button_has_been_up=0;
       Change_res=Change_res^1;
       }
    }  
     #endif

    //up arrows
 if (Button() && button_has_been_up)
   {
    button_has_been_up=0;
    if (mouse_xy.h>off_640_x+380 && mouse_xy.h<off_640_x+396 && mouse_xy.v>off_480_y+393 && mouse_xy.v<off_480_y+409)
    {

       if (Button())
       {
       sound_volume+=8;
       if (sound_volume>63) 
       {
        sound_volume=63;
        play_zsound_always(low_beep, sound_high_pri, ls1_chan, sound_vol_7);
       }
       else
        {
        print_crunched_trans_pict(arrow_up_down,off_640_x+380,off_480_y+393);
        play_zsound_always(setup_sound, sound_high_pri, c1_chan1, sound_vol_7);
        } 
       
       }
    }  

    //music up
    if (mouse_xy.h>off_640_x+380 && mouse_xy.h<off_640_x+396 && mouse_xy.v>off_480_y+416 && mouse_xy.v<off_480_y+432)
    {
       if (Button())
       {
       music_volume+=8;
       
       if (gMusicPlaying==0)     play_zex_resource_now(131);
        
       if (music_volume>63) 
       {
        int old_volume;
        music_volume=63;
        old_volume=sound_volume;
        sound_volume=63;
        play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
        sound_volume=old_volume;
       }
       else
       print_crunched_trans_pict(arrow_up_down,off_640_x+380,off_480_y+416);
             
       music_set_vol(music_volume);
       }
    }  
    
    //down arrows
    if (mouse_xy.h>off_640_x+415 && mouse_xy.h<off_640_x+431 && mouse_xy.v>off_480_y+393 && mouse_xy.v<off_480_y+409)
    {
       if (Button())
       {
       sound_volume-=8;
       if (sound_volume<0) 
       {
        sound_volume=63;
        play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
        sound_volume=0;
       }
       else
        {
        print_crunched_trans_pict(arrow_down_down,off_640_x+415,off_480_y+394);
        play_zsound_always(setup_sound, sound_high_pri, c1_chan1, sound_vol_7);
        }
       }

    }
    //music down
    if (mouse_xy.h>off_640_x+415 && mouse_xy.h<off_640_x+431 && mouse_xy.v>off_480_y+417 && mouse_xy.v<off_480_y+433)
    {
       if (Button())
       {
       music_volume-=8;
       if (music_volume<8) 
       {
        int old_volume;
        music_volume=0;
        old_volume=sound_volume;
        sound_volume=63;
        play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
        sound_volume=old_volume;       
       }
       else
       print_crunched_trans_pict(arrow_down_down,off_640_x+415,off_480_y+417);
       
       if (music_volume<6)	//bit of a bug in the mod player wont let one set volume 0
        {
//        music_set_vol(1);
        stop_music_now();
        }
       else
        music_set_vol(music_volume);
       
       }

    }
  }
//first draw all buttons as off
    for (i=0; i<7;i++)
        print_crunched_pict(button_up,off_640_x+380,off_480_y+button_y_array[i]);
//now draw the down one
    for (i=0; i<7;i++)
    {
     if (mouse_xy.v>off_480_y+button_y_array[i] && mouse_xy.v<off_480_y+button_y_array[i]+36 && mouse_xy.h>off_640_x+390
         )	//controls
     {
     print_crunched_trans_pict(button_down,off_640_x+380,off_480_y+button_y_array[i]);
     return_val=i+1;
     }
    }
//check for keyboard shortcuts
if (check_key(0x2d)) { real_return_val=3; goto end; }	 //N key
if (check_key(0x01)) { real_return_val=1; goto end; }	 //S key
if (check_key(0x08)) { real_return_val=6; goto end; }	 //C key
if (check_key(0x0C)) { real_return_val=7; goto end; }	 //Q key
if (check_key(0x04)) { real_return_val=4; goto end; }	 //H key

//    else
//    print_crunched_pict(button_up,380,145);	//setup controls
//
//    if (mouse_xy.v>195 && mouse_xy.v<235 )	//sound
//    {
//    print_crunched_pict(button_down,380,195);
//    return_val=2;
//    }
//    else
//    print_crunched_pict(button_up,380,195);	//load
//
//    if (mouse_xy.v>243 && mouse_xy.v<283)
//    {
//    print_crunched_pict(button_down,380,243);
//    return_val=3;
//    }
//    else
//    print_crunched_pict(button_up,380,243);	//play
//
//    if (mouse_xy.v>370 && mouse_xy.v<410 )
//    {
//    print_crunched_pict(button_down,380,370);
//    return_val=4;
//    }
//    else
//    print_crunched_pict(button_up,380,370);	//credits
//  
//    if (mouse_xy.v>420 && mouse_xy.v<460)
//    {
//    print_crunched_pict(button_down,380,420);
//    return_val=20;
//    }
//    else
//    print_crunched_pict(button_up,380,420);	//quit
 

if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;
old_button_state=button_state;

end:

if (Button())        last_click_time=TickCount()+10;

//process real_return_val
if (real_return_val==1)
{
HideCursor();
     fade_main_screen();

//GraphicsPaused();
FlushEvents(FLUSH_ALL_EVENTS,0);	//flush all


///GraphicsActive();
 DSpContext_FadeGamma ( mDisplayContext, 0,
                               0);	//black

    setup_main_screen(the_picture,button_up);
    setup_main_screen(the_picture,button_up);
    fade_up_main_screen();
//    setup_main_screen(the_picture,button_up);

    ShowCursor();
}

if (real_return_val==4)	//hiscore
{
//	  Show_it();
//     display_wait();	  //
////    fade_out_music();
// //   music_set_vol(music_volume);
////    play_zex_resource_now(128);
//    do_hiscore_screen();
////    fade_out_music();
////    play_zex_resource_now(131);
////    music_set_vol(music_volume);
//
//    setup_main_screen(the_picture,button_up);
//    setup_main_screen(the_picture,button_up);
////    setup_main_screen(the_picture,button_up);

real_return_val=4;
}


if (real_return_val==6)
{
	Show_it();
//     display_wait();	  //
//    fade_out_music();
 //   music_set_vol(music_volume);
//    play_zex_resource_now(128);
    do_creds_screen();
//    fade_out_music();
//    play_zex_resource_now(131);
//    music_set_vol(music_volume);

    setup_main_screen(the_picture,button_up);
    setup_main_screen(the_picture,button_up);
 //   setup_main_screen(the_picture,button_up);

real_return_val=6;
}

//quit game
if (real_return_val==7)
{
quit=1;
}

return real_return_val;
}
//еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее



void do_creds_screen()
{
extern _3D camera_pos;

	camera_pos.x=0;
	camera_pos.y=0;
	camera_pos.z=-960;
//fade_and_play_zex_resource(134);

show_story_text();

fade_and_play_zex_resource(131);

}

void setup_main_screen(LSRAW* the_picture, LSRAW* the_button)
{
extern int button_y_array[7];

//CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;

//	  SetPort((GrafPtr)the_drawing_buffer);
//    GetPort (&savePort);
//	  saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//	  SetGDevice(mZexGDH);
//	  RGBBackColor (&rgbBlack);
//	  EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect(monitor_w,monitor_h);	//in pictures
//	  SetGDevice(saveGDH);	  //these are here so I can SEE them!
//	  SetPort(savePort);
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;
	print_crunched_trans_pict(the_picture,off_640_x+0,off_480_y+0);
	print_crunched_trans_pict(the_button,off_640_x+380, off_480_y+button_y_array[0]);
	print_crunched_trans_pict(the_button,off_640_x+380, off_480_y+button_y_array[1]);
	print_crunched_trans_pict(the_button,off_640_x+380, off_480_y+button_y_array[2]);
	print_crunched_trans_pict(the_button,off_640_x+380, off_480_y+button_y_array[3]);
	print_crunched_trans_pict(the_button,off_640_x+380, off_480_y+button_y_array[4]);
	print_crunched_trans_pict(the_button,off_640_x+380, off_480_y+button_y_array[5]);
	print_crunched_trans_pict(the_button,off_640_x+380, off_480_y+button_y_array[6]);
        
        	
	Show_it();

}

void load_star(int dyn_slot)
{
load_dyn_object_pb the_params;
int drot_z;
//extern int dynamic_object_index;

//    kill(0);	  //clear out old ship

//    reset_objects();
//set up ship for title screen
	 the_params.unique_id=0;
	the_params.world_x=RangedRdm(0,2000)-1000;
	the_params.world_y=RangedRdm(0,2000)-1000;
	the_params.world_z=RangedRdm(10,300);
	the_params.delta_rot_x=0;
	the_params.delta_rot_y=0;
	drot_z=RangedRdm(0,10)-5;
	if (drot_z==0) drot_z=5;
	
	the_params.delta_rot_z=drot_z;
//	the_params.delta_rot_z=0;	//230299

	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=0;
	the_params.controller_ref=0;
	the_params.control_use_1=0;
	the_params.control_use_2=0;
	the_params.control_use_3=0;
	the_params.control_use_4=0;
	the_params.control_use_5=0;
	the_params.attack_mode=0;
	the_params.normal.x =  0;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  -1;	//objects are ALWAYS defined facing right
	the_params.shield_value=1000;
	the_params.mass=10;
	the_params.dq=0;
	the_params.no_trivial_rejection=0;
	    the_params.object_category=HUMAN;
	 the_params.laser_cat=0;
	 the_params.cannon_cat=0;
	 the_params.missile_cat=0;
	 the_params.number_of_missiles=0;

	 the_params.unique_id=0;
     the_params.parent=-1;
	load_dyn_object(35,dyn_slot,&the_params,-1,1,-1,!DUST_BIT);	//the object, the position (-1=next free)
}

void load_ship(int static_obj)
{
load_dyn_object_pb the_params;
int drot_z;
//int i;
//extern int dynamic_object_index;
extern	DynObjectsFixedSize *ocb_ptr;

//rp 23/6/01    i_know_what_im_doing_kill(0);	    //clear out old ship
    if (ocb_ptr->object_list[0].in_use!=0)
    i_know_what_im_doing_kill(0,TRUE);	//clear out old ship


//    reset_objects();
//set up ship for title screen
	clear_pb(&the_params);
	
	the_params.world_x=00;
	the_params.world_y=0;
	the_params.world_z=0;
	the_params.delta_rot_x=RangedRdm(0,4)-2;
	the_params.delta_rot_y=RangedRdm(0,4)-2;
	drot_z=RangedRdm(0,4)-2;
	if (drot_z==0) drot_z=5;
	
	the_params.delta_rot_z=drot_z;
//	the_params.delta_rot_z=0;	//230299

	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=0;
	the_params.controller_ref=0;
	the_params.control_use_1=0;
	the_params.control_use_2=0;
	the_params.control_use_3=0;
	the_params.control_use_5=0;
	the_params.control_use_4=0;
	 the_params.attack_mode=0;
	the_params.normal.x =  0;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  -1;	//objects are ALWAYS defined facing right
	the_params.shield_value=1000;
	the_params.mass=10;
	the_params.dq=0;
	the_params.no_trivial_rejection=0;
	    the_params.object_category=HUMAN;
	 the_params.laser_cat=0;
	 the_params.cannon_cat=0;
	 the_params.missile_cat=0;
	 the_params.number_of_missiles=0;
	 the_params.unique_id=0;
	load_dyn_object(static_obj,0,&the_params,-1,1,-1,!DUST_BIT);	  //the object, the position (-1=next free)
    


}

void load_animated_ship(int static_obj, unsigned long frame1_id, unsigned long frame2_id, 
                   unsigned long frame3_id, unsigned long frame4_id)
{
load_dyn_object_pb the_params;
int drot_z;
//int i;
//extern int dynamic_object_index;
anim_pb anim;


    i_know_what_im_doing_kill(0,TRUE);	//clear out old ship
    i_know_what_im_doing_kill(1,TRUE);	//clear out old ship
    i_know_what_im_doing_kill(2,TRUE);	//clear out old ship
    i_know_what_im_doing_kill(3,TRUE);	//clear out old ship
    i_know_what_im_doing_kill(4,TRUE);	//clear out old ship
    i_know_what_im_doing_kill(5,TRUE);	//clear out old ship

//    reset_objects();
    anim.object_id[0]=frame1_id;
    anim.object_id[1]=frame2_id;
    anim.object_id[2]=frame3_id;
    anim.object_id[3]=frame4_id;

//set up ship for title screen
	the_params.world_x=00;
	the_params.world_y=0;
	the_params.world_z=0;
	the_params.delta_rot_x=RangedRdm(0,4)-2;
	the_params.delta_rot_y=RangedRdm(0,4)-2;

	drot_z=RangedRdm(0,4)-2;
	if (drot_z==0) drot_z=2;
	
	the_params.delta_rot_z=0;
	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=0;
	the_params.controller_ref=0;
	the_params.control_use_1=0;
	the_params.control_use_2=0;
	the_params.control_use_3=0;
	the_params.control_use_4=0;
	the_params.control_use_5=0;
	 the_params.attack_mode=0;
	the_params.normal.x =  0;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  -1;	//objects are ALWAYS defined facing right
	the_params.shield_value=1000;
	the_params.mass=10;
	the_params.dq=0;
	the_params.no_trivial_rejection=0;
	    the_params.object_category=HUMAN;
     the_params.parent=-1;

//	load_dyn_object(static_obj,0,&the_params);	//the object, the position (-1=next free)
    load_animated_object (&anim,4,3,1,1,&the_params,-1);


}



//еееееееееееееееееееееееее
//display the story and credits text
void show_story_text()
{

extern Ptr screenaddr;
extern int screenwidth;



//CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		        savePort;
load_dyn_object_pb the_params;

#if PORTABLE_FILESYSTEM
void *text_ptr;
#else
Handle text_h;
#endif
Ptr text;
int text_size=0;
int text_pos;
int i,temp_y,text_line_index,text_current_pos;	      //good old i
int end=0;
char the_char;
char text_line[1024];
unsigned long last_button_press_time=0;
#if PORTABLE_FILESYSTEM
ZexPicture* picture_pic_obj;
#else
Handle picture_H;
#endif
LSRAW* the_picture;
LSRAW* pictbuffer;
int memsize;
int scaling_factor;
int picture_x,picture_y;
int click ;
int scroll_pos,scroll_delta,colour;
//array of x coords for creds picture
int pict_x[30]= {
120,150,180,210,230,238,245,238,212,200,
190,165,150,140,120,100,90,78,63,51,
45,40,37,36,35,34,33,32,21,21};


int text_scaled_x[17] = {
    300,299,299,297,295,293,291,287,285,281,
    276,271,265,229,200,160,0
};

int text_scaled_y[17] = {
    240,239,239,237,235,233,231,227,215,201,
    176,171,165,129,100,100,0
};


//int wobble_count=00;
//int wobble_inc=1;
//int wobble_x[20]={
//120,115,110,105,100,105,110,115,120,125,
//130,135,140,145,150,145,140,135,130,125};

//int wobble_y[20]={
//120,115,110,105,100,105,110,115,120,125,
//130,135,140,145,150,145,140,135,130,125};

//variables for moveing picture in a circle
int r=10;	//radius
//int r_inc=1;	//radius inc/dec
int y_inc=2;
int x1=120,y1=100;	//circle center
int y=0;


//get picture
#if PORTABLE_FILESYSTEM
    picture_pic_obj = new ZexPicture ('RCZ ',2001);	//info pict   
    if (picture_pic_obj==0) report_error("Resource missing: RCZ 2001","\p",4);
    the_picture = picture_pic_obj->GetPictureRef();
#else
    picture_H=GetZexPicture ('RCZ ',2001);	//info pict   
 if (picture_H==0) report_error("Resource missing: RCZ 2001","\p",4);
	HLock(picture_H);
	the_picture = (LSRAW*)*picture_H;
#endif

//get text

 #if PORTABLE_FILESYSTEM
 text_ptr = ZGetResource('STRY',128, &text_size);  //Get the Handle to the Resource 
 if (text_ptr==0) report_error("Resource missing: STRY 128","\p",4);
 text = (char *)text_ptr;
 #else
 text_h = (Handle) ZGetResource('STRY',128);  //Get the Handle to the Resource 
 if (text_h==0) report_error("Resource missing: STRY 128","\p",4);
 HLock(text_h);
 text_size=GetHandleSize(text_h);
 text=*text_h;
 #endif
 
    enable_inputs();

//load in 100 stars
    reset_objects();	//clear out ocb's
    load_object ("\pstar.z",0,'ZEXS',no_colldet,0,IS_NOT_TRANSLUCENT);
    for (i=0;i<70;i++)
    {
	the_params.world_x=RangedRdm(1,4000)-2000;
	the_params.world_y=RangedRdm(1,4000)-2000;
	the_params.world_z=RangedRdm(1000,3000);
	the_params.delta_rot_x=0;
	the_params.delta_rot_y=0;
	the_params.delta_rot_z=0;
	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=RangedRdm(1,359);
	the_params.controller_ref=star_for_story;	//these zoom towards us
	the_params.control_use_1=0;
	the_params.control_use_2=0;
	the_params.control_use_3=0;
	the_params.control_use_4=0;
	the_params.control_use_5=0;
	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	the_params.shield_value=0;
	the_params.mass=10;
		 	the_params.dq=0;
	the_params.no_trivial_rejection=0;
	    the_params.object_category=ALIEN;
	 the_params.unique_id=0;
     the_params.parent=-1;
	load_dyn_object(0,i,&the_params,-1,1,-1,!DUST_BIT);	//the object, the position (-1=next free)    
    }

//set up drawing env.
    set_poly_clipping(0,monitor_w-1,0,monitor_h-1);	//set poly clip rect - sb 181298
    last_button_press_time=TickCount()+10;
    click=0;    
    text_current_pos=0;
#define SCROLLMAX 450;

    scroll_pos=SCROLLMAX;
    scroll_delta=-3;
    colour=0;
//change 200699 - we now print ot a pict and then zoom that on and off
    pictbuffer=(LSRAW*)NewPtr(640*500);
    pictbuffer->pwidth=640;
    pictbuffer->pheight=480;
    
  while(end==0)
  {  
//set up to draw into our picture buffer
    screenaddr=(char*)pictbuffer+4;
    screenwidth=monitor_w;
    erase_zex_rect(monitor_w,monitor_h);	//in pictures


//print the strings
    text_pos=text_current_pos;
    temp_y=50;
    for (i=0;i<24;i++)
    {
     text_line_index=0;
     the_char=0;
     while (the_char!=13 && text_line_index<1023)
      {
      the_char=text[text_pos];
      text_line[text_line_index]=the_char;
      text_pos++;
      text_line_index++;
      if (text_pos==text_size) //end of file
      end=1;
      }
      text_line[text_line_index-1]=0;	//make c string
    
      if (temp_y<460 && end==0) SplatText_Transparent_Fade (text_line,30,temp_y,246);
      temp_y+=14;
    }	//end of for
    
//set up for zoom on
scaling_factor=16;  
    while(click==0)
    {
     int scale_x_index,scale_x,scale_y;


//	  SetPort((GrafPtr)the_drawing_buffer);
//    GetPort (&savePort);
//	  saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//	  SetGDevice(mZexGDH);
//	  RGBBackColor (&rgbBlack);
    erase_zex_rect(monitor_w,monitor_h);	//in pictures
//	  EraseRect(&the_drawing_buffer->portRect);
//	  SetGDevice(saveGDH);	      //these are here so I can SEE them!
//	  SetPort(savePort);
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;

    render_frame(monitor_w-1,monitor_h-1);	//calls all the needed 3d stuff
    
//	  scroll_pos+=scroll_delta;
//	  if (scroll_pos<10) scroll_delta=-scroll_delta;
//	  else
//	  if (scroll_pos>450) scroll_delta=-scroll_delta;
	
    do_object_control();
//drae the picture of the text
    scale_x_index=16-scaling_factor;
    scale_x=text_scaled_x[scale_x_index];
    scale_y=text_scaled_y[scale_x_index];
    print_pict_scaled(pictbuffer,scale_x,scale_y,scaling_factor,1);
    if (scaling_factor>0) scaling_factor--;
    
    SplatText_Transparent ("Press Fire To Continue",240,460,colour);
    colour++;
    if (colour>210) colour=0;

	Show_it();
    
    if (TickCount()>last_button_press_time)	//else it gets silly
    click=game_fire_button();
    else    
     {
     gLaser=0;
     gCannon=0;
     }
    }	//end of gcannon
    
    last_button_press_time=TickCount()+10;
//now we need to get the next set of text
    text_current_pos=text_pos;
    gLaser=0;
    gCannon=0;
    click=0;

   }	//end of while (end)
DisposePtr((char*)pictbuffer);

//wait for final click
//now show info pict
//get memory to decompress picture
scaling_factor=25;
memsize=(*the_picture).pwidth;
memsize*=(*the_picture).pheight;
memsize+=80;
pictbuffer=(LSRAW*)NewPtr(memsize);
 if (pictbuffer==0) report_error("Memory Error","\p",-1);
 decrunch_pict(the_picture,pictbuffer);

   click=0;
   while(click==0)
   {

//	  SetPort((GrafPtr)the_drawing_buffer);
//    GetPort (&savePort);
//	  saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//	  SetGDevice(mZexGDH);
//	  RGBBackColor (&rgbBlack);
    erase_zex_rect(monitor_w,monitor_h);	//in pictures
//	  EraseRect(&the_drawing_buffer->portRect);
//	  SetGDevice(saveGDH);	      //these are here so I can SEE them!
//	  SetPort(savePort);
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;

    render_frame(monitor_w-1,monitor_h-1);	//calls all the needed 3d stuff
    if (scaling_factor>0)
    print_pict_scaled(pictbuffer,pict_x[scaling_factor],123,scaling_factor,1);
    else
    {
    picture_x=STATIC_CAST_TO_INT(x1+r*COS(y));
    picture_y=STATIC_CAST_TO_INT(y1+r*SIN(y));
    print_pict_scaled(pictbuffer,picture_x,picture_y,scaling_factor,1);
    y+=y_inc;
    if (y>359)  { y-=360; }
//    if (y<0)  { y+=360; }
    
//    r+=r_inc;
//    if (r>60)  r_inc=-r_inc; 
//    else
//    if (r<0) r_inc=-r_inc;
    
    }
    
    
    if (scaling_factor>0) scaling_factor--;
    
    do_object_control();	//do the stars
	Show_it();
    
    if (TickCount()>last_button_press_time)	//else it gets silly
    click=game_fire_button();
    else     gLaser=0;
   }	//end of gcannon
   
  
//zoom picture out
   while(scaling_factor<25)
   {

//	SetPort((GrafPtr)the_drawing_buffer);
//    GetPort (&savePort);
//	  saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//	  SetGDevice(mZexGDH);
//	  RGBBackColor (&rgbBlack);
//	  EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect(monitor_w,monitor_h);	//in pictures
//	  SetGDevice(saveGDH);	      //these are here so I can SEE them!
//	  SetPort(savePort);
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;

    render_frame(monitor_w-1,monitor_h-1);	//calls all the needed 3d stuff
    print_pict_scaled(pictbuffer,pict_x[scaling_factor],123,scaling_factor,1);
    if (scaling_factor<40) scaling_factor++;
    
    do_object_control();	//do the stars
	Show_it();
    
    if (TickCount()>last_button_press_time)	//else it gets silly
    poll_input_device();
    else     
     {
     gLaser=0;
     gCannon=0;
     }
   }	//end of gcannon
   
   
    set_poly_clipping(0,356,0,440);	//set poly clip rect - sb 181298
    gLaser=0;
   disable_inputs();
    reset_objects();	//clear out ocb's
    #if PORTABLE_FILESYSTEM
    ZReleaseResource(text_ptr);
    #else
    DisposeHandle(text_h);
    #endif
    DisposePtr((Ptr) pictbuffer);
    #if PORTABLE_FILESYSTEM
    delete picture_pic_obj;
    #else
    DisposeHandle(picture_H);
    #endif
}


void fade_main_screen()
{
int fade_percent;
UnsignedWide start_micros,end_micros;
	
    fade_percent=100;
    while(fade_percent>10)

     {
      DSpContext_FadeGamma ( mDisplayContext, fade_percent,
                               0);
      fade_percent-=5;

      
      Microseconds(&start_micros);	 //equalise


//      camera_pos.z=camera_pos.z+camera_pos_delta.z;
//      if (camera_pos.z>-950) camera_pos_delta.z=-camera_pos_delta.z;
//      if (camera_pos.z<-2900) camera_pos_delta.z=-camera_pos_delta.z;      
// 	run_animations();	  //in animation.c
//      render_frame_colour_bg(360,420,242);	  //calls all the needed 3d stuff
//
//#define textcol 246
//
//     
//
//     Show_it();	//splat 3d portion of window
    
 
     Microseconds(&end_micros);
     while (end_micros.lo-start_micros.lo <1000000/30)	//fps
         Microseconds(&end_micros);
    }	//end of while


 erase_zex_rect(monitor_w,monitor_h);	//in pictures
 Show_it();
 erase_zex_rect(monitor_w,monitor_h);	//in pictures
 Show_it();
 
 
 DSpContext_FadeGamma ( mDisplayContext, 100,
                               0);

}

void fade_up_main_screen()
{
int fade_percent;
UnsignedWide start_micros,end_micros;

    fade_percent=10;
    while(fade_percent<100)

     {
      Microseconds(&start_micros);	 //equalise

      DSpContext_FadeGamma ( mDisplayContext, fade_percent,
                               0);
      fade_percent+=10;

     Microseconds(&end_micros);
     while (end_micros.lo-start_micros.lo <1000000/30)	//fps
         Microseconds(&end_micros);
    }	//end of while

 
 
 DSpContext_FadeGamma ( mDisplayContext, 100,
                               0);

}

