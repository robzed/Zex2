/*
 *  NPC.c
 *  zex22
 *
 *  Created by Stuart Ball on Fri Mar 15 2002.
 *  Copyright (c) 2002 Lightsoft (Games). All rights reserved.
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
  #include	"processor.h"
  #include	"access_ocbs.h"
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
#include "objects_controls.h"
#include "pobs.h"
#include "NPC.h"
#include "main_sim.h"
#include "behaviour.h"

static bool check_out_of_bubble(int the_object);
static int handle_survival(int the_object);
static void handle_trader_control(int the_object);
static void handle_fighter_control(int the_object);
void fire_lasers(int the_object);
void insert_exhaust_animation(int the_object);

static float NPC_kill_timer=0.0;

//Launch an NPC from a parent - a mothership
void launch_NPC(int MS, int object_ref, int NPC_class, int allegiance)
{
load_dyn_object_pb the_params;
int ship_slot, dyn_slot,n,m;
ZObject * MS_object_ptr;
extern	DynObjectsFixedSize *ocb_ptr;

 MS_object_ptr=&ocb_ptr->object_list[MS];
 
 if (object_ref==SHUTTLE)
 {
//launch a shuttle
   ship_slot=get_static_object_slot('SHT1');
   if (ship_slot==-1) report_error ("launch_NPC: Couldn't find static SHT1","",-1);
   dyn_slot=find_vacant_dynamic_object_slot();
   if (dyn_slot!=-1)
   {
  //set up ship
	clear_pb(&the_params);
	the_params.world_x = MS_object_ptr->Oworldx;
	the_params.world_y = MS_object_ptr->Oworldy-1000;
	the_params.world_z = MS_object_ptr->Oworldz;
	the_params.rot_z=270;
	the_params.controller_ref=NPC_CONTROL; //ALL NPC's should have NPC_Control as controller_ref
	the_params.NPC_class=NPC_class;
	
	the_params.shield_value=100;
	the_params.shields_class=4;
	the_params.mass=1;	//basic ship weighs 1 tonne
	
	the_params.object_category=allegiance;
	 the_params.laser_cat=1;
	 the_params.cannon_cat=1;
	 the_params.missile_cat=1;
	 the_params.number_of_missiles=3;
	 
	load_dyn_object(ship_slot,dyn_slot,&the_params,get_new_galactic_id(),30,-1,!DUST_BIT);	  //the object, the position (-1=next free)
	
//Set up cargo and update mass
     m=NDRangedRdm(0,(*ocb_ptr).object_list[dyn_slot].Dyn_OCB_control_data.cargo_capacity_tonnes);
	 for (n=0;n<m;n++)
	 {
	  (*ocb_ptr).object_list[dyn_slot].Dyn_OCB_control_data.cargo_bay[n]=NDRangedRdm(1,10);
	 }
	 
	 update_mass(dyn_slot);
	  	
     (*ocb_ptr).object_list[dyn_slot].abs_velocity_vector.y=-1800;
	 
//set up player orientation to up
	 reset_rotation_for_object(dyn_slot);
	 set_rotation_around_world_x(dyn_slot,90);
	 #if ZEX_PROJECT_BUILDER==1 && MS_LAUNCH_LOG==1
	 #if RELEASE==0 //Stuff we dont want to show in the log but need to debug
          fprintf(stderr, "****\n");
          fprintf(stderr, "Mothership %d launched Shuttle %d.\n",MS,dyn_slot);
	 #endif
	 #endif
	
    }
	else
	{
	 #if ZEX_PROJECT_BUILDER==1
      fprintf(stderr, "Couldn't find vacant dyn slot in launch_NPC!\n");
	 #endif
	}
} //end of type==1
else if (object_ref==FALCON)
{
//launch a falcon
   ship_slot=get_static_object_slot('FALC');
   if (ship_slot==-1) report_error ("launch_NPC: Couldn't find static FALC","",-1);
   dyn_slot=find_vacant_dynamic_object_slot();
   if (dyn_slot!=-1)
   {
  //set up ship
	clear_pb(&the_params);
	the_params.world_x = MS_object_ptr->Oworldx;
	the_params.world_y = MS_object_ptr->Oworldy-1000;
	the_params.world_z = MS_object_ptr->Oworldz;
	the_params.rot_z=270;
	the_params.controller_ref=NPC_CONTROL; //ALL NPC's should have NPC_Control as controller_ref
	the_params.NPC_class=NPC_class;
	
	the_params.shield_value=100;
	the_params.shields_class=5;
	the_params.mass=1;	//basic ship weighs 1 tonne
	
	the_params.object_category=allegiance;
	 the_params.laser_cat=1;
	 the_params.cannon_cat=1;
	 the_params.missile_cat=1;
	 the_params.number_of_missiles=3;
	 
	load_dyn_object(ship_slot,dyn_slot,&the_params,get_new_galactic_id(),5,-1,!DUST_BIT);	  //the object, the position (-1=next free)
	
//Set up cargo and update mass
     m=NDRangedRdm(0,(*ocb_ptr).object_list[dyn_slot].Dyn_OCB_control_data.cargo_capacity_tonnes);
	 for (n=0;n<m;n++)
	 {
	  (*ocb_ptr).object_list[dyn_slot].Dyn_OCB_control_data.cargo_bay[n]=NDRangedRdm(1,10);
	 }
	 
	 update_mass(dyn_slot);
	  	
     (*ocb_ptr).object_list[dyn_slot].abs_velocity_vector.y=-2800;
	 
//set up player orientation to up
	 reset_rotation_for_object(dyn_slot);
	 set_rotation_around_world_x(dyn_slot,90);
	 #if ZEX_PROJECT_BUILDER==1 && MS_LAUNCH_LOG==1
	 #if RELEASE==0 //Stuff we dont want to show in the log but need to debug
          fprintf(stderr, "****\n");
          fprintf(stderr, "Mothership %d launched Falcon %d.\n",MS,dyn_slot);
	 #endif
	 #endif
	
    }
	else
	{
	 #if ZEX_PROJECT_BUILDER==1
      fprintf(stderr, "Couldn't find vacant dyn slot in launch_NPC!\n");
	 #endif
	}
}
else
{
 report_error ("launch_NPC: Unknown controller_ref passed in","",object_ref);
}

}

//from object control
//Objects use cu1 as their state
//THIS IS TOP LEVEL OF behaviour
void control_NPC(int the_object)
{
extern	DynObjectsFixedSize *ocb_ptr;
ZObject * object_ptr=&ocb_ptr->object_list[the_object]; //ptr to mothership;

    if( check_out_of_bubble (the_object))
    {
      return;
    }
  
  if (handle_survival(the_object)) return;
  //get class
  
  if (object_ptr->Dyn_OCB_control_data.NPC_class==NPC_CLASS_TRADER) 
  {
   handle_trader_control(the_object);
   return;
  }

  if (object_ptr->Dyn_OCB_control_data.NPC_class==NPC_CLASS_FIGHTER) 
  {
   handle_fighter_control(the_object);
   return;
  }
      
 report_error ("control_NPC: Unknown NPC_class","",-1);

}



static void handle_trader_control(int the_object)
{
extern	DynObjectsFixedSize *ocb_ptr;
ZObject * object_ptr=&ocb_ptr->object_list[the_object]; //ptr to mothership;

 insert_exhaust_animation(the_object);
 
 if (dispatch_behaviour(the_object)==FALSE)
 {
 
  switch (  object_ptr->Dyn_OCB_control_data.control_use_1)
  {

  case 0:
       { 
	   int new_mothership;
	   //find a mothership that's more than 5km's away and set AP to it
	    new_mothership=find_mothership(the_object, 5000,object_ptr->Dyn_OCB_control_data.allegiance, NOT_NEAREST);
		if (new_mothership==-1) return;

		
		object_ptr->Dyn_OCB_control_data.command_autopilot_engaged=AP_TRANSPORT;
		object_ptr->object_targetted=new_mothership;
		 #if ZEX_PROJECT_BUILDER==1  && TRADER_LOG //can only printf to OSX syscon
		fprintf (stderr, "Trader %i has set AP for %i\n",the_object,new_mothership); 
                 #endif
	        object_ptr->Dyn_OCB_control_data.control_use_1++;
		break;
		}
		
  case 1: //Wait for AP to disengage
        if (object_ptr->Dyn_OCB_control_data.command_autopilot_engaged==AP_STEER_CUE)
	    {
	      object_ptr->Dyn_OCB_control_data.control_use_1++;
		 
		  #if ZEX_PROJECT_BUILDER==1  && MS_DOCKING_LOG //can only printf to OSX syscon
		  fprintf (stderr, "Object %i setting DOCK_BEHAVIOUR...\n",the_object); 
          #endif
		  set_behaviour(the_object,DOCK_BEHAVIOUR); //Switch to dock
	    }
	    break;
  
  default:
		 #if ZEX_PROJECT_BUILDER==1  && TRADER_LOG //can only printf to OSX syscon
		 fprintf (stderr, "Trader controller hit default for object %i!\n",the_object); 
         #endif
         break;
  }
 		
 }

object_systems_sim(the_object);
}




static void handle_fighter_control(int the_object)
{

 insert_exhaust_animation(the_object);
 
 if (dispatch_behaviour(the_object)==FALSE)
 {
    set_behaviour(the_object,AGGRESIVE_BEHAVIOUR); //Kick start  the fighter ebhaviour in case it wasn't loadedd with aggressive
 }

 object_systems_sim(the_object);
}


static int handle_survival(int the_object)
{
//returns TRUE if we cuased an action else FALSE
//check fuel
//check shields
//check tiredness

return FALSE;
}


static bool check_out_of_bubble(int the_object)
{
_3D camera_pos, object_pos;
int in_bubble=TRUE;

  get_position_in_m(get_main_camera_object(),&camera_pos); // get zex position
  get_position_in_m(the_object,&object_pos); // get zex position
 
  if(fabs(camera_pos.x-object_pos.x) > AI_BUBBLE_IN_KM*1000) in_bubble=FALSE;
  if(fabs(camera_pos.y-object_pos.y) > AI_BUBBLE_IN_KM*1000) in_bubble=FALSE;
  if(fabs(camera_pos.z-object_pos.z) > AI_BUBBLE_IN_KM*1000) in_bubble=FALSE;
 
 if (in_bubble==FALSE)
 {
   
   if (add_to_kill_list(the_object)!=0)
   {
	 {
		 MyDebugStr(__LINE__, __FILE__,"add_to_kill_list failed!");
	 }
	   
   }
   #if ZEX_PROJECT_BUILDER==1 && BUBBLE_LOG==1
   #if RELEASE==0 //Stuff we dont want to show in the log but need to debug
	 fprintf(stderr, "Object %d moved out of AI bubble; removed.\n",the_object);
   #endif
   #endif
   return TRUE;
  
 }
return FALSE;
}



typedef struct {
int object;
double distance;
} npc_dist_calc;

//Finds any NPC that is at least nearest_distance_in_m away
//used, for example, as the eyes of motherships when they want to launch an NPC
double get_distance_to_nearest_NPC_in_m(int it, float nearest_distance_in_m)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int dynamic_object_index;
register ZObject * current_object_ptr;
int object_counter, active_object_counter;
int npc_distance_index=0;
double nearest_npc=1e50;	//big number
int nearest_npc_slot=-1, n;
npc_dist_calc npc_distances[10]; //so we can debug this

 //allegiance can be 0=earth, 1=alien, 2=neutral, -1=don't care
 //returns mothership slot or -1
 active_object_counter=2;
 object_counter=2;	//0 is zex, 1 is vv
 while (active_object_counter<dynamic_object_index)
 {
    current_object_ptr=&ocb_ptr->object_list[object_counter];

    if (current_object_ptr->in_use==1)
    {
      if (current_object_ptr->Dyn_OCB_control_data.controller==NPC_CONTROL)
	  {
	  
	  double distance_to_NPC;
	   //found a mothership, is it far enough away
	   distance_to_NPC=get_distance_to_from_in_m(object_counter,it);
	  
	    if (distance_to_NPC < nearest_distance_in_m)	    
	    {
		  if (npc_distance_index<10)
		  {
		   npc_distances[npc_distance_index].object=object_counter;
		   npc_distances[npc_distance_index].distance=distance_to_NPC;
		  
		   npc_distance_index++;
		  }
	    }
	  }
	  
      active_object_counter+=1;
	} //end of if in_use
 
object_counter++;

//¥ do next object
}	//end of while

//now search for nearest
if (npc_distance_index==0) return nearest_npc; //didn't find any MS's

  for (n=0;n<npc_distance_index;n++)
  {
   if (npc_distances[n].distance<nearest_npc)
   {
        nearest_npc=npc_distances[n].distance;
        nearest_npc_slot=npc_distances[n].object;
   }

  }
return nearest_npc;

}
 //end of get_distance_to_nearest_npc

static char scanning_str[]="OBC> Detecting scan from XXXXX  ";
#define SCAN_CALLSIGN_OFFSET 25

int find_nearest_object_of_allegiance(int object, int allegiance)
{
extern int dynamic_object_index;
extern	DynObjectsFixedSize *ocb_ptr;
int nearest_object;
double  nearest_distance, dist;
register ZObject * current_object_ptr;
int active_object_counter, object_counter;

nearest_distance=nearest_object=-1;

active_object_counter=0;
object_counter=0;


while (active_object_counter<dynamic_object_index)
{
    current_object_ptr=&ocb_ptr->object_list[object_counter];

    if ((current_object_ptr->in_use==1))
    {
     if(current_object_ptr->Dyn_OCB_control_data.allegiance==allegiance)
        {
          if(current_object_ptr->Dyn_OCB_control_data.was_loaded_from_sds!=0) //is a ship!
          {	
        	dist=get_distance_to_from(object,object_counter);
        	if(nearest_distance==-1 || dist<nearest_distance)
        	  {
		  
		    nearest_distance=dist; nearest_object=object_counter;
        	  
		    if (object_counter==get_main_camera_object()) //camera object scanned?
	            {
		      if (dist<3000000) //OBC can detect scanning upto 30KM's away
		      {
	               //Print Detecting scanning from CALLSIGN
	               copystr (scanning_str+SCAN_CALLSIGN_OFFSET,(*ocb_ptr).object_list[object].Dyn_OCB_control_data.callsign); 
	               add_to_text_display(scanning_str,DLP_YELLOW);
	              }
		    }
		  
		  }
          }
	}
     active_object_counter+=1;
    }
    
object_counter++;
}	//end of while
return nearest_object;
} 






//Called once per frame

void NPC_task(void)
{

 NPC_kill_timer+=time_in_secs;
 if (NPC_kill_timer>20.0) NPC_kill_timer=0.0;	//if 0 controllers check for out of AI bubble
}





void fire_lasers(int the_object)
{
extern	DynObjectsFixedSize *ocb_ptr;
ZObject * object_ptr=&ocb_ptr->object_list[the_object]; //ptr to parent ship;


 if (object_ptr->Dyn_OCB_control_data.laser_trigger_slot==-1)
 {
  int dest_ocb;
  load_dyn_object_pb the_params;

  clear_pb(&the_params);
  dest_ocb=find_vacant_dynamic_object_slot();

    if (dest_ocb!=-1)
    {   
        (*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.laser_trigger_slot=dest_ocb;

	the_params.world_x=0;
	the_params.world_y=0;
	the_params.world_z=0;
	the_params.controller_ref=PARTICLE_GENERATOR_1;
	the_params.control_use_1=1;	//0=single laser, 1=twin
	the_params.mass=1;	//basic ship weighs 1 tonne
	the_params.object_category=NO_RADAR;
         the_params.kill_flags|=KILL_METHOD;
         the_params.invisible_flag=1;
        the_params.parent=the_object;	//Zex owns this object
	load_dyn_object(26,dest_ocb,&the_params,-1,1,-1,DUST_BIT);	//the object, the position (-1=next free)
	
		
    }
 }

 
}

void NPC_fire_cannon(int the_object)
{
extern	DynObjectsFixedSize *ocb_ptr;


  int dest_ocb;
  load_dyn_object_pb the_params;

  clear_pb(&the_params);
  dest_ocb=find_vacant_dynamic_object_slot();

    if (dest_ocb!=-1)
    {   
        (*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.cannon_trigger_slot=dest_ocb;

	the_params.world_x=0;
	the_params.world_y=0;
	the_params.world_z=0;
	the_params.controller_ref=PARTICLE_GENERATOR_CANNON;
	the_params.mass=1;	//basic ship weighs 1 tonne
	the_params.object_category=NO_RADAR;
         the_params.invisible_flag=1;
        the_params.parent=the_object;	//Zex owns this object
	load_dyn_object(26,dest_ocb,&the_params,-1,1,-1,DUST_BIT);	//the object, the position (-1=next free)
	
		
    }

 
}



void insert_exhaust_animation(int the_object)
{
extern	DynObjectsFixedSize *ocb_ptr;
ZObject * object_ptr=&ocb_ptr->object_list[the_object]; //ptr to parentship;


 if (object_ptr->Dyn_OCB_control_data.exhaust_trigger_slot==-1)
 {
  int dest_ocb;
  load_dyn_object_pb the_params;

  clear_pb(&the_params);
  dest_ocb=find_vacant_dynamic_object_slot();

    if (dest_ocb!=-1)
    {   
               #if ZEX_PROJECT_BUILDER==1 && SHOW_VIRTUAL_TRIGGERS==1
             fprintf(stderr, "NPC %d is firing engines\n",the_object);
           #endif

        (*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.exhaust_trigger_slot=dest_ocb;

	the_params.world_x=0;
	the_params.world_y=0;
	the_params.world_z=0;
	the_params.controller_ref=PARTICLE_GENERATOR_EXHAUST;
	the_params.control_use_1=1;	//0=single laser, 1=twin
	the_params.mass=1;	//basic ship weighs 1 tonne
	the_params.object_category=NO_RADAR;
         the_params.kill_flags|=KILL_METHOD;
         the_params.invisible_flag=1;
        the_params.parent=the_object;	//who owns this object
        
	load_dyn_object(26,dest_ocb,&the_params,-1,1,-1,DUST_BIT);	//the object, the position (-1=next free)
	
		
    }
 }

 
}