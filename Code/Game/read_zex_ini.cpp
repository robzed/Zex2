// *************************************MODULE HEADER*********************************
// * FILENAME           : read_zex_ini.cpp
// * MODULE TITLE       : Untitled 1
// * PROJECT            : Zex
// * DATE STARTED       : 00:22:42 on 15th September 1999
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
 * $Log: read_zex_ini.cpp,v $
 * Revision 1.3  2003/09/28 17:29:40  robp
 * Changed files from .c to .cpp and removed spaces out of a couple of filenames.
 *
 * Revision 1.2  2003/09/20 12:57:00  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:35:46  stu_c
 * First Imported.
 *
 * Revision 1.8  2002/09/08 02:03:25  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.7  2002/07/11 21:04:10  rob_c
 * Display configuration changes.
 *
 * Revision 1.6  2002/02/03 19:28:19  stu
 * low_res_planets added
 *
 * Revision 1.5  2002/01/05 20:50:20  rob
 * Default for dock is off.
 *
 * Revision 1.4  2001/12/29 01:10:30  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.3  2001/11/07 22:30:22  stu
 * Hide dock mods.
 *
 * Revision 1.2  2001/11/02 19:31:18  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.1  2001/10/21 01:02:51  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:05  rob
 * First Imported.
 *
 * Revision 1.7  2001/06/21 20:30:02  rob
 * declare variable vs. declare constant changes
 *
 * Revision 1.6  2001/05/28 21:40:12  stu
 * Major commit 280501
 *
 * Revision 1.5  2001/04/25 20:40:49  rob
 * force software renderer changes
 *
 * Revision 1.4  2001/04/08 20:22:53  rob
 * changed init around
 *
 * Revision 1.3  2001/04/01 18:47:44  stu
 * *** empty log message ***
 *
 * Revision 1.2  2001/01/02 18:55:57  stu
 * minor updates
 *
 * Revision 1.1.1.1  2001/01/01 21:13:33  rob
 * First Imported.
 *
 * Revision 1.0.0.1  2000/08/21 21:58:45  stu
 * First Imported.
 *
 */


// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

#include	"Error_handler.h"
#include "3d_structures_def.h"
#endif //pch


#include "file_io.h"
#include "read_zex_ini.h"
#include "load_game.h"
#include "splat_control.h"
#include "physics.h"
#include "coffee.h"
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



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *




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
extern int monitor_w,monitor_h,ogl_depth;	//in splat_control.c
extern int boot_log;
extern int window_w, window_h,screen_res,window_res;	//splat_control
extern int use_zbuffer,zbuffer_depth;
extern float zbuffer_range;
extern int force_software_renderer;
extern int show_dock,low_res_planets;

void read_zex_ini()
{

//first set up default values
monitor_w=640; monitor_h=480;
window_w= 640; window_h=480;
ogl_depth=16;
boot_log=0;		// debugging things off by default
show_dock=0;		// don't show dock by default
low_res_planets=0; //use normal res planets

use_zbuffer=-1;
zbuffer_range=10000*ZEX_UNIT_SIZE_IN_METERS;	//default 10 km zbuffer range
zbuffer_depth=16; //bits     

force_software_renderer=0;

declare_variable("monitor-w",&monitor_w);
declare_variable("monitor-h",&monitor_h);

declare_variable("window-w",&window_w);
declare_variable("window-h",&window_h);

declare_variable("ogl-depth",&ogl_depth);
declare_variable("zbuffer-depth",&zbuffer_depth);
declare_variable("zbuffer-range",&zbuffer_range);	// used in forth as a single
declare_variable("force-software-renderer",&force_software_renderer);
declare_variable("show_dock",&show_dock);
declare_variable("low_res_planets",&low_res_planets);

interpret("include read_init.zsl");
interpret("w-directory");		// lets ensure it really has been put back to source directory

display_configuration_dialog();	// allow user to override current preferences

//set up screen_cent_x and y
screen_cent_x=window_w>>1;
screen_cent_y=window_h>>1;

//for 640*480 screens
off_640_x=(window_w-640)/2;
off_480_y=(window_h-480)/2;
if(off_640_x<0) off_640_x=0;
if(off_480_y<0) off_480_y=0;

win_off_x=(monitor_w-window_w)/2;
win_off_y=(monitor_h-window_h)/2;

//error check
if (window_w>monitor_w || window_h>monitor_h)
  {
  report_error("read_zex_ini: window size (WRES) greater than monitor size (SRES)","\p",-1);
  }

}
