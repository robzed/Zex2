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

// *************************************MODULE HEADER*********************************
// * FILENAME           : access_ocbs.h
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * DATE STARTED       : Sun 08:56:11 on 8th Oct 2000
// * FIRST AUTHOR       : Rob Probin
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************

#define ACCESSORS_DEBUG 1	//causes slot numbers to be checked and error asserted if bad slot


#if ACCESSORS_DEBUG 
void check_slot(int object_slot_number); 
void check_static_slot(int object_slot_number);

#endif

//Misc dyn functions
int ptr_to_dynslot(ZObject* which_object); //converts a ptr to a slot
// reading functions




ZObjectControl* get_object_control_data(int object_slot_number);


double get_mass_in_kg(int object_slot_number);
void get_velocity_in_mps(int object_slot_number,_3D *velocity_ptr);
void get_rel_velocity_in_mps(int object_slot_number,vector *velocity_ptr);

void get_position_in_m(int object_slot_number,_3D *position_ptr);
int get_object_in_use(int object_slot_number);
int object_is_real(int object_slot_number);
int get_object_targetted(int object_slot_number);
int get_object_controller(int object_slot_number);
int get_object_control_use_1(int object_slot_number);

vector get_object_abs_vel_vector(int object);
float get_object_radius_in_metres(int object);

int get_object_category( int object_slot_number);	//0=human, 1=alien, 2=neutral, 3=scenary
int get_galactic_id(int object_slot_number);

void get_raw_position(int object_slot_number,_3D *position_ptr);

int get_object_Oid(int object);
void get_translated_normal(int object_slot_number,_3D *trans_normal_ptr);

float get_total_fuel_consumption(int object_slot_number);
float get_engine_temperature_in_C(int object_slot_number);
float get_hull_temperature_in_C(int object_slot_number);
float get_laser_bay_temperature_in_C(int object_slot_number);


// writing functions
void set_mass_in_kg(int object_slot_number, double mass);
void set_velocity_in_mps(int object_slot_number,_3D velocity);
float get_ND_velocity_in_mps(int object_slot_number);
void set_position_in_m(int object_slot_number,_3D position);
void set_object_position(int object_slot_number, double x, double y, double z);
void set_object_in_use(int object_slot_number);
void set_object_not_in_use(int object_slot_number);

void set_object_targetted(int object_slot_number, int slot_targetted);

void set_object_abs_vel_vector(int object, vector the_vect);

void set_blobbed_object_colour(int object_slot_number, float red, float green, float blue);

void set_engine_temperature_in_C(int object_slot_number, float the_temperature_in_C);
void set_hull_temperature_in_C(int object_slot_number, float the_temperature_in_C);
void set_laser_bay_temperature_in_C(int object_slot_number, float the_temperature_in_C);

void set_has_children(int object_slot_number); //Tell engine that object has at least one child


//static
int STATIC_get_object_Oid(int object);
int STATIC_get_object_in_use(int object);
float STATIC_get_object_radius_in_metres(int object);

