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
 *  object_manipulation.h
 *  zex22
 *
 *  Created by Stuart Ball on Fri Dec 28 2001.
 *  Copyright (c) 2001 LSG. All rights reserved.
 *
 $Header: /home/ls_cvs/ZEX2.2/Code/3d\040Engine/object_manipulation.h,v 1.3 2001/12/29 23:02:10 stu Exp $
 $Log: object_manipulation.h,v $
 Revision 1.3  2001/12/29 23:02:10  stu
 reset_rotation_for_object added

 Revision 1.2  2001/12/28 18:44:15  stu
 Added CVS headers

 */
 
void reset_rotation_for_object(int the_object); //Set Qor to along z
 
void set_rotation_around_world_x(int the_object, int rotation_in_degrees);
void set_rotation_around_world_y(int the_object, int rotation_in_degrees);
void set_rotation_around_world_z(int the_object, int rotation_in_degrees);

void set_rotation_around_arbitary_axis(int the_object, vector* the_axis, int rotation_in_degrees);