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
// * FILENAME           : key_setting.cpp
// * MODULE TITLE       : key_setting
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 20:33 on 17th December 2001
// * FIRST AUTHOR       : Rob
// *
// * COPYRIGHT (c)        LS 2001
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************


// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

#include "Error_handler.h"
#include "processor.h"
#endif //pch

#include "keyboard.h"
#include "key_setting.h"

#include "pictures.h"
#include "OGL.h"
#include "olg_text.h"
#include "splat_control.h"
#include "Zsound.h"
#include "coffee.h"
#include "fpoly4.h"

// ***********************************************************************************
// * CONSTANTS 
// *


// ***********************************************************************************
// * TYPE DEFINITIONS
// *


// ***********************************************************************************
// * DATA STRUCTURE DECLARATIONS 
// *

#define DISABLED 0	// disabled keys are missing from the game (no duplicate checking, or saving)
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define YELLOW 6
#define WHITE 7

#define SPACE 8

#define BRIGHT 16


struct { int enabled; char *string; int keynumber; char *forth_string; } key_names[]={

// Enabled Name
	{ BRIGHT+YELLOW, "Main Thrusters",0,"MTHR" } ,
	{ BRIGHT+YELLOW, "Reverse Thrusters",1,"REVT" } ,
	{ BRIGHT+YELLOW, "Vertical Thrust",2,"VTHR" },
	{ BRIGHT+YELLOW+SPACE, "Inverse Vertical Thrust",3,"IVTH", },
	
	{ YELLOW, "Roll Left",4,"ROLL" },
	{ YELLOW, "Roll Right",5,"ROLR" },
	{ YELLOW, "Pitch Down",6,"PITD" },
	{ YELLOW, "Pitch Up",7,"PITU" },
	{ YELLOW, "Yaw Left",8,"YAWL" },
	{ YELLOW+SPACE, "Yaw Right",9,"YAWR" },
	
	{ BRIGHT+YELLOW, "Cannon",10,"CANN" },
	{ BRIGHT+YELLOW, "Missile",11,"MISS" },
	{ BRIGHT+YELLOW+SPACE, "Laser",12,"LASR" },
	
	{ GREEN, "Eject",13,"EJCT" },
	{ GREEN, "Scanner Range Up",19,"RRUP" },
	{ GREEN, "Scanner Range Down",20,"RRDN" },
	{ GREEN, "Dock",22,"DOCK" },
	{ GREEN, "Damper Control",23,"DAMP" },
	{ GREEN, "Command",24,"COMK" },
	{ GREEN, "Visible Target Item",27,"TARG" },
	{ GREEN, "Radar Target Item",34,"RTAR" },	
	{ GREEN, "Auto Pilot Mode Up",30,"APUP" },
	{ GREEN, "Auto Pilot Mode Down",31,"APDN" },
	{ GREEN+SPACE, "MFD Pane toggle",39,"PANE" },

	{ CYAN, "End Game",14,"ENDG" },
	{ CYAN, "Music Volume Up",15,"MUSU" },
	{ CYAN, "Music Volume Down",16,"MUSD" },
	{ CYAN, "Sound Volume Up",17,"SONU" },
	{ CYAN, "Sound Volume Down",18,"SOND" },
	{ CYAN, "Show Inventory",21,"SHOI" },
	{ CYAN, "System Map",25,"SOLM" },
	{ CYAN+SPACE, "Console",26,"CONN" },

	{ CYAN, "Time Accel. Down",29,"TIMD" },
	{ CYAN, "Time Accel. Up",28,"TIMU" },
	{ DISABLED, "Galactic Map",32,"GALM" },
	{ CYAN+SPACE, "Key Setting",33,"KSET" },

	{ BLUE, "Front View",35,"FVIW" },
	{ BLUE, "Rear View",38,"BVIW" },
	{ BLUE, "Left View",36,"LVIW" },
	{ BLUE, "Right View",37,"RVIW" },
        
	

        { -1, "End Marker",0,"" }
};


struct { int value; char *string; char *forth_string; } key_codes[]={
// Code Name
{ 0x35 , "ESC KEY","ESCKEY" },
{ 0x7A , "F1 KEY","F1KEY" },
{ 0x78 , "F2 KEY","F2KEY" },
{ 0x63 , "F3 KEY","F3KEY" },
{ 0x76 , "F4 KEY","F4KEY" },
{ 0x60 , "F5 KEY","F5KEY" },
{ 0x61 , "F6 KEY","F6KEY" },
{ 0x62 , "F7 KEY","F7KEY" },
{ 0x64 , "F8 KEY","F8KEY" },
{ 0x65 , "F9 KEY","F9KEY" },
{ 0x6D , "F10 KEY","F10KEY" },
{ 0x67 , "F11 KEY","F11KEY" },
{ 0x6F , "F12 KEY","F12KEY" },
{ 0x69 , "F13 KEY","F13KEY" },
{ 0x6B , "F14 KEY","F14KEY" },
{ 0x71 , "F15 KEY","F15KEY" },

// TOP ROW
{ 0x32 , "` KEY","`KEY" },
{ 0x12 , "1 KEY","1KEY" },
{ 0x13 , "2 KEY","2KEY" },
{ 0x14 , "3 KEY","3KEY" },
{ 0x15 , "4 KEY","4KEY" },
{ 0x17 , "5 KEY","5KEY" },
{ 0x16 , "6 KEY","6KEY" },
{ 0x1A , "7 KEY","7KEY" },
{ 0x1C , "8 KEY","8KEY" },
{ 0x19 , "9 KEY","9KEY" },
{ 0x1D , "0 KEY","0KEY" },
{ 0x1B , "- KEY","-KEY" },
{ 0x18 , "= KEY","=KEY" },
{ 0x33 , "BACKSPACE KEY","BACKSPACEKEY" },

{ 0x72 , "HELP KEY","HELPKEY" },
{ 0x73 , "HOME KEY","HOMEKEY" },
{ 0x74 , "PAGEUP KEY","PAGEUPKEY" },

{ 0x47 , "NUM CLEAR KEY","NUMCLEARKEY" },
{ 0x51 , "NUM= KEY","NUM=KEY" },
{ 0x4B , "NUM/ KEY","NUM/KEY" },
{ 0x43 , "NUM* KEY","NUM*KEY" },

// SECOND ROW
{ 0x30 , "TAB KEY","TABKEY" },
{ 0x0C , "Q KEY","QKEY" },
{ 0x0D , "W KEY","WKEY" },
{ 0x0E , "E KEY","EKEY" },
{ 0x0F , "R KEY","RKEY" },
{ 0x11 , "T KEY","TKEY" },
{ 0x10 , "Y KEY","YKEY" },
{ 0x20 , "U KEY","UKEY" },
{ 0x22 , "I KEY","IKEY" },
{ 0x1F , "O KEY","OKEY" },
{ 0x23 , "P KEY","PKEY" },
{ 0x21 , "[ KEY","[KEY" },
{ 0x1E , "] KEY","]KEY" },
{ 0x2A , "\\ KEY","\\KEY" },

{ 0x75 , "DEL KEY","DELKEY" },
{ 0x77 , "END KEY","ENDKEY" },
{ 0x79 , "PAGEDOWN KEY","PAGEDOWNKEY" },

{ 0x59 , "NUM7 KEY","NUM7KEY" },
{ 0x5B , "NUM8 KEY","NUM8KEY" },
{ 0x5C , "NUM9 KEY","NUM9KEY" },
{ 0x4E , "NUM- KEY","NUM-KEY" },

// THIRD ROW
{ 0x39 , "CAPSLOCK KEY","CAPSLOCKKEY" },
{ 0x00 , "A KEY","AKEY" },
{ 0x01 , "S KEY","SKEY" },
{ 0x02 , "D KEY","DKEY" },
{ 0x03 , "F KEY","FKEY" },
{ 0x05 , "G KEY","GKEY" },
{ 0x04 , "H KEY","HKEY" },
{ 0x26 , "J KEY","JKEY" },
{ 0x28 , "K KEY","KKEY" },
{ 0x25 , "L KEY","LKEY" },
{ 0x29 , "; KEY",";KEY" },
{ 0x27 , "' KEY","'KEY" },
{ 0x24 , "ENTER KEY","ENTERKEY" },

{ 0x56 , "NUM4 KEY","NUM4KEY" },
{ 0x57 , "NUM5 KEY","NUM5KEY" },
{ 0x58 , "NUM6 KEY","NUM6KEY" },
{ 0x45 , "NUM+ KEY","NUM+KEY" },

// FOURTH ROW
{ 0x38 , "LEFT SHIFT KEY","LEFTSHIFTKEY" },
{ 0x06 , "Z KEY","ZKEY" },
{ 0x07 , "X KEY","XKEY" },
{ 0x08 , "C KEY","CKEY" },
{ 0x09 , "V KEY","VKEY" },
{ 0x0B , "B KEY","BKEY" },
{ 0x2D , "N KEY","NKEY" },
{ 0x2E , "M KEY","MKEY" },
{ 0x2B , ", KEY",",KEY" },
{ 0x2F , ". KEY",".KEY" },
{ 0x2C , "/ KEY","/KEY" },
{ 0x3C , "RIGHT SHIFT KEY","RIGHTSHIFTKEY" },

{ 0x7E , "UP ARROW KEY","UPARROWKEY" },
{ 0x53 , "NUM1 KEY","NUM1KEY" },
{ 0x54 , "NUM2 KEY","NUM2KEY" },
{ 0x55 , "NUM3 KEY","NUM3KEY" },

// BOTTOM ROW
{ 0x3B , "LEFT CONTROL KEY","LEFTCONTROLKEY" },
{ 0x3A , "LEFT OPTION KEY","LEFTOPTIONKEY" },
{ 0x37 , "LEFT APPLE KEY","LEFTAPPLEKEY" },
{ 0x31 , "SPACE KEY","SPACEKEY" },
{ 0x37 , "RIGHT APPLE KEY","RIGHTAPPLEKEY" },
{ 0x3D , "RIGHT OPTION KEY","RIGHTOPTIONKEY" },
{ 0x3E , "RIGHT CONTROL KEY","RIGHTCONTROLKEY" },

{ 0x7B , "LEFT ARROW KEY","LEFTARROWKEY" },
{ 0x7D , "DOWN ARROW KEY","DOWNARROWKEY" },
{ 0x7C , "RIGHT ARROW KEY","RIGHTARROWKEY" },

{ 0x52 , "NUM0 KEY","NUM0KEY" },
{ 0x41 , "NUM. KEY","NUM.KEY" },
{ 0x4C , "NUM ENTER KEY","NUMENTERKEY" },
{ -1 , "UNKNOWN KEY","" },
};

// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

static int mx, my, mvalid=0, mkeyprogram=-1;
static double mkeyflash;
static int keys_changed_flag=0;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

int handle_key_mouse(LSRAW* button_down, LSRAW* button_up);
void print_keys(void);
char handle_key_entry_mouse(int x1, int y1, int x2, int y2, int i);
void handle_key_entry_click(int x, int y);
char *get_key_name(int keycode);
void handle_actual_key_entry(void);
int get_any_single_key(void);
int find_duplicated_key(int keycode);
void accept_keys(void);
void revert_keys(void);
void load_default_keys(void);
char *get_forth_key_name(int keycode);
void kwrite(char **output, char *string);

// ***********************************************************************************
// * ACTUAL CODE
// *

// +--------------------------------+-------------------------+-----------------------
// | TITLE: do_key_setting          | AUTHOR(s): Rob Probin   | DATE STARTED: 2Jan02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

#define CONTROL_PLATE_X (screen_cent_x+20)
#define CONTROL_PLATE_Y (screen_cent_y+40)

void do_key_setting(void)
{
#if PORTABLE_FILESYSTEM
ZexPicture* picture_pic_obj;
LSRAW* the_picture;

ZexPicture* button_data_pic_obj;
ZexPicture* button_down_data_pic_obj;
#else
Handle picture_H;
LSRAW* the_picture;

Handle button_data_H;
Handle button_down_data_H;
#endif
LSRAW *button;
LSRAW *button_down;
int player_click;

// initilisation block
freeze_physics();
init_screen_stars();

set_poly_clipping(0,monitor_w,0,monitor_h);	//set poly clip rect - sb 181298	
erase_zex_rect(monitor_w,monitor_h);	//in pictures
DSpContext_FadeGamma ( mDisplayContext, 100, 0);	//clear any sun glare

ShowCursor();
mvalid=0; mkeyprogram=-1;
keys_changed_flag=0;	// initially no change


// set up secreen
    #if PORTABLE_FILESYSTEM
    button_data_pic_obj= new ZexPicture ('RCZ ',132);	//prefs_button_up   
    button = button_data_pic_obj->GetPictureRef();
	
    button_down_data_pic_obj= new ZexPicture ('RCZ ',133);	//prefs_button_down   
    button_down = button_down_data_pic_obj->GetPictureRef();

    picture_pic_obj= new ZexPicture ('RCZ ',201);	//diff pict   
    if (picture_pic_obj==0) report_error("Resource missing: RCZ 00200","\p",2047);
    the_picture = picture_pic_obj->GetPictureRef();
 
    #else

    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up   
	HLock(button_data_H);
	button = (LSRAW*)*button_data_H;
	
    button_down_data_H=GetZexPicture ('RCZ ',133);	//prefs_button_down   
	HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;


 picture_H=GetZexPicture ('RCZ ',201);	//diff pict   
 if (picture_H==0) report_error("Resource missing: RCZ 00200","\p",2047);
 HLock(picture_H);

// fade_main_screen();
 
 the_picture = (LSRAW*)*picture_H;
    #endif
    
 player_click=0;

while (player_click==0)
 {
 erase_zex_rect(monitor_w,monitor_h);	 //in pictures
 show_screen_stars();

 print_keys();
 
 print_crunched_trans_pict(the_picture,CONTROL_PLATE_X+65,CONTROL_PLATE_Y+15);

 player_click=handle_key_mouse(button_down,button);

 // Button 1: Accept -- saves in forth file (if changed) & exits
 if (player_click==1) { accept_keys(); }

 else // Button 2: Cancel/Revert -- causes forth to reload from file
 if (player_click==2) { revert_keys(); player_click=0; }

 else // Button 3: Defaults -- loads hardcoded defaults as keyset
 if (player_click==3) { load_default_keys(); player_click=0; }
 
 Show_it();
 
 thread_yield();
 update_microsecs_timer();
 handle_actual_key_entry();
 }

#if PORTABLE_FILESYSTEM
delete picture_pic_obj;
delete button_data_pic_obj;
delete button_down_data_pic_obj;
#else	// resource memory leak?
DisposeHandle(picture_H);
#endif

// disposal cleanup block
destroy_screen_stars();
unfreeze_physics();
HideCursor();

} // end of do_key_setting()





// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

static int button_y2_array[4] = {0,34,68,102};

int handle_key_mouse(LSRAW* button_down, LSRAW* button_up)
{
int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i;
extern int old_button_state;

    ZexGetMouse (&mouse_xy);
    
    button_state=game_fire_button();
    
//first draw all buttons as off
    for (i=0; i<3;i++)
        print_crunched_trans_pict(button_up,CONTROL_PLATE_X,button_y2_array[i]+CONTROL_PLATE_Y);
//now draw the down one
    for (i=0; i<3;i++)
    {
     if(mouse_xy.h > CONTROL_PLATE_X && 
        mouse_xy.v >= (button_y2_array[i]+CONTROL_PLATE_Y) && 
        mouse_xy.v < button_y2_array[i+1]+CONTROL_PLATE_Y)	//controls
     {
     print_crunched_trans_pict(button_down,CONTROL_PLATE_X,button_y2_array[i]+CONTROL_PLATE_Y);
     return_val=i+1;
     }
    }

if (button_state==1 && old_button_state==0) 	//if down and was up
  {
  handle_key_entry_click(mouse_xy.h,mouse_xy.v);
  if(return_val!=0)
    {
    play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
    }
  }
  
if (button_state==0 && old_button_state==1)	//if up and was down
  { 
    real_return_val = return_val; }
else
  { real_return_val=0; }
  
old_button_state=button_state;

return real_return_val;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

#define PK_START_X 40
#define PK_START_Y 40

#define PK_STEP_X 310
#define PK_STEP_Y 15

#define PK_COLUMN_SPACE 150
#define PK_LIMIT_Y 440

#define DIM 0.5

#define FLASH_TIME 0.25

void print_keys(void)
{
 int x,y,i;
 int enabled;
 
 x=PK_START_X; y=PK_START_Y; i=0;
 
 enabled=key_names[i].enabled;
 while(enabled != -1)
   {
   if(enabled>=1)
     {
     // we change the colour to a flashing colour if this function is selected
     if(mkeyprogram==i)
       {
       mkeyflash+=real_frame_time_in_secs;
       if(mkeyflash<FLASH_TIME)
          { enabled&=~WHITE; enabled|=RED+BRIGHT; }
       else
          { 
          if(mkeyflash>(2*FLASH_TIME)) { mkeyflash=0; } // reset
          enabled|=WHITE+BRIGHT;
          }
       }
       
     // highlight it instantly it is clicked, to provide instant feedback
     if(handle_key_entry_mouse(x,y,x+PK_STEP_X,y+PK_STEP_Y,i))
       { enabled|=WHITE+BRIGHT; }

     // select the colour, as per the table (or overridden).
     if(enabled&BRIGHT)
       { glColor3d(((enabled&RED)!=0), ((enabled&GREEN)!=0), (enabled&BLUE)); }
     else
       { glColor3d(DIM*((enabled&RED)!=0), DIM*((enabled&GREEN)!=0), DIM*(enabled&BLUE)); }

     OGL_moveto(x+off_640_x,y+off_480_y); 
     OGL_draw_monaco_bold(key_names[i].string);
     
     OGL_moveto(x+PK_COLUMN_SPACE+off_640_x,y+off_480_y);
     OGL_draw_monaco_bold(get_key_name(get_programmed_keycode(key_names[i].keynumber)));
     
     if(enabled&SPACE) y+=PK_STEP_Y;
     y+=PK_STEP_Y;
     } // end of if enabled
   if(y>=PK_LIMIT_Y) { x+=PK_STEP_X; y=PK_START_Y; }
   
   i++;
   enabled=key_names[i].enabled;
   } // end of while

if(mvalid==1) { mkeyprogram=-1; }	// unrecognised click - deselect
mvalid=0;		// make mouse no longer valid
   
} // end of print keys


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----
#define FONT_SHIFT_Y -10
#define WINDOW_SHIFT_X -40

char handle_key_entry_mouse(int x1, int y1, int x2, int y2, int i)
{
if(mvalid!=0)
  {
  if(mx>=(x1+WINDOW_SHIFT_X) && mx<(x2+WINDOW_SHIFT_X) && my>=(y1+FONT_SHIFT_Y) && my<(y2+FONT_SHIFT_Y))	// click is in this box
    {
    play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
    mvalid=0;		// can only be valid for one
    mkeyprogram=i; 	// store the chosen one
    mkeyflash=0;	// always start flash from beginning
    return TRUE;
    }
  }
return FALSE;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION: Pass a mouse click message to be handled next frame
// +----------------------------------------------------------------ROUTINE HEADER----

void handle_key_entry_click(int x, int y)
{
mvalid=1;
mx=x-off_640_x;
my=y-off_480_y;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void handle_actual_key_entry(void)
{
int keycode, duplicate_slot;
static int rejected_keycode=-1;	// don't play sound twice

if(mkeyprogram!=-1)	// i.e. we want to enter a key
  {
  keycode=get_any_single_key();
  if(keycode != -1)
    {
    duplicate_slot=find_duplicated_key(keycode);
    if(duplicate_slot!=-1 && duplicate_slot!=mkeyprogram) // check if it is a repeat (but not on SAME key)
       {
       if(rejected_keycode!=keycode)	// don't play sound twice
         {
         play_zsound(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
         }
       rejected_keycode=keycode;
       }
    else	// not repeat key
       {
       rejected_keycode=-1;
       if(duplicate_slot==-1) { keys_changed_flag=1; }	// keys changed, except for same key in same slot...
	   load_single_key_def(key_names[mkeyprogram].keynumber, keycode); // store in real key map
       mkeyprogram=-1;	// only a valid key cancels entry
       }

    }	// end of if(there is a key)
  else
    {
    rejected_keycode=-1;
    }
    
  } // end of if (we are in key change mode)
  
} // end of handle_actual_key_entry()


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:  Returns key code, only if single key. Key code is as per think ref.
// +----------------------------------------------------------------ROUTINE HEADER----

int get_any_single_key(void)
{
unsigned char km[16];
int bit,byte,bitcount,bitmask;
int key_byte=0, key_bit=0;

GetKeys ((keymap_ptr)km);


bitcount=0;
for(byte=0; byte<16; byte++)
  {
  bitmask=1;
  for(bit=0; bit<8; bit++)
    {
    if(km[byte] & bitmask)
      {
      key_byte=byte; key_bit=bit;
      bitcount++;
      }
    bitmask<<=1;
    }
  }
if(bitcount!=1) return -1; // check only one key is pressed

// transform bit/byte value into keycode
return key_byte*8+key_bit;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION: Searches down all non-disabled keys looking for duplicate
// +----------------------------------------------------------------ROUTINE HEADER----

int find_duplicated_key(int keycode)
{
 int i;
 int enabled;
 
 i=0;
 
 enabled=key_names[i].enabled;
 while(enabled != -1)
   {
   if(enabled>=1)
     {
     if(keycode==get_programmed_keycode(key_names[i].keynumber))
       {
       return i;
       }
     } // end of if enabled   
   i++;
   enabled=key_names[i].enabled;
   } // end of while
   
return -1;	// key not found
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

char *get_key_name(int keycode)
{
int i,value;

i=0;
value=key_codes[i].value;

while(value!=-1)
  {
  if(keycode==value) break;
  i++;
  value=key_codes[i].value;
  }
  
return key_codes[i].string;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION: Accept -- saves in forth file (if changed) 
// +----------------------------------------------------------------ROUTINE HEADER----

void accept_keys(void)
{
int i;
int enabled;
char *output;
char *output_copy;
int *length;

if(!keys_changed_flag) { return; } // if no change, return

 i=0;

 output=(char *)interpret("keyfile_buffer set-param1");
 output_copy=output;
  
 kwrite(&output,"\\ Zex's Current Keys - Autogenerated File\x0d\x0dreset_key_data\x0d");
 kwrite(&output,"BASE @ HEX\x0d\x0d\x0d");
 
 enabled=key_names[i].enabled;
 while(enabled != -1)
   {
   if(enabled>=1)
     {
     kwrite(&output,key_names[i].forth_string);
     kwrite(&output,"  ");
     kwrite(&output,get_forth_key_name(get_programmed_keycode(key_names[i].keynumber)));
     kwrite(&output,"  load_key\x0d");
     } // end of if enabled
   
   i++;
   enabled=key_names[i].enabled;
   } // end of while

 kwrite(&output,"\x0d\x0d\x0dload_keys\x0d");
 kwrite(&output,"BASE !\x0d");
 
 length=(int *)interpret("keyfile_length set-param1");
 *length=output-output_copy;
 
 i=interpret("write_keys_to_disk set-param1");
 if(i) MyDebugStr(i,"Keys File Write Problem","Here"); 
 

}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void kwrite(char **output, char *string)
{
char *dest;

dest=*output;	// copy address of pointer to string for easier use

while(*string)	// while not at end of string
  {
  *dest=*string;	// copy the characters
  dest++; string++;	// and carry on
  }
  
*output=dest;	// update end address / address of next append
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

static char temp_number[3];

char *get_forth_key_name(int keycode)
{
int i,value;

i=0;
value=key_codes[i].value;

while(value!=-1)
  {
  if(keycode==value) break;
  i++;
  value=key_codes[i].value;
  }

// for unknown values, output the hex keycode
if(value==-1)
  {  
  i=keycode>>4; i&=0x0f;
  if(i>9) i+='A'-10; else i+='0';
  temp_number[0]=i;

  i=keycode&0x0f;
  if(i>9) i+='A'-10; else i+='0';
  temp_number[1]=i;

  temp_number[2]=0;
  return temp_number;
  }
    
return key_codes[i].forth_string;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION: Cancel/Revert -- causes forth to reload from file 
// +----------------------------------------------------------------ROUTINE HEADER----

void revert_keys(void)
{
interpret("init-directory include keys.def w-directory");
keys_changed_flag=0;	// now definitely hasn't changed!
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:Defaults -- loads hardcoded defaults as keyset  
// +----------------------------------------------------------------ROUTINE HEADER----

void load_default_keys(void)
{
interpret("include default_keys.zsl");
keys_changed_flag=1;	// too much effort to check if really changed, so just assume
}


// ***********************************************************************************
// * DESIGN NOTES
// *

