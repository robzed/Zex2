/*
 *  particles.c
 *  zex22
 *
 *  Created by Stuart Ball on Tue May 28 2002.
 *  Copyright (c) 2002 Lightsoft. All rights reserved.
 *
 */
// This file is responsible for handling particles; currently it deals with
// Lasers, cannons and exhaust plumes.
//it deals not only with the birth and killing of the particles but also with their controllers
//i.e. things that position the particles and change their colours etc.
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
  #include "error_handler.h"
  #include "real_time.h"
  #include "processor.h"
  #include "access_ocbs.h"
#endif //pch


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
#include "particle_dyn.h"
#define DRAW_EXHAUSTS_DISTANCE_IN_M 10000 //how near we draw exhaust particles

module_private void insert_laser_particles(int virtual_trigger_object, int owner, int particle_bunch_id, int which_turret);
module_private void insert_gun_particles(int max_count, int it, int which_gun);
module_private void kill_laser_particles(int virtual_trigger_object, int particle_bunch_id);
module_private void insert_exhaust_particles(int vt, int ship, int particle_bunch_id, int which_turret);

int get_new_TrigRef(void);

int get_new_TrigRef(void)
{
 static UInt32 trig_ref=0;
 return trig_ref++;
}

//this is yer basic laser
//we insert 30 particles of type 1
//these particles will exist until this controller is killed.
//this way the laser stays on screen until the player (or npc) leaves
//go of the laser fire button
void handle_particle_generator_1(int vt) //virtual laser trigger object spawned from ship  - alive whilst laser trigger is held down
{
ZObject * current_object_ptr;
int owner;
extern	DynObjectsFixedSize *ocb_ptr;
double las1x,las2x,las3x,las4x;
int particle_bunch_id;

 
    current_object_ptr=&ocb_ptr->object_list[vt];
    owner=current_object_ptr->parent; //the ship holding the virtual laser trigger object

    if(current_object_ptr->in_use==0) //laser off? (called via kill_method)
    {
    //virtual trigger being killed so kill all our particles
    //search for particles with an owner of the virtual trigger object and add them to the kill list
       #if ZEX_PROJECT_BUILDER==1 && SHOW_VIRTUAL_TRIGGERS==1
          fprintf(stderr, "part: VTrig %d owned by %d is being killed\n",vt, owner);
       #endif
       kill_laser_particles(vt,current_object_ptr->Dyn_OCB_control_data.control_use_4);    //kill all the laser particles (not guns!)
    
    }
    else
    {
      if(current_object_ptr->Dyn_OCB_control_data.control_use_3!=1)
      {
        current_object_ptr->Dyn_OCB_control_data.control_use_3=1;	     //only do once - the trigger - insert laser particles
        current_object_ptr->Dyn_OCB_control_data.control_use_4=get_new_TrigRef();  // new trigger reference so we can kill em
        particle_bunch_id=current_object_ptr->Dyn_OCB_control_data.control_use_4;
        #if ZEX_PROJECT_BUILDER==1 && SHOW_VIRTUAL_TRIGGERS==1
          fprintf(stderr, "part: VTrig %d owned by %d created.\n",vt, owner);
        #endif
            
        las1x=(*ocb_ptr).object_list[owner].las01.x;
        las2x=(*ocb_ptr).object_list[owner].las02.x;
        las3x=(*ocb_ptr).object_list[owner].las03.x;
        las4x=(*ocb_ptr).object_list[owner].las04.x;
	    
        if (las1x<2) //9 is not used
        {
          insert_laser_particles(vt,owner,particle_bunch_id,1);
	}
//is there a laser 2?
        if (las2x<2) //9 is not used
	{
          insert_laser_particles(vt,owner,particle_bunch_id,2);
	}

//is there a laser 3?
        if (las3x<2) //9 is not used
	{
          insert_laser_particles(vt,owner,particle_bunch_id,3);
	}
//is there a laser 4?
        if (las4x<2) //9 is not used
	{
          insert_laser_particles(vt,owner,particle_bunch_id,4);
	}
      } //end of trigger
    } //end of laser off
}

    
void insert_laser_particles(int vt, int ship, int particle_bunch_id, int which_turret)
{        
ZObject * current_object_ptr;
int dest_ocb;
load_dyn_object_pb the_params;
int particle_count,max_count;
extern	DynObjectsFixedSize *ocb_ptr;
            
    current_object_ptr=&ocb_ptr->object_list[vt]; //virtual trigger object
                    
    particle_count=0;

/*    if(current_object_ptr->Dyn_OCB_control_data.control_use_1==0) //laser type
    {
      max_count=29;
    }
    else
    {
      max_count=46;
    }
*/
    max_count=30;    

    clear_pb(&the_params);
    the_params.controller_ref=LASER_CONTROL_1;
    the_params.object_category=NO_RADAR;
    the_params.control_use_1=particle_bunch_id;	//so kill_laser_particles can kill the right ones	
    the_params.control_use_3=which_turret;
    the_params.control_use_4=ship;	//so the particles know who they are attahed to        
    the_params.parent=vt;	//so we can kill the particles when we get killed
    //the_params.delta_rot_z=RangedRdm(0,60)-30;
        
    the_params.mass=0.01;
    
    while(particle_count<max_count)	//generate 20 of the things
    {
      particle_count++;
    
      current_object_ptr->Dyn_OCB_control_data.control_use_2=0;
      the_params.init_check='chek';

      dest_ocb=find_vacant_dynamic_object_slot();

      if (dest_ocb!=-1)
      {   
	
       the_params.control_use_2=particle_count;	//pass this particles position
       load_dyn_object(25,dest_ocb,&the_params,-1,1,-1,!DUST_BIT);
       #if ZEX_PROJECT_BUILDER==1 && EXTREME_LASER_LOGGING==1
          fprintf(stderr, "part: Laser particle %d owned by %d owned by %d with TrigRef %d created\n",dest_ocb, vt, ship, particle_bunch_id);
       #endif

      }	//end of it!=-1
     else return;	//couldn't gen dust - no spare slots
    }	//end of while count
}


//Just kill the laser particles for an object
void kill_laser_particles(int vt, int particle_bunch_id)
{
int active_object_counter,object_counter;
register ZObject * current_object_ptr;
extern	DynObjectsFixedSize *ocb_ptr;
extern int dynamic_object_index;


active_object_counter=0;
object_counter=0;


while (active_object_counter<dynamic_object_index)
{
 current_object_ptr=&ocb_ptr->object_list[object_counter];

 if ((current_object_ptr->in_use==1))
  {
   if((current_object_ptr->parent==vt) && (current_object_ptr->Dyn_OCB_control_data.control_use_1==particle_bunch_id))
   {
    #if ZEX_PROJECT_BUILDER==1 && EXTREME_LASER_LOGGING==1
          fprintf(stderr, "part: Laser particle %d, Trigger ref %d, scheduled for kill.\n",object_counter, particle_bunch_id);
    #endif

    if (add_to_kill_list(object_counter)!=0)
    {
    
      {
        MyDebugStr(__LINE__, __FILE__,"add_to_kill_list failed!");
      }
		
    }
   }
   active_object_counter+=1;
  } //end of if in_use
 
object_counter++;
//¥ do next object
}	//end of while

}






void handle_particle_type_laser(int it) //laser
{
ZObject * current_object_ptr;
_ZPoly *the_polys;
int number_of_polys,n;
int owner;
short blue_dec;
vector las;

extern	DynObjectsFixedSize *ocb_ptr;
_quat temp_quat;

    if(time_in_secs==0) return;	//freeze control
    
    current_object_ptr=&ocb_ptr->object_list[it];
    owner=current_object_ptr->Dyn_OCB_control_data.control_use_4;

//we want the particle to do is face zex, so we take zex's quat, invert it and place it in the particle
	 //make particle face the camera

    temp_quat=(*ocb_ptr).object_list[CAMERA_OBJECT].Qor;   
    temp_quat.x=-temp_quat.x;
    temp_quat.y=-temp_quat.y;
    temp_quat.z=-temp_quat.z;
    temp_quat.w=-temp_quat.w;
    (*ocb_ptr).object_list[it].Qor=temp_quat;
	
   //set us to in front of owner
    if(current_object_ptr->Dyn_OCB_control_data.control_use_3==1)	//turret 1
    {
        las.x=(*ocb_ptr).object_list[owner].las01_translated.x;
        las.y=(*ocb_ptr).object_list[owner].las01_translated.y;
        las.z=(*ocb_ptr).object_list[owner].las01_translated.z;
        
    }
    else
    if(current_object_ptr->Dyn_OCB_control_data.control_use_3==2)	//turret 2
    {
        las.x=(*ocb_ptr).object_list[owner].las02_translated.x;
        las.y=(*ocb_ptr).object_list[owner].las02_translated.y;
        las.z=(*ocb_ptr).object_list[owner].las02_translated.z;
    }
    else
    if(current_object_ptr->Dyn_OCB_control_data.control_use_3==3)	//turret 3
    {
        las.x=(*ocb_ptr).object_list[owner].las03_translated.x;
        las.y=(*ocb_ptr).object_list[owner].las03_translated.y;
        las.z=(*ocb_ptr).object_list[owner].las03_translated.z;
    }
    else
    if(current_object_ptr->Dyn_OCB_control_data.control_use_3==4)	//turret 4
    {
        las.x=(*ocb_ptr).object_list[owner].las04_translated.x;
        las.y=(*ocb_ptr).object_list[owner].las04_translated.y;
        las.z=(*ocb_ptr).object_list[owner].las04_translated.z;
    }

    las.x*=(*ocb_ptr).object_list[owner].radius;
    las.y*=(*ocb_ptr).object_list[owner].radius;
    las.z*=(*ocb_ptr).object_list[owner].radius;
        
    current_object_ptr->Oworldx=las.x+RangedRdm(0,25);
    current_object_ptr->Oworldy=las.y+RangedRdm(0,7);
    current_object_ptr->Oworldz=las.z+RangedRdm(0,14);

    current_object_ptr->Oworldx+=(*ocb_ptr).object_list[owner].Oworldx;
    current_object_ptr->Oworldy+=(*ocb_ptr).object_list[owner].Oworldy;
    current_object_ptr->Oworldz+=(*ocb_ptr).object_list[owner].Oworldz;

   //add on parents normal rotation plus the square of the particles number as a distance offset 
   //to give us a line of laser particles
   
    current_object_ptr->Oworldx+=ocb_ptr->object_list[owner].n_translated.x*
    (20*(current_object_ptr->Dyn_OCB_control_data.control_use_2 *current_object_ptr->Dyn_OCB_control_data.control_use_2));
    current_object_ptr->Oworldy+=ocb_ptr->object_list[owner].n_translated.y*
    (20*(current_object_ptr->Dyn_OCB_control_data.control_use_2*current_object_ptr->Dyn_OCB_control_data.control_use_2));
    current_object_ptr->Oworldz+=ocb_ptr->object_list[owner].n_translated.z*
    (20*(current_object_ptr->Dyn_OCB_control_data.control_use_2*current_object_ptr->Dyn_OCB_control_data.control_use_2));
    
    //fade the colour to red
    //get pointer to the polys
    number_of_polys=current_object_ptr->number_of_polys;
    the_polys=current_object_ptr->polys_ptr;

//if frame count is the same as our ref then dec red
     //red_dec=RangedRdm(0,20000)+45000;
     //green_dec=RangedRdm(0,30000)+10000;
     blue_dec=RangedRdm(500,10000);
     for(n=0;n<number_of_polys;n++)
     {
       the_polys->Prgb.green=0;
       the_polys->Prgb.blue-=blue_dec/4;
       the_polys->Prgb.red-=blue_dec;
       the_polys++;
     }
}

// ---------------------------------------------------------------------------
//this is yer basic exhaust
//we insert a few particles of type EXHAUST per exhaust. We can handle a max of two per object
//these particles will exist until this controller is killed.
//this way the exhaust stays on screen until the player (or npc) leaves
//go of the laser fire button
void handle_particle_generator_exhaust(int vt) //virtual exhaust trigger object spawned from ship  - alive whilst laser trigger is held down
{
ZObject * current_object_ptr;
int owner;
extern	DynObjectsFixedSize *ocb_ptr;
double ex1x,ex2x;
int particle_bunch_id;

 
    current_object_ptr=&ocb_ptr->object_list[vt];
    owner=current_object_ptr->parent; //the ship holding the virtual exhaust trigger object

    if(current_object_ptr->in_use==0) //exhaust off? (called via kill_method)
    {
    //virtual trigger being killed so kill all our particles
    //search for particles with an owner of the virtual trigger object and add them to the kill list
       #if ZEX_PROJECT_BUILDER==1 && SHOW_VIRTUAL_TRIGGERS==1
          fprintf(stderr, "part: (DK) Ex VTrig %d owned by %d is being killed\n",vt, owner);
       #endif
       kill_laser_particles(vt,current_object_ptr->Dyn_OCB_control_data.control_use_4);    //kill all the laser particles (not guns!)
    
    }
    else
    {
      if(current_object_ptr->Dyn_OCB_control_data.control_use_3!=1) //we have not inserted the trigger
      {
        if (get_distance_to_from_in_m(get_main_camera_object(), owner)<DRAW_EXHAUSTS_DISTANCE_IN_M)
        {
           current_object_ptr->Dyn_OCB_control_data.control_use_3=1;	     //only do once - the trigger - insert laser particles
           current_object_ptr->Dyn_OCB_control_data.control_use_4=get_new_TrigRef();  // new trigger reference so we can kill em
           particle_bunch_id=current_object_ptr->Dyn_OCB_control_data.control_use_4;
           #if ZEX_PROJECT_BUILDER==1 && SHOW_VIRTUAL_TRIGGERS==1
             fprintf(stderr, "part: (C)Parts for Ex VTrig %d owned by %d created.\n",vt, owner);
             fprintf(stderr, "part: TrigRef %d.\n",particle_bunch_id);
           #endif
            
           ex1x=(*ocb_ptr).object_list[owner].mthr1.x;
           ex2x=(*ocb_ptr).object_list[owner].mthr2.x;
	    
           if (ex1x<2) //9 is not used
           {
             insert_exhaust_particles(vt,owner,particle_bunch_id,1);
	   }
//is there an exhaust 2?
           if (ex2x<2) //9 is not used
	   {
             insert_exhaust_particles(vt,owner,particle_bunch_id,2);
	   }
        } //end of distance check
        
      } //end of not inserted trigger
      else //we did insert the trigger, check if we can kill the particles
      {
        if (get_distance_to_from_in_m(get_main_camera_object(), owner)>DRAW_EXHAUSTS_DISTANCE_IN_M+1000) //a bit of hysterisis
        {
           #if ZEX_PROJECT_BUILDER==1 && SHOW_VIRTUAL_TRIGGERS==1
             fprintf(stderr, "part: (K) Parts for Ex VTrig %d owned by %d are killed\n",vt, owner);
           #endif
           kill_laser_particles(vt,current_object_ptr->Dyn_OCB_control_data.control_use_4);    //kill all the laser particles (not guns!)
           current_object_ptr->Dyn_OCB_control_data.control_use_3=0;
        }        
      }
    } //end of laser off
}




    
void insert_exhaust_particles(int vt, int ship, int particle_bunch_id, int which_turret)
{        
ZObject * current_object_ptr;
int dest_ocb;
load_dyn_object_pb the_params;
int particle_count;
extern	DynObjectsFixedSize *ocb_ptr;
            
    current_object_ptr=&ocb_ptr->object_list[vt]; //virtual trigger object
    particle_count=0;
    clear_pb(&the_params);
    the_params.controller_ref=PARTICLE_CONTROL_EXHAUST;
    the_params.object_category=NO_RADAR;
    the_params.control_use_1=particle_bunch_id;	//so kill_laser_particles can kill the right ones	
    the_params.control_use_3=which_turret;
    the_params.control_use_4=ship;	//so the particles know who they are attahed to        
    the_params.parent=vt;	//so we can kill the particles when we get killed

    //the_params.delta_rot_z=RangedRdm(0,60)-30;
    the_params.mass=0.01;
    while(particle_count<10)	//how many particles per exhaust
    {
      particle_count++;
      the_params.init_check='chek';
      dest_ocb=find_vacant_dynamic_object_slot();
      if (dest_ocb!=-1)
      {   
	
       the_params.control_use_2=particle_count;	//pass this particles position
       load_dyn_object(25,dest_ocb,&the_params,-1,1,-1,!DUST_BIT);
       #if ZEX_PROJECT_BUILDER==1 && EXTREME_LASER_LOGGING==1
          fprintf(stderr, "part: Laser particle %d owned by %d owned by %d with TrigRef %d created\n",dest_ocb, vt, ship, particle_bunch_id);
       #endif

      }	//end of it!=-1
     else return;	//couldn't gen dust - no spare slots
    }	//end of while count
}



void handle_particle_type_exhaust(int it) //exhaust drawing
{
ZObject * current_object_ptr;
_ZPoly *the_polys;
int number_of_polys,n;
int owner;
short colour_dec;
vector ex;
float thrust;
unsigned short red, green, blue;

extern	DynObjectsFixedSize *ocb_ptr;
_quat temp_quat;

    if(time_in_secs==0) return;	//freeze control
    
    current_object_ptr=&ocb_ptr->object_list[it];
    owner=current_object_ptr->Dyn_OCB_control_data.control_use_4;

//we want the particle to do is face zex, so we take zex's quat, invert it and place it in the particle
	 //make particle face the camera

    temp_quat=(*ocb_ptr).object_list[CAMERA_OBJECT].Qor;   
    temp_quat.x=-temp_quat.x;
    temp_quat.y=-temp_quat.y;
    temp_quat.z=-temp_quat.z;
    temp_quat.w=-temp_quat.w;
    (*ocb_ptr).object_list[it].Qor=temp_quat;

    thrust=(*ocb_ptr).object_list[owner].Dyn_OCB_control_data.current_total_fuel_consumption;
    thrust/=(*ocb_ptr).object_list[owner].Dyn_OCB_control_data.main_thrust_consumption_per_second_in_KGS; //total possible thrust
    thrust*=10;  
	
   //set us to behind owner
    if(current_object_ptr->Dyn_OCB_control_data.control_use_3==1)	//exhaust 1
    {
        ex.x=(*ocb_ptr).object_list[owner].mthr1_translated.x;
        ex.y=(*ocb_ptr).object_list[owner].mthr1_translated.y;
        ex.z=(*ocb_ptr).object_list[owner].mthr1_translated.z;        
    }
    else
    if(current_object_ptr->Dyn_OCB_control_data.control_use_3==2)	//exhaust 2
    {
        ex.x=(*ocb_ptr).object_list[owner].mthr2_translated.x;
        ex.y=(*ocb_ptr).object_list[owner].mthr2_translated.y;
        ex.z=(*ocb_ptr).object_list[owner].mthr2_translated.z;
    }

    ex.x*=(*ocb_ptr).object_list[owner].radius;
    ex.y*=(*ocb_ptr).object_list[owner].radius;
    ex.z*=(*ocb_ptr).object_list[owner].radius;
        
    current_object_ptr->Oworldx=ex.x; //+RangedRdm(0,25);
    current_object_ptr->Oworldy=ex.y; //+RangedRdm(0,27);
    current_object_ptr->Oworldz=ex.z; //+RangedRdm(0,24);

    current_object_ptr->Oworldx+=(*ocb_ptr).object_list[owner].Oworldx;
    current_object_ptr->Oworldy+=(*ocb_ptr).object_list[owner].Oworldy;
    current_object_ptr->Oworldz+=(*ocb_ptr).object_list[owner].Oworldz;

   //add on parents -ve normal rotation plus the square of the particles number as a distance offset 
   //to give us a line of particles
    current_object_ptr->Oworldx+=-(ocb_ptr->object_list[owner].n_translated.x*
    (thrust*(current_object_ptr->Dyn_OCB_control_data.control_use_2 *current_object_ptr->Dyn_OCB_control_data.control_use_2)));
    current_object_ptr->Oworldy+=-(ocb_ptr->object_list[owner].n_translated.y*
    (thrust*(current_object_ptr->Dyn_OCB_control_data.control_use_2*current_object_ptr->Dyn_OCB_control_data.control_use_2)));
    current_object_ptr->Oworldz+=-(ocb_ptr->object_list[owner].n_translated.z*
    (thrust*(current_object_ptr->Dyn_OCB_control_data.control_use_2*current_object_ptr->Dyn_OCB_control_data.control_use_2)));
    
    number_of_polys=current_object_ptr->number_of_polys;
    the_polys=current_object_ptr->polys_ptr;
//colour is blue close up fading to white
     blue=60000;
     red=current_object_ptr->Dyn_OCB_control_data.control_use_2*2000;
     green=red;
     colour_dec=NDRangedRdm(0,30000);
     colour_dec-=15000;
     for(n=0;n<number_of_polys;n++)
     {
       the_polys->Prgb.green=green;
       the_polys->Prgb.blue=blue-colour_dec;
       the_polys->Prgb.red=red;
       the_polys++;
     }
}


// ---------------------------------------------------------------------------
//this is yer basic cannon
//we insert 3 particles of type 2
//then kill the vt
void handle_particle_generator_cannon(int vt)
{
ZObject * current_object_ptr;
//vector temp_velocity;
extern	DynObjectsFixedSize *ocb_ptr;
double gun1x,gun2x,gun3x,gun4x;
int owner;

    current_object_ptr=&ocb_ptr->object_list[vt];
    owner=current_object_ptr->parent;

    gun1x=(*ocb_ptr).object_list[owner].gun01.x;
    gun2x=(*ocb_ptr).object_list[owner].gun02.x;
    gun3x=(*ocb_ptr).object_list[owner].gun03.x;
    gun4x=(*ocb_ptr).object_list[owner].gun04.x;
	    
    if (gun1x<2) //9 is not used
    {
      insert_gun_particles(2,vt,1);
    }
//is there a gun 2?
    if (gun2x<2) //9 is not used
    {
     insert_gun_particles(2,vt,2);
    }

//is there a gun 3?
    if (gun3x<2) //9 is not used
    {
     insert_gun_particles(2,vt,3);
    }
//is there a gun 4?
    if (gun4x<2) //9 is not used
    {
     insert_gun_particles(2,vt,4);
    }
	
	
    if (add_to_kill_list(vt)!=0)
    {
      MyDebugStr(__LINE__, __FILE__,"add_to_kill_list failed!");
    }
	
} 



module_private void insert_gun_particles(int max_count, int vt, int which_gun)
{
ZObject * current_object_ptr;
int dest_ocb,owner;
load_dyn_object_pb the_params;
//vector temp_velocity;
int particle_count;
extern	DynObjectsFixedSize *ocb_ptr;
vector p,d;
double gun_x, gun_y, gun_z;
int particle_bunch_id;

    current_object_ptr=&ocb_ptr->object_list[vt];
    particle_count=0;
    owner=current_object_ptr->parent;
    particle_bunch_id=get_new_TrigRef();	//get a unique trigger ref for this bunch of particles
    while(particle_count<max_count)	//generate n of the particles
    {
      particle_count++;
      current_object_ptr->Dyn_OCB_control_data.control_use_2=0;
    //gen a particle
      clear_pb(&the_params);

//******************LOAD THE PARTICLE

         
//set up location of gun on the ship
      if (which_gun==1)
      {
        gun_x=(*ocb_ptr).object_list[owner].gun01_translated.x;
        gun_y=(*ocb_ptr).object_list[owner].gun01_translated.y;
        gun_z=(*ocb_ptr).object_list[owner].gun01_translated.z;
      }
      else
      if (which_gun==2)
      {
        gun_x=(*ocb_ptr).object_list[owner].gun02_translated.x;
        gun_y=(*ocb_ptr).object_list[owner].gun02_translated.y;
        gun_z=(*ocb_ptr).object_list[owner].gun02_translated.z;
      }
      else
      if (which_gun==3)
      {
        gun_x=(*ocb_ptr).object_list[owner].gun03_translated.x;
        gun_y=(*ocb_ptr).object_list[owner].gun03_translated.y;
        gun_z=(*ocb_ptr).object_list[owner].gun03_translated.z;
      }
      else
      if (which_gun==4)
      {
        gun_x=(*ocb_ptr).object_list[owner].gun04_translated.x;
        gun_y=(*ocb_ptr).object_list[owner].gun04_translated.y;
        gun_z=(*ocb_ptr).object_list[owner].gun04_translated.z;
      }
      else
      {
        gun_x=gun_y=gun_z=0;	//compiler warning 
        MyDebugStr(__LINE__, __FILE__,"Bad gun passed to insert_gun_articles!");
      }
	  
      the_params.world_x=gun_x*(*ocb_ptr).object_list[owner].radius; //this is a bit of a cheat; we should use the bounding box dimension/2
      the_params.world_y=gun_y*(*ocb_ptr).object_list[owner].radius;
      the_params.world_z=gun_z*(*ocb_ptr).object_list[owner].radius;
	
      the_params.world_x+=(*ocb_ptr).object_list[owner].Oworldx
                          +(ocb_ptr->object_list[owner].n_translated.x
			  *(*ocb_ptr).object_list[owner].radius*2);

      the_params.world_y+=(*ocb_ptr).object_list[owner].Oworldy + 
			 (ocb_ptr->object_list[owner].n_translated.y * 
			 (*ocb_ptr).object_list[owner].radius*2);

      the_params.world_z+=(*ocb_ptr).object_list[owner].Oworldz + 
			 (ocb_ptr->object_list[owner].n_translated.z * 
			 (*ocb_ptr).object_list[owner].radius*2);

	
      the_params.controller_ref=CANNON_CONTROL_1;
      the_params.object_category=NO_RADAR;
      the_params.control_use_1=vt;	//pass us as the owner, when we die so do the particles	
      the_params.control_use_2=particle_bunch_id;	//pass this bunch of particles trigger ref
      the_params.control_use_1_float=5.0; //time to live in seconds
      the_params.delta_rot_z=RangedRdm(0,100)-50;
        
      if(current_object_ptr->Dyn_OCB_control_data.control_use_1==0)
      {
        the_params.control_use_3=0;	//signle front mounted laser
      }
      else	//twin laser
      {
        if(particle_count&1)
        the_params.control_use_3=1;	//signle front mounted laser
        else
        the_params.control_use_3=2;	//signle front mounted laser
      }

      the_params.control_use_4=owner;	//so the particles know who they are attahed to        
      //the_params.who_spawned_this_object=it;	//so we can kill the particles when we get killed
        
      the_params.mass=0.010;	//10Kg's
      dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object

      if (dest_ocb!=-1)
      {
       
       if(particle_count==1) //first gun particle has colldet
       { load_dyn_object(27,dest_ocb, &the_params,-1,1,-1,!DUST_BIT); } //has collision detection
       else
       { load_dyn_object(25,dest_ocb, &the_params,-1,1,-1,!DUST_BIT); } //no collision detection


       (*ocb_ptr).object_list[dest_ocb].Qor=(*ocb_ptr).object_list[owner].Qor;
       (*ocb_ptr).object_list[dest_ocb].abs_velocity_vector=(*ocb_ptr).object_list[owner].abs_velocity_vector;
       (*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.no_control_this_frame=1;

       //now, kick the particle and kick Firer   
       p.x=0; p.y=0; p.z=-1;	//position of force - right on the bum
       d.x=0; d.y=0; d.z=1;
       apply_initial_force(dest_ocb, 700, 0.5, p /*oint*/ , d /*irection*/);   
 
       //reverse kick firer
       p.x=gun_x; p.y=gun_y; p.z=gun_z;	//position of force - gun mounting point
       d.x=0; d.y=0; d.z=-1;	//direction of force - from front
       apply_initial_force(owner, 5, 0.1, p /*oint*/ , d /*irection*/);   
	 
       }//end of dest_ocb!=-1
	 
    }	//end of while count

}



void handle_particle_type_shell(int it) //Gun
{
ZObject * current_object_ptr;
_ZPoly *the_polys;
int number_of_polys,n;
int owner;
short red_dec,green_dec;
extern	DynObjectsFixedSize *ocb_ptr;
_quat temp_quat;

//shells live for the time specified in control_use_1_float
    if(time_in_secs==0) return;	//freeze control
    

    //make particle face the camera - kills any auto rotation it had!
    temp_quat=(*ocb_ptr).object_list[CAMERA_OBJECT].Qor;   
    temp_quat.x=-temp_quat.x;
    temp_quat.y=-temp_quat.y;
    temp_quat.z=-temp_quat.z;
    temp_quat.w=-temp_quat.w;
    (*ocb_ptr).object_list[it].Qor=temp_quat;

    current_object_ptr=&ocb_ptr->object_list[it];
    current_object_ptr->Dyn_OCB_control_data.control_use_1_float-=time_in_secs;
	
    owner=current_object_ptr->Dyn_OCB_control_data.control_use_4;   
 
    //check if it's time to kill this particle
    if(current_object_ptr->Dyn_OCB_control_data.control_use_1_float<0.0) 
    {
      if (add_to_kill_list(it)!=0)
      {
        MyDebugStr(__LINE__, __FILE__,"add_to_kill_list failed!");
      }
    }



    //Change the colour of the particles
    //get pointer to the polys
    number_of_polys=current_object_ptr->number_of_polys;
    the_polys=current_object_ptr->polys_ptr;

     red_dec=NDRangedRdm(0,20000)+45000;
     green_dec=NDRangedRdm(0,20000)+45000;
     //blue_dec=RangedRdm(0,30000)+10000;
     for(n=0;n<number_of_polys;n++)
     {
       the_polys->Prgb.green+=green_dec;
       the_polys->Prgb.blue=0xffff;
       the_polys->Prgb.red+=red_dec;
       the_polys++;
     }
}



//Given one gun particle, kill all associated particles
//We need to match the particles virtual trigger (the firer of the particle)
//along with the owner of the VT (a ship generally) and the particle_bunch_id (the unique id for a bunch of particles)
//If we don't do this then ship with multi cannons will see all their shells vanish when one collides.
void kill_gun_particles(int vt, int parent, int particle_bunch_id)
{
int active_object_counter,object_counter;
register ZObject * current_object_ptr;
extern	DynObjectsFixedSize *ocb_ptr;
extern int dynamic_object_index;


active_object_counter=0;
object_counter=0;


while (active_object_counter<dynamic_object_index)
{
 current_object_ptr=&ocb_ptr->object_list[object_counter];

 if ((current_object_ptr->in_use==1))
  {
   if((current_object_ptr->Dyn_OCB_control_data.control_use_1==vt) 
      && (current_object_ptr->Dyn_OCB_control_data.control_use_4==parent)
      && (current_object_ptr->Dyn_OCB_control_data.control_use_2==particle_bunch_id))
   {
    #if ZEX_PROJECT_BUILDER==1 && EXTREME_LASER_LOGGING==1
          fprintf(stderr, "part: Cannon particle %d, parent %d, scheduled for kill.\n",object_counter, parent);
    #endif

    if (add_to_kill_list(object_counter)!=0)
    {
    
      {
        MyDebugStr(__LINE__, __FILE__,"add_to_kill_list failed!");
      }
		
    }
   }
   active_object_counter+=1;
  } //end of if in_use
 
object_counter++;
//¥ do next object
}	//end of while

}



