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

void init_input_device();
void configure_input_device();
void process_input_device();
OSStatus InitNeeds();
void SetKeyboardMouseActivation(Boolean active);
void poll_input_device();
void kill_inputs();
void enable_inputs();
void disable_inputs();
void take_mouse_keyboard_as_input();
void untake_mouse_keyboard_as_input();
int game_fire_button();
void ZexGetMouse (Point *the_point);
short isPressed(unsigned short k );	//k is keycode
void init_ip_globs();


