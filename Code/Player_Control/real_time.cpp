// *************************************MODULE HEADER*********************************
// * FILENAME           : real_time.c
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * DATE STARTED       : 03:02:30 on 9th September 1999
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************
/* $Log: real_time.c,v $
/* Revision 1.14  2002/09/08 02:01:57  stu_c
/* Precompiled header adjustments
/*
/* Revision 1.13  2002/05/31 23:51:55  stu
/* Removed all warnings from the code
/*
/* Revision 1.12  2002/02/17 06:18:59  stu
/* Added radar targetting
/*
/* Revision 1.11  2002/02/16 19:14:44  stu
/* DLP_BLUE changed to DLP_PURP
/*
/* Revision 1.10  2002/02/16 19:00:54  stu
/* Radar range decrease key and rationalisation of dlp colours
/*
/* Revision 1.9  2002/02/10 21:53:49  stu
/* Widened gap for salary addition
/*
/* Revision 1.8  2002/01/27 18:09:18  stu
/* Added a few direct set time multiplier functions; now called TEMPMULT
/*
/* Revision 1.7  2002/01/20 17:02:50  stu
/* Added get_time_multiplier call
/*
/* Revision 1.6  2001/12/29 01:09:38  stu
/* Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
/*
/* Revision 1.5  2001/12/27 19:48:04  stu
/* Additional development work for docking
/*
/* Revision 1.4  2001/11/09 18:28:45  rob
/* Various debug changes.
/*
/* Revision 1.3  2001/11/02 19:31:04  rob
/* Build variants for interface lib and carbon.
Coffee fixes after changes for X.
/*
/* Revision 1.2  2001/10/22 21:28:27  rob
/* Carbon warning changes
/*
/* Revision 1.1  2001/10/21 01:02:57  stu
/* Initial porting work
/*
/* Revision 1.0.0.1  2001/10/17 20:46:07  rob
/* First Imported.
/*
/* Revision 1.5  2001/09/18 20:42:36  stu
/* More Sim/ hud numbers print
/*
/* Revision 1.4  2001/07/29 10:38:27  rob
/* real time clock linked to processor.c timing
/*
/* Revision 1.3  2001/05/28 01:48:49  stu
/* 280501
/*
/* Revision 1.2  2001/04/01 18:23:16  stu
/* *** empty log message ***
/*
/* Revision 1.1.1.1  2001/01/01 21:13:38  rob
/* First Imported.
/*
/* Revision 1.6  2000/12/03 22:20:53  rob
/* Changed time multiplier back to sensible values.
/*
/* Revision 1.5  2000/11/28 22:29:51  stu
/* TOD months, weeks, days etc for stats screen
/*
/* Revision 1.4  2000/11/26 22:55:41  stu
/* Various.
/*
/* Revision 1.3  2000/10/08 21:46:16  stu
/* Engine conversion to 64 bit. Bitch of a job.
/*
/* Revision 1.2  2000/09/20 23:55:15  stu
/* HUD/AP/Scaling
/*
/* Revision 1.1  2000/09/09 21:08:02  stu
/* Split salary message over 2 lines
/*
/* Revision 1.0.0.1  2000/08/21 22:08:30  stu
/* First Imported.
/*
 */


// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <Windows.h>
    #include <Quickdraw.h>
    #include <NumberFormatting.h>
  #endif

  #include "options.h"
  #include "3d_structures_def.h"
  #include "real_time.h"
  #include "zex_misc.h"
  #include "game_defs.h"
  #include "processor.h"
  #include "Error_handler.h"
#endif  //pch

#include	"txtsplat.h"
#include "dlp.h"
#include "Zsound.h"
#include "sds.h"
#include "camera_control.h"
// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

extern int accumulated_game_time,start_time;
int real_game_time;	//in ticks
int sleep_timer;	//gets loaded with number of frames till we need to sleep
int pilot_asleep;
int time_multiplier;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

bool check_tempmult(void);


void get_real_time_struct(real_time_struct* time_struct)
{
int current_t, divisor;
current_t=real_game_time;		//time in zex ticks - 10ths of a second


divisor=60*60*60*24;
time_struct->days=current_t/divisor;
current_t-=time_struct->days*divisor;

divisor=60*60*60;
time_struct->hours=current_t/divisor;
current_t-=time_struct->hours*divisor;

divisor=60*60;
time_struct->minutes=current_t/divisor;
current_t-=time_struct->minutes*divisor;

divisor=60;
time_struct->seconds=current_t/divisor;
current_t-=time_struct->seconds*divisor;



}

int time_to_check_salary;
int done_salary;
//static int last_ticks;

void init_game_time()
{
accumulated_game_time=8*60*60*60;	//8 am
sleep_timer=MAX_AWAKE;	//-38*60*60*8;	//14 hours at 38 fps
//sleep_timer=38*60*13+60;	//14 hours at 38 fps
pilot_asleep=0;
time_to_check_salary=100;
done_salary=0;
time_multiplier=1;
//last_ticks=TickCount();
}


void set_time_multiplier_to_1(void)
{
if (!check_tempmult()) return;

if(time_multiplier!=1)
 {
  time_multiplier=1;
  add_to_text_display("OBC> Tempmult forced back to 1",DLP_GREEN);
  time_in_secs=0.03;
  
 }
 
}

void set_time_multiplier_to_2(void)
{
if (!check_tempmult()) return;

if(time_multiplier>2)
 {
  time_multiplier=2;
  add_to_text_display("OBC> Tempmult set to 2 by AP",DLP_GREEN);
  time_in_secs=0.03;
  
 }
 
}



void set_time_multiplier_to_5(void)
{
if (!check_tempmult()) return;

if(time_multiplier>5)
 {
  time_multiplier=5;
  add_to_text_display("OBC> Tempmult set to 5 by AP",DLP_GREEN);
  time_in_secs=0.03;
  
 }
 
}

void set_time_multiplier_to_10(void)
{
if (!check_tempmult()) return;
if(time_multiplier>10)
 {
  time_multiplier=10;
  add_to_text_display("OBC> Tempmult set to 10 by AP",DLP_GREEN);
  time_in_secs=0.03;
  
 }
 
}


void set_time_multiplier_to_1000(void)
{
if (!check_tempmult()) return;
  time_multiplier=1000;
  add_to_text_display("OBC> Tempmult set to 1000",DLP_GREEN);  
}

void set_time_multiplier_to_500(void)
{
if (!check_tempmult()) return;
  time_multiplier=500;
  add_to_text_display("OBC> Tempmult set to 500 by AP",DLP_GREEN);  
}


void set_time_multiplier_to_200(void)
{
if (!check_tempmult()) return;
  time_multiplier=200;
  add_to_text_display("OBC> Tempmult set to 200 by AP",DLP_GREEN);  
}

int get_time_multiplier(void)
{
 return time_multiplier;
}


//Tempmult comes in 5 classes as follows
void flip_time_multiplier_up(void)
{
 sds_attrib_data_type my_attrib;
 int tm_class;

if (!check_tempmult()) return;

 get_ship_attrib_data(get_main_camera_object(), TEMPM, &my_attrib);
 tm_class=my_attrib.the_value;
 
 if (time_multiplier==1)
 {
      add_to_text_display("OBC> Tempmult running at x2",DLP_GREEN);
      time_multiplier=2;
 }
 else
 if (time_multiplier==2)
 {
      add_to_text_display("OBC> Tempmult running at x5",DLP_GREEN);
      time_multiplier=5;
 }
 else
 if (time_multiplier==5)
 {
   if (tm_class<2) return; //class 1 = max 5
      add_to_text_display("OBC> Tempmult running at x10",DLP_GREEN);
      time_multiplier=10;
 }
 else
 if (time_multiplier==10)
 {
     add_to_text_display("OBC> Tempmult running at x25",DLP_GREEN);
      time_multiplier=25;
 }
 else
 if (time_multiplier==25)
 {
   if (tm_class<3) return; //class 2 = max 25
      add_to_text_display("OBC> Tempmult running at x50",DLP_GREEN);
      time_multiplier=50;
 }
  else
 if (time_multiplier==50)
 {
      add_to_text_display("OBC> Tempmult running at x100",DLP_GREEN);
      time_multiplier=100;
 }
  else
 if (time_multiplier==100)
 {
   if (tm_class<4) return; //class 3 = max 100
      add_to_text_display("OBC> Tempmult running at x200",DLP_GREEN);
      time_multiplier=200;
 }
  else
 if (time_multiplier==200)
 {
      add_to_text_display("OBC> Tempmult running at x500",DLP_GREEN);
      time_multiplier=500;
 }
  else
 if (time_multiplier==500)
 {
   if (tm_class<5) return; //class 4 = max 500
      add_to_text_display("OBC> Tempmult running at x1000",DLP_GREEN);
      time_multiplier=1000;
 }
   else
 if (time_multiplier==1000)
 {
      add_to_text_display("OBC> Tempmult running at x10000",DLP_GREEN);
      time_multiplier=10000;
 }

  else
 if (time_multiplier==10000)
 {
      add_to_text_display("OBC> Tempmult running at maximum (10000)",DLP_GREEN);
 }



}


void flip_time_multiplier_down(void)
{

if (!check_tempmult()) return;

 
 if (time_multiplier==1)
 {
      add_to_text_display("OBC> Tempmult running at minimum (1)",DLP_GREEN);
      time_multiplier=1;
 }
 else
 if (time_multiplier==2)
 {
      add_to_text_display("OBC> Tempmult running at x1",DLP_GREEN);
      time_multiplier=1;
 }
 else
 if (time_multiplier==5)
 {
      add_to_text_display("OBC> Tempmult running at x2",DLP_GREEN);
      time_multiplier=2;
 }
 else
 if (time_multiplier==10)
 {
           add_to_text_display("OBC> Tempmult running at x5",DLP_GREEN);
      time_multiplier=5;
 }
 else
 if (time_multiplier==25)
 {
      add_to_text_display("OBC> Tempmult running at x10",DLP_GREEN);
      time_multiplier=10;
 }
 else
 if (time_multiplier==50)
 {
      add_to_text_display("OBC> Tempmult running at x25",DLP_GREEN);
      time_multiplier=25;
 }
 else
 if (time_multiplier==100)
 {
      add_to_text_display("OBC> Tempmult running at x50",DLP_GREEN);
      time_multiplier=50;
 }
 else
 if (time_multiplier==200)
 {
      add_to_text_display("OBC> Tempmult running at x100",DLP_GREEN);
      time_multiplier=100;
 }
 else
 if (time_multiplier==500)
 {
      add_to_text_display("OBC> Tempmult running at x200",DLP_GREEN);
      time_multiplier=200;
 }
 else
 if (time_multiplier==1000)
 {
      add_to_text_display("OBC> Tempmult running at x500",DLP_GREEN);
      time_multiplier=500;
 }
 else
 if (time_multiplier==10000)
 {
      add_to_text_display("OBC> Tempmult running at x1000",DLP_GREEN);
      time_multiplier=1000;
 }

}


bool check_tempmult(void)
{
 sds_attrib_data_type my_attrib;
 get_ship_attrib_data(get_main_camera_object(), TEMPM, &my_attrib);
 
 if (my_attrib.the_value==0)
 {
        add_to_text_display("OBC> Tempmult not fitted.",DLP_RED);
        return FALSE;
 }
 return TRUE;
}

//called every frame
void check_sleep()
{
//extern UInt32 gCannon;
extern int rank,difficulty,credits;
char salary_start[]="OBC> Message from Command. Your daily";
char salary_note[] = "salary of ";
char credits_string[] = " credits has been paid.";
char salary_string [100];
unsigned char number_string[12];
int i,o,m,n;
char the_char;
real_time_struct time_struct;
float salary=0.0,smult=1;
//check time to do salary
if (time_to_check_salary<0)
 {
 time_to_check_salary=100;
 get_real_time_struct(&time_struct);

 if ((time_struct.hours>=8) && (time_struct.hours<=11)) 
 {
  if (done_salary==0)
  {
   //salary based on rank
   if (difficulty==32) smult=1.5;
   else
   if (difficulty==16) smult=1;
   else
   if (difficulty==8) smult=0.75;
   else
   if (difficulty==1) smult=0.5;
   else MyDebugStr(__LINE__,"real_time.c","check_sleep: Difficulty Fail");
  
   
   if (rank==ENS) salary=40;
   else
   if (rank==LT) salary=80;
   else
   if (rank==LTCR) salary=130;
   else
   if (rank==CDR) salary=200;
   else
   if (rank==CAPT) salary=280;
   else
   if (rank==RADM) salary=320;
   else
   if (rank==VADM) salary=360;
   else
   if (rank==ADM) salary=450;
   else
   if (rank==FADM) salary=600;
   else MyDebugStr(__LINE__,"real_time.c"," check_sleep: Rank Fail");
   salary*=smult;
  //add and display salary
   credits+=salary;
  //copy salary_note to salary_string
  i=0;
  o=0;

   add_to_text_display(salary_start,DLP_PURP);


  copy_note: the_char=salary_note[i];
   if (the_char!=0) 
    {
    salary_string[o]=the_char;
    i++;o++;
    goto copy_note;
    }
  //now get the number of credits as a string
    NumToString(salary,number_string);
  //copy to salary_string
   n=number_string[0];
   m=0;
   while (m<n)
    {
    salary_string[o]=number_string[m+1];
    o++; m++;
    }
    i=0;
  copy_creds: the_char=credits_string[i];
   if (the_char!=0) 
    {
    salary_string[o]=the_char;
    i++;o++;
    goto copy_creds;
    }
  salary_string[o]=0;
  
   add_to_text_display(salary_string,DLP_PURP);
      play_zsound_always(message_from_command, sound_high_pri, c1_chan1, sound_vol_7);
  
    done_salary=1;
    }	//end of done salary==0
   }	//end of 8 oclock
 else done_salary=0;	//not 800 to 10:59
 
 }
 else
 time_to_check_salary--;
 
if (pilot_asleep==0) sleep_timer--;

  if (sleep_timer>38*60*120+1 && pilot_asleep==0) return;
 
 if (sleep_timer==38*60*120) 	//2 hours
 add_to_text_display("OBC> Might be time to take some rest. You seem to be getting a little tired.",DLP_PURP);
 else
 if (sleep_timer==38*60*60) 	//1 hour
 add_to_text_display("OBC> I think you are getting tired boss. Best take a break.",DLP_PURP);
 else
 if (sleep_timer==38*60*30) 
 add_to_text_display("OBC> you are very tired boss. Better take a break.",DLP_PURP);
 else
 if (sleep_timer==38*60*15) 
 add_to_text_display("OBC> If you don't get some sleep now we are in big trouble!",DLP_PURP);
 else
 if (sleep_timer==38*60*14) 
 add_to_text_display("OBC> You really must sleep now dude! Find a mothership quick.",DLP_PURP);
 else
 if (sleep_timer==38*60*13) 
 {
 add_to_text_display("OBC> If you don't get some rest soon,you'll fall asleep at the controls!",DLP_PURP);
 }

 if (sleep_timer==38*60*12)
 add_to_text_display("YOU HAVE FALLEN ASLEEP AND LOST CONTROL.",DLP_PURP);
 
 if (sleep_timer<38*60*12 && pilot_asleep==0)
  {
  pilot_asleep=1;
  }


if (pilot_asleep==1)
  {
//  add_to_dlp(SLEEP_DIALOG,0,50,0,0,0,"");
  sleep_timer++;
  if (sleep_timer==38*60*60*13) pilot_asleep=0;	//14 hours at 38 fps

  if (Button()==1) pilot_asleep=0;
  add_to_dlp(DRAW_TEXT,150,100,215,0,0,"You have fallen asleep. Click mouse button to wake.",0,0,0);

  }

}



//real time functions

void draw_real_time(int x, int y, int colour)
{
real_time_struct time_struct;
char number_string1[12];

 get_real_time_struct(&time_struct);


 num_to_string999(time_struct.days,number_string1);	//hours
 SplatText_Transparent ((char *)number_string1,x,y,colour);
    x+=8;
	SplatText_Transparent (":",x+14,y-1,colour);
	
 num_to_string99(time_struct.hours,number_string1);	//hours
 SplatText_Transparent ((char *)number_string1,x+20,y,colour);
//
	SplatText_Transparent (":",x+34,y-1,colour);
//	  
 num_to_string99(time_struct.minutes,number_string1);	//mins
 SplatText_Transparent ((char *)number_string1,x+40,y,colour);

	SplatText_Transparent (":",x+55,y-1,colour);
	
 num_to_string99(time_struct.seconds,number_string1);	//secs
 SplatText_Transparent ((char *)number_string1,x+61,y,colour);


}




//called every frame from main loop and other places
void update_real_time()	//updates real game time with the time since the last call
{
static double remainder_handler=0;
int time_to_add;

//int current_ticks,diff;
//current_ticks=TickCount();
//diff=current_ticks-last_ticks;
//last_ticks=current_ticks;

//accumulated_game_time+=diff*time_multiplier;

remainder_handler+=time_in_secs;	// time in seconds from this frame, plus fraction we couldn't handle from last frame

time_to_add=remainder_handler*60;	// integer number of 60ths that we can handle this frame

remainder_handler=remainder_handler-(((double)time_to_add)/60.0);	// save fractional part we can't handle this frame, for next frame

accumulated_game_time+=time_to_add;		// update 60th timer


real_game_time=accumulated_game_time;


//real_game_time=(TickCount()-start_time)+accumulated_game_time;
//real_game_time*=time_multiplier;
}


void add_a_minute_to_real_time()	//called from sleep
{
real_game_time+=(60*60);
}




//returns the hour
int get_TOD_hours()
{
real_time_struct time_struct;
 get_real_time_struct(&time_struct);
return time_struct.hours;
}

int get_TOD_mins()
{
real_time_struct time_struct;
 get_real_time_struct(&time_struct);
return time_struct.minutes;
}

int get_TOD_secs()
{
real_time_struct time_struct;
 get_real_time_struct(&time_struct);
return time_struct.seconds;
}
