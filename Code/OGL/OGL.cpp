// *************************************MODULE HEADER*********************************
// * FILENAME           : OCL.c
// * MODULE TITLE       : 
// * PROJECT            : Zex Open GL
// * DATE STARTED       : 04:23:51 on 9th August 1999
// * FIRST AUTHOR       : Stu
// *
// * COPYRIGHT (c)        
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// * Zex Open GL Interface functions. Requires OGL to be 1 in OGL.ext
// * else the functions are not compiled, and our renderer is used instead.
// * Won't work with OGL versions belwo 1.2 because of glColor4d?????
// * Currently ONLY does Lambert and Gourad polys - no TM polys or DLP stuff.
// * Note to Stu: Find a spare Sunday to finish it.
// * 'Nuther note: OGL's RGB332 (or maybe 233, whatever) format is the same as Apple's 8 bit clut??? Rob?
// * So if we set interp indexed mode we should be able to use our exapnded RCZd's as-is?
// *
// *
// *
// ***********************************************************************************
/*
 * $Log: OGL.c,v $
 * Revision 1.2  2003/09/20 12:57:10  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:36:11  stu_c
 * First Imported.
 *
 * Revision 1.20  2002/09/08 02:11:03  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.19  2002/06/24 21:40:35  stu_c
 * Screen coords to ogl normalised mapping code removed; we're now mapped via the glOrtho command. Saves 2 divs and 2 adds per vertex.
 *
 * Revision 1.18  2002/04/28 06:22:34  stu
 * This checkin adds more to the HUD and brings out some sim variables to the player.
 *
 * Revision 1.17  2002/02/24 03:41:46  stu
 * (Some) compiler warnings cleaned up
 *
 * Revision 1.16  2002/02/10 21:52:08  stu
 * texture binding optimisation
 *
 * Revision 1.15  2002/01/20 21:37:03  stu
 * GL_CLAMP_TO_EDGE
 *
 * Revision 1.14  2002/01/20 17:00:21  stu
 * GL_CLAMP changed to GL_CLAMP_TO_EDGE
 *
 * Revision 1.13  2002/01/09 23:47:29  stu
 * Enabled textures via ROBIT
 *
 * Revision 1.12  2002/01/02 19:22:31  rob
 * added key setup code
 *
 * Revision 1.11  2002/01/02 13:30:16  stu
 * Continuing development for trading/static screens
 *
 * Revision 1.10  2001/12/29 01:12:04  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.9  2001/12/14 19:41:46  stu
 * Docking
 *
 * Revision 1.8  2001/11/26 23:31:22  rob
 * Remove textures quickly.
 *
 * Revision 1.7  2001/11/09 18:29:08  rob
 * Various debug changes.
 *
 * Revision 1.6  2001/11/02 19:32:31  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.5  2001/10/26 22:41:27  rob
 * Dual headers
 *
 * Revision 1.4  2001/10/24 21:40:20  stu
 * First set of source to build on pb for carbon.
 *
 * Revision 1.3  2001/10/23 20:46:25  stu
 * More carbonising
 *
 * Revision 1.2  2001/10/22 21:28:00  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:10:26  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:07  rob
 * First Imported.
 *
 * Revision 1.9  2001/06/23 20:05:43  stu
 * 0.66 checkin
 *
 * Revision 1.8  2001/06/01 21:38:58  stu
 * Translucency belnding function fixed
 *
 * Revision 1.7  2001/05/28 21:40:19  stu
 * Major commit 280501
 *
 * Revision 1.6  2001/05/28 01:44:44  stu
 * 280501
 *
 * Revision 1.4  2001/04/02 17:28:23  rob
 * made various variables switch on and off-able from forth. These include dust, asteriods, hud, zbuffer.
 *
 * Revision 1.3  2001/04/01 18:36:04  stu
 * *** empty log message ***
 *
 * Revision 1.2  2001/01/14 17:13:17  stu
 * Gourad Fix
 *
 * Revision 1.1.1.1  2001/01/01 21:13:37  rob
 * First Imported.
 *
 * Revision 1.0.0.1  2000/08/21 22:06:37  stu
 * First Imported.
 *
 */

#define ROBIT 1			// ROBIT=0 removes textures, ROBIT=1 adds textures

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "options.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
    #include <DrawSprocket/DrawSprocket.h>
  #else
    #include <Palettes.h>
    #include <quickdraw.h>
    #include <fp.h>
    #include <DrawSprocket.h>
    #include <NumberFormatting.h>
  #endif

  #include "3d_structures_def.h"
  #include "error_handler.h"
  #include "zex_misc.h"
#endif //pch

#include    "fpoly4.h"
#include "genpoly.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "pictures.h"
#include "OGL.h"
#include "olg_text.h"
#include "texture_management.h"
#include "splat_control.h"
#include "keyboard.h"

// ***********************************************************************************
// * CONSTANTS 
// *

/* These from the MacOS X release of OGL!!!!
** Renderer ID numbers
*/
#define AGL_RENDERER_GENERIC_ID      0x00020200
#define AGL_RENDERER_ATI_ID          0x00021000
#define DEPTH_Z 0.95

//BIND OPTIMISATION stops us binding the same texture twice. Might help on some OGL implementations
#define BIND_OPTIMISATION 1	

//#define AGL_NO_ERROR                 0 /* no error                        */
//
//#define AGL_BAD_ATTRIBUTE        10000 /* invalid pixel format attribute  */
//#define AGL_BAD_PROPERTY         10001 /* invalid renderer property       */
//#define AGL_BAD_PIXELFMT         10002 /* invalid pixel format            */
//#define AGL_BAD_RENDINFO         10003 /* invalid renderer info           */
//#define AGL_BAD_CONTEXT          10004 /* invalid context                 */
//#define AGL_BAD_DRAWABLE         10005 /* invalid drawable                */
//#define AGL_BAD_GDEV             10006 /* invalid graphics device         */
//#define AGL_BAD_STATE            10007 /* invalid context state           */
//#define AGL_BAD_VALUE            10008 /* invalid numerical value         */
//#define AGL_BAD_MATCH            10009 /* invalid share context           */
//#define AGL_BAD_ENUM             10010 /* invalid enumerant               */
//#define AGL_BAD_OFFSCREEN        10011 /* invalid offscreen drawable      */
//#define AGL_BAD_FULLSCREEN       10012 /* invalid offscreen drawable      */
//#define AGL_BAD_WINDOW           10013 /* invalid window                  */
//#define AGL_BAD_POINTER          10014 /* invalid pointer                 */
//#define AGL_BAD_MODULE           10015 /* invalid code module             */
//#define AGL_BAD_ALLOC            10016 /* memory allocation failure       */



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

int OGL_active;	//global

static Point start_mouse_xy;	//when we enable the mouse where is it so we can centre the thing
static int mouse_on;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

int	SufficientVideoMemory( AGLPixelFormat fmt );
void cut_out (LSRAW* source, LSRAW* destination, int source_start_x, int source_start_y);
int kill_tile(int tile_id);
void OpenGLCloseDisplay (void);
//module_private Boolean OpenGLCreateDisplay (void);
void vid_to_8_bit();
void vid_to_16_bit();
void vid_to_32_bit();
CGrafPtr open_ogl_window(void);
//module_private void DrawGL(AGLContext ctx);

static unsigned long last_ogl_name=0;	//for context
int	binds;                   //for debugging

void OGL_plot(int x1, int y1, int colour)
{
#if OGL==1
int red,green,blue;
float temp_r,temp_b,temp_g;

convert_to_rgb(colour, &red, &green, &blue);
     temp_r=red;
     temp_g=green;
     temp_b=blue;
     temp_r=temp_r/255;	//0-1
     temp_g=temp_g/255;	//0-1
     temp_b=temp_b/255;	//0-1

//   light=light/32000;
   
//   temp_r=r*light;
   if (temp_r>1) temp_r=1;
   if (temp_r<0) temp_r=0;

//   temp_g=g*light;
   if (temp_g>1) temp_g=1;
   if (temp_g<0) temp_g=0;

//   temp_b=b*light;
   if (temp_b>1) temp_b=1;
   if (temp_b<0) temp_b=0;
//   glLineWidth(2.0);
	glColor3d(temp_r, temp_g, temp_b);

    glBegin(GL_POINTS);

	glVertex2d(x1, y1);

    glEnd();


#endif
}

void OGL_plot_rgb(int x1, int y1, float red, float green, float blue)
{
    glColor3d(red, green, blue);
          
    glBegin(GL_POINTS);
    glVertex2d(x1, y1);
    glEnd();
}


void OGL_plot_rgb_quick(int x1, int y1, float red, float green, float blue)
{

  glColor3d(red, green, blue);         
  glVertex2d(x1, y1);
          
}


void OGL_line_rgb(int x1, int y1, int x3, int y3, float red, float green, float blue)
{

    glColor3d(red, green, blue);
    glBegin(GL_LINES);
    glVertex2d(x1, y1);
    glVertex2d(x3,y3);
    glEnd();
}

//Used if in a blBegin(GL_LINES) block from caller
void OGL_line_rgb_quick(int x1, int y1, int x3, int y3, float red, float green, float blue)
{

  glColor3d(red, green, blue);
  glVertex2d(x1, y1);
  glVertex2d(x3,y3);
}

void OGL_line(int x1, int y1, int x3, int y3, int colour)
{
int red,green,blue;
//int startx, starty, endx, endy,clip,dx,x2,dy,xstep,ystep,y2,e;
float temp_r,temp_b,temp_g;

convert_to_rgb(colour, &red, &green, &blue);
     temp_r=red;
     temp_g=green;
     temp_b=blue;
     temp_r=temp_r/255;	//0-1
     temp_g=temp_g/255;	//0-1
     temp_b=temp_b/255;	//0-1

	glColor3d(temp_r, temp_g, temp_b);
//   
    glBegin(GL_LINES);

	glVertex2d(x1, y1);
	glVertex2d(x3,y3);

    glEnd();


}


void OGL_line_no_colour(int x1, int y1, int x3, int y3)
{
#if OGL==1
   
    glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x3,y3);
    glEnd();

#endif
}




void OGL_polygon(int numb_verts,fcoord *point_list, float r, float g, float b)
{
#if OGL==1

   //int err,
   int i;
	
	glColor3d(r, g, b);

	glBegin(GL_POLYGON);
	for (i=0;i<numb_verts;i++)
	{
	glVertex2d(point_list->x, point_list->y);
	point_list++;
	}

	glEnd();
#endif
}

void OGL_polygonZ(int numb_verts,fcoord *point_list, float r, float g, float b)
{
#if OGL==1

   //int err,
   int i;

	
	glColor3d(r, g, b);

	glBegin(GL_POLYGON);
	for (i=0;i<numb_verts;i++)
	{
	glVertex3d(point_list->x, point_list->y,point_list->z);
	point_list++;
	}
	glEnd();

//   err=aglGetError();
#endif
}

void OGL_GTpolygon(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, int translucent)
{
#if OGL==1

   //int err,
   int i;
   float temp_r,temp_g,temp_b;
   
#if ROBIT
    glEnable(GL_TEXTURE_2D);      
	#if BIND_OPTIMISATION==1
    if (last_ogl_name!=name)	//don't bind if same name
	#endif
	{
	#if ENG_DEBUG_EXPORT==1
	binds++;                   //for debugging
	#endif
    last_ogl_name=name;
	glBindTexture(GL_TEXTURE_2D, name);
    }

#endif

    glBegin(GL_POLYGON);
//	glColor3d(r, g, b);
	for (i=0;i<numb_verts;i++)
	{
   //affect colours by light
   float light;
   
   
   light=point_list->l;
   
     temp_r=(light)/65535;	//0-1
     temp_g=(light)/65535;	//0-1
     temp_b=(light)/65535;	//0-1

//   light=light/32000;
   
//   temp_r=r*light;
   if (temp_r>1) temp_r=1;
   if (temp_r<0) temp_r=0;

//   temp_g=g*light;
   if (temp_g>1) temp_g=1;
   if (temp_g<0) temp_g=0;

//   temp_b=b*light;
   if (temp_b>1) temp_b=1;
   if (temp_b<0) temp_b=0;
  
   
	
	if (translucent==0)
     {
	glColor3d(temp_r, temp_g, temp_b);
    glTexCoord2f(point_list->u, point_list->v);
	glVertex2d(point_list->x, point_list->y);
	}
	else
	{
    glEnable (GL_BLEND);

	glColor4d(temp_r, temp_g, temp_b,0.5);
    glTexCoord2f(point_list->u, point_list->v);
	glVertex2d(point_list->x, point_list->y);
    glDisable (GL_BLEND);

	}
	
	point_list++;
	}

	glEnd();
#if ROBIT
    glDisable(GL_TEXTURE_2D);
#endif
//   err=aglGetError();
#endif
}


//z buffer version used if use_zbuffer!=0
void OGL_GTpolygonZ(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, int translucent)
{
#if OGL==1

   //int err,
   int i;
   float temp_r,temp_g,temp_b;
   

#if ROBIT
    glEnable(GL_TEXTURE_2D);      
	#if BIND_OPTIMISATION==1
    if (last_ogl_name!=name)	//don't bind if same name
	#endif
	{
	#if ENG_DEBUG_EXPORT==1
	binds++;                   //for debugging
	#endif
    last_ogl_name=name;
    glBindTexture(GL_TEXTURE_2D, name);
	}
	
#endif    
    
    glBegin(GL_POLYGON);
//	glColor3d(r, g, b);
	for (i=0;i<numb_verts;i++)
	{
   //affect colours by light
   float light;
   
   
   light=point_list->l;
   
     temp_r=(light)/65535;	//0-1
     temp_g=(light)/65535;	//0-1
     temp_b=(light)/65535;	//0-1

//   light=light/32000;
   
//   temp_r=r*light;
   if (temp_r>1) temp_r=1;
   if (temp_r<0) temp_r=0;

//   temp_g=g*light;
   if (temp_g>1) temp_g=1;
   if (temp_g<0) temp_g=0;

//   temp_b=b*light;
   if (temp_b>1) temp_b=1;
   if (temp_b<0) temp_b=0;
  
   
	
	if (translucent!=2)
     {
	glColor3d(temp_r, temp_g, temp_b);
    glTexCoord2f(point_list->u, point_list->v);
	glVertex3d(point_list->x, point_list->y, point_list->z);
	}
	else
	{
    glEnable (GL_BLEND);

	glColor4d(temp_r, temp_g, temp_b,0.5);
    glTexCoord2f(point_list->u, point_list->v);
	glVertex3d(point_list->x, point_list->y, point_list->z);
    glDisable (GL_BLEND);
	}
	
	point_list++;
	}
	glEnd();
#if ROBIT
    glDisable(GL_TEXTURE_2D);
#endif

#endif
}



void OGL_GTpolygon3(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, float translucent)
{
#if OGL==1

   //int err,
   int i;
   float temp_r,temp_g,temp_b;
   
#if ROBIT
    glEnable(GL_TEXTURE_2D);      
	#if BIND_OPTIMISATION==1
    if (last_ogl_name!=name)	//don't bind if same name
	#endif
	{
	#if ENG_DEBUG_EXPORT==1
	binds++;                   //for debugging
	#endif
    last_ogl_name=name;
    glBindTexture(GL_TEXTURE_2D, name);
    }
#endif
 
    if(translucent>0.98) translucent=0;	//make really opaque
    
     
     if(translucent!=0)
     {
      glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency

     }
    
    glBegin(GL_POLYGON);
	for (i=0;i<numb_verts;i++)
	{
   //affect colours by light
   float light;
   
   
   light=point_list->l;
   
     temp_r=(light)/65535;	//0-1
     temp_g=(light)/65535;	//0-1
     temp_b=(light)/65535;	//0-1

   if (temp_r>1) temp_r=1;
   if (temp_r<0) temp_r=0;

   if (temp_g>1) temp_g=1;
   if (temp_g<0) temp_g=0;

   if (temp_b>1) temp_b=1;
   if (temp_b<0) temp_b=0;
  
   
	
   if (translucent==0)
     {
	glColor3d(temp_r, temp_g, temp_b);
    glTexCoord2f(point_list->u, point_list->v);
	glVertex2d(point_list->x, point_list->y);
	}
	else
	{
	glColor4d(temp_r, temp_g, temp_b,translucent);
        glTexCoord2f(point_list->u, point_list->v);
	glVertex2d(point_list->x, point_list->y);

	}
	
	point_list++;
	}

	glEnd();

#if ROBIT
    glDisable(GL_TEXTURE_2D);
#endif
    glDisable (GL_BLEND);

//   err=aglGetError();
#endif
}


//z buffer version used if use_zbuffer!=0
void OGL_GTpolygonZ3(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, float translucent)
{
#if OGL==1

   //int err,
   int i;
   float temp_r,temp_g,temp_b;
   


#if ROBIT    
         glEnable(GL_TEXTURE_2D);      
	#if BIND_OPTIMISATION==1
    if (last_ogl_name!=name)	//don't bind if same name
	#endif
	{
	#if ENG_DEBUG_EXPORT==1
	binds++;                   //for debugging
	#endif
     last_ogl_name=name;
     glBindTexture(GL_TEXTURE_2D, name);
	}
#endif 
    if(translucent>0.98) translucent=0;	//make really opaque
    
     
     if(translucent!=0)
     {
      glEnable (GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency

     }

   
    glBegin(GL_POLYGON);
//	glColor3d(r, g, b);
	for (i=0;i<numb_verts;i++)
	{
   //affect colours by light
   float light;
   
   
   light=point_list->l;
   
     temp_r=(light)/65535;	//0-1
     temp_g=(light)/65535;	//0-1
     temp_b=(light)/65535;	//0-1

//   light=light/32000;
   
//   temp_r=r*light;
   if (temp_r>1) temp_r=1;
   if (temp_r<0) temp_r=0;

//   temp_g=g*light;
   if (temp_g>1) temp_g=1;
   if (temp_g<0) temp_g=0;

//   temp_b=b*light;
   if (temp_b>1) temp_b=1;
   if (temp_b<0) temp_b=0;
  
   
	
	if (translucent==0)
     {
	glColor3d(temp_r, temp_g, temp_b);
    glTexCoord2f(point_list->u, point_list->v);
	glVertex3d(point_list->x, point_list->y, point_list->z);
	}
	else
	{
    glEnable (GL_BLEND);

	glColor4d(temp_r, temp_g, temp_b,translucent);
    glTexCoord2f(point_list->u, point_list->v);
	glVertex3d(point_list->x, point_list->y, point_list->z);
    glDisable (GL_BLEND);
	}
	
	point_list++;
	}
	glEnd();
#if ROBIT    
    glDisable(GL_TEXTURE_2D);
#endif
    glDisable (GL_BLEND);

#endif
}




void OGL_Tpolygon(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, float r, float g, float b, int translucent)
{
#if OGL==1

   //int err,
   int i;
//   float temp_r,temp_g,temp_b;
   


#if ROBIT	
         glEnable(GL_TEXTURE_2D);      
	#if BIND_OPTIMISATION==1
    if (last_ogl_name!=name)	//don't bind if same name
	#endif
	{
	#if ENG_DEBUG_EXPORT==1
	binds++;                   //for debugging
	#endif
    last_ogl_name=name;
         glBindTexture(GL_TEXTURE_2D, name);
    }
	
#endif
      if(translucent==0)
      {
	glColor3d(r, g, b);
      }
      else
      {
      glEnable (GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4d(r, g, b,0.6);

      }

	glBegin(GL_POLYGON);
	for (i=0;i<numb_verts;i++)
	{
   //affect colours by light
//   float light;
   
   
//	
//	if (translucent==0)
//     {
    glTexCoord2f(point_list->u, point_list->v);
	glVertex2d(point_list->x, point_list->y);
//	}
//	else
//	{
//    glTexCoord2f(point_list->u, point_list->v);
//	glVertex2d(point_list->x, point_list->y);
//
//	}
	
	point_list++;
	}
//	  glColor3d(r, g, b);
//	  glVertex3d(point_list->x, point_list->y, 0.0);
//	  point_list++;
//	  glColor3d(r, g, b);
//	  glVertex3d(point_list->x, point_list->y, 0.0);
//	  point_list++;
	glEnd();
#if ROBIT    
    glDisable(GL_TEXTURE_2D);
#endif
    glDisable (GL_BLEND);

//   err=aglGetError();
#endif
}


void OGL_TpolygonZ(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, float r, float g, float b, int translucent)
{
#if OGL==1

   //int err,
   int i;
//   float temp_r,temp_g,temp_b;
   


#if ROBIT	
         glEnable(GL_TEXTURE_2D);      
	#if BIND_OPTIMISATION==1
    if (last_ogl_name!=name)	//don't bind if same name
	#endif
	{
    last_ogl_name=name;
	#if ENG_DEBUG_EXPORT==1
	binds++;                   //for debugging
	#endif
	glBindTexture(GL_TEXTURE_2D, name);
    }
#endif
	glBegin(GL_POLYGON);

//	if (translucent==0)
	glColor3d(r, g, b);
//	else
//	glColor4d(r, g, b,0.5);
	
	for (i=0;i<numb_verts;i++)
	{
   //affect colours by light
//   float light;
   
   
//	
	if (translucent==0)
     {
    glTexCoord2f(point_list->u, point_list->v);
	glVertex3d(point_list->x, point_list->y, point_list->z);
	}
	else
	{
//    glEnable (GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
    glEnable (GL_BLEND);
	glColor4d(r, g, b,0.5);
    glTexCoord2f(point_list->u, point_list->v);
	glVertex3d(point_list->x, point_list->y, point_list->z);
    glDisable (GL_BLEND);

	}
	
	point_list++;
	}
//	  glColor3d(r, g, b);
//	  glVertex3d(point_list->x, point_list->y, 0.0);
//	  point_list++;
//	  glColor3d(r, g, b);
//	  glVertex3d(point_list->x, point_list->y, 0.0);
//	  point_list++;
	glEnd();
#if ROBIT    
	glDisable(GL_TEXTURE_2D);
#endif

//   err=aglGetError();
#endif
}

// *************************************************************************************************
//textured particles
void OGL_TPpolygon(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, float r, float g, float b, int translucent)
{
#if OGL==1

   //int err,
   int i;
//   float temp_r,temp_g,temp_b;
   


#if ROBIT	
	glEnable(GL_TEXTURE_2D);      
	#if BIND_OPTIMISATION==1
    if (last_ogl_name!=name)	//don't bind if same name
	#endif
	{
    last_ogl_name=name;
	#if ENG_DEBUG_EXPORT==1
	binds++;                   //for debugging
	#endif
        glBindTexture(GL_TEXTURE_2D, name);
    }
#endif

      if(translucent==0)
      {
	glColor3d(r, g, b);
      }
      else
      {
       glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency

       glColor4d(r, g, b,0.6);

      }

	glBegin(GL_POLYGON);
	for (i=0;i<numb_verts;i++)
	{
   //affect colours by light
//   float light;
   
   
    glTexCoord2f(point_list->u, point_list->v);
	glVertex2d(point_list->x, point_list->y);
	
	point_list++;
	}

	glEnd();
#if ROBIT    
    glDisable(GL_TEXTURE_2D);
#endif
    glDisable (GL_BLEND);

//   err=aglGetError();
#endif
}


void OGL_TPpolygonZ(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, float r, float g, float b, int translucent)
{
#if OGL==1

   //int err,
   int i;
//   float temp_r,temp_g,temp_b;
   


#if ROBIT	
	 glEnable(GL_TEXTURE_2D);      
	#if BIND_OPTIMISATION==1
    if (last_ogl_name!=name)	//don't bind if same name
	#endif
	{
    last_ogl_name=name;
         glBindTexture(GL_TEXTURE_2D, name);
	#if ENG_DEBUG_EXPORT==1
	binds++;                   //for debugging
	#endif
    }
#endif

      if(translucent==0)
      {
	glColor3d(r, g, b);
      }
      else
      {
       glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency

       glColor4d(r, g, b,0.6);

      }

	glBegin(GL_POLYGON);

	
	for (i=0;i<numb_verts;i++)
	{
   //affect colours by light
//   float light;
   
   
    glTexCoord2f(point_list->u, point_list->v);
	glVertex3d(point_list->x, point_list->y, point_list->z);
	
	point_list++;
	}
	glEnd();
#if ROBIT    
    glDisable(GL_TEXTURE_2D);
#endif
    glDisable (GL_BLEND);

//   err=aglGetError();
#endif
}


void OGL_Gpolygon(int numb_verts,fdcoord *point_list,float r, float g, float b, int translucent)
{
#if OGL==1

   //int err,
   int i;
   float temp_r,temp_g,temp_b;
      
   glBegin(GL_POLYGON);
   for (i=0;i<numb_verts;i++)
   {
   //affect colours by light
   float light;
   
   
   light=point_list->u;
   
     temp_r=(r+light)/65535;	//0-1
     temp_g=(g+light)/65535;	//0-1
     temp_b=(b+light)/65535;	//0-1

//   light=light/32000;
   
//   temp_r=r*light;
   if (temp_r>1) temp_r=1;
   if (temp_r<0) temp_r=0;

//   temp_g=g*light;
   if (temp_g>1) temp_g=1;
   if (temp_g<0) temp_g=0;

//   temp_b=b*light;
   if (temp_b>1) temp_b=1;
   if (temp_b<0) temp_b=0;
	
	if (translucent==0)
        {
	 glColor3d(temp_r, temp_g, temp_b);
	 glVertex2d(point_list->x, point_list->y);
	}
	else
	{
         glEnable (GL_BLEND);
    	 glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency

	 glColor4d(temp_r, temp_g, temp_b,0.5);
	 glVertex2d(point_list->x, point_list->y);
	glDisable(GL_BLEND);
	}
	
    point_list++;
    }	//end of for
//	  glColor3d(r, g, b);
//	  glVertex3d(point_list->x, point_list->y, 0.0);
//	  point_list++;
//	  glColor3d(r, g, b);
//	  glVertex3d(point_list->x, point_list->y, 0.0);
//	  point_list++;
	glEnd();
//   err=aglGetError();
#endif
}



void OGL_GpolygonZ(int numb_verts,fdcoord *point_list,float r, float g, float b, int translucent)
{
#if OGL==1

   //int err,
   int i;
   float temp_r,temp_g,temp_b;
     
   glBegin(GL_POLYGON);
   for (i=0;i<numb_verts;i++)
   {
   //affect colours by light
   float light;
   
   
   light=point_list->u;
   
     temp_r=(r+light)/65535;	//0-1
     temp_g=(g+light)/65535;	//0-1
     temp_b=(b+light)/65535;	//0-1

   
   if (temp_r>1) temp_r=1;
   if (temp_r<0) temp_r=0;

   if (temp_g>1) temp_g=1;
   if (temp_g<0) temp_g=0;

   if (temp_b>1) temp_b=1;
   if (temp_b<0) temp_b=0;
	
	if (translucent==0)
        {
	 glColor3d(temp_r, temp_g, temp_b);
	 glVertex3d(point_list->x, point_list->y, point_list->z);
	}
	else
	{
    glEnable (GL_BLEND);

	 glColor4d(temp_r, temp_g, temp_b,0.5);
	 glVertex3d(point_list->x, point_list->y, point_list->z);
    glDisable (GL_BLEND);
	}
	
    point_list++;
    }	//end of for
	glEnd();


//   err=aglGetError();
#endif
}










void OGL_show_it(void)	//as it says, wait for all rendering operations
						//to end and then show the result
{ 

#if OGL==1
//   int err;
extern AGLContext     ctx;

//    err=aglGetError();
	//glFinish(); //- Removed SB 210602 on Geoff Stahls advice
	aglSwapBuffers(ctx);
#endif
}

/*

float int_to_ogl_x(int coord_x)
{
#if OGL==1
float return_val;
return_val=coord_x-screen_cent_x;
return_val=return_val/screen_cent_x;

return return_val;
#endif
}


float int_to_ogl_y(int coord_y)
{
#if OGL==1
float return_val;
return_val=monitor_h-coord_y;
return_val-=screen_cent_y;
return_val=return_val/screen_cent_y;

return return_val;
#endif
}
*/

//allow drawing in main area of screen
void OGL_set_stencil_for_above()
{
#if OGL==1
 glClear(GL_STENCIL_BUFFER_BIT);
/* create a full screen sized rectangle-shaped stencil area minus the control panel area */
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-3.0, 3.0, -3.0, 3.0, -1.0, 1.0);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();

 glStencilFunc (GL_ALWAYS, 0x1, 0x1);
 glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE);
 glBegin(GL_QUADS);
 glVertex3f (-1.0, -1.0, 0.0);
 glVertex3f (-1.0, 0.8, 0.0);
 glVertex3f (1.0, 0.8, 0.0);
 glVertex3f (1.0, -1.0, 0.0);
 glEnd();
#endif
}

//allow drawing in all of screen
void OGL_set_stencil_for_all()
{
#if OGL==1
 glClear(GL_STENCIL_BUFFER_BIT);
/* create a full screen sized rectangle-shaped stencil area */
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-3.0, 3.0, -3.0, 3.0, -1.0, 1.0);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();

 glStencilFunc (GL_ALWAYS, 0x1, 0x1);
 glStencilOp (GL_REPLACE, GL_REPLACE, GL_REPLACE);
 glBegin(GL_QUADS);
 glVertex3f (-1.0, -1.0, 0.0);
 glVertex3f (-1.0, 1.0, 0.0);
 glVertex3f (1.0, 1.0, 0.0);
 glVertex3f (1.0, -1.0, 0.0);
 glEnd();

#endif
}


void OGL_draw_pict(int x, int y,LSRAW* pictbuffer_rgba, int picture_w, int picture_h)
{
#if OGL==1

Ptr raw_rgba_data;

    raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, picture_w);
//    glPixelStorei(GL_PACK_ALIGNMENT, 1); 

    glRasterPos2f(x, y);
    glDrawPixels(picture_w,picture_h,GL_RGBA, GL_UNSIGNED_BYTE,raw_rgba_data);
#endif
}


//takes a clut index and sets drawing colour
void OGL_set_pen_colour_from_index(unsigned int colour)
{
#if OGL==1

int red,green,blue;
float temp_r,temp_b,temp_g;
                             
                             convert_to_rgb(colour, &red, &green, &blue);
     						temp_r=red;
     						temp_g=green;
     						temp_b=blue;
     						temp_r=temp_r/255;	//0-1
     						temp_g=temp_g/255;	//0-1
     						temp_b=temp_b/255;	//0-1
	glColor3d(temp_r, temp_g, temp_b);

#endif
}

//takes int x and y
void OGL_moveto(float x, float y)
{
#if OGL==1
    
	glRasterPos2f(x,y);
#endif
}

void OGL_move_and_set_colour(float x, float y, int colour)
{
#if OGL==1

int red,green,blue;
float temp_r,temp_b,temp_g;
                             convert_to_rgb(colour, &red, &green, &blue);
     						temp_r=red;
     						temp_g=green;
     						temp_b=blue;
     						temp_r=temp_r/255;	//0-1
     						temp_g=temp_g/255;	//0-1
     						temp_b=temp_b/255;	//0-1
	glColor3d(temp_r, temp_g, temp_b);    
	glRasterPos2f(x,y);

#endif
}







/* 7/10 \ /
         |
   __          __   .      __                 _._
  /  \        /     |     /  \                 |
  |  O P E N  | -G  L     | C  O N S T A N T   I M A G E S
  \__/        \__/  |__   \__/                _|_

*/

#if OGL==1

/* If possible I'd like you to accelerate the control panel work as I need to leverage some routines not 
yet written to speed up the inventory screen, dialogs, mouse cursor etc. I need a routine that is passed 
a pointer to a 32 bit rgba (i..e. ogl native) bitmap and tags it to an OGL_QUAD (or however you want to 
do it) for display; you'll need a list and a routine run every frame obviously - suggest you return some 
ID I can note and have a kill routine to remove things from the list - errm have an egg, sorry :) My 
pic_utils are way too slow as the data has to travel over PCI every frame rather than being in vram ready 
to bind to some vertices. */

//BIG NOTE. THE LARGEST TEXTURE OGL SEEMS TO COPE WITH IS 256*256
//so we need to split larger ones up into smaller ones
// ---- module data ----

// lets have a structure with various details in
typedef struct 
  {
  int slot_active;
  int context;
  LSRAW *rgba_image;
  int ci_name;
  int x1;
  int y1;
  int x2;
  int y2;
  int tile_id;	//SB 211199
  fdcoord output_points[6];
  } ci_struct;
  
#define CONSTANT_IMAGE_MAX 30

#define INACTIVE 0
#define ACTIVE 1

ci_struct constant_image_array[CONSTANT_IMAGE_MAX];


int current_context;

// ---- functions ----

//this onesits in front of set_as_constant_image and checks for textures>256 pixels
static int unique_tile_id;

// -1 returned means no constant image assigned
int set_as_constant_image(LSRAW *_rgba_image, int _x1, int _y1, int _x2, int _y2)
{
int width, height,tiles_x, tiles_y, ix1,iy1;
//int number_of_tiles;
int cut_top_left_x, cut_top_left_y;
int tx;
int return_val=0;

LSRAW * temp_texture_rgba;

width=_x2-_x1;
height=_y2-_y1;



tiles_x=width/256+1;
tiles_y=height/256+1;
if (tiles_x==1 && tiles_y==1)
 {
//check is factor of 2 wide

if (is_a_factor_of_2(width)==1)
  return set_as_constant_image1(_rgba_image, _x1, _y1, _x2, _y2,-1);

else	//we have to pad the image out to be a factor of 2 wide (256 in our case)

 {
  temp_texture_rgba=(LSRAW*)NewPtr((256*256*4)+80);
  if (temp_texture_rgba==0)
   {
  	  report_error ("set_as_constant_image: Couldn't get temp memory.(2)","\p",-1);

      return -1;
   } 

     cut_out(_rgba_image, temp_texture_rgba,0, 0);	//cut out and pad to 256 wide
    //insert
     ix1=_x1; iy1=_y1;
     return_val=set_as_constant_image1(temp_texture_rgba, ix1, iy1, ix1+256, iy1+256, -1);
     DisposePtr((Ptr) temp_texture_rgba);
     return return_val;
  }
 
 }	//end of texture<256 pixels wide

//texture >256 pixels
else
 {
 
 //we cut each tile of the texture into a temp 256*256*4 buffer, calc its true position
 //relative to the parent tex and pass it to set_as_constant_image1
 //the remainder must be padded with 0's to a power of two boundary
 
 if (unique_tile_id>10000000) unique_tile_id=1;	//call me a cynic but...
 if (unique_tile_id<0) unique_tile_id=1;	//call me a cynic but...
 
  unique_tile_id++;	//couldnt care less what this is just as long as its unique
//get temp
 
 temp_texture_rgba=(LSRAW*)NewPtr((256*256*4)+80);
 if (temp_texture_rgba==0)
 {
  	  report_error ("set_as_constant_image: Couldn't get temp memory.","\p",-1);

  return -1;
 } 
 cut_top_left_x=0;
 cut_top_left_y=0;
// current_top_left_u=0;
// current_top_left_v=0;
 ix1=_x1; iy1=_y1;
 
 while (tiles_y>0)
  {

    
  tx=tiles_x;	//reset horiz cutter
   while (tx>0)
   {
    //cut to temp_texture
    cut_out(_rgba_image, temp_texture_rgba,cut_top_left_x, cut_top_left_y);
    //insert
    return_val=set_as_constant_image1(temp_texture_rgba, ix1, iy1, ix1+256, iy1+256, unique_tile_id);
    
    tx--;
    ix1+=256;
    cut_top_left_x+=256;

   }
   ix1=_x1;
   iy1+=256;
   tiles_y--;
   cut_top_left_x=0;
   cut_top_left_y+=256;
  }
  DisposePtr((Ptr) temp_texture_rgba);
 }
return return_val;
}

//returns 1 if so
int is_a_factor_of_2(int number)
{
int i,bit_count;

bit_count=0;
for (i=0;i<31;i++)
   {
   if ((number&1)==1) bit_count++;
   number=number>>1;
   }
if (bit_count>1 || bit_count==0) return 0;
else return 1;	
}

//cuts 256*256 pixels out of source into dest
void cut_out (LSRAW* source, LSRAW* destination, int source_start_x, int source_start_y)
{
//int source_x, source_y;
int source_width;
int next_line_offset;
unsigned int * source_data;
unsigned int * source_reset;

unsigned int * dest_data;
unsigned int the_rgba,the_rgba_zero;
int x,y,width_pos,h_pos;

destination->pwidth=256; destination->pheight=256;
source_width=source->pwidth;
next_line_offset=source_width-(source_start_x+256);	//offset ot start of next line after we have cut this one (in unsigned ints)

source_data=(unsigned int *)(char *)&source->raw_data;
source_data+=source_width*source_start_y;
source_data+=source_start_x;	//ptr maths remember Stu.
dest_data=(unsigned int *)(char *)&destination->raw_data;

source_reset=source_data;

width_pos=source_start_x;
h_pos=source_start_y;

the_rgba_zero=0;
for (y=0;y<256;y++)
  {
    for (x=0;x<256;x++)
     {
     
     if (width_pos>=source->pwidth || h_pos>=source->pheight)
     {	//fill with transparent
      *dest_data=the_rgba_zero;
      dest_data++;
     }
     else //copy data
     {
     the_rgba=(*source_data);
     *dest_data=the_rgba;
     
     dest_data++;
     source_data++;
     }     
     
     width_pos++;
     }  
  h_pos++;  
  source_data=source_reset;
  source_data+=source_width;
  source_reset=source_data;
//  source_data+=next_line_offset;   
  width_pos=source_start_x;
  }


}

// -1 returned means no constant image assigned
int set_as_constant_image1(LSRAW *_rgba_image, int _x1, int _y1, int _x2, int _y2, int unique_id)
{
int my_glname;
int err;
int constant_image_index, height;
Ptr raw_rgba_data;

// lets try to find an inactive slot
constant_image_index=0; 
height=_y2-_y1;



while(constant_image_array[constant_image_index].slot_active == ACTIVE)
  {
  if(constant_image_index==CONSTANT_IMAGE_MAX)
    {
    return -1; // are there too many constant images to process??
    }
  constant_image_index++;
  }
  

// lets fill up our private array
constant_image_array[constant_image_index].rgba_image=_rgba_image;
constant_image_array[constant_image_index].x1=_x1;
constant_image_array[constant_image_index].y1=_y1;
constant_image_array[constant_image_index].x2=_x2;
constant_image_array[constant_image_index].y2=_y2;

constant_image_array[constant_image_index].output_points[0].x=((float)_x1-screen_cent_x)/screen_cent_x;
constant_image_array[constant_image_index].output_points[0].y=((float)_y1-screen_cent_y)/screen_cent_y;
constant_image_array[constant_image_index].output_points[0].u=0;
constant_image_array[constant_image_index].output_points[0].v=0;
constant_image_array[constant_image_index].output_points[1].x=((float)_x1-screen_cent_x)/screen_cent_x;
constant_image_array[constant_image_index].output_points[1].y=((float)_y2-screen_cent_y)/screen_cent_y;
constant_image_array[constant_image_index].output_points[1].u=0;
constant_image_array[constant_image_index].output_points[1].v=1;
constant_image_array[constant_image_index].output_points[2].x=((float)_x2-screen_cent_x)/screen_cent_x;
constant_image_array[constant_image_index].output_points[2].y=((float)_y2-screen_cent_y)/screen_cent_y;
constant_image_array[constant_image_index].output_points[2].u=1;
constant_image_array[constant_image_index].output_points[2].v=1;
constant_image_array[constant_image_index].output_points[3].x=((float)_x2-screen_cent_x)/screen_cent_x;
constant_image_array[constant_image_index].output_points[3].y=((float)_y1-screen_cent_y)/screen_cent_y;
constant_image_array[constant_image_index].output_points[3].u=1;
constant_image_array[constant_image_index].output_points[3].v=0;

constant_image_array[constant_image_index].tile_id=unique_id;

constant_image_array[constant_image_index].context=current_context;

constant_image_array[constant_image_index].slot_active=ACTIVE;

// construct a unique name
my_glname=get_unique_texture_name();
constant_image_array[constant_image_index].ci_name=my_glname;

// lets upload the image to GLgod
//	glEnable(GL_TEXTURE_2D);      

     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, _x2-_x1);
raw_rgba_data=(char *)&_rgba_image->raw_data;
  glBindTexture(GL_TEXTURE_2D, my_glname);	//we supply the name...

//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1+_x2-_x1, 1+_y2-_y1, 0, GL_RGBA,
//  GL_UNSIGNED_BYTE, _rgba_image);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _x2-_x1, height, 0, GL_RGBA,
  GL_UNSIGNED_BYTE, raw_rgba_data);

   #if ZEX_ANVIL==1	//Clamp to edge only on Mach-O
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   #else
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   #endif
   
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   err=aglGetError();
   if (err!=0) report_error ("OGL:set as ci: AGL error.","\p",err);

//set_constant_image_pos(constant_image_index, _x1, _y1);
return constant_image_index;
}



void draw_constant_images(void) // - calls texturedpoly, or recreates plot?
{
int my_glname;
int i;



i=0;		// start from the beginning
while(i<CONSTANT_IMAGE_MAX)	// inly do it if there are some constant images!
  {

  if(constant_image_array[i].context==current_context
   && 
     constant_image_array[i].slot_active==ACTIVE)
    {
//print_crunched32_trans_pict(constant_image_array[i].rgba_image,100,100);

//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    glPixelStorei(GL_UNPACK_ROW_LENGTH, 314);
////    glPixelStorei(GL_PACK_ALIGNMENT, 1); 
//
//    glRasterPos2f(int_to_ogl_x(100), int_to_ogl_y(100));
//    glDrawPixels(314,278,GL_RGBA, GL_UNSIGNED_BYTE,
//    constant_image_array[i].rgba_image);


    // get unique name
    my_glname=constant_image_array[i].ci_name;
  
    // physically draw this quaderlateral (That's a quadrilateral in english)
    OGL_Tpolygon  (4,
               constant_image_array[i].rgba_image->raw_data, /* texture address (not used) */
               constant_image_array[i].output_points,
               my_glname, /* name */
               0.5, /* redness */
               0.5, /* greeness */
               0.5, /* blueness */ 
               0 /* not translucent */ 
               );
     }
     i++;          
       
   } // end of while loop
   
}


//has to check for unique tile id's
void delete_constant_image(int constant_image_id)
{
GLuint the_name;
//int i;

if((constant_image_id<=-1) || (constant_image_id>=CONSTANT_IMAGE_MAX)) return; // protect against unassigned constant images

if (constant_image_array[constant_image_id].slot_active==INACTIVE) return;	 //this is an "oops" situation from main loop on first frame
// unload all images from the GLgod
if (OGL_active==1)
  {

  the_name=constant_image_array[constant_image_id].ci_name;
  glDeleteTextures(1,&the_name);
 

  }	//end of OGL active

// remove from the ci_array
constant_image_array[constant_image_id].slot_active=INACTIVE;

//texs loaded as a single tile have a tile id of -1
//tex's loaded as multiple tiles have a unique +ve id
if (constant_image_array[constant_image_id].tile_id!=-1)
 {
 int continue_delete,tile_id;
 //search for thers with similar tile id's 
  tile_id=constant_image_array[constant_image_id].tile_id; //cause it's about to get set to -1!
  continue_delete=1;
  while (continue_delete==1)
  {
  int result;
     result=kill_tile(tile_id);
     if (result==0) continue_delete=0;
  }
 }
}


//search const_images for something with the tile id
//return 1 if deleted else 0
int kill_tile(int tile_id)
{
int i;
GLuint the_name;

  i=0;		// start from the beginning
  while(i<CONSTANT_IMAGE_MAX)	  	  // inly do it if there are some constant images!
    {
    if (constant_image_array[i].tile_id==tile_id && constant_image_array[i].slot_active==ACTIVE)
     {
      the_name=constant_image_array[i].ci_name;
      glDeleteTextures(1,&the_name);
     // remove from the ci_array
      constant_image_array[i].slot_active=INACTIVE;
      constant_image_array[i].tile_id=-1;
      return 1;
     }
     i++;
    }
return 0;
}


void delete_all_constant_images(void)
{
//GLuint the_name;
int i;


// unload all images from the GLgod
if (OGL_active==1)
 {

  i=0;		// start from the beginning
  while(i<CONSTANT_IMAGE_MAX)	  	  // inly do it if there are some constant images!
    {
     delete_constant_image(i);
     i++;
    }

  }	//end of OGL active


// reinit the entire ci_array
i=current_context;
init_constant_image();
current_context=i;
}

extern int inv_pict_reference;	//from constant images or -1 if not set
extern int inv_cursor_reference;	//same
extern int cp_reference,short_range_reticule_ref,long_range_reticule_ref;



// init makes all constant images inactive
void init_constant_image(void)
{
int i;

for(i=0; i<CONSTANT_IMAGE_MAX; i++)
  {
  constant_image_array[i].slot_active=INACTIVE;
  constant_image_array[i].tile_id=-1;
  }
  
current_context=0; mouse_on=0;
 inv_pict_reference=-1;	//from constant images or -1 if not set
 inv_cursor_reference=-1;	//same
 cp_reference=-1;
long_range_reticule_ref=-1;
short_range_reticule_ref=-1;

}


void set_constant_image_pos(int constant_image_id, int x, int y)
{
//GLuint the_name;
//int i;
int the_width, the_height;
int _x1,_x2,_y1,_y2,constant_image_index;

if((constant_image_id<=-1) || (constant_image_id>=CONSTANT_IMAGE_MAX)) return; // protect against unassigned constant images

y=monitor_h-y;

constant_image_index=constant_image_id;	//I'm lazy and pasted the code below in



  the_width=constant_image_array[constant_image_id].x2-constant_image_array[constant_image_id].x1;
  the_height=constant_image_array[constant_image_id].y2-constant_image_array[constant_image_id].y1;
 
  _x1=x;
  _x2=x+the_width;
  _y1=y;
  _y2=y+the_height;

constant_image_array[constant_image_index].x1=_x1;
constant_image_array[constant_image_index].y1=_y1;
constant_image_array[constant_image_index].x2=_x2;
constant_image_array[constant_image_index].y2=_y2;

constant_image_array[constant_image_index].output_points[0].x=((float)_x1-screen_cent_x)/screen_cent_x;
constant_image_array[constant_image_index].output_points[0].y=((float)_y1-screen_cent_y)/screen_cent_y;
//constant_image_array[constant_image_index].output_points[0].u=0;
//constant_image_array[constant_image_index].output_points[0].v=0;
constant_image_array[constant_image_index].output_points[1].x=((float)_x1-screen_cent_x)/screen_cent_x;
constant_image_array[constant_image_index].output_points[1].y=((float)_y2-screen_cent_y)/screen_cent_y;
//constant_image_array[constant_image_index].output_points[1].u=0;
//constant_image_array[constant_image_index].output_points[1].v=1;
constant_image_array[constant_image_index].output_points[2].x=((float)_x2-screen_cent_x)/screen_cent_x;
constant_image_array[constant_image_index].output_points[2].y=((float)_y2-screen_cent_y)/screen_cent_y;
//constant_image_array[constant_image_index].output_points[2].u=1;
//constant_image_array[constant_image_index].output_points[2].v=1;
constant_image_array[constant_image_index].output_points[3].x=((float)_x2-screen_cent_x)/screen_cent_x;
constant_image_array[constant_image_index].output_points[3].y=((float)_y1-screen_cent_y)/screen_cent_y;
//constant_image_array[constant_image_index].output_points[3].u=1;
//constant_image_array[constant_image_index].output_points[3].v=0;



}



void swap_context(int context) /* 2 or 3 contexts, so we can swap between the game screen, and say a fixed image without 
	               reiniting the entire specific game ci array */
{
  current_context=context;
}

int get_context(void)
{
return current_context;
}

#endif

extern int inv_cursor_reference;
extern LSRAW *inv_cursor_picture_rgba;


//ogl mouse cursor support
void ogl_show_mouse_cursor()
{
    if (mouse_on==0)
    {
	if (inv_cursor_reference==-1)
	inv_cursor_reference = set_as_constant_image(inv_cursor_picture_rgba, 100, 100, 100+inv_cursor_picture_rgba->pwidth, 100+inv_cursor_picture_rgba->pheight);
    ZexGetMouse (&start_mouse_xy);
    mouse_on=1;
    }
}

#if OGL==1

void ogl_hide_mouse_cursor()
{
 if (inv_cursor_reference!=-1)	 //the inventory picture is not resident
 {
 delete_constant_image(inv_cursor_reference);
 inv_cursor_reference=-1;
 }
 mouse_on=0;
}

void ogl_update_mouse_cursor()
{
Point mouse_xy;
//we take it that the start_mouse_xy is the center of the screen - i.e. 320,240
//and take new positions relative to there

    ZexGetMouse (&mouse_xy);
    mouse_xy.h=(mouse_xy.h-start_mouse_xy.h)+screen_cent_x;
    mouse_xy.v=(mouse_xy.v-start_mouse_xy.v)+screen_cent_y;
    set_constant_image_pos(inv_cursor_reference,mouse_xy.h,mouse_xy.v);

}
#endif

#if OGL==1
/*
extern LSRAW* raw_rgba_data_CP;	//ptr to rgba control panel data

void ogl_load_control_panel()
{


	if (cp_reference==-1)
	cp_reference = set_as_constant_image(raw_rgba_data_CP, 0, 0, raw_rgba_data_CP->pwidth, 
    raw_rgba_data_CP->pheight);	//remember OGL origin is bottom left
}


void ogl_hide_control_panel()
{

 if (cp_reference!=-1)	//the inventory picture is not resident
 {
 delete_constant_image(cp_reference);
 cp_reference=-1;
 }
}

*/

void ogl_hide_radar_reticules()
{
extern int short_range_reticule_ref,long_range_reticule_ref;
//extern LSRAW* short_range_ret_rgba;
//extern LSRAW* long_range_ret_rgba;

       delete_constant_image(short_range_reticule_ref);
       short_range_reticule_ref=-1;   
       delete_constant_image(long_range_reticule_ref); 
       long_range_reticule_ref=-1;   

}



static AGLPixelFormat aglFormat;
static AGLContext aglContext;
static AGLDrawable aglWindow;
static int glShutdown; 
//static Boolean hasCompiledVertices;
//static Boolean hasPalettedTextures;
static GLint hasFullscreen;
//static int winwidth, winheight;

#if 0
Boolean OpenGLCreateDisplay (void)
{
	GLint attrib[5] = {  AGL_RGBA, 
							AGL_ACCELERATED,
							AGL_DOUBLEBUFFER,
							AGL_FULLSCREEN, /* useful only for 3dfx combined with Mesa */
							AGL_NONE };
//	GLint vMajor, vMinor;
	GLenum aglErr;
	GDHandle	theDevice;
	
	// See if the library is linked
	if ((void *)aglGetVersion == (void *)kUnresolvedCFragSymbolAddress) return false;
	
	// This must be the first agl call if we're using Mesa. It is ignored by the Apple library.
	
	// Look for a device which meets our needs, starting with the main one
	theDevice = GetMainDevice ();
	
	// First, look for a fullscreen, double-buffered device
	// This should pick a 3dfx card if Mesa is present
	attrib[2] = AGL_DOUBLEBUFFER;
	attrib[3] = AGL_FULLSCREEN;
	aglFormat = aglChoosePixelFormat (NULL, 0, attrib);
	
	// If that failed, look for the first accelerated device
	// On my machine, this picks the Rage II+
	if (aglFormat == NULL)
	{
		// The Rage II+ will accept the DOUBLEBUFFER param, but it can be orders of magnitude slower
//		attrib[2] = AGL_NONE;
		attrib[3] = AGL_NONE;
	
		// iterate through all the available monitors, starting with the main one
		do
		{
			aglFormat = aglChoosePixelFormat (&theDevice, 1, attrib);

			// If we've got one, bust out while theDevice is still valid
			if (aglFormat) break;

			theDevice = GetNextDevice (theDevice);
		}
		while (theDevice != NULL);
	}
	
//	CheckAGLError ("aglChoosePixelFormat");
	if (aglFormat == NULL)
	{
	report_error ("OGL: Couldn't find a suitable OpenGL display.","\p",-1);
		return false;
	}

	aglDescribePixelFormat (aglFormat, AGL_FULLSCREEN, &hasFullscreen);

	// Create an AGL context
	aglContext = aglCreateContext (aglFormat, NULL);
//	CheckAGLError ("aglCreateContext");
	if (aglContext == NULL)
	{
//		aglDestroyPixelFormat (aglFormat);
//		SysBeep (0);
	report_error ("OGL: Couldn't create OGL context!","\p",-1);
		return false;
	}

	if (!hasFullscreen)
	{
		// Create a window for this context
		aglWindow = open_ogl_window();	//(AGLDrawable)NewCWindow (NULL, &(*theDevice)->gdRect, "\pOpenGL Output", true, plainDBox, (WindowRef)-1, false, 0);

		// Attach the context to the window
		if (!aglSetDrawable (aglContext, aglWindow))
		{
			GLboolean result;
		
//			aglDestroyPixelFormat (aglFormat);
			result = aglDestroyContext (aglContext);
			DisposeWindow ((WindowRef)aglWindow); aglWindow = NULL;
//			SysBeep (0);
	report_error ("OGL: Couldn't attach OGL context to window.","\p",-1);
			return false;
		}

		winwidth = (*theDevice)->gdRect.right - (*theDevice)->gdRect.left;
		winheight = (*theDevice)->gdRect.bottom - (*theDevice)->gdRect.top;
	}
	else
	{
		long glideGamma;
		
		/* Disable screen takeover */
//		aglDisable (aglContext, AGL_3DFX_TAKEOVER);
	
		/* We scale up if the vertical size is > 410 so that games that are borderline
	   	in height, e.g. Tapper at exactly 480, can make use of bilinear filtering. */
//	  	  if (Machine->drv->visible_area.max_y - Machine->drv->visible_area.min_y > 410)
//	  	  {
//	  	  	  winwidth = 800;
//	  	  	  winheight = 600;
//	  	  }
//	  	  else
//	  	  {
			winwidth = monitor_w;
			winheight = monitor_h;
//		}
		
		/* Attach the window to the context */
		/* TODO: figure out the proper deviceNum */
		aglErr = aglSetFullScreen (aglContext, winwidth, winheight, 60 /* frequency */, 1 /* deviceNum */ );
		if (!aglErr) return false;
	
		/* Enables the screen takeover */
//		aglEnable (aglContext, AGL_3DFX_TAKEOVER);
		
		/* Set the Glide gamma to 1.0 */
		glideGamma = (1.0) * 65536;
//		aglSetInteger (aglContext, AGL_3DFX_GAMMA_VALUE, &glideGamma);
	}

//	if (errorlog) fprintf (errorlog, "device found, width: %d, height: %d\n", winwidth, winheight);
	
	/* Make the context the current context */
	aglErr = aglSetCurrentContext (aglContext);
	if(!aglErr)
	{
//		SysBeep (0);
		return false;
	}

	/* Pixel format is no longer needed */
//	aglDestroyPixelFormat (aglFormat);

//	  aglGetVersion (&vMajor, &vMinor);
//	  if (errorlog)
//	  {
//	  	  fprintf (errorlog, "  AGL v%d.%d\n", vMajor, vMinor);
//	  	  fprintf (errorlog, "  Renderer: %s\n", glGetString (GL_RENDERER));
//	  	  fprintf (errorlog, "  Vendor: %s\n", glGetString (GL_VENDOR));
//	  	  fprintf (errorlog, "  Version: %s\n", glGetString (GL_VERSION));
//	  	  fprintf (errorlog, "  Extensions: %s\n", glGetString (GL_EXTENSIONS));
//	  }
//	  
//	  hasCompiledVertices = checkGLFeature ("GL_EXT_compiled_vertex_array");
//	  hasPalettedTextures = checkGLFeature ("GL_EXT_paletted_texture");
//	  
////	if (errorlog)
////	{
////		if (hasCompiledVertices)
////			fprintf (errorlog, "* hasCompiledVertices\n");
////		if (hasPalettedTextures)
////			fprintf (errorlog, "* hasPalettedTextures\n");
////	}
//	  
//	  CheckAGLError ("aglMakeCurrent");
//
//	  bytesPerPixel = scrbitmap->depth / 8;
//	  if (bytesPerPixel == 2)
//	  {
//	  	  glPixelType = GL_RGBA;
//	  	  glPixelSize = GL_UNSIGNED_SHORT;
////		  glPixelType = GL_RGB5_A1;
////		  glPixelSize = GL_UNSIGNED_SHORT;
////		  glPixelType = GL_COLOR_INDEX;
////		  glPixelSize = GL_UNSIGNED_BYTE;
//	  }
//	  else
//	  {
//	  	  glPixelType = GL_COLOR_INDEX;
//	  	  glPixelSize = GL_UNSIGNED_BYTE;
//	  }
//
//	  /* clear the buffer */
//	  glClearColor (0.0, 0.0, 0.0, 1);
//	  glClear (GL_COLOR_BUFFER_BIT);
//	  glFlush ();
//
//	  CheckGLError ("glInit stuff");
//
//#ifdef CABINET_SUPPORT
//	  if(!dodepth) cabview=0;
//#endif
//	  
//	  vscrnaspect = (GLfloat) winwidth / (GLfloat) winheight;
//
//	  {
//	  	  // Super-New-Improved Aspect Ratio Calculation
//	  	  int i;
//
//	  	  // 99% of games use a 4:3 monitor...
//	  	  scrnaspect = 4.0 / 3.0;
//	  	  
//	  	  i = 0;
//	  	  // check to see if this game is an exception to the rule
//	  	  while (nonStandardAspectList[i].aspect != 0)
//	  	  {
//	  	  	  if (strcmp (nonStandardAspectList[i].driverName, Machine->gamedrv->name) == 0)
//	  	  	  {
//	  	  	  	  scrnaspect = nonStandardAspectList[i].aspect;
//	  	  	  	  break;
//	  	  	  }
//	  	  	  i ++;
//	  	  }
//	  	  
//	  	  // swap the aspect if it is a vertical game or if it is a rotated horizontal game
//	  	  if (Machine->orientation & ORIENTATION_SWAP_XY)
//	  	  	  scrnaspect = 1.0 / scrnaspect;
//	  }
//
//	  if (!hasFullscreen)
//	  {
//	  	  // Set aside some fudge space for the rounded corners
//	  	  glViewport (8, 8, winwidth-16, winheight-16);
//	  }
//	  glShutdown = 0;
//
//#ifdef CABINET_SUPPORT
//	  if (cabview)
//	  	  SetupFrustum();
//	  else
//#endif
//	  	  SetupOrtho();
//
////	glShadeModel (GL_SMOOTH);
//	  glShadeModel (GL_FLAT);
//
//	  glEnable (GL_POLYGON_SMOOTH);
//  
//	  glEnable (GL_BLEND);
//	  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	  if (dodepth)
//	  	  glDepthFunc (GL_LEQUAL);
//	  else
//	  	  glDepthFunc (GL_NEVER);
//
//	  CheckGLError ("OpenGLCreateDisplay Init Stuff");
//
//	  if (scrnaspect < vscrnaspect)
//	  {
//	  	  vscrnheight = (GLfloat) winheight;
//	  	  vscrnwidth = vscrnheight * scrnaspect;
//	  	  vscrntlx = ((GLfloat) winwidth - vscrnwidth) / 2.0;
//	  	  vscrntly = 0.0;
//	  }
//	  else
//	  {
//	  	  vscrnwidth = (GLfloat) winwidth;
//	  	  vscrnheight = vscrnwidth / scrnaspect;
//	  	  vscrntlx = 0.0;
//	  	  vscrntly = ((GLfloat) winheight - vscrnheight) / 2.0;
//	  }
//
//#ifdef CABINET_SUPPORT
//	  if (cabspecified || !LoadCabinet (drivers[game_index]->name))
//	  {
//	  	  if (!LoadCabinet (cabname))
//	  	  {
//	  	  	  printf ("Unable to load cabinet %s\n", cabname);
//	  	  	  cabname = "glmame";
//	  	  	  LoadCabinet (cabname);
//	  	  }
//	  }
//
//	  /* Calulate delta vectors for screen height and width */
//	  DeltaVec (cscrx1,cscry1,cscrz1,cscrx2,cscry2,cscrz2, &cscrwdx,&cscrwdy,&cscrwdz);
//	  DeltaVec (cscrx1,cscry1,cscrz1,cscrx4,cscry4,cscrz4, &cscrhdx,&cscrhdy,&cscrhdz);
//#endif
//	  
//	  CheckGLError ("OpenGLCreateDisplay");

	return 0;
}

#endif


/* Close down the virtual screen */
void OpenGLCloseDisplay (void)
{
//	int x,y;

	if (glShutdown) return;
	glShutdown = 1;
	
	if (hasFullscreen)
	{
		/* Disables the screen takeover */
//		aglDisable (aglContext, AGL_3DFX_TAKEOVER);
	}

//	  if (hasPalettedTextures)
//	  {
//	  	  free (ctable);
//	  	  free (gamma_ctable);
//	  }
//	  else
//	  {
//	  	  free (rcolmap);
//	  	  free (bcolmap);
//	  	  free (gcolmap);
//	  }
//
//#ifdef CABINET_SUPPORT
//	  if (cpan)
//	  	  free (cpan);
//#endif
//
//	  /* Free Texture stuff */
//	  if (texgrid)
//	  {
//	  	  for (y = 0; y < texnumy; y++)
//	  	  {
//	  	  	  for (x = 0; x < texnumx; x++)
//	  	  	  {
//	  	  	  	  free (texgrid[y * texnumx + x].texture);
//	  	  	  }
//	  	  }
//	  	  free(texgrid);
//	  }
	
	aglDestroyPixelFormat (aglFormat);
	aglDestroyContext (aglContext);
	if (aglWindow) DisposeWindow ((WindowRef)aglWindow); aglWindow = NULL;
}
#endif



