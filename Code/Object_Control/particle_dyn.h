/*
 *  particle_dyn.h
 *  zex22
 *
 *  Created by Stuart Ball on Sun Jun 02 2002.
 *  Copyright (c) 2002 Lightsoft. All rights reserved.
 *
 */

void init_Pdyns(void);
int load_Pdyn_object(int static_slot, load_dyn_object_pb *the_pb, int parent_slot);
void Pi_know_what_im_doing_kill( int the_object, int kill_textures);
