//angular stuff
//SB/Lightsoft Apr 99
/*
 * $Log: angular_stuff.c,v $
 * Revision 1.1.1.1  2003/09/05 22:35:12  stu_c
 * First Imported.
 *
 * Revision 1.7  2002/09/08 02:12:05  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.6  2002/05/31 23:55:39  stu
 * Removed all warnings from the code
 *
 * Revision 1.5  2001/12/29 01:11:39  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.4  2001/12/14 19:41:08  stu
 * Docking
 *
 * Revision 1.3  2001/11/02 19:32:41  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:29:13  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:03:16  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:04  rob
 * First Imported.
 *
 * Revision 1.1.1.1  2001/01/01 21:13:34  rob
 * First Imported.
 *
 * Revision 1.0.0.1  2000/08/21 21:59:58  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <fp.h>
  #endif

  #include "options.h"		// should always be first include
  #include "3d_structures_def.h"
  #include "processor.h"
  #include "error_handler.h"
#endif //pch

#include "angular_stuff.h"
#include "debug_timing.h"

extern	DynObjectsFixedSize *ocb_ptr;

//10/4/99
//Zatan is a rough but fast arctan function suitable for our intentended use.
//Takes a value between 0 and 1 (i.e. half a quadrant)
//and returns an integer result in Degrees between 0 and 45
//Resolution about 2.5 degrees
//table computed from atan values betten 0 and 1 in .05 steps
int tan_result_table []
 = {
   0,3,6,8,11,14,17,19,22,24,26,29,31,33,35,37,38,40,42,43,45
   };


int Zatan (float in)
{
//extern UnsignedWide stu_timer1;

int index;
//if (in<0 || in>1) report_error("Zatan input range error","\p",9);
index=(int) (in*20);	//mem write/read to int register - bah!

if (index<0 || index>20) return 0;	//this is an error but we silently ignore it

index=tan_result_table[index];

return index;	//read
}


//calculates difference between angle 1 and 2 - may return either minor or major angle
int get_diff(int ang_1,int ang_2)
{
int diff;
diff=ang_1-ang_2;
if (diff<0) diff+=360;
else
if (diff>360) diff-=360;
return diff;
}

//returns the minor angle
int get_minor_diff(int ang_1,int ang_2)
{
int diff1,diff2;

diff1=ang_1-ang_2;
if (diff1<0) diff1+=360;
else
if (diff1>360) diff1-=360;

diff2=ang_2-ang_1;
if (diff2<0) diff2+=360;
else
if (diff2>360) diff2-=360;

if (diff2<diff1)
return diff2;
else return diff1;
}

//returns difference between ang1 and ang2 as +-.
//+ means left, -=right
//took a while to figure this out...
int left_or_right(int ang1, int ang2)
{
if (ang1>179) ang1=360-ang1;	//+0 to +180
else ang1=-ang1;	//-0 to -179

if (ang2>179) ang2=360-ang2;
else ang2=-ang2;

return ang1-ang2;
 
}
//takes two objects and returns if second is pointing exactly at first
//used in cannon fire mainly
//9/4/99
int is_aimed_at(int to_object, int from_object)
{
int angle_to;

angle_to=get_angle_toobject_fromobject (to_object,from_object);
if ((*ocb_ptr).object_list[from_object].Orotz==angle_to) return true;
else return false;
}


//9/4/99
int get_angle_toobject_fromobject (int to_object, int from_object)
{
float zex_wx,zex_wy,obj_wx,obj_wy;

  zex_wx=(*ocb_ptr).object_list[to_object].Oworldx;
  zex_wy=(*ocb_ptr).object_list[to_object].Oworldy;

  obj_wx=(*ocb_ptr).object_list[from_object].Oworldx;
  obj_wy=(*ocb_ptr).object_list[from_object].Oworldy;

  return get_angle_to_from(zex_wx,zex_wy,obj_wx,obj_wy);
}



//takes two objects and returns if second is pointing at first
//9/4/99
int facing_towards_target(int to_object, int from_object)
{
int angle_to,our_angle,diff;

angle_to=get_angle_toobject_fromobject (to_object,from_object);
our_angle=(*ocb_ptr).object_list[from_object].Orotz;
//calc angular diff and if in the range of -90 to + 90 then true else false
diff=get_diff(angle_to,our_angle);

if (diff>270 || diff<90) return true;
else
return false;
}

//set to newangle 0 to use mathlib atan function
#define NEWANGLE 1
#if NEWANGLE==1
//SB12/12/98
// 10/4/99 - update to use zatan(0 to 1 - i.e. 0-45 degrees) rather than rad based mathlib
// HOT SPOT
int get_angle_to_from (int to_x, int to_y, int from_x, int from_y)
{
//#define PIover180 PI/180

int quad,zangle=0;
//float atan_result;
float length_x,length_y;
//float PIover180 = PI/180; //use if atan returns rads

   if (to_y>=from_y &&  to_x>=from_x) quad=1;
   else
//check for angle=225 - above and left
   if (to_y>=from_y &&  to_x<=from_x) quad=2;
   else
//check for angle=135 - below and left
   if (to_y<from_y &&  to_x<from_x) quad=3;
   else quad=4;
//now we need to construct right angle triangles

   length_y=abs_nobranch(to_y-from_y);
   length_x=abs_nobranch(to_x-from_x);

/*
Each quad is split into 2 halfs, a and b, where a is the angular less than half and b is the angular greater than
half, so each oct occupies 45 degrees which is the limit of atan, well Zatan anyway...
      90
       |
    2  | 1
180---------- 0
    3  | 4
       |     
      270
God I am proud of that diagram... */
   if (quad==1)
   {
   if (length_y>length_x)
   {
//   atan_result=Zatan(length_x/length_y);
//   zangle=atan_result/PIover180;
     zangle=Zatan(length_x/length_y);
     zangle=45-zangle;	//correction
     zangle=zangle+45;	//add on oct 1b base angle
   }
   else	//quad 0a
   {
//   atan_result=Zatan(length_y/length_x);
//   zangle=atan_result/PIover180;
     zangle=Zatan(length_y/length_x);

   }
   }
   
   else
   if (quad==2)	//angle is between 90 and 180
   {
   if (length_y>length_x)
   {
//   atan_result=Zatan(length_x/length_y);
//   zangle=atan_result/PIover180;
     zangle=Zatan(length_x/length_y);
     zangle=zangle+90;	//add on oct 2a base angle
   }
   else	//oct 2b
   {
//   atan_result=Zatan(length_y/length_x);
//   zangle=atan_result/PIover180;
     zangle=Zatan(length_y/length_x);
     zangle=45-zangle;
     zangle=zangle+135;	//add on oct 2b base angle

   }
   }
   
   else
   if (quad==3)	//angle is between 180 and 270
   {
   if (length_x>length_y)
   {
//   atan_result=Zatan(length_y/length_x);
//   zangle=atan_result/PIover180;
     zangle=Zatan(length_y/length_x);
     zangle=zangle+180;	//add on oct 3a base angle

   }
   else
   {
//   atan_result=Zatan(length_x/length_y);
//   zangle=atan_result/PIover180;
   zangle=Zatan(length_x/length_y);
   zangle=45-zangle;
   zangle=zangle+225;	//add on oct 3b base angle

   }

   }
   
   else
   if (quad==4)	//angle is between 270 and 359
   {
   if (length_x<length_y)	//4a
   {
//   atan_result=Zatan(length_x/length_y);
//   zangle=atan_result/PIover180;
   zangle=Zatan(length_x/length_y);
   zangle=zangle+270;	//add on oct 3a base angle

   }
   else	//4b
   {	
//   atan_result=Zatan(length_y/length_x);
//   zangle=atan_result/PIover180;
   zangle=Zatan(length_y/length_x);
   zangle=45-zangle;
   zangle=zangle+315;	//add on oct 4b base angle

   }

   }

   if (zangle>359) zangle-=360;	//final crash check
  return zangle;
}



#else	//new_angle==0
//---------------old version
//SB12/12/98
// 1/1/99 - update for accuracy to 1 degrees
int get_angle_to_from (int to_x, int to_y, int from_x, int from_y)
{
//#define PIover180 PI/180

int quad,zangle;
float atan_result,length_x,length_y;
float PIover180 = PI/180;

   
   if (to_y>from_y &&  to_x>from_x) quad=1;
   else
//check for angle=225 - above and left
   if (to_y>from_y &&  to_x<from_x) quad=2;
   else
//check for angle=135 - below and left
   if (to_y<from_y &&  to_x<from_x) quad=3;
   else quad=4;
//now we need to construct right angle triangles

   length_y=abs_nobranch(to_y-from_y);
   length_x=abs_nobranch(to_x-from_x);

/*
Each quad is split into 2 halfs, a and b, where a is the angular less than half abd b is the angular greater than
half, so each octy occupies 45 degrees which is the limit of atan
      90
       |
    2  | 1
180---------- 0
    3  | 4
       |     
      270
 */
   if (quad==1)
   {
   if (length_y>length_x)
   {
   atan_result=atan(length_x/length_y);
   zangle=atan_result/PIover180;
   zangle=45-zangle;	//correction
   zangle=zangle+45;	//add on oct 1b base angle
   }
   else	//quad 0a
   {
   atan_result=atan(length_y/length_x);
   zangle=atan_result/PIover180;
   }
   }
   
   else
   if (quad==2)	//angle is between 90 and 180
   {
   if (length_y>length_x)
   {
   atan_result=atan(length_x/length_y);
   zangle=atan_result/PIover180;
   zangle=zangle+90;	//add on oct 2a base angle
   }
   else	//oct 2b
   {
   atan_result=atan(length_y/length_x);
   zangle=atan_result/PIover180;
   zangle=45-zangle;
   zangle=zangle+135;	//add on oct 2b base angle

   }
   }
   
   else
   if (quad==3)	//angle is between 180 and 270
   {
   if (length_x>length_y)
   {
   atan_result=atan(length_y/length_x);
   zangle=atan_result/PIover180;
   zangle=zangle+180;	//add on oct 3a base angle

   }
   else
   {
   atan_result=atan(length_x/length_y);
   zangle=atan_result/PIover180;
   zangle=45-zangle;
   zangle=zangle+225;	//add on oct 3b base angle

   }

   }
   
   else
   if (quad==4)	//angle is between 270 and 359
   {
   if (length_x<length_y)	//4a
   {
   atan_result=atan(length_x/length_y);
   zangle=atan_result/PIover180;
   zangle=zangle+270;	//add on oct 3a base angle

   }
   else	//4b
   {	
   atan_result=atan(length_y/length_x);
   zangle=atan_result/PIover180;
   zangle=45-zangle;
   zangle=zangle+315;	//add on oct 4b base angle

   }

   }
   
//010199
//now we know the octent we can use arctan to get the real angle
//we get 0 to 45, to that we must add the oct start angle
   if (zangle>359) zangle-=360;	//final crash check
   return zangle;
}

#endif


int negate_angle(int the_angle)
{
if (the_angle>=180) return the_angle-180;
else
return the_angle+180;
}


int offset_ang(int the_angle, int the_offset)
{
 the_angle+=the_offset;
 if (the_angle>359) the_angle-=360;	//final crash check
 else
 if (the_angle<0) the_angle+=360;	//final crash check
return the_angle; 
}