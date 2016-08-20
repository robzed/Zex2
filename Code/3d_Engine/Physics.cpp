// *************************************MODULE HEADER*********************************
// * FILENAME           : Physics.cpp
// * MODULE TITLE       : ZPhysics
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 17:36:28 on 9th May 2000
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************
/*
 * $Log: Physics.cpp,v $
 * Revision 1.2  2003/09/20 12:57:11  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:35:12  stu_c
 * First Imported.
 *
 * Revision 1.12  2002/09/08 02:10:48  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.11  2002/04/14 15:57:31  stu
 * Further work
 *
 * Revision 1.10  2002/04/13 15:29:17  stu
 * Rotation, Alycians, behaviour
 *
 * Revision 1.9  2002/02/21 22:48:02  stu
 * reset_angular_torque function added
 *
 * Revision 1.8  2002/01/27 18:11:34  stu
 * Put in but commented out acceleration too great breaks.
 *
 * Revision 1.7  2001/12/29 01:12:10  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.6  2001/12/14 19:43:46  stu
 * Docking
 *
 * Revision 1.5  2001/11/28 21:40:23  stu
 * Finish Eclipse so works on Non os x carbon as well.
 *
 * Revision 1.4  2001/11/24 19:27:45  stu
 * RA_line_line in place of sqrt
 *
 * Revision 1.3  2001/11/02 19:32:30  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:29:22  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:10:57  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:04  rob
 * First Imported.
 *
 * Revision 1.4  2001/09/04 20:39:02  stu
 * Sim stuff
 *
 * Revision 1.3  2001/07/29 10:36:21  rob
 * time_in_secs prototype removed
 *
 * Revision 1.2  2001/06/23 20:04:31  stu
 * 0.66 checkin
 *
 * Revision 1.1.1.1  2001/01/01 21:13:34  rob
 * First Imported.
 *
 * Revision 1.9  2000/11/26 22:54:37  stu
 * Various.
 *
 * Revision 1.8  2000/11/26 22:36:06  rob
 * Fixed gravity
 *
 * Revision 1.7  2000/10/19 19:08:25  stu
 * Relative velocities, turbulence, planet names.
 *
 * Revision 1.6  2000/10/08 21:43:22  stu
 * Engine conversion to 64 bit. Huge pain of a job.
 *
 * Revision 1.5  2000/10/08 13:21:56  rob
 * Added:

1. apply linear force with reference to world coords
 *
 * Revision 1.4  2000/09/20 23:53:24  stu
 * HUD/AP/Scaling
 *
 * Revision 1.3  2000/08/27 21:40:51  stu
 * Lurkers clear up...
 *
 * Revision 1.2  2000/08/27 16:28:24  stu
 * Changes for movement decoupling in time.
 *
 * Revision 1.1  2000/08/27 01:34:06  stu
 * Hud changes
 *
 * Revision 1.0.0.1  2000/08/21 21:59:50  stu
 * First Imported.
 *
 */


// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else 
    #include <fp.h>
  #endif
  #include "3d_structures_def.h"
  #include "processor.h"
  #include "access_ocbs.h"
#endif //pch

#include "matrix_primitives.h"
#include "Physics.h"
#include "camera_control.h"

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

// ----------------------------------------------------------------------------------------

void apply_a_linear_force_world_reference(int object, vector force)
{
vector velocity;
extern DynObjectsFixedSize *ocb_ptr;
double magnitude,mass_in_kg;

// The routine uses F=ma, and velocity = accleration*time
// we have F, and time, and we want velocity. v = a * t, where F=ma, or a=F/m
// so... v = Ft/m. Since F is a vector, we want to calculate t/m first, and 
// multiply it to the force to get the velocity change.

mass_in_kg=get_mass_in_kg(object);
magnitude=time_in_secs/(mass_in_kg*ZEX_UNIT_SIZE_IN_METERS);	//time is seconds


Vmultiply(&force,magnitude,&velocity);

(*ocb_ptr).object_list[object].abs_velocity_vector.x+=velocity.x;
(*ocb_ptr).object_list[object].abs_velocity_vector.y+=velocity.y;
(*ocb_ptr).object_list[object].abs_velocity_vector.z+=velocity.z;
}


// apply a force (is local to object coords and ) can be perhaps optimised for the special case of 
// a non-rotational force, e.g. perhaps a tracker beam?
void apply_initial_force (int object, float magnitude, float for_how_long_in_secs, vector in_p /*oint*/ , vector in_d /*irection*/)
{
vector torque,accel;
//vector temp;
vector p,d;	//converted to local space of in_p and 

float torqueMag;
extern DynObjectsFixedSize *ocb_ptr;
//int coeff;
double Or_mat[4][4];


//translate Qor to orientation matrix
NewQuatToMat(&(*ocb_ptr).object_list[object].Qor,Or_mat);

VEC_MultMatrix(&in_p,Or_mat,&p);
VEC_MultMatrix(&in_d,Or_mat,&d);
Vmultiply(&d,(magnitude/(*ocb_ptr).object_list[object].mass),&accel);


//apply linear component
(*ocb_ptr).object_list[object].abs_velocity_vector.x+=accel.x;
(*ocb_ptr).object_list[object].abs_velocity_vector.y+=accel.y;
(*ocb_ptr).object_list[object].abs_velocity_vector.z+=accel.z;

//Time for torque taken care of in apply_extern_forces

 //Give d the magnitude of the force
 VmultiplyBy(&d,magnitude);

 //Find the torque vector
 Vcrossproduct(&d,&p,&torque);

 //And extract and store the magnitude of the torque
 torqueMag=vectorMod(&torque);

// add to cumlative torque vect total for this object
(*ocb_ptr).object_list[object].torque_sum.x+=torque.x;
(*ocb_ptr).object_list[object].torque_sum.y+=torque.y;
(*ocb_ptr).object_list[object].torque_sum.z+=torque.z;


}

void reset_angular_torque(int dyn_object)
{
extern DynObjectsFixedSize *ocb_ptr;
//reset current angular torque
   (*ocb_ptr).object_list[dyn_object].torque_sum.y=0;
   (*ocb_ptr).object_list[dyn_object].torque_sum.x=0;
   (*ocb_ptr).object_list[dyn_object].torque_sum.z=0;
}



//Apply a force to an object; handled linear and angular acceleration
void apply_a_force(int object, float magnitude, vector in_p /*oint*/ , vector in_d /*irection*/) 
{
vector torque,accel;
//vector temp;
vector p,d;	//converted to local space of in_p and 

float torqueMag;
extern DynObjectsFixedSize *ocb_ptr;
//int coeff;
double Or_mat[4][4];
float mag_frame;
//1. Take current object Quat and create orientation matrix
//2. Multiply both p and d to get rotated p and d

//translate Qor to orientation matrix
NewQuatToMat(&(*ocb_ptr).object_list[object].Qor,Or_mat);

VEC_MultMatrix(&in_p,Or_mat,&p);
VEC_MultMatrix(&in_d,Or_mat,&d);

 //convert direction into a unit vector (ie magnitude ==1)
// Vnormalise(&d);

// /*  The (linear) Force; magnitude calculated using Newton's 2nd Law;
//  *   F = ma -> a = F/m
//  *  direction is that of the d vector, but to work out the *sign* along..........
//  *  that axis, we need to use the following bit of entertaining trigonometry;
//  */
//
// /* coeff is either -1 or 1.
//  *  It is evaluated by determining if the distance from the origin to the end
//  * of the force vector is further from the origin than the start of the force
//  * vector. ie, if ( mod( p + d ) - mod (p) ) is greater than 0
//  */
//
////do direction
//Vmultiply(&d,coeff*(magnitude/(*ocb_ptr).object_list[object].mass),&accel);
mag_frame=magnitude*time_in_secs;	//time is seconds

Vmultiply(&d,(mag_frame/(*ocb_ptr).object_list[object].mass),&accel);


//apply linear component
(*ocb_ptr).object_list[object].abs_velocity_vector.x+=accel.x;
(*ocb_ptr).object_list[object].abs_velocity_vector.y+=accel.y;
(*ocb_ptr).object_list[object].abs_velocity_vector.z+=accel.z;


 /* To find the torque;
  *
  * y| p.----> d      P is the point the force is acting on, d is the direction.
  *  | /
  *  |/
  *  +----- x
  *
  *  What we're doing is resolving the force vector ( = d * magnitude) into two separate
  * components; the component perpendicular to the position vector, and the component along the position
  * vector. Since the position is a vector from the origin, this splits the applied force into
  * the part through the center of mass, and the component perpendicular to the center of mass,
  * which will only rotate the body.
  *
  *  We're trying to find the Torque vector, which has direction as the axis
  * the torque will produce rotation around, and magnitude equal to the magnitude
  * of the force times the perpendicular distance to the origin.
  *  This (rather conveniently :) is given by the cross (vector) product. You have
  * any idea how long it took me to notice that?
  *
  * nb;
  * Torque = Force * Distance
  * Torque = Rate of change of ang momentum
  */

 //Give d the magnitude of the force
 VmultiplyBy(&d,magnitude);

 //Find the torque vector
 Vcrossproduct(&d,&p,&torque);

 //And extract and store the magnitude of the torque
 torqueMag=vectorMod(&torque);

// add to cumlative torque vect total for this object
(*ocb_ptr).object_list[object].torque_sum.x+=torque.x;
(*ocb_ptr).object_list[object].torque_sum.y+=torque.y;
(*ocb_ptr).object_list[object].torque_sum.z+=torque.z;

}


void apply_extern_forces(_quat* QrotObj, vector* TorqueSum, float mass)
{
_quat deltaRot;
_quat rotVel;
_quat angMom;
double momentOfInertia = mass*1000;	//mass is in Tonnes and I want it in kgs
double rotQMag,root;

angMom.w=0;
angMom.x=TorqueSum->x*time_in_secs;
angMom.y=TorqueSum->y*time_in_secs;
angMom.z=TorqueSum->z*time_in_secs;

//angMom.x=0.0;
//angMom.y=0;
//angMom.z=0.005;

   //Increase the angular momentum by the torque/time (torque = rate of change of angular momentum)
//   angMom.x += sumTorque.x * seconds;
//   angMom.y += sumTorque.y * seconds;
//   angMom.z += sumTorque.z * seconds;

   //Calculate the angular velocity ( w = angular momentum / moment of Inertia = L / I)
   divide(&angMom,momentOfInertia,&rotVel);
   rotVel.w=0;
      
   quaternionMultiply(&rotVel,    QrotObj,    &deltaRot);

   //deltaRotV.multiplyBy(0.5 * seconds);
   //deltaRotS *= 0.5 * seconds;
   //rotQVector.incrementBy(&deltaRotV);
   
   QrotObj->x += deltaRot.x;
   QrotObj->y += deltaRot.y;
   QrotObj->z += deltaRot.z;
   
   QrotObj->w += deltaRot.w;

   //Normalise the quaternion
   root = sqrt((QrotObj->w*QrotObj->w) + (QrotObj->x*QrotObj->x) + (QrotObj->y*QrotObj->y) 
          + (QrotObj->z*QrotObj->z));
   if(root) rotQMag = (double)1.0 / root; //Prevent divide by 0 errors
    else rotQMag = 1.0;
    
   QrotObj->x *= rotQMag;
   QrotObj->y *= rotQMag;
   QrotObj->z *= rotQMag;
   QrotObj->w *= rotQMag;
}


// --------------------------------1 second functions used for camera
//Apply a force to an object; handled linear and angular acceleration
void apply_a_force_1sec(int object, float magnitude, vector in_p /*oint*/ , vector in_d /*irection*/) 
{
vector torque,accel;
//vector temp;
vector p,d;	//converted to local space of in_p and 

float torqueMag;
extern DynObjectsFixedSize *ocb_ptr;
//int coeff;
double Or_mat[4][4];
float mag_frame;
//1. Take current object Quat and create orientation matrix
//2. Multiply both p and d to get rotated p and d

//translate Qor to orientation matrix
NewQuatToMat(&(*ocb_ptr).object_list[object].Qor,Or_mat);

VEC_MultMatrix(&in_p,Or_mat,&p);
VEC_MultMatrix(&in_d,Or_mat,&d);

mag_frame=magnitude*1;	//time is seconds

Vmultiply(&d,(mag_frame/(*ocb_ptr).object_list[object].mass),&accel);


//apply linear component
(*ocb_ptr).object_list[object].abs_velocity_vector.x+=accel.x;
(*ocb_ptr).object_list[object].abs_velocity_vector.y+=accel.y;
(*ocb_ptr).object_list[object].abs_velocity_vector.z+=accel.z;



 //Give d the magnitude of the force
 VmultiplyBy(&d,magnitude);

 //Find the torque vector
 Vcrossproduct(&d,&p,&torque);

 //And extract and store the magnitude of the torque
 torqueMag=vectorMod(&torque);

// add to cumlative torque vect total for this object
(*ocb_ptr).object_list[object].torque_sum.x+=torque.x;
(*ocb_ptr).object_list[object].torque_sum.y+=torque.y;
(*ocb_ptr).object_list[object].torque_sum.z+=torque.z;

}


void apply_extern_forces_1sec(_quat* QrotObj, vector* TorqueSum, float mass)
{
_quat deltaRot;
_quat rotVel;
_quat angMom;
double momentOfInertia = mass*1000;	//mass is in Tonnes and I want it in kgs
double rotQMag,root;

angMom.w=0;
angMom.x=TorqueSum->x*1;
angMom.y=TorqueSum->y*1;
angMom.z=TorqueSum->z*1;


   //Calculate the angular velocity ( w = angular momentum / moment of Inertia = L / I)
   divide(&angMom,momentOfInertia,&rotVel);
   rotVel.w=0;
      
   quaternionMultiply(&rotVel,    QrotObj,    &deltaRot);

   //deltaRotV.multiplyBy(0.5 * seconds);
   //deltaRotS *= 0.5 * seconds;
   //rotQVector.incrementBy(&deltaRotV);
   
   QrotObj->x += deltaRot.x;
   QrotObj->y += deltaRot.y;
   QrotObj->z += deltaRot.z;
   
   QrotObj->w += deltaRot.w;

   //Normalise the quaternion
   root = sqrt((QrotObj->w*QrotObj->w) + (QrotObj->x*QrotObj->x) + (QrotObj->y*QrotObj->y) 
          + (QrotObj->z*QrotObj->z));
   if(root) rotQMag = (double)1.0 / root; //Prevent divide by 0 errors
    else rotQMag = 1.0;
    
   QrotObj->x *= rotQMag;
   QrotObj->y *= rotQMag;
   QrotObj->z *= rotQMag;
   QrotObj->w *= rotQMag;
}








//¥¥¥¥¥¥¥¥


//can return either distance or -1 if too far
double get_distance_to_zex(int object)
{
return  get_distance_to_from(0,object);
}

//can return either distance or -1 if too far
double get_distance_to_from(int Oto, int Ofrom)
{
vector to_pos,from_pos;
extern DynObjectsFixedSize *ocb_ptr;

to_pos.x=(*ocb_ptr).object_list[Oto].Oworldx;
to_pos.y=(*ocb_ptr).object_list[Oto].Oworldy;
to_pos.z=(*ocb_ptr).object_list[Oto].Oworldz;

from_pos.x=(*ocb_ptr).object_list[Ofrom].Oworldx;
from_pos.y=(*ocb_ptr).object_list[Ofrom].Oworldy;
from_pos.z=(*ocb_ptr).object_list[Ofrom].Oworldz;

return get_distance_to_from_point(from_pos.x,from_pos.y,from_pos.z,to_pos.x,to_pos.y,to_pos.z);
}


void get_vector_to(vector* v, int Oto, int Ofrom)
{
 _3D a,b;

  get_position_in_m(Oto, &a);
  get_position_in_m(Ofrom, &b);
  v->x=a.x-b.x; v->y=a.y-b.y; v->z=a.z-b.z;
  Vnormalise(v);
}

double get_distance_to_from_in_m(int Oto, int Ofrom)
{
_3D to,from;

get_position_in_m(Oto, &to);
get_position_in_m(Ofrom, &from);

return get_distance_apart(to,from);
}


double get_distance_apart(_3D a, _3D b)
{
_3D v;

v.x=a.x-b.x; v.y=a.y-b.y; v.z=a.z-b.z;
//return RA_line_len(v.x, v.y, v.z);
return sqrt( (v.x*v.x) + (v.y*v.y) + (v.z*v.z) );	// pythagorus
}

double get_length_of_vector(vector a)
{
  return RA_line_len(a.x, a.y, a.z);
}


//can return either distance or -1 if too far
double get_distance_to_from_point(double from_point_x,double from_point_y,double from_point_z,
                               double to_point_x,double to_point_y,double to_point_z)
{
double scal_x,scal_y,scal_z;
//int ret_val;
//float sqrt_in;

//scal_x=abs_nobranch(to_point_x-from_point_x);
//scal_y=abs_nobranch(to_point_y-from_point_y);
//scal_z=abs_nobranch(to_point_z-from_point_z);
scal_x=to_point_x-from_point_x;
scal_y=to_point_y-from_point_y;
scal_z=to_point_z-from_point_z;

//if (scal_x<0) scal_x=-scal_x;
//if (scal_y<0) scal_y=-scal_y;
//if (scal_z<0) scal_z=-scal_z;


//return RA_line_len(scal_x, scal_y, scal_z);
return sqrt(scal_x*scal_x+scal_y*scal_y+scal_z*scal_z);
}