//debug_timing.c
//SB 6/2/99
//DO NOT RELEASE THIS CODE - IT ONLY WORKS WHEN LINKED WITH DRIVERSEERVICESLIB
//WHICH IS NOT AVAILABLE EVERYWHERE
/* $Log: debug_timing.c,v $
/* Revision 1.5  2002/09/08 02:00:38  stu_c
/* Precompiled header adjustments
/*
/* Revision 1.4  2001/12/29 01:10:00  stu
/* Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
/*
/* Revision 1.3  2001/11/02 19:30:56  rob
/* Build variants for interface lib and carbon.
Coffee fixes after changes for X.
/*
/* Revision 1.2  2001/10/22 21:28:51  rob
/* Carbon warning changes
/*
/* Revision 1.1  2001/10/21 01:01:34  stu
/* Initial porting work
/*
/* Revision 1.0.0.1  2001/10/17 20:46:04  rob
/* First Imported.
/*
/* Revision 1.3  2001/08/12 19:01:38  rob
/* Sorted out some minor problems with the timings code.
/*
/* Revision 1.2  2001/07/29 10:34:49  rob
/* changes for coffee
/*
/* Revision 1.1.1.1  2001/01/01 21:13:34  rob
/* First Imported.
/*
/* Revision 1.0.0.1  2000/08/21 22:00:17  stu
/* First Imported.
/*
 */

// ***********************************************************************************
// * INCLUDED FILES
// *

#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "options.h"		// should always be first include

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <DriverServices.h>
  #endif

#endif //pch


#include "debug_timing.h"

// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

int game_timing=PCI_TIMING_DEBUG;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

void start_timer(UnsignedWide *user_time_var);
void stop_timer(UnsignedWide *user_time_var);


//#if !PCI_TIMING_DEBUG
//#error !!!! NEED TO SORT DUMMY PCI TIMING ROUTINES !!!! 
//#endif


//this routine takes a pointer to an UnsignedWide (thats a 64 bit to you and me) and 
//starts a timer
//User should not touch the int32 whilst timer is in operation
void start_timer(UnsignedWide *user_time_var)
{
	(*user_time_var)=UpTime();

}

//this routine stops the appropriate timer
//it returns in the lower 32 bits of the input var, the diff between start
//and stop time
void stop_timer(UnsignedWide *user_time_var)
{
UnsignedWide t;
//UInt32 stop_low;
	t=UpTime();
	
	(*user_time_var)=AbsoluteToNanoseconds(SubAbsoluteFromAbsolute(t,*user_time_var));

}

#define DEFAULT_FRAMES_TO_RESET_AFTER 600	/* set to -1 to disable reset */

int frames_to_reset_after=DEFAULT_FRAMES_TO_RESET_AFTER;
//#define FRAMES_TO_RESET_AFTER 600	/* set to -1 to disable reset */
					// Note: setting too low gives false readings due to interrupts.
					

#define NUMLOC T_MAX-T_MIN+10		/* 10 for safety */
double timemin[NUMLOC];
double timemax[NUMLOC];
double timetotal[NUMLOC];
double timeav[NUMLOC];
double timepercent[NUMLOC];

int timed_frames=DEFAULT_FRAMES_TO_RESET_AFTER+1;
int total_frames=0;				// never reset

UnsignedWide timesofar;
UnsignedWide *T_timesofar_ptr=&timesofar;



void T(int routine_code)
{
double current_time;
int count;

if(!game_timing) return;


if(routine_code==0) total_frames++;

if(frames_to_reset_after>0 && timed_frames>frames_to_reset_after)
  {
  timed_frames=0;
  for(count=T_MIN; count<=T_MAX; count++)
    {
    timemin[count]=0;
    timemax[count]=0;
    timetotal[count]=0;
    timeav[count]=0;
    timepercent[count]=0;
    
    }
  }
else // else not reset
  {
  stop_timer(T_timesofar_ptr);

  current_time=((int)T_timesofar_ptr->lo);


  // increment number of frames
  if(routine_code==0) timed_frames++;

  // TOTAL
  timetotal[routine_code]+=current_time;

  // *** MINIMUMS ***
  if(timemin[routine_code]==0) { timemin[routine_code]=999999999; }	// zero means non-initialised
  if(current_time==0) { timemin[routine_code]=1; }
  else { if(timemin[routine_code]>current_time) timemin[routine_code]=current_time; }
  // minimum CANNOT be zero - will use 1 instead

  // *** MAXIMUMS ***
  if(timemax[routine_code]<current_time) { timemax[routine_code]=current_time; }
  // BIG NOTE!! maximums cannot be trusted too much - interrupts (e.g. music) can make them look much worst. This
  // effects maximums much more than averages (which over long periods even out over the whole range).

  if(timed_frames != 0 )
    {
    // *** MEAN AVERAGE ***
    timeav[routine_code]=timetotal[routine_code]/timed_frames;
    }
    
   
  if(timed_frames > 1 )
    {
    // *** PERCENTAGE ***
    current_time=0;

    for(count=T_MIN; count<=T_MAX; count++)
      {
      current_time+=timeav[count]; 		    	    // create sum of all averages
      }

    timepercent[routine_code]=100*(timeav[routine_code]/current_time); // 100 * fraction of total time
    }
    
  } // end ... else if not reset
  

// start the timer for the next go
start_timer(T_timesofar_ptr);
}

