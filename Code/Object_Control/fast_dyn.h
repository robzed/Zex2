/*
 *  fast_dyn.h
 *  zex22
 *
 *  Created by Stuart Ball on Sat Feb 09 2002.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
 */

#include <Carbon/Carbon.h>

int load_Fdyn_object(int static_slot,int dest_focb, load_dyn_object_pb *the_pb, float scale, int parent_slot);
void Fi_know_what_im_doing_kill( int the_object, int kill_textures);
void clear_out_Fdyn();
int find_vacant_Fdynamic_object_slot();