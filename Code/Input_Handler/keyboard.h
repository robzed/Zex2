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

#define NUMBER_OF_KEYS 40
////array of keys as follows
//typedef struct  
//{
// unsigned char mthr;
// unsigned char revt;
//
// unsigned char vthr;
// unsigned char ivth;
// 
// unsigned char roll;
// unsigned char rolr;
// unsigned char pitd;
// unsigned char pitu;
// unsigned char yawl;
// unsigned char yawr;	  //10
//
// unsigned char cann;
// unsigned char miss;
// unsigned char lasr;	  //13
//
// unsigned char ejct;
// unsigned char endg;    //pause
// unsigned char musu;
// unsigned char musd;    //music
// unsigned char sonu;
// unsigned char sond;    //sound
// unsigned char rrup;    //radar range
// unsigned char orbt;    //orbit stab - 21
//
// unsigned char shoi;	  //inventory - 22
//
// unsigned char dock;
// unsigned char damp;
// unsigned char comk;
// unsigned char solm;   //solar map - 26
//
// unsigned char conn;
//
// unsigned char targ;
// unsigned char timu;	  //accelerate time
// unsigned char timd;    //decelerate time
// unsigned char apup;
// unsigned char apdn;    //32
// 
// unsigned char galm;
// 
//} keys_table;




//Our controller positions
extern UInt32 gThrustUp;
extern UInt32 gThrustDown;
extern UInt32	gVertUpThrust;
extern UInt32  gVertDownThrust;
extern UInt32 gRolll;
extern UInt32 gPitchu;
extern UInt32 gYawl;
extern UInt32 gRollr;
extern UInt32 gPitchd;
extern UInt32 gYawr; //9


extern UInt32 gCannon;
extern UInt32 gMissile;
extern UInt32 gLaser; //12

extern UInt32 gEject; //13
extern UInt32 gAbort;
extern UInt32 gMusic_up;
extern UInt32 gMusic_down;
extern UInt32 gSound_up;
extern UInt32 gSound_down;
extern UInt32 gRadarRangeInput; //radar range up
extern UInt32 gRadarDown;	//radar range down -20

extern UInt32 gShowInventory;	//show the inventory screen
extern UInt32 gAskForDock;	//ask mothership for dock
extern UInt32 gDampers;	//velocity limiter
extern UInt32 gCommandControl;	//Auto Pilot
extern UInt32 gSolarMap;	//Solar System Map - 25
extern UInt32 gConsole; //26
extern UInt32 gTarget; //27
extern UInt32 gRadartarget; //28
extern UInt32 gTimeaccelup; //29
extern UInt32 gTimeacceldown; //30
extern UInt32 gAutopilotUp;
extern UInt32 gAutopilotDown;
extern UInt32 gGalacticMap;	// Galactic Map
extern UInt32 gKeyMapping;	// Key Mapping

extern UInt32 gFrontView; //36
extern UInt32 gLeftView; //37
extern UInt32 gRightView; //38
extern UInt32 gRearView; //38
extern UInt32 gPane; //39

void init_keyboard_input(void);
void enable_inputs(void);
void disable_inputs(void);
void poll_input_device(void);
void init_input_device(void);
void ZexGetMouse(Point* where);
void ZexGetMouseDelta (Point *the_point);

short isPressed(unsigned short k );
int game_fire_button(void);

void load_key_def( unsigned int * the_keys);	//called from LSGF to load up the key bindings

unsigned char get_programmed_keycode(int key_number);
void load_single_key_def(int key_number, int keycode);
