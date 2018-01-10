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
// * FILENAME           : cargo.cpp
// * MODULE TITLE       : Untitled 1
// * PROJECT            : Zex
// * DATE STARTED       : 03:45:11 on 11th August 1999
// * FIRST AUTHOR       : Stu
// *
// * COPYRIGHT (c)        
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************
/*
 * $Log: cargo.cpp,v $
 * Revision 1.2  2003/09/20 12:56:57  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:35:59  stu_c
 * First Imported.
 *
 * Revision 1.9  2002/09/08 02:04:03  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.8  2002/07/28 17:13:29  stu_c
 * Exhausts and further particle work
 *
 * Revision 1.7  2002/05/05 22:27:30  stu
 * Further developement - hull temperature now correct for multiple suns and atmosphere.

All calls to add_to_kill_list check the return
 *
 * Revision 1.6  2002/03/16 00:44:24  stu
 * Continued development
 *
 * Revision 1.5  2002/02/16 19:02:54  stu
 * Radar range decrease key and rationalisation of dlp colours
 *
 * Revision 1.4  2002/01/09 23:48:52  stu
 * Furhter work on trading.
 *
 * Revision 1.3  2001/11/02 19:31:23  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:28:50  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:01:27  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:06  rob
 * First Imported.
 *
 * Revision 1.4  2001/06/23 20:08:26  stu
 * 0.66 checkin
 *
 * Revision 1.3  2001/06/03 03:18:33  stu
 * OCB structure now contains a sub structure called:
ZObjectControl
Which contains all the control data for an object.

Added accessor in access_ocbs which returns a pointer to type ZObjectControl
 *
 * Revision 1.2  2001/04/01 18:27:04  stu
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2001/01/01 21:13:38  rob
 * First Imported.
 *
 * Revision 1.2  2000/11/16 20:38:11  rob
 * orbit
 *
 * Revision 1.1  2000/09/20 23:47:47  stu
 * HUD/AP/Scaling
 *
 * Revision 1.0.0.1  2000/08/21 22:06:44  stu
 * First Imported.
 *
 */

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "ls_standard.h"
  #include "3d_structures_def.h"
  #include	"zex_misc.h"
  #include "Error_handler.h"
#endif //pch

#include "dlp.h"
#include "cargo.h"
#include "instrum.h"
#include	"load_objects.h"
#include	"object_control_top.h"
#include "obj_control_misc_routines.h"
#include	"objects_controls.h"	//controller definitions
#include "ship_kit.h"
#include "Zsound.h"
#include "file_io.h"
#include "static_object_support.h"
#include "buy.h"

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

int can_gen_cargo;	//if 0 no
int number_of_cargos;	//filled from read_in_cargos (bearing in mind our array starts at index 0)
int jettison_timer;	//if > 0 we can't pick up items

hold_desc cargos_array[NUMBER_OF_COMMODITIES]=
// name                        dofq cls  bph   tr  pr  mfr mkup stk bp  sp  id
{{ "Generic Wood",             1,   10,  10,   0,  0, 1+2,  4,   0,  0,  0,  0}, //0
 { "Pine",                     1,   10,  14,   0,  0, 13,  9,   0,  0,  0,  0}, //1
 { "Oak",                      1,   10,  18,   0,  0, 13,  9,   0,  0,  0,  0}, //2
 { "Raw Rubber",               1,   10,  10,   0,  0, 13,  4,   0,  0,  0,  0}, //3
 { "Synthetic Rubber",         1,   10,  11,   0,  0,  9,  6,   0,  0,  0,  0}, //4
//ores - 5
 { "Iron ore",                 1,   10,  120,  0,  0,  3,  5,   0,  0,  0,  0}, //5
 { "Aluminium ore",            1,   10,  150,  0,  0,  3,  5,   0,  0,  0,  0}, //6
 { "Copper ore",               1,   10,  210,  0,  0,  3,  5,   0,  0,  0,  0}, //7
 { "Silver ore",               4,   10,  380,  0,  2,  3,  15,  0,  0,  0,  0}, //8
 { "Gold ore",                 4,   10,  720,  0,  3,  3,  18,  0,  0,  0,  0}, //9
//Processed ores -10
 { "Steel",                    1,   10,  200,  0,  0,  3,  8,   0,  0,  0,  0}, //10
 { "Aluminium",                1,   10,  220,  0,  0,  1,  8,   0,  0,  0,  0}, //11
 { "Copper",                   1,   10,  300,  0,  0,  1,  8,   0,  0,  0,  0}, //12
 { "Zinc",                     1,   10,  380,  0,  0,  1,  9,   0,  0,  0,  0}, //13
 { "Silver",                   4,   10,  600,  0,  3,  3,  20,  0,  0,  0,  0}, //14
 { "Gold",                     4,   10,  850,  0,  7,  3,  22,  0,  0,  0,  0}, //15
 { "Plutonium",                4,   10,  999,  2,  6,  3,  25,  0,  0,  0,  0}, //16
 { "Triridium",                4,   10, 1700,  2,  7,  2,  30,  0,  0,  0,  0}, //17
//Basic Foodstuffs - cheap on agri, expensive on tech
 { "Wheat",                    1,   11,  20,   0,  0,  7,   3,  0,  0,  0,  0}, //18
 { "Maize",                    1,   11,  13,   0,  0,  1,   3,  0,  0,  0,  0}, //19
 { "Barley",                   1,   11,  18,   0,  0,  1,   3,  0,  0,  0,  0}, //20
 { "Greenstuffs",              1,   11,  22,   0,  0,  1,   3,  0,  0,  0,  0}, //21
 { "Garouda (Alien Wheat)",    1,   11,  13,   0,  0,  2,   3,  0,  0,  0,  0}, //22
 { "Jenilka (Alien Maize)",    1,   11,  17,   0,  0,  2,   3,  0,  0,  0,  0}, //23
 { "Darouda (Alien Barley)",   1,   11,  25,   0,  0,  2,   3,  0,  0,  0,  0}, //24
 { "Yucka (Alien Cabbage)",    1,   11,  30,   0,  0,  2,   3,  0,  0,  0,  0}, //25
 { "Wheat Flour",              1,   11,  24,   0,  0,  1,   4,  0,  0,  0,  0}, //26
// name                        dofq cls  bph   bpa  tr  pr  mfr mkup stk bp  sp  id
//Meat
 { "Beef (Frozen)",            1,   11,  290,  0,  0,  1,   13, 0,  0,  0,  0}, //27
 { "Lamb (Frozen)",            1,   11,  250,  0,  0,  1,   12, 0,  0,  0,  0}, //28
 { "Pork (Frozen)",            1,   11,  230,  0,  0,  1,   12, 0,  0,  0,  0}, //29
 { "Chicken (Frozen)",         1,   11,  200,  0,  0,  1,   12, 0,  0,  0,  0}, //30

 { "Golum (Frozen)",           1,   11,  320,  0,  0,  2,   12, 0,  0,  0,  0}, //31
 { "Bramin (Frozen)",          1,   11,  280,  0,  0,  2,   12, 0,  0,  0,  0}, //32
 { "Herruck (Frozen)",         1,   11,  270,  0,  0,  2,   12, 0,  0,  0,  0}, //33
 { "Cluckum (Frozen)",         1,   11,  190,  0,  0,  2,   12, 0,  0,  0,  0}, //34
 { "Meat Substitute (Frozen)", 1,   11,  34,   0,  0,  2,   5,  0,  0,  0,  0}, //35
// name                        dofq cls  bph   bpa  tr  pr  mfr mkup stk bp  sp  id
//Liquids
 { "Water",                    2,   15,   3,   0,  2,  3,   20, 0,  0,  0,  0}, //36
 { "Carbonated Drinks",        2,   15,  15,   0,  1,  1,   15, 0,  0,  0,  0}, //37
 { "Beer",                     2,   15,  90,   0,  1,  1,   25, 0,  0,  0,  0}, //38
 { "Guinness",                 2,   15, 140,   0,  1,  1,   22, 0,  0,  0,  0}, //39

 { "Whiskey",                  3,   15, 650,   0,  4,  1,   36, 0,  0,  0,  0}, //40
 { "Muhaha",                   3,   15,  55,   0,  0,  2,   20, 0,  0,  0,  0}, //41
 { "Zombie",                   3,   15, 890,   5,  1,  2,   25, 0,  0,  0,  0}, //quite flammable

//machinery
 { "Gears",                    1,   12, 400,   0,  0,  3,   10, 0,  0,  0,  0}, //43
 { "Basic Machinery",          1,   12, 600,   0,  0,  3,   10, 0,  0,  0,  0}, //44
 { "Advanced Machinery",       1,   12, 940,   0,  1,  3,   12, 0,  0,  0,  0}, //45
 { "Basic Electronics",        1,   12, 1000,  0,  0,  3,   10, 0,  0,  0,  0}, //46
 { "Advanced Electronics",     1,   12, 1300,  0,  1,  3,   12, 0,  0,  0,  0}, //47
 { "CPU's (Low Tech)",         1,   12, 1200,  0,  1,  3,   15, 0,  0,  0,  0}, //48
 { "CPU's (High Tech)",        1,   12, 1500,  0,  2,  2,   16, 0,  0,  0,  0}, //49
 { "Memory Systems",           1,   12,  300,  0,  1,  2,   15, 0,  0,  0,  0}, //50
 { "Memory Systems (Fast)",    1,   12,  400,  0,  1,  2,   16, 0,  0,  0,  0}, //51
 { "Raw Fuel",                 1,   12,  200,  4,  4,  3,    6, 0,  0,  0,  0}, //52
 { "Refined Fuel",             1,   12,  300,  5,  5,  3,   20, 0,  0,  0,  0}, //53
 { "Batteries",                1,   12,  400,  0,  0,  3,    5, 0,  0,  0,  0}, //54
 { "Batteries (High Power)",   1,   12,  600,  0,  2,  3,    6, 0,  0,  0,  0}, //55

 { "Basic Tech (Alien)",       1,   12,  600,  0,  0,  2,   10, 0,  0,  0,  0}, //56
 { "Advanced Tech (Alien)",    1,   12,  700,  0,  0,  2,   12, 0,  0,  0,  0}, //57
 { "Basic Tech (Human)",       1,   12,  620,  0,  0,  1,   10, 0,  0,  0,  0}, //58
 { "Advanced Tech (Human)",    1,   12,  750,  0,  0,  1,   12, 0,  0,  0,  0}, //59

 { "Cooling Equipment",        1,   12,  600,  0,  0,  3,   10, 0,  0,  0,  0}, //60

//textiles
// name                        dofq cls  bph   bpa  tr  pr  mfr mkup stk bp  sp  id
 { "Basic Textiles",           1,   13,   23,  0,  0,  3,   5,  0,  0,  0,  0}, //61
 { "Furs",                     1,   13,  160,  0,  0,  1,  15,  0,  0,  0,  0}, //62
 { "Leather Goods",            1,   13,  100,  0,  0,  1,  13,  0,  0,  0,  0}, //63
 { "Silks",                    1,   13,  100,  0,  0,  1,  16,  0,  0,  0,  0}, //bit of a gem this one...

//gems
 { "Human Gemstones",          4,  13,  280,   0,  7,  1,  25,  0,  0,  0,  0}, //64
 { "Alien Gemstones",          4,  13,  200,   0,  7,  2,  26,  0,  0,  0,  0}, //65

 { "Silver Jewellery (Human)", 4,  13,  580,   0,  8,  1,  20,  0,  0,  0,  0}, //66
 { "Silver Jewellery (Alien)", 4,  13,  300,   0,  8,  2,  20,  0,  0,  0,  0}, //67
 { "Gold Jewellery (Human)",   4,  13,  980,   0,  9,  1,  25,  0,  0,  0,  0}, //68
 { "Gold Jewellery (Alien)",   4,  13,  450,   0,  9,  2,  25,  0,  0,  0,  0}, //69

//
 { "Cigars",                   1,  13,  300,   0,  4,  1,  20,  0,  0,  0,  0}, //70
 { "Tobacco",                  1,  13,  272,   0,  3,  3,  16,  0,  0,  0,  0}, //71

//Live items 74
 { "Live Cattle",              1,  16,  500,   4,  0,  1,  15,  0,  0,  0,  0}, //72
 { "Live Pigs",                1,  16,  400,   3,  0,  1,  15,  0,  0,  0,  0}, //73
//WEAPONS
 { "Hand Guns",                1,  14,  600,   0,  9,  2048,  30,  0,  0,  0,  0}, //74
 { "Explosives",               1,  14, 1600,   6,  8,  2048,  30,  0,  0,  0,  0}, //75
 { "High Tech. Weaponry",      1,  14, 1700,   0,  9,  2048,  30,  0,  0,  0,  0}, //76
 { "Missile Parts",            1,  14, 2000,   0,  9,  2048,  25,  0,  0,  0,  0}, //77
 { "Cooling Equipment",        1,  12,  600,   0,  0,  2048,  10,  0,  0,  0,  0}, //78

};


extern	DynObjectsFixedSize *ocb_ptr;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *




//cargo.c

//gen a random cargo pod
//lieklyhood is the chance the object destroyed was carrying cargo - 
//a shuttle might be 80 whilst a fighter would be say 20
void gen_cargo(int object,int likelyhood)	
{
//load_dyn_object_pb the_params;

if (RangedRdm(0,100)<likelyhood)
    gen_specific_cargo(object, RangedRdm(0,number_of_cargos));	//gen a random item
}


//use to gen a cargo pod with a specific cargo - called from
//kill_zex for example
void gen_specific_cargo(int object,int the_cargo)
{
load_dyn_object_pb the_params;
int static_slot;
int dest_ocb;
//int test;
int cargs_div_4;
//load in a cargo container
        clear_pb(&the_params);
  
	the_params.world_x=(*ocb_ptr).object_list[object].Oworldx;
	the_params.world_y=(*ocb_ptr).object_list[object].Oworldy;
    the_params.delta_rot_z=5;
    the_params.delta_rot_x=3;
    
	the_params.world_z=MONSTER_Z;

//pick colour depending on cargo
//we have about 80 cargos so...
    cargs_div_4=number_of_cargos/4;
    
    if (the_cargo<cargs_div_4)
      static_slot=get_static_object_slot('CAR1');	//go fetch fido (the object)
    else
    if (the_cargo<cargs_div_4*2)
      static_slot=get_static_object_slot('CAR2');	//go fetch fido (the object)
    else
    if (the_cargo<cargs_div_4*3)
      static_slot=get_static_object_slot('CAR3');	//go fetch fido (the object)
    else
      static_slot=get_static_object_slot('CAR4');	//go fetch fido (the object)
    
    
      if (static_slot==-1) 
      return;	//oops

      dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
      if (dest_ocb!=-1)
        {
	     the_params.controller_ref=CARGO_CONTAINER;
    	
	     the_params.shield_value=470;
	     the_params.mass=100;
	     the_params.no_trivial_rejection=1;
	     the_params.object_category=(*ocb_ptr).object_list[object].Dyn_OCB_control_data.allegiance;
	     load_dyn_object(static_slot,dest_ocb,&the_params,-1,1,-1,!DUST_BIT);	//the object, the position (-1=next free)
       }
}

//returns 1 if picked up
int handle_pickup(int what_object)
{
extern hold_desc hold_items[MAX_HOLD_ITEMS];
int hold_slot,i;
//int j;
char  cargo_descript_string[300];	//string for cargo descript.


if (jettison_timer>0) return 0;

hold_slot=-1;
//can we find an empty slot in the hold?
for (i=0;i<MAX_HOLD_ITEMS;i++)
    {
    if (hold_items[i].id==-1) 
     {
      hold_slot=i;
      break;
     }
    }
if (hold_slot==-1) 
 {
 add_to_text_display("OBC> No room in hold for cargo container!",DLP_YELLOW);

 return 0;	  //all slots full
 }
//ok, we can take it

i=(*ocb_ptr).object_list[what_object].Dyn_OCB_control_data.control_use_2;;	//get the item

if(add_to_kill_list(what_object)!=0)
  {
			 MyDebugStr(__LINE__, __FILE__,"add_to_kill_list failed!");
  }


hold_items[hold_slot]=cargos_array[i];	//copy all the data for this item to the hold

gen_cargo_description_string(cargo_descript_string,i);

 add_to_text_display(cargo_descript_string,DLP_GREEN);
     play_zsound(pickup, sound_low_pri, c1_chan1, sound_vol_7);

return 1;

}

void gen_cargo_description_string(char * where_string, int which_id)
{
if (cargos_array[which_id].dofq==1)
{
strcpy (where_string,"One Tonne of ");
where_string+=13;
}
else
if (cargos_array[which_id].dofq==4)
{
strcpy (where_string,"1Kg of ");
where_string+=7;
}
else
if (cargos_array[which_id].dofq==2)
{
strcpy (where_string,"1 Barrel of ");
where_string+=12;
}
else
if (cargos_array[which_id].dofq==3)
{
strcpy (where_string,"1 case of ");
where_string+=10;
}


strcpy (where_string,cargos_array[which_id].string);
}


void read_in_cargo()
{
//Handle mitm_res_h;
//short * mitm_ptr;
//hold_desc * mitm_ptr1;
int i;
/*
 mitm_res_h = (Handle) ZGetResource('CITM',128);  //Get the Handle to the Resource 
 if (mitm_res_h==0) report_error("Resource missing: CITM 128","\p",4);
 HLock (mitm_res_h);
 mitm_ptr=(short *) *mitm_res_h;	//Oh wonderful god of casting
 
 number_of_cargos=(*mitm_ptr);
 mitm_ptr++;
 
 mitm_ptr1=(hold_desc*) mitm_ptr;	//skip number of entries
//now blockmove the rest
 BlockMoveData(mitm_ptr1, cargos_array,sizeof(hold_desc)*number_of_cargos);
*/
 
//set up ID's (the private field of the resedit template
for (i=0;i<NUMBER_OF_COMMODITIES;i++)
 cargos_array[i].id=i;

}