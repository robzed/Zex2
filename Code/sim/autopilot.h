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

#define AP_OFF 0
#define AP_STEER_CUE 1
#define AP_STEER 2
#define AP_TRANSPORT 3
#define AP_COMBAT 4
#define AP_HYPERTRANSPORT 5

#define HYPERTRANSPORT_DELAY 10.0 //seconds before engagement


extern int camera_object_quat_x,camera_object_quat_y,camera_object_quat_z,camera_object_quat_w;
extern int camera_quat_x,camera_quat_y,camera_quat_z,camera_quat_w;
extern int debug_time_to_stop, debug_time_to_target, debug_distance_to_target;

void run_AP(int object);
void run_flight_computations(int object);
void engage_ap(int the_object, int the_mode);
void disengage_AP(int object);
int find_nearest_planet(int object);
int is_AP_aligned(int object);
