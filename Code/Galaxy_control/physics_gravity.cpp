
#ifndef __ZEX_PCH__
  #include "zselector.h"
  #include "zplatform.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <fp.h>
  #endif

  #include "3d_structures_def.h"
  #include "zex_misc.h"
  #include "access_ocbs.h"
#endif //pch

#include "matrix_primitives.h"
#include "Physics.h"
#include "physics_gravity.h"


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

module_private void apply_one_gravity(int big_object);

module_private double return_stable_orbital_velocity(double mass_in_kg, double orbited_mass_in_kg, double distance_apart_in_m); /* returns m/s */
module_private double return_force_of_gravity(double mass1_in_kg, double mass2_in_kg, double distance_apart_in_m); /* returns Newtons */

module_private void STATIC_calculate_stable_orbit_vector(_3D *velocity, int orbiting_slot, int orbited_slot);
module_private void DOUBLE_calculate_stable_orbit_vector(_3D *velocity, int orbiting_slot, int orbited_slot1, int orbited_slot2);

// ***********************************************************************************
// * OUR FUNCTIONS
// *


void apply_gravity(int object)		// run once per frame to apply gravity to all objects
{
//extern int dynamic_object_index;
double this_mass;
//int active_object_counter,object;
//
//active_object_counter=0;
//object=0;
//while (active_object_counter<dynamic_object_index)
//  {
//    if (object_in_use(object))
//    {

	  #if REMOVE_GRAVITY_FROM_GAME
	    return;
	  #endif
	  
          #if TEST_SYSTEM==1
            return;
          #endif


    this_mass=get_mass_in_kg(object);

       if(this_mass > 1e9)	// check if it is a big object (1e9 = 1 million tonnes )
            {
            apply_one_gravity(object);
            }
//    active_object_counter++;
//    }
//  object++;
//  }
  
} // end of apply_gravity



void apply_one_gravity(int big_object)
{
extern int dynamic_object_index;
vector force;
int active_object_counter,object;

active_object_counter=0;
object=0;
while (active_object_counter<dynamic_object_index)
  {
    if (get_object_in_use(object))
    {
       if(big_object!=object)		// don't apply it to ourselves, cause thats stupid
            {
	    if(object_is_real(object))	// lets only effect objects that are real
	    		{
			calculate_gravity_force_vector(&force, object, big_object);
			apply_a_linear_force_world_reference(object,force);
			}
            }
    active_object_counter++;
    }
  object++;
  }
  
} // end of apply_one_gravity


// =====================================================================================
//
//


// ======================================================================================
// ------------------------------------------------------------------------------
//
// WORK OUT SPECIFIC VECTORS
//
// ------------------------------------------------------------------------------
void  force_object_orbit(int object_to_force, int object_to_orbit) // add another paramter - fixed offset - at a later date
{
_3D satellite_position, orbit_position;

#if TEST_SYSTEM == 1
return;
#endif

get_position_in_m(object_to_force,&satellite_position);

get_basic_orbit_position(object_to_orbit,&orbit_position);

satellite_position.x += orbit_position.x;
satellite_position.y += orbit_position.y;
satellite_position.z += orbit_position.z;

set_position_in_m(object_to_force,satellite_position);

set_stable_orbit_vector(object_to_force,object_to_orbit);
}

// =======================================================

void get_basic_orbit_position(int object_to_orbit, _3D *orbit_position)
{
double x1,z1, orbit_dist;

get_position_in_m(object_to_orbit, orbit_position);
orbit_dist = get_object_radius_in_metres(object_to_orbit)*1.1;  // orbit height (10%) plus radius of planet
//orbit_dist+=RangedRdm(0,100);		// random 0 to 100m away from object - for start up object loads
//angle=RangedRdm(0,359); x1=orbit_dist*(COS(angle)); z1=orbit_dist*(SIN(angle));
x1=0; z1=orbit_dist;	// remove this line, and add one above to make random orbit angle for forced objects

orbit_position->x+=x1;
orbit_position->z+=z1;
}

// =======================================================

void set_stable_orbit_vector(int object, int parent_object)
{
_3D velocity;
	  #if REMOVE_GRAVITY_FROM_GAME
	    return;
	  #endif

          #if TEST_SYSTEM==1
          return;
          #endif
return;

// lets set up the (hopefully) stable orbit velocity
        calculate_stable_orbit_vector(&velocity,object, parent_object);
	set_velocity_in_mps(object,velocity);
}


extern	DynObjectsFixedSize *ocb_ptr;

// =======================================================

void calculate_stable_orbit_vector(_3D *velocity, int orbiting_slot, int orbited_slot) 
{
_3D a,b,temp;
vector p;
double distance,velocity_magnitude;
          #if TEST_SYSTEM==1
          return;
          #endif

get_position_in_m(orbiting_slot, &a);
get_position_in_m(orbited_slot, &b);

temp.x=b.x; temp.y=40e20; /* randomly big number */  temp.z=b.z;	// this is the third point of the poly
calculate_normal(&p,&a,&b,&temp);

distance=get_distance_apart(a,b);

// we don't scale the units, as we are not converting to another metric
velocity_magnitude=return_stable_orbital_velocity(get_mass_in_kg(orbiting_slot),get_mass_in_kg(orbited_slot),distance);

// get velocity vector of orbited object - it no use orbiting something that is miles away...
get_velocity_in_mps(orbited_slot, velocity);

// add on this calculated orbit vector
velocity->x += p.x*velocity_magnitude;
velocity->y += p.y*velocity_magnitude;
velocity->z += p.z*velocity_magnitude;

}
// =======================================================

// does not take mass of parent - used for binary suns
void STATIC_set_stable_orbit_vector(int object, int parent_object)
{
_3D velocity;
	  #if REMOVE_GRAVITY_FROM_GAME
	    return;
	  #endif

          #if TEST_SYSTEM==1
          return;
          #endif

// lets set up the (hopefully) stable orbit velocity
        STATIC_calculate_stable_orbit_vector(&velocity,object, parent_object);
	set_velocity_in_mps(object,velocity);
}


extern	DynObjectsFixedSize *ocb_ptr;

// =======================================================

// does not take mass of parent - used for binary suns
void STATIC_calculate_stable_orbit_vector(_3D *velocity, int orbiting_slot, int orbited_slot) 
{
_3D a,b,temp;
vector p;
double distance,velocity_magnitude;

get_position_in_m(orbiting_slot, &a);
get_position_in_m(orbited_slot, &b);

temp.x=b.x; temp.y=40e20; /* randomly big number */  temp.z=b.z;	// this is the third point of the poly
calculate_normal(&p,&a,&b,&temp);

distance=get_distance_apart(a,b);

// we don't scale the units, as we are not converting to another metric
velocity_magnitude=return_stable_orbital_velocity(get_mass_in_kg(orbiting_slot),get_mass_in_kg(orbited_slot),distance);

// add on this calculated orbit vector
velocity->x = p.x*velocity_magnitude;
velocity->y = p.y*velocity_magnitude;
velocity->z = p.z*velocity_magnitude;

}

// =======================================================

// need to orbit around double mass
void DOUBLE_set_stable_orbit_vector(int object, int sun_slot1, int sun_slot2)
{
_3D velocity;
	  #if REMOVE_GRAVITY_FROM_GAME
	    return;
	  #endif

          #if TEST_SYSTEM==1
          return;
          #endif

// lets set up the (hopefully) stable orbit velocity
        DOUBLE_calculate_stable_orbit_vector(&velocity,object,sun_slot1,sun_slot2);
	set_velocity_in_mps(object,velocity);
}

// =======================================================

void DOUBLE_calculate_stable_orbit_vector(_3D *velocity, int orbiting_slot, int orbited_slot1, int orbited_slot2)
{
_3D a,b,temp;
vector p;
double distance,velocity_magnitude;

a.x=0; a.y=0; a.z=0;		// center of solar system
get_position_in_m(orbiting_slot, &b);

temp.x=b.x; temp.y=40e20; /* randomly big number */  
temp.z=b.z;	// this is the third point of the poly
calculate_normal(&p,&a,&b,&temp);

distance=get_distance_apart(a,b);

// we don't scale the units, as we are not converting to another metric
velocity_magnitude=return_stable_orbital_velocity(get_mass_in_kg(orbiting_slot),get_mass_in_kg(orbited_slot1)+get_mass_in_kg(orbited_slot2),distance);

// add on this calculated orbit vector
velocity->x = p.x*velocity_magnitude;
velocity->y = p.y*velocity_magnitude;
velocity->z = p.z*velocity_magnitude;

}




// =======================================================
//
//void calculate_stable_orbit_vector(_3D *velocity, int orbiting_slot, int orbited_slot) 
//{
//_3D a,b,temp;
//vector p;
//double distance,velocity_magnitude;
//
//position_in_m(orbiting_slot, &a);
//position_in_m(orbited_slot, &b);
//
//temp.x=b.x; temp.y=40e20; /* randomly big number */  temp.z=b.z;	  // this is the third point of the poly
//calculate_normal(&p,a,b,temp);
//
//distance=get_distance_apart(a,b);
//
//// we don't scale the units, as we are not converting to another metric
//velocity_magnitude=return_stable_orbital_velocity(mass_in_kg(orbiting_slot),mass_in_kg(orbited_slot),distance);
//
//// get velocity vector of orbited object - it no use orbiting something that is miles away...
//velocity_in_mps(orbited_slot, velocity);
//
//// add on this calculated orbit vector
//velocity->x += p.x*velocity_magnitude;
//velocity->y += p.y*velocity_magnitude;
//velocity->z += p.z*velocity_magnitude;
//
//}


//calculate_gravity_force(&vec_x,&vec_y,&vec_z,target_obj_slot1,source_obj_slot2); // effect on slot1 of slot2
// \..distance   subtract points, then sqrt(sum of squared points) - maybe function
//  ..vector  subtract points
//  ..feed into calc gravity
//  ..multiply vector by gravity 
//  .. F=ma, so a=F/m - but isn't this done by engine?

void calculate_gravity_force_vector(vector *force,int target_obj_slot,int source_obj_slot)
{
_3D a,b,v;
double am,bm;
double distance_apart,force_mag;

get_position_in_m(target_obj_slot,&a);
am=get_mass_in_kg(target_obj_slot);
get_position_in_m(source_obj_slot,&b);
bm=get_mass_in_kg(source_obj_slot);

distance_apart=get_distance_apart(a,b);
force_mag=return_force_of_gravity(am,bm,distance_apart ); 

v.x=b.x-a.x; v.y=b.y-a.y; v.z=b.z-a.z;		// coordinate difference - opposite to cause attraction
v.x/=distance_apart; v.y/=distance_apart; v.z/=distance_apart;	// make vector normalised.
      
v.x*=force_mag; v.y*=force_mag; v.z*=force_mag;		// scale by size of force

force->x=v.x; 
force->y=v.y; 
force->z=v.z;		// return values, as a Newton scaled force vector

}




// ------------------------------------------------------------------------------
//
//              B A S E   P H Y S I  C S   E M B E D E D 
//
// ------------------------------------------------------------------------------

// zex units in meters 
// mass in tonnes

double return_stable_orbital_velocity(double mass_in_kg, double orbited_mass_in_kg, double distance_apart_in_m) /* returns m/s */
{
double force;
double angular_velocity;

/* CENTRIFUGAL FORCE - the force that appears to act outwards on bodies rotated in a
circular path. The centrifugal force is directed along the radius of the path and is
equal to mrw^2, where m is the mass of the body, r is the distance from the center 
of rotation, and w is the angular velocity (in radians per second) of the body.
(from Dictionary of space) */

/* therefore sqrt(force/(m*r)) = w 
   force = force-of-attraction (gravity) = F = G m1 m2 / d^2 , 
   where G = 6.672 x 10^-11 Nm^2kg^-2
   masses are in kg, distances in metres. 
   Additional note: at a point outside their physical size, gravity can be 
   considered a point source */

/* if w is in 1 radian/sec the object must cover 2.PI.r metres per second. So if
w is 0.5 r/s the object must cover PI r meters per second. Therefore, 
velocity = w * PI * r 
*/

force = return_force_of_gravity(mass_in_kg,orbited_mass_in_kg,distance_apart_in_m);

angular_velocity=sqrt(force/(mass_in_kg*distance_apart_in_m));		// distance apart is radian

return (angular_velocity /* 2 * PI */ * distance_apart_in_m);

}


#define GRAVITATIONAL_CONSTANT 6.672e-11

double return_force_of_gravity(double mass1_in_kg, double mass2_in_kg, double distance_apart_in_m) /* returns Newtons */
{
return (GRAVITATIONAL_CONSTANT * mass1_in_kg * mass2_in_kg) / (distance_apart_in_m * distance_apart_in_m);
}

