/*
 *  behaviours.c
 *  zex22
 *
 *  Created by Stuart Ball on Thu Apr 11 2002.
 *  Copyright (c) 2002 Lightsoft. All rights reserved.
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
  #include "Error_handler.h"
  #include "processor.h"
  #include "zex_misc.h"
#endif //pch

#include	"autopilot.h"
#include    "physics.h"
#include "object_control_top.h"
#include "npc.h"
#include "behaviours.h"
#include "dlp.h"
#include "camera_control.h"


static char locked_str[]="OBC> Detecting lock-on from XXXXX  ";
#define LOCKED_CALLSIGN_OFFSET 28


void behaviour_aggressive(int the_object)
{
extern	DynObjectsFixedSize *ocb_ptr;
ZObject * object_ptr=&ocb_ptr->object_list[the_object];


 switch (  object_ptr->Dyn_OCB_control_data.control_use_1)
 {

 case 0: //find target
       {
	int allegiance=NEUTRAL, target;   
        //find a target
        if(object_ptr->Dyn_OCB_control_data.allegiance==ALIEN)
        { allegiance=HUMAN; }
        else
        if(object_ptr->Dyn_OCB_control_data.allegiance==HUMAN)
        { allegiance=ALIEN; }
        else
        { 
         report_error ("Behaviour_aggressive: Bad allegiance","\p",-1);
        }
        
        target=find_nearest_object_of_allegiance(the_object, allegiance);
        if (target!=-1)
        {
          //found a target. Is it close enough?
          if (get_distance_to_from_in_m(the_object,target)<30000)
          {
             object_ptr->Dyn_OCB_control_data.control_use_1++;
             object_ptr->object_targetted=target; 
             #if ZEX_PROJECT_BUILDER==1  && BEHAVIOUR_AGGRESSIVE_LOG //can only printf to OSX syscon
             fprintf (stderr, "behaviour_aggressive: Object %i targetted object %i\n",the_object, target); 
             #endif
	     if (target==get_main_camera_object())
	     {
	        //Print Detecting scanning from CALLSIGN
	        copystr (locked_str+LOCKED_CALLSIGN_OFFSET,(*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.callsign); 
	        add_to_text_display(locked_str,DLP_RED);
	     }
          }
        }
        else //couldn't find a target of suitable allegiance
        {
        }

        break;
        }

 case 1:
       {
        double distance=get_distance_to_from_in_m(the_object,object_ptr->object_targetted);
        object_ptr->Dyn_OCB_control_data.command_autopilot_engaged=AP_TRANSPORT;
        //if we're more than 15KM's away go to combat else transport
       // object_ptr->Dyn_OCB_control_data.command_autopilot_engaged=AP_COMBAT;
       // if (distance<15000)
       // {
       //  object_ptr->Dyn_OCB_control_data.command_autopilot_engaged=AP_TRANSPORT;
	//}
        
        //is target alive?
        if (get_object_in_use(object_ptr->object_targetted))   
	{
          //are we aligned?
          if (is_AP_aligned(the_object))
          {
           //we can fire
           object_ptr->Dyn_OCB_control_data.control_use_1_float+=time_in_secs;
           if (object_ptr->Dyn_OCB_control_data.control_use_1_float>0.8)
           { 
             
             if(distance<4000) //close enough for guns?
             {
              object_ptr->Dyn_OCB_control_data.control_use_1_float=0.0;
              NPC_fire_cannon(the_object);
             }

           }
          }
        }
        else
        {
         object_ptr->Dyn_OCB_control_data.control_use_1=0; //reset
        }
        
        
        break;
       }


 }


}



void behaviour_guard(int the_object)
{
extern	DynObjectsFixedSize *ocb_ptr;
ZObject * object_ptr=&ocb_ptr->object_list[the_object];


 switch (  object_ptr->Dyn_OCB_control_data.control_use_1)
 {

 case 0: //find target
       {
	int allegiance=NEUTRAL, target;   
        //find a target
        if(object_ptr->Dyn_OCB_control_data.allegiance==ALIEN)
        { allegiance=HUMAN; }
        else
        if(object_ptr->Dyn_OCB_control_data.allegiance==HUMAN)
        { allegiance=ALIEN; }
        else
        { 
         report_error ("Behaviour_aggressive: Bad allegiance","\p",-1);
        }
        
        target=find_nearest_object_of_allegiance(the_object, allegiance);
        if (target!=-1)
        {
          //found a target. Is it close enough?
          if (get_distance_to_from_in_m(the_object,target)<30000)
          {
             object_ptr->Dyn_OCB_control_data.control_use_1++;
             object_ptr->object_targetted=target; 
             #if ZEX_PROJECT_BUILDER==1  && BEHAVIOUR_AGGRESSIVE_LOG //can only printf to OSX syscon
             fprintf (stderr, "behaviour_aggressive: Object %i targetted object %i\n",the_object, target); 
             #endif
	     if (target==get_main_camera_object())
	     {
	        //Print Detecting scanning from CALLSIGN
	        copystr (locked_str+LOCKED_CALLSIGN_OFFSET,(*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.callsign); 
	        add_to_text_display(locked_str,DLP_RED);
	     }
          }
        }
        else //couldn't find a target of suitable allegiance
        {
        }

        break;
        }

 case 1:
       {
        double distance=get_distance_to_from_in_m(the_object,object_ptr->object_targetted);
        object_ptr->Dyn_OCB_control_data.command_autopilot_engaged=AP_STEER;
        //if we're more than 15KM's away go to combat else transport
       // object_ptr->Dyn_OCB_control_data.command_autopilot_engaged=AP_COMBAT;
       // if (distance<15000)
       // {
       //  object_ptr->Dyn_OCB_control_data.command_autopilot_engaged=AP_TRANSPORT;
	//}
        
        //is target alive?
        if (get_object_in_use(object_ptr->object_targetted))   
	{
          //are we aligned?
          if (is_AP_aligned(the_object))
          {
           //we can fire
           object_ptr->Dyn_OCB_control_data.control_use_1_float+=time_in_secs;
           if (object_ptr->Dyn_OCB_control_data.control_use_1_float>1.5)
           { 
             
             if(distance<4000) //close enough for guns?
             {
              object_ptr->Dyn_OCB_control_data.control_use_1_float=0.0;
              NPC_fire_cannon(the_object);
             }

           }
          }
        }
        else
        {
         object_ptr->Dyn_OCB_control_data.control_use_1=0; //reset
        }
        
        
        break;
       }


 }


}

//Used to dock an object

void behaviour_dock(int the_object)
{
extern	DynObjectsFixedSize *ocb_ptr;
ZObject * object_ptr=&ocb_ptr->object_list[the_object]; //ptr to mothership;

//state machine to control the dock
switch (  object_ptr->Dyn_OCB_control_data.control_use_1)
{

 case 0:
       {
	   
        //Get close enough to mship for dock
       if (get_distance_to_from(object_ptr->object_targetted,the_object)<500000)
	   {
	     object_ptr->Dyn_OCB_control_data.control_use_1++;
             #if ZEX_PROJECT_BUILDER==1  && MS_DOCKING_LOG //can only printf to OSX syscon
             fprintf (stderr, "behaviour_dock: Object %i within dock distance\n",the_object); 
             #endif
	   }
	   else	//apply main thruster
	   {
	     object_ptr->Dyn_OCB_control_data.command_autopilot_engaged=AP_STEER;
         object_ptr->Dyn_OCB_control_data.ORCS_thrust_down=0;
		 object_ptr->Dyn_OCB_control_data.ORCS_thrust_up=0.1;	//manouevering thrust
	   }
	   break;
	   }

 case 1:
       {
	   
        //Now stop
		 object_ptr->Dyn_OCB_control_data.command_ORCS_damper=ORCS_DAMPER_ROT+ORCS_DAMPER_LIN;
         object_ptr->Dyn_OCB_control_data.ORCS_thrust_down=0;
		 object_ptr->Dyn_OCB_control_data.ORCS_thrust_up=0;	//thrust off
		 
		 #if ZEX_PROJECT_BUILDER==1  && MS_DOCKING_LOG //can only printf to OSX syscon
		 fprintf (stderr, "behaviour_dock: Object %i stopping. \n",the_object); 
         #endif
	     object_ptr->Dyn_OCB_control_data.control_use_1++;
		 
	   break;
	   }

 case 2:
       {
// Wait for stop	   
        if (get_ND_velocity_in_mps(the_object)<50.0)
	    {
	     object_ptr->Dyn_OCB_control_data.control_use_1++;
		 
		 #if ZEX_PROJECT_BUILDER==1  && MS_DOCKING_LOG //can only printf to OSX syscon
		 fprintf (stderr, "behaviour_dock: Object %i ready to dock. Moving below mothership\n",the_object); 
         #endif
	    }
	    break;
	   }


 case 3:
       {
	   vector raw_position,mship_position;
// Get below mothership	   
        get_raw_position(the_object,&raw_position);
        get_raw_position(object_ptr->object_targetted,&mship_position);
		
        if (raw_position.y>mship_position.y)
	    {
	     object_ptr->Oworldy-=(40/ZEX_UNIT_SIZE_IN_METERS)*time_in_secs;	//40ms
	    }
        else
		{
	     object_ptr->Dyn_OCB_control_data.control_use_1++;
		 
		 #if ZEX_PROJECT_BUILDER==1  && MS_DOCKING_LOG //can only printf to OSX syscon
		 fprintf (stderr, "behaviour_dock: Object %i requesting dock. behaviour_dock: idling.\n",the_object); 
         #endif
		}
	    break;
	   }


 case 4:
       {
         object_ptr->Dyn_OCB_control_data.request_dock=1;
	     disengage_AP(object_ptr->object_targetted);
	   //We will get killed by the mothership when dock complete
	   break;
	   }


 default: 
       {
		  #if ZEX_PROJECT_BUILDER==1 && MS_DOCKING_LOG //can only printf to OSX syscon
          fprintf (stderr, "***WARNING*** behaviour_dock. Object %i hit default in switch. value=%i\n",the_object,object_ptr->Dyn_OCB_control_data.control_use_1); 
          #endif
	   break;
	   }

}


}