//zex.defs
#ifndef __zex_defs__
#define __zex_defs__
#endif

#define MAX_NUMBER_OF_OBJECTS 1000

#define MAX_VELOCITY 360	//zex's maximum velocity

#if TEST_SYSTEM==1
#define VERS_STRING "\pV0.70 Dev TEST SYSTEM 031101"
#else
#define VERS_STRING "\pV0.70 Dev 031101 #16206"
#endif

#define DATE_STRING "\p 03 Nov 01 "

#define ENS 0
#define LT 1
#define LTCR 2
#define CDR 3
#define CAPT 4
#define RADM	5
#define VADM 6
#define ADM 7
#define FADM 8
//level 1 laser bay temeprature rise and fall rate
#define LASER_TEMP_RISE_RATE 500
#define LASER_TEMP_COOL_RATE 500
#define MAX_LASER_TEMP 1500


// Zex file location

#if PORTABLE_FILESYSTEM				// remain until each subsystem is fully up and running

    #if PORTABLE_RUNNING_FROM_SDL
        #define BASE_DIRECTORY "ZD3/"		// The top level directory of the data files
    #else
        #define BASE_DIRECTORY "../../ZD3/"	// when we are running from SDL this won't need the ../../
    #endif

    #define RESOURCE_DIRECTORY "R/"
    #define SCRIPT_DIRECTORY "W/"
    #define OBJECT_DIRECTORY "O/"
    #define TEXTURE_DIRECTORY "T/"
    #define LEVEL_DIRECTORY "L/"
    #define INIT_DIRECTORY "Init/"

#else	// remove after all file system is portable

    #define DirStr "\pZD3"	//what folder we are running from

#endif

