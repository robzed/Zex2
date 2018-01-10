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

#define MAX_NUMBER_OF_OMNILIGHTS 2	// how many suns Rob's allowed to have...

//zex 2 - 101200 Stu
void init_lights(void);
int set_light_position(int light_number, double position_x, double position_y, double position_z);
int get_light_position(int light_number, _3D *return_val);
int get_light_brightness(int light_number, int *return_val);
int set_light_default_brightness(int light_number);
int set_light_brightness(int light_number, int brightness);

//3d functions
void	calculate_light_normal(int object, _3D* light_normal, int * light_amplitude);	
void   set_black_level(int the_black_level); //0=black, 32767=white


