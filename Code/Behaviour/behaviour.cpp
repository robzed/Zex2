/*
 *  behaviour.c
 *  zex22
 *
 *  Created by Stuart Ball on Sun Apr 07 2002.
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
#endif

#include	"autopilot.h"
#include    "physics.h"
#include "object_control_top.h"

#include "behaviour.h"
#include "behaviours.h"

/*
BEHAVIOURS - SB Apr 2002
------------------------

Each object in Zex has a behaviour stack (hereinafter refered to the BSTACK). The NPC Controller
dispatches control as per the current behaviour type found in Dyn_OCB_control_data.behaviour
to the current behaviour controller. 

Controllers have at their disposal 10 control variables each 32 bits in length. There are 
6 integer variables and 4 float32's - these are labeled control_use_1 through 6 and 
control_use_1_float through 4_float

Control in Zex is through small control modules called behaviours. When a behaviour wants
to change behaviour it calls:
 
push_behaviour and passes the object and the new behaviour type.

push_behaviour stores the current control variables and the current behaviour on the objects' BSTACK, 
updates the BSTACK pointer, initialises the control variables to 0 and sets the 
Dyn_OCB_control_data.behaviour member to the new behaviour.

When a behaviour has terminated it calls 

pop_behaviour which:
pops the new behaviour then the 10 control variables and updates the BSTACK pointer.

An ancillary call is provided - get_behaviour which returns an int containing the current behaviour as
defined in behaviour.h


BEHAVIOUR STACK FRAME
---------------------
0:  pushed behaviour
4:  cu1
8:  cu2
12: cu3
16: cu4
20: cu5
24: cu6
28: cu1f
32: cu2f
36: cu3f
40: cu4f
Total size=44 bytes


BSTACK_SIZE (3d_structures_def.h) defines how many complete frames we can store on the stack - 15 by
default.

ERROR HANDLING.
---------------
If push_behaviour has run out of stack space then an error is logged to stdlog. The object is then
added to the kill list and killed.

If popping a behaviour would result in a stack underflow then the pop is not carried out. An error
is logged but the object is NOT killed.


TYPICAL BEHAVIOURS
------------------

	   NULL_BEHAVIOUR,
	   DOCK_BEHAVIOUR,
	   DEFENSIVE_BEHAVIOUR,
	   AGGRESIVE_BEHAVIOUR,
           GUARD_BEHAVIOUR,
	   LOW_FUEL_BEHAVIOUR,
	   
*/


int get_behaviour (int the_object)
{
extern	DynObjectsFixedSize *ocb_ptr;

 return ocb_ptr->object_list[the_object].Dyn_OCB_control_data.behaviour; 
}


//For a given object set its behaviour - sets cu1 to 0
//Should never be called from a behaviour - top level controller use only...

void set_behaviour (int the_object, int behaviour_type)
{
extern	DynObjectsFixedSize *ocb_ptr;

#if BEHAVIOUR_DEBUG==1
 if ((behaviour_type<0) || (behaviour_type>LAST_BEHAVIOUR))
 {
   report_error ("set_behaviour: Unknown behaviour type passed in","",behaviour_type);
 }
#endif //BEHAVIOUR_DEBUG

 ocb_ptr->object_list[the_object].Dyn_OCB_control_data.behaviour=behaviour_type;
 ocb_ptr->object_list[the_object].Dyn_OCB_control_data.control_use_1=0;
}


//A fairly expensive routine that pushes a wanted behaviour onto the BSTACK
//Saves current behaviour control variables
//Clears all control variables for the new behaviour
void push_behaviour(int the_object, int behaviour_type)
{
extern	DynObjectsFixedSize *ocb_ptr;
ZObject * object_ptr=&ocb_ptr->object_list[the_object]; //ptr to object
int* current_stack_pos_ptr;

#if BEHAVIOUR_DEBUG==1
 if ((behaviour_type<0) || (behaviour_type>LAST_BEHAVIOUR))
 {
   report_error ("push_behaviour: Unknown behaviour type passed in","",behaviour_type);
 }
#endif //BEHAVIOUR_DEBUG

/* behaviour stack frame:
0:  pushed behaviour
4:  cu1
8:  cu2
12: cu3
16: cu4
20: cu5
24: cu6
28: cu1f
32: cu2f
36: cu3f
40: cu4f
 */

//check we can push
if (object_ptr->Dyn_OCB_control_data.BSTACK_ptr>BSTACK_SIZE*BSTACK_FRAME_SIZE_IN_INTS)
{
 //Fatal control error - object is going to get killed
   #if ZEX_PROJECT_BUILDER==1
   fprintf (stderr, "push_behaviour: BSTACK overflowflow. Object=%i is killed",the_object);
   #endif
   
   if (add_to_kill_list(the_object)!=0)
   {
		 MyDebugStr(__LINE__, __FILE__,"add_to_kill_list failed!");
   }
   return;

}

//do the push
current_stack_pos_ptr=(int*)&object_ptr->Dyn_OCB_control_data.bstack;

current_stack_pos_ptr+=object_ptr->Dyn_OCB_control_data.BSTACK_ptr; //right place on stack to start copying the control variables

*current_stack_pos_ptr=object_ptr->Dyn_OCB_control_data.behaviour; current_stack_pos_ptr++;

*current_stack_pos_ptr=object_ptr->Dyn_OCB_control_data.control_use_1; current_stack_pos_ptr++;
*current_stack_pos_ptr=object_ptr->Dyn_OCB_control_data.control_use_2; current_stack_pos_ptr++;
*current_stack_pos_ptr=object_ptr->Dyn_OCB_control_data.control_use_3; current_stack_pos_ptr++;
*current_stack_pos_ptr=object_ptr->Dyn_OCB_control_data.control_use_4; current_stack_pos_ptr++;
*current_stack_pos_ptr=object_ptr->Dyn_OCB_control_data.control_use_5; current_stack_pos_ptr++;
*current_stack_pos_ptr=object_ptr->Dyn_OCB_control_data.control_use_6; current_stack_pos_ptr++;

*current_stack_pos_ptr=object_ptr->Dyn_OCB_control_data.control_use_1_float; current_stack_pos_ptr++;
*current_stack_pos_ptr=object_ptr->Dyn_OCB_control_data.control_use_2_float; current_stack_pos_ptr++;
*current_stack_pos_ptr=object_ptr->Dyn_OCB_control_data.control_use_3_float; current_stack_pos_ptr++;
*current_stack_pos_ptr=object_ptr->Dyn_OCB_control_data.control_use_4_float; current_stack_pos_ptr++;
object_ptr->Dyn_OCB_control_data.BSTACK_ptr+=11;

//Clear current control vars
object_ptr->Dyn_OCB_control_data.control_use_1=0; object_ptr->Dyn_OCB_control_data.control_use_2=0;
object_ptr->Dyn_OCB_control_data.control_use_3=0; object_ptr->Dyn_OCB_control_data.control_use_4=0;
object_ptr->Dyn_OCB_control_data.control_use_5=0; object_ptr->Dyn_OCB_control_data.control_use_6=0;

object_ptr->Dyn_OCB_control_data.control_use_1_float=0.0; object_ptr->Dyn_OCB_control_data.control_use_2_float=0.0;
object_ptr->Dyn_OCB_control_data.control_use_3_float=0.0; object_ptr->Dyn_OCB_control_data.control_use_4_float=0.0;

}



//Pops the current behaviour and reinstates the control variables
void pop_behaviour(int the_object)
{
extern	DynObjectsFixedSize *ocb_ptr;
ZObject * object_ptr=&ocb_ptr->object_list[the_object]; //ptr to object
int* current_stack_pos_ptr;
//BSTACK_ptr is pointing to next free frame

if (object_ptr->Dyn_OCB_control_data.BSTACK_ptr<11)
{
   #if ZEX_PROJECT_BUILDER==1
   fprintf (stderr, "pop_behaviour: BSTACK underflow. Object=%i",the_object);
   #endif
   return;
}

current_stack_pos_ptr=(int*)&object_ptr->Dyn_OCB_control_data.bstack;

current_stack_pos_ptr+=object_ptr->Dyn_OCB_control_data.BSTACK_ptr; //right place on stack to start copying the control variables

--current_stack_pos_ptr; object_ptr->Dyn_OCB_control_data.control_use_4_float=*current_stack_pos_ptr;
--current_stack_pos_ptr; object_ptr->Dyn_OCB_control_data.control_use_3_float=*current_stack_pos_ptr;
--current_stack_pos_ptr; object_ptr->Dyn_OCB_control_data.control_use_2_float=*current_stack_pos_ptr;
--current_stack_pos_ptr; object_ptr->Dyn_OCB_control_data.control_use_1_float=*current_stack_pos_ptr;

--current_stack_pos_ptr; object_ptr->Dyn_OCB_control_data.control_use_6=*current_stack_pos_ptr;
--current_stack_pos_ptr; object_ptr->Dyn_OCB_control_data.control_use_5=*current_stack_pos_ptr;
--current_stack_pos_ptr; object_ptr->Dyn_OCB_control_data.control_use_4=*current_stack_pos_ptr;
--current_stack_pos_ptr; object_ptr->Dyn_OCB_control_data.control_use_3=*current_stack_pos_ptr;
--current_stack_pos_ptr; object_ptr->Dyn_OCB_control_data.control_use_2=*current_stack_pos_ptr;
--current_stack_pos_ptr; object_ptr->Dyn_OCB_control_data.control_use_1=*current_stack_pos_ptr;
--current_stack_pos_ptr; object_ptr->Dyn_OCB_control_data.behaviour=*current_stack_pos_ptr;

object_ptr->Dyn_OCB_control_data.BSTACK_ptr-=11;

}



//DO not call this function - internal to module behaviour
bool dispatch_behaviour(int the_object)
{
extern	DynObjectsFixedSize *ocb_ptr;
ZObject * object_ptr=&ocb_ptr->object_list[the_object]; //ptr to mothership;

 switch (  object_ptr->Dyn_OCB_control_data.behaviour)
 {

  case DOCK_BEHAVIOUR:
	  behaviour_dock(the_object); //object needs to have targetted a mothership
	  return TRUE;

  case AGGRESIVE_BEHAVIOUR:
	  behaviour_aggressive(the_object);
	  return TRUE;

  case GUARD_BEHAVIOUR:
	  behaviour_guard(the_object);
	  return TRUE;
	   
  default:
       return FALSE;
 }


}




