// *****************************************************************MODULE HEADER*****
// * FILENAME		: dlp_explosions.c
// * MODULE TITLE	: Explosions based on the DLP
// * PROJECT		: Zex
// * DATE STARTED	: 13th March 1999, Sat 11:00pm
// * FIRST AUTHOR	: Rob Probin
// *
// * COPYRIGHT (c) 1999 Lightsoft
// *		   (c) 1999 Robert Probin and Stuart Ball
// *		   http://www.lightsoft.co.uk/
// *
// * Lightsoft is a trading name of Robert Probin and Stuart Ball.
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial		  Descrip.
// *
// *
// *
// ***********************************************************************************

// ***********************************************************************************
// * INCLUDED FILES
// *
/*
 * $Log: dlp_explosions.c,v $
 * Revision 1.1.1.1  2003/09/05 22:35:19  stu_c
 * First Imported.
 *
 * Revision 1.10  2002/09/08 01:57:37  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.9  2002/07/28 17:13:41  stu_c
 * Exhausts and further particle work
 *
 * Revision 1.8  2002/05/31 23:53:33  stu
 * Removed all warnings from the code
 *
 * Revision 1.7  2002/05/05 22:27:41  stu
 * Further developement - hull temperature now correct for multiple suns and atmosphere.

All calls to add_to_kill_list check the return
 *
 * Revision 1.6  2001/12/04 22:58:26  rob
 * Sin tab proto removed
 *
 * Revision 1.5  2001/11/09 18:29:02  rob
 * Various debug changes.
 *
 * Revision 1.4  2001/11/02 19:31:57  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.3  2001/10/23 21:56:01  stu
 * More carbon work
 *
 * Revision 1.2  2001/10/22 21:28:52  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:03:44  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:04  rob
 * First Imported.
 *
 * Revision 1.5  2001/06/23 20:08:32  stu
 * 0.66 checkin
 *
 * Revision 1.4  2001/06/03 03:20:12  stu
 * OCB structure now contains a sub structure called:
ZObjectControl
Which contains all the control data for an object.

Added accessor in access_ocbs which returns a pointer to type ZObjectControl
 *
 * Revision 1.3  2001/05/28 01:48:57  stu
 * 280501
 *
 * Revision 1.2  2001/04/01 18:37:48  stu
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2001/01/01 21:13:34  rob
 * First Imported.
 *
 * Revision 1.4  2000/12/10 03:36:51  stu
 * Calls to kill replaced with either calls to add_to_kill_list OR
i_know_what_im_doing_kill
 *
 * Revision 1.3  2000/12/03 23:52:39  rob
 * changed constants for object type to get uniformity
 *
 * Revision 1.2  2000/11/16 20:19:35  rob
 * orbit
 *
 * Revision 1.1  2000/09/20 23:48:06  stu
 * HUD/AP/Scaling
 *
 * Revision 1.0.0.1  2000/08/21 22:00:44  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"
  #include "options.h"

  #include "3d_structures_def.h"
  #include	"zex_misc.h"
  #include	"error_handler.h"
#endif  /pch

#include "dlp_explosions.h"
#include	"object_control_top.h"
#include	"objects_controls.h"	//controller definitions
#include	"load_objects.h"
#include "explode.h"
#include "Zsound.h"
#include "3d_top.h"
#include "dlp.h"
#include "objects_misc.h"
#include "ship_control.h"
#include "check_bounding_boxes.h"
#include "score_and_credits.h"
#include "instrum.h"
#include	"splat_control.h"
#include "fpoly4.h"
#include "f_light.h"
#include "genpoly.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "obj_control_misc_routines.h"
#include "pictures.h"
#if OGL
#include "ogl.h"
#endif


// ***********************************************************************************
// * CONSTANTS 
// *

#define MAX_NUM_EXPLOSIONS 8		// the maximum number of simulataneous explosions

#define FRAMES_TO_LIVE 15			// the number of frames alive   --- cannot change!!
//#define START_ROTATION 0	  // might want to make this random
#define ROTATION_STEP 10

// ***********************************************************************************
// * TYPE DEFINITIONS
// *

typedef struct {
		// basic unit operation
		int used;
		int frames_left;
				
		// basic screen parameters
		int current_rotation;
		int current_size;
		
		// basic object data
		int object_num;
		int t3d_center_x;
		int t3d_center_y;
		int t3d_center_z;
		
		int	object_size;

		} dlp_explosion_type;


// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

dlp_explosion_type explosion_array[MAX_NUM_EXPLOSIONS];

int last_explosion;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

void plot_polys(int x1, int y1, int u1, int x2, int y2, int u2, int x3, int y3, int u3, int r, int g, int b);


// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: clear_dlp_explosions  NOT USED ANY MORE!!!!!!
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 14/3/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Initalises the internal structures for dlp explosions, and ensures data 
// | is at rest value. Can be called more than once (e.g. at start or end of each game.), or even during
// | the game to kill all explosions.
// |
// | Should be called before any other routines at least once.
// +----------------------------------------------------------------------------------
// | PARAM IN	: none
// |
// | PARAM OUT	: none
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------
// | ADDITIONAL NOTES:
// | put call into clear_dlp()


//void clear_dlp_explosions(void)
//{
//int count;
//
//last_explosion=0;
//
//for(count=0; count<MAX_NUM_EXPLOSIONS; count++)
//  {
//  explosion_array.used[MAX_NUM_EXPLOSIONS]=0;
//  }
//  
//}


// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: cause_dlp_explosion
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 14/3/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Cause a dlp explosion.
// |
// | Creates a small invisible object, similar to the lasers, which 
// | then acts as a the stimulus to producing the effect on screen.
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	: the object that is have the explosion effect (sets the position)
// |
// | PARAM OUT	: none
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------
// object control top
//  - handle burining particle
//  - handle explode 
//
// controller is in .. add to explode list ... explode.c ... current object (int), and small or large type
//					ocb pointer 
//		find object center ... look at lasers  3d_top.c
//		get_screen_front_coord (world_points is translated points)
//
//		generate a new object, assign a control, give it x y z 3d coords, call load in dynamic object
//      in controller do what we ever need to. player_laser.
//
// objects controls.h    ... run_always
//

void cause_dlp_explosion(int old_object_num, int size)
{
int slot;
float shot_x,shot_y,shot_z;
//extern _3D ship_rotated_normal; //from engine
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects
//extern	int last_bullet_fired_frame;
//extern int real_frame_counter;
//extern int zex_alive;
//extern kit_desc ship_items[ITEMS_COUNT];
//extern int shields;

//_3D *this_object_points;
//_ZPoly *the_polys;
//textured_connections *pconnections;
//short conn;
//int cannon_velocity;	  //depends on type of cannon
//int object_aimed_at_slot,distance_from_zex;
load_dyn_object_pb the_params;


slot=find_vacant_dynamic_object_slot();
if (slot!=-1)
  {
       clear_pb(&the_params);
//	object_aimed_at_slot=aim_from_zex(12000);	//pass the distance limit
    
//    #ifdef studebug
//	if (object_aimed_at_slot>500) DebugStr ("\pBad laser aimed at slot from zex >500");
//	#endif
	
//	distance_from_zex=get_distance_to_zex(object_aimed_at_slot);
//	if (object_aimed_at_slot==-1 || ship_items[KSHIELDS].value1<40)
//	{ 
//	//¥¥¥¥this is a NOP in C -- might need to do something here...¥¥¥¥ 
//	}
//	else
//	{
//     ship_items[KSHIELDS].value1=ship_items[KSHIELDS].value1-(*ocb_ptr).object_list[0].laser_category*10;	
//FRIG
//we no laonger use the engine to draw the laser - it's done with teh dlp,
//but we still need a laser object in there for the controller, so we insert
//a basic and very small triangle so as not to upset the renderer	

//	play_zsound_relative_to_zex1(object_aimed_at_slot,laser1, sound_high_pri, sound_vol_7);

	 shot_x=ocb_ptr->object_list[old_object_num].Oworldx;
//	shot_x=shot_x+(400*ship_rotated_normal.x);
	 shot_y=ocb_ptr->object_list[old_object_num].Oworldy;
//	shot_y=shot_y+(400*ship_rotated_normal.y);
	 shot_z=ocb_ptr->object_list[old_object_num].Oworldz;
		
	 the_params.world_x=shot_x;
	 the_params.world_y=shot_y;
	 the_params.world_z=shot_z;
//	the_params.Omovedx=0;
//	the_params.Omovedy=0;
//	the_params.Omovedz=0;
//	the_params.delta_rot_x=0;
//	the_params.delta_rot_y=0;
//	the_params.delta_rot_z=0;
//	the_params.rot_x=0;
	
//	the_params.rot_y=0;
	   
//	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	the_params.rot_z=0;
	 the_params.controller_ref=dlp_exploding_object;	//controller
	 the_params.control_use_1=FRAMES_TO_LIVE;			// frames before disppears
	 the_params.control_use_2=RangedRdm(0,359);	// START_ROTATION;	 	 	 // current rotation
	 the_params.control_use_3=size;				//start size and colour
	 the_params.mass=8;	//should be 8 for weeny laser
//		 	 the_params.dq=0;
	 the_params.object_category=HUMAN;
	 the_params.no_trivial_rejection=1;
//	the_params.unique_id=0;
	 load_dyn_object(ship_laser1,slot,&the_params,-1,1,-1,!DUST_BIT);	//the static object, the position (-1=next free)
//	}
  }
}

// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: draw_dlp_explosions
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 14/3/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Draws an explosion on the screen. Notice, this routine does not work with
// | 3d objects AT ALL.
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	:
// |
// | PARAM OUT	:
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------
// | ADDITIONAL NOTES:
// | put call into run_dlp as case


#define NUMBER_OF_POLYS 8

int explosion_shape[]={
					  0,-33,0, 	19,-161,0,  -19,-161,0,		
						0,33,0,  	 -19,161,0,  19,161,0,	
						35,0,0,       181,20,0,   181,-20,0,    			
						-35,0,0,     -181,-20,0,   -181,20,0,    			

						12,12,0,     41,69,0,     69,42,0,      			
						-12,-12,0,	 -41,-69,0,	-69,-42,0,    			
						12,-12,0, 	  69,-42,0,	41,-69,0,		 
						-12,12,0,    -69,42,0,  -41,69,0,				
					};


// brighten the frame with drawsprockets


void draw_dlp_explosion(int x, int y, int rotation, int type, int frame_key)
{
//extern Ptr screenaddr;	//pointer to current drawing buffer
//extern int screenwidth;
float cos_val,sin_val;
//Ptr pix;
int count;
int *exp_pix;
int xs1,xs2,xs3,ys1,ys2,ys3;
int xc1,xc2,xc3,yc1,yc2,yc3;
int x1,x2,x3,y1,y2,y3;
float frame_key2;
int top_brite,bot_brite;

int r,g,b;


switch(frame_key)
  {
  case 0:
			r=16384; g=0; b=0;
  			break;
  case 1:
			r=16384; g=0; b=0;
  			break;
  case 2:
			r=32768; g=0; b=0;
  			break;
  case 3:
			r=32768; g=0; b=0;
  			break;
  case 4:
			r=49152; g=0; b=0;
  			break;
  case 5:
			r=49152; g=0; b=0;
  			break;
  case 6:
			r=65535; g=0; b=0;
  			break;
  case 7:
			r=65535; g=16384; b=0;
  			break;
  case 8:
			r=65535; g=32768; b=0;
  			break;
  case 9:
			r=65535; g=49152; b=0;
  			break;
  case 10:
			r=65535; g=65535; b=0;
  			break;
  case 11:
			r=65535; g=65535; b=16384;
  			break;
  case 12:
			r=65535; g=65535; b=32768;
  			break;
  case 13:
			r=65535; g=65535; b=49152;
  			break;
  case 14:
			r=65535; g=65535; b=65535;
  			break;
  case 15:
			r=65535; g=65535; b=65535;
  			break;
  default:
			r=65535; g=65535; b=65535;
  			break;
  
  }

cos_val=SIN(rotation);
sin_val=COS(rotation);
	
	frame_key2=(16-frame_key)*0.1;		    // size keying
	top_brite=30000;
	bot_brite=-10000;
	
	  frame_key2*=2;
	  
	if(type==0)
	  {
	  frame_key2/=2;
	  }
	
	exp_pix=explosion_shape;

	for (count=0; count<8; count++)
	  {	
	  xc1=(*(exp_pix)); yc1=(*(exp_pix+1)); xc2=(*(exp_pix+3)); yc2=(*(exp_pix+4)); xc3=(*(exp_pix+6)); yc3=(*(exp_pix+7));
	  xs1=sin_val*xc1; ys1=sin_val*yc1;  xs2=sin_val*xc2; ys2=sin_val*yc2;  xs3=sin_val*xc3; ys3=sin_val*yc3;
	  xc1=cos_val*xc1; yc1=cos_val*yc1;  xc2=cos_val*xc2; yc2=cos_val*yc2;  xc3=cos_val*xc3; yc3=cos_val*yc3;
	  x1=xc1-ys1; y1=xs1+yc1; x2=xc2-ys2; y2=xs2+yc2; x3=xc3-ys3; y3=xs3+yc3;
	  
	  x1*=frame_key2; y1*=frame_key2;
	  x2*=frame_key2; y2*=frame_key2;
	  x3*=frame_key2; y3*=frame_key2;
	  
	  plot_polys(x1+x,y1+y,top_brite,x2+x,y2+y,bot_brite,x3+x,y3+y,bot_brite,r, g, b);
	  exp_pix+=9;
      }


return;
}



/* struct {	int xp1; int yp1; int br1; 
			int xp2; int yp2; int br2; 
			int xp3; int yp2; int br3;
		} explosion_polygons[] = 
		
		{ 
		
*/


// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: process_dlp_explosion
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 14/3/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: move/animate the explosion, and arrange for the dlp to display it.
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	: The object we created earlier.
// |
// | PARAM OUT	:
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------


void process_dlp_explosion(int it)
{
extern	DynObjectsFixedSize *ocb_ptr;
_2D pos_coord;

int rotation,type;
//int colour_index;
int framekey;

ocb_ptr->object_list[it].Dyn_OCB_control_data.control_use_1--;

if (ocb_ptr->object_list[it].Dyn_OCB_control_data.control_use_1<=0)
  { 
  if (add_to_kill_list(it)!=0)
	{
	  MyDebugStr(__LINE__, __FILE__,"add_to_kill_list failed!");
	}
 }
else
  { 
// if it hasn't died...

  // post a note dlp to print it later
  pos_coord=get_any_screen_coord(it);  
  
  rotation=ocb_ptr->object_list[it].Dyn_OCB_control_data.control_use_2;
  type=ocb_ptr->object_list[it].Dyn_OCB_control_data.control_use_3;
  framekey=ocb_ptr->object_list[it].Dyn_OCB_control_data.control_use_1;

  if(type>=2)
    {
    add_to_dlp(DRAW_OUTER_FLARE,pos_coord.x,pos_coord.y,(16-framekey)<<2,1,0,0,0,0,0);
    }

  add_to_dlp(DRAW_DLP_EXPLOSION,pos_coord.x,pos_coord.y,rotation,type,framekey,0,0,0,0);

//  add_to_dlp(DRAW_DLP_EXPLOSION,pos_coord.x,pos_coord.y,rotation,type,ocb_ptr->object_list[it].control_use_1,0);


  // actual controlling
  ocb_ptr->object_list[it].Dyn_OCB_control_data.control_use_2+=ROTATION_STEP;
  
  if(ocb_ptr->object_list[it].Dyn_OCB_control_data.control_use_2>=360)
    {
    ocb_ptr->object_list[it].Dyn_OCB_control_data.control_use_2-=360;
    }
  if(ocb_ptr->object_list[it].Dyn_OCB_control_data.control_use_2<0)
    {
    ocb_ptr->object_list[it].Dyn_OCB_control_data.control_use_2+=360;
    }

  }
}



// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: plot_polys
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 18/3/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: plot a polygon
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	: x1,y1,u1
// |              x2,y2,u2
// |              x3,y3,u3
// |
// | PARAM OUT	:
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------


dcoord *point_list;
dcoord points[10];
dcoord temp_points[10];
dcoord output_pointsd[30];
int polys;

#if OGL==1
extern fdcoord ftoutput_points[30];	//float temp out points (OGL) - in fcircle
#endif

void plot_polys(int x1, int y1, int u1, int x2, int y2, int u2, int x3, int y3, int u3, int r, int g, int b)
{
//extern int OGL_active;

point_list=points;
point_list->x=x1; point_list->y=y1; point_list->u=I_TO_F(u1); point_list++;
point_list->x=x2; point_list->y=y2; point_list->u=I_TO_F(u2); point_list++;
point_list->x=x3; point_list->y=y3; point_list->u=I_TO_F(u3); point_list++;

distort_window_clipper3(points,temp_points,output_pointsd,&polys);
point_list=output_pointsd;
#if OGL==1

while(polys>0)
  {
//  gouraud3((char *)screenaddr,screenwidth, point_list, r, g, b, 1);       	    // red polygon
	  dcoord * op;
	  fdcoord * fop;
	  
//ogl likes all points in the range -1 to 1, where 0 is center, so we have to convert our integer points to floating points in this range
//y is also reversed
          int i;
          

          op=point_list;
          fop=ftoutput_points;
          
          for (i=0;i<3;i++)
          {
          fop->x=op->x;
          fop->x-=screen_cent_x;
          fop->x = fop->x/screen_cent_x;

          fop->y=monitor_h-op->y;
          fop->y-=screen_cent_y;
          fop->y = fop->y/screen_cent_y;
          
          fop->u=op->u;	//lighting
//          fop->u=fop->u;
//          fop->v=op->v;
          op++;
          fop++;
          }
//       polygon(output_lines,(coord *)outpoints,fpoly_colour,screenaddr,screenwidth);
       OGL_Gpolygon(3,(fdcoord *)ftoutput_points,r,g,b, 1);
  
  point_list+=3;
  polys--;
  
}


#else
while(polys>0)
  {
  gouraud3((char *)screenaddr,screenwidth, point_list, r, g, b, 1);       	  // red polygon 
  point_list+=3;
  polys--;
  }
#endif



} // end of function



// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: 
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 8/4/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: 
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	: 
// |              
// |              
// |
// | PARAM OUT	:
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------


/*

1. produce scattering of polygons with some randomness

2. rotate and plot this set


*/




// coord my_points[60];		// 20 polygons



