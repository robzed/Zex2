// *************************************MODULE HEADER*********************************
// * FILENAME           : ogl_text.c
// * MODULE TITLE       : Untitled 1
// * PROJECT            : Zex OGL
// * DATE STARTED       : 16:19:47 on 16th October 1999
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************
/*
 * $Log: ogl_text.c,v $
 * Revision 1.1.1.1  2003/09/05 22:36:11  stu_c
 * First Imported.
 *
 * Revision 1.10  2002/09/08 02:11:20  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.9  2002/06/24 21:59:22  stu_c
 * Update for mapping now done via glOrtho rather than int_to_ogl
 *
 * Revision 1.8  2002/01/09 23:49:11  stu
 * Furhter work on trading.
 *
 * Revision 1.7  2002/01/02 13:30:11  stu
 * Continuing development for trading/static screens
 *
 * Revision 1.6  2001/12/29 01:12:09  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.5  2001/11/09 18:29:11  rob
 * Various debug changes.
 *
 * Revision 1.4  2001/11/02 19:32:34  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.3  2001/10/26 22:41:08  rob
 * Dual headers
 *
 * Revision 1.2  2001/10/24 21:43:04  stu
 * First set of source to build on pb for carbon.
 *
 * Revision 1.1  2001/10/21 01:10:49  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:07  rob
 * First Imported.
 *
 * Revision 1.2  2001/06/01 21:50:31  stu
 * GetFNum used to set up fonts
 *
 * Revision 1.1.1.1  2001/01/01 21:13:37  rob
 * First Imported.
 *
 * Revision 1.0.0.1  2000/08/21 22:06:40  stu
 * First Imported.
 *
 */

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "options.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
    #include <DrawSprocket/DrawSprocket.h>
  #else
    #include <DrawSprocket.h>
    #include <quickdraw.h>
    #include <fonts.h>
  #endif
  #include "error_handler.h"
  #include "zex_misc.h"

#endif //pch

#include    "fpoly4.h"
#include "genpoly.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "pictures.h"
#include "OGL.h"
#include "zextypes.h"
#include "olg_text.h"

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *
GLuint gGLCharStart;
extern AGLContext     ctx;


// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *



int init_ogl_text()
{
short font_num;

#define MONACO_BASE 1
#define HELVETICA_BASE 128+1
#define HELVETICA_BASE_7 128+128+1

int err;
//load in the Helvetica font as a display list
	gGLCharStart = glGenLists(128);
        
        GetFNum("\pMonaco",&font_num);
	err=aglUseFont(ctx, font_num, normal, 9, 0, 128,gGLCharStart);

        err=aglGetError();
        if (err!=0) report_error ("init_ogl_text: AGL error (1).","\p",err);



	gGLCharStart = glGenLists(128);
        GetFNum("\pHelvetica",&font_num);

	err=aglUseFont(ctx, font_num, normal, 9, 0, 128,gGLCharStart);
   err=aglGetError();
   if (err!=0) report_error ("init_ogl_text: AGL error (2).","\p",err);

	gGLCharStart = glGenLists(128);
	err=aglUseFont(ctx, font_num, extend, 9, 0, 128,gGLCharStart);

   err=aglGetError();
   if (err!=0) report_error ("init_ogl_text: AGL error (3).","\p",err);

    if (err!=0)
	return(-1);
	
return 0;
}



void OGL_draw_monaco_bold(char* the_string)
{
#if OGL==1
int len,stop;


	len = 0;
	stop=0;
	while (stop==0)
	 {
	  if (the_string[len]==0)
	  stop=1;
	  else
	  len++;
	 }
	glListBase(MONACO_BASE);	
	glCallLists(len, GL_BYTE, the_string);
#endif
}


void OGL_draw_helvetica(char* the_string)
{
#if OGL==1
int len,stop;

	len = 0;
	stop=0;
	while (stop==0)
	 {
	  if (the_string[len]==0)
	  stop=1;
	  else
	  len++;
	 }
	glListBase(HELVETICA_BASE);	
	glCallLists(len, GL_BYTE, the_string);
#endif
}

void OGL_draw_helvetica_small(char* the_string)
{
#if OGL==1
int len,stop;

	len = 0;
	stop=0;
	while (stop==0)
	 {
	  if (the_string[len]==0)
	  stop=1;
	  else
	  len++;
	 }
	glListBase(HELVETICA_BASE_7);	
	glCallLists(len, GL_BYTE, the_string);
#endif
}

//Pstring version
void OGL_draw_helveticaP(unsigned char * the_string)
{
	glListBase(HELVETICA_BASE);	
	glCallLists(the_string[0], GL_BYTE, the_string+1);
}

//void OGL_DrawStr(char *str,int the_len,int x, int y)
//{
//	  static int bitmapBase = 0;
//	  
//	  if(bitmapBase == 0)
//	  {
//	  	  bitmapBase = glGenLists(256);
//	  	  aglUseFont(aglGetCurrentContext(), kFontIDNewYork, normal, 10, 0, 256, bitmapBase);
//	  }
//	  
//	  glListBase(bitmapBase);
//	  glCallLists(the_len, GL_UNSIGNED_BYTE, (GLubyte *) str);
//}


//replacements for moveto and drawstring that do the right thing if ogl is active
static int last_x, last_y;
void ZMoveTo(int x, int y)
{
extern int OGL_active;
last_x=x;last_y=y;

#if OGL==1
if (OGL_active==1)
  {
  OGL_moveto(x,y);
  }
  else
#endif
  MoveTo(x,y);
}



void ZLineTo(int x, int y)
{
   if (single_line_clip(&last_x,&last_y,&x,&y)==0) return;
   OGL_line_no_colour( last_x, last_y, x, y);
}

void ZDrawString(unsigned char * the_string)
{

   OGL_draw_helveticaP(the_string);
}

void ZRGBForeColor (const RGBColor * RGB_Colour)
{
float temp_r,temp_b,temp_g;

     temp_r=RGB_Colour->red;
     temp_g=RGB_Colour->green;
     temp_b=RGB_Colour->blue;
     temp_r=temp_r/65535.0;	//0-1
     temp_g=temp_g/65535.0;	//0-1
     temp_b=temp_b/65535.0;	//0-1

	glColor3d(temp_r, temp_g, temp_b);

}



static float bg_r,bg_b,bg_g;
//USed exlusively for eraserect
void ZRGBBackColor (const RGBColor * RGB_Colour)
{

     bg_r=RGB_Colour->red;
     bg_g=RGB_Colour->green;
     bg_b=RGB_Colour->blue;
     bg_r=bg_r/65535.0;	//0-1
     bg_g=bg_g/65535.0;	//0-1
     bg_b=bg_b/65535.0;	//0-1

}


void ZEraseRect(Rect * the_rect)	//quite an expensive one this. To be avoided if poss...
{
float top,left,bottom,right;

  top=the_rect->top;
  left=the_rect->left;
  bottom=the_rect->bottom;
  right=the_rect->right;
  glColor3d(bg_r, bg_g, bg_b);
  glBegin(GL_POLYGON);
  glVertex2d (left,top);
  glVertex2d (right,top);
  glVertex2d (right,bottom);
  glVertex2d (left,bottom);
  glVertex2d (left,top);
  
  glEnd();
  
}
