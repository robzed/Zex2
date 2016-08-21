/***********************************************************************
* FPOLY4_1.cpp
*
* FANTASY 		POLYGON DRAWER	      	      	      
* VERSION:      	M003.004		
* PROGRAMMER:   	ROBERT PROBIN		
* LAST MODIFIED: 	29/10/98
* PREVIOUS LAST MODIFIED:	 26/09/94
* UPDATES:				
* V10:      FROM ROB AS PLANE PRINT DATE 3/4/92		
* M001.000  MODIFIED BY S.B. TO INTERFACE WITH PAINTIT.	
*           NEEDS I/P ARRAY AS X/Y.W IN DRAW_XY, WITH COORD COUNT IN  
*           NUMBER_LINES AND THE COLOUR OF THE POLY IN POLY_COL.      
* M001.001  MODIFIED CLIPPING TO WORK OF POLY ABOVE SCREEN, OR POLY   
*           ONLY 1 LINE HIGH.			
* M003.003  Turboing again, after the massive window clipping thing.  
*           SUMMARY: A lot of changes, mainly to do with the way the  
*           line drawer works. See notes below.                       
*           4th Dec 93 RP   
* M003.004  Adjusted to use new 32 bit cook_table, plus 256 colour mode
*           on the mac. Structuring change. In order to make it easier
*   for me to find my way around this large subroutine, I've split
*    the main flow up into several subroutines. This probably gives
*	a 2 to 4 ms overhead on a 8MHz 68K running 100 polygons, and as 
*           this is meant to be run on a faster processor I decided this was
*   		acceptable. - RP
*
* 15/9/94:  11:20pm. Ek! Found another two bugs in here. Both in find_initial. They
*           could have helped be the reason for previous crashes. I feel really glad
*           I broke up the main stream into subroutines. It means I can comb thro
*           each one, bit by bit. I've written a (very) full description in find_initial
*			so that I can understand it in the future. Oh, BTW> the bugs were, a)
*			the count was off by 1 (again!) it should have been subtract 2 from the
*			count rather than subtract one, since there is one for the dbra and also
*			one for the fact we have already taken a point to use as initial data.
*			b) the bge, blt, bgt, ble were all muddled up. Serves me right for
*           not commenting it 10000% - RP
*
* 20/9/94	After getting the routine working on the mac, A revision of register
*           usage has been done.
*
* 26/9/94	Corrected a possible bugget - add.w to a0 (only possible)
*
* 10/29/98	Four years later and I'm converting to C. And the last bugget wasn't.
* 
* 11/28/98      I haven't converted it, but re-written it in it's simplest form. We'll need to optimise later
*
* 02/12/98      SB: Added to Zex proj tree. Enabled by defining f_poly in LS_Standard.h
*
* 03/12/98	Added window clipping
*
* 18/12/98      SB: Made clip rect dymaic, setup by calling set_poly_clipping

***********************************************************************
 * $Log: FPOLY4_1.cpp,v $
 * Revision 1.2  2003/09/20 12:57:03  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:36:28  stu_c
 * First Imported.
 *
 * Revision 1.5  2002/09/08 02:07:34  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.4  2001/12/29 01:10:59  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.3  2001/11/09 18:28:48  rob
 * Various debug changes.
 *
 * Revision 1.2  2001/11/02 19:31:44  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.1  2001/10/21 01:04:40  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:07  rob
 * First Imported.
 *
 * Revision 1.1.1.1  2001/01/01 21:13:39  rob
 * First Imported.
 *
 * Revision 1.0.0.1  2000/08/21 22:09:47  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "ls_standard.h"

  #include "Error_handler.h"
#endif



#include "fpoly4.h"
#include "genpoly.h"

#define INPUT_CHECKING	1

    #if INPUT_CHECKING

      void check_all_point_boundaries(int num_lines, coord *points);
    #endif


// internal functions
void clear_draw_area(int ymin,int ymax);
void plot(int current_x,int *my_draw_area);
void draw(int x1,int y1, int x2,int y2);
void plot(int current_x,int *my_draw_area);
void pline(int x1, int x2, char *line_screen_address, int colour);
void draw_one_high_poly(coord *points, int num_lines, char *line_screen_address, int colour);
int find_max_y(coord *point_list, int num_lines);
int find_min_y(coord *point_list, int num_lines);

int *draw_area;
//int *reciprocals;

/* ================================================================== */

void polygon(int num_lines, coord *point_list, int colour, char *screen_address, int screen_width)
{
int min_y,max_y;
int n;
int first_y,first_x;
int last_y,last_x;
int *my_draw_area;
int small_x,large_x;
char *line_screen_address;

if(num_lines<3) 
  { 

  #if INPUT_CHECKING    
  MyDebugStr(__LINE__,"FPOLY4_1.c","Not a poly - insufficent lines"); 
  #endif
  
  return;
  }

  #if INPUT_CHECKING    
    check_all_point_boundaries(num_lines,point_list);
  #endif


colour&=255;


max_y=find_max_y(point_list, num_lines);
min_y=find_min_y(point_list, num_lines);

clear_draw_area(min_y,max_y);

// check for a special case of a one high polygon
if(max_y==min_y)  
  {
  line_screen_address=screen_address+(screen_width*min_y);
  draw_one_high_poly(point_list, num_lines, line_screen_address, colour);
  }

// draw all the lines into the buffer
first_x=point_list->x;
first_y=point_list->y;

for(n=1; n<num_lines; n++)
  {
  last_x=point_list->x;
  last_y=point_list->y;
  
  point_list++;
  
  draw(last_x,last_y,point_list->x,point_list->y);
  }

draw(point_list->x, point_list->y, first_x, first_y);


// *****SPLAT all the lines to screen*******

line_screen_address=screen_address+(screen_width*min_y);

my_draw_area=draw_area;
my_draw_area += (min_y<<1);	// displace down to area relevant to this line


for(n=min_y; n<=max_y; n++)
  {
  small_x=*my_draw_area;
  my_draw_area++;
  large_x=*my_draw_area;
  my_draw_area++;

  pline(small_x, large_x, line_screen_address, colour);

  line_screen_address+=screen_width;

  }


}
// developer.apple.com/dev/games/index.html
/************************************************************************/

#if INPUT_CHECKING
void check_all_point_boundaries(int num_lines, coord *points)
{
int a;

  for(a=0; a<num_lines; a++)
    {
	if(points->x>screen_x_max || points->x<screen_x_min ||
	   points->y>screen_y_max || points->y<screen_y_min)
	   {
	   MyDebugStr(__LINE__,"FPOLY4_1.c","Poly bigger than screen");
	   }
	   points++;
	}
}
#endif



void draw_one_high_poly(coord *point_list, int num_lines, char *line_screen_address, int colour)
{
int min_x,max_x;
int current_x;
int n;

min_x=screen_x_max+1;
max_x=screen_x_min-1;

for(n=0; n<num_lines;n++)
  {
  current_x=point_list->x;
  
  if(current_x>max_x) max_x=current_x;
  if(current_x<min_x) min_x=current_x;
  point_list++;
  }

pline(min_x, max_x, line_screen_address, colour);

}




int find_max_y(coord *point_list, int num_lines)
{
int max_y;
int current_y;
int n;

max_y=screen_y_min-1;

for(n=0; n<num_lines;n++)
  {
  current_y=point_list->y;
  
  if(current_y>max_y) max_y=current_y;
  point_list++;
  }
  
return max_y;
}


int find_min_y(coord *point_list, int num_lines)
{
int min_y;
int current_y;
int n;

min_y=screen_y_max+1;

for(n=0; n<num_lines;n++)
  {
  current_y=point_list->y;
  
  if(current_y<min_y) min_y=current_y;
  point_list++;
  }
  
return min_y;
}




// notice so we don't have to clear draw area, we need to record the 
void clear_draw_area(int ymin,int ymax)
{
int n;
int *my_draw_area;

my_draw_area=draw_area+(2*ymin);

for(n=ymin; n<=ymax; n++)
  {
  *my_draw_area=screen_x_max+1;		// minimum
  my_draw_area++;
  
  *my_draw_area=screen_x_min-1;		// maximum
  my_draw_area++;
  
  }
  
}


// we need to draw into draw area
void draw(int x1,int y1, int x2,int y2)
{
int *my_draw_area;
int temp;
int x_width;
int y_width;
int x_dir;
int error;

my_draw_area=draw_area;

if(y2<y1)			// always want to draw down
  {
  temp=y1;  y1=y2;  y2=temp;
  temp=x1;  x1=x2;  x2=temp;
  }

my_draw_area += (y1<<1);	// displace down to area relevant to this line
y2-=y1;				// then disgard this displacement from coordinates
y_width=y2;

x_width=x2-x1;			// may be negative, since lines may go from right to left

if(x_width<0)
  {
  x_width=-x_width;
  x_dir=-1;
  }
else
  {
  x_dir=1;
  }


//if(x_width==0)		// check for vertical line special case
//  {
//  }
  
if(y_width==0)		// check for horizontal line special case
  {
  plot(x1,my_draw_area);
  plot(x1,my_draw_area);
  return;
  }


//if(y_width==x_width)		// check for diagonal line special case
//  {
//  }

  
if(y_width>x_width)		// then its a mostly (nearly?) vertical line  (so x steps can be in addition to y steps)
  {
  error=-(y_width>>1);		// ensure even start and stop to line
  
  while(y2>0 || x1!=x2)
    {
    //plot a point
    plot(x1,my_draw_area);
    
    y2--;
    error+=x_width;
    my_draw_area+=2;

    if(error>0)
      {
      x1+=x_dir;
      error-=y_width;
      }
      
    }  // while not at endpoint

  plot(x1, my_draw_area);

  }
else			// else its a mostly (nearly?) horizontal line (so y steps can be in addition to x steps)
  {
  error=x_width>>1;

  while(y2>0 || x1!=x2)
    {
    //plot a point
    plot(x1,my_draw_area);
    
    x1+=x_dir;
    error-=y_width;

    if(error<=0)
      {
      y2--;
      error+=x_width;
      my_draw_area+=2;
      }

           
    }  // while not at endpoint

  plot(x1, my_draw_area);

  }  // vertical or horizontal line selection

}


void plot(int current_x,int *my_draw_area)
{
if(current_x<*my_draw_area)
  {
  *my_draw_area=current_x;
  }

my_draw_area++;

if(current_x>*my_draw_area)
  {
  *my_draw_area=current_x;
  }
  
// my_draw_area--;
}


void pline(int x1, int x2, char *line_screen_address, int colour)
{
int long_colour;
int last_run;
int first_run;
int mid_run;

/* dont do anything if negative length */
if(x1>x2) { return; }

/* produce a 32-bit colour value */
long_colour=colour+(colour<<8)+(colour<<16)+(colour<<24);

/* calculate the lengths of the various sections */
first_run = 4 - (x1 & 0x03);

last_run = x2 & 0x03;

mid_run = (x2 - x1 - first_run - last_run) >> 2;	/* longs */

last_run++;

/* set the start address of the line */
line_screen_address+=x1;

/* now the actual splat */
if(mid_run<0)
  {
  mid_run=x2-x1+1;
  while(mid_run>0)
    {
	*line_screen_address=colour;
	line_screen_address++;
	mid_run--;
	}
  }
else
  {
  /* splat first bytes */
  while(first_run>0)
    {
	*line_screen_address=colour;
	line_screen_address++;
	first_run--;
	}
	
  /* splat longs */
  while(mid_run>0)
    {
	*(long *)line_screen_address=long_colour;
	((long *)line_screen_address)++;
	mid_run--;
	}
	
  /* finish of with bytes */
  while(last_run>0)
    {
	*line_screen_address=colour;
	line_screen_address++;
	last_run--;
	}
  
  }
  
} /* end of pline */



int temp_space[2][1000];

// init_polygon needs to be called once only, and we cannot change screen area...
// 			(create a kill_polygon() if we want to change screen rez).
void init_polygon(void)
{
//int max_size;
int max_height;

//if ((SCREEN_X_MAX-SCREEN_X_MIN) > (SCREEN_Y_MAX-SCREEN_Y_MIN))
//  {
//  max_size=SCREEN_X_MAX-SCREEN_X_MIN;
//  }
//else
//  {
//  max_size=SCREEN_Y_MAX-SCREEN_Y_MIN;
//  }
//
//max_size++

// create memory partition of size "max_height" so that we can create reciprocals array


max_height=screen_y_max-screen_y_min+1;

// create 2 memory areas ==> longs * max_height
draw_area=(int *)temp_space;

}


























