/*

Sound.c calls Eclipse.c
Owner: SB
Started: 29/10/98
©Lightsoft 98
$Log: sound.c,v $
Revision 1.2  2003/09/24 21:39:38  robp
More work on Portable file-system. No work should effect current operation on Zex. When it is up and running as the standard system, we can remove the #if conditional compilation. Currently the sound needs work, plus one ZGetResource before we can debug. Then rest of the file-system can be completed.

Revision 1.1.1.1  2003/09/05 22:37:04  stu_c
First Imported.

Revision 1.23  2002/09/08 02:00:14  stu_c
Precompiled header adjustments

Revision 1.22  2002/07/28 17:17:56  stu_c
Exhaust particles and further work

Revision 1.21  2002/03/19 23:19:55  stu
Now two engine sounds - one for Zex and one for everything else with an engine

Revision 1.20  2002/02/23 18:56:16  stu
Removed some unused locals

Revision 1.19  2002/02/17 06:18:32  stu
Added radar targetting

Revision 1.18  2002/01/27 18:14:03  stu
Music further development

Revision 1.17  2002/01/05 00:12:07  rob
music driver conversion

Revision 1.16  2001/12/29 01:10:07  stu
Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.

Revision 1.15  2001/12/14 19:43:19  stu
Docking

Revision 1.14  2001/11/25 00:11:20  stu
Fixes to get classic carbon build up

Revision 1.13  2001/11/24 19:28:30  stu
Eclipse 2 further work

Revision 1.12  2001/11/12 21:49:34  stu
Patched through to Eclipse

Revision 1.11  2001/11/10 13:24:27  stu
Continuous sounds now run.

Revision 1.10  2001/11/09 18:28:54  rob
Various debug changes.

Revision 1.9  2001/11/07 20:49:23  stu
More Carbon work. SOund now plays OK

Revision 1.8  2001/11/05 20:03:34  stu
More OSX work

Revision 1.7  2001/11/02 23:18:57  stu
Further OS X work. Enabled syscon logging.

Revision 1.6  2001/11/02 19:30:53  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.5  2001/10/22 21:28:32  rob
Carbon warning changes

Revision 1.4  2001/10/21 18:12:46  stu
Carbon work.

Revision 1.3  2001/10/21 12:36:51  rob
Sound stupid error

Revision 1.2  2001/10/21 00:59:33  stu
Initial port work

 */

#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include	"LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <types.h>
    #include <sound.h>
    #include <CodeFragments.h>
    #include <files.h>
  #endif
  #include	"error_handler.h"
  #include "zex_misc.h"
  #include "3d_structures_def.h"
  #include "zex_misc.h"
  #include "processor.h"
#endif //pch


#include	"Zsound.h"
#include "3d_top.h"
#include "objects_controls.h"
#include "angular_stuff.h"
#include "file_io.h"
#include "physics.h"
#include "engines.h"
#include "camera_control.h"

#include "Eclipse2.h"


//sound.int
//fixed data - basically sound channel codes for various ops


#if ZEX_CORE_CARBON==0
static struct SndCommand amp_command = { ampCmd, 0, 0 }; 
#endif


//status buffer
struct SCStatus chan_status;

//extern SndChannelPtr sound_chan [12];
//extern	SndListHandle the_sounds[max_sounds];	//array of handles for sounds

#define Async	-1


#define NO_EXTERNAL_PLAYER 0

#if NO_EXTERNAL_PLAYER

// if is a temporary declaration - since we do not 
// have the PlayerPro 5.9 Mod player dev kit yet...
int mod_player( int command, Ptr music)
{
music=0;
command=0;

return 0;	// music never busy (all ever used for)
}

#endif

//SndChannelPtr sound_chan [17];	//17 sound channels
#if PORTABLE_FILESYSTEM
// perhaps this should be the type - but what happens about the mac sound calls?
//
#error "No ZexSoundPtr defined yet rob!!"
// typedef void *ZexSoundPtr;
//
ZexSoundPtr the_sounds[max_sounds];	//array of handles for sounds
#else
SndListHandle the_sounds[max_sounds];	//array of handles for sounds
#endif
Boolean sound_on;
int music_volume=63;	//0-63 permissable
int sound_volume=63;		//0-63 permissable
int  gMusicPlaying=0;
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects
//divisor list for master volume div into volume


extern int showing_stats;


/*----------------------------------------------------------------------*/
int init_zsound()
{
int	temp=0;
int	sound_counter=0;
#if PORTABLE_FILESYSTEM
void *inter_sound_load=(void *)1;
#else
Handle inter_sound_load=(Handle)1;
#endif

//now get our sound resources into sound_array
//we start at resource ID 128 and carry on until either we get an error or reach max_sounds
while (inter_sound_load!=0 && sound_counter<80)
      { //try for the old format mac snd resource
		if ((sound_counter+128)==169)
		{
        int a;
		
		 a=0;
		
		}
        #if PORTABLE_FILESYSTEM
	    inter_sound_load=ZGetResource('snd ',sound_counter+128, NULL);	//returns zero when no resource 
            if (inter_sound_load) 
		{
		  the_sounds[sound_counter]=(ZexSoundPtr)inter_sound_load; //store handle in array
		}
            sound_counter++;	//next resource
        #else
	    inter_sound_load=ZGetResource('snd ',sound_counter+128);	//returns zero when no resource 
      	if (inter_sound_load) 
		{
		 //is it a sound or a movie?
		 //GetSoundHeaderOffset((SndListHandle)inter_sound_load,&header_offset);
         //if (sound_counter+128 < 169)	//is it a flat snd resource?
		 //{
		  the_sounds[sound_counter]=(SndListHandle)inter_sound_load; //store handle in array
		 //}
/*
		 else //not really a snd resource, try for a movie
		 {		
		  Movie* the_movie;
		  Boolean* datachanged;
          OSErr err;
		  
		  err=NewMovieFromHandle(the_movie, inter_sound_load, newMovieDontResolveDataRefs, datachanged);
		  if (err)
		  {
		   report_error("init_zsound: Bad sound file. Conversion Err. Sound ID:","\p",sound_counter+128); 
		  }
		  
          my_handle=ExtractSoundTrackIntoHandle((Movie)the_movie);	 
		  if (my_handle==0)
		  {
		   report_error("init_zsound: Bad sound file. ID:","\p",sound_counter+128); 

		  }
		  else
		  the_sounds[sound_counter]=(SndListHandle)my_handle; //store handle in array
		  
		 }
*/
		 
		}
		

        HLock((Handle) the_sounds[sound_counter]);
        sound_counter++;	//next resource
        
        #endif
      }

temp|=eclipse_init();

sound_on=1;
show_text_in_loading_screen("F4: Initialising music system\n");	

load_mod_player();



if (temp!=0) report_error("Sound Init failed.","\p",temp);

return temp;
}





void run_engine_sound(int the_object)
{
float max_volume;
float total_fuel_consumption;
int volume;
eclipse_sound_ref sound_ref;
double distance_to_camera;



//first calc distance to camera object
distance_to_camera=get_distance_to_from_in_m(get_main_camera_object(), the_object);

//Now if < engine_min_listen_distance do we have the sound playing?
if (distance_to_camera<MIN_LISTEN_DISTANCE_IN_M)
{
//Do we have a chan?
  sound_ref=(*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.eclipse_engine_sound_ref;

  if(sound_ref==-1)	//there is no engine sound playing for this object
  {
     if((*ocb_ptr).object_list[the_object].Oid !='ZEXS')
	 {
	  sound_ref=eclipse_play_looped_sound(the_sounds[LOW_ENGINE_LOOP16],the_object);
	 }
	 else
	 {
	  sound_ref=eclipse_play_looped_sound(the_sounds[engine_loop],the_object);
      //sound_ref=-1;
	 }
	 
     if (sound_ref < ECLIPSE_NO_ERR) //All eclipse errors are -ve
	 {
	   goto skip_engine_sound_this_frame;	//we couldn't get a sound channel this frame, try next frame...
     }

	 (*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.eclipse_engine_sound_ref=sound_ref; //store it away
  }
  //calc volume and set it. Eclipse will take care of everything else.
  max_volume=(*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.main_thrust_consumption_per_second_in_KGS;
  total_fuel_consumption=(*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.current_total_fuel_consumption;
  if(total_fuel_consumption>0)
  {
   //we want a percentage
   volume=STATIC_CAST_TO_INT((total_fuel_consumption/max_volume)*100);
   if (volume<0) volume=0; //prevent those nasty 'Oops' situations...
  }
  else
  { volume=0; } //and again...no oops please.


  
  eclipse_set_looped_chan_volume(sound_ref, volume);

skip_engine_sound_this_frame: return;
}
else //object is too far away to be heard
{
 //clear out the sound if needs be
  sound_ref=(*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.eclipse_engine_sound_ref;

  if(sound_ref!=-1)	//there _is_ an engine sound playing for this object
  { 
    //clear out the sound channel
	eclipse_kill_looped_sound(sound_ref);
    (*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.eclipse_engine_sound_ref=-1;
  }
  
}

}

//play a sound sound_number given priority priority on channel channel with volume volume
//returns nothing
void play_zsound(int sound_number, int priority, int channel, int volume)
{
eclipse_play_one_shot_sound(the_sounds[sound_number]);

return;

}


//play a sound sound_number given priority priority on channel channel with volume volume
//returns nothing. Always plays the sound
void play_zsound_always(int sound_number, int priority, int channel, int volume)
{
 eclipse_play_one_shot_sound(the_sounds[sound_number]);
 return;
}

//new 2d sound driver 130799 SB - 6 channels
void play_zsound_relative_to_zex(int the_object, int sound_number)
{
}

//simple play sound takes ares id and starts that sound playing.
//simple stop sound stops the sound and frees up the memory

#if PORTABLE_FILESYSTEM
ZexSoundPtr the_sound;
#else
SndListHandle the_sound;
#endif
SndChannelPtr simple_sound_chan;

void simple_play_sound(int id)
{
SndNewChannel (&simple_sound_chan,sampledSynth,initStereo,nil);
#if PORTABLE_FILESYSTEM
the_sound=(ZexSoundPtr) ZGetResource('snd ',id, NULL);
#else
the_sound=(SndListHandle) ZGetResource('snd ',id);
//DetachResource((Handle)the_sound);
HLock((Handle)the_sound);
#endif
SndPlay (simple_sound_chan,the_sound,Async);

}

void simple_stop_sound()
{
 SndDisposeChannel(simple_sound_chan,1);
 #if PORTABLE_FILESYSTEM
 ZReleaseResource(the_sound);
 #else
 DisposeHandle((Handle)the_sound);
 #endif
}




int sound_last_run=0;
int rd_timer=0;

void run_periodic_sound_tasks()
{


//ongoing looped sound?
//transmitting request dock signal?

/*
 if ((*ocb_ptr).object_list[0].Dyn_OCB_control_data.request_dock==1 && tractor==0)
      {
      if (ZexTickCount()>rd_timer)
         {
         rd_timer=ZexTickCount()+60;
          play_zsound(bleeper, sound_high_pri, c1_chan1, sound_vol_7);

         }
      }

*/

eclipse_update_sounds();

}




Handle get_sound_handle(int the_sound)
{
	return (Handle) the_sounds[the_sound];
}


//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//load and init the mod player


#if !CARBONIZE_MUSIC
#error Music MUST be Carbonized now....
#endif


//NOTE: Using 3.2 headers knewcopy et. al are obsolete
//This function uses the new names 
void load_mod_player()
{
//int hsize;
//OSErr my_err;
//CFragConnectionID mod_conn_id;
//Str255 ErrName;
//Str63 fragname = "\pMODPlayer";

//Handle tune_handle;
//Ptr tune;
//int test;

// commented out when converted to internal mod_player controller
//mod_handle=ZGetResource('pMoD',128);
//if (mod_handle==0) report_error("Corrupt ZEX. Couldn't load pMoD code resource.","\p",4);
//MoveHHi(mod_handle);
//HLock(mod_handle);
//hsize=GetHandleSize(mod_handle);
//my_err=GetMemFragment(*mod_handle, hsize, fragname, kPrivateCFragCopy, &mod_conn_id, 
//               (Ptr *) &mod_main_addr,ErrName);
//
//if (my_err!=0) report_error("Couldn't link pMoD code resource. Try giving ZEX more memory.","\p",my_err);
//
//#if !CARBONIZE_MUSIC
//  mod_ptr=NewRoutineDescriptor (mod_main_addr,modProcInfo, GetCurrentISA());
//#endif

//  CallUniversalProc(mod_ptr,modProcInfo,0,0);	//init

  mod_player(0,0); // init


}

//takes valume 0-63
void music_set_vol(int volume)
{
 mod_player(volume+10,0);
//CallUniversalProc(mod_ptr,modProcInfo,volume+10,0);
}


int gFadeMusic=0;
int firstcall=1;	//set to zero on subsequent calls
int actual_volume;
int gNewTune=0;	//at end of fade, if set music in new_music_ptr is played
#if PORTABLE_FILESYSTEM
void *new_music_pointer;
#else
Handle new_music_handle;
#endif


void fade_out_music()
{
//extern int music_volume;
gFadeMusic=1;
gNewTune=0;
  #if  ZEX_PROJECT_BUILDER&&MUSIC_LOG==1
  fprintf(stderr, "Music: fade_out_music\n");
  #endif

}

extern short zex_res_file;
//fades the music and then plays the new tune held as a Zex resource
void fade_and_play_zex_resource (unsigned long res_id)
{
#if PORTABLE_FILESYSTEM
void *music_ptr;
#else
Handle music_h;
#endif
 if (music_volume<7)
 {
        stop_music_now();
        return;
 }


  #if  ZEX_PROJECT_BUILDER&&MUSIC_LOG==1
  fprintf (stderr, "Music: fade_and_play %d \n",(int) res_id);
  #endif

    #if PORTABLE_FILESYSTEM
        music_ptr = ZGetResource('MADH',res_id, NULL);  //Get the Handle to the Resource
        fade_and_play(music_ptr);
    #else
//        UseResFile(zex_res_file);
        music_h = (Handle) ZGetResource('MADH',res_id);  //Get the Handle to the Resource
        HLock(music_h);
//        DetachResource(music_h);
        fade_and_play(music_h);
    #endif
}


#if PORTABLE_FILESYSTEM
void fade_and_play(void *the_tune)
#else
void fade_and_play(Handle the_tune)
#endif
{
gFadeMusic=1;
 if (music_volume>6)
 {
  gNewTune=1;
  #if PORTABLE_FILESYSTEM
  new_music_pointer=the_tune;
  #else
  new_music_handle=the_tune;
  #endif
 }
 else
 {
        stop_music_now();
 }
}

static int do_fade_schedule=0;

//called from our vbl proc - fades the music volume to zero and then stops the player
//or plays a new tune if gNewTune is set
void do_fade_out_music()
{
extern int music_volume;

do_fade_schedule--;	//slow down the fade for todays much faster machines!

if(do_fade_schedule<0)
{
 do_fade_schedule=2;	//every 2 frames

 if (firstcall==1) 
 {
 actual_volume=music_volume;	//copy current volume
 firstcall=0;
 }
 
 if (actual_volume>0)
 {
  mod_player(actual_volume+10,0);
  // CallUniversalProc(mod_ptr,modProcInfo,actual_volume+10,0);
  actual_volume--;
 }

 if (actual_volume==0) 
 {
  gFadeMusic=0;	//stop VBL routine calling us
  firstcall=1;
 
  if (gNewTune==1) 
  {
  gNewTune=0;
  #if PORTABLE_FILESYSTEM
  mod_player(1,new_music_pointer); // play
  ZReleaseResource(new_music_pointer);
  #else
  mod_player(1,*new_music_handle); // play
  //CallUniversalProc(mod_ptr,modProcInfo,1,*new_music_handle);	//play
  DisposeHandle(new_music_handle);
  #endif
   gMusicPlaying=1;
  music_set_vol(music_volume);
  }
  else 
  {
  #if  ZEX_PROJECT_BUILDER&&MUSIC_LOG==1
  fprintf(stderr, "Music: Fade out has stopped music\n");
  #endif
  stop_music_now();
  }
 }
}	//end of do_fade_schedule
}


void play_zex_resource_now (unsigned long res_id)
{
#if PORTABLE_FILESYSTEM
void *music_ptr;
#else
Handle music_h;
#endif

 if (music_volume<7)
 {
        stop_music_now();
        return;
 }

  #if  ZEX_PROJECT_BUILDER&&MUSIC_LOG==1
  fprintf(stderr,"Music: play_zex_resource_now %i\n",(int)res_id);
  #endif

  #if PORTABLE_FILESYSTEM
  music_ptr = ZGetResource('MADH',res_id, NULL);  //Get the Handle to the Resource
  mod_player(1,music_ptr); // play
  ZReleaseResource(music_ptr);
  #else
	music_h = (Handle) ZGetResource('MADH',res_id);  //Get the Handle to the Resource
	HLock(music_h);
	mod_player(1,*music_h); // play
    DisposeHandle(music_h);
  #endif
   gMusicPlaying=1;
  music_set_vol(music_volume);
}




//returns 1=yes, 0=no - doesn't work...?
int is_music_busy(void)
{
int busy;
busy=mod_player(3,0);
// busy = CallUniversalProc(mod_ptr,modProcInfo,3,0);
return busy;

}




FSSpec the_level_file;

//looks in the resource fork of the level for a madh128 - if found plays it, else plays 130 from Zex
//void run_level_music(FSSpec * the_file)
//{
//the_level_file=(*the_file);	  //make a note of the fsspec
//play_level_music();
//
//}




//tyupically called from the level controller
void run_level_aux_music(int index)
{
//the_level_file is already set up from the initial level loader
play_level_aux_music(index);
}


//run_level_music must be called before this rouitne, which is typically called when undocking
void play_level_aux_music(int index)
{
//extern short zex_res_file;
//short frefnum;
//int have_level_music=0;
//Handle music_h;
//Ptr music;

MyDebugStr(__LINE__,"sound.c","play_level_aux_music");

//if (music_volume<7)
//{
// stop_music_now();
// return;	  //if volume not zero then play
//}
////open the res file
//frefnum=FSpOpenResFile(&the_level_file,fsRdPerm);
//
//     UseResFile(frefnum);
//     
//      music_h = (Handle) Get1Resource('MADH',128+index);  //Get the Handle to the Resource 
//       if (music_h==0 || ResError()!=0)
//        {
//        UseResFile(zex_res_file);
//        music_h = (Handle) Get1Resource('MADH',130);  //Get the Handle to the Resource
//         HLock(music_h);
////         music=*music_h;
//         DetachResource(music_h);
//         fade_and_play(music_h);
//         
////        play_tune(130);	//default
//        }
//       else 
//        {
//         have_level_music=1;
//         HLock(music_h);
////         music=*music_h;
//         DetachResource(music_h);
//         fade_and_play(music_h);
// //        CallUniversalProc(mod_ptr,modProcInfo,1,music);	    //play
////         ReleaseResource(music_h);
//        }
// CloseResFile(frefnum);
//
// UseResFile(zex_res_file);
// gMusicPlaying=1;
}





//run_level_music must be called before this rouitne, which is typically called when undocking
void play_level_music()
{
//extern short zex_res_file;
//extern int  training_mission;
//short frefnum;
//int have_level_music=0;
//Handle music_h;
//Ptr music;
//show_text_in_loading_screen("\pZex: play_level_music");	
//
//if (music_volume<7)
//{
// stop_music_now();
// return;	  //if volume not zero then play
//}
////open the res file
//frefnum=FSpOpenResFile(&the_level_file,fsRdPerm);
//if (frefnum!=-1 && ResError()==0)
//{
//     UseResFile(frefnum);
//if (training_mission==1)    
//      music_h = (Handle) Get1Resource('MADH',128);  //Get the Handle to the Resource 
//else
//{
////real game
//      music_h = (Handle) Get1Resource('MADH',128+current_quad);  //Get the Handle to the Resource 
//     
//}       
//       if (music_h==0 || ResError()!=0)
//        {
//          UseResFile(zex_res_file);
//          music_h = (Handle) Get1Resource('MADH',130);  //Get the Handle to the Resource
//          if (music_h!=0 || ResError()==0)
//           {
////         music=*music_h;
//              DetachResource(music_h);
//              HLock(music_h);
//              fade_and_play(music_h);
//            }
////        play_tune(130);	//default
//        }
//       else 
//        {
//           have_level_music=1;
////         music=*music_h;
//           DetachResource(music_h);
//           HLock(music_h);
//           fade_and_play(music_h);
// //        CallUniversalProc(mod_ptr,modProcInfo,1,music);	    //play
////         ReleaseResource(music_h);
//        }
// CloseResFile(frefnum);
//
// UseResFile(zex_res_file);
// gMusicPlaying=1;
//}	//end of frefnum==-1

}





//stop all music immediately, reset fading control
void stop_music_now()
{
 mod_player(4,0); // stop music
 gMusicPlaying=0;
 actual_volume=0;
 gFadeMusic=0;
 firstcall=1;
}

int do_we_think_music_is_playing(void)
{
 return gMusicPlaying;
}

