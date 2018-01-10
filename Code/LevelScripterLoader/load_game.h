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

void load_level (unsigned char* the_filename);
void interpret_file(Ptr the_level);
void show_level_text();
void GetBGRAW(FSSpec* the_file);
int make_data_fsspec(unsigned char *the_folder_name,const unsigned char *the_filename, FSSpec* the_fsspec);	//make fsspec of file name in zd3 folder

int load_player_ship(double x_in_m, double y_in_m, double z_in_m, short galactic_id );
int load_player_vv(double x_in_m, double y_in_m, double z_in_m, short galactic_id );
int load_camera(double x_in_m, double y_in_m, double z_in_m, short galactic_id );





