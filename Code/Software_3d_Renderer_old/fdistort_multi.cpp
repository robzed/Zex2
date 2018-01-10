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

// Robs distort routine, usually used for afine texture mapping.
//
// Multisided version, for transparent and non-transparent texture onto polygons.
//
// All int's must be 32 bit.
//
//
//
//
// Version 1.02 - RP - Wed 6/1/99 - Fixed slightly off horizontal distorting
// Version 1.03 - RP - Mon 11/1/99 - Created light values in distort at request of Stu. Thanks Stu. (Welcome).
// (1.04 was mask version)
// Version 1.05 - RP - Wed 29/9/99 - Kind Stu wants multisided transparent polygons, so that his gas clouds
//			            don't break up when being triangularizied at the edge of the screen. Thanks Stu.
//
// Note: This version is all light shaded textures. Don't mix with project that have an fdistort that is defined
// with no light shaded textures, or else!
//
// Note2: Concave polygons only please. (Hopefully it won't crash if not...) 
//        Also please ensure points are anti-clockwise.
//
// Note3: This multisided version of distort, although all textures are lit to a certain level (was actually to simplify
//        the number of lines when coding - it would be trival to back convert), will also do non-transparent multisided.
//
/* $Log: fdistort_multi.c,v $
/* Revision 1.0.0.1  2001/10/17 20:46:08  rob
/* First Imported.
/*
/* Revision 1.1.1.1  2001/01/01 21:13:39  rob
/* First Imported.
/*
/* Revision 1.1  2000/09/04 20:04:58  rob
/* Hopefully fixed little black line problem with software renderer.
/*
/* Revision 1.0.0.1  2000/08/21 22:09:34  stu
/* First Imported.
/*
 */

#include "platform.h"

#include "f_light.int"
#include "fpoly4.ext"
#include "genpoly.int"
#include "fdistort.ext"

#define WIDTH_FIXED_AT_128 1


extern border_type tex_startspace[1000];	// 1000 of line start, bmpstartx, bmpstarty
extern border_type tex_endspace[1000];		// 1000 of line end, bmpendx, bmpendy


//extern void tex_draw(border_type *data_dest,int x1,int y1,int x2,int y2, int from_a, int from_b, int to_a, int to_b);

extern void scanoneline_t(char *screen_line_address, int line_startx, int line_endx,
	  char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width, char *light_colour_address);



void distortm(char *saddr, int swidth, dcoord *point_list, char *texture_addr, int texture_width, int light_value, int transparent, int num_connections)
{
int x1,x2,y1,y2;
int u1,u2,v1,v2;
border_type *starttemp;
border_type *endtemp;
//int count;
int ymin,ymax;
int xmin,xmax,xminu,xmaxu,xminv,xmaxv;
dcoord *first_point_list;

char *light_address;

if(num_connections<3) { return; }  // get the hell out of here if a 0 1 or 2 sided polygon!

// first connection
x1=point_list->x; y1=point_list->y; u1=point_list->u; v1=point_list->v;
ymin=y1; ymax=y1;
xmin=x1; xmax=x1; xminu=point_list->u; xmaxu=point_list->u; xminv=point_list->v; xmaxv=point_list->v;


first_point_list=point_list;

do
  {
  if(num_connections!=1) 	// last point is first point
    {
    point_list++; 
    }
  else 
    {
    point_list=first_point_list;
    }
  
  x2=point_list->x; y2=point_list->y; u2=point_list->u; v2=point_list->v;
  
  if(y1==y2)       // horizontal line - either at top or bottom
      {		  
        if(x2<xmin)
          { xmin=x2; xminu=u2; xminv=v2; }
        if(x2>xmax)
          { xmax=x2; xmaxu=u2; xmaxv=v2; }
      }   	/* ignore connection - but store for single height case*/

//  else if(y1<y2)  // due to anticlockwise, must be left side  (going down - y getting greater)
//    {
//    starttemp=tex_startspace;
//    tex_draw(starttemp,x1,y1,x2,y2,u1,v1,u2,v2);
//    }
//  else           // y1>y2  - due to anticlockwise, must be right side (going up - y getting smaller)
//    { 
//    endtemp=tex_endspace;
//    tex_draw(endtemp,x2,y2,x1,y1,u2,v2,u1,v1);
//    }

  tex_draw2(tex_startspace,tex_endspace,x1,y1,x2,y2,u1,v1,u2,v2);



  // second connection becomes first connection    
  x1=x2; y1=y2; u1=u2; v1=v2;
  if(y1<ymin) { ymin=y1; }  if(y1>ymax) { ymax=y1; } 

  num_connections--;
  } while(num_connections!=0);


{		// for light shaded textures
int temp;
temp=(light_value>>(8+LIGHT256_SHIFT));
temp=(NUM_LIGHT_LEVELS/2);
light_address=light_table+(light_value>>(8+LIGHT256_SHIFT))+(NUM_LIGHT_LEVELS/2);
}

if(!transparent)
{
// now draw the individual scan lines.

 starttemp=tex_startspace + ymin;
 endtemp=tex_endspace + ymin;
 saddr+=(ymin*swidth);

 if(ymin==ymax)
   {
  scanoneline(saddr, xmin,xmax, texture_addr,xminu,xminv,xmaxu,xmaxv,texture_width,light_address);
   }

 while(ymin<=ymax)
  { // scanoneline(s,startx,  endx,    bmp_address,  bmp_startx,  bmp_starty,   bmp_endx,      bmp_endy,   bmp_width)
  scanoneline(saddr, starttemp->linex,endtemp->linex, texture_addr, starttemp->bmpx,starttemp->bmpy, endtemp->bmpx,endtemp->bmpy, texture_width,light_address);
  saddr+=swidth;
  
  starttemp++;
  endtemp++;
  
  ymin++;
  }
 
}
else // if tranparent
{
// now draw the individual scan lines.

 starttemp=tex_startspace + ymin;
 endtemp=tex_endspace + ymin;
 saddr+=(ymin*swidth);

 if(ymin==ymax)
   {
  scanoneline_t(saddr, xmin,xmax, texture_addr,xminu,xminv,xmaxu,xmaxv, texture_width,light_address);
   }

 while(ymin<=ymax)
  { // scanoneline(s,startx,  endx,    bmp_address,  bmp_startx,  bmp_starty,   bmp_endx,      bmp_endy,   bmp_width)
  scanoneline_t(saddr, starttemp->linex,endtemp->linex, texture_addr, starttemp->bmpx,starttemp->bmpy, endtemp->bmpx,endtemp->bmpy, texture_width,light_address);
  saddr+=swidth;
  
  starttemp++;
  endtemp++;
  
  ymin++;
  }
 
}


}  // end of distortm()




