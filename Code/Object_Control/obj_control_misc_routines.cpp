/*
 * $Log: object_control_misc_routines.c,v $
 * Revision 1.1.1.1  2003/09/05 22:36:07  stu_c
 * First Imported.
 *
 * Revision 1.16  2002/09/08 02:13:22  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.15  2002/07/28 17:12:58  stu_c
 * Exhausts and further particle work
 *
 * Revision 1.14  2002/05/31 23:54:32  stu
 * Removed all warnings from the code
 *
 * Revision 1.13  2002/05/05 22:28:39  stu
 * Further developement - hull temperature now correct for multiple suns and atmosphere.

All calls to add_to_kill_list check the return
 *
 * Revision 1.12  2002/04/28 06:23:07  stu
 * This checkin adds more to the HUD and brings out some sim variables to the player.
 *
 * Revision 1.11  2002/04/21 14:45:11  stu
 * Further work
 *
 * Revision 1.10  2002/04/14 16:00:48  stu
 * Further work
 *
 * Revision 1.9  2002/04/13 15:32:51  stu
 * Rotation, Alycians, behaviour
 *
 * Revision 1.8  2002/03/19 23:16:33  stu
 * Clear control_use_6 in clear_pb
 *
 * Revision 1.7  2002/03/16 00:44:47  stu
 * Continued development
 *
 * Revision 1.6  2002/02/04 23:08:08  stu
 * ADded get_unique_object_id (for bolts)
 *
 * Revision 1.5  2002/01/20 17:10:59  stu
 * removed planet_on_screen
 *
 * Revision 1.4  2001/11/02 19:32:14  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.3  2001/10/24 21:40:01  stu
 * First set of source to build on pb for carbon.
 *
 * Revision 1.2  2001/10/22 21:28:59  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:05:33  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:06  rob
 * First Imported.
 *
 * Revision 1.6  2001/06/23 23:09:43  stu
 * 0.66
 *
 * Revision 1.5  2001/06/13 18:19:41  rob
 * Two lines added - ????
 *
 * Revision 1.4  2001/06/03 03:21:16  stu
 * OCB structure now contains a sub structure called:
ZObjectControl
Which contains all the control data for an object.

Added accessor in access_ocbs which returns a pointer to type ZObjectControl
 *
 * Revision 1.3  2001/05/28 01:54:27  stu
 * 280501
 *
 * Revision 1.2  2001/04/01 18:22:02  stu
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2001/01/01 21:13:38  rob
 * First Imported.
 *
 * Revision 1.3  2000/12/03 23:52:31  rob
 * changed constants for object type to get uniformity
 *
 * Revision 1.2  2000/11/16 20:40:05  rob
 * orbit
 *
 * Revision 1.1  2000/09/20 23:50:32  stu
 * HUD/AP/Scaling
 *
 * Revision 1.0.0.1  2000/08/21 22:08:09  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"
  #include "3d_structures_def.h"
  #include	"zex_misc.h"
  #include	"error_handler.h"
#endif

#include	"object_control_top.h"
#include	"objects_controls.h"	//controller definitions
#include	"load_objects.h"
#include "explode.h"
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

extern kit_desc ship_items[ITEMS_COUNT];
extern int hull_strength,credits,score;
//ееееееее















//еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
void update_mass(int the_object)
{
int i;
float mass=0;
extern	DynObjectsFixedSize *ocb_ptr;
//this function updates the mass of the ship from:
// bk_mass_tonnes
// cargo bay

//loaded from SDS?
 if ((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.was_loaded_from_sds!=0)
  {
   for (i=0;i<(*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.cargo_capacity_tonnes;i++)
    {
    if ((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.cargo_bay[i]!=-1) 
     {
      mass+=1.01;  //Containers weigh 10Kgs	
     }
    }
  
  mass+=(*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.bk_mass_tonnes;
  mass+=(*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.fuel_tank_value_in_KGS/1000;
  (*ocb_ptr).object_list[the_object].mass=mass;
  }
}



//used for any object except Zex to damage shields and check for death
//returns 1 if object killed
int damage_object (int current_object, int shield_damage)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int flash_screen;

int hit_by_what,who_fired,explosion_class;
int return_val=0;


//modify for Zex's shield strength
     if (current_object==zex)
     {
      int a;
      a=ship_items[KSHIELDS].modifier-1;	//1,2,3,4,5
      shield_damage-=a*6;
      if (shield_damage<1) shield_damage=1;
     }
     
     (*ocb_ptr).object_list[current_object].Dyn_OCB_control_data.shield_value-=shield_damage;

     if ((*ocb_ptr).object_list[current_object].Dyn_OCB_control_data.shield_value<0 && 
     (*ocb_ptr).object_list[current_object].Oid!='LAS1')	//we dont explode lasers!
       {
       int temp,id;
       
       return_val=1;
//change 070699
//randomly we sometimes send these to tumble rather than immediately destroyed
         id=(*ocb_ptr).object_list[current_object].Oid;
         if (RangedRdm(0,10)>5)
         {
            if (id!='MIS1' && id!='MIS2' && id!='MIS3' && id!='IODA')	//we do not tumble bullets OR iodevices!
             {
              add_to_score_and_credits(current_object);
              tumble_object(current_object);
              flash_screen=2;
              return 1;
             }
         }
         //calc explosion effect
//         if ((*ocb_ptr).object_list[current_object].object_category==baddie_object)	  //score and credits
//          add_to_score_and_credits(current_object);

         switch (id)
         {
          case 'ZZ1A': case 'ZZ1B' : case 'ZZ1C': case 'WARP': case 'MON6': case 'IODA':
          {
          explosion_class=5;
          }


          case 'MON2': case 'MON3':
          {
          explosion_class=3;
          }
          
          default:
          {
          Byte *bp;
          
             bp = (Byte *)&id;
          //see if ionic device
          if (bp[0]=='I' && bp[1]=='O') explosion_class=5;
          else
          if (bp[0]=='M' && bp[1]=='N') explosion_class=4;
          else
          explosion_class=0;
          }
         }
         
         add_to_explode_list(current_object,explosion_class);


        
  //if the thing that destroyed us was fired by zex then score etc.
         if ((*ocb_ptr).object_list[current_object].Dyn_OCB_control_data.callsign!=0) 
         {
//          do_callsign_text("     DESTROYED!",current_object,2);	//callsign goes in first 4 chars of string
          
//          if (score>1000) score-=1000;
//	else score=0;

          }
          
          
         temp=(*ocb_ptr).object_list[current_object].hit_by_slot; //what object hit this object
         
          if ((*ocb_ptr).object_list[temp].Dyn_OCB_control_data.allegiance==HUMAN)	//was the thing that hit this object a zex object
          { add_to_score_and_credits(current_object); }
         
          if (add_to_kill_list (current_object)!=0)
			{
			  MyDebugStr(__LINE__, __FILE__,"add_to_kill_list failed!");
			}
		            
          if ((*ocb_ptr).object_list[current_object].Oid=='IODA')
          {
          play_zsound_relative_to_zex(current_object, explosion4);
          play_zsound_relative_to_zex(current_object, screech);
          }
          else          
          {
          play_zsound_relative_to_zex(current_object, short_explosion);
          }
          //find out if it was a wingman that killed object and issue message if so
          hit_by_what=(*ocb_ptr).object_list[current_object].hit_by_slot;
          if (hit_by_what!=-1)	//180699 - fix crash bug
          {
           who_fired=(*ocb_ptr).object_list[hit_by_what].parent;
           if (who_fired!=-1)
            {
//             if ((*ocb_ptr).object_list[who_fired].controller==WINGMAN_FOR_GAME) wingman_diealien_text(who_fired);
            }
          }
        }

return return_val;

}





//this routine takes an object and changes its controller to
//tumbling_to_explode_object which will ultimately destroy it
//in the meantime however it is assigned a tumble direction and speed
void tumble_object(int the_object)
{
}

//and sent tumbling off into space
//void tumble_object(int the_object)
//{
//int temp,delta_x,delta_y,hit_by;
//add_to_explode_list(the_object,5);
//hit_by=(*ocb_ptr).object_list[the_object].hit_by_slot;
//if (hit_by!=-1)
// {
//    delta_x=(*ocb_ptr).object_list[hit_by].Omovedx/5;
//    delta_y=(*ocb_ptr).object_list[hit_by].Omovedy/5;
// }
//else
//{
//    temp=RangedRdm(0,80);
//    if (RangedRdm(0,10)>5) temp=-temp;
//    delta_x=temp;
//
//    temp=RangedRdm(0,80);
//    if (RangedRdm(0,10)>5) temp=-temp;
//    delta_y=temp;
//
//}
//
//
//(*ocb_ptr).object_list[the_object].controller=tumbling_to_explode_object;
//(*ocb_ptr).object_list[the_object].crashable=0;
//(*ocb_ptr).object_list[the_object].control_use_1=30;	  //frames to live
//(*ocb_ptr).object_list[the_object].Omovedx=delta_x;
//(*ocb_ptr).object_list[the_object].Omovedy=delta_y;
//(*ocb_ptr).object_list[the_object].Omovedz=-60;
////(*ocb_ptr).object_list[the_object].Orotdz=RangedRdm(30,60);
////(*ocb_ptr).object_list[the_object].Orotdy=RangedRdm(10,40);
//
//        play_zsound_relative_to_zex(the_object, short_explosion);
//
//}

static int object_unique_id=20000;	//we allow 20000 for game loader and run from 20000 for dynamic id's

int get_object_unique_id(void)
{
 if (object_unique_id<20000) object_unique_id=20000;
 return object_unique_id++;
 
}

//find the object with the unique id
int find_unique_id(int the_id)
{
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects
extern int dynamic_object_index;

int i,active_object_count;

i=active_object_count=0;
while (active_object_count<dynamic_object_index)
{
  if ((*ocb_ptr).object_list[i].in_use==1)
  {
   if ((*ocb_ptr).object_list[i].unique_id==the_id)
   {
     return i;
     break;
   }
   active_object_count++;
 }
 i++;
}
return -1;

}


//given a pointer to a pb, clear it
//saves loads of code!
void clear_pb(load_dyn_object_pb* the_pb)
{

 the_pb->delta_rot_x=the_pb->delta_rot_y=the_pb->delta_rot_z=0;
 the_pb->rot_x=the_pb->rot_y=the_pb->rot_z=0;
 the_pb->controller_ref=0;
 
 the_pb->control_use_1=0;
 the_pb->control_use_2=0;
 the_pb->control_use_3=0;
 the_pb->control_use_4=0;
 the_pb->control_use_1_float=0;
 the_pb->control_use_5=0;
 
 the_pb->normal.y=0;
 the_pb->normal.z=0;
 the_pb->normal.x=1;
 
 the_pb->aggressive=1;
 the_pb->NPC_class=-1;
 the_pb->shields_class=1;
 the_pb->max_vel=0;
 the_pb->min_vel=0;

 the_pb->attack_mode=0;
 the_pb->parent=-1;
 
 the_pb->shield_value=the_pb->mass=0;
 the_pb->dq=0;
 the_pb->no_trivial_rejection=0;
 the_pb->object_category=0;
 the_pb->laser_cat=0;
 the_pb->cannon_cat=0;
 the_pb->missile_cat=0;
 the_pb->number_of_missiles=0;
 the_pb->unique_id=0;
 the_pb->kill_flags=0;
 the_pb->invisible_flag=0;
 the_pb->init_check='chek';

}
