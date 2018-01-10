// Zex 2 - 3D Space Trading Game with in-cockpit view 
// Copyright (C) 1998-2018  Stuart Ball and Robert Probin
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

/********************************************************/
/*
mod_player.cpp
Zex Mod Player
SB: Dec 98
Update: 20 Apr 99 - Surround and deluxe for G3's/4's
RP: Hacked 15 Oct 01 to work with direct PP source interface.
SB: 2018-01-09 - todo: replace with SDLMixer
*/
/********************************************************/
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
#endif

#include "ppsound.h"

#include "RDriver.h"		// player pro's heaer

#include "Zsound.h"


//#include "file_io.ext"
//#include "Error_handler.ext"
//
//CFragConnectionID my_connID;
//Ptr mainaddr;
//Ptr MADInitLibrary_addr;
//Ptr MADGetBestDriver_addr;
//Ptr MADCreateDriver_addr;
//Ptr MADDisposeDriver_addr;
//Ptr MADStartDriver_addr;
//Ptr MADStopDriver_addr;
//Ptr MADPlayMusic_addr;
//Ptr MADStopMusic_addr;
//Ptr MADDisposeMusic_addr;
//Ptr MADLoadMusicPtr_addr;
//Ptr MADAttachDriverToMusic_addr;
//Ptr MADDisposeLibrary_addr;
//
//void get_playerpro_shared_lib(void);
//
//Handle mod_handle;
//
//void get_playerpro_shared_lib(void)
//{
//Str255 errname;
//OSErr my_err;
//CFragSymbolClass my_sym_class;
//Str63 fragname = "\pMOD Plug Carbon";
//int hsize;
//
////my_err=GetSharedLibrary(fragname,kPowerPCCFragArch,kLoadCFrag,&my_connID,&mainaddr,errname);
//
//mod_handle=ZGetResource('MODP',128);
//if (mod_handle==0) report_error("Couldn't load music Library.","\p",304);
//MoveHHi(mod_handle);
//HLock(mod_handle);
//hsize=GetHandleSize(mod_handle);
//my_err=GetMemFragment(*mod_handle, hsize, fragname, kPrivateCFragCopy, &my_connID, &mainaddr, errname);
//
//if(my_err) { report_error("Problem loading music library",errname,my_err); }
//
//my_err=FindSymbol(my_connID,"\pMADInitLibrary",&MADInitLibrary_addr,&my_sym_class);
//if(my_err) { report_error("Problem using music library","\p",my_err); }
//if(my_sym_class != kTVectorCFragSymbol ) { report_error("Problem using music library","\p",301); }	  // check it is a PPC code label
//
//my_err=FindSymbol(my_connID,"\pMADGetBestDriver",&MADGetBestDriver_addr,&my_sym_class);
//if(my_err) { report_error("Problem using music library","\p",my_err); }
//if(my_sym_class != kTVectorCFragSymbol ) { report_error("Problem using music library","\p",301); }	  // check it is a PPC code label
//
//report_error("Got to here ok - add more defs","\p",100);
//
//}
//
//
//typedef OSErr (*MADInitLibrary_t)( char *PlugsFolderName, Boolean sysMemory, MADLibrary **MADLib);  // Library initialisation, you have to CALL this function if you want to use other functions & variables
//typedef void  (*MADGetBestDriver_t)( MADDriverSettings *DriverInitParam); // Found and identify the current Mac sound hardware and fill DriverInitParam
//typedef OSErr (*MADCreateDriver_t)( MADDriverSettings *DriverInitParam, MADLibrary *MADLib, MADDriverRec** returnDriver); // Music Driver initialization and memory allocation
//typedef OSErr (*MADDisposeDriver_t)( MADDriverRec *MDriver); // Dispose the music driver, use it after RInitMusic()
//typedef OSErr (*MADStartDriver_t)( MADDriverRec *MDriver); // NEW - Activate the sound generating procedure (interruption)
//typedef OSErr (*MADStopDriver_t)( MADDriverRec *MDriver); // NEW - DESActivate the sound generating procedure (interruption)
//typedef OSErr (*MADPlayMusic_t)( MADDriverRec *MDriver); // NEW - Read and play current music in memory - Call MADStartInterruption BEFORE
//typedef OSErr (*MADStopMusic_t)( MADDriverRec *MDriver); // NEW - Stop reading current music in memory, Use MADCleanDriver to stop sounds
//typedef OSErr (*MADDisposeMusic_t)( MADMusic **, MADDriverRec *MDriver); // Dispose the current music, use it after RLoadMusic(), RLoadMusicRsrc(), RInstallMADF()
//typedef OSErr (*MADLoadMusicPtr_t)( MADMusic **music, Ptr myPtr); // MAD ONLY - Load a MAD Ptr into memory, you can DisposPtr your Ptr after this call
//typedef OSErr (*MADAttachDriverToMusic_t)( MADDriverRec *driver, MADMusic *music, unsigned char*);
//typedef OSErr (*MADDisposeLibrary_t)( MADLibrary *MADLib); // Close Library, close music, close driver, free all memory
//
//#define MADInitLibrary(xx,yy,zz)  (*(MADInitLibrary_t)  MADInitLibrary_addr)  (xx,yy,zz)
//#define MADGetBestDriver(xx)      (*(MADGetBestDriver_t)MADGetBestDriver_addr)(xx)
//#define MADCreateDriver(xx,yy,zz) (*(MADCreateDriver_t) MADCreateDriver_addr) (xx,yy,zz)
//#define MADDisposeDriver(xx)      (*(MADDisposeDriver_t)MADDisposeDriver_addr)(xx)
//#define MADStartDriver(xx)        (*(MADStartDriver_t)  MADStartDriver_addr)  (xx)
//#define MADStopDriver(xx)         (*(MADStopDriver_t)   MADStopDriver_addr)   (xx)
//#define MADPlayMusic(xx)          (*(MADPlayMusic_t)    MADPlayMusic_addr)    (xx)
//#define MADStopMusic(xx)          (*(MADStopMusic_t)    MADStopMusic_addr)    (xx)
//#define MADDisposeMusic(xx,yy)    (*(MADDisposeMusic_t) MADDisposeMusic_addr) (xx,yy)
//#define MADLoadMusicPtr(xx,yy)    (*(MADLoadMusicPtr_t) MADLoadMusicPtr_addr) (xx,yy)
//#define MADAttachDriverToMusic(xx,yy,zz) (*(MADAttachDriverToMusic_t) MADAttachDriverToMusic_addr) (xx,yy,zz)
//#define MADDisposeLibrary(xx)     (*(MADDisposeLibrary_t) MADDisposeLibrary_addr) (xx)
//

/*****************************/
/****** MAIN FUNCTION ********/
/*****************************/

int mod_player( int command, Ptr music)
{
    OSErr		err;

    static int          driver_running=0;
    
    int return_val=0;
    
    if (command==0) //init
    {
        //Do init
        //...
        driver_running=0;
    }
    else
    if (command==1) //play music
    {
       if(driver_running)
       {
              //Stop driver
              //...
       }
       else
       {
           //Load and play
           //...
           driver_running=1;
       }
    }
    else if (command==2)	//clean up music
    {
       if(driver_running)
          {
              //Stop driver
              //...
          }

       driver_running=0;
    }
    else if (command==3) //status
    {
           //If running, return 0 else return 1
    }
    else if (command==4) //stop music
    {
       if(driver_running)
          {
              //Stop driver
              //...
          }
       driver_running=0;
       return_val=0;
    }
    else if (command>10 )	     //volume (0-64)
    {
           //Set volume to command-10
    }

return return_val;
}

