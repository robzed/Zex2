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

#define DC_NOM 28 //volts DC
#define AC_NOM 400 //volts AC
void apply_control_inputs();
void run_ORCS();


short check_key(unsigned short k);
void ship_init();
void do_player_fire();
void reset_ship();
void reset_ship_with_carry_over();
short fire_button();
void do_player_laser();
void do_player_missile();
void kill_zex();


int get_pylon();
void select_next_pylon();
void reset_screen_from_status();
void clear_down_ship_on_eject();
void do_ejection();
void do_player_rear_laser();

void end_game_right_here_right_now(int EOLCode, char * the_reason_string);


float get_ship_mass_in_kgs(void);

extern int selectGalacticMap;
extern int selectSolarMap;

#define ZEX2 1	//mainly debug

extern unsigned int last_pause_frame;

