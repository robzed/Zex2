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
 *  mac_emulation.cpp
 *  Zex
 *
 *  Created by Rob Probin on 01/05/2005.
 *  Copyright 2005 Lightsoft. All rights reserved.
 *
 * $Author: rob $
 * $Date: 2006/04/02 11:49:00 $
 * $Revision: 1.41 $
 *
 * For log see end of file...
 *
 */

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"
#include "stdlib.h"
#include <new>
using namespace std;

#include <string.h>
#include <vector>

#include "game_defs.h"
#include "zex_misc.h"
#include "txtsplat.h"

#include "mac_emulation.h"
#include "Error_handler.h"
#include "splat_control.h"
#include "input_control.h"
#include "main.h"

#include "dlp.h"
#include "timing.h"
#include "main.h"

// ***********************************************************************************
// * INTERNAL VARIABLES
// *

// storage for handles
const int TOTAL_NUMBER_OF_HANDLES = 100;

typedef struct
{
	Ptr pointer;				// storage for the pointer
	Size size;					// storage for the requested sizes
	Handle self_address_error_check;		// used for error checking find handles fast version
	bool is_really_resource;	// is this a resource? used for error trapping
	ResType res_type;		// when it's a resource, the type goes here
	short res_id;			// when it's a resource, the id goes here
} complex_handle_type;

static complex_handle_type complex_handle[TOTAL_NUMBER_OF_HANDLES];

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

static void handle_mouse_button_change(SDL_Event* event_ptr);
static int GetEmptyFreeHandle(void);				// fetches the index of one of the free handle pointer
static int FindAllocatedHandle(Handle h);			// returns the index of an allocated handle
static void our_fill_rect(const Rect *r, Uint32 the_colour);
static void draw_char_copy(unsigned char *char_to_draw_from, int char_width, int char_height, int char_line_step, unsigned char *dest_ptr, int dest_line_step, unsigned char fg_colour, unsigned char bg_colour);
static void draw_char_or(unsigned char *char_to_draw_from, int char_width, int char_height, int char_line_step, unsigned char *dest_ptr, int dest_line_step, unsigned char fg_colour);

// ***********************************************************************************
// * ACTUAL CODE
// *

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 1 May 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

Handle LS_GetResource(ResType   theType,short     theID)
{
	// check if the resource already existing
	for(int i=0; i<TOTAL_NUMBER_OF_HANDLES; i++)
	{
		// even if not allocated..
		if(theType == complex_handle[i].res_type)
		{
			if(theID == complex_handle[i].res_id)
			{
				if(complex_handle[i].pointer && complex_handle[i].is_really_resource)	// make sure this hasn't been de-allocated or 
				{
					// we have a match that is valid!!!
					return &(complex_handle[i].pointer);
				}
			}
		}
	}
	
	// doesn't exist - load it...
	char filename[100];
	sprintf(filename, "ZD3/R/%c%c%c%c%05hu.rbs", 
			static_cast<int>(0xff&(theType>>24)), 
			static_cast<int>(0xff&(theType>>16)), 
			static_cast<int>(0xff&(theType>> 8)),
			static_cast<int>(0xff&(theType)), 
			theID);

	// open the file
	FILE* f = fopen(filename, "rb");
	if(!f)
	{
		char error_string[200];
		sprintf(error_string, "Error opening resource - %s",filename);
		report_error_c(error_string, "", theID);
		return 0; //let caller deal with the error because fnf
	}
	
	// find the size
	unsigned long file_size = ls_get_file_size(f);

	// get some free space
	int handle_index = GetEmptyFreeHandle();
	Handle resource_handle = &(complex_handle[handle_index].pointer);

	*resource_handle = new char[file_size+10];		// +10 is just in case
	
	// store other details
	complex_handle[handle_index].size = file_size;			// pretend it's just the file size
	complex_handle[handle_index].is_really_resource = true;
	complex_handle[handle_index].res_type = theType;
	complex_handle[handle_index].res_id = theID;
	
	size_t read_size = fread(*resource_handle, sizeof(char), file_size, f);
	if(read_size != file_size)
	{
		char error_string[200];
		sprintf(error_string, "Error loading resource - size problem - %s",filename);
		report_error_c(error_string, "", theID);
	}
	
	if(ferror(f))
	{
		char error_string[200];
		sprintf(error_string, "Error loading resource - error returned - %s",filename);
		report_error_c(error_string, "", theID);
	}
	
	if(fclose(f))
	{
		char error_string[200];
		sprintf(error_string, "Error closing resource - %s",filename);
		report_error_c(error_string, "", theID);
	}
	
	return resource_handle;
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 1 May 2005
// +
// | Mac emulation function
// +----------------------------------------------------------------ROUTINE HEADER----

OSErr ResError ()
{
	return 0;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: ls_get_file_size        | AUTHOR(s): Rob Probin   | DATE STARTED: 22 Sept 03
// +
// | DESCRIPTION: This finds the length of a file. Returns -1 for error.
// | 
// | This may not be overly portable in C since:-
// |	(a) does ftell return the length after a SEEK_END?
// | 	(b) not all systems support SEEK_END.
// |
// | An alternative method is to read successive bytes into a small array, adding up
// | how many have been read. If this method is going to be used, and performance
// | of reading the twice is an issue, then it would be better to allocate a 64K buffer
// | read up to 64K, then extend (if too small) and read the rest or shrink the buffer if
// | too much has been allocated.
// |
// | Also see notes after routine.
// |
// +----------------------------------------------------------------ROUTINE HEADER----
// | Notes: This is a version modified from Z_dungeon.
// |

unsigned long ls_get_file_size(FILE *file)
{
	long current_position;
	long end_position;
		
	current_position = ftell(file);		// get the current file position
	if(current_position == -1)
    { 
		report_error_c("lsf> ftell_1 error in lsf_get_file_size!\n","",0); 
		return 0; // shouldn't get here
    }
	
	if(fseek(file, 0, SEEK_END))		// set to the end of the file
    { 
		report_error_c("lsf>seek end error in lsf_get_file_size!\n","",0); 
		return 0; // shouldn't get here
    }
	
	end_position = ftell(file);		// this might not be a valid method for determining the length in C??
	if(end_position == -1)
    { 
		report_error_c("lsf> ftell_2 error in lsf_get_file_size!\n","",0); 
		return 0; // shouldn't get here
    }
	
	if(fseek(file, current_position, SEEK_SET))	// set back to where we were.
    { 
		report_error_c("lsf> ftell_1 error in lsf_get_file_size!\n","",0); 
		return 0; // shouldn't get here
    }
	
	return end_position;
}

// from http://open-systems.ufl.edu/mirrors/ftp.isc.org/pub/usenet/comp.sources.unix/volume26/funnelweb/part08
//
// comment from the FunnelWeb Source 
//
/* Finding The Length of a File                                               */
/* ----------------------------                                               */
/* We have to be able to find out the length of a file before reading it in   */
/* because, in this version of FunnelWeb, the entire file must be read into   */
/* one contiguous block of memory.                                            */
/*                                                                            */
/* As it turns out, finding the length of a file in portable C turns out to   */
/* be a very hard problem. Here are some possible solutions:                  */
/*                                                                            */
/*    1. Read the entire file in and see how long it is.                      */
/*    2. Use fseek to move to the end of the file and then use ftell.         */
/*    3. Use the Unix 'stat' call.                                            */
/*                                                                            */
/* Of these, only the first is portable. The second alternative is            */
/* non-portable because some environments do not support the SEEK_END symbol  */
/* required to perform a seek to the end of file.                             */
/* Alternatives to needing the length are as follows:                         */
/*                                                                            */
/*    4. Read the file/allocate memory in 64K blocks.                         */
/*    5. Read the file in 64K blocks and then copy them to a contiguous one.  */
/*                                                                            */
/* Perhaps options 4 or 5 could be implemented later. However, right now I    */
/* haven't got the time to do anything except strive for portability, so      */
/* option 1 it is.                                                            */



// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 1 May 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void LS_HLock(Handle h)
{
	// do we need this any more? - all handles are locked anyway
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 16 June 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void LS_HUnlock(Handle h)
{
	// do we need this any more? - all handles are locked anyway
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 1 May 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

CTabHandle LS_GetCTable(short ctID)
{
	static Handle colour_table = 0;

	if(ctID != 128)
	{
		report_error_c("We only support one clut ...", "", 0);
		exit(1);		// shouldn't get here
	}
	
	if(colour_table == 0)
	{
		colour_table = LS_GetResource('clut',128);
	}

//Every single short in here needs swapping if little endian
#if ARCH_LITTLE_ENDIAN
	UInt16* clut_data=(Uint16*)*colour_table;
	clut_data+=4;	// first 4 shorts are boring (0000 0000 0000 00FF)


	for(int i=0;i<256;i++)
	{
		*clut_data=Endian16_Swap(*clut_data); clut_data++;	//index
		*clut_data=Endian16_Swap(*clut_data); clut_data++;	//r
		*clut_data=Endian16_Swap(*clut_data); clut_data++;	//g
		*clut_data=Endian16_Swap(*clut_data); clut_data++;	//b
	}
#elif !ARCH_BIG_ENDIAN
#error "Endian defines missing"
#endif	
	return colour_table;	
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE: GetEmptyFreeHandle         | AUTHOR(s): Rob Probin   | DATE STARTED: 1 May 2005
// +
// | Return the location of a free handle
// +----------------------------------------------------------------ROUTINE HEADER----
int handles_in_use_count=0;	//these 2 vars are globals for debugging
int number_of_handles_peak=0;
static int GetEmptyFreeHandle(void)
{
	for(int i=0; i<TOTAL_NUMBER_OF_HANDLES; i++)
	{
		if(!complex_handle[i].pointer)
		{
			complex_handle[i].self_address_error_check = &complex_handle[i].pointer;	// for error trapping
			handles_in_use_count++;	//debugging
			if (handles_in_use_count>number_of_handles_peak) { number_of_handles_peak=handles_in_use_count; }	//debugging
			return i;
		}
	}

	report_error_c("No handles available","",0);
	return 0;		// MUST never get here
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: FindAllocatedHandle        | AUTHOR(s): Rob Probin   | DATE STARTED: 22 May 2005
// +
// | Return the location of an allocated - or throw an error
// +----------------------------------------------------------------ROUTINE HEADER----

#define FAST_FIND_HANDLE_CODE 1

static int FindAllocatedHandle(Handle h)
{
#if FAST_FIND_HANDLE_CODE

	if(!h)
	{
		report_error_c("Handle doesn't exist - it's NULL","",0);
	}
	
	char* complex_handles_start = reinterpret_cast<char *>(complex_handle);
	char* complex_handles_end = reinterpret_cast<char *>(&complex_handle[TOTAL_NUMBER_OF_HANDLES]);
	char* handle_in = reinterpret_cast<char *>(h);
	if(handle_in < complex_handles_start || 
	   handle_in >= complex_handles_end)
	{
		report_error_c("Couldn't find handle!","",0);
	}
	
	int handle_index =  (handle_in-complex_handles_start) / sizeof(complex_handle_type);

	// this catches errors just in case the above casting is broken...
	if(complex_handle[handle_index].self_address_error_check != h)
	{
		report_error_c("FindAllocatedHandle (fast) is broken!!","",0);
	}
	
	return handle_index;
	
#else
// old code did it via searching - safer, but slower
	int handle_index = 0;				// never returns this value ... 
		
	if(!h)
	{
			report_error_c("Handle doesn't exist - it's NULL","",0);
	}
	
	bool search_failed = true;
	for(int i=0; i<TOTAL_NUMBER_OF_HANDLES; i++)
	{
		if(&complex_handle[i].pointer==h)
		{
			handle_index = i;
			search_failed = false;
			break;
		}
	}
	
	if(search_failed)
	{
		report_error_c("Couldn't find handle!","",0);
	}
	
	return handle_index;
	
#endif
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 1 May 2005
// +
// | All memory pointers are converted to new and delete - so we can delete these.
// +----------------------------------------------------------------ROUTINE HEADER----

void LS_DisposeHandle(Handle h)
{
	int index = FindAllocatedHandle(h);
	if(!complex_handle[index].pointer)
	{
		report_error_c("Handle already deleted!","",0);
	}
	if(complex_handle[index].is_really_resource)
	{
		report_error_c("Trying to Dispose a resource!!","",0);
	}
	delete[] complex_handle[index].pointer;
	complex_handle[index].pointer=0;		// remove our record as well
	handles_in_use_count--;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 22 May 2005
// +
// | Mac function emulation
// +----------------------------------------------------------------ROUTINE HEADER----

void ReleaseResource(Handle theResource)
{
	LS_DetachResource(theResource);
	// now, it's just a normal handle, unlike on in Mac OS (which has some synchronisation with the resource file going on also)
	LS_DisposeHandle(theResource);
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 22 May 2005
// +
// | Mac emulation. This works on resource as well as handles - so don't need to trap that case.
// +----------------------------------------------------------------ROUTINE HEADER----
Size GetHandleSize(Handle h)
{
	int index = FindAllocatedHandle(h);			// find the handle. Will never return an illegal value
	return complex_handle[index].size;					// get the size
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 22 May 2005
// +
// | Mac emulation - thin later over new
// +----------------------------------------------------------------ROUTINE HEADER----
Ptr NewPtr(Size byteCount)
{
	return new char[byteCount];
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 22 May 2005
// +
// | Mac emulation - thin later over delete
// +----------------------------------------------------------------ROUTINE HEADER----
void DisposePtr(Ptr p)
{
	delete[] p;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 1 May 2005
// +
// |
// +----------------------------------------------------------------ROUTINE HEADER----

void LS_DetachResource(Handle theResource)
{
	int index = FindAllocatedHandle(theResource);
	if( ! complex_handle[index].is_really_resource)
	{
		report_error_c("Tried to Detach a handle!!","",0);
	}

	complex_handle[index].is_really_resource = 0;
	// these two speed up searches of the handle system for resources
	complex_handle[index].res_type = 0;
	complex_handle[index].res_id = 0;		
}






// +--------------------------------+-------------------------+-----------------------
// | TITLE: LS_TickCount60          | AUTHOR(s): Rob Probin   | DATE STARTED: 24 Apr 05
// +
// | DESCRIPTION: Very like Mac OS TickCount except it handles SDL_GetTicks going backwards.
// | 
// | Notice: that unlike the system function LS_TickCount60, this stops working after 49 
// | days. (rather than the 828 for LS_TickCount60). We could fix this in LS_GetTicks.
// +----------------------------------------------------------------ROUTINE HEADER----

Uint32 LS_TickCount60(void)
{
	double ticks = LS_GetTicks();
	ticks = ticks * 60.0;
	ticks /= 1000.0;
	return static_cast<Uint32>(ticks);
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: LS_FlushEvents          | AUTHOR(s): Rob Probin   | DATE STARTED: 25 Apr 05
// +
// | DESCRIPTION: Very like Mac OS FlushEvents.
// | 
// +----------------------------------------------------------------ROUTINE HEADER----
void LS_FlushEvents(int event_mask, int stop_mask)
{
	// I don't think we need to flush events
	// but if we do then we can call SDL_PollEvent()
	// also maybe SDL_EventState() and SDL_SetEventFilter()
	//
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: LS_Left_Button          | AUTHOR(s): Rob Probin   | DATE STARTED: 10 May 05
// +
// | DESCRIPTION: Duplicates Mac button() call.
// +----------------------------------------------------------------ROUTINE HEADER----
static bool left_button_state_down = false;

bool LS_Left_Button(void)
{
	// we may need to poll events in here
	process_standard_event_functions();

	return left_button_state_down;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: GetKeys                 | AUTHOR(s): Rob Probin   | DATE STARTED: 17 May 05
// +
// | DESCRIPTION: Duplicates Mac GetKeys() call.
// +----------------------------------------------------------------ROUTINE HEADER----
//oldmac//
/*void GetKeys (KeyMap theKeys)
{
	// we will need to poll events in here
	process_standard_event_functions();
	for(int i=0; i<4; i++)		// number of longs in key map
	{
		theKeys[i]=0;			// blank them all to start with
	}
}*/

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 10 May 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
static void Set_Left_Button_State(bool button_down)
{
	left_button_state_down = button_down;
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE: event_switch_statement     | AUTHOR(s): Rob Probin   | DATE STARTED: 16-11-2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----
static int our_mouse_x = 0;
static int our_mouse_y = 0;
static int mouse_coords_invalid = true;

static inline void event_switch_statement(SDL_Event& event)
{
	switch (event.type) {
		
		case SDL_MOUSEMOTION:
			mouse_coords_invalid = false;
			our_mouse_x = event.motion.x;
			our_mouse_y = event.motion.y;
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			mouse_coords_invalid = false;
			our_mouse_x = event.button.x;
			our_mouse_y = event.button.y;
			handle_mouse_button_change(&event);
			break;
		case SDL_KEYDOWN:
			handle_key_change(event.key.keysym.sym, true);
			break;
		case SDL_KEYUP:
			handle_key_change(event.key.keysym.sym, false);
			break;
		case SDL_ACTIVEEVENT:
#if 1
			if(event.active.state == SDL_APPMOUSEFOCUS && !are_we_in_full_screen_mode())
			{
				cursor_inside_of_window(event.active.gain==1);	// gain=1 in window, gain=0 outside window
			}
			break;
			
#else			// code in progress
			{
			printf("Active Event Gain = %u, State = %u\n", static_cast<unsigned int>(event.active.gain), static_cast<unsigned int>(event.active.state));

			static bool mouse_focus_flag = true;
			if(event.active.state & SDL_APPMOUSEFOCUS && !are_we_in_full_screen_mode())
			{
				mouse_focus_flag = (event.active.gain==1);	// gain=1 in window, gain=0 outside window
				cursor_inside_of_window(event.active.gain==1);	// gain=1 in window, gain=0 outside window
			}

			static bool input_focus_flag = true;
			if(event.active.state & SDL_APPINPUTFOCUS)
			{
				input_focus_flag = (event.active.gain==1);	// gain=1 in window, gain=0 outside window
			}

			static bool app_active_flag = true;
			if(event.active.state & SDL_APPACTIVE)
			{
				app_active_flag = (event.active.gain==1);	// gain=1 in window, gain=0 outside window
			}
#warning "Not happy with this code"
			if(app_active_flag && input_focus_flag && mouse_focus_flag)
			{
				SDL_ShowCursor(SDL_DISABLE);
			}
			else	// enable the system cursor if we are outside the window...
			{
				SDL_ShowCursor(SDL_ENABLE);
			}
			break;
		}
#endif
		case SDL_QUIT:
			exit(0);			// quit immediately
			break;
		default:
			//printf("Unknown event type %u\n",static_cast<unsigned int>(event.type));
			break;
	}	
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: process_standard_event_functions | AUTHOR(s): Rob Probin   | DATE STARTED: 10 May 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

static bool disable_zex_polling_flag = false;

void process_standard_event_functions(void)
{
	SDL_Event event;
	
	if(disable_zex_polling_flag) return;
	   
	//printf("*");		// uncomment this line when doing event tests
	
	/* Check for events */
	while ( SDL_PollEvent(&event) ) {
		event_switch_statement(event);
	}
	
}

// +------------------------------------+-------------------------+-----------------------
// | TITLE:                             | AUTHOR(s): Rob Probin   | DATE STARTED: 2 April 2006
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void zex_polling_switch(bool enable_zex_polling)
{
	disable_zex_polling_flag = !enable_zex_polling;
}

// +------------------------------------+-------------------------+-----------------------
// | TITLE: update_zex_main_event_state | AUTHOR(s): Rob Probin   | DATE STARTED: 10 May 2005
// +
// | update Zex's state from another event loop to keep mouse, keys in correct state.
// +----------------------------------------------------------------ROUTINE HEADER----

void update_zex_main_event_state(SDL_Event& event)
{
	event_switch_statement(event);	
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 7 Dec 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

bool has_mouse_ever_moved()
{
	return !mouse_coords_invalid;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 19 May 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void get_mouse_x_and_y(int *x, int *y)
{
	// we will need to poll events in here
	process_standard_event_functions();
	
	*x = our_mouse_x;
	*y = our_mouse_y;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: handle_mouse_button_change | AUTHOR(s): Rob Probin   | DATE STARTED: 10 May 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

static void handle_mouse_button_change(SDL_Event* event_ptr)
{
	// perform the work to duplcate the Button() function of the Mac.
	if(event_ptr->type == SDL_MOUSEBUTTONDOWN)
	{
		if(event_ptr->button.button == SDL_BUTTON_LEFT)
		{
			Set_Left_Button_State(true);
		}
	}
	else if(event_ptr->type == SDL_MOUSEBUTTONUP)
	{
		if(event_ptr->button.button == SDL_BUTTON_LEFT)
		{
			Set_Left_Button_State(false);
		}
	}
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: LS_ShowCursor          | AUTHOR(s): Rob Probin   | DATE STARTED: 1 May 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void LS_ShowCursor(void)
{
	Zex_ShowCursor(SDL_ENABLE);
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: LS_HideCursor          | AUTHOR(s): Rob Probin   | DATE STARTED: 1 May 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void LS_HideCursor(void)
{
	Zex_ShowCursor(SDL_DISABLE);
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: do_fixed_frame_rate     | AUTHOR(s): Rob Probin   | DATE STARTED: 24 Apr 05
// +
// | DESCRIPTION: 
// |
// | Modified version of the fixed frame rate delay from SDL OpenGL example.
// +----------------------------------------------------------------ROUTINE HEADER----

void do_fixed_frame_rate(bool re_initialise)
{
	const int fps = MAX_FRAME_RATE;
	static int delay = 1000/fps;
	static bool first_run = true;
	static Uint32 thenTicks;
	static Uint32 nowTicks;
	
	if (re_initialise == FRAME_RATE_DELAY)
	{
		//#if equalise
		//oldmac//Microseconds(&end_micros);
		//oldmac//while (end_micros.lo-start_micros.lo <1000000/MAX_FRAME_RATE)	//fps
		//oldmac//	Microseconds(&end_micros);	//wait for time to pass. We should do something here maybe
		
		// Draw at fixed frame rate
		//     This approach is not normally recommended - it is better to
		//     use time-based animation and run as fast as possible
		
		// Time how long each draw-swap-delay cycle takes
		// and adjust delay to get closer to target framerate
		if (!first_run) {
			nowTicks = SDL_GetTicks ();
			delay += (1000/fps - (nowTicks-thenTicks));
			thenTicks = nowTicks;
			if (delay < 0)
				delay = 1000/fps;
		}
		else {
			thenTicks = SDL_GetTicks ();
			first_run = false;
		}
		
		SDL_Delay (delay);
		
		//while (end_micros.lo-start_micros.lo <1000000/24)	//fps
		//Microseconds(&end_micros);	  //wait for time to pass. We should do something here maybe
		
		
		//#endif
	}
	else
	{
		first_run = true;		// make sure the thenTicks is reset
	}
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE: ZexNumToString             | AUTHOR(s): Stu & Rob    | DATE STARTED: Late cretaceous 
// +
// | Replacement for MacOS NumToString
// | Converts an signed integer to a Pascal string
// +----------------------------------------------------------------ROUTINE HEADER----

void ZexNumToString(long number, Str255 string)
{
	unsigned char char_count;
	unsigned int div;
	char_count=1;
	
	if(number<0)
	{
		string[char_count++]='-';
		number = -number;
	}
	// (signed) long int's on a 32 bit machine are between -2,147,483,648 (- (2^31)) and 2,147,483,647 (2^31 - 1)
	// note: we could replace the goto with a switch statement. But still ... this is quicker and the original code
	if (number<10)  goto units;
	if (number<100) goto tens;
	if (number<1000) goto hundreds;
	if (number<10000) goto thousands;
	if (number<100000) goto tenthousands;
	if (number<1000000) goto hunthousands;
	if (number<10000000) goto millions;
	if (number<100000000) goto tenmillions;
	if (number<1000000000) goto hunmillions;
	
	// billions ... 1,000,000,000
	div=number/1000000000;	// 1,000,000,000
	number=number-(div*1000000000);
	string[char_count]=div+'0';
	char_count++;
	
	// some idiot is running this as a 64 bit build. Display overflow.
	if(div > 2)
	{
		//string = "overflow";
		string[1]='x';
		string[0]=1;
		return;
	}
	
hunmillions: div=number/100000000;	// 100,000,000
	number=number-(div*100000000);
	string[char_count]=div+'0';
	char_count++;
	
tenmillions: div=number/10000000;
	number=number-(div*10000000);
	string[char_count]=div+'0';
	char_count++;
	
millions: div=number/1000000;
	number=number-(div*1000000);
	string[char_count]=div+'0';
	char_count++;
	
	
hunthousands: div=number/100000;
	number=number-(div*100000);
	string[char_count]=div+'0';
	char_count++;
	
tenthousands: div=number/10000;
	number=number-(div*10000);
	string[char_count]=div+'0';
	char_count++;
	
thousands: div=number/1000;
	number=number-(div*1000);
	string[char_count]=div+'0';
	char_count++;
	
hundreds: div=number/100;
	number=number-(div*100);
	string[char_count]=div+'0';
	char_count++;
	
tens: div=number/10;
	number=number-(div*10);
	string[char_count]=div+'0';
	char_count++;
	
units: string[char_count]=number+'0';
	string[0]=char_count;
	return;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: RGBForeColor               | AUTHOR(s): Rob          | DATE STARTED: 17 May 2005
// +
// | Replacement for Mac function
// +----------------------------------------------------------------ROUTINE HEADER----

static RGBColor foreground_colour;
static Uint32 foreground_colour_for_SDL;
static unsigned char fade_fore_colour = 0;

void RGBForeColor(const RGBColor * color)
{
	if(fade_fore_colour == 0)
	{
		foreground_colour.red = color->red;
		foreground_colour.green = color->green;
		foreground_colour.blue = color->blue;
	}
	else	// faded version
	{
		int temp = 100-fade_fore_colour;
		foreground_colour.red = (color->red * temp) / 100;
		foreground_colour.green = (color->green * temp) / 100;
		foreground_colour.blue = (color->blue * temp) / 100;
	}
	
	SDL_Surface *screen = Get_back_buffer();
	foreground_colour_for_SDL = SDL_MapRGB(screen->format,(foreground_colour.red)>>8,(foreground_colour.green)>>8,(foreground_colour.blue)>>8);
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: FadeForeColour             | AUTHOR(s): Rob          | DATE STARTED: 12 March 2006
// +
// | Allows draw colour to be faded..
// +----------------------------------------------------------------ROUTINE HEADER----

void FadeForeColour(unsigned char fade_percent)
{
	if(fade_percent <= 100)
	{
		fade_fore_colour = fade_percent;
	}
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: RGBForeColor               | AUTHOR(s): Rob          | DATE STARTED: 21 June 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void set_foreground_colour(unsigned char colour)
{
	foreground_colour_for_SDL = colour;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: RGBBackColor               | AUTHOR(s): Rob          | DATE STARTED: 17 May 2005
// +
// | Replacement for Mac function
// +----------------------------------------------------------------ROUTINE HEADER----

static RGBColor background_colour;
static Uint32 background_colour_for_SDL;

void RGBBackColor(const RGBColor * color)
{
	background_colour.red = color->red;
	background_colour.green = color->green;
	background_colour.blue = color->blue;
	
	SDL_Surface *screen = Get_back_buffer();
	background_colour_for_SDL = SDL_MapRGB(screen->format,(background_colour.red)>>8,(background_colour.green)>>8,(background_colour.blue)>>8);
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE: cls                        | AUTHOR(s): Rob          | DATE STARTED: 17 May 2005
// +
// | clear the screen. From Z_dungeon
// +----------------------------------------------------------------ROUTINE HEADER----
void cls(void)
{	
	Rect our_rect;
	our_rect.left = WIND_LEFT;
	our_rect.top = WIND_TOP;
	our_rect.right = WIND_RIGHT;
	our_rect.bottom = WIND_BOTTOM;
	
	our_fill_rect(&our_rect, background_colour_for_SDL);	
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: our_fill_rect              | AUTHOR(s): Rob          | DATE STARTED: 17 May 2005
// +
// | Common function called by PaintRect and EraseRect
// +----------------------------------------------------------------ROUTINE HEADER----
static void our_fill_rect(const Rect *r, Uint32 the_colour)
{
	// set up the rectangle
	int width = (r->right - r->left) + 1;
	int height = (r->bottom - r->top) + 1 ;
	if(width<0 || height<0)
	{
		report_error_c("PaintRect() width or height error","",0);
	}

	//
	// now find out where the screen bitmap is...
	//
	extern Ptr screenaddr;
	extern int screenwidth;

	Ptr fill_address_dest = screenaddr+(screenwidth*r->top)+r->left;
	
	unsigned char byte_colour = the_colour;
	
 	for(int i=0; i<height; i++)
	{
		memset(fill_address_dest, byte_colour, width);		// write the colour into 1 line
		fill_address_dest += screenwidth;
	}
	
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: PaintRect                  | AUTHOR(s): Rob          | DATE STARTED: 17 May 2005
// +
// | Replacement for Mac function
// +----------------------------------------------------------------ROUTINE HEADER----

void PaintRect(const Rect * r)
{
	our_fill_rect(r, foreground_colour_for_SDL);
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: EraseRect                  | AUTHOR(s): Rob          | DATE STARTED: 17 May 2005
// +
// | Replacement for Mac function
// +----------------------------------------------------------------ROUTINE HEADER----
void EraseRect(const Rect * r)
{
	//SDL_Surface *screen = Get_back_buffer();
	//our_fill_rect(r, SDL_MapRGB(screen->format,255,255,255));			// paint it white
	our_fill_rect(r, background_colour_for_SDL);
}



// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 17 May 2005
// +
// | Replacement for Mac function
// +----------------------------------------------------------------ROUTINE HEADER----
static short my_font = 21;

void TextFont(short font)				// Zex only uses 21
{
	my_font = 21;
	
	if(font != 21)
	{
		report_error_c("Unsupported font passed to TextFont() in mac_emulation.cpp","",0);
	}
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 17 May 2005
// +
// | Replacement for Mac function
// +----------------------------------------------------------------ROUTINE HEADER----
static short my_size = 12;
enum {
	VAR_FONT9,
	VAR_FONT12,
	VAR_FONT14,
	VAR_FONT24,
	NUMBER_OF_FONT_SIZES
};
static int font_size_index = VAR_FONT12;

void TextSize(short size)		// zex uses 9, 12, 14, 24
{
	switch(size)
	{
		case 9:
			my_size = 9;
			font_size_index = VAR_FONT9;
			break;
		case 12:
			my_size = 12;
			font_size_index = VAR_FONT12;
			break;
		case 14:
			my_size = 14;
			font_size_index = VAR_FONT14;
			break;
		case 24:
			my_size = 24;
			font_size_index = VAR_FONT24;
			break;
		default:
			report_error_c("Unsupported font size passed to TextFont() in mac_emulation.cpp","",size);
			my_size = 12;
			font_size_index = VAR_FONT12;
			break;
	}

}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 17 May 2005
// +
// | Replacement for Mac function
// +----------------------------------------------------------------ROUTINE HEADER----
static short my_face=normal;

void TextFace(short face)			// two faces used 0=normal, 1=bold
{
	my_face =face;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 17 May 2005
// +
// | Replacement for Mac function
// +----------------------------------------------------------------ROUTINE HEADER----
static short current_pos_h = 0;
static short current_pos_v = 0;

void MoveTo(short h, short v)
{
	current_pos_h = h;
	current_pos_v = v;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 17 May 2005
// +
// | Replacement for Mac function
// +----------------------------------------------------------------ROUTINE HEADER----
static short my_mode = 1;

void TextMode(short mode)		// Zex uses TextMode 0=srcCopy, 1=srcOr, 4=notSrcCopy (source is inverted before copy)
{								// Since the QuickdrawText was monochrome - or meant leave the background, Copy meant overwrite the background
	my_mode = mode;
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 17 May 2005
// +
// | Replacement for Mac function.
// +----------------------------------------------------------------ROUTINE HEADER----

typedef struct {
	int start_x;				// how far across the graphic starts this font
	int start_y;				// how far down the graphic starts this font
	int character_height_y;		// how high the character is for this font
	vector<int> character_offset_x;	// offset of each character from the left hand side of the graphic, starting at character 32
	vector<int> character_width_x;	// width of each character, starting at character 32
} font_data_t;

class draw_string_data_class {
public:
	// public data (due to the fact this came from a struct definition)
	Handle resource_hdl;
	unsigned char* pixel_data;
	int pixel_width;
	int pixel_height;
	vector<font_data_t> font_specifics;
 
	// member functions
	draw_string_data_class();	// this doesn't initialise the font stuff ... you must call initialise
	bool init_successful();
	void initialise();			// set everything up
	
private:
	void search_pixel_map();		// look for the delineation marks
	void find_font_starts(vector<unsigned char*> &glyph_seperation_marks);		// look for the fond start delineation marks
	void find_character_positions(vector<unsigned char*> &glyph_seperation_marks);	// look for the individual character delineation marks
	
	// private data
	bool init_done;
};

draw_string_data_class::draw_string_data_class()
{
	init_done = false;
}

void draw_string_data_class::initialise()
{
	init_done = false;
	resource_hdl = LS_GetResource('varf',128);
	if(resource_hdl)
	{
		init_done = true;

		unsigned char* my_pointer = (unsigned char*)(*resource_hdl);
		int width = (*my_pointer)*256; my_pointer++;
		width += (*my_pointer); my_pointer++;
		int height = (*my_pointer)*256; my_pointer++;
		height += (*my_pointer); my_pointer++;
		pixel_width = width;
		pixel_height = height;
		pixel_data = my_pointer;
		
		// now go and find the individual fonts and the glyphs for those
		search_pixel_map();
	}
}

bool draw_string_data_class::init_successful()
{
	return init_done;
}

void draw_string_data_class::search_pixel_map()
{
	vector<unsigned char*> glyph_seperation_marks;		// where the glyph marks are per font
	
	find_font_starts(glyph_seperation_marks);

	int number_of_fonts_loaded = font_specifics.size();
	if(NUMBER_OF_FONT_SIZES != number_of_fonts_loaded)
	{
		init_done = false;
		report_error_c("Variable font is broken - not enough fonts loaded\n","",font_specifics.size());
		return;			// not needed
	}
	
	find_character_positions(glyph_seperation_marks);
}

void draw_string_data_class::find_font_starts(vector<unsigned char*> &glyph_seperation_marks)
{	
	font_data_t font_in_construction;
	//
	// first we search down to get the font heights for each font
	//
	unsigned char *pixel_ptr = pixel_data;
	int above_character_set_line = 0;
	bool done_above = false;
	for(int pixel_line=0; pixel_line<pixel_height; pixel_line++)
	{
		if(*pixel_ptr)
		{
			if(! done_above)
			{
				glyph_seperation_marks.push_back(pixel_ptr);	// record top line so we can get the seperation marks in a minute
				above_character_set_line = pixel_line;		// record line number so we can use it in a moment
				done_above = true;
			}
			else	// we now have the above line and the below line (just) so we can add a character set line 
			{
				font_in_construction.start_x = 0;
				font_in_construction.start_y = above_character_set_line+1;	// line below glyph marks
				font_in_construction.character_height_y = (pixel_line-1) - above_character_set_line;	// don't include the top or bottom in the character set marks
				font_specifics.push_back(font_in_construction);	// add a font character set on the vector
				done_above = false;
			}
		}
		pixel_ptr += pixel_width;
	}
	
}

void draw_string_data_class::find_character_positions(vector<unsigned char*> &glyph_seperation_marks)
{
	//
	// we search cross each top font row to find the inidividual font glyph widths and record those in a vector along with each specific character offset
	//
	int number_of_fonts_loaded = font_specifics.size();
	for(int font=0; font < number_of_fonts_loaded; font++)
	{
		unsigned char *pixel_ptr = glyph_seperation_marks[font];
		int previous_position = -1;
		
		for(int pixel_column=0; pixel_column<pixel_width; pixel_column++)
		{
			if(*pixel_ptr)
			{
				if(previous_position != -1)		// don't do anything for the first mark
				{
					// add on the data for the previous character
					font_specifics[font].character_offset_x.push_back(previous_position);
					font_specifics[font].character_width_x.push_back(pixel_column-previous_position);		// width of character
				}
				
				previous_position = pixel_column;
			}
			
			pixel_ptr ++ ;
		}
	}	
}

static draw_string_data_class draw_string_data;		// define a local instance

//
// Get font height of the currently set font
//
int get_font_height()
{
	//
	// Make sure the font is loaded
	//
	if( ! draw_string_data.init_successful())	// if not initialised
	{ 
		draw_string_data.initialise();							// try to initialise
		if( ! draw_string_data.init_successful())	// if still not initialised ... fail
		{ 
			report_error_c("Couldn't initialise variable font in StringWidth() - mac_emulation.cpp","",0);
			return 100;		// random value for the compiler
		}
	}

	return draw_string_data.font_specifics[font_size_index].character_height_y;
}


//
// C input version of Drawstring
//
void DrawString_c(const char *c_string)
{
	Str255 p_string;
	CToPascal(c_string, p_string);
	DrawString(p_string);
}

//
// Pascal version of Drawstring
//
void DrawString(const unsigned char *pascal_string)
{
	// zex uses text sizes 9, 12, 14, 24
	// zex uses text modes 0, 1 and 4.
	// zex uses text faces normal and bold. But we only support normal
#define selector 2
#if selector == 0
	char c_string[256];
	PascalToC(pascal_string, c_string);
	SplatText(c_string, current_pos_h, current_pos_v, foreground_colour_for_SDL, background_colour_for_SDL);
#elif selector == 2
	if( ! draw_string_data.init_successful())	// if not initialised
	{ 
		draw_string_data.initialise();							// try to initialise
		if( ! draw_string_data.init_successful())	// if still not initialised ... fail
		{ 
			report_error_c("Couldn't initialise variable font in DrawString() - mac_emulation.cpp","",0);
			return;		// theoretical return
		}
	}

	// load the base font details
	int source_pixel_buffer_width = draw_string_data.pixel_width;
	int source_char_start_x = draw_string_data.font_specifics[font_size_index].start_x;
	int source_char_start_y = draw_string_data.font_specifics[font_size_index].start_y;
	vector<int> &source_char_width_x = draw_string_data.font_specifics[font_size_index].character_width_x;
	int source_char_height_y = draw_string_data.font_specifics[font_size_index].character_height_y;
	vector<int> &source_char_offset = draw_string_data.font_specifics[font_size_index].character_offset_x;
	
	// calculate the screen destination position
	extern Ptr screenaddr;
	extern int screenwidth;
	
	unsigned char* double_buffer=(unsigned char*)screenaddr;
	int screen_width=screenwidth;

	int x_pos = current_pos_h;
	int y_pos = current_pos_v - source_char_height_y;			// subtract height to get to position at top of character (DrawString expects us to draw at the bottom of the character)

	if(x_pos < 0) { report_error("Attempt to draw text out of bounds in DrawString() (x<min)", pascal_string, x_pos); }
	if(y_pos < 0) { report_error("Attempt to draw text out of bounds in DrawString() (y<min)", pascal_string, y_pos); }
	if(current_pos_v > 479) { report_error("Attempt to draw text out of bounds in DrawString() (y>max)", pascal_string, y_pos); }
	if((x_pos+StringWidth(pascal_string))>639) { report_error("Attempt to draw text out of bounds in DrawString() (x>max)", pascal_string, x_pos); }
	y_pos+=2;	//not sure why we're off by 2 in the vertical, but we are
	
	unsigned char *db_ptr=(unsigned char *) (double_buffer+x_pos+(y_pos*screen_width));
	
	// calculate the character base pointer
	unsigned char *char_base_ptr = draw_string_data.pixel_data + source_char_start_x + (source_char_start_y * source_pixel_buffer_width);
		
	// fetch the colours
	int bg_colour = background_colour_for_SDL;
	int fg_colour = foreground_colour_for_SDL;
	if(my_mode == 4)	// mode 4 ( notSrcCopy inverts the colours)
	{
		bg_colour = foreground_colour_for_SDL;
		fg_colour = background_colour_for_SDL;
	}

	// this is now the character drawing section
	int length = *pascal_string++;

	for(int i=0; i<length; i++)
	{
		int character = *pascal_string++;
		
		// calculate the character position
		int num_chars = source_char_width_x.size();
		if(character < 32 || character-32 >= num_chars) { continue; }	//we do get Cr's passed through, lets just ignore them
		character -= ' ';

		unsigned char *char_ptr = char_base_ptr + (source_char_offset[character]);
		
		if(my_mode != 1) { // assume TextMode == 0 srcCopy (i.e. overwrite background) or TextMode == 4 (invert srcCopy)
			draw_char_copy(char_ptr, source_char_width_x[character], source_char_height_y, source_pixel_buffer_width, db_ptr, screen_width, fg_colour, bg_colour); 
		}
		else { // TextMode == 1 srcOr (i.e. don't overwrite background)
			draw_char_or(char_ptr, source_char_width_x[character], source_char_height_y, source_pixel_buffer_width, db_ptr, screen_width, fg_colour); 
		}

		db_ptr += source_char_width_x[character];		// add on x width plus some to destination pointer
		//db_ptr++;	// for testing
		current_pos_h += source_char_width_x[character];	// make sure drawing reference pointer has been updated
	}
	
#endif

}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 24 July 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

static void draw_char_copy(unsigned char *char_to_draw_from, int char_width, int char_height, int char_line_step, unsigned char *dest_ptr, int dest_line_step, unsigned char fg_colour, unsigned char bg_colour)
{
	unsigned char *db_ptr = dest_ptr;
	unsigned char *char_ptr = char_to_draw_from;

	for(int line=0; line < char_height; line++)
	{
		for(int col=0; col < char_width; col++)
		{
			if (*char_ptr != 0) { *db_ptr = fg_colour; }
			else { *db_ptr = bg_colour; }
			
			char_ptr++;
			db_ptr++;
		}
		
		char_ptr += char_line_step - char_width;
		db_ptr += dest_line_step - char_width;
	}
	
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 24 July 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

static void draw_char_or(unsigned char *char_to_draw_from, int char_width, int char_height, int char_line_step, unsigned char *dest_ptr, int dest_line_step, unsigned char fg_colour)
{
	unsigned char *db_ptr = dest_ptr;
	unsigned char *char_ptr = char_to_draw_from;
	
	for(int line=0; line < char_height; line++)
	{
		for(int col=0; col < char_width; col++)
		{
			if (*char_ptr != 0) { *db_ptr = fg_colour; }
			
			char_ptr++;
			db_ptr++;
		}
		
		char_ptr += char_line_step - char_width;
		db_ptr += dest_line_step - char_width;
	}
	
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 17 May 2005
// +
// | Replacement for Mac function
// +----------------------------------------------------------------ROUTINE HEADER----
short StringWidth (const unsigned char *pascal_string)
{	
	if( ! draw_string_data.init_successful())	// if not initialised
	{ 
		draw_string_data.initialise();							// try to initialise
		if( ! draw_string_data.init_successful())	// if still not initialised ... fail
		{ 
			report_error_c("Couldn't initialise variable font in StringWidth() - mac_emulation.cpp","",0);
			return 100;		// random value for the compiler
		}
	}
	
	int calculated_string_width = 0;		// this is what we want to get to...
	vector<int> &source_char_width_x = draw_string_data.font_specifics[font_size_index].character_width_x;

	int length = *pascal_string++;	
	for(int i=0; i<length; i++)
	{
		int character = *pascal_string++;
		
		// calculate the character position
		int num_chars = source_char_width_x.size();
		if(character < 32 || character-32 >= num_chars) { character = '?'; }
		character -= ' ';

		calculated_string_width += source_char_width_x[character];	// make sure drawing reference pointer has been updated
	}

	
	return calculated_string_width;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 17 May 2005
// +
// | Replacement for Mac function
// +----------------------------------------------------------------ROUTINE HEADER----
void LineTo (short h, short v)
{
	draw_line(current_pos_h, current_pos_v, h, v, foreground_colour_for_SDL);	// x1, y1, x2, y2, colour
	
	// update the graphics pen position
	current_pos_h = h;
	current_pos_v = v;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 19 May 2005
// +
// | Replacement for Mac function
// +----------------------------------------------------------------ROUTINE HEADER----

void FrameRect(const Rect * r)
{
	// we emulate this by drawing a box
	MoveTo(r->left, r->top);
	LineTo(r->right, r->top);
	LineTo(r->right, r->bottom);
	LineTo(r->left, r->bottom);
	LineTo(r->left, r->top);
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 17 May 2005
// +
// | Replacement for Mac function
// +----------------------------------------------------------------ROUTINE HEADER----
struct IndStringDataStruct {
	bool used;
	unsigned short resource_id;
	unsigned short number_of_entries;
	Handle resource_handle;
};

const int NUMBER_OF_IND_STRING_DATA_ELEMENTS = 4;

static struct IndStringDataStruct ind_string_data[NUMBER_OF_IND_STRING_DATA_ELEMENTS];

void GetIndString (Str255 theString, short strListID, short index)
{	
	int match_id = -1;
	
	//
	// first search to see if it's an existing data elemnt
	//
	for(int i=0; i<NUMBER_OF_IND_STRING_DATA_ELEMENTS; i++)
	{
		if(ind_string_data[i].used)
		{
			if(ind_string_data[i].resource_id == strListID)
			{
				// we have a match!!!
				match_id = i;
				break;
			}
		}
	}
	
	//
	// no match, let's load the resource - if there is space
	//
	if(match_id == -1)		// this one isn't loaded
	{
		for(int i=0; i<NUMBER_OF_IND_STRING_DATA_ELEMENTS; i++)
		{
			if(!ind_string_data[i].used)
			{
				// free space - let's load the resource here
				ind_string_data[i].resource_handle = LS_GetResource('STR#', strListID);
				ind_string_data[i].used = true;
				ind_string_data[i].resource_id = strListID;
				#if (ARCH_LITTLE_ENDIAN)
				ind_string_data[i].number_of_entries = Endian16_Swap(*reinterpret_cast<unsigned short*>(*(ind_string_data[i].resource_handle)));
				#else
				ind_string_data[i].number_of_entries = *reinterpret_cast<unsigned short*>(*(ind_string_data[i].resource_handle));
				#endif
				match_id = i;
				break;
			}
		}
		
		if(match_id == -1)
		{
			report_error_c("No space in GetIndString()","",strListID);
			return;	// never get's here
		}
		
	}
	
	//
	// we now have found or loaded the resource ... let's extract the required string
	//
	if(index > ind_string_data[match_id].number_of_entries || index < 1)
	{
		report_error_c("Index out of range of data in resource inside GetIndString()","",strListID);
		return;	// never get's here
	}
	
	char *string_list = *(ind_string_data[match_id].resource_handle);	
	string_list += 2;			// skip over resource number of entries field

	// find the string we are interested in...
	for(int i=0; i < (index-1); i++)
	{
		string_list += (UInt8)*string_list;	// skip over this pascal name by using the string size field
		string_list++;						// include length
	}

	// copy the pascal string to the target
	int string_size = (UInt8)*string_list;
	for(int i=0; i < (string_size+1); i++)		// copy the size as well
	{
		*theString = *string_list;
		theString++; string_list++;
	}
	// and we are done...

}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 18 May 2005
// +
// | Replacement for Mac function
// +----------------------------------------------------------------ROUTINE HEADER----
Handle NewHandleClear(unsigned long byteCount)
{
	Handle new_handle = NewHandle(byteCount);
	if(new_handle)		// only blank data that exists...
	{
		// I'm sure there is a better way to do this with new and C++ but for the 
		// moment I'm going to take the easy wayout ...
		memset(*new_handle, 0, byteCount);		// write zero to all bytes requested
	}		
	return new_handle;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 18 May 2005
// +
// | Replacement for Mac function
// +----------------------------------------------------------------ROUTINE HEADER----
Handle NewHandle(unsigned long byteCount)
{
	// get some free space
	int handle_index = GetEmptyFreeHandle();
	Handle new_handle = &(complex_handle[handle_index].pointer);

	*new_handle = new(nothrow) char[byteCount];

	// store the size for later (maybe)
	complex_handle[handle_index].size = byteCount;
	complex_handle[handle_index].is_really_resource = false;
	
	if(*new_handle == 0)		// if we can't get the space (zero is a a blank handle anyway)
	{
		return 0;		// return a NULL
	}
	
	return new_handle;
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 22 May 2005
// +
// | Replacement for Mac function
// |
// | The ScrollRect function shifts pixels that are inside the specified rectangle of the current graphics port.
// | The empty area created by the scrolling is filled with the graphics port background colour.
// +----------------------------------------------------------------ROUTINE HEADER----

void ScrollRect(const Rect * r, short dh, short dv, int updateRgn)
{
	// Parameters:
	// dh, dv - distances to be scrolled
	// r - rectangle to be scrolled
	// update region handle to be scrolled (not supported)
	
	//
	// make variables I can understand...
	//
	int x1 = r->left;
	int x2 = r->right;
	int y1 = r->top;
	int y2 = r->bottom;
	int rect_width = (x2-x1)+1;
	int rect_height = (y2-y1)+1;	

	//
	// let's validate these
	//
	if(x1 < WIND_LEFT || x1 > WIND_RIGHT  || rect_width<0  ||
	   x2 < WIND_LEFT || x2 > WIND_RIGHT  ||
	   y1 < WIND_TOP  || y1 > WIND_BOTTOM || rect_height<0 ||
	   y2 < WIND_TOP  || y2 > WIND_BOTTOM
	   )
	{
		report_error_c("Scroll Rect illegal coordinates","",0);
		return;
	}
	
	int x_shift = dh;
	int y_shift = dv;
	
	// it turns out Zex doesn't use dh - so check this and issue a warning if someone changes the code
	if(x_shift)
	{
		report_error_c("Horizontal scrolling not supported yet","",0);
		return;
	}
	// it also turns out that Zex doesn't use a positive (or zero) dv. Check this and issue a warning because I haven't tested it.
	if(y_shift >= 0)
	{
		report_error_c("Downwards scrolling not supported yet","",0);
		return;
	}
	y_shift = -y_shift;				// ensure we use the correct shift direction
	
	//
	// now find out where the screen bitmap is...
	//
	extern Ptr screenaddr;
	extern int screenwidth;
	
	// 
	// the first section is the area to scroll
	// 
	int scroll_height = rect_height-y_shift;			// the area we actually have to scroll ... so a 5 pixel high window scrolled 1 pixel would be 4 pixel lines to scroll

	// we now need to move the pixels ... these calculations assume an 8 bit screen
	Ptr scroll_address_dest = screenaddr+(screenwidth*y1)+x1;					// assuming negative y scrolling only, then the destination is at the top of the screen
	Ptr scroll_address_src = screenaddr+(screenwidth*(y1+y_shift))+x1;			// it comes from just down a bit (y_shift)
	
	for(int i=0; i<scroll_height; i++)			// should work if scroll is bigger than the screen
	{
		memcpy(scroll_address_dest, scroll_address_src, rect_width);		// copy 1 line
		scroll_address_dest += screenwidth;
		scroll_address_src += screenwidth;
	}
	
	//
	// now we need to fill the older pixels with the background colour
	//
	int fill_height = rect_height-scroll_height;		// remainder of the area
	
	for(int i=0; i<fill_height; i++)
	{
		memset(scroll_address_dest, background_colour_for_SDL, rect_width);		// write the background colour into 1 line
		scroll_address_dest += screenwidth;
	}
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: LS_NormalGamma             | AUTHOR(s): Rob          | DATE STARTED: 1 Jan 2006
// +
// | Gets rid of any Gamma fade.
// |
// +----------------------------------------------------------------ROUTINE HEADER----

void LS_NormalGamma()
{
	LS_FadeGamma(100,0);
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE: LS_FadeGamma               | AUTHOR(s): Rob          | DATE STARTED: 30 Dec 2005
// +
// | Replacement for Mac function DSpContext_FadeGamma
// |
// +----------------------------------------------------------------ROUTINE HEADER----
/*
 Parameters
	 dummy_Context
	 Originally, a reference to the context whose display is to be faded. If you pass NULL for this 
     parameter, the fade operation applies simultaneously to all displays. Now you should pass NULL 
     - but it's currently ignored.
	 
	 inPercentOfOriginalIntensity
	 The percentage (0–100) of the display’s full intensity that you want to achieve with this call. 
     Values above 100 percent begin to converge on white. If you have specified an intensity color, 
     values less than zero begin to converge on black.
 
	 inZeroIntensityColor
	 A pointer to the color that is to correspond to zero intensity (represented by a value of 0 in 
	 the inPercentOfOriginalIntensity parameter). If you pass NULL for this parameter, the zero-intensity 
     color is black.
 
 Return Value
	 A result code. However, it currently always returns 0 (noErr) even if the fade is ignored.
 
 Discussion
	LS_FadeGamma performs a gamma fade, which gives better results than a simple indexed fade.
 
    Fading using LS_FadeGamma is an incremental process. That is, over a period of time, you 
	make repeated, timed calls to LS_FadeGamma, each time passing it an incrementally different 
    value for the inPercentOfOriginalIntensity parameter, until the final desired intensity is achieved. 
    The intensity value you pass is usually an integer between 0 and 100. It can be greater than 100, if 
	you want to use fading to create a high-intensity burst of light, or less than 100 if you have 
	specified a zero-intensity color and want to fade the color toward black.
 
    The zero-intensity value that you fade out to is by default black, but it can be any color that you 
    specify in the inZeroIntensityColor parameter. You can achieve special effects by fading partially 
    toward one zero-intensity color and then completing the fade to a different one. 
 
    To automatically accomplish a smooth fade all the way from full intensity to zero intensity, or vice 
    versa, in a single operation, use the function DSpContext_FadeGammaIn and the DSpContext_FadeGammaOut function.
 */

static RGBColor black = { 0,0,0 };
static RGBColor white = { 0xffff, 0xffff, 0xffff };

enum GammaFadeType { OffFade, SDLGammaFade, PaletteFade, PixelTranslationFade };
// notice: PixelTranslationFade is broken ... I think the problem is in translated_surface_copy()

GammaFadeType select_gamma_fade_type = PaletteFade;


OSStatus LS_FadeGamma(SInt32 inPercentOfOriginalIntensity, RGBColor *inZeroIntensityColor)
{
	RGBColor *dest = &black;
	if(inZeroIntensityColor != 0)			// if there is no destination colour, use black
	{
		dest = inZeroIntensityColor;
	}

	RGBColor *start = &white;
	if(inPercentOfOriginalIntensity < 0)		// if we are below zero we are fading from colour to black
	{
		inPercentOfOriginalIntensity += 100;
		dest = &black;
		start = inZeroIntensityColor;
	}

	if(inPercentOfOriginalIntensity > 100 || inPercentOfOriginalIntensity < 0)
	{
		report_error_c("FadeGamma percentage out of supported range","",inPercentOfOriginalIntensity);
	}

	// fade is short hand name for inPercentOfOriginalIntensity
	int fade = inPercentOfOriginalIntensity;		// 100% is full brightness
													// 0% is inZeroIntensityColor

	// fade should give us a mid_point between white and destination
	int fade_red = (dest->red * (100-fade)) + (start->red * fade);
	int fade_green = (dest->green * (100-fade)) + (start->green * fade);
	int fade_blue = (dest->blue * (100-fade)) + (start->blue * fade);

	fade_red /= 100; fade_green /= 100; fade_blue /= 100;

	//
	// Select the method of fading
	//
	if(select_gamma_fade_type == OffFade)
	{
		// do nothing
	}
	if(select_gamma_fade_type == SDLGammaFade)
	{
		if(fade == 100)			// ensure full gamma if set, since floating point compares inaccurate
		{
			SDL_SetGamma(1.0f, 1.0f, 1.0f);
		}
		else
		{
			SDL_SetGamma(fade_red/65535.0, fade_green/65535.0, fade_blue/65535.0);
		}
	}
	else if(select_gamma_fade_type == PaletteFade)
	{
		set_clut_modification(fade_red, fade_green, fade_blue);
	}
	else if(select_gamma_fade_type == PixelTranslationFade)
	{
		// Theoretically we need to convert all the pixels to RGB and then convert them 
		// back to pixel colours once we have reduced levels. The actual pixel colours
		// are static because in this mode we are not altering the palette.
		
		unsigned char pixels_translation_array[256];
		SDL_Surface *screen = Get_back_buffer();
		Uint8 r,g,b;

		for(unsigned int i=0; i<256; i++)
		{
			SDL_GetRGB(i, screen->format, &r, &g, &b);
			r = (r*fade_red) / 0xffff;
			g = (g*fade_green) / 0xffff;
			b = (b*fade_blue) / 0xffff;
			pixels_translation_array[i] = SDL_MapRGB(screen->format,r,g,b);			
		}
		set_translation_array(pixels_translation_array);
		enable_pixel_translation();
	}
	else
	{
		report_error_c("Invalid gamma fade selected","", select_gamma_fade_type);
	}
	
	return 0;
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 15 Jan 2006
// +
// |
// +----------------------------------------------------------------ROUTINE HEADER----

void safe_number_id_to_4_character_string(unsigned int id, char* string_out)
{
	string_out[0] = 0xff & (id>>24);
	string_out[1] = 0xff & (id>>16);
	string_out[2] = 0xff & (id>>8);
	string_out[3] = 0xff & (id);	
	
	for(int i = 0; i < 4; i++)
	{
		if(string_out[i] < 32)
		{
			string_out[i] = '?';
		}
#if CHAR_MAX > 127
		if(string_out[i] > 127)
		{
			string_out[i] = '?';			
		}
#endif
	}
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 15 Jan 2006
// +
// |
// +----------------------------------------------------------------ROUTINE HEADER----

void safe_number_id_to_4_character_string(unsigned int id, unsigned char* string_out)
{
	string_out[0] = 0xff & (id>>24);
	string_out[1] = 0xff & (id>>16);
	string_out[2] = 0xff & (id>>8);
	string_out[3] = 0xff & (id);	
	
	for(int i = 0; i < 4; i++)
	{
		if(string_out[i] < 32)
		{
			string_out[i] = '?';
		}
		if(string_out[i] > 127)
		{
			string_out[i] = '?';			
		}
	}
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 22 Jan 2006
// +
// |
// +----------------------------------------------------------------ROUTINE HEADER----

#if DEBUG_GAME_STATE!=0
void debug_printf(debug_enum level, const char*s)
{
	if(level <  DEBUG_GAME_STATE)
	{
		fprintf(stderr, s); 
	}
}
#endif
// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 22 Jan 2006
// +
// |
// +----------------------------------------------------------------ROUTINE HEADER----

#if DEBUG_GAME_STATE!=0
void debug_printf(debug_enum level, const char*s, int i)
{
	if(level < DEBUG_GAME_STATE)
	{
		fprintf(stderr, s, i);
	} 
}
#endif


/* CVS LOG
*
* $Log: mac_emulation.cpp,v $
* Revision 1.41  2006/04/02 11:49:00  rob
* Zex2005: Finally fixed the keys disappearing on the save screen bug. Remember, don't take keys from the event queue from two places ... even by accident via Show_It & get_mouse_x_and_y.
*
* Revision 1.40  2006/03/19 12:34:22  rob
* Zex2005: Highlight rather than naff arrow for menus.
*
* Revision 1.39  2006/02/05 19:30:22  rob
* Zex2005: Debug stuff, currently disabled.
*
* Revision 1.38  2006/01/22 12:36:05  rob
* Zex2005: Modify 4 digit callsign print to be safe even if callsign is not ASCII.
*
* Revision 1.37  2006/01/01 17:48:31  rob
* Zex2005: Added back in screen fading, especially for fast dock.
*
* Revision 1.36  2005/12/28 03:49:16  stu
* Zex2005: Code: Fixes for: 1. get_memory_for_dyn_text exec warning in DLP. 2. Galactic Map STOP if player flies outside current quadrant (strings). 3. Fly off right side of the map and bad things(tm) happen. This fixes #77 and #83 and if we had a ticket for the dyn_textures messages, it fixes that too.
*
* Revision 1.35  2005/12/10 22:57:43  stu
* Zex2005: Code: Training missions should now end correctly
*
* Revision 1.34  2005/12/10 21:04:53  rob
* Zex2005: Full screen on intel mac change.
*
* Revision 1.33  2005/12/10 10:50:55  rob
* Zex2005: Fixed full screen cursor flicker (hopefully) by implementing our own cursor.
*
* Revision 1.32  2005/11/16 21:12:14  rob
* Zex2005: Update that was causing all sorts of pushed buttons to appear due to the menu system stealing events away from the state based main event loop.
*
* Revision 1.31  2005/11/15 22:20:55  rob
* Zex2005: File selector patched in.
*
* Revision 1.30  2005/11/13 01:08:34  stu
* Zex2005: Code: A few fixes
*
* Revision 1.29  2005/11/08 22:44:51  stu
* Zex2005: Code: Various bug fixes as per the TODO
*
* Revision 1.28  2005/10/30 22:21:44  stu
* Zex2005: Code: Peak handle usage monitor
*
* Revision 1.27  2005/10/12 19:22:32  rob
* ZEX2005: Hopefully endian issues for Windows will now be sorted out.
*
* Revision 1.26  2005/10/12 18:32:05  stu
* Zex2005: Code: LS_GetResource now returns to caller if FNF
*
* Revision 1.25  2005/10/10 21:27:24  rob
* Zex2005: Added newline at end to stop compiler whinging.
*
* Revision 1.24  2005/10/08 13:14:05  rob
* Zex2005: "/p" pascal string size marker is not supported in normal gcc - it's a apple specific extension turned on by the option -fpascal_strings. This change removes that string so that the code will run both on Mac and Windows. Lots of files have been changed - but nearly all the changes are trivial. By the fun-jobs-are-us dept.
*
* Revision 1.23  2005/10/03 23:20:39  rob
* Zex2005: Single text line - normally commented out.
*
* Revision 1.22  2005/10/02 23:44:07  rob
* Zex2005: String Width function provided. Seems to work ok - needs more debugging.
*
* Revision 1.21  2005/10/02 23:23:35  rob
* Zex2005: Initial check-in of variable font support. Large fonts are still broken and no font width routine yet. However, small fonts are ok.
*
* Revision 1.20  2005/09/29 00:48:38  stu
* Zex2005: Code: Basic x86 port
*
* Revision 1.19  2005/07/30 11:49:00  rob
* Zex2005: Provisional check-in for DrawString fonts.
*
* Revision 1.18  2005/06/27 21:46:46  rob
* Zex2005: cls, EraseRect, PaintRect (via our_fill_rect) now all draw to our software buffer rather than the SDL screen. This means that, for instance, the show status screen whilst dock erases the background when running.
*
* Revision 1.17  2005/06/23 21:09:09  rob
* Zex2005: Added call to set RGB colour as fixed byte. Patched in draw_line and fixed problems.
*
* Revision 1.16  2005/06/18 14:57:30  rob
* Zex2005: Key setting now working.
*
* Revision 1.15  2005/06/04 11:17:33  rob
* Zex2005: GetKeys is no more! Replaced with emulation function. Therefore auxilliary keys should now work.
*
* Revision 1.14  2005/06/04 08:54:06  rob
* Zex2005: We now have keyboard control in the game!!!
*
* Revision 1.13  2005/06/02 21:40:38  rob
* Zex2005: mac emulation changes. Added code to allow an already loaded resource handle to be returned. Strange things might happen if one of the users disposes of this resource handle and the other one doesn't do a LS_GetResource() but just continues to use the (now deleted) handle. Just detaching of the resource should be relatively safe but probably not advised. Also added code to check if a handle function is called on a resource.
*
* Revision 1.12  2005/06/01 19:06:47  rob
* Zex2005: Updated project - now runs main menu.
*
* Revision 1.11  2005/05/27 19:24:47  rob
* Zex2005: Resource load fix.
*
* Revision 1.10  2005/05/23 21:13:15  rob
* Zex2005: More changes to files.
*
* Revision 1.9  2005/05/20 21:25:49  rob
* Zex2005: More file changes.
*
* Revision 1.8  2005/05/19 20:24:35  rob
* Zex2005: More files changed.
*
* Revision 1.7  2005/05/17 16:22:03  rob
* Zex2005: Even more work sorting out build problems on gcc4.0/MacOSX/SDL conversion.
*
* Revision 1.6  2005/05/17 13:11:10  rob
* Zex2005: Various assorted conversion fixes.
*
* Revision 1.5  2005/05/15 19:32:22  rob
* Zex2005: Moved ZexNumToString to mac_emulation layer.
*
* Revision 1.4  2005/05/15 17:42:54  rob
* Zex2005: ZexNumToString used throughout, file case changes, general gcc4.0 build changes.
*
* Revision 1.3  2005/05/13 09:24:46  rob
* Zex2005: Changed Button to internal version. Altered string to dec code to use atof().
*
* Revision 1.2  2005/05/03 18:54:06  rob
* Zex2005: LS_Get_Resource implementation.
*
* Revision 1.1  2005/05/02 10:26:49  rob
* Zex2005: More Updates (code)
*
*
*
*/



