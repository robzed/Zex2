
#ifndef __ZEX_PCH__
  #include "zselector.h"
  #include "zplatform.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <MacTypes.h>
    #include <fp.h>
  #endif

  #include "3d_structures_def.h"
  #include "zex_misc.h"
  #include "processor.h"
  #include "real_time.h"
  #include "access_ocbs.h"
  #include "Error_handler.h"
#endif //pch

#include "dlp.h"
#include "hud.h"
#include "instrum.h"
#include "physics.h"
#include "txtsplat.h"
#include "matrix_primitives.h"
#include "3d_top.h"
#include "fcircle.h"
#include "Zsound.h"
#include "objects_controls.h"
#include "angular_stuff.h"
#include "ship_control.h"
#include "planet_generator.h"
#include "splat_control.h"
#include    "fpoly4.h"
#include "genpoly.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "pictures.h"
#include "OGL.h"
#include "sds.h"
#include "autopilot.h"
#include "engines.h"
#include "electrics.h"
#include "camera_control.h"
#include "draw_radar_and_cwp.h"
#include "hud2.h"
#include "game_defs.h"


extern ship_power_status_type ship_power_status;
extern DynObjectsFixedSize *ocb_ptr;
extern float gradar_range;


extern int ship_buffet;
extern int gHideTheScreen;

kit_desc ship_items[ITEMS_COUNT];

module_private void draw_emergency_sight(int the_object);
module_private void draw_sights(int the_object);
module_private void draw_velocity(int the_object);
module_private void do_hud_calcs(int the_object);
module_private void draw_velocity_vector(int the_object);
module_private void draw_insts(int the_object);
module_private void draw_bar_multi(int value,int x, int y, int draw_code /*1=left circ , 2=left lin, 3=right circ, 4=right lin */ );
module_private int is_it_a_target(int object);
module_private void draw_time_stuff(int the_object);
module_private void do_pull_up(int the_object);
module_private void calc_target_name(int the_object);
module_private void target_nearest_planet(int object);

#define SIGHTS_ROT_PER_SECOND 180
#define SIGHTS_RADIUS_MAX 30
#define SBY_SIGHT_COLOUR 245

#define SIGHT_COLOUR 220	//250
#define HUD_SCHED 10;



#define MISC_TEXT_COLOR 17
#define TARGET_COLOR	11

static float sight_colour_red = 0.8;
static float sight_colour_green = 0;
static float sight_colour_blue = 0;

static float fwd_vv_r =0;
static float fwd_vv_g =1;
static float fwd_vv_b =0;


float misc_text_red=1, misc_text_green=0.0, misc_text_blue=0.1;

static int sights_targetted;
static int old_target;

static float sights_rotation;
static float sights_radius_min;
static int sights_length_delta_per_second;
static float old_x,old_y,old_z;
float hud_calc_velocity;
static int targetting_in_progress_flag;

static float hud_run_time;	  //how often hud calcs run

#define TARGET_STRING_OFFSET 13
#define TARGET_STRING_OFFSET_NO_CALLSIGN 8

static char target_name_str[256]="Target: XXXXX ";
static int  target_name_len;

float zex_rel_velocity;
 

static int pull_up_displayed;
static int fast_flash=0;
static float fast_flash_time;

int hud_initting;
int slow_flash=0;
float hud_fuel;
double target_distance=0;
void init_HUD(int the_object)
{
   sights_targetted=FALSE;
   sights_rotation=2.0;
   sights_radius_min=10;
   sights_length_delta_per_second=8;
   old_x=old_y=old_z=0;
   hud_run_time=0;
   hud_calc_velocity=0;
   hud_fuel=0;
   hud_initting=1;
   targetting_in_progress_flag=0;
   target_distance=0;
   fast_flash=0;
   fast_flash_time=0;
   zex_rel_velocity=0;
   calc_target_name(the_object);
   init_mfd_pane(); //hud 2
}

int hud_drawn=TRUE;

extern int gHideTheScreen;

// ==========================================
void run_hud(int the_object)
{
if (gHideTheScreen) return;
if(!hud_drawn) return;
if (camera_get_view()!=1) return;


    glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
    
   fast_flash_time+=time_in_secs;
   if (fast_flash_time>0.07 /* secs */ )	
   {
     fast_flash_time=0;
     fast_flash^=1;
   }

pull_up_displayed=0;
//check for pull up command
if ((ship_buffet>10000) && (get_main_camera_object()==the_object))
  {
   pull_up_displayed=1;
  }

//run normal hud
   hud_run_time+=time_in_secs;
   if (hud_run_time>0.3 /* secs */ )	
   {
     hud_run_time=0;
     do_hud_calcs(the_object);
     slow_flash^=1;
   }
   
   
   draw_velocity_vector(the_object);
   draw_sights(the_object);
   draw_velocity(the_object);
   draw_insts(the_object);
   draw_time_stuff(the_object);
   process_and_draw_ancillary_info();  

 if(pull_up_displayed)    do_pull_up(the_object);
    glDisable (GL_BLEND);
}

#define CAN_COLOUR 251

double old_distance_to_target;

void do_pull_up(int the_object)
{
if(gHideTheScreen==1) return;
//draw a big white flashing cross on the screen
 if ((is_dc_good(the_object)==1) && fast_flash==1)
     {
	    draw_line_no_clip(screen_cent_x-40,screen_cent_y-40,screen_cent_x+40,screen_cent_y+40,SBY_SIGHT_COLOUR);
	    draw_line_no_clip(screen_cent_x+40,screen_cent_y-40,screen_cent_x-40,screen_cent_y+40,SBY_SIGHT_COLOUR);
	    SplatText_Small_Transparent("ALTITUDE",screen_cent_x-19 , screen_cent_y+30, SBY_SIGHT_COLOUR);
      }

}



#define MT_X screen_cent_x-73
static char tod_string[]="    00:00:00  ";
static char eta_string[]="ETA 00:00  ";
static char vel_string[]="Vel";

void draw_time_stuff(int the_object)	//time of day clock
{
if(gHideTheScreen==1) return;

 if ((is_ac_good(the_object)==1))
 {

      SplatText_Small_rgb(tod_string,screen_cent_x-77 , monitor_h-110, misc_text_red, misc_text_green, misc_text_blue);
      
      
      if (get_object_targetted(the_object)!=-1 && targetting_in_progress_flag==0) 
      {
      //velocity display in scanner
       SplatText_Small_rgb(vel_string,screen_cent_x+32 , monitor_h-110, misc_text_red, misc_text_green, misc_text_blue);
       print_distance_in_metres_3_digit_just(screen_cent_x+58 , monitor_h-110, hud_calc_velocity,misc_text_red, misc_text_green, misc_text_blue,0);

      }
      
      
      if((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.command_ORCS_damper==ORCS_DAMPER_ROT+ORCS_DAMPER_LIN)	//ld on
      {
       SplatText_Small_rgb("DP",MT_X , monitor_h-120, misc_text_red, misc_text_green, misc_text_blue);
      }
      else
      if((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.command_ORCS_damper==ORCS_DAMPER_LIN)	//ld on
      {
       SplatText_Small_rgb("LN",MT_X , monitor_h-120, misc_text_red, misc_text_green, misc_text_blue);
      }
      else
      if((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.command_ORCS_damper==ORCS_DAMPER_ROT)	//ld on
      {
       SplatText_Small_rgb("RT",MT_X , monitor_h-120, misc_text_red, misc_text_green, misc_text_blue);
      }
      
      
      if((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.command_autopilot_engaged>AP_STEER)
      {
       if(((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.AP_flags&AP_LOCKED)==0)	//ap aligned
       {
        SplatText_Small_rgb("ALIGN",MT_X+20 , monitor_h-120, misc_text_red, misc_text_green, misc_text_blue);
       }	
       else
       {
        SplatText_Small_rgb("LOCK",MT_X+20 , monitor_h-120, misc_text_red, misc_text_green, misc_text_blue);
       }
       
       if((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.ORCS_thrust_up>0.1)	//ap engine command
       {
        SplatText_Small_rgb("THRST",MT_X+55 , monitor_h-120, misc_text_red, misc_text_green, misc_text_blue);
       }
       else
       if((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.ORCS_thrust_down>0.1)	//ap engine command
       {
        SplatText_Small_rgb("REV",MT_X+55 , monitor_h-120, misc_text_red, misc_text_green, misc_text_blue);
       }

       else
       {
        SplatText_Small_rgb("COAST",MT_X+55 , monitor_h-120, misc_text_red, misc_text_green, misc_text_blue);
       }
     	
	
      }
  }
}




static int last_cons=0;
static float check_cons_time;

void draw_insts(int the_object)
{
int laser_bay_temperature;
float main_thrust_consumption,temp_fuel_consumption;
ZObject temp_obj;
float max_fuel;

	if (hud_initting==1 || hud_initting==2)
	{
	  if (hud_fuel<12) hud_fuel+=0.1;
	  else
          {
	  if (hud_fuel<20) 
           {
           hud_fuel+=0.1;
	   hud_initting=2;
	   //add_to_text_display("WEAPONS SYSTEMS ON-LINE",2);
	   }
	   else hud_initting=0;
	  }
	}
	else
	{
	//calculate fuel in range 0-20
 	 max_fuel=(*ocb_ptr).object_list[0].Dyn_OCB_control_data.bk_fuel_capacity_kg;
	 hud_fuel=(*ocb_ptr).object_list[0].Dyn_OCB_control_data.fuel_tank_value_in_KGS/max_fuel*20;
	}
	//fuel+=1;

if(gHideTheScreen==1) return;
    
	temp_obj=(*ocb_ptr).object_list[0];
	
	draw_bar_multi(STATIC_CAST_TO_INT(hud_fuel),screen_cent_x-72,monitor_h-97,1);	//fuel guage
//get fuel consumption in range 0-20
//calc max possible consumption
        main_thrust_consumption=(*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.main_thrust_consumption_per_second_in_KGS;
        if (is_engine2_fitted(0))
        main_thrust_consumption*=2;
//calc actual in range 0-20

        temp_fuel_consumption=(total_fuel_consumption/main_thrust_consumption)*20;
        
        check_cons_time+=time_in_secs;
        if (check_cons_time>0.1 /* secs */ )	
        {
         check_cons_time=0;
	 if (temp_fuel_consumption>last_cons)
	 last_cons++;
	 
	 if (temp_fuel_consumption<last_cons)
	 last_cons--;
        }
	if (last_cons>18 && fast_flash==1)
	{
         //flash if > 15
        }
        else
	draw_bar_multi(last_cons,screen_cent_x-92,monitor_h-97,2);	//fuel consumption
        
	draw_bar_multi((*ocb_ptr).object_list[0].Dyn_OCB_control_data.shield_value/5,screen_cent_x+74,monitor_h-97,3);	//shields

        laser_bay_temperature=STATIC_CAST_TO_INT(get_laser_bay_temperature_in_C(get_main_camera_object()));
	laser_bay_temperature=laser_bay_temperature/(MAX_LASER_TEMP/20);	//max temperature of 5000 by 20 bars
	
	if(laser_bay_temperature>20) laser_bay_temperature=20;
	
	draw_bar_multi(laser_bay_temperature,screen_cent_x+94,monitor_h-97,4);	//laser temp
 	
	
}


static char left_bar_data[] = 
	{
	9,32,
	8,27,
	7,23,
	6,19,
	5,16,
	4,14,
	3,12,
	2,11,
	1,10,
	0,9,
	-1,9,
	-2,9,
	-3,9,
	-4,10,
	-5,12,
	-6,13,
	-7,15,
	-8,18,
	-9,20,
	-10,24,
	};

static char left1_bar_data[] = 
	{
	18,19,
	16,18,
	14,17,
	12,16,
	10,15,
	8,14,
	6,13,
	4,12,
	2,11,
	0,10,
	-2,9,
	-4,8,
	-6,7,
	-8,6,
	-10,5,
	-12,4,
	-14,3,
	-16,2,
	-18,1,
	-20,0,
	};

static char right_bar_data[] = 
	{
	-32,-9,
	-27,-8,
	-23,-7,
	-19,-6,
	-16,-5,
	-14,-4,
	-12,-3,
	-11,-2,
	-10,-1,
	-9,0,
	-9,1,
	-9,2,
	-9,3,
	-10,4,
	-12,5,
	-13,6,
	-15,7,
	-18,8,
	-20,9,
	-24,10,
	};

static char right1_bar_data[] = 
	{
	-19,-18,
	-18,-16,
	-17,-14,
	-16,-12,
	-15,-10,
	-14,-8,
	-13,-6,
	-12,-4,
	-11,-2,
	-10,0,
	-9,2,
	-8,4,
	-7,6,
	-6,8,
	-5,10,
	-4,12,
	-3,14,
	-2,16,
	-1,18,
	0,20,
	};

void draw_bar_multi(int value,int x, int y, int draw_code /*1=left circ , 2=left lin, 3=right circ, 4=right lin */ )
{
//fsd 20
//res 1
int n;
unsigned char colour;
//int red,green,blue;
float temp_r,temp_b,temp_g;
float temp_r1,temp_b1,temp_g1;
float temp_r3,temp_b3,temp_g3;
float temp_r4,temp_b4,temp_g4;



//if (value<=4) colour=SIGHT_COLOUR-10;	//red
//else 
//if (value<=12) colour=SIGHT_COLOUR+7;	//blue
//else colour=SIGHT_COLOUR;

colour=SIGHT_COLOUR;

//     convert_to_rgb(colour, &red, &green, &blue);
//     temp_r=red;
//     temp_g=green;
//     temp_b=blue;
//     temp_r=temp_r/255;	//0-1
//     temp_g=temp_g/255;	//0-1
//     temp_b=temp_b/255;	//0-1
     temp_r=sight_colour_red;
     temp_g=sight_colour_green;
     temp_b=sight_colour_blue;

     temp_r1=sight_colour_red-0.1;
     temp_g1=sight_colour_green;
     temp_b1=sight_colour_blue;

     temp_r3=sight_colour_red-0.4;
     temp_g3=sight_colour_green;
     temp_b3=sight_colour_blue;

     temp_r4=sight_colour_red-0.5;
     temp_g4=sight_colour_green;
     temp_b4=sight_colour_blue;
     
//     convert_to_rgb(colour+1, &red, &green, &blue);
//     temp_r1=red;
//     temp_g1=green;
//     temp_b1=blue;
//     temp_r1=temp_r1/255;	//0-1
//     temp_g1=temp_g1/255;	//0-1
//     temp_b1=temp_b1/255;	//0-1
//
//     convert_to_rgb(colour+3, &red, &green, &blue);
//     temp_r3=red;
//     temp_g3=green;
//     temp_b3=blue;
//     temp_r3=temp_r3/255;	//0-1
//     temp_g3=temp_g3/255;	//0-1
//     temp_b3=temp_b3/255;	//0-1
//
//     convert_to_rgb(colour+4, &red, &green, &blue);
//     temp_r4=red;
//     temp_g4=green;
//     temp_b4=blue;
//     temp_r4=temp_r4/255;	//0-1
//     temp_g4=temp_g4/255;	//0-1
//     temp_b4=temp_b4/255;	//0-1

if (value<0) value=0;

if (value>20) value=20;
value=20-value;


if (draw_code==1)
{
glBegin(GL_LINES);

for (n=0;n<value;n++)
 {
	   OGL_line_rgb_quick(x+left_bar_data[n*2]+1,-1+y+n*4,x+left_bar_data[n*2+1]-1,-1+y+n*4,temp_r4,temp_g4,temp_b4);
	   OGL_line_rgb_quick(x+left_bar_data[n*2],y+n*4,x+left_bar_data[n*2+1],y+n*4,temp_r3,temp_g3,temp_b3);
	   OGL_line_rgb_quick(x+left_bar_data[n*2]+1,1+y+n*4,x+left_bar_data[n*2+1]-1,1+y+n*4,temp_r4,temp_g4,temp_b4);
 }

for (n=value;n<20;n++)
 {
	   OGL_line_rgb_quick(x+left_bar_data[n*2]+1,-1+y+n*4,x+left_bar_data[n*2+1]-1,-1+y+n*4,temp_r1,temp_g1,temp_b1);
	   OGL_line_rgb_quick(x+left_bar_data[n*2],y+n*4,x+left_bar_data[n*2+1],y+n*4,temp_r,temp_g,temp_b);
	   OGL_line_rgb_quick(x+left_bar_data[n*2]+1,1+y+n*4,x+left_bar_data[n*2+1]-1,1+y+n*4,temp_r1,temp_g1,temp_b1);
 	


 }
glEnd();
}
else
if (draw_code==2)	//left linear
{
glBegin(GL_LINES);
for (n=0;n<value;n++)
 {

	   OGL_line_rgb_quick(x+left1_bar_data[n*2]+1,-1+y+n*4,x+left1_bar_data[n*2+1]-1,-1+y+n*4,temp_r4,temp_g4,temp_b4);
	   OGL_line_rgb_quick(x+left1_bar_data[n*2],y+n*4,x+left1_bar_data[n*2+1],y+n*4,temp_r3,temp_g3,temp_b3);
	   OGL_line_rgb_quick(x+left1_bar_data[n*2]+1,1+y+n*4,x+left1_bar_data[n*2+1]-1,1+y+n*4,temp_r4,temp_g4,temp_b4);
 	
 }
for (n=value;n<20;n++)
 {
	   OGL_line_rgb_quick(x+left1_bar_data[n*2]+1,-1+y+n*4,x+left1_bar_data[n*2+1]-1,-1+y+n*4,temp_r1,temp_g1,temp_b1);
	   OGL_line_rgb_quick(x+left1_bar_data[n*2],y+n*4,x+left1_bar_data[n*2+1],y+n*4,temp_r,temp_g,temp_b);
	   OGL_line_rgb_quick(x+left1_bar_data[n*2]+1,1+y+n*4,x+left1_bar_data[n*2+1]-1,1+y+n*4,temp_r1,temp_g1,temp_b1);

 

//	     draw_line(x+left1_bar_data[n*2]+1,-1+y+n*4,x+left1_bar_data[n*2+1]-1,-1+y+n*4,colour+1);
//	     draw_line(x+left1_bar_data[n*2],y+n*4,x+left1_bar_data[n*2+1],y+n*4,colour);
//	     draw_line(x+left1_bar_data[n*2]+1,1+y+n*4,x+left1_bar_data[n*2+1]-1,1+y+n*4,colour+1);
// 	
 
 }
glEnd();
}
else
if (draw_code==3)	//right
{

glBegin(GL_LINES);
for (n=0;n<value;n++)
 {
	   OGL_line_rgb_quick(x+right_bar_data[n*2]+1,-1+y+n*4,x+right_bar_data[n*2+1]-1,-1+y+n*4,temp_r4,temp_g4,temp_b4);
	   OGL_line_rgb_quick(x+right_bar_data[n*2],y+n*4,x+right_bar_data[n*2+1],y+n*4,temp_r3,temp_g3,temp_b3);
	   OGL_line_rgb_quick(x+right_bar_data[n*2]+1,1+y+n*4,x+right_bar_data[n*2+1]-1,1+y+n*4,temp_r4,temp_g4,temp_b4);
 	
 }

for (n=value;n<20;n++)
 {
	   OGL_line_rgb_quick(x+right_bar_data[n*2]+1,-1+y+n*4,x+right_bar_data[n*2+1]-1,-1+y+n*4,temp_r1,temp_g1,temp_b1);
	   OGL_line_rgb_quick(x+right_bar_data[n*2],y+n*4,x+right_bar_data[n*2+1],y+n*4,temp_r,temp_g,temp_b);
	   OGL_line_rgb_quick(x+right_bar_data[n*2]+1,1+y+n*4,x+right_bar_data[n*2+1]-1,1+y+n*4,temp_r1,temp_g1,temp_b1);
 
 }
glEnd();

}
else
if (draw_code==4)	//right lin
{
glBegin(GL_LINES);
for (n=0;n<value;n++)
 {
	   OGL_line_rgb_quick(x+right1_bar_data[n*2]+1,-1+y+n*4,x+right1_bar_data[n*2+1]-1,-1+y+n*4,temp_r4,temp_g4,temp_b4);
	   OGL_line_rgb_quick(x+right1_bar_data[n*2],y+n*4,x+right1_bar_data[n*2+1],y+n*4,temp_r3,temp_g3,temp_b3);
	   OGL_line_rgb_quick(x+right1_bar_data[n*2]+1,1+y+n*4,x+right1_bar_data[n*2+1]-1,1+y+n*4,temp_r4,temp_g4,temp_b4);
 	
 }

for (n=value;n<20;n++)
 {
	   OGL_line_rgb_quick(x+right1_bar_data[n*2]+1,-1+y+n*4,x+right1_bar_data[n*2+1]-1,-1+y+n*4,temp_r1,temp_g1,temp_b1);
	   OGL_line_rgb_quick(x+right1_bar_data[n*2],y+n*4,x+right1_bar_data[n*2+1],y+n*4,temp_r,temp_g,temp_b);
	   OGL_line_rgb_quick(x+right1_bar_data[n*2]+1,1+y+n*4,x+right1_bar_data[n*2+1]-1,1+y+n*4,temp_r1,temp_g1,temp_b1);
 
 }
glEnd();

}


}




void do_hud_calcs(int the_object)
{
//float dist_x,dist_y,dist_z;
static double scalar;
//float hud_calc_time;
//vector zex_vv,obj_vv, res_vv, heading_vector,cross_vector,res1_vv;
int hours,mins,secs;

if(gHideTheScreen==1) return;

	//tod - if HT we freeze it
        if ((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.ap_ht_engage_timer<0.1 &&
        (*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.command_autopilot_engaged==AP_HYPERTRANSPORT)
        {
        }
        else
        {
	scalar=get_TOD_hours();
        hours=STATIC_CAST_TO_INT(scalar);
        num_to_string99(STATIC_CAST_TO_INT(scalar),&tod_string[4]);
        tod_string[6]=':';   
        
	scalar=get_TOD_mins();
        mins=STATIC_CAST_TO_INT(scalar);
        num_to_string99(STATIC_CAST_TO_INT(scalar),&tod_string[7]);
        tod_string[9]=':';   
	scalar=get_TOD_secs();
        secs=STATIC_CAST_TO_INT(scalar);
        num_to_string99(STATIC_CAST_TO_INT(scalar),&tod_string[10]);
        }
		

		
                //ETA & TOT
        //get distance to target
        //scalar=target_distance; 
//        if( (*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.command_autopilot_engaged>=AP_TRANSPORT)
//        {
//        scalar=(*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.calculated_time_to_target_in_secs;
//        eta_string[0]='C'; //calculated time of arrival
//        }
//        else 
//        {
        scalar=(*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.time_to_target_in_secs_as_crow_flies;
//        eta_string[0]='E';	//estimated time of arrival
//        }

        if ((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.ap_ht_engage_timer<0.1 &&
        (*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.command_autopilot_engaged==AP_HYPERTRANSPORT)
        {
             scalar=NDRangedRdm(1,3600);
        }

        if (scalar<1 )
        {
          eta_string[4]='5';
          eta_string[5]='9';
          eta_string[7]='5';
          eta_string[8]='9';

        
        }
        else	//we have the distance in metres
        {
          int temp;
          //divide by velocity to get time to target in seconds
//          scalar/=hud_calc_velocity;
          temp=STATIC_CAST_TO_INT(scalar/60);
          mins+=temp;
          if (temp<=59)
          {
  
            num_to_string99(temp,&eta_string[4]);
	    
            eta_string[6]=':';   
            scalar=scalar-(temp*60);
            num_to_string99(STATIC_CAST_TO_INT(scalar),&eta_string[7]);

            if(mins>59) 
             {
               mins-=60;
               hours++;
               if(hours>23) hours=0;
             }            

             
          }
          else	//> 59 mins away
          {
           eta_string[4]='5';
           eta_string[5]='9';
           eta_string[7]='5';
           eta_string[8]='9';

          }
                    
        }
}

//draws a small marker to indicate current direction.
//if in front of us then we draw a small diamond, if behind it's a square
_3D vd;
void draw_velocity_vector(int the_object)
{
ZObject *source_object;
_3D *world_points_ptr;
_2Dp vv_coord;
unsigned char colour;
int red,green,blue;
float temp_r1,temp_b1,temp_g1;

if(gHideTheScreen==1) return;
 
 
 if ((is_ac_good(the_object)==1) && (pull_up_displayed==0))
 {

//This is a spherical projection from the difference between the normalised velocity vector (vv) and the heading vector (hv)
//The radius is 1 (normalised)

//get the first point of dyn object 1
	source_object=&(*ocb_ptr).object_list[1];	//vv object
	
	world_points_ptr=(*source_object).world_points_ptr;
	vd=*world_points_ptr;
//perspective transform the first point

           vv_coord=point_to_screen_with_bflag(vd.x,vd.y,vd.z,monitor_w,monitor_h);
           vv_coord.x-=2;
           vv_coord.y-=2;
 	   
 	   glBegin(GL_LINES);



 	   if (vv_coord.behind==0)
 	   {

	   OGL_line_rgb_quick(vv_coord.x+2,vv_coord.y,vv_coord.x+2,vv_coord.y+4,fwd_vv_r,fwd_vv_g,fwd_vv_b);
	   OGL_line_rgb_quick(vv_coord.x,vv_coord.y,vv_coord.x,vv_coord.y+4,fwd_vv_r,fwd_vv_g,fwd_vv_b);
 	   }
 	   else 
 	   {
 	   colour = SBY_SIGHT_COLOUR;
     convert_to_rgb(colour, &red, &green, &blue);
     temp_r1=red;
     temp_g1=green;
     temp_b1=blue;
     temp_r1=temp_r1/255;	//0-1
     temp_g1=temp_g1/255;	//0-1
     temp_b1=temp_b1/255;	//0-1	        
           OGL_line_rgb_quick(vv_coord.x-3,vv_coord.y,vv_coord.x+4,vv_coord.y,temp_r1,temp_g1,temp_b1);
	   OGL_line_rgb_quick(vv_coord.x,vv_coord.y-3,vv_coord.x,vv_coord.y+4,temp_r1,temp_g1,temp_b1);

	   }
	   glEnd();


 }	//end of ac power

}

unsigned char auto_colour;
float auto_flash_time=0;


void draw_velocity(int the_object)
{
//float scalar, this_distance;
//double dist_x,dist_y,dist_z;
//float temp;
//vector vv_diff,vtt,our_vv, vh_ratio;
float flash_time=0.5;

 if ((is_ac_good(the_object)==1))
 {
        if ((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.ap_ht_engage_timer<0.1 &&
        (*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.command_autopilot_engaged==AP_HYPERTRANSPORT)
            {
             hud_calc_velocity=NDRangedRdm(0,50000)*NDRangedRdm(1,50000);
        }
        else
	if (get_object_targetted(0)!=-1)	//handled in do_target_vel
	{	
		hud_calc_velocity=(*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.current_relative_velocity; //from flight;
                
                if(gHideTheScreen==0) 
                SplatText_Small_rgb(target_name_str,screen_cent_x-((target_name_len*6)/2) , screen_cent_y+50, misc_text_red, misc_text_green, misc_text_blue);

	}
	else
        {
                hud_calc_velocity=(*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.current_abs_velocity;	//from flight
        }	


        if(gHideTheScreen==1) return;

	if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_HYPERTRANSPORT)	//cruise - auto thrust
	{
          if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.ap_ht_engage_timer<0.1)
          {
           flash_time=0.25;
          }
          else
          {
           flash_time=1.0;
          }
        } 
        
        auto_flash_time+=real_frame_time_in_secs;
        if (auto_flash_time>flash_time)	
        {
            auto_flash_time=0;
            if (auto_colour==230) auto_colour=250;
            else auto_colour=230;
        }

	

	if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_STEER_CUE)	//manual thrust
	{
     
	  if (auto_colour==230 )
	  SplatText_Small_rgb("CUE",screen_cent_x-9 , screen_cent_y-40, misc_text_red, misc_text_green, misc_text_blue);

        }
        else
	if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_STEER)	//cruise - auto thrust
	{
     
	  if (auto_colour==230 )
	  SplatText_Small_rgb("STEER",screen_cent_x-14 , screen_cent_y-40, misc_text_red, misc_text_green, misc_text_blue);

        }
        else
	if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_TRANSPORT)	//cruise - auto thrust
	{
     
	  if (auto_colour==230 )
	  SplatText_Small_rgb("TRANSPORT",screen_cent_x-21 , screen_cent_y-40, misc_text_red, misc_text_green, misc_text_blue);

        }
        else
	if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_HYPERTRANSPORT)	//cruise - auto thrust
	{
     
	  if (auto_colour==230 )
	  SplatText_Small_rgb("HYPERTRANSPORT",screen_cent_x-41 , screen_cent_y-40, misc_text_red, misc_text_green, misc_text_blue);

        }
        else
	if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_COMBAT)	//combat
	{
     
	  if (auto_colour==230 )
	  SplatText_Small_rgb("COMBAT",screen_cent_x-13 , screen_cent_y-40, misc_text_red, misc_text_green, misc_text_blue);

        }

        else

        if (hud_initting==1)
        {	
	  if (auto_colour==230 )
        	SplatText_Small_rgb("TEST",screen_cent_x-9 , screen_cent_y-40, misc_text_red, misc_text_green, misc_text_blue);
        } 
        else

        if (hud_initting==2)
        {	
	  if (auto_colour==230 )
        	SplatText_Small_rgb("TEST PASS",screen_cent_x-20 , screen_cent_y-40, misc_text_red, misc_text_green, misc_text_blue);
        } 

 }
 	 
}




//given a distance it does the following
//if less than 1000 prints xxxM (1)
//if less than 10000 prints x.xxK (2)
//if less than 100000 prints xx.xK (3)

//if less than 1000000 print xxxK

//if less than 10000000 prints x.xxKK
//if less than 100000000 prints xx.xKK
//if less than 1000000000 prints xxxKK
//else prints FAR
//just=0=left, 1=center


void print_distance_in_metres_3_digit_just(int x, int y, float value, float red, float green, float blue, int just)
{
char number_string[12];
int strlen;


if (gHideTheScreen==1) return;

strlen = make_3_digit(value,number_string);


if (just==0)	//left just  
     SplatText_Small_rgb((char*)number_string,x , y, red, green, blue);
else	        //center just
     {
     strlen=strlen/2*5;
     SplatText_Small_rgb((char*)number_string,x-strlen , y, red, green, blue);   
     }
}


#define AU_in_m 149597870691.0
#define LY_in_m 9460528404846000.0
#define PA_in_m (3.262 * LY_in_m)


// correct metric capitalisation taken from http://lamar.colostate.edu/~hillger/correct.htm
int make_3_digit(double value, char *number_string)
{
unsigned char units_char[2];
int string_pos;
int mode,temp,int_value;

string_pos=0;


if (value<0) 
{
           number_string[string_pos]='-';
           string_pos++;
           value=-value;
}

//first find out units char

if(value<1e3) 			// under 1 kilometer print in m
{
 units_char[0]='m';
 units_char[1]=0;
 int_value=STATIC_CAST_TO_INT(100 * value);
}
else if(value<1e6)		// under 1 thousand kilometers print in km 
{ 
 units_char[0]='k';
 units_char[1]='m';
 int_value=STATIC_CAST_TO_INT(100 * (value/1e3));
}
else if(value<1e9) 	// under 1 million kilometers print in thousand km
{ 
 units_char[0]='k';
 units_char[1]='k';
 int_value=STATIC_CAST_TO_INT(100 * (value/1e6));
}
else if(value < (0.14 * AU_in_m)) // under 0.14 AU (that 21 million kilometers to you and 
{				//  me) print in million kilometers
 units_char[0]='M';
 units_char[1]='k';
 int_value=STATIC_CAST_TO_INT(100 * (value / 1e9));
}
else if(value < (1e3 * AU_in_m)) // if from 0.14 to 999 AU print in Astronomical units
{
 units_char[0]='A';
 units_char[1]='U';
 int_value=STATIC_CAST_TO_INT(100 * (value / AU_in_m));	// make in AU
}
else if(value < (15e3 * AU_in_m)) // if between 1000AU and 15K AU then print in K.AU
{
 units_char[0]='k';
 units_char[1]='A';
 int_value=STATIC_CAST_TO_INT(100 * (value / (1e3 * AU_in_m)));
}
else if(value < (1e3 * LY_in_m))	// if between 0.25 ly and 999 ly then print in ly
{
 units_char[0]='l';
 units_char[1]='y';
 int_value=STATIC_CAST_TO_INT(100 * ( value / ( LY_in_m)));
}
else if(value < (1e3 * PA_in_m))	// if between 1000ly and 999 Parsecs, print in parsecs
{
 units_char[0]='P';
 units_char[1]='A';
 int_value=STATIC_CAST_TO_INT(100 * ( value / ( PA_in_m)));
}
else if(value< (1e6 * PA_in_m))		// if between 1000PA and 999KiloParsecs print in kP 
{
 units_char[0]='k';
 units_char[1]='P';
 int_value=STATIC_CAST_TO_INT(100 * ( value / (1e3 * PA_in_m)));
}
else	//>999GK .... was 999Mkm
{
        number_string[0]='-';
        number_string[1]='-';
        number_string[2]='-';
        number_string[3]=0;
        return 3;
}

/*

3.26 ly = 1 PA

1000PA = 1KP

------------------------------------------------------------
Is the c, speed of light in vacuum, constant?

                                      At the 1983 Conference Generale des Poids et Mesures the
                                      following SI (Systeme International) definition of the meter was
                                      adopted:

                                             The meter is the length of the path travelled by
                                             light in vacuum during a time interval of 1/299 792
                                             458 of a second. 

                                      This defines the speed of light in vacuum to be exactly 299,792,458
                                      m/s. This provides a very short answer to the question "Is c constant":
                                      Yes, c is constant by definition!
------------------------------------------------------
                          1 AU = 149,597,870.691 kilometers 

                          Definition: An Astronomical Unit is the mean distance between the Earth and the Sun. It is
                          a derived constant and used to indicate distances within a solar system. The Earth orbits at a
                          distance of 1 AU from the Sun. 

                          Technical Definition: AU is short for Astronomical Unit and defined as: the radius of a
                          Keplerian circular orbit of a point-mass having an orbital period of 2*(pi)/k days (k is the
                          Gaussian gravitational constant). Since an AU is based on radius of a circular orbit, one AU
                          is actually slightly less than the average distance between the Earth and the Sun
                          (approximately 150 million km or 93 million miles). 
                          
                                                      Light Year Conversion Table 
                                                      Kilometres 
                                                                         9,460,528,404,846 
                                                      Miles 
                                                                         5,878,499,812,499 
                                                      Astronomical Units 
                                                                                 63,240 
                                                      Parsecs 
                                                                                 0.3066 

AU
ly
Pa

*/                        
                          
// isolate where the decimal point is AND scale to be between 100 and 999
if (int_value>=10000 && int_value<100000)  //eg 999m, 123K, 123KK, 123KK, 123MKm
  {
  int_value/=100;
  mode=1;	//xxx
  }
else if (int_value>=1000 && int_value<10000)	  	  // e.g 10.0, 99.9
  {
  mode=3; int_value/=10; //xx.x
  }
else // if (int_value>= 100   && int_value < 1000) //e.g. 1.23K, 1.23M, 1.23Gkm
  {
  mode=2;      //x.xx
  }

	  temp=int_value/100;
	  ZNumToString1(temp, &number_string[string_pos]);
          string_pos++;
          if (mode==2)
          {
           number_string[string_pos]='.';
           string_pos++;
          }
          
  	  int_value=int_value-(temp*100);	//294
          temp=int_value/10;
	  ZNumToString1(temp, &number_string[string_pos]);
          string_pos++;

          if (mode==3)
          {
           number_string[string_pos]='.';
           string_pos++;
	  }
	  
  	  int_value=int_value-(temp*10);
	  temp=int_value;
	  ZNumToString1(temp, &number_string[string_pos]);
          string_pos++;
	  
	  number_string[string_pos]=units_char[0];
          string_pos++;

          if(units_char[1]!=0)
            {
	    number_string[string_pos]=units_char[1];
            string_pos++;
            }

          number_string[string_pos]=0;
          return string_pos;	// length
}



//called from ship_control when T key pressed
void do_target(int the_object)
{
//scan visible objects and check vectors to; if we get one within a certain range we select it
//as the target
int got_target=0;
int active_object_counter,object_counter;
extern int dynamic_object_index;
register ZObject * current_object_ptr;


if ((is_ac_good(the_object)==1))
{ 
active_object_counter=object_counter=0;

while (active_object_counter<dynamic_object_index)
 {
    current_object_ptr=&ocb_ptr->object_list[object_counter];
    if (current_object_ptr->in_use==1 ) //if object is alive
    {
    
     if (current_object_ptr->is_visible==1 ) //if object is visible
      {
        if (is_it_a_target(object_counter)==1) 
        {
         if (object_counter!=old_target)
         {
         got_target=object_counter;
         old_target=object_counter;
         break;
         }
        }
      }
     active_object_counter++;
     }
  object_counter++;
 }
 
if (got_target==0) 
{
 play_zsound_always(select_unknown, sound_high_pri, c1_chan1, sound_vol_7);
 old_target=0; target_distance=0;

 set_object_targetted(0,-1);
 if(get_object_targetted(0)!=-1)
 ocb_ptr->object_list[get_object_targetted(0)].targetted=0;	//clear out old targetted
 
 (*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged=AP_OFF;	//disengage autopilot
}
else 
{
sights_rotation=46;
play_zsound_always(select_planet, sound_high_pri, c1_chan1, sound_vol_7);
add_to_text_display("HUD> Targetting...",DLP_GREEN);
targetting_in_progress_flag=1;

if(get_object_targetted(0)!=-1)
ocb_ptr->object_list[get_object_targetted(0)].targetted=0;	//clear out old targetted


 ocb_ptr->object_list[object_counter].targetted=1;
 set_object_targetted(0,got_target);

}

calc_target_name(0);

}	//end of ac power
}



static int radar_target_slot=0; 
extern radar_entry radar_list[];
extern int radar_points_counter;
void do_radar_target(void)
{
int got_target;
int debug_target_slot, debug_radar_points_counter;

radar_target_slot++; 
debug_target_slot=radar_target_slot;
debug_radar_points_counter=radar_points_counter;

if (radar_points_counter==0) return;

if (radar_target_slot>=radar_points_counter) radar_target_slot=0;

debug_target_slot=radar_target_slot;

got_target= radar_list[radar_target_slot].object_slot;

 sights_rotation=46;
 play_zsound_always(select_planet, sound_high_pri, c1_chan1, sound_vol_7);
 add_to_text_display("HUD> Radar Targetting...",DLP_GREEN);
 targetting_in_progress_flag=1;

 if(get_object_targetted(0)!=-1)
 ocb_ptr->object_list[get_object_targetted(0)].targetted=0;	//clear out old targetted
 ocb_ptr->object_list[got_target].targetted=1;
 set_object_targetted(0,got_target);
 calc_target_name(0);

 }
 


void set_targetted_for_object(int object, int targetted)
{
 //clear out old targetted for object
 if(get_object_targetted(object)!=-1)
 ocb_ptr->object_list[get_object_targetted(object)].targetted=0;	//clear out old targetted

 //set targetted
 set_object_targetted(object,targetted);
 sights_rotation=46;
 play_zsound_always(select_planet, sound_high_pri, c1_chan1, sound_vol_7);
 add_to_text_display("HUD> Targetting...",DLP_GREEN);
 targetting_in_progress_flag=1;
 calc_target_name(object);
 
}

//tis routine shoves the target name into target_name_str
//and the length of the string in target_name_len
void calc_target_name(int the_object)
{
int object_targetted;
int object_controller;
//Str255 temp_p_str;

	object_targetted=get_object_targetted(the_object);
	
	if (object_targetted==-1)
        {
          copystr (target_name_str+TARGET_STRING_OFFSET_NO_CALLSIGN,"NONE");
          target_name_len=strlen(target_name_str);
        }
        else
        {
		  object_controller=get_object_controller(object_targetted);
        
		//is it a planet?
          if(PLANET_CONTROL==object_controller
             || SUN==object_controller
			 || FAR_STAR==object_controller)
            {
            	//get name from object - stored as pascal string
 	         PascalToC ((*ocb_ptr).object_list[object_targetted].Dyn_OCB_control_data.name_str,target_name_str+TARGET_STRING_OFFSET_NO_CALLSIGN);
	        
            }
            else	//mothership
			if(MOTHERSHIP_CONTROL==object_controller)
			{
			  if (get_object_category(object_targetted)==HUMAN)
			  {
               copystr (target_name_str+TARGET_STRING_OFFSET_NO_CALLSIGN,"Human Mothership Docking Bay");
			  }
			  else if (get_object_category(object_targetted)==ALIEN)
			  {
               copystr (target_name_str+TARGET_STRING_OFFSET_NO_CALLSIGN,"R'mparian Mothership Docking Bay");
			  }
			  else
			  {
               copystr (target_name_str+TARGET_STRING_OFFSET_NO_CALLSIGN,"Alycian Mothership Docking Bay");
			  }
			  
			  
			}
            else	//mothership
			if( MOTHERSHIP_PART==object_controller)
			{
			  if (get_object_category(object_targetted)==HUMAN)
			  {
               copystr (target_name_str+TARGET_STRING_OFFSET_NO_CALLSIGN,"Human Mothership");
			  }
			  else if (get_object_category(object_targetted)==ALIEN)
			  {
               copystr (target_name_str+TARGET_STRING_OFFSET_NO_CALLSIGN,"R'mparian Mothership");
			  }
			  else
			  {
               copystr (target_name_str+TARGET_STRING_OFFSET_NO_CALLSIGN,"Alycian Mothership");
			  }
			  
			}
            else	//Falcon
			if((get_object_Oid(object_targetted)) == 'FALC')
			{
			
			//insert callsign so it looks like Target: H1234 Human Falcon
			 copystr (target_name_str+TARGET_STRING_OFFSET_NO_CALLSIGN,(*ocb_ptr).object_list[object_targetted].Dyn_OCB_control_data.callsign); 
			  if (get_object_category(object_targetted)==HUMAN)
			  {
              copystr (target_name_str+TARGET_STRING_OFFSET," Earth Falcon");
			  }
			  else if (get_object_category(object_targetted)==ALIEN) 
			  {
              copystr (target_name_str+TARGET_STRING_OFFSET," R'mparian Falcon");
			  }
			  else 
			  {
              copystr (target_name_str+TARGET_STRING_OFFSET," Alycian Falcon");
			  }
			  
			}
            else	//Cargo canister
			if((get_object_Oid(object_targetted)) == 'CBOX')
			{
              copystr (target_name_str+TARGET_STRING_OFFSET_NO_CALLSIGN,"Cargo Container");			  
			}
            else	//shuttle
			if((get_object_Oid(object_targetted)) == 'SHT1')
			{
			//insert callsign so it looks like Target: H1234 Human Falcon
			 copystr (target_name_str+TARGET_STRING_OFFSET_NO_CALLSIGN,(*ocb_ptr).object_list[object_targetted].Dyn_OCB_control_data.callsign); 
			  if (get_object_category(object_targetted)==HUMAN)
			  {
              copystr (target_name_str+TARGET_STRING_OFFSET," Earth Shuttle");
			  }
			  else if (get_object_category(object_targetted)==ALIEN)
			  {
              copystr (target_name_str+TARGET_STRING_OFFSET," R'mparian Shuttle");
			  }
			  else
			  {
              copystr (target_name_str+TARGET_STRING_OFFSET," Alycian Shuttle");
			  }
			  
			}

			else
            {
              copystr (target_name_str+TARGET_STRING_OFFSET_NO_CALLSIGN,"Unknown object");
            }
            
          target_name_len=strlen(target_name_str);
          //target_name_len+=TARGET_STRING_OFFSET;
        }
}  






void check_autopilotup(int object)
{
ZObject *source_object;

source_object=&(*ocb_ptr).object_list[object];

  if (get_object_targetted(0)!=-1)
   {

             fade_out_music();		//kill AP slowdown music
set_time_multiplier_to_1();	//any activation of AP sets time to 1             

//if auto on, switch it off
if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_OFF)	
 {
  add_to_text_display("AP> Steer Cue Mode",DLP_GREEN);

       play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
        engage_ap(0,AP_STEER_CUE);
 }
else

if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_STEER_CUE)	
 {
  add_to_text_display("AP> Steer Mode",DLP_GREEN);
       play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
        engage_ap(0,AP_STEER);
 }
else
if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_STEER)	
 {
  add_to_text_display("AP> Transport Mode",DLP_GREEN);
       play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
        fade_and_play_zex_resource(133);	//technological - cruise music
        engage_ap(0,AP_TRANSPORT);
 }
else
if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_TRANSPORT)	
 {
  add_to_text_display("AP> Combat Mode",DLP_YELLOW);
        play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
        engage_ap(0,AP_COMBAT);
 }
else
if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_COMBAT)	
 {
  add_to_text_display("AP> Hypertransport",DLP_YELLOW);
        play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
        engage_ap(0,AP_HYPERTRANSPORT);
 }

 }	//end of targetted

}



void check_autopilotdown(int object)
{
ZObject *source_object;

source_object=&(*ocb_ptr).object_list[object];

             fade_out_music();		//kill AP slowdown music
set_time_multiplier_to_1();	//any activation of AP sets time to 1             

  if (get_object_targetted(0)!=-1)
   {


//if auto on, switch it off
if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_STEER)	
 {
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
        engage_ap(0,AP_STEER_CUE);
  add_to_text_display("AP> Steer Cue Mode",DLP_GREEN);

 }
else
if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_TRANSPORT)	
 {
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
        engage_ap(0,AP_STEER);
   add_to_text_display("AP> Steer Mode",DLP_GREEN);

 }
else
if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_COMBAT)	
 {
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
        fade_and_play_zex_resource(133);	//technological - cruise music
        engage_ap(0,AP_TRANSPORT);
  add_to_text_display("AP> Transport Mode",DLP_GREEN);
 }
else
if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_HYPERTRANSPORT)	
 {
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
 	(*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged=AP_COMBAT;	//go to combat
        fade_and_play_zex_resource(133);	//technological - cruise music
        engage_ap(0,AP_COMBAT);
  add_to_text_display("AP> Combat Mode",DLP_GREEN);
 }
else
if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_STEER_CUE)	
 {
 play_zsound(sclick, sound_high_pri, c1_chan1, sound_vol_7);
       disengage_AP(0);
  add_to_text_display("AP> Off. Lin and Rot dampers on",DLP_GREEN);

 }

}

}



int is_it_a_target(int object)
{
_3D p1,p2;
int distance_to;
extern _3D ship_rotated_normal; //from engine

     	p1.x=ocb_ptr->object_list[0].Oworldx;
     	p1.y=ocb_ptr->object_list[0].Oworldy;
     	p1.z=ocb_ptr->object_list[0].Oworldz;

     	p2.x=ocb_ptr->object_list[object].Oworldx;
     	p2.y=ocb_ptr->object_list[object].Oworldy;
     	p2.z=ocb_ptr->object_list[object].Oworldz;
     
     	p1.x=p2.x-p1.x; p1.y=p2.y-p1.y; p1.z=p2.z-p1.z;	//get vector to object
        
        distance_to=abs_nobranch(STATIC_CAST_TO_LONG(sqrt(p1.x*p1.x+p1.y*p1.y+p1.z*p1.z)));
     
	 Vnormalise(&p1);	//normalised vector to object
	
	//compare normal to ship_rotated_normal and if         
         p1.x=p1.x-ship_rotated_normal.x;
         p1.y=p1.y-ship_rotated_normal.y;
         p1.z=p1.z-ship_rotated_normal.z;
        
         if (p1.x<0) p1.x=-p1.x;
         if (p1.y<0) p1.y=-p1.y;
         if (p1.z<0) p1.z=-p1.z;
         if (ocb_ptr->object_list[object].Dyn_OCB_control_data.controller==DUST) return 0;
         //if (ocb_ptr->object_list[object].Dyn_OCB_control_data.controller==FAR_STAR) return 0;
         if (ocb_ptr->object_list[object].Dyn_OCB_control_data.controller==VV_IND) return 0;

         if (p1.x<0.03 && p1.y<0.03 && p1.z <0.03 /* && distance_to<gradar_range */ )        return 1;
 	 else return 0;

}

int targ_xdbg,targ_ydbg;
int targ_behind;


void draw_sights(int the_object)
{
int xS,yS,xE,yE;
int this_line_rotation;
int angle_to_2d=0;
_2Dp temp;
double dist;
 int targ_x=0,targ_y=0;
unsigned char arrow_colour;

static int arrow_x1,arrow_x2,arrow_y1,arrow_y2;
//static int arrow_x3,arrow_y3;  


if(gHideTheScreen!=0) return;

//draw sights
 if ((is_ac_good(the_object)==1))
 {
    if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged<AP_TRANSPORT && pull_up_displayed==0)	      //sterring or off then draw capture circle
    fcircle_nofill_noclip(screen_cent_x, screen_cent_y, 40, SIGHT_COLOUR);
  //get targetted objects screen pos
  if (get_object_targetted(0)==-1)
  { 
   targ_x=screen_cent_x; targ_y=screen_cent_y;

  } 
  else
  { 
   if (ocb_ptr->object_list[get_object_targetted(0)].in_use==1)
   {
   int object_targetted;
   float targetted_radius;
   
   object_targetted=get_object_targetted(0);
   
   temp=get_screen_center_coord_with_bflag (object_targetted);	 //lots of processing
   targ_x=temp.x; targ_y=temp.y;
   


   
   targ_xdbg=targ_x; targ_ydbg=targ_y;
   targ_behind=temp.behind;
      
   dist = get_distance_to_from(object_targetted, 0);
   dist*=ZEX_UNIT_SIZE_IN_METERS;	
   
   targetted_radius=get_object_radius_in_metres(object_targetted);
   
   target_distance=dist-(targetted_radius+((float)(ATMOSPHERE_PERCENTAGE+0.04) * targetted_radius));
   
   
    if(get_object_targetted(0)!=-1 && sights_rotation==45)
    print_distance_in_metres_3_digit_just(screen_cent_x+58 , monitor_h-120, dist,misc_text_red, misc_text_green, misc_text_blue,0);
   
    SplatText_Small_rgb("Dist", screen_cent_x+32 , monitor_h-120,misc_text_red, misc_text_green, misc_text_blue);
//   SplatText_Small_Transparent((char*)"DISTANCE ",targ_x , targ_y, SIGHT_COLOUR);
   //calc and show distance to target
   if (sights_rotation==45)	//we are targetted
    {
     //calc 2d angle to target so we can draw a direction arrow on the hud
     angle_to_2d= get_angle_to_from (targ_x, targ_y, screen_cent_x, screen_cent_y);

    if (targ_x>monitor_w-10 || targ_x<10 || targ_y>monitor_h-20 || targ_y<20 || temp.behind==1) 
     {	//lost lock
//      object_targetted=0;
//      play_zsound_always(select_unknown, sound_high_pri, c1_chan1, sound_vol_7);
//      add_to_text_display("TARGET LOST!.",1);
        targ_x=-1; targ_y=-1;
     }
     else
     {
   
        print_distance_in_metres_3_digit_just(targ_x-5 , targ_y+18, dist,misc_text_red, misc_text_green, misc_text_blue,1);

//        print_mk(targ_x-5, targ_y+18, dist,misc_text_red, misc_text_green, misc_text_blue);
        SplatText_Small_rgb(eta_string,targ_x-22 , targ_y+32, misc_text_red, misc_text_green, misc_text_blue);
     }
     if (targetting_in_progress_flag==1)
     {
      targetting_in_progress_flag=0;
      if(gHideTheScreen==0)
      {
        add_to_text_display("HUD> Target Locked",DLP_GREEN);
      }
     }

    }
   }
   else 
   {
      play_zsound_always(select_unknown, sound_high_pri, c1_chan1, sound_vol_7);     
      ocb_ptr->object_list[get_object_targetted(0)].targetted=0;
      set_object_targetted(0,-1);	//not visible
      if(gHideTheScreen==0)
      {
       add_to_text_display("HUD> Target lost",DLP_YELLOW);
      }
   }
  }	//end of object targetted==0 
 
  
    if (sights_targetted==FALSE)
    {
       //rotating sights
       //if mot sitting at 0 degrees rotation then rotate to 0.
     //if (sights_rotation>358) sights_rotation=0;
       
      if (sights_rotation!=45)
      {  
        sights_rotation+=SIGHTS_ROT_PER_SECOND*time_in_secs;
        if (sights_rotation>315) 
          {
           sights_rotation=45;	//sights_rotation-=360;
           sights_radius_min=10;
          }
        sights_radius_min+=sights_length_delta_per_second*time_in_secs;
        if (sights_radius_min>28) sights_length_delta_per_second=-sights_length_delta_per_second;
        if (sights_radius_min<10) sights_length_delta_per_second=-sights_length_delta_per_second;
       }

	
	
	if (pull_up_displayed==0 )
	{

                 
        //line 1
        //optimised 190501 - SB
        glBegin(GL_LINES); 
        
        this_line_rotation=STATIC_CAST_TO_INT(sights_rotation);
        if (this_line_rotation>359) this_line_rotation-=360;       
        xS=STATIC_CAST_TO_INT(targ_x+sights_radius_min*COS((int)this_line_rotation));
        yS=STATIC_CAST_TO_INT(targ_y+sights_radius_min*SIN((int)this_line_rotation));
        xE=STATIC_CAST_TO_INT(targ_x+SIGHTS_RADIUS_MAX*COS((int)this_line_rotation));
        yE=STATIC_CAST_TO_INT(targ_y+SIGHTS_RADIUS_MAX*SIN((int)this_line_rotation));
 	     OGL_line_rgb_quick(xS,yS,xE,yE,sight_colour_red, sight_colour_green, sight_colour_blue);
 
        //line 2
        this_line_rotation=STATIC_CAST_TO_INT(sights_rotation+90);
        if (this_line_rotation>359) this_line_rotation-=360;       
        xS=STATIC_CAST_TO_INT(targ_x+sights_radius_min*COS((int)this_line_rotation));
        yS=STATIC_CAST_TO_INT(targ_y+sights_radius_min*SIN((int)this_line_rotation));
        xE=STATIC_CAST_TO_INT(targ_x+SIGHTS_RADIUS_MAX*COS((int)this_line_rotation));
        yE=STATIC_CAST_TO_INT(targ_y+SIGHTS_RADIUS_MAX*SIN((int)this_line_rotation));
 	     OGL_line_rgb_quick(xS,yS,xE,yE,sight_colour_red, sight_colour_green, sight_colour_blue);
 
        //line 3
        this_line_rotation=STATIC_CAST_TO_INT(sights_rotation+180);
        if (this_line_rotation>359) this_line_rotation-=360;       
        xS=STATIC_CAST_TO_INT(targ_x+sights_radius_min*COS((int)this_line_rotation));
        yS=STATIC_CAST_TO_INT(targ_y+sights_radius_min*SIN((int)this_line_rotation));
        xE=STATIC_CAST_TO_INT(targ_x+SIGHTS_RADIUS_MAX*COS((int)this_line_rotation));
        yE=STATIC_CAST_TO_INT(targ_y+SIGHTS_RADIUS_MAX*SIN((int)this_line_rotation));
 	     OGL_line_rgb_quick(xS,yS,xE,yE,sight_colour_red, sight_colour_green, sight_colour_blue);
        //line 4
        this_line_rotation=STATIC_CAST_TO_INT(sights_rotation+270);
        if (this_line_rotation>359) this_line_rotation-=360;       
        xS=STATIC_CAST_TO_INT(targ_x+sights_radius_min*COS((int)this_line_rotation));
        yS=STATIC_CAST_TO_INT(targ_y+sights_radius_min*SIN((int)this_line_rotation));
        xE=STATIC_CAST_TO_INT(targ_x+SIGHTS_RADIUS_MAX*COS((int)this_line_rotation));
        yE=STATIC_CAST_TO_INT(targ_y+SIGHTS_RADIUS_MAX*SIN((int)this_line_rotation));
 	     OGL_line_rgb_quick(xS,yS,xE,yE,sight_colour_red, sight_colour_green, sight_colour_blue);
        
        glEnd();
        }
     //draw arrow to target from center of hud
        arrow_colour=SIGHT_COLOUR;

     
     if (get_object_targetted(0)!=-1 && sights_rotation==45 && temp.behind==0  )
     {

      if (angle_to_2d>=0 && angle_to_2d<359)	//prevent weird crashes
      {
      if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged!=AP_OFF && pull_up_displayed==0)
       {
        arrow_x1=STATIC_CAST_TO_INT(screen_cent_x+39*COS(angle_to_2d));
        arrow_y1=STATIC_CAST_TO_INT(screen_cent_y+39*SIN(angle_to_2d));
        arrow_x2=STATIC_CAST_TO_INT(screen_cent_x+21*COS(angle_to_2d));
        arrow_y2=STATIC_CAST_TO_INT(screen_cent_y+21*SIN(angle_to_2d));
        //arrow_x3=screen_cent_x+50*COS(angle_to_2d);
        //arrow_y3=screen_cent_y+50*SIN(angle_to_2d);
        
        arrow_colour=248;

       }
      }
     }
        if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.command_autopilot_engaged==AP_STEER_CUE)	//sterring only
        {
         draw_line(arrow_x1,arrow_y1,arrow_x2,arrow_y2,arrow_colour);
        }

    }
    else
    {
    
    }

 }
    
 else draw_emergency_sight(the_object);
}




void draw_emergency_sight(int the_object)
{
 if ((is_dc_good(the_object)==1))
     {
     if (gHideTheScreen==1) return;
     
	    draw_line_no_clip(screen_cent_x-30,screen_cent_y-30,screen_cent_x-15,screen_cent_y-15,SBY_SIGHT_COLOUR);
	    draw_line_no_clip(screen_cent_x+30,screen_cent_y-30,screen_cent_x+15,screen_cent_y-15,SBY_SIGHT_COLOUR);

	    draw_line_no_clip(screen_cent_x-30,screen_cent_y+30,screen_cent_x-15,screen_cent_y+15,SBY_SIGHT_COLOUR);
	    draw_line_no_clip(screen_cent_x+30,screen_cent_y+30,screen_cent_x+15,screen_cent_y+15,SBY_SIGHT_COLOUR);
      }
}



float time_to_stop;

//use fly_pitch and fly_yaw and target_behind.
//two channel - roll and pitch 
//works thus:
//from targetting we get a YAW command and a PITCH command
//along with a targ_behind flag
vector velocity_error;



//this routine called from main loop checks if 
void check_target(void)
{
   if(get_object_targetted(0)==-1)
   target_nearest_planet(0);
}


//this routine searches dyn objs for nearest target and targets it
//takes the object doing the targetting as a param


void target_nearest_planet(int object)
{
int nearest_planet;


nearest_planet=find_nearest_planet(object);
    
if (nearest_planet!=-1) 
  {
   ocb_ptr->object_list[object].targetted=1;

   ocb_ptr->object_list[nearest_planet].targetted=1;

   set_object_targetted(object,nearest_planet);

   if(get_main_camera_object()==object)
   {
    calc_target_name(object);
    targetting_in_progress_flag=1;
   
    add_to_text_display("HUD> Auto targetting nearest planet...",DLP_GREEN);
   }
 
  }
}



//====================================================

 