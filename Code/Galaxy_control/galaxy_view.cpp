// *************************************MODULE HEADER*********************************
// * FILENAME           : galaxy_view.cpp
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * DATE STARTED       : 18:24:00 on 18th November 2001
// * FIRST AUTHOR       : Rob Probin
// *
// * COPYRIGHT (c)        2001 Lightsoft
// *
// ***********************************************************************************
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "options.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <mactypes.h>
  #endif
  #include "3d_structures_def.h"
  #include	"zex_misc.h"
  #include "game_defs.h"
  #include "Error_handler.h"
  #include "processor.h"
  #include "access_ocbs.h"
#endif //pch

#include "galaxy.h"

#include "objects_controls.h"
#include "file_io.h"
#include "pictures.h"
#include	"splat_control.h"
#include	"txtsplat.h"

#include "ship_control.h"
#include "game_pause.h"
#include "object_control_top.h"
#include "camera_control.h"
#include "dlp.h"
#include "inventory.h"
#include    "fpoly4.h"
#include "genpoly.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "planet_generator.h"
#include "instrum.h"
#include "obj_control_misc_routines.h"
#include "load_objects.h"
#include "Zsound.h"
#include "physics.h"
#include "light_control.h"
#include "texture_management.h"
#include "physics_gravity.h"
#include "objects_misc.h"

#include "hud.h"
#include "dlp.h"

#include "olg_text.h"
#include "ogl.h"

#include "keyboard.h"

// ***********************************************************************************
// * CONSTANTS 
// *

#define module_private static


// ***********************************************************************************
// * TYPE DEFINITIONS
// *


// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *


// ***********************************************************************************
// * NOTES
// *


// ***********************************************************************************
// * ACTUAL CODE
// *


/* init_galactic_map
 *
 * DESCRIPTION: 
 *
 */
void init_galactic_map(void)
{
/* do nothing in init */
}


/* do_galaxy_map_single_frame
 *
 * DESCRIPTION: Draws a single frame of the galaxy map
 *
 */

void do_galaxy_map_single_frame(void)
{
//int active_object_counter,object_counter;
//float screen_x,screen_y;
//extern int dynamic_object_index;
//register ZObject * current_object_ptr;
GDHandle	saveGDH;
GrafPtr		  savePort;
//unsigned char temp_str[256];
// unsigned char colour=20;

extern GDHandle		        		mZexGDH;
//extern LSRAW *map_cursor_picture;
//extern DSpContextReference		  mDisplayContext;


DSpContext_FadeGamma ( mDisplayContext, 100, 0);	//clear any sun glare

//colour flipping for player
//if(zex_colour>255) zex_colour_delta=-zex_colour_delta;
//if(zex_colour<247) zex_colour_delta=-zex_colour_delta;

//zex_colour+=zex_colour_delta;


           
    GetPort (&savePort);
    saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
	SetGDevice(mZexGDH);
	 
	glClearColor(0.1f, 0.0f, 0.0f, 1.0f);	//red
	glClear(GL_COLOR_BUFFER_BIT);


SplatText_Small_Transparent("Galaxy Map ",(monitor_w/2)-40,6,246);

//active_object_counter=0;
//object_counter=0;
// while (active_object_counter<dynamic_object_index)
// {
 
//    current_object_ptr=&ocb_ptr->object_list[object_counter];
 // The screen display is centered around Gx,Gy,Gz at 0,0,0
 // To display Z we effect y by a quarter of the change in Z.
 //
 // Sy = Gy + Gz/4 + screen_height/2
 // Sx = Gx + screen_width/2
 

//             screen_x=current_object_ptr->Oworldx;
//             screen_x-=x_off;		  	  // shift start point
//             screen_x*=scale_x;
//	screen_x+=screen_cent_x;	 	 	 // center in x direction
//             screen_y=current_object_ptr->Oworldz;
//             screen_y-=y_off;		  	  // shift start point
//             screen_y*=scale_y;
//             screen_y+=screen_cent_y;		  	  // center in y direction
//    	       screen_y=monitor_h-screen_y;		  // invert y for screen plotting
    	     
//    if (screen_y>=0 && screen_y<monitor_h && screen_x>=0 && screen_x<monitor_w)
//       {
//
//       if(object_counter==0)	     //zex
//                    {
//                     dlp_draw_box(screen_y-2,screen_x-2,screen_y+2, screen_x+2,zex_colour);
//
//                     PascalToC("\pZex",temp_str);
//            	  	   if(screen_x < (monitor_w * 0.92))
//              		  	  {
//              		  	  SplatText_Small_Transparent((char*)temp_str, screen_x+20 , screen_y-20,zex_colour);
//              		  	  draw_line_no_clip(screen_x+4, screen_y-4, screen_x+18, screen_y-18, zex_colour);
//              		  	  }
//            	  	   else
//              		  	  { 
//              		  	  SplatText_Small_Transparent((char*)temp_str, (monitor_w*0.92) , screen_y-20,zex_colour);
//              		  	  draw_line_no_clip(screen_x+4, screen_y-4, (monitor_w*0.92), screen_y-18, zex_colour);
//              		  	  }
//
//
//                    }
//       else
//       if ((current_object_ptr->in_use==1))	//zex or vv
//         {
//         //if it's a sun or planet we need to process it
//         if (current_object_ptr->Dyn_OCB_control_data.controller==PLANET_CONTROL)
//             {
//             draw_planetary_object(screen_y, screen_x, 2, current_object_ptr->radius*scale_x, colour);
//
//             //labels under planets
//             PascalToC(current_object_ptr->Dyn_OCB_control_data.name_str,temp_str);
//	           if(screen_x>(monitor_w*0.93)) screen_x-=40;
//	           if(screen_y>(monitor_h*0.95)) screen_y-=20;
//             SplatText_Small_Transparent((char*)temp_str, screen_x , screen_y+10,colour);
// 	colour+=10;
//              }
//          else
//          if(current_object_ptr->Dyn_OCB_control_data.controller==SUN)
//            {
//            draw_planetary_object(screen_y, screen_x, 3, current_object_ptr->radius*scale_x, 248);
//
//            PascalToC(current_object_ptr->Dyn_OCB_control_data.name_str,temp_str);
//            if(screen_x < (monitor_w * 0.92))
//              {
//              SplatText_Small_Transparent((char*)temp_str, screen_x+20 , screen_y+20,248);
//              draw_line_no_clip(screen_x+4, screen_y+4, screen_x+18, screen_y+18, 248);
//              }
//            else
//              { 
//              SplatText_Small_Transparent((char*)temp_str, (monitor_w*0.92) , screen_y+20,248);
//              draw_line_no_clip(screen_x+4, screen_y+4, (monitor_w*0.92), screen_y+18, 248);
//              }
//            
//            }
//          active_object_counter+=1;
//          } // end of if in use
//          
//      } // end of if on screen
//
//   else	// we might need to increment active_object_counter even if not on screen
//      {
//       if(object_counter==0)	     //zex
//         { /* nothing */   }
//       else
//       if ((current_object_ptr->in_use==1))	//zex or vv
//         {          
//         if (current_object_ptr->Dyn_OCB_control_data.controller==PLANET_CONTROL)
//             {
// 	colour+=10;		  // update planet colour for hidden planets
//              }
//         active_object_counter+=1; 
//         } // end of if in use
//      }
//      
//  object_counter++;
//
//  } // end of while loop
//
//    DoSolarSelectionCode();
//
//    ZexGetMouse (&solar_mouse_xy);
//    if(solar_mouse_xy.h<0) solar_mouse_xy.h = 0;
//    if(solar_mouse_xy.h >= monitor_w) solar_mouse_xy.h = monitor_w-1;
//    if(solar_mouse_xy.v<0) solar_mouse_xy.v = 0;
//    if(solar_mouse_xy.v >= monitor_h) solar_mouse_xy.v = monitor_h-1;
//    
//    if( ! special_cursor)
//      {
//      print_crunched_trans_pict(map_cursor_picture,solar_mouse_xy.h,solar_mouse_xy.v);
//      }
//    else if(special_cursor==1)
//      {
//      dlp_draw_unfilled_box(solar_mouse_xy.v-4, solar_mouse_xy.h-4, 
//                         solar_mouse_xy.v+4, solar_mouse_xy.h+4,225);
//      }
//    else
//      {
//      dlp_draw_box(solar_mouse_xy.v-4, solar_mouse_xy.h-4, 
//                         solar_mouse_xy.v+4, solar_mouse_xy.h+4,5);
//      }
    
            
	SetGDevice(saveGDH);	//these are here so I can SEE them!
	SetPort(savePort);

} // end of do_galaxy_map_single_frame
