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
 *  NPC.h
 *  zex22
 *
 *  Created by Stuart Ball on Fri Mar 15 2002.
 *  Copyright (c) 2002 Lightsoft (Games). All rights reserved.
 *
 */

#define AI_BUBBLE_IN_KM 100000.0        //HIgh res AI bubble radius in kilmetres

void launch_NPC(int MS, int object_ref, int NPC_class, int allegiance);
void control_NPC(int the_object);
double get_distance_to_nearest_NPC_in_m(int it, float nearest_distance_in_m);
void NPC_task(void);
int find_nearest_object_of_allegiance(int object, int allegiance);
void NPC_fire_cannon(int the_object);

void insert_exhaust_animation(int the_object);



