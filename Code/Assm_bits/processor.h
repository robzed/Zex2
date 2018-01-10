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

// *****************************************************************MODULE HEADER*****
// * FILENAME		: processor.ext
// * MODULE TITLE	: processor related functions external (public) definitions.
//
// * PROJECT		: Zex
// *
// * COPYRIGHT (c) 1999 Lightsoft
// *           (c) 1999 Robert Probin and Stuart Ball
// *		   http://www.lightsoft.co.uk/
// *
// * Lightsoft is a trading name of Robert Probin and Stuart Ball.
// *
// ***********************************************************************************



void init_cache_line_size(char *big_buffer);
int get_cache_line_size(void);
int InRange(int value, int min, int max);
long abs_nobranch (long i);
void reset_microsecs_timer(void);
void update_microsecs_timer(void);

void freeze_physics(void);
void unfreeze_physics(void);
int physics_frozen(void);


extern double time_in_secs;	// this is "game_frame_time_in_secs"
extern double real_frame_time_in_secs;

void InitThreads();
void SetHighPriority();
void SetNormalPriority();
void thread_yield();
