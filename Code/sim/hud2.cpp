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

/*
 *  hud2.cpp
 *  zex22
 *
 *  Created by Stuart Ball on Sun Apr 21 2002.
 *  Copyright (c) 2002 Lightsoft. All rights reserved.
 *
 * $Log$
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"
  #include "zplatform.h"
  #include "LS_Standard.h"

  #include "3d_structures_def.h"
  #include "zex_misc.h"
  #include "processor.h"
  #include "real_time.h"
  #include "Error_handler.h"
  #include "access_ocbs.h"
#endif //pch

#include "dlp.h"
#include "hud.h"
#include "command_and_control.h"
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
#include "galaxy.h"


extern DynObjectsFixedSize *ocb_ptr;

void show_gen_page(void);
void calc_strings(void);
void draw_frame(void);
void show_targ_page(void);
void show_status1_page(void);


/*maintains and displays the following data:
rank, credits, mass, dc volts, dc amps, ac volts, ac amps, fuel, fuel consumption, TEMPMULT
*/


//Misc page
static char   rank_str[]= "Rank";
static char   cred_str[]= "Cred";
static char   mass_str[]= "Mass";
static char shield_str[]= "Shld";
static char   hull_str[]= "Hull";

static char   fuel_str[]= "Fuel";
static char   flow_str[]= "Flow";
static char     tm_str[]= "TM  ";
static char     et_str[]= "EngT";
static char    oat_str[]= "HulT"; //outside temperature


static char rankc_str[20]= "";
static char credc_str[20]= "";
static char massc_str[20]= "";

static char fuelc_str[20]= "";
static char flowc_str[20]= "";
static char   tmc_str[20]= "";
static char   etc_str[20]= "";
static char  oatc_str[20]= ""; //outside temperature

static char  shieldc_str[20]= ""; //shields
static char  hullc_str[20]= ""; //hull

//status 1 strings
static char  hypert_str[]= "HT Shells"; //hypertransport
static char  hypertvm_str[20]= "/"; //hypertransport max count
static char  hypertv_str[20]= ""; //hypertransport count

static char ac_str[]="AC Volts";
static char   acc_str[20]= "";

static char dc_str[]="DC Volts";
static char   dcc_str[20]= "";

/*
//Weapons page
static char miss_str[]= "    Missiles :                                                      ";
static char laser_str[]="  Laser Temp :                                                      ";
static char targ_str[]= "  Tgt Status :                                                      ";
static char cool_str[]= " Cool Status :                                                      ";

//Nav page
static char   AP_str[]= "  AP Status  :                                                      ";
static char scnr_str[]= "Scnr Status  :                                                      ";
static char tgt_str[]=  " Tgt Status  :                                                      ";

//Environment page
static char env_str[]=  "  Env Status :                                                      ";
static char hold_str[]= " Hold Status :                                                      ";
static char targ1_str[]="  Rep Status :                                                      ";
*/


static float misc_text_red2=1, misc_text_green2=0.0, misc_text_blue2=0.1;

static float rot_array[]={0.6, 0.8, 1.0, 0.8, 0.6, 0.8, 1.0, 0.8};
static float rot_time=0.0;
static int pane=0;

void init_mfd_pane(void)
{
 pane=0;
}

void toggle_mfd_pane(void)
{
 pane++;
 if (pane>2) pane=0;
}


void process_and_draw_ancillary_info(void)
{
float tempf;

    glDisable (GL_BLEND); //None of this blended
    calc_strings();
	
    if(do_throb==1)
	{
        if(rot_time>0.12) //rotate the bars if aliens on scanner
		{
		 rot_time=0.0;
         tempf=rot_array[0];
		 rot_array[0]=rot_array[1];
		 rot_array[1]=rot_array[2];
		 rot_array[2]=rot_array[3];
		 rot_array[3]=rot_array[4];
		 rot_array[4]=rot_array[5];
		 rot_array[5]=rot_array[6];
		 rot_array[6]=rot_array[7];
		 rot_array[7]=tempf;
		}
	}
	else
	{
	    if(rot_array[2]!=1.0)
		{
		 rot_array[0]=0.6;
	     rot_array[1]=0.8;
	     rot_array[2]=1.0;
	     rot_array[3]=0.8;
	     rot_array[4]=0.6;
	     rot_array[5]=0.8;
	     rot_array[6]=1.0;
	     rot_array[7]=0.8;
		 
	    }
	}	
	
		rot_time+=real_frame_time_in_secs;
       draw_frame();        
       
    if (pane==0)
    {
     show_gen_page();
    }
    else
    if (pane==1)
    {
       show_targ_page();
    }
    else
    if (pane==2)
    {
       show_status1_page();
    }

}


#define RB_START 160 //from centre of screen
#define LB_START RB_START
#define RB_MID1 240
#define RB_START2 280


#define TOP 120

void draw_frame(void)
{
//Horrible mess of code but it's just an unlooped bunch of line calls...


//static int compiled=0;
//GLuint name=0;

//        if (compiled==0)
//		{		
//	     name=1;
//         glNewList(name, GL_COMPILE);
		glBegin(GL_LINES);
//top right left
	    OGL_line_rgb_quick(screen_cent_x+(RB_START-2),monitor_h-(TOP+2),screen_cent_x+(RB_MID1),monitor_h-(TOP+2),rot_array[0],misc_text_green2,misc_text_blue2);	    
		OGL_line_rgb_quick(screen_cent_x+(RB_START-1),monitor_h-(TOP+1),screen_cent_x+(RB_MID1),monitor_h-(TOP+1),rot_array[1],misc_text_green2,misc_text_blue2);	    
		OGL_line_rgb_quick(screen_cent_x+RB_START,monitor_h-(TOP),screen_cent_x+(RB_MID1),monitor_h-TOP,rot_array[2],misc_text_green2,misc_text_blue2);
		OGL_line_rgb_quick(screen_cent_x+(RB_START+1),monitor_h-(TOP-1),screen_cent_x+(RB_MID1),monitor_h-(TOP-1),rot_array[3],misc_text_green2,misc_text_blue2);	    
		OGL_line_rgb_quick(screen_cent_x+(RB_START+2),monitor_h-(TOP-2),screen_cent_x+(RB_MID1),monitor_h-(TOP-2),rot_array[4],misc_text_green2,misc_text_blue2);
//the hoop - top
	    OGL_line_rgb_quick(screen_cent_x+(RB_MID1),monitor_h-(TOP+2),screen_cent_x+(RB_MID1+5),monitor_h-(TOP+7),rot_array[0],misc_text_green2,misc_text_blue2);	    
	    OGL_line_rgb_quick(screen_cent_x+(RB_MID1),monitor_h-(TOP+1),screen_cent_x+(RB_MID1+5),monitor_h-(TOP+6),rot_array[1],misc_text_green2,misc_text_blue2);	    

	    OGL_line_rgb_quick(screen_cent_x+(RB_MID1+5),monitor_h-(TOP+7),screen_cent_x+(RB_START2-5),monitor_h-(TOP+7),rot_array[0],misc_text_green2,misc_text_blue2);	    
	    OGL_line_rgb_quick(screen_cent_x+(RB_MID1+5),monitor_h-(TOP+6),screen_cent_x+(RB_START2-5),monitor_h-(TOP+6),rot_array[1],misc_text_green2,misc_text_blue2);	    

	    OGL_line_rgb_quick(screen_cent_x+(RB_START2-5),monitor_h-(TOP+7),screen_cent_x+(RB_START2),monitor_h-(TOP+2),rot_array[0],misc_text_green2,misc_text_blue2);	    
	    OGL_line_rgb_quick(screen_cent_x+(RB_START2-5),monitor_h-(TOP+6),screen_cent_x+(RB_START2),monitor_h-(TOP+1),rot_array[1],misc_text_green2,misc_text_blue2);	    



//the hoop - bottom
	    OGL_line_rgb_quick(screen_cent_x+(RB_MID1),monitor_h-(TOP-2),screen_cent_x+(RB_MID1+5),monitor_h-(TOP-7),rot_array[4],misc_text_green2,misc_text_blue2);	    
	    OGL_line_rgb_quick(screen_cent_x+(RB_MID1),monitor_h-(TOP-1),screen_cent_x+(RB_MID1+5),monitor_h-(TOP-6),rot_array[3],misc_text_green2,misc_text_blue2);	    

	    OGL_line_rgb_quick(screen_cent_x+(RB_MID1+5),monitor_h-(TOP-7),screen_cent_x+(RB_START2-5),monitor_h-(TOP-7),rot_array[4],misc_text_green2,misc_text_blue2);	    
	    OGL_line_rgb_quick(screen_cent_x+(RB_MID1+5),monitor_h-(TOP-6),screen_cent_x+(RB_START2-5),monitor_h-(TOP-6),rot_array[3],misc_text_green2,misc_text_blue2);	    

	    OGL_line_rgb_quick(screen_cent_x+(RB_START2-5),monitor_h-(TOP-7),screen_cent_x+(RB_START2),monitor_h-(TOP-2),rot_array[4],misc_text_green2,misc_text_blue2);	    
	    OGL_line_rgb_quick(screen_cent_x+(RB_START2-5),monitor_h-(TOP-6),screen_cent_x+(RB_START2),monitor_h-(TOP-1),rot_array[3],misc_text_green2,misc_text_blue2);	    
//text

//top right right
	    OGL_line_rgb_quick(screen_cent_x+RB_START2,monitor_h-(TOP+2),monitor_w,monitor_h-(TOP+2),rot_array[0],misc_text_green2,misc_text_blue2);	    
		OGL_line_rgb_quick(screen_cent_x+RB_START2,monitor_h-(TOP+1),monitor_w,monitor_h-(TOP+1),rot_array[1],misc_text_green2,misc_text_blue2);	    
		OGL_line_rgb_quick(screen_cent_x+RB_START2,monitor_h-(TOP),monitor_w,monitor_h-TOP,rot_array[2],misc_text_green2,misc_text_blue2);
		OGL_line_rgb_quick(screen_cent_x+RB_START2,monitor_h-(TOP-1),monitor_w,monitor_h-(TOP-1),rot_array[3],misc_text_green2,misc_text_blue2);	    
		OGL_line_rgb_quick(screen_cent_x+RB_START2,monitor_h-(TOP-2),monitor_w,monitor_h-(TOP-2),rot_array[4],misc_text_green2,misc_text_blue2);	    



//diagonal right
	    OGL_line_rgb_quick(screen_cent_x+118,monitor_h-1,screen_cent_x+(RB_START-2),monitor_h-(TOP+2),rot_array[0],misc_text_green2,misc_text_blue2);
	    OGL_line_rgb_quick(screen_cent_x+119,monitor_h-1,screen_cent_x+(RB_START-1),monitor_h-(TOP+1),rot_array[1],misc_text_green2,misc_text_blue2);
	    OGL_line_rgb_quick(screen_cent_x+120,monitor_h-1,screen_cent_x+RB_START,monitor_h-TOP,rot_array[2],misc_text_green2,misc_text_blue2);
	    OGL_line_rgb_quick(screen_cent_x+121,monitor_h-1,screen_cent_x+(RB_START+1),monitor_h-(TOP-1),rot_array[3],misc_text_green2,misc_text_blue2);
	    OGL_line_rgb_quick(screen_cent_x+122,monitor_h-1,screen_cent_x+(RB_START+2),monitor_h-(TOP-2),rot_array[4],misc_text_green2,misc_text_blue2);



#define LB_MID1 240
#define LB_START2 280


//top left right
	    OGL_line_rgb_quick(screen_cent_x-(LB_START-2),monitor_h-(TOP+2),screen_cent_x-(LB_MID1+1),monitor_h-(TOP+2),rot_array[0],misc_text_green2,misc_text_blue2);
	    OGL_line_rgb_quick(screen_cent_x-(LB_START-1),monitor_h-(TOP+1),screen_cent_x-(LB_MID1+1),monitor_h-(TOP+1),rot_array[1],misc_text_green2,misc_text_blue2);
	    OGL_line_rgb_quick(screen_cent_x-LB_START,monitor_h-(TOP),screen_cent_x-(LB_MID1+1),monitor_h-TOP,rot_array[2],misc_text_green2,misc_text_blue2);
	    OGL_line_rgb_quick(screen_cent_x-(LB_START+1),monitor_h-(TOP-1),screen_cent_x-(LB_MID1+1),monitor_h-(TOP-1),rot_array[3],misc_text_green2,misc_text_blue2);
	    OGL_line_rgb_quick(screen_cent_x-(LB_START+2),monitor_h-(TOP-2),screen_cent_x-(LB_MID1+1),monitor_h-(TOP-2),rot_array[4],misc_text_green2,misc_text_blue2);


//top left left
	    OGL_line_rgb_quick(screen_cent_x-(LB_START2),monitor_h-(TOP+2),0,monitor_h-(TOP+2),rot_array[0],misc_text_green2,misc_text_blue2);
	    OGL_line_rgb_quick(screen_cent_x-(LB_START2),monitor_h-(TOP+1),0,monitor_h-(TOP+1),rot_array[1],misc_text_green2,misc_text_blue2);
	    OGL_line_rgb_quick(screen_cent_x-LB_START2,monitor_h-(TOP),0,monitor_h-TOP,rot_array[2],misc_text_green2,misc_text_blue2);
	    OGL_line_rgb_quick(screen_cent_x-(LB_START2),monitor_h-(TOP-1),0,monitor_h-(TOP-1),rot_array[3],misc_text_green2,misc_text_blue2);
	    OGL_line_rgb_quick(screen_cent_x-(LB_START2),monitor_h-(TOP-2),0,monitor_h-(TOP-2),rot_array[4],misc_text_green2,misc_text_blue2);


//the hoop - top
	    OGL_line_rgb_quick(screen_cent_x-(LB_START2),monitor_h-(TOP+2),screen_cent_x-(LB_START2-5),monitor_h-(TOP+7),rot_array[0],misc_text_green2,misc_text_blue2);	    

	    OGL_line_rgb_quick(screen_cent_x-(LB_START2),monitor_h-(TOP+1),screen_cent_x-(LB_START2-5),monitor_h-(TOP+6),rot_array[1],misc_text_green2,misc_text_blue2);	    

	    OGL_line_rgb_quick(screen_cent_x-(LB_START2-5),monitor_h-(TOP+7),screen_cent_x-(LB_MID1+5),monitor_h-(TOP+7),rot_array[0],misc_text_green2,misc_text_blue2);	    
	    OGL_line_rgb_quick(screen_cent_x-(LB_START2-5),monitor_h-(TOP+6),screen_cent_x-(LB_MID1+5),monitor_h-(TOP+6),rot_array[1],misc_text_green2,misc_text_blue2);	    

	    OGL_line_rgb_quick(screen_cent_x-(LB_MID1+5),monitor_h-(TOP+7),screen_cent_x-LB_MID1,monitor_h-(TOP+2),rot_array[0],misc_text_green2,misc_text_blue2);	    
	    OGL_line_rgb_quick(screen_cent_x-(LB_MID1+5),monitor_h-(TOP+6),screen_cent_x-LB_MID1,monitor_h-(TOP+1),rot_array[1],misc_text_green2,misc_text_blue2);	    



//the hoop - bottom
	    OGL_line_rgb_quick(screen_cent_x-(LB_START2),monitor_h-(TOP-2),screen_cent_x-(LB_START2-5),monitor_h-(TOP-7),rot_array[4],misc_text_green2,misc_text_blue2);	    
	    OGL_line_rgb_quick(screen_cent_x-(LB_START2),monitor_h-(TOP-1),screen_cent_x-(LB_START2-5),monitor_h-(TOP-6),rot_array[3],misc_text_green2,misc_text_blue2);	    

	    OGL_line_rgb_quick(screen_cent_x-(LB_START2-5),monitor_h-(TOP-7),screen_cent_x-(LB_MID1+5),monitor_h-(TOP-7),rot_array[4],misc_text_green2,misc_text_blue2);	    
	    OGL_line_rgb_quick(screen_cent_x-(LB_START2-5),monitor_h-(TOP-6),screen_cent_x-(LB_MID1+5),monitor_h-(TOP-6),rot_array[3],misc_text_green2,misc_text_blue2);	    

	    OGL_line_rgb_quick(screen_cent_x-(LB_MID1+5),monitor_h-(TOP-7),screen_cent_x-(LB_MID1),monitor_h-(TOP-2),rot_array[4],misc_text_green2,misc_text_blue2);	    
	    OGL_line_rgb_quick(screen_cent_x-(LB_MID1+5),monitor_h-(TOP-6),screen_cent_x-(LB_MID1),monitor_h-(TOP-1),rot_array[3],misc_text_green2,misc_text_blue2);	    



//DIagonal left
	    OGL_line_rgb_quick(screen_cent_x-118,monitor_h-1,screen_cent_x-(LB_START-2),monitor_h-(TOP+2),rot_array[0],misc_text_green2,misc_text_blue2); //outside
	    OGL_line_rgb_quick(screen_cent_x-119,monitor_h-1,screen_cent_x-(LB_START-1),monitor_h-(TOP+1),rot_array[1],misc_text_green2,misc_text_blue2);
	    OGL_line_rgb_quick(screen_cent_x-120,monitor_h-1,screen_cent_x-LB_START,monitor_h-TOP,rot_array[2],misc_text_green2,misc_text_blue2);
	    OGL_line_rgb_quick(screen_cent_x-121,monitor_h-1,screen_cent_x-(LB_START+1),monitor_h-(TOP-1),rot_array[3],misc_text_green2,misc_text_blue2);
	    OGL_line_rgb_quick(screen_cent_x-122,monitor_h-1,screen_cent_x-(LB_START+2),monitor_h-(TOP-2),rot_array[4],misc_text_green2,misc_text_blue2);
        glEnd();		

//Now the text for the frames
		   glColor3d(misc_text_red2, misc_text_green2, misc_text_blue2);
		SplatText_mon_fast("MFD",screen_cent_x+RB_MID1+13 , monitor_h-(TOP+2));
		SplatText_mon_fast("OBC",screen_cent_x-LB_START2+13 , monitor_h-(TOP+2));


/*
		glEndList();
	}
	else
	{
	 glCallList(1);
	}
*/

}

static int calc_string_index=0;
static int calc_string_stat1_index=0;

static float calc_string_timer=0.0;
void calc_strings(void)
{
extern int rank, credits;
int camera_object=get_main_camera_object();

calc_string_timer+=time_in_secs;

if (calc_string_timer<0.1) return; //every 1/20 of second
calc_string_timer=0;
if (pane==0)
{ 
 
 if (calc_string_index==0)
 {
  //calc rank string
  fill_rank(rank, rankc_str);
  
  calc_string_index++;
 }
 else
 if (calc_string_index==1)
 {
  //calc credits string
  NumToString(credits,(unsigned char *)credc_str);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
  credc_str[credc_str[0]+1]='C';
  credc_str[credc_str[0]+2]='r';
  credc_str[credc_str[0]+3]=0;
  credc_str[0]=' ';
  calc_string_index++;
 }
 else  
 if (calc_string_index==2) //fuel
 {
  //calc fuel string
  NumToString(STATIC_CAST_TO_LONG((*ocb_ptr).object_list[camera_object].Dyn_OCB_control_data.fuel_tank_value_in_KGS*1000),(unsigned char *)fuelc_str);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
  fuelc_str[fuelc_str[0]+1]='m';
  fuelc_str[fuelc_str[0]+2]='g';
  fuelc_str[fuelc_str[0]+3]=0;	//quick p to c hack
  fuelc_str[0]=' ';
  //calc mass string
  calc_string_index++;
 }
 else  
 if (calc_string_index==3) //fuel consumption
 {
  //calc fuel string
  NumToString(STATIC_CAST_TO_LONG(get_total_fuel_consumption(camera_object)*1000000.0*get_time_multiplier()),(unsigned char *)flowc_str); //multiplier should be 1e6 but orcs consumption is big frig
  flowc_str[flowc_str[0]+1]='n';
  flowc_str[flowc_str[0]+2]='g';
  flowc_str[flowc_str[0]+3]=0;	//quick p to c hack
  flowc_str[0]=' ';
  //calc mass string
  calc_string_index++;
 }
 else  
 if (calc_string_index==4) //engine temperature
 {
  //calc fuel string
  NumToString(STATIC_CAST_TO_LONG(get_engine_temperature_in_C(camera_object)),(unsigned char *)etc_str);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
  etc_str[etc_str[0]+1]='C';	//quick p to c hack
  etc_str[etc_str[0]+2]=0;	//quick p to c hack
  etc_str[0]=' ';
  //calc mass string
  calc_string_index++;
 }
 else 
  if (calc_string_index==5) //nothing
  {
   calc_string_index++;
  }  
 if (calc_string_index==6) //Hull
 {
  //calc fuel string
  NumToString(100,(unsigned char *)hullc_str);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
  hullc_str[hullc_str[0]+1]='%';
  hullc_str[hullc_str[0]+2]=0;	//quick p to c hack
  hullc_str[0]=' ';
  //calc mass string
  calc_string_index++;
 }
 else  
 if (calc_string_index==7) //shields
 {
  //calc fuel string
  NumToString(100,(unsigned char *)shieldc_str);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
  shieldc_str[shieldc_str[0]+1]='%';
  shieldc_str[shieldc_str[0]+2]=0;	//quick p to c hack
  shieldc_str[0]=' ';
  //calc mass string
  calc_string_index++;
 }
 else  
 if (calc_string_index==8) //tempmult
 {
  //calc fuel string
  NumToString(get_time_multiplier(),(unsigned char *)tmc_str);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
  tmc_str[tmc_str[0]+1]=0;	//quick p to c hack
  tmc_str[0]=' ';
  //calc mass string
  calc_string_index++;
 }
 else  
 if (calc_string_index==9) //mass
 {
  //calc fuel string
  NumToString(STATIC_CAST_TO_LONG(get_mass_in_kg(camera_object)),(unsigned char *)massc_str);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
  massc_str[massc_str[0]+1]='k';
  massc_str[massc_str[0]+2]='g';
  massc_str[massc_str[0]+3]=0;	//quick p to c hack
  massc_str[0]=' ';
  //calc mass string
  calc_string_index++;
 }
 else  
 if (calc_string_index==10) //hull temperature
 {
  //calc fuel string
  NumToString(STATIC_CAST_TO_LONG(get_hull_temperature_in_C(camera_object)),(unsigned char *)oatc_str);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
  oatc_str[oatc_str[0]+1]='C';
  oatc_str[oatc_str[0]+2]=0;	//quick p to c hack
  oatc_str[0]=' ';
  //calc mass string
  calc_string_index=0;
 }

} //end of pane=0 
else
if (pane==2)
{
 if (calc_string_stat1_index==0)
 {
 sds_attrib_data_type my_attrib;
  get_ship_attrib_data(get_main_camera_object(), HTRNV, &my_attrib);
  
   NumToString(my_attrib.the_value, (unsigned char *)hypertv_str);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
   hypertv_str[hypertv_str[0]+1]=0;	//quick p to c hack
   hypertv_str[0]=' ';

  get_ship_attrib_data(get_main_camera_object(), HTRNMAX, &my_attrib); //display max count
  
   NumToString(my_attrib.the_value,(unsigned char *)&hypertvm_str[1]);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
   hypertvm_str[hypertvm_str[1]+2]=0;	//quick p to c hack
   hypertvm_str[1]=' ';

   calc_string_stat1_index++;
 }
  else  
 if (calc_string_stat1_index==1) //AC
 {
  //calc fuel string
  NumToString(STATIC_CAST_TO_LONG(get_ac_volts(camera_object)),(unsigned char *)acc_str);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
  acc_str[acc_str[0]+1]='V';
  acc_str[acc_str[0]+2]=0;	//quick p to c hack
  acc_str[0]=' ';
  //calc mass string
  calc_string_stat1_index++;
 }
  else  
 if (calc_string_stat1_index==2) //DC
 {
  //calc fuel string
  NumToString(STATIC_CAST_TO_LONG(get_dc_volts(camera_object)),(unsigned char *)dcc_str);		// this hack is HORRID ... pstrings are unsigned, cstrings are signed
  dcc_str[dcc_str[0]+1]='V';
  dcc_str[dcc_str[0]+2]=0;	//quick p to c hack
  dcc_str[0]=' ';
  //calc mass string
  calc_string_stat1_index=0;
 }

 
}
}



#define TEXT_LEFT1 RB_START+20
#define NUMBER_LEFT1 RB_START+50

#define TEXT_RIGHT1 RB_START+130
#define NUMBER_RIGHT1 RB_START+160

#define TEXT_OFFSET_FROM_TOP 20

void show_targ_page(void)
{
  glColor3d(misc_text_red2, misc_text_green2, misc_text_blue2);

  draw_item_info(get_object_targetted(get_main_camera_object()),monitor_w-210,monitor_h-80,250);

}


void show_status1_page(void)
{
  glColor3d(misc_text_red2, misc_text_green2, misc_text_blue2);

  SplatText_mon_fast(hypert_str,screen_cent_x+TEXT_LEFT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP));
  SplatText_mon_fast(hypertv_str,screen_cent_x+NUMBER_LEFT1+24 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP));
  SplatText_mon_fast(hypertvm_str,screen_cent_x+NUMBER_LEFT1+40 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP));



  SplatText_mon_fast(ac_str,screen_cent_x+TEXT_LEFT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-20));
  SplatText_mon_fast(acc_str,screen_cent_x+NUMBER_LEFT1+24 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-20));

  SplatText_mon_fast(dc_str,screen_cent_x+TEXT_LEFT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-40));
  SplatText_mon_fast(dcc_str,screen_cent_x+NUMBER_LEFT1+24 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-40));

}

void show_gen_page(void)
{
	    glColor3d(misc_text_red2, misc_text_green2, misc_text_blue2);

		SplatText_mon_fast(rank_str,screen_cent_x+TEXT_LEFT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP));
		SplatText_mon_fast(rankc_str,screen_cent_x+NUMBER_LEFT1+7 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP));
       
		SplatText_mon_fast(cred_str,screen_cent_x+ TEXT_LEFT1, monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-20));
		SplatText_mon_fast(credc_str,screen_cent_x+NUMBER_LEFT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-20));
		
        SplatText_mon_fast(fuel_str,screen_cent_x+TEXT_LEFT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-40));
		SplatText_mon_fast(fuelc_str,screen_cent_x+NUMBER_LEFT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-40));

        SplatText_mon_fast(flow_str,screen_cent_x+TEXT_LEFT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-60));
		SplatText_mon_fast(flowc_str,screen_cent_x+NUMBER_LEFT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-60));

        SplatText_mon_fast(et_str,screen_cent_x+TEXT_LEFT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-80));
  		SplatText_mon_fast(etc_str,screen_cent_x+NUMBER_LEFT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-80));



        SplatText_mon_fast(hull_str,screen_cent_x+TEXT_RIGHT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP));
		SplatText_mon_fast(hullc_str,screen_cent_x+NUMBER_RIGHT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP));

		SplatText_mon_fast(shield_str,screen_cent_x+TEXT_RIGHT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-20));
		SplatText_mon_fast(shieldc_str,screen_cent_x+NUMBER_RIGHT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-20));
		
        SplatText_mon_fast(tm_str,screen_cent_x+TEXT_RIGHT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-40));
		SplatText_mon_fast(tmc_str,screen_cent_x+NUMBER_RIGHT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-40));

        SplatText_mon_fast(mass_str,screen_cent_x+TEXT_RIGHT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-60));
        SplatText_mon_fast(massc_str,screen_cent_x+NUMBER_RIGHT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-60));

		
        SplatText_mon_fast(oat_str,screen_cent_x+TEXT_RIGHT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-80));
        SplatText_mon_fast(oatc_str,screen_cent_x+NUMBER_RIGHT1 , monitor_h-(TOP-TEXT_OFFSET_FROM_TOP-80));
		


		
		
		
		
}