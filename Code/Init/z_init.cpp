/*

z_init.cpp
game init

$Log: z_init.cpp,v $
Revision 1.3  2003/09/28 17:29:34  robp
Changed files from .c to .cpp and removed spaces out of a couple of filenames.

Revision 1.2  2003/09/27 08:58:31  robp
Removal of Invalid Conversion Warning for C++ (I hate unsigned/signed chars)

Revision 1.1.1.1  2003/09/05 22:35:53  stu_c
First Imported.

Revision 1.22  2002/09/08 02:02:22  stu_c
Precompiled header adjustments

Revision 1.21  2002/05/05 22:26:40  stu
Further developement - hull temperature now correct for multiple suns and atmosphere.

All calls to add_to_kill_list check the return

Revision 1.20  2002/02/23 01:38:57  stu
Lurkers clean up

Revision 1.19  2002/02/10 21:35:59  stu
added a comment

Revision 1.18  2002/02/10 18:57:27  rob
*** empty log message ***

Revision 1.17  2002/02/02 23:00:38  stu
i_know_what_im_doing_kill now takes a BOOL telling it whether it should kill the textures.

Revision 1.16  2002/01/29 22:56:11  rob
pobs change

Revision 1.15  2002/01/27 18:13:06  stu
Music and init of no_buffet on new game

Revision 1.14  2001/12/31 20:05:30  stu
changes for load_level now taking a string for the filename

Revision 1.13  2001/12/29 01:10:34  stu
Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.

Revision 1.12  2001/12/28 23:09:39  stu
Fixed a problem releasing memory between games. Now reruns without errors - hurrah!

Revision 1.11  2001/12/27 19:45:22  stu
Added #if INIT_LOG==1 to control logging of debug info

Revision 1.10  2001/12/14 19:42:42  stu
Docking

Revision 1.9  2001/11/03 14:28:08  stu
Further OS X work

Revision 1.8  2001/11/02 23:20:31  stu
Further OS X work. Enabled syscon logging.

Revision 1.7  2001/11/02 19:30:51  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.6  2001/10/27 18:58:00  rob
dual interface lib/carbon build changes

Revision 1.5  2001/10/24 21:41:58  stu
First set of source to build on pb for carbon.

Revision 1.4  2001/10/22 21:28:34  rob
Carbon warning changes

Revision 1.3  2001/10/21 12:36:30  rob
Int to void change

Revision 1.2  2001/10/21 00:59:11  stu
Initial port work

Revision 1.1  2001/10/20 23:59:54  rob
Various carbonisation fixes.

Revision 1.0.0.1  2001/10/17 20:46:06  rob
First Imported.

Revision 1.8  2001/07/29 10:40:11  rob
minor bug fix

Revision 1.7  2001/06/24 21:54:12  stu
Removed some old camera init code

Revision 1.6  2001/06/13 18:18:34  rob
Clear dynamic object ratified

Revision 1.5  2001/06/03 03:17:56  stu
OCB structure now contains a sub structure called:
ZObjectControl
Which contains all the control data for an object.

Added accessor in access_ocbs which returns a pointer to type ZObjectControl

Revision 1.4  2001/04/02 13:41:59  rob
additions so coffee knows abotut the start and end of a game

Revision 1.3  2001/04/01 18:37:10  stu
*** empty log message ***

Revision 1.2  2001/01/02 18:56:32  stu
Dunno?

Revision 1.1.1.1  2001/01/01 21:13:36  rob
First Imported.

Revision 1.10  2000/12/11 19:50:07  stu
Light fix in 3d_top - calc_normal

Revision 1.9  2000/12/10 03:33:29  stu
Calls to kill replaced with either calls to add_to_kill_list OR
i_know_what_im_doing_kill

New light control calls

Revision 1.8  2000/12/03 23:02:12  rob
Fixed radar

Revision 1.7  2000/11/20 20:04:19  rob
init radar range

Revision 1.6  2000/11/16 20:34:45  rob
orbit

Revision 1.5  2000/09/20 23:46:54  stu
HUD/AP/Scaling

Revision 1.4  2000/09/13 20:31:23  stu
Changes for hud targetting and sun corona effects.

Revision 1.3  2000/08/27 19:42:01  rob
removed light day night movement code

Revision 1.2  2000/08/27 01:36:15  stu
Hud changes

Revision 1.1  2000/08/27 00:02:36  rob
added initial startup call

Revision 1.0.0.1  2000/08/21 22:03:28  stu
First Imported.

*/
//#include <DrawSprocket.h>
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include	"LS_Standard.h"
  #include "options.h"
  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <ConditionalMacros.h>
    #include <Dialogs.h>
    #include <Types.h>
    #include <Quickdraw.h>
    #include	<fonts.h>
  #endif
#include "zex_misc.h"
#include "processor.h"
#include "Error_handler.h"
#include "real_time.h"
#endif //pch


#include	"Zsound.h"	//sound public defs
#include	"splat_control.h"
#include	"z_init.h"
#include	"matrix_primitives.h"
#include	"file_io.h"
#include "load_objects.h"
#include	"ship_control.h"
#include "camera_control.h"
#include "ship_control.h"
#include "object_control_top.h"
#include "explode.h"
#include "load_game.h"
#include "texture_management.h"
#include "dlp.h"
#include "buy.h"
#include "light_control.h"
#include "instrum.h"
#include "pictures.h"
#include "ship_kit.h"
#include "options.h"
#include "cargo.h"
#include "command_and_control.h"
#include "ogl.h"
#include "coffee.h"
#include "draw_radar_and_cwp.h"
#include "keyboard.h"
#include "galaxy.h"
#include "docked.h"

#ifdef fpoly
#include "fpoly4.h"
#endif
#include "txtsplat.h"
int added_nav;

#include "POBS.h"

void setup_fgouraud(void);
void OGL_load_control_panel();
void clear_scaled_points(void);
CCrsrHandle our_cursor;

int disable_camera_control;

extern int game_over;
extern int  level_has_control;
extern int can_gen_dust;
extern int fastdock_in_progress,fastdock_fade_value;

extern kit_desc ship_items[ITEMS_COUNT];
extern int global_request_dock;

#if OGL
extern int OGL_active;
#endif
int init_mac(void)
{

#if INTERFACE_LIB_BUILD
extern QDGlobals qd;
	MaxApplZone();				// Grab application memory.

	InitGraf(&qd.thePort);		// Initialize QuickDraw variables for our program.
	InitFonts();				// Initialize fonts.
	InitWindows();				// Initialize the Window Manager.
	InitMenus();				// Ditto for the Menu Manager.
	TEInit();					// blah, blah Text Edit.
	InitDialogs(0L);			// blah, blah Dialog Manager.
#endif

#if !ZEX_CORE_CARBON
	GetDateTime((unsigned long *)&qd.randSeed);		// Randomize random seed.
#endif
	InitCursor();				// Set the cursor to the arrow cursor and init.
	
	
//	MoreMasters();				// Allocate a block of master pointers.
//	MoreMasters();				// And allocate more.
	
//	FlushEvents(everyEvent, 0);	// Clear event queue of any pending events.
	FlushEvents(mDownMask | keyDownMask | autoKeyMask, 0);
	
	#if OGL
	OGL_active=0;
	#endif
	
	return 0;
}

#if PORTABLE_FILESYSTEM
ZexPicture* float_dialog_pic_obj;
LSRAW *float_dialog;
ZexPicture* small_flare_pic_obj;
LSRAW *small_flare_picture;
ZexPicture* large_flare_pic_obj;
LSRAW *large_flare_picture;
ZexPicture* sleep_dialog_pic_obj;
LSRAW *sleep_dialog;
ZexPicture* order_dialog_pic_obj;
LSRAW *order_dialog;
LSRAW *map_cursor_picture;
ZexPicture* map_cursor_pic_obj;

//inv
LSRAW *inv_picture;
ZexPicture* inv_pic_obj;
LSRAW *up_picture;
ZexPicture* up_pic_obj;
LSRAW *down_picture;
ZexPicture* down_pic_obj;
LSRAW *armed_picture;
ZexPicture* armed_pic_obj;

LSRAW *inv_cursor_picture;
ZexPicture* inv_cursor_pic_obj;

LSRAW *radar_picture;
ZexPicture* radar_picture_pic_obj;

#else

Handle float_dialog_H;
LSRAW *float_dialog;
Handle small_flare_H;
LSRAW *small_flare_picture;
Handle large_flare_H;
LSRAW *large_flare_picture;
Handle sleep_dialog_H;
LSRAW *sleep_dialog;
Handle order_dialog_H;
LSRAW *order_dialog;
LSRAW *map_cursor_picture;
Handle map_cursor_H;

//inv
LSRAW *inv_picture;
Handle inv_H;
LSRAW *up_picture;
Handle up_H;
LSRAW *down_picture;
Handle down_H;
LSRAW *armed_picture;
Handle armed_H;

LSRAW *inv_cursor_picture;
Handle inv_cursor_H;

LSRAW *radar_picture;
Handle radar_picture_H;
#endif



//ogl 32 bit textures
LSRAW *inv_picture_rgba;
LSRAW *inv_cursor_picture_rgba;
//LSRAW* raw_rgba_data_CP;	  //ptr to the rgba data for the control panel
LSRAW* short_range_ret_rgba;
LSRAW* long_range_ret_rgba;

int inv_pict_reference;	//from constant images or -1 if not set
int inv_cursor_reference;	//same
int cp_reference;
int long_range_reticule_ref;
int short_range_reticule_ref;	//radar reticules

int	init_game(void)
{
int	return_val=0;
//LSRAW* pictbuffer;
//LSRAW* pictbuffer_rgba;
//UInt32 picture_w,picture_h;
//Ptr raw_rgba_data;
//Ptr raw_data;

//Handle the_file_data_H;
//LSRAW *the_picture;


//extern FSSpec Zex_FSSpec;
extern int monsters_can_fire,text_display_timer1,text_display_timer2;
extern Handle bpic;
extern int have_bpic;

//extern Ptr screenaddr;		// for cache line stuff
extern int end_of_level_code;	//if not zero, we display end_of_level_reason_string
extern int thrust_lockout;

POBs_init();

        init_wingmen_under_command();
#if INIT_LOG==1
show_text_in_loading_screen("Init: Loading Graphics\n");
#endif

        #if PORTABLE_FILESYSTEM
        float_dialog_pic_obj= new ZexPicture ('RCZ ',3203);	//floating dialog pict for tutorial   
        if (float_dialog_pic_obj==0) report_error("Resource missing: RCZ 3203","\p",4);
	float_dialog = float_dialog_pic_obj->GetPictureRef();

        radar_picture_pic_obj= new ZexPicture ('RCZ ',4500);	//floating dialog pict for tutorial   
        if (radar_picture_pic_obj==0) report_error("Resource missing: RCZ 4500 (Radar Grat)","\p",4);
	radar_picture = radar_picture_pic_obj->GetPictureRef();

        order_dialog_pic_obj= new ZexPicture ('RCZ ',3116);	//floating dialog pict for tutorial   
        if (order_dialog_pic_obj==0) report_error("Resource missing: RCZ 3116 - order box top","\p",4);
	order_dialog = order_dialog_pic_obj->GetPictureRef();
	
        map_cursor_pic_obj= new ZexPicture ('RCZ ',3304);	//cross hair pict   
	map_cursor_picture = map_cursor_pic_obj->GetPictureRef();

//inventory screens
        inv_pic_obj= new ZexPicture ('RCZ ',4000);	//inventory pict   
	inv_picture = inv_pic_obj->GetPictureRef();

        up_pic_obj= new ZexPicture ('RCZ ',3301);	//jett up pict   
	up_picture = up_pic_obj->GetPictureRef();

        down_pic_obj= new ZexPicture ('RCZ ',3300);	//jett down pict   
	down_picture = down_pic_obj->GetPictureRef();

        inv_cursor_pic_obj= new ZexPicture ('RCZ ',3302);	    //arrow pict   
	inv_cursor_picture = inv_cursor_pic_obj->GetPictureRef();

        armed_pic_obj= new ZexPicture ('RCZ ',3303);	//arrow pict   
	armed_picture = armed_pic_obj->GetPictureRef();

        #else

        float_dialog_H=GetZexPicture ('RCZ ',3203);	//floating dialog pict for tutorial   
        if (float_dialog_H==0) report_error("Resource missing: RCZ 3203","\p",4);
	HLock(float_dialog_H);
	float_dialog = (LSRAW*)*float_dialog_H;

        radar_picture_H=GetZexPicture ('RCZ ',4500);	//floating dialog pict for tutorial   
        if (radar_picture_H==0) report_error("Resource missing: RCZ 4500 (Radar Grat)","\p",4);
	HLock(radar_picture_H);
	radar_picture = (LSRAW*)*radar_picture_H;




        order_dialog_H=GetZexPicture ('RCZ ',3116);	//floating dialog pict for tutorial   
        if (order_dialog_H==0) report_error("Resource missing: RCZ 3116 - order box top","\p",4);
	HLock(order_dialog_H);
	order_dialog = (LSRAW*)*order_dialog_H;
	
	
        map_cursor_H=GetZexPicture ('RCZ ',3304);	//cross hair pict   
	HLock(map_cursor_H);
	map_cursor_picture = (LSRAW*)* map_cursor_H;



//inventory screens
    inv_H=GetZexPicture ('RCZ ',4000);	//inventory pict   
	HLock(inv_H);
	inv_picture = (LSRAW*)*inv_H;

    up_H=GetZexPicture ('RCZ ',3301);	//jett up pict   
	HLock(up_H);
	up_picture = (LSRAW*)*up_H;

    down_H=GetZexPicture ('RCZ ',3300);	//jett down pict   
	HLock(down_H);
	down_picture = (LSRAW*)*down_H;

    inv_cursor_H=GetZexPicture ('RCZ ',3302);	    //arrow pict   
	HLock(inv_cursor_H);
	inv_cursor_picture = (LSRAW*)* inv_cursor_H;

    armed_H=GetZexPicture ('RCZ ',3303);	//arrow pict   
	HLock(armed_H);
	armed_picture = (LSRAW*)* armed_H;
    #endif
        
#if INIT_LOG==1
 show_text_in_loading_screen("Init: Success\n");
#endif
//load_32bit_graphics();
#if INIT_LOG==1
	show_text_in_loading_screen("Init: init_dynamic_tecxtures()\n");
#endif

        init_dynamic_textures();


global_request_dock=-1;

thrust_lockout=0;

fastdock_in_progress=0; fastdock_fade_value=100;
 can_gen_dust=1;
 level_has_control=0;
 end_of_level_code=0;
  have_bpic=0;
 bpic=0;
 game_over=0;
 level_has_control=0;
//get files
//    our_cursor=GetCCursor(128);
//    SetCCursor(our_cursor);
#if INIT_LOG==1
show_text_in_loading_screen("Init: DLP Init\n");	
#endif
    init_dlp();  


#if INIT_LOG==1
show_text_in_loading_screen("Init: Reading Prefs...\n");	
#endif
	get_zex_fsspec();
    text_display_timer1=0;
    text_display_timer2=0;
    read_prefs();	//in file io


//init drawsprocket
#if INIT_LOG==1
show_text_in_loading_screen("Init: Screen Init\n");	
#endif

	return_val |= init_screen();



//show_loading_screen();
#if INIT_LOG==1
show_text_in_loading_screen("Exec: DrawSprocket initted OK.\n");	   
#endif
/*
show_text_in_loading_screen("Exec: Getting cache line size\n");	
	// after the screen is init'd we can use the screen address as a check for cache_line
	if(!return_val)
	  {
	  Ptr BigPtr;
	  
	  BigPtr=NewPtr(128000);
	  init_cache_line_size((char *)BigPtr);
	  DisposePtr(BigPtr);
	  }
*/
	  
#if INIT_LOG==1
show_text_in_loading_screen("Init: Initialising sound system\n");	
#endif
	return_val |= init_zsound();
	play_zex_resource_now(129);	// - was 131 main theme - starshine by Purple Motion

//    play_zsound_always(sound_test, sound_high_pri, right_chan, sound_vol_7);
#if INIT_LOG==1
show_text_in_loading_screen("Zex: Setting up player\n");	
#endif
	ship_init();
#if INIT_LOG==1
show_text_in_loading_screen("F4: Maths Init\n");	
#endif
	Z3D_TrigInit();	//set up our sin/cos tables

#ifdef fpoly
show_text_in_loading_screen("F4: Backend Init 1\n");
    init_polygon();
#endif
#if INIT_LOG==1
show_text_in_loading_screen("F4: Text Init\n");	
#endif
    InitText();
#if INIT_LOG==1
show_text_in_loading_screen("F4: Overlay Init\n");	
#endif

//    InitControlPanel();
    init_zex_random();

#if INIT_LOG==1
show_text_in_loading_screen("F4: Loading textures\n");	
#endif
        load_misc_graphics();
//        ogl_active();
	load_textures(); //now done in load_level
//	ogl_inactive();
#if INIT_LOG==1
show_text_in_loading_screen("F4: Backend Init 2\n");
#endif
    setup_fgouraud();
    monsters_can_fire=1;
#if INIT_LOG==1
show_text_in_loading_screen("F4: DLP Init 2\n");	
#endif

    clear_dlp();	//clear the 2d display processor
#if INIT_LOG==1
show_text_in_loading_screen("F4(Mac):Input Sprocket Init\n");	
#endif

   init_input_device();
#if INIT_LOG==1
show_text_in_loading_screen("F4: Engine Loaded - EOT 0\n");	
#endif

#if TRADE_LOG==1
show_text_in_loading_screen("Zex: Loading stocks                       \n  ");	  
#endif

    load_up_stock_strings();
//    init_batcom();
    read_in_cargo();
//    play_zsound_always(sound_test, sound_high_pri, left_chan, sound_vol_7);
	return return_val;
}


void load_32bit_graphics()
{
LSRAW* pictbuffer;
LSRAW* pictbuffer_rgba;
UInt32 picture_w,picture_h;
Ptr raw_rgba_data;
Ptr raw_data;

#if PORTABLE_FILESYSTEM
ZexPicture* the_file_data_pic_obj;
#else
Handle the_file_data_H;
#endif
LSRAW *the_picture;

        //load up 32 bit pictures
	#if OGL==1
	//inv pict 32 pict into some memory
       picture_w=	(*inv_picture).pwidth;
       picture_h=	(*inv_picture).pheight;	//now pointing to data

       pictbuffer=(LSRAW*)NewPtr(picture_w*picture_h+80);
       if (pictbuffer==0) report_error("Memory Error in init_game(OGL),ID=1","\p",-1);
       decrunch_pict(inv_picture,pictbuffer);
       invert_y_pict(pictbuffer);
       pictbuffer_rgba=(LSRAW*)NewPtr((picture_w*picture_h*4)+80);
       if (pictbuffer_rgba==0) report_error("Memory Error in print_crunched_trans_pict(OGL),ID=2","\p",-1);
   //now load up the texture
       raw_data=(char *)&pictbuffer->raw_data;
       raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
   //
       convert_to_rgba_wh(raw_data,raw_rgba_data,2,picture_w,picture_h);
       inv_picture_rgba=pictbuffer_rgba;
	pictbuffer_rgba->pwidth=picture_w;
	pictbuffer_rgba->pheight=picture_h;
        
        DisposePtr((Ptr)pictbuffer);

	//mouse cursor pict 32 pict into some memory
       picture_w=	(*inv_cursor_picture).pwidth;
       picture_h=	(*inv_cursor_picture).pheight;	//now pointing to data

       pictbuffer=(LSRAW*)NewPtr(picture_w*picture_h+80);
       if (pictbuffer==0) report_error("Memory Error in zinit(OGL),ID=2","\p",-1);
       decrunch_pict(inv_cursor_picture,pictbuffer);
       invert_y_pict(pictbuffer);
       pictbuffer_rgba=(LSRAW*)NewPtr((picture_w*picture_h*4)+80);
       if (pictbuffer_rgba==0) report_error("Memory Error in zinit(OGL),ID=3","\p",-1);
   //now load up the texture
       raw_data=(char *)&pictbuffer->raw_data;
       raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
   //
       convert_to_rgba_wh(raw_data,raw_rgba_data,2,picture_w,picture_h);
       inv_cursor_picture_rgba=pictbuffer_rgba;
	pictbuffer_rgba->pwidth=picture_w;
	pictbuffer_rgba->pheight=picture_h;

        DisposePtr((Ptr)pictbuffer);
////load up control panel pict
//    the_file_data_H=GetZexPicture ('ZRAW',400);	//control panel   
//    HLock(the_file_data_H);
//    
////	  FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"\pzcp1.RAW",&the_file);
////	  the_file_data_H=read_file(the_file);
//	  the_picture = (LSRAW*)*the_file_data_H;
//	  
//	  
//	  //mouse cursor pict 32 pict into some memory
//       picture_w=	(*the_picture).pwidth;
//       picture_h=	(*the_picture).pheight;	//now pointing to data
//
////       pictbuffer=(LSRAW*)NewPtr(picture_w*picture_h+80);
////       if (pictbuffer==0) report_error("Memory Error in zinit(OGL),ID=2","\p",-1);
////       decrunch_pict(the_picture,pictbuffer);
//       invert_y_pict(the_picture);
//       pictbuffer_rgba=(LSRAW*)NewPtr((picture_w*picture_h*4)+80);
//       if (pictbuffer_rgba==0) report_error("Memory Error in zinit(OGL),ID=3","\p",-1);
//   //now load up the texture
//       raw_data=(char *)&the_picture->raw_data;
//       raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
//   //
//       convert_to_rgba_wh(raw_data,raw_rgba_data,2,picture_w,picture_h);
//       raw_rgba_data_CP=pictbuffer_rgba;
//	  pictbuffer_rgba->pwidth=picture_w;
//	  pictbuffer_rgba->pheight=picture_h;
//
//DisposeHandle(the_file_data_H);
//

//load short range radar reticules
     
    #if PORTABLE_FILESYSTEM
        the_file_data_pic_obj= new ZexPicture ('RCZA',130);	//short range radar pict   
        the_picture = the_file_data_pic_obj -> GetPictureRef();
    #else
     the_file_data_H=GetZexPicture ('RCZA',130);	//short range radar pict   
    HLock(the_file_data_H);
    
	the_picture = (LSRAW*)*the_file_data_H;
    #endif
	
	
       picture_w=(*the_picture).pwidth;;	// Ian hard coded these rather than stick a header
                                //on his raw files!! SO I had to redo them
       picture_h=(*the_picture).pheight;;	//

       invert_y_pict(the_picture);
       pictbuffer_rgba=(LSRAW*)NewPtr((picture_w*picture_h*4)+80);
       if (pictbuffer_rgba==0) report_error("Memory Error in zinit(OGL),ID=3","\p",-1);
   //now load up the texture
       raw_data=(char *)&the_picture->raw_data;
       raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
   //
       convert_to_rgba_wh(raw_data,raw_rgba_data,2,picture_w,picture_h);
       short_range_ret_rgba=pictbuffer_rgba;
	pictbuffer_rgba->pwidth=picture_w;
	pictbuffer_rgba->pheight=picture_h;

#if PORTABLE_FILESYSTEM
delete the_file_data_pic_obj;
#else
DisposeHandle(the_file_data_H);
#endif

//long range reticules 
    #if PORTABLE_FILESYSTEM
        the_file_data_pic_obj= new ZexPicture ('RCZA',131);
        the_picture = the_file_data_pic_obj -> GetPictureRef();
    #else
     the_file_data_H=GetZexPicture ('RCZA',131);
    HLock(the_file_data_H);
    
	the_picture = (LSRAW*)*the_file_data_H;
    #endif
		
       picture_w=	(*the_picture).pwidth;
       picture_h=	(*the_picture).pheight;	//now pointing to data

       invert_y_pict(the_picture);
       pictbuffer_rgba=(LSRAW*)NewPtr((picture_w*picture_h*4)+80);
       if (pictbuffer_rgba==0) report_error("Memory Error in zinit(OGL),ID=3","\p",-1);
   //now load up the texture
       raw_data=(char *)&the_picture->raw_data;
       raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
   //
       convert_to_rgba_wh(raw_data,raw_rgba_data,2,picture_w,picture_h);
       long_range_ret_rgba=pictbuffer_rgba;
	pictbuffer_rgba->pwidth=picture_w;
	pictbuffer_rgba->pheight=picture_h;

#if PORTABLE_FILESYSTEM
delete the_file_data_pic_obj;
#else
DisposeHandle(the_file_data_H);
#endif
	#endif
}

int	reinit_game(void)
{
int	return_val=0;
//extern FSSpec Zex_FSSpec;
//extern int	zex_alive;
extern int monsters_can_fire,game_pause;
extern int frame_counter,real_frame_counter,objs_displayed,number_of_stars;
extern int freeze;
//extern int day_night_timer;
extern int score, credits, start_time;
extern int showing_stats,old_mothership_stock_slot_number,done_load_flag;
extern int docked_at;
extern int can_gen_cargo,rank,kills,promotion_points,tutorial;
extern float gradar_range;	//in cpanel
extern int done_corona_message;
extern int end_of_level_code;	//if not zero, we display end_of_level_reason_string
extern int thrust_lockout;
extern int no_buffet;
extern int ship_buffet;
#if TEST_SYSTEM==0
unsigned char level_filename[]="\pgame.zlv";
#else
unsigned char level_filename[]="\ptest.zlv";
#endif

POBs_init();
ship_buffet=0;
no_buffet=0;
done_corona_message=0;

gradar_range=RADAR_RANGE_SHORTEST;	//set to short range
rank=0; 
kills=promotion_points=0;

reset_docked();
	
init_game_time();
global_request_dock=-1;

can_gen_cargo=1;
thrust_lockout=0;

fastdock_in_progress=0; fastdock_fade_value=100;
 can_gen_dust=1;

 end_of_level_code=0;
	game_over=0;
	docked_at=-1;	//not docked
    old_mothership_stock_slot_number=-1;
    showing_stats=0;
    monsters_can_fire=1;
    game_pause=0;
    if (done_load_flag==0)
    {
     score=0;
     if (tutorial==1)
     credits=300;
     else
     credits=1000;
    }
    start_time=TickCount();
	ship_init();
//get files
	reset_camera();

     load_level(level_filename);	//load up all objects etc.
//    set_up_solar_system();


 //   if(done_load_flag==0)
 //   show_level_text();	//dont show the text on a load


//	return_val |= load_objects();	//load objects into static arrays
	reset_ship();
	coffee_start_new_game();
	
	frame_counter=real_frame_counter=objs_displayed=0;
    reset_kill_list();
    reset_explode_list();
    clear_dlp();	//clear the 2d display processor
    init_zex_random();
    number_of_stars=0;
    //set camera to zex
//    camera_to_zex();
    freeze=0;	//game is not frozen
    
    reset_microsecs_timer();


	return return_val;
}


//reset important engine vars
void reset_3D()
{
extern  _3D *points_list_ptr_reset;
extern  _3D *points_list_ptr;
extern textured_connections *connection_list_ptr;	//global connection list ptr
extern textured_connections *connection_list_ptr_reset;	//global connection list ptr
extern	_ZPoly *Spoly_list_ptr;
extern	_ZPoly *Spoly_list_ptr_reset;
extern int connection_list_index;
extern int points_list_index;
extern int world_points_list_index;
extern int dynamic_object_index;
extern int poly_list_index;

	connection_list_index=0;
	points_list_index=0;
	world_points_list_index=0;
	dynamic_object_index=0;
	poly_list_index=0;

	points_list_ptr=points_list_ptr_reset;	//reset static object points ptr
    connection_list_ptr=connection_list_ptr_reset;
    Spoly_list_ptr=Spoly_list_ptr_reset;	

}


//¥¥must be called before loading a new level
void clear_out_dyn()
{
extern int	dynamic_object_index;
int dest_ocb;

for (dest_ocb=0;dest_ocb<MAX_NUMBER_OF_DYN_OBJECTS;dest_ocb++)
	{
	i_know_what_im_doing_kill(dest_ocb,TRUE);	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.
	}

dynamic_object_index=0;
clear_scaled_points();
}


//¥¥must be called before loading a new level


extern scaled_points_array_type *scaled_points_ptr;

void clear_scaled_points(void)
{
int n;

for (n=0;n<NUMBER_OF_SCALED_OBJECTS;n++)
 {
   scaled_points_ptr->scaled_points[n].in_use=0;
 }

}



//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//resets static ocbs


void reset_static_slots(void)
{
extern	StatObjectsFixedSize *ocb_Sptr;
extern int	static_object_index;	//how many static objects

int i;
		for (i=0;i<MAX_NUMBER_OF_STAT_OBJECTS;i++)
		{
		(*ocb_Sptr).object_list[i].in_use=0;
		}
    static_object_index=0;

}


void load_misc_graphics()
{
//flares for galactic map screen
    #if PORTABLE_FILESYSTEM
    small_flare_pic_obj= new ZexPicture ('RCZA',3000);	//star pict   
    if (small_flare_pic_obj!=0)
	{
	small_flare_picture = small_flare_pic_obj->GetPictureRef();
        }
    else
        report_error("Not enough memory or resource missing.","\pRCZA",3000);

    large_flare_pic_obj= new ZexPicture ('RCZA',3001);	//star pict   
    if (large_flare_pic_obj!=0)
	{
	large_flare_picture = large_flare_pic_obj->GetPictureRef();
        }
    else
        report_error("Not enough memory or resource missing.","\pRCZA",3001);
        
    #else
    small_flare_H=GetZexPicture ('RCZA',3000);	//star pict   
	if (small_flare_H!=0)
	{
	HLock(small_flare_H);
	small_flare_picture = (LSRAW*)*small_flare_H;
    }
    else
        report_error("Not enough memory or resource missing.","\pRCZA",3000);

    large_flare_H=GetZexPicture ('RCZA',3001);	//star pict   
	if (large_flare_H!=0)
	{
	HLock(large_flare_H);
	large_flare_picture = (LSRAW*)*large_flare_H;
    }
    else
        report_error("Not enough memory or resource missing.","\pRCZA",3001);
    #endif

}


