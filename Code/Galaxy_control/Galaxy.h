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

#define GALAXY_WIDTH NUMBER_OF_SECTIONS_IN_A_QUAD_X * 2 * SECTION_SIZE
#define GALAXY_H NUMBER_OF_SECTIONS_IN_A_QUAD_Y * 2 * SECTION_SIZE

//galactic scenary etc.
#define ASTEROID_FIELD1 0X00060001	//6 ASTEROIDS
#define ASTEROID_FIELD2 0x000C0001	//12 asteroids
#define COWS1 0x00020002	//3 space cows
#define COWS2 0x00060002	//10 space cows
#define CORRAL_FACING_RIGHT 0x00000003	//right facing corral
 
typedef struct {
int number_of_earth_mships;
int total_earth_supply_rating;
int av_earth_supply_rating;
int total_earth_power_rating;
int av_earth_power_rating;

int number_of_alien_mships;
int total_alien_supply_rating;
int av_alien_supply_rating;
int total_alien_power_rating;
int av_alien_power_rating;
} galactic_stats;


void make_new_galaxy();
int get_supply_rating(int mothership_index);
void handle_galactic_map();
int get_supply_rating_docked(int ocb_slot);

void get_galactic_stats(galactic_stats* the_stats);


// --------------
void set_up_solar_system();

int load_planet(double x_in_m, double y_in_m, double z_in_m, int seed, short galactic_id, int POBs_id, double planet_diameter_in_m, double planet_mass_in_kg, double rotation_time_in_secs);
int load_moon(double x_in_m, double y_in_m, double z_in_m, int seed, short galactic_id,int POBs_id, double planet_diameter_in_m, double planet_mass_in_kg, double rotation_time_in_secs);
int load_sun(double x_in_m, double y_in_m, double z_in_m, double sun_diameter_in_m, double sun_mass_in_kg, int seed, int sun_number, short galactic_id);
int load_mothership(double x_in_m, double y_in_m, double z_in_m, short galactic_id, int allegiance);

// from "make_solar_system.c"
void my_make_planets(double minimum_distance_away_from_center_in_m);
double load_suns(void);


// ---------------

void init_solar_system_map(void);	// at the start of a game
void reinit_solar_system_map(void);	// each time the map is viewed
void do_solar_map_single_frame(void);
void load_background_stars(void);
void load_background_galaxies(void);

extern int bg_galaxies;

// ---------------

void init_galactic_map(void);
void do_galaxy_map_single_frame(void);
void draw_item_info(int the_object, int screen_x, int screen_y, int colour);

