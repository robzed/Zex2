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

//load_objects.ext
int load_objects();
int load_dyn_object(int static_slot,int dest_ocb,load_dyn_object_pb *the_pb, int mothership_galactic_slot, float scale, 
                    int parent_slot, int is_dust);

void replace_dyn_object(int static_slot,int dest_ocb);
int find_vacant_dynamic_object_slot();	//returns an index into the dynamic_objects_array
int find_vacant_dyn_poly_slot();	//returns an index into the dynamic_polys_array
void reset_objects();
int load_object(const unsigned char *filename, int static_slot, unsigned long id,int colldetect, unsigned long texture_id, int translucent);
void load_animated_object(anim_pb* the_objects, int number_of_objects, int frame_rate, int control,int state, load_dyn_object_pb *the_pb, short galactic_id);     

void swap_dyn_object(int dest_ocb, unsigned long ID);
int get_scaled_points_slot();
extern int FILE_LOAD_ERROR_QUITS;


