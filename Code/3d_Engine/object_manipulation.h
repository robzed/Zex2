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