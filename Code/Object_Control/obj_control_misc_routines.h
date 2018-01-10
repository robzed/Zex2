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

//object_control_misc_routines.ext
int damage_object (int current_object, int shield_damage);
void tumble_object(int the_object);
void set_quick_fade();
void clear_quick_fade();

int find_unique_id(int the_id);
int get_object_unique_id(void);
void clear_pb(load_dyn_object_pb* the_pb);

void update_mass(int the_object);




