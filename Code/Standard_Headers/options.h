// *****************************************************************MODULE HEADER*****
// * FILENAME		: options.h
// * MODULE TITLE	: ============ZEX BUILD OPTIONS FILE=========
//
// * PROJECT		: Zex
// *
// * COPYRIGHT (c) 1999 Lightsoft
// *		   (c) 1999 Robert Probin and Stuart Ball
// *		   http://www.lightsoft.co.uk/
// *
// * Lightsoft is a trading name of Robert Probin and Stuart Ball.
// *
// ***********************************************************************************
// Notes:
//
// All flags should be 1 or 0 to enable or disable, NOT commented out/uncommented, since you cannot
// tell if a header (e.g. options.h) has been left out, rather than disabled. The end
// result is that you can have different parts of the program built in different manners.
//
// This file should always be first include in a source file.
//


#define PCI_TIMING_DEBUG	1		// use PCI timing to generate percentage time spent in different sections
						// DISABLE FOR TEST AND FINAL BUILDS - non-pci macs don't support this!

#define SHOW_PERCENTAGES_DEBUG 0		// show percentages of different sections on screen
						// best to run it without splat in vbl !!!!! (all interrupt costs are hidden)


#define Z_DEBUG 1	//if 1, Pers checks Z coordinates are not behind the camera


#define USE_EXEC_WATCHDOG 0	//if set, vbl task decs exec_watchdog. If it gets to zero we error!

#define MESSAGES_CENTER 0	//all messages centered if 1
#define TEXT_DISPLAY_TIME 600	//frame count for a message to stay on screen
#define OGL 1	//set to 1 for OGL version, 0 for SW renderer
#define FIRST_PERSON 1	//set to 1 to enable first person view (Zex 2 ish)


//
// Flags for protecting existing code, whilst porting it over to a more portable system.
//
#define PORTABLE_ZEX 0

#if PORTABLE_ZEX
    #define PORTABLE_RUNNING_FROM_SDL 0		// not currently running from SDL
    #define PORTABLE_FILESYSTEM	1		// these labels (and all the #if code) can disappear as they become the "defacto" Zex sub-systems
    #define REMOVE_OLD_FILESYSTEM_JUNK 0 	// only used this when all the filesystem stuff is removed
#endif

