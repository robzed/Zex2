// *************************************MODULE HEADER*********************************
// * FILENAME           : physics_gravity.h
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * DATE STARTED       : Sun 10:47:29 on 8th Oct 2000
// * FIRST AUTHOR       : Rob Probin
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************

// complete position and velocity solution
void force_object_orbit(int object_to_force, int object_to_orbit);
void get_basic_orbit_position(int object_to_orbit, _3D *orbit_position);

// high level routines
void apply_gravity(int object);		// run once per frame to apply gravity to all objects

void set_stable_orbit_vector(int object, int parent_object);


// low level gravity based routines

void calculate_stable_orbit_vector(_3D *velocity, int orbiting_slot, int orbited_slot);
void calculate_gravity_force_vector(vector *force,int target_obj_slot,int source_obj_slot);

void set_stable_orbit_vector(int object, int parent_object);
void STATIC_set_stable_orbit_vector(int object, int parent_object);
void DOUBLE_set_stable_orbit_vector(int dyn_slot, int sun_slot1, int sun_slot2);



