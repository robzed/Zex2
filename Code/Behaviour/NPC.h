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



