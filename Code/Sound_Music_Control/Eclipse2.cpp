/*
 *  Eclipse2.cpp
 *  zex22
 *
 *  Created by Stu on Sat Nov 10 2001.
 *  Copyright (c) 2001 Lightsoft (Games). All rights reserved.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include	"LS_Standard.h"

  #include "3d_structures_def.h"
  #include "processor.h"
  #include	"zex_misc.h"
#endif //pch

#include "physics.h"
#include "camera_control.h"

#include "Eclipse2.h"
/*
Eclipse 2 is a general purpose 3D sound system for the Mac.

It provides 6 one shot sound channels and 6 continuous sound channels.
Ruutines are provided for modifying the continuous sound channels.

In Zex it sits between Zex's sound.c and the OS


Requires:
    An external timer. The resolution of the timer should be better than 1/10th second.
 
It works thus:
    All sounds are played with bufferCmd with a callback in the channel. When the sound stops
	playing the callback examines param2 of the callback params and if NO_LOOP_SOUND cleans up the channel.
	If LOOP_SOUND, the sound is rebuffered and hence looped.
	
	The callback is executed when the hardware DMA buffer is filled with the last set of samples (in theory).
    
*/


// ----------------------------------------------------------------------
// Eclipse constants

#define NUMBER_OF_ONE_SHOT_CHANS 6
#define NUMBER_OF_LOOPED_CHANS 6
#define TOTAL_NUMBER_OF_CHANS NUMBER_OF_ONE_SHOT_CHANS+NUMBER_OF_LOOPED_CHANS

#define LOOPED_CHANS_START NUMBER_OF_ONE_SHOT_CHANS  /* in the chan list, looped chans start here */
#define LOOPED_CHANS_END LOOPED_CHANS_START+NUMBER_OF_LOOPED_CHANS  /* in the chan list, looped chans start here */


// ----------------------------------------------------------------------
// Eclipse internals

static pascal void    CarbonSndPlayDoubleBufferCallBackProc1 
               (SndChannelPtr theChannel, SndCommand * theCallBackCmd);

SoundHeaderPtr GetSoundHeader (Handle theSndHandle);

module_private double eclipse_sound_timer;

// ----------------------------------------------------------------------
// Mac specific datums

static struct SndCommand quiet_now = { quietCmd, 0, 0 };
static struct SndCommand flush_now = { flushCmd, 0, 0 };

static struct SndCommand volume_command = {volumeCmd, 0, 0 }; 

static SndChannelPtr sound_chan [TOTAL_NUMBER_OF_CHANS];	//array of sound channels
static int looped_sound_objects[TOTAL_NUMBER_OF_CHANS];
static int looped_sound_volumes_percent[TOTAL_NUMBER_OF_CHANS];     //wanted volume as a percentage


SndCallBackUPP gCarbonSndPlayDoubleBufferCallBackUPP;

#if INTERFACE_LIB_BUILD==1
  #define NewRoutineDescriptor(theProc, theProcInfo, theISA) ((UniversalProcPtr)theProc)
  enum { uppSndCallBackProcInfo = 0x000003C0 };  /* pascal no_return_value Func(4_bytes, 4_bytes) */
  #define NewSndCallBackUPP(userRoutine) (SndCallBackUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppSndCallBackProcInfo, GetCurrentArchitecture())
#endif



// --------------------------------------------------------
// Init Eclipse
// --------------------------------------------------------

zex_public eclipse_err eclipse_init(void)
{
  int i;
  int err=0;
  
  gCarbonSndPlayDoubleBufferCallBackUPP = NewSndCallBackUPP (CarbonSndPlayDoubleBufferCallBackProc1);

  eclipse_sound_timer=0;
  eclipse_kill_all_looped_sounds();
  
  for (i=0;i<TOTAL_NUMBER_OF_CHANS;i++)
  {
    err=err | SndNewChannel (&sound_chan[i],sampledSynth,initStereo,nil);
  }
  
  
  if(err)
     show_text_in_loading_screen("Eclipse2: eclipse_init ***FAILED*** \n");
  else
  {
    #if ECLIPSE_DEBUG_MESSAGES==1

     show_text_in_loading_screen("Eclipse2: Initted OK \n");
    #endif
  }		  	  
  return err;
}




// --------------------------------------------------------
// Insert a new one-shot sound
// Returns 0 if sound could be inserted else ECLIPSE_NO_SLOT_ERR
// --------------------------------------------------------
zex_public eclipse_err eclipse_play_one_shot_sound(SndListHandle the_sound)
{
int chan_index,err;
SndChannelPtr the_chan;
struct SCStatus chan_status;
long header_offset;
SndCommand mySndCmd;
SndListResource * Sndptr;
float float_vol=511;
short int_vol;
int volume;
float master_vol_adjust, fvolume;
extern int sound_volume;


//find a free channel
 chan_index=0;
 for(EVER)
 {
  the_chan=sound_chan[chan_index];
  SndChannelStatus(the_chan, sizeof chan_status, &chan_status);	//get status
  if (chan_status.scChannelBusy==FALSE) break; //found a free chan
  chan_index++; 
  if (chan_index==NUMBER_OF_ONE_SHOT_CHANS) 
  {
  		 #if ZEX_PROJECT_BUILDER==1  && ECLIPSE_DEBUG_MESSAGES //can only printf to OSX syscon
		fprintf (stderr, "Eclipse: eclipse_play_one_shot_sound failed to find a free chan\n"); 
        #endif
   return ECLIPSE_NO_SLOT_ERR;
  }
 }

 //got chan, so lets get it going with a bufferCmd

 SndDoImmediate(the_chan, &quiet_now);
 SndDoImmediate(the_chan, &flush_now);

//insert the sound as a voice into the channel
  GetSoundHeaderOffset(the_sound,&header_offset);
  if (header_offset==0) return ECLIPSE_BAD_SOUND_FORMAT_ERR;
  Sndptr=*the_sound;
  


//#if ZEX_CORE_CARBON==1
  the_chan->callBack = gCarbonSndPlayDoubleBufferCallBackUPP;

// Issue bufferCmd to play the sound, using SndDoImmediate.
  mySndCmd.cmd = bufferCmd;
  mySndCmd.param1 = 0;
  mySndCmd.param2 = (unsigned long) Sndptr+header_offset;

  err = SndDoImmediate(the_chan, &mySndCmd);
  if (err != 0)
   return ECLIPSE_BUFFCMD_ERR;

// Issue callBackCmd using SndDoCommand so that we get called back 
// when the buffer is done playing.
  mySndCmd.cmd = callBackCmd;
  mySndCmd.param1 = NO_LOOP_SOUND;
  mySndCmd.param2 = (unsigned long) Sndptr+header_offset; //param 2 is pointer to sound to play (not used in no_loop mode)	

  err=SndDoCommand(the_chan, &mySndCmd, 1);
  if (err != 0)
    return ECLIPSE_CALLBACK_STORE_ERR;
//#else

//#endif

  //adjust volume by master game volume
   master_vol_adjust=(float) sound_volume*(100.0/64.0); //sound_volume ranges 0-63, make in range 0-99
   fvolume=(float)(master_vol_adjust/100.0);
   float_vol*=(float)fvolume;
   int_vol=(short)float_vol;
   volume=int_vol<<16;
   volume+=int_vol;   
   volume_command.param2=volume;
   SndDoImmediate(the_chan, &volume_command);



 return ECLIPSE_NO_ERR;
 
}



// --------------------------------------------------------
// Insert a new looped sound, returns a +ve ref the caller can use when
// modifying the sound. E.G. Engines otherwise returns an Eclipse error 
// (-ve)
// --------------------------------------------------------


zex_public eclipse_sound_ref  eclipse_play_looped_sound(SndListHandle the_sound, int the_object)
{
eclipse_sound_ref sound_ref;
int chan_index,err;
SndChannelPtr the_chan;
struct SCStatus chan_status;
long header_offset;
SndCommand mySndCmd;
SndListResource * Sndptr;
SoundHeaderPtr		mySndHeader;

//find a free channel
 chan_index=LOOPED_CHANS_START;
 for(EVER)
 {
  the_chan=sound_chan[chan_index];
  SndChannelStatus(the_chan, sizeof chan_status, &chan_status);	//get status
  if (chan_status.scChannelBusy==FALSE) break; //found a free chan
  chan_index++; 
  if (chan_index==LOOPED_CHANS_END) 
   {
   
    #if ZEX_PROJECT_BUILDER==1 && PB_PRINT_TO_LOG_ENABLED //can only printf to OSX syscon
    #if ECLIPSE_STATUS_TO_LOG==1
     fprintf (stderr, "Eclipse: eclipse_play_looped_sound failed to find a free chan\n"); 
    #endif
    #endif
    return ECLIPSE_NO_SLOT_ERR;
   }
 }

 sound_ref=chan_index;	//what we return to caller
 #if ZEX_PROJECT_BUILDER==1 && PB_PRINT_TO_LOG_ENABLED //can only printf to OSX syscon
 #if ECLIPSE_STATUS_TO_LOG==1
 #if ECLIPSE_DEBUG_MESSAGES == 1
  fprintf (stderr, "Eclipse: Assigned channel %i for object %i \n",sound_ref,the_object); 
 #endif
 #endif
 #endif

 looped_sound_objects[chan_index]=the_object;	// store object slot so eclipse_update_sounds can find the objects 
												// associated with each sound
  
 //got chan, so lets get it going with a bufferCmd

 SndDoImmediate(the_chan, &quiet_now);
 SndDoImmediate(the_chan, &flush_now);

//insert the sound as a voice into the channel
  mySndHeader=GetSoundHeader((Handle) the_sound);
  


  GetSoundHeaderOffset(the_sound,&header_offset);
  if (header_offset==0)
  {
#if ZEX_PROJECT_BUILDER==1 && PB_PRINT_TO_LOG_ENABLED //can only printf to OSX syscon
#if ECLIPSE_DEBUG_MESSAGES == 1
  fprintf (stderr, "Eclipse: BAD_SOUND_FORMAT_ERR for object %i \n",the_object); 
#endif
#endif
	looped_sound_objects[chan_index]=-1;	// store object slot so eclipse_update_sounds can find the objects 
    return ECLIPSE_BAD_SOUND_FORMAT_ERR;
  }
  Sndptr=*the_sound;
  


  volume_command.param2=0;  //Start the sound with no volume!
  SndDoImmediate(the_chan, &volume_command);


//#if ZEX_CORE_CARBON==1
  the_chan->callBack = gCarbonSndPlayDoubleBufferCallBackUPP;


// Issue bufferCmd to play the sound, using SndDoImmediate.
  mySndCmd.cmd = bufferCmd;
  mySndCmd.param1 = 0;
  mySndCmd.param2 = (unsigned long) Sndptr+header_offset;

  err = SndDoImmediate(the_chan, &mySndCmd);
  if (err != 0)
  {
   #if ZEX_PROJECT_BUILDER==1 && PB_PRINT_TO_LOG_ENABLED //can only printf to OSX syscon
   #if ECLIPSE_DEBUG_MESSAGES == 1
   fprintf (stderr, "Eclipse: BAD_BUFFCMD_ERR for object %i \n",the_object); 
   #endif 
   #endif
	looped_sound_objects[chan_index]=-1;	// store object slot so eclipse_update_sounds can find the objects 
   return ECLIPSE_BUFFCMD_ERR;
  }
// Issue callBackCmd using SndDoCommand so that we get called back 
// when the buffer is done playing.
  mySndCmd.cmd = callBackCmd;
  mySndCmd.param1 = LOOP_SOUND;
  mySndCmd.param2 = (unsigned long) Sndptr+header_offset; //param 2 is pointer to sound to play (not used in no_loop mode)	

  err=SndDoCommand(the_chan, &mySndCmd, 1);
  if (err != 0)
  {
   #if ZEX_PROJECT_BUILDER==1 && PB_PRINT_TO_LOG_ENABLED //can only printf to OSX syscon
   #if ECLIPSE_DEBUG_MESSAGES == 1
   fprintf (stderr, "Eclipse: CALLBACK_STORE_ERR for object %i \n",the_object); 
   #endif 
   #endif
	looped_sound_objects[chan_index]=-1;	// store object slot so eclipse_update_sounds can find the objects 
    return ECLIPSE_CALLBACK_STORE_ERR;
  }	
	
//#else

//#endif
  

  return sound_ref;
}



// ----------------------------------------------------------------------
// Looped sound modifier functions

zex_public eclipse_err eclipse_set_looped_chan_volume (eclipse_sound_ref the_ref, int volume_percent)
{


#if ECLIPSE_CHECK_PARAMS==1
 if((the_ref<LOOPED_CHANS_START) || (the_ref>=LOOPED_CHANS_END))
 return ECLIPSE_BAD_SOUND_REF;
#endif

looped_sound_volumes_percent[the_ref]=volume_percent;

/*

*/
 return ECLIPSE_NO_ERR;
 
}



// ----------------------------------------------------------------------
// Kill functions

zex_public void eclipse_kill_all_looped_sounds(void)
{
int n;
 for(n=LOOPED_CHANS_START; n<LOOPED_CHANS_END; n++)
 {
   eclipse_kill_looped_sound(n);
 }
}


//eclipse_kill_looped_sound, eclipse_kill_all_looped_sounds, eclipse_kill_one_shot_sounds, eclipse_kill_all_sounds.
zex_public eclipse_err eclipse_kill_looped_sound(eclipse_sound_ref the_ref)
{
struct SCStatus chan_status;
int object;
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects


#if ECLIPSE_CHECK_PARAMS==1
 if((the_ref<LOOPED_CHANS_START) || (the_ref>=LOOPED_CHANS_END))
 {
  return ECLIPSE_BAD_SOUND_REF;
 }
#endif

#if ZEX_PROJECT_BUILDER==1 && PB_PRINT_TO_LOG_ENABLED //can only printf to OSX syscon
#if ECLIPSE_STATUS_TO_LOG==1
#if ECLIPSE_DEBUG_MESSAGES == 1
  if (looped_sound_objects[the_ref]==-1)
  fprintf (stderr, "Eclipse: Channel %d released. Object is invalid (-1).\n",the_ref); 
  else
  fprintf (stderr, "Eclipse: Channel %d released. Object is: %d.\n",the_ref,looped_sound_objects[the_ref]); 
#endif
#endif
#endif
 //kill the sound ref in the object
 
 object=looped_sound_objects[the_ref];
 if (((*ocb_ptr).object_list[object].in_use==1) && (object!=-1))
 {
  (*ocb_ptr).object_list[object].Dyn_OCB_control_data.eclipse_engine_sound_ref=-1;
 }
 else
 {
 #if ZEX_PROJECT_BUILDER==1 && PB_PRINT_TO_LOG_ENABLED //can only printf to OSX syscon
 #if ECLIPSE_DEBUG_MESSAGES == 1
  if (object!=-1)
  {
   fprintf (stderr, "Eclipse: soundref %d references object %d which is dead!\n",the_ref,object); 
  }
 #endif
 #endif
 }
 looped_sound_objects[the_ref]=-1;	//clear out object reference
 looped_sound_volumes_percent[the_ref]=0; //clear volume
 
 //Although we could, we will not hush a channel that is not in use

 SndChannelStatus(sound_chan[the_ref], sizeof chan_status, &chan_status);	      //get status
 if (chan_status.scChannelBusy==FALSE)
 {
 #if ZEX_PROJECT_BUILDER==1 && PB_PRINT_TO_LOG_ENABLED //can only printf to OSX syscon
 #if ECLIPSE_STATUS_TO_LOG==1
 #if ECLIPSE_DEBUG_MESSAGES == 1
  fprintf (stderr, "Eclipse: Channel %d is not busy - returning ECLIPSE_CANT_KILL.\n",the_ref); 
 #endif
 #endif
 #endif
 
   return ECLIPSE_CANT_KILL; //Chan not in use, return err
 }
 
 //Clear out the chan
	SndDoImmediate(sound_chan[the_ref], &quiet_now); 
    SndDoImmediate(sound_chan[the_ref], &flush_now);

 return ECLIPSE_NO_ERR;
}



// ----------------------------------------------------------------------
// Eclipse Internals


// -------------------------------------------------
// Callback for bufferCmd sounds
// Please Compiler, optimise this to death. Ta.
module_private pascal void CarbonSndPlayDoubleBufferCallBackProc1 (SndChannelPtr theChannel, SndCommand * theCallBackCmd) 
{
SndCommand mySndCmd;


  if (theCallBackCmd->param1==LOOP_SOUND)
  { //restart the sound
    mySndCmd.cmd = bufferCmd;
    mySndCmd.param1 = 0;
    mySndCmd.param2 = (unsigned long) theCallBackCmd->param2;	//sound passed in param2
    SndDoImmediate(theChannel, &mySndCmd); //get it playing as soon as possible!
  
    mySndCmd.cmd = callBackCmd;
    mySndCmd.param1 =  LOOP_SOUND;
    mySndCmd.param2 = (unsigned long) theCallBackCmd->param2;
    SndDoCommand(theChannel, &mySndCmd, 1); //and set up another callback with noWait
  }
  
  else //not looped sound, so clear the chan out
  
  {
//    SndDoImmediate(theChannel, &quiet_now); 
//    SndDoImmediate(theChannel, &flush_now);
  }
  
}



// --------------------------------------------------------
// Autonomous sound update proc
// --------------------------------------------------------


//set position and amplitude for each continuous sound
zex_public void eclipse_update_sounds(void)
{
float float_vol;
short int_vol;
int volume;
float master_vol_adjust, fvolume;
extern int sound_volume;
int volume_percent;
int the_chan;


 eclipse_sound_timer+=real_frame_time_in_secs;
 if (eclipse_sound_timer<SOUND_UPDATE_TIME_IN_SECS) return;
 
 // ----------------- TIME TO RUN -------------------
 
 eclipse_sound_timer=0;  //Reset time to next run
 
 //for each object we have a channel for, update any continuous sound it is making
   for(the_chan=LOOPED_CHANS_START;the_chan<LOOPED_CHANS_END; the_chan++)
   {
    
    if(looped_sound_objects[the_chan]!=-1)
    {
	float distance_to_camera;
	
	 float_vol=511;
//adjust volume by master volume
     volume_percent=looped_sound_volumes_percent[the_chan];
     master_vol_adjust=(float) sound_volume*(100.0/64.0); //sound_volume ranges 0-63, make in range 0-99
     fvolume=(float)volume_percent*(master_vol_adjust/100.0);

//the_ref is actually the index of the channel in sound_chan array
     float_vol*=(float)fvolume/100.0;
//calculate attenuation for distance as a percentage
//first calc distance to camera object
     distance_to_camera=get_distance_to_from_in_m(get_main_camera_object(), looped_sound_objects[the_chan]);

//Now if < engine_min_listen_distance do we have the sound playing?
     if (distance_to_camera<MIN_LISTEN_DISTANCE_IN_M)
     {
  	 float percent_vol;
	   percent_vol=(float)MIN_LISTEN_DISTANCE_IN_M/100.0;
	   percent_vol=100.0-(distance_to_camera/percent_vol); //0-99
	   float_vol*=percent_vol/100.0; //attenuate for distance
     }
	 
	
     int_vol=(short)float_vol;
     volume=int_vol<<16;
     volume+=int_vol;   
     volume_command.param2=volume;
     SndDoImmediate(sound_chan[the_chan], &volume_command);
    }
   
   }
}


SoundHeaderPtr GetSoundHeader (Handle theSndHandle)
{
	SoundHeaderPtr		mySndHeader = NULL;
	long				myOffset = 0;
	OSErr				myErr = noErr;
	
	myErr = GetSoundHeaderOffset((SndListHandle)theSndHandle, &myOffset);
	if (myErr == noErr)
		mySndHeader = (SoundHeaderPtr)((long)*theSndHandle + myOffset);

	return(mySndHeader);
}


