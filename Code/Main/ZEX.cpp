/*йLightsoft 1998/9/0-03

Zex.cpp

060599 - Zex 1
030300 - Zex 2
060903 - Zex 2.3 in CVS

Here is what people have been up to:
$Header: /home/ls_cvs/ZEX2.3/Code/Main/ZEX.cpp,v 1.6 2003/09/28 17:30:11 robp Exp $
$Log: ZEX.cpp,v $
Revision 1.6  2003/09/28 17:30:11  robp
Changed files from .c to .cpp and removed spaces out of a couple of filenames.

Revision 1.5  2003/09/28 10:36:10  robp
Signed/Unsigned comparison fixes from last night, plus collision fix.

Revision 1.4  2003/09/27 21:52:43  robp
Fixed places where we were loading an unsigned with -1.

Revision 1.3  2003/09/19 20:54:52  robp
Removed items causing warnings.

Revision 1.2  2003/09/06 17:32:22  stu_c
Just a test change.

Revision 1.1.1.1  2003/09/05 22:35:58  stu_c
First Imported.

Revision 1.55  2002/09/08 02:18:53  stu_c
Precompiled header adjustments

Revision 1.54  2002/07/28 17:10:14  stu_c
Exhausts and further particle work

Revision 1.53  2002/07/07 03:43:58  stu_c
Lasers now work but need moving to POCB's

Revision 1.52  2002/07/04 22:40:53  stu_c
Deleted/edited/made more clear boot up messages

Revision 1.51  2002/06/24 21:38:32  stu_c
We now use aglSwapBuffers rather than glEnd. As such main loop has been reorganised to better schedule rendering versus control code.

Revision 1.50  2002/06/16 23:19:23  stu
Particles booked in for backup purposes - not working yet or complete.

Revision 1.49  2002/05/31 23:54:59  stu
Removed all warnings from the code

Revision 1.48  2002/05/19 03:18:20  stu
Guns fix

Revision 1.47  2002/05/05 22:26:14  stu
Further developement - hull temperature now correct for multiple suns and atmosphere.

All calls to add_to_kill_list check the return

Revision 1.46  2002/04/28 06:21:45  stu
This checkin adds more to the HUD and brings out some sim variables to the player.

Revision 1.45  2002/04/14 15:57:54  stu
Further work

Revision 1.44  2002/04/13 15:30:57  stu
Rotation, Alycians, behaviour

Revision 1.43  2002/03/19 23:22:07  stu
Call to NPC_task from main loop

Revision 1.42  2002/03/10 02:48:03  stu
Fix for suns/particles not being rendered

Revision 1.41  2002/03/09 15:17:21  stu
SKIP_MENU set to 0

Revision 1.40  2002/02/27 23:03:31  stu
removed test system form around run_pobs

Revision 1.39  2002/02/24 19:10:56  rob
init depth of field subsystem

Revision 1.38  2002/02/24 04:55:25  stu
If TEST_SYSTEM is 1, skip menu screen.

Revision 1.37  2002/02/24 03:36:45  stu
If TEST_SYSTEM is 1, skip menu screen.

Revision 1.36  2002/02/21 22:49:48  stu
No diffs?

Revision 1.35  2002/02/16 19:04:12  stu
Radar range decrease key and rationalisation of dlp colours

Revision 1.34  2002/02/15 23:23:48  stu
Call yield in main loop

Revision 1.33  2002/02/12 23:57:59  rob
Kill kill list always resets kill list
Added new dock code to dock.

Revision 1.32  2002/02/10 21:40:05  stu
average fps now reported at end of session

Revision 1.31  2002/02/03 19:29:06  stu
highest fps now reported at the end of a session

Revision 1.30  2002/02/02 23:01:27  stu
Now init_solar_map before we go to main loop

Revision 1.29  2002/01/29 22:56:30  rob
pobs change

Revision 1.28  2002/01/28 23:24:04  stu
Now plays madh00135 when returning to menu screen

Revision 1.27  2002/01/27 18:07:12  stu
Music changes

Revision 1.26  2002/01/20 19:38:28  stu
Just some top level logging

Revision 1.25  2002/01/20 16:47:59  stu
set_black_level calls

Revision 1.24  2002/01/09 23:48:31  stu
Furhter work on trading.

Revision 1.23  2002/01/02 19:27:37  rob
Added key setup code

Revision 1.22  2001/12/31 20:06:15  stu
Lurkers clean up

Revision 1.21  2001/12/29 23:03:08  stu
Added Goutside

Revision 1.20  2001/12/28 23:11:18  stu
Removed some annoying Success log messages

Revision 1.19  2001/12/28 18:37:17  stu
Just a few externs declared for debug

Revision 1.18  2001/12/27 19:45:53  stu
Added #if  EXEC_DEBUG_MESSAGES to control logging of debug messages

Revision 1.17  2001/12/14 19:42:01  stu
Docking

Revision 1.16  2001/11/28 22:40:38  stu
Removed background galaxies

Revision 1.15  2001/11/26 23:27:40  rob
Galaxy map changes

Revision 1.14  2001/11/24 19:27:15  stu
RA_line_len in place of sqrt

Revision 1.13  2001/11/09 18:29:12  rob
Various debug changes.

Revision 1.12  2001/11/06 00:26:42  rob
First stage of getting keyboard working.

Revision 1.11  2001/11/05 22:54:19  stu
Removal of debugging code for frame #1 crash on PB

Revision 1.10  2001/11/05 22:44:41  stu
Object thrust initialised correctly so engines doesn't crash on frame #1

Revision 1.9  2001/11/05 19:58:16  stu
More OSX work

Revision 1.8  2001/11/03 14:26:07  rob
Anvil build problems corrected.

Revision 1.7  2001/11/02 23:18:19  stu
Further OS X work. Enabled syscon logging.

Revision 1.6  2001/11/02 19:33:10  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.5  2001/10/27 18:58:31  rob
dual interface lib/carbon build

Revision 1.4  2001/10/26 22:45:28  rob
Anvil changes

Revision 1.3  2001/10/25 22:19:06  rob
Removed the OSX label.

Revision 1.2  2001/10/24 21:41:09  stu
First set of source to build on pb for carbon.

Revision 1.1  2001/10/20 23:59:58  rob
Various carbonisation fixes.

Revision 1.0.0.1  2001/10/17 20:46:03  rob
First Imported.

Revision 1.20  2001/09/23 19:00:00  stu
AP work, improved number print, engines.

Revision 1.19  2001/09/18 20:41:28  stu
More Sim/ hud numbers print

Revision 1.18  2001/09/10 21:38:23  stu
100901
More Sim, AP broken.

Revision 1.17  2001/09/07 20:40:54  rob
Added console debug_print
Added instant_console flag

Revision 1.16  2001/09/04 20:36:36  stu
Sim stuff

Revision 1.15  2001/07/29 10:35:41  rob
timing changes for coffee




 */

#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "ls_standard.h"
  #include "options.h"		// should always be first include
  #include "game_defs.h"

  #if  (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
   #include <Carbon/Carbon.h>
  #else
    #include <Quickdraw.h>
    #include <timer.h>
    #include <NumberFormatting.h>
    #include	<Processes.h>
  #endif
  #include "zex_misc.h"
  #include "error_handler.h"
  #include "3d_structures_def.h"
  #include "processor.h"
  #include "real_time.h"

#endif //pch



#include	"z_init.h"
#include	"Zsound.h"
#include	"splat_control.h"
#include	"drawing.h"
#include "3d_top.h"
#include "camera_control.h"
#include "init_memory.h"
#include "ship_control.h"
#include "dust.h"
#include	"object_control_top.h"
#include "explode.h"
#include "txtsplat.h"
#include "do_intro.h"
#include "fpoly4.h"
#include "draw_radar_and_cwp.h"
#include "load_objects.h"
#include "main_sel_screen.h"
#include "pictures.h"
#include "dlp.h"
#include "game_pause.h"
#include "docked.h"
#include "debug_timing.h"
#include "show_stats.h"
#include "inventory.h"
#include "instrum.h"
#include "light_control.h"
#include "OGL.h"
#include "load_objects.h"
#include "objects_misc.h"
#include "file_io.h"
#include "ship_kit.h"
#include "texture_management.h"
#include "hud.h"
#include "galaxy.h"
#include "physics.h"
#include "matrix_primitives.h"

#include "read_zex_ini.h"
#include "command_and_control.h"
#include "coffee_file_interface.h"
#include "console.h"
#include "coffee.h"
#include "engines.h"
#include "keyboard.h"
#include "key_setting.h"
#include "object_manipulation.h"


#include "POBS.h"
#include "3d_depth_of_field_manager.h"
#include "NPC.h"

//#define equalise 1
//#define debug_info
//fps
#define SKIP_MENU 0 //If 1 we skip right to the game with difficulty=1 and rank=1


//define the following to get various info up on screen
//#define timings
//#define copyright
//#define ship_pos
//#define object_info
//#define showfc

//proto
void render_frame(int width, int height);
//void render_frame_no_erase(int width, int height);
void render_frame_colour_bg(int width, int height, int colour);

void print_debug(int number,char* string);
void reset_debug();
void print_debug_blank();

#if !(MAC68K)
#if ZEX_CORE_CARBON==0 || INTERFACE_LIB_BUILD
QDGlobals qd;
#endif
#endif

int max_frame_rate=600;
int average_frame_rate=0;

int quit=0;	//global - if set to 1 we quit game
int difficulty,tutorial;
int can_gen_asteroids=0;
int game_pause;	//1=pause, 0=run
int game_over=0;
int number_of_polys_displayed;
int frame_counter;
int real_frame_counter;
int objs_displayed;
int do_launch,do_next_level;
int done_load_flag;	//from main sel screen - if 1 we've loaded ship from saved game
int ask_for_load;	//when we come bak from game if this is set we go straight into new game from load game
int next_level;
int have_initted;
UnsignedWide frame_timer1;
int var_a,var_b;	//debug vars
int fps,highest_fps,average_fps,run_time;	//global frames per second reading
int end_of_level_code;	//if not zero, we display end_of_level_reason_string
char end_of_level_reason_string[512];	//we can print this if eolc is not zero
int in_wormhole;
int training_mission,game_running,Goutside;	//>0 if we are in galaxy
int test1;
static int first_frame;

extern int Use_SpeedLimit,user_abort;
extern int exec_watchdog;
extern UInt32 gLaser;
extern int global_control_monsters;
extern int  level_has_control;
extern int gHideTheScreen;
extern int gen_dust_called;
extern int kill_dust_called;
extern int gShowConsole;
extern int global_show_status;
extern int rank;
extern int gourad_polys;

extern LSRAW *inv_picture_rgba;
extern LSRAW *inv_cursor_picture_rgba;
extern int inv_pict_reference;	//from contant images or -1 if not set
extern int inv_cursor_reference;
extern LSRAW *inv_picture;
extern float pitch_channel;
unsigned char number_string[14];
extern _3D vd;
extern	DynObjectsFixedSize *ocb_ptr;
extern int use_zbuffer_flag;
//ееееееееееееееееееееееееееееееееееееееееееееDEBUG STUFF
extern float debug_orcs_pitch, debug_orcs_roll, debug_orcs_yaw;
extern double cos_to_target,s_to_target;
extern double rot_to_target;
extern float debug_brake_time,debug_calcT;
extern vector debug_target_vector;
extern vector debug_calc_targ_vector,debug_heading_vector;
extern int binds;

//ееееееееееееееееееееееееееееееееееееееееееееMAIN


extern int show_dock;


/*!
    @function 
    @abstract   (main, where execution of Zex starts)
    @discussion (Zex main)
    @param      (none) (none)
    @result     (none)
*/

int main (void) {
EventRecord the_event;
int n;

extern int	zex_alive;	//how many dynamic objects to look at

extern int command_slot_dialog;
extern int docked_at_time;
GrafPtr		savePort;
GDHandle	saveGDH;

int	cant_run=0;


int start_time=0, end_time;
int heartbeat=0;
UnsignedWide start_micros,end_micros;
int next_frame_start_time;	//equaliser


//_ZPoly *the_polys;
extern int global_show_status;
extern int global_open_doors;	//door master on/off switch


extern int heat_brightness;

//еееееееееееStart



have_initted=0;

#if !ZEX_CORE_CARBON
  MaxApplZone();	//almost mandatory for any mac prog these days (sadly).
#endif


cant_run |= init_mac();	//do this or die...
	GetPort(&savePort);
	saveGDH = GetGDevice();	//these are here so I can SEE them being done
   
   #if TEST_SYSTEM==1 //if TE, skip main menu, go straight to game
   #define SKIP_MENU 1
   #endif
   
//	show_loading_screen();	//in misc
   #if ZEX_PROJECT_BUILDER==1
    fprintf(stderr, "\n==============================================================\n");
    fprintf(stderr,"\nZex: This version built with GCC version %i.%i\n",__GNUC__,__GNUC_MINOR__);
    //fprintf(stderr,"Details: %s \n",__VERSION__);
    fprintf(stderr,"on %s at %s\n\n",__DATE__, __TIME__);
   
   #if TEST_SYSTEM==1
    fprintf(stderr, "LSG: Zex (c) LSG 2003 Mach-O 0.73  Booting to Test Environment...\n\n");
   #else
    fprintf(stderr, "LSG: Zex (c) LSG 2003 Mach-O 0.73  Booting to System...\n\n");
   #endif
   #endif
   
   
disable_watchdog();

#if  EXEC_DEBUG_MESSAGES == 1
show_text_in_loading_screen("Exec: Initialising Memory \n");	  
#endif

InitThreads();
	
cant_run |= init_ocbs();
#if  EXEC_DEBUG_MESSAGES == 1
show_text_in_loading_screen("Exec: Initialising Memory: No problems, continuing...\n");	
#endif

get_zex_fsspec();


#if  EXEC_DEBUG_MESSAGES == 1
show_text_in_loading_screen("Exec: Init Console\n");	
#endif

init_console();
#if  EXEC_DEBUG_MESSAGES == 1	
show_text_in_loading_screen("Exec: Loading Coffee\n");	
#endif

get_coffee();

#if  EXEC_DEBUG_MESSAGES == 1

show_text_in_loading_screen("Exec: run_zex_ini() \n");	
#endif

read_zex_ini();
#if  EXEC_DEBUG_MESSAGES == 1
show_text_in_loading_screen("Exec: init_game() \n");	
#endif
init_depth_of_field();		// now we have the window size, initialise the 3d perspective depth of field

//start accepting events so dock stops bouncing


cant_run |= init_game();	//as it says






#if  EXEC_DEBUG_MESSAGES == 1
//show_text_in_loading_screen("\pExec: Loading 32 bit graphics...");	  

//load_32bit_graphics();

show_text_in_loading_screen("\n");	
#endif

if (cant_run) ExitToShell();	//stop if error before we get going
//stutest
//cfi_test();
//end test

#if  EXEC_DEBUG_MESSAGES == 1
show_text_in_loading_screen("Coffee: Loading/compiling main.zsl\n");	
#endif

complete_coffee_setup();

#if  EXEC_DEBUG_MESSAGES == 1
   show_text_in_loading_screen("Coffee: Loaded OK.\n");	
   show_text_in_loading_screen("Exec: Telling system we have launched OK... \n");	  
#endif

   for(n=0;n<20;n++)
   {
    WaitNextEvent(0xffff, &the_event, 3, 0);
   }
#if  EXEC_DEBUG_MESSAGES == 1
   show_text_in_loading_screen("Exec: Launch OK. Continue with boot... \n");	  
#endif




   enable_watchdog();	   //have to keep reseting this else we'll error

   reset_objects();	//clear out ocb's

#if !INTERFACE_LIB_BUILD
if (show_dock==0)
  {
   HideMenuBar();
  }
#endif

#if  EXEC_DEBUG_MESSAGES == 1
show_text_in_loading_screen("Zex: Requesting free memory check...\n");	

show_text_in_loading_screen("Exec: Free memory (K)=");
NumToString(FreeMem()/1024,number_string);
number_string[number_string[0]+1]='\0';
show_text_in_loading_screen((char *)&number_string[1]);
show_text_in_loading_screen("\n");
//  user_delay(30);
#endif


	 #if ZEX_PROJECT_BUILDER==1 && SHOW_OCB_SIZE==1
	  {
	  int size_of_object=sizeof(ZObject);

       fprintf(stderr, " \n");
       fprintf(stderr, "****\n");
       fprintf(stderr, "Zex: OCB size is %d bytes\n",size_of_object);
	   
	  }
	  
	  {
	  int size_of_object=sizeof(ZFObject);
       fprintf(stderr, "Zex: FOCB size is %d bytes\n",size_of_object);
	  }
	  {
	  int size_of_object=sizeof(ZPObject);
       fprintf(stderr, "Zex: POCB size is %d bytes\n",size_of_object);
       fprintf(stderr, "****\n");
	  }
	 #endif

if (FreeMem()<4000000)
report_error("Not enough memory to run. 4000K needed at this point. Available:","\p",FreeMem()/1024); 

//----------------------------------------------------


//set_map_update_rate(76);
//show_text_in_loading_screen("\pZex: NOTE: This test version expires 19 Oct 99.");
//show_text_in_loading_screen("\pExec: Checking Expiration");	  
//date check
//   GetDateTime(&date);
//   if (date>0xb431968d) report_error("Zex Expired. Check www.lightsoft.co.uk/zex/","\p",-1);


#if  EXEC_DEBUG_MESSAGES == 1	
show_text_in_loading_screen("Zex: Good Luck Player!\n ");	        
#endif

	game_running=0;
   //do_intro();	  //run the intro bits
    enable_inputs();
    
//   take_mouse_keyboard_as_input();	//120299 - take mouse and keyboard as input sprocket devices
//
//   insert_score(100,5);
//   untake_mouse_keyboard_as_input();	  //120299 - take mouse and keyboard as input sprocket devices
#if RELEASE==1
  user_delay(180); //wait for music to stop
#endif

#if  EXEC_DEBUG_MESSAGES == 1	
show_text_in_loading_screen("Exec: Stopping music\n");	
#endif

     stop_music_now();
 	 play_zex_resource_now(131);	// - was 131 main theme - starshine by Purple Motion

fred:   

	game_running=0;
    set_black_level(0); //0=black, 32000=white
	
//   disable_inputs();

   reset_objects();	//clear out ocb's
   reset_3D();	//reset engine vars
   clear_dlp();
   done_load_flag=0;
//   set_erase_colour(255);
	



    glDisable (GL_BLEND);
#if  EXEC_DEBUG_MESSAGES == 1
show_text_in_loading_screen("Zex: Running main menu.\n");
#endif
  

#if SKIP_MENU==0
   do_main_sel_screen();
   if (quit==1) goto end_game;	//useless comment goes here
#else
   #if ZEX_PROJECT_BUILDER==1
    fprintf(stderr, "SKIP_MENU==1. Use BYE from console to quit.\n\n");
   #endif
   difficulty=16;
   rank=1;
#endif

show_text_in_loading_screen("-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
show_text_in_loading_screen("Zex: New Game.\n\n");
   
#if  EXEC_DEBUG_MESSAGES == 1
show_text_in_loading_screen("Zex: Getting ready to run.\n");
#endif


	
    glEnable (GL_BLEND);
//    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency

run_next_level:  
	init_lights();

#if  EXEC_DEBUG_MESSAGES == 1
show_text_in_loading_screen("Zex: Taking keyboard as input\n");
#endif

   init_keyboard_input();
   
   set_erase_colour(255);
   global_open_doors=0;
   global_control_monsters=1;	//allow monster moves
   user_abort=0;
   docked_at_time=0;
//   remove_batcom_message();
   show_text_in_loading_screen("Zex: Reset maths\n");
   reset_3D();	//reset engine vars (important bug fix to stop crash when changing levels 6/5/99 SB)

   if (quit==0)	//if we're not quitting
   {   
    enable_inputs();

#if  EXEC_DEBUG_MESSAGES == 1
show_text_in_loading_screen("Zex: Reinit...\n");
#endif

    reinit_game();	//
//    reinit_screen();	  //set up control panel etc - in drawing.c
    init_constant_image();

    start_time=TickCount();

     global_open_doors=1;
    set_poly_clipping(0,monitor_w,0,monitor_h);	//set poly clip rect - sb 181298

    }

   ask_for_load=0;	//when we come back in it means player loaded a game
   
   if (!quit && done_load_flag==0) 
    { 
//     ogl_active();	  //as it says, in ogl.c
 show_text_in_loading_screen("Zex: Loading textures\n");
 
 load_ogl_textures();

   }
   first_frame=1;	//flag for main loop telling it is first frame
   
//   reinit_screen();
   
   if(done_load_flag==1) 
   {
   int loaded;

#if  EXEC_DEBUG_MESSAGES == 1
   show_text_in_loading_screen("Zex: Loading saved game\n");
#endif

   loaded=load_game();
   if (loaded==0) goto fred;	//user canx load
   goto finish_load;
   }

#if  EXEC_DEBUG_MESSAGES == 1
show_text_in_loading_screen("Zex: Setting up OBC display.\n");
#endif

   add_to_text_display("OBC> Booted. Version 1.00",DLP_GREEN); 
   
   add_to_text_display("OBC> All systems check OK. Running.",DLP_GREEN); 
  populate_dust_for_start();	//load in dust after map loaded

gen_dust_called=0;	//debug
kill_dust_called=0;

show_text_in_loading_screen("Zex: Fade out menu music\n");
     fade_out_music();

#define CRASH_DEBUG 1

//set_light_position(0,0,5000000,0);
#if TEST_SYSTEM==1
#if  EXEC_DEBUG_MESSAGES == 1
show_text_in_loading_screen("Zex: Setting test environment lighting\n");
#endif

	set_light_position(0,-500000000,0,0);
	set_light_default_brightness(0);
#endif

#if  EXEC_DEBUG_MESSAGES == 1
show_text_in_loading_screen("Zex: Creating stars\n");
#endif

//load_background_stars();
//show_text_in_loading_screen("Zex: Creating background graphics\n");
load_background_stars();

show_text_in_loading_screen("Zex: Running. Waiting for 'Quit'\n");
show_text_in_loading_screen("Zex: -=-=-=-SESSION REPORTING FOLLOWS=-=-=-=-\n");
SetHighPriority();

    set_black_level(4500); //0=black, 32000=white
init_solar_system_map();
highest_fps=0; average_fps=0; run_time=0;

fprintf (stderr, "Zex: Difficulty set to %d\n", difficulty);


//<<<<<<<<<<<<<<<Game loop>>>>>>>>>>>>>>>>>>>>>>>>>>>
heartbeat=0;

while(!quit)	//not end of game
{
//This is the main game loop. We need to go as long as possible without drawing
//as the card is busy rendering the last frame. If start drawing before it has finished
//rendering the scene we will just spin until it's ready. SB 220602

next_frame_start_time=(ZexTickCount()+2);	//30 fps
Microseconds(&start_micros);

reset_debug();
   #if ZEX_PROJECT_BUILDER==1
	  thread_yield();
   #endif
 
   
get_system_ticks();	//time for ZexTickCount
update_microsecs_timer();
update_real_time();	//tod clock

check_target();	//hud.c - need not run every single frame
check_sleep();

if(DUST_flag) gen_dust();
check_if_docked();	//in docked.c - will not return until player undocks - becomes new main loop    
run_periodic_sound_tasks();



// ** FRAME STARTS HERE _ DO NOT KILL ANYTHING BEFORE THIS ** //
kill_kill_list();
run_camera_control();	//in camera.c
 NPC_task();
 service_POBS();
 poll_input_device();
apply_control_inputs();
frame_counter++;

   #if ZEX_PROJECT_BUILDER==1
	  thread_yield();
   #endif
   
number_of_polys_displayed=0;

//This is where we start drawing. We should do as much calculation as possible before we
// get here as we'll spin if the card hasn't finished drawing the last frame.
if (global_show_status==0)	//we can render the frame
{
  set_poly_clipping(0,monitor_w-1,0,monitor_h-1);	//set poly clip rect - sb 181298
  render_frame(monitor_w,monitor_h);	//in this file - calls all the needed 3d stuff
  if (camera_get_view()==1) 
  {
   update_radar();
   run_dlp();
   run_dlp_text();	//run for text items and clear
   run_cwp();	//cpanel.c

  }
}	 //end of global show status==0
else //show status screen
{
  do_inventory_single_frame();	//file buy.c
  do_3d();	//3d stuff - transilates active object points to world_points_list_ptr
}


if (selectSolarMap)
   {
     do_solar_map_single_frame();
     do_3d();	//3d stuff - transilates active object points to world_points_list_ptr
   }
else if (gHideTheScreen==1 && selectGalacticMap)
   {
     do_galaxy_map_single_frame();
     do_3d();	//3d stuff - transilates active object points to world_points_list_ptr
   }

   #if ZEX_PROJECT_BUILDER==1
	  thread_yield();
   #endif

 run_hud(0);	//display hud symbology


//if (in_wormhole==0) show_nav();	//show directional arrow

run_console();

#if SHOW_PERCENTAGES_DEBUG

{
extern double timepercent[];
extern double timeav[];
int count;

  for(count=T_MIN; count<=T_MAX; count++)
    {
    NumToString((int)(timeav[count]/100000),number_string);
    number_string[number_string[0]+1]=0;	//quick p to c hack
    number_string[0]=' ';
    SplatText_Transparent ((char *)number_string,70+(30*count),130,10);

    NumToString((int)(timepercent[count]),number_string);
    number_string[number_string[0]+1]=0;	//quick p to c hack
    number_string[0]=' ';
    SplatText_Transparent ((char *)number_string,70+(30*count),140,10);
     }
}

#endif



T(T_DLP);

run_coffee(); //will draw to screen if debug panes on
T(T_COFFEE);

if (command_slot_dialog!=-1)
   run_control_dialog();	//in command_and_control
 
//do_object_control();

Show_it();	// - card is now busy; we can't draw to it so we do control
do_object_control();

#ifdef timings
Microseconds(&end_micros);
timer=(end_micros.lo-start_micros.lo);
#endif

explode_explode_list();
reset_explode_list();	//as it says


Microseconds(&end_micros);

while (end_micros.lo-start_micros.lo < STATIC_CAST_TO_UNSIGNED(1000000/max_frame_rate))	//fps
Microseconds(&end_micros);	//wait for time to pass. We should do something here maybe

average_fps++;
//measure real fps (might affect delta values)
end_time=TickCount();	//end of frame time
if (end_time-start_time>=60)	//calc fps 2 times/sec
 {
 run_time++;
 #if  ZEX_PROJECT_BUILDER==1 && PB_PRINT_TO_LOG_ENABLED==1 && HEARTBEAT_LOG==1
  if ((run_time%30)==0)
  { 
   _3D pos;
    get_raw_position(get_main_camera_object(),&pos);
    
    fprintf (stderr, "Exec: HEARTBEAT %d secs. Location x:%.0f y:%.0f z:%.0f\n", heartbeat+1,pos.x, pos.y, pos.z);
  }
 heartbeat++;
 #endif
 start_time=TickCount();
 fps=frame_counter;	/////  was *2
 if (fps>highest_fps) highest_fps=fps;
 frame_counter=0;
 }

real_frame_counter++;

//if dead, restart
if (zex_alive==0)
{
     
     if (ask_for_load==0)
     {
	 
   #if  ZEX_PROJECT_BUILDER==1
   fprintf (stderr, "Zex: -=-=-=-END OF SESSION=-=-=-\n");
   fprintf (stderr, "Zex: Highest Frame Rate = %i\n", highest_fps);
   fprintf (stderr, "Zex: Average Frame Rate = %i\n", average_fps/run_time);
   SetNormalPriority();
   #endif

     fade_and_play_zex_resource(135);
     game_over=1;
     heat_brightness=0;

     display_wait();	//
     if (ask_for_load==1) goto run_next_level;
     else
     {

#if  EXEC_DEBUG_MESSAGES == 1
	 show_text_in_loading_screen("Zex: Coffee_end_game\n");
#endif

     coffee_end_game();

     goto fred;
     }
    }
}

finish_load: 

if (game_pause==1)
 {
  do_game_pause();	//can set end of game or continue - file game_pause.c
  game_pause=0;
 }

//the game is NOT fully running until after the first frame!!!!
game_running=1; Goutside=1;  //If we are in this loop then we are outside

 

}	//end of while !quit
//еееееееееееееееееееееееEND GAME


end_game:  show_text_in_loading_screen("Exec: Clean up for quit\n");


     fade_out_music();
     display_wait();	//
     //do_exit();

     screen_quit();
      FlushEvents(FLUSH_ALL_EVENTS,0);	//flush all
#if  EXEC_DEBUG_MESSAGES == 1
show_text_in_loading_screen("Exec: ExitToShell\n");
#endif

ExitToShell();

return 0;
}

//еееееееееееееееееееееееееееееееееееееееее


void render_frame(int width, int height)
{

//exec_watchdog=100;	//have to keep reseting this else we'll error
do_3d();	//3d stuff - translates active object points to world_points_list_ptr
T(T_MATRIX);	 	 	 //maths

do_culling2();
T(T_CULLING);

do_pers3(width,height);	//translate points to view
T(T_PERS);

//do_zsort();	 //sort polys furthest first - this can safely be commented out for testing

if (gHideTheScreen==0 && global_show_status==0)
Draw_it(width,height); //furthest poly first

T(T_POLYDRAW);



//quick hack to show fps
#ifdef copyright
SplatText_Transparent ("Zex BY LSG й2003",120,30);
#endif

if (camera_get_view()==1)
{
  SplatText_Transparent ("Front View",(width/2-30),(height-130),216);
}
else
if (camera_get_view()==2)
{
  SplatText_Transparent ("Left View",(width/2-26),(height-130),216);
}
else
if (camera_get_view()==3)
{
  SplatText_Transparent ("Right View",(width/2-30),(height-130),216);
}
else
if (camera_get_view()==4)
{
  SplatText_Transparent ("Rear View",(width/2-26),(height-130),216);
}

//Show_it(CONTROL_PANEL_TOP);	//splat 3d portion of window
}	//end of render frame

//void render_frame_no_erase(int width, int height)
//{
////timing/debugging
////int ms_misc_start,ms_do_3d,ms_do_culling,ms_do_pers,ms_draw_it;
////int ms_misc_end,ms_total,ms_text_display,end_micros;
//
//exec_watchdog=100;	  //have to keep reseting this else we'll error
//
//do_3d();	  //3d stuff - translates active object points to world_points_list_ptr
//	  	  	  //etc.
//
//do_culling2();
//
//
//do_pers3(width,height);	//translate points to view
//Draw_it_no_erase(width,height); //furthest poly first
//
////quick hack to show fps
//#ifdef copyright
//SplatText_Transparent ("ZEX BY LSG й2000",120,30);
//#endif
//
////Show_it(CONTROL_PANEL_TOP);	//splat 3d portion of window
//}	//end of render frame


//render with a certain bg colour
void render_frame_colour_bg(int width, int height, int colour)
{
//timing/debugging
//int ms_misc_start,ms_do_3d,ms_do_culling,ms_do_pers,ms_draw_it;
//int ms_misc_end,ms_total,ms_text_display,end_micros;
exec_watchdog=100;	//have to keep reseting this else we'll error

//T(T_MISC_END);

do_3d();	//3d stuff - translates active object points to world_points_list_ptr
T(T_MATRIX);			//maths

do_culling2();
T(T_CULLING);

do_pers3(width,height);	//translate points to view

T(T_PERS);

//do_zsort();	//sort polys furthest first - this can safely be commented out for testing

Draw_it_colour_bg(width,height,colour); //furthest poly first
T(T_POLYDRAW);


//quick hack to show fps
#ifdef copyright
SplatText_Transparent ("Zex BY LSG й2003",120,30);
#endif

}



//void render_frame_no_erase(int width, int height)
//{
////timing/debugging
////int ms_misc_start,ms_do_3d,ms_do_culling,ms_do_pers,ms_draw_it;
////int ms_misc_end,ms_total,ms_text_display,end_micros;
//
//exec_watchdog=100;	  //have to keep reseting this else we'll error
//
//do_3d();	  //3d stuff - translates active object points to world_points_list_ptr
//	  	  	  //etc.
//
//do_culling2();
//
//
//do_pers3(width,height);	//translate points to view
//Draw_it_no_erase(width,height); //furthest poly first
//
////quick hack to show fps
//#ifdef copyright
//SplatText_Transparent ("ZEX BY LSG й2000",120,30);
//#endif
//
////Show_it(CONTROL_PANEL_TOP);	//splat 3d portion of window
//}	//end of render frame
