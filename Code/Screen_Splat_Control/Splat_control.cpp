/*
 * Splat_control.cpp
 *
$Header: /home/ls_cvs/ZEX2.3/Code/Screen_Splat_Control/Splat_control.cpp,v 1.4 2003/09/28 18:04:29 robp Exp $
$Log: Splat_control.cpp,v $
Revision 1.4  2003/09/28 18:04:29  robp
Fixed filename too long (!) problem, and booked in project file.

Revision 1.3  2003/09/28 17:30:02  robp
Changed files from .c to .cpp and removed spaces out of a couple of filenames.

Revision 1.2  2003/09/27 21:52:41  robp
Fixed places where we were loading an unsigned with -1.

Revision 1.1.1.1  2003/09/05 22:36:18  stu_c
First Imported.

Revision 1.35  2002/09/08 02:06:42  stu_c
Precompiled header adjustments

Revision 1.34  2002/07/11 21:12:05  rob_c
Changes for display configuration

Revision 1.33  2002/07/07 03:45:38  stu_c
Lasers now work but need moving to POCB's

Revision 1.32  2002/07/04 22:42:03  stu_c
Added some debug switches for window centering and desktop occlusion

Revision 1.31  2002/06/30 04:23:40  stu_c
Added some vars needed when getting ogl info

Revision 1.30  2002/06/24 21:33:17  stu_c
Version upped to 0.72, start up spinning rectangle removed.

Revision 1.29  2002/06/04 16:28:22  stu
Display captures now only if pb build

Revision 1.27  2002/04/28 06:21:57  stu
This checkin adds more to the HUD and brings out some sim variables to the player.

Revision 1.26  2002/03/10 02:48:42  stu
Fix for suns/particles not being rendered

Revision 1.25  2002/01/29 23:15:39  stu
Our GDH now passed to all fade routines

Revision 1.24  2002/01/27 18:06:53  stu
Unused variables removed

Revision 1.23  2002/01/09 23:48:50  stu
Furhter work on trading.

Revision 1.22  2001/12/29 01:11:04  stu
Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.

Revision 1.21  2001/12/27 19:43:37  stu
#if SPLAT_CONTROL_LOG==1

Revision 1.20  2001/12/04 23:35:53  stu
Motherships work

Revision 1.19  2001/11/28 21:40:47  stu
Finish Eclipse so works on Non os x carbon as well.

Revision 1.18  2001/11/24 19:29:05  stu
Added some OGL info dump to the SysCon

Revision 1.17  2001/11/14 23:49:05  stu
Added some OGL info dump to the SysCon

Revision 1.16  2001/11/12 22:09:56  stu
Eclipse work

Revision 1.15  2001/11/12 22:00:17  stu
Eclipse

Revision 1.14  2001/11/11 21:47:51  rob
Fixes switch screen blank problem.

Revision 1.13  2001/11/09 18:28:44  rob
Various debug changes.

Revision 1.12  2001/11/07 22:30:05  stu
Hide dock mods.

Revision 1.11  2001/11/05 20:04:05  stu
More OSX work

Revision 1.10  2001/11/03 22:30:18  stu
Bundles

Revision 1.9  2001/11/03 14:28:29  stu
Further OS X work

Revision 1.8  2001/11/02 23:19:22  stu
Further OS X work. Enabled syscon logging.

Revision 1.7  2001/11/02 19:31:39  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.6  2001/10/27 18:55:58  rob
dual interface lib/carbon lib changes

Revision 1.5  2001/10/26 22:42:24  rob
Dual headers

Revision 1.4  2001/10/23 21:55:17  stu
More carbon work

Revision 1.3  2001/10/22 21:28:23  rob
Carbon warning changes

Revision 1.2  2001/10/21 17:42:39  stu
Carbonised

Revision 1.1  2001/10/21 01:06:37  stu
Initial porting work

Revision 1.0.0.1  2001/10/17 20:46:07  rob
First Imported.

*/


#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "options.h"		// should always be first include

  #include	"LS_Standard.h"
  #include	"Error_handler.h"
  #include	"zex_misc.h"
  #include "3d_structures_def.h"
#endif //pch

#include	"splat_control.h"
#include "Zsound.h"

#include "pictures.h"
#include	"screen_splats.h"
//#include    "fpoly4.ext"
#include "ogl.h"
#include "olg_text.h"

#include "file_io.h"
#include "physics.h"
#include "display_config_dialog.h"


#define MAXBUFFERS 2	//how many screen buffers we using - valid is 2 or 3

/* These from the MacOS X release of OGL!!!!
** Renderer ID numbers
*/

#define AGL_RENDERER_GENERIC_ID      0x00020200
#define AGL_RENDERER_ATI_ID          0x00021000



CGrafPtr open_ogl_window(void);

int run_ogl();
void DspGraphicsActive(void);
void do_describe_renderer(AGLPixelFormat fmt);



int monitor_w, monitor_h;
int screen_cent_x, screen_cent_y;
int off_640_x,off_480_y,win_off_x, win_off_y;
int ogl_depth;
int window_w, window_h,screen_res,window_res;
int show_dock;

DSpContextReference		mDisplayContext;
DSpContextAttributes	mDisplayAttributes;
GDHandle				mZexGDH;
CTabHandle	 	 	    mZexCLUT;
CGrafPtr	backBuff;
GDHandle				origGDH;

int Use_SpeedLimit;
int Use_VBL;
int Change_res;

int flash_screen=0;	//if 1 screen is erased to white rather than black

int splat_offset_x, splat_offset_y;	//video splat offsets also used in ZexGetMouse()
int vram_width;
Ptr vram_base;

extern int global_show_status;
extern int OGL_active;



//our backbuffers_array
typedef struct {
GWorldPtr TheGWorld;
char * buffer_address;
int buffer_width;
int pending;
} bbtype;

bbtype backbuffers_array[MAXBUFFERS];

int backbuffer_counter=0;	//used in get back buffer


//ZBUFFER
int use_zbuffer=-1;	//from ini file
int zbuffer_active=0;	//used in drawing. Switched on when z gets within 10Km's
float zbuffer_range=10000*ZEX_UNIT_SIZE_IN_METERS;	//2km's default zbuffer range, read from ini
int zbuffer_depth=16;	//depth of zbuffer in bits, read from ini

int force_software_renderer=0;

//our VBL queue
typedef struct {
char * dest_address;	//where splat to
int dest_width;		//width of destination
char * source_address;
int source_width;
int bb_array_index;	//where this backbuffer came from so we can clr the pending bit
} vblqueuetype;

vblqueuetype vbl_queue[MAXBUFFERS];

int vbl_queue_write=0;
int vbl_queue_read=0;

GrafPtr		        savePort;
GrafPtr		        origPort;
//Zex screen driver
//A vbl driven, double buffered with pixel oubling driver



AGLContext     ctx;

//static GLint hasFullscreen;
AGLDrawable aglWindow;
static 	AGLPixelFormat fmt;


int init_screen() 
{
int n;
#if SPLAT_CONTROL_LOG==1
show_text_in_loading_screen("Splat_control: Trying to select a context\n");	
#endif

GraphicsSelectContext();
#if SPLAT_CONTROL_LOG==1
show_text_in_loading_screen("Splat_control: Success\n");	
#endif


#if SPLAT_CONTROL_LOG==1

show_text_in_loading_screen("Splat_control: calling run_OGL\n");	
#endif

run_ogl();
#if SPLAT_CONTROL_LOG==1
show_text_in_loading_screen("Splat_control: Success\n");	
show_text_in_loading_screen("Splat_control: calling init_OGL_text\n");	
#endif
    init_ogl_text();
#if SPLAT_CONTROL_LOG==1
show_text_in_loading_screen("Splat_control: Success\n");	
#endif

   for (n=0;n<40; n++)
   {
   TestDrawGL();
   }
   
return 0;
}



void ogl_active()
{
run_ogl();
}



//destroy the OGL screen
void ogl_inactive()
{

//	aglSetCurrentContext(NULL);
 clearGL();

	aglSetDrawable(ctx, NULL);
	 if (aglWindow!=0)
	 {
	 DisposeWindow ((WindowRef)aglWindow); 
	 aglWindow = NULL;
	 aglWindow=0;
	 }
	 cleanupAGL();


     OGL_active=0;
}

int run_ogl()
{
#if OGL==1
extern int zbuffer_depth;
GLfloat Projection[16];
GLint swapInt=1;
// any renderer
	GLint          attrib16[] = { AGL_RGBA, AGL_DOUBLEBUFFER, AGL_DEPTH_SIZE, 16, AGL_NONE };
	GLint          attrib32[] = { AGL_RGBA, AGL_DOUBLEBUFFER, AGL_DEPTH_SIZE, 32, AGL_NONE };
// software renderer
	GLint          attrib16s[] = { AGL_RGBA, AGL_RENDERER_ID, AGL_RENDERER_GENERIC_ID, AGL_ALL_RENDERERS, AGL_DOUBLEBUFFER, AGL_DEPTH_SIZE, 16, AGL_NONE };
	GLint          attrib32s[] = { AGL_RGBA, AGL_RENDERER_ID, AGL_RENDERER_GENERIC_ID, AGL_ALL_RENDERERS, AGL_DOUBLEBUFFER, AGL_DEPTH_SIZE, 32, AGL_NONE };
//	GLint          attrib[] = { AGL_RGBA, AGL_DOUBLEBUFFER, AGL_FULLSCREEN, AGL_DEPTH_SIZE, 16, AGL_NONE };
//	GLint          attrib[] = { AGL_DOUBLEBUFFER, AGL_DEPTH_SIZE, 16, AGL_NONE };
//	GLint          attrib[] = {  AGL_NONE };

//	GLint          attrib[] = { AGL_RGBA,AGL_ACCELERATED, AGL_DOUBLEBUFFER, AGL_NONE };

// software renderer
//	GLint          attrib[] = { AGL_RGBA, AGL_RENDERER_ID, AGL_RENDERER_GENERIC_ID, AGL_ALL_RENDERERS, AGL_DOUBLEBUFFER, AGL_NONE };

// OpenGL compliant ATI HWA renderer 
//	GLint          attrib[] = { AGL_RGBA, AGL_RENDERER_ID, AGL_RENDERER_ATI_ID, AGL_ACCELERATED, AGL_DOUBLEBUFFER, AGL_NONE };
 
// OpenGL compliant HWA renderer 
//	GLint          attrib[] = { AGL_RGBA, AGL_ACCELERATED, AGL_DOUBLEBUFFER, AGL_NONE };


	int err;
//    extern DSpContextReference		mDisplayContext;
	
	
	aglWindow = open_ogl_window();	//(AGLDrawable)NewCWindow (NULL, &(*mZexGDH)->gdRect, "\pOpenGL Output", true, plainDBox, (WindowRef)-1, false, 0);


	SetPort(GetWindowPort((WindowRef)aglWindow));
#if OGL_LOG==1
    show_text_in_loading_screen("run_ogl: Trying to get an OGL format \n");	
#endif

        #if ZEX_PROJECT_BUILDER==1
		if(force_software_renderer)
          {
          if(zbuffer_depth==32)
	    fmt = aglChoosePixelFormat(0, 0, attrib32s);	//32 bit zbuffer
          else
	    fmt = aglChoosePixelFormat(0, 0, attrib16s);	//16 bit zbuffer
          }
        else
          {
          if(zbuffer_depth==32)
	    fmt = aglChoosePixelFormat(0, 0, attrib32);	//32 bit zbuffer
          else
	    fmt = aglChoosePixelFormat(0, 0, attrib16);	//16 bit zbuffer
          }
 
	#else
		if(force_software_renderer)
          {
          if(zbuffer_depth==32)
	    fmt = aglChoosePixelFormat(&mZexGDH, 1, attrib32s);	//32 bit zbuffer
          else
	    fmt = aglChoosePixelFormat(&mZexGDH, 1, attrib16s);	//16 bit zbuffer
          }
        else
          {
          if(zbuffer_depth==32)
	    fmt = aglChoosePixelFormat(&mZexGDH, 1, attrib32);	//32 bit zbuffer
          else
	    fmt = aglChoosePixelFormat(&mZexGDH, 1, attrib16);	//16 bit zbuffer
          }
        #endif
		
	if(fmt==0) 
	{
#if OGL_LOG==1
    show_text_in_loading_screen("Splat_control: Couldn't init OpenGL (aglChoosePixelFormat).\nNo 3d hardware found. \n");	
#endif

	report_error("Couldn't init OpenGL (aglChoosePixelFormat).\nNo 3d hardware found.","\p",20000); 
	return -1;
	}
	

#if OGL_LOG==1
    show_text_in_loading_screen("run_ogl: Success \n");	
    show_text_in_loading_screen("run_ogl: Trying to get an OGL context \n");	
#endif

	/* Create an AGL context but dont activate it yet*/
	ctx = aglCreateContext(fmt, NULL);


	if(ctx==0) 
	{
#if OGL_LOG==1
    show_text_in_loading_screen("init_ogl: Couldn't init OpenGL (aglCreateContext). \n");	
#endif

	report_error("init_ogl: Couldn't init OpenGL (aglCreateContext).","\p",20001); 

	return -1;
	}

#if OGL_LOG==1
	
    show_text_in_loading_screen("run_ogl: Success \n");	
    show_text_in_loading_screen("run_ogl: Trying to set the context drawable \n");	
#endif

	if (!aglSetDrawable (ctx, GetWindowPort((WindowRef)aglWindow)))
		{
			GLboolean result;
		
			result = aglDestroyContext (ctx);
			DisposeWindow ((WindowRef)aglWindow); aglWindow = NULL;

            show_text_in_loading_screen("OGL: Couldn't attach OGL context to window. \n");	
	        report_error("OGL: Couldn't attach OGL context to window","\p",20002); 

			return -1;
		}
#if OGL_LOG==1
    show_text_in_loading_screen("run_ogl: Success \n");	
    show_text_in_loading_screen("run_ogl: Trying to set the context current \n");	
#endif

	/* Make the context the current context */
	err = aglSetCurrentContext(ctx);
	if(err==0) 
	{
	err=aglGetError();
	report_error("ogl_active: Couldn't activate OpenGL (aglSetCurrentContext). Error.","\p",err); 

	return -1;
	}

#if OGL_LOG==1
    show_text_in_loading_screen("run_ogl: Success \n");	
#endif

    do_describe_renderer(fmt);
	/* Pixel format is no longer needed */
	aglDestroyPixelFormat(fmt);

	err=aglGetError();
    if(err)
	{
	 report_error("ogl_active: Couldn't activate OpenGL. Error number:","\p20003",err); 
    }


    glDisable (GL_BLEND);	//off initially
//    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);	    	    	    	    	    // Set The Blending Function For Translucency

	if(use_zbuffer==0)	//from zex.ini
        {
         glDepthFunc (GL_ALWAYS);
         glDisable(GL_DEPTH_TEST);
        }
        else
        { 
         glDisable(GL_DEPTH_TEST);
         glDepthFunc (GL_LESS);	//accept pixel if z is less than current
	//glClearDepth(.01);
         }


// *	  glMatrixMode(GL_PROJECTION);
// *	  glLoadIdentity();
// *	  glOrtho(0.0f, (GLfloat) window_w, 0.0f, (GLfloat) window_h, 0.0f, 1.0f);
// *	  glMatrixMode(GL_MODELVIEW);
// *	  glLoadIdentity();
// *	  glViewport(0, 0, width, height);
// *      glDepthRange(0.0f, 1.0f);

 
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
// 	glOrtho(-1.0f, (GLfloat) 1/*window_w*/, -1.0f, (GLfloat) 1 /*window_h*/, 0.0f, -1.0f);
//        glDepthRange(0.0f, 1.0f);


//        glFrustum (-1.0, 1.0, 1.0, -1.0, /* transformation */
//            0.0, 0.9);


    glGetFloatv(GL_PROJECTION_MATRIX, Projection);
    
    
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
//        glMatrixMode(GL_MODELVIEW);
    
	glViewport(0, 0, window_w, window_h);
	//glFrustum(-1,1,-1,1,0.01,1.0);
	
	monitor_w=window_w; monitor_h=window_h;
        //if (Use_VBL==1)
        //vbl always on now
        {
	  aglSetInteger (ctx, AGL_SWAP_INTERVAL, &swapInt);	//sync to vbl
        }
        glOrtho( 0 /* left */, (GLfloat) (window_w-1) /* right */, (GLfloat) (window_h-1) /* bottom */, 
            0 /* top */ , 0 /* znear */ , -1 /* zfar */ ); // SB 230602 - make OGL map real coords 
                                                           // rather than +-1

    OGL_active=1;
    

#if OGL_LOG==1
    show_text_in_loading_screen("run_ogl: OpenGL active. \n");	
#endif


	return 0;
#endif
}



void do_describe_renderer(AGLPixelFormat fmt)
{
#if OGL_LOG==1
GLint vMajor, vMinor, value;
GLboolean result;


#if ZEX_PROJECT_BUILDER==1 && PB_PRINT_TO_LOG_ENABLED //can only printf to OSX syscon
	       aglGetVersion (&vMajor, &vMinor);
	  	  fprintf (stderr, "  AGL v%d.%d\n", (int)vMajor, (int)vMinor);
	  	  fprintf (stderr, "  Renderer: %s\n", glGetString (GL_RENDERER));
	  	  fprintf (stderr, "  Vendor: %s\n", glGetString (GL_VENDOR));
	  	  fprintf (stderr, "  Version: %s\n", glGetString (GL_VERSION));
	  	  fprintf (stderr, "  Extensions: %s\n", glGetString (GL_EXTENSIONS));
#endif
#endif


#if OGL_LOG==1

result = aglDescribePixelFormat ( fmt, AGL_ACCELERATED, &value );
if (result==FALSE)     show_text_in_loading_screen("run_ogl:do_describe_renderer: aglDescribePixelFormat(AGL_ACCELERATED) failed. \n");	
else
{

if (value==TRUE)     
   show_text_in_loading_screen("run_ogl:do_describe_renderer: OpenGL has HW acceleration. \n");	
else
   show_text_in_loading_screen("run_ogl:do_describe_renderer: OpenGL is using software renderer. \n");	 
}
#endif



#if OGL_LOG==1

#if ZEX_PROJECT_BUILDER==1 && PB_PRINT_TO_LOG_ENABLED //can only printf to OSX syscon
result = aglDescribePixelFormat ( fmt, AGL_DEPTH_SIZE, &value );
if (result==FALSE)     show_text_in_loading_screen("run_ogl:do_describe_renderer: aglDescribePixelFormat(AGL_DEPTH_SIZE) failed. \n");	
else
{
   fprintf(stderr,"run_ogl:do_describe_renderer: OpenGL depth buffer depth = %d bits\n", (int)value);	
}

result = aglDescribePixelFormat ( fmt, AGL_DOUBLEBUFFER, &value );
if (result==FALSE)     show_text_in_loading_screen("run_ogl:do_describe_renderer: aglDescribePixelFormat(AGL_DOUBLEBUFFER) failed. \n");	
else
{
   fprintf(stderr,"run_ogl:do_describe_renderer: OpenGL double buffered = %d\n", (int)value);	
}
#endif //ZEX_PROJECT_BUILDER
#endif


}



// ==================================================================
// Keep track of Dsp active state for report error
//
int draw_sprockets_is_active_module_variable=FALSE;


void set_drawsprockets_active_flag(void);
void set_drawsprockets_inactive_flag(void);



int drawsprockets_is_running(void)
{
return(draw_sprockets_is_active_module_variable);
}


void set_drawsprockets_active_flag(void)
{
draw_sprockets_is_active_module_variable=TRUE;
}


void set_drawsprockets_inactive_flag(void)
{
draw_sprockets_is_active_module_variable=FALSE;
}

// ==================================================================

void GraphicsSelectContext()
{

//ColorSpec	trueGreen	= { 0, 0x0000, 0xFFFF, 0x0000 };
OSStatus		theError;
DisplayIDType	displayID;
int wanted_width, wanted_height;


GetPort(&origPort);

theError=DSpStartup();
if(theError) MyDebugStr(theError,"DrawSprocket Startup2","Couldn't start up Drawsprockets!");      /* process the error */

DSpSetDebugMode(1);

wanted_width=monitor_w;
wanted_height=monitor_h;	//zex native resolution


	//А	Initialize our context description
	GraphicsInitAttributes(&mDisplayAttributes);

	//А	Tell DrawSprocket the exact needs we have for our game
	mDisplayAttributes.displayWidth		            	= wanted_width; //640;
	mDisplayAttributes.displayHeight		= wanted_height; //480;
	mDisplayAttributes.colorNeeds		            	= kDSpColorNeeds_Require;
	mDisplayAttributes.backBufferDepthMask	      = STATIC_CAST_TO_UNSIGNED(kDSpDepthMask_All);
	mDisplayAttributes.displayDepthMask		    = STATIC_CAST_TO_UNSIGNED(kDSpDepthMask_All);
	mDisplayAttributes.backBufferBestDepth	      = ogl_depth;
	mDisplayAttributes.displayBestDepth		    = ogl_depth;
	mDisplayAttributes.pageCount			= 1;	//not used any more....
	mDisplayAttributes.colorTable		            	= 0;


        #if USE_DISLAY_CONFIGURATION_SETTINGS
          theError = DSpFindBestContextOnDisplayID(&mDisplayAttributes, &mDisplayContext, get_displayID_for_selected_context());
        #else
//    mDisplayAttributes.frequency   = Long2Fix(67);
	//	  Find the best context for these attributes
	//	  This call will check to see if multiple monitors can handle the request
	//	  If there is more than one valid device it will put up the selection interface
	  theError = DSpFindBestContext(&mDisplayAttributes, &mDisplayContext); //enable for release
//	theError = DSpUserSelectContext(&mDisplayAttributes, 0L, nil, &mDisplayContext); //disable for release
        #endif

	if (theError)
	        {
	        if(theError==kDSpContextNotFoundErr)	// if user pressed cancel
	          { 
	          ExitToShell(); /* can we do this once DSpStartup has been called? */ 
	          }
	        else
	          { 
	          report_error("A suitable display could not be found","\p",theError);
	          //	DebugStr("\pA suitable display could not be found.");
	          }
			}

//	mDisplayAttributes.contextOptions |= kDSpContextOption_PageFlip; //if possible give us hw page flipping
//	mDisplayAttributes.contextOptions |= kDSpContextOption_PageFlip; //if possible give us hw page flipping
//	mDisplayAttributes.contextOptions |= kDSpContextOption_DontSyncVBL; //dont sync to vbl
	
	//	  Set the blanking color to black
	DSpSetBlankingColor(&rgbBlack);

	//	  Find the DisplayID for this draw context and then find the GDevice
	//	  that goes along with it.  We need this because if we need to perform
	//	  QuickDraw operations in a context that isn't on the main monitor then
	//	  we need to have the proper GDevice set or QuickDraw dies.
	theError =  DSpContext_GetDisplayID(mDisplayContext, &displayID);
	if (theError)
		MyDebugStr(__LINE__,"Splat_control.c","DSpContext_GetDisplayID() had an error.");

	theError = DMGetGDeviceByDisplayID(displayID, &mZexGDH, false);
	if (theError)
		MyDebugStr(__LINE__,"Splat_control.c","DMGetGDeviceByDisplayID() had an error.");

	//	  Reserve the draw context so that we get the attributes we want
	theError = DSpContext_Reserve(mDisplayContext, &mDisplayAttributes);
	if (theError)
		MyDebugStr(__LINE__,"Splat_control.c","Could not reserve display context.");

  	  // Fade the screen to black and make our context activ
	DSpContext_FadeGammaOut(mDisplayContext, NULL);	
	DspGraphicsActive();
//	DSpContext_SetCLUTEntries (mDisplayContext, &trueGreen,9,1);	//fix to get green
	DSpContext_FadeGammaIn(mDisplayContext, NULL);

	set_drawsprockets_active_flag();
}





void TestDrawGL(void)
{
	static float red=0.5, green=0.5, blue=1;

	glClearColor(red,green, blue, 1.0f);	     	     	     	     	     // Clear color buffer to dark grey
        glClear(GL_COLOR_BUFFER_BIT);
ZMoveTo(10,20);

#if ZEX_PROJECT_BUILDER==1
 #if TEST_SYSTEM==0
 ZDrawString("\pZex V0.73 Mach-O Loading. Running from ZD3");
 #else
 ZDrawString("\pZex V0.73 Mach-O Loading. Running from ZD3_test");
 #endif
#elif !INTERFACE_LIB_BUILD
 #if TEST_SYSTEM==0
 ZDrawString("\pZex V0.73 PEFCarb Loading. Running from ZD3");
 #else
 ZDrawString("\pZex V0.73 PEFCarb Loading. Running from ZD3_test");
 #endif
#else // interface lib
 #if TEST_SYSTEM==0
 ZDrawString("\pZex V0.73 IFLib Loading. Running from ZD3");
 #else
 ZDrawString("\pZex V0.73 IFLIb Loading. Running from ZD3_test");
 #endif
#endif //pb
	
   ZMoveTo(monitor_w/2-30,monitor_h/2);
   ZDrawString("\pPlease wait...");
	
   aglSwapBuffers(ctx);
}


void clearGL()
{
//cleqr both buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	     	     	     	     	     // Clear color buffer to dark grey
	glClear(GL_COLOR_BUFFER_BIT);
		
	aglSwapBuffers(ctx);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	     	     	     	     	     // Clear color buffer to dark grey
	glClear(GL_COLOR_BUFFER_BIT);
		
	aglSwapBuffers(ctx);

}

CGrafPtr open_ogl_window(void)
{
	WindowRef portptr_out;
	Rect rectWin;
	GrafPtr pGrafSave;
	RGBColor rgbSave;	 
	Rect portRect;              /*ее new variable to hold the value of  */
	// note: OpenGL is expecting a window so it can enumerate the devices it spans, 
	// center window in our context's gdevice
	#if ALIGN_OGL_WIN_CENTRE==1
        rectWin.top  = (short) ((**mZexGDH).gdRect.top + ((**mZexGDH).gdRect.bottom - (**mZexGDH).gdRect.top) / 2);		// h center
	rectWin.top  -= (short) (window_h / 2);
	rectWin.left  = (short) ((**mZexGDH).gdRect.left + ((**mZexGDH).gdRect.right - (**mZexGDH).gdRect.left) / 2);	// v center
	rectWin.left  -= (short) (window_w / 2);
	rectWin.right = (short) (rectWin.left + window_w);
	rectWin.bottom = (short) (rectWin.top + window_h);
        #else //align right=1 -- debug only
        rectWin.top  = (short) (**mZexGDH).gdRect.top+30;		// top plus menu height
	rectWin.left  = (short) ((**mZexGDH).gdRect.right - window_w)-4;	// align right
	rectWin.right = (short) (rectWin.left + window_w);
	rectWin.bottom = (short) (rectWin.top + window_h);
        #endif //left_win
        	
	portptr_out = NewCWindow (NULL, &rectWin, "\p", 0, plainDBox, (WindowPtr)-1, 0, 0);

	ShowWindow(portptr_out);
	GetPort (&pGrafSave);
	SetPort ((GrafPtr)GetWindowPort(portptr_out));
	GetForeColor (&rgbSave);
	RGBForeColor (&rgbBlue);
	#if ZEX_CORE_CARBON==1
	GetPortBounds(GetWindowPort(portptr_out), &portRect); /* new accessor added */
	PaintRect (&portRect);
	#else
	PaintRect (&portptr_out->portRect);
	#endif
	
	RGBForeColor (&rgbSave);		// ensure color is reset for proper blitting
//    SetPort(GetWindowPort((WindowRef)pGrafSave));
	SetPort (pGrafSave);


	return (CGrafPtr) portptr_out;
}





void cleanupAGL()
{   
#if OGL==1
    if (ctx!=0)
    {
//     DebugStr("\pcleanupAGL");
	 aglDestroyContext(ctx);
	 ctx=0;	//little note for myself
     OGL_active=0;
    }
#endif
}
//Heavy mods - SB 17 05 99 to use our own splat routine


//returns the backbuffer as a CGrafPtr
CGrafPtr Get_back_buffer(void) {
 MyDebugStr(__LINE__,"Splat_control.c","GetBackBuffer");
 return 0;

}



//A routine ShowIt places the current backbuffer GWorldPtr into our vbl_queue along with a queue serial number and it's backbuffer index and 
//sets the pending flag in backbuffer_array. If the current slot in the vbl queue is in-use, it waits 9although this is impossoble
//due to the blocking on the serving of backbuffers - see next line).
//
//The pending flag is used in getbackbuffer to determine if the buffer is available to be served up.
//If not it waits.
//
//
//The vbl queue is maintained by two variables - vbl_queue_write and vbl_queue_read. These can range between 0 and MAXBUFFERS.
//write is the index for insertion and read is the index for splatting.
//
//Showit then increments/rolls over backbuffer_counter

int graphics_frozen_flag=0;

void Show_it(void)
{
ZexPseudoVBLProc();
if(graphics_frozen_flag) return;

OGL_show_it();
}





int vbl_block=0;	//set to 1 if we are procesing a vbl task
int exec_watchdog=100;	//if this gets to zero we exec out
int gCheckWatchDog;


Boolean ZexPseudoVBLProc ()
{
extern int gFadeMusic;
//DeferredTask vbl_task;
//RoutineDescriptor myRD = BUILD_ROUTINE_DESCRIPTOR (uppDeferredTaskProcInfo, (ProcPtr) deferred_vbl_proc);
// 
//vbl_task.qType= dtQType; //{set the queue type}
//vbl_task.dtAddr = &myRD; //{set address of deferred task}
//vbl_task.dtParam = 0; //{no parameter needed here}
//vbl_task.dtReserved = 0;
//DTInstall (&vbl_task);

//080699 - Exec watchdog timer (this isn't the fill watchdog)
//sys_watchdog should be set to 100 in the main loop somewhere.
//this code decs it and if it gets to zero throws the exec error
#if USE_EXEC_WATCHDOG
if (gCheckWatchDog==1)
{
 exec_watchdog--;
 if (exec_watchdog<0) 
 {
 MyDebugStr(__LINE__,"Splat_control.c","Zex Executive WatchDog Fired! G to continue into error handler (May not be safe!)");
 report_error("Zex Executive WatchDog Fired! Please report this to LSG as a bug.","\p",3);
 }
}
#endif

if (vbl_block==0)	//just incase we get called before we're finished executing
{
vbl_block=1;
//check for music fade out
if (gFadeMusic==1)
   {
     do_fade_out_music();
   }

vbl_block=0;

}	//end of vbl_block
   
return 0;

}



void screen_quit() 	//Call before quitting game
{
//unsigned char OutBusyFlag;
//	while (OutBusyFlag!=0)
//	{
//	DSpContext_IsBusy(mDisplayContext,&OutBusyFlag);
//	}
ogl_inactive();
//	SetGDevice(origGDH);	//these are here so I can SEE them!
	SetPort(origPort);
	
	DSpContext_FadeGammaOut(mDisplayContext, NULL);
	DSpContext_SetState(mDisplayContext, kDSpContextState_Inactive);
	ShowCursor();
	DSpContext_FadeGammaIn(mDisplayContext, NULL);

	DSpContext_Release(mDisplayContext);
	DSpShutdown();

        set_drawsprockets_inactive_flag();
	#if !INTERFACE_LIB_BUILD	
           ShowMenuBar();
	#endif
	
}



void GraphicsInitAttributes(DSpContextAttributes *inAttributes)
{
	if (! inAttributes)
		MyDebugStr(__LINE__,"Splat_control.c","Nil context passed to GraphicsInitAttributes().");

   inAttributes->frequency=0;
   inAttributes->displayWidth=0;
   inAttributes->displayHeight=0;
   inAttributes->reserved1=0;
   inAttributes->reserved2=0;
   inAttributes->colorNeeds=0;
   inAttributes->colorTable=NULL;
   inAttributes->contextOptions=0;
   inAttributes->backBufferDepthMask=0;
   inAttributes->displayDepthMask=0;
   inAttributes->backBufferBestDepth=0;
   inAttributes->displayBestDepth=0;
   inAttributes->pageCount=0;
   inAttributes->gameMustConfirmSwitch=false;
   inAttributes->reserved3[0]=0;
   inAttributes->reserved3[1]=0;
   inAttributes->reserved3[2]=0;
   inAttributes->reserved3[3]=0;

   inAttributes->filler[0]=0;
   inAttributes->filler[1]=0;
   inAttributes->filler[2]=0;


//	  BlockZero(inAttributes, sizeof (DSpContextAttributes));
}

void DspGraphicsActive(void)
{
#if DESKTOP_BLANKING==1
OSStatus	theErr;
#endif
//	SetGDevice(origGDH);	//these are here so I can SEE them!
//	SetPort(origPort);

	HideCursor();

#if ZEX_PROJECT_BUILDER==1
#if CAPTURE_DISPLAYS==1 //in 3d_structures.def
	CGCaptureAllDisplays();
#else
    fprintf(stderr, "CAPTURE_DISPLAYS==0. Display not captured, other applications windows may move...\n");
#endif

#endif


	#if DESKTOP_BLANKING==1
        theErr = DSpContext_SetState(mDisplayContext, kDSpContextState_Active);
	if (theErr)
		MyDebugStr(__LINE__,"Splat_control.c","SetState active failed in GraphicsActive().");
        #else
      
         #if ZEX_PROJECT_BUILDER==1
              fprintf(stderr, "DESKTOP_BLANKING=0. Desktop not blanked out.\n");
        
         #endif
        #endif
}


int remove_opengl_context_on_switch=0;	// don't remove by default - no reason

void GraphicsActive(void)
{
OSStatus	theErr;
graphics_frozen_flag=0;
	HideCursor();

#if ZEX_PROJECT_BUILDER==1

	CGCaptureAllDisplays();
#endif


	theErr = DSpContext_SetState(mDisplayContext, kDSpContextState_Active);
	
	if (theErr)
		MyDebugStr(__LINE__,"Splat_control.c","SetState active failed in GraphicsActive().");

//	ShowWindow((WindowRef)aglWindow);    
        SetPort(GetWindowPort((WindowRef)aglWindow));

	SelectWindow((WindowRef)aglWindow);
		
        if(remove_opengl_context_on_switch)
          {
          if(aglSetDrawable (ctx, GetWindowPort((WindowRef)aglWindow))==GL_FALSE)
            { MyDebugStr(__LINE__,"Splat_control.c","aglSetDrawable failed"); }
          }
        
        if(aglUpdateContext(ctx)==GL_FALSE)
          { MyDebugStr(__LINE__,"Splat_control.c","aglUpdateContext failed"); }
 
}



//only called from Coffee
void GraphicsInactive(void)
{
OSStatus	theErr;
        
//        return;

	theErr = DSpContext_SetState(mDisplayContext, kDSpContextState_Inactive);
	if (theErr)
		MyDebugStr(__LINE__,"Splat_control.c","SetState inactive failed in GraphicsInactive().");
        
//	SetGDevice(origGDH);	//these are here so I can SEE them!
//	SetPort(origPort);
#if ZEX_PROJECT_BUILDER==1
    CGReleaseAllDisplays();
#endif

}



void GraphicsPaused(void)
{

graphics_frozen_flag=1;

	DSpContext_SetState(mDisplayContext, kDSpContextState_Paused);
        
        if(remove_opengl_context_on_switch)
          {
	  aglSetDrawable(ctx, NULL);
          }

	return;

}


void display_wait()
{
 user_delay(2);

}






//unused functions
void vid_to_8_bit()
{
SetDepth(mZexGDH,8,0,1);
}

void vid_to_16_bit()
{
SetDepth(mZexGDH,16,0,1);

}

//void vid_to_32_bit()
//{
//SetDepth(mZexGDH,32,0,1);
//
//}


// *******************Exec watchdog functions

void reset_watchdog()
{
exec_watchdog=700;	//made this 700 frames after running zex off a Zip disk!
}

void enable_watchdog()
{
exec_watchdog=700;
gCheckWatchDog=1;
}

void disable_watchdog()
{
gCheckWatchDog=0;
}
