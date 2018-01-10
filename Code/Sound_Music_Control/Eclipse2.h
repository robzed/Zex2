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

/*
 *  Eclipse2.h
 *  zex22
 *
 *  Created by stu on Sat Nov 10 2001.
 *  Copyright (c) 2001 Lightsoft Software (Games). All rights reserved.
 *
 */
typedef signed int eclipse_sound_ref ;
typedef signed int eclipse_err;

zex_public eclipse_err eclipse_init(void);
zex_public eclipse_err eclipse_play_one_shot_sound(SndListHandle the_sound);
zex_public eclipse_sound_ref  eclipse_play_looped_sound(SndListHandle the_sound, int the_object);
zex_public eclipse_err eclipse_set_looped_chan_volume (eclipse_sound_ref the_ref, int volume_percent);

zex_public void eclipse_kill_all_looped_sounds(void);
zex_public int eclipse_kill_looped_sound(eclipse_sound_ref the_ref);

zex_public void eclipse_update_sounds(void);



#define ECLIPSE_CHECK_PARAMS 1	//If 1, Eclipse routines will check param validity
#define ECLIPSE_STATUS_TO_LOG 1 //If 1, status messages are sent to stderr

#define MIN_LISTEN_DISTANCE_IN_M 7500 // Distance in metres from camera object
                                       // at which we start calculating positional sounds

//Eclipse Errors
#define ECLIPSE_NO_ERR 0
#define ECLIPSE_NO_SLOT_ERR -1
#define ECLIPSE_BAD_SOUND_FORMAT_ERR -2
#define ECLIPSE_BUFFCMD_ERR -3
#define ECLIPSE_CALLBACK_STORE_ERR -4
#define ECLIPSE_BAD_SOUND_REF -5
#define ECLIPSE_CANT_KILL -6

//params to our buffer expiry callback proc
#define NO_LOOP_SOUND 10  // - private
#define LOOP_SOUND 11     // - private

//Control constants
#define SOUND_UPDATE_TIME_IN_SECS 0.1 // secs - how often we recalc sound position - private



