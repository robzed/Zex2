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

//3d_top.ext

#ifndef __3d_structures__
#include "3d_structures_def.h"
#endif

extern int backface_cull;

void do_3d ();
void do_pers3(int width, int height);
void calc_normal(int the_object, int poly_select);
void do_culling2();
void do_zsort();
void calc_normal(int the_object, int poly_select);
void insert_to_z_buffer(_ZPoly *the_poly);
void reset_zsort_insert();
void do_gourad_polys();
_2Dp get_screen_center_coord_with_bflag(int object_slot);
_2D get_screen_front_coord(int object_slot);
_2D get_screen_rear_coord(int object_slot);
_2D point_to_screen(double x, double y, double z, int width, int h);
_2Dp point_to_screen_with_bflag(double x, double y, double z, int width, int h);
_2D get_any_screen_coord(int object_slot);

int is_in_frustrum(int object, bool fast_object);
_3D generate_point_in_frustrum(int object);


extern double depth_of_field;
extern double ZMin;

