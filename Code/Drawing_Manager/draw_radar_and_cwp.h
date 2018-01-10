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

// ****************************
// Name:    		CPanel.C
// Description:		Controls Control Panel Updates
// 
// Author:			Stuart Ball
//
// ****************************

void display_control_panel();
void update_radar();
void run_cwp(void);

#define RADAR_RANGE_INIT  32e9 //  16000 KK's
#define RADAR_RANGE_SHORTEST  1e5 //  1 K's
#define RADAR_RANGE_MULTIPLIER 2 // 3.16227766	  // mulitply by this every press
#define RADAR_RANGE_LONGEST   4.294967296e14 // 5000 million km's (approx pluto orbit)

extern int radar_grat;
extern int do_throb;	//this is changed to 1 if aliens detected (Radar graticule)


typedef struct 
{
_3D the_vect;
int the_type;	//R'mparian, human, neutral
int is_target;	// temporary whilst target is zex centric
int Oid;        //object ID
int object_slot;
} radar_entry;

typedef struct {
int x1;
int y1;
int y2;
int the_colour;
} radar_display_entry;

