// *************************************MODULE HEADER*********************************
// * FILENAME           : ship_control.c
// * MODULE TITLE       : ship_control.c
// * PROJECT            : Zex
// * DATE STARTED       : 22:29:16 on 22nd June 1999
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
// $Log: ship_control.c,v $
// Revision 1.1.1.1  2003/09/05 22:36:14  stu_c
// First Imported.
//
// Revision 1.35  2002/09/08 02:19:10  stu_c
// Precompiled header adjustments
//
// Revision 1.34  2002/08/31 19:23:19  stu_c
// Callsigns
//
// Revision 1.33  2002/08/11 22:05:50  stu_c
// behaviour_aggressive started, falcons from motherships.
//
// Revision 1.32  2002/07/28 17:12:31  stu_c
// Exhausts and further particle work
//
// Revision 1.31  2002/07/09 21:43:48  stu_c
// NPC lasers; might become exhausts
//
// Revision 1.30  2002/07/08 21:09:19  stu_c
// Laser and gun triggers now part of the OCB, so in theory any object could now shoot lasers and cannons.
//
// Revision 1.29  2002/07/07 03:44:35  stu_c
// Lasers now work but need moving to POCB's
//
// Revision 1.28  2002/05/19 03:17:29  stu
// Guns fix
//
// Revision 1.27  2002/05/05 22:26:52  stu
// Further developement - hull temperature now correct for multiple suns and atmosphere.

All calls to add_to_kill_list check the return
//
// Revision 1.26  2002/04/13 15:30:42  stu
// Rotation, Alycians, behaviour
//
// Revision 1.25  2002/03/09 15:12:16  stu
// Faster cannon reload
//
// Revision 1.24  2002/02/21 22:47:09  stu
// AP now disengaged if we request dock
//
// Revision 1.23  2002/02/17 06:20:10  stu
// Added radar targetting
//
// Revision 1.22  2002/02/16 19:14:57  stu
// DLP_BLUE changed to DLP_PURP
//
// Revision 1.21  2002/02/16 19:02:18  stu
// Radar range decrease key and rationalisation of dlp colours
//
// Revision 1.20  2002/02/10 21:44:35  stu
// removed an unused local
//
// Revision 1.19  2002/02/04 23:05:01  stu
// time multiplier forced to 1 if buffet active
//
// Revision 1.18  2002/02/03 19:30:46  stu
// Reduced buffet intensity
//
// Revision 1.17  2002/02/03 10:52:59  rob
// reinit solar system on multiple solar system entries to stop in progress selection.
//
// Revision 1.16  2002/02/02 23:01:54  stu
// Removed init_solar_map
//
// Revision 1.15  2002/01/27 18:12:42  stu
// Time accel set to 1 in various places. no_buffet cheat added.
1 no_buffet ! in console.
//
// Revision 1.14  2002/01/20 16:44:03  stu
// Buffet now works again.
Sets bg colour as well
//
// Revision 1.13  2002/01/02 19:30:53  rob
// Added key setup code
//
// Revision 1.12  2001/12/29 01:12:43  stu
// Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
//
// Revision 1.11  2001/12/14 19:42:28  stu
// Docking
//
// Revision 1.10  2001/11/26 23:28:36  rob
// Galaxy map.
//
// Revision 1.9  2001/11/24 19:29:16  stu
// *** empty log message ***
//
// Revision 1.8  2001/11/07 20:49:53  stu
// More Carbon work. SOund now plays OK
//
// Revision 1.7  2001/11/06 00:26:45  rob
// First stage of getting keyboard working.
//
// Revision 1.6  2001/11/05 19:57:51  stu
// More OSX work
//
// Revision 1.5  2001/11/02 19:32:57  rob
// Build variants for interface lib and carbon.
Coffee fixes after changes for X.
//
// Revision 1.4  2001/10/26 22:44:34  rob
// Anvil changes
//
// Revision 1.3  2001/10/24 21:39:44  stu
// First set of source to build on pb for carbon.
//
// Revision 1.2  2001/10/22 21:29:02  rob
// Carbon warning changes
//
// Revision 1.1  2001/10/21 01:11:13  stu
// Initial porting work
//
// Revision 1.0.0.1  2001/10/17 20:46:07  rob
// First Imported.

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
    #include <fp.h>
  #endif
  
  #include	"zex_misc.h"
  #include "3d_structures_def.h"
  #include "game_defs.h"
  #include "Error_handler.h"
  #include "processor.h"
  #include "real_time.h"
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
#include "obj_control_misc_routines.h"
#include "light_control.h"
#include "txtsplat.h"
#include "cargo.h"
#include "command_and_control.h"
#include "ogl.h"
#include "matrix_primitives.h"
#include "Physics.h"
#include "HUD.h"
#include "sds.h"
#include "game_constants.h"
#include "main_sim.h"
#include "Galaxy.h"
#include "autopilot.h"
#include "draw_radar_and_cwp.h"
#include "key_setting.h"
#include "keyboard.h"
#include "npc.h"
#include "hud2.h"

#include "access_ocbs.h"
// ***********************************************************************************
// * CONSTANTS 
// *
#define MAIN_THRUST 60000	//newtons per seconds
#define REVERSE_THRUST 40000

#define ANGULAR_THRUST 1	//newtons per second
// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *




_3D ship_normal;	//Start pointing right
_3D ship_rotated_normal; //from engine
int global_request_dock;

int have_ejected;	//1 if so, keeps game alive!

int launch_timer;
int ship_y_ang,ship_x_ang,ship_z_ang;
int last_bullet_fired_frame=0,last_laser_fired_frame=0;
int thrust,zero_crossed;	//-2>>2 if go from + to - or - to +. Generates ship flipped run
int unlim_thrust;
int hull,zex_alive,retro,shields_recharge_counter,shields_recharge_reset;
int death_frame_counter,user_abort;
int global_show_status=0;
int gHideTheScreen=0;
int hull_strength;
//int shields_quality;	  //starts at 5 and goes down to 1 as better sheilds added
float turn;
int gvel_limiter=1;
int last_range_toggle_frame=0;	//for debounce
int last_target_toggle_frame=0;	//for debounce
int last_dock_toggle_frame=0;	//for debounce
int last_missile_toggle_frame=0;	//for debounce
int last_camera_toggle_frame=0;
int last_pause_frame=0;

int last_inv_toggle_frame=0;	//for debounce
int shields_warning_timer=0;
int tractor;	//if one, we are under tractor beam control and cant fly ourselfs
int ship_laser_category,ship_cannon_category;
int attack_message_timer;	//for motherrship under attack message
int hide_zex;
//170799
int level_has_control;	//important - if 1, the player has no control over the ship!!!

int zex_laser_experience=0;
int thrust_lockout;	//if 1, we dont allow thrust or directions - used in tutorial
int rank,kills,promotion_points;
int player_input_counter;	//reset if we turn left/right
int ship_buffet;
ship_power_status_type ship_power_status;

extern int can_consume_fuel;

static int time_since_last_cannon_shot=0;

//fuel consumption vars. The maximum possible consumption
//is 20mg of fuel per second. Total fuel consumption shows this
float main_fuel_consumption,reverse_fuel_consumption;

module_private int old_laser_button_state=0;


extern camera_type camera;
extern	DynObjectsFixedSize *ocb_ptr;
// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *


module_private void run_ship_misc_control(void);

module_private void dz1(void);
module_private void run_buffet(int object);
module_private void kill_player_laser(void);
void do_player_cannon();

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееy



float get_ship_mass_in_kgs(void)
{
 return (*ocb_ptr).object_list[0].mass*1000;
}



void reset_ship()
{
extern kit_desc ship_items[ITEMS_COUNT];
extern int missile_flip;
extern int have_ejected,done_load_flag;
extern int jettison_timer;
extern float gradar_range;


old_laser_button_state=0;
time_since_last_cannon_shot=0;

ship_power_status.battery_charge_state=100;	//reset battery charge
ship_power_status.DC_voltage=DC_NOM;
ship_power_status.DC_on=TRUE;
ship_power_status.AC_voltage=0;
ship_power_status.AC_on=FALSE;
ship_power_status.fuel_status=GOOD;
ship_power_status.engine_running=TRUE;
init_HUD(0);

gradar_range=RADAR_RANGE_INIT;
gvel_limiter=1;
have_ejected=0;	//have not ejected
jettison_timer=0;
reinit_screen();
hide_zex=0;	//zex invisible counter
ship_normal.x=1;
ship_normal.y=0;
ship_normal.z=0;
thrust=0;
unlim_thrust=0;

//shields=1000;
//hull=25;
zex_alive=1;
death_frame_counter=100;	//frames of circling camera - crucial we do this so ship can get killed
last_bullet_fired_frame=0;
last_laser_fired_frame=0;
tractor=0;
launch_timer=0;
attack_message_timer=0;
missile_flip=0;

if (done_load_flag==0)
{
 ship_items[KRADAR].status = 1;  //Radar Available and operative
 ship_items[KRADAR]._class=EQUIPMENT;
 
 ship_items[KLONG_RANGE_RADAR].status = 1;  //Long Range Radar Available and operative
 ship_items[KLONG_RANGE_RADAR]._class=EQUIPMENT;
 ship_items[KCANNON].status = 1;  //Cannon Available and Operative
 ship_items[KCANNON]._class=WEAPONS;
 ship_items[KCANNON].modifier=1;
 
 ship_items[KLASER].status = 1;  // Laser available
 ship_items[KLASER]._class=WEAPONS;
 ship_items[KLASER].modifier=1;

 ship_items[KMISSILES].status = 1;  // Missiles Available
 ship_items[KMISSILES]._class=WEAPONS;

 ship_items[KREPAIR_SYSTEMS].status = 1;  // Repair systems
 ship_items[KREPAIR_SYSTEMS]._class=REPAIRS;
 ship_items[KREPAIR_SYSTEMS].modifier=1;

ship_items[KSHIELDS].status=1;
ship_items[KSHIELDS].value1=1000;
ship_items[KSHIELDS].modifier=1;
ship_items[KSHIELDS]._class=EQUIPMENT;

ship_items[KNORMAL_FUEL_TANK].value1=180;	//should be 180
ship_items[KNORMAL_FUEL_TANK].status=1;
ship_items[KNORMAL_FUEL_TANK]._class=EQUIPMENT;
can_consume_fuel=1;

ship_items[KMISSILE_PYLON1].status=1;
ship_items[KMISSILE_PYLON1]._class=WEAPONS;
ship_items[KMISSILE_PYLON1].modifier=1;

ship_items[KMISSILE_PYLON2].status=1;
ship_items[KMISSILE_PYLON2]._class=WEAPONS;
ship_items[KMISSILE_PYLON2].modifier=1;

ship_items[KMISSILE_PYLON3].status=1;
ship_items[KMISSILE_PYLON3]._class=WEAPONS;
ship_items[KMISSILE_PYLON3].modifier=1;

ship_items[KFLIGHT_COMPUTER].status=1;
ship_items[KFLIGHT_COMPUTER]._class=EQUIPMENT;
ship_items[KFLIGHT_COMPUTER].modifier=1;

ship_items[KNAV_COMPUTER].status=1;
ship_items[KNAV_COMPUTER]._class=EQUIPMENT;
ship_items[KNAV_COMPUTER].modifier=1;

ship_items[KENGINE].status=1;

//now the bits we dont get at start up
ship_items[KFUEL_MAXIMISER].status=0;
ship_items[KAUTO_HULL_REPAIR].status=0;
ship_items[KAUTO_HULL_REPAIR].modifier=0;

ship_items[KREAR_LASER].status=0;	//should be 0
ship_items[KREAR_LASER].modifier=0;	//same

ship_items[K360].status=0;
ship_items[KSHOCKWAVE].status=0;
ship_items[KECM].status=0;
ship_items[KMISSILE_FACTORY].status=0;
ship_items[KFAST_DOCK].status=0;
ship_items[KFLATPACK].status=1;
ship_items[KFLATPACK].modifier=0;
ship_items[KFLATPACK].value1=1;
ship_items[KRETRO_ROCKETS].status=0;
ship_items[KRETRO_ROCKETS].value1=3;
ship_items[KHYPERSPACE].status=0;
ship_items[KEJECTION].status=1;	//test =1 
ship_items[KTRADING_COMPUTER].status=0;
ship_items[KTRADING_COMPUTER].modifier=0;
ship_items[KAUTO_HULL_REPAIR].status=0;
ship_items[KBOID_ADDITION].status=0;
ship_items[KASTEROID_ADDITION].status=0;
}	//end of done_load_flag

}

void reset_ship_with_carry_over()
{
extern kit_desc ship_items[ITEMS_COUNT];
extern float gradar_range;
extern int jettison_timer;

//int i;
gradar_range=RADAR_RANGE_SHORTEST;
gvel_limiter=1;

jettison_timer=0;
reinit_screen();
hide_zex=0;	//zex invisible counter
ship_normal.x=1;
ship_normal.y=0;
ship_normal.z=0;
thrust=0;
unlim_thrust=0;

//shields=1000;
//hull=25;
zex_alive=1;
death_frame_counter=100;	//frames of circling camera - crucial we do this so ship can get killed
last_bullet_fired_frame=0;
last_laser_fired_frame=0;
tractor=0;
launch_timer=0;
attack_message_timer=0;

 ship_items[KRADAR].status = 1;  //Radar Available and operative
 ship_items[KRADAR]._class=EQUIPMENT;
 
 ship_items[KLONG_RANGE_RADAR].status = 1;  //Long Range Radar Available and operative
 ship_items[KLONG_RANGE_RADAR]._class=EQUIPMENT;
 ship_items[KCANNON].status = 1;  //Cannon Available and Operative
 ship_items[KCANNON]._class=WEAPONS;
 ship_items[KCANNON].modifier=1;
 
 ship_items[KLASER].status = 1;  // Laser available
 ship_items[KLASER]._class=WEAPONS;
 //ship_items[KLASER].modifier=1;

 ship_items[KMISSILES].status = 1;  // Missiles Available
 ship_items[KMISSILES]._class=WEAPONS;

 ship_items[KREPAIR_SYSTEMS].status = 1;  // Repair systems
 ship_items[KREPAIR_SYSTEMS]._class=REPAIRS;
 //ship_items[KREPAIR_SYSTEMS].modifier=1;

ship_items[KSHIELDS].status=1;
ship_items[KSHIELDS].value1=1000;
//ship_items[KSHIELDS].modifier=1;
ship_items[KSHIELDS]._class=EQUIPMENT;

ship_items[KENGINE].status=1;

//ship_items[KNORMAL_FUEL_TANK].value1=180;
ship_items[KNORMAL_FUEL_TANK].status=1;
//ship_items[KNORMAL_FUEL_TANK].class=EQUIPMENT;

//ship_items[KMISSILE_PYLON1].status=1;
//ship_items[KMISSILE_PYLON1].class=WEAPONS;
//ship_items[KMISSILE_PYLON1].modifier=1;

//ship_items[KMISSILE_PYLON2].status=1;
//ship_items[KMISSILE_PYLON2].class=WEAPONS;
//ship_items[KMISSILE_PYLON2].modifier=1;

//ship_items[KMISSILE_PYLON3].status=1;
//ship_items[KMISSILE_PYLON3].class=WEAPONS;
//ship_items[KMISSILE_PYLON3].modifier=1;

ship_items[KFLIGHT_COMPUTER].status=1;
ship_items[KFLIGHT_COMPUTER]._class=EQUIPMENT;
ship_items[KFLIGHT_COMPUTER].modifier=1;

//ship_items[KNAV_COMPUTER].status=1;
//ship_items[KNAV_COMPUTER].class=EQUIPMENT;
//ship_items[KNAV_COMPUTER].modifier=1;

//now the bits we dont get at start up
//ship_items[KFUEL_MAXIMISER].status=0;
ship_items[KREAR_LASER].status=0;
//ship_items[K360].status=0;
ship_items[KSHOCKWAVE].status=0;
//ship_items[KECM].status=0;
//ship_items[KMISSILE_FACTORY].status=0;
//ship_items[KFAST_DOCK].status=0;
ship_items[KFLATPACK].status=0;
ship_items[KFLATPACK].modifier=0;

//ship_items[KRETRO_ROCKETS].status=0;
//ship_items[KRETRO_ROCKETS].value1=3;
//ship_items[KHYPERSPACE].status=0;
//ship_items[KEJECTION].status=0;
//ship_items[KTRADING_COMPUTER].status=0;
//ship_items[KTRADING_COMPUTER].modifier=0;
//reset any cargo being carried

//init hold items


}


int selectGalacticMap=0;
int selectSolarMap=0;

//static int need_to_show_cp;
#if ZEX2==1
static float zex_roll, zex_pitch, zex_yaw;
#endif

void apply_control_inputs()
{

//new routine 14/1298
//SB
//sds_attrib_data_type the_sds_attrib;
//sds_mount_data_type the_mount_info;
//float fvalue;
//_3D reverse_thrust_vector;

//extern _3D ship_normal;	//Start pointing out of screen
//extern _3D ship_rotated_normal; //from engine
//extern float g_radar_range;
extern int game_pause;	//1=pause, 0 = run

//extern	int ship_y_ang,ship_x_ang,ship_z_ang;
//extern int thrust,zero_crossed,retro;
//extern float turn;
//extern float ORCS_thrust_up,ORCS_thrust_down;


extern int music_volume;
extern int sound_volume;

//extern int real_frame_counter;
//extern int quit;
//extern int missile_flip;
extern int global_show_status;
//extern int camera_mode;
extern int added_nav;
extern int pilot_asleep;

//extern UInt32 CAMyawr;
//extern UInt32 CAMyawl;
//extern UInt32 CAMrollr;
//extern UInt32 CAMrolll;

extern kit_desc ship_items[ITEMS_COUNT];
extern float gradar_range;


//float dx,dy;	//thrust components
//SInt32 signed_turn;
//float fturn,fmoment;

//int thrust_roll_input;
//int tacc,mint,maxt;
//vector p,d;


//get_ship_mount_data (0, VTHR1V, &the_mount_info);

//get_ship_attrib_data(0, BATT_CLASSV, &the_sds_attrib);

//fvalue = get_ship_non_status_value (0, MASSV);

insert_exhaust_animation(0);


//check if we added a nav computer
if (added_nav==1)
{
      add_to_text_display("MK I Nav Computer fitted. 400 credits charged.",DLP_PURP);
      added_nav=0;
}



//if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.request_dock==1) global_request_dock=0;	//shove our ID in to request a dock please


//store old x and y
//(*ocb_ptr).object_list[0].previous_x=(*ocb_ptr).object_list[0].Oworldx;
//(*ocb_ptr).object_list[0].previous_y=(*ocb_ptr).object_list[0].Oworldy;
//(*ocb_ptr).object_list[0].previous_z=(*ocb_ptr).object_list[0].Oworldz;

//ееMISC CONTROLS WE ALWAYS PROCESS
if (gPane && ZexTickCount()>last_target_toggle_frame)
{
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
 toggle_mfd_pane();		//in hud
 last_target_toggle_frame=ZexTickCount()+10;
}
if (gFrontView && ZexTickCount()>last_target_toggle_frame)
{
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
     add_to_text_display("OBC> View locked front.",DLP_GREEN);
 camera_set_front_view();
 last_target_toggle_frame=ZexTickCount()+10;
}

if (gLeftView && ZexTickCount()>last_target_toggle_frame)
{
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);

 camera_set_left_view();
 last_target_toggle_frame=ZexTickCount()+10;
}

if (gRightView && ZexTickCount()>last_target_toggle_frame)
{
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
 camera_set_right_view();
 last_target_toggle_frame=ZexTickCount()+10;
}

if (gRearView && ZexTickCount()>last_target_toggle_frame)
{
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
 camera_set_rear_view();
 last_target_toggle_frame=ZexTickCount()+10;
}

//handle music volume
if (gMusic_up)
{ 
  if (music_volume<63) 
   {
   music_volume++;
   music_set_vol(music_volume);
   }
}

if (gMusic_down)
{
 if(music_volume>0) 
 {
  music_volume--;
   music_set_vol(music_volume);
 }
}





//handle sound volume
if (gSound_up && sound_volume<63) sound_volume++;

if (gSound_down && sound_volume>0) sound_volume--;



//handle quit game - esc key
if (gAbort && ZexTickCount()>last_pause_frame)	//ESC key
  {
   last_pause_frame=ZexTickCount()+20;
   game_pause=1;
   if (global_show_status!=0)
   reset_screen_from_status();	//we were showin the status screen
   else
   if (gHideTheScreen!=0)
   reset_screen_from_status();	//we were showin the status screen
   
  }

//command and control
if (gCommandControl && ZexTickCount()>last_pause_frame)	//ESC key
  {
   last_pause_frame=ZexTickCount()+20;
   if (global_show_status!=0)
   reset_screen_from_status();	//we were showin the status screen
   else
   if (gHideTheScreen!=0)
   reset_screen_from_status();	//we were showin the status screen
   do_command_and_control();	//file
  }



//if (level_has_control==1 || pilot_asleep==1) return;	     //level is controlling zex
if (level_has_control==1) return;	//level is controlling zex


if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.controller!=zex) return;	//probs replaced with an ejection pod

//allow the request_dock to be toggled about twice a second max
if (gAskForDock && ZexTickCount()>last_dock_toggle_frame)	//request dock
{
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
(*ocb_ptr).object_list[0].Dyn_OCB_control_data.request_dock^=1;
	disengage_AP(get_main_camera_object());
if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.request_dock==1)
 {     
     add_to_text_display("OBC> Transmitting docking request signal.",DLP_GREEN);
 }
else
{
     add_to_text_display("OBC> Docking request signal cancelled.",DLP_GREEN);
}
 last_dock_toggle_frame=ZexTickCount()+10;

}

//handle eject
if (gEject && have_ejected==0)	//manual eject
{
 if (ship_items[KEJECTION].status<=0)
 {
   add_to_text_display("OBC> What ejection pod?!",DLP_YELLOW); 
 }
 else
 if (launch_timer==0)
 {
  int closest_mship=-1;
  
//     closest_mship=find_nearest_mothership_to_object(0,-1);
      //SB 040699
      if (closest_mship==-1)	//no motherships in area!
      {
      add_to_text_display("OBC> Can't eject! No motherships in vicinity.",DLP_YELLOW);
      return; 
      }

  do_ejection();
 }
}


if (gTarget && ZexTickCount()>last_target_toggle_frame)	//toggle radar range
{
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
 
 do_target(0);	//in HUD
 last_target_toggle_frame=ZexTickCount()+10;
}

if (gRadartarget && ZexTickCount()>last_target_toggle_frame)	//toggle radar range
{
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
 
 do_radar_target();	//in HUD
 last_target_toggle_frame=ZexTickCount()+10;
}

if (gTimeaccelup && ZexTickCount()>last_target_toggle_frame)	//toggle radar range
{
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
 
 flip_time_multiplier_up();
 
 last_target_toggle_frame=ZexTickCount()+10;
}

if (gTimeacceldown && ZexTickCount()>last_target_toggle_frame)	//toggle radar range
{
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
 
 flip_time_multiplier_down();
 
 last_target_toggle_frame=ZexTickCount()+10;
}




if (gAutopilotUp && ZexTickCount()>last_target_toggle_frame)	//toggle radar range
{
 
 check_autopilotup(0);	//hud.c
 
 last_target_toggle_frame=ZexTickCount()+10;
}

if (gAutopilotDown && ZexTickCount()>last_target_toggle_frame)	//toggle radar range
{
 
 check_autopilotdown(0);	//hud.c
 
 last_target_toggle_frame=ZexTickCount()+10;
}


//allow the radar to be toggled about twice a second max
if (gRadarRangeInput && ZexTickCount()>last_range_toggle_frame)	//toggle radar range
{
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);


 if (gradar_range<RADAR_RANGE_LONGEST)
  {
  gradar_range*=RADAR_RANGE_MULTIPLIER;
  }
 else
  {
  gradar_range=RADAR_RANGE_SHORTEST;
  add_to_text_display("OBC> Scanner set to shortest range.",DLP_YELLOW);
  }
 
 
 last_range_toggle_frame=ZexTickCount()+10;
}


if (gRadarDown && ZexTickCount()>last_range_toggle_frame)	//toggle radar range
{
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
 
 if (gradar_range>RADAR_RANGE_SHORTEST) //defined in draw_radar_and_cwp.h
  {
  gradar_range/=RADAR_RANGE_MULTIPLIER;
  }
 else
  {
  gradar_range=RADAR_RANGE_LONGEST;
  add_to_text_display("OBC> Scanner set to maximum range.",DLP_YELLOW);
  }
 
 
 last_range_toggle_frame=ZexTickCount()+10;
}


// ====================================================================
if (gSolarMap && ZexTickCount()>last_range_toggle_frame)	 // select solar map?
{
 extern int gHideTheScreen;
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
 gHideTheScreen^=1;
 if(gHideTheScreen==1)
    {
    selectSolarMap=1;
    selectGalacticMap=0;
    reinit_solar_system_map();
    }

 last_range_toggle_frame=ZexTickCount()+10;
 
 if (gHideTheScreen==0)	//we need to reset the screen
 {
  reset_screen_from_status();
 }

}
// ====================================================================
// no longer used 
if (gGalacticMap && ZexTickCount()>last_range_toggle_frame)	// select galactic map?
{
 extern int gHideTheScreen;
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
 gHideTheScreen^=1;
 if(gHideTheScreen==1)
    {
    selectSolarMap=0;
    selectGalacticMap=1;
    init_galactic_map();
    }

 last_range_toggle_frame=ZexTickCount()+10;
 
 if (gHideTheScreen==0)	//we need to reset the screen
 {
 reset_screen_from_status();
 }

}
// ====================================================================
if (gKeyMapping && ZexTickCount()>last_range_toggle_frame)	// select key mapping mode
{
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
 do_key_setting();
 reset_screen_from_status();
 last_range_toggle_frame=ZexTickCount()+10; 
}

// ====================================================================



if (gDampers && ZexTickCount()>last_range_toggle_frame)	//toggle radar range
{
UInt8 local_damper;

 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
 
 local_damper=(*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_ORCS_damper;
 
 if(local_damper==0)  { local_damper=1;        add_to_text_display("ORCS> Rot dampers on, linear dampers off.",DLP_GREEN); }
 else
 if(local_damper==1)  { local_damper=2;     add_to_text_display("ORCS> Linear dampers on, rot dampers off.",DLP_GREEN); }
 else
 if(local_damper==2)  { local_damper=3;     add_to_text_display("ORCS> Linear and rot dampers on.",DLP_GREEN); }
 else
 if(local_damper==3)  { local_damper=0;      add_to_text_display("ORCS> All dampers off.",DLP_GREEN); }

 (*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_ORCS_damper=local_damper;

 last_range_toggle_frame=ZexTickCount()+10;
}


//handle show inv key /
if (gShowInventory && ZexTickCount()>last_inv_toggle_frame)	//toggle radar range
{
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
 last_inv_toggle_frame=ZexTickCount()+10;;
   
   #if OGL==1
    if (global_show_status==0)
    {
     //ogl_hide_control_panel();
    }
  #endif

 global_show_status++;	//set/reset global variable
 if (global_show_status>2) global_show_status=0;
 
 if (global_show_status==0)	//we need to reset the screen
 {
  reset_screen_from_status();
  //reshow control panel
 }
}




//if ((*ocb_ptr).object_list[0].request_dock==1) return;	//dock request



if ((*ocb_ptr).object_list[0].invincible_count>1)
{
// (*ocb_ptr).object_list[0].invincible_count--;	//set when we die
 (*ocb_ptr).object_list[0].eng_invisible=(*ocb_ptr).object_list[0].eng_invisible^1;	//flash it
}

if ((*ocb_ptr).object_list[0].invincible_count==1) 
{
(*ocb_ptr).object_list[0].eng_invisible=0;	//set when we die
(*ocb_ptr).object_list[0].invincible_count=0;
}

//ееSHIP CONTROLS WE ONLY PROCESS IF SHIP IS ACTIVE
//   if (fire_button()) Debugger();	//press space to enter debugger
//see if we need to make zex visible
if (hide_zex>0)
   {
   hide_zex--;
   if (hide_zex==0)
    {
    (*ocb_ptr).object_list[0].eng_invisible=0;
    do_game_dead();
    }
   }


if (pilot_asleep==1) return;	   //level is controlling zex
   


//correct to get ship facing into screen (along z)

if (tractor==0)
{
//change for SDS 110801

//handle main engine


if (gThrustDown)
  {
  set_ship_thruster (0, MREV1V, 190);
  set_ship_thruster (0, MREV2V, 190);
  }
  

if (gThrustUp)
    {
  set_ship_thruster (0, MTHR1V, 230);
  set_ship_thruster (0, MTHR2V, 230);
    }
    

if (gVertUpThrust)
  {
    set_ship_thruster (0, VTHR1V, 255);
    set_ship_thruster (0, VTHR2V, 255);
  }
  
  
if (gVertDownThrust)
  {
    set_ship_thruster (0, RVTH1V, 255);
    set_ship_thruster (0, RVTH2V, 255);
  }


   if (gYawl)	//left arrow
   {
     set_time_multiplier_to_1();
     set_ship_rotational_thruster (0, ROTYAWV, -127);    
   }
   else //no left input
   if (gYawr)	//right arrow
   {  
     set_time_multiplier_to_1();

     set_ship_rotational_thruster (0, ROTYAWV, 127);    
   }


   if (gRolll)	
   {
     set_time_multiplier_to_1();

     set_ship_rotational_thruster (0, ROTROLLV, -127);    
   }
   else //no left input
   if (gRollr)	
   {
     set_time_multiplier_to_1();
     set_ship_rotational_thruster (0, ROTROLLV, 127);    
   }

   if (gPitchu )	
   {
     set_time_multiplier_to_1();
     set_ship_rotational_thruster (0, ROTPITCHV, -127);    
   }
   else //no left input
   if (gPitchd)	
   {
     set_time_multiplier_to_1();
     set_ship_rotational_thruster (0, ROTPITCHV, 127);    
   }

}	//end of tractor==0


//laser
//if laser was up and is now down then fire laser
if (gLaser && old_laser_button_state==0)
{
 old_laser_button_state=1;
 do_player_laser();
}

if (!gLaser && old_laser_button_state==1)
{
 old_laser_button_state=0;
 kill_player_laser();
}

if (gCannon)
{
 do_player_cannon();
}


//if laser was down and is now up, kill laser
   run_ship_misc_control();

//еееееееееееееееееееееееееееееееееееееееееееееееееееееBINARY INPUTS



//run_ORCS();
object_systems_sim(0);	//handle all ships systems/engines/fuel etc

if (ship_buffet!=0) run_buffet(0);



}







void do_ejection()
{

}






void clear_down_ship_on_eject()
{
extern int thrust;
extern kit_desc ship_items[ITEMS_COUNT];

thrust=0;
unlim_thrust=0;

ship_items[KSHIELDS].value1=0;
//			 hull_damage=50;
//			 damage_me_no_sound(180);
//             hull_strength=0;
ship_items[KRADAR].status=0;
ship_items[KLONG_RANGE_RADAR].status=0;
ship_items[KCANNON].status=0;
ship_items[KMISSILES].status=0;
ship_items[KREPAIR_SYSTEMS].status=0;
ship_items[KLASER].status=0;
ship_items[KNORMAL_FUEL_TANK].value1=0;
ship_items[KNAV_COMPUTER].status=0;
ship_items[KMISSILE_PYLON1].modifier=0;
ship_items[KMISSILE_PYLON2].modifier=0;
ship_items[KMISSILE_PYLON3].modifier=0;
ship_items[KFLATPACK].value1=0;	//lost!
}

void reset_screen_from_status()
{
//  extern kit_desc ship_items[ITEMS_COUNT];
//  int old_hull_strength;
//  extern int hull_strength;
//     old_hull_strength=hull_strength;
     reinit_screen();	//resets hull strength...
//     hull_strength=old_hull_strength;
     global_show_status=0;
     gHideTheScreen=0;
     
     selectSolarMap=0;
     selectGalacticMap=0;
}
//еееMoves pylon toggle to next valid pylon (one with a missile)


void select_next_pylon()
{
extern int missile_flip;
extern kit_desc ship_items[ITEMS_COUNT];
int count=0;	//there are only three pylons - this is the check
 
 while (count<3)
 {
 missile_flip++;
 if (missile_flip>2) missile_flip=0;
  {
    //check the pylon to see if a missile exists
    if (ship_items[KMISSILE_PYLON1+missile_flip].modifier!=0) return;	//got one
  }
 count++;	//check next pylon
 }	//end of while

missile_flip=-1;	//no valid pylons 
}
//ееееееееееееееLASERS

void do_player_laser()
{
int dest_ocb;
//int owner;
load_dyn_object_pb the_params;

clear_pb(&the_params);
dest_ocb=find_vacant_dynamic_object_slot();

    if (dest_ocb!=-1)
    {   
        (*ocb_ptr).object_list[0].Dyn_OCB_control_data.laser_trigger_slot=dest_ocb;

	the_params.controller_ref=PARTICLE_GENERATOR_1;
	the_params.control_use_1=1;	//0=single laser, 1=twin
	the_params.mass=1;	//basic ship weighs 1 tonne
	the_params.object_category=NO_RADAR;
         the_params.kill_flags|=KILL_METHOD;
         the_params.invisible_flag=1;
        the_params.parent=0;	//Zex owns this object
	load_dyn_object(26,dest_ocb,&the_params,-1,1,-1,DUST_BIT);	//the object, the position (-1=next free)
	
		
    }

}	//end of zex fires its laser

void kill_player_laser(void)
{
 if((*ocb_ptr).object_list[0].Dyn_OCB_control_data.laser_trigger_slot!=-1)
  {
   if (add_to_kill_list((*ocb_ptr).object_list[0].Dyn_OCB_control_data.laser_trigger_slot)!=0)
     {
	   MyDebugStr(__LINE__,"ship_control.c","add_to_kill_list failed!");
	 }
	    (*ocb_ptr).object_list[0].Dyn_OCB_control_data.laser_trigger_slot=-1;
  }
}



//static int cannon_slot;


void do_player_cannon()
{
//int owner;
//this is a one shot particle generator that generates a bunch of particles shot from the front of the ship


if(ZexTickCount()<time_since_last_cannon_shot+40) return;

time_since_last_cannon_shot=ZexTickCount();

NPC_fire_cannon(0);

/*
clear_pb(&the_params);
dest_ocb=find_vacant_dynamic_object_slot();

    if (dest_ocb!=-1)
    {   
        (*ocb_ptr).object_list[0].Dyn_OCB_control_data.cannon_trigger_slot=dest_ocb;

	the_params.world_x=0;
	the_params.world_y=0;
	the_params.world_z=0;

	the_params.controller_ref=PARTICLE_GENERATOR_CANNON;

	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	the_params.shield_value=0;
	the_params.mass=1;
	the_params.object_category=NO_RADAR;
         the_params.invisible_flag=1;
	 the_params.unique_id=0;
     the_params.parent=0;	//Zex owns this object
	load_dyn_object(26,dest_ocb,&the_params,-1,1,-1,DUST_BIT);	//the object, the position (-1=next free)	 
    }
    */

}	//end of zex fires its cannon



//fire a missile - very similar to laser
void do_player_missile()
{


}	//end of zex fires a missile

//returns the selected pylon and missile type as a ship_items index
int get_pylon()
{
 extern int missile_flip;
 if (missile_flip<0) return -1;
 return KMISSILE_PYLON1+missile_flip;
}
//еееееееееееееееееееееееееееееееееееееееееее


void do_player_fire()
{

}

//еееееееееееееееееееееееееее
int damage_repair_timer;

unsigned int time_to_do_shields=0;

void run_ship_misc_control(void)
{
//this routine responsible for misc ship systems and damage repair

//extern int shields_recharge_counter;
//extern int thrust;
extern int shields_warning_timer,zex_alive;
extern kit_desc ship_items[ITEMS_COUNT];

extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects

if (launch_timer>0) launch_timer--;	//stops various systems from running if >0
if (attack_message_timer>0) attack_message_timer--;	//stops various systems from running if >0

if (zex_alive==0) return;	//obvious - we dont run systems if we're deaded
//depelete fuel
//do fuel consumption - total_fuel_consumption;

//   if (ship_items[KFUEL_MAXIMISER].status==1)	//100mg per sec
//   {
//
//   if (ship_items[KNORMAL_FUEL_TANK].value1>0 && can_consume_fuel!=0)
//	  {
//	   ship_items[KNORMAL_FUEL_TANK].value1-=total_fuel_consumption*time_in_secs;
//	  }
//   }
//   else	//200mg/sec
//   
//   {
//   if (ship_items[KNORMAL_FUEL_TANK].value1>0 && can_consume_fuel!=0)
//	  {
//	   ship_items[KNORMAL_FUEL_TANK].value1-=(total_fuel_consumption/2)*time_in_secs;
//	  }
//   }


 
 
 if (ship_items[KNORMAL_FUEL_TANK].value1==19) add_to_text_display("OBC> Fuel low.",DLP_RED);
 else
 if (ship_items[KNORMAL_FUEL_TANK].value1==10) add_to_text_display("OBC> WARNING: Fuel very low.",DLP_RED);
 else
 if (ship_items[KNORMAL_FUEL_TANK].value1==5) add_to_text_display("OBC> Fuel dangerously low. will lose control shortly!",DLP_RED);
 else
 if (ship_items[KNORMAL_FUEL_TANK].value1==2) 
 {
 add_to_text_display("OBC> NO FUEL! Laser and missiles going off line",DLP_RED);
 ship_items[KLASER].status=2;
 ship_items[KMISSILES].status=2;
 ship_items[KREPAIR_SYSTEMS].status=2;
 }

 else
 if (ship_items[KNORMAL_FUEL_TANK].value1==1) 
 {
 add_to_text_display("OBC> NO FUEL! Switching to batteries for essentials",DLP_RED);
 add_to_text_display("OBC> Systems going off line!",DLP_RED);
 add_to_text_display("OBC> Daisy, daisy, give me your answer do...",DLP_RED);
 ship_items[KRADAR].status=2;
 ship_items[KLONG_RANGE_RADAR].status=2;
 ship_items[KNORMAL_FUEL_TANK].value1=0; 
 }

 
//charge up shields
 


//repair a damaged system
damage_repair_timer--;
if (damage_repair_timer<0  && ship_items[KNORMAL_FUEL_TANK].value1>1)
   {
   damage_repair_timer=RangedRdm(200,600);
//we repair in order of priority - 
//REPAIR, missiles, laser,FLIGHT, cannon, radar ,long range
   if (ship_items[KREPAIR_SYSTEMS].status>1)	//no point in doing missiles if we have none! 
   {
   ship_items[KREPAIR_SYSTEMS].status=1;
   if (ship_items[KNORMAL_FUEL_TANK].value1>30) ship_items[KNORMAL_FUEL_TANK].value1-=15;   
   add_to_text_display("REPAIR SYSTEMS BACK ON-LINE",0);

   }
//   else
//   if (ship_items[KMISSILES].status>1 && (*ocb_ptr).object_list[zex].Dyn_OCB_control_data.number_of_missiles!=0)	  //no point in doing missiles if we have none! 
//   {
//   ship_items[KMISSILES].status=1;
//   if (ship_items[KNORMAL_FUEL_TANK].value1>30) ship_items[KNORMAL_FUEL_TANK].value1-=15;   
//   add_to_text_display("MISSILE CONTROLLER BACK ON-LINE",0);

//   }
   else
   if (ship_items[KLASER].status>1)
   {
   ship_items[KLASER].status=1;
   if (ship_items[KNORMAL_FUEL_TANK].value1>40) ship_items[KNORMAL_FUEL_TANK].value1-=15;   
   add_to_text_display("LASER BACK ON-LINE",0);
   }
   else
   if (ship_items[KENGINE].status>1)	
   {
   ship_items[KENGINE].status=1;
   if (ship_items[KNORMAL_FUEL_TANK].value1>30) ship_items[KNORMAL_FUEL_TANK].value1-=15;   
   add_to_text_display("ENGINE REPAIRED",0);

   }

   else
   if (ship_items[KFLIGHT_COMPUTER].status>1)	//no point in doing missiles if we have none! 
   {
   ship_items[KFLIGHT_COMPUTER].status=1;
   if (ship_items[KNORMAL_FUEL_TANK].value1>30) ship_items[KNORMAL_FUEL_TANK].value1-=15;   
   add_to_text_display("ORCS BACK ON-LINE",0);

   }

   else
   if (ship_items[KCANNON].status>1) 
   {
   ship_items[KCANNON].status=1;
   if (ship_items[KNORMAL_FUEL_TANK].value1>40) ship_items[KNORMAL_FUEL_TANK].value1-=15;
   add_to_text_display("CANNON BACK ON-LINE",0);
   
   }



   else
   if (ship_items[KRADAR].status>1) 
   {
   ship_items[KRADAR].status=1;
   add_to_text_display("RADAR BACK ON-LINE",0);

   }
   else
   if (ship_items[KLONG_RANGE_RADAR].status>1) 
   {
   ship_items[KLONG_RANGE_RADAR].status=1;
   add_to_text_display("LONG RANGE RADAR BACK ON-LINE",0);
	}
	else	//finally check missiles again
   if (ship_items[KMISSILES].status>1) 
   {
   ship_items[KMISSILES].status=1;
   if (ship_items[KNORMAL_FUEL_TANK].value1>30) ship_items[KNORMAL_FUEL_TANK].value1-=15;   
   add_to_text_display("MISSILES BACK ON-LINE",0);

   }

//   instruments[RangedRdm(0,5)]=1;	//repair a system at random

   }
   
//recharge shields 

if(ZexTickCount()>time_to_do_shields)
{
time_to_do_shields=ZexTickCount()+120;	//every two seconds do shields

ship_items[KSHIELDS].value1=(*ocb_ptr).object_list[0].Dyn_OCB_control_data.shield_value;

if (ship_items[KSHIELDS].value1<0) ship_items[KSHIELDS].value1=0;
if (ship_items[KSHIELDS].modifier<1) ship_items[KSHIELDS].value1=0;	//do we have shields? Saves loads of procedural code

if (ship_items[KNORMAL_FUEL_TANK].value1>1 && ship_items[KSHIELDS].modifier>0) //and we actually have shields!
   {
    if (ship_items[KSHIELDS].value1+5<100)
     ship_items[KSHIELDS].value1+=5;
     else
     ship_items[KSHIELDS].value1 = 100;
     
   }


//playing warning voice
  if (ship_items[KSHIELDS].value1<50)
  {
   if (shields_warning_timer<=0 && zex_alive==1)
   {
      play_zsound(danger, sound_low_pri, speech_chan, sound_vol_7);
      shields_warning_timer=260;
   }
   shields_warning_timer--;
  } 

  else
  if (ship_items[KSHIELDS].value1<25)
  {
   if (shields_warning_timer<=0 && zex_alive==1)
   {
      play_zsound(warning, sound_low_pri, speech_chan, sound_vol_7);
      shields_warning_timer=270;
   }
   shields_warning_timer--;
  } 

(*ocb_ptr).object_list[0].Dyn_OCB_control_data.shield_value=ship_items[KSHIELDS].value1;
}	//end of shields


}


void ship_init()
{
extern int ship_y_ang,ship_x_ang,ship_z_ang,thrust,zex_alive,retro;
extern int shields_recharge_counter,shields_warning_timer;
extern float turn;
extern kit_desc ship_items[ITEMS_COUNT];

//extern int instruments[6];
extern int damage_repair_timer,tractor,done_load_flag;

#if ZEX2==1
//extern float zex_roll, zex_pitch, zex_yaw;

#endif
int i;


#if ZEX2==1
zex_roll= zex_pitch= zex_yaw=0;

#endif

//now, we take the main engine as to be half of the maximum fuel consumption
//so we need to calculate what reverse and rotational thrusters are

reverse_fuel_consumption=get_reverse_thruster_power_in_newtons(0)*STANDARD_FUEL_USAGE_PER_NEWTONSECOND_IN_KGS;

//rotational_fuel_consumption=get_ship_non_status_value (0, ROTPITCHV)*STANDARD_FUEL_USAGE_PER_NEWTONSECOND_IN_KGS;
main_fuel_consumption=get_main_thruster_power_in_newtons(0)*STANDARD_FUEL_USAGE_PER_NEWTONSECOND_IN_KGS;


damage_repair_timer=300;
ship_y_ang=0; ship_x_ang=0; ship_z_ang=0; unlim_thrust=thrust=0;

zero_crossed=0;
zex_alive=1;	//0 if not
tractor=0;
//shields=1000;
if (done_load_flag==0)
{
ship_items[KSHIELDS].status=1;
ship_items[KSHIELDS].value1=1000;
ship_items[KSHIELDS].modifier=1;
ship_items[KSHIELDS]._class=EQUIPMENT;
ship_items[KNORMAL_FUEL_TANK].value1=180;
     for (i=0;i<6;i++)
     ship_items[i].status=1;	//clear instrument lights
}
retro=0;	//special controller -- retro rockets
turn=0;
shields_recharge_reset=40;
shields_recharge_counter=shields_recharge_reset;	//recharge 1 unit every 10 frames initially
//shields_quality=1;	//initial shields - 4 is best
shields_warning_timer=0;
//additional items would be reset here


}


//is a key of char code pressed?
short check_key(unsigned short k)
//k=any keyboard code 0-127 (Code from Think Ref)
{
unsigned char km[16];
GetKeys ((keymap_ptr)km);
return ((km[k>>3]>>(k&7))&1);
}




short fire_button()
//k=any keyboard code 0-127 (Code from Think Ref)
{
unsigned char km[16];
GetKeys ((keymap_ptr)km);
return ((km[0x31>>3]>>(0x31&7))&1);
}




void kill_zex()
{
//extern int instruments[6];
//extern int zex_alive;
//extern int tractor,lives,showing_stats,credits;
//extern int camera_spin_on_zex_death;
//extern int flash_screen;
//int i;



if (global_show_status!=0)
 reset_screen_from_status();	//we were showin the status screen
if (gHideTheScreen!=0)
 reset_screen_from_status();	//we were showin the status screen

 
 
if ((*ocb_ptr).object_list[zex].in_use==0) return;	//Zex IS dead!

//we have a flatpack
//     if (ship_items[KFLATPACK].value1>0 )
//     {
//     ship_items[KFLATPACK].value1--;
//      do_large_explode(zex);     
//     (*ocb_ptr).object_list[zex].eng_invisible=1;	//lose zex from screen
////    (*ocb_ptr).object_list[0].extern_movedx=0;
////    (*ocb_ptr).object_list[0].extern_movedy=0;
////    (*ocb_ptr).object_list[0].thrust_x=0;
////    (*ocb_ptr).object_list[0].thrust_y=0;
//     
////     add_to_kill_list (zex);
//     play_zsound(explosion4, sound_high_pri, stereo1_chan, sound_vol_7);
//     (*ocb_ptr).object_list[zex].hit_by_slot=-1;
//	   add_to_text_display("SHIP DESTROYED! USING NEW FLATPACK",1);	//clear down any messages
//     reset_ship_with_carry_over();
//     hide_zex=50;	//dec'd in apply_control_inputs = makes zex invisible
//     camera_spin_on_zex_death=45;
//     showing_stats=1;	//stop everything firing
//     unlim_thrust=thrust=0;
//directional_control=1;
//     }
//     else
//     
////no flatpacks left, see if we can eject
//     if (ship_items[KEJECTION].status==1 && have_ejected==0)
//      {
//       int closest_mship=-1;
//  
////       closest_mship=find_nearest_mothership_to_object(0,-1);
//      //SB 040699
//       if (closest_mship==-1)	//no motherships in area!
//       {
//       add_to_text_display("CAN'T AUTO EJECT! NO MOTHERSHIPS IN VICINITY.",1);
//         dz1();	//kill zex
//       return; 
//       }
//
//
//       add_to_text_display("EJECTION POD AUTO ACTIVATED",1);
//       do_ejection();
//       } 
//     
//     
//       else	  //cant eject
     
     {
//       if (have_ejected==0)
//        {
         dz1();	//kill zex
       }
     
}

//private routine
void dz1(void)
{
int i;
extern kit_desc ship_items[ITEMS_COUNT];
extern int flash_screen;

         for (i=0;i<6;i++)
         ship_items[i].status=0;	//clear instrument lights
         if ((*ocb_ptr).object_list[zex].Oid=='EC01' || (*ocb_ptr).object_list[zex].Oid=='EC02')
	     add_to_text_display("EJECTION POD DESTROYED! GAME OVER.",1);
//              fade_and_play_zex_resource(132);	//Spring by Purple Motion

         zex_alive=0;
         tractor=0;
         add_to_explode_list(zex,4);
         if (add_to_kill_list (zex)!=0)
		   {
			 MyDebugStr(__LINE__, __FILE__,"add_to_kill_list failed!");
		   }
		      play_zsound(explosion4, sound_high_pri, c1_chan1, sound_vol_7);
//         play_zsound(short_explosion, sound_high_pri, twin1_chan, sound_vol_7);
         (*ocb_ptr).object_list[zex].hit_by_slot=-1;
         flash_screen=2;

	     add_to_text_display(" ",0);	//clear down any messages
	     add_to_text_display(" ",0);	//clear down any messages
}



//End the game RIGHT NOW - typically clled from a level controllere
//EOGCode is a termination code, where zero means OK and -1 means show the reason in the string 
//pointed to by param2
void end_game_right_here_right_now(int EOLCode, char * the_reason_string)
{
extern int end_of_level_code;
extern char end_of_level_reason_string[];	//we can print this if eolc is not zero
extern kit_desc ship_items[ITEMS_COUNT];

//Set up termination reasons to show player why it's all over
 end_of_level_code=EOLCode;
 strcpy (&end_of_level_reason_string[0],the_reason_string);

//And STOP
 ship_items[KFLATPACK].value1=0;
 ship_items[KEJECTION].status=0;
 kill_zex();
}





int no_buffet=0;	//if 1, we don't die from buffet and we don't shake either

void run_buffet(int object)
{
int random_x,random_z;
vector p,d;

//buffet has an input into laser bay temperature reaching max (20)
//at 20000


if (no_buffet==1) return; //Cheat

set_time_multiplier_to_1();

if (ship_buffet>29000)
{
	kill_zex();
}

//apply random force
random_x=RangedRdm (0,ship_buffet)-(ship_buffet/2);
//random_y=RangedRdm (0,ship_buffet)-(ship_buffet/2);
random_z=RangedRdm (0,ship_buffet)-(ship_buffet/2);


//   p.x=0; p.y=0; p.z=-1;	  //position of force
//   d.x=0; d.y=0; d.z=1;	//direction of force
//   apply_a_force(object,random_z /*newton*/ , p   /*point*/ , d /*direction*/);

//   p.x=0; p.y=-1; p.z=0;	  //position of force
//   d.x=0; d.y=1; d.z=0;	//direction of force
//   apply_a_force(object,random_y /*newton*/ , p   /*point*/ , d /*direction*/);

//   p.x=-1; p.y=0; p.z=0;	  //position of force
//   d.x=1; d.y=0; d.z=0;	//direction of force
//   apply_a_force(object,random_y /*newton*/ , p   /*point*/ , d /*direction*/);

random_x/=110; //assume a max of 30000
//random_y/=200;
random_z/=40;

//    if (random_y!=0)
//        {
//
//        	  //rotation around y
//	  	     p.x=1; p.y=0; p.z=0;	//position of force
//	  	     d.x=0; d.y=0; d.z=-1;	  //direction of force
//	  	     apply_a_force(object, (float)random_y,p /*oint*/ , d /*irection*/);   
//
//	  	     p.x=-1; p.y=0; p.z=0;	  //position of force
//	  	     d.x=0; d.y=0; d.z=1;	//direction of force
//	  	     apply_a_force(object, (float)random_y,p /*oint*/ , d /*irection*/);   
//
//        }

    if (random_x!=0)
        {

        	//rotation around x
		   p.x=0; p.y=1; p.z=0;	//position of force
		   d.x=0; d.y=0; d.z=1;	//direction of force
		   apply_a_force(object, (float)random_x,p /*oint*/ , d /*irection*/);   

		   p.x=0; p.y=-1; p.z=0;	//position of force
		   d.x=0; d.y=0; d.z=-1;	//direction of force
		   apply_a_force(object, (float)random_x,p /*oint*/ , d /*irection*/);   

        }


    if (random_z!=0)
        {

        	//rotation around x
		   p.x=1; p.y=0; p.z=0;	//position of force
		   d.x=0; d.y=1; d.z=0;	//direction of force
		   apply_a_force(object, (float)random_z,p /*oint*/ , d /*irection*/);   

		   p.x=-1; p.y=0; p.z=0;	//position of force
		   d.x=0; d.y=-1; d.z=0;	//direction of force
		   apply_a_force(object, (float)random_z,p /*oint*/ , d /*irection*/);   

        }
}
