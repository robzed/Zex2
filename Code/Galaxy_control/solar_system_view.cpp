// *************************************MODULE HEADER*********************************
// * FILENAME           : solar_system_view.c
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * DATE STARTED       : 09:18:00 on 23rd Septemeber 2001
// * FIRST AUTHOR       : Rob Probin
// *
// * COPYRIGHT (c)        2001 Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"


  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <textutils.h>
  #endif

  #include "options.h"
  #include "3d_structures_def.h"
  #include	"zex_misc.h"
  #include "Error_handler.h"
  #include "processor.h"
  #include "access_ocbs.h"
#endif //pch

#include "Galaxy.h"

#include "objects_controls.h"
#include "game_defs.h"
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
#include "POBS.h"
#include "Physics.h"
// ***********************************************************************************
// * CONSTANTS 
// *


// ***********************************************************************************
// * TYPE DEFINITIONS
// *


// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

int zex_colour;
int zex_colour_delta=-1;
Point solar_mouse_xy;


extern int OGL_active;
extern	DynObjectsFixedSize *ocb_ptr;
extern int window_w, window_h;

//variables that control the planet info display
static int previous_button=0;
static int selected_object=0;
static int info_pos_x;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

void calc_ss_extents(void);
void draw_item_info(int the_object, int screen_x, int screen_y, int colour);
void concat_supply_type(unsigned char* the_string, int supply_type);

// ***********************************************************************************
// * ACTUAL CODE
// *


/* 
 * Variables for the solar system map
 */

static vector solar_extents_max;
static vector solar_extents_min;

int cursor_dragging;		// set to true when we are selecting a portion
Point start_selection_pos;

int special_cursor;		// cursor mode when 

// stuff for the scale of the viewport
double scale_x,scale_y;
double x_off,y_off;

#define INCREMENTALLY_SCALE 1
#define TOTAL_CHANGE_TIME 0.5
#define DUAL_CHANGE_TIME (TOTAL_CHANGE_TIME/2)
#define SEPERATE_ZOOM 0		// if no seperate zoom, does loopy thing

// stuff to incrementally change viewport
double inc_scale_x,inc_scale_y;
double inc_x_off,inc_y_off;
int frames_to_change_offset;
int frames_to_change_scale;
int swap_change_order;		// changes scale before offset, instead of vis-versa.


/* check_scale_is_valid
 *
 * DESCRIPTION: Ensure scale is valid
 *
 */
void check_scale_is_valid(double *scale_x, double *scale_y);
void check_scale_is_valid(double *scale_x, double *scale_y)
{
if(*scale_x>*scale_y) *scale_x=*scale_y; else *scale_y=*scale_x;	// take smallest scale factor

if(*scale_x > 1e-6) { *scale_x=1e-6; *scale_y=*scale_x; }	// 800km
if(*scale_x < 1e-13) { *scale_x=1e-13; *scale_y=*scale_x; }

}

/* set_solar_view_size_to_max_instant
 *
 * DESCRIPTION: sets the view size of the solar system to max instantly
 *
 */
void set_solar_view_size_to_max_instant(void);
void set_solar_view_size_to_max_instant(void)
{
float solar_extent_x,solar_extent_y;

//find extents of solar system so we can calculate plotting ratios
calc_ss_extents();
solar_extent_x=solar_extents_max.x-solar_extents_min.x;
solar_extent_y=solar_extents_max.z-solar_extents_min.z;

scale_x=(window_w*0.96)/solar_extent_x;			// was 620.0
scale_y=(window_h*0.96)/solar_extent_y;			// was 460.0

check_scale_is_valid(&scale_x, &scale_y);

// offset around middle of playing area
x_off=solar_extent_x/2+solar_extents_min.x;
y_off=solar_extent_y/2+solar_extents_min.z;
}


/* set_solar_view_size_to_max
 *
 * DESCRIPTION: sets the view size of the solar system to max
 *
 */
void set_solar_view_size_to_max(void);
void set_solar_view_size_to_max(void)
{
double solar_extent_x,solar_extent_y;
double target_x_off, target_y_off;
double target_scale_x, target_scale_y;

//find extents of solar system so we can calculate plotting ratios
calc_ss_extents();
solar_extent_x=solar_extents_max.x-solar_extents_min.x;
solar_extent_y=solar_extents_max.z-solar_extents_min.z;

target_scale_x=(window_w*0.96)/solar_extent_x;			// was 620.0
target_scale_y=(window_h*0.96)/solar_extent_y;			// was 460.0

check_scale_is_valid(&target_scale_x, &target_scale_y);

// offset around middle of playing area
target_x_off=solar_extent_x/2+solar_extents_min.x;
target_y_off=solar_extent_y/2+solar_extents_min.z;

#if INCREMENTALLY_SCALE

  frames_to_change_offset = DUAL_CHANGE_TIME/real_frame_time_in_secs;
  frames_to_change_scale = DUAL_CHANGE_TIME/real_frame_time_in_secs;
  
  inc_x_off = (target_x_off - x_off) / frames_to_change_offset;
  inc_y_off = (target_y_off - y_off) / frames_to_change_offset;
  inc_scale_x = (target_scale_x - scale_x) / frames_to_change_scale;
  inc_scale_y = (target_scale_y - scale_y) / frames_to_change_scale;

  swap_change_order=1;
  
  if(frames_to_change_offset==0)	// if no time to change, change instantly
    {
    x_off = target_x_off; y_off = target_y_off;
    scale_x = target_scale_x; scale_y = target_scale_y;
    }
#else
  x_off = target_x_off; y_off = target_y_off;
  scale_x = target_scale_x; scale_y = target_scale_y;
#endif
}

/* set_solar_view_size_double
 *
 * DESCRIPTION: sets the view size of the solar system to double
 *
 */
void set_solar_view_size_double(void);
void set_solar_view_size_double(void)
{
double target_scale_x, target_scale_y;

target_scale_x=scale_x/2;
target_scale_y=scale_y/2;

check_scale_is_valid(&target_scale_x, &target_scale_y);

#if INCREMENTALLY_SCALE
  frames_to_change_scale = DUAL_CHANGE_TIME/real_frame_time_in_secs;
  frames_to_change_offset = 0;
  
  inc_scale_x = (target_scale_x - scale_x) / frames_to_change_scale;
  inc_scale_y = (target_scale_y - scale_y) / frames_to_change_scale;

  swap_change_order=1;
  
  if(frames_to_change_scale==0)	// if no time to change, change instantly
    {
    scale_x = target_scale_x; scale_y = target_scale_y;
    }
#else
  scale_x = target_scale_x; scale_y = target_scale_y;
#endif
}


/* set_solar_view_offset
 *
 * DESCRIPTION: sets the view size of the solar system to a specified offset
 *
 */
void set_solar_view_offset(int v1, int h1, int v2, int h2);
void set_solar_view_offset(int v1, int h1, int v2, int h2)
{
double target_x_off, target_y_off;

// offset around middle of selection
target_x_off = x_off + (((h2+h1)/2) - (window_w/2)) / scale_x;			// was 320.0
target_y_off = y_off - (((v2+v1)/2) - (window_h/2)) / scale_y;		// was 240.0

#if INCREMENTALLY_SCALE

  frames_to_change_offset = TOTAL_CHANGE_TIME / real_frame_time_in_secs;
  frames_to_change_scale = 0;
  
  inc_x_off = (target_x_off - x_off) / frames_to_change_offset;
  inc_y_off = (target_y_off - y_off) / frames_to_change_offset;
  inc_scale_x = 0;
  inc_scale_y = 0;
  
  swap_change_order=0;  

  if(frames_to_change_offset==0)	// if no time to change, change instantly
    {
    x_off = target_x_off; y_off = target_y_off;
    }

#else
  x_off = target_x_off; y_off = target_y_off;
#endif
}


/* set_solar_view_size
 *
 * DESCRIPTION: sets the view size of the solar system to a specified size
 *
 */
void set_solar_view_size(int v1, int h1, int v2, int h2);
void set_solar_view_size(int v1, int h1, int v2, int h2)
{
double target_x_off, target_y_off;
double target_scale_x, target_scale_y;

// offset around middle of selection
target_x_off = x_off + (((h2+h1)/2) - (window_w/2)) / scale_x;		// was 320.0
target_y_off = y_off - (((v2+v1)/2) - (window_h/2)) / scale_y;		// was 240.0

// the scale is a fraction of the selection
target_scale_x = scale_x * (window_w / (h2-h1));			// was 640.0
target_scale_y = scale_y * (window_h / (v2-v1));			// was 480.0

check_scale_is_valid(&target_scale_x, &target_scale_y);

#if INCREMENTALLY_SCALE

  frames_to_change_offset = DUAL_CHANGE_TIME/real_frame_time_in_secs;
  frames_to_change_scale = DUAL_CHANGE_TIME/real_frame_time_in_secs;
  
  inc_x_off = (target_x_off - x_off) / frames_to_change_offset;
  inc_y_off = (target_y_off - y_off) / frames_to_change_offset;
  inc_scale_x = (target_scale_x - scale_x) / frames_to_change_scale;
  inc_scale_y = (target_scale_y - scale_y) / frames_to_change_scale;
  
  swap_change_order=0;

  if(frames_to_change_offset==0)	// if no time to change, change instantly
    {
    x_off = target_x_off; y_off = target_y_off;
    scale_x = target_scale_x; scale_y = target_scale_y;
    }
#else
  x_off = target_x_off; y_off = target_y_off;
  scale_x = target_scale_x; scale_y = target_scale_y;
#endif
}

/* init_solar_system_map
 *
 * DESCRIPTION: sets the magnification of the solar system map to zero.
 *
 */
void reinit_solar_system_map(void)
{
cursor_dragging=0;
special_cursor=0;
frames_to_change_offset=0;
frames_to_change_scale=0;
swap_change_order=0;  
}

/* init_solar_system_map
 *
 * DESCRIPTION: sets the magnification of the solar system map to zero.
 *
 */
void init_solar_system_map(void)
{
reinit_solar_system_map();
set_solar_view_size_to_max_instant();
selected_object=0;
info_pos_x=0; previous_button=0;
}

/* DoSolarSelectionCode
 *
 * DESCRIPTION: Selects a region to zoom
 *
 */

void DoSolarSelectionCode(void);
void DoSolarSelectionCode(void)
{
int button;
Point current_drag_pos;

#if INCREMENTALLY_SCALE
  #if SEPERATE_ZOOM

if(frames_to_change_offset!=0 && swap_change_order==0)
  {  frames_to_change_offset--; x_off+=inc_x_off; y_off+=inc_y_off; } 
else if(frames_to_change_scale!=0)	// if we do both at once it sort of loops
  { frames_to_change_scale--; scale_x+=inc_scale_x; scale_y+=inc_scale_y; }
else
  { swap_change_order=0; }

  #else
  
if(frames_to_change_offset!=0)
  {  frames_to_change_offset--; x_off+=inc_x_off; y_off+=inc_y_off; } 
if(frames_to_change_scale!=0)
  { frames_to_change_scale--; scale_x+=inc_scale_x; scale_y+=inc_scale_y; }

  #endif
  
#endif

special_cursor=0;

button=game_fire_button();

if(button && !cursor_dragging)
  {
  // start dragging
  cursor_dragging=1;
  start_selection_pos.h=solar_mouse_xy.h;  start_selection_pos.v=solar_mouse_xy.v;
  }
else if(button && cursor_dragging)
  {
  // doing drag
  current_drag_pos.h=solar_mouse_xy.h;  current_drag_pos.v=solar_mouse_xy.v;

  if(start_selection_pos.v < current_drag_pos.v 
      && start_selection_pos.h < current_drag_pos.h)
    {
    dlp_draw_unfilled_box(start_selection_pos.v, start_selection_pos.h, 
                         current_drag_pos.v, current_drag_pos.h,235);
    }
  else if(start_selection_pos.v-100 >= current_drag_pos.v 
      || start_selection_pos.h-100 >= current_drag_pos.h)
    {
    special_cursor=1;
    }
  else if(start_selection_pos.v-10 >= current_drag_pos.v 
      || start_selection_pos.h-10 >= current_drag_pos.h)
    {
    special_cursor=2;
    }

  }
else if(!button && cursor_dragging)
  {
  // finished dragging
  cursor_dragging=0;

  current_drag_pos.h=solar_mouse_xy.h;  current_drag_pos.v=solar_mouse_xy.v;

  if(start_selection_pos.v < current_drag_pos.v 
      && start_selection_pos.h < current_drag_pos.h)
    {
    dlp_draw_box(start_selection_pos.v, start_selection_pos.h, 
                         current_drag_pos.v, current_drag_pos.h,12);
    set_solar_view_size(start_selection_pos.v, start_selection_pos.h, 
                         current_drag_pos.v, current_drag_pos.h);
    }
  else // a dot or negative size sets it to a different offset
    {
    if(start_selection_pos.v-10 < current_drag_pos.v 
      && start_selection_pos.h-10 < current_drag_pos.h)
      {
      set_solar_view_offset(start_selection_pos.v, start_selection_pos.h, 
                         current_drag_pos.v, current_drag_pos.h);
      }
    else if(start_selection_pos.v-100 < current_drag_pos.v 
      && start_selection_pos.h-100 < current_drag_pos.h)
      {
      set_solar_view_size_double();
      }
    else

      {
      set_solar_view_size_to_max();
      }
    }
  }
}


/* DrawScale
 *
 * DESCRIPTION: Prints a little scale thing on the screen
 *
 */

extern int gHideTheScreen;

void DrawScale(void);
void DrawScale(void)
{
draw_line_no_clip(20, (monitor_h*0.95), 100, (monitor_h*0.95), 246);
draw_line_no_clip(20, (monitor_h*0.95)-5, 20, (monitor_h*0.95)+5, 246);
draw_line_no_clip(100, (monitor_h*0.95)-5, 100, (monitor_h*0.95)+5, 246);
gHideTheScreen=0;
print_distance_in_metres_3_digit_just(105 , (monitor_h*0.95), ((80.0/scale_x)*(ZEX_UNIT_SIZE_IN_METERS)),1.0,1.0,1.0,0);
gHideTheScreen=1;
}


float zex_x, zex_y;

/* draw_planetary_object
 *
 * DESCRIPTION: Shows the size of a planetary object if worth it
 *
 */
void draw_planetary_object(int screen_y,int screen_x,int size, int radius, int colour, int selected);
void draw_planetary_object(int screen_y,int screen_x,int size, int radius, int colour, int selected)
{
if(radius > size+2)
  {
  dlp_draw_unfilled_box(screen_y-radius,screen_x-radius,screen_y+radius,screen_x+radius,colour);
  }

  if (selected)
   {
    //draw a line from here to Zex
     Zline_rgb(screen_x, screen_y, zex_x, zex_y, 0.8,0.8,0.8);
    //Zline_rgb(-50, 100, 2000, 100, 1,1,1);
   }
   dlp_draw_rounded_box(screen_y-size,screen_x-size,screen_y+size,screen_x+size,colour);
}

/* do_solar_map_single_frame
 *
 * DESCRIPTION: Shows a single frame of the solar system
 *
 */

void do_solar_map_single_frame(void)
{
int active_object_counter,object_counter;
float screen_x,screen_y;
//float disp_x,disp_y;
//int zex_display_x,zex_display_y;
extern int dynamic_object_index;
register ZObject * current_object_ptr;
//float zex_x,zex_y;
//float ship_x,ship_y;
//unsigned char number_string[12];
//CGrafPtr the_drawing_buffer;
GDHandle	saveGDH;
GrafPtr		  savePort;
unsigned char temp_str[256];
 unsigned char colour=20;

extern GDHandle		        		mZexGDH;
extern LSRAW *map_cursor_picture;
extern DSpContextReference		mDisplayContext;
int button, scanning; //for info display
Point click_mouse_xy;
static int info_pos_x=0;

          DSpContext_FadeGamma ( mDisplayContext, 100, 0);	//clear any sun glare

//colour flipping for player
if(zex_colour>255) zex_colour_delta=-zex_colour_delta;
if(zex_colour<247) zex_colour_delta=-zex_colour_delta;

zex_colour+=zex_colour_delta;


           
     GetPort (&savePort);
     saveGDH = GetGDevice();	    //All calls to QD are surrounded by save and set gdevices
	 SetGDevice(mZexGDH);
#if OGL==1
    if (OGL_active==1)
    {
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);	//red
	glClear(GL_COLOR_BUFFER_BIT);
    }
    else
    erase_zex_rect_colour(monitor_w,monitor_h,244);	//white
    
#else
    erase_zex_rect_colour(monitor_w,monitor_h,244);	//white
#endif


SplatText_Small_Transparent("Solar Map ",(monitor_w/2)-40,6,246);
DrawScale();

             button=game_fire_button();
              scanning=FALSE;
             if (!button)
             {
              //was button previously down?
              if (previous_button)
              {
               scanning=TRUE;
               ZexGetMouse (&click_mouse_xy);
               click_mouse_xy.h+=5;
               click_mouse_xy.v+=5;
               //selected_object=0; //assume miss
              }
              
            }
            
if (selected_object)
{
  if (info_pos_x<210) info_pos_x+=20;
  glColor3d(0.8, 0.8, 0.8);
   draw_item_info(selected_object,monitor_w-info_pos_x,monitor_h-80,250);
}

active_object_counter=0;
object_counter=0;
 while (active_object_counter<dynamic_object_index)
 {
 
    current_object_ptr=&ocb_ptr->object_list[object_counter];


             screen_x=current_object_ptr->Oworldx;
             screen_x-=x_off;			// shift start point
             screen_x*=scale_x;
	     screen_x+=screen_cent_x;	  	  	  // center in x direction
             screen_y=current_object_ptr->Oworldz;
             screen_y-=y_off;			// shift start point
             screen_y*=scale_y;
             screen_y+=screen_cent_y;			// center in y direction
    	     screen_y=monitor_h-screen_y;		// invert y for screen plotting
    	     
    if (screen_y>=0 && screen_y<monitor_h && screen_x>=0 && screen_x<monitor_w)
       {

       if(object_counter==0)	   //zex
                    {
                     dlp_draw_box(screen_y-2,screen_x-2,screen_y+2, screen_x+2,zex_colour);
                     zex_x=screen_x; zex_y=screen_y;

                     PascalToC("\pZex",temp_str);
            		 if(screen_x < (monitor_w * 0.92))
              			{
              			SplatText_Small_Transparent((char*)temp_str, screen_x+20 , screen_y-20,zex_colour);
              			Zline_rgb(screen_x+4, screen_y-4, screen_x+18, screen_y-18, 0.8,0.8,0.8);
              			}
            		 else
              			{ 
              			SplatText_Small_Transparent((char*)temp_str, (monitor_w*0.92) , screen_y-20,zex_colour);
              			Zline_rgb(screen_x+4, screen_y-4, (monitor_w*0.92), screen_y-18, 0.8,0.8,0.8);
              			}


                    }
       else
       if ((current_object_ptr->in_use==1))	//zex or vv
         {
         //if it's a sun or planet we need to process it
         if (current_object_ptr->Dyn_OCB_control_data.controller==PLANET_CONTROL)
             {
             colour=POBs_get_planet_or_moon_colour(object_counter);
             draw_planetary_object(screen_y, screen_x, 2, current_object_ptr->radius*scale_x,colour,object_counter==selected_object );
             //dlp_draw_rounded_box(screen_y-2,screen_x-2,screen_y+2,screen_x+2,colour);
             if (scanning)
             {
               //yes
               //Are we within a 10 pixel box of the object?
               if ((screen_x>(click_mouse_xy.h-5)) && (screen_x<(click_mouse_xy.h+5)))
               {
                 if ((screen_y>(click_mouse_xy.v-5)) && (screen_y<(click_mouse_xy.v+5)))
                 {
                 //Mark this object as selected
                   selected_object=object_counter;
                   info_pos_x=0;
                 //and target it..
                   set_targetted_for_object(0, object_counter); //hud function
                   //create_selected_object_info(object_info_str, selected_object); //Create the info string.
                 }
               }
              }
             
             
             //labels under planets
             PascalToC(current_object_ptr->Dyn_OCB_control_data.name_str,temp_str);

	         if(screen_x>(monitor_w*0.93)) screen_x-=40;
	         if(screen_y>(monitor_h*0.95)) screen_y-=20;
             SplatText_Small_Transparent((char*)temp_str, screen_x , screen_y+10,colour);
 	       //   colour+=10;
              }
          else
          if(current_object_ptr->Dyn_OCB_control_data.controller==SUN)
            {
            
             if (scanning)
             {
               //yes
               //Are we within a 10 pixel box of the object?
               if ((screen_x>(click_mouse_xy.h-5)) && (screen_x<(click_mouse_xy.h+5)))
               {
                 if ((screen_y>(click_mouse_xy.v-5)) && (screen_y<(click_mouse_xy.v+5)))
                 {
                 //Mark this object as selected
                   selected_object=object_counter;
                   info_pos_x=0;
                 //and target it..
                   set_targetted_for_object(0, object_counter); //hud function
                   //create_selected_object_info(object_info_str, selected_object); //Create the info string.
                 }
               }
              }

            draw_planetary_object(screen_y, screen_x, 3, current_object_ptr->radius*scale_x, 248,object_counter==selected_object);
            //dlp_draw_rounded_box(screen_y-3,screen_x-3,screen_y+3,screen_x+3,248);

            PascalToC(current_object_ptr->Dyn_OCB_control_data.name_str,temp_str);
            if(screen_x < (monitor_w * 0.92))
              {
              SplatText_Small_Transparent((char*)temp_str, screen_x+20 , screen_y+20,248);
              Zline_rgb(screen_x+4, screen_y+4, screen_x+18, screen_y+18, 0.8,0.8,0.8);
              }
            else
              { 
              SplatText_Small_Transparent((char*)temp_str, (monitor_w*0.92) , screen_y+20,248);
              Zline_rgb(screen_x+4, screen_y+4, (monitor_w*0.92), screen_y+18, 0.8,0.8,0.8);
              }
            
            }
          active_object_counter+=1;
          } // end of if in use
          
      } // end of if on screen

   else	// we might need to increment active_object_counter even if not on screen
      {
       if(object_counter==0)	   //zex
         { /* nothing */   }
       else
       if ((current_object_ptr->in_use==1))	//zex or vv
         {          
//         if (current_object_ptr->Dyn_OCB_control_data.controller==PLANET_CONTROL)
//             {
// 	  colour+=10;		// update planet colour for hidden planets
//              }
         active_object_counter+=1; 
         } // end of if in use
      }
      
  object_counter++;

  } // end of while loop
    previous_button=button; //note button state for next frame

    DoSolarSelectionCode();

    ZexGetMouse (&solar_mouse_xy);
    if(solar_mouse_xy.h<0) solar_mouse_xy.h = 0;
    if(solar_mouse_xy.h >= monitor_w) solar_mouse_xy.h = monitor_w-1;
    if(solar_mouse_xy.v<0) solar_mouse_xy.v = 0;
    if(solar_mouse_xy.v >= monitor_h) solar_mouse_xy.v = monitor_h-1;
    
    if( ! special_cursor)
      {
      print_crunched_trans_pict(map_cursor_picture,solar_mouse_xy.h,solar_mouse_xy.v);
      }
    else if(special_cursor==1)
      {
      dlp_draw_unfilled_box(solar_mouse_xy.v-4, solar_mouse_xy.h-4, 
                         solar_mouse_xy.v+4, solar_mouse_xy.h+4,225);
      }
    else
      {
      dlp_draw_box(solar_mouse_xy.v-4, solar_mouse_xy.h-4, 
                         solar_mouse_xy.v+4, solar_mouse_xy.h+4,5);
      }
    
            
	SetGDevice(saveGDH);	//these are here so I can SEE them!
	SetPort(savePort);

} // end of do_solar_map_single_frame


// -----------------------------------------------------------------------
//
// Given an object, draw a bunch of pobs info for it in lower right of screen
//

// ----------------------------
void fill_in_name(int the_object, unsigned char* the_string, int str_offset)
{
register ZObject * current_object_ptr;
current_object_ptr=&ocb_ptr->object_list[the_object];



if (current_object_ptr->Dyn_OCB_control_data.controller==PLANET_CONTROL ||
    current_object_ptr->Dyn_OCB_control_data.controller==SUN )
{
 PascalToC(current_object_ptr->Dyn_OCB_control_data.name_str,&the_string[str_offset]);
}
else
{
 PascalToC("\pNo name",&the_string[str_offset]);
}
}


// ----------------------------
void fill_in_distance(int the_object, unsigned char* the_string, int str_offset)
{
double distance;
 distance=get_distance_to_from_in_m(the_object,0);
 make_3_digit(distance, &the_string[str_offset]);
}

// ----------------------------
void fill_in_rotation_period(int the_object, unsigned char* rot_str, int str_offset)
{
int str_pos;
double days;
register ZObject * current_object_ptr;

current_object_ptr=&ocb_ptr->object_list[the_object];

if (current_object_ptr->Dyn_OCB_control_data.controller==PLANET_CONTROL ||
    current_object_ptr->Dyn_OCB_control_data.controller==SUN )
{
 days = get_POBs_rotation(get_galactic_id(the_object)); //in seconds
 days/=60.0*60.0; //hours
 if (days<1) days=1;
 str_pos=ZNumToStringBig(days, &rot_str[str_offset])+str_offset;
 rot_str[str_pos]=' '; str_pos++;
 rot_str[str_pos]='H'; str_pos++;
 rot_str[str_pos]='o'; str_pos++;
 rot_str[str_pos]='u'; str_pos++;
 rot_str[str_pos]='r'; str_pos++;
 rot_str[str_pos]='s'; str_pos++;
 rot_str[str_pos]=0;
}
else
{
   PascalToC("\pNot applicable",&rot_str[str_offset]);
}

}


// ----------------------------
void fill_in_planet_type(int the_object, unsigned char* type_str, int str_offset)
{
if (POBs_is_this_a_rock_moon(get_galactic_id(the_object)))
{
 copystr(&type_str[str_offset],"Moon");
}
else
if (POBs_is_this_earthish_size(get_galactic_id(the_object)))
{
 copystr(&type_str[str_offset],"Earth Size");
}
else
if (POBs_is_this_a_sun(get_galactic_id(the_object)))
{
 copystr(&type_str[str_offset],"Star");
}
else
if (POBs_is_this_a_hypermassive_gas_giant(get_galactic_id(the_object)))
{
 copystr(&type_str[str_offset],"Hypermassive Gas");
}
else
if (POBs_is_this_a_giant_planet(get_galactic_id(the_object)))
{
 copystr(&type_str[str_offset],"Giant");
}
else
if (POBs_is_this_a_gas_giant(get_galactic_id(the_object)))
{
 copystr(&type_str[str_offset],"Gas Giant");
}
else
{
 copystr(&type_str[str_offset],"Not Known");
}
}


// ----------------------------
void fill_in_production_type(int the_object, unsigned char* stype_str, int str_offset)
{
double pop_value;
int value;
register ZObject * current_object_ptr;

current_object_ptr=&ocb_ptr->object_list[the_object];

if (current_object_ptr->Dyn_OCB_control_data.controller==PLANET_CONTROL ||
    current_object_ptr->Dyn_OCB_control_data.controller==SUN )
{

value = POBs_get_supply_type(get_galactic_id(the_object));
pop_value = POBs_what_is_population(get_galactic_id(the_object));
if (pop_value<1.0) value=0; //if no people then no output

switch (value)
{
 case 1: case 6:
 {
  copystr(&stype_str[str_offset],"Agri and Tech");
  break;
 }
 case 2:
 {
  copystr(&stype_str[str_offset],"Mildly Agricultural");
  break;
 }
 case 3:
 {
  copystr(&stype_str[str_offset],"Somewhat Agricultural");
  break;
 }
 case 4:
 {
  copystr(&stype_str[str_offset],"Mainly Agricultural");
  break;
 }
 case 5:
 {
  copystr(&stype_str[str_offset],"Agricultural");
  break;
 }


 case 7:
 {
  copystr(&stype_str[str_offset],"Mildly Technological");
  break;
 }
 case 8:
 {
  copystr(&stype_str[str_offset],"Somewhat Technological");
  break;
 }
 case 9:
 {
  copystr(&stype_str[str_offset],"Mainly Technological");
  break;
 }
 case 10:
 {
  copystr(&stype_str[str_offset],"Technological");
  break;
 }

 default:
 {
  copystr(&stype_str[str_offset],"None");
 }
}
}
else
{
   PascalToC("\pNot applicable",&stype_str[str_offset]);
}
}

// ----------------------------
void fill_in_population(int the_object, unsigned char* pop_str, int str_offset)
{
double pop_value = POBs_what_is_population(get_galactic_id(the_object));
ZNumToStringBig(pop_value, &pop_str[str_offset]);
}

// ----------------------------


//Must set glColor3d before calling this routine
void draw_item_info(int the_object, int screen_x, int screen_y, int colour)
{
#define X_OFF 12
static unsigned char  name_str[64]= "      Name: ";
static unsigned char stype_str[64]= "    Output: ";
static unsigned char pop_str[64]=   "Population: ";
static unsigned char type_str[64]=  "      Type: ";
static unsigned char rot_str[64]=   "  Rotation: ";
static unsigned char dist_str[64]=  "  Distance: ";

if (the_object<0) return;

//show planet name
fill_in_name(the_object,name_str, X_OFF);
fill_in_distance(the_object,dist_str,X_OFF);
fill_in_planet_type(the_object,type_str,X_OFF);
fill_in_rotation_period(the_object,rot_str,X_OFF);
fill_in_production_type(the_object,stype_str,X_OFF);
fill_in_population(the_object,pop_str,X_OFF);


SplatText_mon_fast((char*)name_str, screen_x , screen_y);
SplatText_mon_fast((char*)type_str, screen_x , screen_y+10);
SplatText_mon_fast((char*)rot_str, screen_x , screen_y+20);
SplatText_mon_fast((char*)stype_str, screen_x , screen_y+30);
SplatText_mon_fast((char*)pop_str, screen_x , screen_y+40);
SplatText_mon_fast((char*)dist_str, screen_x , screen_y+50);

}

void concat_supply_type(unsigned char* the_string, int supply_type)
{
/*
1=Neutral
2=slightly agrigultural
3=Somewhat agricultural
4=Mainly agricultural
5=Agricultural
6=Neutral
7=slightly technological
8=Somewhat technological
9=Mainly technological
10=Technological
*/

}
// -----------------------------------------------------------------------
//
// calculate extents of solar system
//
void calc_ss_extents(void)
{
int initd=0;
int active_object_counter,object_counter;
extern int dynamic_object_index;
register ZObject * current_object_ptr;

active_object_counter=0;
object_counter=0;
 while (active_object_counter<dynamic_object_index)
 {
    current_object_ptr=&ocb_ptr->object_list[object_counter];

    if ((current_object_ptr->in_use==1))	//zex or vv
    {
      //if it's a sun or planet we need to process it
       if (current_object_ptr->Dyn_OCB_control_data.controller==PLANET_CONTROL || current_object_ptr->Dyn_OCB_control_data.controller==SUN)
            {
            if(initd)
              {
              if(current_object_ptr->Oworldx<solar_extents_min.x) 
                solar_extents_min.x=current_object_ptr->Oworldx;
              if(current_object_ptr->Oworldy<solar_extents_min.y) 
                solar_extents_min.y=current_object_ptr->Oworldy;
              if(current_object_ptr->Oworldz<solar_extents_min.z) 
                solar_extents_min.z=current_object_ptr->Oworldz;
                
              if(current_object_ptr->Oworldx>solar_extents_max.x) 
                solar_extents_max.x=current_object_ptr->Oworldx;
              if(current_object_ptr->Oworldy>solar_extents_max.y) 
                solar_extents_max.y=current_object_ptr->Oworldy;
              if(current_object_ptr->Oworldz>solar_extents_max.z) 
                solar_extents_max.z=current_object_ptr->Oworldz;
	      }
	    else
	      {
                solar_extents_min.x=current_object_ptr->Oworldx;
                solar_extents_min.y=current_object_ptr->Oworldy;
                solar_extents_min.z=current_object_ptr->Oworldz;
                
                solar_extents_max.x=current_object_ptr->Oworldx;
                solar_extents_max.y=current_object_ptr->Oworldy;
                solar_extents_max.z=current_object_ptr->Oworldz;
                
                initd=1;		// thankyou and goodnight
	      }
                
            }
      active_object_counter+=1;
    }
 object_counter++;
 }
 
}
