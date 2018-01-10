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

//angular_Stuff.ext
//SB/Lightsoft Apr 99
int Zatan (float in);
int get_diff(int ang_1,int ang_2);
int get_angle_to_from (int to_x, int to_y, int from_x, int from_y);
int get_angle_toobject_fromobject (int to_object, int from_object);
int is_aimed_at(int to_object, int from_object);
int facing_towards_target(int to_object, int from_object);
int negate_angle(int the_angle);
int offset_ang(int the_angle, int the_offset);

int get_minor_diff(int ang_1,int ang_2);
int left_or_right(int ang1, int ang2);


