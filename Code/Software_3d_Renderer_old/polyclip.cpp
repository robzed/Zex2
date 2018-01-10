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

// Multipurpose clipper
// ====================
//
// v 1.00 - RP - 31//12/98 Moved standard polyclip from fpoly, added texture
//               stuff.
//
/* $Log: polyclip.c,v $
/* Revision 1.0.0.1  2001/10/17 20:46:08  rob
/* First Imported.
/*
/* Revision 1.2  2001/05/28 01:50:32  stu
/* 280501
/*
/* Revision 1.1.1.1  2001/01/01 21:13:39  rob
/* First Imported.
/*
/* Revision 1.0.0.1  2000/08/21 22:09:57  stu
/* First Imported.
/*
 */

#include "platform.h"


#include "genpoly.int"
#include "fpoly4.ext"
#include "fdistort.ext"
#include <Types.h>

#include "3d_structures_def.h"

module_private void clip_edge(int num_lines, int edge, coord *input_list, coord *outlist, int *outlines);
module_private void distort_clip_edge(int num_lines, int edge, dcoord *input_list, dcoord *outlist, int *outlines);


//module level vars that define clipping rect for fpoly so we can dynamically alter it. Set with set_poly_clipping
int screen_x_min, screen_x_max, screen_y_min, screen_y_max;	//module vars for width and height - sb 181298

extern int screen_x_center,screen_y_center;

//¥¥set screen bounds
void set_poly_clipping (int x_min, int x_max, int y_min, int y_max)
{
screen_x_min=x_min;
screen_x_max=x_max;
screen_y_min=y_min;
screen_y_max=y_max;

// various junk init's

screen_x_center=(screen_x_max-screen_x_min)/2+screen_x_min;
screen_y_center=(screen_y_max-screen_y_min)/2+screen_y_min;


}


/*************************************************************************/
// for a three point polygon, temp poly must be 10 elements, and output_list must be 30 elements
void distort_window_clipper3(dcoord *input_list, dcoord *temp_list, dcoord *output_list,int *polys_out)
{
int out_lines,out_lines2;

*polys_out=0;

// first clip into a multisided polygon.

distort_clip_edge(3,1,input_list,output_list,&out_lines); 
if(out_lines<3) { return; }

distort_clip_edge(out_lines,2,output_list,temp_list,&out_lines2); 
if(out_lines2<3) { return; }

distort_clip_edge(out_lines2,3,temp_list,output_list,&out_lines); 
if(out_lines<3) { return; }

distort_clip_edge(out_lines,4,output_list,temp_list,&out_lines2); 
//if(out_lines2<3) { return; }


// now split into triangles

while(out_lines2>2)
  {
  output_list->x=temp_list->x;   output_list->y=temp_list->y;   output_list->u=temp_list->u;   output_list->v=temp_list->v; output_list++; temp_list++;
  output_list->x=temp_list->x;   output_list->y=temp_list->y;   output_list->u=temp_list->u;   output_list->v=temp_list->v; output_list++; temp_list++;
  output_list->x=temp_list->x;   output_list->y=temp_list->y;   output_list->u=temp_list->u;   output_list->v=temp_list->v;  output_list-=2; temp_list--;
  // temp_list at second of 3, output list at first of 3.
  
  // copy first point to second location in input array.
  temp_list->x=output_list->x;   temp_list->y=output_list->y;   temp_list->u=output_list->u;   temp_list->v=output_list->v;
  
  output_list+=3;  // output list is at new location, temp is at second point.

  out_lines2--;			// one less point
  (*polys_out)++;		// one more poly made it
  }
}

/*************************************************************************/
// for a three point polygon, temp poly must be 10 elements, and output_list must be 30 elements
// 
// Similar to distort_window_clipper3, except the resultant multifacted polygon is not split back down into further polygons.
//

void distort_window_clipper_m(dcoord *input_list, dcoord *temp_list, dcoord *output_list,int *points_out)
{
int out_lines,out_lines2;

*points_out=0;

// first clip into a multisided polygon.

distort_clip_edge(3,1,input_list,temp_list,&out_lines); 
if(out_lines<3) { return; }

distort_clip_edge(out_lines,2,temp_list,output_list,&out_lines2); 
if(out_lines2<3) { return; }

distort_clip_edge(out_lines2,3,output_list,temp_list,&out_lines); 
if(out_lines<3) { return; }

distort_clip_edge(out_lines,4,temp_list,output_list,&out_lines2); 
//if(out_lines2<3) { return; }

*points_out=out_lines2;

}



/*************************************************************************/
// for a three point polygon, the max temp_list and output list is 10 elements (7 actually, with some spare)
void window_clipper(int num_lines, coord *input_list, coord *temp_list, coord *output_list, int *output_lines)
{
int out_lines;

if(num_lines<2) 
  { 
  #if INPUT_CHECKING
  DebugStr("\pInsufficent lines - cannot clip"); 
  #endif
  }


clip_edge(num_lines,1,input_list,temp_list,&out_lines);
if(out_lines<2) { *output_lines=0; return; }

clip_edge(out_lines,2,temp_list,output_list,output_lines);
if((*output_lines)<2) { return; }

clip_edge(*output_lines,3,output_list,temp_list,&out_lines);
if(out_lines<2) { *output_lines=0; return; }

clip_edge(out_lines,4,temp_list,output_list,output_lines);

//check_all_point_boundaries(*output_lines,output_list);

}
/*************************************************************************/
int visible(int x, int y, int edge);
void clip_line(int x1,int y1,int x2,int y2,int edge,int *xout,int *yout);
void distort_clip_line(dcoord *start, dcoord *end, int edge, dcoord *out);


/*************************************************************************/
void clip_edge(int num_lines, int edge, coord *input_list, coord *outlist, int *outlines)
{
int x1,y1,x2,y2,vis1,vis2;
int xout,yout;
coord *inlist;

inlist=input_list;

*outlines=0;

x1=inlist->x; y1=inlist->y; inlist++;
vis1=visible(x1,y1,edge);

do 
   {
   if(num_lines!=1)
     { x2=inlist->x; y2=inlist->y; inlist++; } /* get the next point */
   else
     { x2=input_list->x; y2=input_list->y; } /* the start point! is the next point */

   vis2=visible(x2,y2,edge);

   if(vis1) /* is first point visible */
     {
     outlist->x=x1; outlist->y=y1; outlist++; (*outlines)++; /* log it then! */
     
     if(!vis2) /* if second point not visible */
       {
       clip_line(x1,y1,x2,y2,edge,&xout,&yout);
       outlist->x=xout; outlist->y=yout; outlist++; (*outlines)++;
       }
     }
   else /* first point not visible */
     {
     if(vis2)
       {
       clip_line(x1,y1,x2,y2,edge,&xout,&yout);
       outlist->x=xout; outlist->y=yout; outlist++; (*outlines)++;
       }
     /* else don't log anything if first and second points not visible */
     }
     
   x1=x2; y1=y2; vis1=vis2;	/* second point becomes the first, and around we go */
   num_lines--;

   } while(num_lines);
   
}
/*************************************************************************/
void distort_clip_edge(int num_lines, int edge, dcoord *start_of_input_list, dcoord *output_list, int *outlines)
{
int vis1,vis2;
//int xoutyout;
dcoord *current_point;
dcoord *previous_point;

current_point=start_of_input_list;
*outlines=0;

vis1=visible(current_point->x,current_point->y,edge);

do 
   {
   if(num_lines!=1)
     { previous_point=current_point; current_point++; } /* get the next point */
   else
     { previous_point=current_point; current_point=start_of_input_list; } /* the start point! is the next point */

   vis2=visible(current_point->x,current_point->y,edge);

   if(vis1) /* is first point visible */
     {
     output_list->x=previous_point->x; output_list->y=previous_point->y;
     output_list->u=previous_point->u; output_list->v=previous_point->v; 
     output_list++; (*outlines)++; /* log it then! */
     
     if(!vis2) /* if second point not visible */
       {
       distort_clip_line(previous_point,current_point,edge,output_list); output_list++; (*outlines)++;
       }
     }
   else /* first point not visible */
     {
     if(vis2)
       {
       distort_clip_line(previous_point,current_point,edge,output_list); output_list++; (*outlines)++;
       }
     /* else don't log anything if first and second points not visible */
     }
     
   vis1=vis2;	/* second point becomes the first, and around we go */
   num_lines--;

   } while(num_lines);
   
}
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
int visible(int x, int y, int edge)
{
switch(edge)
  {
  case 1: return x<=screen_x_max;
  case 2: return x>=screen_x_min;
  case 3: return y<=screen_y_max;
  case 4: return y>=screen_y_min;
  default: DebugStr("\pIllegal case in visible"); return 1;
  }
  
}

/*************************************************************************/
void clip_line(int x1,int y1,int x2,int y2,int edge,int *xout,int *yout)
{
int old_x1, old_y1;
int x_cut;

  switch(edge)
    {
    case 1: x_cut=screen_x_max; 
            break;
    case 2: x_cut=screen_x_min; 
            break; 
    case 3: x_cut=screen_y_max; 
            old_y1=y1; y1=x1; x1=old_y1; 
            old_y1=y2; y2=x2; x2=old_y1; 
            break; 
    case 4: x_cut=screen_y_min; 
            old_y1=y1; y1=x1; x1=old_y1; 
            old_y1=y2; y2=x2; x2=old_y1; 
            break;
    }

  x1 -= x_cut;
  x2 -= x_cut;
  
  if(x2 == 0) 
    {  
    if(edge<3)
      {
      *xout=x2+x_cut;   // x2,y2 is clip point
      *yout=y2; 
      }
    else
      {
      *xout=y2;		// y2,x2 is clip point    
      *yout=x2+x_cut; 
      }
    return;       
    } 


  old_x1=x2; old_y1=y2;		// stop initialswap if x1>0

  while(x1!=0)		// whilst x1 isn't clip point
    {
    while(x1<0) // whilst x1 is smaller than zero
       {
       old_x1=x1; old_y1=y1;
       y1+=y2; y1>>=1; 		// y1= (y1+y2)/2  
       x1+=x2; x1>>=1;		// x1= (x1+x2)/2
       }

    x2=old_x1; y2=old_y1;

    while(x1>0) // whilst x1 is larger than zero
       {
       old_x1=x1; old_y1=y1;
       y1+=y2; y1>>=1; 		// y1= (y1+y2)/2  
       x1+=x2; x1>>=1;		// x1= (x1+x2)/2
       } 
  
    x2=old_x1; y2=old_y1;
   }

  if(edge<3)
    {
    *xout=x1+x_cut; 
    *yout=y1;		// x1,y1 is clip point
    }
  else
    {
    *xout=y1;		// x1,y1 is clip point    
    *yout=x1+x_cut; 
    }
    
    
  return;
}



/*************************************************************************/
void distort_clip_line(dcoord *start, dcoord *end, int edge, dcoord *out)
{
int x1,y1,x2,y2;
int u1,v1,u2,v2;
float fraction_upline;
int cut_point;

  switch(edge)
    {
    case 1: cut_point=screen_x_max; break;
    case 2: cut_point=screen_x_min; break; 
    case 3: cut_point=screen_y_max; break; 
    case 4: cut_point=screen_y_min; break;
    }


x1=start->x; y1=start->y; u1=start->u; v1=start->v;
x2=end->x; y2=end->y; u2=end->u; v2=end->v;

if(edge<3)  // x axis clipping
  {
  if(x2==x1) { DebugStr("\pCannot clip vertical line!"); }

//  if(x2 == x_cut) 
//    {  
//    if(edge<3)
//      { *xout=x2; *yout=y2;  /* x2,y2 is clip point */ }
//    else
//      { *xout=y2;	*yout=x2;  /* y2,x2 is clip point */ }
//    return;       
//    } 

  fraction_upline=( (float)(cut_point-x1) / (float) (x2-x1) );

  x1=cut_point;
  y1+=(y2-y1)*fraction_upline;
  u1+=(u2-u1)*fraction_upline;
  v1+=(v2-v1)*fraction_upline;
  }
else  // y axis clipping
  {
  if(y2==y1) { DebugStr("\pCannot clip horizontal line!"); }

  fraction_upline=( (float) (cut_point-y1) / (float) (y2-y1) );

  x1+=(x2-x1)*fraction_upline;
  y1=cut_point;
  u1+=(u2-u1)*fraction_upline;
  v1+=(v2-v1)*fraction_upline;
  }

out->x=x1;
out->y=y1;
out->u=u1;
out->v=v1;

}

/*************************************************************************/
//
// General purpose line clip, for single lines.
// ============================================
// IN:  Pointers to the start and end coordinates of the lines
//
// OUT: 1=visible, 0=totally invisible
//
//
// NOTES:
// 
// I would normally base a line clip on the Cohen-Sutherland clipping algorithm... but
// as we don't have many lines to clip, and not a lot of time to develop code, I've 
// just used the above polygon methods, applied to a single line. 
//
// What this means in worst case is that you can get 4 clips when you should only get 2, 
// or 2 when a line is invisible before flagged as such.
// 
// RP 29/9/99

int single_line_clip (int *x1, int *y1, int *x2, int *y2)
{
int edge_sel;
int point1_vis, point2_vis;
int temp_x,temp_y;


for(edge_sel=1; edge_sel<=4; edge_sel++)
  {
  point1_vis=visible(*x1,*y1,edge_sel);
  point2_vis=visible(*x2,*y2,edge_sel);
  
  if(!point1_vis && !point2_vis)	// both points not visible, quit
    {
    return 0;  // quit
    } 
  else if(!point1_vis && point2_vis)		     // point2 is visible, point1 is not on this edge
    {
    clip_line(*x1,*y1,*x2,*y2,edge_sel,&temp_x,&temp_y);
    *x1=temp_x; *y1=temp_y;
    }
  else if(point1_vis && !point2_vis)		// point1 is visible, point2 is not on this edge
    {
    clip_line(*x1,*y1,*x2,*y2,edge_sel,&temp_x,&temp_y);
    *x2=temp_x; *y2=temp_y;
    }
  					// other case is both visible in this clip edge
  } // for edge_sel
return 1;	//better return something sensible...
}


