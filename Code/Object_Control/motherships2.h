/*
 *  motherships2.h
 *  zex22
 *
 *  Created by stu on Sun Dec 02 2001.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
 */
#define NEAREST 1
#define NOT_NEAREST 0

void handle_mothership(int it);
void MS_handle_dock(int it);
int find_mothership(int it, float nearest_distance_in_m, int allegiance, int nearest); //If nearest!=0 then get nearest else any of up to 10

