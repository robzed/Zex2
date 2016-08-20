/*
 * draw_radar_and_cmp.cpp
 *
 * $Log: draw_radar_and_cwp.cpp,v $
 * Revision 1.3  2003/09/27 20:41:43  robp
 * Fixed float->int warnings.
 *
 * Revision 1.2  2003/09/20 12:56:56  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:35:16  stu_c
 * First Imported.
 *
 * Revision 1.17  2002/09/08 02:08:24  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.16  2002/07/28 17:17:04  stu_c
 * Exhaust particles and further work
 *
 * Revision 1.15  2002/05/05 22:26:03  stu
 * Further developement - hull temperature now correct for multiple suns and atmosphere.

All calls to add_to_kill_list check the return
 *
 * Revision 1.14  2002/04/28 06:24:30  stu
 * This checkin adds more to the HUD and brings out some sim variables to the player.
 *
 * Revision 1.13  2002/04/20 18:36:19  stu
 * Furhter work for AP and control at time>1
 *
 * Revision 1.12  2002/02/23 01:11:10  stu
 * Fix for radar not working on release build.
 *
 * Revision 1.11  2002/02/17 06:19:56  stu
 * Added radar targetting
 *
 * Revision 1.10  2002/02/16 19:01:24  stu
 * Radar range decrease key and rationalisation of dlp colours
 *
 * Revision 1.9  2002/02/04 23:12:48  stu
 * time accelerator removal from internal timings, only throbs if alien displayed, cwp working
 *
 * Revision 1.8  2001/12/29 01:11:08  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.7  2001/12/02 19:32:05  stu
 * Radar runs every fifth frame. Now draws bottom, graticule, top items.
 *
 * Revision 1.6  2001/11/26 23:16:20  rob
 * Galaxy hide screen stuff
 *
 * Revision 1.5  2001/11/24 19:28:47  stu
 * Dunno
 *
 * Revision 1.4  2001/11/09 18:28:59  rob
 * Various debug changes.
 *
 * Revision 1.3  2001/11/02 19:31:53  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:28:15  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:01:41  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:04  rob
 * First Imported.
 *
 * Revision 1.11  2001/09/23 18:59:18  stu
 * AP work, improved number print, engines.
 *
 * Revision 1.10  2001/09/21 19:59:39  rob
 * Better names = happer Stu
 *
 * Revision 1.9  2001/09/18 20:39:44  stu
 * More Sim/ hud numbers print
 *
 * Revision 1.8  2001/09/18 20:03:38  rob
 * Added flashing radar indicator for targetted object.
 *
 * Revision 1.7  2001/09/10 21:42:08  stu
 * 100901
More Sim, AP broken.
 *
 * Revision 1.6  2001/09/04 20:35:53  stu
 * Sim stuff
 *
 * Revision 1.5  2001/07/29 10:41:09  rob
 * radar_grat and time_in_secs size change
 *
 * Revision 1.4  2001/06/23 20:01:32  stu
 * 0.66 checkin
 *
 * Revision 1.3  2001/06/01 21:36:57  stu
 * Radar now translucent
 *
 * Revision 1.2  2001/05/28 01:49:09  stu
 * 280501
 *
 * Revision 1.1.1.1  2001/01/01 21:13:38  rob
 * First Imported.
 *
 * Revision 1.13  2000/12/10 03:35:52  stu
 * Calls to kill replaced with either calls to add_to_kill_list OR
i_know_what_im_doing_kill
 *
 * Revision 1.12  2000/12/04 00:06:57  rob
 * Added extra type just in case another object type is added
 *
 * Revision 1.11  2000/12/03 23:52:34  rob
 * changed constants for object type to get uniformity
 *
 * Revision 1.10  2000/12/03 23:01:19  rob
 * Fixed Radar
 *
 * Revision 1.9  2000/11/20 20:02:22  rob
 * Altered radar ranges, and also made planets appear on radar
 *
 * Revision 1.8  2000/10/08 21:45:51  stu
 * Engine conversion to 64 bit. Huge pain of a job.
 *
 * Revision 1.7  2000/10/01 14:28:56  rob
 * Altered centerings for new ranges
 *
 * Revision 1.6  2000/10/01 14:06:41  rob
 * Altered the ranges for the radar to closer map to the vastness of space. Provisional only.
 *
 * Revision 1.5  2000/09/20 23:54:09  stu
 * HUD/AP/Scaling
 *
 * Revision 1.4  2000/09/13 22:06:34  stu
 * Added planet buffet to temperature warning
 *
 * Revision 1.3  2000/09/13 20:30:44  stu
 * Changes for hud targetting and sun corona effects.
 *
 * Revision 1.2  2000/09/09 21:08:19  stu
 * Radar changes - colour etc.
 *
 * Revision 1.1  2000/09/05 00:38:36  stu
 * Removed old radar graticule.
Added new dynamic one.
 *
 * Revision 1.0.0.1  2000/08/21 22:06:47  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <MacTypes.h>
  #endif
  
  #include "3d_structures_def.h"
  #include "options.h"
  #include "processor.h"
  #include "zex_misc.h"
  #include "access_ocbs.h"
#endif //pch

#include "pictures.h"
#include "matrix_primitives.h"
#include "dlp.h"
#include "fcircle.h"
#include "physics.h"
#include "txtsplat.h"
#include "instrum.h"
#include "Zsound.h"
#include "hud.h"
#include "splat_control.h"
#include    "fpoly4.h"
#include "genpoly.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "OGL.h"
#include "camera_control.h"
#include "electrics.h"
#include "draw_radar_and_cwp.h"
#include "objects_misc.h"

//#define GREEN_START 251
#define GREEN_START 220
#define RED_START 219
//#define GREY_START 249
#define GREY_START 220	//throb colour

//#define WARM_UP_END 248
#define WARM_UP_END 218

//module_private void SplatEnemy(int colour, int x_pos, int y_pos);
//module_private void SplatBlock(int colour, int x_pos, int y_pos);
module_private void SplatVLine(int colour, int x_pos, int y_pos,int y_pos2);
module_private void draw_radar_graticule();
module_private void draw_radar_range(unsigned char colour);
module_private void draw_light (int x, int y, int status);
void SplatLine (int colour, int x1, int y1, int x2, int y2);

extern int	thrust;
extern int hud_initting;
extern int gHideTheScreen;
//unsigned char no_string[12];
static char throb_delta=1;	
int do_throb=0;	//this is changed to 1 if aliens detected (Radar graticule)
float grat_flash_time=0.2;
static unsigned char radar_warm_up_colour=WARM_UP_END;

long score,credits;
int lives;
//int sys, con;
//int dummy;
int missile_flip;
//scheduling vars
//int last_shields,shields_display_count;
//int last_fuel,fuel_display_count;
//int do_splat_radar;

int need_to_do_cp_update;
int  radar_x = 255;
int  radar_y = 370;
extern ship_power_status_type ship_power_status;

//Zex 2 RADAR
radar_entry radar_list[151];	//150 transformed points for radar
radar_display_entry radar_display_above[151];
radar_display_entry radar_display_below[151];

int radar_points_counter;
float gradar_range;

int target_flashing(int targetted_flag);


double flash_time=0;

int target_flashing(int targetted_flag)
{
if(!targetted_flag) return TRUE;

flash_time+=real_frame_time_in_secs;

if(flash_time>0.125)
  {
  if(flash_time > 0.25)
    {
    flash_time=0;
    }
  return TRUE;
  }
else
  {
  return FALSE;
  }
}


int can_do_radar_display_calcs=1;
int top_index=0;
int bottom_index=0;

void update_radar()
{
int i;
int object_type;
int x1,y1,y2;
int the_object;
float scale_x, scale_y;

vector vect;
the_object=get_main_camera_object();
radar_x=screen_cent_x-65;
radar_y=monitor_h-110;

if (is_ac_good(the_object)==0) return;

    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
 


if (can_do_radar_display_calcs)
{
bottom_index=0;
top_index=0;

i=0; 
do_throb=0;

 while (i<radar_points_counter)	//process all "in use" objects
 {
    object_type=radar_list[i].the_type;
      
    vect.x = radar_list[i].the_vect.x;
    vect.y = radar_list[i].the_vect.y;
    vect.z = radar_list[i].the_vect.z;

 if (target_flashing(radar_list[i].is_target) )	    //is object in range of radar - global var
 {

  if (object_type==ALIEN) 
  {
   do_throb=1;
   grat_flash_time=0.14;	  
  }

 //our picture size is 128*108
 scale_x=gradar_range/128;
 scale_y=gradar_range/108;
 //x is x1, z is y1, y adds to y1


 x1=STATIC_CAST_TO_INT((radar_x+64)+(vect.x/scale_x));
 y1=STATIC_CAST_TO_INT((radar_y+54)-(vect.z/scale_y));
 y2=STATIC_CAST_TO_INT(y1-vect.y/scale_y);
 
//Next line not needed any more
// if ((x1>radar_x) && (x1<(radar_x+150)) && (y1>radar_y) && (y1<(radar_y+100)) && (y2>radar_y) && (y2<(radar_y+100)))
 {
 int the_colour;
	if (object_type==ALIEN)	//rmparian
	the_colour=218;	//orange - R'MParian
	else
	if (object_type==HUMAN)		
	the_colour=228;	//green - Human
	else 
	if (object_type==SCENARY)
	the_colour=235; 	// blue - scenary
	else 
	if (object_type==MASSIVE_SCENARY)
	the_colour=245; 	// white - planets 
	else
	the_colour=5; 	// yellow - unknown object type (defensive?)

			  
	if (y2<y1)	//above graticule (Screen coords)
    {
     radar_display_above[top_index].x1=x1;
     radar_display_above[top_index].y1=y1;   
     radar_display_above[top_index].y2=y2;
     radar_display_above[top_index].the_colour=the_colour;
     top_index++;
    }
    else //below graticule
    {
     radar_display_below[bottom_index].x1=x1;
     radar_display_below[bottom_index].y1=y1;
     radar_display_below[bottom_index].y2=y2;
     radar_display_below[bottom_index].the_colour=the_colour;
     bottom_index++;
    }
			  
   } //end of clip test
 } //end of in radar range test
    
i++;           
} // end of while     
}	//end of can do radar display


//do the drawing
//items below the graticule


i=0;
while(i<bottom_index)
{
			  
              SplatVLine(radar_display_below[i].the_colour, radar_display_below[i].x1,
			            radar_display_below[i].y1, radar_display_below[i].y2);
i++;
}

//draw graticule
glDisable (GL_BLEND);
draw_radar_graticule();
glEnable (GL_BLEND);

//items above the graticule
i=0;
while(i<top_index)
{
			  
              SplatVLine(radar_display_above[i].the_colour, radar_display_above[i].x1,
			            radar_display_above[i].y1, radar_display_above[i].y2);
i++;
}

glDisable (GL_BLEND);

}


 
 
 
 
void SplatLine (int colour, int x1, int y1, int x2, int y2)
{
 OGL_line( x1, y1, x2,y2, colour);
}



void SplatVLine(int colour, int x_pos, int y_pos,int y_pos2)
 {
 
 OGL_line( x_pos, y_pos, x_pos,y_pos2, colour);
 OGL_line( x_pos, y_pos2, x_pos+2, y_pos2, colour);

 }





static unsigned char grat_colour=GREY_START;
static unsigned char colour_start=GREY_START;

//static int radar_initting;

static float grat_colour_time=0;	//how often hud calcs run
//line drawn graticule

int radar_grat=TRUE;

void draw_radar_graticule()
{
if(radar_grat==FALSE) return;

if (hud_initting>0 && radar_warm_up_colour==WARM_UP_END) radar_warm_up_colour=WARM_UP_END+5;

if (radar_warm_up_colour==WARM_UP_END)
{  
   if (do_throb==1)
   {
   grat_colour_time+=real_frame_time_in_secs;
   if (grat_colour_time>grat_flash_time)	//LOVE IT!
    {
     grat_colour_time=0;
     grat_colour+=throb_delta;
     if (grat_colour>colour_start+2) throb_delta=-throb_delta;
     if (grat_colour<colour_start) throb_delta=-throb_delta;
    }
    felipse_nofill_noclip(screen_cent_x, monitor_h-61, 40, 34, grat_colour);
    felipse_nofill_noclip(screen_cent_x, monitor_h-60, 80, 67, grat_colour);
    felipse_nofill_noclip(screen_cent_x, monitor_h-56, 120, 96, grat_colour);
    draw_radar_range(WARM_UP_END);

   }	//end of do_throb
   else
   {
   
    felipse_nofill_noclip(screen_cent_x, monitor_h-61, 40, 34, GREEN_START);
    felipse_nofill_noclip(screen_cent_x, monitor_h-60, 80, 67, GREEN_START);
    felipse_nofill_noclip(screen_cent_x, monitor_h-56, 120, 96, GREEN_START);
    draw_radar_range(WARM_UP_END);
  	
   }
}
else	//we have not warmed up
{
   grat_colour_time+=real_frame_time_in_secs;
   if (grat_colour_time>1.2)	//LOVE IT!
   {
    grat_colour_time=0;
    radar_warm_up_colour--;
   }
    felipse_nofill_noclip(screen_cent_x, monitor_h-61, 40, 34, radar_warm_up_colour);
    felipse_nofill_noclip(screen_cent_x, monitor_h-60, 80, 67, radar_warm_up_colour);
    felipse_nofill_noclip(screen_cent_x, monitor_h-56, 120, 96, radar_warm_up_colour);

}

}

void draw_radar_range(unsigned char colour)
{
if (check_for_object_near()==-1) //green rane if no objects near else red
{
 print_distance_in_metres_3_digit_just(screen_cent_x , monitor_h-39, (gradar_range*(ZEX_UNIT_SIZE_IN_METERS))/2,0, 0.7, 0, 1);
}
else
{
 print_distance_in_metres_3_digit_just(screen_cent_x , monitor_h-39, (gradar_range*(ZEX_UNIT_SIZE_IN_METERS))/2,0.5, misc_text_green, misc_text_blue,1);
}

//print_mk(screen_cent_x-9 , monitor_h-39, (gradar_range*(ZEX_UNIT_SIZE_IN_METERS))/2, misc_text_red, misc_text_green, misc_text_blue);	//change 9/12/00 so range is radius and not diameter	
}


static float cwp_calc_time=0;
int cwp_power=0;
int cwp_temp=0;
int cwp_shields=0;
int cwp_fuel=0;
int cwp_radar=0;
int cwp_weapons=0;
int cwp_linear_damper=0;
int cwp_autopilot=0;

extern float hud_fuel;
extern int gvel_limiter;
extern kit_desc ship_items[ITEMS_COUNT];

void run_cwp(void)	//central warning panel
{
int light_pos;
float hull_temperature;

if(gHideTheScreen==1) return;
//processing once per 5 seconds
   cwp_calc_time+=real_frame_time_in_secs;
   if (cwp_calc_time>0.2)
    {
     cwp_calc_time=0;

     if (is_dc_good(0))
     {    
      hull_temperature=get_hull_temperature_in_C(get_main_camera_object());
//calc temp warning
      if (hull_temperature>3000)
      {
      if (cwp_temp!=1)
       {
             add_to_text_display("OBC> Danger: Hull temperature excessive",DLP_RED); 
             play_zsound(danger, sound_low_pri, speech_chan, sound_vol_7);
       }
       cwp_temp=1;
      
      }
      else
      cwp_temp=2;
      
            
      if (!is_ac_good(0))
      {
      cwp_power=1;
      cwp_radar=1;
      cwp_weapons=1;
      cwp_linear_damper=1;
      cwp_autopilot=1;
      }
      else	//ac is on
      {
      
//      if(gvel_limiter==0)	//ld on
//      {
//       cwp_linear_damper=3;
//      }
//      else
//       cwp_linear_damper=0;
      
//      if(AP_aligned==0)	//ap aligned
//      {
//       cwp_autopilot=3;
//      }
//      else
//       cwp_autopilot=0;
      
      
      cwp_power=2;
      if (radar_warm_up_colour==WARM_UP_END)
      {
        cwp_radar=2;
      }
      else
        cwp_radar=1;
      
      if(hud_initting==1)
       {
        cwp_weapons=1;
       }
       else
       {
        cwp_weapons=2;
       }
       
       if (hud_fuel>3)
        cwp_fuel=2;
       else
       {
        if (cwp_fuel!=1 && hud_initting==0)
         {
             add_to_text_display("OBC> Danger! Fuel low",DLP_RED); 
             play_zsound(danger, sound_low_pri, speech_chan, sound_vol_7);
         }
        cwp_fuel=1;
       }
        
        
        if (ship_items[KSHIELDS].value1>40)
        cwp_shields=2;
        else
        cwp_shields=1;	//wot shields?
//now we have the startup/power status       
      }	//end of ac on
      
      
     }
     else	//dc off
     {
      cwp_power=0;
      cwp_temp=0;
      cwp_shields=0;
      cwp_fuel=0;
      cwp_radar=0;
      cwp_weapons=0;
      cwp_linear_damper=0;
      cwp_autopilot=0;
     }
    }

//display every frame
	light_pos=monitor_h-15;
glBegin(GL_LINES);
   draw_light(screen_cent_x-90,light_pos,cwp_power);	//Power
   draw_light(screen_cent_x-70,light_pos,cwp_fuel);	//Low Fuel
   draw_light(screen_cent_x-50,light_pos,cwp_radar);	//Radar
   draw_light(screen_cent_x+50,light_pos,cwp_temp);	//temperature
   draw_light(screen_cent_x+70,light_pos,cwp_shields);	     //Low shields
   draw_light(screen_cent_x+90,light_pos,cwp_weapons);	     //Weapons
glEnd();
   SplatText_Small_Transparent("PR FL SC",screen_cent_x-89,monitor_h-9, 215 );
   SplatText_Small_Transparent("TP SH WP",screen_cent_x+51,monitor_h-9, 215 );
}

static float cwp_flash_time;
static unsigned char flash_colour=216;

void draw_light (int x, int y, int status)
{
unsigned char colour;
int red,green,blue;
float temp_r3,temp_b3,temp_g3;
float temp_r4,temp_b4,temp_g4;

cwp_flash_time+=real_frame_time_in_secs;
if (cwp_flash_time>0.9)	
{
   cwp_flash_time=0;
   if (flash_colour==216) flash_colour=250;
   else flash_colour=216;
     
}

if (status==0) return;	//colour=250;	//grey

if (status>3)
{
// show_text_in_loading_screen("Zex: Bad status in draw_light\n");
		 #if ZEX_PROJECT_BUILDER==1 //can only printf to OSX syscon
		fprintf (stderr, "****BAD STATUS in draw_light\n"); 
        #endif
 return;
}

if (status==1) colour=flash_colour;	//red flashing
else
if (status==2) colour=227;	//green
else
if (status==3) colour=126;	//green
else colour=126;

     convert_to_rgb(colour+3, &red, &green, &blue);
     temp_r3=red;
     temp_g3=green;
     temp_b3=blue;
     temp_r3=temp_r3/255;	//0-1
     temp_g3=temp_g3/255;	//0-1
     temp_b3=temp_b3/255;	//0-1

     convert_to_rgb(colour+4, &red, &green, &blue);
     temp_r4=red;
     temp_g4=green;
     temp_b4=blue;
     temp_r4=temp_r4/255;	//0-1
     temp_g4=temp_g4/255;	//0-1
     temp_b4=temp_b4/255;	//0-1


	   OGL_line_rgb_quick(x+1,y,x+7,y,temp_r4,temp_g4,temp_b4);
	   OGL_line_rgb_quick(x,y+1,x+8,y+1,temp_r3,temp_g3,temp_b3);
	   OGL_line_rgb_quick(x+1,y+2,x+7,y+2,temp_r4,temp_g4,temp_b4);

}
