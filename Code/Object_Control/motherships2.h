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

