// *************************************MODULE HEADER*********************************
// * FILENAME           : collision.cpp
// * MODULE TITLE       : physics
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 15:08:41 on 11th June 2000
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
 * $Log: collision.cpp,v $
 * Revision 1.5  2003/09/28 10:36:02  robp
 * Signed/Unsigned comparison fixes from last night, plus collision fix.
 *
 * Revision 1.4  2003/09/27 20:41:54  robp
 * Fixed float->int warnings.
 *
 * Revision 1.3  2003/09/20 12:57:08  robp
 * Removed nested comments
 *
 * Revision 1.2  2003/09/16 20:55:38  stu_c
 * Zex: particles: Fix for control freeze if hit by gun particles.
 *
 * Revision 1.1.1.1  2003/09/05 22:35:10  stu_c
 * First Imported.
 *
 * Revision 1.10  2002/09/14 02:39:34  stu_c
 * Some tidy up work and shells now killed on collision
 *
 * Revision 1.9  2002/09/09 21:45:02  rob_c
 * New collision calculations
 *
 * Revision 1.8  2002/09/08 02:12:19  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.7  2002/03/09 15:10:44  stu
 * Work for complex objects docking other objects - tracing the bolts back to parent object.
 *
 * Revision 1.6  2002/01/27 18:12:04  stu
 * No diff
 *
 * Revision 1.5  2001/12/14 19:43:54  stu
 * Docking
 *
 * Revision 1.4  2001/11/02 19:32:43  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.3  2001/10/22 22:12:11  stu
 * More Carbon work
 *
 * Revision 1.2  2001/10/22 21:29:05  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:09:17  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:04  rob
 * First Imported.
 *
 * Revision 1.3  2001/09/04 22:42:16  stu
 * SDS
 *
 * Revision 1.2  2001/06/23 20:01:17  stu
 * 0.66 checkin
 *
 * Revision 1.1.1.1  2001/01/01 21:13:34  rob
 * First Imported.
 *
 * Revision 1.1  2000/10/19 19:08:34  stu
 * Relative velocities, turbulence, planet names.
 *
 * Revision 1.0.0.1  2000/08/21 21:59:39  stu
 * First Imported.
 *
 */

// ***********************************************************************************
// * INCLUDED FILES
// *

#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "3d_structures_def.h"
#endif //pch

#include	"object_control_top.h"
#include	"objects_controls.h"	//controller definitions
#include "physics.h"
#include "matrix_primitives.h"
#include "collision.h"
#include "Zsound.h"
#include "particles.h"
#include "obj_control_misc_routines.h"
#include "load_objects.h"
#include "camera_control.h"
#include "access_ocbs.h"

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *
void check_collision(ZObject * the_object_ptr, ZObject * compare_object_ptr);

void Cdamage_object(ZObject* the_object_ptr, int damage);

void run_collision_processing(ZObject* object_1_ptr, ZObject* object_2_ptr);

void insert_shield(ZObject* which_object);

// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

extern DynObjectsFixedSize *ocb_ptr;
extern int dynamic_object_index;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

void handle_collision_velocities(vector* pos_obj1, vector* vel_obj1, float mass1, double *change_in_vel1,
                            vector* pos_obj2, vector* vel_obj2, float mass2, double *change_in_vel2);


// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE              : 
// | AUTHOR(s)          :
// | DATE STARTED       : 
// +----------------------------------------------------------------------------------
// | DESCRIPTION:  
// | 
// | 
// |
// |
// +----------------------------------------------------------------------------------
// | PARAM IN   : none
// |
// | PARAM OUT  : none
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial                Descrip.
// |
// |
// +----------------------------------------------------------------------------------
// | ADDITIONAL NOTES:
// | 


int collisions = 1;

//run through all objects checking if this object has collided with em
//we can whittle this down a bit:
//if the object is not collidable then bugger off
void run_collision_detection(int object)
{
register ZObject * current_object_ptr;
register ZObject * compare_object_ptr;
int object_counter,active_object_counter;


    if(!collisions) return;

  current_object_ptr=&ocb_ptr->object_list[object];
//if collision detection is enabled for this object...  
  if (current_object_ptr->crashable==1)
   {
    //run through all objects checking collisions; we can optimise this lots and lots and lots
    //for example splitting the world up into squares and only considering objects in this objects square etc
    
    active_object_counter=0;
    object_counter=0;


    while (active_object_counter<dynamic_object_index)
     {
       compare_object_ptr=&ocb_ptr->object_list[object_counter];

       if (compare_object_ptr->in_use==1 ) //if object is alive and this object is not the same object
       {
         if (compare_object_ptr->crashable==1 && active_object_counter!=object)
          {
          check_collision(current_object_ptr, compare_object_ptr);
          
          }	//end of if compare_obj is crashable
          
          
         active_object_counter++;
       }	//end of compare object in use
        
      object_counter++;  
     }	//end of while
    
   }	//end of current object is crashable
   

}	//end of func



//this routine checks one object against another for collisions and does 'the right thing'
//it also sets hit_by_slot in each object so a controller can check if it has been hit
void check_collision(ZObject* the_object_ptr, ZObject* compare_object_ptr)
{
float distance,radii_distance;
ZObject* collision_ptr;
int docking_what_object;

//vector force_vector;
//first check for MS10 hitting MS11's; this is acceptable
if (the_object_ptr->Oid=='MS10' && compare_object_ptr->Oid=='MS11') return; 
if (the_object_ptr->Oid=='MS11' && compare_object_ptr->Oid=='MS10') return; 

if (the_object_ptr->Dyn_OCB_control_data.controller== MOTHERSHIP_CONTROL && compare_object_ptr->Dyn_OCB_control_data.controller==MOTHERSHIP_PART) return; 
if (the_object_ptr->Dyn_OCB_control_data.controller== MOTHERSHIP_PART && compare_object_ptr->Dyn_OCB_control_data.controller==MOTHERSHIP_CONTROL) return; 
if (the_object_ptr->Dyn_OCB_control_data.controller== MOTHERSHIP_PART && compare_object_ptr->Dyn_OCB_control_data.controller==MOTHERSHIP_PART) return; 

//stop us doing to collisions per object
if (   the_object_ptr->hit_by_slot== ptr_to_dynslot(compare_object_ptr)) 
   {
	return;
    }

//If the object is being docked by a mothership
//    
//   }
//check sphere; basically a distance calculation compared to the addition of each objects
//radius
distance=get_distance_to_from_point(the_object_ptr->Oworldx,the_object_ptr->Oworldy,the_object_ptr->Oworldz,
                               compare_object_ptr->Oworldx,compare_object_ptr->Oworldy,compare_object_ptr->Oworldz);

radii_distance=the_object_ptr->radius+compare_object_ptr->radius;

if (radii_distance>distance)	//collision according to the radius calcs
   {
   vector pos_vector1, pos_vector2;
   float damage1, damage2, damage_each;
   double change_in_vel1, change_in_vel2;
   unsigned int bolted_to;
   
   
//Check if it's a mothership docking something - we do not colide in this case
  docking_what_object=the_object_ptr->Dyn_OCB_control_data.docking_what;
  if (docking_what_object!=-1)
  {

  //convert docking what to a pointer and compare against the thing we have collided with
  collision_ptr=&ocb_ptr->object_list[docking_what_object];
  if (collision_ptr==compare_object_ptr) return;	//the_object_ptr is docking collision_ptr
  }

 //also if this object is bolted to another, is  it a mothership docking an object?
 bolted_to=the_object_ptr->bolted_to;
 if (bolted_to>0)
 {
  docking_what_object=(*ocb_ptr).object_list[bolted_to].Dyn_OCB_control_data.docking_what;
  if (docking_what_object!=-1)
  {
    //convert docking what to a pointer and compare against the thing we have collided with
    collision_ptr=&ocb_ptr->object_list[docking_what_object];
    if (collision_ptr==compare_object_ptr) return;	//the_object_ptr is docking collision_ptr
  }
  
 }


//Check the other way round
docking_what_object=compare_object_ptr->Dyn_OCB_control_data.docking_what;
if (docking_what_object!=-1)
{
 //convert docking what to a pointer and compare against the thing we have collided with
 collision_ptr=&ocb_ptr->object_list[docking_what_object];
 if (collision_ptr==the_object_ptr) return;	//the_object_ptr is docking collision_ptr
}

 //also if this object is bolted to another, is  it a mothership docking an object?
 bolted_to=compare_object_ptr->bolted_to;
 if (bolted_to>0)
 {
  docking_what_object=(*ocb_ptr).object_list[bolted_to].Dyn_OCB_control_data.docking_what;
  if (docking_what_object!=-1)
  {
    //convert docking what to a pointer and compare against the thing we have collided with
    collision_ptr=&ocb_ptr->object_list[docking_what_object];
    if (collision_ptr==the_object_ptr) return;	//the_object_ptr is docking collision_ptr
  }
  
 }

//
// We have a legal collision here.
//

// store positions as vectors (these don't get altered, so no need to put them back in ocb's)
pos_vector1.x = the_object_ptr->Oworldx; 
pos_vector1.y = the_object_ptr->Oworldy; 
pos_vector1.z = the_object_ptr->Oworldz; 

// store positions as vectors (these don't get altered, so no need to put them back in ocb's)
pos_vector2.x = compare_object_ptr->Oworldx; 
pos_vector2.y = compare_object_ptr->Oworldy; 
pos_vector2.z = compare_object_ptr->Oworldz; 

// this alters the object velocities, and return damage for each object
handle_collision_velocities(&pos_vector1, &the_object_ptr->abs_velocity_vector,     the_object_ptr->mass,     &change_in_vel1, 
                                 &pos_vector2, &compare_object_ptr->abs_velocity_vector, compare_object_ptr->mass, &change_in_vel2);
        

	//set collided with ptrs so we don't do them again
	the_object_ptr->hit_by_slot=ptr_to_dynslot(compare_object_ptr);
	compare_object_ptr->hit_by_slot = ptr_to_dynslot(the_object_ptr);
	
	//now, we need to translate the acceleration into damage...Hmmm...
	//we really can't get into complex elasticity calculations here so I'll pull
	//some figures out of the air.
	//If an object has mass of 1 tonne and it is instantaneously accelerated by 100m/S then 
	//it will suffer 100 dq's (damage quotient)
	//thus an object of mass 20 tonnes needs to be acclerated by 5m/S to suffer 100 dq's
	
	//Hulls have variable max dq; for example a good starting point would be for Zex to have a hull
	//of 100 dq, and a shield of 100 dq.
	
	//A collision with an object which results in a dq of 100 would wipe out the shields but not touch the
	//hull. Conversly a collision which resulted in 200 dq's would destroy Zex.
	//Shields are regenerative, hulls need repair when docked.
	
	//Most ships have shields and a hull. Most objects just have a hull rating
	
	//When damaging shields, we take the shields class (1 upwards) into account. This is simply 
	//a divisor for the dq

	//accel=(Vft1.x+Vft1.y+Vft1.z);	//this should be sqr(x*x+y*y+z*z)
        //damage=accel/compare_object_ptr->mass*10;
	
// ****DAMAGE***
// We do the same as Stu was doing:
//		change-in-Velocity * mass 
// This is the change in inertia - and use this as damage.
// 
        damage1 = change_in_vel1 * the_object_ptr->mass;
        damage2 = change_in_vel2 * compare_object_ptr->mass;
// we do something different here though - we say that each object has the same damage - half of the combined damage
        damage_each = (damage1 + damage2) / 2;
        
// now we apply this to the objects
        if(damage_each > 0)		// only apply if there IS damage (i.e. damage!=0) and no negative damage allowed!
          {
          Cdamage_object(compare_object_ptr,STATIC_CAST_TO_INT(damage_each));

          // old code
	  //accel=(Vft2.x+Vft2.y+Vft2.z);
          //damage=accel/the_object_ptr->mass*10;


// ===================
          if(the_object_ptr->Oid=='ZEXS')
          {
             play_zsound(large_shield_sound, sound_low_pri, stereo1_chan, sound_vol_7);
          }

          Cdamage_object(the_object_ptr,STATIC_CAST_TO_INT(damage_each));
          
          run_collision_processing(the_object_ptr, compare_object_ptr); //check for bullets, shields explosions etc

//if it is us that has hit the object then play a sound
       } // end of if(damage_each > 0)
       
   } // end of if(radii_distance>distance)
 
} // end of check_collision()


// OVERVIEW
// ========
// these functions sort out what the velocities end up as
//

// Name: calc_unit_vector_between
// Auth: Rob Probin
// Date: 11/8/2002 (for insertion into Zex from BASIC)
// Desc: Calculates a unit vector between two objects
vector calc_unit_vector_between(vector* pobj1, vector* pobj2)
{
vector DIFF;
double mag;

DIFF.x = pobj1->x - pobj2->x;
DIFF.y = pobj1->y - pobj2->y;
DIFF.z = pobj1->z - pobj2->z;

// normalise the vector (i.e. create a unit vector)
mag=sqrt(DIFF.x*DIFF.x + DIFF.y*DIFF.y + DIFF.z*DIFF.z);
//   mag=RA_line_len(DIFF.x,DIFF.y,DIFF.z);	// haven't tried - don't know what accuracy I need

DIFF.x/=mag;
DIFF.y/=mag;
DIFF.z/=mag;

return DIFF;
}

// Name: calc_vel_vector_in_direction
// Auth: Rob Probin
// Date: 11/8/2002 (for insertion into Zex from BASIC)
// Desc: 
double calc_vector_in_direction( vector* obj1, vector* obj2)
{
// this is dot product
//        = AB Cos <angle>
//        = x1x2 + y1y2 + z1z2
// used for getting the vector component in the direction of something
return obj1->x*obj2->x + obj1->y*obj2->y + obj1->z*obj2->z;
}


// Name: calc_final_velocities
// Auth: Rob Probin
// Date: 11/8/2002 (for insertion into Zex from BASIC)
// Desc: Calculate final velocities conservation of momentum and energy
vector calc_final_velocities(vector* vobj1, float mass1, vector* vobj2, float mass2)
{
// v1 = v1i * (m1-m2) / (m1+m2) + v2i *  (2*m2) / (m1+m2)
// v2 = v1i * (2*m2)  / (m1+m2) + v2i * (m2-m1) / (m1+m2)
vector FVEL;

 FVEL.x =  vobj1->x * ( (mass1-mass2) / (mass1+mass2) );
 FVEL.x += vobj2->x * ( (2*mass2)     / (mass1+mass2) );

 FVEL.y =  vobj1->y * ( (mass1-mass2) / (mass1+mass2) );
 FVEL.y += vobj2->y * ( (2*mass2)     / (mass1+mass2) );

 FVEL.z =  vobj1->z * ( (mass1-mass2) / (mass1+mass2) );
 FVEL.z += vobj2->z * ( (2*mass2)     / (mass1+mass2) );

return FVEL;
}

// Name: calc_final_velocity
// Auth: Rob Probin
// Date: 11/8/2002 (for insertion into Zex from BASIC)
// Date: 08/9/2002 (for making into non-vector form)
// Desc: Calculate final velocities conservation of momentum and energy
double calc_final_velocity(double vobj1, float mass1, double vobj2, float mass2)
{
// v1 = v1i * (m1-m2) / (m1+m2) + v2i *  (2*m2) / (m1+m2)
// v2 = v1i * (2*m2)  / (m1+m2) + v2i * (m2-m1) / (m1+m2)
double final_vel;

 final_vel =  vobj1 * ( (mass1-mass2) / (mass1+mass2) );
 final_vel += vobj2 * ( (2*mass2)     / (mass1+mass2) );

return final_vel;
}


// Name: handle_collision_velcities
// Auth: Rob Probin
// Date: 11/8/2002 (for insertion into Zex from BASIC)
// Desc: This function modifies the velocities of two objects that have collided.
void handle_collision_velocities(vector* pos_obj1, vector* vel_obj1, float mass1, double *change_in_vel1,
                            vector* pos_obj2, vector* vel_obj2, float mass2, double *change_in_vel2)
// ???_obj1 is the same as the BASICs index 0
// ???_obj2 is the same as the BASICs index 1
{
vector UNIT1,UNIT2;	// directional vector between objects (positional)
double dir_vel1, dir_vel2;	// magnitude of velocity in direction of impact
vector DIR1, DIR2;	// velocity vectors in direction of impact
vector PERP1, PERP2;	// velocity vectors not in direction of impact
vector FINAL_D1, FINAL_D2;	// final directional velocities
double post_dir_vel1, post_dir_vel2;	// magnitude of velocity in direction of impact

  // *** calc unit between 1 and 0****
  UNIT1 = calc_unit_vector_between( pos_obj2, pos_obj1);

  // *** calc unit between 0 and 1 (opposite of above) ****
  UNIT2.x = -UNIT1.x;  UNIT2.y = -UNIT1.y;  UNIT2.z = -UNIT1.z;
  
  // *** calculate magnitude in direction of impact
  dir_vel1 = calc_vector_in_direction(vel_obj1, &UNIT1);

  // *** calculate velocity vector in direction of impact ***
  DIR1.x = dir_vel1*UNIT1.x; DIR1.y = dir_vel1*UNIT1.y; DIR1.z = dir_vel1*UNIT1.z;

  // *** calculate magnitude in direction of impact
  dir_vel2 = calc_vector_in_direction(vel_obj2, &UNIT2);

  if((dir_vel1+dir_vel2) > 0)	// ensure these will hit!!
    {
    // *** calculate velocity vector in direction of impact ***
    DIR2.x = dir_vel2*UNIT2.x; DIR2.y = dir_vel2*UNIT2.y; DIR2.z = dir_vel2*UNIT2.z;

    // *** calculate velocity vector not in direction of impact
    Vsub(vel_obj1, &DIR1, &PERP1);

    // *** calculate velocity vector not in direction of impact
    Vsub(vel_obj2, &DIR2, &PERP2);

    // *** calculate directional final velocities
    FINAL_D1 = calc_final_velocities(&DIR1,mass1,&DIR2,mass2);

    // *** what are the actual final velocities then?
    vel_obj1->x = FINAL_D1.x + PERP1.x;
    vel_obj1->y = FINAL_D1.y + PERP1.y;
    vel_obj1->z = FINAL_D1.z + PERP1.z;

    // *** calculate directional final velocities
    FINAL_D2 = calc_final_velocities(&DIR2,mass2,&DIR1,mass1);

    // *** what are the actual final velocities then?
    vel_obj2->x = FINAL_D2.x + PERP2.x;
    vel_obj2->y = FINAL_D2.y + PERP2.y;
    vel_obj2->z = FINAL_D2.z + PERP2.z;

    // lets find the magnitude of the velocity after the impact cause by the velocity in the direction of impact
    post_dir_vel1 = calc_vector_in_direction(vel_obj1, &UNIT1);
    post_dir_vel2 = calc_vector_in_direction(vel_obj2, &UNIT2);

// now calculate the change in velocities - this will be used in the collision calculations
    *change_in_vel1 = fabs(dir_vel1-post_dir_vel1);
    *change_in_vel2 = fabs(dir_vel2-post_dir_vel2);     
    } // if actually going to collide
  else
    {
    // not going to collide
    *change_in_vel1=0;	// this is used to calculate damage - no collision, no damage
    *change_in_vel2=0;  // this is used to calculate damage - no collision, no damage
    }
    
}






// This function does the damage stuff
//
void Cdamage_object(ZObject* the_object_ptr, int damage)
{
int shield_class;
  
  damage/=64;	//playability frig
  
  shield_class=   the_object_ptr->Dyn_OCB_control_data.shields1_class;
  if(shield_class>1) damage/=shield_class;
  
  if(the_object_ptr->Dyn_OCB_control_data.shield_value>damage)
  the_object_ptr->Dyn_OCB_control_data.shield_value-=damage;
  else
  {
   damage-=the_object_ptr->Dyn_OCB_control_data.shield_value;
   the_object_ptr->Dyn_OCB_control_data.shield_value=0;
  the_object_ptr->Dyn_OCB_control_data.hull_value-=damage;
  
  if(the_object_ptr->Dyn_OCB_control_data.hull_value<0)
  {
	//kill object here
  }
  
  
  }
}



void clear_collided_withs()
{
register ZObject * current_object_ptr;
int object_counter,active_object_counter;
    active_object_counter=0;
    object_counter=0;


    while (active_object_counter<dynamic_object_index)
     {
       current_object_ptr=&ocb_ptr->object_list[object_counter];

       if (current_object_ptr->in_use==1 ) //if object is alive and this object is not the same object
       {
          current_object_ptr->hit_by_slot = -1;
          
         active_object_counter++;
       }	//end of compare object in use
        
      object_counter++;  
     }	//end of while

}


//this routine gets called when two objects collide. It is passed pointers to
//the 2 objects.
//It deals with:
//1. Removing cannon shells on collision
//2. Exploding objects
//3. generating shield graphics
//4. Anything else needs doing when two things have collided
void run_collision_processing(ZObject* object_1_ptr, ZObject* object_2_ptr)
{

  //Check for bullets
  if (object_1_ptr->Dyn_OCB_control_data.controller==CANNON_CONTROL_1) //gun?
  {
    //control_1 contains the parent object (VT)
    //control_2 contains trigger ref
    //control_4 contains the parent of the vt
    kill_gun_particles(object_1_ptr->Dyn_OCB_control_data.control_use_1, object_1_ptr->Dyn_OCB_control_data.control_use_4, object_1_ptr->Dyn_OCB_control_data.control_use_2);
  }

  if (object_2_ptr->Dyn_OCB_control_data.controller==CANNON_CONTROL_1) //gun?
  {
    //control_1 contains the parent object (VT)
    //control_2 contains trigger ref
    //control_4 contains the parent of the vt
    kill_gun_particles(object_2_ptr->Dyn_OCB_control_data.control_use_1, object_2_ptr->Dyn_OCB_control_data.control_use_4, object_2_ptr->Dyn_OCB_control_data.control_use_2);
  }

  //check for death
  
  //check if we need to set off a shield object
  if (object_1_ptr->Dyn_OCB_control_data.shield_value>0) //typically 0-100
  {
    insert_shield(object_1_ptr);
  }

  //check if we need to set off a shield object
  if (object_2_ptr->Dyn_OCB_control_data.shield_value>0) //typically 0-100
  {
    insert_shield(object_2_ptr);
  }
  
}




void insert_shield(ZObject* which_object_ptr)
{
int dest_ocb,temp_debug;
load_dyn_object_pb the_params;
float scale;
extern	DynObjectsFixedSize *ocb_ptr;
_quat temp_quat;
    
    if (which_object_ptr->showing_shield!=0) return;
    which_object_ptr->showing_shield=1;
    
    clear_pb(&the_params);
    the_params.world_x=which_object_ptr->Oworldx;
    the_params.world_y=which_object_ptr->Oworldy;
    the_params.world_z=which_object_ptr->Oworldz;

    the_params.delta_rot_z=1;
    
    the_params.controller_ref=LARGE_SHIELD;
    the_params.object_category=NO_RADAR;
    temp_debug=ptr_to_dynslot(which_object_ptr);
    the_params.control_use_1=temp_debug;	//so kill_laser_particles can kill the right ones	
    the_params.control_use_1_float=2.0;	//2 seconds to live	
    the_params.mass=0.01;
    
    //calc scaling - the large shield is 100 units as loaded
    scale=which_object_ptr->radius/100;
    
    dest_ocb=find_vacant_dynamic_object_slot();
    load_dyn_object(17,dest_ocb,&the_params,-1,scale,-1,!DUST_BIT); //obj 28=shield 100
    
    //make shield face camera
    temp_quat=(*ocb_ptr).object_list[CAMERA_OBJECT].Qor;   
    temp_quat.x=-temp_quat.x;
    temp_quat.y=-temp_quat.y;
    temp_quat.z=-temp_quat.z;
    temp_quat.w=-temp_quat.w;
    (*ocb_ptr).object_list[dest_ocb].Qor=temp_quat;
}