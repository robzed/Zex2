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

//poly_fill.c
#include "3d_structures_def.h"
#include "stu_poly.ext"

#define MIN(a,b) ((a<b)?(a):(b))
#define MAX(a,b) ((a>b)?(a):(b))
#define MaxPoint(a,b) ((a.y > b.y) ? a : b)
#define MinPoint(a,b) ((b.y > a.y) ? a : b)

#define MaxPoint3(a,b,c) MaxPoint(MaxPoint(a,b),MaxPoint(b,c))
#define MidPoint3(a,b,c) MaxPoint(MinPoint(a,b),MinPoint(a,c))
#define MinPoint3(a,b,c) MinPoint(MinPoint(a,b),MinPoint(b,c))


void POLY_Triangle(_2D p1,_2D p2,_2D p3, RGBColor the_Colour)
{
   _2D p1d,p2d,p3d;
   int xd1,yd1,xd2,yd2,i;
   int Lx,Rx;
   int top,left,bottom,right,min,max;

	extern	Polygon **Polygon_handle;
	extern	Polygon *Polygon_ptr;

//   Polygon *the_poly;
//get bounding rect for poly
	min=0;
	min=p1.y;
	if (p2.y<min) min=p2.y;
	if (p3.y<min) min=p3.y;
	top=min;

	min=0;
	min=p1.x;
	if (p2.x<min) min=p2.x;
	if (p3.x<min) min=p3.x;
	left=min;
	
	max=0;
	max=p1.y;
	if (p2.y>max) max=p2.y;
	if (p3.y>max) max=p3.y;
	bottom=max;

	max=0;
	max=p1.x;
	if (p2.x>max) max=p2.x;
	if (p3.x>max) max=p3.x;
	right=max;
	
	(*Polygon_ptr).polyBBox.top=top;
	(*Polygon_ptr).polyBBox.left=left;
	(*Polygon_ptr).polyBBox.bottom=bottom;
	(*Polygon_ptr).polyBBox.right=right;
    
	(*Polygon_ptr).polyPoints[0].h=p1.x;
	(*Polygon_ptr).polyPoints[0].v=p1.y;

	(*Polygon_ptr).polyPoints[1].h=p2.x;
	(*Polygon_ptr).polyPoints[1].v=p2.y;

	(*Polygon_ptr).polyPoints[2].h=p3.x;
	(*Polygon_ptr).polyPoints[2].v=p3.y;
	(*Polygon_ptr).polySize=10+12;
	PaintPoly(Polygon_handle);
}
/*
   MoveTo (p1.x,p1.y);
   
   p1d = MinPoint3(p1,p2,p3);
   p2d = MidPoint3(p2,p3,p1);
   p3d = MaxPoint3(p3,p1,p2);


   if(p2.y < p1.y)
   {
      p1d=MinPoint3(p2,p1,p3);
      p2d=MidPoint3(p1,p3,p2);
   }


   xd1=p2d.x-p1d.x;
   yd1=p2d.y-p1d.y;
   xd2=p3d.x-p1d.x;
   yd2=p3d.y-p1d.y;


   if(yd1)
      for(i=p1d.y; i<=p2d.y; i++)
      {


          Lx = p1d.x + ((i - p1d.y) * xd1) / yd1;
          Rx = p1d.x + ((i - p1d.y) * xd2) / yd2;


          if(Lx!=Rx)
//             VID_HLine(MIN(Lx,Rx),MAX(Lx,Rx),i);
  				MoveTo (MIN(Lx,Rx),i);
  				LineTo (MAX(Lx,Rx),i);
  
      }


   xd1=p3d.x-p2d.x;
   yd1=p3d.y-p2d.y;

   if(yd1)
      for(i   = p2d.y;   i <= p3d.y;   i++)
      {
         Lx =   p1d.x   + ((i   - p1d.y)   * xd2) / yd2;
         Rx =   p2d.x   + ((i   - p2d.y)   * xd1) / yd1;
         if(Lx!=Rx)
//            VID_HLine(MIN(Lx,Rx),MAX(Lx,Rx),i,c);
  				MoveTo (MIN(Lx,Rx),i);
  				LineTo (MAX(Lx,Rx),i);

      }
}
*/
/*
void VID_HLine(int x1, int x2, int y)
{
   int x;
   for(x=x1; x<=x2; x++)
      putpixel(x, y, c);
}
*/