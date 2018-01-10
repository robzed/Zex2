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

// *************************************MODULE HEADER*********************************
// * FILENAME           : 3d_depth_of_field_manager.h
// * MODULE TITLE       : Handles setting up the screen correctly
// * PROJECT            : Zex
// * DATE STARTED       : 23rd February 2002
// * FIRST AUTHOR       : Rob
// *
// * COPYRIGHT (c)        Lightsoft 2002
// *
// ***********************************************************************************

void init_depth_of_field(void);
void set_angle_of_view(int angle);
int get_angle_of_view(void);
double get_cos_of_angle_of_view(void);
