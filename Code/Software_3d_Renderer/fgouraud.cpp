// fgrourand.cpp
// Robs gouraud routine
//
//
// All int's must be 32 bit.
//
//
//
//
// Version 1.02 - RP - Wed 6/1/99 - Fixed slightly off horizontal distorting
//
//
//
/*
 * $Log: fgouraud.cpp,v $
 * Revision 1.2  2003/09/20 12:57:03  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:36:27  stu_c
 * First Imported.
 *
 * Revision 1.3  2002/09/08 02:07:46  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.2  2001/11/02 19:31:45  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.1  2001/10/21 01:04:31  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:07  rob
 * First Imported.
 *
 * Revision 1.2  2001/10/02 22:25:25  rob
 * Detected unused types.h usage for when building via mox
 *
 * Revision 1.1.1.1  2001/01/01 21:13:39  rob
 * First Imported.
 *
 * Revision 1.2  2000/09/30 20:05:59  stu
 * Solar System Map changes and autopilot combat mode.
 *
 * Revision 1.1  2000/09/04 20:04:56  rob
 * Hopefully fixed little black line problem with software renderer.
 *
 * Revision 1.0.0.1  2000/08/21 22:09:40  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
#endif


//#include <types.h>
#include "fpoly4.h"
#include "genpoly.h"
#include "fdistort.h"
#include "fgouraud.h"


#define NUM_LIGHT_LEVELS 16

#if NUM_LIGHT_LEVELS==16
  #define LIGHT256_SHIFT 4
#elif NUM_LIGHT_LEVELS==32
  #define LIGHT256_SHIFT 3
#endif


char light_table[256*NUM_LIGHT_LEVELS];
char transparency_table[256*256];

void shadeoneline(char *screen_line_address, int line_startx, int line_endx, char *light_colour_address, FLOAT_TYPE light_start, FLOAT_TYPE light_end);
void shadeoneline_t(char *screen_line_address, int line_startx, int line_endx, char *light_colour_address, FLOAT_TYPE light_start, FLOAT_TYPE light_end);


void gouraud3(char *saddr, int swidth, dcoord *point_list, int r, int g, int b, int transparent)
{
int x1,x2,y1,y2;
border_type *starttemp;
border_type *endtemp;
//int count;
int ymin,ymax;
int xmin,xmax,xminl,xmaxl;
char *lightaddr;
extern border_type tex_startspace[1000];	// 1000 of line start, bmpstartx, bmpstarty
extern border_type tex_endspace[1000];		       // 1000 of line end, bmpendx, bmpendy

// start of debugging code
//for(count=0; count<1000; count++)
//  {
//  tex_startspace[count].linex=0xDEADBEEF;
//  tex_endspace[count].linex=0xDEADBEEF;  
//  }
// end of debugging code


// first connection
x1=point_list->x; y1=point_list->y; 
ymin=y1; ymax=y1;
xmin=x1; xmax=x1; xminl=(point_list->u)/2; xmaxl=(point_list->u)/2;
point_list++; x2=point_list->x; y2=point_list->y;
  
  if(y1==y2)       // horizontal line - either at top or bottom
      { 
        if(x2<xmin) { xmin=x2; xminl=(point_list->u)/2; }
        if(x2>xmax) { xmax=x2; xmaxl=(point_list->u)/2; }
      }    	/* ignore connection - but store for single height case*/


tex_draw2(tex_startspace,tex_endspace,x1,y1,x2,y2,((point_list-1)->u/2),0,(point_list->u)/2,0);


// second connection
x1=x2; y1=y2;
if(y1<ymin) { ymin=y1; }  if(y1>ymax) { ymax=y1; } 
point_list++; x2=point_list->x; y2=point_list->y; 


  if(y1==y2)       // horizontal line - either at top or bottom
      { 
        if(x2<xmin) { xmin=x2; xminl=(point_list->u)/2; }
        if(x2>xmax) { xmax=x2; xmaxl=(point_list->u)/2; }
      }    	/* ignore connection - but store for single height case*/


tex_draw2(tex_startspace,tex_endspace,x1,y1,x2,y2,((point_list-1)->u/2),0,(point_list->u)/2,0);
      
    
// third connection
x1=x2; y1=y2; 
if(y1<ymin) { ymin=y1; }  if(y1>ymax) { ymax=y1; } 
point_list-=2; x2=point_list->x; y2=point_list->y;

  if(y1==y2)       // horizontal line - either at top or bottom
      { 
        if(x2<xmin) { xmin=x2; xminl=(point_list->u)/2; }
        if(x2>xmax) { xmax=x2; xmaxl=(point_list->u)/2; }
      }    	/* ignore connection - but store for single height case*/
  
tex_draw2(tex_startspace,tex_endspace,x1,y1,x2,y2,((point_list-1)->u/2),0,(point_list->u)/2,0);


// convert the colour from rgb, and get the address of the light table

r/=256; g/=256; b/=256;
lightaddr=light_table + (convert_to_palette(r,g,b) * NUM_LIGHT_LEVELS);

// now draw the individual scan lines

 starttemp=tex_startspace + ymin;
 endtemp=tex_endspace + ymin;
 saddr+=(ymin*swidth);

 if(!transparent)
 {
   if(ymin==ymax)
     {
     shadeoneline(saddr,xmin,xmax,lightaddr,xminl,xmaxl);
     }

   while(ymin<=ymax)
    {
//  if(starttemp->linex==0xDEADBEEF || endtemp->linex==0xDEADBEEF)
//    {
//    DebugStr("\p DEADBEEF found");
//    }
    shadeoneline(saddr,starttemp->linex,endtemp->linex, lightaddr, starttemp->bmpx, endtemp->bmpx);
    saddr+=swidth;
  
    starttemp++;
    endtemp++;
  
    ymin++;
    }
    

  }
  else // if transparent
  {
   if(ymin==ymax)
     {
     shadeoneline_t(saddr,xmin,xmax,lightaddr,xminl,xmaxl);
     }

   while(ymin<=ymax)
    {
//  if(starttemp->linex==0xDEADBEEF || endtemp->linex==0xDEADBEEF)
//    {
//    DebugStr("\p DEADBEEF found");
//    }
    shadeoneline_t(saddr,starttemp->linex,endtemp->linex, lightaddr, starttemp->bmpx, endtemp->bmpx);
    saddr+=swidth;
  
    starttemp++;
    endtemp++;
  
    ymin++;
    }
 
  }  // end of if not/transparent
  
  
}  // end of gouraud3()



void shadeoneline(char *screen_line_address, int line_startx, int line_endx, char *light_colour_address, FLOAT_TYPE light_start, FLOAT_TYPE light_end)
{
int width;
FLOAT_TYPE light_displacement;
FLOAT_TYPE light_increment;

// lets find how many steps across, and where the address of the screen is
width=line_endx-line_startx+1;
screen_line_address+=line_startx;

// light reduction

light_end>>=(8+LIGHT256_SHIFT-1);
light_start>>=(8+LIGHT256_SHIFT-1);

light_end+=I_TO_F(NUM_LIGHT_LEVELS/2);		// change from -8 ... 7  to 0 ... 15.
light_start+=I_TO_F(NUM_LIGHT_LEVELS/2);

// we need to interpolate the light across the line
light_increment=light_end-light_start;
light_increment/=width;
 
// calculate the start point of light
light_displacement=light_start;

// now the actual loop that does the drawing.
while(width>0)
  {
  *(screen_line_address++)=*(light_colour_address + F_TO_I(light_displacement));
  light_displacement+=light_increment;
  width--;
  }

 
}




void shadeoneline_t(char *screen_line_address, int line_startx, int line_endx, char *light_colour_address, FLOAT_TYPE light_start, FLOAT_TYPE light_end)
{
int width;
FLOAT_TYPE light_displacement;
FLOAT_TYPE light_increment;

// lets find how many steps across, and where the address of the screen is
width=line_endx-line_startx+1;
screen_line_address+=line_startx;

// light reduction

light_end>>=(8+LIGHT256_SHIFT-1);
light_start>>=(8+LIGHT256_SHIFT-1);

light_end+=I_TO_F(NUM_LIGHT_LEVELS/2);		// change from -8 ... 7  to 0 ... 15.
light_start+=I_TO_F(NUM_LIGHT_LEVELS/2);

// we need to interpolate the light across the line
light_increment=light_end-light_start;
light_increment/=width;
 
// calculate the start point of light
light_displacement=light_start;


// now the actual loop that does the drawing.
while(width>0)
  {
  *screen_line_address=transparency_table[((*(unsigned char *)screen_line_address)<<8) + *(unsigned char *)(light_colour_address + F_TO_I(light_displacement))];
  screen_line_address++;
  light_displacement+=light_increment;
  width--;
  }

}








// 256 system colours = 6 levels of r,g, and b and 10 pure red, pure blue, pure green, and grey.
//
// r+l,g+l,b+l
//
// rgb->8bit
// 
// TO WRITE
//
// convert from rgb to 8 bit
//
// light table that has, say, 16 * 256 values.
// (=4096 bytes)
//


int convert_to_palette(int r, int g, int b);
void convert_to_rgb(int colour, int *r, int *g, int *b);



void setup_fgouraud(void)
{
char *light_table_ptr;
int r,g,b;
int rcopy,gcopy,bcopy;
int colour,light;

int fg,bg;
int fg_r,fg_g,fg_b;
int bg_r,bg_g,bg_b;


// set up light table

light_table_ptr=light_table;

for(colour=0; colour<256; colour++)
  {
  convert_to_rgb(colour,&rcopy,&gcopy,&bcopy);
  for(light=-(NUM_LIGHT_LEVELS/2); light<(NUM_LIGHT_LEVELS/2); light+=1)
    {
    r=rcopy; g=gcopy; b=bcopy;
    r+=light<<LIGHT256_SHIFT; if(r>255) r=255; if(r<0) r=0;
    g+=light<<LIGHT256_SHIFT; if(g>255) g=255; if(g<0) g=0;
    b+=light<<LIGHT256_SHIFT; if(b>255) b=255; if(b<0) b=0;
    
    *light_table_ptr=convert_to_palette(r,g,b);
    light_table_ptr++;
    }
    
  }

// set up transparency table
light_table_ptr=transparency_table;

for(bg=0; bg<256; bg++)
  {
  convert_to_rgb(bg,&bg_r,&bg_g,&bg_b);

  for(fg=0; fg<256; fg++)
    {
    convert_to_rgb(fg,&fg_r,&fg_g,&fg_b);
  
    r=(fg_r * (255-bg_r))/255 + bg_r;			// New BG = (FG/MAX * (MAX-BG)) + BG
    g=(fg_g * (255-bg_g))/255 + bg_g;
    b=(fg_b * (255-bg_b))/255 + bg_b;
    
    *light_table_ptr=convert_to_palette(r,g,b);
    light_table_ptr++;
    }
  }
}


#define RED_START 215
#define GREEN_START 225
#define BLUE_START 235
#define GRAY_START 245

/*

convert_to_palette - Converted to C by RP for Zex from LS_New_Year_1997 demo.

Notes From demo...

NOTE: all colours need to handle black (a special case)
NOTE2: low levels of red/green/blue combo's may well be handled better by going back to pure
colours, but who cares, this is a demo for heavens sake!!!

New Notes

NOTE 3: Some pure values might be better from the colour cube...
NOTE 4: Colour approximation also might work well.

*/


int convert_to_palette(int r, int g, int b)
{
int colour;

// test for gray
if(r==g && g==b)
  {
  colour=255-(r/23);					// 12 levels of grey
  if(colour==GRAY_START-1) { colour=0; } // black is black
  return colour;
  }
  
// test for pure blue
if(r==0 && g==0)
  {
  colour=GRAY_START-(b/25);
  if(colour==GRAY_START) { colour=255; } // black is black
  return colour;
  }

// test for pure green
if(r==0 && b==0)
  {
  colour=BLUE_START-(g/25);
  if(colour==BLUE_START) { colour=255; } // black is black
  return colour;
  }

// test for pure red
if(b==0 && g==0)
  {
  colour=GREEN_START-(r/25);
  if(colour==GREEN_START) { colour=255; } // black is black
  return colour;
  }

// some horrible colour, then...

r/=51; g/=51; b/=51;	// divide by meaning of life magic number
colour=215 - ( (r*36) + (g*6) + (b) );
if(colour==215) { colour=255; }

return colour;

}


void convert_to_rgb(int colour, int *r, int *g, int *b)
{
if(colour<RED_START)
  { // rgb section
  colour=215-colour;	// reverse order of sequence
  
  *r=colour/36;		// extract the top red value
  colour-=(*r)*36;	// remove it from the colour

  *g=colour /6;		// extract the green
  colour-=(*g)*6;	// remove it from the colour
  
  *b=colour;			// the only one left is the blue
  
  *r *= 51;
  *g *= 51;
  *b *= 51;			// convert to 0 to 255 levels
  
  return;
  }
  
if(colour<GREEN_START)
  { // red section
  *r=(255*(GREEN_START-colour))/10;
  *g=0;
  *b=0;
  
  return;
  }
  
if(colour<BLUE_START)
  { // green section
  *r=0;
  *g=(255*(BLUE_START-colour))/10;
  *b=0;
  
  return;
  }
  
if(colour<GRAY_START) 
  {  // blue section
  *r=0;
  *g=0;
  *b=(255*(GRAY_START-colour))/10;
  
  return;
  }

     // gray section
*r=(231*(255-colour))/10;
*g=*r;
*b=*r;

}





