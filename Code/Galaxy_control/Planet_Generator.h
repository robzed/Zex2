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

void gen_planet_name(unsigned int seed, unsigned char * output_string, int type);

void planet_gen(int seed, int is_inhabited, unsigned char *scratch_address, unsigned char *output_address);
//void galaxy_gen(int seed, char *output_address);
void galaxy_gen(int seed, unsigned char *output_address, int squish_flag);

void init_name_generator(void);	//must be called before generating names


// new planetgen routines
void flat_planet_gen(int seed, int POBs_id, unsigned char *scratch_address, unsigned char *output_address);
