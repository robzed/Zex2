/*
 *  motherships2.c
 *  zex22
 *
 *  Created by stu on Sat Dec 01 2001.
 *  Copyright (c) 2001 Lightsoft (Games). All rights reserved.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <fp.h>
    #include <sound.h>
  #endif

  #include "3d_structures_def.h"
  #include	"access_ocbs.h"
  #include	"processor.h"
  #include "real_time.h"
  #include "zex_misc.h"
  #include "Error_handler.h"
#endif //pch

#include	"object_control_top.h"
#include	"physics.h"
#include	"motherships2.h"
#include	"dlp.h"
#include	"autopilot.h"
#include "camera_control.h"
#include "hud.h"
#include "eclipse2.h"
#include "Zsound.h"
#include "docked.h"
#include "static_object_support.h"
#include "load_objects.h"
#include "obj_control_misc_routines.h"
#include "object_manipulation.h"
#include "NPC.h"
#include "objects_controls.h"
#include "behaviour.h"

void check_if_anything_to_dock(int it);
void MS_launch_ship(int MS, int MS_allegiance);

//float 5 is check anything to dock timer
//float 6 is launch timer
//cu1 is docking state machine control
//cu2 is looped sound ref for docking
//cu3 is number of ships in dock
//cu4 is object being launched
//cu7 is launch a ship timer
//cu8 is old game secs

void handle_mothership(int it)
{
ZObject * current_object_ptr;
extern	DynObjectsFixedSize *ocb_ptr;

 current_object_ptr=&ocb_ptr->object_list[it];

//check the launch ship timer

  current_object_ptr->Dyn_OCB_control_data.control_use_1_float-=real_frame_time_in_secs; //so we're not affected by TempMult

if ( current_object_ptr->Dyn_OCB_control_data.control_use_1_float<0) //time to launch
 {
  if ((*ocb_ptr).object_list[it].Dyn_OCB_control_data.docking_what==-1)	//we're not docking
  { 
    if (get_distance_to_nearest_NPC_in_m(it,5000)>5000) //there's no NPC within a 5 km radius
	{
          MS_launch_ship(it, current_object_ptr->Dyn_OCB_control_data.allegiance);           
	  current_object_ptr->Dyn_OCB_control_data.control_use_1_float=NDRangedRdm(120,600);
    }
  }
 }

 if (current_object_ptr->Dyn_OCB_control_data.docking_what!=-1)
 {
  MS_handle_dock(it);
  return;
 }
 //not docking anything, see if there is anything in the vicinity that we could dock

 check_if_anything_to_dock(it);
}


//We need some pobs integration here, but for now I'll jsut randomise it
void MS_launch_ship(int MS, int MS_allegiance)
{
int val=NDRangedRdm(0,10);
   
   //return;
   
   if (val<9)
   { 
     launch_NPC(MS, FALCON, NPC_CLASS_GUARD, ALIEN); //Just hangs about looking for enemies to attack
   }
   else //launch a shuittle
   {
	  //time to launch a ship
	  if (MS_allegiance==NEUTRAL)
	  {
	   int val=NDRangedRdm(0,12);
	   if (val<5) 
           { launch_NPC(MS,SHUTTLE, NPC_CLASS_TRADER, NEUTRAL); }
	   else
	   if (val<8) 
           { launch_NPC(MS,SHUTTLE, NPC_CLASS_TRADER, HUMAN); }
	   else
           { launch_NPC(MS,SHUTTLE, NPC_CLASS_TRADER, ALIEN); }
	   
	  }
	  else
	  {
              launch_NPC(MS,SHUTTLE, NPC_CLASS_TRADER, MS_allegiance);
          }
   }

}



void check_if_anything_to_dock(int it)
{
int active_object_counter=0;
int object_counter=0;
ZObject * current_object_ptr;
extern	DynObjectsFixedSize *ocb_ptr;
extern int dynamic_object_index;


//every 1 second we look for something to dock. We use control_use_5_float as the time counter
if ((*ocb_ptr).object_list[it].Dyn_OCB_control_data.allegiance==1)
{
 int a=0;
 a++;
}

(*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_2_float-=time_in_secs;
if ((*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_2_float>0) return;
(*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_2_float=1.0; //seconds till next run

//Are we docking summat already?
if ((*ocb_ptr).object_list[it].Dyn_OCB_control_data.docking_what!=-1) return;	//Yes, lets not do anything silly then.

//Scan all objects looking for the request_dock flag is set.
//If so, calc distance and if close enough dock
while (active_object_counter<dynamic_object_index)
{

 current_object_ptr=&ocb_ptr->object_list[object_counter];
 if ((current_object_ptr->in_use==1))	//zex or vv
  {
   //is the object requesting dock?
   if(current_object_ptr->Dyn_OCB_control_data.request_dock==1)
   {
    //find out how far away it is...
	if ( get_distance_to_from_in_m(it, object_counter)<5000)	//if within 5 kilometres
	 {
	 double dockee_y,MS_y;
	  //object is close enough to dock. Find out if object can affort the docking toll
	  
	  //make sure object is below mothership
	  dockee_y=current_object_ptr->Oworldy;
	  MS_y=(*ocb_ptr).object_list[it].Oworldy;
	  
	 
      if (dockee_y>MS_y)
	  {
	   //Messsage dockee
	   if (get_main_camera_object()==object_counter)
	   { 
	    add_to_text_display("OBC> We need to be below docking bay to dock",DLP_YELLOW);
	    add_to_text_display("OBC> Docking request cancelled",DLP_YELLOW);
	   }
	   //turn off dock on dockee
	   current_object_ptr->Dyn_OCB_control_data.request_dock=0;
	   return;
	  }
	  
	  if ( get_length_of_vector(current_object_ptr->abs_velocity_vector)  > 50/ZEX_UNIT_SIZE_IN_METERS)	//vel below 50m/s
	  {
	   //Messsage dockee
	   if (get_main_camera_object()==object_counter)
	   { 
 	     add_to_text_display("OBC> Velocity too high to dock!",DLP_YELLOW);
	     add_to_text_display("OBC> Docking request cancelled",DLP_YELLOW);
	   }
	   //turn off dock on dockee
	   current_object_ptr->Dyn_OCB_control_data.request_dock=0;
	   return;
	  }
	  
	  //If we are docking the camera object, object should target the mothership
	  
	  if (get_main_camera_object()==object_counter)
	  {
	    if (get_object_targetted(object_counter)!=it)
		{
	     set_targetted_for_object(object_counter /* targetted */ ,it /* object */);
	    }
	  }
	  
	  
	  //OK, lets set this object as the one we are docking
	  (*ocb_ptr).object_list[it].Dyn_OCB_control_data.docking_what=object_counter;
	  
	  //Set up docking state machine for this mothership to phase 1
	  (*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_1=1;
	  
	  //Set looped tractor sound going
	  (*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_2=eclipse_play_looped_sound((SndListHandle)get_sound_handle(tractor_beam_loop),it);
       eclipse_set_looped_chan_volume ((*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_2, 100 /* percent */);
	  
		#if ZEX_PROJECT_BUILDER==1 && MS_DOCKING_LOG //can only printf to OSX syscon
		fprintf (stderr, "Mothership %i is docking object %i\n",it, object_counter); 
        #endif

	  return;
	 }
   }
   
   
   active_object_counter++;
  } //end of if in_use
 
object_counter++;

//¥ do next object
}	//end of while


}


// ------------------------------------------------
// Actual dock mechanism
//cu1 is phase.
//cu2 is sound_ref (tractor sound)
//cu4 is item being launched
//cu6_float is launch timer

// Has to dock the object. 
//We know the object is below us
// 1. Get the object well below motherships y - lets say 200 metres
// 2. Align x and z to motherships x
// 3. Draw object up y
//We use the following control variables
// cu1 is current state, cu2 is eclipse sound ref, cu9 is launch timer

void MS_handle_dock(int it)
{
ZObject * MS_object_ptr;
ZObject * Docking_object_ptr;
double ms_x, ms_y, ms_z;
double do_x, do_y, do_z;
double distance_to_mship,vel;

extern	DynObjectsFixedSize *ocb_ptr;
int docking_what;

MS_object_ptr=&ocb_ptr->object_list[it]; //ptr to mothership
ms_x=MS_object_ptr->Oworldx;
ms_y=MS_object_ptr->Oworldy;
ms_z=MS_object_ptr->Oworldz;

docking_what=MS_object_ptr->Dyn_OCB_control_data.docking_what;
Docking_object_ptr=&ocb_ptr->object_list[docking_what]; //ptr to object being docked


//check if object being docked has canceled dock request or is dead!

if (Docking_object_ptr->Dyn_OCB_control_data.request_dock==0)
{
		 #if ZEX_PROJECT_BUILDER==1 && MS_DOCKING_LOG //can only printf to OSX syscon
		fprintf (stderr, "Object %i has cancelled dock request with Mothership %i\n",docking_what,it); 
        #endif
  Docking_object_ptr->Dyn_OCB_control_data.command_autopilot_engaged=0; 
  MS_object_ptr->Dyn_OCB_control_data.docking_what=-1;
  eclipse_kill_looped_sound(MS_object_ptr->Dyn_OCB_control_data.control_use_2); //Sound ref
  return;
}


if (Docking_object_ptr->in_use==0) //it's deaded
{
		 #if ZEX_PROJECT_BUILDER==1 && MS_DOCKING_LOG //can only printf to OSX syscon
		fprintf (stderr, "Object %i that was being docked by Mothership %i is dead.\n",docking_what,it); 
        #endif
  Docking_object_ptr->Dyn_OCB_control_data.command_autopilot_engaged=0; 
  MS_object_ptr->Dyn_OCB_control_data.docking_what=-1;
  eclipse_kill_looped_sound(MS_object_ptr->Dyn_OCB_control_data.control_use_2); //Sound ref
  return;
}



//Set AP to steer mode for object being docked
Docking_object_ptr->Dyn_OCB_control_data.command_autopilot_engaged=AP_STEER;



//Stop the object dead!
Docking_object_ptr->abs_velocity_vector.x=0;
Docking_object_ptr->abs_velocity_vector.y=0;
Docking_object_ptr->abs_velocity_vector.z=0;


do_x=Docking_object_ptr->Oworldx;
do_y=Docking_object_ptr->Oworldy;
do_z=Docking_object_ptr->Oworldz;

distance_to_mship=get_distance_to_from_in_m(it, docking_what);

if (distance_to_mship>300) vel=40/ZEX_UNIT_SIZE_IN_METERS;
else vel=14/ZEX_UNIT_SIZE_IN_METERS;

//state machine to control the dock
switch (MS_object_ptr->Dyn_OCB_control_data.control_use_1)
{
float dist_y,delta;


 case 1:	//get 200m - 180 m below
       {
	    dist_y=ms_y-do_y;
		
		
 	    if (dist_y>200/ZEX_UNIT_SIZE_IN_METERS)
	    {
		float max_delta=ms_y-do_y;
		delta=20/ZEX_UNIT_SIZE_IN_METERS*time_in_secs; if (delta>max_delta) delta=max_delta;

	     Docking_object_ptr->Oworldy+=delta;  
	    }
	   else
 	    if (dist_y<180/ZEX_UNIT_SIZE_IN_METERS)
	    {
  		 delta=20/ZEX_UNIT_SIZE_IN_METERS*time_in_secs; if (delta>100) delta=100;
	     Docking_object_ptr->Oworldy-=delta;  
	    }
	   else
	   {
 	    MS_object_ptr->Dyn_OCB_control_data.control_use_1++;
		 #if ZEX_PROJECT_BUILDER==1  && MS_DOCKING_LOG //can only printf to OSX syscon
		fprintf (stderr, "Docking state machine for MS %i changing value to %i\n",it,MS_object_ptr->Dyn_OCB_control_data.control_use_1); 
        #endif
	   }
	   
	   break;
	   }


 case 2:	//align x and z
       {
	   int align_x_flag=0;
       int align_z_flag=0;
	   
	   
	   //align x
 	    if (ms_x-do_x>150)
	    {
		 float max_delta=ms_x-do_x;
		 delta=vel*time_in_secs; if (delta>max_delta) delta=max_delta;
	     Docking_object_ptr->Oworldx+=delta;  
	    }
	   else
 	    if (do_x-ms_x>150)
	    {
		 float max_delta=do_x-ms_x;
		 delta=vel*time_in_secs; if (delta>max_delta) delta=max_delta;
	     Docking_object_ptr->Oworldx-=delta;  
	    }
	   else
 	   {
         align_x_flag=1;
	   }


//align z
 	    if (ms_z-do_z>150)
	    {
		 float max_delta=ms_z-do_z;
		 delta=vel*time_in_secs; if (delta>max_delta) delta=max_delta;
	     Docking_object_ptr->Oworldz+=delta;  
	    }
	   else
 	    if (do_z-ms_z>150)
	    {
		 float max_delta=do_z-ms_z;
		 delta=vel*time_in_secs; if (delta>max_delta) delta=max_delta;
	     Docking_object_ptr->Oworldz-=delta;  
	    }
	   else
	   {
	    align_z_flag=1;
	   }
	   
	   
	   if (align_x_flag&align_z_flag)
	   {
		 MS_object_ptr->Dyn_OCB_control_data.control_use_1++;
		 #if ZEX_PROJECT_BUILDER==1  && MS_DOCKING_LOG //can only printf to OSX syscon
		fprintf (stderr, "Docking state machine for MS %i changing value to %i\n",it,MS_object_ptr->Dyn_OCB_control_data.control_use_1); 
        #endif
		 
 	     if (docking_what==get_main_camera_object())
	     {	 
	      add_to_text_display("OBC>  Aligned for docking. Proceeding...",DLP_GREEN);
	     } 
       
	   }	//end of aligned
	   
	   break;
	   }


 case 3:	//now dock
       {
	    dist_y=ms_y-do_y;
 	    if (dist_y>30/ZEX_UNIT_SIZE_IN_METERS)	//within 30 metres will count as docked
	    {
		float max_delta=ms_y-do_y;
		float delta=(5/ZEX_UNIT_SIZE_IN_METERS*time_in_secs);
		if(delta>max_delta) delta=max_delta;
		
	     Docking_object_ptr->Oworldy+=delta;  
	    }
	   else
	   {
 	    MS_object_ptr->Dyn_OCB_control_data.control_use_1++;
		 #if ZEX_PROJECT_BUILDER==1 && MS_DOCKING_LOG //can only printf to OSX syscon
		fprintf (stderr, "Object %i docked\n", docking_what); 
        #endif
		
		if(docking_what==get_main_camera_object())
		{
		 player_has_docked_at(it); //main loop picks up on this and runs docked
	    }
		else
		{
		 if (add_to_kill_list(docking_what)!=0)
		   {
			   MyDebugStr(__LINE__, __FILE__,"add_to_kill_list failed!");
		   }
			 
	    }
	   }
	   
	   break;
	   }

 default: 
       {
		  #if ZEX_PROJECT_BUILDER==1 && PB_PRINT_TO_LOG_ENABLED //can only printf to OSX syscon
          fprintf (stderr, "***WARNING*** MS_handle_dock for MS %i hit default in switch. case = %i\n",it, MS_object_ptr->Dyn_OCB_control_data.control_use_1); 
          #endif
	   break;
	   }

}


}








typedef struct {
int object;
double distance;
} mothership_dist_calc;



//Finds a mothership that is at least nearest_distance_in_m away - used by npc's when trading
int find_mothership(int it, float nearest_distance_in_m, int allegiance, int nearest)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int dynamic_object_index;
register ZObject * current_object_ptr;
int object_counter, active_object_counter;
int ms_distance_index=0;
double nearest_ms=1e50;	//big number
int nearest_ms_slot=-1, n;
mothership_dist_calc mothership_distances[10]; //so we can debug this

 //allegiance can be 0=earth, 1=alien, 2=neutral, -1=don't care
 //returns mothership slot or -1
 active_object_counter=2;
 object_counter=2;	//0 is zex, 1 is vv
 while (active_object_counter<dynamic_object_index)
 {
    current_object_ptr=&ocb_ptr->object_list[object_counter];

    if (current_object_ptr->in_use==1)	//zex or vv
    {
      if (current_object_ptr->Dyn_OCB_control_data.controller==MOTHERSHIP_CONTROL)
	  {
	  
	  double distance_to_MS;
	   //found a mothership, is it far enough away
	   distance_to_MS=get_distance_to_from_in_m(object_counter,it);
	  
	    if (distance_to_MS > nearest_distance_in_m)
	    
	    {
	    //right allegiance?
		
		
		if ((current_object_ptr->Dyn_OCB_control_data.allegiance==NEUTRAL) ||
			(allegiance==NEUTRAL)) //Mothership allegiance==NEUTRAL or callers allegiance neutral?
		{
		 {
		  if (ms_distance_index<10)
		  {
		   mothership_distances[ms_distance_index].object=object_counter;
		   mothership_distances[ms_distance_index].distance=distance_to_MS;
		  
		   ms_distance_index++;
		  }
		 } 
		}
		else //otherwise is the allegiance the wanted one?
		if (current_object_ptr->Dyn_OCB_control_data.allegiance==allegiance)
		 {
		  if (ms_distance_index<10)
		  {
		   mothership_distances[ms_distance_index].object=object_counter;
		   mothership_distances[ms_distance_index].distance=distance_to_MS;
		  
		   ms_distance_index++;
		  }
		 } 		 
	   
	   }
	   
	  }
	  
      active_object_counter+=1;
	} //end of if in_use
 
object_counter++;

//¥ do next object
}	//end of while

//now search for nearest MS
if (ms_distance_index==0) return -1; //didn't find any MS's

if (nearest!=NOT_NEAREST)
{
  for (n=0;n<ms_distance_index;n++)
  {
   if (mothership_distances[n].distance<nearest_ms)
   {
    nearest_ms=mothership_distances[n].distance;
    nearest_ms_slot=mothership_distances[n].object;
   }

  }
return nearest_ms_slot;
}
else //pick one at random
{
 return mothership_distances[NDRangedRdm(0,ms_distance_index)].object;
}


}
