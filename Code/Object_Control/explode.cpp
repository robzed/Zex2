/* explode.cpp
 *
 * $Log: explode.cpp,v $
 * Revision 1.2  2003/09/20 12:57:02  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:36:00  stu_c
 * First Imported.
 *
 * Revision 1.7  2002/09/08 02:13:48  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.6  2002/05/31 23:54:09  stu
 * Removed all warnings from the code
 *
 * Revision 1.5  2002/05/05 22:28:04  stu
 * Further developement - hull temperature now correct for multiple suns and atmosphere.

All calls to add_to_kill_list check the return
 *
 * Revision 1.4  2002/03/16 00:44:36  stu
 * Continued development
 *
 * Revision 1.3  2001/11/02 19:32:18  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:29:09  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:03:56  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:06  rob
 * First Imported.
 *
 * Revision 1.3  2001/06/23 20:10:29  stu
 * 0.66 checkin
 *
 * Revision 1.2  2001/06/03 03:22:17  stu
 * OCB structure now contains a sub structure called:
ZObjectControl
Which contains all the control data for an object.

Added accessor in access_ocbs which returns a pointer to type ZObjectControl
 *
 * Revision 1.1.1.1  2001/01/01 21:13:38  rob
 * First Imported.
 *
 * Revision 1.4  2000/12/03 23:52:11  rob
 * changed constants for object type to get uniformity
 *
 * Revision 1.3  2000/11/16 20:38:37  rob
 * orbit
 *
 * Revision 1.2  2000/09/20 23:51:07  stu
 * HUD/AP/Scaling
 *
 * Revision 1.1  2000/08/27 01:31:26  stu
 * Hud changes
 *
 * Revision 1.0.0.1  2000/08/21 22:06:54  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h" 
  #include "3d_structures_def.h"
  #include "zex_misc.h"
  #include	"error_handler.h"
#endif

#include "objects_controls.h"
#include	"explode.h"
#include "load_objects.h"
#include "object_control_top.h"
#include "dlp_explosions.h"
#include "obj_control_misc_routines.h"

int explode_list[10];
int explode_list_type[10];	//0=small explode, 1=large explode
int explode_list_index;


// Explosion types:- 
//
// From 0 (small) to 5 (large)
//
// 0 = small
// 1 = med
// 2 = large
// 3 = 1/2 size dlp
// 4 = full size dlp
// 5 = full size dlp with lense flair


void add_to_explode_list (int current_object, int type)
{
extern int explode_list[10];
extern int explode_list_index;

     explode_list[explode_list_index]=current_object;
     explode_list_type[explode_list_index]=type;
     if (explode_list_index<10)
        explode_list_index++;
}

void reset_explode_list()
{
//extern int explode_list[10];
extern int explode_list_index;

 explode_list_index=0;
}

void explode_explode_list()
{
extern int explode_list[10];
extern int explode_list_type[10];
extern int explode_list_index;
int i;
for (i=0;i<explode_list_index;i++)
   {
   if (explode_list_type[i]==0)
      {
       do_explode(explode_list[i]);
      }

   else if (explode_list_type[i]==1)
      do_large_explode(explode_list[i]);
   
   else if (explode_list_type[i]==2)
      {
      do_large_explode(explode_list[i]);
      }
   else if (explode_list_type[i]>=3 && explode_list_type[i]<=5)
      {
   	cause_dlp_explosion(explode_list[i],explode_list_type[i]-3);
      }

   }
}

//explode
//takes an object and explodes it!
//notes:
//this routine isn't perfect because it takes each triag from the object
//and has to calculate it's base points address.
//to do this we have to replace the real connections with one that reads 0,1,2
//so about half the traigs will have their visibility wrong.
//BUT as we're exploding the thing, and all surfaces are given random rotations and vectors
//the only people who will ever know this is us!
//returns nothing
void do_explode(int object)
{
ZObject *source_object;
//ZObject test;

extern	DynObjectsFixedSize *ocb_ptr;
extern _ZPoly_objs *dynamic_polys_ptr;
extern int	dynamic_object_index;	//how many dynamic objects
extern textured_connections fixed_triag_connection [3];
//_3D *points_list_ptr_base;

int number_of_objs_to_gen,i,dest_ocb,poly_slot, size_of_polys;
_ZPoly *source_object_polys;
_ZPoly *new_polys_address;
_ZPoly *old_polys_address;
_ZPoly *new_poly;
textured_connections *new_connection;
int conn;
//float shot_z;
//load_dyn_object_pb the_params;

////insert our exploding ball
//   	slot=find_vacant_dynamic_object_slot();
////   slot=-1;	//dont fire - testing
//   	if (slot!=-1)
// 		  {
//	  	  shot_x=(*ocb_ptr).object_list[object].Oworldx;
//	  	  shot_y=(*ocb_ptr).object_list[object].Oworldy;
//	  	  shot_z=(*ocb_ptr).object_list[object].Oworldz;
//	  	  shot_z=shot_z-700;
//	  	  
//	  	  the_params.world_x=shot_x;
//	  	  the_params.world_y=shot_y;
//	  	  the_params.world_z=shot_z;
//	  	  the_params.Omovedx=0;
//	  	  the_params.Omovedy=0;
//	  	  the_params.Omovedz=0;
//	  	  the_params.delta_rot_x=0;
//	  	  the_params.delta_rot_y=0;
//	  	  the_params.delta_rot_z=0;
//	  	  the_params.rot_x=0;
//	  	  the_params.rot_y=0;
//	     	
//	  	  the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	  	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	  	  the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//
//	  	  the_params.rot_z=0;
//	  	  the_params.controller_ref=explosion;	  //controller
//	  	  the_params.control_use_1=0;	//frames to live counter
//	  	  the_params.control_use_2=0;	//use to control deceleration.
//	  	  the_params.control_use_3=0;
//	  	  the_params.control_use_4=0;
//	  	  the_params.current_velocity=0;
//	  	  the_params.wanted_velocity=0;
//	      the_params.attack_mode=0;
//	  	  the_params.shield_value=0;
// 	the_params.object_category=scenary_object;
//     the_params.has_roll=0;
//	  	  the_params.mass=0;
//	  	  load_dyn_object(explosion,slot,&the_params);	  //the static object, the position (-1=next free)
//
//    	  }	//end of good slot for ejection pod
//        add_to_kill_list(slot);	//so we dont have a ball floating about!
//now explode the object
source_object=&(*ocb_ptr).object_list[object];	//saves lots of possible code
//source_object=&(*ocb_ptr).object_list[slot];	//saves lots of possible code

source_object_polys=(*source_object).polys_ptr;
number_of_objs_to_gen=(*source_object).number_of_polys;

for (i=0;i<number_of_objs_to_gen;i++)
 {
  dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
  if (dest_ocb!=-1)
  {
//create a new object from each poly

//	(*ocb_ptr).object_list[dest_ocb]=(*ocb_ptr).object_list[static_slot];	//copy the object
	(*ocb_ptr).object_list[dest_ocb].in_use=1;	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.
	(*ocb_ptr).object_list[dest_ocb].Oid='EXP1';	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.
	(*ocb_ptr).object_list[dest_ocb].Oworldx=(*source_object).Oworldx;
	(*ocb_ptr).object_list[dest_ocb].Oworldy=(*source_object).Oworldy;
	(*ocb_ptr).object_list[dest_ocb].Oworldz=(*source_object).Oworldz;
//	(*ocb_ptr).object_list[dest_ocb].Orotdx=RangedRdm(0,20);
//	(*ocb_ptr).object_list[dest_ocb].Orotdy=RangedRdm(0,20);
//	(*ocb_ptr).object_list[dest_ocb].Orotdz=RangedRdm (0,20);
	(*ocb_ptr).object_list[dest_ocb].Orotx=(*source_object).Orotx;
	(*ocb_ptr).object_list[dest_ocb].Oroty=(*source_object).Oroty;
	(*ocb_ptr).object_list[dest_ocb].Orotz=(*source_object).Orotz;
//	(*ocb_ptr).object_list[dest_ocb].Omovedx=RangedRdm(0,30)-15;
//	(*ocb_ptr).object_list[dest_ocb].Omovedy=RangedRdm(0,30)-15;
//	(*ocb_ptr).object_list[dest_ocb].Omovedz=RangedRdm(0,20)-10;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.controller=burning_particle;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.allegiance=SCENARY;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_1=RangedRdm(25,50);	     //controller counts this down
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_2=0;

	  (*ocb_ptr).object_list[dest_ocb].hit_by_slot=-1;
	  (*ocb_ptr).object_list[dest_ocb].crashable=NO;	//this object does not need col_detect
//	(*ocb_ptr).object_list[dest_ocb].number_owned=0;
//	(*ocb_ptr).object_list[dest_ocb].Oreference=1;
//	(*ocb_ptr).object_list[dest_ocb].immediate_child=NULL;
	  (*ocb_ptr).object_list[dest_ocb].number_of_polys=1;
	  (*ocb_ptr).object_list[dest_ocb].number_of_points=3;
	(*ocb_ptr).object_list[dest_ocb].anim_frame_rate=0;
	(*ocb_ptr).object_list[dest_ocb].anim_counter=0;
	(*ocb_ptr).object_list[dest_ocb].anim_frame=0;
	(*ocb_ptr).object_list[dest_ocb].anim_max_object=1;	//not animated

//now copy polys out of static into vacant dynamic
	poly_slot=find_vacant_dyn_poly_slot();
	if	(poly_slot!=-1)
 	 {
	  (*dynamic_polys_ptr)[poly_slot].in_use=1;	//prevent a crash - this poly slot is in use!
	  //copy the polys across
	  new_polys_address=&(*dynamic_polys_ptr)[poly_slot].the_obj_polys[0];	//so we can poke the new object
	  old_polys_address=source_object_polys+i;	//point to right poly!
	  size_of_polys=sizeof(_ZPoly);	//we have just one poly to copy	
	  BlockMoveData	(old_polys_address,new_polys_address,size_of_polys);
	  //poke the address of the polys into the new object
      (*ocb_ptr).object_list[dest_ocb].polys_ptr=new_polys_address;
	  (*ocb_ptr).object_list[dest_ocb].active_polys_slot=poly_slot;	//so we can quickly kill these polys
     
//points ptr needs to come from the first connection in the poly
	  new_poly=new_polys_address;
      new_connection=((*new_poly).poly_connections_ptr);
	  conn=(*new_connection).the_node;	//first connection in this triag
	  (*new_polys_address).poly_connections_ptr=fixed_triag_connection;	//poke our new connections in - 1,2,3

	  (*ocb_ptr).object_list[dest_ocb].points_ptr=(*source_object).points_ptr+conn;	//now originated at 0
	 }	//end of poly slot
	dynamic_object_index++;	//we've added an object that needs to be processed

  }	//end of dest_ocb!=-1
 
 } //end of for
// kill (object);	//remove original object (the thing that exploded)
}

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//large explode - same as explode but also explodes a ball in the vicinity of the object
void do_large_explode(int object)
{
ZObject *source_object;
//ZObject test;

extern	DynObjectsFixedSize *ocb_ptr;
extern _ZPoly_objs *dynamic_polys_ptr;
extern int	dynamic_object_index;	//how many dynamic objects
extern textured_connections fixed_triag_connection [3];
//_3D *points_list_ptr_base;

int number_of_objs_to_gen,i,dest_ocb,poly_slot, size_of_polys;
_ZPoly *source_object_polys;
_ZPoly *new_polys_address;
_ZPoly *old_polys_address;
_ZPoly *new_poly;
textured_connections *new_connection;
int conn,slot;
float shot_x,shot_y,shot_z;
load_dyn_object_pb the_params;


clear_pb(&the_params);	//saves loads of zero code
//insert our exploding ball
   	slot=find_vacant_dynamic_object_slot();
//   slot=-1;	//dont fire - testing
   	if (slot!=-1)
 		{
		shot_x=(*ocb_ptr).object_list[object].Oworldx;
		shot_y=(*ocb_ptr).object_list[object].Oworldy;
		shot_z=(*ocb_ptr).object_list[object].Oworldz;
		shot_z=shot_z-700;
		
		the_params.world_x=shot_x;
		the_params.world_y=shot_y;
		the_params.world_z=shot_z;
//		the_params.Omovedx=0;
//		the_params.Omovedy=0;
//		the_params.Omovedz=0;
//		the_params.delta_rot_x=0;
//		the_params.delta_rot_y=0;
//		the_params.delta_rot_z=0;
//		the_params.rot_x=0;
//		the_params.rot_y=0;
	   	
//		the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//		the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//		the_params.normal.z =  0;	//objects are ALWAYS defined facing right

//		the_params.rot_z=0;
		the_params.controller_ref=explosion;	//controller
//		the_params.control_use_1=0;	//frames to live counter
//		the_params.control_use_2=0;	//use to control deceleration.
//		the_params.control_use_3=0;
//		the_params.control_use_4=0;
//	the_params.control_use_7=0;
//		the_params.current_velocity=0;
//		the_params.wanted_velocity=0;
//	the_params.attack_mode=0;
//		the_params.shield_value=0;
//     the_params.has_roll=0;
 	    the_params.object_category=SCENARY;
//		the_params.mass=0;
//			 	 the_params.dq=0;
//	the_params.laser_cat=0;
//	the_params.cannon_cat=0;
//	the_params.missile_cat=0;
//	the_params.number_of_missiles=0;
//	the_params.unique_id=0;
//     the_params.who_spawned_this_object=0;
		load_dyn_object(explosion,slot,&the_params,-1,1,-1,!DUST_BIT);	  //the static object, the position (-1=next free)

    	}	//end of good slot for ejection pod
        
		if (add_to_kill_list(slot)!=0)	//so we dont have a ball floating about!
		  {
			MyDebugStr(__LINE__, __FILE__,"add_to_kill_list failed!");
		  }
	//now explode the ball
//source_object=&(*ocb_ptr).object_list[object];	//saves lots of possible code
source_object=&(*ocb_ptr).object_list[slot];	//saves lots of possible code

source_object_polys=(*source_object).polys_ptr;
number_of_objs_to_gen=(*source_object).number_of_polys;

for (i=0;i<number_of_objs_to_gen;i++)
 {
  dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
  if (dest_ocb!=-1)
  {
//create a new object from each poly

//	(*ocb_ptr).object_list[dest_ocb]=(*ocb_ptr).object_list[static_slot];	//copy the object
	(*ocb_ptr).object_list[dest_ocb].in_use=1;	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.
	(*ocb_ptr).object_list[dest_ocb].Oid='EXP1';	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.
	(*ocb_ptr).object_list[dest_ocb].Oworldx=(*source_object).Oworldx;
	(*ocb_ptr).object_list[dest_ocb].Oworldy=(*source_object).Oworldy;
	(*ocb_ptr).object_list[dest_ocb].Oworldz=(*source_object).Oworldz;
//	(*ocb_ptr).object_list[dest_ocb].Orotdx=RangedRdm(0,20);
//	(*ocb_ptr).object_list[dest_ocb].Orotdy=RangedRdm(0,20);
//	(*ocb_ptr).object_list[dest_ocb].Orotdz=RangedRdm (0,20);
	(*ocb_ptr).object_list[dest_ocb].Orotx=(*source_object).Orotx;
	(*ocb_ptr).object_list[dest_ocb].Oroty=(*source_object).Oroty;
	(*ocb_ptr).object_list[dest_ocb].Orotz=(*source_object).Orotz;
//	(*ocb_ptr).object_list[dest_ocb].Omovedx=RangedRdm(0,30)-15;
//	(*ocb_ptr).object_list[dest_ocb].Omovedy=RangedRdm(0,30)-15;
//	(*ocb_ptr).object_list[dest_ocb].Omovedz=RangedRdm(0,20)-10;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.controller=burning_particle;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_1=RangedRdm(25,50);	     //controller counts this down
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_2=0;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.allegiance=SCENARY;

	  (*ocb_ptr).object_list[dest_ocb].hit_by_slot=-1;
	  (*ocb_ptr).object_list[dest_ocb].crashable=NO;	//this object does not need col_detect
//	(*ocb_ptr).object_list[dest_ocb].number_owned=0;
//	(*ocb_ptr).object_list[dest_ocb].Oreference=1;
//	(*ocb_ptr).object_list[dest_ocb].immediate_child=NULL;
	  (*ocb_ptr).object_list[dest_ocb].number_of_polys=1;
	  (*ocb_ptr).object_list[dest_ocb].number_of_points=3;
	(*ocb_ptr).object_list[dest_ocb].anim_frame_rate=0;
	(*ocb_ptr).object_list[dest_ocb].anim_counter=0;
	(*ocb_ptr).object_list[dest_ocb].anim_frame=0;
	(*ocb_ptr).object_list[dest_ocb].anim_max_object=1;	//not animated

//now copy polys out of static into vacant dynamic
	poly_slot=find_vacant_dyn_poly_slot();
	if	(poly_slot!=-1)
 	 {
	  (*dynamic_polys_ptr)[poly_slot].in_use=1;	//prevent a crash - this poly slot is in use!
	  //copy the polys across
	  new_polys_address=&(*dynamic_polys_ptr)[poly_slot].the_obj_polys[0];	//so we can poke the new object
	  old_polys_address=source_object_polys+i;	//point to right poly!
	  size_of_polys=sizeof(_ZPoly);	//we have just one poly to copy	
	  BlockMoveData	(old_polys_address,new_polys_address,size_of_polys);
	  //poke the address of the polys into the new object
      (*ocb_ptr).object_list[dest_ocb].polys_ptr=new_polys_address;
	  (*ocb_ptr).object_list[dest_ocb].active_polys_slot=poly_slot;	//so we can quickly kill these polys
     
//points ptr needs to come from the first connection in the poly
	  new_poly=new_polys_address;
      new_connection=((*new_poly).poly_connections_ptr);
	  conn=(*new_connection).the_node;	//first connection in this triag
	  (*new_polys_address).poly_connections_ptr=fixed_triag_connection;	//poke our new connections in - 1,2,3

	  (*ocb_ptr).object_list[dest_ocb].points_ptr=(*source_object).points_ptr+conn;	//now originated at 0
	 }	//end of poly slot
	dynamic_object_index++;	//we've added an object that needs to be processed

  }	//end of dest_ocb!=-1
 
 } //end of for
 
//mow explode the object as well
source_object=&(*ocb_ptr).object_list[object];	//saves lots of possible code
//source_object=&(*ocb_ptr).object_list[slot];	//saves lots of possible code

source_object_polys=(*source_object).polys_ptr;
number_of_objs_to_gen=(*source_object).number_of_polys;

for (i=0;i<number_of_objs_to_gen;i++)
 {
  dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
  if (dest_ocb!=-1)
  {
//create a new object from each poly

//	(*ocb_ptr).object_list[dest_ocb]=(*ocb_ptr).object_list[static_slot];	//copy the object
	(*ocb_ptr).object_list[dest_ocb].in_use=1;	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.
	(*ocb_ptr).object_list[dest_ocb].Oworldx=(*source_object).Oworldx;
	(*ocb_ptr).object_list[dest_ocb].Oworldy=(*source_object).Oworldy;
	(*ocb_ptr).object_list[dest_ocb].Oworldz=(*source_object).Oworldz;
//	(*ocb_ptr).object_list[dest_ocb].Orotdx=RangedRdm(0,20);
//	(*ocb_ptr).object_list[dest_ocb].Orotdy=RangedRdm(0,20);
//	(*ocb_ptr).object_list[dest_ocb].Orotdz=RangedRdm (0,20);
	(*ocb_ptr).object_list[dest_ocb].Orotx=(*source_object).Orotx;
	(*ocb_ptr).object_list[dest_ocb].Oroty=(*source_object).Oroty;
	(*ocb_ptr).object_list[dest_ocb].Orotz=(*source_object).Orotz;
//	(*ocb_ptr).object_list[dest_ocb].Omovedx=RangedRdm(0,30)-15;
//	(*ocb_ptr).object_list[dest_ocb].Omovedy=RangedRdm(0,30)-15;
//	(*ocb_ptr).object_list[dest_ocb].Omovedz=RangedRdm(0,20)-10;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.controller=exploding_object;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_1=RangedRdm(25,50);	     //controller counts this down
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_2=0;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.allegiance=SCENARY;

	  (*ocb_ptr).object_list[dest_ocb].hit_by_slot=-1;
	  (*ocb_ptr).object_list[dest_ocb].crashable=NO;	//this object does not need col_detect
//	(*ocb_ptr).object_list[dest_ocb].number_owned=0;
//	(*ocb_ptr).object_list[dest_ocb].Oreference=1;
//	(*ocb_ptr).object_list[dest_ocb].immediate_child=NULL;
	  (*ocb_ptr).object_list[dest_ocb].number_of_polys=1;
	  (*ocb_ptr).object_list[dest_ocb].number_of_points=3;
	(*ocb_ptr).object_list[dest_ocb].anim_frame_rate=0;
	(*ocb_ptr).object_list[dest_ocb].anim_counter=0;
	(*ocb_ptr).object_list[dest_ocb].anim_frame=0;
	(*ocb_ptr).object_list[dest_ocb].anim_max_object=1;	//not animated

//now copy polys out of static into vacant dynamic
	poly_slot=find_vacant_dyn_poly_slot();
	if	(poly_slot!=-1)
 	 {
	  (*dynamic_polys_ptr)[poly_slot].in_use=1;	//prevent a crash - this poly slot is in use!
	  //copy the polys across
	  new_polys_address=&(*dynamic_polys_ptr)[poly_slot].the_obj_polys[0];	//so we can poke the new object
	  old_polys_address=source_object_polys+i;	//point to right poly!
	  size_of_polys=sizeof(_ZPoly);	//we have just one poly to copy	
	  BlockMoveData	(old_polys_address,new_polys_address,size_of_polys);
	  //poke the address of the polys into the new object
      (*ocb_ptr).object_list[dest_ocb].polys_ptr=new_polys_address;
	  (*ocb_ptr).object_list[dest_ocb].active_polys_slot=poly_slot;	//so we can quickly kill these polys
     
//points ptr needs to come from the first connection in the poly
	  new_poly=new_polys_address;
      new_connection=((*new_poly).poly_connections_ptr);
	  conn=(*new_connection).the_node;	//first connection in this triag
	  (*new_polys_address).poly_connections_ptr=fixed_triag_connection;	//poke our new connections in - 1,2,3

	  (*ocb_ptr).object_list[dest_ocb].points_ptr=(*source_object).points_ptr+conn;	//now originated at 0
	 }	//end of poly slot
	dynamic_object_index++;	//we've added an object that needs to be processed

  }	//end of dest_ocb!=-1
 
 } //end of for

// kill (object);	//remove original object (the thing that exploded)
}
