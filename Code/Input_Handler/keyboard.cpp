// *************************************MODULE HEADER*********************************
// * FILENAME           : keyboard.cpp
// * MODULE TITLE       : keyboard
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 18:19:16 on 7th October 2001
// * FIRST AUTHOR       : Stu
// *
// * COPYRIGHT (c)        LS
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************
/* CVS bits
$Log: keyboard.cpp,v $
Revision 1.2  2003/09/28 17:29:49  robp
Changed files from .c to .cpp and removed spaces out of a couple of filenames.

Revision 1.1.1.1  2003/09/05 22:35:55  stu_c
First Imported.

Revision 1.17  2002/09/08 02:00:50  stu_c
Precompiled header adjustments

Revision 1.16  2002/02/17 06:21:25  stu
Added radar targetting

Revision 1.15  2002/02/16 19:05:09  stu
Radar range decrease key and rationalisation of dlp colours

Revision 1.14  2002/01/09 23:48:58  stu
Furhter work on trading.

Revision 1.13  2002/01/02 19:24:46  rob
Added key setup code

Revision 1.12  2001/12/29 01:10:13  stu
Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.

Revision 1.11  2001/11/26 23:28:08  rob
Galaxy map.

Revision 1.10  2001/11/07 22:21:57  rob
No game input from keyboard when in input mode.

Revision 1.9  2001/11/06 20:11:12  rob
Key input changes

Revision 1.8  2001/11/06 00:26:43  rob
First stage of getting keyboard working.

Revision 1.7  2001/11/02 19:31:00  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.6  2001/10/26 22:44:14  rob
Dual header and anvil changes

Revision 1.5  2001/10/23 21:55:29  stu
More carbon work

Revision 1.4  2001/10/22 21:28:06  rob
Carbon warning changes

Revision 1.3  2001/10/21 17:38:53  rob
got port from window.

Revision 1.2  2001/10/21 12:38:14  rob
carbon changes

Revision 1.1  2001/10/21 01:12:44  stu
Initial porting work

Revision 1.0.0.1  2001/10/17 20:46:06  rob
First Imported.

*/

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
    #include <AGL/agl.h>
    #include <DrawSprocket/DrawSprocket.h>
  #else
    #include <agl.h>
    #include <events.h>
    #include <DrawSprocket.h>
  #endif
  
  #include "Error_handler.h"
#endif //pch

#include "keyboard.h"

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

typedef struct
{
unsigned char key_code;
unsigned char the_long; 
unsigned long the_bit;
} keymap_decode;

keymap_decode key_decodes [NUMBER_OF_KEYS]; //byte, bit in keymap (16 bytes) - so we don't have to calculate



//Our controller positions
UInt32 gThrustUp;
UInt32 gThrustDown;
UInt32 gVertUpThrust;
UInt32 gVertDownThrust;
UInt32 gRolll;	//was gTurn
UInt32 gRollr;	//was gTurn
UInt32 gPitchu;
UInt32 gPitchd;
UInt32 gYawl;
UInt32 gYawr;

UInt32 gCannon;
UInt32 gMissile;
UInt32 gLaser;

UInt32 gEject;	//as it says
UInt32 gAbort;
UInt32 gMusic_up;
UInt32 gMusic_down;
UInt32 gSound_up;
UInt32 gSound_down;
UInt32 gRadarRangeInput; //radar range up
UInt32 gRadarDown;	//radar range down

UInt32 gShowInventory;	//show the inventory screen
UInt32 gAskForDock;	//ask mothership for dock
UInt32 gDampers;	//velocity limiter
UInt32 gCommandControl;	//Auto Pilot
UInt32 gGalacticMap;	//Galactic Map
UInt32 gConsole;
UInt32 gTarget;
UInt32 gRadartarget;
UInt32 gTimeaccelup;
UInt32 gTimeacceldown;
UInt32 gAutopilotUp;
UInt32 gAutopilotDown;
UInt32 gSolarMap;	// Solar System Map
UInt32 gKeyMapping;	// Select Keys

UInt32 gFrontView;
UInt32 gLeftView;
UInt32 gRightView;
UInt32 gRearView;

UInt32 gPane;     //MFD pane toggle

static int keyboard_is_input=0;
static int mouse_first_run; //for mouse delta
Point mouse_last;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *
void clear_keyglobs(void);



void enable_inputs(void)
{
 keyboard_is_input=1;
}


void disable_inputs(void)
{
 keyboard_is_input=0;
}

void init_keyboard_input(void)
{
 keyboard_is_input=0;
 clear_keyglobs(); 
 mouse_first_run=1;
}


void clear_keyglobs(void)
{
 gThrustUp=0;
 gThrustDown=0;
 
 gVertUpThrust=0;
 gVertDownThrust=0;
 gRolll=0;	//was gTurn
 gRollr=0;	//was gTurn
 gPitchu=0;
 gPitchd=0;
 gYawl=0;
 gYawr=0;


 gCannon=0;
 gMissile=0;
 gLaser=0;
 gEject=0;	//as it says
 gAbort=0;
 gMusic_up=0;
 gMusic_down=0;
 gSound_up=0;
 gSound_down=0;
 gRadarRangeInput=0;
 gRadarDown=0;	//orbit stabilisation
 gShowInventory=0;	//show the inventory screen
 gAskForDock=0;	//ask mothership for dock
 gDampers=0;	//velocity limiter
 gCommandControl=0;	//Auto Pilot
 gGalacticMap=0;	//Galactic Map
 gSolarMap=0;	//Solar Map
 gConsole=0;
 gTarget=0;
 gRadartarget=0;
 gTimeaccelup=0;
 gTimeacceldown=0;
 gAutopilotUp=0;
 gAutopilotDown=0;
 gKeyMapping=0;
 gFrontView=0;
 gLeftView=0;
 gRightView=0;
 gRearView=0;
 gPane=0;
 
}

void poll_input_device(void)
{
	unsigned long km[16];
    //int n;

if(keyboard_is_input==0)
  {
  clear_keyglobs();
  }
else
  {
    GetKeys((keymap_ptr)km);

    gThrustUp=km[key_decodes[0].the_long] & key_decodes[0].the_bit;
    gThrustDown=km[key_decodes[1].the_long] & key_decodes[1].the_bit;
    gVertUpThrust=km[key_decodes[2].the_long] & key_decodes[2].the_bit;
    gVertDownThrust=km[key_decodes[3].the_long] & key_decodes[3].the_bit;
    gRolll=km[key_decodes[4].the_long] & key_decodes[4].the_bit;
    gRollr=km[key_decodes[5].the_long] & key_decodes[5].the_bit;
    gPitchd=km[key_decodes[6].the_long] & key_decodes[6].the_bit;
    gPitchu=km[key_decodes[7].the_long] & key_decodes[7].the_bit;
    gYawl=km[key_decodes[8].the_long] & key_decodes[8].the_bit;
    gYawr=km[key_decodes[9].the_long] & key_decodes[9].the_bit;
    
    gCannon=km[key_decodes[10].the_long] & key_decodes[10].the_bit;
    gMissile=km[key_decodes[11].the_long] & key_decodes[11].the_bit;
    gLaser=km[key_decodes[12].the_long] & key_decodes[12].the_bit;

    gEject=km[key_decodes[13].the_long] & key_decodes[13].the_bit;
    gAbort=km[key_decodes[14].the_long] & key_decodes[14].the_bit;
    
    gMusic_up=km[key_decodes[15].the_long] & key_decodes[15].the_bit;
    gMusic_down=km[key_decodes[16].the_long] & key_decodes[16].the_bit;

    gSound_up=km[key_decodes[17].the_long] & key_decodes[17].the_bit;
    gSound_down=km[key_decodes[18].the_long] & key_decodes[18].the_bit;
   
    gRadarRangeInput=km[key_decodes[19].the_long] & key_decodes[19].the_bit;
    gRadarDown=km[key_decodes[20].the_long] & key_decodes[20].the_bit;

    gShowInventory=km[key_decodes[21].the_long] & key_decodes[21].the_bit;
    gAskForDock=km[key_decodes[22].the_long] & key_decodes[22].the_bit;
    gDampers=km[key_decodes[23].the_long] & key_decodes[23].the_bit;
    gCommandControl=km[key_decodes[24].the_long] & key_decodes[24].the_bit;

    gSolarMap=km[key_decodes[25].the_long] & key_decodes[25].the_bit;	//Solar Map
    gConsole=km[key_decodes[26].the_long] & key_decodes[26].the_bit;
    gTarget=km[key_decodes[27].the_long] & key_decodes[27].the_bit;
    gTimeaccelup=km[key_decodes[28].the_long] & key_decodes[28].the_bit;
    gTimeacceldown=km[key_decodes[29].the_long] & key_decodes[29].the_bit;
    gAutopilotUp=km[key_decodes[30].the_long] & key_decodes[30].the_bit;
    gAutopilotDown=km[key_decodes[31].the_long] & key_decodes[31].the_bit;
//    gGalacticMap=km[key_decodes[32].the_long] & key_decodes[32].the_bit;	  //Galactic Map
    gGalacticMap=0;	// disable galactic map display

    gKeyMapping=km[key_decodes[33].the_long] & key_decodes[33].the_bit;
    gRadartarget=km[key_decodes[34].the_long] & key_decodes[34].the_bit;

    gFrontView=km[key_decodes[35].the_long] & key_decodes[35].the_bit;
    gLeftView=km[key_decodes[36].the_long] & key_decodes[36].the_bit;
    gRightView=km[key_decodes[37].the_long] & key_decodes[37].the_bit;
    gRearView=km[key_decodes[38].the_long] & key_decodes[38].the_bit;
    gPane=km[key_decodes[39].the_long] & key_decodes[39].the_bit;

  } // end of keyboard is input

}


void init_input_device(void)
{
//this is where we read the 
}

unsigned char get_programmed_keycode(int key_number)
{
return key_decodes[key_number].key_code;
}

void load_key_def(unsigned int* the_keys)	//called from LSGF to load up the key bindings
{
int n;

 for (n=0;n<NUMBER_OF_KEYS; n++)
 {
 load_single_key_def(n,*the_keys);
 the_keys++;
 }
}


void load_single_key_def(int key_number, int keycode)
{
int bit_select;

  key_decodes[key_number].key_code=keycode;		// store keycode for key programming mode  
  key_decodes[key_number].the_long=keycode>>5;		// divide by 32
  bit_select = keycode & 0x1F;	    	    		// select the byte select plus bit selection
  bit_select ^= 0x18;					// invert so 0 becomes 3, 1 becomes 2, 2->1, 3->0, affective counting down - byte address becomes bit ordering right to left.
  key_decodes[key_number].the_bit = 1<<bit_select;	// make a mask
}

short isPressed(unsigned short k )
// k =  any keyboard scan code, 0-127
{
	unsigned char km[16];

	GetKeys( (keymap_ptr) km);
	return ( ( km[k>>3] >> (k & 7) ) & 1);
}


void ZexGetMouse (Point *the_point)
{
extern	  AGLDrawable aglWindow;

DSpGetMouse (the_point);
SetPort(GetWindowPort((WindowRef)aglWindow));
GlobalToLocal (the_point);

}

void ZexGetMouseDelta (Point *the_point)
{
extern	  AGLDrawable aglWindow;
Point current_point;

DSpGetMouse (&current_point);
SetPort(GetWindowPort((WindowRef)aglWindow));
GlobalToLocal (&current_point);

if (mouse_first_run)
{
  mouse_last=current_point;
  mouse_first_run=0;
}

the_point->h=mouse_last.h-current_point.h;
the_point->v=mouse_last.v-current_point.v;

}


int game_fire_button(void)
{

  return Button();

}