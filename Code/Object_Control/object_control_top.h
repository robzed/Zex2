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

void do_object_control();
void handle_weeny_bullet(int it);
void handle_explode(int it);
void handle_monster1(int it);
void handle_not_so_weeny_bullet(int it);
void handle_ejection(int it);
void kill_kill_list();
void reset_kill_list();
int add_to_kill_list (int current_object);
void handle_laser_weeny (int it, int rear);
void handle_gun1(int it);
void handle_persistent_asteroid(int it);
void handle_burning_particle(int it);
void handle_missile1(int it);
void handle_space_cow(int it);
void handle_earth_mothership(int it);
void handle_planet_roty(int it);	//rotate slowly every control_1 frames
void handle_planet_rotz(int it);	//rotate slowly every control_1 frames
void do_mothership_hit(int object_slot, int mothership_slot);
void handle_static_laser1(int it);	//static laser cointroller

void i_know_what_im_doing_kill( int the_object, int kill_ogl_textures);

void reset_torque(ZObject* current_object_ptr);

void kill_children(int parent);



