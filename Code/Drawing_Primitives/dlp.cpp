// dlp.c - new header by Rob.
// (It's brilliant mate...)
//
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"
  #include "options.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <mactypes.h>
  #endif
  
  #include "zex_misc.h"
  #include "Error_handler.h"
  #include "processor.h"
#endif //pch

#include "dlp.h"
#include "txtsplat.h"
#include "dlp_explosions.h"
#include "fcircle.h"
#include "pictures.h"
#include "ogl.h"
#include "olg_text.h"
#include "fpoly4.h"
#include "genpoly.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "splat_control.h"
#include "Zsound.h"


#define MAX_DLP_SIZE 30
#define DLP_FLASH_BIT 0x80000000
#define DLP_FLASH_RATE 0.5  //in seconds
//sub funcs of dlp
void draw_line(int start_x, int start_y, int end_x, int end_y, int colour);
void draw_weeny_laser(int start_x, int start_y, int end_x, int end_y);
void draw_nsweeny_laser(int x1, int y1, int x3, int y3);
void draw_laser(int x1, int y1, int x3, int y3);
void draw_tractor(int x1, int y1, int x3, int y3, int tractor_type);

void draw_line_no_clip(int x1, int y1, int x3, int y3, int colour);

void dlp_draw_box(int top, int left, int bottom, int right, int colour);
void dlp_floating_dialog(int x, int y, char * the_string);
void dlp_sleep_dialog(int x, int y, char * the_string);

int dlp_plot(int x, int y, int colour); //return 1 if clip else 0




typedef struct {
int the_command;
int param1,param2,param3,param4,param5;
char* the_ptr1;
float param7,param8,param9;	//typically normalised rgb
} dlp_command;


//the list of commands
dlp_command display_list[MAX_DLP_SIZE];

int dlp_add_index;

typedef struct {
char the_text[256];
int the_timer;
float red; float green; float blue;
int the_x;
} dlp_text_buffer_type;

#define NUMBER_OF_TEXT_LINES 7
dlp_text_buffer_type dlp_text_buffer[NUMBER_OF_TEXT_LINES];	//we cope with 6 lines
int dlp_text_buffer_insert_point;	//which of the 6 is the current start (so we dont have to copy stuff about)
 

char the_old_text[256];	//for compare so we don't get same message twice
int text_display_timer1,text_display_timer2,text_display_timer3;
char the_text[256];
char the_text2[256];
char the_text3[256];
Rect dlp_clip_rect;
static Rect normal_no_CP ={0,0,479,639};

void init_dlp()
{
int i;

normal_no_CP.top=0;
normal_no_CP.left=0;
normal_no_CP.bottom=monitor_h;
normal_no_CP.right=monitor_w;

the_old_text[0]=0;	//clear string=last string check string
text_display_timer1=text_display_timer2=text_display_timer3;
the_text[0]=0;
the_text2[0]=0;
the_text3[0]=0;
//init all the text buffers
for (i=0;i<NUMBER_OF_TEXT_LINES;i++) 
   {
   dlp_text_buffer[i].the_text[0]=0;
   dlp_text_buffer[i].the_timer=0;
   }
dlp_text_buffer_insert_point=0;

}

//add a command to the display list
void add_to_dlp(int command,int param1,int param2, int param3, int param4, int param5, char* param6,
                    float param7, float param8, float param9)
{
extern dlp_command display_list[30];
extern int dlp_add_index;
if (dlp_add_index<MAX_DLP_SIZE)
 {
  display_list[dlp_add_index].the_command=command;
  display_list[dlp_add_index].param1=param1;
  display_list[dlp_add_index].param2=param2;
  display_list[dlp_add_index].param3=param3;
  display_list[dlp_add_index].param4=param4;
  display_list[dlp_add_index].param5=param5;
  display_list[dlp_add_index].the_ptr1=param6;
  display_list[dlp_add_index].param7=param7;
  display_list[dlp_add_index].param8=param8;
  display_list[dlp_add_index].param9=param9;
  
  dlp_add_index++;
 }
}

//clear the display_list - called at end of run_dlp
void clear_dlp()
{
extern int dlp_add_index;
dlp_add_index=0;	//clear list index
}



//process the display list
void run_dlp()
{
extern int dlp_add_index;
//extern int showing_stats,freeze;
int dlp_index=0;
int command,p1,p2,p3,p4,p5;
char* the_ptr;

//set up the clipping rect

    set_dlp_clip_bounds(normal_no_CP);

// }
 
//if (showing_stats==1 || freeze==1) return;

//if (showing_stats==1) return;

//draw_line(1000,1000,-1480,-1630,32);


while (dlp_index<dlp_add_index)
  {
  command=display_list[dlp_index].the_command;
  p1=display_list[dlp_index].param1;
  p2=display_list[dlp_index].param2;
  p3=display_list[dlp_index].param3;
  p4=display_list[dlp_index].param4;
  p5=display_list[dlp_index].param5;
  the_ptr=display_list[dlp_index].the_ptr1;  
  dlp_index++;
  


  if (command==DRAW_LINE_NO_CLIP)	//draw a line from p1,p2 to p3,p4
  {
  draw_line_no_clip(p1,p2,p3,p4,p5);
  }

//  else
//  if (command==DRAW_TEXT)
//  {
//  SplatText_Transparent (the_ptr,p1,p2,p3);
//  }
  else
  if (command==DRAW_LASER1)	//draw a line from p1,p2 to p3,p4
  {
  draw_weeny_laser(p1,p2,p3,p4);
  }
  else
  if (command==DRAW_LASER2)	//draw a line from p1,p2 to p3,p4
  {
  draw_nsweeny_laser(p1,p2,p3,p4);
  }
  else
  if (command==DRAW_LASER3)	//draw a line from p1,p2 to p3,p4
  {
  draw_laser(p1,p2,p3,p4);
  }


  else
  if (command==DRAW_LINE)	//draw a line from p1,p2 to p3,p4
  {
  draw_line(p1,p2,p3,p4,p5);
  }

  else
  if (command==DRAW_TRACTOR)	//draw a line from p1,p2 to p3,p4
  {
  draw_tractor(p1,p2,p3,p4,p5);
  }

  else
  if (command==DRAW_FILLED_BOX)	//draw a line from p1,p2 to p3,p4
  {
  dlp_draw_box(p1,p2,p3,p4,p5);
  }


 }	//end of while

//finally clear out list
//clear_dlp();	
}

static float dlp_flash_timer=0;	//how fast we flash red text
static int dlp_flash_toggle=0;	//the flasher toggle
static int flash_sound_flag=0;

//process the display list text items and clear
//run AFTER render_frame
void run_dlp_text()
{
extern int dlp_add_index;
int dlp_index=0;
int command,p1,p2,p3,p4,p5;
char* the_ptr;
float fade;
//draw_line(1000,1000,-1480,-1630,32);

	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency

dlp_flash_timer+=real_frame_time_in_secs;
if (dlp_flash_timer>DLP_FLASH_RATE)
{
 dlp_flash_toggle^=1;
 dlp_flash_timer=0;
 if (dlp_flash_toggle==1) flash_sound_flag=1;
 
}


while (dlp_index<dlp_add_index)
  {
  command=display_list[dlp_index].the_command;
  
  if (command==DRAW_TEXT)
  {
    p1=display_list[dlp_index].param1;
    p2=display_list[dlp_index].param2;
    p3=display_list[dlp_index].param3;
    if (p3&DLP_FLASH_BIT)
	{
     p4=1;	//flash flag
	 if (flash_sound_flag) 
	 {     
	   play_zsound(bleeper, sound_high_pri, c1_chan1, sound_vol_2); 
	   flash_sound_flag=0;
	 }
	}
	else p4=0;
	p3&=~DLP_FLASH_BIT;
    
    fade=(float)p3/(float)TEXT_DISPLAY_TIME;

 	  the_ptr=display_list[dlp_index].the_ptr1;  
	  glColor4d(display_list[dlp_index].param7, display_list[dlp_index].param8, display_list[dlp_index].param9,fade);
	  OGL_moveto(p1,p2);
	  
	 if ((p4==1) && (dlp_flash_toggle==0))	//if flash
	 {
     }
	 else //not flash or flash toggle is true
	 {
	  OGL_draw_helvetica_small(the_ptr);	//x,y,colour
     }
  }  
  
  else
  if (command==DRAW_OUTER_FLARE)
  {
  p1=display_list[dlp_index].param1;
  p2=display_list[dlp_index].param2;
  p3=display_list[dlp_index].param3;
  p4=display_list[dlp_index].param4;
  create_outer_flare(p1,p2,p3,p4);
  }
  else
  if (command==DRAW_RADAR_LINE)	//draw a line from p1,p2 to p3,p4
  {
  p1=display_list[dlp_index].param1;
  p2=display_list[dlp_index].param2;
  p3=display_list[dlp_index].param3;
  p4=display_list[dlp_index].param4;
  p5=display_list[dlp_index].param5;

  draw_line_no_clip(p1,p2,p3,p4,p5);
  }
 
  else
  if (command==DRAW_DLP_EXPLOSION)
  {
  p1=display_list[dlp_index].param1;
  p2=display_list[dlp_index].param2;
  p3=display_list[dlp_index].param3;
  p4=display_list[dlp_index].param4;
  p5=display_list[dlp_index].param5;
  draw_dlp_explosion(p1,p2,p3,p4,p5);
  }
  else
  if (command==FLOATING_DIALOG)	//draw a line from p1,p2 to p3,p4
  {
  p1=display_list[dlp_index].param1;
  p2=display_list[dlp_index].param2;
  the_ptr=display_list[dlp_index].the_ptr1;  
    dlp_floating_dialog(p1,p2,the_ptr);
  }

  else
  if (command==SLEEP_DIALOG)	//draw a line from p1,p2 to p3,p4
  {
  p1=display_list[dlp_index].param1;
  p2=display_list[dlp_index].param2;
  the_ptr=display_list[dlp_index].the_ptr1;  
    dlp_sleep_dialog(p1,p2,the_ptr);
  }
  

  dlp_index++;

 }	//end of while

//finally clear out list
clear_dlp();
glDisable (GL_BLEND);
	
}



//the purpose of this code is to draw what looks like the top half of a tube.
//we do this by defining 10 items - each contains a colour
//and a length addition  (in 100ths) which is added onto each of the lines we draw
typedef struct
{
int colour;
int length;
} earth_tractor_cl;

//do NOT put zeros in the length field for cross platform purposes
earth_tractor_cl earth_tractor_def [10] =
     {
       { 240,7, },
       { 239,6, },
       { 238,4, },
       { 237,2, },
       { 236,1, },
       { 235,1, },
       { 236,2, },
       { 237,4, },
       { 238,6, },
       { 239,7, }
     };

earth_tractor_cl alien_tractor_def [10] =
     {
       { 230,7, },
       { 229,6, },
       { 228,4, },
       { 227,2, },
       { 226,1, },
       { 225,1, },
       { 226,2, },
       { 227,4, },
       { 228,6, },
       { 229,7, }
     };


static int external_count=0;

void draw_tractor(int x1, int y1, int x3, int y3, int tractor_type)
{
int x_temp,y_temp,i,colour;

  external_count++;
  if (external_count>21) external_count=0;

if (tractor_type==0)
{
 for(i=0;i<10;i++)
 {
 x_temp=x3-x1;	//change in x
 x_temp*=100;
 x_temp/=earth_tractor_def[i].length;
 x_temp/=1000;
 x_temp=x_temp>>3;
 x_temp+=x3+(i);
 
 y_temp=y3-y1;	//change in y
 y_temp*=100;
 y_temp/=earth_tractor_def[i].length;
 y_temp/=1000;
 y_temp=y_temp>>4;
 y_temp+=y3+(i);
 
// if (RangedRdm(1,100)>96) colour=RangedRdm(215,222);	  //little random red fleck
 if (i==external_count) 
 {
  colour=RangedRdm(215,222);
 }
	else
 colour=earth_tractor_def[i].colour;
 
 draw_line(x1,y1,x_temp,y_temp,colour);
 }
}
else	//alien tractor beam
{
 for(i=0;i<10;i++)
 {
 x_temp=x3-x1;	//change in x
 x_temp*=100;
 x_temp/=alien_tractor_def[i].length;
 x_temp/=1000;
 x_temp=x_temp>>3;
 x_temp+=x3+(i);
 
 y_temp=y3-y1;	//change in y
 y_temp*=100;
 y_temp/=alien_tractor_def[i].length;
 y_temp/=1000;
 y_temp=y_temp>>4;
 y_temp+=y3+(i);
 
// if (RangedRdm(1,100)>96) colour=RangedRdm(215,222);	  //little random red fleck
 if (i==external_count) 
 {
  colour=RangedRdm(235,242);
 }
	else
 colour=alien_tractor_def[i].colour;
 
 draw_line(x1,y1,x_temp,y_temp,colour);
 }
}

}



void draw_weeny_laser(int x1, int y1, int x3, int y3)
{

x1--; y1--;
draw_line(x1,y1,x3,y3,228);	//green
x1++; y1++;
draw_line(x1,y1,x3,y3,245);	//white
x1++; y1++;
draw_line(x1,y1,x3,y3,228);

}


void draw_nsweeny_laser(int x1, int y1, int x3, int y3)
{

x1--; y1--;
draw_line(x1,y1,x3,y3,235);	//blue
x1++; y1++;
draw_line(x1,y1,x3,y3,245);	//white
x1++; y1++;
draw_line(x1,y1,x3,y3,235);

}

void draw_laser(int x1, int y1, int x3, int y3)
{

x1--; y1--;
draw_line(x1,y1,x3,y3,215);	//red
x1++; y1++;
draw_line(x1,y1,x3,y3,245);	//white
x1++; y1++;
draw_line(x1,y1,x3,y3,215);

}


//set up dlp clipping bounds - public function
//should be called at init and whenever the bounds change, such as when the control panel is invisible
void set_dlp_clip_bounds(Rect the_bounds)
{
dlp_clip_rect=the_bounds;
}

//sub funcs of dlp
//draw a filled box - does no clipping so can crash if called with bad params
void dlp_draw_box(int top, int left, int bottom, int right, int colour)
{
int line_loop_ctr;

for (line_loop_ctr=top;line_loop_ctr<bottom;line_loop_ctr++)
 {
 draw_line_no_clip(left, line_loop_ctr, right, line_loop_ctr, colour);
 }
 
}


//draw a unfilled box - does no clipping so can crash if called with bad params
void dlp_draw_unfilled_box(int top, int left, int bottom, int right, int colour)
{
draw_line_no_clip(left, top, right, top, colour);
draw_line_no_clip(left, bottom, right, bottom, colour);
draw_line_no_clip(left, top, left, bottom, colour);
draw_line_no_clip(right, top, right, bottom, colour);
}

//draw a filled rounded box - does no clipping so can crash if called with bad params

void dlp_draw_rounded_box(int top, int left, int bottom, int right, int colour)
{
int line_loop_ctr;

draw_line_no_clip(left+1, top, right-1, top, colour);

for (line_loop_ctr=top+1;line_loop_ctr<bottom;line_loop_ctr++)
 {
 draw_line_no_clip(left, line_loop_ctr, right, line_loop_ctr, colour);
 }

draw_line_no_clip(left+1, bottom, right-1, bottom, colour);
 
}


void draw_line(int x1, int y1, int x3, int y3, int colour)
{
//int dx,dy,x2,y2,xstep,ystep,e,clip;
//int right,bottom;
//

if (single_line_clip(&x1,&y1,&x3,&y3)==0) return;
    draw_line_no_clip(x1,y1,x3,y3,colour);
}


void draw_line_no_clip(int x1, int y1, int x3, int y3, int colour)
{
OGL_line( x1, y1, x3, y3, colour);
}


void Zline_rgb(int x1, int y1, int x2, int y2, float red, float green, float blue)
{
 OGL_line_rgb(x1, y1, x2, y2, red, green, blue);
}


int dlp_plot(int x, int y, int colour)
{
extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
Ptr pix;
MyDebugStr(__LINE__,"dlp.c","dlp_plot");


    if (x<dlp_clip_rect.left) return 1;
    if (x>dlp_clip_rect.right) return 1;
    if (y<dlp_clip_rect.top) return 1;
    if (y>dlp_clip_rect.bottom) return 1;
    pix=(char*)screenaddr+(y*screenwidth)+x;	//screenaddr
    *pix=colour;
return 0;
}


//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
int text_x,text_x2,text_x3;
int colour1,colour2,colour3;


void add_to_text_display(Ptr text_in,int colour)
{
char the_char=13;
char temp_char;
//char temp_char1;
int count=0;
//int match=0;
int i,screen_center;	//good old i
float red,green,blue;
extern int OGL_active;
int flash;

screen_center=screen_cent_x;

//compare to old text and get out if =
//this is a different compare to what I normally write...
//for (i=0;i<255;i++)
//{
//  temp_char1=the_old_text[i];
//  temp_char=text_in[i];
//  if (temp_char1==0) break;	  //eos
//  if (temp_char!=temp_char1) goto strings_no_match;
//}
//  if(temp_char1==0 && temp_char==0) return;
//strings_no_match:
//copy to old text
//cant use a while cause of length protection...
for (i=0;i<255;i++)
{
  the_old_text[i]=temp_char=text_in[i];
  if(temp_char==0) break;
}



  while(the_char!=0 && count<255)
  {
    if (OGL_active==1)
    {
    the_char=text_in[count];	
//    if (the_char!=0) the_char++; //our font is off by 1
    }
    else
    {
     the_char=text_in[count];
//convert to upper case
     if (the_char>='a') the_char-=0x20;
    }
        
    dlp_text_buffer[dlp_text_buffer_insert_point].the_text[count]=the_char;
    count++;
  }
//  }
  
//calc text x - center the string
//the font is 8 pixels wide
#if MESSAGES_CENTER
  text_x=(count>>1)*8;
  text_x=screen_center-text_x;
  if (text_x<0) text_x=0;
#else
  text_x=1;
#endif

flash=0;
if (colour==DLP_RED) { red=1; green=0;blue=0; flash=1; }
else
if (colour==DLP_PURP) { red=1; green=0;blue=1; }
else
if (colour==DLP_YELLOW) { red=1; green=1;blue=0; }
else
if (colour==DLP_GREEN) { red=0; green=1;blue=0; }
else
{ red=1; green=1; blue=1; }

dlp_text_buffer[dlp_text_buffer_insert_point].red=red;
dlp_text_buffer[dlp_text_buffer_insert_point].green=green;
dlp_text_buffer[dlp_text_buffer_insert_point].blue=blue;
dlp_text_buffer[dlp_text_buffer_insert_point].the_x=text_x;
if (flash==0)
{
 dlp_text_buffer[dlp_text_buffer_insert_point].the_timer=TEXT_DISPLAY_TIME;
}
else
{
 dlp_text_buffer[dlp_text_buffer_insert_point].the_timer=TEXT_DISPLAY_TIME|DLP_FLASH_BIT;	//set flash bit
}

dlp_text_buffer_insert_point++;	//drawing takes place
if (dlp_text_buffer_insert_point>NUMBER_OF_TEXT_LINES-1) dlp_text_buffer_insert_point=0; 

}

void reset_dlp_text_display()
{
int i;
for (i=0;i<NUMBER_OF_TEXT_LINES;i++) dlp_text_buffer[i].the_timer=0;

//text_display_timer1=0;
//text_display_timer2=0;
//text_display_timer3=0;

}


void display_text_display()
{

//int colour;	//max bright
int i,flash;
int index;
int y_pos;
int text_display_timer;
//this is a clever bit
//we want a scrolling list of text in chronological order, fading out as it expires
//the text has been inserted in a circular list
//to get the list printed chornologically (oldest at top), we print the list backwards
//taadaah.
//so if dlp_text_buffer_insert_point==0, we print 5,4,3,2,1,0
//if it is 3, we print 2,1,0,5,4,3
//etc


//OGL_draw_helvetica("Test",0,300,10);


y_pos=monitor_h-19;	//dlp_clip_rect.bottom-19;

index=dlp_text_buffer_insert_point;


for (i=0;i<NUMBER_OF_TEXT_LINES;i++)
{
//dec index
 index--;
 if (index<0) index=NUMBER_OF_TEXT_LINES;
 //print the line if timer > 0
 text_display_timer=dlp_text_buffer[index].the_timer;
 if (text_display_timer&DLP_FLASH_BIT) flash=1;
 else
 flash=0;
 
 text_display_timer&=~DLP_FLASH_BIT;
 
 if (text_display_timer!=0)
     {
     //calc correct colour    

//     if (dlp_text_buffer[index].the_colour==0) colour=225;	  //green
//     else
//     if (dlp_text_buffer[index].the_colour==1) colour=215;
//     else colour=245;	//red
//     //calc colour fade according to time
//     if (text_display_timer<128)
//     colour+=((9-text_display_timer/16));


     //finally issue the command to draw the text
     add_to_dlp(DRAW_TEXT,dlp_text_buffer[index].the_x,
                y_pos,dlp_text_buffer[index].the_timer,flash,0,dlp_text_buffer[index].the_text,
                dlp_text_buffer[index].red,dlp_text_buffer[index].green,dlp_text_buffer[index].blue);
     
     y_pos-=15;
     dlp_text_buffer[index].the_timer--;	//ready for next frame
 
     }
}

return;
}

//takes the string from global_dialog_string
//sets gdlp_button if mouse clicked over the button
void dlp_floating_dialog(int x, int y, char * the_string)
{
extern LSRAW *float_dialog;
int print_x,print_y,src_pos,dest_pos;
int terminate=0;
char the_line [300];
char the_char;


	print_crunched_trans_pict(float_dialog,x,y);
//draw the text
//get each line and print it, terminate on zero
print_y=20+y;
src_pos=0;
while(terminate==0)
    {
    print_x=16+x;	//newline
    dest_pos=0;
    //get line
gl_loop: the_char=the_string[src_pos];
    the_line[dest_pos]=the_char;
    src_pos++;
    dest_pos++;
    if (the_char==0) { terminate=1; goto got_line;}
    if (the_char!=13) goto gl_loop;
//we have the line
got_line:  the_line[dest_pos-1]=0;
    
     SplatText_Transparent (the_line,print_x,print_y,246);
     print_y+=16;
    }

}

void dlp_sleep_dialog(int x, int y, char * the_string)
{
extern LSRAW *sleep_dialog;
int print_x,print_y,src_pos,dest_pos;
int terminate=0;
char the_line [300];
char the_char;


	print_crunched_trans_pict(sleep_dialog,x,y);
//draw the text
//get each line and print it, terminate on zero
print_y=20+y;
src_pos=0;
while(terminate==0)
    {
    print_x=16+x;	//newline
    dest_pos=0;
    //get line
gl_loop: the_char=the_string[src_pos];
    the_line[dest_pos]=the_char;
    src_pos++;
    dest_pos++;
    if (the_char==0) { terminate=1; goto got_line;}
    if (the_char!=13) goto gl_loop;
//we have the line
got_line:  the_line[dest_pos-1]=0;
    
     SplatText_Transparent (the_line,print_x,print_y,246);
     print_y+=16;
    }

}