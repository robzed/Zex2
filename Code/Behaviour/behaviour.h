/*
 *  behaviour.h
 *  zex22
 *
 *  Created by Stuart Ball on Sun Apr 07 2002.
 *  Copyright (c) 2002 Lightsoft. All rights reserved.
 *
 */

enum { //behaviour types
	   NULL_BEHAVIOUR,
	   DOCK_BEHAVIOUR,
	   DEFENSIVE_BEHAVIOUR,
	   AGGRESIVE_BEHAVIOUR,
           GUARD_BEHAVIOUR,
	   LOW_FUEL_BEHAVIOUR,
     };
	 
enum { //Character classes  
	   NPC_CLASS_FIGHTER, 
	   NPC_CLASS_TRADER, 
	   NPC_CLASS_GUARD, 
	   NPC_CLASS_PIRATE, 
           
// this one is what they start as. NPC_control will throw an error if it doesn't change from this.
           NPC_CLASS_UNINTIALISED,
	};
	

#define LAST_BEHAVIOUR LOW_FUEL_BEHAVIOUR //for error checking in set_behaviour

#define BEHAVIOUR_DEBUG 1 //Param checking on for behaviour

bool dispatch_behaviour(int the_object);
void behaviour_dock(int the_object);
void set_behaviour (int the_object, int behaviour_type);

//Behaviour state changes 
void push_behaviour (int the_object, int behaviour_type);
void pop_behaviour (int the_object);
int get_current_behaviour (int the_object);
