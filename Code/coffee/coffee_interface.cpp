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


// *************************************MODULE HEADER*********************************
// * FILENAME           : coffee_interface.cpp
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 22:29:51 on 26th May 2000
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *					  Zex C interface to LSGF AKA Coffee
// *
// *
/* ***********************************************************************************
 * $Log: coffee_interface.cpp,v $
 * Revision 1.6  2003/09/28 18:04:30  robp
 * Fixed filename too long (!) problem, and booked in project file.
 *
 * Revision 1.5  2003/09/28 17:29:57  robp
 * Changed files from .c to .cpp and removed spaces out of a couple of filenames.
 *
 * Revision 1.4  2003/09/28 10:36:04  robp
 * Signed/Unsigned comparison fixes from last night, plus collision fix.
 *
 * Revision 1.3  2003/09/24 21:39:40  robp
 * More work on Portable file-system. No work should effect current operation on Zex. When it is up and running as the standard system, we can remove the #if conditional compilation. Currently the sound needs work, plus one ZGetResource before we can debug. Then rest of the file-system can be completed.
 *
 * Revision 1.2  2003/09/20 12:57:08  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:35:15  stu_c
 * First Imported.
 *
 * Revision 1.32  2002/09/08 02:17:19  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.31  2002/07/11 21:10:16  rob_c
 * Accessors for display
 *
 * Revision 1.30  2002/04/21 21:43:18  rob
 * Accessors to view rotation of planets & moons.
 *
 * Revision 1.29  2002/04/01 22:53:33  rob
 * More POBs data accessors
 *
 * Revision 1.28  2002/03/24 00:46:17  rob
 * Added commands so could check POBs data in a pane.
 *
 * Revision 1.27  2002/02/27 23:02:30  stu
 * Fixed motherships not geting taken out of view - too many visible polys error.
 *
 * Revision 1.26  2002/02/24 19:05:47  rob
 * set field of view angle commands for coffee
 *
 * Revision 1.25  2002/02/10 21:43:24  stu
 * binds declared to coffee
 *
 * Revision 1.24  2002/02/02 23:13:36  stu
 * Declared some 3d_top debug vars
 *
 * Revision 1.23  2002/01/27 18:11:52  stu
 * Declared some ORCS variables
 *
 * Revision 1.22  2002/01/20 19:38:51  stu
 * Declared ship_buffet to coffee
 *
 * Revision 1.21  2002/01/05 20:49:21  rob
 * LSGF must be more than 13, otherwise liable for crashing.
 *
 * Revision 1.20  2002/01/02 19:29:37  rob
 * Added key setup code
 *
 * Revision 1.19  2001/12/29 01:09:44  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.18  2001/11/26 23:13:44  rob
 * space change only
 *
 * Revision 1.17  2001/11/24 21:21:32  rob
 * Background galaxy changes
 *
 * Revision 1.16  2001/11/12 22:10:54  stu
 * Eclipse work
 *
 * Revision 1.15  2001/11/11 21:47:50  rob
 * Fixes switch screen blank problem.
 *
 * Revision 1.14  2001/11/09 18:28:42  rob
 * Various debug changes.
 *
 * Revision 1.13  2001/11/03 19:38:37  rob
 * Check for root.zsl success.
 *
 * Revision 1.12  2001/11/03 19:27:52  rob
 * Check for root.zsl success.
 *
 * Revision 1.11  2001/11/02 20:00:47  rob
 * PB build changes
 *
 * Revision 1.10  2001/11/02 19:30:49  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.9  2001/10/28 15:11:13  rob
 * Detached resource before resizing.
Checks for re-size or illegal addresses made.
 *
 * Revision 1.7  2001/10/27 22:10:22  stu
 * Furhter porting work
 *
 * Revision 1.6  2001/10/26 22:40:47  rob
 * Dual headers.
 *
 * Revision 1.0.0.1  2000/08/21 22:00:06  stu
 * First Imported.
 *
 */


// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "options.h"

  #include "3d_structures_def.h"
  #include "Error_handler.h"
  #include "zex_misc.h"
  #include "processor.h"
  #include "real_time.h"
#endif //pch


#include "file_io.h"
#include "PrefsFile.h"
#include	"splat_control.h"
#include "instrum.h"
#include "ship_kit.h"
#include "camera_control.h"
#include "docked.h"
#include "coffee.h"
#include "coffee_file_interface.h"
#include "console.h"
#include "fpoly4.h"
#include "fcircle.h"
#include "hud.h"
#include "ship_control.h"

#include "access_ocbs.h"
#include "dust.h"

#include "physics.h"
#include "object_control_top.h"
#include "dlp.h"

#include "load_objects.h"
#include "obj_control_misc_routines.h"
#include "3d_top.h"
#include "drawing.h"
#include "sds.h"
#include "engines.h"
#include "autopilot.h"
#include "static_object_support.h"
#include "keyboard.h"

#include "debug_timing.h"
#include "draw_radar_and_cwp.h"
#include "galaxy.h"
#include "3d_depth_of_field_manager.h"
#include "POBS.h"
#include "display_config_dialog.h"

// ***********************************************************************************
// * CONSTANTS 
// *


// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *


extern UInt32 gLaser;
extern int collisions,can_gen_asteroids;
extern int max_frame_rate;
extern int no_buffet;
extern int debug_orcs_pitch, debug_orcs_roll, debug_orcs_yaw;

//from 3d top

extern int debug_dynamic_objects;
extern int debug_radar_objects;
extern int debug_radius_objects;
extern int debug_in_frustum_objects;
extern int debug_polys_translated;

extern int view_fast_object;
extern int view_sun_object;
extern int view_planet_object;
extern int view_moon_object;
extern int view_mship_object;

extern int view_far_star_object;
extern int view_dust_object;
extern int view_other_object;


//from ogl
extern int binds;
// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

module_private int get_word_address(char *name);
//void declare_function(char *name,int *function(),int parameters,int return_param);

module_private void define_display_primitives(void);
module_private void switch_app(unsigned int process_id);
module_private void process_pending_updates(void);

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

void setup_coffee(void);

// ********************

#define FORTH_SIZE 500000
//char forthspace[FORTH_SIZE];
//Handle forth_space_handle;
Ptr forthspace;

#if PORTABLE_FILESYSTEM
char *coffee_loaded_ptr;
#else
Handle coffee_code_handle;
#endif

#if !CARBONIZE_COFFEE_INTERFACE
UniversalProcPtr coffee_code_ptr;
unsigned long coffeeProcInfo=0x3FFF1;	//7 in as above, 0 out, C
#endif

//ProcPtr coffee_main_addr;	// note: ProcPtr is in MacTypes.h and is typedef long (*ProcPtr)();
				// E.G. same as ... long (*coffee_main_addr)();

#if ZEX_PROJECT_BUILDER==0

typedef long (*coffee1ProcPtr)(long);
typedef long (*coffee2ProcPtr)(long,long);
typedef long (*coffee3ProcPtr)(long,long,long);
typedef long (*coffee4ProcPtr)(long,long,long,long);
typedef long (*coffee5ProcPtr)(long,long,long,long,long);
#else
typedef long (*coffee1ProcPtr)(long,long);
typedef long (*coffee2ProcPtr)(long,long,long);
typedef long (*coffee3ProcPtr)(long,long,long,long);
typedef long (*coffee4ProcPtr)(long,long,long,long,long);
typedef long (*coffee5ProcPtr)(long,long,long,long,long,long);

#endif

coffee1ProcPtr coffee_main_addr;


//(const void * source_bitmap, int source_rowbytes, int source_width, int source_height, void * dest_bitmap, 
//int dest_rowbytes)

extern AGLDrawable aglWindow;

//NOTE: Using 3.2 headers knewcopy et. al are obsolete
//This function uses the new names 
void get_coffee(void)
{
int hsize;
OSErr my_err;
CFragConnectionID coffee_conn_id;
Str255 ErrName;
Str63 fragname = "\pcoffee";
//long *clear_ptr;
//int i;

#if PORTABLE_FILESYSTEM
coffee_loaded_ptr = static_cast<char *>(ZGetResource('LSGF',128, &hsize));	// load and get the size of the resource
if (coffee_loaded_ptr==NULL) report_error("Exec: Couldn't load Coffee. Zex corrupt?","\p",4000);
#else
coffee_code_handle=ZGetResource('LSGF',128);
//coffee_code_handle=Get1Resource('LSGF',128);		// fetch the resource with the code in
//DetachResource(coffee_code_handle);			// make it only a memory handle

if (coffee_code_handle==0) report_error("Exec: Couldn't load Coffee. Zex corrupt?","\p",4000);

hsize=GetHandleSize(coffee_code_handle);		// get the size of the resource
HUnlock (coffee_code_handle);
#endif

#define OVERRUN_GAP 512					// protection only

#if PORTABLE_FILESYSTEM
coffee_loaded_ptr = static_cast<char *>(ZSetResourceSize(coffee_loaded_ptr, hsize+OVERRUN_GAP+FORTH_SIZE));	// set space for the forth environment as well
if(coffee_loaded_ptr==NULL)
    {
    report_error("Memory problem loading coffee","\p",-1);
    }

forthspace = coffee_loaded_ptr + OVERRUN_GAP + hsize;	// get to the beginning of forth empty space

// clear the forth program area - just in case
// clear_ptr=(long *)forthspace; for(i=0; i<(FORTH_SIZE/sizeof(long)); i++) { *clear_ptr=0; clear_ptr++; }

my_err=GetMemFragment(coffee_loaded_ptr, hsize, fragname, kPrivateCFragCopy, &coffee_conn_id, 
               (Ptr *) &coffee_main_addr,ErrName);	// prepare the code for running
#else
SetHandleSize(coffee_code_handle,hsize+OVERRUN_GAP+FORTH_SIZE);	// set space for the forth environment as well
if(MemError()) report_error("Memory problem loading coffee","\pMemError1",MemError());

if(GetHandleSize(coffee_code_handle) < hsize+OVERRUN_GAP+FORTH_SIZE) report_error("Memory problem loading coffee.","\p",4001);
HLock(coffee_code_handle);				// keep it in one position - moving code is bad.

//forthspace=NewPtrClear(FORTH_SIZE); // - old seperate memory allocation
//if(MemError()) report_error("Memory problem loading coffee","\pMemError2",MemError());

forthspace=*coffee_code_handle+OVERRUN_GAP+hsize;	// get to the beginning of forth empty space

// clear the forth program area - just in case
// clear_ptr=(long *)forthspace; for(i=0; i<(FORTH_SIZE/sizeof(long)); i++) { *clear_ptr=0; clear_ptr++; }

my_err=GetMemFragment(*coffee_code_handle, hsize, fragname, kPrivateCFragCopy, &coffee_conn_id, 
               (Ptr *) &coffee_main_addr,ErrName);	// prepare the code for running
#endif
// ensure on an even address
if(0x03 & (int)coffee_main_addr) report_error("Address error loading coffee","\p",4002);


if (my_err!=0) report_error("Couldn't link coffee (LSGF 128). Try giving ZEX more memory.","\p",my_err);

#if !CARBONIZE_COFFEE_INTERFACE
coffee_code_ptr=NewRoutineDescriptor (coffee_main_addr,coffeeProcInfo, GetCurrentISA());
#endif
setup_coffee();

}

#if ZEX_PROJECT_BUILDER==0
  #if CARBONIZE_COFFEE_INTERFACE

    #define coffee1(xx) (*(coffee1ProcPtr)coffee_main_addr)(xx)
    #define coffee2(xx,yy) (*(coffee2ProcPtr)coffee_main_addr)(xx,yy)
    #define coffee3(xx,yy,zz) (*(coffee3ProcPtr)coffee_main_addr)(xx,yy,zz)
    #define coffee4(ww,xx,yy,zz) (*(coffee4ProcPtr)coffee_main_addr)(ww,xx,yy,zz)
    #define coffee5(vv,ww,xx,yy,zz) (*(coffee5ProcPtr)coffee_main_addr)(vv,ww,xx,yy,zz)

  #else

    #define coffee1(xx) CallUniversalProc(coffee_code_ptr,coffeeProcInfo,xx)
    #define coffee2(xx,yy) CallUniversalProc(coffee_code_ptr,coffeeProcInfo,xx,yy) 
    #define coffee3(xx,yy,zz) CallUniversalProc(coffee_code_ptr,coffeeProcInfo,xx,yy,zz)
    #define coffee4(ww,xx,yy,zz) CallUniversalProc(coffee_code_ptr,coffeeProcInfo,ww,xx,yy,zz)
    #define coffee5(vv,ww,xx,yy,zz) CallUniversalProc(coffee_code_ptr,coffeeProcInfo,vv,ww,xx,yy,zz)

  #endif

#else   //pb build

  #define coffee1(xx) (*(coffee1ProcPtr*)coffee_main_addr) ((int)*(coffee1ProcPtr)coffee_main_addr,xx)
  #define coffee2(xx,yy) (*(coffee2ProcPtr*)coffee_main_addr) ((int)*(coffee1ProcPtr)coffee_main_addr,xx,yy)
  #define coffee3(xx,yy,zz) (*(coffee3ProcPtr*)coffee_main_addr) ((int)*(coffee1ProcPtr)coffee_main_addr,xx,yy,zz)
  #define coffee4(ww,xx,yy,zz) (*(coffee4ProcPtr*)coffee_main_addr) ((int)*(coffee1ProcPtr)coffee_main_addr,ww,xx,yy,zz)
  #define coffee5(vv,ww,xx,yy,zz) (*(coffee5ProcPtr*)coffee_main_addr) ((int)*(coffee1ProcPtr)coffee_main_addr,vv,ww,xx,yy,zz)

#endif


#define CELLS_IN_EACH_STACK 300

int c_string_interp_addr;
int get_address_of_word;
int declare_c_function;
int declare_func_advanced;

int declare_c_variable;

int run_zex_tasks;


extern int dynamic_object_index;
extern int quit;
extern	int number_of_polys_displayed;
extern int use_zbuffer_flag;
extern int objs_displayed;
extern int zbuffer_divisor;	//debug
extern double cos_to_target,s_to_target;
extern double rot_to_target;
extern int fps;	//global frames per second reading
extern int ship_buffet;
extern int camera_pan_force_debug;

void setup_coffee(void)
{
int testdata;

// set up coffee
c_string_interp_addr=coffee4(0,(long)forthspace,FORTH_SIZE,CELLS_IN_EACH_STACK);   
if(c_string_interp_addr==0) report_error("Coffee init error","\p",119);

// test an interpreted string - basic functionality of forth
testdata=interpret("12344 1 + set-param1");
if(testdata!=12345) report_error("Coffee init error","\p",12345);

// get some doing words
get_address_of_word=interpret("' get-address-of-word set-param1");
if(get_address_of_word==0) report_error("Coffee init error","\p",120);

declare_c_function=get_word_address("declare-c-function");
if(declare_c_function==0) report_error("Coffee init error","\p",121);

declare_func_advanced=get_word_address("declare-c-function-advanced");
if(declare_func_advanced==0) report_error("Coffee init error","\p",117);

declare_c_variable=get_word_address("declare-c-variable");
if(declare_c_variable==0) report_error("Coffee init error","\p",122);



// now lets check we have enough free space to proceed
if(interpret("UNUSED set-param1")<10000) report_error("Coffee init error","\pLow memory",123);

if(interpret("lsgf-core-ver set-param1") < 13 ) report_error("Coffee init error","\pLSGF Core too old",130);

// now lets start assembling the zex multitasking structure
interpret("BACKGROUND: console-task QUIT STOP ;"); // should call quit but currently crashes -
//or maybe reseting the RSP in quit?

interpret(": set-up-zex-tasks MULTI   quit-to-command-line console-task QUIT-MODE 's !   console-task WAKE ;");
interpret(": run-zex-tasks PAUSE ; ");

interpret("quit-returns-to-caller QUIT-MODE !");	// main task should return to caller


// lets get the main run address
run_zex_tasks=get_word_address("run-zex-tasks");
if(run_zex_tasks==0) report_error("Coffee init error","\p",124);


// lets patch in the forth i/o system to zex's console

declare_function("my-console-emit",console_emit,1,0);	// one param, no return
//declare_function("my-console-emit",console_emit,1,0);	// one param, no return
declare_function("my-console-cr",console_cr,0,0);		// no params, no return
declare_function("console-return-any-key",console_return_any_key,0,1);		// no params, one return
declare_function("KEY?",console_check_for_key,0,1);

interpret(" ' my-console-emit ' EMIT patch-word");
interpret(" ' my-console-cr ' CR patch-word");
interpret(" : my-console-key BEGIN console-return-any-key ?DUP 0= WHILE PAUSE REPEAT ;");
interpret(" ' my-console-key ' KEY patch-word");

// now lets patch in the file system to forth
declare_function("coffee-get-file-size",coffee_get_file_size,1,1);	// one param, one return
declare_function("coffee-read-file",coffee_read_file,2,1); // two params, one return
declare_function("coffee-write-file",coffee_write_file,3,1); // three params, one return
declare_function("coffee-get-file-spec",coffee_get_file_spec,1,1); // one param, one return
declare_variable("directory",&directory);

declare_variable("console-colour",&console_colour);
declare_variable("cursor-colour",&cursor_colour);
declare_variable("console-buffer",&console_buffer[0]);
declare_variable("console-attrib-buffer",&console_attrib_buffer[0]);
declare_variable("no_buffet",&no_buffet);

declare_constant("console-length",CONSOLE_LENGTH);

declare_constant("console-columns",CONSOLE_COLUMNS);
declare_constant("console-rows",CONSOLE_ROWS);

declare_constant("ARROW_UP",ARROW_UP);
declare_constant("ARROW_DOWN",ARROW_DOWN);
declare_constant("ARROW_LEFT",ARROW_LEFT);
declare_constant("ARROW_RIGHT",ARROW_RIGHT);
declare_constant("ESCAPE",ESCAPE);


declare_variable("cursor-char-x",&cursor_char_x);
declare_variable("cursor-char-y",&cursor_char_y);

declare_variable("instant_console",&instant_console);

declare_variable("backface_cull",&backface_cull);
declare_variable("rendering",&rendering);
declare_variable("laser",&gLaser);
declare_variable("collision",&collisions);

declare_variable("ZMin",&ZMin);
declare_variable("depth_of_field",&depth_of_field);
declare_variable("ship_buffet",&ship_buffet);

declare_variable("orcs_pitch",&debug_orcs_pitch);
declare_variable("orcs_roll",&debug_orcs_roll);
declare_variable("orcs_yaw",&debug_orcs_yaw);


declare_variable("debug_dynamic_objects",&debug_dynamic_objects);
declare_variable("debug_radar_objects",&debug_radar_objects);
declare_variable("debug_radius_objects",&debug_radius_objects);
declare_variable("debug_in_frustum_objects",&debug_in_frustum_objects);
declare_variable("debug_polys_translated",&debug_polys_translated);
declare_variable("binds",&binds); 

declare_variable("view_fast_object",&view_fast_object);
declare_variable("view_sun_object",&view_sun_object);
declare_variable("view_planet_object",&view_planet_object);
declare_variable("view_moon_object",&view_moon_object);
declare_variable("view_far_star_object",&view_far_star_object);
declare_variable("view_dust_object",&view_dust_object);
declare_variable("view_other_object",&view_other_object);
declare_variable("view_mship_object",&view_mship_object);



// =====================================================
// define some various accessor words and variables
declare_function("freeze-physics",freeze_physics,0,0);
declare_function("unfreeze-physics",unfreeze_physics,0,0);
declare_function("physics-frozen",physics_frozen,0,1);	// one return

declare_function("reset-debug",reset_debug,0,0);
declare_function("print-debug-blank",print_debug_blank,0,0);
declare_function("print-debug",print_debug,2,0);

define_display_primitives();

declare_function("GraphicsActive",GraphicsActive,0,0);
declare_function("GraphicsInactive",GraphicsInactive,0,0);
declare_function("GraphicsPaused",GraphicsPaused,0,0);
declare_function("ShowCursor",ShowCursor,0,0);
declare_function("switch_app",switch_app,1,0);
declare_function("process_pending_updates",process_pending_updates,0,0);




// =====================================================
// added for controllers
declare_function("RangedRdm",RangedRdm,2,1);

// parameter as 0=last, 1=int, 2=double, 3=single, 4=double in both float and 2 gpr registers, 5=single in both float and gpr register
//     ... use the "in both" for direct structures that are floating point, and variable parameter calls  
// Return 0=none, 1=int, 2=double or single

declare_advanced_function("get_distance_to_from_in_m",get_distance_to_from_in_m,0x11000000,2); // 2 int parameters, double return
declare_function("add_to_kill_list",add_to_kill_list,1,1);	// 1 parameter, 1 return
declare_function("add_to_text_display",add_to_text_display,2,0);


// =====================================================
// object control connections

declare_variable("dynamic-object-index",&dynamic_object_index);
declare_function("get-object-controller",get_object_controller,1,1);	// one param, one return
declare_function("get-object-in-use",get_object_in_use,1,1);	// one param, one return
declare_function("find_vacant_dynamic_object_slot",find_vacant_dynamic_object_slot,0,1); // just 1 return
declare_function("clear_pb",clear_pb,1,0); // one param, no return
declare_function("get_raw_position",get_raw_position,2,0); // 2 param, 0 return
declare_function("get_static_object_slot",get_static_object_slot,1,1); // 1 parameter, 1 return
declare_function("report_error",report_error,3,0);
declare_advanced_function("load_dyn_object",load_dyn_object,0x11113110,1);
declare_function("get_object_abs_vel_vector",get_object_abs_vel_vector,2,0);	// 2 param, 0 ret ( returned structure is pointer parameter )
declare_advanced_function("set_object_abs_vel_vector",set_object_abs_vel_vector,0x14440000,0); // 2 params: 1 int + 3 double structure, no return

declare_function("get_object_Oid",get_object_Oid,1,1);	// 1 param, 1 return

// =====================================================
// load object models

declare_function("raw_c_load_object",load_object,6,1);	// 6 params, 1 return
declare_function("get_main_camera_object",get_main_camera_object,0,1);
declare_function("set_main_camera_object",set_main_camera_object,1,1);	//takes dynamic slot, returns error
declare_function("get_object_targetted",get_object_targetted,1,1);	// takes dyn slot, returns object targetted


declare_constant("MAX_NUMBER_OF_STAT_OBJECTS",MAX_NUMBER_OF_STAT_OBJECTS);
declare_function("STATIC_get_object_Oid",STATIC_get_object_Oid,1,1);
declare_function("STATIC_get_object_in_use",STATIC_get_object_in_use,1,1);
declare_advanced_function("STATIC_get_object_radius_in_metres",STATIC_get_object_radius_in_metres,0x10000000,2);

declare_function("get_position_in_m",get_position_in_m,2,0);
declare_function("get_velocity_in_mps",get_velocity_in_mps,2,0);
declare_advanced_function("set_velocity_in_mps",set_velocity_in_mps,0x14440000,0);
declare_function("get_object_radius_in_metres",get_object_radius_in_metres,1,1);
declare_function("get_translated_normal",get_translated_normal,2,0);

declare_variable("FILE_LOAD_ERROR_QUITS",&FILE_LOAD_ERROR_QUITS);


// =====================================================
//mounting points

declare_function("init_ship_data",init_ship_data,0,0);	// no return
declare_function("load_ship_data",load_ship_data,1,0);	// one in (ptr to mount_pb) no return
declare_function("sds_update",sds_update,0,0);	// no return


// =====================================================
// Input manager
declare_function("load_key_def",load_key_def,1,0);	// one in (ptr to int), no return



// =====================================================
// other controls

declare_variable("radar_grat",&radar_grat);	// radar graiticule draw switch

declare_variable("hud",&hud_drawn);		// hud control variable
declare_variable("fuel",&can_consume_fuel);	// fuel consumption 
declare_variable("zbuffer",&use_zbuffer);		// zbuffer control variable
declare_variable("asteroids",&can_gen_asteroids);	// asteriod control variable
declare_variable("dust",&DUST_flag);		// variable so we can turn zbuffer on and off

declare_variable("quit-var",&quit);		// the variable that quits the game

// =====================================================
// time controls

declare_constant("T_MATRIX",T_MATRIX);
declare_constant("T_CULLING",T_CULLING);
declare_constant("T_PERS",T_PERS);
declare_constant("T_COFFEE",T_COFFEE);

declare_constant("T_POLYDRAW",T_POLYDRAW);
declare_constant("T_DLP",T_DLP);
declare_constant("T_MISC",T_MISC);
declare_constant("T_SWAP",T_SWAP);

declare_constant("T_MIN",T_MIN);
declare_constant("T_MAX",T_MAX);

declare_variable("game_timing",&game_timing);
declare_variable("frames_to_reset_after",&frames_to_reset_after);

declare_variable("timemin",timemin);		// array of doubles
declare_variable("timemax",timemax);		// array of doubles
declare_variable("timetotal",timetotal);	// array of doubles
declare_variable("timeav",timeav);		// array of doubles
declare_variable("timepercent",timepercent);	// array of doubles

declare_variable("timed_frames",&timed_frames);
declare_variable("total_frames",&total_frames);

// =====================================================

declare_variable("time_in_secs",&time_in_secs);		// time taken to do this frame

declare_constant("TEST_SYSTEM",TEST_SYSTEM);		// whether we are running a test system
declare_variable("max_fps",&max_frame_rate);


// =====================================================
// =====================================================
// =====================================================
// =====================================================

declare_variable("number_of_polys_displayed",&number_of_polys_displayed);
declare_variable("use_zbuffer_flag",&use_zbuffer_flag);
declare_variable("objs_displayed",&objs_displayed);
declare_variable("zbuffer_divisor",&zbuffer_divisor);

declare_variable("cos_to_target",&cos_to_target);
declare_variable("rot_to_target",&rot_to_target);
declare_variable("s_to_target",&s_to_target);

declare_variable("fps",&fps);


//sim pane exports
declare_variable("camera_object_total_fuel",&camera_object_total_fuel);
declare_variable("camera_object_total_fuel_consumption",&camera_object_total_fuel_consumption);
declare_variable("camera_object_orcs_fuel_consumption",&camera_object_orcs_fuel_consumption);
declare_variable("camera_object_orcs_lin_fuel_consumption",&camera_object_orcs_lin_fuel_consumption);
declare_variable("camera_object_orcs_rot_fuel_consumption",&camera_object_orcs_rot_fuel_consumption);

declare_variable("camera_object_quat_x",&camera_object_quat_x);
declare_variable("camera_object_quat_y",&camera_object_quat_y);
declare_variable("camera_object_quat_z",&camera_object_quat_z);
declare_variable("camera_object_quat_w",&camera_object_quat_w);


declare_variable("camera_pan_force_debug",&camera_pan_force_debug);

// depth of field accessors - allow init to set depth_of_field
declare_function("init_depth_of_field",init_depth_of_field,0,0);		// void (void)
declare_function("set_angle_of_view",set_angle_of_view,1,0);
declare_function("get_angle_of_view",get_angle_of_view,0,1);


//ap_pane

declare_function("draw_real_time",draw_real_time,3,0);
declare_function("get_real_time_struct",get_real_time_struct,1,0);
// parameter as 0=last, 1=int, 2=double, 3=single, 4=double in both float and 2 gpr registers, 5=single in both float and gpr register
//     ... use the "in both" for direct structures that are floating point, and variable parameter calls  
// Return 0=none, 1=int, 2=double or single

declare_advanced_function("make_3_digit",make_3_digit,0x41000000,1);

// =====================================================

declare_function("get_galactic_id",get_galactic_id,1,1);
declare_function("POBs_get_number_of_ships_docked",POBs_get_number_of_ships_docked,2,1);
declare_function("POBs_get_allegiance",POBs_get_allegiance,1,1); //returns HUMAN, ALIEN, NEUTRAL

declare_function("POBs_get_supply_rating",POBs_get_supply_rating,1,1); //0-100
declare_function("POBs_get_supply_type",POBs_get_supply_type,1,1); //1-5=agri, 6-10=tech
declare_advanced_function("POBs_what_is_population",POBs_what_is_population,0x10000000,2);
declare_function("POBs_is_inhabited",POBs_is_inhabited,1,1);

declare_function("POBs_is_this_earthish_size",POBs_is_this_earthish_size,1,1);
declare_function("POBs_is_this_a_moon",POBs_is_this_a_moon,1,1);
declare_function("POBs_is_this_a_planet",POBs_is_this_a_planet,1,1);
declare_function("POBs_is_this_a_planet_or_moon",POBs_is_this_a_planet_or_moon,1,1);
declare_function("POBs_is_this_a_mothership",POBs_is_this_a_mothership,1,1);
declare_function("POBs_is_this_a_rock_moon",POBs_is_this_a_rock_moon,1,1);
declare_function("POBs_is_this_a_gas_giant",POBs_is_this_a_gas_giant,1,1);
declare_function("POBs_is_this_a_sun",POBs_is_this_a_sun,1,1);
declare_function("POBs_is_this_a_giant_planet",POBs_is_this_a_giant_planet,1,1);
declare_function("POBs_is_this_a_hypermassive_gas_giant",POBs_is_this_a_hypermassive_gas_giant,1,1);

// linkage
declare_function("POBs_object_parent",POBs_object_parent,1,1);
declare_function("POBs_object_first_child",POBs_object_first_child,1,1);
declare_function("POBs_object_next",POBs_object_next,1,1);
declare_function("POBs_object_previous",POBs_object_previous,1,1);

// random stuff
declare_advanced_function("get_POBs_diameter",get_POBs_diameter,0x10000000,2);
declare_advanced_function("get_POBs_rotation",get_POBs_rotation,0x10000000,2);
// =====================================================
// =====================================================
declare_variable("hide_screen_preferences",&hide_screen_preferences);
declare_variable("force_screen_select_dialog",&force_screen_select_dialog);
declare_variable("number_of_previous_screens",&number_of_previous_screens);
declare_variable("selected_screen",&selected_screen);
// =====================================================
// =====================================================
declare_variable("remove_opengl_context_on_switch",&remove_opengl_context_on_switch);
declare_variable("bg_galaxies",&bg_galaxies);

// =====================================================
declare_variable("time_to_target",&debug_time_to_target);
declare_variable("time_to_stop",&debug_time_to_stop);
declare_variable("dist_to_targ",&debug_distance_to_target);

// now lets wake all the zex tasks
interpret("set-up-zex-tasks");
interpret("VARIABLE file-buffer 10000 ALLOT");

interpret("1 coffee-get-file-spec DROP");
interpret(": check-root S\" root.zsl\" PAD convert-to-stored-string PAD coffee-get-file-size DUP 10000 > ;");
interpret(": load-root file-buffer S\" root.zsl\" PAD convert-to-stored-string PAD coffee-read-file ?DUP IF .\" error reading root.zsl file \" . ELSE file-buffer SWAP INTERPRET THEN ;");
interpret(": interpret-root check-root IF .\" root.zsl too big, size=\" . ELSE load-root THEN ;");
interpret(" interpret-root ");

testdata = get_word_address("end_of_root_marker"); 
if(testdata==0) report_error("Coffee init error - reading root.zsl","\p",130);
}


// We used this to test the alignment of double's ... and it is quad-aligned at the moment.
//struct { char c;
//         double d;
//        } test  = { 255, 123.456 };

int end_game_addr;
int start_new_game_addr;
int object_deleted;
int object_created;


void complete_coffee_setup(void)
{
int last_word_addr;

interpret("include main.zsl");

end_game_addr = get_word_address("end-game");
if(end_game_addr==0) report_error("Coffee init error - reading main.zsl","\p",125);

start_new_game_addr = get_word_address("start-new-game");
if(start_new_game_addr==0) report_error("Coffee init error","\p",126);

object_deleted = get_word_address("object-deleted-c");
if(object_deleted==0) report_error("Coffee init error","\p",127);

object_created = get_word_address("object-created-c");
if(object_created==0) report_error("Coffee init error","\p",128);

last_word_addr = get_word_address("last-zex-definition");
if(last_word_addr==0) report_error("Coffee init error - Zex not complete","\p",129);

//declare_variable("test",(int)&test);		// test double alignment

}


// ==============================================

void fcircle_test(void);


void define_display_primitives(void)
{
extern Ptr screenaddr;
extern int screenwidth;

declare_variable("screenaddr",&screenaddr);
declare_variable("screenwidth",&screenwidth);

declare_function("fcircle-test",fcircle_test,0,0);
declare_function("create-outer-flare",create_outer_flare,4,0);
declare_function("fcircle",fcircle,5,0);
declare_function("polygon",polygon,5,0);

}


// -----------------------------------------------------
// these are functions to simplify the look of the setup
// -----------------------------------------------------


void run_coffee(void)
{
coffee1(run_zex_tasks);
}


int interpret(char *string)
{
return coffee2(c_string_interp_addr,(long)string);
}


//void declare_function(char *name,int *function(),int parameters,int return_param)
//{
//coffee5(declare_c_function,name,function,parameters,return_param); // three params, no return
//}

int get_word_address(char *name)
{
return coffee2(get_address_of_word,(long)name);
}


void declare_variable(char *name, void *var_name)
{
coffee3(declare_c_variable,(long)name,(int)var_name);
}

void declare_constant(char *name, int const_name)
{
coffee3(declare_c_variable,(long)name,const_name);
}



void coffee_start_new_game(void)
{
coffee1(start_new_game_addr);
}


void coffee_end_game(void)
{
coffee1(end_game_addr);
}


void coffee_object_created(int id)
{
coffee2(object_created,id);
}

void coffee_object_deleted(int id)
{
coffee2(object_deleted,id);
}




void switch_app(unsigned int process_id)
{
ProcessSerialNumber my_PSN;
ProcessInfoRec my_PIR;
OSErr err;
int quit;


  GraphicsPaused();
  GraphicsInactive();

quit=0;
my_PSN.highLongOfPSN=0;
my_PSN.lowLongOfPSN=0;


while(quit==0)
   {
   	err=GetNextProcess(&my_PSN);
   	if(err) { quit=1; break; }	 //not found
   	
   	my_PIR.processName=NULL;
   	my_PIR.processAppSpec=NULL;
   	
   	GetProcessInformation(&my_PSN,&my_PIR);
   	
   	if(my_PIR.processSignature==process_id)
   	{ 
   	err=SetFrontProcess(&my_PSN); 
        ShowCursor();
	process_pending_updates();	//sit in an event loop until we are brought forward
   	break; 
   	}
    }

//	while(!Button()) { }

GraphicsActive();
}


//called from coffee when we come back into the game
void process_pending_updates(void)
{
   	EventRecord	theEvent;
        int activate=0;
		
//	#if ZEX_CORE_CARBON==1	
//    MyDebugStr(__LINE__,__FILE__,"More carbon work needed here"); 
//    #else
	    
   	while(!activate)
   	{
   	int err;
   	Boolean was_processed;

   	
   	WaitNextEvent(everyEvent, &theEvent, 3, 0);

        
         if(theEvent.what==activateEvt)	//activate our window
         { 

           if(theEvent.message==(UInt32) aglWindow)	//resume
           {
              if(theEvent.modifiers&1)
                 {
                 activate=1; break;
   	}
           }
   	
   	}
   	err=DSpProcessEvent(&theEvent,&was_processed);	//see if DSp can handle the event
   	
   	if(!was_processed)	//no, see if we can update our window
   	{
   	if(theEvent.what==updateEvt)
   	{
       	if(theEvent.message==(UInt32) aglWindow)
            {
   		      BeginUpdate((WindowRef)aglWindow);
   		      EndUpdate((WindowRef)aglWindow);
   	}
   	}
        }	//end of !processed
     }	//end of in_background
//   #endif //ZEX_CORE_CARBON
   
}

