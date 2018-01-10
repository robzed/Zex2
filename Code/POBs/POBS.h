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
// * FILENAME           : POBS.h
// * MODULE TITLE       : POBs headers
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 14th January 2002
// * FIRST AUTHOR       : Rob
// *
// * COPYRIGHT (c)        LS 2002
// *
// ***********************************************************************************

// from "POBS_game_data_accessors.c"
// =========================================================================
// Required POBS calls for trading
int POBs_get_number_of_ships_docked(int allegiance, int galactic_id);
int POBs_get_allegiance(int galactic_id); //returns HUMAN, ALIEN, NEUTRAL
int POBs_get_supply_rating(int galactic_id); //0-100
int POBs_get_supply_type(int galactic_id); //1-5=agri, 6-10=tech

// bonus routines
void tell_POBs_ship_launched(int allegiance, int galactic_id);
void tell_POBs_ship_docked(int allegiance, int galactic_id);


// from "POBS_object_managment.c"
// =========================================================================
void POBs_init(void);
void service_POBS(void);
void POBs_load_sun(double x_in_m, double y_in_m, double z_in_m, double sun_diameter_in_m, double sun_mass_in_kg, int seed, int sun_number);
void POBs_load_planet(double x_in_m, double y_in_m, double z_in_m, int seed, double planet_diameter_in_m, double planet_mass_in_kg, double rotation_time_in_sec);
void POBs_load_moon(double x_in_m, double y_in_m, double z_in_m, int seed, double planet_diameter_in_m, double planet_mass_in_kg, double rotation_time_in_sec);
void POBs_load_spacestation(double x_in_m, double y_in_m, double z_in_m, int seed, int allegiance);
int my_planet(void);		// hack!

void POBs_load_player_ship(double x_in_m, double y_in_m, double z_in_m);
void POBs_load_player_vv(double x_in_m, double y_in_m, double z_in_m);
void POBs_load_camera(double x_in_m, double y_in_m, double z_in_m);

// world control
void POBS_unload_scene(void);
void POBS_load_scene(void);
void check_all_POBS(void);
void inform_POBs_we_have_finished_constructing_solar_system(void);

// general data accessor functions
int get_new_galactic_id(void);

// planet & moon accessor functions
char POBs_get_planet_or_moon_colour(int dyn_slot);
int POBs_is_this_a_moon(int gal_id);
int POBs_is_this_a_planet(int gal_id);
int POBs_is_this_a_planet_or_moon(int gal_id);
int POBs_is_this_a_mothership(int gal_id);
int POBs_is_inhabited(int gal_id);
double POBs_what_is_population(int gal_id);
int POBs_is_this_a_rock_moon(int gal_id);
int POBs_is_this_a_gas_giant(int gal_id);
int POBs_is_this_earthish_size(int gal_id);
int POBs_is_this_a_sun(int gal_id);
int POBs_is_this_a_giant_planet(int gal_id);
int POBs_is_this_a_hypermassive_gas_giant(int gal_id);

// linkage data accessors
int POBs_object_parent(int gal_id);
int POBs_object_first_child(int gal_id);
int POBs_object_next(int gal_id);
int POBs_object_previous(int gal_id);

// random stuff
double get_POBs_diameter(int gal_id);
double get_POBs_rotation(int gal_id);

