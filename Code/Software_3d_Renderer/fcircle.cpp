// *****************************************************************MODULE HEADER*****
// * FILENAME		: fcircle.c
// * MODULE TITLE	: Circle drawing
// * PROJECT		: Zex
// * DATE STARTED	: 10th April 1999, Sat 10:12pm
// * ORIGINAL AUTHOR: Rob Probin
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
/*
 * $Log: fcircle.c,v $
 * Revision 1.1.1.1  2003/09/05 22:36:25  stu_c
 * First Imported.
 *
 * Revision 1.11  2002/09/12 21:26:01  stu_c
 * Commented out software circle drawing code
 *
 * Revision 1.10  2002/09/12 21:18:46  stu_c
 * Fixed lens flares not working
 *
 * Revision 1.9  2002/09/08 02:06:27  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.8  2001/12/29 01:10:38  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.7  2001/12/04 22:59:01  rob
 * sin tab proto removed
 *
 * Revision 1.6  2001/11/09 18:28:56  rob
 * Various debug changes.
 *
 * Revision 1.5  2001/11/02 19:31:35  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.4  2001/10/24 21:41:33  stu
 * First set of source to build on pb for carbon.
 *
 * Revision 1.3  2001/10/23 21:55:01  stu
 * More carbon work
 *
 * Revision 1.2  2001/10/22 21:28:47  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:04:08  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:07  rob
 * First Imported.
 *
 * Revision 1.2  2001/05/28 01:49:49  stu
 * 280501
 *
 * Revision 1.1.1.1  2001/01/01 21:13:39  rob
 * First Imported.
 *
 * Revision 1.3  2000/09/13 20:32:18  stu
 * Changes for hud targetting and sun corona effects.
 *
 * Revision 1.2  2000/09/09 22:31:39  stu
 * Various unfilled circle functions
 *
 * Revision 1.1  2000/09/05 00:39:03  stu
 * Added new unfilled circle and elipse routines
 *
 * Revision 1.0.0.1  2000/08/21 22:09:22  stu
 * First Imported.
 *
 */

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <memory.h>
  #endif
  #include "Error_handler.h"
  #include "options.h"
  #include "3d_structures_def.h"

#endif //pch

#include "fpoly4.h"
#include "genpoly.h"
#include "fcircle.h"
#include "f_light.h"
#include "pictures.h"
#include	"splat_control.h"

#if OGL
#include "ogl.h"
#endif

#include "fdistort.h"

fdcoord ftoutput_points[30];	 //float temp out points (OGL)
// ***********************************************************************************
// * CONSTANTS 
// *

#define CIRCLE_ENTRIES 1024

// ***********************************************************************************
// * TYPE DEFINITIONS
// *

#define I_TO_FIX(x) (  ((int)(x))  <<  16  )
#define FIX_TO_I(x) ((x)>>16)

// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

typedef float circle_type;

circle_type *circle_table_ptr;
void fcircle_test(void);



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

module_private float squareroot( float R);
//module_private void tpline(int x1, int x2, char *line_screen_address, unsigned char colour);
//module_private void aliased_v_pixel(Ptr screen_x, unsigned char colour);

// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: fcircle_test
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 10/4/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Tests the circle drawing
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

void fcircle_test(void)
{
static int pause_count=0;
static int size_count=20;
static int colour_count=0;

fcircle(100,100,size_count,colour_count,MODE_SOLID);

pause_count++; 
if(pause_count>5) 
  {
  pause_count=0;
  size_count++; if(size_count>10) size_count=0;
  colour_count++; colour_count&=0xff;
  }

}

// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: create_outer_flare
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 10/4/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Draws various sizes of circle to create the outside of a lens flair effect
// | as used in the explosion routines.
// +----------------------------------------------------------------------------------
// | PARAM IN	: x = x center of the effect. Effect is as if the screen center is the lens center
// |              y = y center of the effect. 
// |              size = size of the effect 0-
// |              brightness = brightness of the effect. Currently used, but probably should be in 
// |              relation to the size of the effect, and also the "power" of the effect. No range yet.
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

int screen_x_center;
int screen_y_center;			// init by init circle.


void create_outer_flare(int x, int y, int size, int brightness)
{
int tempx,tempy;

// plot five circles, three further size, two close size.

//if(Mode320==0)
//  {
//  size*=2;
//  }
  
// close size circle, further from center than effect location
tempx=x-screen_x_center; tempy=y-screen_y_center;
tempx=tempx*1.3; tempy=tempy*1.3;
tempx=tempx+screen_x_center; tempy=tempy+screen_y_center;
fcircle(tempx,tempy,size/3,231,MODE_TRANSPARENT);

// close size circle, closer to center than effect location
tempx=x-screen_x_center; tempy=y-screen_y_center;
tempx=tempx*0.75; tempy=tempy*0.75;
tempx=tempx+screen_x_center; tempy=tempy+screen_y_center;
fcircle(tempx,tempy,size/4,141,MODE_TRANSPARENT);

// further size circles
tempx=x-screen_x_center; tempy=y-screen_y_center;
tempx=tempx*1.4; tempy=tempy*1.4;
tempx=screen_x_center-tempx; tempy=screen_y_center-tempy;
fcircle(tempx,tempy,size/3,170,MODE_TRANSPARENT);

tempx=x-screen_x_center; tempy=y-screen_y_center;
tempx=tempx*0.4; tempy=tempy*0.4;
tempx=screen_x_center-tempx; tempy=screen_y_center-tempy;
fcircle(tempx,tempy,size/4,137,MODE_TRANSPARENT);

tempx=x-screen_x_center; tempy=y-screen_y_center;
tempx=tempx*0.5; tempy=tempy*0.5;
tempx=screen_x_center-tempx; tempy=screen_y_center-tempy;
fcircle(tempx,tempy,size/5,227,MODE_TRANSPARENT);

// center circle
tempx=x; tempy=y;
fcircle(tempx,tempy,size/2,0,MODE_TRANSPARENT);

}

module_private void fcircle_ogl(int x, int y, int r, unsigned char colour, int mode);
//module_private void fcircle_sw(int x, int y, int r, unsigned char colour, int mode);
module_private void strange_ogl_poly(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int mode);
void convert_to_rgb(int colour, int *r, int *g, int *b);

void fcircle(int x, int y, int r, unsigned char colour, int mode)
{
  fcircle_ogl(x,y,r,colour,mode);
}

#define NUM_SIDES 20
 
void fcircle_ogl(int x, int y, int r, unsigned char colour, int mode)
{
int angle,angle_incr;
int last_x,last_y;
int curr_x,curr_y;
int red,green,blue;

convert_to_rgb(colour, &red, &green, &blue);


red<<=8; green<<=8; blue<<=8;

angle=0;		
last_x=x+r; last_y=y;
angle_incr=360/NUM_SIDES;

while(angle<360)
  {
  curr_x=(r*COS(angle))+x;
  curr_y=(r*SIN(angle))+y;

  strange_ogl_poly(x,y,last_x,last_y,curr_x,curr_y,red,green,blue,mode);

  angle+=angle_incr;
  last_x=curr_x; last_y=curr_y;
  }

// now draw last poly...
curr_x=x+r; curr_y=y;		        	// start position
strange_ogl_poly(x,y,last_x,last_y,curr_x,curr_y,red,green,blue,mode);

}





void strange_ogl_poly(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int mode)
{
#if OGL==1

dcoord *point_list;
dcoord points[10];
dcoord temp_points[10];
dcoord output_pointsc[30];
int polys;


point_list=points;
point_list->x=x1; point_list->y=y1; point_list->u=20000; point_list++;
point_list->x=x2; point_list->y=y2; point_list->u=20000; point_list++;
point_list->x=x3; point_list->y=y3; point_list->u=20000; point_list++;

distort_window_clipper3(points,temp_points,output_pointsc,&polys);
point_list=output_pointsc;

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
          //fop->x-=screen_cent_x;
          //fop->x = fop->x/screen_cent_x;

          fop->y=op->y;
          //fop->y=monitor_h-op->y;
          //fop->y-=screen_cent_y;
          //fop->y = fop->y/screen_cent_y;
          
          fop->u=op->u;	//lighting
//          fop->u=fop->u;
//          fop->v=op->v;
          op++;
          fop++;
          }
//       polygon(output_lines,(coord *)outpoints,fpoly_colour,screenaddr,screenwidth);
       OGL_Gpolygon(3,(fdcoord *)ftoutput_points,r,g,b, mode);
  
  point_list+=3;
  polys--;
  }
#endif
}

// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: fcircle
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 10/4/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Draws a circle on the screen
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	: 
// |              
// |              
// |
// | PARAM OUT	:
// |
// | ASSUMPTIONS:
// | LIMITATIONS: 1. It is clipped with the screen edges BUT this is a slow clipping method.
// |              2. Requires fpoly for the pline routine.
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------

#if 0
void fcircle_sw(int x, int y, int r, unsigned char colour, int mode)
{
int ytop,ybot,xstart,xend;
int ypos;
int accumulator,increment;
circle_type width;
extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
char *line_screen_address;

ytop=y-r;  ybot=y+r;  						// top and bottom of circle (line y)

if(ybot<screen_y_min) return;				// quit if the circle is off the top

increment=I_TO_FIX(CIRCLE_ENTRIES)/(r*2);	// increment between lines in the circle table
accumulator=0;							// accumulator of the lines to access the circle table

line_screen_address = screenaddr+(screenwidth*ytop);	// calculate the physical screen address of the top line of the circle  

if(mode==0)
  {
  for(ypos=ytop; ypos<ybot; ypos++)		  	  // go from the top line to "one below" the bottom, since r=2, ytop=-1, ybot=0 NOT ybot=1!
    {
    if(ypos>screen_y_max) break; 		  	  	  // skip if off screen to the bottom
    if(ypos>=screen_y_min) 		  	  	  	  // skip this line if off the top
      {
      width=*(circle_table_ptr+FIX_TO_I(accumulator));  // get value out of circle table
      width*=r;								// scale returned value
    
      xstart=x-width; xend=x+width;
											// clipping for x at each line
      if(xstart<screen_x_min)					// if start is less than the minimum, make it the minimum
        { xstart=screen_x_min; }
      if(xend>screen_x_max)					// if end is greater than the maximum, make it the maximum
        { xend=screen_x_max; }
													// only print if the line is on screen
      if(xstart<=screen_x_max && xend>=screen_x_min)	// start must be before the end of the line, and the end before the start!
        {
        pline(xstart, xend, line_screen_address, colour);
        }
      } /* end of if */

    line_screen_address+=screenwidth;
    accumulator+=increment;

    } /* end of for */
  }
else
  {
  for(ypos=ytop; ypos<ybot; ypos++)		  	  // go from the top line to "one below" the bottom, since r=2, ytop=-1, ybot=0 NOT ybot=1!
    {
    if(ypos>screen_y_max) break; 		  	  	  // skip if off screen to the bottom
    if(ypos>=screen_y_min) 		  	  	  	  // skip this line if off the top
      {
      width=*(circle_table_ptr+FIX_TO_I(accumulator));  // get value out of circle table
      width*=r;								// scale returned value
    
      xstart=x-width; xend=x+width;
											// clipping for x at each line
      if(xstart<screen_x_min)					// if start is less than the minimum, make it the minimum
        { xstart=screen_x_min; }
      if(xend>screen_x_max)					// if end is greater than the maximum, make it the maximum
        { xend=screen_x_max; }
													// only print if the line is on screen
      if(xstart<=screen_x_max && xend>=screen_x_min)	// start must be before the end of the line, and the end before the start!
        {
        tpline(xstart, xend, line_screen_address, colour);
        }
      } /* end of if */
    
    line_screen_address+=screenwidth;
    accumulator+=increment;

    } /* end of for */

  } /* end for mode if */

} /* end of fcircle */
#endif //0


// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: fcircle_nofill
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 10/4/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Draws a circle on the screen
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	: 
// |              
// |              
// |
// | PARAM OUT	:
// |
// | ASSUMPTIONS:
// | LIMITATIONS: 1. It is clipped with the screen edges BUT this is a slow clipping method.
// |              2. Requires fpoly for the pline routine.
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------


void fcircle_nofill_noclip(int x, int y, int r, unsigned char colour)
{
int ytop,ybot,xstart,xend;
int ypos;
int accumulator,increment;
circle_type width;
//extern Ptr screenaddr;	//pointer to current drawing buffer
//extern int screenwidth;
//char *line_screen_address;



ytop=y-r;  ybot=y+r;  						// top and bottom of circle (line y)

//if(ybot<screen_y_min) return;		  	  	  // quit if the circle is off the top

increment=I_TO_FIX(CIRCLE_ENTRIES)/(r*2);	// increment between lines in the circle table
accumulator=0;							// accumulator of the lines to access the circle table

//line_screen_address = screenaddr+(screenwidth*ytop);	  // calculate the physical screen address of the top line of the circle  

  for(ypos=ytop; ypos<ybot; ypos++)		  	  // go from the top line to "one below" the bottom, since r=2, ytop=-1, ybot=0 NOT ybot=1!
    {
//    if(ypos>screen_y_max) break; 		    	    	    // skip if off screen to the bottom
//    if(ypos>=screen_y_min) 	  	    	    	    	    // skip this line if off the top
//      {
      width=*(circle_table_ptr+FIX_TO_I(accumulator));  // get value out of circle table
      width*=r;								// scale returned value
    
      xstart=x-width; xend=x+width;
											// clipping for x at each line
      width=*(circle_table_ptr+FIX_TO_I(accumulator));  // get value out of circle table
      width*=r;								// scale returned value
    
      xstart=x-width; xend=x+width;
											// clipping for x at each line
	OGL_plot(xstart,ypos,colour+1);
	OGL_plot(xstart+1,ypos,colour);
	OGL_plot(xstart+2,ypos,colour+1);

	OGL_plot(xend,ypos,colour+1);
	OGL_plot(xend+1,ypos,colour);
	OGL_plot(xend+2,ypos,colour+1);
	
    accumulator+=increment;

    } /* end of for */

} /* end of fcircle */


// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: fcircle_nofill
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 10/4/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Draws a circle on the screen
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	: 
// |              
// |              
// |
// | PARAM OUT	:
// |
// | ASSUMPTIONS:
// | LIMITATIONS: 1. It is clipped with the screen edges BUT this is a slow clipping method.
// |              2. Requires fpoly for the pline routine.
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------


void felipse_nofill_noclip(int x, int y, int w, int h, unsigned char colour)
{
int ytop,ybot,xstart,xend;
int ypos;
int accumulator,increment;
circle_type width;
//extern Ptr screenaddr;	//pointer to current drawing buffer
//extern int screenwidth;
//char *line_screen_address;
//Ptr screen_x,screen_x1;
int red,green,blue;
float temp_r1,temp_b1,temp_g1;
float temp_r2,temp_b2,temp_g2;


ytop=y-(h/2);  ybot=y+(h/2);  						// top and bottom of circle (line y)

//if(ybot<screen_y_min) return;		  	  	  // quit if the circle is off the top

increment=I_TO_FIX(CIRCLE_ENTRIES)/(h);	// increment between lines in the circle table
accumulator=0;							// accumulator of the lines to access the circle table

//line_screen_address = screenaddr+((screenwidth*ytop));	// calculate the physical screen address of the top line of the circle  
        glBegin(GL_POINTS);
        
  for(ypos=ytop; ypos<=ybot; ypos++)	 	   	// go from the top line to "one below" the bottom, since r=2, ytop=-1, ybot=0 NOT ybot=1!
    {
    //if(ypos>screen_y_max) break; 		    	    	    // skip if off screen to the bottom
//    if(ypos>=screen_y_min) 	  	    	    	    	    // skip this line if off the top
//      {
      width=*(circle_table_ptr+FIX_TO_I(accumulator));  // get value out of circle table
      width*=w/2;								// scale returned value
    
      xstart=x-width; xend=x+width;
	
     convert_to_rgb(colour, &red, &green, &blue);
     temp_r1=red;
     temp_g1=green;
     temp_b1=blue;
     temp_r1=temp_r1/255;	//0-1
     temp_g1=temp_g1/255;	//0-1
     temp_b1=temp_b1/255;	//0-1
	
     convert_to_rgb(colour+1, &red, &green, &blue);
     temp_r2=red;
     temp_g2=green;
     temp_b2=blue;
     temp_r2=temp_r2/255;	//0-1
     temp_g2=temp_g2/255;	//0-1
     temp_b2=temp_b2/255;	//0-1
	
											// clipping for x at each line
	OGL_plot_rgb_quick(xstart,ypos,temp_r2,temp_g2,temp_b2);
	OGL_plot_rgb_quick(xstart+1,ypos,temp_r1,temp_g1,temp_b1);
	OGL_plot_rgb_quick(xstart+2,ypos,temp_r2,temp_g2,temp_b2);

	OGL_plot_rgb_quick(xend,ypos,temp_r2,temp_g2,temp_b2);
	OGL_plot_rgb_quick(xend+1,ypos,temp_r1,temp_g1,temp_b1);
	OGL_plot_rgb_quick(xend+2,ypos,temp_r2,temp_g2,temp_b2);
//        screen_x=line_screen_address+xstart;
//        screen_x1=line_screen_address+xend;

//        aliased_v_pixel(screen_x,colour);
//        aliased_v_pixel(screen_x1,colour);
         
     

//    line_screen_address+=screenwidth;
    accumulator+=increment;

    } /* end of for */
        glEnd();

} /* end of fcircle */


//void aliased_v_pixel(Ptr screen_x, unsigned char colour)
//{
//        *screen_x=colour+1;
//	  screen_x++;
//        *screen_x=colour;
//	  screen_x++;
//        *screen_x=colour+1;
//}

// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: init_circle
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 10/4/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Allocates memory for, and init the data table required for ftable
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



void init_circle(void)
{
int element;
circle_type *ctp_copy;
Handle temp_h;

float fractional_element;



// init 2048 bytes of memory, pointed to by circle_table_ptr

temp_h=NewHandleClear(CIRCLE_ENTRIES*sizeof(circle_type));
if (temp_h!=0)
	{ 
	HLock(temp_h);
	circle_table_ptr=(float *)*temp_h;
	}
else
	report_error("Not enough memory (init_ocbs:init_circle).","\p",3);


// fill array with values

ctp_copy=circle_table_ptr;


for(element=0; element<CIRCLE_ENTRIES; element++)
  {
  fractional_element = ((float)element) - (( ((float)CIRCLE_ENTRIES) -1)/2);	// after this ranges from -511.5 to 511.5 in steps of 1
  fractional_element /=  (( ((float)CIRCLE_ENTRIES) -1)/2);			// after this ranges from -1 to 1
  
  *ctp_copy=squareroot(1 - fractional_element*fractional_element);		// n.b. 1*1=1
  ctp_copy++;

  }
}


// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: squareroot
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 10/4/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Does a squareroot, stupid.
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

float squareroot( float R)
{
float x_1,x_0;
int count;

x_0=0.2;		// starting position chosen at random (-ish)

// magic magic oh my god, magic.
for(count=0; count<8; count++) 
  { 
    x_1=0.5*(x_0 + R/x_0);
    x_0=x_1;
  }

return x_1;
}



// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: tpline
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 10/4/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: Like fpoly pline, but produces a transparent line
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	: 
// |              
// |              
// |
// | PARAM OUT	:
// |
// | ASSUMPTIONS: Does not check for x1>x2
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------
#if 0
void tpline(int x1, int x2, char *line_screen_address, unsigned char colour)
{
int width;

MyDebugStr(__LINE__,"fcircle.c","tpline");

width=x2-x1;

line_screen_address+=x1;

while(width>0)
  {
  *line_screen_address = transparency_table[((*line_screen_address)<<8)+colour];
  line_screen_address++;
  width--;
  }
}
#endif //0