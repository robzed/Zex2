/*
$Header: /home/ls_cvs/ZEX2.2/Code/static\040stuff/buy.c,v 1.20 2002/09/08 02:18:18 stu_c Exp $
$Log: buy.c,v $
Revision 1.20  2002/09/08 02:18:18  stu_c
Precompiled header adjustments

Revision 1.19  2002/08/31 19:24:24  stu_c
Callsigns

Revision 1.18  2002/06/16 23:19:12  stu
Particles booked in for backup purposes - not working yet or complete.

Revision 1.17  2002/05/31 23:56:29  stu
Removed all warnings from the code

Revision 1.16  2002/03/16 00:44:06  stu
Continued development

Revision 1.15  2002/03/10 03:41:08  stu
Additional work on trading - we can buy and sell on different motehrships.

Revision 1.14  2002/02/24 03:41:12  stu
(Some) compiler warnings cleaned up

Revision 1.13  2002/01/27 18:04:14  stu
Unused variables removed

Revision 1.12  2002/01/20 21:38:02  stu
Changes to get it building on ANvil

Revision 1.11  2002/01/09 23:48:20  stu
Furhter work on trading.

Revision 1.10  2002/01/02 19:25:35  rob
Added key setup code

Revision 1.9  2002/01/02 13:29:48  stu
Continuing development for trading/static screens

Revision 1.8  2001/12/29 01:12:23  stu
Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.

Revision 1.7  2001/12/14 19:41:23  stu
Docking

Revision 1.6  2001/11/09 18:29:04  rob
Various debug changes.

Revision 1.5  2001/11/02 19:33:00  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.4  2001/10/24 21:40:41  stu
First set of source to build on pb for carbon.

Revision 1.3  2001/10/22 21:27:56  rob
Carbon warning changes

Revision 1.2  2001/10/21 16:30:42  stu
Fixed up quickdraw OS X errors. As we're OGL it was a simple removal job.

Revision 1.1  2001/10/21 01:08:01  stu
Initial porting work

Revision 1.0.0.1  2001/10/17 20:46:08  rob
First Imported.
First Imported.
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <memory.h>
    #include <timer.h>
    #include <textutils.h>
    #include <files.h>
  #endif
  #include "Error_handler.h"
  #include "zex_misc.h"
  #include "real_time.h"
#endif //pch

#include "Zsound.h"
#include "pobs.h"
#include "pictures.h"
#include "splat_control.h"
#include	"txtsplat.h"
#ifndef __3d_structures__  //might be from pch
#include	"3d_structures_def.h"
#endif

#include "instrum.h"

#include "buy.h"
#include "ship_kit.h"
#include "inventory.h"
#include "cargo.h"
#include "dlp.h"
#include "keyboard.h"
#include "olg_text.h"
#include "sds.h"


extern const RGBColor	rgbLightBlue;
extern int training_mission;
extern	DynObjectsFixedSize *ocb_ptr;
//extern galactic_map *GM_ptr;	  //ptr to the galactic map

//array for items and services available
//#define NUMBER_OF_GOODS 44
//items must be defined as per their enum found in buy.int
//as it is used as an index into this array
typedef struct
{
int quantity, basic_price, actual_price, _class, rcz_id, type, temp_removed;
Str255 short_description; Str255 long_description ; } stock;


//do not move these about - the enums in buy.int specify array index
stock ship_stock [NUMBER_OF_GOODS] =
{
  { 1,	56,	0,	WEAPONS, 3000,	MISSILE1, 0, "\pMissile 1","", },	   //got gfx
  { 0,	162,	0,	WEAPONS, 3001,	MISSILE2,0,"\pMissile 2","", },
  { 0,	324,	0,	WEAPONS, 3002,	MISSILE3,0,"\pMissile Type 3","", },
  { 1,	252,	0,	WEAPONS, 3003,	LASER1,0,"\pLaser Type 1","", },	  //got gfx
  { 0,	2741,	0,	WEAPONS, 3004,	LASER2,0,"\pLaser Type 2","", },	  //got gfx
  { 0,	6139,	0,	WEAPONS, 3005,	LASER3,0,"\pLaser Type 3","", },	  //gpt gfx

  { 1,	1112,	0,	WEAPONS, 3044,	TASER1,0,"\pTaser Type 1","", },	  //got gfx
  { 0,	5712,	0,	WEAPONS, 3045,	TASER2,0,"\pTaser Type 2","", },	  //got gfx
  { 0,	16012,	0,	WEAPONS, 3046,	TASER3,0,"\pTaser Type 3","", },	  //gpt gfx
  { 0,	1375,	0,	WEAPONS, 3006,	REAR_LASER1,0,"\pRear Laser Type 1","", },	//got gfx
  { 0,	3600,	0,	WEAPONS, 3007,	REAR_LASER2,0,"\pRear Laser Type 2","", },	//got gfx
  { 0,	9560,	0,	WEAPONS, 3008,	REAR_LASER3,0,"\pRear laser Type 3","", },	//got gfx
  { 1,	93,		0,	WEAPONS, 3009,	CANNON1,0,"\pCannon Type 1","", },
  { 0,	691,	0,	WEAPONS, 3010,	CANNON2,0,"\pCannon Type 2","", },
  { 0,	2903,	0,	WEAPONS, 3011,	CANNON3,0,"\pCannon Type 3","", },
  { 0,	654,	0,	WEAPONS, 3012,	SHOCKWAVE1,0,"\pShockwave Type 1","", },
  { 0,	1644,	0,	WEAPONS, 3013,	SHOCKWAVE2,0,"\pShockwave Type 2","", },
  { 0,	3987,	0,	WEAPONS, 3014,	ECM,0,"\pECM","", },	//got gfx
  { 0,	19309,	0,	WEAPONS, 3015,	MISSILE_FACTORY,0,"\pMissile Factory","", },	//got gfx
  { 0,	2092,	0,	WEAPONS, 3016,	TARGETTER_360,0,"\p360 Degrees Targeting","", },	  //got gfx

  { 0,	718,	0,	EQUIPMENT, 3017,	FAST_DOCK,0,"\pFast Dock","", },	  //got gfx
  { 0,	1998,	0,	EQUIPMENT, 3018,	FLATPACK,0,"\pFlatpack Ship","", },	//got gfx
  { 0,	352,	0,	EQUIPMENT, 3019,	EFFICIENT_ENGINE,0,"\pFuel Maximizer","", }, 	  //got gfx
  { 0,	1323,	0,	EQUIPMENT, 3020,	RETRO_ROCKETS,0,"\pRetro Thrust","", },	//got gfx
  { 0,	2930,	0,	EQUIPMENT, 3021,	HYPERSPACE,0,"\pWarp Jump","", },	  //got gfx
  { 0,	115,	0,	EQUIPMENT, 3022,	FLIGHT_COMPUTER1,0,"\pShip Computer","",  },
  { 0,	832,	0,	EQUIPMENT, 3023,	BULLET_PROOF_EJECTION,0,"\pIndestructable Ejection Pod","", },	  //got gfx
  { 0,	69,		0,	EQUIPMENT, 3024,	SHIELDS1,0,"\pShields 1","", },	//got gfx
  { 0,	301,	0,	EQUIPMENT, 3025,	SHIELDS2,0,"\pShields 2","", },	//got gfx
  { 0,	1808,	0,	EQUIPMENT, 3026,	SHIELDS3,0,"\pShields 3","", },	//got gfx
  { 0,	3892,	0,	EQUIPMENT, 3027,	SHIELDS4,0,"\pShields 4","", },	//got gfx
  { 0,	8622,	0,	EQUIPMENT, 3028,	SHIELDS5,0,"\pShields 5","", },	//got gfx

  { 0,	9,		0,	REPAIRS, 3029,	HULL_REPAIR,0,"\pHull Repair","", },
  { 0,	9,		0,	REPAIRS, 3030,	FUEL,0,"\pFuel","", },	//got gfx
  { 0,	221,	0,	REPAIRS, 3031,	REPAIR_SYSTEMS1,0,"\pRepair Systems 1","", },
  { 0,	813,	0,	REPAIRS, 3032,	REPAIR_SYSTEMS2,0,"\pRepair Systems 2","", },
  { 0,	3898,	0,	REPAIRS, 3033,	REPAIR_SYSTEMS3,0,"\pRepair Systems 3","", },

  { 0,	434,	0,	EQUIPMENT, 3034,	EJECTION,0,"\pStandard Ejection Pod","", },	//got gfx
  { 0,	715,	0,	EQUIPMENT, 3035,	FLIGHT_COMPUTER2,0,"\pShip Computer","", },
  { 0,	103,	0,	EQUIPMENT, 3036,	NAV_COMPUTER1,0,"\p","", },	//got gfx
  { 0,	415,	0,	EQUIPMENT, 3037,	NAV_COMPUTER2,0,"\p","", },	//got gfx
  { 0,	429,	0,	EQUIPMENT,	3038,	TRADING_COMPUTER1,0,"\p","", },
  { 0,	1941,	0,	EQUIPMENT,	3039,	TRADING_COMPUTER2,0,"\p","", },
  { 0,	3882,	0,	EQUIPMENT,	3040,	TRADING_COMPUTER3,0,"\p","", },
  { 0,	3482,	0,	EQUIPMENT,	3041,	AUTO_HULL_REPAIR,0,"\p","", },

  { 0,	1092,	0,	WEAPONS, 3042,	BOID_ADDITION,0,"\p","", },
  { 0,	500,	0,	WEAPONS, 3043,	ASTEROID_ADDITION,0,"\p","", },

};

//weapons......... stock, basic price
//stock buy_missiles1 = {1,100,WEAPONS,MISSILE1};
//stock buy_missiles2 = {0,150,WEAPONS,MISSILE2};
//stock buy_missiles3 = {0,200,WEAPONS,MISSILE3};
//
//stock buy_laser1 = {1,100,WEAPONS,LASER1};
//stock buy_laser2 = {0,200,WEAPONS,LASER2};
//stock buy_laser3 = {0,300,WEAPONS,LASER3};
//
//stock buy_rear_laser1 = {0,400,WEAPONS,REAR_LASER1};
//stock buy_rear_laser2 = {0,600,WEAPONS,REAR_LASER2};
//stock buy_rear_laser3 = {0,800,WEAPONS,REAR_LASER3};
//
//stock buy_cannon1 = {1,20,WEAPONS,CANNON1};
//stock buy_cannon2 = {0,190,WEAPONS,CANNON2};
//stock buy_cannon3 = {0,300,WEAPONS,CANNON3};
//
//stock buy_shockwave1 = {0,400,WEAPONS,SHOCKWAVE1};	//smart bomb 1 - 3000 units [S]
//stock buy_shockwave2 = {0,600,WEAPONS,SHOCKWAVE2};	//smart bomb 2 - 5000 units [S]	
//stock buy_ecm = {0,2000,WEAPONS,ECM};		  //destroys missiles [E]
//stock buy_missile_factory = {0,3000,WEAPONS,MISSILE_FACTORY};
//stock buy_360_degree_targetter = {0,2000,WEAPONS,TARGETTER_360};
////misc bits
//stock buy_fast_dock = {0,400,EQUIPMENT,FAST_DOCK};	//[F]
//stock buy_flatpack = {0,1000,EQUIPMENT,FLATPACK};	//spare life
//stock buy_aux_fuel_tank = {0,200,EQUIPMENT,AUX_FUEL_TANK};
//stock buy_retro_rockets = {0,1000,EQUIPMENT,RETRO_ROCKETS};	//[R]
//stock buy_hyperspace = {0,930,EQUIPMENT,HYPERSPACE};	  //[H]
//stock buy_nav_computer = {0,100,EQUIPMENT,NAV_COMPUTER};
//stock buy_bullet_proof_ejection = {0,3000,EQUIPMENT,BULLET_PROOF_EJECTION};	//cant be destroyed
//
//stock buy_shields1 = {0,200,EQUIPMENT,SHIELDS1};	  //diff level 1
//stock buy_shields2 = {0,350,EQUIPMENT,SHIELDS2};
//stock buy_shields3 = {0,700,EQUIPMENT,SHIELDS3};
//stock buy_shields4 = {0,1000,EQUIPMENT,SHIELDS4};
//stock buy_shields5 = {0,2000,EQUIPMENT,SHIELDS5};	//diff level 5
////repairs
//stock buy_hull_repair = {0,120,REPAIRS,HULL_REPAIR};	  //120 creds per 10 units needed
//stock buy_fuel = {0,2,REPAIRS,FUEL};	  //2 creds per unit
//stock buy_repair_systems1 = {0,200,REPAIRS,REPAIR_SYSTEMS1};	  //standard if destroyed
//stock buy_repair_systems2 = {0,800,REPAIRS,REPAIR_SYSTEMS2};	  //super duper
//stock buy_repair_systems3 = {0,1800,REPAIRS,REPAIR_SYSTEMS3};	//super duper and uses no fuel

//еееееееееееееееееееее

int old_mothership_stock_slot_number=0;
int docked_at_allegiance;
static int mship_supply_rating;
static int mship_supply_type;	//<5=agri, >=5 = tech

static int how_many_users;	//how many other users logged on?
extern int difficulty;

hold_desc hold_items[MAX_HOLD_ITEMS];

void do_buy_intro();
void buy_repairs();
void buy_weapons();
void buy_equipment();
void display_goods(int what_class);
void init_show_stock();
void show_stock(int stock_index);
void wait_for_mouse();
void draw_windows();
void setup_prices();
int do_buy_connect();
void combat_buy(LSRAW* exit_button);
void combat_sell(LSRAW* exit_button);
void display_ship_goods(int what_class);
void run_other_users(int from_class);
void reset_other_users();
void sell_equipment(int what_class);
void do_yer_actual_sell(int ship_item_id);
int translate_ship_to_stock(int item_id);
void display_commodities(LSRAW * exit_button);
void draw_lines(int trading_open);
void draw_commodity_strings(int from_index,int selected_item);
void setup_commodity_stocks();
void run_commodities();
void setup_commodity_stock_prices();
void draw_hold_items_docked(LSRAW* bar_pict, int trading_open);
void draw_bottom_bar(LSRAW*bar_pict,int can_exit, int can_buy, int can_sell, int can_inv);
void draw_inventory_strings(int from_index, int selected_item);
void sell_cargo(int item_selected);
void buy_cargo(int item_selected);
void do_string_dialog(unsigned char * the_string);
void draw_vert_lines(void);



void buy(int slot_docked_at)
{
extern int showing_stats;
extern int comms_will_fail;	//from docked

Point mouse_xy;

LSRAW *the_zoom_picture;
Handle zoom_H;
LSRAW *exit_button;
Handle exit_button_H;


int wait,i;
int button_state,exit;



//mship_supply_rating=get_supply_rating_docked(slot_docked_at);
mship_supply_rating=POBs_get_supply_rating(get_galactic_id(slot_docked_at));
//1 to 5=agri, 6-9=tech

//  mship_supply_type=(*ocb_ptr).object_list[slot_docked_at].Dyn_OCB_control_data.mship_ptr->supply_planet_index;
//  mship_supply_type=GM_ptr->the_quadrant[current_quad].quadrant_planets[mship_supply_type].type;	  //<5 agri
mship_supply_type=POBs_get_supply_type(get_galactic_id(slot_docked_at));	//quick fix to get zex 2 up

setup_commodity_stock_prices();

//see if player has been waiting long enough to get on the computer
//if (TickCount()>docked_at_time+(60*60*3)) comms_will_fail=0;	      //can now connect
comms_will_fail=0;
if (difficulty<16)	//never fail on easy setting
{
if (RangedRdm(0,100)>90) comms_will_fail=1;
}

 showing_stats=0;	//we're not any more
   
    disable_watchdog();
    zoom_H=GetZexPicture ('RCZ ',2005);	//galanet  
 	if (zoom_H==0) report_error("Memory Error: buy, zoom_H","\p",-1);
	HLock(zoom_H);
	the_zoom_picture = (LSRAW*)*zoom_H;
    exit_button_H=GetZexPicture ('RCZ ',2006);	//exit button   
 	if (exit_button_H==0) report_error("Memory Error: buy, exit_button_H","\p",-1);
	HLock(exit_button_H);
	exit_button = (LSRAW*)*exit_button_H;

reset_other_users();	//reset other buyrs/sellers

do_buy_intro();	//zoom computer screen onto screen

if (do_buy_connect()==0)
exit=0;
else
exit=1;	//skip if we couldn't connect

while (!exit)
{
//set up first screen

reset_main:	wait=0;
	for (i=0;i<2;i++)
	{
    erase_zex_rect(monitor_w,monitor_h);	//in pictures
	 print_crunched_pict(the_zoom_picture,off_640_x+0,0+off_480_y);
 	 print_crunched_trans_pict(exit_button,off_640_x+260,380+off_480_y);
    Show_it();
    }
 while (Button());	//wait for mouse up
 
while (wait==0)
{
     ZexGetMouse (&mouse_xy);
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
     button_state=Button();
     if (button_state==1)
      {
        if (mouse_xy.h>(off_640_x+260) && mouse_xy.h<(off_640_x+392) && mouse_xy.v>(off_480_y+410) && mouse_xy.v<(off_480_y+460))
        { wait=1; exit=1; }	//exit


        if (mouse_xy.h>(off_640_x+108) && mouse_xy.h<(off_640_x+247) && mouse_xy.v>(off_480_y+288) && mouse_xy.v<(off_480_y+348))
        wait=2;	//cbuy

        if (mouse_xy.h>(off_640_x+247) && mouse_xy.h<(off_640_x+378) && mouse_xy.v>(off_480_y+288) && mouse_xy.v<(off_480_y+348))
        wait=3;	//csell


        if (mouse_xy.h>(off_640_x+378) && mouse_xy.h<(off_640_x+539) && mouse_xy.v>(off_480_y+288) && mouse_xy.v<(off_480_y+348))
        wait=4;	//csell

//        else       
//        if (mouse_xy.h<395)
//        wait=3;	//weapons
//        else
//        wait=4;	//repairs
      }
    
    
    if (wait==2) 
    {
    combat_buy(exit_button);	//pass the exit button
    goto reset_main;
    }

    if (wait==3) 
    {
    combat_sell(exit_button);	//pass the exit button
    goto reset_main;
    }

    if (wait==4) 
    {
    display_commodities(exit_button);	//pass the exit button
    goto reset_main;
    }

//    else
//    if (wait==3) buy_weapons();
//    else
//    if (wait==4) buy_repairs();
  
 }	//end of !exit
 
}
    
    DisposeHandle(zoom_H);
    DisposeHandle(exit_button_H);

enable_watchdog();
}

void combat_buy(LSRAW* exit_button)
{
LSRAW *cbuy_picture;
Handle cbuy_H;
int wait,button_state;
Point mouse_xy;
int exit,i;

        cbuy_H=GetZexPicture ('RCZ ',6000);	//galanet  
	HLock(cbuy_H);
	cbuy_picture = (LSRAW*)*cbuy_H;


reset_cbuy:	for (i=0;i<2;i++)
	{
    erase_zex_rect(monitor_w,monitor_h);	//in pictures
	 print_crunched_pict(cbuy_picture,0+off_640_x,0+off_480_y);
 	 print_crunched_trans_pict(exit_button,260+off_640_x,380+off_480_y);
    Show_it();
    }
    
 while (Button());	//wait for mouse up
wait=0;
//buy equipment
while (wait==0)
{
     ZexGetMouse (&mouse_xy);
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
     button_state=Button();
     if (button_state==1)
      {
        if (mouse_xy.h>(260+off_640_x) && mouse_xy.h<(392+off_640_x) && mouse_xy.v>(410+off_480_y) && mouse_xy.v<(460+off_480_y))
        { wait=1; exit=1; }	//exit

        if (mouse_xy.h>(60+off_640_x) && mouse_xy.h<(565+off_640_x) && mouse_xy.v>(180+off_480_y) && mouse_xy.v<(390+off_480_y))
        {
        if (mouse_xy.h<(230+off_640_x))	//equipment
        wait=2;
        else       
        if (mouse_xy.h<(395+off_640_x))
        wait=3;	//weapons
        else
        wait=4;	//repairs
        }
      }
    
    
    if (wait==2) 
     {
      buy_equipment();
      goto reset_cbuy;
     }
    else
    if (wait==3)
    {
     buy_weapons();
     goto reset_cbuy;
    }
    else
    if (wait==4) 
    {
     buy_repairs();
     goto reset_cbuy;
    }
 }	//end of !exit
    DisposeHandle(cbuy_H);

}



void combat_sell(LSRAW* exit_button)
{
LSRAW *csell_picture;
Handle csell_H;
int wait,button_state;
Point mouse_xy;
int exit,i;

        csell_H=GetZexPicture ('RCZ ',6001);	 //combat net sell  
	HLock(csell_H);
	csell_picture = (LSRAW*)*csell_H;


reset_csell:	for (i=0;i<2;i++)
	{
    erase_zex_rect(monitor_w,monitor_h);	//in pictures
	 print_crunched_pict(csell_picture,0+off_640_x,0+off_480_y);
 	 print_crunched_trans_pict(exit_button,260+off_640_x,380+off_480_y);
    Show_it();
    }
    
 while (Button());	//wait for mouse up
wait=0;
//buy equipment
while (wait==0)
{
     ZexGetMouse (&mouse_xy);
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
     button_state=Button();
     if (button_state==1)
      {
        if (mouse_xy.h>(260+off_640_x) && mouse_xy.h<(392+off_640_x) && mouse_xy.v>(410+off_480_y) && mouse_xy.v<(460+off_480_y))
        { wait=1; exit=1; }	//exit

        if (mouse_xy.h>(162+off_640_x) && mouse_xy.h<(324+off_640_x) && mouse_xy.v > (180+off_480_y) && mouse_xy.v<(408+off_480_y) )	//equipment
        wait=2;
        else       
        if (mouse_xy.h>(324+off_640_x) && mouse_xy.h<(483+off_640_x) && mouse_xy.v > (180+off_480_y) && mouse_xy.v<(408+off_480_y))
        wait=3;	//weapons
      }
    
    
    if (wait==2) 
     {
      sell_equipment(EQUIPMENT);
      goto reset_csell;
     }
    else
    if (wait==3)
    {
      sell_equipment(WEAPONS);
     goto reset_csell;
    }
 }	//end of !exit
    DisposeHandle(csell_H);

}

void sell_equipment(int what_class)
{
int button_state,exit;
int sell_what;

     button_state=Button();
     while (button_state==1) button_state=Button();	//wait for mouse up

//display the inventory screen with prices
//    global_show_status=1;
   exit=0;
   while(exit==0)
   {
    sell_what=do_sell_kit(what_class);	//in inventory
    if (sell_what!=-1)
      {
      //do sell dialog
      do_yer_actual_sell(sell_what);
      }
      else
      exit=1;
   }
   
}

//these routines have to display goods and tie up a mouse click with a good
//the goods are displayed on a grid where each node is 128*128 - this gives a grid
//of 5*3.
//we note what good is at each location with an array
int display_item_array[15];

void buy_equipment()
{
extern int credits;
int button_state,exit;
int ax,ay,index,wait;
Point mouse_xy;

     button_state=Button();
     while (button_state==1) button_state=Button();	//wait for mouse up

display_goods(EQUIPMENT);
    Show_it();
//display_item_array[] 0-14 is now full of items
    exit=0;
    while (exit==0)
     {
//     wait_for_mouse();

    
     wait=0;
     //button down
      while (wait==0)
     {
      run_other_users(EQUIPMENT);	//others buy/sell!
      if (credits==0) break;
         button_state=Button();
         if (button_state==1) wait=1;
     }
    //button up
    wait=0;
    while (wait==0)
    {
     button_state=Button();
     if (button_state==0) wait=1;
    }

//bottom 20, right 30 is exit
     ZexGetMouse (&mouse_xy);
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
     if (((mouse_xy.h>(540+off_640_x)) && (mouse_xy.h<(640+off_640_x)) && (mouse_xy.v>(430+off_480_y)) && (mouse_xy.v<(480+off_480_y))) || credits==0)
     break;
     
//check if y<400, if so it's a bona fide selection
     if (mouse_xy.v<(400+off_480_y))
      {
      //calc display_item_array index
      ax=(mouse_xy.h-off_640_x)/(640/5);	//5 rows	
      ay=(mouse_xy.v-off_480_y)/(400/3);	//3 columns
      index=ay*5+ax;
      if (index>14) index=14;	//cant be, but just in case
      if (display_item_array[index]==-1)	//nothiong there
      play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
      else
      {
      //buy it...
      do_yer_actual_buy(display_item_array[index],EQUIPMENT);
      display_goods(EQUIPMENT);	//redraw and update display
    Show_it();
      }     
      }
     else	//y>400
     play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);

    }

}

void buy_repairs()
{
extern int credits;
int button_state,exit;
int ax,ay,index,wait;
Point mouse_xy;

     button_state=Button();
     while (button_state==1) button_state=Button();	//wait for mouse up

display_goods(REPAIRS);
    Show_it();
//display_item_array[] 0-14 is now full of items
    exit=0;
    while (exit==0)
     {
//     wait_for_mouse();

    
     wait=0;
     //button down
      while (wait==0)
     {
      run_other_users(REPAIRS);	//others buy/sell!
      if (credits==0) break;
      
         button_state=Button();
         if (button_state==1) wait=1;
     }
    //button up
    wait=0;
    while (wait==0)
    {
     button_state=Button();
     if (button_state==0) wait=1;
    }
//bottom 20, right 30 is exit
     ZexGetMouse (&mouse_xy);
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
     if (((mouse_xy.h>(540+off_640_x)) && (mouse_xy.h<(640+off_640_x)) && (mouse_xy.v>(430+off_480_y)) && (mouse_xy.v<(480+off_480_y))) || credits==0)
     break;
     
//check if y<400, if so it's a bona fide selection
     if (mouse_xy.v<(400+off_480_y))
      {
      //calc display_item_array index
      ax=(mouse_xy.h-off_640_x)/(640/5);	//5 rows	
      ay=(mouse_xy.v-off_480_y)/(400/3);	//3 columns
      index=ay*5+ax;
      if (index>14) index=14;	//cant be, but just in case
      if (display_item_array[index]==-1)	//nothiong there
      play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
      else
      {
      //buy it...
      do_yer_actual_buy(display_item_array[index],REPAIRS);
      display_goods(REPAIRS);	//redraw and update display
    Show_it();
      }     
      }
     else	//y>400
     play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);

    }

//int button_state;
//
//     button_state=Button();
//     while (button_state==1) button_state=Button();	//wait for mouse up
//display_goods(REPAIRS);
//    Show_it();
//     wait_for_mouse();
}


void buy_weapons()
{
extern int credits;
int button_state,exit;
int ax,ay,index,wait;
Point mouse_xy;

     button_state=Button();
     while (button_state==1) button_state=Button();	//wait for mouse up

    display_goods(WEAPONS);
    Show_it();
//display_item_array[] 0-14 is now full of items
    exit=0;
    while (exit==0)
     {
    
     wait=0;
     //button down
      while (wait==0)
     {
      run_other_users(WEAPONS);	//others buy/sell!
      if (credits==0) break;
         button_state=Button();
         if (button_state==1) wait=1;
     }
    //button up
    wait=0;
    while (wait==0)
    {
     button_state=Button();
     if (button_state==0) wait=1;
    }
//bottom 20, right 30 is exit
     ZexGetMouse (&mouse_xy);
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
     if (((mouse_xy.h>(540+off_640_x)) && (mouse_xy.h<(640+off_640_x)) && (mouse_xy.v>(430+off_480_y)) && (mouse_xy.v<(480+off_480_y))) || credits==0)
     break;
     
//check if y<400, if so it's a bona fide selection
     if (mouse_xy.v<(400+off_480_y))
      {
      //calc display_item_array index
      ax=(mouse_xy.h-off_640_x)/(640/5);	//5 rows	
      ay=(mouse_xy.v-off_480_y)/(400/3);	//3 columns
      index=ay*5+ax;
      if (index>14) index=14;	//cant be, but just in case
      if (display_item_array[index]==-1)	//nothiong there
      play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
      else
      {
      //buy it...
      do_yer_actual_buy(display_item_array[index],WEAPONS);
      display_goods(WEAPONS);	//redraw and update display
    Show_it();
      }     
      }
     else	//y>400
     play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);

    }


//int button_state;
//
//     button_state=Button();
//     while (button_state==1) button_state=Button();	//wait for mouse up
//display_goods(WEAPONS);
//    Show_it();
//     wait_for_mouse();
}


//routine that checks and buys an item
void do_yer_actual_buy(int stock_item_id, int screen_class)
{
extern kit_desc ship_items[ITEMS_COUNT];
extern int credits;
extern int hull_strength;
extern GDHandle				mZexGDH;
LSRAW *dialog_picture;
Handle dialog_H;
GDHandle	saveGDH;
GrafPtr		savePort;
unsigned char number_string[12];
Point mouse_xy;
int buyers_price,needed_fuel;
int tc_tries,tc_max_tries;

     if (check_needed(stock_item_id)==0) 	//not needed?
     {
           play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
           return;
     }
//calc tc_max_tries
tc_tries=0;
tc_max_tries=ship_items[KTRADING_COMPUTER].modifier*5;
     
rehaggle:	//err, a label!

     if (ship_items[KTRADING_COMPUTER].status==1)
     dialog_H=GetZexPicture ('RCZ ',2011);	  //haggle dialog box
     else
     dialog_H=GetZexPicture ('RCZ ',2012);	//confirm dialog box
        
	HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;

//accept price or negatiate?

//    if (Button()==1);	//wait for mouse up
    
    display_goods(screen_class);

	print_crunched_pict(dialog_picture,180+off_640_x,130+off_480_y);
    TextFont(21);
TextSize(12);
TextFace(normal);
ZRGBForeColor(&rgbBlue);
     ZMoveTo(426+off_640_x,150+off_480_y);
     ZexNumToStringPascal(ship_stock[stock_item_id].actual_price,number_string);
     ZDrawString("\pCr ");
     ZDrawString(number_string);
     
     draw_string_with_cr(200,150,ship_stock[stock_item_id].long_description);
//if we have a trading computer calculate price
     if (ship_items[KTRADING_COMPUTER].status==1)
     {
     //if fuel or hull
     if (stock_item_id==FUEL)
      {
        needed_fuel=180-ship_items[KNORMAL_FUEL_TANK].value1;
        if (needed_fuel>ship_stock[stock_item_id].quantity) needed_fuel=ship_stock[stock_item_id].quantity;
        
        buyers_price=do_trading_computer
                     (ship_stock[stock_item_id].actual_price*needed_fuel, stock_item_id);
      }
     else if (stock_item_id==HULL_REPAIR)
      {
        buyers_price=do_trading_computer(ship_stock[stock_item_id].actual_price
                  *(50-hull_strength), stock_item_id);
      }
     else
     buyers_price=do_trading_computer(ship_stock[stock_item_id].actual_price, stock_item_id);

     ZMoveTo (200+off_640_x,220+off_480_y);
	 ZDrawString ("\pTrading Computer gets a price of ");
	 ZexNumToStringPascal(buyers_price,number_string);
     ZDrawString(number_string);
     }
     else	//no trading computer
     {
          if (stock_item_id==FUEL)
      {
        buyers_price=ship_stock[stock_item_id].actual_price
                  *(180-ship_items[KNORMAL_FUEL_TANK].value1);
      }
      else if (stock_item_id==HULL_REPAIR)
      {
        buyers_price=ship_stock[stock_item_id].actual_price*(50-hull_strength);
      }
      else	//not fuel or hull
       buyers_price=ship_stock[stock_item_id].actual_price;
     }
     

     ZMoveTo (200+off_640_x,240+off_480_y);
     if (ship_items[KTRADING_COMPUTER].status==1)
     ZDrawString("\pAccept the price, re-negotiate or cancel?");
     else
     {
     ZDrawString("\pAccept the price of ");
     ZexNumToStringPascal(buyers_price,number_string);
     ZDrawString(number_string);
     ZDrawString("\p credits or cancel?");
     
     }
   Show_it();
   DisposeHandle(dialog_H);
     wait_for_mouse();

//bottom 20, right 30 is exit
     ZexGetMouse (&mouse_xy);
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
 
//have a trading computer?
     if (ship_items[KTRADING_COMPUTER].status==1)
     {
     if (mouse_xy.h>(380+off_640_x) && mouse_xy.h<(480+off_640_x) && mouse_xy.v>(250+off_480_y) && mouse_xy.v<(280+off_480_y)) return;	//cancel
     if (mouse_xy.h>(280+off_640_x) && mouse_xy.h<(380+off_640_x) && mouse_xy.v>(250+off_480_y) && mouse_xy.v<(280+off_480_y)) 
        {
          //check if exceeeded number of tries
          tc_tries++;
          if (tc_tries>tc_max_tries)
               {

                GetPort (&savePort);
	        saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
// 	SetPort((GrafPtr)the_drawing_buffer);
	        SetGDevice(mZexGDH);

                dialog_H=GetZexPicture ('RCZ ',2010);	     //blank dialog box   
        
	         HLock(dialog_H);
	         dialog_picture = (LSRAW*)*dialog_H;
               ship_stock[stock_item_id].temp_removed=1;	//seller got bored and removed the item
               display_goods(screen_class);
               ZRGBForeColor(&rgbRed);

               print_crunched_pict(dialog_picture,180+off_640_x,130+off_480_y);
               ZMoveTo (200+off_640_x,180+off_480_y);
               ZDrawString("\pThe seller became frustrated with");
               ZMoveTo (200+off_640_x,200+off_480_y);
               ZDrawString("\pyour trading computer and has");
               ZMoveTo (200+off_640_x,220+off_480_y);
               ZDrawString("\pwithdrawn the item from sale.");
               Show_it();
               
               wait_for_mouse();
              DisposeHandle(dialog_H);
               
               return;
               }
          play_zsound_always(sound_test, sound_high_pri, c1_chan1, sound_vol_7);
          goto rehaggle;	//recalc
        }
     }
     
     else
     if (mouse_xy.h>(320+off_640_x) && mouse_xy.h<(480+off_640_x) && mouse_xy.v>(256+off_480_y) && mouse_xy.v<(278+off_480_y)) return;	//cancel
     if (mouse_xy.h>(180+off_640_x) && mouse_xy.h<(320+off_640_x) && mouse_xy.v>(256+off_480_y) && mouse_xy.v<(278+off_480_y))
     goto buy_it;
     else
     {
        play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
        goto rehaggle;	      //do nothing
     }
//must be OK!

   
buy_it:   dialog_H=GetZexPicture ('RCZ ',2010);	//blank dialog box   
	HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;

//bottom 20, right 30 is exit
     ZexGetMouse (&mouse_xy);
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);

//show you have bought
       GetPort (&savePort);
	 saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
 
//	SetPort((GrafPtr)the_drawing_buffer);
	 SetGDevice(mZexGDH);
   display_goods(screen_class);

	print_crunched_pict(dialog_picture,180+off_640_x,130+off_480_y);

      TextFont(21);
TextSize(14);
TextFace(bold);
ZRGBForeColor(&rgbBlue);
//do text
    if (buyers_price>credits)
    {
     ZMoveTo (200+off_640_x,170+off_480_y);

     if (ship_stock[stock_item_id].short_description[1]=='R' && 
        ship_stock[stock_item_id].short_description[2]=='e' &&
        ship_stock[stock_item_id].short_description[2]=='f')
      {
       ZDrawString("\pYou don't have enough credit to refuel.");
      }
     else
     {

      ZDrawString("\pYou don't have enough credit to buy");
      ZMoveTo (200+off_640_x,188+off_480_y);
      if (ship_stock[stock_item_id].short_description[1]=='S' && 
        ship_stock[stock_item_id].short_description[2]=='h' )
       {
       //if shields, dont draw "the"
       }    
	  else
	   {
 	   ZDrawString("\pthe ");
	   }
      ZDrawString (ship_stock[stock_item_id].short_description);
      ZDrawString ("\p.");
     }
     ZMoveTo (200+off_640_x,210+off_480_y);
     ZDrawString ("\pYou have ");
     ZexNumToStringPascal(credits,number_string);
     ZDrawString(number_string);
     ZDrawString ("\p credits.");
   
     ZMoveTo (200+off_640_x,228+off_480_y);
     ZDrawString ("\pIt costs ");
     ZexNumToStringPascal(buyers_price, number_string);
     ZDrawString(number_string);
     ZDrawString ("\p credits.");
    }	//end of price too great
    
    else	//price OK
    {
     ZMoveTo (200+off_640_x,170+off_480_y);
     
   if (ship_stock[stock_item_id].short_description[1]=='R' && 
        ship_stock[stock_item_id].short_description[2]=='e' &&
        ship_stock[stock_item_id].short_description[2]=='f')
   {
   ZDrawString("\pYou have refuelled.");
   }
   else
   {
     ZDrawString("\pYou have bought ");
     if (ship_stock[stock_item_id].short_description[1]=='S' && 
        ship_stock[stock_item_id].short_description[2]=='h' )
       {
       //if shields, dont draw "the"
       }    
	 else
	  {
 	   ZDrawString("\pthe");
       ZMoveTo (200+off_640_x,188+off_480_y);
	  }
     ZDrawString (ship_stock[stock_item_id].short_description);
     ZDrawString ("\p for ");
     ZexNumToStringPascal(buyers_price, number_string);
     ZMoveTo (200+off_640_x,206+off_480_y);
     ZDrawString(number_string);
     ZDrawString ("\p credits.");
   }
    
     credits=credits-buyers_price;
     ship_stock[stock_item_id].quantity--;
     //update ship
     update_ship(stock_item_id);	//add the item to zex
     
    }
   
	SetGDevice(saveGDH);	//these are here so I can SEE them!
	SetPort(savePort);
    Show_it();

    DisposeHandle(dialog_H);
    wait_for_mouse();
}



void do_yer_actual_sell(int ship_item_id)
{
extern kit_desc ship_items[ITEMS_COUNT];
extern int credits;
extern GDHandle				mZexGDH;
LSRAW *dialog_picture;
Handle dialog_H;
GDHandle	saveGDH;
GrafPtr		savePort;
unsigned char number_string[12];
Point mouse_xy;
int buyers_price;
int tc_tries,tc_max_tries;
int stock_item_id;

//translate ship item to stock item
stock_item_id=translate_ship_to_stock(ship_item_id);

if (stock_item_id==-1)             report_error("translate_to_ship_item returned error!","\p",ship_item_id);

//calc tc_max_tries
tc_tries=0;
tc_max_tries=ship_items[KTRADING_COMPUTER].modifier*5;
     
rehaggle:	//err, a label!

     GetPort (&savePort);
	 saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
// 	SetPort((GrafPtr)the_drawing_buffer);
	 SetGDevice(mZexGDH);

     if (ship_items[KTRADING_COMPUTER].status==1)
     dialog_H=GetZexPicture ('RCZ ',2011);	  //haggle dialog box
     else
     dialog_H=GetZexPicture ('RCZ ',2012);	//confirm dialog box
        
	HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;

//accept price or negatiate?

//    if (Button()==1);	//wait for mouse up
    
//    display_goods(screen_class);

	print_crunched_pict(dialog_picture,180+off_640_x,130+off_640_x);
    TextFont(21);
TextSize(12);
TextFace(normal);
ZRGBForeColor(&rgbBlue);
     ZMoveTo(426+off_640_x,150+off_480_y);
     ZexNumToStringPascal(ship_stock[stock_item_id].actual_price,number_string);
     ZDrawString("\pCr ");
     ZDrawString(number_string);
     
     draw_string_with_cr(200,150,ship_stock[stock_item_id].long_description);
//if we have a trading computer calculate price
//     if (ship_items[KTRADING_COMPUTER].status==1)
//     {
//     float buying_price,percent;
//     buying_price=ship_stock[stock_item_id].actual_price;
//     percent=buying_price/RangedRdm(30,60);	  //between 40 and 60% off
//     buying_price=buying_price-percent;
//     
//     buyers_price=do_trading_computer(buying_price, stock_item_id);
//
//     MoveTo (200,220);
//	   DrawString ("\pTrading Computer gets a price of ");
//	   NumToString(buyers_price,number_string);
//     DrawString(number_string);
//     }
//     else	//no trading computer
     {
      float temp_price,fdiscount;
       buyers_price=ship_stock[stock_item_id].actual_price;
      temp_price=buyers_price;
      fdiscount=.40;
      temp_price=temp_price-(temp_price*fdiscount);
      if (temp_price<1) temp_price=1;	//dont want credits!     
      buyers_price=temp_price;
     }
     

     ZMoveTo (200+off_640_x,240+off_480_y);
     if (ship_items[KTRADING_COMPUTER].status==1)
     ZDrawString("\pAccept the offer, re-negotiate or cancel?");
     else
     {
     ZDrawString("\pAccept the offer of ");
	 ZexNumToStringPascal(buyers_price,number_string);
     ZDrawString(number_string);
     ZDrawString("\p credits or cancel?");
     
     }
 	 SetGDevice(saveGDH);	 //these are here so I can SEE them!
	SetPort(savePort);
   Show_it();
   DisposeHandle(dialog_H);
     wait_for_mouse();

//bottom 20, right 30 is exit
     ZexGetMouse (&mouse_xy);
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
 
//have a trading computer?
     if (ship_items[KTRADING_COMPUTER].status==1)
     {
     if (mouse_xy.h>(380+off_640_x) && mouse_xy.h<(480+off_640_x) && mouse_xy.v>(250+off_480_y) && mouse_xy.v<(280+off_480_y)) return;	//cancel
     if (mouse_xy.h>(280+off_640_x) && mouse_xy.h<(380+off_640_x) && mouse_xy.v>(250+off_480_y) && mouse_xy.v<(280+off_480_y)) 
        {
          //check if exceeeded number of tries
          play_zsound_always(sound_test, sound_high_pri, c1_chan1, sound_vol_7);
          goto rehaggle;	//recalc
        }
     }
     
     else
     if (mouse_xy.h>(320+off_640_x) && mouse_xy.h<(480+off_640_x) && mouse_xy.v>(256+off_480_y) && mouse_xy.v<(278+off_480_y)) return;	//cancel
     if (mouse_xy.h>(180+off_640_x) && mouse_xy.h<(320+off_640_x) && mouse_xy.v>(256+off_480_y) && mouse_xy.v<(278+off_480_y))
     goto buy_it;
     else
     {
        play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
        goto rehaggle;	      //do nothing
     }
//must be OK!

   
buy_it:   dialog_H=GetZexPicture ('RCZ ',2010);	//blank dialog box   
	HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;

//bottom 20, right 30 is exit
     ZexGetMouse (&mouse_xy);
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);

//show you have bought
       GetPort (&savePort);
	 saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
 
//	SetPort((GrafPtr)the_drawing_buffer);
	 SetGDevice(mZexGDH);

	print_crunched_pict(dialog_picture,180+off_640_x,130+off_480_y);

      TextFont(21);
TextSize(14);
TextFace(bold);
ZRGBForeColor(&rgbBlue);
//do text
     ZMoveTo (200+off_640_x,170+off_480_y);
     
      {
     ZDrawString("\pYou have sold ");
 	   ZDrawString("\pthe");
       ZMoveTo (200+off_640_x,188+off_480_y);
     ZDrawString (ship_stock[stock_item_id].short_description);
     ZDrawString ("\p for ");
     ZexNumToStringPascal(buyers_price, number_string);
     ZMoveTo (200+off_640_x,206+off_480_y);
     ZDrawString(number_string);
     ZDrawString ("\p credits.");
      }
    
     credits=credits+buyers_price;
     ship_stock[stock_item_id].quantity++;
     ship_items[ship_item_id].status=0;
     ship_items[ship_item_id].modifier=0;
     //update ship
//     update_ship(stock_item_id);	  //add the item to zex
     
//    }	//here
   
	SetGDevice(saveGDH);	//these are here so I can SEE them!
	SetPort(savePort);
    Show_it();

    DisposeHandle(dialog_H);
    wait_for_mouse();
}

//takes an item and return 0 - fitted already, or 1 if needed
int check_needed(int item_id)
{
extern kit_desc ship_items[ITEMS_COUNT];
extern int hull_strength;
int return_value=0;

if (ship_stock[item_id].temp_removed==1) return 0;	  //withdrawn

   switch (item_id)
        {
        case MISSILE1:
           if (ship_items[KMISSILE_PYLON1].modifier==0 ||
               ship_items[KMISSILE_PYLON2].modifier==0 ||
               ship_items[KMISSILE_PYLON3].modifier==0)
             {
              return_value=1;
             }
           break;

        case MISSILE2:
           if (ship_items[KMISSILE_PYLON1].modifier==0 ||
               ship_items[KMISSILE_PYLON2].modifier==0 ||
               ship_items[KMISSILE_PYLON3].modifier==0)
             {
              return_value=1;
             }
           break;
           
        case MISSILE3:
           if (ship_items[KMISSILE_PYLON1].modifier==0 ||
               ship_items[KMISSILE_PYLON2].modifier==0 ||
               ship_items[KMISSILE_PYLON3].modifier==0)
             {
              return_value=1;
             }
           break;
           
           
        case LASER1:
             if (ship_items[KLASER].status<=0)  return_value=1;
             if(ship_items[KLASER].modifier<1)  return_value=1;
           break;
        case LASER2:
             if (ship_items[KLASER].status<=0) return_value=1;
             else
             if(ship_items[KLASER].modifier<2)  return_value=1;

           break;
        case LASER3:
             if (ship_items[KLASER].status<=0)  return_value=1;
             else
             if(ship_items[KLASER].modifier<3)  return_value=1;
           break;

        case TASER1:
             if (ship_items[KLASER].status<=0)  return_value=1;
             if(ship_items[KLASER].modifier<11)  return_value=1;
           break;
        case TASER2:
             if (ship_items[KLASER].status<=0) return_value=1;
             else
             if(ship_items[KLASER].modifier<12)  return_value=1;

           break;
        case TASER3:
             if (ship_items[KLASER].status<=0)  return_value=1;
             else
             if(ship_items[KLASER].modifier<13)  return_value=1;
           break;
           
        case REAR_LASER1:
             if (ship_items[KREAR_LASER].status<=0) return_value=1;
             if(ship_items[KREAR_LASER].modifier<1)  return_value=1;
           break;
        case REAR_LASER2:
             if (ship_items[KREAR_LASER].status<=0)  return_value=1;
             else
             if(ship_items[KREAR_LASER].modifier<2)  return_value=1;
           break;
        case REAR_LASER3:
             if(ship_items[KREAR_LASER].status<=0)  return_value=1;
             else
             if(ship_items[KREAR_LASER].modifier<3)  return_value=1;
           break;

           break;

        case CANNON1:
             if(ship_items[KCANNON].status<=0)  return_value=1;
             if(ship_items[KCANNON].modifier==0)  return_value=1;
           break;
           
        case CANNON2:
             if(ship_items[KCANNON].status<=0)  return_value=1;
             else
             if(ship_items[KCANNON].modifier<2)  return_value=1;
           break;

        case CANNON3:
             if(ship_items[KCANNON].status<=0)  return_value=1;
             else
             if(ship_items[KCANNON].modifier<3)  return_value=1;
           break;

        case SHOCKWAVE1:
             if(ship_items[KSHOCKWAVE].status<=0)  return_value=1;
            break;

        case SHOCKWAVE2:
             if(ship_items[KSHOCKWAVE].status<=0)  return_value=1;
             else
             if(ship_items[KSHOCKWAVE].modifier<2)  return_value=1;
           break;

        case ECM:
             if(ship_items[KECM].status<=0) return_value=1;
           break;

        case MISSILE_FACTORY:
             if(ship_items[KMISSILE_FACTORY].status<=0) return_value=1;
           break;

        case TARGETTER_360:
             if(ship_items[K360].status<=0)  return_value=1;
           break;

        case FAST_DOCK:
             if(ship_items[KFAST_DOCK].status<=0)  return_value=1;
           break;

        case FLATPACK:
             if(ship_items[KFLATPACK].status<=0)  return_value=1;
             else
             if(ship_items[KFLATPACK].value1<3)  return_value=1;
             
           break;

        case EFFICIENT_ENGINE:
             if(ship_items[KFUEL_MAXIMISER].status<=0)  return_value=1;
           break;

        case RETRO_ROCKETS:
             if(ship_items[KRETRO_ROCKETS].status<=0)  return_value=1;
           break;

        case HYPERSPACE:
             if(ship_items[KHYPERSPACE].status<=0)  return_value=1;
           break;

        case FLIGHT_COMPUTER1:
             if(ship_items[KFLIGHT_COMPUTER].status<=0)  return_value=1;
           break;

        case FLIGHT_COMPUTER2:
             if(ship_items[KFLIGHT_COMPUTER].status<=0)  return_value=1;
             else
             if(ship_items[KFLIGHT_COMPUTER].modifier==1)  return_value=1;
           break;

        case NAV_COMPUTER1:
             if(ship_items[KNAV_COMPUTER].status<=0)  return_value=1;
             if (ship_items[KNAV_COMPUTER].modifier==0) return_value=1;
           break;
        case NAV_COMPUTER2:
             if(ship_items[KNAV_COMPUTER].status<=0)  return_value=1;
             else
             if(ship_items[KNAV_COMPUTER].modifier<2)  return_value=1;
           break;


        case BULLET_PROOF_EJECTION:
             if(ship_items[KEJECTION].status<=0)  return_value=1;
             else
             if(ship_items[KEJECTION].modifier<2)  return_value=1;
           break;

        case SHIELDS1:
             if(ship_items[KSHIELDS].status<=0)  return_value=1;
             if (ship_items[KSHIELDS].modifier==0) return_value=1;
           break;

        case SHIELDS2:
             if(ship_items[KSHIELDS].status<=0)  return_value=1;
             else
             if(ship_items[KSHIELDS].modifier<2)  return_value=1;
           break;

        case SHIELDS3:
             if(ship_items[KSHIELDS].status<=0)  return_value=1;
             else
             if(ship_items[KSHIELDS].modifier<3)  return_value=1;
           break;

        case SHIELDS4:
             if(ship_items[KSHIELDS].status<=0)  return_value=1;
             else
             if(ship_items[KSHIELDS].modifier<4)  return_value=1;
           break;

        case SHIELDS5:
             if(ship_items[KSHIELDS].status<=0)  return_value=1;
             else
             if(ship_items[KSHIELDS].modifier<5)  return_value=1;
           break;

        case HULL_REPAIR:
        if(hull_strength!=50) return_value=1;
           break;

        case FUEL:
			 if(ship_items[KNORMAL_FUEL_TANK].value1!=180) return_value=1;

           break;

        case REPAIR_SYSTEMS1:
             if(ship_items[KREPAIR_SYSTEMS].status<=0)  return_value=1;
           break;

        case REPAIR_SYSTEMS2:
             if(ship_items[KREPAIR_SYSTEMS].status<=0)  return_value=1;
             else
             if(ship_items[KREPAIR_SYSTEMS].modifier<2)  return_value=1;
           break;

        case REPAIR_SYSTEMS3:
             if(ship_items[KREPAIR_SYSTEMS].status<=0)  return_value=1;
             else
             if(ship_items[KREPAIR_SYSTEMS].modifier<3)  return_value=1;
           break;

        case EJECTION:
             if(ship_items[KEJECTION].status<=0) return_value=1;
           break;

        case TRADING_COMPUTER1:
             if(ship_items[KTRADING_COMPUTER].status<=0)  return_value=1;
           break;

        case TRADING_COMPUTER2:
             if(ship_items[KTRADING_COMPUTER].status<=0)  return_value=1;
             else
             if(ship_items[KTRADING_COMPUTER].modifier<2)  return_value=1;
           break;

        case TRADING_COMPUTER3:
             if(ship_items[KTRADING_COMPUTER].status<=0)  return_value=1;
             else
             if(ship_items[KTRADING_COMPUTER].modifier<3)  return_value=1;
           break;

        case AUTO_HULL_REPAIR:
             if(ship_items[KAUTO_HULL_REPAIR].status<=0) return_value=1;
           break;

        case BOID_ADDITION:
             if(ship_items[KBOID_ADDITION].status<=0) return_value=1;
           break;

        case ASTEROID_ADDITION:
             if(ship_items[KASTEROID_ADDITION].status<=0) return_value=1;
           break;

        default:
            report_error("Unknown item in check_needed!","\p",item_id);
           break;
        }
    return return_value;
}	//end of check_needed



void update_ship(int item_id)
{
//extern int hull_damage;
extern kit_desc ship_items[ITEMS_COUNT];
extern int hull_strength;

   switch (item_id)
        {
        case MISSILE1:
           if (ship_items[KMISSILE_PYLON1].modifier==0)
             {
             ship_items[KMISSILE_PYLON1].status=1;
             ship_items[KMISSILE_PYLON1].modifier=1;
             }
           else
           if (ship_items[KMISSILE_PYLON2].modifier==0)
             {
             ship_items[KMISSILE_PYLON2].status=1;
             ship_items[KMISSILE_PYLON2].modifier=1;
             }
           else
           if (ship_items[KMISSILE_PYLON3].modifier==0)
             {
             ship_items[KMISSILE_PYLON3].status=1;
             ship_items[KMISSILE_PYLON3].modifier=1;
             }
                      
           break;
        case MISSILE2:
           if (ship_items[KMISSILE_PYLON1].modifier==0)
             {
             ship_items[KMISSILE_PYLON1].status=1;
             ship_items[KMISSILE_PYLON1].modifier=2;
             }
           else
           if (ship_items[KMISSILE_PYLON2].modifier==0)
             {
             ship_items[KMISSILE_PYLON2].status=1;
             ship_items[KMISSILE_PYLON2].modifier=2;
             }
           else
           if (ship_items[KMISSILE_PYLON3].modifier==0)
             {
             ship_items[KMISSILE_PYLON3].status=1;
             ship_items[KMISSILE_PYLON3].modifier=2;
             }
                      
           break;
        case MISSILE3:
           if (ship_items[KMISSILE_PYLON1].modifier==0)
             {
             ship_items[KMISSILE_PYLON1].status=1;
             ship_items[KMISSILE_PYLON1].modifier=3;
             }
           else
           if (ship_items[KMISSILE_PYLON2].modifier==0)
             {
             ship_items[KMISSILE_PYLON2].status=1;
             ship_items[KMISSILE_PYLON2].modifier=3;
             }
           else
           if (ship_items[KMISSILE_PYLON3].modifier==0)
             {
             ship_items[KMISSILE_PYLON3].status=1;
             ship_items[KMISSILE_PYLON3].modifier=3;
             }
                      
           break;
           
           
        case LASER1:
             ship_items[KLASER].status=1;
             ship_items[KLASER].modifier=1;
           break;
        case LASER2:
             ship_items[KLASER].status=1;
             ship_items[KLASER].modifier=2;
           break;
        case LASER3:
             ship_items[KLASER].status=1;
             ship_items[KLASER].modifier=3;
           break;

        case TASER1:
             ship_items[KLASER].status=1;
             ship_items[KLASER].modifier=11;
           break;
        case TASER2:
             ship_items[KLASER].status=1;
             ship_items[KLASER].modifier=12;
           break;
        case TASER3:
             ship_items[KLASER].status=1;
             ship_items[KLASER].modifier=13;
           break;


        case REAR_LASER1:
             ship_items[KREAR_LASER].status=1;
             ship_items[KREAR_LASER].modifier=1;
           break;
        case REAR_LASER2:
             ship_items[KREAR_LASER].status=1;
             ship_items[KREAR_LASER].modifier=2;
           break;
        case REAR_LASER3:
             ship_items[KREAR_LASER].status=1;
             ship_items[KREAR_LASER].modifier=2;
           break;

        case CANNON1:
             ship_items[KCANNON].status=1;
             ship_items[KCANNON].modifier=1;
           break;
        case CANNON2:
             ship_items[KCANNON].status=1;
             ship_items[KCANNON].modifier=2;
           break;
        case CANNON3:
             ship_items[KCANNON].status=1;
             ship_items[KCANNON].modifier=2;
           break;

        case SHOCKWAVE1:
             ship_items[KSHOCKWAVE].status=1;
             ship_items[KSHOCKWAVE].modifier=1;
             ship_items[KSHOCKWAVE].value1=3;

           break;
        case SHOCKWAVE2:
             ship_items[KSHOCKWAVE].status=1;
             ship_items[KSHOCKWAVE].modifier=2;
             ship_items[KSHOCKWAVE].value1=3;
           break;

        case ECM:
             ship_items[KECM].status=1;
             ship_items[KECM].modifier=1;
           break;

        case MISSILE_FACTORY:
             ship_items[KMISSILE_FACTORY].status=1;
             ship_items[KMISSILE_FACTORY].modifier=1;
           break;

        case TARGETTER_360:
             ship_items[K360].status=1;
             ship_items[K360].modifier=1;
           break;

        case FAST_DOCK:
             ship_items[KFAST_DOCK].status=1;
             ship_items[KFAST_DOCK].modifier=1;
           break;

        case FLATPACK:
             ship_items[KFLATPACK].status=1;
             ship_items[KFLATPACK].value1++;
           break;

        case EFFICIENT_ENGINE:
             ship_items[KFUEL_MAXIMISER].status=1;
             ship_items[KFUEL_MAXIMISER].modifier=1;
           break;

        case RETRO_ROCKETS:
             ship_items[KRETRO_ROCKETS].status=1;
             ship_items[KRETRO_ROCKETS].value1=3;
             ship_items[KRETRO_ROCKETS].modifier=1;
           break;

        case HYPERSPACE:
             ship_items[KHYPERSPACE].status=1;
             ship_items[KHYPERSPACE].modifier=1;
           break;

        case FLIGHT_COMPUTER1:
             ship_items[KFLIGHT_COMPUTER].status=1;
             ship_items[KFLIGHT_COMPUTER].modifier=1;
           break;

        case FLIGHT_COMPUTER2:
             ship_items[KFLIGHT_COMPUTER].status=1;
             ship_items[KFLIGHT_COMPUTER].modifier=2;
           break;

        case NAV_COMPUTER1:
             ship_items[KNAV_COMPUTER].status=1;
             ship_items[KNAV_COMPUTER].modifier=1;
           break;
        case NAV_COMPUTER2:
             ship_items[KNAV_COMPUTER].status=1;
             ship_items[KNAV_COMPUTER].modifier=2;
           break;


        case BULLET_PROOF_EJECTION:
             ship_items[KEJECTION].status=1;
             ship_items[KEJECTION].modifier=2;
           break;

        case SHIELDS1:
             ship_items[KSHIELDS].status=1;
             ship_items[KSHIELDS].modifier=1;
             ship_items[KSHIELDS].value1=1000;
           break;

        case SHIELDS2:
             ship_items[KSHIELDS].status=1;
             ship_items[KSHIELDS].modifier=2;
             ship_items[KSHIELDS].value1=1000;

           break;

        case SHIELDS3:
             ship_items[KSHIELDS].status=1;
             ship_items[KSHIELDS].modifier=3;
            ship_items[KSHIELDS].value1=1000;
           break;

        case SHIELDS4:
             ship_items[KSHIELDS].status=1;
             ship_items[KSHIELDS].modifier=4;
             ship_items[KSHIELDS].value1=1000;

           break;

        case SHIELDS5:
             ship_items[KSHIELDS].status=1;
             ship_items[KSHIELDS].modifier=5;
             ship_items[KSHIELDS].value1=1000;

           break;

        case HULL_REPAIR:
			 //hull_damage=0;
             hull_strength=50;
           break;

        case FUEL:
			 ship_items[KNORMAL_FUEL_TANK].value1=180;

           break;

        case REPAIR_SYSTEMS1:
             ship_items[KREPAIR_SYSTEMS].status=1;
             ship_items[KREPAIR_SYSTEMS].modifier=1;
           break;

        case REPAIR_SYSTEMS2:
             ship_items[KREPAIR_SYSTEMS].status=1;
             ship_items[KREPAIR_SYSTEMS].modifier=2;
           break;

        case REPAIR_SYSTEMS3:
             ship_items[KREPAIR_SYSTEMS].status=1;
             ship_items[KREPAIR_SYSTEMS].modifier=3;
           break;

        case EJECTION:
             ship_items[KEJECTION].status=1;
             ship_items[KEJECTION].modifier=1;
           break;

        case TRADING_COMPUTER1:
             ship_items[KTRADING_COMPUTER].status=1;
             ship_items[KTRADING_COMPUTER].modifier=1;
           break;

        case TRADING_COMPUTER2:
             ship_items[KTRADING_COMPUTER].status=1;
             ship_items[KTRADING_COMPUTER].modifier=2;
           break;

        case TRADING_COMPUTER3:
             ship_items[KTRADING_COMPUTER].status=1;
             ship_items[KTRADING_COMPUTER].modifier=3;
           break;

        case AUTO_HULL_REPAIR:
             ship_items[KAUTO_HULL_REPAIR].status=1;
           break;

        case BOID_ADDITION:
             ship_items[KBOID_ADDITION].status=1;
           break;

        case ASTEROID_ADDITION:
             ship_items[KASTEROID_ADDITION].status=1;
           break;

        default:
            report_error("Unknown item in update_ship!","\p",item_id);
           break;
        }
}


int translate_ship_to_stock(int item_id)
{
extern kit_desc ship_items[ITEMS_COUNT];

   switch (item_id)
        {
        case KMISSILE_PYLON1:
           if (ship_items[KMISSILE_PYLON1].modifier==1)
		return 	MISSILE1;
           else
           if (ship_items[KMISSILE_PYLON1].modifier==2)
	        return 	MISSILE2;
	    else
           if (ship_items[KMISSILE_PYLON1].modifier==3)
                return 	MISSILE3;                      
           break;
        
        case KMISSILE_PYLON2:
           if (ship_items[KMISSILE_PYLON2].modifier==1)
		return 	MISSILE1;
           else
           if (ship_items[KMISSILE_PYLON2].modifier==2)
	        return 	MISSILE2;
	    else
           if (ship_items[KMISSILE_PYLON2].modifier==3)
                return 	MISSILE3;                      
                      
           break;
        case KMISSILE_PYLON3:
           if (ship_items[KMISSILE_PYLON3].modifier==1)
		return 	MISSILE1;
           else
           if (ship_items[KMISSILE_PYLON3].modifier==2)
	        return 	MISSILE2;
	    else
           if (ship_items[KMISSILE_PYLON3].modifier==3)
                return 	MISSILE3;                      
                      
                      
           break;
           
           
        case KLASER:
           if (ship_items[KLASER].modifier==1)
		return 	LASER1;
           else
           if (ship_items[KLASER].modifier==2)
	        return 	LASER2;
	    else
           if (ship_items[KLASER].modifier==3)
                return 	LASER3;                      

           if (ship_items[KLASER].modifier==11)
		return 	TASER1;
           else
           if (ship_items[KLASER].modifier==12)
	        return 	TASER2;
	    else
           if (ship_items[KLASER].modifier==13)
                return 	TASER3;                      
                      
                      
           break;



        case KREAR_LASER:
           if (ship_items[KREAR_LASER].modifier==1)
		return 	REAR_LASER1;
           else
           if (ship_items[KREAR_LASER].modifier==2)
	        return 	REAR_LASER2;
	    else
           if (ship_items[KREAR_LASER].modifier==3)
                return 	REAR_LASER3;                      
           break;


        case KCANNON:
           if (ship_items[KCANNON].modifier==1)
		return 	CANNON1;
           else
           if (ship_items[KCANNON].modifier==2)
	        return 	CANNON2;
	    else
           if (ship_items[KCANNON].modifier==3)
                return 	CANNON3;                      
           break;


        case KSHOCKWAVE:
           if (ship_items[KSHOCKWAVE].modifier==1)
		return 	SHOCKWAVE1;
           else
           if (ship_items[KSHOCKWAVE].modifier==2)
	        return 	SHOCKWAVE2;

           break;

        case KECM:
                 return ECM;
           break;


        case KMISSILE_FACTORY:
		return MISSILE_FACTORY;
           break;

        case K360:
		return TARGETTER_360; 
	    break;

        case KFAST_DOCK:
		return FAST_DOCK;
           break;

        case KFLATPACK:
		return FLATPACK;
           break;

        case KFUEL_MAXIMISER:
		return EFFICIENT_ENGINE;
           break;

        case KRETRO_ROCKETS:
		return RETRO_ROCKETS;
           break;

        case KHYPERSPACE:
		return HYPERSPACE;
           break;

        case KFLIGHT_COMPUTER:
             if (ship_items[KFLIGHT_COMPUTER].modifier==1)
                 return FLIGHT_COMPUTER1;
             else
             if (ship_items[KFLIGHT_COMPUTER].modifier==2)
                 return FLIGHT_COMPUTER2;
                 
           break;


        case KNAV_COMPUTER:
             if (ship_items[KNAV_COMPUTER].modifier==1)
                 return NAV_COMPUTER1;
             else
             if (ship_items[KNAV_COMPUTER].modifier==2)
                 return NAV_COMPUTER2;
                 
           break;


        case KEJECTION:
             if (ship_items[KEJECTION].modifier==1)
                 return EJECTION;
             else
             if (ship_items[KEJECTION].modifier==2)
                 return BULLET_PROOF_EJECTION;
                 
           break;


        case KSHIELDS:
             if (ship_items[KSHIELDS].modifier==1)
                 return SHIELDS1;
             else
             if (ship_items[KSHIELDS].modifier==2)
                 return SHIELDS2;
             else
             if (ship_items[KSHIELDS].modifier==3)
                 return SHIELDS3;
             else
             if (ship_items[KSHIELDS].modifier==4)
                 return SHIELDS4;
             else
             if (ship_items[KSHIELDS].modifier==5)
                 return SHIELDS5;
                 
           break;



        case KREPAIR_SYSTEMS:
             if (ship_items[KREPAIR_SYSTEMS].modifier==1)
                 return REPAIR_SYSTEMS1;
             else
             if (ship_items[KREPAIR_SYSTEMS].modifier==2)
                 return REPAIR_SYSTEMS2;
             else
             if (ship_items[KREPAIR_SYSTEMS].modifier==3)
                 return REPAIR_SYSTEMS3;
           break;


        case KTRADING_COMPUTER:
             if (ship_items[KTRADING_COMPUTER].modifier==1)
                 return TRADING_COMPUTER1;
             else
             if (ship_items[KTRADING_COMPUTER].modifier==2)
                 return TRADING_COMPUTER2;
             else
             if (ship_items[KTRADING_COMPUTER].modifier==3)
                 return TRADING_COMPUTER3;
           break;



        case KAUTO_HULL_REPAIR:
             return HULL_REPAIR;
           break;

        case KBOID_ADDITION:
             return BOID_ADDITION;
           break;

        case KASTEROID_ADDITION:
             return ASTEROID_ADDITION;
           break;

        case KRADAR:
             return ASTEROID_ADDITION;
           break;

        default:
            report_error("Unknown item in translate_ship_to_stock!","\p",item_id);
           break;
        }
return -1;
}



int do_trading_computer(int actual_price, int stock_index)
{
extern kit_desc ship_items[ITEMS_COUNT];
float temp_price,fdiscount;
int tc_class;
int discount=0;

temp_price=actual_price;

     if (ship_items[KTRADING_COMPUTER].status==1)
     {
      tc_class=ship_items[KTRADING_COMPUTER].modifier;
      if (tc_class==1) 
      {
       discount=5;
       discount=RangedRdm(discount*100,(discount+ship_stock[stock_index].quantity)*100 );
      }
      else
      if (tc_class==2)
      {
       discount=16;
       discount=RangedRdm(discount*100,(discount+(ship_stock[stock_index].quantity*4)*100) );
      }
      else
      if (tc_class==3)
      {
       discount=25;
       discount=RangedRdm(discount*100,(discount+(ship_stock[stock_index].quantity*8)*100) );
      }

      fdiscount=(float)discount/100;
      if (fdiscount>60) fdiscount=60;
      temp_price=temp_price-(temp_price*(fdiscount/100));
      if (temp_price<1) temp_price=1;	//dont want credits!     
      return temp_price;
     }
     else
     return actual_price;

}
void draw_string_with_cr(int x, int y, Str255 the_string)
{
int end=1;
int line_end;
int in_index=1;
int char_count;
int total_char_count;
Str255 line_str;	//this must be pascal

total_char_count=(int)the_string[0];

while (end==1)	//another line?
	{
		ZMoveTo(x+off_640_x,y+off_480_y);
		//now copy a line
		char_count=0;	//chars on this line
		line_end=0;
		//copy char to line
		while(line_end==0)
		{
			line_str[char_count+1]=the_string[in_index];
			if (line_str[char_count+1]==13) line_end=1;	//it's a cr
			in_index++;
			if (in_index==total_char_count) 
			{
				end=0;	//done all chars?
				line_end=1;	//eol as well
			}
			char_count++;
		}
		line_str[0]=(unsigned char) char_count;
		ZDrawString(line_str);
		y=y+15;
	}
}

void wait_for_mouse()
{
int wait=0;
int button_state;

wait=0;
//button down
while (wait==0)
 {
     button_state=Button();
     if (button_state==1) wait=1;
 }

//button up
wait=0;
while (wait==0)
 {
     button_state=Button();
     if (button_state==0) wait=1;
 }

}


void display_goods(int what_class)
{
extern int credits,tutorial;

int display_item_array_index,ship_stock_index,x;
GDHandle	saveGDH;
GrafPtr		savePort;
unsigned char number_string[12];

init_show_stock();	//reset prinitng coords
//draw 15 small windows
     GetPort (&savePort);
	saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
// 	SetPort((GrafPtr)the_drawing_buffer);
	SetGDevice(mZexGDH);
	RGBBackColor (&rgbBlack);
//	   EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect_colour(monitor_w,monitor_h,242);	//in pictures
     TextFace(0);
     draw_windows();

display_item_array_index=ship_stock_index=0;
for (x=0;x<15;x++) display_item_array[x]=-1;	//clear out display array


      for (x=0;x<NUMBER_OF_GOODS+1;x++)
           {
             if (ship_stock[ship_stock_index]._class==what_class && ship_stock[ship_stock_index].quantity>0)
                {
                  show_stock(ship_stock_index);
                  display_item_array[display_item_array_index]=ship_stock_index;
                  display_item_array_index++;
                  if (display_item_array_index>14) x=1000;	//prevent weird crashes - too many goods to display
                }  
             ship_stock_index++;               
           }
if (training_mission!=0 || tutorial==1)
   {
    ZMoveTo(245+off_640_x,420+off_480_y);
    ZRGBForeColor(&rgbRed);
    ZDrawString("\p*Simulated items for training purposes*");
   }

//show credits in bottom of screen

TextFont(21);
TextSize(14);
TextFace(bold);
ZRGBForeColor(&rgbBlue);

//MoveTo (0,440);
//DrawString("\pAvailable credit:");
ZexNumToStringPascal(credits,number_string);
ZMoveTo (130+off_640_x,454+off_480_y);
ZDrawString(number_string);

	SetGDevice(saveGDH);	//these are here so I can SEE them!
	SetPort(savePort);
//    Show_it();

}



//runs through ships items displaying those that fit the class
void display_ship_goods(int what_class)
{
extern int credits;

int display_item_array_index,ship_stock_index,x;
GDHandle	saveGDH;
GrafPtr		savePort;
unsigned char number_string[12];

init_show_stock();	//reset prinitng coords
//draw 15 small windows
     GetPort (&savePort);
	saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
// 	SetPort((GrafPtr)the_drawing_buffer);
	SetGDevice(mZexGDH);
	RGBBackColor (&rgbBlack);
//	   EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect_colour(monitor_w,monitor_h,242);	//in pictures
     TextFace(0);
     draw_windows();

display_item_array_index=ship_stock_index=0;
for (x=0;x<15;x++) display_item_array[x]=-1;	//clear out display array


      for (x=0;x<NUMBER_OF_GOODS+1;x++)
           {
             if (ship_stock[ship_stock_index]._class==what_class && ship_stock[ship_stock_index].quantity>0)
                {
                  show_stock(ship_stock_index);
                  display_item_array[display_item_array_index]=ship_stock_index;
                  display_item_array_index++;
                  if (display_item_array_index>14) x=1000;	//prevent weird crashes - too many goods to display
                }  
             ship_stock_index++;               
           }
//show credits in bottom of screen
TextFont(21);
TextSize(14);
TextFace(bold);
ZRGBForeColor(&rgbBlue);

//MoveTo (0,440);
//DrawString("\pAvailable credit:");
ZexNumToStringPascal(credits,number_string);
ZMoveTo (130+off_640_x,454+off_480_y);
ZDrawString(number_string);

	SetGDevice(saveGDH);	//these are here so I can SEE them!
	SetPort(savePort);
//    Show_it();

}

void draw_windows()
{
LSRAW *the_window_picture;
Handle window_H;
LSRAW *the_bar_picture;
Handle bar_H;

int x,y,i;
x=0;
y=0;

    window_H=GetZexPicture ('RCZ ',2008);	//small window   
	HLock(window_H);
	the_window_picture = (LSRAW*)*window_H;

    bar_H=GetZexPicture ('RCZA',129);	//bottom bar   
	HLock(bar_H);
	the_bar_picture = (LSRAW*)*bar_H;
	print_pict(the_bar_picture,0+off_640_x,430+off_480_y);

   for (i=0; i<15;i++)
   {
	print_crunched_pict(the_window_picture,x*127+2+off_640_x,y*140+off_480_y);
     x++; 
     if (x>4) 
     {
      x=0;
      if (y<4)	//a little protection
      y++;
     }
    }
    
DisposeHandle(window_H);
DisposeHandle(bar_H);
}

int show_x,show_y;
void init_show_stock()
{
show_x=show_y=0;
}

void show_stock(int stock_index)
{
extern kit_desc ship_items[ITEMS_COUNT];
extern int hull_strength;
int x,y,width,price;
Rect erase_desc = {111,20,123,100};
unsigned char number_string[12];
LSRAW *item_picture;
Handle item_picture_H;

x=show_x*128; y=show_y*140;

//get the items picture
    item_picture_H=GetZexPicture ('RCZ ',ship_stock[stock_index].rcz_id);	//the picture handle  
	HLock(item_picture_H);
	item_picture = (LSRAW*)*item_picture_H;
	print_crunched_trans_pict(item_picture,x+30+off_640_x,y+24+off_480_y);
	DisposeHandle(item_picture_H);

//draw the string
ZRGBBackColor(&rgbBlack);

TextFont(21);
TextSize(9);

//center the string
width=StringWidth(ship_stock[stock_index].short_description);
width=128-width;
width/=2;
erase_desc.top+=y+off_480_y;
erase_desc.left+=x+off_640_x;
erase_desc.bottom+=y+off_480_y;
erase_desc.right+=x+off_640_x;
ZEraseRect(&erase_desc);

ZRGBForeColor(&rgbLightBlue);
ZMoveTo(x+width+off_640_x,y+120+off_480_y);
ZDrawString(ship_stock[stock_index].short_description);
//show stock level
ZMoveTo (x+46+off_640_x,y+100+off_480_y);
  ZexNumToStringPascal(ship_stock[stock_index].quantity,number_string);
  ZDrawString(number_string);


//show price
ZMoveTo (x+43+off_640_x,y+19+off_480_y);
if (ship_stock[stock_index].temp_removed==1)
{
ZRGBForeColor(&rgbRed);
  ZDrawString("\pWithdrawn!");
ZRGBForeColor(&rgbLightBlue);
show_x++; 
if (show_x>4) 
 {
  show_x=0;
  if (show_y<4)	//a little protection
  show_y++;
 }
return;
}

if (check_needed(stock_index))
{
  //if stock is fuel or hull repairs we multiply needed by basic
  price=ship_stock[stock_index].actual_price;
  if (stock_index==FUEL)
  {
  price=price*(180-ship_items[KNORMAL_FUEL_TANK].value1);
  }
  else if (stock_index==HULL_REPAIR)
  {
  price=price*(50-hull_strength);
  }
  ZexNumToStringPascal(price,number_string);
  ZDrawString(number_string);
}
else
  ZDrawString("\pNot Needed");

show_x++; 
if (show_x>4) 
 {
  show_x=0;
  if (show_y<4)	//a little protection
  show_y++;
 }
}




//flash connecting on screen
int do_buy_connect()
{
//extern kit_desc ship_items[ITEMS_COUNT];
//extern int mothership_cat;
extern int comms_will_fail;
Handle zoom_H;

LSRAW *the_zoom_picture;
int i, toggle;
GDHandle	saveGDH;
GrafPtr		savePort;
int return_val;

return_val=0;	//asume OK

toggle=1;
     GetPort (&savePort);
     saveGDH = GetGDevice();	    //All calls to QD are surrounded by save and set gdevices
     SetGDevice(mZexGDH);
//first fade the screen o blue
for (i=243;i>235;i--)
{
//    the_drawing_buffer=Get_back_buffer();
    erase_zex_rect_colour(monitor_w,monitor_h,i);	//in pictures
    Show_it();
    user_delay(5);

}
//no show connection porgress
for (i=0;i<20;i++)
{
    erase_zex_rect_colour(monitor_w,monitor_h,235);	//in pictures
    toggle=toggle^1;
    if (toggle==0) 
    {
//     	TextFont(21);
//	TextSize(24);
//	TextFace(normal);
	ZRGBForeColor(&rgbLightBlue);

     	ZMoveTo(240+off_640_x, 200+off_480_y);
        if (i<10)
      	ZDrawString ("\pConnecting...");
        else if (i<16) ZDrawString ("\pNegotiating...");
        else 
        {
 
         if (comms_will_fail>0)
         {
 	 ZRGBForeColor(&rgbRed);
      	 ZMoveTo(210+off_640_x, 200+off_480_y);
         ZDrawString ("\pCouldn't negotiate!");        
 	 return_val=-2;
         }
         else
         //if alien ship and we have no trading computer then fail
         if ( 0 /* mothership_cat==ALIEN  && player_control_data.trading_comp_class<=0*/)
         {
 	 ZRGBForeColor(&rgbRed);
      	 ZMoveTo(210+off_640_x, 200+off_480_y);
         ZDrawString ("\pCouldn't negotiate!");        
 	 return_val=-1;
         }
         else /* if (mothership_cat==ALIEN) */
/*         DrawString ("\pTranslating.");
         else  */
         ZDrawString ("\pConnected.");
        }
    }
    Show_it();
    user_delay(12);

}

if (return_val==-1)
{
//    the_drawing_buffer=Get_back_buffer();
    erase_zex_rect_colour(monitor_w,monitor_h,235);	//in pictures
	ZRGBForeColor(&rgbRed);
     	ZMoveTo(0+off_640_x,200+off_480_y);
        ZDrawString ("\pConnect Failed. Trading Computer needed for alien systems.");        
	TextSize(12);
     	ZMoveTo(240+off_640_x,300+off_480_y);
        ZDrawString ("\pClick mouse button to exit.");        
    Show_it();
    wait_for_mouse();


}
else
if (return_val==-2)
{
//    the_drawing_buffer=Get_back_buffer();
    erase_zex_rect_colour(monitor_w,monitor_h,235);	//in pictures
	ZRGBForeColor(&rgbRed);
     	ZMoveTo(60+off_640_x,200+off_480_y);
     	if (comms_will_fail==1)
        ZDrawString ("\pConnect failed; network too busy. Please try later.");        
	else
        ZDrawString ("\pConnect failed; network damaged.  Please try later.");        
	
	TextSize(12);
     	ZMoveTo(240+off_640_x,300+off_480_y);
        ZDrawString ("\pClick mouse button to exit.");        
    Show_it();
    wait_for_mouse();


}



	SetGDevice(saveGDH);	//these are here so I can SEE them!
	SetPort(savePort);

if (return_val==0)
{
 play_zsound(sound_test, sound_high_pri, ls1_chan, sound_vol_7);
   zoom_H=GetZexPicture ('RCZ ',2005);	//zoomed   
 	if (zoom_H==0) report_error("Memory Error: do_buy_connect, zoom_H","\p",-1);
	HLock(zoom_H);
	the_zoom_picture = (LSRAW*)*zoom_H;

//    the_drawing_buffer=Get_back_buffer();
//	  SetPort((GrafPtr)the_drawing_buffer);
//    GetPort (&savePort);
//	  saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//	  SetGDevice(mZexGDH);
//	  RGBBackColor (&rgbBlack);
//	  EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect(monitor_w,monitor_h);	//in pictures
//	  SetGDevice(saveGDH);	  //these are here so I can SEE them!
//	  SetPort(savePort);
	print_crunched_pict(the_zoom_picture,0+off_640_x,0+off_480_y);
    Show_it();

	DisposeHandle(zoom_H);
}
return return_val;
}

void do_buy_intro()
{
//get the picture - rcz 2004

Handle computer_H;

LSRAW *the_picture;

LSRAW* pictbuffer;
int memsize,i,y_correct,x_correct;
float mag;
 
 
 return;
 

    computer_H=GetZexPicture ('RCZ ',2004);	//computer   
 	if (computer_H==0) report_error("Memory Error: do_buy_intro, computer_H","\p",-1);
	HLock(computer_H);
	the_picture = (LSRAW*)*computer_H;

//    zoom_H=GetZexPicture ('RCZ ',2005);	//zoomed   
// 	if (zoom_H==0) report_error("Memory Error: do_buy_intro, zoom_H","\p",-1);
//	  HLock(zoom_H);
//	  the_zoom_picture = (LSRAW*)*zoom_H;

//we need to decompress this picture using my ever-so-useful picture utilities
	memsize=(*the_picture).pwidth;
	memsize*=(*the_picture).pheight;
	memsize+=80;
	pictbuffer=(LSRAW*)NewPtr(memsize);
 	if (pictbuffer==0) report_error("Memory Error: do_buy_intro","\p",-1);
 	decrunch_pict(the_picture,pictbuffer);


 	mag=1.0;
 	y_correct=0;
    	x_correct=0;
    
//	the_drawing_buffer=Get_back_buffer();
	print_pict_magnified(pictbuffer,0,0,y_correct,x_correct,mag);
    	Show_it();	//show us what happened!!!
    	user_delay(30);

 	for (i=0;i<11;i++)
 	{
//	the_drawing_buffer=Get_back_buffer();
	print_pict_magnified(pictbuffer,0,0,y_correct,x_correct,mag);
     Show_it();	//show us what happened!!!
     user_delay(1);
     y_correct+=34;
     x_correct-=8;	//these get us centered as we zoom in
     mag+=0.2;
    }


//now print the zoomed picture
//    the_drawing_buffer=Get_back_buffer();
////	    SetPort((GrafPtr)the_drawing_buffer);
////    GetPort (&savePort);
////	    saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
////	    SetGDevice(mZexGDH);
////	    RGBBackColor (&rgbBlack);
////	    EraseRect(&the_drawing_buffer->portRect);
//    erase_zex_rect(640,480);	  //in pictures
////	    SetGDevice(saveGDH);	//these are here so I can SEE them!
////	    SetPort(savePort);
//	  print_crunched_pict(the_zoom_picture,0,0);
//    Show_it();
//	DisposeHandle(zoom_H);
	DisposeHandle(computer_H);
	DisposePtr((Ptr) pictbuffer);
}


//this routine has the tricky job of setting up the stock for a mothership. 


void setup_stock(int slot_number)	//of mothership
{
//we only recalculate if this mothership is different to last visited
extern int score,current_level;


//if (in_ramparian_space==1) DebugStr("\pMothership in RMParian space!");

docked_at_allegiance=(*ocb_ptr).object_list[slot_number].Dyn_OCB_control_data.allegiance;
//if (old_mothership_stock_slot_number==slot_number) return;
old_mothership_stock_slot_number=slot_number;
setup_commodity_stocks();	//set up the ommodities area
setup_prices();	//calculate actual kit from basic prices


if ((*ocb_ptr).object_list[slot_number].Dyn_OCB_control_data.allegiance==HUMAN)
{
	ship_stock[MISSILE1].quantity = RangedRdm(3,25);
    if (score>5000)
	ship_stock[MISSILE2].quantity = RangedRdm(1,5);

    if (score>55500)
	ship_stock[MISSILE3].quantity = RangedRdm(1,5);

ship_stock[LASER1].quantity = RangedRdm(1,4);

if (score>4000)
ship_stock[LASER2].quantity = RangedRdm(1,2);

if (score>35000)
ship_stock[LASER3].quantity = RangedRdm(0,2);



if (score>2000)
ship_stock[NAV_COMPUTER2].quantity = RangedRdm(0,2);

if (score>1000)
ship_stock[REAR_LASER1].quantity = RangedRdm(1,4);
if (score>19000)
ship_stock[REAR_LASER2].quantity = RangedRdm(1,4);
if (score>50000)
ship_stock[REAR_LASER3].quantity = RangedRdm(0,4);


ship_stock[TRADING_COMPUTER1].quantity = RangedRdm(1,3);
if (score>9000)
ship_stock[TRADING_COMPUTER2].quantity = RangedRdm(1,3);
if (score>30000)
ship_stock[TRADING_COMPUTER3].quantity = RangedRdm(0,4);


if (score>1000)
ship_stock[CANNON1].quantity = RangedRdm(1,4);

//if (score>20000)
ship_stock[CANNON2].quantity = RangedRdm(1,4);

if (score>70000)
ship_stock[CANNON3].quantity = RangedRdm(0,4);


if (score>100000)
ship_stock[SHOCKWAVE1].quantity = RangedRdm(1,2);

//if (score>160000)
//ship_stock[SHOCKWAVE2].quantity = 0;	  //not available on human ships

if (score>20000)
ship_stock[ECM].quantity = RangedRdm(0,2);



//misc bits
if (score>5000)
ship_stock[FAST_DOCK].quantity = RangedRdm(1,2);	//[F]

if (score>50000)
ship_stock[MISSILE_FACTORY].quantity = RangedRdm(0,2);


ship_stock[FLATPACK].quantity = RangedRdm(1,5);
ship_stock[EJECTION].quantity = RangedRdm(1,5);

if (score>5000)
ship_stock[TARGETTER_360].quantity = 0;	//not available on human ships

if (score>20000)
ship_stock[EFFICIENT_ENGINE].quantity = RangedRdm(0,2);

ship_stock[RETRO_ROCKETS].quantity = RangedRdm(0,2);

if (score>50000)
ship_stock[HYPERSPACE].quantity = RangedRdm(0,2);

ship_stock[NAV_COMPUTER1].quantity = RangedRdm(1,2);
ship_stock[BULLET_PROOF_EJECTION].quantity = RangedRdm(0,2);
ship_stock[SHIELDS1].quantity = RangedRdm(1,2);

if (score>5000 || RangedRdm(1,100)>80)
ship_stock[SHIELDS2].quantity = RangedRdm(1,3);
if (score>15000 || RangedRdm(1,100)>80)
ship_stock[SHIELDS3].quantity = RangedRdm(0,2);
if (score>30000)
ship_stock[SHIELDS4].quantity = RangedRdm(0,2);
if (score>100000)
ship_stock[SHIELDS5].quantity = RangedRdm(0,2);
////repairs
ship_stock[HULL_REPAIR].quantity = 1;

if (current_level>=1)
ship_stock[ASTEROID_ADDITION].quantity = 6;

ship_stock[FUEL].quantity = RangedRdm(0,500);
ship_stock[REPAIR_SYSTEMS1].quantity = RangedRdm(0,2);
if (score>10000)
ship_stock[REPAIR_SYSTEMS2].quantity = RangedRdm(0,2);

if (score>50000)
ship_stock[REPAIR_SYSTEMS3].quantity = RangedRdm(0,2);
}	//end of zex category

else	//alein mothership

{
	ship_stock[MISSILE1].quantity = RangedRdm(3,15);
    if (score>5000 || RangedRdm(1,100)>80)
	ship_stock[MISSILE2].quantity = RangedRdm(1,5);

    if (score>45000)
	ship_stock[MISSILE3].quantity = RangedRdm(1,5);
ship_stock[EJECTION].quantity = RangedRdm(1,2);

ship_stock[LASER1].quantity = RangedRdm(1,4);

if (score>5000 || RangedRdm(1,100)>80)
ship_stock[LASER2].quantity = RangedRdm(1,2);
if (score>45000)
ship_stock[LASER3].quantity = RangedRdm(0,2);

ship_stock[TASER1].quantity = RangedRdm(1,4);

if (score>30000 || RangedRdm(1,100)>80)
ship_stock[TASER2].quantity = RangedRdm(1,2);
if (score>45000)
ship_stock[TASER3].quantity = RangedRdm(0,2);


ship_stock[ASTEROID_ADDITION].quantity = 1;

if (score>5000)
ship_stock[NAV_COMPUTER2].quantity = RangedRdm(0,2);

if (score>1000 || RangedRdm(1,100)>80)
ship_stock[REAR_LASER1].quantity = RangedRdm(1,4);
if (score>9000)
ship_stock[REAR_LASER2].quantity = RangedRdm(1,4);
if (score>50000)
ship_stock[REAR_LASER3].quantity = RangedRdm(0,4);

if (score>1000)
ship_stock[CANNON1].quantity = RangedRdm(1,4);

if (score>20000 || RangedRdm(1,100)>80)
ship_stock[CANNON2].quantity = RangedRdm(0,4);

if (score>50000)
ship_stock[CANNON3].quantity = RangedRdm(1,4);


if (score>50000 || RangedRdm(1,100)>80)
ship_stock[SHOCKWAVE1].quantity = RangedRdm(1,2);

if (score>50000)
ship_stock[SHOCKWAVE2].quantity = RangedRdm(1,2);

if (score>2000 || RangedRdm(1,100)>80)
ship_stock[ECM].quantity = RangedRdm(1,2);



//misc bits
if (score>10000 || RangedRdm(1,100)>80)
ship_stock[FAST_DOCK].quantity = RangedRdm(1,2);	//[F]

if (score>20000 || RangedRdm(1,100)>80)
ship_stock[MISSILE_FACTORY].quantity = RangedRdm(0,2);


ship_stock[FLATPACK].quantity = RangedRdm(1,5);

if (score>5000 || RangedRdm(1,100)>80)
ship_stock[TARGETTER_360].quantity = RangedRdm(1,2);

if (score>20000 || RangedRdm(1,100)>80)
ship_stock[EFFICIENT_ENGINE].quantity = RangedRdm(0,2);
ship_stock[RETRO_ROCKETS].quantity = RangedRdm(0,2);

if (score>50000 || RangedRdm(1,100)>80)
ship_stock[HYPERSPACE].quantity = RangedRdm(0,2);
ship_stock[NAV_COMPUTER1].quantity = RangedRdm(0,2);
ship_stock[BULLET_PROOF_EJECTION].quantity = RangedRdm(0,2);
ship_stock[SHIELDS1].quantity = RangedRdm(1,2);

if (score>1000 || RangedRdm(1,100)>80)
ship_stock[SHIELDS2].quantity = RangedRdm(0,2);
if (score>15000 || RangedRdm(1,100)>80)
ship_stock[SHIELDS3].quantity = RangedRdm(0,2);
if (score>30000 || RangedRdm(1,100)>80)
ship_stock[SHIELDS4].quantity = RangedRdm(0,2);
if (score>100000)
ship_stock[SHIELDS5].quantity = RangedRdm(0,2);

ship_stock[TRADING_COMPUTER1].quantity = RangedRdm(1,3);
if (score>9000)
ship_stock[TRADING_COMPUTER2].quantity = RangedRdm(1,3);
if (score>50000)
ship_stock[TRADING_COMPUTER3].quantity = RangedRdm(0,4);

////repairs
ship_stock[HULL_REPAIR].quantity = 1;

ship_stock[FUEL].quantity = RangedRdm(0,500);
ship_stock[REPAIR_SYSTEMS1].quantity = RangedRdm(0,2);
if (score>10000 || RangedRdm(1,100)>80)
ship_stock[REPAIR_SYSTEMS2].quantity = RangedRdm(0,2);

if (score>50000 || RangedRdm(1,100)>80)
ship_stock[REPAIR_SYSTEMS3].quantity = RangedRdm(0,2);

if (score>1000)
ship_stock[AUTO_HULL_REPAIR].quantity = RangedRdm(0,2);

    if (current_level>=2 && RangedRdm(1,100)>70)
	ship_stock[BOID_ADDITION].quantity = 1;


}

}


void setup_prices()
{

//setup prices
//these are based on a players credit of 5000 max.
//so at 5000 credits prices are most expensive
int i,temp_quantity;
float temp_credits,temp_price;
float discount=0,discountc,fprice,fdiff;
extern int credits;

temp_credits=credits;
if (temp_credits>50000) temp_credits=50000;
temp_credits/=200;	//between 0 and 200 - this is a percentage value
//go through array filling actual price
for (i=0;i<NUMBER_OF_GOODS;i++)
  {
   temp_price=ship_stock[i].basic_price;
//   add_price=temp_price*(temp_credits/100);

   ship_stock[i].actual_price=temp_price;
//calc discount
temp_quantity=ship_stock[i].quantity;

if (i==FUEL) temp_quantity/=10;

if (temp_quantity<2) discount=0.0;
else
if (temp_quantity<3) discount=.03;
else
if (temp_quantity<4) discount=.04;
else
if (temp_quantity<5) discount=.05;
else
if (temp_quantity<6) discount=.07;
else
if (temp_quantity<8) discount=.10;
else
if (temp_quantity<10) discount=.20;
else
if (temp_quantity<16) discount=.25;
else
if (temp_quantity<31) discount=.35;
else
if (temp_quantity<61) discount=.45;
else
if (temp_quantity<201) discount=.50;
else
if (temp_quantity<401) discount=.55;
else
if (temp_quantity<701) discount=.60;
else
if (temp_quantity>=701) discount=.65;

//difficulty=16 for easy, 8 for medium and 1 for hard
fdiff=difficulty;
fdiff/=100;	//percentage deduction

fprice=ship_stock[i].actual_price;

discountc=fprice*(discount+fdiff);	//proces go up as difficulty increases

fprice-=discountc;
ship_stock[i].actual_price=fprice;

   ship_stock[i].temp_removed=0;
  }

}



//Loads up the ships stock strings from resource STR#128
void load_up_stock_strings()
{
extern Str255 ship_items_strings[35];	//read from str resource 130

//Str255 short_string;

int i;

for (i=1;i<NUMBER_OF_GOODS+1; i++)
 {
  GetIndString((unsigned char*)&ship_stock[i-1].short_description,128,i);
  GetIndString((unsigned char*)&ship_stock[i-1].long_description,129,i);
//  GetIndString((unsigned char*)&ship_items_strings[i-1],130,i);	//in inventory
   
 }	//end of i


for (i=1;i<KLAST+1; i++)
 {
  GetIndString((unsigned char*)&ship_items_strings[i-1],130,i);	//in inventory
   
 }	//end of i


}

int other_buyers_timer;
int credits_timer;

//From 0.88 onwards we have simulated other buyers/sellers
void reset_other_users()
{
other_buyers_timer=TickCount()+120;
credits_timer=TickCount()+10*60;	//we dock 2 credits every ten seconds
how_many_users=RangedRdm(1,6);	//how many stocks to buy and sell

}


void run_other_users(int from_class)	//needs class to redisplay
{
extern kit_desc ship_items[ITEMS_COUNT];
extern int credits;
int buy_what,sell_what;
if (TickCount()>credits_timer)
{
int creds_dec;
     if (ship_items[KTRADING_COMPUTER].status==1)
     creds_dec=3;
     else
     creds_dec=5;
     
credits_timer=TickCount()+10*60;
if (credits>creds_dec) credits-=creds_dec;
else credits=0;
}


if (TickCount()<other_buyers_timer) return;
other_buyers_timer=TickCount()+(60*RangedRdm(1,10));
//now we can run
//buy something?

buy_what=RangedRdm(0,NUMBER_OF_GOODS-1);

if (buy_what!=FUEL)
{
   ship_stock[buy_what].quantity--;
}
else
   ship_stock[buy_what].quantity-=RangedRdm(30,200);

if (ship_stock[buy_what].quantity<0) ship_stock[buy_what].quantity=0;

//sell something?


 sell_what=RangedRdm(0,NUMBER_OF_GOODS-1);

 if (sell_what!=FUEL)
 {
 if (ship_stock[sell_what].quantity<10)
   ship_stock[sell_what].quantity++;
 } 
 else
   ship_stock[sell_what].quantity+=RangedRdm(30,200);
//reshow stock
setup_prices();
      display_goods(from_class);	//redraw and update display
    Show_it();



}


// -----------COMMODITIES PROCESSING ----------
hold_desc commodity_array[NUMBER_OF_COMMODITIES];


//simulate buyrs and sellers
void run_commodities()
{
extern kit_desc ship_items[ITEMS_COUNT];
extern int credits;
int buy_what,sell_what;
int max,base_price,i,temp,next_run_time;

if (TickCount()>credits_timer)
{
int creds_dec;
     if (ship_items[KTRADING_COMPUTER].status==1)
     creds_dec=3;
     else
     creds_dec=5;
     
credits_timer=TickCount()+10*60;
if (credits>creds_dec) credits-=creds_dec;
else credits=0;
}



if (TickCount()<other_buyers_timer) return;
next_run_time=33-how_many_users;
if (next_run_time<1) next_run_time=1;
other_buyers_timer=TickCount()+(20*next_run_time);

temp=(RangedRdm(1,100));

     if (temp>70)
    {
    how_many_users++;
    }
    else
    if (temp>40)
    {
    how_many_users--;
    }
    else {};
 if (how_many_users<0) how_many_users=0;
if (how_many_users>33) how_many_users=33;

/*
 if (difficulty==8) { if (how_many_users<5) how_many_users=5; }

 if (difficulty==1) { if (how_many_users<11) how_many_users=11; }
*/
    
if (how_many_users==0) return;	//"oops"


for (i=0;i<2;i++)	
{
//now we can run
//buy something?
buy_what=RangedRdm(0,NUMBER_OF_COMMODITIES);

//calc how many
base_price=commodity_array[buy_what].base_price;
max=5;	//in case I bugger it up...
//rarity based on price
if (base_price<11) max=200;
else
if (base_price<21) max=100;
else
if (base_price<31) max=80;
else
if (base_price<41) max=60;
else
if (base_price<50) max=40;
else
if (base_price<80) max=30;
else
if (base_price<100) max=20;
else
if (base_price<150) max=18;
else
if (base_price<200) max=15;
else
if (base_price<300) max=9;
else
if (base_price<500) max=7;
else
if (base_price>=500) max=3;



     commodity_array[buy_what].stock-=RangedRdm(1,max);

if (commodity_array[buy_what].stock<0) commodity_array[buy_what].stock=0;

//simulated sell something?


 sell_what=RangedRdm(0,NUMBER_OF_COMMODITIES);
base_price=commodity_array[sell_what].base_price;
max=5;	//in case I bugger it up...
//rarity based on price
if (base_price<11) max=200;
else
if (base_price<21) max=100;
else
if (base_price<31) max=80;
else
if (base_price<41) max=60;
else
if (base_price<50) max=40;
else
if (base_price<80) max=30;
else
if (base_price<100) max=20;
else
if (base_price<150) max=18;
else
if (base_price<200) max=13;
else
if (base_price<300) max=9;
else
if (base_price<500) max=7;
else
if (base_price>=500) max=3;


     commodity_array[sell_what].stock+=RangedRdm(1,max);	//sell max 50%
}	//end of for how_many

setup_commodity_stock_prices();

}





//draws stock with buy and sell prices
//25 lines per page, sorted by category
void display_commodities(LSRAW * exit_button)
{
extern int credits;
//extern kit_desc ship_items[ITEMS_COUNT];

GDHandle	saveGDH;
GrafPtr		savePort;
int wait,quit,button_state;
LSRAW *bottom_bar;
Handle bottom_bar_H;
LSRAW *bottom_bar1;
Handle bottom_bar1_H;
LSRAW *fwd;
Handle fwd_H;
LSRAW *back;
Handle back_H;
int item_selected;
Point mouse_xy;
int display_base_index;
real_time_struct time_struct;
int trading_open;
sds_attrib_data_type kit_data;


while (Button()==1);	//standard

 get_real_time_struct(&time_struct);
 //trade between 0800 and 1800 hours only
 if (time_struct.hours>18 || time_struct.hours<8) trading_open=0;
 else trading_open=1;
 

      get_kit_attrib_data(TRADINGCOMPV,&kit_data);
      if (kit_data.the_value==1)
      {
      LSRAW *dialog_picture;
      Handle dialog_H;
      
//           the_drawing_buffer=Get_back_buffer();
     GetPort (&savePort);
	saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices

        dialog_H=GetZexPicture ('RCZ ',2010);	//blank dialog box   
	HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;


      print_crunched_pict(dialog_picture,180+off_640_x,130+off_480_y);

      TextFont(21);
      TextSize(14);
      TextFace(bold);
      ZRGBForeColor(&rgbBlue);
//do text
      ZMoveTo (200+off_640_x,170+off_480_y);
      ZDrawString("\pYour Trading Computer is reducing ");
      ZMoveTo (200+off_640_x,190+off_480_y);
      ZDrawString("\pon-line charges to 18Cr per minute.");
      DisposeHandle(dialog_H);
	SetGDevice(saveGDH);	//these are here so I can SEE them!
	SetPort(savePort);
         Show_it();
     wait_for_mouse();

      }

         bottom_bar_H=GetZexPicture ('RCZ ',3400);	//computer   
	HLock(bottom_bar_H);
        bottom_bar = (LSRAW*)*bottom_bar_H;
         bottom_bar1_H=GetZexPicture ('RCZ ',3401);	//computer   
	HLock(bottom_bar1_H);
        bottom_bar1 = (LSRAW*)*bottom_bar1_H;

        fwd_H=GetZexPicture ('RCZ ',3402);  
	HLock(fwd_H);
        fwd = (LSRAW*)*fwd_H;
        
        back_H=GetZexPicture ('RCZ ',3403);  
	HLock(back_H);
        back = (LSRAW*)*back_H;
        

display_base_index=0;
//init_show_stock();	//reset prinitng coords
//     setup_commodity_stocks();
     while(Button()==1);
     
//     the_drawing_buffer=Get_back_buffer();
	RGBBackColor (&rgbBlack);
item_selected=-1;

quit=0;

while(quit==0)	//from exit button
{
//find out where mouse was when down
     wait=0;

//the loop     
 while (wait==0)	//while mouse isn't pressed
     {
     int can_buy;
//     the_drawing_buffer=Get_back_buffer();
     GetPort (&savePort);
	saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
      
      if (trading_open==1)
      run_commodities();	//others buy/sell!
      
      erase_zex_rect_colour(monitor_w,monitor_h,242);	    //in pictures

      draw_lines(trading_open);	//light/dark blue
      
      if (trading_open==1)
      draw_commodity_strings(display_base_index,item_selected);	//draw the strings from index 0
      else
      {
      ZRGBForeColor (&rgbWhite);
      ZMoveTo(220+off_640_x,197+off_480_y);
      ZDrawString("\pTradeNet available between 08:00 and 18:00 hours.");
      ZMoveTo(260+off_640_x,219+off_480_y);
      ZDrawString("\pPlease come back in the morning.");

      }
//if something is selected then show buy else buy blanked out
     if (item_selected==-1) can_buy=0;
     else can_buy=1;
     
     if (trading_open==0) can_buy=0;
     
     draw_bottom_bar(bottom_bar1,1,can_buy,0,1);
//draw the little fwd and back arrows
      if (trading_open==1)
      {
       if (display_base_index>0)
       print_crunched_trans_pict(back,602+off_640_x,412+off_480_y);
       if (display_base_index<NUMBER_OF_COMMODITIES-36)
       print_crunched_trans_pict(fwd,620+off_640_x,412+off_480_y);
      }
      
	SetGDevice(saveGDH);	//these are here so I can SEE them!
	SetPort(savePort);
      Show_it();
	SetGDevice(saveGDH);	//these are here so I can SEE them!
	SetPort(savePort);
      if (credits==0) break;

//wait for mouse down/up
     button_state=Button();
     if (button_state==1) wait=1;
     }
    //button up

    wait=0;
    while (wait==0)
    {
     button_state=Button();
     if (button_state==0) wait=1;
    }
   
//ok, mouse has gone down and up, decode it
      if (credits==0) break;
     ZexGetMouse (&mouse_xy);
     
     if (mouse_xy.v<(430+off_480_y))
     {
     //select an item or fwd back
       if (mouse_xy.v<(411+off_480_y))
          {
          int pos_y,old_item_selected;
           //select an item
           //convert to line number
           pos_y=mouse_xy.v-off_480_y;
           pos_y-=12;	//title
           pos_y=pos_y/11;
           old_item_selected=item_selected;
           item_selected=display_base_index+pos_y;
           if (old_item_selected==item_selected) item_selected=-1;	//toggle
          }
       else
         {
         //fwd back
         if (mouse_xy.v<(428+off_480_y))
           {
            if (mouse_xy.h>(602+off_640_x) && mouse_xy.h<(618+off_640_x) && display_base_index>0)
                  {
                  item_selected=-1;
                  display_base_index-=36;
                  if (display_base_index<0) display_base_index=0;
                  }
            else
            if (mouse_xy.h>(620+off_640_x) && mouse_xy.h<(636+off_640_x) && display_base_index<NUMBER_OF_COMMODITIES-36)
                  {
                  item_selected=-1;
                  display_base_index+=36;
                  if (display_base_index>NUMBER_OF_COMMODITIES-36) display_base_index=NUMBER_OF_COMMODITIES-36;
                  
                  }
           }
         }
     }
     else	//bottom bar
     {
     if (mouse_xy.h>(283+off_640_x) && mouse_xy.h<(358+off_640_x)) quit=1;
//     else
//     if (trading_open==0) quit=1;
     else
     if (mouse_xy.h>(529+off_640_x) && mouse_xy.h<(630+off_640_x)) draw_hold_items_docked(bottom_bar1,trading_open);	//inventory. returns on exit
     else
     if (mouse_xy.h>(358+off_640_x) && mouse_xy.h<(438+off_640_x) && item_selected!=-1 && trading_open==1) buy_cargo(item_selected);	   //inventory. returns on exit
     
     }
 
}
DisposeHandle(bottom_bar_H);
DisposeHandle(bottom_bar1_H);
DisposeHandle(back_H);
DisposeHandle(fwd_H);
}

void draw_bottom_bar(LSRAW*bar_pict,int can_exit, int can_buy, int can_sell, int can_inv)
{
extern int credits;
unsigned char number_string[12];
RGBColor	rgbDarkBlue = {0,0,0x3333};
RGBColor	rgbDarkRed = {0x3333,0,0};

      print_crunched_pict(bar_pict,0+off_640_x,430+off_480_y);
      
//show credits in bottom of screen
      TextFont(21);
      TextSize(14);
      TextFace(bold);
      ZRGBForeColor(&rgbBlue);
      ZMoveTo (10+off_640_x,454+off_480_y);
      ZDrawString("\pAvailable credit:");

       ZexNumToStringPascal(credits,number_string);
       ZMoveTo (130+off_640_x,454+off_480_y);
       ZDrawString(number_string);
//draw exit
       if (can_exit==1)
       ZRGBForeColor(&rgbRed);
       else
       ZRGBForeColor(&rgbDarkRed);
       
       ZMoveTo(305+off_640_x,455+off_480_y);
       ZDrawString("\pExit");
       
//draw buy
       if (can_buy==1)
       ZRGBForeColor(&rgbBlue);
       else
       ZRGBForeColor(&rgbDarkBlue);
       ZMoveTo(390+off_640_x,455+off_480_y);
       ZDrawString("\pBuy");

//draw sell
       if (can_sell==1)
      ZRGBForeColor(&rgbBlue);
       else
       ZRGBForeColor(&rgbDarkBlue);
	   
       ZMoveTo(470+off_640_x,455+off_480_y);
       ZDrawString("\pSell");
       
//draw inventory
       if (can_inv==1)
       ZRGBForeColor(&rgbBlue);
       else
       ZRGBForeColor(&rgbDarkBlue);
       ZMoveTo(547+off_640_x,455+off_480_y);
       ZDrawString("\pInventory");
       
       
//       TextFace(0);

}

void draw_lines(int trading_open)
{
extern int tutorial;
int i;
unsigned char number_string[12];
RGBColor	rgbDarkLightBlue	= { 0x0000, 0x9999, 0x9999 };
Rect temp_rect;

// non-constant initialisation
temp_rect.top=12+off_480_y; 
temp_rect.left=0+off_640_x;
temp_rect.bottom=23+off_480_y;
temp_rect.right=639+off_640_x;


TextFont(21);
TextSize(9);
TextFace(bold);
ZRGBForeColor(&rgbLightBlue);

ZMoveTo(2+off_640_x,9+off_480_y);
ZDrawString("\pDescription");

ZMoveTo(240+off_640_x,9+off_480_y);
ZDrawString("\pDofQ");

ZMoveTo(300+off_640_x,9+off_480_y);
ZDrawString ("\pCategory");

ZMoveTo(400+off_640_x,9+off_480_y);
ZDrawString ("\pCurrent Stock");

ZMoveTo(480+off_640_x,9+off_480_y);
ZDrawString("\pWe Buy At");

ZMoveTo(560+off_640_x,9+off_480_y);
ZDrawString("\pWe Sell At");

ZMoveTo(0+off_640_x,11+off_480_y);
ZLineTo (640+off_640_x,11+off_480_y);
for (i=0;i<36;i++)
  {
  int colour;
//  if (i&1==1) RGBForeColor(&LightBlue);
//   else RGBForeColor(&DarkBlue);
  if ((i&1)==1) colour=239;
   else colour=204;
//  PaintRect(&temp_rect);
  dlp_draw_box(temp_rect.top,temp_rect.left,temp_rect.bottom,temp_rect.right,colour);
  temp_rect.top+=11; temp_rect.bottom+=11;
  }

draw_vert_lines();

//show how many users
TextFont(21);
TextSize(9);
TextFace(0);

if (trading_open==1)
{
ZRGBForeColor(&rgbDarkLightBlue);
ZMoveTo(2+off_640_x,420+off_480_y);
if (training_mission!=0 || tutorial==1)
ZDrawString("\pCurrent simulated system load: ");
else
ZDrawString("\pCurrent system load: ");
     ZexNumToStringPascal(how_many_users+1,number_string);
     ZDrawString(number_string);
     if (how_many_users+1>1)
     ZDrawString ("\p users.");
     else
     ZDrawString ("\p user.");
}

      
TextFace(0);

}

void draw_vert_lines(void)
{
//draw vert lines
ZRGBForeColor(&rgbWhite);

ZMoveTo(238+off_640_x,0+off_480_y);
ZLineTo(238+off_640_x,408+off_480_y);
ZMoveTo(298+off_640_x,0+off_480_y);	//cat
ZLineTo(298+off_640_x,408+off_480_y);

ZMoveTo(398+off_640_x,0+off_480_y);
ZLineTo(398+off_640_x,408+off_480_y);

ZMoveTo(478+off_640_x,0+off_480_y);
ZLineTo(478+off_640_x,408+off_480_y);

ZMoveTo(558+off_640_x,0+off_480_y);
ZLineTo(558+off_640_x,408+off_480_y);
//bottom H line
ZMoveTo(0+off_640_x,408+off_480_y);
ZLineTo(639+off_640_x,408+off_480_y);
ZMoveTo(0+off_640_x,0+off_480_y);
ZLineTo(639+off_640_x,0+off_480_y);
ZMoveTo(0+off_640_x,0+off_480_y);
ZLineTo(0+off_640_x,408+off_480_y);
ZMoveTo(639+off_640_x,0+off_480_y);
ZLineTo(639+off_640_x,408+off_480_y);
}

void draw_commodity_strings(int from_index,int selected_item)	//draw 35 lines from index
{
extern int tutorial;
int i,py;
unsigned char temp_string[100];
int temp;
unsigned char number_string[12];

TextFont(21);
TextSize(9);
TextFace(0);
ZRGBForeColor(&rgbLightBlue);
py=21;
if (training_mission!=0 || tutorial==1)
   {
    ZMoveTo(off_640_x+245,420+off_480_y);
    ZRGBForeColor(&rgbRed);
    ZDrawString("\p*Simulated stock for training purposes*");
   }
   
for (i=from_index;i<from_index+36;i++)
  {
  if (i==NUMBER_OF_COMMODITIES-1) break;	//don't do an "oops"
  
  if (i==selected_item)
  {
    //fill selected line in red
    dlp_draw_box(py+off_480_y-9,1+off_640_x,py+off_480_y+2, 639+off_640_x, 221);
	draw_vert_lines();
  }
  
  if (commodity_array[i].stock<1)
    ZRGBForeColor(&rgbRed);
  else
  if (commodity_array[i].stock<10)
    ZRGBForeColor(&rgbOrange);
  else
  if (commodity_array[i].stock>150)
    ZRGBForeColor(&rgbYellow);
  else
    ZRGBForeColor(&rgbLightBlue);
    
  
  ZMoveTo(off_640_x+6,py+off_480_y);
  CToPascal(commodity_array[i].string,(char *)temp_string);

  ZDrawString(temp_string);  
  
  if ((commodity_array[i].MFR & 2048) ==2048)
  ZDrawString ("\p (Illegal)");
  
//dofq
  ZMoveTo(242+off_640_x,py+off_480_y);
  temp=  commodity_array[i].dofq;
  if (temp==1)
  ZDrawString("\pTonne");  
  else
  if (temp==2)
  ZDrawString("\pBarrel");  
  else
  if (temp==3)
  ZDrawString("\pCase");  
  else
  if (temp==4)
  ZDrawString("\pKilogramme");  


//category
  ZMoveTo(302+off_640_x,py+off_480_y);
  temp=  commodity_array[i]._class;
  if (temp==10)
  ZDrawString("\pMaterials");  
  else
  if (temp==11)
  ZDrawString("\pFoodstuffs");  
  else
  if (temp==12)
  ZDrawString("\pTech.");  
  else
  if (temp==13)
  ZDrawString("\pLuxury/Textiles");  
  else
  if (temp==14)
  ZDrawString("\pFirearms");  
  else
  if (temp==15)
  ZDrawString("\pDrinks");  
  else
  if (temp==16)
  ZDrawString("\pLivestock");  
//stock
  ZMoveTo(402+off_640_x,py+off_480_y);
     ZexNumToStringPascal(commodity_array[i].stock,number_string);
     ZDrawString(number_string);
     if (commodity_array[i].stock!=0)
     
    {
     if (commodity_array[i].stock==1)
     {
     if (commodity_array[i].dofq==1) ZDrawString ("\p tonne");
     else
     if (commodity_array[i].dofq==2) ZDrawString ("\p barrel");
     else
     if (commodity_array[i].dofq==3) ZDrawString ("\pcase");
     else
     if (commodity_array[i].dofq==4) ZDrawString ("\p kilogramme");
     }

     else //more than 1
     
     {
     if (commodity_array[i].dofq==1) ZDrawString ("\p tonnes");
     else
     if (commodity_array[i].dofq==2) ZDrawString ("\p barrels");
     else
     if (commodity_array[i].dofq==3) ZDrawString ("\p cases");
     else
     if (commodity_array[i].dofq==4) ZDrawString ("\p kilogrammes");
     }
    }
//buy price
  ZMoveTo(482+off_640_x,py+off_480_y);
     ZexNumToStringPascal(commodity_array[i].buy_price,number_string);
     ZDrawString(number_string);

//sell price
  ZMoveTo(562+off_640_x,py+off_480_y);
  if (commodity_array[i].stock==0)
     ZDrawString("\p-");
     else
     {
     ZexNumToStringPascal(commodity_array[i].sell_price,number_string);
     ZDrawString(number_string);
     }
     
  py+=11;

  }
//draw vert lines
ZRGBForeColor(&rgbWhite);
TextMode(1);

}



short allegiance_to_bit[]={1,2,4,8,16,32,64,128,256,512,1024,2048};

void setup_commodity_stocks()
{
extern hold_desc cargos_array[NUMBER_OF_COMMODITIES];
int i,manufacturer,supply_rating;
int docked_at_allegiance_bit;
int ms_allegiance;

init_zex_random();	//good time to do this...

ms_allegiance=docked_at_allegiance;

docked_at_allegiance_bit=allegiance_to_bit[ms_allegiance];

//copy the base stock strings to commodity_array
     for (i=0;i<NUMBER_OF_COMMODITIES;i++)
     commodity_array[i]=cargos_array[i];	//who sed C wasn't a high level language???

supply_rating=mship_supply_rating;
supply_rating/=10;
if (supply_rating<1) supply_rating=1;

for (i=0;i<NUMBER_OF_COMMODITIES;i++)
{
 int max,base_price;
 base_price=commodity_array[i].base_price;
 max=5;	//in case I bugger it up...
 //rarity based on price
 if (base_price<11) max=40*supply_rating;
 else
 if (base_price<21) max=30*supply_rating;
 else
 if (base_price<31) max=20*supply_rating;
 else
 if (base_price<41) max=15*supply_rating;
 else
 if (base_price<50) max=12*supply_rating;
 else
 if (base_price<80) max=9*supply_rating;
 else
 if (base_price<100) max=7*supply_rating;
 else
 if (base_price<150) max=5*supply_rating;
 else
 if (base_price<200) max=4*supply_rating;
 else
 if (base_price<300) max=3*supply_rating;
 else
 if (base_price<500) max=2*supply_rating;
 else
 if (base_price>=500) max=1*supply_rating;
 
 
 if (max!=0)
 commodity_array[i].stock=RangedRdm(0,max);
 else
 commodity_array[i].stock=0;
 
 manufacturer=(commodity_array[i].MFR); //this is a bitmap
 
/*
 //if it's illegal (bit 3 set) then the chances are there won't be any
 if ((commodity_array[i].MFR & 8)==8)	  //illegal
 {
   if (docked_at_allegiance==0)	//earth mship
   {
   if (RangedRdm(0,100)<80)
     commodity_array[i].stock=0;
   }
   else	//alien mship
   if (RangedRdm(0,100)<40)
     commodity_array[i].stock=0;
  
}
*/
/*

if (manufacturer&docked_at_allegiance_bit) //this race manufactures this item?
    {
     float fstock;
     //add on 10%
     fstock=commodity_array[i].stock;
     fstock+=fstock*0.1;
     commodity_array[i].stock=fstock;
    }
    else	//subtract 20%
    {
     float fstock;
     //add on 10%
     fstock=commodity_array[i].stock;
     fstock-=fstock*0.2;
     if (fstock<0) fstock=0;
     commodity_array[i].stock=fstock;
    
    }
*/
}
//calc buy and sell price
setup_commodity_stock_prices();

}


void setup_commodity_stock_prices()
{
int i,temp_quantity;
float basic_price,buy_price,sell_price,cut;
float bulk_discount=0,fmarkup,supply_rating_multiplier;
int manufacturer;


supply_rating_multiplier=(100-mship_supply_rating)/33+1;
supply_rating_multiplier/=2;	//range 0.5 to 2

if (supply_rating_multiplier<=0.5) supply_rating_multiplier=0.5;	//just in case i've miscalculated
if (supply_rating_multiplier>=2) supply_rating_multiplier=2.0;	//just in case i've miscalculated

for (i=0;i<NUMBER_OF_COMMODITIES;i++)
  {
    
    basic_price=commodity_array[i].base_price;
    
if (difficulty==8) basic_price+=basic_price*0.1;
else
if (difficulty==1) basic_price+=basic_price*0.3;

basic_price*=supply_rating_multiplier;
//do planet type adjustment
//is this an agri or tech stock or neither?
//1 to 5 is agri, 6 to 10 is tech - 5 is a big agri, 10 is a big tech, 1 and 6 are neutral
if(commodity_array[i]._class==ORES || commodity_array[i]._class==FOOD
   || commodity_array[i]._class==DRINKS || commodity_array[i]._class==LIVESTOCK)
   {
     if (mship_supply_type<6) basic_price-=basic_price*(mship_supply_type)/20;
   }
   
   if (mship_supply_type==1 && commodity_array[i]._class==LUXURY)    //neutral agri
   {
    basic_price-=basic_price*0.2; //20% off luxury goods on a neutral agri
   }

if(commodity_array[i]._class==TECH || commodity_array[i]._class==LUXURY
   || commodity_array[i]._class==FIREARMS)
   {
   if (mship_supply_type>=6) basic_price-=basic_price*(mship_supply_type-5)/20;
   }
   
//calc bulk_discount which ups the buy and drops the sell price
temp_quantity=commodity_array[i].stock;
if (temp_quantity<10) bulk_discount=-.09;	//up the price!
else
if (temp_quantity<20) bulk_discount=.05;
else
if (temp_quantity<31) bulk_discount=.07;
else
if (temp_quantity<61) bulk_discount=.09;
else
if (temp_quantity<101) bulk_discount=.11;
else
if (temp_quantity<141) bulk_discount=.13;
else
if (temp_quantity<200) bulk_discount=.15;
else
if (temp_quantity>=200) bulk_discount=.20;

//if manufacturer is same as this allegiance then dec price by 10%

manufacturer=commodity_array[i].MFR & 3;

    if (manufacturer!=3) //made in both galaxies
     {
     if (commodity_array[i].MFR-1==docked_at_allegiance)	//1 is earth, 2 is alien, in code 0 is earth, 1 is alien
          basic_price-=basic_price*0.1;
     }
//    cut=basic_price*0.20;
    fmarkup=commodity_array[i].markup;
    fmarkup/=100;
    cut=basic_price*fmarkup;
    
    bulk_discount=basic_price*bulk_discount;
    
    buy_price=(basic_price-cut)-bulk_discount;
    sell_price=(basic_price+cut)-bulk_discount;
    
    if (buy_price==sell_price) sell_price+=1;	//will happen
    commodity_array[i].buy_price=buy_price;
    commodity_array[i].sell_price=sell_price;

    
  }
}



//show items in hold and trailer (if fitted)
void draw_hold_items_docked(LSRAW* bar_pict, int trading_open)
{
extern hold_desc hold_items[MAX_HOLD_ITEMS];

//GDHandle	saveGDH;
//GrafPtr		savePort;
Point mouse_xy;
int quit,wait,button_state;
int item_selected;

item_selected=-1;
quit=0;
while (Button()==1);

while(quit==0)
{
//loop
  wait=0;
  while(wait==0)
  {
//     the_drawing_buffer=Get_back_buffer();
//     GetPort (&savePort);
//	saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//
//	EraseRect(&the_drawing_buffer->portRect);
	draw_lines(trading_open);
	draw_inventory_strings(0,item_selected);
	if (item_selected==-1 || trading_open==0)
        draw_bottom_bar(bar_pict,1,0,0,0);
	else
        draw_bottom_bar(bar_pict,1,0,1,0);	//enable sell
        
        if (trading_open==1)
        run_commodities();	  //others buy/sell!
	
        Show_it();
     button_state=Button();
     if (button_state==1) wait=1;
     }
    //button up

    wait=0;
    while (wait==0)
    {
     button_state=Button();
     if (button_state==0) wait=1;
    }

        ZexGetMouse (&mouse_xy);
     
//	wait_for_mouse();	//wait for mouse button

     if (mouse_xy.v<(430+off_480_y) && trading_open==1)
     {
     //select an item or fwd back
       if (mouse_xy.v<(411+off_480_y))
          {
          int pos_y,old_item_selected;
           //select an item
           //convert to line number
           pos_y=mouse_xy.v-off_480_y;
           pos_y-=12;	//title
           pos_y=pos_y/11;
           old_item_selected=item_selected;
           item_selected=pos_y;
           if (hold_items[item_selected].id==-1) item_selected=-1;	//nothing there
           if (old_item_selected==item_selected) item_selected=-1;	//toggle
          }

     }
     else	//mouse on bar
     {
     if (mouse_xy.h>(283+off_640_x) && mouse_xy.h<(358+off_640_x)) 
       {
       item_selected=-1;
       quit=1;
       }
     else
     if (mouse_xy.h>(440+off_640_x) && mouse_xy.h<(525+off_640_x) && item_selected!=-1) 
       {
       //sell
       sell_cargo(item_selected);
       item_selected=-1;
       }
     
     }
}	//end of while quit


while (Button()==1);

}





void draw_inventory_strings(int from_index, int selected_item)
{
extern hold_desc cargos_array[NUMBER_OF_COMMODITIES];
int py;
SInt8 i;

unsigned char temp_string[100];
int temp,hold_slot;
unsigned char number_string[12];
//extern hold_desc hold_items[MAX_HOLD_ITEMS];

TextFont(21);
TextSize(9);
TextFace(0);
ZRGBForeColor(&rgbLightBlue);

py=21;

for (hold_slot=from_index; hold_slot<player_kit.cargo_capacity_tonnes; hold_slot++)
  {
//  if (i==NUMBER_OF_COMMODITIES-1) break;	  //don't do an "oops"
  i=player_kit.cargo_bay[hold_slot];
  
  //i=hold_items[hold_slot].id;	//get id of item at this slot
  

  if (hold_slot==selected_item)
  {
	      //fill selected line in red
    dlp_draw_box(py+off_480_y-9,1+off_640_x,py+off_480_y+2, 639+off_640_x, 221);
	draw_vert_lines();
  } 
  
  ZMoveTo(6+off_640_x,py+off_480_y);
  
  if (i!=-1)
  {
  CToPascal(cargos_array[i].string,(char *)temp_string);

  ZDrawString(temp_string);  
  }
  else
  ZDrawString("\p****Empty****");  

  if (i!=-1)
  {  
//dofq
  ZMoveTo(242+off_640_x,py+off_480_y);
  temp=  cargos_array[i].dofq;
  if (temp==1)
  ZDrawString("\pTonne");  
  else
  if (temp==2)
  ZDrawString("\pBarrel");  
  else
  if (temp==3)
  ZDrawString("\pCase");  
  else
  if (temp==4)
  ZDrawString("\pKilogramme");  


//category
  ZMoveTo(302+off_640_x,py+off_480_y);
  temp=  cargos_array[i]._class;
  if (temp==10)
  ZDrawString("\pMaterials");  
  else
  if (temp==11)
  ZDrawString("\pFoodstuffs");  
  else
  if (temp==12)
  ZDrawString("\pTech.");  
  else
  if (temp==13)
  ZDrawString("\pLuxury/Textiles");  
  else
  if (temp==14)
  ZDrawString("\pFirearms");  
  else
  if (temp==15)
  ZDrawString("\pDrinks");  
  else
  if (temp==16)
  ZDrawString("\pLivestock");  
//stock
  ZMoveTo(400+off_640_x,py+off_480_y);
     ZexNumToStringPascal(commodity_array[i].stock,number_string);
     ZDrawString(number_string);

//buy price
  ZMoveTo(482+off_640_x,py+off_480_y);
     ZexNumToStringPascal(commodity_array[i].buy_price,number_string);
     ZDrawString(number_string);

//sell price
  ZMoveTo(562+off_640_x,py+off_480_y);
     ZexNumToStringPascal(commodity_array[i].sell_price,number_string);
     ZDrawString(number_string);
  }	//end of not -1
  py+=11;
    }	  //end of for
//draw vert lines
ZRGBForeColor(&rgbWhite);
//TextMode(1);

}


//this is (basically) a paste of do_yer_actual_sell
//time, time, time...
void sell_cargo(int item_selected)
{
extern int credits;
extern GDHandle				mZexGDH;
LSRAW *dialog_picture;
Handle dialog_H;
GDHandle	saveGDH;
GrafPtr		savePort;
unsigned char number_string[12];
unsigned char temp_string[300];	//how long is a piece of string????
Point mouse_xy;
int buyers_price;
int tc_tries,tc_max_tries;
int commodity_id;
//extern hold_desc hold_items[MAX_HOLD_ITEMS];
extern kit_desc ship_items[ITEMS_COUNT];


//translate ship item to stock item
commodity_id=player_kit.cargo_bay[item_selected];
//hold_items[item_selected].id;

//stock_item_id=translate_ship_to_stock(ship_item_id);

if (commodity_id==-1)             report_error("sell_cargo translate error. Pooh!","\p",commodity_id);

//calc tc_max_tries
tc_tries=0;
tc_max_tries=ship_items[KTRADING_COMPUTER].modifier*5;
     
//rehaggle:	//err, a label!
//     the_drawing_buffer=Get_back_buffer();

     GetPort (&savePort);
	 saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
// 	SetPort((GrafPtr)the_drawing_buffer);
	 SetGDevice(mZexGDH);

//     if (ship_items[KTRADING_COMPUTER].status==1)
//     dialog_H=GetZexPicture ('RCZ ',2011);	    //haggle dialog box
//     else
//     dialog_H=GetZexPicture ('RCZ ',2012);	  //confirm dialog box
//        
//	  HLock(dialog_H);
//	  dialog_picture = (LSRAW*)*dialog_H;
//
////accept price or negatiate?
//
////    if (Button()==1);	//wait for mouse up
//    
////    display_goods(screen_class);
//
//	  print_crunched_pict(dialog_picture,180,130);
//    TextFont(21);
//TextSize(12);
//TextFace(normal);
//RGBForeColor(&rgbBlue);
//     MoveTo(426,150);
//     NumToString(commodity_array[commodity_id].buy_price,number_string);
//     DrawString("\pCr ");
//     DrawString(number_string);
//     CToPascal(commodity_array[commodity_id].string,temp_string);
//     MoveTo(200,150);
//     DrawString((unsigned char *) temp_string);
////if we have a trading computer calculate price
////     if (ship_items[KTRADING_COMPUTER].status==1)
////     {
////     float buying_price,percent;
////     buying_price=commodity_array[commodity_id].buy_price;
////
////     percent=buying_price/RangedRdm(30,60);	//between 40 and 60% off
////     buying_price=buying_price-percent;
////     
////     buyers_price=do_trading_computer(buying_price, stock_item_id);
////
////     MoveTo (200,220);
////	     DrawString ("\pTrading Computer gets a price of ");
////	     NumToString(buyers_price,number_string);
////     DrawString(number_string);
////     }
////     else	  //no trading computer
//     {
//      float temp_price,fdiscount;
       buyers_price=commodity_array[commodity_id].buy_price;
////      temp_price=buyers_price;
////      fdiscount=.40;
////      temp_price=temp_price-(temp_price*fdiscount);
////      if (temp_price<1) temp_price=1;	//dont want credits!     
////      buyers_price=temp_price;
//     }
//     
//
//     MoveTo (200,240);
//     if (ship_items[KTRADING_COMPUTER].status==1)
//     DrawString("\pAccept the offer, re-negotiate or cancel?");
//     else
//     {
//     DrawString("\pAccept the offer of ");
//	   NumToString(buyers_price,number_string);
//     DrawString(number_string);
//     DrawString("\p credits or cancel?");
//     
//     }
// 	SetGDevice(saveGDH);	//these are here so I can SEE them!
//	  SetPort(savePort);
//   Show_it();
//   DisposeHandle(dialog_H);
//     wait_for_mouse();
//
////bottom 20, right 30 is exit
//     ZexGetMouse (&mouse_xy);
////     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
// 
////have a trading computer?
//     if (ship_items[KTRADING_COMPUTER].status==1)
//     {
//     if (mouse_xy.h>380 && mouse_xy.h<480 && mouse_xy.v>250 && mouse_xy.v<280) return;	//cancel
//     if (mouse_xy.h>280 && mouse_xy.h<380 && mouse_xy.v>250 && mouse_xy.v<280) 
//        {
//          //check if exceeeded number of tries
//          play_zsound_always(sound_test, sound_high_pri, c1_chan1, sound_vol_7);
//          goto rehaggle;	  //recalc
//        }
//     }
//     
//     else
//     if (mouse_xy.h>320 && mouse_xy.h<480 && mouse_xy.v>256 && mouse_xy.v<278) return;	//cancel
//     if (mouse_xy.h>180 && mouse_xy.h<320 && mouse_xy.v>256 && mouse_xy.v<278)
//     goto buy_it;
//     else
//     {
//        play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
//        goto rehaggle;	//do nothing
//     }
////must be OK!

   
//buy_it:   
        dialog_H=GetZexPicture ('RCZ ',2010);	//blank dialog box   
	HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;

//bottom 20, right 30 is exit
     ZexGetMouse (&mouse_xy);
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);

//show you have bought
       GetPort (&savePort);
	 saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
 
//     the_drawing_buffer=Get_back_buffer();
//	SetPort((GrafPtr)the_drawing_buffer);
	 SetGDevice(mZexGDH);

	print_crunched_pict(dialog_picture,180+off_640_x,130+off_480_y);

      TextFont(21);
TextSize(14);
TextFace(bold);
ZRGBForeColor(&rgbBlue);
//do text
     ZMoveTo (200+off_640_x,170+off_480_y);
     
      {
     ZDrawString("\pYou have sold ");
 	   ZDrawString("\pthe");
       ZMoveTo (200+off_640_x,188+off_480_y);
     CToPascal(commodity_array[commodity_id].string,(char *)temp_string);
     ZDrawString (temp_string);	//cast this, cast that...
     ZDrawString ("\p for ");
     ZexNumToStringPascal(buyers_price, number_string);
     ZMoveTo (200+off_640_x,206+off_480_y);
     ZDrawString(number_string);
     ZDrawString ("\p credits.");
      }
    
     credits=credits+buyers_price;
     player_kit.cargo_bay[item_selected]=-1;
     commodity_array[commodity_id].stock++;
//     hold_items[item_selected].id=-1;
   
	SetGDevice(saveGDH);	//these are here so I can SEE them!
	SetPort(savePort);
    Show_it();

    DisposeHandle(dialog_H);
    wait_for_mouse();

}



void buy_cargo(int item_selected)
{
extern int credits;
LSRAW *dialog_picture;
Handle dialog_H;
unsigned char number_string[12];
unsigned char temp_string[300];	//how long is a piece of string????
Point mouse_xy;
int buyers_price;
int commodity_id,hold_slot,i;
//extern hold_desc hold_items[MAX_HOLD_ITEMS];
int tc_tries,tc_max_tries;
sds_attrib_data_type kit_data;

//translate ship item to stock item
//commodity_id=hold_items[item_selected].id;
commodity_id=item_selected;

//stock_item_id=translate_ship_to_stock(ship_item_id);

if (commodity_id==-1)             report_error("buy_cargo translate error. Rats!","\p",commodity_id);


//find a hold slot
hold_slot=-1;
//can we find an empty slot in the hold?
for (i=0;i<player_kit.cargo_capacity_tonnes;i++)
    {
    if (player_kit.cargo_bay[i]==-1) 
     {
      hold_slot=i;
      break;
     }
    }

if (hold_slot==-1) 
 {
	do_string_dialog("\pYou have no space for the item.");
 return;	//all slots full
 }


if (commodity_array[commodity_id].stock==0)
 {
	do_string_dialog("\pThat item is not in stock.");
 return;	//all slots full
 }

if (credits<commodity_array[commodity_id].sell_price)
 {
	do_string_dialog("\pYou can't afford the item.");
 return;	//all slots full
 }

 
//calc tc_max_tries
tc_tries=0;
      get_kit_attrib_data(TRADINGCOMPV,&kit_data);
tc_max_tries=kit_data.the_value*5;
     
rehaggle:	//err, a label!
//     the_drawing_buffer=Get_back_buffer();
//
//     GetPort (&savePort);
//	   saveGDH = GetGDevice();	  //All calls to QD are surrounded by save and set gdevices
//// 	  SetPort((GrafPtr)the_drawing_buffer);
//	   SetGDevice(mZexGDH);
//
//     if (kit_data.the_value>0)
//     dialog_H=GetZexPicture ('RCZ ',2011);	    //haggle dialog box
//     else
     dialog_H=GetZexPicture ('RCZ ',2012);	  //confirm dialog box
        
	  HLock(dialog_H);
	  dialog_picture = (LSRAW*)*dialog_H;

//accept price or negatiate?

//    if (Button()==1);	//wait for mouse up
//    
////    display_goods(screen_class);

	  print_crunched_pict(dialog_picture,180+off_640_x,130+off_480_y);
     TextFont(21);
     TextSize(12);
     TextFace(normal);
     ZRGBForeColor(&rgbBlue);
     ZMoveTo(426+off_640_x,150+off_480_y);
     NumToString(commodity_array[commodity_id].sell_price,number_string);
     ZDrawString("\pCr ");
     ZDrawString(number_string);
     CToPascal( commodity_array[commodity_id].string,(char*)temp_string);
     ZMoveTo(200+off_640_x,150+off_480_y);
     ZDrawString((unsigned char *) temp_string);
//if we have a trading computer calculate price
     if (kit_data.the_value>0)
     {
     float buying_price;
     buying_price=commodity_array[commodity_id].buy_price;

//     percent=buying_price/RangedRdm(30,60);	//between 40 and 60% off
//     buying_price=buying_price-percent;
     
     buyers_price=do_trading_computer(buying_price, commodity_id);

     ZMoveTo (200+off_640_x,220+off_480_y);
	     ZDrawString ("\pTrading Computer gets a price of ");
	     NumToString(buyers_price,number_string);
     ZDrawString(number_string);
     }
     else	  //no trading computer
    {
      float temp_price,fdiscount;
      buyers_price=commodity_array[commodity_id].sell_price;
      temp_price=buyers_price;
      fdiscount=.40;
     temp_price=temp_price-(temp_price*fdiscount);
      if (temp_price<1) temp_price=1;	//dont want credits!     
      buyers_price=temp_price;
    }
     

    ZMoveTo (200+off_640_x,240+off_480_y);
//     if (kit_data.the_value>0)
//     ZDrawString("\pAccept the price, re-negotiate or cancel?");
//     else
//     {
     ZDrawString("\pAccept the price of ");
	   NumToString(buyers_price,number_string);
     ZDrawString(number_string);
     ZDrawString("\p credits or cancel?");
     
//     }
// 	SetGDevice(saveGDH);	//these are here so I can SEE them!
//	  SetPort(savePort);
   Show_it();
   DisposeHandle(dialog_H);
     wait_for_mouse();

//bottom 20, right 30 is exit
     ZexGetMouse (&mouse_xy);
     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
 
/*
//have a trading computer?
     if (kit_data.the_value>0)
     {
     if (mouse_xy.h>(380+off_640_x) && mouse_xy.h<(480+off_640_x) && mouse_xy.v>(250+off_480_y) && mouse_xy.v<(280+off_480_y)) return;	//cancel
     if (mouse_xy.h>(280+off_640_x) && mouse_xy.h<(380+off_640_x) && mouse_xy.v>(250+off_480_y) && mouse_xy.v<(280+off_480_y)) 
        {
          //check if exceeeded number of tries
          play_zsound_always(sound_test, sound_high_pri, c1_chan1, sound_vol_7);
          goto rehaggle;	  //recalc
        }
     }
     
     else
*/
     if (mouse_xy.h>(320+off_640_x) && mouse_xy.h<(480+off_640_x) && mouse_xy.v>(256+off_480_y) && mouse_xy.v<(278+off_480_y)) return;	//cancel
     if (mouse_xy.h>(180+off_640_x) && mouse_xy.h<(320+off_640_x) && mouse_xy.v>(256+off_480_y) && mouse_xy.v<(278+off_480_y))
     goto buy_it;
     else
     {
        play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
        goto rehaggle;	//do nothing
     }
//must be OK!

   
buy_it:   
        dialog_H=GetZexPicture ('RCZ ',2010);	//blank dialog box   
	HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;

//bottom 20, right 30 is exit
     ZexGetMouse (&mouse_xy);
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);

//show you have bought
//       GetPort (&savePort);
//	 saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
 
//     the_drawing_buffer=Get_back_buffer();
//	SetPort((GrafPtr)the_drawing_buffer);/
//	 SetGDevice(mZexGDH);

	print_crunched_pict(dialog_picture,180+off_640_x,130+off_480_y);

      TextFont(21);
TextSize(14);
TextFace(bold);
ZRGBForeColor(&rgbBlue);
//do text
     ZMoveTo (200+off_640_x,170+off_480_y);
     
      {
     ZDrawString("\pYou have bought ");
 	   ZDrawString("\pthe");
       ZMoveTo (200+off_640_x,188+off_480_y);
     CToPascal(commodity_array[commodity_id].string,(char *)temp_string);
     ZDrawString (temp_string);	//cast this, cast that...
     ZDrawString ("\p for ");
     ZexNumToStringPascal(buyers_price, number_string);
     ZMoveTo (200+off_640_x,206+off_480_y);
     ZDrawString(number_string);
     ZDrawString ("\p credits.");
      }
    
     credits=credits-buyers_price;
     commodity_array[commodity_id].stock--;
     player_kit.cargo_bay[hold_slot]=commodity_id;	//transfer to players hold
//     hold_items[hold_slot].id=commodity_id;
   
//	SetGDevice(saveGDH);	//these are here so I can SEE them!
//	SetPort(savePort);
    Show_it();

    DisposeHandle(dialog_H);
    wait_for_mouse();

}


void do_string_dialog(unsigned char * the_string)
{
LSRAW *dialog_picture;
Handle dialog_H;
GDHandle	saveGDH;
GrafPtr		savePort;

        dialog_H=GetZexPicture ('RCZ ',2010);	     //blank dialog box   
	HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;

       GetPort (&savePort);
	 saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
 
//     the_drawing_buffer=Get_back_buffer();
//	SetPort((GrafPtr)the_drawing_buffer);
	 SetGDevice(mZexGDH);

	print_crunched_pict(dialog_picture,180+off_640_x,130+off_480_y);

      TextFont(21);
      TextSize(14);
      TextFace(bold);
      ZRGBForeColor(&rgbBlue);
//do text
     ZMoveTo (200+off_640_x,170+off_480_y);
     
      {
     ZDrawString(the_string);
      }
    
   
	SetGDevice(saveGDH);	//these are here so I can SEE them!
	SetPort(savePort);
    Show_it();
    wait_for_mouse();
    DisposeHandle(dialog_H);

}