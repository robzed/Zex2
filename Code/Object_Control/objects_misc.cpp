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

//objects_misc.cpp
/*
 * $Log: objects_misc.cpp,v $
 * Revision 1.2  2003/09/20 12:57:06  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:36:08  stu_c
 * First Imported.
 *
 * Revision 1.3  2002/09/08 02:14:00  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.2  2001/11/02 19:32:19  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.1  2001/10/21 01:13:02  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:07  rob
 * First Imported.
 *
 * Revision 1.1.1.1  2001/01/01 21:13:38  rob
 * First Imported.
 *
 * Revision 1.0.0.1  2000/08/21 22:08:05  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #include "3d_structures_def.h"
#endif //pch

#include "objects_misc.h"
#include "camera_control.h"
#include "physics.h"
#include "dlp.h"

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//searches static objects for an id, if found returns the slot of the object else -1
int find_dynamic_object_slot(unsigned long the_id)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int	dynamic_object_index;	//how many dynamic objects
int return_val;

int match=0,active_object_count, object_count;
return_val=-1;

object_count=active_object_count=0;
while (active_object_count<dynamic_object_index)
{
  if ((*ocb_ptr).object_list[object_count].in_use==1)
  {
   if ((*ocb_ptr).object_list[object_count].Oid==the_id)
   {
     match=1;
     return_val=object_count;
     break;
   }
   active_object_count++;
 }
 object_count++;
}
if (match==1) return return_val;	//slot of static definition of bg object
else return return_val;

}

//called from main loop.
//Every frame it examines one object to see if it is near enoguh to the camera object
//to stop time accel and hypertransport from working
//and sets near_object_flag if found
//run from object_control_top

static int near_object=-1;

void calculate_object_near_flag(void)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int	dynamic_object_index;	//how many dynamic objects
int camera_object;
static int object_index=3; //0=zex, 1=vv, 2=camera
double distance;

  camera_object=get_main_camera_object();

  if (object_index>dynamic_object_index) //check we're not about to read from a duff object
  {
   object_index=3;
       if (near_object!=-1)
       {
            add_to_text_display("OBC> No mass in immediate vicinity.",DLP_GREEN);  

       }

   near_object=-1;
  }
  
  if (object_index!=camera_object)
  {
    if (((*ocb_ptr).object_list[object_index].in_use==1) && ((*ocb_ptr).object_list[object_index].mass>0.5))
    {
      //if it is near enough then we set the near object and restart the scan.
      //only if we get to the end of all the objects do we clear the near object
      distance=get_distance_to_from_in_m(object_index,camera_object);
      if (distance<50000.0) //anything within 50KM's
      {
       if (near_object==-1)
       {
            add_to_text_display("OBC> Nearby mass detected!",DLP_RED);  
            add_to_text_display("OBC> Hypertransport disabled.",DLP_RED);  

       }
       near_object=object_index;
       object_index=3;
      }
    }
  }
  
 object_index++; //increment for next run
}


//returns -1 if no near object
int check_for_object_near(void)
{
 return near_object;
}



//returns 0=no, 1=yes
int point_in_rect(int x, int y, LSRect the_rect)
{
if (x<the_rect.left) return 0;
if (x>the_rect.right) return 0;
if (y>the_rect.top) return 0;
if (y<the_rect.bottom) return 0;
return 1;
}

//returns 0=no, 1=yes
int point_in_float_rect(float x, float y, LSFloatRect the_rect)
{
if (x<the_rect.left) return 0;
if (x>the_rect.right) return 0;
if (y>the_rect.top) return 0;
if (y<the_rect.bottom) return 0;
return 1;
}