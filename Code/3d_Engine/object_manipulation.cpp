/*
 *  object_manipulation.c
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
$Header: /home/ls_cvs/ZEX2.2/Code/3d\040Engine/object_manipulation.c,v 1.5 2002/09/08 02:15:37 stu_c Exp $
$Log: object_manipulation.c,v $
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




