//pictures
//SB - Lightsoft SW 8/12/98
/* $Log: pictures.c,v $
/* Revision 1.16  2002/09/08 02:36:02  stu_c
/* Pre compiled header support
/*
/* Revision 1.15  2002/09/01 14:54:20  rob_c
/* Fixed 640x480 main select screen.
/*
/* Revision 1.14  2002/06/24 21:58:16  stu_c
/* Update for mapping now done via ogl with glOrtho
/*
/* Revision 1.13  2002/02/21 22:51:56  stu
/* erase_zex_rect_rgb added
/*
/* Revision 1.12  2002/01/27 18:06:21  stu
/* Unused varaibles removed
/*
/* Revision 1.11  2002/01/09 23:49:15  stu
/* Furhter work on trading.
/*
/* Revision 1.10  2002/01/02 19:25:05  rob
/* Added key setup code
/*
/* Revision 1.9  2002/01/02 13:30:01  stu
/* Continuing development for trading/static screens
/*
/* Revision 1.8  2001/12/29 01:11:11  stu
/* Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
/*
/* Revision 1.7  2001/12/28 23:08:32  stu
/* Fixed a comment
/*
/* Revision 1.6  2001/11/09 18:28:58  rob
/* Various debug changes.
/*
/* Revision 1.5  2001/11/05 19:58:59  stu
/* More OSX work
/*
/* Revision 1.4  2001/11/02 23:21:53  stu
/* Further OS X work. Enabled syscon logging.
/*
/* Revision 1.3  2001/11/02 19:31:49  rob
/* Build variants for interface lib and carbon.
Coffee fixes after changes for X.
/*
/* Revision 1.2  2001/10/22 21:28:41  rob
/* Carbon warning changes
/*
/* Revision 1.1  2001/10/21 01:13:59  stu
/* Initial porting work
/*
/* Revision 1.0.0.1  2001/10/17 20:46:04  rob
/* First Imported.
/*
/* Revision 1.3  2001/07/08 20:24:26  stu
/* 0.66 release 080701
/*
/* Revision 1.2  2001/05/28 01:45:14  stu
/* 280501
/*
/* Revision 1.1.1.1  2001/01/01 21:13:35  rob
/* First Imported.
/*
/* Revision 1.1  2000/09/20 23:52:56  stu
/* HUD/AP/Scaling
/*
/* Revision 1.0.0.1  2000/08/21 22:01:03  stu
/* First Imported.
/*
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <mactypes.h>
  #endif

  #include "error_handler.h"
  #include "options.h"
  #include "3d_structures_def.h"
#endif //pch


//#include <intrinsics.h>
#include "splat_control.h"
#include "pictures.h"
#include "texture_management.h"
#include "ogl.h"
#include "file_io.h"
#include "genpoly.h"
#include "fdistort.h"
#include "fgouraud.h"

void internal_erase_zex_rect_colour(int w, int h);
char conv_hex(char the_char);
void copy_pict(LSRAW *the_picture, LSRAW *where);


//    kQAPixel_RGB16              = 1,                            /* 16 bits/pixel, R=14:10, G=9:5, B=4:0 */
//    kQAPixel_ARGB16             = 2,                            /* 16 bits/pixel, A=15, R=14:10, G=9:5, B=4:0 */
//    kQAPixel_RGB32              = 3,                            /* 32 bits/pixel, R=23:16, G=15:8, B=7:0 */


//takes a 128*128 QD3D mipmap data and converts to and rgba at picture_ptr
//nothing speedy needed here...
#define TEST_TEXTURE 0

void  convert_rgb16text_to_rgba(char* source, int* picture_ptr)
{
int line_count,pixel_count;
unsigned char byte1,byte2;
unsigned short rgb16;
unsigned int the_32pixel;
unsigned char red,green,blue;
//unsigned char alpha;

#if TEST_TEXTURE==1
unsigned int test_pixel=0xf0f0f07f;
unsigned int test_pixel2=0x0f0f0f7f;
#endif

#if TEST_TEXTURE==0
for (line_count=0; line_count<128; line_count++)
  {
   for (pixel_count=0; pixel_count<128; pixel_count++)
     {
      //read two bytes from source
       source=getbyte(source,&byte1);
       source=getbyte(source,&byte2);
       rgb16=(byte1<<8)+byte2;
       blue=rgb16&0x1f;	//blue
       green=(rgb16&0x3e0)>>5;	//green
       red=(rgb16&0x7c00)>>10;	//red
       red=red<<3;
       green=green<<3;
       blue=blue<<3;
       
       the_32pixel=(red<<24)+(green<<16)+(blue<<8)+0x7f;
       *picture_ptr=the_32pixel;
       picture_ptr++;
     }
  }

#else

for (line_count=0; line_count<128; line_count++)
  {
   for (pixel_count=0; pixel_count<63; pixel_count++)
     {
      //read three bytes from source
       source=getbyte(source,&byte1);
       source=getbyte(source,&byte2);
       rgb16=(byte1<<8)+byte2;
       blue=rgb16&0x1f;	//blue
       green=(rgb16&0x3e0)>>5;	//green
       red=(rgb16&0x7c00)>>10;	//red
       the_32pixel=(red<<24)+(green<<16)+(blue<<8)+0x7f;
//       *picture_ptr=the_32pixel;
       *picture_ptr=test_pixel;
       picture_ptr++;
       *picture_ptr=test_pixel2;
       picture_ptr++;
     }
  }

#endif

}


void  convert_argb32text_to_rgba(char* source, int* picture_ptr)
{
int line_count,pixel_count;
unsigned char byte1,byte2,byte3,byte4;
//short rgb16;
unsigned int the_32pixel;
//unsigned char red,green,blue,alpha;
#if TEST_TEXTURE==1
unsigned int test_pixel=0xf0f0f07f;
unsigned int test_pixel2=0x0f0f0f7f;
#endif

#if TEST_TEXTURE==0
for (line_count=0; line_count<128; line_count++)
  {
   for (pixel_count=0; pixel_count<128; pixel_count++)
     {
      //read four bytes from source
       source=getbyte(source,&byte1);	//alpha
       source=getbyte(source,&byte2);	//red
       source=getbyte(source,&byte3);	//green
       source=getbyte(source,&byte4);	//blue
       
       
       the_32pixel=(byte2<<24)+(byte3<<16)+(byte4<<8)+byte1;
       *picture_ptr=the_32pixel;
       picture_ptr++;
     }
  }

#else

for (line_count=0; line_count<128; line_count++)
  {
   for (pixel_count=0; pixel_count<63; pixel_count++)
     {
      //read three bytes from source
       source=getbyte(source,&byte1);
       source=getbyte(source,&byte2);
       rgb16=(byte1<<8)+byte2;
       blue=rgb16&0x1f;	//blue
       green=(rgb16&0x3e0)>>5;	//green
       red=(rgb16&0x7c00)>>10;	//red
       the_32pixel=(red<<24)+(green<<16)+(blue<<8)+0x7f;
//       *picture_ptr=the_32pixel;
       *picture_ptr=test_pixel;
       picture_ptr++;
       *picture_ptr=test_pixel2;
       picture_ptr++;
     }
  }

#endif

}

void  convert_rgb32text_to_rgba(char* source, int* picture_ptr)
{
int line_count,pixel_count;
unsigned char byte1,byte2,byte3,byte4;
//short rgb16;
unsigned int the_32pixel;
//unsigned char red,green,blue,alpha;
for (line_count=0; line_count<128; line_count++)
  {
   for (pixel_count=0; pixel_count<128; pixel_count++)
     {
      //read four bytes from source
       source=getbyte(source,&byte1);	//00
       source=getbyte(source,&byte2);	//red
       source=getbyte(source,&byte3);	//green
       source=getbyte(source,&byte4);	//blue
       
       
       the_32pixel=(byte2<<24)+(byte3<<16)+(byte4<<8)+0xff;
       *picture_ptr=the_32pixel;
       picture_ptr++;
     }
  }

}



char* getbyte(char* source, unsigned char * where)
{
int char_count=0;
char conv_text[2];
char res1,res2;

 while(char_count<2)
 {
   if( *(source+1)=='x') source+=2;	//skip 0x
   if( ((*source)==13) || ((*source)==9) || ((*source)==32) )
   {
   }	//skip white space/eol
   else
   {
     conv_text[char_count]=(*source);
     char_count++;
   }
  source++;
 }	//end of while count<2
res1=conv_hex(conv_text[0]);
res2=conv_hex(conv_text[1]);
(*where)=res1*0x10+res2;
return source;
}


char conv_hex(char the_char)
{
//char hex;

  if((the_char>='0') && (the_char<='9')) 
  {
    return the_char-'0';
  }
  else
  if((the_char>='A') && (the_char<='F')) 
  {
  return the_char - 'A' + 10;
  }
  else
  {
    report_error("conv_hex: Error in Hex number.","\p",800); 
  }

return 0; // else return 0

}


//print_pict splats a RAW picture to the current drawing buffer. The RAW file MUST
//have the width and hieght as the first two entries 
void print_pict(LSRAW *the_picture, UInt32 x, UInt32 y)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix,start_pix;
extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
LSRAW* pictbuffer;
LSRAW* pictbuffer_rgba;
Ptr raw_rgba_data;
float pos_x, pos_y;

picture_w=	(*the_picture).pwidth;
picture_h=	(*the_picture).pheight;	//now pointing to data


raw_data=(char *)&the_picture->raw_data;

y=y+picture_h;	//ogl origin is bottom left

//expand the crunched pict into some memory
    pictbuffer=(LSRAW*)NewPtr(picture_w*picture_h+80);
    if (pictbuffer==0) report_error("Memory Error in print_crunched_trans_pict(OGL),ID=1","\p",-1);
    copy_pict(the_picture,pictbuffer);
	invert_y_pict(pictbuffer);	//inverted 8 bit picture
	
    pictbuffer_rgba=(LSRAW*)NewPtr((picture_w*picture_h*4)+80);
    if (pictbuffer_rgba==0) report_error("Memory Error in print_crunched_trans_pict(OGL),ID=2","\p",-1);
//now load up the texture
    raw_data=(char *)&pictbuffer->raw_data;
    raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
//
    convert_to_rgba_wh(raw_data,raw_rgba_data,2,picture_w,picture_h);

    raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, picture_w);
	pos_x=x;
	pos_y=y;
	
    glRasterPos2f(pos_x, pos_y);
    glDrawPixels(picture_w,picture_h,GL_RGBA, GL_UNSIGNED_BYTE,raw_rgba_data);


   DisposePtr((Ptr)pictbuffer);
   DisposePtr((Ptr)pictbuffer_rgba);
return;


MyDebugStr(__LINE__,"pictures.c","print_pict");


picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
raw_data=(char *)&the_picture->raw_data;

start_pix=(char*)screenaddr+(y*screenwidth)+x;

for (i=0;i<picture_h;i++)	//line loop
 {
  splat_pix=start_pix;		//splat pixels from here
  for (j=0;j<picture_w;j++)	//pixel loop
   {
   *splat_pix=(*raw_data);
   raw_data++;
   splat_pix++;
   }
   start_pix+=screenwidth;	//next line
 } //end of line loop
}




extern int exec_watchdog;

char all_f[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char erase_colour;
//sets the colour erase_zex_rect fills with
void set_erase_colour(unsigned char colour_8)
{
int i;
 for (i=0;i<8;i++)
 all_f[i]=colour_8;
}

//assumes a 640 * 390 picture
void erase_zex_rect(int w, int h)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	     	     	     	     	     // Clear color buffer to dark grey
	glClear(GL_COLOR_BUFFER_BIT);


}


void erase_zex_rect_colour(int w, int h, unsigned char colour_8)
{
int red,green,blue;
//int startx, starty, endx, endy,clip,dx,x2,dy,xstep,ystep,y2,e;
float temp_r,temp_b,temp_g;

convert_to_rgb(colour_8, &red, &green, &blue);
     temp_r=red;
     temp_g=green;
     temp_b=blue;
     temp_r=temp_r/255;	//0-1
     temp_g=temp_g/255;	//0-1
     temp_b=temp_b/255;	//0-1
	 
	glClearColor(temp_r, temp_g, temp_b, 1.0f);	     	     	     	     	     // Clear color buffer to dark grey
	glClear(GL_COLOR_BUFFER_BIT);
}

void erase_zex_rect_rgb(float red, float green, float blue)
{
	 
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}



//and clips and fills with black where picture is not drawn
void print_pict_quick(LSRAW *the_picture, int x, int y)
{
//UInt32 picture_w,picture_h,i,j;
UInt32 line,pix;
Ptr raw_data;
//Ptr splat_pix;
Ptr start_pix;
Float64 *src_float,*dest_float;
char *src,*dest;

extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
//int x_index;	//index into each line of the picture at where we start drawing
//int x_fill;	//how many pixels at the start of the line we have to fill with black
//int x_draw;	//how many pixels from the start of the line do we have to draw
//int x_end;	//how many black pixels do we have to fill atthe ned of a line
//int src_start_pix;	//the first pixel in the data we have to copy
//picture_w=(*the_picture).pwidth;
//picture_h=(*the_picture).pheight;	//now pointing to data
MyDebugStr(__LINE__,"pictures.c","print_pict_quick");

raw_data=(char *)&the_picture->raw_data;

start_pix=(char*)screenaddr;
dest=start_pix;
src=raw_data;

src_float=(Float64 *) raw_data;
dest_float=(Float64 *) start_pix;


src_float=(Float64 *) raw_data;
dest_float=(Float64 *) start_pix;

for (line=0;line<390;line++)
    {
    //now the lines
   for (pix=0;pix<640/8;pix++)
         *dest_float++=*src_float++;

     src = src+640;	//point to next src line;
     dest=dest+screenwidth;

     src_float=(Float64 *) src;
     dest_float=(Float64 *) dest;

    }	//do next line

}

//Ptr black_line(Ptr dest,int pixel_count)
//{
//}
//
//Ptr copy_line(Ptr dest_float, Ptr src_float, int start_pix, int pixel_count)
//{
//}

//print_crunched_pict splats a crunched RAW picture to the current drawing buffer. The RAW file MUST
//have the width and hieght as the first two entries
//See ZEX_Cruncher for details. 
//clips in h

void print_crunched_pict(LSRAW *the_picture, UInt32 x, UInt32 y)
{
UInt32 picture_w,picture_h;
Ptr raw_data;
char rle_colour;

LSRAW* pictbuffer;
LSRAW* pictbuffer_rgba;
float pos_x, pos_y;
Ptr raw_rgba_data;



rle_colour=0;

picture_w=	(*the_picture).pwidth;
picture_h=	(*the_picture).pheight;	//now pointing to data


raw_data=(char *)&the_picture->raw_data;

y=y+picture_h;	//ogl origin is bottom left

//expand the crunched pict into some memory
    pictbuffer=(LSRAW*)NewPtr(picture_w*picture_h+80);
    if (pictbuffer==0) report_error("Memory Error in print_crunched_pict(OGL),ID=1","\p",-1);
    decrunch_pict(the_picture,pictbuffer);
    invert_y_pict(pictbuffer);
    pictbuffer_rgba=(LSRAW*)NewPtr((picture_w*picture_h*4)+80);
    if (pictbuffer_rgba==0) report_error("Memory Error in print_crunched_pict(OGL),ID=2","\p",-1);
//now load up the texture
    raw_data=(char *)&pictbuffer->raw_data;
    raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
//
    convert_to_rgba_wh(raw_data,raw_rgba_data,0,picture_w,picture_h);
//    glEnable (GL_BLEND);
//    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	    	    	    	    	    // Set The Blending Function For Translucency

    raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, picture_w);
	pos_x=x;
	pos_y=y;
	
    glRasterPos2f(pos_x, pos_y);
    glDrawPixels(picture_w,picture_h,GL_RGBA, GL_UNSIGNED_BYTE,raw_rgba_data);

//    glDisable (GL_BLEND);
   DisposePtr((Ptr)pictbuffer);
   DisposePtr((Ptr)pictbuffer_rgba);
}


//cinemascope version of above
void print_crunched_pict_C(LSRAW *the_picture, UInt32 x, UInt32 y, UInt32 top, UInt32 bott)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix,start_pix;
char rle_length, rle_colour;
int actual_x;

extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
MyDebugStr(__LINE__,"pictures.c","print_crunched_pict_C");

rle_colour=0;

picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
raw_data=(char *)&the_picture->raw_data;

start_pix=(char*)screenaddr+(y*screenwidth)+x;	//screenaddr
rle_length=0;

for (i=0;i<picture_h;i++)	//line loop
 {
  actual_x=x;	 //so we can keep track of where we are x wise
 
  splat_pix=start_pix;		//splat pixels from here
  for (j=0;j<picture_w;j++)	//pixel loop
   {
   if (rle_length==0)	//need to get some more data?
   {
    rle_length=(*raw_data);
    raw_data++;
    rle_colour=(*raw_data);
    raw_data++;
   }
   if (actual_x>0 && actual_x<monitor_w-1 && i>top && i<bott)
   {
    *splat_pix=rle_colour;
    splat_pix++;
   }
    rle_length--;
    actual_x++;
   }
   start_pix+=screenwidth;	//next line
 } //end of line loop
}

void print_crunched32_trans_pict(LSRAW *the_picture, UInt32 x, UInt32 y)
{
UInt32 picture_w,picture_h;
//LSRAW* pictbuffer;
//LSRAW* pictbuffer_rgba;

Ptr raw_rgba_data;
//int err;
//char pix;

picture_w=	(*the_picture).pwidth;
picture_h=	(*the_picture).pheight;	//now pointing to data


raw_rgba_data=(char *)&the_picture->raw_data;


y=y+picture_h;	//ogl origin is bottom left


    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);	    	    	    	    	    // Set The Blending Function For Translucency

    raw_rgba_data=(char *)&the_picture->raw_data;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, picture_w);
//    glPixelStorei(GL_PACK_ALIGNMENT, 1); 

    glRasterPos2f(x, y);
    glDrawPixels(picture_w,picture_h,GL_RGBA, GL_UNSIGNED_BYTE,raw_rgba_data);

    glDisable (GL_BLEND);
//   DisposePtr((Ptr)pictbuffer);
//   DisposePtr((Ptr)pictbuffer_rgba);


}

//same as above but colour 255 is transparent

void print_crunched_trans_pict(LSRAW *the_picture, UInt32 x, UInt32 y)
{
UInt32 picture_w,picture_h;
Ptr raw_data;
char rle_colour;
//GLuint texture_name;
float pos_x, pos_y;

//LSRAW * the_raw;
//LSRAW_RGBA * the_raw_rgba;
LSRAW* pictbuffer;
LSRAW* pictbuffer_rgba;

Ptr raw_rgba_data;
//int err;
//char pix;
rle_colour=0;

picture_w=	(*the_picture).pwidth;
picture_h=	(*the_picture).pheight;	//now pointing to data


raw_data=(char *)&the_picture->raw_data;

y=y+picture_h-1;	//ogl origin is bottom left

//expand the crunched pict into some memory
    pictbuffer=(LSRAW*)NewPtr(picture_w*picture_h+80);
    if (pictbuffer==0) report_error("Memory Error in print_crunched_trans_pict(OGL),ID=1","\p",-1);
    decrunch_pict(the_picture,pictbuffer);
    invert_y_pict(pictbuffer);
    pictbuffer_rgba=(LSRAW*)NewPtr((picture_w*picture_h*4)+80);
    if (pictbuffer_rgba==0) report_error("Memory Error in print_crunched_trans_pict(OGL),ID=2","\p",-1);
//now load up the texture
    raw_data=(char *)&pictbuffer->raw_data;
    raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
//
    convert_to_rgba_wh(raw_data,raw_rgba_data,2,picture_w,picture_h);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	    	    	    	    	    // Set The Blending Function For Translucency

    raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, picture_w);
	pos_x=x;
	pos_y=y;
	
    glRasterPos2f(pos_x, pos_y);
    glDrawPixels(picture_w,picture_h,GL_RGBA, GL_UNSIGNED_BYTE,raw_rgba_data);

    glDisable (GL_BLEND);
   DisposePtr((Ptr)pictbuffer);
   DisposePtr((Ptr)pictbuffer_rgba);

}


//magnifies a picture. Takes a floating magnification value
//which must be >=1.0
//wont shrink pictures, see above for that.
//does full clipping assuming y is zero ,if not you'll crash with a bad heap if picture extends off
//bottom of screen. Same for x.
//this can be speeded up by calculating start clip x and y
void print_pict_magnified(LSRAW *the_picture, UInt32 x, UInt32 y, int y_correction, int x_correction, float magnification)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix,start_pix;
//Ptr vram_start,vram_end;
//char colour;
extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
//int pixel_skip_counter, line_skip_counter;
//int trans_on=0;
float source_x_index_inc,source_y_index_inc;
float source_x_index,source_y_index;
int start_x,actual_x,actual_y;
int dest_width,dest_h;
int temp_y;


MyDebugStr(__LINE__,"pictures.c","print_pict_magnified");


source_x_index=0; source_y_index=0;	//take data from here

picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
raw_data=(char *)&the_picture->raw_data;


start_pix=(char*)screenaddr+(y*screenwidth)+x;

source_x_index_inc=1/magnification;	//index into source
source_y_index_inc=1/magnification;
dest_width=picture_w*magnification;	//how wide final picture is
dest_h=picture_h*magnification;
actual_x=x-(dest_width-monitor_h)/2;
actual_y=y-(dest_h-monitor_h)/2;
actual_y+=y_correction;
actual_x+=x_correction;
start_x=actual_x;

//pixel_skip_counter=0;
//line_skip_counter=0;
//
//
 for (i=0;i<dest_h;i++)	//line loop
  {
  if (actual_y>monitor_h-1) break;	//y clip - all done
  if (actual_y>=0)	//y clip
  {
   splat_pix=start_pix;		//splat pixels to here
   temp_y=(int)source_y_index*picture_w;
     for (j=0;j<dest_width;j++)	//pixel loop
      {
        if (actual_x>monitor_w-1) break;	//x clip - all done
         if (actual_x>=0)	//x clip
         {
          *splat_pix=*(raw_data+(int)source_x_index+temp_y);
          // *splat_pix=colour;
          splat_pix++;	 //inc vram pixel
         }
         
         source_x_index+=source_x_index_inc;	//inc in data x

         actual_x++;	//inc clipping variable
         
      }	//end of pixel loop

   start_pix+=screenwidth;	//vram to start of next line
   }	//end of y clipping
   
   source_x_index=0;	//data to start of line
   source_y_index+=source_y_index_inc;	//data inc
   actual_x=start_x;	//vram x start
   actual_y++;	//inc vram line
 } //end of line loop

}	//end of function



//same as above but translucent - 150999
void print_crunched_translucent_pict(LSRAW *the_picture, UInt32 x, UInt32 y)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix,start_pix;
char rle_length, rle_colour;
int actual_x;

extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
extern char transparency_table[];
MyDebugStr(__LINE__,"pictures.c","print_crunched_translucent_pict");

rle_colour=0;
picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
raw_data=(char *)&the_picture->raw_data;

start_pix=(char*)screenaddr+(y*screenwidth)+x;	//screenaddr
rle_length=0;

for (i=0;i<picture_h;i++)	//line loop
 {
  actual_x=x;	 //so we can keep track of where we are x wise
 
  splat_pix=start_pix;		//splat pixels from here
  for (j=0;j<picture_w;j++)	//pixel loop
   {
   if (rle_length==0)	//need to get some more data?
   {
    rle_length=(*raw_data);
    raw_data++;
    rle_colour=(*raw_data);
    raw_data++;
   }
   if (actual_x>0 && actual_x<monitor_w-1)
   { 
    if (rle_colour!=-1) *splat_pix=transparency_table[((*(unsigned char *)splat_pix)<<8) + rle_colour];;	//colour 255 is transparent
    splat_pix++;
   }
    rle_length--;
    actual_x++;
   }
   start_pix+=screenwidth;	//next line
 } //end of line loop
}



Handle GetZexPicture (unsigned long res_type, short resid)
{
Handle hpic;

 hpic = (Handle) ZGetResource(res_type,resid);  //Get the Handle to the Resource 
 if (hpic==0) report_error("GetZexPicture: RCZ Resource missing","\p",resid);
// DetachResource (hpic);
 return hpic;
}



//prints a raw scaled by sf - sf may be a number between 1 (no scaling
//and about 20 which means we only take every 20th pixel and line
void print_pict_scaled(LSRAW *the_picture, UInt32 x, UInt32 y, UInt32 sf, UInt32 trans_on)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix,start_pix;
char colour;
extern Ptr screenaddr;	//pointer to current drawing buffer
extern int screenwidth;
int pixel_skip_counter, line_skip_counter;
picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
raw_data=(char *)&the_picture->raw_data;
MyDebugStr(__LINE__,"pictures.c","print_pict_scaled");


start_pix=(char*)screenaddr+(y*screenwidth)+x;
pixel_skip_counter=0;
line_skip_counter=0;


for (i=0;i<picture_h;i++)	//line loop
 {
  splat_pix=start_pix;		//splat pixels to here
  
  if(line_skip_counter==sf)	//only plot if pix and line =sf
  {
    line_skip_counter=0;
    for (j=0;j<picture_w;j++)	//pixel loop
     {
      
      if (pixel_skip_counter==sf)
       {
         pixel_skip_counter=0;	 //reset pixel skip
         colour=(*raw_data);
         if (trans_on==1)
         {
         if (colour!=-1) *splat_pix=colour;
         }
         else
         *splat_pix=colour;
         
         splat_pix++;
       }
       else    pixel_skip_counter++;
      
      raw_data++;
     }	//end of for
   start_pix+=screenwidth;	//next line of dest
   }	//end of if lineskip
   else	//we have to skip a line
   {
    raw_data+=picture_w;	//skip in source
    line_skip_counter++;
   }
//  line_skip_counter=0;
//  }	//end of line_skip==sf
//  else	//line skip<>sf
//  {
//   raw_data+=picture_h;	//skip to next line of source
//   line_skip_counter++;
//  }
  
  pixel_skip_counter=0;
 } //end of line loop
}



//invert_y_pict inverts an 8 bit picture in y for ogl mainly
//not finished - SB 17/10/99

void invert_y_pict(LSRAW *the_picture)
{
UInt32 picture_w,picture_h;
//UInt32 i,j;
Ptr raw_data_top, raw_data_bottom;
char source_from_bottom, source_from_top;
int x,y;
Ptr start_top,start_bottom;


picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data

raw_data_top=(char *)&the_picture->raw_data;
raw_data_bottom=(char *)&the_picture->raw_data;
raw_data_bottom+=picture_w*(picture_h-1);

for (y=0;y<picture_h/2;y++)
  {
   start_top=raw_data_top; start_bottom=raw_data_bottom;
   for (x=0;x<picture_w;x++)
     {
        source_from_top=*raw_data_top;
        
        source_from_bottom=*raw_data_bottom;
        
        
        *raw_data_bottom=source_from_top;
        raw_data_bottom++;
        *raw_data_top=source_from_bottom;
        raw_data_top++;
     
     }
     raw_data_bottom=start_bottom;
     raw_data_top=start_top;
     raw_data_bottom-=picture_w;
     raw_data_top+=picture_w;
  }
}

//decrunch_pict splats an RCZ to memory as a contiguous RAW file. The RCZ file MUST
//have the width and hieght as the first two entries (as output from cruncher)
//See ZEX_Cruncher for details. 
//SB 210199
//Used in print_pict_scaled which needs a decompressed pict to work from
void decrunch_pict(LSRAW *the_picture, LSRAW *where)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix;
char rle_length, rle_colour;

//extern Ptr screenaddr;	//pointer to current drawing buffer
//extern int screenwidth;
rle_colour=0;

picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
(*where).pwidth=picture_w;
(*where).pheight=picture_h;

raw_data=(char *)&the_picture->raw_data;

splat_pix=(char *)&where->raw_data;	//screenaddr
rle_length=0;

for (i=0;i<picture_h;i++)	//line loop
 {
  for (j=0;j<picture_w;j++)	//pixel loop
   {
   if (rle_length==0)	//need to get some more data?
    {
      rle_length=(*raw_data);
      raw_data++;
      rle_colour=(*raw_data);
      raw_data++;
    }
    *splat_pix=rle_colour;
    splat_pix++;
    rle_length--;
   }
 } //end of line loop
}


void copy_pict(LSRAW *the_picture, LSRAW *where)
{
UInt32 picture_w,picture_h,i,j;
Ptr raw_data;
Ptr splat_pix;
char rle_colour;


picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
(*where).pwidth=picture_w;
(*where).pheight=picture_h;

raw_data=(char *)&the_picture->raw_data;

splat_pix=(char *)&where->raw_data;	//screenaddr

for (i=0;i<picture_h;i++)	//line loop
 {
  for (j=0;j<picture_w;j++)	//pixel loop
   {
      rle_colour=(*raw_data);
      raw_data++;
      *splat_pix=rle_colour;
      splat_pix++;
   }
 } //end of line loop
}



