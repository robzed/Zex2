
// Name:   		TxtSplat.c
// Description:		This routine, reads in a string and the 
//			location of where it needs to be printed
//			on screen.
// Date Started:	25/11/98
// Author(s):		ID/RP/SB
//
// Comments:		This routine has a pointer to the start of
//			a resource (the alphabet). It reads in a 
//			string and for each letter calculates the offset
//			in the resource and copies that 'letter' to
//			an off-screen buffer.
//			The buffer will be updated on a screen refresh.
//
// Limitations:		The Alphabet only has Upper Case Characters
//			so when a string is sent it needs to be in
//			upper case characters (We use Ascii Value - 65)
//			to calculate offset for letters.
//  
// Copyright(c) Lightsoft Software   		
//
// Revs:
// 2/12/98 SB - Removed base_addr and width as params and ref'd to externs calc'd on page flip
//              in draw_it.
//              Check if ZRAW resource got else error (text looks bad without it <G>).
// $Log: textsplat.c,v $
// Revision 1.2  2003/09/22 21:44:23  robp
// Box of Curios 1: Code tidy up things, which weren't strictly incorrect, just weird. No functional changes. Examples include a block of variables that were declared twice. These things were all thrown up by the C++ compiler.
//
// Revision 1.1.1.1  2003/09/05 22:35:21  stu_c
// First Imported.
//
// Revision 1.7  2002/09/08 02:08:37  stu_c
// Precompiled header adjustments
//
// Revision 1.6  2002/04/28 06:24:07  stu
// This checkin adds more to the HUD and brings out some sim variables to the player.
//
// Revision 1.5  2001/11/09 18:29:01  rob
// Various debug changes.
//
// Revision 1.4  2001/11/02 20:00:46  rob
// PB build changes
//
// Revision 1.3  2001/11/02 19:32:02  rob
// Build variants for interface lib and carbon.
// Coffee fixes after changes for X.
//
// Revision 1.2  2001/10/25 21:57:54  stu
// reinstated some code removed for initial build
//
// Revision 1.1  2001/10/24 22:05:56  stu
// Initial issue
//
// Revision 1.3  2001/10/23 20:45:25  stu
// More carbonising
//
// Revision 1.2  2001/10/22 21:28:24  rob
// Carbon warning changes
//
// Revision 1.1  2001/10/21 01:07:03  stu
// Initial porting work
//
// Revision 1.0.0.1  2001/10/17 20:46:04  rob
// First Imported.
//
// Revision 1.3  2001/05/28 01:49:27  stu
// 280501
//
// Revision 1.2  2001/01/02 18:57:32  stu
// OGL redirection removed in small_text
//
// Revision 1.1.1.1  2001/01/01 21:13:35  rob
// First Imported.
//
// Revision 1.1  2000/09/13 20:34:53  stu
// Changes for hud targetting and sun corona effects.
//
// Revision 1.0.0.1  2000/08/21 22:01:25  stu
// First Imported.
//
 //


#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "3d_structures_def.h"
  #include "Error_handler.h"
  #include 	"options.h"
#endif

#include "txtsplat.h"
#include "olg_text.h"
#include "fpoly4.h"
#include "genpoly.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "pictures.h"
#include "ogl.h"
#include "file_io.h"
#include "console.h"

//#define CHAR_PIXSIZE 56  // in Pixels to the next character  
//#define CHAR_COLUMN_SIZE 7
//#define CHAR_LINES 8

#define CHAR_PIXSIZE 7*16  // in Pixels to the next character  
#define CHAR_COLUMN_SIZE 7	//width
#define CHAR_LINES 12	//how many lines to splat

#define SML_PIXSIZE 5*6
#define SML_DIMENSION 5

/*
module_private void Draw_to_Double_Buffer_Transparent(char *char_ptr, int x_pos, int y_pos, unsigned char colour);
module_private void Draw_to_Double_Buffer_Small(char *char_ptr, int x_pos, int y_pos , unsigned char fg_colour,unsigned char bg_colour);
*/

//module_private void Draw_to_Double_Buffer_Transparent(char *char_ptr, int x_pos, int y_pos, unsigned char colour);
module_private void Draw_to_Double_Buffer(char *char_ptr, int x_pos, int y_pos, unsigned char fg_colour, unsigned char bg_colour);
//module_private void Draw_to_Double_Buffer_Small(char *char_ptr, int x_pos, int y_pos , unsigned char fg_colour,unsigned char bg_colour);


 Ptr double_buffer;
 int screen_width;
 #if PORTABLE_FILESYSTEM
 void *font_pointer;
 #else
 Handle hpic;
 #endif
 Ptr medium_font;
 Ptr tiny_font;
 #if PORTABLE_FILESYSTEM
 void *small_font_pointer;
 void *large_font_pointer;
 #else
 Handle small_font_h;
 Handle large_font_h;
 #endif
Ptr large_font;
void InitText()
 {
 #if PORTABLE_FILESYSTEM
 
 font_pointer = ZGetResource('JESS',129, NULL);  //Get the Handle to the Resource 
 if (font_pointer==NULL) report_error("Resource missing: JESS 129","\p.",4);
 medium_font = font_pointer;
 
 large_font_pointer = ZGetResource('JESS',128, NULL);  //Get the Handle to the Large font Resource 
 if (large_font_pointer==NULL) report_error("Resource missing: JESS 128","\p.",4);
 large_font = large_font_pointer; 

 small_font_pointer = ZGetResource('JESS',130,NULL);  //Get the Handle to the Large font Resource 
 if (small_font_pointer==NULL) report_error("Resource missing: JESS 130","\p.",4);
 tiny_font = small_font_pointer; 
 
 #else
// hpic = (Handle) ZGetResource('ZRAW',128);  //Get the Handle to the Resource 
 hpic = (Handle) ZGetResource('JESS',129);  //Get the Handle to the Resource 
 if (hpic==0) report_error("Resource missing: ZRAW 128","\p.",4);
 HLock(hpic);
 medium_font=*hpic;
 
 large_font_h = (Handle) ZGetResource('JESS',128);  //Get the Handle to the Large font Resource 
 if (large_font_h==0) report_error("Resource missing: JESS 128","\p.",4);
 HLock(large_font_h);
 large_font=*large_font_h; 

 small_font_h = (Handle) ZGetResource('JESS',130);  //Get the Handle to the Large font Resource 
 if (small_font_h==0) report_error("Resource missing: JESS 130","\p.",4);
 HLock(small_font_h);
 tiny_font=*small_font_h; 
 #endif
 }

void Draw_to_Double_Buffer(char *char_ptr, int x_pos, int y_pos , unsigned char fg_colour,unsigned char bg_colour)
  {
extern Ptr screenaddr;
extern int screenwidth;
char *db_ptr;
int a,b;

MyDebugStr(__LINE__,"textsplat.c","Draw_to_Double_Buffer");
double_buffer=screenaddr;
screen_width=screenwidth;

db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < CHAR_LINES; b++)
  {
    for(a=0; a < CHAR_COLUMN_SIZE; a++)
     {
//     pixel=(*char_ptr);
     if (*char_ptr != -1)
       {
//       *db_ptr = (*char_ptr);
       *db_ptr = fg_colour;
       }
       else
       *db_ptr = bg_colour;
       
 //    if (pixel==0) pixel=0xff;	//if 0 then 255
 //    *db_ptr = pixel;
     char_ptr++;
     db_ptr++;
     }
  char_ptr-=CHAR_COLUMN_SIZE;
  db_ptr-=CHAR_COLUMN_SIZE;
  char_ptr+=CHAR_COLUMN_SIZE;
  db_ptr+=screen_width;
  }

  }






 
 
 
#if 0
void Draw_to_Double_Buffer_Small(char *char_ptr, int x_pos, int y_pos , unsigned char fg_colour,unsigned char bg_colour)
  {
extern Ptr screenaddr;
extern int screenwidth;
char *db_ptr;
int a,b;

MyDebugStr(__LINE__,"textsplat.c","Draw_to_Double_Buffer_Small");
double_buffer=screenaddr;
screen_width=screenwidth;

db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < SML_DIMENSION; b++)
  {
    for(a=0; a < SML_DIMENSION; a++)
     {
     if (*char_ptr != -1)
       {
       *db_ptr = fg_colour;
       }
       else
       *db_ptr = bg_colour;
       
     char_ptr++;
     db_ptr++;
     }
  char_ptr-=SML_DIMENSION;
  db_ptr-=SML_DIMENSION;
  char_ptr+=SML_DIMENSION;
  db_ptr+=screen_width;
  }

  }
#endif





void Draw_to_Double_Buffer_Small_Transparent(char *char_ptr, int x_pos, int y_pos , unsigned char fg_colour)
  {
extern Ptr screenaddr;
extern int screenwidth;
char *db_ptr;
int a,b;
MyDebugStr(__LINE__,"textsplat.c","Draw_to_Double_Buffer_Small_Transparent");

double_buffer=screenaddr;
screen_width=screenwidth;

db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < SML_DIMENSION; b++)
  {
    for(a=0; a < SML_DIMENSION; a++)
     {
     if (*char_ptr != -1)
       {
       *db_ptr = fg_colour;
       }
//       else
//       *db_ptr = bg_colour;
       
     char_ptr++;
     db_ptr++;
     }
  char_ptr-=SML_DIMENSION;
  db_ptr-=SML_DIMENSION;
  char_ptr+=SML_DIMENSION;
  db_ptr+=screen_width;
  }

  }


void SplatText(char splat_str[80], int x_pos, int y_pos, unsigned char fg_colour, unsigned char  bg_colour)
 {
  int i,my_char;
  char *char_ptr;
  
  int char_pos,char_offset;
  
  MyDebugStr(__LINE__,"textsplat.c","SplatText");
  char_pos=x_pos;
  i=0;
  
  char_offset = 0;
  my_char=splat_str[i];
  

  while(my_char)
   {
//     if (my_char == 169)
//      {
//      char_offset = 59;
//      }
//     else 
//      {
       char_offset = my_char - 32;
//      } 
     char_ptr = (CHAR_PIXSIZE * char_offset) + medium_font; 
    
     Draw_to_Double_Buffer(char_ptr,char_pos,y_pos,fg_colour,bg_colour);

     char_pos+= CHAR_COLUMN_SIZE;
     i++;
     my_char=splat_str[i];  
   } 

	
 }
 

 void SplatText_Small_rgb(char splat_str[16], int x_pos, int y_pos, float red, float green, float blue )
 { 
  
   glColor3d(red, green, blue);

   OGL_moveto(x_pos,y_pos+6);
//   OGL_draw_monaco_bold(splat_str);	
   OGL_draw_helvetica_small(splat_str);	

 }

 void SplatText_mon_fast(char splat_str[16], int x_pos, int y_pos)
 { 
   OGL_moveto(x_pos,y_pos+6);
   OGL_draw_monaco_bold(splat_str);	
 }


 
 void SplatText_Small(char splat_str[16], int x_pos, int y_pos, unsigned char fg_colour)
 {
  

   OGL_set_pen_colour_from_index(fg_colour);

   OGL_moveto(x_pos,y_pos+6);
   OGL_draw_monaco_bold(splat_str);
 

	
 }
 

 void SplatText_Small_Transparent(char splat_str[16], int x_pos, int y_pos, unsigned char fg_colour)
 {
  
  SplatText_Small(splat_str, x_pos, y_pos, fg_colour);
  return;
	
 }


// ***************************************************
// ***************************************************
//	TRANSPARENT VERSIONS
// ***************************************************
// ***************************************************


void SplatText_Transparent(char splat_str[80], int x_pos, int y_pos, int colour)
 {
  
     OGL_set_pen_colour_from_index(colour);
     OGL_moveto(x_pos,y_pos);
     OGL_draw_monaco_bold(splat_str);
 }   


#if 0
void Draw_to_Double_Buffer_Transparent(char *char_ptr, int x_pos, int y_pos, unsigned char colour) // Colour is the colour that needs masking
  {
extern Ptr screenaddr;
extern int screenwidth;

char *db_ptr;
int a,b;

MyDebugStr(__LINE__,"textsplat.c","Draw_to_Double_Buffer_Transparent");
double_buffer=screenaddr;
screen_width=screenwidth;
//stumod
if (y_pos>460 ) return;

db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < CHAR_LINES; b++)
  {
    for(a=0; a < CHAR_COLUMN_SIZE; a++)
     {
     if (*char_ptr != -1)
       {
//       *db_ptr = (*char_ptr);
       *db_ptr = colour;
       }
     char_ptr++;
     db_ptr++;
     }
  char_ptr-=CHAR_COLUMN_SIZE;
  db_ptr-=CHAR_COLUMN_SIZE;
  char_ptr+=CHAR_COLUMN_SIZE;
  db_ptr+=screen_width;
  }

  }
#endif


void SplatText_Console(char splat_str[80], int x_pos, int y_pos, int colour, char attrib_str[80])
 {
    

     OGL_set_pen_colour_from_index(colour);
     OGL_moveto(x_pos,y_pos);
     OGL_draw_monaco_bold(splat_str);
  
 }   

//void Draw_to_Double_Buffer_Console(char *char_ptr, int x_pos, int y_pos, unsigned char colour, unsigned char attrib) // Colour is the colour that needs masking
//  {
//extern Ptr screenaddr;
//extern int screenwidth;
//
//char *db_ptr;
//int a,b;
//
//double_buffer=screenaddr;
//screen_width=screenwidth;
////stumod
//if (y_pos>460 ) return;
//if (y_pos<0 ) return;
//
//db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));
//
//  for(b=0; b < CHAR_LINES; b++)
//  {
//    for(a=0; a < CHAR_COLUMN_SIZE; a++)
//     {
//     
//       if (attrib==0)
//       {
//         if (*char_ptr != -1)
//         {
//         *db_ptr = colour;
//         }
//       }
//       else
//       {
//         if (*char_ptr == -1)
//         {
//         *db_ptr = colour;
//         }
//       }
//     char_ptr++;
//     db_ptr++;
//     }
//  char_ptr-=CHAR_COLUMN_SIZE;
//  db_ptr-=CHAR_COLUMN_SIZE;
//  char_ptr+=CHAR_COLUMN_SIZE;
//  db_ptr+=screen_width;
//  }
//
//  }
//

void SplatText_Transparent_Fade(char splat_str[80], int x_pos, int y_pos, unsigned char colour)
 {
  int i,my_char;
  char *char_ptr;
  
  int char_pos;
  int char_offset;
  
  MyDebugStr(__LINE__,"textsplat.c","SplatText_Transparent_Fade");  
  char_pos=x_pos;
  i=0;
  char_offset = 0;
  my_char=splat_str[i];
  

  while(my_char)
   {
//     if (my_char == 169)
//      {
//       char_offset = 59;
//      }
//     else 
//      {
       char_offset = my_char - 32;
//      } 
     char_ptr = (CHAR_PIXSIZE * char_offset) + medium_font; 
     Draw_to_Double_Buffer_TransparentFade(char_ptr,char_pos,y_pos, colour);
     char_pos+= CHAR_COLUMN_SIZE;
     i++;
     my_char=splat_str[i];  
   } 

	
 }   

//given a base colour, this starts drawing in colour+5, goes back to colour over five lines
//then up to colour+5 again
void Draw_to_Double_Buffer_TransparentFade(char *char_ptr, int x_pos, int y_pos, unsigned char colour) // Colour is the colour that needs masking
  {
extern Ptr screenaddr;
extern int screenwidth;

char *db_ptr;
int a,b;
int actual_colour,colour_counter,colour_delta;

MyDebugStr(__LINE__,"textsplat.c","Draw_to_Double_Buffer_TransparentFade");
double_buffer=screenaddr;
screen_width=screenwidth;
//stumod
if (y_pos>464) return;
actual_colour=colour+5;
colour_counter=0;
colour_delta=-1;

db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < CHAR_LINES; b++)	//lines loop
  {
    for(a=0; a < CHAR_COLUMN_SIZE; a++)	//pix loop
     {
     if (*char_ptr != -1)
       {
//       *db_ptr = (*char_ptr);
       *db_ptr = actual_colour;
       }
     char_ptr++;
     db_ptr++;
     }
  //do colour
  colour_counter++;
  if (colour_counter==5) colour_delta=-colour_delta;
  actual_colour+=colour_delta;
//  if (actual_colour>colour+5) actual_colour=colour+5;
  char_ptr-=CHAR_COLUMN_SIZE;
  db_ptr-=CHAR_COLUMN_SIZE;
  char_ptr+=CHAR_COLUMN_SIZE;
  db_ptr+=screen_width;
  }

  }



void SplatText_Fade(char splat_str[80], int x_pos, int y_pos, unsigned char fg_colour, unsigned char  bg_colour)
 {
  int i,my_char;
  char *char_ptr;
  
  int char_pos,char_offset;

  MyDebugStr(__LINE__,"textsplat.c","SplatText_Fade");  
  
  char_pos=x_pos;
  i=0;
  
  char_offset = 0;
  my_char=splat_str[i];
  

  while(my_char)
   {
//     if (my_char == 169)
//      {
//      char_offset = 59;
//      }
//     else 
//      {
       char_offset = my_char - 32;
//      } 
     char_ptr = (CHAR_PIXSIZE * char_offset) + medium_font; 
    
     Draw_to_Double_Buffer_Fade(char_ptr,char_pos,y_pos,fg_colour,bg_colour);

     char_pos+= CHAR_COLUMN_SIZE;
     i++;
     my_char=splat_str[i];  
   } 

	
 }


void Draw_to_Double_Buffer_Fade(char *char_ptr, int x_pos, int y_pos , unsigned char fg_colour,unsigned char bg_colour)
  {
extern Ptr screenaddr;
extern int screenwidth;
char *db_ptr;
int a,b;
int actual_colour,colour_counter,colour_delta;

MyDebugStr(__LINE__,"textsplat.c","Draw_to_Double_Buffer_Fade");
double_buffer=screenaddr;
screen_width=screenwidth;

actual_colour=fg_colour+5;
colour_counter=0;
colour_delta=-1;

db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < CHAR_LINES; b++)
  {
    for(a=0; a < CHAR_COLUMN_SIZE; a++)
     {
//     pixel=(*char_ptr);
     if (*char_ptr != -1)
       {
//       *db_ptr = (*char_ptr);
       *db_ptr = actual_colour;
       }
       else
       *db_ptr = bg_colour;
       
 //    if (pixel==0) pixel=0xff;	//if 0 then 255
 //    *db_ptr = pixel;
     char_ptr++;
     db_ptr++;
     }
  //do colour
  colour_counter++;
  if (colour_counter==5) colour_delta=-colour_delta;
  actual_colour+=colour_delta;

  char_ptr-=CHAR_COLUMN_SIZE;
  db_ptr-=CHAR_COLUMN_SIZE;
  char_ptr+=CHAR_COLUMN_SIZE;
  db_ptr+=screen_width;
  }

  }

//Large font support
//The large font is proportional width - this table details the widths of each character
//from space (00x20) to "Z"
#define LCHAR_OFFSET 32*48
#define LCHAR_COLUMN_SIZE 32
#define LCHAR_LINES 48

int h_offsets[] = 
        {
         24,	// space
         16,	//!
         16,	//"
         32,	//#
         32,	//$
         32,	//%
         32,	//&
         16,	//'
         16,	//(
         16,	// )
         24,	 // *
         24,12,	//+,
         32,12,	//-.
         16,20,	///0
         20,20,
         20,20,
         20,20,
         20,20,
         20,	//upto 9
         16,	//:
         16,	//;
         18,	//<
         32,	//=
         16,	//>
         24,	//?
         32,	//@
         22,	//A
         21,	//B
         21,	//C
         21,	//D
         19,	//E
         19,	//F
         21,	//G
         21,	//H
         14,	//I
         18,	//J
         22,	//K
         19,	//L
         30,	//M
         21,	//N
         22,	//O
         21,	//P
         21,	//Q
         22,	//R
         20,	//S
         19,	//T
         21,	//U
         22,	//V
         32,	//W
         28,	//X
         21,	//Y
         21,	//Z
        };
        
void SplatTextLarge_Transparent(char splat_str[80], int x_pos, int y_pos)
 {
  int i,my_char;
  char *char_ptr;
  int char_pos;
  
//  Rect my_rect;
  
  MyDebugStr(__LINE__,"textsplat.c","SPlatTextLarge_Transparent");
  char_pos=x_pos;
  i=0;
  my_char=splat_str[i];
  

  while(my_char)
   {
//   char_ptr = characters_start[my_char-32] + large_font;  
   char_ptr =  (LCHAR_OFFSET * (my_char - 32)) +4 + large_font+(32*8);	//+4 for width and height
   if (my_char<='Z')	
   Large_Draw_to_Double_Buffer_Transparent(char_ptr,char_pos,y_pos);

   char_pos+= h_offsets[my_char-32];
   i++;
   
   my_char=splat_str[i];  
   } 

	
 }   


void Large_Draw_to_Double_Buffer_Transparent(char *char_ptr, int x_pos, int y_pos) // Colour is the colour that needs masking
  {
extern Ptr screenaddr;
extern int screenwidth;

char *db_ptr;
int a,b;

MyDebugStr(__LINE__,"textsplat.c","Large_Draw_to_Double_Buffer_Transparent");

double_buffer=screenaddr;
screen_width=screenwidth;
//stumod
if (y_pos>450) return;

db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < LCHAR_LINES; b++)
  {
    for(a=0; a < LCHAR_COLUMN_SIZE; a++)
     {
     if (*char_ptr != -1)
       {
       *db_ptr = (*char_ptr);
       }
     char_ptr++;
     db_ptr++;
     }
  char_ptr-=LCHAR_COLUMN_SIZE;
  db_ptr-=LCHAR_COLUMN_SIZE;
  char_ptr+=LCHAR_COLUMN_SIZE;
  db_ptr+=screen_width;
  }

  }


void SplatTextLarge_TransparentC(char splat_str[80], int x_pos, int y_pos, int colour)
 {
  int i,my_char;
  char *char_ptr;
  int char_pos;
  
//  Rect my_rect;
MyDebugStr(__LINE__,"textsplat.c","SplatTextLarge_TransparentC");
  
  char_pos=x_pos;
  i=0;
  my_char=splat_str[i];
  

  while(my_char)
   {
//   char_ptr = characters_start[my_char-32] + large_font;  
   char_ptr =  (LCHAR_OFFSET * (my_char - 32)) +4 + large_font+(32*8);	//+4 for width and height
   if (my_char<='Z')	
   Large_Draw_to_Double_Buffer_TransparentC(char_ptr,char_pos,y_pos,colour);

   char_pos+= h_offsets[my_char-32];
   i++;
   
   my_char=splat_str[i];  
   } 

	
 }   


void Large_Draw_to_Double_Buffer_TransparentC(char *char_ptr, int x_pos, int y_pos,int colour) // Colour is the colour that needs masking
  {
extern Ptr screenaddr;
extern int screenwidth;

char *db_ptr;
int a,b;

MyDebugStr(__LINE__,"textsplat.c","Large_Draw_to_Double_Buffer_TransparentC");


double_buffer=screenaddr;
screen_width=screenwidth;
//stumod
if (y_pos>450) return;

db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < LCHAR_LINES; b++)
  {
    for(a=0; a < LCHAR_COLUMN_SIZE; a++)
     {
     if (*char_ptr != -1)
       {
       *db_ptr = colour;
       }
     char_ptr++;
     db_ptr++;
     }
  char_ptr-=LCHAR_COLUMN_SIZE;
  db_ptr-=LCHAR_COLUMN_SIZE;
  char_ptr+=LCHAR_COLUMN_SIZE;
  db_ptr+=screen_width;
  }

  }



//¥¥¥
void SplatTextLarge_Treplace(char splat_str[80], int x_pos, int y_pos, int colour, int replace)
 {
  int i,my_char;
  char *char_ptr;
  int char_pos;
MyDebugStr(__LINE__,"textsplat.c","SplatTextLarge_Treplace"); 
//  Rect my_rect;
  
  
  char_pos=x_pos;
  i=0;
  my_char=splat_str[i];
  

  while(my_char)
   {
//   char_ptr = characters_start[my_char-32] + large_font;  
   char_ptr =  (LCHAR_OFFSET * (my_char - 32)) +4 + large_font+(32*8);	//+4 for width and height
   if (my_char<='Z')	
   Large_Draw_to_Double_Buffer_Treplace(char_ptr,char_pos,y_pos,colour,replace);

   char_pos+= h_offsets[my_char-32];
   i++;
   
   my_char=splat_str[i];  
   } 

	
 }   


void Large_Draw_to_Double_Buffer_Treplace(char *char_ptr, int x_pos, int y_pos, int colour, int replace) // Colour is the colour that needs masking
  {
extern Ptr screenaddr;
extern int screenwidth;

unsigned char *db_ptr;
int a,b;

MyDebugStr(__LINE__,"textsplat.c","Large_Draw_to_Double_Buffer_Treplace"); 

double_buffer=screenaddr;
screen_width=screenwidth;
//stumod
if (y_pos>450) return;

db_ptr=(unsigned char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < LCHAR_LINES; b++)
  {
    for(a=0; a < LCHAR_COLUMN_SIZE; a++)
     {
     if (*char_ptr != -1)
       {
       *db_ptr = (char) ((*char_ptr)-2);	//from green to blue - 36 is good
       }
     char_ptr++;
     db_ptr++;
     }
  char_ptr-=LCHAR_COLUMN_SIZE;
  db_ptr-=LCHAR_COLUMN_SIZE;
  char_ptr+=LCHAR_COLUMN_SIZE;
  db_ptr+=screen_width;
  }

  }

//¥¥¥¥¥¥
void SplatTextLarge_weird(char splat_str[80], int x_pos, int y_pos)
 {
  int i,my_char;
  char *char_ptr;
  int char_pos;
  
//  Rect my_rect;
MyDebugStr(__LINE__,"textsplat.c","SplatTextLarge_weird");  
  
  char_pos=x_pos;
  i=0;
  my_char=splat_str[i];
  

  while(my_char)
   {
//   char_ptr = characters_start[my_char-32] + large_font;  
   char_ptr =  (LCHAR_OFFSET * (my_char - 32)) +4 + large_font+(32*8);	//+4 for width and height
   if (my_char<='Z')	
   Large_Draw_to_Double_Buffer_weird(char_ptr,char_pos,y_pos);

   char_pos+= h_offsets[my_char-32];
   i++;
   
   my_char=splat_str[i];  
   } 

	
 }   

int clut_value=215;
float inc_value=0;
int weird_count=0;
int wc_inc=1;

void Large_Draw_to_Double_Buffer_weird(char *char_ptr, int x_pos, int y_pos)
  {
extern Ptr screenaddr;
extern int screenwidth;

char *db_ptr;
int a,b;
int temp;
MyDebugStr(__LINE__,"textsplat.c","Large_Draw_to_Double_Buffer_weird"); 
double_buffer=screenaddr;
screen_width=screenwidth;
//stumod
if (y_pos>450) return;
db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < LCHAR_LINES; b++)
  {
  if (clut_value==224 ||
    clut_value==234 ||
    clut_value==244) clut_value+=1;	//stop from getting too dark

    for(a=0; a < LCHAR_COLUMN_SIZE; a++)
     {
     if (*char_ptr != -1)
       {
       *db_ptr = clut_value;
       }
     char_ptr++;
     db_ptr++;
     }

//     clut_value++;
//     if (clut_value>244) clut_value=215;
//ypos can be between 0 and 479, we need a clut value of between215 and 244
 temp=y_pos+weird_count;
 clut_value=temp>>4;	//div 16
 clut_value+=215;
 
// clut_value=y_pos>>1;
//
 y_pos++;  
  char_ptr-=LCHAR_COLUMN_SIZE;
  db_ptr-=LCHAR_COLUMN_SIZE;
  char_ptr+=LCHAR_COLUMN_SIZE;
  db_ptr+=screen_width;
  }

  }
  
//¥¥¥¥¥¥backwards version of above
int weird_countb=230;
int wc_incb=1;

void SplatTextLarge_weirdB(char splat_str[80], int x_pos, int y_pos)
 {
  int i,my_char;
  char *char_ptr;
  int char_pos;
  
//  Rect my_rect;
  
MyDebugStr(__LINE__,"textsplat.c","SplatTextLarge_weirdB");   
  char_pos=x_pos;
  i=0;
  my_char=splat_str[i];
  

  while(my_char)
   {
//   char_ptr = characters_start[my_char-32] + large_font;  
   char_ptr =  (LCHAR_OFFSET * (my_char - 32)) +4 + large_font+(32*8);	//+4 for width and height
   if (my_char<='Z')	
   Large_Draw_to_Double_Buffer_weirdB(char_ptr,char_pos,y_pos);

   char_pos+= h_offsets[my_char-32];
   i++;
   
   my_char=splat_str[i];  
   } 

	
 }   


void Large_Draw_to_Double_Buffer_weirdB(char *char_ptr, int x_pos, int y_pos)
  {
extern Ptr screenaddr;
extern int screenwidth;

char *db_ptr;
int a,b;
int temp;
MyDebugStr(__LINE__,"textsplat.c","Large_Draw_to_Double_Buffer_weirdB"); 
double_buffer=screenaddr;
screen_width=screenwidth;
//stumod
if (y_pos>450) return;
db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < LCHAR_LINES; b++)
  {
    if (clut_value==224 ||
    clut_value==234 ||
    clut_value==244) clut_value+=1;	//stop from getting too dark

    for(a=0; a < LCHAR_COLUMN_SIZE; a++)
     {
     if (*char_ptr != -1)
       {
       *db_ptr = clut_value;
       }
     char_ptr++;
     db_ptr++;
     }

//     clut_value++;
//     if (clut_value>244) clut_value=215;
//ypos can be between 0 and 479, we need a clut value of between215 and 244
 temp=y_pos+weird_countb;
 clut_value=temp>>4;	//div 16
 clut_value+=215;
// clut_value=y_pos>>1;
//
 y_pos++;  
  char_ptr-=LCHAR_COLUMN_SIZE;
  db_ptr-=LCHAR_COLUMN_SIZE;
  char_ptr+=LCHAR_COLUMN_SIZE;
  db_ptr+=screen_width;
  }

  }
  
