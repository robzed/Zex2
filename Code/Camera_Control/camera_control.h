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


//camera_control.ext


#define DEFAULT_CAMERA_ZOOM 10000
#define MIN_CAMERA_ZOOM 3500
#define MAX_CAMERA_ZOOM 17500

#define CAMERA_OBJECT 2

void run_camera_control(void);
void reset_camera();
int get_main_camera_object(void);
int set_main_camera_object(int dynamic_slot);

void camera_set_front_view();
void camera_set_right_view();
void camera_set_left_view();
void camera_set_rear_view();

int has_camera_switched_view();

int camera_get_view(); //returns 1=front, !1=panned
float camera_get_pan(); //returns camera pan force




