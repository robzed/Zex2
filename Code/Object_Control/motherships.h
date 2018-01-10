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

typedef struct
{
int slot;
int allegiance;
float x,y;
} mothership_nav_entry;


void handle_earth_mothership(int it);	//tractor and laser - stationary position
void do_mothership_hit(int object_slot, int mothership_slot);
void cancel_dock(int object_being_docked);

void handle_active_mothership(int it);	  //a live mothership, gens bad guys
void load_motherships_into_nav();
void update_motherships();
void handle_game_mothership(int it);
int is_a_mothership(int slot);
//mammary

mothership_block * mship_get_mem();
int mship_release_mem(mothership_block * the_block);
void init_mship_mem();