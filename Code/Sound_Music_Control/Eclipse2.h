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

public eclipse_err eclipse_init(void);
public eclipse_err eclipse_play_one_shot_sound(SndListHandle the_sound);
public eclipse_sound_ref  eclipse_play_looped_sound(SndListHandle the_sound, int the_object);
public eclipse_err eclipse_set_looped_chan_volume (eclipse_sound_ref the_ref, int volume_percent);

public void eclipse_kill_all_looped_sounds(void);
public int eclipse_kill_looped_sound(eclipse_sound_ref the_ref);

public void eclipse_update_sounds(void);



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



