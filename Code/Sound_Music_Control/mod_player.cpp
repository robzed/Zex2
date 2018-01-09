/********************************************************/
/*
mod_player.cpp
Zex Mod Player
SB: Dec 98
Update: 20 Apr 99 - Surround and deluxe for G3's/4's
RP: Hacked 15 Oct 01 to work with direct PP source interface.
SB: 2018-01-09 - todo: update this to SDLMixer
*/
/********************************************************/
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
#endif


#include "Zsound.h"


/*****************************/
/****** MAIN FUNCTION ********/
/*****************************/

int mod_player( int command, Ptr music)
{
OSErr		err;
static int          driver_running=0;
//    static int shlb_loaded=0;
int return_val=0;
    
/*******************************************************************************************/
/****** Library Initialisation : choose the best driver for the current hardware  ******/
/*******************************************************************************************/
if (command==0) //init
{
    //Init music player
}

else
if (command==1) //play music
   {
   if(driver_running)
      {
          //Stop playing
      }
    //Load and play music
   }
else if (command==2)	//clean up music
  {
   if(driver_running)
      {
          //Stop the music
      }
      //Clean uo
   driver_running=0;
   }
else if (command==3) //status
   {
       //return_val=0 if not end of music, else 1
   }
else if (command==4) //stop music
   {
   if(driver_running)
      {
          //Stop the music
      }
   driver_running=0;
   return_val=0;
   }

else if (command>10 )	     //volume (0-64)
   {
       //Set volume (command-10)
   }

return return_val;
}

