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

#define MAX_AWAKE 38*60*60*13

typedef struct {
int days,hours,minutes,seconds;
} real_time_struct;


void get_real_time_struct(real_time_struct* time_struct);
void update_real_time();
void draw_real_time(int x, int y, int colour);
void init_game_time();
void add_a_minute_to_real_time();
void check_sleep();
int get_TOD_hours();
int get_TOD_mins();
int get_TOD_secs();

void flip_time_multiplier_up(void);
void flip_time_multiplier_down(void);
void set_time_multiplier_to_1(void);
void set_time_multiplier_to_5(void);
void set_time_multiplier_to_2(void);
void set_time_multiplier_to_10(void);


void set_time_multiplier_to_1000(void);
void set_time_multiplier_to_500(void);
void set_time_multiplier_to_200(void);
int get_time_multiplier(void);

