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
 *  fast_dyn.h
 *  zex22
 *
 *  Created by Stuart Ball on Sat Feb 09 2002.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
 */


int load_Fdyn_object(int static_slot,int dest_focb, load_dyn_object_pb *the_pb, float scale, int parent_slot);
void Fi_know_what_im_doing_kill( int the_object, int kill_textures);
void clear_out_Fdyn();
int find_vacant_Fdynamic_object_slot();