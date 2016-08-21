// Zex - 2D Space Trading Game with 3D graphics
// Copyright (C) 1998-2005  Stuart Ball and Robert Probin
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

/*
 *  mac_emulation.h
 *  Zex
 *
 *  Created by Rob Probin on 01/05/2005.
 *  Copyright 2005 Lightsoft. All rights reserved.
 *
 * $Author: stu $
 * $Date: 2006/04/09 01:12:15 $
 * $Revision: 1.29 $
 *
 * For log see end of file...
 *
 */


#ifndef _MAC_EMULATION_H_
#define _MAC_EMULATION_H_

#include "options.h"


//
// Stolen types from MacOS
//
typedef char *                          Ptr;
typedef const char *                    const_Ptr;			// special so we use pointers to constant strings
typedef Ptr *                           Handle;

struct RGBColor {
	unsigned short      red;                    /*magnitude of red component*/
	unsigned short      green;                  /*magnitude of green component*/
	unsigned short      blue;                   /*magnitude of blue component*/
};


typedef struct RGBColor                 RGBColor;

// Specific sized types
typedef unsigned char                   UInt8;
typedef signed char                     SInt8;
typedef unsigned short                  UInt16;
typedef signed short                    SInt16;
typedef unsigned long                   UInt32;
typedef signed long                     SInt32;

typedef UInt8                           Byte;
typedef SInt8                           SignedByte;

typedef unsigned long                   FourCharCode;
typedef FourCharCode                    OSType;
typedef FourCharCode                    ResType;
typedef SInt16                          OSErr;
typedef SInt32                          OSStatus;
typedef unsigned char                   Boolean;
typedef unsigned char                   Str255[256];
typedef SDL_Surface* LS_CGrafPtr;		// one of these
typedef double              Float64;
typedef long                            Size;

typedef float               Float32;

struct Point {
	short               v;
	short               h;
};
typedef struct Point                    Point;
typedef Point *                         PointPtr;
struct Rect {
	short               top;
	short               left;
	short               bottom;
	short               right;
};
typedef struct Rect                     Rect;
typedef Rect *                          RectPtr;

struct MacPolygon {
	short               polySize;
	Rect                polyBBox;
	Point               polyPoints[1];
};
typedef struct MacPolygon               MacPolygon;

typedef MacPolygon                      Polygon;


//
// These are mac types we'd like to get rid of
//

#if 0			// comment out - we don't have access to a 1us timer
#ifndef _OS_OSTYPES_H
#define TARGET_RT_BIG_ENDIAN 1			// need to fix this for intel build

#if TARGET_RT_BIG_ENDIAN
struct LS_wide {
	SInt32              hi;
	UInt32              lo;
};
typedef struct LS_wide                     LS_wide;
struct LS_UnsignedWide {
	UInt32              hi;
	UInt32              lo;
};
typedef struct LS_UnsignedWide             LS_UnsignedWide;
#else
struct LS_wide {
	UInt32              lo;
	SInt32              hi;
};
typedef struct LS_wide                     LS_wide;
struct LS_UnsignedWide {
	UInt32              lo;
	UInt32              hi;
};
typedef struct LS_UnsignedWide             LS_UnsignedWide;
#endif  /* TARGET_RT_BIG_ENDIAN */

#endif
#endif

//
// file specs
//
typedef unsigned char                   Str63[64];
typedef Str63                           StrFileName;
struct FSSpec {
	short               vRefNum;
	long                parID;
	StrFileName         name;                   /* a Str63 on MacOS*/
	StrFileName			fullpathname;
};
typedef struct FSSpec                   FSSpec;

// OGL type
typedef SInt32 GLuint;

//
// *****************************************************************************
//

//
// Replace for Mac Functions
//
void ZexNumToString(long theNum, Str255 theString);
Handle LS_GetResource(ResType theType,short theID);
void ReleaseResource(Handle theResource);
void LS_HLock(Handle h);
void LS_HUnlock(Handle h);
void LS_DisposeHandle(Handle h);
void LS_DetachResource(Handle theResource);
OSErr ResError ();

unsigned long ls_get_file_size(FILE *file);
Handle NewHandleClear(unsigned long byteCount);	// fetches some space that is zeroed
Handle NewHandle(unsigned long byteCount);
Size GetHandleSize(Handle h);

Ptr NewPtr(Size byteCount);
void DisposePtr(Ptr p);

typedef Handle CTabHandle;
CTabHandle LS_GetCTable(short ctID);

// utility functions
Uint32 LS_TickCount60(void);		// counts in 60Hz jumps
void LS_FlushEvents(int event_mask, int stop_mask);
const int everyEvent = 0xFFFF;			// specific event mask from mac

void LS_ShowCursor(void);
void LS_HideCursor(void);

bool LS_Left_Button(void);		// Replacement for Mac Button
void process_standard_event_functions(void);		// process the standard event functions
void update_zex_main_event_state(SDL_Event& event);	// update Zex's state from another event loop to keep mouse, keys in correct state.
void zex_polling_switch(bool enable_zex_polling);		// disable it from things like ShowIt (which uses it for the cursors)

void get_mouse_x_and_y(int *x, int *y);
bool has_mouse_ever_moved();

// non-macos utilities
void do_fixed_frame_rate(bool re_initialise);
enum { FRAME_RATE_DELAY, FRAME_RATE_REINIT};

//
// GetKeys emulation
//
//oldmac//typedef unsigned long KeyMap[4];
//oldmac//void GetKeys (KeyMap theKeys);

void RGBForeColor(const RGBColor * color);
void RGBBackColor(const RGBColor * color);
void set_foreground_colour(unsigned char colour);
void FadeForeColour(unsigned char fade_percent);

void PaintRect(const Rect * r);
void EraseRect(const Rect * r);

// emulation for the popular BlockMoveData
#define BlockMoveData(srcPtr, destPtr, byteCount) memcpy(destPtr, srcPtr, byteCount);	// used memcpy rather than memmove - they shouldn't overlap (since BlockMoveData can zero the target!)

// we can draw on the screen with these...
void TextFont(short font);
void TextSize(short size);
enum {		// these are values for the face parameter in TextFace. We only support normal and bold.
	normal                        = 0,
	bold                          = 1 //,
//	italic                        = 2,
//	underline                     = 4,
//	outline                       = 8,
//	shadow                        = 0x10,
//	condense                      = 0x20,
//	extend                        = 0x40
};
void TextFace(short face);
void TextMode(short mode);
void MoveTo(short h, short v);
void DrawString(const unsigned char *pascal_string);
void DrawString_c(const char *c_string);
int get_font_height();

short StringWidth (const unsigned char *pascal_string);
void LineTo (short h, short v);
void FrameRect(const Rect * r);
void ScrollRect(const Rect * r, short dh, short dv, int updateRgn);

void GetIndString (Str255 theString, short strListID, short index);

// not really a mac call - but a replacement for EraseRect(the whole port)
void cls(void);


OSStatus LS_FadeGamma(SInt32 inPercentOfOriginalIntensity, RGBColor *inZeroIntensityColor);	// see mac_emulation.cpp for parameter detail
void LS_NormalGamma();

void safe_number_id_to_4_character_string(unsigned int id, char* string_out);
void safe_number_id_to_4_character_string(unsigned int id, unsigned char* string_out);


//
// goes to stderr to ensure it get's spooled straight away
//
enum debug_enum { debug_overview, debug_medium, debug_verbose };
#if DEBUG_GAME_STATE!=0		// overview info
void debug_printf(debug_enum level, const char*s);
void debug_printf(debug_enum level, const char*s, int i);
#else	// info off
inline void debug_printf(debug_enum level, const char*s) { }
inline void debug_printf(debug_enum level, const char*s, int i) { }
#endif



#endif // _MAC_EMULATION_H_




/* CVS LOG
*
* $Log: mac_emulation.h,v $
* Revision 1.29  2006/04/09 01:12:15  stu
* Zex2005: Human and alien shuttles, faster falcons, Pythons and Excaliburs.
*
* Revision 1.28  2006/04/02 11:49:03  rob
* Zex2005: Finally fixed the keys disappearing on the save screen bug. Remember, don't take keys from the event queue from two places ... even by accident via Show_It & get_mouse_x_and_y.
*
* Revision 1.27  2006/03/19 12:34:24  rob
* Zex2005: Highlight rather than naff arrow for menus.
*
* Revision 1.26  2006/02/05 19:30:20  rob
* Zex2005: Debug stuff, currently disabled.
*
* Revision 1.25  2006/01/22 12:36:08  rob
* Zex2005: Modify 4 digit callsign print to be safe even if callsign is not ASCII.
*
* Revision 1.24  2006/01/01 17:48:41  rob
* Zex2005: Added back in screen fading, especially for fast dock.
*
* Revision 1.23  2005/12/10 10:51:11  rob
* Zex2005: Fixed full screen cursor flicker (hopefully) by implementing our own cursor.
*
* Revision 1.22  2005/11/16 21:12:18  rob
* Zex2005: Update that was causing all sorts of pushed buttons to appear due to the menu system stealing events away from the state based main event loop.
*
* Revision 1.21  2005/11/15 22:21:16  rob
* Zex2005: File selector patched in.
*
* Revision 1.20  2005/10/10 21:28:05  rob
* Zex2005: Added newline at end to stop compiler whinging.
*
* Revision 1.19  2005/10/08 13:15:29  rob
* Zex2005: "/p" pascal string size marker is not supported in normal gcc - it's a apple specific extension turned on by the option -fpascal_strings. This change removes that string so that the code will run both on Mac and Windows. Lots of files have been changed - but nearly all the changes are trivial. By the fun-jobs-are-us dept.
*
* Revision 1.18  2005/06/27 19:57:08  rob
* Zex2005: Files changed so that use of a string constant (aka a string literal) is only used by const char * routines - this way we can alter the compiler to warning about access by char *. Real reason for altering this is because we had a case of altering these string constants (to insert call signs) which caused a bus error. This has been corrected by copying the string before modification.
*
* Revision 1.17  2005/06/23 21:09:13  rob
* Zex2005: Added call to set RGB colour as fixed byte. Patched in draw_line and fixed problems.
*
* Revision 1.16  2005/06/18 14:57:33  rob
* Zex2005: Key setting now working.
*
* Revision 1.15  2005/06/04 11:17:19  rob
* Zex2005: GetKeys is no more! Replaced with emulation function. Therefore auxilliary keys should now work.
*
* Revision 1.14  2005/06/04 08:54:15  rob
* Zex2005: We now have keyboard control in the game!!!
*
* Revision 1.13  2005/06/01 19:06:56  rob
* Zex2005: Updated project - now runs main menu.
*
* Revision 1.12  2005/05/25 20:27:18  rob
* Zex2005: More changes.
*
* Revision 1.11  2005/05/23 21:13:18  rob
* Zex2005: More changes to files.
*
* Revision 1.10  2005/05/20 21:25:52  rob
* Zex2005: More file changes.
*
* Revision 1.9  2005/05/19 20:24:19  rob
* Zex2005: More files changed.
*
* Revision 1.8  2005/05/17 16:22:01  rob
* Zex2005: Even more work sorting out build problems on gcc4.0/MacOSX/SDL conversion.
*
* Revision 1.7  2005/05/17 13:50:45  rob
* Zex2005: More file changes.
*
* Revision 1.6  2005/05/17 13:11:13  rob
* Zex2005: Various assorted conversion fixes.
*
* Revision 1.5  2005/05/15 19:32:19  rob
* Zex2005: Moved ZexNumToString to mac_emulation layer.
*
* Revision 1.4  2005/05/15 17:42:29  rob
* Zex2005: ZexNumToString used throughout, file case changes, general gcc4.0 build changes.
*
* Revision 1.3  2005/05/13 09:24:55  rob
* Zex2005: Changed Button to internal version. Altered string to dec code to use atof().
*
* Revision 1.2  2005/05/04 17:31:47  rob
* Zex2005: More changes for build.
*
* Revision 1.1  2005/05/02 10:26:53  rob
* Zex2005: More Updates (code)
*
*
*
*/



