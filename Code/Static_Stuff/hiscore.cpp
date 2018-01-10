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

//hiscore.c
//SB 070299
#include <mactypes.h>
#include <resources.h>
#include <NumberFormatting.h>

#include "LS_Standard.h"
#include "3d_structures_def.h"
#include "objects_controls.h"
#include "fpoly4.ext"
#include "load_objects.ext"
#include "compile_zmap.ext"
#include "zex_misc.ext"

#include "ship_control.ext"
#include "input_control.ext"
#include "splat_control.ext"
#include "animation.ext"
#include "Error_handler.ext"
#include "object_control_top.ext"
#include "hiscore.ext"
#include "file_io.ext"
#include	"txtsplat.h"
#include "dlp.ext"
#include "zex_misc.ext"
#include "pictures.ext"
#include	"camera.ext"
#include "splat_control.ext"
#include "sound.ext"
#include "obj_control_misc_routines.ext"
render_frame(int width, int height);
void draw_hiscores();
//sub funcs of dlp
void draw_line(int start_x, int start_y, int end_x, int end_y, int colour);


do_hiscore_screen()
{

extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;
extern UInt32 gLaser,gRoll,gPitch;
extern UInt32 gCannon;
extern	float SinTable[360], CosTable[360];


CGrafPtr the_drawing_buffer;
GDHandle	saveGDH;
GrafPtr		savePort;
load_dyn_object_pb the_params;

int i;	//good old i
long last_button_press_time=0;
Handle shield_data_H;

LSRAW *the_picture_shield;
int y_inc=-1;
int x1=220,y1=160;	//circle center
int y=0;
int picture_x,picture_y;
int r=52,r_inc=0;	//radius
int scaling_factor=2;
float scale_count=40;

clear_pb(&the_params);	//saves loads of zero code



while (game_fire_button()==1);
//    reset_camera();
//    
////load in 100 stars
    reset_objects();	//clear out ocb's
//    load_object ("\pshield1.z",0,'ZEXS',no_colldet,'MZ01');
    load_object ("\pstar.z",0,'ZEXS',no_colldet,0,0);
////load in the shield
//	  the_params.world_x=0;
//	  the_params.world_y=0;
//	  the_params.world_z=-1000;
//	  the_params.Omovedx=0;
//	  the_params.Omovedy=0;
//	  the_params.Omovedz=0;
//	  the_params.delta_rot_x=0;
//	  the_params.delta_rot_y=0;
//	  the_params.delta_rot_z=1;
//	  the_params.rot_x=0;
//	  the_params.rot_y=0;
//	  the_params.rot_z=0;
//	  the_params.controller_ref=0;	  //these zoom towards us
//	  the_params.control_use_1=0;
//	  the_params.control_use_2=0;
//	  the_params.control_use_3=0;
//	  the_params.control_use_4=0;
//	  the_params.current_velocity=0;
//	  the_params.wanted_velocity=0;
//	  the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	  the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	  the_params.shield_value=0;
//	  the_params.mass=100;
//	  the_params.no_trivial_rejection=0;
//	      the_params.object_category=baddie_object;
//
//	  load_dyn_object(0,0,&the_params);	//the object, the position (-1=next free)    


    for (i=0;i<50;i++)
    {
	the_params.world_x=RangedRdm(1,4000)-2000;
	the_params.world_y=RangedRdm(1,4000)-2000;
	the_params.world_z=RangedRdm(1000,3000);
//	the_params.Omovedx=0;
//	the_params.Omovedy=0;
	the_params.Omovedz=-RangedRdm(15,50);
//	the_params.delta_rot_x=0;
//	the_params.delta_rot_y=0;
//	the_params.delta_rot_z=0;
//	the_params.rot_x=0;
//	the_params.rot_y=0;
	the_params.rot_z=RangedRdm(1,359);
	the_params.controller_ref=star_for_story;	//these zoom towards us
//	the_params.control_use_1=0;
//	the_params.control_use_2=0;
//	the_params.control_use_3=0;
//	the_params.control_use_4=0;
//	the_params.control_use_7=0;
//	the_params.current_velocity=0;
//	the_params.wanted_velocity=0;
//	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	the_params.shield_value=0;
//     the_params.has_roll=0;
	the_params.mass=100;
//	the_params.dq=0;
	the_params.no_trivial_rejection=1;
	    the_params.object_category=baddie_object;
//	the_params.unique_id=0;
     the_params.who_spawned_this_object=-1;
	load_dyn_object(0,i,&the_params,-1);	//the object, the position (-1=next free)    
    }

//set up drawing env.
    set_poly_clipping(0,639,0,460);	//set poly clip rect - sb 181298

    shield_data_H=GetZexPicture ('RCZA',128);	//shield pict   
	HLock(shield_data_H);
	the_picture_shield = (LSRAW*)*shield_data_H;
   
    while (game_fire_button()==0)
    {

	the_drawing_buffer=Get_back_buffer();
//	SetPort((GrafPtr)the_drawing_buffer);
//    GetPort (&savePort);
//	  saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//	  SetGDevice(mZexGDH);
//	  RGBBackColor (&rgbBlack);
//	  EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect(640,480);	//in pictures
//	  SetGDevice(saveGDH);	  //these are here so I can SEE them!
//	  SetPort(savePort);
 //   screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
 //   screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;

    render_frame(639,460);	//calls all the needed 3d stuff
    scaling_factor=scale_count;
    if (scale_count>0)
    scale_count-=0.5;
    picture_x=x1+r*COS(y);
    picture_y=y1+r*SIN(y);
    print_pict_scaled(the_picture_shield,picture_x,picture_y,scaling_factor,0);
    y+=y_inc;
    if (y>359) y-=360; 
    if (y<0) y+=360;
//    r=r+r_inc;
//    if (r>80) 
//    {
//    r_inc=-r_inc;
//    y_inc=-y_inc;
//    }
//    if (r<10) r_inc=-r_inc;
    
//    print_pict(the_picture_shield,130,80);

    draw_hiscores();	//show the high scores
//print the strings
    do_object_control();
	Show_it();
    }	//end of gcannon
    
   
   
    set_poly_clipping(0,356,0,440);	//set poly clip rect - sb 181298
    gLaser=0;
    gCannon=0;
    reset_objects();	//clear out ocb's

	 DisposeHandle(shield_data_H);
    while (game_fire_button()==1);


}


void draw_hiscores()
{
extern prefs gzex_prefs;
extern int weird_count,wc_inc;
extern int weird_countb,wc_incb;
int i,line;
unsigned char number_string[12];

weird_count+=wc_inc;
if (weird_count>210) 
    { 
    weird_count=209;
    wc_inc=-wc_inc;
    }
    
if (weird_count<1) 
   {
   weird_count=1;
   wc_inc=-wc_inc;
   }
   
weird_countb+=wc_incb;

if (weird_countb>210) 
    {
    wc_incb=-wc_incb;
    weird_countb=209;
    }
    
if (weird_countb<1) 
   {
   weird_countb=1;
   wc_incb=-wc_incb;
   }
line=30;

for (i=0; i<10; i++)
	{
	 NumToString(gzex_prefs.the_scores[i].score,number_string);
	 number_string[number_string[0]+1]=0;	//quick p to c hack
	 number_string[0]=' ';
//	   SplatTextLarge_weird ((char *) number_string,390,line);
//
//     SplatTextLarge_weirdB (gzex_prefs.the_scores[i].name,100,line);


	 SplatTextLarge_Transparent ((char *) number_string,390,line);

     SplatTextLarge_Transparent (gzex_prefs.the_scores[i].name,100,line);

     line+=40;

	}
SplatText_Transparent ("PRESS FIRE TO CONTINUE",220,20,33);

}

//this routine examines a score and enters it if it's higher than any of the current top ten
//SB 8/2/99
void handle_hiscore()
{
extern prefs gzex_prefs;
extern int score;

int i;

for (i=0;i<10;i++)
   {
   if (score>gzex_prefs.the_scores[i].score)
     {
     insert_score(score,i);
     do_hiscore_screen();
     break;
     }
   }

}



draw_box(int new_pos);

void insert_score(int the_score,int score_pos)
{
extern prefs gzex_prefs;
extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;
extern GDHandle				mZexGDH;
char the_name[63];
char my_string[2];
CGrafPtr the_drawing_buffer;
GDHandle	saveGDH;
GrafPtr		savePort;
extern UInt32 gRoll;	//-1,0,1
extern UInt32 gLaser;	//0,1
extern UInt32 gCannon;	//0,1
extern UInt32 gThrustUp;
extern UInt32 gThrustDown;
int i;	//good old i
int x,y,name_pos,char_pos,button_up,right_up,left_up,Up_up,Down_up;
int end=0;
int did_flag=0;	//key delay flag
extern int weird_count,wc_inc;
extern int weird_countb,wc_incb;



   name_pos=0;	//in string counter
   char_pos=0;

   right_up=left_up=button_up=Up_up=Down_up=1;
   the_name[0]='\0';

gRoll=0x7fffffff;
gCannon=0;
gLaser=0;
gThrustUp=0;
gThrustDown=0;

while (game_fire_button()==1);	//wait for button up
while(end==0)
{
weird_count+=wc_inc;
if (weird_count>330) wc_inc=-wc_inc;
if (weird_count<1) wc_inc=-wc_inc;
    my_string[1]='\0';
    GetPort (&savePort);
	saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
	the_drawing_buffer=Get_back_buffer();
//	SetPort((GrafPtr)the_drawing_buffer);
	SetGDevice(mZexGDH);
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;
	RGBBackColor (&rgbBlack);
//	my_flashy_erase_back();

//	EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect(640,480);	//in pictures


	do_down_bars();


	SetGDevice(saveGDH);	//these are here so I can SEE them!
	SetPort(savePort);
//print char set
    x=80;y=80;
    for (i=' '; i<'[';i++)
     {
     if (x>540)
        {
        x=80;
        y=y+40;
        }
     my_string[0]=i;
     SplatTextLarge_Transparent(my_string,x,y);
     x=x+40;
     } 
     y+=20;
     
     SplatText_Transparent ("DEL",x,y,10);
     x=80;
     y+=40;
     SplatText_Transparent ("OK",x,y,10);
//now draw the name string
     SplatTextLarge_Transparent (the_name,120,y+40);

   
//now watch left and right keys and fire.
//draw box around selected char
   draw_box(char_pos);

   do_up_bars();
	
   Show_it();
   if (did_flag==1)
   {
//   user_delay(10);
   did_flag=0;
   }
   gCannon=0;
   gLaser=0;
   gRoll=0x7fffffff;
   gPitch=0x7fffffff;
   
   user_delay(2);

//wait for left, right or fire
//while(gCannon==0 && gTurn==0x7fffffff)
//{
//poll_input_device();
//}

poll_input_device();

if (gRoll<0x70000000 && left_up==1)
 {
  if (char_pos>0) char_pos--;
  else char_pos=60;	//wrap
  left_up=0;
 }

 if (gRoll>0x90000000 && right_up==1)
 {
  if (char_pos<60) char_pos++;
  else char_pos=0;	 //wrap
  right_up=0;
 }

//up by 12
if (gThrustUp==1 && Up_up==1)
 {
  if (char_pos<60-11) char_pos+=12;
  else char_pos=char_pos+12-60;	//wrap
  Up_up=0;
 }

//down by 12
if (gThrustDown==1 && Down_up==1)
 {
  if (char_pos>11) char_pos-=12;
  else char_pos=char_pos-12+60;	//wrap
  Down_up=0;
 }

if (gThrustUp==0) Up_up=1;
if (gThrustDown==0) Down_up=1;

 if (game_fire_button()==1 && char_pos==60) end=1;

 if (game_fire_button()==0) button_up=1;

 if (gRoll>0x70000000 && gRoll<0x90000000)
 {
  left_up=1; right_up=1;	//left/right is centered
 }
  
 if (game_fire_button()==1 && char_pos!=60 && char_pos!=59) 
 {
  if (name_pos>=NAME_LEN) play_zsound_always(low_beep, sound_high_pri, ls1_chan, sound_vol_7);
  else
  if (button_up==1)
  {
   the_name[name_pos]=char_pos+32;	//33?
   the_name[name_pos+1]=0;
   name_pos++;
   did_flag=1;
   button_up=0;
  }

  
 }
 
 if ((game_fire_button()==1) && char_pos==59)	 //delete
  {
   if (name_pos>0 && (button_up==1))
   {
    name_pos--;
    the_name[name_pos]=0;	
    did_flag=1;
    button_up=0;
   }
  }
}


//now insert the name into the hiscores array and save 'em
//copy from 10 to score_pos
for (i=9;i>score_pos;i--)
 {
 strcpy(gzex_prefs.the_scores[i].name,gzex_prefs.the_scores[i-1].name);
 gzex_prefs.the_scores[i].score=gzex_prefs.the_scores[i-1].score;
 }
strcpy(gzex_prefs.the_scores[score_pos].name,the_name);
gzex_prefs.the_scores[i].score=the_score;
gCannon=0;
gLaser=0;
}









int box_colour=215;

draw_box(int new_pos)
{
int top,left;
int x,y;
//calc top left of old box
top=85;
left=70;
y=new_pos/12;
x=new_pos-(y*12);	//the remainder
x*=40;
y*=40;
x+=left;
y+=top;
draw_line(x,y,x+40,y,box_colour);	//dlp function
draw_line(x+40,y,x+40,y+40,box_colour);	//dlp function
draw_line(x,y+40,x+40,y+40,box_colour);	//dlp function
draw_line(x,y,x,y+40,box_colour);	//dlp function

box_colour++;
if (box_colour>=245) box_colour=215;

if (box_colour==223 ||
    box_colour==233 ||
    box_colour==243) box_colour+=2;	//stop from getting too dark
}

