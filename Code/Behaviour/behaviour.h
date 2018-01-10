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
