/*
	File:		OpenGL DrawSprocket.cp

	Contains:	An example of OpenGL and DrawSprocket integration for full screen drawing.  
				Handles multiple displays and timing of swaps per second.
				
				The optimal way to select a rendering context would be to check the available devices, find the best,
				then present the user with a selction dialog only if there are more than one "best" device.
				Currently there is not a good mechanism to do this easily, so the two best other options are
				
				1) iterate throught the available devices find the best renderer, build a DSp context on this,
				then build your OpenGL context.  The downfall of this method is that you know nothing about the graphics
				capabilities of the device and the user can't select another screen.  The upside is that you get 
				a renderer that is has the capabilities you want.
				
				2) Allow the user to select a screen to use, build a DSp context and then a OpenGL context.  This
				second method is simpler but relies on user knowing which device is the best at 3D.
				
				This sample illustrates the second method which makes it simpler and allows us to illustrate the
				DSp/OpenGL interface well.				

	Written by:	Geoff Stahl
				original code by John Stauffer

	Copyright:	1999 Apple Computer, Inc., All Rights Reserved

	Change History (most recent first):

         <6>     11/24/99   GGS    	Added better error checking, now all contexts built on window, put gl headers in system path, 
         							added agl font support, added averaged frame rate, cleaned up code
         <6>     9/14/99    GGS    	Corrected buffer rect handling and cleaned up code
         <5>     7/14/99    GGS    	Fixed multi-monitor window centering
         <4>     7/13/99    GGS    	Add work around for over zealous checking in single buffer DSp context attributes
         <3>     7/5/99     GGS    	Now correctly handle multi-monitor (DSp front buffer for single device; Window on top of context for multiple devices)
         <2>     5/28/99    GGS     Added better multi-monitor support, clean code, corrected blanking bug, added timing, correct pixel formats
         <1>        ?        ?      Initial build

	This code can be built for DrawSprocket 1.7.  Enable the #define kDSp17 below to add this.

  	You may incorporate this sample code into your applications without
  	restriction, though the sample code has been provided "AS IS" and the
  	responsibility for its operation is 100% yours.  However, what you are
  	not permitted to do is to redistribute the source as "DSC Sample Code"
  	after having made changes. If you're going to re-distribute the source,
 	we require that you make it clear in the source that the code was
    descended from Apple Sample Code, but that you've made changes.
    
*/

// Building with DrawSprocket 1.7
//#define kDSp17 1

#include <DrawSprocket.h>
#include <DriverServices.h>
#include <Files.h>
#include <Folders.h>
#include <Fonts.h>
#include <Resources.h>
#include <Sound.h>
#include <TextUtils.h>

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <agl.h>
#include <aglRenderers.h>
#include <gl.h>

//-----------------------------------------------------------------------------------------------------------------------

enum 
{
	kBitsPerPixel = 32,
	kContextWidth = 400,
	kContextHeight = 400
};

// globals ---------------------------------------------------------------------------------------------------------------

const RGBColor	rgbBlack	= { 0x0000, 0x0000, 0x0000 };

NumVersion gVersionDSp;
DSpContextAttributes gTheContextAttributes;
DSpContextReference gTheContext;
GLuint gFontList;
char gcstrMode [256] = "";


// function prototypes ---------------------------------------------------------------------------------------------------

void CToPStr (StringPtr outString, const char *inString);
void ReportError (char * strError);
OSStatus DSpDebugStr (OSStatus error);
GLenum aglDebugStr (void);

CGrafPtr SetupDSp (GDHandle *hGD, short *numDevices);
void ShutdownDSp (CGrafPtr *ppWin);

AGLContext SetupAGL (GDHandle hGD, AGLDrawable win);
void CleanupAGL (AGLContext ctx);
void DrawGL (AGLContext ctx);

void DrawFrameRate (GLuint fontList);
void DrawContextInfo (char * cstrContextMode, GLuint fontList);

void DrawPSringGL (Str255 pstrOut, GLuint fontList);
void DrawCSringGL (char * cstrOut, GLuint fontList);
GLuint BuildFontGL (AGLContext ctx, GLint fontID, Style face, GLint size);
void DeleteFontGL (GLuint fontList);


//-----------------------------------------------------------------------------------------------------------------------

// main: setup, draw loop, frames per swap timing, clean up (exit with mouse click)

int main(void)
{
	CGrafPtr pWin;
	GDHandle hGD;
	short numDevices = 0;
	AGLContext ctx;
	short fNum;
	
	// Mac Init
	MaxApplZone ();
	InitGraf(&qd.thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(nil);
	InitCursor();
			
	pWin = SetupDSp (&hGD, &numDevices);						// Setup DSp for OpenGL
	if (!pWin) 
		return 0;
	ctx = SetupAGL(hGD, (AGLDrawable) pWin);					// Setup the OpenGL context
	if (!ctx) 
		return 0;
	if (hGD)
		sprintf (gcstrMode, "%d x %d x %d", pWin->portRect.right, pWin->portRect.right, (**(**hGD).gdPMap).pixelSize);
	else
		sprintf (gcstrMode, "%d x %d", pWin->portRect.right, pWin->portRect.right);
	
	GetFNum("\pMonaco", &fNum);									// build font
	gFontList = BuildFontGL (ctx, fNum, normal, 9);

	do {		
		DrawGL(ctx);											// OGL draw
	} while (!Button());

	DeleteFontGL (gFontList);
	CleanupAGL(ctx);											// Cleanup the OpenGL context
	ShutdownDSp (&pWin);										// DSp shutdown
	
	FlushEvents(everyEvent, 0);
	return 0;
}

#pragma mark -
//-----------------------------------------------------------------------------------------------------------------------

// Copy C string to Pascal string

void CToPStr (StringPtr outString, const char *inString)
{	
	unsigned char x = 0;
	do
		*(((char*)outString) + x) = *(inString + x++);
	while ((*(inString + x) != 0)  && (x < 256));
	*((char*)outString) = (char) x;									
}

// --------------------------------------------------------------------------

void ReportError (char * strError)
{
	char errMsgCStr [256];
	Str255 strErr;

	sprintf (errMsgCStr, "%s\n", strError); 

	// out as debug string
	CToPStr (strErr, errMsgCStr);
	DebugStr (strErr);
}

//-----------------------------------------------------------------------------------------------------------------------

OSStatus DSpDebugStr (OSStatus error)
{
	switch (error)
	{
		case noErr:
			break;
		case kDSpNotInitializedErr:
			ReportError ("DSp Error: Not initialized");
			break;
		case kDSpSystemSWTooOldErr:
			ReportError ("DSp Error: system Software too old");
			break;
		case kDSpInvalidContextErr:
			ReportError ("DSp Error: Invalid context");
			break;
		case kDSpInvalidAttributesErr:
			ReportError ("DSp Error: Invalid attributes");
			break;
		case kDSpContextAlreadyReservedErr:
			ReportError ("DSp Error: Context already reserved");
			break;
		case kDSpContextNotReservedErr:
			ReportError ("DSp Error: Context not reserved");
			break;
		case kDSpContextNotFoundErr:
			ReportError ("DSp Error: Context not found");
			break;
		case kDSpFrameRateNotReadyErr:
			ReportError ("DSp Error: Frame rate not ready");
			break;
		case kDSpConfirmSwitchWarning:
//			ReportError ("DSp Warning: Must confirm switch"); // removed since it is just a warning, add back for debugging
			return 0; // don't want to fail on this warning
			break;
		case kDSpInternalErr:
			ReportError ("DSp Error: Internal error");
			break;
		case kDSpStereoContextErr:
			ReportError ("DSp Error: Stereo context");
			break;
	}
	return error;
}

//-----------------------------------------------------------------------------------------------------------------------

// if error dump agl errors to debugger string, return error

GLenum aglDebugStr (void)
{
	GLenum err = aglGetError();
	if (AGL_NO_ERROR != err)
		ReportError ((char *)aglErrorString(err));
	return err;
}

#pragma mark -
//-----------------------------------------------------------------------------------------------------------------------

// Set up DSp screens, handles multi-monitor correctly

CGrafPtr SetupDSp (GDHandle *phGD, short *numDevices)
{
	OSStatus err = noErr;
	GDHandle hDevice;
	CGrafPtr pCGOut;
	DSpContextAttributes foundAttributes;
	DisplayIDType displayID;
	Rect rectWin;
	RGBColor rgbSave;
	GrafPtr pGrafSave;
	*numDevices = 0;

	// check for DSp
	if ((Ptr) kUnresolvedCFragSymbolAddress == (Ptr) DSpStartup) 
		ReportError ("DSp not installed");

	if (noErr != DSpDebugStr (DSpStartup()))
		return NULL;

#ifdef kDSp17
	if ((Ptr) kUnresolvedCFragSymbolAddress == (Ptr) DSpGetVersion) 
#endif
	{
		// must be 1.1.4 or earlier so look at extension
		short resFileSave, resFileDSp;
		short vRefDSp;
		long dirIDDSp;
		FSSpec fsSpecDSp;
		OSErr err;
		Handle hVersion;
		
		// assume 1.0 version to start with (worst case)
		gVersionDSp.majorRev = 1;
		gVersionDSp.minorAndBugRev = 0;
		gVersionDSp.stage = 0x80;
		gVersionDSp.nonRelRev = 0;

		resFileSave = CurResFile();
		SetResLoad (false);
		// search application directory for debug lib
		resFileDSp = OpenResFile ("\pDrawSprocketDebugLib");
		err = ResError ();
		if (fnfErr == err)
		{
			// search application directory for lib
			resFileDSp = OpenResFile ("\pDrawSprocketLib");
			err = ResError ();
			if (fnfErr == err)
			{
				// search extensions folder for debug lib
				FindFolder (kOnSystemDisk, kExtensionFolderType, kDontCreateFolder, &vRefDSp, &dirIDDSp);
				FSMakeFSSpec (vRefDSp, dirIDDSp, "\pDrawSprocketLib", &fsSpecDSp);
				resFileDSp = FSpOpenResFile (&fsSpecDSp, fsRdPerm);
				err = ResError ();
				if (fnfErr == err)
				{
					// search extensions folder directory for lib
					FSMakeFSSpec (vRefDSp, dirIDDSp, "\pDrawSprocketDebugLib", &fsSpecDSp);
					resFileDSp = FSpOpenResFile (&fsSpecDSp, fsRdPerm);
					err = ResError ();
				}
			}
		}
		// if we have an open resource file and no error
		if ((noErr == err) && (-1 != resFileDSp))
		{
			SetResLoad (true);
			hVersion = GetResource ('vers', 1); 
			err = ResError ();
			if ((noErr == err) && (NULL != hVersion))
			{
				gVersionDSp.majorRev = *(((unsigned char *)*hVersion) + 0);
				gVersionDSp.minorAndBugRev = *(((unsigned char *)*hVersion) + 1);
				gVersionDSp.stage = *(((unsigned char *)*hVersion) + 2);
				gVersionDSp.nonRelRev = *(((unsigned char *)*hVersion) + 3);
				ReleaseResource (hVersion);
			}
			UseResFile(resFileSave);
			CloseResFile(resFileDSp);
		}	
	}
#ifdef kDSp17
	else
		gVersionDSp = DSpGetVersion ();
#endif

	hDevice = DMGetFirstScreenDevice (true);								// check number of screens
	do
	{
		(*numDevices)++;
		hDevice = DMGetNextScreenDevice (hDevice, true);
	}
	while (hDevice);
			
	// Note: DSp < 1.7.3 REQUIRES the back buffer attributes even if only one buffer is required
	memset(&gTheContextAttributes, 0, sizeof (DSpContextAttributes));
	gTheContextAttributes.displayWidth			= kContextWidth;
	gTheContextAttributes.displayHeight			= kContextHeight;
	gTheContextAttributes.colorNeeds			= kDSpColorNeeds_Require;
	gTheContextAttributes.displayBestDepth		= kBitsPerPixel;
	gTheContextAttributes.backBufferBestDepth	= kBitsPerPixel;
	gTheContextAttributes.displayDepthMask		= kDSpDepthMask_All;
	gTheContextAttributes.backBufferDepthMask	= kDSpDepthMask_All;
	gTheContextAttributes.pageCount				= 1;								// only the front buffer is needed
	
	// will display user dialog if context selection is required otherwise as find best context
	if (noErr != DSpDebugStr (DSpUserSelectContext(&gTheContextAttributes, 0L, nil, &gTheContext)))
		return NULL;
	if (noErr != DSpDebugStr (DSpContext_GetAttributes (gTheContext, &foundAttributes))) // see what we actually found
		return NULL;
		
	// reset width and height to full screen and handle our own centering
	// HWA will not correctly center less than full screen size contexts
	gTheContextAttributes.displayWidth 		= foundAttributes.displayWidth;
	gTheContextAttributes.displayHeight 	= foundAttributes.displayHeight;
	gTheContextAttributes.pageCount			= 1;									// only the front buffer is needed
	gTheContextAttributes.contextOptions	= 0 | kDSpContextOption_DontSyncVBL;	// no page flipping and no VBL sync needed

	DSpSetBlankingColor(&rgbBlack);
	if (noErr !=  DSpDebugStr (DSpContext_GetDisplayID(gTheContext, &displayID))) 	// get our device for future use
		return NULL;
	if (noErr !=  DMGetGDeviceByDisplayID(displayID, phGD, false)) 					// get GDHandle for ID'd device
	{
		ReportError ("DMGetGDeviceByDisplayID() had an error.");
		return NULL;
	}
	if (noErr !=  DSpDebugStr (DSpContext_Reserve ( gTheContext, &gTheContextAttributes))) // reserve our context
		return NULL;

	HideCursor ();

	DSpDebugStr (DSpContext_FadeGammaOut (NULL, NULL)); 							// fade display, remove for debug
	if (noErr != DSpDebugStr (DSpContext_SetState (gTheContext, kDSpContextState_Active))) // activate our context
		return NULL;
	
	// create a new window in our context 
	// note: OpenGL is expecting a window so it can enumerate the devices it spans, 
	// center window in our context's gdevice
	rectWin.top  = (short) ((***phGD).gdRect.top + ((***phGD).gdRect.bottom - (***phGD).gdRect.top) / 2);	  	// h center
	rectWin.top  -= (short) (kContextHeight / 2);
	rectWin.left  = (short) ((***phGD).gdRect.left + ((***phGD).gdRect.right - (***phGD).gdRect.left) / 2);	// v center
	rectWin.left  -= (short) (kContextWidth / 2);
	rectWin.right = (short) (rectWin.left + kContextWidth);
	rectWin.bottom = (short) (rectWin.top + kContextHeight);
	
	pCGOut = (CGrafPtr)NewCWindow (NULL, &rectWin, "\p", 0, plainDBox, (WindowPtr)-1, 0, 0);

	// paint back ground black before fade in to avoid white background flash
	ShowWindow((GrafPtr)pCGOut);
	GetPort (&pGrafSave);
	SetPort ((GrafPtr)pCGOut);
	GetForeColor (&rgbSave);
	RGBForeColor (&rgbBlack);
	PaintRect (&pCGOut->portRect);
	RGBForeColor (&rgbSave);		// ensure color is reset for proper blitting
	SetPort (pGrafSave);
	
	DSpDebugStr (DSpContext_FadeGammaIn (NULL, NULL));
	
	return pCGOut;
}

//-----------------------------------------------------------------------------------------------------------------------

// clean up DSp

void ShutdownDSp (CGrafPtr *ppWin)
{
	DSpContext_FadeGammaOut (NULL, NULL);
	if (*ppWin)
		DisposeWindow ((WindowPtr)*ppWin);
	*ppWin = NULL;
	DSpContext_SetState( gTheContext, kDSpContextState_Inactive );
	DSpContext_FadeGammaIn (NULL, NULL);
	ShowCursor ();
	DSpContext_Release (gTheContext);
	DSpShutdown ();
}

#pragma mark -
//-----------------------------------------------------------------------------------------------------------------------

// OpenGL Setup

AGLContext SetupAGL (GDHandle hGD, AGLDrawable win)
{
// different possible pixel format choices for different renderers 
// basics requirements are RGBA and double buffer
// OpenGL will select acclerated context if available

// software renderer
//	GLint          attrib[] = { AGL_RGBA, AGL_RENDERER_ID, AGL_RENDERER_GENERIC_ID, AGL_ALL_RENDERERS, AGL_DOUBLEBUFFER, AGL_NONE };

// any renderer
//	GLint          attrib[] = { AGL_RGBA, AGL_ALL_RENDERERS, AGL_DOUBLEBUFFER, AGL_NONE };

// OpenGL compliant HWA renderer 
	GLint          attrib[] = { AGL_RGBA, AGL_ACCELERATED, AGL_DOUBLEBUFFER, AGL_NONE };

// OpenGL compliant ATI renderer 
//	GLint          attrib[] = { AGL_RGBA, AGL_RENDERER_ID, AGL_RENDERER_ATI_ID, AGL_DOUBLEBUFFER, AGL_NONE };

// OpenGL compliant ATI HWA renderer 
//	GLint          attrib[] = { AGL_RGBA, AGL_RENDERER_ID, AGL_RENDERER_ATI_ID, AGL_ACCELERATED, AGL_DOUBLEBUFFER, AGL_NONE };

	AGLPixelFormat 	fmt;
	AGLContext     	ctx;

	if ((Ptr) kUnresolvedCFragSymbolAddress == (Ptr) aglChoosePixelFormat) // check for existance of OpenGL
	{
		ReportError ("OpenGL not installed\n");
		return NULL;
	}	

	if (hGD)
		fmt = aglChoosePixelFormat (&hGD, 1, attrib); 			// get an appropriate pixel format
	else
		fmt = aglChoosePixelFormat(NULL, 0, attrib);			// get an appropriate pixel format
	aglDebugStr ();
	if (NULL == fmt) 
	{
		ReportError("Could not find valid pixel format\n");
		return NULL;
	}
	
	ctx = aglCreateContext (fmt, NULL);							// Create an AGL context
	aglDebugStr ();
	if (NULL == ctx)
	{
		ReportError ("Could not create context\n");
		return NULL;
	}
	
	if (!aglSetDrawable (ctx, win))								// attach the CGrafPtr to the context
		aglDebugStr ();
	else if (!aglSetCurrentContext (ctx))						// make the context the current context
	{
		aglDebugStr ();
		aglSetDrawable (ctx, NULL);
	}

	aglDestroyPixelFormat(fmt);									// pixel format is no longer needed

	return ctx;
}

//-----------------------------------------------------------------------------------------------------------------------

// OpenGL Cleanup

void CleanupAGL(AGLContext ctx)
{
	aglSetCurrentContext(NULL);
	aglSetDrawable(ctx, NULL);
	aglDestroyContext(ctx);
}

//-----------------------------------------------------------------------------------------------------------------------

// OpenGL Drawing

void DrawGL(AGLContext ctx)
{
	static float f, s, c;
	GLboolean fState = GL_FALSE;

	f += 0.01;
	s = (float) sin(f);
	c = (float) cos(f);

	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);					// Clear color buffer to dark grey
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_POLYGON);										// Draw a smooth shaded polygon
	glColor3d(1.0, 0.0, 0.0);
	glVertex3d(s, c, 0.0);
	glColor3d(0.0, 1.0, 0.0);
	glVertex3d(-c, s, 0.0);
	glColor3d(0.0, 0.0, 1.0);
	glVertex3d(-s, -c, 0.0);
	glColor3d(0.7, 0.7, 0.7);
	glVertex3d(c, -s, 0.0);
	glEnd();
	
	// Draw frame rate (set color and position first)
	glColor3d(1.0, 1.0, 1.0);
	glRasterPos3d (-0.95, (kContextHeight - 40.0) / (float) kContextHeight, 0.0); 
	DrawFrameRate (gFontList);
	glRasterPos3d (-0.95, (kContextHeight - 65.0) / (float) kContextHeight, 0.0); 
	DrawContextInfo (gcstrMode, gFontList);

	glRasterPos3d (-0.95, -(kContextHeight - 45.0) / (float) kContextHeight, 0.0); 
	DrawCSringGL ((char*) glGetString (GL_VENDOR), gFontList);
	glRasterPos3d (-0.95, -(kContextHeight - 20.0) / (float) kContextHeight, 0.0); 
	DrawCSringGL ((char*) glGetString (GL_RENDERER), gFontList);

	aglSwapBuffers(ctx);
}

#pragma mark -
//-----------------------------------------------------------------------------------------------------------------------

// Draw frame rate in curent color at current raster positon with provided font display list

void DrawFrameRate (GLuint fontList)
{	
	static char aChar[256] = "";
	static AbsoluteTime time = {0,0};
	static long frames = 0;

	AbsoluteTime currTime = UpTime ();
	float deltaTime = (float) AbsoluteDeltaToDuration (currTime, time);
	
	frames++;

	if (0 > deltaTime)	// if negative microseconds
		deltaTime /= -1000000.0;
	else				// else milliseconds
		deltaTime /= 1000.0;
	if (0.5 <= deltaTime)	// has update interval passed
	{
		sprintf (aChar, "Swaps/Sec: %0.1f", frames / deltaTime);
		time = currTime;	// reset for next time interval
		frames = 0;
	}
	
	DrawCSringGL (aChar, fontList);
}

//-----------------------------------------------------------------------------------------------------------------------

// Draw height,width, bit pixel and frequency for context

void DrawContextInfo (char * cstrContextMode, GLuint fontList)
{	
	DrawCSringGL (cstrContextMode, fontList);
}

#pragma mark -
//-----------------------------------------------------------------------------------------------------------------------

void DrawPSringGL (Str255 pstrOut, GLuint fontList)
{
	GLint i;
	for (i = 1; i <= pstrOut[0]; i++)
		glCallList (fontList + pstrOut[i]);
}

//-----------------------------------------------------------------------------------------------------------------------

void DrawCSringGL (char * cstrOut, GLuint fontList)
{
	GLint i = 0;
	while (cstrOut [i])
		glCallList (fontList + cstrOut[i++]);
}

//-----------------------------------------------------------------------------------------------------------------------

GLuint BuildFontGL (AGLContext ctx, GLint fontID, Style face, GLint size)
{
	GLuint listBase = glGenLists (256);
	if (aglUseFont (ctx, fontID , face, size, 0, 256, (long) listBase))
	{
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		return listBase;
	}
	else
	{
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glDeleteLists (listBase, 256);
		return 0;
	}
}

//-----------------------------------------------------------------------------------------------------------------------

void DeleteFontGL (GLuint fontList)
{
	if (fontList)
		glDeleteLists (fontList, 256);
}