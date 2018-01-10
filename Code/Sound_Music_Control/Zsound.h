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

//ZEX sound public defs


//channels
#define ls1_chan	0	//mono1
#define ls2_chan	1	//mono2
#define ls3_chan	2	//mono3
#define ls4_chan 3	//mono4
#define ls5_chan	4	//stereo 1
#define ls6_chan	5	//stereo 2
#define speech_chan 6
#define c1_chan1	7
#define stereo1_chan	8

#define c1_chan2 9
#define c1_chan3	10
//#define laser_middle	  11
//
//#define laser2_right 12
//#define laser2_left	  13
//#define laser2_middle	14

#define continuous1	11
#define continuous2	12


//priorities
#define sound_low_pri	0	//if a channel is busy, this sound is ignored
#define sound_high_pri	1	//if a channel is busy, chan is flushed and this sound is played

//channel volumes - linear to non-linear translation (audio gain is a logarithmic function)
#define sound_vol_default	0	//don't set sound level
#define sound_vol_0	1
#define sound_vol_1	10
#define sound_vol_2	50
#define sound_vol_3	90
#define sound_vol_4	120
#define sound_vol_5	150
#define sound_vol_6	210
#define sound_vol_7	255	//max

//misc defs
#define max_sounds 80	//maximum number of sounds we can load
//sound definitions

/*
enum the_sounds 
	{
	short_explosion,	//128
	short_explosion_with_echo,
	shield_scratch,	//130
	space_synth,
	zip,
	retro_sound,
	mono_roar,
	LS,
	laser1,
	warning,
	danger,
	missile_explode,
	missile_launch,	//140
	refuel_beep,
	LOW_BEEP,
	sound_test,
	thud,
	laser2,
	laser3,
	click,
	large_shield_sound,
	setup_sound,
	bleeper,	//150
	growl,
	explosion4,
	screech,
	tractor_beam_loop,	//continuous sound
	rumble_loop,	//continuous sound
	mac_startup,
	pickup,
	jettison,
	select_mship,
	select_planet,	//160
	select_unknown,
	message_from_command,
	launch,
	out_of_cruise,
	thrust_sound,
	thrust_reverse_sound,
	engine_loop,	//251100
	};
*/


	
#define short_explosion 0	//128
#define 	short_explosion_with_echo 1
#define 	shield_scratch 2	//130
#define	space_synth 3
#define	zip 4
#define	retro_sound 5
#define	mono_roar 6
#define	LS 7

#define	laser1 8
#define	warning 9
#define	danger 10
#define	missile_explode 11
#define	missile_launch 12	//140
#define	refuel_beep 13
#define	low_beep 14
#define	sound_test 15
#define	thud 16
#define laser2 17
#define	laser3 18
#define	sclick 19
#define	large_shield_sound 20
#define	setup_sound 21
#define	bleeper 22	//150
#define	growl 23
#define	explosion4 24
#define	screech 25
#define	tractor_beam_loop 26	//continuous sound
#define	rumble_loop 27	//continuous sound
#define	mac_startup 28
#define	pickup 29
#define	jettison 30

#define	select_mship 31
#define	select_planet 32	//160
#define	select_unknown 33
#define	message_from_command 34

#define	launch 35

#define	out_of_cruise 36
#define	thrust_sound 37
#define	thrust_reverse_sound 38

#define	engine_loop 39	//251100
#define	LOW_ENGINE_LOOP16 40	//181101
#define	HIGH_ENGINE_LOOP16 41	//181101


//sound functions protos
int	init_zsound();

void run_engine_sound(int the_object);


void play_zsound(int sound_number, int priority, int channel, int volume);
void play_zsound_relative_to_zex(int the_object, int sound_number);
void play_zsound_always(int sound_number, int priority, int channel, int volume);
//void play_laser_relative_to_object(int the_object, int sound_number, int priority, int volume);
//void play_laser_relative_to_object2(int the_object, int sound_number, int priority, int volume);
void run_periodic_sound_tasks();

void load_mod_player();
void music_set_vol(int volume);
int is_music_busy(void);
void fade_out_music();
void do_fade_out_music();
#if PORTABLE_FILESYSTEM
void fade_and_play(char *the_tune);
#else
void fade_and_play(Handle the_tune);
#endif
void fade_and_play_zex_resource (unsigned long res_id);
void play_zex_resource_now (unsigned long res_id);

//void run_level_music(FSSpec * the_file);
void play_level_music();
void stop_music_now();
int do_we_think_music_is_playing(void);

void run_level_aux_music(int index);
void play_level_aux_music(int index);

//simple sound routines
void simple_stop_sound();
void simple_play_sound(int id);


//Accessors
Handle get_sound_handle(int the_sound);

// external sound interface to playerpro
int mod_player( int command, Ptr music);

