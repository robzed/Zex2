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

extern double total_fuel_consumption;
extern int can_consume_fuel;

extern int camera_object_total_fuel;
extern int camera_object_total_fuel_consumption;
extern int camera_object_orcs_fuel_consumption;
extern int camera_object_orcs_rot_fuel_consumption;
extern int camera_object_orcs_lin_fuel_consumption;


void run_engines(int object);

int is_engine1_available (int the_object);
int is_engine2_available (int the_object);
int is_rev1_available (int the_object);
int is_rev2_available (int the_object);

int is_engine2_fitted (int the_object);
