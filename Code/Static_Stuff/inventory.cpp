/*

inventory.cpp

// $Log: inventory.cpp,v $
// Revision 1.4  2003/09/27 20:41:59  robp
// Fixed float->int warnings.
//
// Revision 1.3  2003/09/26 19:20:48  robp
// Alteration for C++ const means internal unless specified extern. Made header extern so that C compiler wouldn't complain.
//
// Revision 1.2  2003/09/20 12:57:14  robp
// Removed nested comments
//
// Revision 1.1.1.1  2003/09/05 22:37:13  stu_c
// First Imported.
//
// Revision 1.8  2002/09/08 02:18:39  stu_c
// Precompiled header adjustments
//
// Revision 1.7  2002/05/31 23:56:45  stu
// Removed all warnings from the code
//
// Revision 1.6  2001/12/29 01:12:35  stu
// Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
//
// Revision 1.5  2001/12/27 19:48:23  stu
// Additional development work for docking
//
// Revision 1.4  2001/11/02 19:33:03  rob
// Build variants for interface lib and carbon.
Coffee fixes after changes for X.
//
// Revision 1.3  2001/10/22 21:28:11  rob
// Carbon warning changes
//
// Revision 1.2  2001/10/21 12:37:27  rob
// Carbon changes
//
// Revision 1.1  2001/10/21 01:12:31  stu
// Initial porting work
//
// Revision 1.0.0.1  2001/10/17 20:46:08  rob
// First Imported.
//
// Revision 1.1.1.1  2001/01/01 21:13:39  rob
// First Imported.
//
// Revision 1.0.0.1  2000/08/21 22:11:23  stu
// First Imported.
//
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"
  #include "options.h"

  #include "Error_handler.h"
  #include "zex_misc.h"
  #include "real_time.h"

#endif  //pch

#include "Zsound.h"
#include "pictures.h"
#include "splat_control.h"
#ifndef __3d_structures__  //might be from pch
#include	"3d_structures_def.h"
#endif

#include "inventory.h"
#include "instrum.h"
#include	"txtsplat.h"
#include "cargo.h"
#include "ship_kit.h"
#include "game_defs.h"
#include "ogl.h"
#include "olg_text.h"
#include "keyboard.h"


void draw_items();
void draw_hold_items(LSRAW * up_pict, LSRAW * down_pict, LSRAW * cursor, LSRAW * armed);
int draw_sell_items(int what_class);

Str255 ship_items_strings[35];	//read from str resource 130

void do_inventory()
{
//extern GDHandle				mZexGDH;
extern LSRAW *inv_picture;
//extern LSRAW *inv_picture_rgba;
//Handle inv_H;
//CGrafPtr the_drawing_buffer;
GDHandle	saveGDH;
GrafPtr		savePort;


while (Button()==1);

//    inv_H=GetZexPicture ('RCZ ',4000);	//inventory pict   
//	  HLock(inv_H);
//	  inv_picture = (LSRAW*)*inv_H;

     GetPort (&savePort);
	 saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices

         #if ZEX_CORE_CARBON
           MyDebugStr(__LINE__,__FILE__,"Carbon Conversion not finished");
         #else
           // NOTE for PORTer -- exactly where is the_drawing_buffer initialised anyway????
	 EraseRect(&the_drawing_buffer->portRect);
         #endif
         
	 print_crunched_trans_pict(inv_picture,100,100);
	 draw_items();
     Show_it();

//DisposeHandle(inv_H);

while (Button()==0);

}


extern int global_show_status;
void do_inventory_single_frame()	//draws picture and returns - speed is not an issue.
{
//extern LSRAW *inv_picture;
//extern Handle inv_H;
//extern LSRAW *up_picture;
//extern Handle up_H;
//extern LSRAW *down_picture;
//extern Handle down_H;
//extern LSRAW *armed_picture;
//extern Handle armed_H;

//extern LSRAW *inv_cursor_picture;
//extern Handle inv_cursor_H;
extern int OGL_active;



//CGrafPtr the_drawing_buffer;
GDHandle	saveGDH=0;
GrafPtr		savePort=0;




//picture will be loaded from main loop
#if OGL==1
    if (OGL_active==1)
    {
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);	//dk blue
	glClear(GL_COLOR_BUFFER_BIT);
//	print_crunched32_trans_pict(inv_picture_rgba,100,100);
    }
    else
    erase_zex_rect_colour(monitor_w,monitor_h,244);	//dk blue
    
#else
    erase_zex_rect_colour(monitor_w,monitor_h,244);	//dk blue
//        erase_zex_rect_colour(640,480,242);	  //in pictures
//	EraseRect(&the_drawing_buffer->portRect);
	print_crunched_trans_pict(inv_picture,100,100);
#endif
	 if (global_show_status==1)
	 {
	 draw_items();
        #if OGL==1

         ogl_hide_mouse_cursor();
  	 #endif
  	 }
  	 else
	 {
	 #if OGL==1

         ogl_show_mouse_cursor();
	 #endif
//	draw_hold_items(up_picture, down_picture, inv_cursor_picture, armed_picture);	   //with jettison buttons
	    
	 }
	 SetGDevice(saveGDH);	 //these are here so I can SEE them!
	 SetPort(savePort);

//DisposeHandle(inv_H);
//DisposeHandle(up_H);
//DisposeHandle (down_H);
//DisposeHandle (cursor_H);
//DisposeHandle (armed_H);
}

int do_sell_kit(int _class)	//draws picture and returns - speed is not an issue.
{
extern LSRAW *inv_picture;
//Handle inv_H;
//extern LSRAW *up_picture;
//Handle up_H;
//extern LSRAW *down_picture;
//Handle down_H;
//extern LSRAW *armed_picture;
//Handle armed_H;

//extern LSRAW *inv_cursor_picture;
//Handle cursor_H;

//CGrafPtr the_drawing_buffer;
GDHandle	saveGDH=0;
GrafPtr		savePort=0;
int done;
int return_val=-1;

//    inv_H=GetZexPicture ('RCZ ',4000);	//inventory pict   
//	  HLock(inv_H);
//	  inv_picture = (LSRAW*)*inv_H;
//
//    up_H=GetZexPicture ('RCZ ',3301);	//jett up pict   
//	  HLock(up_H);
//	  up_picture = (LSRAW*)*up_H;
//
//    down_H=GetZexPicture ('RCZ ',3300);	//jett down pict   
//	  HLock(down_H);
//	  down_picture = (LSRAW*)*down_H;
//
//    cursor_H=GetZexPicture ('RCZ ',3302);	//arrow pict   
//	  HLock(cursor_H);
//	  cursor_picture = (LSRAW*)* cursor_H;
//
//    armed_H=GetZexPicture ('RCZ ',3303);	  //arrow pict   
//	  HLock(armed_H);
//	  armed_picture = (LSRAW*)* armed_H;

done=0;
ShowCursor();
while(done==0)
     {
     int which_item;
	 print_crunched_trans_pict(inv_picture,100,100);
	 which_item=draw_sell_items(_class);
	 SetGDevice(saveGDH);	 //these are here so I can SEE them!
	 SetPort(savePort);
     
         Show_it();
         if (Button()==1)
         {
         if (which_item==-1) 
          { done=1;
            return_val=-1;
          }
         else
         //sell the item
          {
          done=1;
          return_val=which_item;
          }
         }
         
      }
         
return return_val;
}





spoint string_pos [] = 
{
  { 10,360,125,360, },	//radar
  { 10,390,127,362, },	//lr radar
  { 10,430,137,362, },	//cannon
  { 10,325,160,350, },	//laser
  { 150,430,270,300, },	//missile controller
  { 330,430,290,280, },	//repair systems
  { 450,430,310,260, },	//flight computer
  { 530,430,334,250, },	//engine
  { 10,290,165,330, },	//pylon 1
  { 10,230,220,230, },		//pylon 2
  { 10,190,255,180, },		//pylon 3
  { 10,135,260,160, },		//shields
  { 10,106,280,140, },		//fuel tank
  { 420,160,360,164, },	//rear_laser
  { 10,75,295,140, },		//360
  { 10,40,300,135, },		//shockwave
  { 150,40,310,145, },		//ecm
  { 330,40,320,155, },		//missile factory
  { 450,40,330,165, },		//fastdock
  { 530,40,340,160, },		//flatpacks
  { 530,380,350,240, },	//fuel maximiser
  { 530,335,370,230, },	//retro
  { 530,290,380,220, },	//hyperspace
  { 530,255,385,210, },	//nav computer
  { 530,210,365,200, },	//ejection
  { 530,165,325,180, },	//trading computer
  { 530,120,325,170, },	//hull auto repair
  { 150,390,245,300, },	//boid addition
  { 530,80,345,165, }	//asteroid addition
};



void draw_items()
{
extern kit_desc ship_items[ITEMS_COUNT];
//extern int zex_laser_experience;
//extern int score,credits,start_time,accumulated_game_time;
int i;
unsigned char number_string[12];
//int alive_time,hours,mins,secs,stop_time;
//char number_string1[12];

TextFont(21);
//TextSize(12);
//   ZMoveTo(10,20);
//   RGBForeColor(&rgbWhite);

//SplatText_Transparent ("EQUIPMENT",280,6,235);

draw_topbottom_lines();
//#define COLOUR 225
//
//SplatText_Transparent ("SCORE",10,6,COLOUR);
//NumToString(score,number_string);
//number_string[number_string[0]+1]=0;	  //quick p to c hack
//number_string[0]=' ';
//SplatText_Transparent ((char *)number_string,42,6,COLOUR);
//      
//SplatText_Transparent ("CREDITS",500,6,COLOUR);
//NumToString(credits,number_string);
//number_string[number_string[0]+1]=0;	  //quick p to c hack
//number_string[0]=' ';
//SplatText_Transparent ((char *)number_string,560,6,COLOUR);
//
//   RGBForeColor(&rgbRed);
//ZMoveTo(0,20);
//ZLineTo(640,20);
//      
//
//#define TIMEX 100
//#define TIMEY 460
//SplatText_Transparent ("REAL TIME",10,TIMEY,COLOUR);
//stop_time=TickCount();
//alive_time=(stop_time-start_time);
//
//hours=alive_time/(60*60*60);
//alive_time-=hours*(60*60*60);
//
//mins=alive_time/(60*60);
//alive_time-=mins*(60*60);
//
//secs=alive_time/60;
//num_to_string99(hours,number_string1);	//hours
//SplatText_Transparent ((char *)number_string1,TIMEX,TIMEY,COLOUR);
////
//	  SplatText_Transparent_Fade (":",TIMEX+14,TIMEY,COLOUR);
////	    
//num_to_string99(mins,number_string1);	//mins
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX+20,TIMEY,COLOUR);
//
//	  SplatText_Transparent_Fade (":",TIMEX+34,TIMEY,COLOUR);
//	  
//num_to_string99(secs,number_string1);	//secs
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX+40,TIMEY,COLOUR);
//
//
//#define TIMEX 560
//#define TIMEY 460
//SplatText_Transparent_Fade ("TOTAL TIME",480,TIMEY,COLOUR);
//stop_time=TickCount();
//alive_time=(stop_time-start_time)+accumulated_game_time;
//
//hours=alive_time/(60*60*60);
//alive_time-=hours*(60*60*60);
//
//mins=alive_time/(60*60);
//alive_time-=mins*(60*60);
//
//secs=alive_time/60;
//num_to_string99(hours,number_string1);	//hours
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX,TIMEY,COLOUR);
////
//	  SplatText_Transparent_Fade (":",TIMEX+14,TIMEY,COLOUR);
////	    
//num_to_string99(mins,number_string1);	//mins
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX+20,TIMEY,COLOUR);
//
//	  SplatText_Transparent_Fade (":",TIMEX+34,TIMEY,COLOUR);
//	  
//num_to_string99(secs,number_string1);	//secs
//SplatText_Transparent_Fade ((char *)number_string1,TIMEX+40,TIMEY,COLOUR);
//
//
////   RGBForeColor(&rgbRed);
//ZMoveTo(0,456);
//ZLineTo(640,456);


TextSize(9);
TextFace(normal);

for (i=0;i<KLAST;i++)
   {
   ZMoveTo(string_pos[i].sx,string_pos[i].sy);
   ZRGBForeColor(&rgbWhite);

   if (ship_items[i].status==0) 
     {
      ZRGBForeColor(&rgbDarkGrey);
      ZDrawString("\pEmpty slot");
     }
     else
     {
      ZDrawString(ship_items_strings[i]);
//      if (ship_items[i].status==-1) 
//       {
//       RGBForeColor(&rgbRed);
//       ZDrawString("\p Lost!");
//       }
      }
      
   ZRGBForeColor(&rgbDarkGrey);

   if (string_pos[i].sx<screen_cent_x)
   ZLineTo(string_pos[i].lex,string_pos[i].ley);
   else
   {
   ZMoveTo(string_pos[i].sx,string_pos[i].sy);
   ZLineTo(string_pos[i].lex,string_pos[i].ley);   
   }
//now show status
//   RGBForeColor(&rgbGrey);

   ZMoveTo(string_pos[i].sx,string_pos[i].sy+10);
   if (ship_items[i].status==0) 
   {
   ZRGBForeColor(&rgbRed);
//   ZDrawString ("\pNot Fitted");
//   RGBForeColor(&rgbGrey);
   }
   else //fitted
   {
 
   if (ship_items[i].status==-1) 
   {
    ZRGBForeColor(&rgbRed);
    ZDrawString ("\pDestroyed.");
   }
   else
   if (ship_items[i].status==1)
   {
    ZRGBForeColor(&rgbGreen);
    ZDrawString ("\pServiceable. ");
//    RGBForeColor(&rgbGrey);
   }
  else
   {
//      if (ship_items[i].status==-1) 
//       {
//       RGBForeColor(&rgbRed);
//       ZDrawString("\p Lost!");
 
    ZRGBForeColor(&rgbOrange);
    ZDrawString ("\pDamaged. ");
//    RGBForeColor(&rgbGrey);
   }
//now show extra info depending on type
   if (i==KSHIELDS)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\pType 1. ");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\pType 2. ");
       else
       if(ship_items[i].modifier==3) ZDrawString ("\pType 3. ");
       else
       if(ship_items[i].modifier==4) ZDrawString ("\pType 4. ");
       else
       if(ship_items[i].modifier==5) ZDrawString ("\pType 5. ");

       NumToString(STATIC_CAST_TO_LONG(ship_items[KSHIELDS].value1/10),number_string);
       ZDrawString (number_string);
       ZDrawString ("\p%");
       
       
     }
   else

   if (i==KCANNON)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\p65mm rounds.");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\p120mm rounds.");
       else
       if(ship_items[i].modifier==3) ZDrawString ("\p200mm rounds.");
     }
   else
   if (i==KEJECTION)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\pStandard.");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\pHardened.");
     }

   else
   if (i==KTRADING_COMPUTER)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\pType S1.");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\pType S2.");
     }

   else

   if (i==KFLATPACK)
     {
       if(ship_items[i].value1==0) ZDrawString ("\pNone.");
       else
       if(ship_items[i].value1==1) ZDrawString ("\p1.");
       else
       if(ship_items[i].value1==2) ZDrawString ("\p2.");
       else
       if(ship_items[i].value1==3) ZDrawString ("\p3.");
     }
   else

   if (i==KLASER)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\p40 Megawatts.");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\p130 Megawatts.");
       else
       if(ship_items[i].modifier==3) ZDrawString ("\p350 Megawatts.");
        else	//tasers
        if(ship_items[i].modifier==11) ZDrawString ("\p40 Megawatts x 2.");
       else
       if(ship_items[i].modifier==12) ZDrawString ("\p130 Megawatts x 2.");
       else
       if(ship_items[i].modifier==13) ZDrawString ("\p350 Megawatts x 2.");
    
     }
    
   else

   if (i==KNORMAL_FUEL_TANK)
     {
       NumToString(STATIC_CAST_TO_LONG(ship_items[KNORMAL_FUEL_TANK].value1),number_string);
       ZDrawString (number_string);
       ZDrawString ("\p tonnes.");
     }

   else
   if (i==KREAR_LASER)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\p60 Megawatts.");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\p140 Megawatts.");
       else
       if(ship_items[i].modifier==3) ZDrawString ("\p270 Megawatts.");
     }

  else
   if (i==KFLIGHT_COMPUTER || i==KSHOCKWAVE || i==KNAV_COMPUTER)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\p MK I");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\p MK II");
     }

  else
   if (i==KMISSILE_PYLON1 || i==KMISSILE_PYLON2 || i==KMISSILE_PYLON3)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\pT1 20 Megatonnes.");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\pT2 60 Megatonnes.");
       else
       if(ship_items[i].modifier==3) ZDrawString ("\pT3 150 Megatonnes.");


     }

   }	//end of fitted
   
   }	//end of for loop
//finally show hull, which is a special case
   ZMoveTo(10,165);
   ZRGBForeColor(&rgbWhite);
   ZDrawString("\pHull");
   ZRGBForeColor(&rgbDarkGrey);
   ZLineTo(260,160);

   ZMoveTo(10,175);
   ZRGBForeColor(&rgbGreen);


}




int draw_sell_items(int what_class)
{
extern kit_desc ship_items[ITEMS_COUNT];
//extern int zex_laser_experience;
extern int score,credits,start_time,accumulated_game_time;
int i,mouse_item;
unsigned char number_string[12];
int alive_time,hours,mins,secs,stop_time;
char number_string1[12];
Point mouse_xy;


mouse_item=-1;
TextFont(21);
//TextSize(12);
//   ZMoveTo(10,20);
//   RGBForeColor(&rgbWhite);
if (what_class==EQUIPMENT)
SplatText_Transparent ("SELL EQUIPMENT - CLICK AN ITEM TO SELL",150,6,247);
else
if (what_class==WEAPONS)
SplatText_Transparent ("SELL WEAPONS - CLICK AN ITEM TO SELL",150,6,247);


#define COLOUR1 225

SplatText_Transparent ("SCORE",10,6,COLOUR1);
NumToString(score,number_string);
number_string[number_string[0]+1]=0;	//quick p to c hack
number_string[0]=' ';
SplatText_Transparent ((char *)number_string,42,6,COLOUR1);
      
SplatText_Transparent ("CREDITS",500,6,COLOUR1);
NumToString(credits,number_string);
number_string[number_string[0]+1]=0;	//quick p to c hack
number_string[0]=' ';
SplatText_Transparent ((char *)number_string,560,6,COLOUR1);

   ZRGBForeColor(&rgbRed);
ZMoveTo(0,20);
ZLineTo(monitor_w,20);
      

#define TIME1X 100
#define TIME1Y 460
SplatText_Transparent ("REAL TIME",10,TIME1Y,COLOUR1);
stop_time=TickCount();
alive_time=(stop_time-start_time);

hours=alive_time/(60*60*60);
alive_time-=hours*(60*60*60);

mins=alive_time/(60*60);
alive_time-=mins*(60*60);

secs=alive_time/60;
num_to_string99(hours,number_string1);	//hours
SplatText_Transparent ((char *)number_string1,TIME1X,TIME1Y,COLOUR1);
//
	SplatText_Transparent (":",TIME1X+14,TIME1Y,COLOUR1);
//	  
num_to_string99(mins,number_string1);	//mins
SplatText_Transparent ((char *)number_string1,TIME1X+20,TIME1Y,COLOUR1);

	SplatText_Transparent (":",TIME1X+34,TIME1Y,COLOUR1);
	
num_to_string99(secs,number_string1);	//secs
SplatText_Transparent ((char *)number_string1,TIME1X+40,TIME1Y,COLOUR1);


#define TIME2X 560
#define TIME2Y 460
SplatText_Transparent ("TOTAL TIME",480,TIME2Y,COLOUR1);
stop_time=TickCount();
alive_time=(stop_time-start_time)+accumulated_game_time;

hours=alive_time/(60*60*60);
alive_time-=hours*(60*60*60);

mins=alive_time/(60*60);
alive_time-=mins*(60*60);

secs=alive_time/60;
num_to_string99(hours,number_string1);	//hours
SplatText_Transparent ((char *)number_string1,TIME2X,TIME2Y,COLOUR1);
//
	SplatText_Transparent (":",TIME2X+14,TIME2Y,COLOUR1);
//	  
num_to_string99(mins,number_string1);	//mins
SplatText_Transparent ((char *)number_string1,TIME2X+20,TIME2Y,COLOUR1);

	SplatText_Transparent (":",TIME2X+34,TIME2Y,COLOUR1);
	
num_to_string99(secs,number_string1);	//secs
SplatText_Transparent ((char *)number_string1,TIME2X+40,TIME2Y,COLOUR1);


//   RGBForeColor(&rgbRed);
ZMoveTo(0,monitor_h-24);
ZLineTo(monitor_w,monitor_h-24);


TextSize(9);
TextFace(normal);

for (i=0;i<KLAST;i++)
   {
   if (i==KRADAR) i++;
   if (i==KLONG_RANGE_RADAR) i++;
   if (i==KNORMAL_FUEL_TANK) i++;
   if (i==KMISSILES) i++;
   
   if (ship_items[i]._class==what_class)
   {
   ZMoveTo(string_pos[i].sx,string_pos[i].sy);
   ZRGBForeColor(&rgbWhite);

   if (ship_items[i].status==0) 
     {
//      RGBForeColor(&rgbDarkGrey);
//      ZDrawString("\pEmpty slot");
     }
     else
     {
     int strlen;
     
      //if mouse over item, highlight it, else just draw it
      ZexGetMouse (&mouse_xy);
      strlen=StringWidth(ship_items_strings[i]);
     
      if (
          mouse_xy.v>string_pos[i].sy-10 && 
          mouse_xy.v<string_pos[i].sy    &&
          mouse_xy.h>string_pos[i].sx    &&
          mouse_xy.h<string_pos[i].sx+strlen
          )
          {
          TextMode(4);
          mouse_item=i;
          }
      else
          {
          TextMode(1);

          }
          
      ZDrawString(ship_items_strings[i]);

      ZRGBForeColor(&rgbDarkGrey);

      if (string_pos[i].sx<screen_cent_x)
      ZLineTo(string_pos[i].lex,string_pos[i].ley);
      else
      {
      ZMoveTo(string_pos[i].sx,string_pos[i].sy);
      ZLineTo(string_pos[i].lex,string_pos[i].ley);   
      }

      }
      
//now show status
//   RGBForeColor(&rgbGrey);

   ZMoveTo(string_pos[i].sx,string_pos[i].sy+10);
   if (ship_items[i].status==0) 
   {
   ZRGBForeColor(&rgbRed);
//   ZDrawString ("\pNot Fitted");
//   RGBForeColor(&rgbGrey);
   }
   else //fitted
   {
 
   if (ship_items[i].status==-1) 
   {
    ZRGBForeColor(&rgbRed);
    ZDrawString ("\pDestroyed.");
   }
   else
   if (ship_items[i].status==1)
   {
    ZRGBForeColor(&rgbGreen);
    ZDrawString ("\pServiceable. ");
//    RGBForeColor(&rgbGrey);
   }
  else
   {
//      if (ship_items[i].status==-1) 
//       {
//       RGBForeColor(&rgbRed);
//       ZDrawString("\p Lost!");
 
    ZRGBForeColor(&rgbOrange);
    ZDrawString ("\pDamaged. ");
//    RGBForeColor(&rgbGrey);
   }
//now show extra info depending on type
   if (i==KSHIELDS)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\pType 1. ");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\pType 2. ");
       else
       if(ship_items[i].modifier==3) ZDrawString ("\pType 3. ");
       else
       if(ship_items[i].modifier==4) ZDrawString ("\pType 4. ");
       else
       if(ship_items[i].modifier==5) ZDrawString ("\pType 5. ");

       NumToString(STATIC_CAST_TO_LONG(ship_items[KSHIELDS].value1/10),number_string);
       ZDrawString (number_string);
       ZDrawString ("\p%");
       
       
     }
   else

   if (i==KCANNON)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\p65mm rounds.");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\p120mm rounds.");
       else
       if(ship_items[i].modifier==3) ZDrawString ("\p200mm rounds.");
     }
   else
   if (i==KEJECTION)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\pStandard.");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\pHardened.");
     }

   else
   if (i==KTRADING_COMPUTER)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\pType S1.");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\pType S2.");
     }

   else

   if (i==KFLATPACK)
     {
       if(ship_items[i].value1==0) ZDrawString ("\pNone.");
       else
       if(ship_items[i].value1==1) ZDrawString ("\p1.");
       else
       if(ship_items[i].value1==2) ZDrawString ("\p2.");
       else
       if(ship_items[i].value1==3) ZDrawString ("\p3.");
     }
   else

   if (i==KLASER)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\p40 Megawatts.");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\p130 Megawatts.");
       else
       if(ship_items[i].modifier==3) ZDrawString ("\p350 Megawatts.");
        else	//tasers
        if(ship_items[i].modifier==11) ZDrawString ("\p40 Megawatts x 2.");
       else
       if(ship_items[i].modifier==12) ZDrawString ("\p130 Megawatts x 2.");
       else
       if(ship_items[i].modifier==13) ZDrawString ("\p350 Megawatts x 2.");
    
     }
    
   else

   if (i==KNORMAL_FUEL_TANK)
     {
       NumToString(STATIC_CAST_TO_LONG(ship_items[KNORMAL_FUEL_TANK].value1),number_string);
       ZDrawString (number_string);
       ZDrawString ("\p tonnes.");
     }

   else
   if (i==KREAR_LASER)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\p60 Megawatts.");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\p140 Megawatts.");
       else
       if(ship_items[i].modifier==3) ZDrawString ("\p270 Megawatts.");
     }

  else
   if (i==KFLIGHT_COMPUTER || i==KSHOCKWAVE || i==KNAV_COMPUTER)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\p MK I");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\p MK II");
     }

  else
   if (i==KMISSILE_PYLON1 || i==KMISSILE_PYLON2 || i==KMISSILE_PYLON3)
     {
       if(ship_items[i].modifier==1) ZDrawString ("\pT1 20 Megatonnes.");
       else
       if(ship_items[i].modifier==2) ZDrawString ("\pT2 60 Megatonnes.");
       else
       if(ship_items[i].modifier==3) ZDrawString ("\pT3 150 Megatonnes.");


     }

   }	//end of fitted
           TextMode(1);
   }	//end of item class match
   
   }	//end of for loop
return mouse_item;
}




spoint cargo_pos [] = 
{
  { 40,100,270,200, },	//slot 0
  { 40,140,320,250, },	//slot 1
  { 40,180,250,230, },	//slot 2
  { 40,220,257,270, },	//slot 3
};


//draw items in hold
void draw_hold_items(LSRAW * up_pict, LSRAW * down_pict, LSRAW * cursor, LSRAW *armed_pict)
{
//extern hold_desc hold_items[MAX_HOLD_ITEMS];
//extern int score,credits,start_time,accumulated_game_time;
//extern int jettison_timer;

//int i;
//unsigned char number_string[12];
//int alive_time,hours,mins,secs,stop_time;
//char number_string1[12];
//Point mouse_xy;

return;

//TextFont(21);
////TextSize(12);
////   ZMoveTo(10,20);
////   RGBForeColor(&rgbWhite);
//#define COLOUR 225
//SplatText_Transparent ("INVENTORY",280,6,235);
//draw_topbottom_lines();
//
//
//
//
//TextSize(9);
//TextFace(normal);
//
//
//
//
//for (i=0;i<MAX_HOLD_ITEMS;i++)
//   {
//   extern hold_desc cargos_array[NUMBER_OF_COMMODITIES];
//   ZMoveTo(cargo_pos[i].sx,cargo_pos[i].sy);
//   ZRGBForeColor(&rgbWhite);
//
//   if (hold_items[i].id==-1) 
//     {
//      ZRGBForeColor(&rgbDarkGrey);
//      ZDrawString("\pEmpty cargo slot");
//     }
//     else
//     {
//      int slot;
//      char temp_string[300];
//
//      slot=hold_items[i].id;
//      CToPascal(cargos_array[slot].string,temp_string);
//      
//      ZDrawString((unsigned char *) temp_string);
////      if (ship_items[i].status==-1) 
////       {
////       RGBForeColor(&rgbRed);
////       ZDrawString("\p Lost!");
////       }
//      }
//      
//   ZRGBForeColor(&rgbDarkGrey);
//
//   if (cargo_pos[i].sx<screen_cent_x)
//   ZLineTo(cargo_pos[i].lex,cargo_pos[i].ley);
//   else
//   {
//   ZMoveTo(cargo_pos[i].sx,cargo_pos[i].sy);
//   ZLineTo(cargo_pos[i].lex,cargo_pos[i].ley);   
//   }
//}
//
////now draw jettison up buttons
//   ZRGBForeColor(&rgbRed);
//
//   ZMoveTo(20,100);
//   ZLineTo(20,300);   
//   ZMoveTo(20,300);
//   ZLineTo(40,300);   
//   ZMoveTo (42,304);
//   ZDrawString ("\pJettison Cargo");
//
//  for (i=0;i<4;i++)
//   {
//    print_crunched_trans_pict(up_pict,4,cargo_pos[i].sy-18);
//   }
////draw the mouse
//    ZexGetMouse (&mouse_xy);
//
//       for (i=0;i<4;i++)
//           {
//           if (mouse_xy.h>4 && mouse_xy.h<36
//               && mouse_xy.v>cargo_pos[i].sy-18 && mouse_xy.v<cargo_pos[i].sy+14
//               && hold_items[i].id!=-1)
//                {
//                   print_crunched_trans_pict(armed_pict,4,cargo_pos[i].sy-18);
//                } 
//           }
//      
//      
//
//    if (Button()==1)
//      {
//
//       for (i=0;i<4;i++)
//           {
//           if (mouse_xy.h>4 && mouse_xy.h<36
//               && mouse_xy.v>cargo_pos[i].sy-18 && mouse_xy.v<cargo_pos[i].sy+14
//               && hold_items[i].id!=-1)
//                {
//                   print_crunched_trans_pict(down_pict,4,cargo_pos[i].sy-18);
//                   //eject it
//                   gen_specific_cargo(0,hold_items[i].id);
//                   hold_items[i].id=-1;
//                   jettison_timer=20;	//so we don't pick it back up immediately
//                   play_zsound(jettison, sound_high_pri, c1_chan1, sound_vol_7);
//
//                } 
//           }
//      }
//    
////draw mouse cursor
//#if OGL==1
//    if (OGL_active==1)
//    {
//     ogl_update_mouse_cursor();
//    }
//    else
//#endif
//    print_crunched_trans_pict(cursor,mouse_xy.h,mouse_xy.v);

}



void draw_topbottom_lines()
{
extern int rank,promotion_points,score,credits,start_time;
unsigned char number_string[12];
int alive_time, /* hours,mins,secs,*/ stop_time;
//char number_string1[12];




#define COLOUR2 246

SplatText_Transparent ("Score",10,6,COLOUR2);
NumToString(score,number_string);
number_string[number_string[0]+1]=0;	//quick p to c hack
number_string[0]=' ';
SplatText_Transparent ((char *)number_string,42,6,COLOUR2);
      
SplatText_Transparent ("Credits",510,6,COLOUR2);
NumToString(credits,number_string);
number_string[number_string[0]+1]=0;	//quick p to c hack
number_string[0]=' ';
SplatText_Transparent ((char *)number_string,560,6,COLOUR2);

//   RGBForeColor(&rgbRed);
//ZMoveTo(0,20);
//ZLineTo(640,20);
      

#define TIME3X 100
#define TIME3Y 460
SplatText_Transparent ("Real time",10,TIME3Y,COLOUR2);
stop_time=TickCount();
alive_time=(stop_time-start_time);


draw_real_time(84,460,COLOUR2);

#define textcol COLOUR2
//rank
             SplatText_Transparent ("Rank",250,460,textcol);

             if (rank==ENS) SplatText_Transparent (" Ensign",280,460,textcol);
             else
             if (rank==LT) SplatText_Transparent (" Lieutenant",280,460,textcol);
             else
             if (rank==LTCR) SplatText_Transparent (" LT Commander",280,460,textcol);
             else
             if (rank==CDR) SplatText_Transparent (" Commander",280,460,textcol);
             else
             if (rank==CAPT) SplatText_Transparent (" Captain",280,460,textcol);
             else
             if (rank==RADM) SplatText_Transparent (" Rear Admiral",280,460,textcol);
             else
             if (rank==VADM) SplatText_Transparent (" Vice Admiral",280,460,textcol);
             else
             if (rank==ADM) SplatText_Transparent (" Admiral",280,460,textcol);
             else
             if (rank==FADM) SplatText_Transparent (" Fleet Admiral",280,460,textcol);

             SplatText_Transparent ("Promotion Points",430,460,textcol);
             
             NumToString(promotion_points,number_string);
             number_string[number_string[0]+1]='/';	//quick p to c hack
             
             if (rank==ENS)
             {
             number_string[number_string[0]+2]='5';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]=0;	//quick p to c hack
             }
             else
             if (rank==LT)
             {
             number_string[number_string[0]+2]='2';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]=0;	//quick p to c hack
             }
            
             else
             if (rank==LTCR)
             {
             number_string[number_string[0]+2]='1';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
            

             else
             if (rank==CDR)
             {
             number_string[number_string[0]+2]='2';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (rank==CAPT)
             {
             number_string[number_string[0]+2]='4';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (rank==RADM)
             {
             number_string[number_string[0]+2]='5';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (rank==VADM)	//as high as it goes baby
             {
             number_string[number_string[0]+2]='7';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (rank==ADM)	//as high as it goes baby
             {
             number_string[number_string[0]+2]='9';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (rank==FADM)	//as high as it goes baby
             {
             number_string[number_string[0]+2]='N';	//quick p to c hack
             number_string[number_string[0]+3]='/';	//quick p to c hack
             number_string[number_string[0]+4]='A';	//quick p to c hack
             number_string[number_string[0]+5]=0;	//quick p to c hack
             }




             number_string[0]=' ';
             SplatText_Transparent ((char *)number_string,544,460,textcol);

//promotion points


//hours=alive_time/(60*60*60);
//alive_time-=hours*(60*60*60);
//
//mins=alive_time/(60*60);
//alive_time-=mins*(60*60);
//
//secs=alive_time/60;
//num_to_string99(hours,number_string1);	//hours
//SplatText_Transparent ((char *)number_string1,TIMEX,TIMEY,COLOUR);
////
//	  SplatText_Transparent (":",TIMEX+14,TIMEY,COLOUR);
////	    
//num_to_string99(mins,number_string1);	//mins
//SplatText_Transparent ((char *)number_string1,TIMEX+20,TIMEY,COLOUR);
//
//	  SplatText_Transparent (":",TIMEX+34,TIMEY,COLOUR);
//	  
//num_to_string99(secs,number_string1);	//secs
//SplatText_Transparent ((char *)number_string1,TIMEX+40,TIMEY,COLOUR);


//#define TIMEX 560
//#define TIMEY 460
//SplatText_Transparent ("TOTAL TIME",480,TIMEY,COLOUR);
//stop_time=TickCount();
//alive_time=(stop_time-start_time)+accumulated_game_time;
//
//hours=alive_time/(60*60*60);
//alive_time-=hours*(60*60*60);
//
//mins=alive_time/(60*60);
//alive_time-=mins*(60*60);
//
//secs=alive_time/60;
//num_to_string99(hours,number_string1);	//hours
//SplatText_Transparent ((char *)number_string1,TIMEX,TIMEY,COLOUR);
////
//	  SplatText_Transparent (":",TIMEX+14,TIMEY,COLOUR);
////	    
//num_to_string99(mins,number_string1);	//mins
//SplatText_Transparent ((char *)number_string1,TIMEX+20,TIMEY,COLOUR);
//
//	  SplatText_Transparent (":",TIMEX+34,TIMEY,COLOUR);
//	  
//num_to_string99(secs,number_string1);	//secs
//SplatText_Transparent ((char *)number_string1,TIMEX+40,TIMEY,COLOUR);
//

//   RGBForeColor(&rgbRed);
//ZMoveTo(0,456);
//ZLineTo(640,456);

}