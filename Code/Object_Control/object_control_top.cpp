//object control top
/*
$Log: object_control_top.c,v $
Revision 1.1.1.1  2003/09/05 22:36:08  stu_c
First Imported.

Revision 1.34  2002/09/14 02:38:43  stu_c
Some tidy up work and shells now killed on collision

Revision 1.33  2002/09/08 02:16:02  stu_c
Precompiled header adjustments

Revision 1.32  2002/07/28 17:10:33  stu_c
Exhausts and further particle work

Revision 1.31  2002/06/16 23:19:37  stu
Particles booked in for backup purposes - not working yet or complete.

Revision 1.30  2002/05/31 23:54:46  stu
Removed all warnings from the code

Revision 1.29  2002/05/19 03:17:43  stu
Guns fix

Revision 1.28  2002/05/05 22:27:06  stu
Further developement - hull temperature now correct for multiple suns and atmosphere.

All calls to add_to_kill_list check the return

Revision 1.27  2002/04/21 14:46:31  stu
Further work

Revision 1.26  2002/04/14 16:00:16  stu
Further work

Revision 1.25  2002/04/13 15:30:25  stu
Rotation, Alycians, behaviour

Revision 1.24  2002/04/01 22:55:11  rob
space change

Revision 1.23  2002/03/19 23:21:21  stu
Dispatch control to shuttle

Revision 1.22  2002/02/23 18:56:59  stu
Removed some unused locals

Revision 1.21  2002/02/16 19:15:27  stu
DLP_BLUE changed to DLP_PURP

Revision 1.20  2002/02/12 23:57:56  rob
Kill kill list always resets kill list
Added new dock code to dock.

Revision 1.19  2002/02/10 21:42:28  stu
run_fast_object_control

Revision 1.18  2002/02/03 19:27:49  stu
Planet LOD now honours low_res_planets

Revision 1.17  2002/02/02 23:07:39  stu
Planet LOD

Revision 1.16  2002/01/29 22:58:59  rob
anvil complains about unsigned int

Revision 1.15  2002/01/28 23:23:26  stu
Planets now unblobbed at a much arther distance

Revision 1.14  2002/01/20 17:04:00  stu
Deleted old star controller.

Revision 1.13  2002/01/09 23:49:05  stu
Furhter work on trading.

Revision 1.12  2001/12/29 01:11:55  stu
Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.

Revision 1.11  2001/12/28 23:12:10  stu
kill now just silently returns if trying to kill an already killed object. Used to abend.

Revision 1.10  2001/12/27 19:46:51  stu
Added conditional logging of debug messages

Revision 1.9  2001/12/14 19:44:05  stu
Docking

Revision 1.8  2001/12/04 23:36:19  stu
Motherships work

Revision 1.7  2001/12/02 19:31:03  stu
Motherships

Revision 1.6  2001/11/24 19:28:19  stu
Added FAlcon controller

Revision 1.5  2001/11/05 20:00:33  stu
More OSX work

Revision 1.4  2001/11/02 23:19:38  stu
Further OS X work. Enabled syscon logging.

Revision 1.3  2001/11/02 19:32:13  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.2  2001/10/22 21:28:18  rob
Carbon warning changes

Revision 1.1  2001/10/21 01:05:48  stu
Initial porting work

Revision 1.0.0.1  2001/10/17 20:46:07  rob
First Imported.

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

  #include "3d_structures_def.h"
  #include	"zex_misc.h"
  #include "processor.h"
  #include "access_ocbs.h"
  #include "error_handler.h"
  #include "real_time.h"
#endif

#include	"object_control_top.h"
#include	"objects_controls.h"	//controller definitions
#include	"load_objects.h"
#include "explode.h"
#include "dust.h"
#include "Zsound.h"
#include "3d_top.h"
#include "dlp.h"
#include "objects_misc.h"
#include "ship_control.h"
#include "check_bounding_boxes.h"
#include "score_and_credits.h"
#include "instrum.h"
#include "dlp_explosions.h"
#include "angular_stuff.h"
#include "obj_control_misc_routines.h"
#include "cargo.h"
#include "physics.h"
#include "collision.h"
#include "matrix_primitives.h"
#include "hud.h"
#include "physics_gravity.h"
#include "texture_management.h"
#include "coffee.h"
#include "autopilot.h"
#include "main_sim.h"
#include	"motherships2.h"
#include "npc.h"
#include "camera_control.h"
#include "particles.h"
#include "npc_gen.h"

#define KILL_LIST_MAX 200

int kill_list[KILL_LIST_MAX];
int kill_list_index=0;
int number_of_stars;	//how many stars displayed?
int monsters_can_fire;	//if 1, monsters can fire
//int difficulty;	//loaded from level loader
int global_open_doors;	//if zero, doors don't open
int global_control_monsters;	//if 0, monsters are frozen
int done_atmosphere_messages;
int low_res_planets;

extern int credits,score;
extern _3D camera_pos;
void run_fast_object_control();
module_private void handle_planet ( int the_planet);
module_private void reset_buffet(void);
module_private void check_atmosphere_flags(void);
module_private void handle_vv(int the_object);
module_private void handle_sun(int it);
module_private void handle_bolted_object(int it);
module_private void handle_cloud(int it);
module_private void handle_far_star(int it);
void handle_shield(int it);
void handle_camera(int it);

void kill_children(int the_object);
void handle_falcon(int it);

module_private void dispatch_control(int the_object);
//module_private void kill_particles(int owner);

module_private void Zkill(int the_object, int kill_ogl_texture);




//changed 8/12/00 SB - now returns an error!!!
int add_to_kill_list (int current_object)
{
//extern	DynObjectsFixedSize *ocb_ptr;
//if ((*ocb_ptr).object_list[current_object].in_use==0) { DebugStr("\padd_to_kill_list - You seem to be killing a dead object."); }


     kill_list[kill_list_index]=current_object;
     if (kill_list_index<KILL_LIST_MAX)
     {
        kill_list_index++;
        return 0;
     }
     else 
	 {
	  MyDebugStr(__LINE__,__FILE__ ,"add_to_kill_list failed! Step out to see caller..");
	  return -1;
     }
}

void reset_kill_list()
{
 kill_list_index=0;
}

void kill_kill_list() //it is quite possible for kill_kill_list to call add_to_kill_list 
{                     //via a child kill or kill method which will inc kill_list_index
int i;                // so we should be OK
for (i=0;i<kill_list_index;i++)
   {
   Zkill(kill_list[i],1);
   }

reset_kill_list();	//as it says
}

//this routine should only be called if main loop is not running
//because dynamic_object_index will get out of sync otherwise 
//it also overrides the checking of an already killed object!
public void i_know_what_im_doing_kill( int the_object, int kill_textures)
{
//extern	DynObjectsFixedSize *ocb_ptr;

//if ((*ocb_ptr).object_list[the_object].in_use==0) { DebugStr("\pi_know_what_im_doing_kill - You seem to be killing a dead object."); }

	Zkill(the_object,kill_textures);
}

module_private void Zkill(int the_object, int kill_ogl_textures)	//this should be called kill_obj, but this way I can say "kill(it)"
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _ZPoly_objs *dynamic_polys_ptr;
extern int	dynamic_object_index;
register ZObject * current_object_ptr;
extern scaled_points_array_type *scaled_points_ptr;

int polys_slot,i,scaled_points_slot;

//mod 210599 - SB
//we check if the object is alive before killing it to prevent dynamic_object-index being erroneously
//decremented.

// check that we aren't killing dead objects
if (ocb_ptr->object_list[the_object].in_use==0)
  {
     return;     //we now silently ignore the kill request
  }


     current_object_ptr=&ocb_ptr->object_list[the_object];
     
//check for scaled points
     scaled_points_slot=current_object_ptr->scaled_points_entry;
     
     if (scaled_points_slot!=-1)
     {
      scaled_points_ptr->scaled_points[scaled_points_slot].in_use=0;
     }
     
	current_object_ptr->in_use=0;	//deaded

	//if has_kill_method set then call the controller
	if (current_object_ptr->kill_flags&HAS_CHILDREN) 
        {
          kill_children(the_object);
	}
	//if has_kill_method set then call the controller
	if (current_object_ptr->kill_flags&KILL_METHOD) 
        {
          dispatch_control(the_object);
	}
	// call the forth controller kill controller for this object - just in case it has
	// a forth controller. Forth routines handle bounds checking
	coffee_object_deleted(the_object);
	
	polys_slot=current_object_ptr->active_polys_slot;	 //point to the polys
	(*dynamic_polys_ptr)[polys_slot].in_use=0;	//this poly slot is not in us
//if dynamic texture, release the memory
        if (current_object_ptr->dynamic_tex_ptr!=0) //if it's a dynamic texture we kill the texture
	{
          if(kill_ogl_textures!=0) //if we're allowed
	  {
	     kill_dynamic_texture(current_object_ptr->dynamic_tex_ptr);
          }
	}
//if animatd then kill all the poly slots

    if (current_object_ptr->anim_max_object!=1)
    {
    for (i=0;i<current_object_ptr->anim_max_object;i++)
     {
     (*dynamic_polys_ptr)[current_object_ptr->the_anim_ptrs[i].polys_slot].in_use=0;
     }
    }
    
    if (dynamic_object_index>0) dynamic_object_index--;
}

//Scan all objects looking for objects with a parent matching the_object
//If so, kill 'em
void kill_children(int parent)
{
int active_object_counter, object_counter;
extern DynObjectsFixedSize *ocb_ptr;
register ZObject * current_object_ptr;
extern int dynamic_object_index;

active_object_counter=0;
object_counter=0;

while (active_object_counter<dynamic_object_index)
 {
  current_object_ptr=&ocb_ptr->object_list[object_counter];

  if (current_object_ptr->in_use==1 ) //if object is alive and not bolted to anything
   {     
    if(current_object_ptr->parent==parent)
    {
      #if ZEX_PROJECT_BUILDER==1
        //fprintf(stderr, "KILL: child %d owned by %d killed.\n",object_counter, parent);
       #endif

      add_to_kill_list(object_counter); //now kill calls kill_children, so this is a recursive call... 
    }
    
    active_object_counter++;
   }	//end of is alive

  object_counter++;  
 }



}


//еееееееееееееееееееееееееееееееееееееееееееееееееееее
//Note about lasers
//These are targetted devices and as such the controller for the laser knows the target
//and calculates the shield damage to the object on each frame the laser controller is run.
//All other weapons rely on collisions to do their damage.
//SB 30/12/98
static int error_count=0;

void do_object_control()
{
int object_counter,active_object_counter,targetted_object,old_dynamic_object_index;
extern DynObjectsFixedSize *ocb_ptr;
register ZObject * current_object_ptr;
extern int dynamic_object_index;

vector p,d;
//signed char delta_rot_and;
run_fast_object_control();	//do the Fdyns
calculate_object_near_flag();
//first clear out all hit by slots

clear_collided_withs();
reset_buffet();

run_npc_gen(); //npc_gen.c

active_object_counter=0;
object_counter=0;
old_dynamic_object_index=dynamic_object_index; //we copy because we may add objects
while (active_object_counter<old_dynamic_object_index)
 {
  current_object_ptr=&ocb_ptr->object_list[object_counter];
  if (object_counter>MAX_NUMBER_OF_DYN_OBJECTS-1)
  {
   //oh, we have f*cked up in a big way
   		  #if ZEX_PROJECT_BUILDER==1  //can only printf to OSX syscon
		  fprintf (stderr, "EXEC: FATAL internal error(1) #%i: do_object_control loop! Probably you've added an object within this loop \n",error_count); 
          #endif
   error_count++;
   break;
  }

  if (current_object_ptr->in_use==1 ) //if object is alive and not bolted to anything
   {     
 
//    current_object_ptr->torque_sum.x=0;
//    current_object_ptr->torque_sum.y=0;
//    current_object_ptr->torque_sum.z=0;
  
//start autorotating objects
    if ((current_object_ptr->Orotdy!=0))
        {
		#if ENG_DEBUG_MESSAGES==1 //can only printf to OSX syscon
		#if ZEX_PROJECT_BUILDER==1 //can only printf to OSX syscon
          fprintf (stderr, "Eng: Object %i is has %f initial rotational force around Y \n",object_counter,current_object_ptr->Orotdy); 
		#endif
		#endif //ENG_DEBUG_MESSAGES


        	//rotation around y
		   p.x=1; p.y=0; p.z=0;	//position of force
		   d.x=0; d.y=0; d.z=-1;	//direction of force
		   apply_initial_force(object_counter, (float)current_object_ptr->Orotdy,1,p /*oint*/ , d /*irection*/);   

		   p.x=-1; p.y=0; p.z=0;	//position of force
		   d.x=0; d.y=0; d.z=1;	//direction of force
		   apply_initial_force(object_counter, (float)current_object_ptr->Orotdy,1,p /*oint*/ , d /*irection*/);   

        current_object_ptr->Orotdy=0;
        }


    if ((current_object_ptr->Orotdx!=0))
        {
		#if ENG_DEBUG_MESSAGES==1 //can only printf to OSX syscon
		#if ZEX_PROJECT_BUILDER==1 //can only printf to OSX syscon
          fprintf (stderr, "Eng: Object %i is has %f initial rotational force around X \n",object_counter,current_object_ptr->Orotdx); 
		#endif
		#endif //ENG_DEBUG_MESSAGES

        	//rotation around x
		   p.x=0; p.y=1; p.z=0;	//position of force
		   d.x=0; d.y=0; d.z=1;	//direction of force
		   apply_initial_force(object_counter, (float)current_object_ptr->Orotdx,1,p /*oint*/ , d /*irection*/);   

		   p.x=0; p.y=-1; p.z=0;	//position of force
		   d.x=0; d.y=0; d.z=-1;	//direction of force
		   apply_initial_force(object_counter, (float)current_object_ptr->Orotdx,1,p /*oint*/ , d /*irection*/);   

        current_object_ptr->Orotdx=0;
        }


    if ((current_object_ptr->Orotdz)!=0)
        {
		#if ENG_DEBUG_MESSAGES==1 //can only printf to OSX syscon
		#if ZEX_PROJECT_BUILDER==1 //can only printf to OSX syscon
          fprintf (stderr, "Eng: Object %i is has %f initial rotational force around Z \n",object_counter,current_object_ptr->Orotdz); 
		#endif
		#endif //ENG_DEBUG_MESSAGES

        	//rotation around x
		   p.x=1; p.y=0; p.z=0;	//position of force
		   d.x=0; d.y=1; d.z=0;	//direction of force
		   apply_initial_force(object_counter, (float)current_object_ptr->Orotdz,1,p /*oint*/ , d /*irection*/);   

		   p.x=-1; p.y=0; p.z=0;	//position of force
		   d.x=0; d.y=-1; d.z=0;	//direction of force
		   apply_initial_force(object_counter, (float)current_object_ptr->Orotdz,1,p /*oint*/ , d /*irection*/);   

        current_object_ptr->Orotdz=0;
        }
  
   
   if(current_object_ptr->bolted_to==0)
   {  
      if (current_object_ptr->Dyn_OCB_control_data.no_control_this_frame==1) //object has been spawned during this frame
	  {
	    current_object_ptr->Dyn_OCB_control_data.no_control_this_frame=0;
	  }
	  else
	  {
	    dispatch_control(object_counter);
	   	  
        apply_gravity(object_counter);		      // apply this objects gravity to anything else
   
	  
	  
        current_object_ptr->Oworldx+=current_object_ptr->abs_velocity_vector.x*time_in_secs;
        current_object_ptr->Oworldy+=current_object_ptr->abs_velocity_vector.y*time_in_secs;
        current_object_ptr->Oworldz+=current_object_ptr->abs_velocity_vector.z*time_in_secs;
   
        //calc relative velocity vector
        targetted_object=current_object_ptr->object_targetted;
        if(targetted_object!=0)
        {
          current_object_ptr->rel_velocity_vector.x=current_object_ptr->abs_velocity_vector.x
                                             - (*ocb_ptr).object_list[targetted_object].abs_velocity_vector.x;
          current_object_ptr->rel_velocity_vector.y=current_object_ptr->abs_velocity_vector.y
                                             - (*ocb_ptr).object_list[targetted_object].abs_velocity_vector.y;
          current_object_ptr->rel_velocity_vector.z=current_object_ptr->abs_velocity_vector.z
                                             - (*ocb_ptr).object_list[targetted_object].abs_velocity_vector.z;
        }
        else
        { 
          current_object_ptr->rel_velocity_vector.x
          =current_object_ptr->rel_velocity_vector.y
          =current_object_ptr->rel_velocity_vector.z=0;
        }
    } //end of no control this frame
 
 
 //if(object_counter==0) calc_zexs_relative_velocity();	//to the object we've targetted - in HUD.C
 
 //collision detection
 
 	run_collision_detection(object_counter);	//file collision.c
 
 	 
//     dispatch_control(object_counter);
        
    }	//end of bolted==0
    else
    {
      handle_bolted_object(object_counter);
    }
    
    active_object_counter++;

   }	//end of is alive

  object_counter++;  
 }



 check_atmosphere_flags();
}

static int slot_being_processed=0;

void run_fast_object_control()
{
extern DynFObjectsFixedSize *Focb_ptr;
extern DynObjectsFixedSize *ocb_ptr;
register ZFObject * current_object_ptr;
int n, max_fasts;

if (has_camera_switched_view()) //if camera has switched view, then we need to calculate
{                               // all fast object orientations, else just a few
 max_fasts=MAX_NUMBER_OF_FAST_DYN_OBJECTS;
}
else
{
 max_fasts=MAX_NUMBER_OF_FAST_DYN_OBJECTS;
}
for (n=0;n<max_fasts;n++)	//do 30 objects per frame
{
  if (slot_being_processed>=MAX_NUMBER_OF_FAST_DYN_OBJECTS)
  slot_being_processed=0;
  
  current_object_ptr=&Focb_ptr->Fobject_list[slot_being_processed];


  if (current_object_ptr->in_use==1 ) //if object is alive and not bolted to anything
   {
   _quat temp_quat;
   
   temp_quat=(*ocb_ptr).object_list[CAMERA_OBJECT].Qor;   
   temp_quat.x=-temp_quat.x;
   temp_quat.y=-temp_quat.y;
   temp_quat.z=-temp_quat.z;
   temp_quat.w=-temp_quat.w;
   
   current_object_ptr->Qor=temp_quat;
   }	//end of is alive

slot_being_processed++;
}	//end of n

}


void dispatch_control(int the_object)
{
extern	DynObjectsFixedSize *ocb_ptr;
ZObject* current_object_ptr = &ocb_ptr->object_list[the_object];

 //handle the object
    if (current_object_ptr->Dyn_OCB_control_data.controller== CAMERA_CONTROL)
    {
       handle_camera(the_object);
    }
    else		
    if (current_object_ptr->Dyn_OCB_control_data.controller== FALCON_CONTROL)
    {
       handle_falcon(the_object);
    }
    else		
    if (current_object_ptr->Dyn_OCB_control_data.controller== MOTHERSHIP_CONTROL)
    {
       handle_mothership(the_object);
    }
    else	  
    if (current_object_ptr->Dyn_OCB_control_data.controller==DUST)
    {
       handle_dust(the_object);
    }
    else	
    if (current_object_ptr->Dyn_OCB_control_data.controller==PLANET_CONTROL)
    {
       handle_planet(the_object);
    }
    else	
    if (current_object_ptr->Dyn_OCB_control_data.controller==LARGE_SHIELD)
    {
       handle_shield(the_object);
    }
    else	
    if (current_object_ptr->Dyn_OCB_control_data.controller==VV_IND)
    {
       handle_vv(the_object);
    }     
    else	
    if (current_object_ptr->Dyn_OCB_control_data.controller==SUN)
    {
       handle_sun(the_object);
    }    
    else	
    if (current_object_ptr->Dyn_OCB_control_data.controller==CLOUD_CONTROL)
    {
       handle_cloud(the_object);
    }     
    else	
    if (current_object_ptr->Dyn_OCB_control_data.controller==FAR_STAR)
    {
       handle_far_star(the_object);
    }     
    else	
    if (current_object_ptr->Dyn_OCB_control_data.controller==PARTICLE_GENERATOR_1)
    {
       handle_particle_generator_1(the_object);
    }   
    else	
    if (current_object_ptr->Dyn_OCB_control_data.controller==PARTICLE_GENERATOR_CANNON)
    {
       handle_particle_generator_cannon(the_object);
    }   
    else	
    if (current_object_ptr->Dyn_OCB_control_data.controller==PARTICLE_GENERATOR_EXHAUST)
    {
       handle_particle_generator_exhaust(the_object);
    }   
    else	
    if (current_object_ptr->Dyn_OCB_control_data.controller==LASER_CONTROL_1)
    {
       handle_particle_type_laser(the_object);
    }   
    else	
    if (current_object_ptr->Dyn_OCB_control_data.controller==CANNON_CONTROL_1)
    {
       handle_particle_type_shell(the_object);
    }   
    else	
    if (current_object_ptr->Dyn_OCB_control_data.controller==PARTICLE_CONTROL_EXHAUST)
    {
       handle_particle_type_exhaust(the_object);
    }   
    else	
    if (current_object_ptr->Dyn_OCB_control_data.controller==NPC_CONTROL)
    {
       control_NPC(the_object);
    }   


}



void reset_torque(ZObject* current_object_ptr)
{
    current_object_ptr->torque_sum.x=0;
    current_object_ptr->torque_sum.y=0;
    current_object_ptr->torque_sum.z=0;
}

//
vector vv;
//ееееееееееееееееееееееееееееееееееееееееееееееееееееее




// ******************************

void handle_camera(int it)
{
ZObject * current_object_ptr;
extern	DynObjectsFixedSize *ocb_ptr;
vector p,d;

    current_object_ptr=&ocb_ptr->object_list[it];
    if(current_object_ptr->in_use==1)  //just a bit of protection!
    {
     if (camera_get_view()==1)
     { //locked so copy quaternion from camera object
      current_object_ptr->Qor= ocb_ptr->object_list[get_main_camera_object()].Qor;
     }
     else //camera not locked to front view so we copy ships quat to camera then apply the camera pan forces
     {
      current_object_ptr->Qor= ocb_ptr->object_list[get_main_camera_object()].Qor;
      //now lets apply the yaw 
       reset_angular_torque(2);
        p.x=0; p.y=0; p.z=1;	//position of force
        d.x=1; d.y=0; d.z=0;	//direction of force
        apply_a_force_1sec(2, camera_get_pan(), p /*oint*/ , d /*irection*/);   


        p.x=0; p.y=0; p.z=-1;	//position of force
        d.x=-1; d.y=0; d.z=0;	//direction of force
        apply_a_force_1sec(2, camera_get_pan(), p /*oint*/ , d /*irection*/);   

        apply_extern_forces_1sec(&current_object_ptr->Qor, &current_object_ptr->torque_sum, current_object_ptr->mass);
       reset_angular_torque(2);

     }
    }
}

void handle_shield(int it)
{
ZObject * current_object_ptr;
extern	DynObjectsFixedSize *ocb_ptr;

    current_object_ptr=&ocb_ptr->object_list[it];
    if(current_object_ptr->in_use==1)  //just a bit of protection!
    {
      current_object_ptr->Dyn_OCB_control_data.control_use_1_float-=time_in_secs;
      if (current_object_ptr->Dyn_OCB_control_data.control_use_1_float<0.0)
      {
       int parent=current_object_ptr->Dyn_OCB_control_data.control_use_1;
       
       (*ocb_ptr).object_list[parent].showing_shield=0; //clear parents showing_shield attrib
       add_to_kill_list(it);
      }
      
    }
}

void handle_falcon(int it)
{
ZObject * current_object_ptr;
extern	DynObjectsFixedSize *ocb_ptr;

    current_object_ptr=&ocb_ptr->object_list[it];
    if(current_object_ptr->in_use==1)  //just a bit of protection!
    {
	    //set AP mode to transport
		current_object_ptr->Dyn_OCB_control_data.command_autopilot_engaged=AP_TRANSPORT;
		current_object_ptr->object_targetted=0;
		object_systems_sim(it);
 	}
}





int line_len;
double ox,oy,oz,mx,my,mz;	//debug



void handle_bolted_object(int it)
{
ZObject * current_object_ptr;
ZObject * current_object_ptr1;
//ZObject * current_object_ptr2;
extern	DynObjectsFixedSize *ocb_ptr;


  current_object_ptr=&ocb_ptr->object_list[it];
  current_object_ptr1=&ocb_ptr->object_list[current_object_ptr->bolted_to];

//set next two objects worldx,y,z to this objects so arms of mothership follow body!
//mothership must be loaded as body, arm, arm
  current_object_ptr->Oworldx=current_object_ptr1->Oworldx+current_object_ptr->bolt_x_offset;    
  current_object_ptr->Oworldy=current_object_ptr1->Oworldy+current_object_ptr->bolt_y_offset;    
  current_object_ptr->Oworldz=current_object_ptr1->Oworldz+current_object_ptr->bolt_z_offset;    

  current_object_ptr->rel_velocity_vector.x=current_object_ptr1->rel_velocity_vector.x;
  current_object_ptr->rel_velocity_vector.y=current_object_ptr1->rel_velocity_vector.y;
  current_object_ptr->rel_velocity_vector.z=current_object_ptr1->rel_velocity_vector.z;

  current_object_ptr->abs_velocity_vector.x=current_object_ptr1->abs_velocity_vector.x;
  current_object_ptr->abs_velocity_vector.y=current_object_ptr1->abs_velocity_vector.y;
  current_object_ptr->abs_velocity_vector.z=current_object_ptr1->abs_velocity_vector.z;

}


void handle_sun(int it)
{
_quat temp_quat;
extern	DynObjectsFixedSize *ocb_ptr;

//all we want the sun to do is face zex, so we take zex's quat, invert it and place it in the suns

   temp_quat=(*ocb_ptr).object_list[CAMERA_OBJECT].Qor;   
   temp_quat.x=-temp_quat.x;
   temp_quat.y=-temp_quat.y;
   temp_quat.z=-temp_quat.z;
   temp_quat.w=-temp_quat.w;
   
   (*ocb_ptr).object_list[it].Qor=temp_quat;
   
      
}


void handle_cloud(int it)
{
extern	DynObjectsFixedSize *ocb_ptr;

_quat temp_quat;
	return;
//all we want the cloud to do is face zex, so we take zex's quat, invert it and place it in the suns
//return;

   temp_quat=(*ocb_ptr).object_list[zex].Qor;   
   temp_quat.x=-temp_quat.x;
   temp_quat.y=-temp_quat.y;
   temp_quat.z=-temp_quat.z;
   temp_quat.w=-temp_quat.w;
   
   (*ocb_ptr).object_list[it].Qor=temp_quat;
   
      
}

void handle_far_star(int it)
{
_quat temp_quat;
extern	DynObjectsFixedSize *ocb_ptr;   
   
   temp_quat=(*ocb_ptr).object_list[zex].Qor;   
   temp_quat.x=-temp_quat.x;
   temp_quat.y=-temp_quat.y;
   temp_quat.z=-temp_quat.z;
   temp_quat.w=-temp_quat.w;
   
   (*ocb_ptr).object_list[it].Qor=temp_quat;
    
	return;
	
    
}


void handle_vv(int it)
{

double x,y,z;
extern	DynObjectsFixedSize *ocb_ptr;


   vv=(*ocb_ptr).object_list[zex].rel_velocity_vector;

   if ((vv.x<0.2) && (vv.y<0.2) && (vv.z<0.2)) 
   {
     if ((vv.x>-0.2) && (vv.y>-0.2) && (vv.z>-0.2))
     vv=(vector)(*ocb_ptr).object_list[zex].n_translated;	//no velocity
   }
   
   Vnormalise(&vv);
   
   x=(*ocb_ptr).object_list[zex].Oworldx+(vv.x*1000);
   y=(*ocb_ptr).object_list[zex].Oworldy+(vv.y*1000);
   z=(*ocb_ptr).object_list[zex].Oworldz+(vv.z*1000);
   (*ocb_ptr).object_list[it].Oworldx=x;
   (*ocb_ptr).object_list[it].Oworldy=y;
   (*ocb_ptr).object_list[it].Oworldz=z;
   
   
      
}


int heat_brightness;	//make screen red in atmosphere
void handle_planet ( int the_planet)
{
extern int ship_buffet;
//extern int test1;
extern int zex_alive;
ZObject * current_object_ptr;
_ZPoly *the_polys;
double distance,lod_range;
//double temp;
float atmosphere_top, atmosphere_bottom, atmosphere_depth,atmosphere_penetration;
  int temp_buffet;
extern	DynObjectsFixedSize *ocb_ptr;


//find Zex, get distance, if too close cause buffet


if(zex_alive==0) { heat_brightness=0; return; }

//find out if we can blobit
//if distance>radius*50 we blob it


distance=get_distance_to_zex(the_planet);
current_object_ptr=&ocb_ptr->object_list[the_planet];
lod_range=atmosphere_bottom=(*ocb_ptr).object_list[the_planet].radius;

if(distance<(lod_range*11))	//USe hires object?
 {
 //use pnt1 - hi res
   if(low_res_planets==0)
   {
   swap_dyn_object(the_planet,'PNT1');
   }
   else
   {
   swap_dyn_object(the_planet,'PNT2');
   }
   
   current_object_ptr->is_blobbed=0;	//tell system to process ONLY first poly
   the_polys=current_object_ptr->polys_ptr;
   (*the_polys).polyflags&=~PLANET_BIT;	//clear blob first poly
 }
 
else	//not in hires range
if(distance<(lod_range*30))	//USe mde object?
 {
 //use pnt2 - med res
   swap_dyn_object(the_planet,'PNT2');
   current_object_ptr->is_blobbed=0;	//tell system to process ONLY first poly
   the_polys=current_object_ptr->polys_ptr;
   (*the_polys).polyflags|=PLANET_BIT;	//tell system to draw name
    return; 
 }
 
else	//not in medres range
if(distance<(lod_range*80))	//use very low res object? - should be 100
 {
   swap_dyn_object(the_planet,'PNT3');
   current_object_ptr->is_blobbed=0;	//tell system to process ONLY first poly
   the_polys=current_object_ptr->polys_ptr;
   (*the_polys).polyflags|=PLANET_BIT;	////tell system to draw name
   return; //dont do atmosphere calcs - too far
 }
 
else //greater than lod range so blobb it
 {
   swap_dyn_object(the_planet,'PNT4'); //ultra low res - about 8 polys
   the_polys=current_object_ptr->polys_ptr;
   (*the_polys).polyflags|=PLANET_BIT;	////tell system to draw name
    current_object_ptr->is_blobbed=1;	//tell system to process ONLY first poly
  
  return;
 }   
//atmosphere runs from planet radius to planet radius+(planet_radius*0.01)


//atmosphere_bottom=(*ocb_ptr).object_list[the_planet].radius;
atmosphere_top=atmosphere_bottom+(*ocb_ptr).object_list[the_planet].radius*ATMOSPHERE_PERCENTAGE;
atmosphere_depth=atmosphere_top-atmosphere_bottom;

//suppose top is 100000 and bottom is 90000 and our distance is 200000
//we want buffet in the range of 0 (at top) to 30000 at bottom

atmosphere_penetration=distance-(*ocb_ptr).object_list[the_planet].radius; //200000-90000=110000
atmosphere_penetration=atmosphere_penetration/atmosphere_depth;	//110000/10000=11
if(atmosphere_penetration>1) atmosphere_penetration=1;

temp_buffet=atmosphere_penetration*30000;	//runs 0 to 30000
//ship buffet is reset at the start of every frame
ship_buffet+=30000-temp_buffet;			//so 0 at top and 30000 at base




// need to make atmosphere take 1% of planet radius

if(ship_buffet>10) // (temp>ship_buffet) remove planet atmosphere temporarily
 {
  if (done_atmosphere_messages==0)
   {
     add_to_text_display("OBC> Warning: Atmosphere detected",DLP_YELLOW);
     done_atmosphere_messages=1;
	 set_time_multiplier_to_1();
   }
   

  if (ship_buffet>3000)	//make screen red
  {
  
   temp_buffet=ship_buffet;
   temp_buffet-=3000;

  }

  
  if ((done_atmosphere_messages==1) && (ship_buffet>12000))
   {
     add_to_text_display("OBC> Danger: Hull damage imminent",DLP_RED);
     done_atmosphere_messages=2;
   }
 }
else	//ship_buffet>10
heat_brightness=0;
}

void reset_buffet(void)
{
extern int ship_buffet;
ship_buffet=0;

}
//run at end of object control
void check_atmosphere_flags(void)
{
extern int ship_buffet;
if (ship_buffet==0) done_atmosphere_messages=0;
}







//ееееееееееееееееееееееееееееееееееееееееееееееееееееее
void handle_ejection(int it)	//control ejection pod
{
extern	DynObjectsFixedSize *ocb_ptr;


(*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_1++;
// (*ocb_ptr).object_list[it].Orotdx+=1;
if ((*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_1>75)
 { 
//  (*ocb_ptr).object_list[it].Omovedx-=30;
//   (*ocb_ptr).object_list[it].thrust_x=-30;
     if ((*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_2==0)
     {
      play_zsound(zip, sound_low_pri, c1_chan1, sound_vol_7);
      (*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_2=1;	     //set lock
     }	//only play the sound once
 }


if ((*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_1>160)
 { 
 Zkill(it,TRUE);
 }
}



//ееееееееееееееееееееееееееееееееееееееееееееееееееееее
void handle_not_so_weeny_bullet(int it)	//lets the bullet live for 50 frames
{
extern	DynObjectsFixedSize *ocb_ptr;
//int attacking_this;
//int zangle,wangle,d1,d2;


(*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_1++;
if ((*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_1>120)
 { 
 Zkill(it,TRUE);
 }
else
 { 
// if (find_something_to_attack(it)==-1) return;	//didn't find anything
// if ((*ocb_ptr).object_list[it].Oworldx>1000000) Debugger();
// if ((*ocb_ptr).object_list[it].Oworldx<-1000000) Debugger();
 
// attacking_this=(*ocb_ptr).object_list[it].attack_what;

// if (RangedRdm(1,10)>7) ram_object(attacking_this,it,2);
 
//      (*ocb_ptr).object_list[it].thrust_y= (*ocb_ptr).object_list[it].Omovedy;	//update thrust y
//       (*ocb_ptr).object_list[it].thrust_x=(*ocb_ptr).object_list[it].Omovedx;
 }
}



//ееееееееееееееееееееееееееееееееееееееееееееееееееееее
void handle_burning_particle(int it)	//lets the shard live for a few frames and changes it's colour
{
extern	DynObjectsFixedSize *ocb_ptr;
_ZPoly *the_polys;
signed long temp_colour;

(*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_1--;	//explode sets this to a count
if ((*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_1<1)
 { 
 Zkill(it,TRUE);
 }
else
{
//if not time to kill, is this the first time we've been run?
//if so, set the colour to white
    if ((*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_1<15)
    {
    the_polys=(*ocb_ptr).object_list[it].polys_ptr;
//alter laser colour
//we need to get blue down to zero
    temp_colour=(*the_polys).Prgb.blue;
    temp_colour-=9000;
    if (temp_colour<0) temp_colour=0;
    (*the_polys).Prgb.blue=temp_colour;

    temp_colour=(*the_polys).Prgb.green;
    temp_colour-=4000;
    if (temp_colour<0) temp_colour=0;
    (*the_polys).Prgb.green=temp_colour;

    temp_colour=(*the_polys).Prgb.red;
    temp_colour-=2000;
    if (temp_colour<0) temp_colour=0;
    (*the_polys).Prgb.red=temp_colour;
    }
}
}


//ееееееееееееееееееееееееееееееееееееееееееееееееееееее
void handle_explode(int it)	//lets the shard live for a few frames
{
extern	DynObjectsFixedSize *ocb_ptr;


(*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_1--;	//explode sets this to a count
if ((*ocb_ptr).object_list[it].Dyn_OCB_control_data.control_use_1<1)
 { 
 
 if (add_to_kill_list(it)!=0)
   {
	 MyDebugStr(__LINE__, __FILE__,"add_to_kill_list failed!");
   }
 }
}







