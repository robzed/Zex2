// Robs distort routine, usually used for afine texture mapping.
//
//
// All int's must be 32 bit.
//
//
//
//
// Version 1.02 - RP - Wed 6/1/99 - Fixed slightly off horizontal distorting
// Version 1.03 - RP - Mon 11/1/99 - Created light values in distort at request of Stu. Thanks Stu. (Welcome).
//
//
/* $Log: fdistort.c,v $
/* Revision 1.4  2002/09/08 02:07:59  stu_c
/* Precompiled header adjustments
/*
/* Revision 1.3  2001/11/02 19:31:48  rob
/* Build variants for interface lib and carbon.
Coffee fixes after changes for X.
/*
/* Revision 1.2  2001/10/22 21:28:03  rob
/* Carbon warning changes
/*
/* Revision 1.1  2001/10/21 01:04:21  stu
/* Initial porting work
/*
/* Revision 1.0.0.1  2001/10/17 20:46:07  rob
/* First Imported.
/*
/* Revision 1.2  2001/05/28 01:50:24  stu
/* 280501
/*
/* Revision 1.1.1.1  2001/01/01 21:13:39  rob
/* First Imported.
/*
/* Revision 1.2  2000/09/30 20:05:44  stu
/* Solar System Map changes and autopilot combat mode.
/*
/* Revision 1.1  2000/09/04 20:04:53  rob
/* Hopefully fixed little black line problem with software renderer.
/*
/* Revision 1.0.0.1  2000/08/21 22:09:27  stu
/* First Imported.
/*
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "3d_structures_def.h"
  #include "processor.h"
#endif

#include "f_light.h"
#include "fpoly4.h"
#include "genpoly.h"
#include "fdistort.h"

#include "console.h"


#define WIDTH_FIXED_AT_128 1


#define abs(x) abs_nobranch(x)


//typedef struct {
//	  	  int linex; 
//	      FLOAT_TYPE bmpx; 
//	      FLOAT_TYPE bmpy;
//
//	  	  } border_type;		  

border_type tex_startspace[1000];	 // 1000 of line start, bmpstartx, bmpstarty
border_type tex_endspace[1000];		// 1000 of line end, bmpendx, bmpendy

#if 0
//void tex_draw(border_type *data_dest,int x1,int y1,int x2,int y2, int from_a, int from_b, int to_a, int to_b);
//void tex_draw_rev(border_type *data_dest,int x1,int y1,int x2,int y2, int from_a, int from_b, int to_a, int to_b);
#if LIGHT_SHADED_TEXTURES
module_private void scanoneline(char *screen_line_address, int line_startx, int line_endx,
	  char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width, char *light_colour_address);
module_private void scanoneline_t(char *screen_line_address, int line_startx, int line_endx,
	  char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width, char *light_colour_address);
#else
module_private void scanoneline(char *screen_line_address, int line_startx, int line_endx,
	  char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width);
module_private void scanoneline_t(char *screen_line_address, int line_startx, int line_endx,
	  char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width);
#endif
#endif

#if 0
#if LIGHT_SHADED_TEXTURES 
module_private void distort3(char *saddr, int swidth, dcoord *point_list, char *texture_addr, int texture_width, int light_value, int transparent)
#else
module_private void distort3(char *saddr, int swidth, dcoord *point_list, char *texture_addr, int texture_width, int transparent)
#endif
{
int x1,x2,y1,y2;
border_type *starttemp;
border_type *endtemp;
int ymin,ymax;
int xmin,xmax,xminu,xmaxu,xminv,xmaxv;
#if LIGHT_SHADED_TEXTURES
char *light_address;
#endif


// first connection
x1=point_list->x; y1=point_list->y; 
ymin=y1; ymax=y1;
xmin=x1; xmax=x1; xminu=point_list->u; xmaxu=point_list->u; xminv=point_list->v; xmaxv=point_list->v;
point_list++; x2=point_list->x; y2=point_list->y;
  
  if(y1==y2)       // horizontal line
    {		  
        if(x2<xmin) { xmin=x2; xminu=(point_list->u); xminv=(point_list->v); }
        if(x2>xmax) { xmax=x2; xmaxu=(point_list->u); xmaxv=(point_list->v); }
    }          	/* ignore connection - but store for single height case*/
      
tex_draw2(tex_startspace,tex_endspace,x1,y1,x2,y2,(point_list-1)->u,(point_list-1)->v,point_list->u,point_list->v);

// second connection    
x1=x2; y1=y2;
if(y1<ymin) { ymin=y1; }  if(y1>ymax) { ymax=y1; } 
point_list++; x2=point_list->x; y2=point_list->y; 
  
  if(y1==y2)       // horizontal line
    {		  
        if(x2<xmin) { xmin=x2; xminu=(point_list->u); xminv=(point_list->v); }
        if(x2>xmax) { xmax=x2; xmaxu=(point_list->u); xmaxv=(point_list->v); }
    }          	/* ignore connection - but store for single height case*/

tex_draw2(tex_startspace,tex_endspace,x1,y1,x2,y2,(point_list-1)->u,(point_list-1)->v,point_list->u,point_list->v);
    
    
// third connection
x1=x2; y1=y2; 
if(y1<ymin) { ymin=y1; }  if(y1>ymax) { ymax=y1; } 
point_list-=2; x2=point_list->x; y2=point_list->y;
  
  if(y1==y2)       // horizontal line
    {		  
        if(x2<xmin) { xmin=x2; xminu=(point_list->u); xminv=(point_list->v); }
        if(x2>xmax) { xmax=x2; xmaxu=(point_list->u); xmaxv=(point_list->v); }
    }          	/* ignore connection - but store for single height case*/

tex_draw2(tex_startspace,tex_endspace,x1,y1,x2,y2,(point_list-1)->u,(point_list-1)->v,point_list->u,point_list->v);



#if LIGHT_SHADED_TEXTURES
{
int temp;
temp=(light_value>>(8+LIGHT256_SHIFT));
temp=(NUM_LIGHT_LEVELS/2);
light_address=light_table+(light_value>>(8+LIGHT256_SHIFT))+(NUM_LIGHT_LEVELS/2);
}
#endif

if(!transparent)
{
// now draw the individual scan lines.

 starttemp=tex_startspace + ymin;
 endtemp=tex_endspace + ymin;
 saddr+=(ymin*swidth);

 if(ymin==ymax)
   {
#if LIGHT_SHADED_TEXTURES
  scanoneline(saddr, xmin,xmax, texture_addr,xminu,xminv,xmaxu,xmaxv,texture_width,light_address);
#else
  scanoneline(saddr, xmin,xmax, texture_addr,xminu,xminv,xmaxu,xmaxv,texture_width);
#endif
   }

 while(ymin<=ymax)
  { // scanoneline(s,startx,  endx,    bmp_address,  bmp_startx,  bmp_starty,   bmp_endx,      bmp_endy,   bmp_width)
#if LIGHT_SHADED_TEXTURES
  scanoneline(saddr, starttemp->linex,endtemp->linex, texture_addr, starttemp->bmpx,starttemp->bmpy, endtemp->bmpx,endtemp->bmpy, texture_width,light_address);
#else
  scanoneline(saddr, starttemp->linex,endtemp->linex, texture_addr, starttemp->bmpx,starttemp->bmpy, endtemp->bmpx,endtemp->bmpy, texture_width);
#endif
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
#if LIGHT_SHADED_TEXTURES
  scanoneline_t(saddr, xmin,xmax, texture_addr,xminu,xminv,xmaxu,xmaxv, texture_width,light_address);
#else
  scanoneline_t(saddr, xmin,xmax, texture_addr,xminu,xminv,xmaxu,xmaxv, texture_width);
#endif
   }

 while(ymin<=ymax)
  { // scanoneline(s,startx,  endx,    bmp_address,  bmp_startx,  bmp_starty,   bmp_endx,      bmp_endy,   bmp_width)
#if LIGHT_SHADED_TEXTURES
  scanoneline_t(saddr, starttemp->linex,endtemp->linex, texture_addr, starttemp->bmpx,starttemp->bmpy, endtemp->bmpx,endtemp->bmpy, texture_width,light_address);
#else
  scanoneline_t(saddr, starttemp->linex,endtemp->linex, texture_addr, starttemp->bmpx,starttemp->bmpy, endtemp->bmpx,endtemp->bmpy, texture_width);
#endif
  saddr+=swidth;
  
  starttemp++;
  endtemp++;
  
  ymin++;
  }
 
}


}  // end of distort3()

#endif // if 0





//void draw_virtual_lines(dcoord *point_list, int *ymax, int *ymin, int *xmin, int *xmax, 
//xminu, xmaxu, xminv, xminv, space, space)
//{
//
//}






void set_scanline_point(border_type *dest, int x, int a, int b );
void swap(int *i, int *j);

void set_scanline_point(border_type *dest, int x, int a, int b )
{
      dest->linex = x;
      dest->bmpx = a; 
      dest->bmpy = b;
}

void swap(int *i, int *j)
{
int temp;

  temp=*i;
  *i=*j;
  *j=temp;
}

//
// NAME: tex_draw2
//
// AUTHOR: Rob Probin, latest mod 2/9/00
//
// FUNCTION: To draw a line in the outline arrays (start and end), including specific texture for that point.
// 
// VARIABLES: ????_a and ????_b relate to the texture coordinates, sometimes refered to as u and v
//            from_a and from_b are the texture coordinates associated with the point (x1,y1)
//            to_a and to_b are the texture coordinates associated with the point (x2,y2)
//
// ACTIONS:   1. We have to draw up and down.
//            2. Keep the stepping even along the line. This is quite difficult, since if we were drawing
//               down all the time (by reversing the points to make it so), then we would have to plot at the
//               start of an x sub-line, or the end of an x-sub-line depending upon the specific direction. This
//               only applies to near horizontal lines, and is a result of point 4, below.
//            3. Cater for one line high polygons
//            4. We don't want to step along a same-height x subline
//            5. We don't want to compare either start or end array values in the core loop, we just want to write
//               directly into a specific array.
//            6. We want to make sure that the end and start are recorded properly.
//
// ASSUMPTIONS:  Lines are taken from a concave polygon that is defined in an anticlockwise manner.
//
void tex_draw2(border_type *start_dest, border_type *end_dest, int x1,int y1,int x2,int y2, int from_a, int from_b, int to_a, int to_b)
{
FLOAT_TYPE x_step;
FLOAT_TYPE a_step;
FLOAT_TYPE b_step;
int y_width;
FLOAT_TYPE x;
FLOAT_TYPE a;
FLOAT_TYPE b;
border_type *data_dest;
int going_down;
int array_increment;

 
 if(y1==y2)		// single line, ensure start and end point are taken case of
    {
     if(x2>x1)
       {
       set_scanline_point(start_dest+y1, x1, from_a, from_b);
       set_scanline_point(end_dest+y2, x2, to_a, to_b);
       }
     else
       {
       set_scanline_point(end_dest+y1, x1, from_a, from_b);
       set_scanline_point(start_dest+y2, x2, to_a, to_b);
       }
     return;
    }
 

// Decide which direction the line is going in. This only matters for (a) to make y count (y_width) positive,
// (b) to ensure the correct array is used, (c) effect later line direction decisions.

 if(y1<y2)  // due to anticlockwise, must be left side  (going down - y getting greater)
   {    
   data_dest=start_dest;
   going_down=1;
   y_width=y2-y1+1; 	   		// this is the y axis width , e.g. 0 to 2 is width 3.
   }
 else
   {
   data_dest=end_dest;
   going_down=0;
   y_width=y1-y2+1;
   }


if(  // near-horizontal left-going heading-upwards and left-going heading-downwards to be draw upwards
    ( abs(x2-x1) > abs(y2-y1) )	// near horizonal slope (i.e. doesn't increment y every x)
     && (x1>x2) // since drawing down is for end points of each y line, we therefore know whether we 
//     && ((x2>x1  && going_down ) || (x1>x2 && !going_down )) // since drawing down is for end points of each y line, we therefore know whether we 
  )							  // should be plotting the initial point at the end or the start of each slope
  {
  if( going_down ) { swap (&x1,&x2); swap(&from_a,&to_a); swap(&from_b,&to_b); swap(&y1, &y2); }
  array_increment=-1;				// destination pointer needs to be incremented in upwards direction
  }
else  // any other type of line - draw downwards
  {
  if(! going_down) { swap (&x1,&x2); swap(&from_a,&to_a); swap(&from_b,&to_b); swap(&y1, &y2); }
  array_increment=1;				// destination pointer needs to be incremented in downwards direction
  }


x_step=x2-x1; if(x_step>0) x_step++; if(x_step<0) x_step--; 
//print_debug(x_step,"x_width");
//print_debug(x1,"x1");
//print_debug(x2,"x2");
//print_debug(y_width,"y_width");

x_step=I_TO_F(x_step); x_step/=y_width;	// the x step is the amount of sideways movement, therefore
							// for x2=x1, should be 0

//print_debug(F_TO_I(x_step),"x step");
//print_debug(0xffff & x_step,"x step lower");

a_step=to_a-from_a; a_step/=y_width; 	     // a_step=I_TO_F(a_step); a_step/=y_width;
b_step=to_b-from_b; b_step/=y_width; 	     //b_step=I_TO_F(b_step); b_step/=y_width;

x=I_TO_F(x1) /* + 0x7fff */ ;
a=from_a; 		// I_TO_F(from_a);
b=from_b; 		// I_TO_F(from_b);

data_dest+=y1;

//print_debug(F_TO_I(x),"x start");


// This loop plots the points in the start or end array, plotting the point first, then incrementing. In this
//   way the point gets plotted first, and then the line, x coordinate and texture coordinates get updated. This
//   is fine for all line directions apart from two of the four near-horizontal: (1) left-going heading-upwards,
//   and (2) right-going heading downwards.
// Problems with end points not being plotted in the array are handled on entry to this function by the 
//   check_scanline_endpoint calls. Theoretically at least one of these is not needed, but with line reversing 
//   it is safer to leave them in than risk a missing corner point.
while(y_width>=1)
  {
  set_scanline_point(data_dest,F_TO_I(x),a,b);

// if(y_width<4) print_debug(F_TO_I(x),"x");
  
  data_dest+=array_increment;

  x+=x_step;
  a+=a_step;
  b+=b_step;
  
  y_width--;
  }


//set_scanline_point(data_dest,F_TO_I(x),a,b);

}


#if 0
#if LIGHT_SHADED_TEXTURES
void scanoneline(char *screen_line_address, int line_startx, int line_endx,
	  char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width, char *light_colour_address)
#else
void scanoneline(char *screen_line_address, int line_startx, int line_endx, char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width)
#endif
{
int width;
FLOAT_TYPE bmp_x_displacement,bmp_y_displacement;
FLOAT_TYPE bmp_x_increment,bmp_y_increment;
int temp;

// lets find how many steps across, and where the address of the screen is
width=line_endx-line_startx+1;
screen_line_address+=line_startx;

// we need to step diagonally across bmp. We do so in "width" number of steps.
bmp_y_increment=bmp_endy-bmp_starty;
bmp_y_increment/=((float)width);
// test //bmp_y_increment*=bmp_width;

bmp_x_increment=bmp_endx-bmp_startx; 
bmp_x_increment/=((float)width);
 
// calculate the start point on the bitmap
bmp_x_displacement=bmp_startx;
bmp_y_displacement=bmp_starty; //test  // *bmp_width;

// now the actual loop that does the drawing.
while(width>0)
  {
#if WIDTH_FIXED_AT_128
  temp=(F_TO_I(bmp_y_displacement) << 7);
#else
  temp=(F_TO_I(bmp_y_displacement) * bmp_width);
#endif
//  temp=(F_TO_I(bmp_y_displacement) * bmp_width);   /// transfering bmp_width to the other side of this F_TO_I gives us problems - can we rely
  													// on the fact that bmp width will be 128????
  													
  													// also, if we do get this optimisation, perhaps we should save the original for
  													// 1/z texture mapping 
  *(screen_line_address++) =
  
  #if LIGHT_SHADED_TEXTURES
  	  	  *(  (  ((unsigned char) (*( bmp_address + F_TO_I(bmp_x_displacement) + temp)))<<LIGHT_SHIFT  )+light_colour_address  );
  	  	  //                        |------------------------------------------------|
  	  	  //      |-------------| |---------------------------------------------------|
  	  	  //     |---------------------------------------------------------------------|
  	  	  //  |---------------------------------------------------------------------------------------|
  #else
  	  	  *( bmp_address + F_TO_I(bmp_x_displacement) + temp);
  #endif
  
  bmp_x_displacement+=bmp_x_increment;
  bmp_y_displacement+=bmp_y_increment;
  
  width--;
  }

}

#endif
#if 0
#if LIGHT_SHADED_TEXTURES
void scanoneline_t(char *screen_line_address, int line_startx, int line_endx,
	  char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width, char *light_colour_address)
#else
void scanoneline_t(char *screen_line_address, int line_startx, int line_endx,
	  char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width)
#endif
{
int width;
FLOAT_TYPE bmp_x_displacement,bmp_y_displacement;
FLOAT_TYPE bmp_x_increment,bmp_y_increment;
int temp;

// lets find how many steps across, and where the address of the screen is
width=line_endx-line_startx+1;
screen_line_address+=line_startx;

// we need to step diagonally across bmp. We do so in "width" number of steps.
bmp_y_increment=bmp_endy-bmp_starty;
bmp_y_increment/=width;
// test //bmp_y_increment*=bmp_width;

bmp_x_increment=bmp_endx-bmp_startx; 
bmp_x_increment/=width;
 
// calculate the start point on the bitmap
bmp_x_displacement=bmp_startx;
bmp_y_displacement=bmp_starty; //test  // *bmp_width;

// now the actual loop that does the drawing.
while(width>0)
  {
#if WIDTH_FIXED_AT_128
  temp=(F_TO_I(bmp_y_displacement) << 7);
#else
  temp=(F_TO_I(bmp_y_displacement) * bmp_width);
#endif
//  temp=(F_TO_I(bmp_y_displacement) * bmp_width);   /// transfering bmp_width to the other side of this F_TO_I gives us problems - can we rely
  													// on the fact that bmp width will be 128????
  													
  													// also, if we do get this optimisation, perhaps we should save the original for
  													// 1/z texture mapping 
  *(screen_line_address) = transparency_table[((*(unsigned char *)screen_line_address)<<8) + *(unsigned char* )
 
  #if LIGHT_SHADED_TEXTURES
  
  	  	  (  (  ((unsigned char) (*( bmp_address + F_TO_I(bmp_x_displacement) + temp)))<<LIGHT_SHIFT  )+light_colour_address  )
  	     //                        |------------------------------------------------|
  	  	 //      |-------------| |---------------------------------------------------|
  	  	 //     |---------------------------------------------------------------------|
  	  	 //  |---------------------------------------------------------------------------------------|
  #else
  	  	  ( bmp_address + F_TO_I(bmp_x_displacement) + temp)
  #endif
		  ];
		  
  screen_line_address++;    
  bmp_x_displacement+=bmp_x_increment;
  bmp_y_displacement+=bmp_y_increment;
  width--;
  }

}
#endif
