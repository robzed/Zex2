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

// *****************************************************************MODULE HEADER*****
// * FILENAME		: processor.cpp
// * MODULE TITLE	: processor related functions
// * PROJECT		: Zex
// * DATE STARTED	: 2nd June 1999, Wed 10:49pm
// * ORIGINAL AUTHOR: Rob Probin
// *
// * COPYRIGHT (c) 1999 Lightsoft
// *		   (c) 1999 Robert Probin and Stuart Ball
// *		   http://www.lightsoft.co.uk/
// *
// * Lightsoft is a trading name of Robert Probin and Stuart Ball.
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial		  Descrip.
// *
// *
// *
// ***********************************************************************************
/*
 * $Log: processor.cpp,v $
 * Revision 1.2  2003/09/20 12:57:00  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:35:13  stu_c
 * First Imported.
 *
 * Revision 1.16  2002/09/08 02:00:26  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.15  2002/08/27 20:59:39  stu_c
 * Colliding shells
 *
 * Revision 1.14  2002/07/07 03:45:23  stu_c
 * Lasers now work but need moving to POCB's
 *
 * Revision 1.13  2002/07/04 22:43:30  stu_c
 * debug switch to switch off rt thread in 3d_structures_def.h
 *
 * Revision 1.12  2002/04/14 15:59:49  stu
 * Further work
 *
 * Revision 1.11  2002/04/13 15:31:44  stu
 * Rotation, Alycians, behaviour
 *
 * Revision 1.10  2002/02/16 19:04:23  stu
 * Radar range decrease key and rationalisation of dlp colours
 *
 * Revision 1.9  2002/02/15 22:50:02  stu
 * Added thread control code
 *
 * Revision 1.8  2001/12/31 20:03:56  stu
 * Dunno
 *
 * Revision 1.7  2001/12/29 01:10:08  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.6  2001/12/14 19:44:18  stu
 * Docking
 *
 * Revision 1.5  2001/11/03 14:28:17  stu
 * Further OS X work
 *
 * Revision 1.4  2001/11/02 19:30:59  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.3  2001/10/27 18:56:35  rob
 * dual interface lib/carbon lib changes
 *
 * Revision 1.2  2001/10/22 21:27:53  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:02:45  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:04  rob
 * First Imported.
 *
 * Revision 1.5  2001/09/23 17:58:27  rob
 * now have real_frame_time_in_secs
 *
 * Revision 1.4  2001/09/18 20:42:42  stu
 * More Sim/ hud numbers print
 *
 * Revision 1.3  2001/09/10 21:40:57  stu
 * 100901
More Sim, AP broken.
 *
 * Revision 1.2  2001/07/29 10:38:00  rob
 * time_in_secs became double, various timing changes
 *
 * Revision 1.1.1.1  2001/01/01 21:13:34  rob
 * First Imported.
 *
 * Revision 1.3  2000/09/20 23:56:10  stu
 * HUD/AP/Scaling
 *
 * Revision 1.2  2000/08/27 01:34:13  stu
 * Hud changes
 *
 * Revision 1.1  2000/08/27 00:01:23  rob
 * added freeze physics routines to stop time (returns zero time per frame, therefore not changing physics)
 *
 * Revision 1.0.0.1  2000/08/21 22:00:01  stu
 * First Imported.
 *
 */

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

  #include "options.h"
  #include "processor.h"

  #include	"error_handler.h"
  #include "3d_structures_def.h"
#endif //pch


#include "dlp.h"
#include "logging.h"

// ***********************************************************************************
// * CONSTANTS 
// *

// ***********************************************************************************
// * TYPE DEFINITIONS
// *


// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *


static int our_cache_line_size;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *


// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: init_cache_line_size
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 2/6/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: 
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	: 
// |              
// |              
// |
// | PARAM OUT	:
// |
// | ASSUMPTIONS:
// | LIMITATIONS: 
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------


// WARNING: Cache lines are ALWAYS 32 byte aligned. It zero's the bottom 5 bits of the 
// 			address supplied when using a cache instruction!!!
//

/* --we don't use this anymore, taken out
void init_cache_line_size(char *big_buffer)
{
char *temp_ptr;
long *longtemp_ptr;
int count;

// 32 byte align
big_buffer+=32;
big_buffer=big_buffer - (((int)big_buffer)&31);


// first fill 16384 bytes with FFh

longtemp_ptr=(long *)big_buffer;

for(count=0; count<16384/4; count++)
  {
  *longtemp_ptr=0xffffffff;
  longtemp_ptr++;
  }


// do the real thing
        #if ZEX_PROJECT_BUILDER
//          MyDebugStr(__LINE__,__FILE__,"dcbz not sorted yet in Project Builder");
        our_cache_line_size=64;
		return;
          //__dcbz((void *)big_buffer,8192);
	#else
	  __dcbz((void *)big_buffer,8192);
        #endif
        
// find the start of the zero's

temp_ptr=big_buffer;

for(count=0; count<16384; count++)
  {
  if(*temp_ptr==0)
    {
    break;
    }
  temp_ptr++;
  }


if(count==16384)		// no zero's found - what the hell happened to the dcbz data!!!!
  {
  our_cache_line_size=32;		// default size (chicken solution)
  return; 
  }

// find out how much has been zero'd

for(count=0; count<8192; count++)
  {
  if(*temp_ptr!=0)
    {
    break;
    }
  temp_ptr++;
  }

our_cache_line_size=count;

}
*/


// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE			: get_cache_line_size
// | AUTHOR(s) 		: Rob Probin
// | DATE STARTED	: 2/6/99
// +----------------------------------------------------------------------------------
// | DESCRIPTION: 
// |
// +----------------------------------------------------------------------------------
// | PARAM IN	: 
// |              
// |              
// |
// | PARAM OUT	:
// |
// | ASSUMPTIONS:
// | LIMITATIONS: 
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial		  Descrip.
// |
// |
// +----------------------------------------------------------------------------------

// WARNING: Cache lines are ALWAYS 32 byte aligned. It zero's the bottom 5 bits of the 
// 			address supplied when using a cache instruction!!!
//


int get_cache_line_size(void)
{
return our_cache_line_size;
}

//optimized PPC functions
//See tn1174 for details
long abs_nobranch (long i)
{
 long sign, temp, result;
 
 sign = i >> 31;
 temp = i ^ sign;
 result = temp - sign;
 
 return result;
}


int InRange(int value, int min, int max)
{
    return ((unsigned) (value - min) <= (unsigned) (max - min));
}



/////////////////////////////////////////////////////////
//time routines
//
//

// ***time removed - not used**** UInt32 time;	  //time per frame in milliseconds
double time_in_secs;		// became double 11-7-2001
double real_frame_time_in_secs;	// the actual frame time, as opposed to the game time frame time

static AbsoluteTime start_ticks;

static int startup_frame_count;

//
// ***************************************************************************
//

void reset_microsecs_timer(void)
{
 start_ticks=UpTime();
 startup_frame_count=0;
}

extern int time_multiplier;	//real_time.c

//
// ***************************************************************************
//
void update_microsecs_timer(void)	//to be called once per frame to calc time per frame for physics
{
AbsoluteTime current_nanoseconds,new_ticks;


//
// First calculate the actual frame time "real_frame_time_in_secs"
// 
new_ticks=UpTime();
current_nanoseconds=AbsoluteToNanoseconds(SubAbsoluteFromAbsolute(new_ticks,start_ticks)); //time in nanoseconds
start_ticks=new_ticks;

real_frame_time_in_secs=current_nanoseconds.lo;
real_frame_time_in_secs /= 1000000000.0;		      // convert to seconds

//
// Now we deal with the game time frame timer "time_in_secs"
//
if(physics_frozen())
  {
// when we restart time we need to reset the start time so that the forces applied don't jump when restarted.
  time_in_secs=0;
  }
else  if(startup_frame_count==0)	//wait for frame time to settle
  {
  startup_frame_count++;
  time_in_secs=0;
  }   
else	// physics not frozen
  {
  time_in_secs = real_frame_time_in_secs;
  time_in_secs *= time_multiplier;	// time_multiplier is from real_time.c
  }
} // end of update_microsecs_timer


//
// ***************************************************************************
//



// private variable
// internal state for the physics frozen
static int physics_frozen_private=0;

// public routine
// an accessor to see if the physics are frozen
int physics_frozen(void)
{
return physics_frozen_private;
}

// public routine
// freeze the physics where it is
// this includes the time (used when applying forces), the force applications per frame,
// and the monster, special events, (and probably player) controllers.
void freeze_physics(void)
{
physics_frozen_private=-1;
}

// public rotine
// restart the physics engine
void unfreeze_physics(void)
{
physics_frozen_private=0;
}


// ------------------------------------------------------
// OS X priority routines



#define SCHED_RR    2

#define __SCHED_PARAM_SIZE__ 4
//struct sched_param { int sched_priority;  char opaque[__SCHED_PARAM_SIZE__]; };

pthread_t theThread;
struct sched_param origThreadParam;
int origPolicy;



void InitThreads()
{
#if RT_PRIORITY==1
    theThread=pthread_self();
    pthread_getschedparam(theThread,&origPolicy,&origThreadParam);
#endif
}

void SetHighPriority()
{
#if RT_PRIORITY==1
    int thePolicy;
    struct sched_param theThreadParam;
	
	
    #if ZEX_PROJECT_BUILDER==1
    fprintf(stderr,"Exec: Switching to high priority thread\n");
	#endif
		
    theThreadParam=origThreadParam;
    thePolicy=SCHED_RR;
    theThreadParam.sched_priority = 30;	//no higher than 47 (which is real time)
    pthread_setschedparam(theThread,thePolicy,/*(sched_param*)*/&theThreadParam);
#else
    #if ZEX_PROJECT_BUILDER==1
     fprintf(stderr,"Exec: Switch to high priority thread denied - RT_PRIORITY=0\n");
    #endif
#endif
}

void SetNormalPriority()
{
#if RT_PRIORITY==1
    #if ZEX_PROJECT_BUILDER==1
    fprintf(stderr,"Exec: Switching to normal priority thread\n");	
	#endif
    pthread_setschedparam(theThread,origPolicy,/*(sched_param*)*/&origThreadParam);
#endif
}


void thread_yield(void)
{
//#if RT_PRIORITY==1
EventRecord the_event;
//AbsoluteTime absTime;
//Nanoseconds nanTime;
extern int gShowConsole;


//nanTime.lo=20e7; //10 ms
//nanTime.hi=0;

//absTime=UpTime();	//for MPDelayUntil
//absTime = AddNanosecondsToAbsolute(absTime, nanTime);
//MPDelayUntil(&absTime);
  if (gShowConsole==0)
	  WaitNextEvent(0xffff, &the_event, 0/* time in ticks */, 0 /* region */ );
//#endif //RT_PRIORITY
}
