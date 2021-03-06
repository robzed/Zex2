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
 *  object_manipulation.cpp
 *  zex22
 *
 *  Created by Stuart Ball on Fri Dec 28 2001.
 *  Copyright (c) 2001 LSG. All rights reserved.
 *  This file contains basic object rotation functions.
 *  All functions take a dynamic slot number.
 *  These functions sit on top of matrix_primitives
 *  WARNING: These functions do no error checking. Angles are in degrees.
 *  Passing in an angle outside the range 0>x>359 will result in garbage results.
 *  Similarly, where a vector is specified it MUST be normalised else again garbage results.
 
 Here is what people have been up to:
$Header: /home/ls_cvs/ZEX2.3/Code/3d_Engine/object_manipulation.cpp,v 1.1.1.1 2003/09/05 22:35:11 stu_c Exp $
$Log: object_manipulation.cpp,v $
Revision 1.1.1.1  2003/09/05 22:35:11  stu_c
First Imported.

Revision 1.5  2002/09/08 02:15:37  stu_c
Precompiled header adjustments

Revision 1.4  2002/01/20 17:12:36  stu
QuatAddValue changed to QuatSetValue

Revision 1.3  2001/12/29 23:00:46  stu
Added reset_rotation_for_object and QuatsetvalueNormalised functions replaced with QuatAddValue.

Revision 1.2  2001/12/28 18:42:38  stu
Added CVS headers


 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

  #include "3d_structures_def.h"
#endif //pch

#include "matrix_primitives.h"
#include "object_manipulation.h"

extern	DynObjectsFixedSize *ocb_ptr;

void reset_rotation_for_object(int the_object)
{
	QuatSetValue(&ocb_ptr->object_list[the_object].Qor,0,0,1,0);	//pointing into z
}

void set_rotation_around_world_x(int the_object, int rotation_in_degrees)
{
	QuatAddValue(&ocb_ptr->object_list[the_object].Qor,1,0,0,rotation_in_degrees);	//along x
}

void set_rotation_around_world_y(int the_object, int rotation_in_degrees)
{
	QuatAddValue(&ocb_ptr->object_list[the_object].Qor,0,1,0,rotation_in_degrees);	//along y
}


void set_rotation_around_world_z(int the_object, int rotation_in_degrees)
{
	QuatAddValue(&ocb_ptr->object_list[the_object].Qor,0,0,1,rotation_in_degrees);	//along z

}

//Warning, the_axis MUST be a normalised vector else weird things happen
void set_rotation_around_arbitary_axis(int the_object, vector* the_axis, int rotation_in_degrees)
{
	QuatSetValue(&ocb_ptr->object_list[the_object].Qor, the_axis->x, the_axis->y, the_axis->z, rotation_in_degrees);	//along x
}




