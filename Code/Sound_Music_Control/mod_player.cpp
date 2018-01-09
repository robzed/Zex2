/********************************************************/
/*
mod_player.cpp
Zex Mod Player
SB: Dec 98
Update: 20 Apr 99 - Surround and deluxe for G3's/4's
RP: Hacked 15 Oct 01 to work with direct PP source interface.

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
    MADDriverSettings	init;
    OSErr		err;

    static MADDriverRec	*the_driver;
    static MADLibrary	*MADLib;
    static MADMusic     *MADMusic;
    static int          driver_running=0;
//    static int shlb_loaded=0;
    
    int return_val=0;
    
/*******************************************************************************************/
/****** MAD Library Initialisation : choose the best driver for the current hardware  ******/
/*******************************************************************************************/
if (command==0) //init
{
//	long gestaltAnswer;
	OSErr err;
	
//        if(!shlb_loaded)
//          {
//	    get_playerpro_shared_lib();
//	    shlb_loaded=1;
//	    }
//

/*	Since we are OGL only, why adjust for lower machines?


	Gestalt('cput',&gestaltAnswer);
*/	
	err = MADInitLibrary("", false, &MADLib);
	if( err) DebugStr("\pMADInitLibrary Err");
	
	MADGetBestDriver( &init);
	init.repeatMusic=1;

/*
	if (gestaltAnswer==101)	//if 601 then rate11025hz
	{
	init.outPutRate=rate11025hz;
    init.Interpolation=false;
    init.surround=false;
    }
	else
	if (gestaltAnswer<104)
    {
	init.outPutRate=rate22050hz;	//603's get 22Khz
    init.Interpolation=false;
    init.surround=false;
    }
	else	
	{
*/

	init.outPutRate=rate44khz;	//anything else gets 44Khz
//not in latest pp    init.Interpolation=true;
    init.surround=false;
    init.outPutMode=DeluxeStereoOutPut;
//    }
    
	err = MADCreateDriver( &init, MADLib, &the_driver);
	if( err) DebugStr("\pMADCreateDriver Err");
//	the_driver=MADGetMADDriverPtr();
	the_driver->VolGlobal=63;	//set max volume

	driver_running=0;
   }

else
/***************************************************/
/***   Open MADH Resource ID 3214 and play it !   **/
/***************************************************/
if (command==1) //play music
   {
   if(driver_running)
      {
      MADStopMusic(the_driver);			  // Stop the music
      MADStopDriver(the_driver);		// Stop the driver
      MADDisposeMusic(&MADMusic, the_driver);	 // Clear music
      }
      
   if( (err=MADLoadMusicPtr(&MADMusic, music)) != noErr) 
   {
   #if ZEX_PROJECT_BUILDER==1 //can only printf to OSX syscon
    fprintf (stderr, "Mod Player: ***Error*** loading music. MADLoadMusicPtr. Error = %i \n",err); 
   #endif
   

	//MyDebugStr(err,"Music Error","MADLoadMusicPtr Err"); // DebugStr("\pMADLoadMusicPtr Err");
   }
   else
   {
    MADAttachDriverToMusic( the_driver, MADMusic, 0L);
    if( MADStartDriver(the_driver) != noErr) DebugStr("\pMADStartDriver Err");
    MADPlayMusic(the_driver);
    driver_running=1;
   }
   }

else if (command==2)	//clean up music
  {
   if(driver_running)
      {
      MADStopMusic(the_driver);			  // Stop the music
      MADStopDriver(the_driver);		// Stop the driver
      MADDisposeMusic(&MADMusic, the_driver);	 // Clear music
      }

   MADDisposeDriver(the_driver);					// Dispose driver
   MADDisposeLibrary(MADLib);				// Close Music Library
   driver_running=0;
   }
else if (command==3) //status
   {
	//the_driver=MADGetMADDriverPtr();
	if (!the_driver->musicEnd) return_val=0;	//not end of music
	else return_val=1;
   }
else if (command==4) //stop music
   {
   if(driver_running)
      {
      MADStopMusic(the_driver);			  // Stop the music
      MADStopDriver(the_driver);		// Stop the driver
      MADDisposeMusic(&MADMusic, the_driver);	 // Clear music
      }
   driver_running=0;
   return_val=0;
   }

else if (command>10 )	     //volume (0-64)
   {
   the_driver->VolGlobal=command-10;
   }

return return_val;
}

