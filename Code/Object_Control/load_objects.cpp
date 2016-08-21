// *************************************MODULE HEADER*********************************
// * FILENAME           : load_objects.cpp
// * MODULE TITLE       : load_objects.cpp
// * PROJECT            : Zex
// * DATE STARTED       : 22:20:32 on 22nd June 1999
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
 * $Log: load_objects.cpp,v $
 * Revision 1.5  2003/09/28 17:29:46  robp
 * Changed files from .c to .cpp and removed spaces out of a couple of filenames.
 *
 * Revision 1.4  2003/09/27 08:58:30  robp
 * Removal of Invalid Conversion Warning for C++ (I hate unsigned/signed chars)
 *
 * Revision 1.3  2003/09/22 20:45:59  stu_c
 * Zex:Weapons: laser bay temperature and overheat (in progress)
 *
 * Revision 1.2  2003/09/20 12:57:04  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:36:03  stu_c
 * First Imported.
 *
 * Revision 1.29  2002/09/08 02:14:14  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.28  2002/08/31 19:25:16  stu_c
 * Callsigns
 *
 * Revision 1.27  2002/08/11 22:07:31  stu_c
 * behaviour_aggressive started, falcons from motherships.
 *
 * Revision 1.26  2002/07/28 17:11:29  stu_c
 * Exhausts and further particle work
 *
 * Revision 1.25  2002/07/09 21:44:02  stu_c
 * NPC lasers; might become exhausts
 *
 * Revision 1.24  2002/06/16 23:19:20  stu
 * Particles booked in for backup purposes - not working yet or complete.
 *
 * Revision 1.23  2002/05/31 23:53:57  stu
 * Removed all warnings from the code
 *
 * Revision 1.22  2002/05/19 03:18:08  stu
 * Guns fix
 *
 * Revision 1.21  2002/04/28 06:22:21  stu
 * This checkin adds more to the HUD and brings out some sim variables to the player.
 *
 * Revision 1.20  2002/04/14 16:00:34  stu
 * Further work
 *
 * Revision 1.19  2002/04/13 15:28:51  stu
 * Rotation, Alycians, behaviour
 *
 * Revision 1.18  2002/03/16 00:43:12  stu
 * Continued development
 *
 * Revision 1.17  2002/02/24 04:52:33  stu
 * Just some formatting
 *
 * Revision 1.16  2002/02/16 19:03:57  stu
 * Radar range decrease key and rationalisation of dlp colours
 *
 * Revision 1.15  2002/02/10 21:36:53  stu
 * fast_dyn_objects
 *
 * Revision 1.14  2002/02/04 00:46:36  rob
 * ensure LOD code honours galactic id
 *
 * Revision 1.13  2002/02/03 19:32:41  stu
 * swap_dyn_object now inlines setting of the blobbed colour
 *
 * Revision 1.12  2002/02/02 23:09:43  stu
 * new function - swap_dyn_object still in-progress
 *
 * Revision 1.11  2002/01/09 23:48:42  stu
 * Furhter work on trading.
 *
 * Revision 1.10  2001/12/31 20:03:12  stu
 * More error reporting in load_dyn_object
 *
 * Revision 1.9  2001/12/29 01:13:04  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.8  2001/12/04 23:36:33  stu
 * Motherships work
 *
 * Revision 1.7  2001/11/26 23:29:30  rob
 * Whitespace changes
 *
 * Revision 1.6  2001/11/24 19:30:11  stu
 * *** empty log message ***
 *
 * Revision 1.5  2001/11/05 20:01:19  stu
 * More OSX work
 *
 * Revision 1.4  2001/11/02 23:19:54  stu
 * Further OS X work. Enabled syscon logging.
 *
 * Revision 1.3  2001/11/02 19:32:21  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:28:07  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:05:16  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:06  rob
 * First Imported.
 *
 * Revision 1.12  2001/09/10 21:39:47  stu
 * 100901
More Sim, AP broken.
 *
 * Revision 1.11  2001/09/10 20:44:04  rob
 * 1. Stopped linear dampers.
2. Turned off test system.
3. Altered so solar system map comes up.
 *
 * Revision 1.10  2001/09/04 20:38:15  stu
 * Sim stuff
 *
 * Revision 1.9  2001/06/23 23:10:02  stu
 * 0.66
 *
 * Revision 1.8  2001/06/21 20:30:50  rob
 * make so load object doesn't quit on bad load
 *
 * Revision 1.7  2001/06/13 18:22:56  rob
 * Object created alterations
 *
 * Revision 1.6  2001/06/03 03:27:15  stu
 * OCB structure now contains a sub structure called:
ZObjectControl
Which contains all the control data for an object.

Added accessor in access_ocbs which returns a pointer to type ZObjectControl
 *
 * Revision 1.5  2001/06/01 21:36:31  rob
 * Space change
 *
 * Revision 1.4  2001/05/28 01:50:09  stu
 * 280501
 *
 * Revision 1.3  2001/04/01 18:28:05  stu
 * *** empty log message ***
 *
 * Revision 1.2  2001/01/15 19:55:32  stu
 * Bolts, fuel freeze
 *
 * Revision 1.1.1.1  2001/01/01 21:13:38  rob
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

  #include "3d_structures_def.h"
  #include "zex_misc.h"
  #include "access_ocbs.h"
#endif //pch


#include "file_io.h"
#include "MeshToZex.h"
#include "load_objects.h"
#include "objects_controls.h"
#include "z_init.h"
#include "pictures.h"
#include "texture_management.h"
#include "Error_handler.h"
#include "load_game.h"
#include "light_control.h"
#include "dlp.h"
#include "obj_control_misc_routines.h"
#include "matrix_primitives.h"
#include "physics_gravity.h"
#include "object_control_top.h"
#include "autopilot.h"
#include "static_object_support.h"
#include "fast_dyn.h"
#include "coffee.h"
#include "behaviour.h"
#include "particle_dyn.h"
#include "call_signs.h"
#include "object_manipulation.h"
#include "physics.h"

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *





// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *
int	static_object_index;	//how many static objects

char *taddr;


// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *





//Zex Load objects
//SB 14/1198
/*
 * $Log: load_objects.cpp,v $
 * Revision 1.5  2003/09/28 17:29:46  robp
 * Changed files from .c to .cpp and removed spaces out of a couple of filenames.
 *
 * Revision 1.4  2003/09/27 08:58:30  robp
 * Removal of Invalid Conversion Warning for C++ (I hate unsigned/signed chars)
 *
 * Revision 1.3  2003/09/22 20:45:59  stu_c
 * Zex:Weapons: laser bay temperature and overheat (in progress)
 *
 * Revision 1.2  2003/09/20 12:57:04  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:36:03  stu_c
 * First Imported.
 *
 * Revision 1.29  2002/09/08 02:14:14  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.28  2002/08/31 19:25:16  stu_c
 * Callsigns
 *
 * Revision 1.27  2002/08/11 22:07:31  stu_c
 * behaviour_aggressive started, falcons from motherships.
 *
 * Revision 1.26  2002/07/28 17:11:29  stu_c
 * Exhausts and further particle work
 *
 * Revision 1.25  2002/07/09 21:44:02  stu_c
 * NPC lasers; might become exhausts
 *
 * Revision 1.24  2002/06/16 23:19:20  stu
 * Particles booked in for backup purposes - not working yet or complete.
 *
 * Revision 1.23  2002/05/31 23:53:57  stu
 * Removed all warnings from the code
 *
 * Revision 1.22  2002/05/19 03:18:08  stu
 * Guns fix
 *
 * Revision 1.21  2002/04/28 06:22:21  stu
 * This checkin adds more to the HUD and brings out some sim variables to the player.
 *
 * Revision 1.20  2002/04/14 16:00:34  stu
 * Further work
 *
 * Revision 1.19  2002/04/13 15:28:51  stu
 * Rotation, Alycians, behaviour
 *
 * Revision 1.18  2002/03/16 00:43:12  stu
 * Continued development
 *
 * Revision 1.17  2002/02/24 04:52:33  stu
 * Just some formatting
 *
 * Revision 1.16  2002/02/16 19:03:57  stu
 * Radar range decrease key and rationalisation of dlp colours
 *
 * Revision 1.15  2002/02/10 21:36:53  stu
 * fast_dyn_objects
 *
 * Revision 1.14  2002/02/04 00:46:36  rob
 * ensure LOD code honours galactic id
 *
 * Revision 1.13  2002/02/03 19:32:41  stu
 * swap_dyn_object now inlines setting of the blobbed colour
 *
 * Revision 1.12  2002/02/02 23:09:43  stu
 * new function - swap_dyn_object still in-progress
 *
 * Revision 1.11  2002/01/09 23:48:42  stu
 * Furhter work on trading.
 *
 * Revision 1.10  2001/12/31 20:03:12  stu
 * More error reporting in load_dyn_object
 *
 * Revision 1.9  2001/12/29 01:13:04  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.8  2001/12/04 23:36:33  stu
 * Motherships work
 *
 * Revision 1.7  2001/11/26 23:29:30  rob
 * Whitespace changes
 *
 * Revision 1.6  2001/11/24 19:30:11  stu
 * *** empty log message ***
 *
 * Revision 1.5  2001/11/05 20:01:19  stu
 * More OSX work
 *
 * Revision 1.4  2001/11/02 23:19:54  stu
 * Further OS X work. Enabled syscon logging.
 *
 * Revision 1.3  2001/11/02 19:32:21  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:28:07  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:05:16  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:06  rob
 * First Imported.
 *
 * Revision 1.12  2001/09/10 21:39:47  stu
 * 100901
More Sim, AP broken.
 *
 * Revision 1.11  2001/09/10 20:44:04  rob
 * 1. Stopped linear dampers.
2. Turned off test system.
3. Altered so solar system map comes up.
 *
 * Revision 1.10  2001/09/04 20:38:15  stu
 * Sim stuff
 *
 * Revision 1.9  2001/06/23 23:10:02  stu
 * 0.66
 *
 * Revision 1.8  2001/06/21 20:30:50  rob
 * make so load object doesn't quit on bad load
 *
 * Revision 1.7  2001/06/13 18:22:56  rob
 * Object created alterations
 *
 * Revision 1.6  2001/06/03 03:27:15  stu
 * OCB structure now contains a sub structure called:
ZObjectControl
Which contains all the control data for an object.

Added accessor in access_ocbs which returns a pointer to type ZObjectControl
 *
 * Revision 1.5  2001/06/01 21:36:31  rob
 * Space change
 *
 * Revision 1.4  2001/05/28 01:50:09  stu
 * 280501
 *
 * Revision 1.3  2001/04/01 18:28:05  stu
 * *** empty log message ***
 *
 * Revision 1.2  2001/01/15 19:55:32  stu
 * Bolts, fuel freeze
 *
 * Revision 1.1.1.1  2001/01/01 21:13:38  rob
 * First Imported.
 *
 * Revision 1.12  2000/12/11 19:47:20  stu
 * Light fix in 3d_top - calc_normal
 *
 * Revision 1.11  2000/12/10 03:33:06  stu
 * Calls to kill replaced with either calls to add_to_kill_list OR
i_know_what_im_doing_kill

Dust now self lit.
 *
 * Revision 1.10  2000/12/03 23:52:16  rob
 * changed constants for object type to get uniformity
 *
 * Revision 1.9  2000/11/16 20:39:21  rob
 * orbit
 *
 * Revision 1.8  2000/10/19 19:07:43  stu
 * Relative velocities, turbulence, planet names.
 *
 * Revision 1.7  2000/10/11 21:29:05  stu
 * Finished off 64 bit conversion. Made engines bigger.
AP tweaks.
 *
 * Revision 1.6  2000/10/11 00:08:56  rob
 * add error message to failed find dyn object slot
 *
 * Revision 1.5  2000/10/08 21:48:13  stu
 * Engine conversion to 64 bit. Huge pain of a job.
 *
 * Revision 1.4  2000/10/01 13:29:59  rob
 * Stu's changes to load objects so that it can scale different copies of the same object properly.
 *
 * Revision 1.3  2000/09/20 23:45:06  stu
 * HUD/AP/Scaling
 *
 * Revision 1.2  2000/09/13 20:35:37  stu
 * Changes for hud targetting and sun corona effects.
 *
 * Revision 1.1  2000/08/27 16:28:05  stu
 * Changes for movement decoupling in time.
 *
 * Revision 1.0.0.1  2000/08/21 22:07:21  stu
 * First Imported.
 *
 * Revision 1.0.0.1  2000/08/11 23:19:33  stu
 * First Imported.
 *
 * Revision 1.0.0.1  2000/08/11 23:08:35  stu
 * First Imported.
 *
 * Revision 1.0.0.1  2000/08/11 22:34:22  stu
 * First Imported.
 *
 * Revision 1.2  2000/04/07 01:59:12  stu
 * Ongoing camera in cockpit
 *
 * Revision 1.1.1.1  2000/03/23 22:43:32  stu
 * First Imported.
 *
 * Revision 1.29  1999/11/07 17:33:29  stu
 * *** empty log message ***
 *
 * Revision 1.28  1999/11/07 04:04:16  stu
 * 0.94.3
071199 SB
 *
 * Revision 1.27  1999/10/17 20:42:40  stu
 * 17 Oct 99 SB
 *
 * Revision 1.26  1999/10/16 04:22:53  stu
 * 151099
 *
 * Revision 1.25  1999/10/01 04:11:44  stu
 * 0.94.2 011099 SB
 *
 * Revision 1.24  1999/09/27 14:49:28  stu
 * Fixes NPC's bust on 260999
 *
 * Revision 1.23  1999/09/26 18:34:42  stu
 * 260999
 *
 * Revision 1.22  1999/09/20 19:03:30  stu
 * 0.93.3
 *
 * Revision 1.21  1999/09/19 05:38:53  stu
 * 0.93 as posted
 *
 * Revision 1.20  1999/09/14 15:07:47  stu
 * 14/9/99
 *
 * Revision 1.19  1999/09/12 00:56:13  stu
 * 120999
 *
 * Revision 1.18  1999/08/30 22:23:00  stu
 * 300899
 *
 * Revision 1.17  1999/08/20 09:57:29  stu
 * 20 Aug 99
 *
 * Revision 1.16  1999/08/15 22:17:00  stu
 * Cleanup 150899
 *
 * Revision 1.15  1999/07/26 19:08:43  stu
 * Pre 0.86 test 27 July 99
 *
 * Revision 1.14  1999/07/19 17:42:21  stu
 * 200799
 *
 * Revision 1.13  1999/07/17 18:35:52  stu
 * 180799 - level 4 changes
 *
 * Revision 1.12  1999/07/13 04:07:59  stu
 * Clean up 130799
 *
 * Revision 1.11  1999/07/04 18:37:17  stu
 * Clean up, Sunday 4th july
 *
 * Revision 1.10  1999/07/04 00:21:29  stu
 * Major changes and development.
Colldet changes.
Level 2,3,4
Version 0.86
 *
 * Revision 1.9  1999/06/25 01:14:36  stu
 * Lurkers clewan up
 *
 * Revision 1.8  1999/06/23 00:51:50  stu
 * Changes for new lighting control
 *
 * Revision 1.7  1999/06/22 10:57:31  stu
 * Lurkers clean up
 *
 * Revision 1.6  1999/05/25 04:49:34  stu
 * Missiles not working fix
 *
 * Revision 1.5  1999/05/21 03:31:23  stu
 * 1. New screen driver.
2. VBL Driven music fade and play
 *
 * Revision 1.4  1999/05/12 15:20:12  stu
 * Added $Log to top of file
 *
*/



int find_vacant_dyn_poly_slot();


//loads up animation objects from the_objects
void load_animated_object(anim_pb* the_objects, int number_of_objects, int frame_rate, int control,
     int state, load_dyn_object_pb *the_pb, short galactic_id)
{
extern	  DynObjectsFixedSize *ocb_ptr;
extern _ZPoly_objs *dynamic_polys_ptr;
extern	StatObjectsFixedSize *ocb_Sptr;
extern int	dynamic_object_index;	//how many dynamic objs
int poly_slot=0,size_of_polys;
_ZPoly *new_polys_address=0;
_ZPoly *old_polys_address;
int i,static_slot=-1,dest_ocb;
anim_obj_bits temp_anim_ptrs[6];
register ZObject * current_object_ptr;


if(the_pb->init_check!='chek') report_error ("load_animated_object: Uninitialised parameter block.","\p Static slot =",static_slot);
the_pb->init_check=0;


     dest_ocb=find_vacant_dynamic_object_slot();	//get us a slot to put it in
     if (dest_ocb!=-1)
      {

      for (i=number_of_objects-1;i>-1;i--)
      {
      static_slot=get_static_object_slot((*the_objects).object_id[i]);	//go fetch fido (the object)
      if (static_slot==-1)
	  {
	   report_error_id ("load_objects : load_animated_object couldn't find the following static object:",(*the_objects).object_id[i]);
       return;
	  }
	  else
      {

//now copy polys out of static into vacant dynamic
     	poly_slot=find_vacant_dyn_poly_slot();
	    if	(poly_slot!=-1)
	    {
	     (*dynamic_polys_ptr)[poly_slot].in_use=1;	//prevent a crash - this poly slot is in use!
	//copy the polys across
	     new_polys_address=&(*dynamic_polys_ptr)[poly_slot].the_obj_polys[0];	//so we can poke the new object
	     old_polys_address=(*ocb_Sptr).object_list[static_slot].polys_ptr;
	
	     size_of_polys=(*ocb_Sptr).object_list[static_slot].number_of_polys;
	     size_of_polys=size_of_polys*sizeof(_ZPoly);
	
	     BlockMoveData	(old_polys_address,new_polys_address,size_of_polys);
	//
        temp_anim_ptrs[i].polys_ptr=new_polys_address;
        temp_anim_ptrs[i].polys_slot=poly_slot;
        temp_anim_ptrs[i].points_ptr=(*ocb_Sptr).object_list[static_slot].points_ptr;
        }	//end of load polys
      }	//end of got static
     }	//end of i loop

current_object_ptr=&ocb_ptr->object_list[dest_ocb];

//now load in the object
	(*ocb_ptr).object_list[dest_ocb]=(*ocb_Sptr).object_list[static_slot];	//copy the object

	current_object_ptr->in_use=1;	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.
	current_object_ptr->Oworldx=the_pb->world_x;
	current_object_ptr->Oworldy=the_pb->world_y;
	current_object_ptr->Oworldz=the_pb->world_z;
	current_object_ptr->Ostartx=the_pb->world_x;
	current_object_ptr->Ostarty=the_pb->world_y;
	current_object_ptr->targetted=0;
	current_object_ptr->object_targetted=-1;

	current_object_ptr->Orotdx=the_pb->delta_rot_x;
	current_object_ptr->Orotdy=the_pb->delta_rot_y;
	current_object_ptr->Orotdz=the_pb->delta_rot_z;
	current_object_ptr->Orotx=the_pb->rot_x;
	current_object_ptr->Oroty=the_pb->rot_y;
	current_object_ptr->Orotz=the_pb->rot_z;
    current_object_ptr->invincible_count=0;
	current_object_ptr->is_blobbed=0;	//if 1, we just plot a point
	current_object_ptr->Dyn_OCB_control_data.controller=the_pb->controller_ref;
	current_object_ptr->Dyn_OCB_control_data.no_control_this_frame=0;
	
	current_object_ptr->Dyn_OCB_control_data.control_use_1=the_pb->control_use_1;
	current_object_ptr->Dyn_OCB_control_data.control_use_2=the_pb->control_use_2;
	current_object_ptr->Dyn_OCB_control_data.control_use_3=the_pb->control_use_3;
	current_object_ptr->Dyn_OCB_control_data.control_use_4=the_pb->control_use_4;
	current_object_ptr->Dyn_OCB_control_data.control_use_1_float=0;
	current_object_ptr->Dyn_OCB_control_data.control_use_2_float=0;
	
	current_object_ptr->Dyn_OCB_control_data.control_use_5=0;
	current_object_ptr->Dyn_OCB_control_data.callsign[0]='\0';
	current_object_ptr->n=the_pb->normal;
	current_object_ptr->Dyn_OCB_control_data.shield_value=the_pb->shield_value;
	current_object_ptr->unique_id=0;
	current_object_ptr->bolted_to=0;

//stuff for NPC_Control
	current_object_ptr->Dyn_OCB_control_data.NPC_class=the_pb->NPC_class;
	current_object_ptr->dynamic_tex_ptr=0;
	current_object_ptr->Dyn_OCB_control_data.rank=0;

//
//mothership specifc
	current_object_ptr->Dyn_OCB_control_data.docking_what=-1;	//very important!
	current_object_ptr->Dyn_OCB_control_data.galactic_id=galactic_id;

//next 4 130499
	current_object_ptr->Dyn_OCB_control_data.shield_value=the_pb->shield_value;
    current_object_ptr->Dyn_OCB_control_data.docked_object_slot=-1;

	current_object_ptr->mass=the_pb->mass;
	current_object_ptr->Dyn_OCB_control_data.dq=the_pb->dq;
	current_object_ptr->hit_by_slot=-1;

	current_object_ptr->just_hit=0;
	current_object_ptr->no_trivial_rejection=the_pb->no_trivial_rejection;
	current_object_ptr->Dyn_OCB_control_data.allegiance=the_pb->object_category;

//	current_object_ptr->Dyn_OCB_control_data.laser_category=the_pb->laser_cat;
//	current_object_ptr->Dyn_OCB_control_data.cannon_category=the_pb->cannon_cat;
//	current_object_ptr->Dyn_OCB_control_data.missile_category=the_pb->missile_cat;
//	current_object_ptr->Dyn_OCB_control_data.number_of_missiles=the_pb->number_of_missiles;
	current_object_ptr->parent=the_pb->parent;

	current_object_ptr->polys_ptr=new_polys_address;
	current_object_ptr->active_polys_slot=poly_slot;	//so we can quickly kill these polys
//        current_object_ptr->controller_disabled_count = 0;	   //set in coll_detect on coll_detect as it reverses thurst vector

	current_object_ptr->blobbed_colour_red=1.0;	//defaulted to white when blobbed
	current_object_ptr->blobbed_colour_green=1.0;
	current_object_ptr->blobbed_colour_blue=1.0;

	current_object_ptr->anim_frame_rate=frame_rate;
	current_object_ptr->anim_control=control;
	current_object_ptr->anim_state=state;

	current_object_ptr->anim_counter=0;
	current_object_ptr->anim_frame=0;
	current_object_ptr->anim_max_object=number_of_objects;
	current_object_ptr->the_anim_ptrs[0]=temp_anim_ptrs[0];	//store the new ptrs in the anim block
	current_object_ptr->the_anim_ptrs[1]=temp_anim_ptrs[1];	//store the new ptrs in the anim block
	current_object_ptr->the_anim_ptrs[2]=temp_anim_ptrs[2];	//store the new ptrs in the anim block
	current_object_ptr->the_anim_ptrs[3]=temp_anim_ptrs[3];	//store the new ptrs in the anim block
	current_object_ptr->the_anim_ptrs[4]=temp_anim_ptrs[4];	//store the new ptrs in the anim block
	current_object_ptr->the_anim_ptrs[5]=temp_anim_ptrs[5];	//store the new ptrs in the anim block


	dynamic_object_index++;	//we've added an object that needs to be processed
	
	// call the forth controller new object controller for this object - just in case
	// it has a forth controller. Forth routines handle bounds checking
        coffee_object_created(dest_ocb);
	
	}	


}


//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//this is called to load an object into the dynamic arrays
//modified 020801 SB
//if dest_obj_ptr->Dyn_OCB_control_data->was_loaded_from_sds==1
//then we don't load mass from pb

int load_dyn_object(int static_slot,int dest_ocb,load_dyn_object_pb *the_pb, 
                    int mothership_galactic_slot, float scale, int parent_slot,
                    int is_dust)
{
extern	StatObjectsFixedSize *ocb_Sptr;
extern	DynObjectsFixedSize *ocb_ptr;
extern _ZPoly_objs *dynamic_polys_ptr;
extern int	dynamic_object_index;	//how many static objects
int poly_slot,size_of_polys;
_ZPoly *new_polys_address;
_ZPoly *old_polys_address;
register ZObject * current_object_ptr;
extern scaled_points_array_type *scaled_points_ptr;

//for dust bit in polys
_ZPoly *the_polys;
unsigned char or_bit;
int how_many_polys,poly_count;
int n;
char debug_oid[5];

if(the_pb->init_check!='chek') report_error ("load_dyn_object: Uninitialised parameter block.","\p Static slot =",static_slot);

if ((static_slot>=42) && (static_slot<=46))
{
              fprintf (stderr, "load_dyn_object: Non human mothership part loaded\n"); 
}

if(scale<1)  report_error ("load_dyn_object: Scale less than 1.","\p Static slot =",static_slot);

the_pb->init_check=0;

//check if mass=0 and error if so
if (the_pb->mass<=0) report_error ("load_dyn_object: Mass=0 or -ve! Static slot =","\p",static_slot);
if (dest_ocb!=-1)
    {
        current_object_ptr=&ocb_ptr->object_list[dest_ocb];
        if (the_pb->parent!=-1)
        {
            set_has_children(the_pb->parent); //tell parent it has had a child!
        }

     if (current_object_ptr->in_use!=0)  report_error ("load_dyn_object: Dynamic Object already in use. Object must be dead.","\p Static slot =",static_slot);
	(*ocb_ptr).object_list[dest_ocb]=(*ocb_Sptr).object_list[static_slot];	//copy the object
//debug
        #if DYNLOAD_LOG==1
            debug_oid[0]=(current_object_ptr->Oid>>24);
            debug_oid[1]=(current_object_ptr->Oid>>16);
            debug_oid[2]=(current_object_ptr->Oid>>8);
            debug_oid[3]=(current_object_ptr->Oid);
            debug_oid[4]=0;
            
            fprintf (stderr, "load_dyn_object: StatObj: %i (%s). DynObj: %i Pos (metres)= x:%.0f, y:%.0f, z:%.0f\n",static_slot, 
            debug_oid, dest_ocb, the_pb->world_x*ZEX_UNIT_SIZE_IN_METERS, the_pb->world_y*ZEX_UNIT_SIZE_IN_METERS, 
            the_pb->world_z*ZEX_UNIT_SIZE_IN_METERS);

            if (the_pb->world_x==0.0 && the_pb->world_y==0.0 && the_pb->world_z==0.0)
            {
              fprintf (stderr, "load_dyn_object: WARNING: StatObj: %i (%s). DynObj: %i Is loaded at 0,0,0\n",static_slot,debug_oid, dest_ocb); 
              if (current_object_ptr->parent!=-1)
              {
              fprintf (stderr, "Parent is %i\n",current_object_ptr->parent); 
              }
              else
              {
              fprintf (stderr, "NO Parent\n"); 
              }
              
            }

        #endif
        
//end debug

	//fill in callsign
	current_object_ptr->in_use=1;	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.
	current_object_ptr->targetted=0;
	current_object_ptr->object_targetted=-1;
	current_object_ptr->scale=scale;
	current_object_ptr->showing_shield=0;
	
	current_object_ptr->Oworldx=the_pb->world_x;
	current_object_ptr->Oworldy=the_pb->world_y;
	current_object_ptr->Oworldz=the_pb->world_z;
	current_object_ptr->Ostartx=the_pb->world_x;
	current_object_ptr->Ostarty=the_pb->world_y;

	current_object_ptr->Orotdx=the_pb->delta_rot_x;
	current_object_ptr->Orotdy=the_pb->delta_rot_y;
	current_object_ptr->Orotdz=the_pb->delta_rot_z;
	current_object_ptr->Orotx=the_pb->rot_x;
	current_object_ptr->Oroty=the_pb->rot_y;
	current_object_ptr->Orotz=the_pb->rot_z;
	current_object_ptr->Dyn_OCB_control_data.controller=the_pb->controller_ref;
	current_object_ptr->Dyn_OCB_control_data.no_control_this_frame=0;

	current_object_ptr->is_blobbed=0;	//if 1, we just plot a point
	current_object_ptr->Dyn_OCB_control_data.control_use_1=the_pb->control_use_1;
	current_object_ptr->Dyn_OCB_control_data.control_use_2=the_pb->control_use_2;
	current_object_ptr->Dyn_OCB_control_data.control_use_3=the_pb->control_use_3;
	current_object_ptr->Dyn_OCB_control_data.control_use_4=the_pb->control_use_4;
	current_object_ptr->Dyn_OCB_control_data.control_use_5=the_pb->control_use_5;
	
	current_object_ptr->Dyn_OCB_control_data.control_use_1_float=the_pb->control_use_1_float;
	current_object_ptr->Dyn_OCB_control_data.control_use_2_float=0;
	current_object_ptr->Dyn_OCB_control_data.control_use_3_float=0;
	current_object_ptr->Dyn_OCB_control_data.control_use_4_float=0;
	current_object_ptr->Dyn_OCB_control_data.laser_trigger_slot=-1;
	current_object_ptr->Dyn_OCB_control_data.cannon_trigger_slot=-1;
	current_object_ptr->Dyn_OCB_control_data.exhaust_trigger_slot=-1;
        
	current_object_ptr->Dyn_OCB_control_data.behaviour=NULL_BEHAVIOUR;
	
	//init he cargo bay
	for (n=0;n<MAX_CARGO_BAY_SIZE;n++)
	{
	 current_object_ptr->Dyn_OCB_control_data.cargo_bay [n]=-1;
    }

//stuff for NPC_Control
	current_object_ptr->Dyn_OCB_control_data.NPC_class=the_pb->NPC_class;
	current_object_ptr->dynamic_tex_ptr=0;
	current_object_ptr->Dyn_OCB_control_data.rank=0;

	
//mothership specifc
	current_object_ptr->Dyn_OCB_control_data.galactic_id=mothership_galactic_slot;
	current_object_ptr->Dyn_OCB_control_data.docking_what=-1;	//very important!

//init sds specific data
	current_object_ptr->Dyn_OCB_control_data.command_mthr1=0;	//very important!
	current_object_ptr->Dyn_OCB_control_data.command_mthr2=0;	//very important!

	current_object_ptr->Dyn_OCB_control_data.command_mrev1=0;	//very important!
	current_object_ptr->Dyn_OCB_control_data.command_mrev2=0;	//very important!

	current_object_ptr->Dyn_OCB_control_data.command_vthr1=0;	//very important!
	current_object_ptr->Dyn_OCB_control_data.command_vthr2=0;	//very important!

	current_object_ptr->Dyn_OCB_control_data.command_rvth1=0;	//very important!
	current_object_ptr->Dyn_OCB_control_data.command_rvth2=0;	//very important!

	current_object_ptr->Dyn_OCB_control_data.command_pitch=0;	//very important!
	current_object_ptr->Dyn_OCB_control_data.command_roll=0;	//very important!
	current_object_ptr->Dyn_OCB_control_data.command_yaw=0;	//very important!

	current_object_ptr->Dyn_OCB_control_data.command_fired_flags=0;	//very important!

//fuel
//IS loaded from the SDS so don't mess with it here (Stu you tosser!)
//ORCS
	current_object_ptr->Dyn_OCB_control_data.ORCS_thrust_up=0;	//very important!
	current_object_ptr->Dyn_OCB_control_data.ORCS_thrust_down=0;	//very important!

	current_object_ptr->Dyn_OCB_control_data.yaw_channel=0;	
	current_object_ptr->Dyn_OCB_control_data.pitch_channel=0;	
	current_object_ptr->Dyn_OCB_control_data.roll_channel=0;
		
	current_object_ptr->Dyn_OCB_control_data.command_autopilot_engaged=AP_OFF;
	current_object_ptr->Dyn_OCB_control_data.command_ORCS_damper=ORCS_DAMPER_ROT +ORCS_DAMPER_LIN;	   //both dampers on
	
//AP Data
	    current_object_ptr->Dyn_OCB_control_data.AP_flags=0;
        current_object_ptr->Dyn_OCB_control_data.current_relative_velocity=0;
        current_object_ptr->Dyn_OCB_control_data.AP_pull_up_timer=0;
        current_object_ptr->Dyn_OCB_control_data.AP_roll_lock_out=0;

//Electrics
	    current_object_ptr->Dyn_OCB_control_data.DC_power_volts=0;
	    current_object_ptr->Dyn_OCB_control_data.AC_power_volts=0;
	    current_object_ptr->Dyn_OCB_control_data.DC_power_amps=0;
	    current_object_ptr->Dyn_OCB_control_data.AC_power_amps=0;
	    current_object_ptr->Dyn_OCB_control_data.power_flags=0;
//engines data
	    current_object_ptr->Dyn_OCB_control_data.actual_main_thrust=0;
	    current_object_ptr->Dyn_OCB_control_data.eclipse_engine_sound_ref=-1;
	    current_object_ptr->Dyn_OCB_control_data.current_total_fuel_consumption=0;
	    current_object_ptr->Dyn_OCB_control_data.eng_temperature_in_C=20;

	    current_object_ptr->Dyn_OCB_control_data.hull_temperature_in_C=-76;
	    current_object_ptr->Dyn_OCB_control_data.laser_bay_temperature_in_C=1000;

//	current_object_ptr->swerve_direction=0;
	current_object_ptr->invincible_count=0;


//	current_object_ptr->attack_mode=the_pb->attack_mode;

	QuatSetValue(&current_object_ptr->Qor,0,0,1,0);	//pointing into z
        //set_rotation_around_world_y(dest_ocb, 180);
        
	current_object_ptr->torque_sum.x=0;
	current_object_ptr->torque_sum.y=0;
	current_object_ptr->torque_sum.z=0;

	current_object_ptr->abs_velocity_vector.x=0;
	current_object_ptr->abs_velocity_vector.y=0;
	current_object_ptr->abs_velocity_vector.z=0;


//	(*ocb_ptr).object_list[dest_ocb].object_normal=the_pb->normal;
	current_object_ptr->n.x=0;
	current_object_ptr->n.y=0;
	current_object_ptr->n.z=1;

	current_object_ptr->v.x=0;	//vertical vector
	current_object_ptr->v.y=1;
	current_object_ptr->v.z=0;

	current_object_ptr->s.x=1;	//side vector
	current_object_ptr->s.y=0;
	current_object_ptr->s.z=0;



	current_object_ptr->Dyn_OCB_control_data.shield_value=the_pb->shield_value;
//next 4  130499
    current_object_ptr->Dyn_OCB_control_data.docked_object_slot=-1;	//if -1, not docked
	  
 
      if (current_object_ptr->Dyn_OCB_control_data.was_loaded_from_sds==0)
	current_object_ptr->mass=the_pb->mass;
	
	
	current_object_ptr->Dyn_OCB_control_data.dq=the_pb->dq;

	current_object_ptr->parent=the_pb->parent;
	current_object_ptr->hit_by_slot=-1;

	current_object_ptr->just_hit=0;
	current_object_ptr->no_trivial_rejection=the_pb->no_trivial_rejection;
	current_object_ptr->Dyn_OCB_control_data.allegiance=the_pb->object_category;
//	current_object_ptr->Dyn_OCB_control_data.laser_category=the_pb->laser_cat;
//	current_object_ptr->Dyn_OCB_control_data.cannon_category=the_pb->cannon_cat;
//	current_object_ptr->Dyn_OCB_control_data.missile_category=the_pb->missile_cat;
//	current_object_ptr->Dyn_OCB_control_data.number_of_missiles=the_pb->number_of_missiles;
	current_object_ptr->unique_id=the_pb->unique_id;
	current_object_ptr->bolted_to=0;

	current_object_ptr->anim_frame_rate=0;
	current_object_ptr->anim_counter=0;
	current_object_ptr->anim_frame=0;
	current_object_ptr->anim_max_object=1;	   //not animated
	current_object_ptr->blobbed_colour_red=1.0;	//defaulted to white when blobbed
	current_object_ptr->blobbed_colour_green=1.0;
	current_object_ptr->blobbed_colour_blue=1.0;
	
	current_object_ptr->kill_flags=the_pb->kill_flags;	//up to caller to set this to 1 if it knows the controller supports control on kill
	current_object_ptr->eng_invisible=the_pb->invisible_flag;	//up to caller to set this to 1 if it knows the controller supports control on kill

	gen_callsign(dest_ocb);

//        (*ocb_ptr).object_list[dest_ocb].controller_disabled_count = 0;	//set in coll_detect on coll_detect as it reverses thurst vector

//now copy polys out of static into vacant dynamic
	poly_slot=find_vacant_dyn_poly_slot();
	if	(poly_slot!=-1)
	{
	(*dynamic_polys_ptr)[poly_slot].in_use=1;	//prevent a crash - this poly slot is in use!
	//copy the polys across
	new_polys_address=&(*dynamic_polys_ptr)[poly_slot].the_obj_polys[0];	//so we can poke the new object
	old_polys_address=(*ocb_ptr).object_list[dest_ocb].polys_ptr;
	
	size_of_polys=(*ocb_Sptr).object_list[static_slot].number_of_polys;
	size_of_polys=size_of_polys*sizeof(_ZPoly);
	
	BlockMoveData	(old_polys_address,new_polys_address,size_of_polys);
	//poke the address of the polys into the new object
	(*ocb_ptr).object_list[dest_ocb].polys_ptr=new_polys_address;
	(*ocb_ptr).object_list[dest_ocb].active_polys_slot=poly_slot;	//so we can quickly kill these polys
	
	

	} else { report_error ("load_dyn_object: Couldn't find vacant poly slot =","\p",(int)dynamic_polys_ptr); }

//change 311200 - if dust set DUST_BIT in each poly so when we draw we just plot a point
        if(is_dust) 
        {
         or_bit=DUST_BIT;
        }
        else or_bit=0;
        
          how_many_polys=current_object_ptr->number_of_polys;
          the_polys=current_object_ptr->polys_ptr;

          for (poly_count=0;poly_count<how_many_polys;poly_count++)
          {
		(*the_polys).polyflags|=or_bit;
		(*the_polys).owner=dest_ocb;	//so we can back track from a poly to an object
		the_polys++;	//next poly please

          }
//change as of version 0.53
//if scale!=1 then we need to copy and scale the points and update ocb[n].points_ptr to point to the new points
//the maximum number of points we can store in an object is 500
//and the maximum number of caled objects is 30. This is defined in scaled_object_points.
//when an object is killed we need to check the scaled flag and kill the scaled points object
//if necessary.
	 (*ocb_ptr).object_list[dest_ocb].scaled_points_entry=-1;
	 
	 if (scale!=1)
	 {
	 int slot,number_of_points,n;
	 _3D the_point;
	 _3D *source_point_ptr,*dest_point_ptr;
	 //scale radius
	 (*ocb_ptr).object_list[dest_ocb].radius*=scale;
	 //scale bounding box
	 (*ocb_ptr).object_list[dest_ocb].box_min.x*=scale;
	 (*ocb_ptr).object_list[dest_ocb].box_min.y*=scale;
	 (*ocb_ptr).object_list[dest_ocb].box_min.z*=scale;
	 (*ocb_ptr).object_list[dest_ocb].box_max.x*=scale;
	 (*ocb_ptr).object_list[dest_ocb].box_max.y*=scale;
	 (*ocb_ptr).object_list[dest_ocb].box_max.z*=scale;
	 
	 //scale points and redirect source points pointer
	  slot=get_scaled_points_slot();
	  if (slot==-1) report_error ("load_dyn_object: Couldn't find empty scaled object points slot","\p",static_slot);
	  (*ocb_ptr).object_list[dest_ocb].scaled_points_entry=slot;	//so we can kill em
	  number_of_points=(*ocb_ptr).object_list[dest_ocb].number_of_points;

	  if(number_of_points>NUMBER_OF_SCALED_POINTS)
	     report_error ("load_dyn_object: Too many points in object to scale","\p",static_slot);
	  //now copy the points and scale em
          source_point_ptr=(*ocb_ptr).object_list[dest_ocb].points_ptr;
          dest_point_ptr=scaled_points_ptr->scaled_points[slot].scaled_points_start;	//_start[slot].scaled_points;
	  for(n=0;n<number_of_points;n++)
	  {
	   the_point=*source_point_ptr;
	   the_point.x*=scale;
	   the_point.y*=scale;
	   the_point.z*=scale;
	   *dest_point_ptr=the_point;
	   source_point_ptr++;
	   dest_point_ptr++;
	   
	  }
	 }

	if(parent_slot != -1)		// -1 means with reference to nothing, otherwise orbit parent
	{
	  // lets set up the (hopefully) stable orbit velocity
          set_stable_orbit_vector(dest_ocb,parent_slot);
	}

	dynamic_object_index++;	//we've added an object that needs to be processed

	// call the forth controller new object controller for this object - just in case
	// it has a forth controller. Forth routines handle bounds checking
   //     coffee_object_created(dest_ocb);
        return 0;
	} // end of if(dest_ocb!=-1)
	
report_error ("load_dyn_object: Destination slot not empty!.","\p Static slot =",static_slot);
return -1;
} // end of load_dyn_object











//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//this is called to replace an object into the dynamic arrays
//dont use on motherships!!!!!!!!
//SB 180799
//Hmm, why can't I use it on motherships? I need to...
//SB 060400
//Oh, I get it because of the memory pointer
//we'll have another routine then...

void replace_dyn_object(int static_slot,int dest_ocb)
{
extern	StatObjectsFixedSize *ocb_Sptr;
extern	DynObjectsFixedSize *ocb_ptr;
extern _ZPoly_objs *dynamic_polys_ptr;
extern int	dynamic_object_index;	//how many static objects
int poly_slot,size_of_polys;
_ZPoly *new_polys_address;
_ZPoly *old_polys_address;
load_dyn_object_pb the_pb;

if (dest_ocb!=-1)
	{

//copy old attribs
	the_pb.world_x=(*ocb_ptr).object_list[dest_ocb].Oworldx;
	the_pb.world_y=(*ocb_ptr).object_list[dest_ocb].Oworldy;
	the_pb.world_z=(*ocb_ptr).object_list[dest_ocb].Oworldz;
//	the_pb.delta_rot_x=(*ocb_ptr).object_list[dest_ocb].Orotdx;
//	the_pb.delta_rot_y=(*ocb_ptr).object_list[dest_ocb].Orotdy;
//	the_pb.delta_rot_z=(*ocb_ptr).object_list[dest_ocb].Orotdz;
	the_pb.rot_x=(*ocb_ptr).object_list[dest_ocb].Orotx;
	the_pb.rot_y=(*ocb_ptr).object_list[dest_ocb].Oroty;
	the_pb.rot_z=(*ocb_ptr).object_list[dest_ocb].Orotz;
	the_pb.controller_ref=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.controller;
	the_pb.control_use_1=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_1;
	the_pb.control_use_2=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_2;
	the_pb.control_use_3=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_3;
	the_pb.control_use_4=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_4;
	the_pb.control_use_5=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_5;
//	the_pb.attack_mode=(*ocb_ptr).object_list[dest_ocb].attack_mode;

	the_pb.shield_value=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.shield_value;
//next 4  130499
//	the_pb.shield_value=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.original_shield_value;
//	the_pb.has_roll=(*ocb_ptr).object_list[dest_ocb].has_roll;
//	the_pb.rot_z=(*ocb_ptr).object_list[dest_ocb].last_heading;
	  
	the_pb.mass=(*ocb_ptr).object_list[dest_ocb].mass;
	the_pb.dq=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.dq;
//	the_pb.Omovedx=(*ocb_ptr).object_list[dest_ocb].extern_movedx;
//	the_pb.Omovedy=(*ocb_ptr).object_list[dest_ocb].extern_movedy;

	the_pb.parent=(*ocb_ptr).object_list[dest_ocb].parent;

	the_pb.no_trivial_rejection=(*ocb_ptr).object_list[dest_ocb].no_trivial_rejection;
	the_pb.object_category=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.allegiance;
//	the_pb.laser_cat=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.laser_category;
//	the_pb.cannon_cat=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.cannon_category;
//	the_pb.missile_cat=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.missile_category;
//	the_pb.number_of_missiles=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.number_of_missiles;
	the_pb.unique_id=(*ocb_ptr).object_list[dest_ocb].unique_id;
	
	i_know_what_im_doing_kill(dest_ocb,TRUE); //DO NOT KILL THE TEXTURES
//insert new blank object
	(*ocb_ptr).object_list[dest_ocb]=(*ocb_Sptr).object_list[static_slot];	//copy the object
	(*ocb_ptr).object_list[dest_ocb].in_use=1;	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.

	(*ocb_ptr).object_list[dest_ocb].anim_frame_rate=0;
	(*ocb_ptr).object_list[dest_ocb].anim_counter=0;
	(*ocb_ptr).object_list[dest_ocb].anim_frame=0;
	(*ocb_ptr).object_list[dest_ocb].anim_max_object=1;	//not animated
	(*ocb_ptr).object_list[dest_ocb].hit_by_slot=-1;
	(*ocb_ptr).object_list[dest_ocb].just_hit=0;
//	(*ocb_ptr).object_list[dest_ocb].extern_rotdz=0;
//	(*ocb_ptr).object_list[dest_ocb].wanted_roll=0;	//calc'd from current and last
//    (*ocb_ptr).object_list[dest_ocb].tractor=0;
    (*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.docked_object_slot=-1;	//if -1, not docked
	(*ocb_ptr).object_list[dest_ocb].n.x=1;
	(*ocb_ptr).object_list[dest_ocb].n.y=0;
	(*ocb_ptr).object_list[dest_ocb].n.z=0;
	
//	(*ocb_ptr).object_list[dest_ocb].u.x=0;
//	(*ocb_ptr).object_list[dest_ocb].u.y=1;
//	(*ocb_ptr).object_list[dest_ocb].u.z=0;	//y axis normal
	
//	(*ocb_ptr).object_list[dest_ocb].v.x=0;
//	(*ocb_ptr).object_list[dest_ocb].v.y=0;
//	(*ocb_ptr).object_list[dest_ocb].v.z=-1;	//z axis normal


	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_1_float=0;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_2_float=0;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_3_float=0;
//	(*ocb_ptr).object_list[dest_ocb].swerve_direction=0;
	(*ocb_ptr).object_list[dest_ocb].invincible_count=0;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.callsign[0]='\0';
//mothership specific - don't use this routine to replace a mothership!
//	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.mship_ptr=0;



//set new attribs
	(*ocb_ptr).object_list[dest_ocb].Oworldx=the_pb.world_x;
	(*ocb_ptr).object_list[dest_ocb].Oworldy=the_pb.world_y;
	(*ocb_ptr).object_list[dest_ocb].Oworldz=the_pb.world_z;
//	(*ocb_ptr).object_list[dest_ocb].Orotdx=the_pb.delta_rot_x;
//	(*ocb_ptr).object_list[dest_ocb].Orotdy=the_pb.delta_rot_y;
//	(*ocb_ptr).object_list[dest_ocb].Orotdz=the_pb.delta_rot_z;
	(*ocb_ptr).object_list[dest_ocb].Orotx=the_pb.rot_x;
	(*ocb_ptr).object_list[dest_ocb].Oroty=the_pb.rot_y;
	(*ocb_ptr).object_list[dest_ocb].Orotz=the_pb.rot_z;
//	(*ocb_ptr).object_list[dest_ocb].Omovedx=the_pb.Omovedx;
//	(*ocb_ptr).object_list[dest_ocb].Omovedy=the_pb.Omovedy;
//	(*ocb_ptr).object_list[dest_ocb].Omovedz=the_pb.Omovedz;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.controller=the_pb.controller_ref;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_1=the_pb.control_use_1;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_2=the_pb.control_use_2;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_3=the_pb.control_use_3;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_4=the_pb.control_use_4;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_5=the_pb.control_use_5;
//	(*ocb_ptr).object_list[dest_ocb].swerve_direction=0;
	(*ocb_ptr).object_list[dest_ocb].invincible_count=0;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.callsign[0]='\0';
//	(*ocb_ptr).object_list[dest_ocb].attack_mode=the_pb.attack_mode;

//	(*ocb_ptr).object_list[dest_ocb].object_normal=the_pb->normal;
//	(*ocb_ptr).object_list[dest_ocb].n.x=1;
//	(*ocb_ptr).object_list[dest_ocb].n.y=0;
//	(*ocb_ptr).object_list[dest_ocb].n.z=0;

	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.shield_value=the_pb.shield_value;
//next 4  130499
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.shield_value=the_pb.shield_value;
//    (*ocb_ptr).object_list[dest_ocb].tractor=0;
    (*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.docked_object_slot=-1;	//if -1, not docked
	  
	(*ocb_ptr).object_list[dest_ocb].mass=the_pb.mass;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.dq=the_pb.dq;

	(*ocb_ptr).object_list[dest_ocb].parent=the_pb.parent;
	(*ocb_ptr).object_list[dest_ocb].hit_by_slot=-1;

	(*ocb_ptr).object_list[dest_ocb].just_hit=0;
	(*ocb_ptr).object_list[dest_ocb].no_trivial_rejection=the_pb.no_trivial_rejection;
	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.allegiance=the_pb.object_category;
//	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.laser_category=the_pb.laser_cat;
//	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.cannon_category=the_pb.cannon_cat;
//	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.missile_category=the_pb.missile_cat;
//	(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.number_of_missiles=the_pb.number_of_missiles;
	(*ocb_ptr).object_list[dest_ocb].unique_id=the_pb.unique_id;

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
	old_polys_address=(*ocb_ptr).object_list[dest_ocb].polys_ptr;
	
	size_of_polys=(*ocb_Sptr).object_list[static_slot].number_of_polys;
	size_of_polys=size_of_polys*sizeof(_ZPoly);
	
	BlockMoveData	(old_polys_address,new_polys_address,size_of_polys);
	//poke the address of the polys into the new object
	(*ocb_ptr).object_list[dest_ocb].polys_ptr=new_polys_address;
	(*ocb_ptr).object_list[dest_ocb].active_polys_slot=poly_slot;	//so we can quickly kill these polys
	
	}	
	dynamic_object_index++;	//we've added an object that needs to be processed
	}
}




void swap_dyn_object(int dest_ocb, unsigned long ID)
{
extern	DynObjectsFixedSize *ocb_ptr;
load_dyn_object_pb the_pb;
int static_slot;
float scale;
unsigned char old_name[128];
float blobbed_green_save,blobbed_blue_save,blobbed_red_save;
dynamic_texture* tex_ptr_save;
_ZPoly *current_poly;
register ZObject * current_object_ptr;
int number_of_polys_in_object,i;
unsigned long ogl_name_save;
int targetted_save;
int galactic_id;
_quat Qor_saved;


vector torque_sum_saved;

//first check if the object we want to swap out is the same as the one we want to put in...

if((*ocb_ptr).object_list[dest_ocb].Oid==ID) return; //yes. do nothing
//return;

//get static slot of object to load
static_slot=get_static_object_slot(ID);

#if PB_PRINT_TO_LOG_ENABLED==1 && LOD_LOG==1 && ZEX_PROJECT_BUILDER==1
    fprintf(stderr, "ENG: swap_dyn_object : DynObject %i changed to %i\n",dest_ocb,static_slot);
#endif

if (static_slot==-1)
{
 report_error ("swap_dyn_object: Couldn't static slot for ID!","\p",777);
}

//save old texture name
            current_poly=(*ocb_ptr).object_list[dest_ocb].polys_ptr;
            ogl_name_save= (*current_poly).Pid;

//copy old name
pascal_copystr(old_name,(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.name_str);

tex_ptr_save=(*ocb_ptr).object_list[dest_ocb].dynamic_tex_ptr;

torque_sum_saved=(*ocb_ptr).object_list[dest_ocb].torque_sum;
Qor_saved=(*ocb_ptr).object_list[dest_ocb].Qor;

blobbed_red_save=(*ocb_ptr).object_list[dest_ocb].blobbed_colour_red;
blobbed_green_save=(*ocb_ptr).object_list[dest_ocb].blobbed_colour_green;
blobbed_blue_save=(*ocb_ptr).object_list[dest_ocb].blobbed_colour_blue;
targetted_save=(*ocb_ptr).object_list[dest_ocb].targetted;

//copy old attribs
clear_pb(&the_pb);	//saves loads of zero code

    scale=(*ocb_ptr).object_list[dest_ocb].scale;
	the_pb.world_x=(*ocb_ptr).object_list[dest_ocb].Oworldx;
	the_pb.world_y=(*ocb_ptr).object_list[dest_ocb].Oworldy;
	the_pb.world_z=(*ocb_ptr).object_list[dest_ocb].Oworldz;
	the_pb.delta_rot_x=(*ocb_ptr).object_list[dest_ocb].Orotdx;
	the_pb.delta_rot_y=(*ocb_ptr).object_list[dest_ocb].Orotdy;
	the_pb.delta_rot_z=(*ocb_ptr).object_list[dest_ocb].Orotdz;
	the_pb.rot_x=(*ocb_ptr).object_list[dest_ocb].Orotx;
	the_pb.rot_y=(*ocb_ptr).object_list[dest_ocb].Oroty;
	the_pb.rot_z=(*ocb_ptr).object_list[dest_ocb].Orotz;
	the_pb.controller_ref=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.controller;
	the_pb.control_use_1=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_1;
	the_pb.control_use_2=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_2;
	the_pb.control_use_3=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_3;
	the_pb.control_use_4=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_4;
	the_pb.control_use_5=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.control_use_5;
//	the_pb.attack_mode=(*ocb_ptr).object_list[dest_ocb].attack_mode;

	the_pb.shield_value=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.shield_value;
//next 4  130499
//	the_pb.shield_value=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.original_shield_value;
//	the_pb.has_roll=(*ocb_ptr).object_list[dest_ocb].has_roll;
//	the_pb.rot_z=(*ocb_ptr).object_list[dest_ocb].last_heading;
	  
	the_pb.mass=(*ocb_ptr).object_list[dest_ocb].mass;
	the_pb.dq=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.dq;
//	the_pb.Omovedx=(*ocb_ptr).object_list[dest_ocb].extern_movedx;
//	the_pb.Omovedy=(*ocb_ptr).object_list[dest_ocb].extern_movedy;

	the_pb.parent=(*ocb_ptr).object_list[dest_ocb].parent;

	the_pb.no_trivial_rejection=(*ocb_ptr).object_list[dest_ocb].no_trivial_rejection;
	the_pb.object_category=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.allegiance;
//	the_pb.laser_cat=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.laser_category;
//	the_pb.cannon_cat=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.cannon_category;
//	the_pb.missile_cat=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.missile_category;
//	the_pb.number_of_missiles=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.number_of_missiles;
	the_pb.unique_id=(*ocb_ptr).object_list[dest_ocb].unique_id;

	galactic_id=(*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.galactic_id;
        
	i_know_what_im_doing_kill(dest_ocb, FALSE); //DO NOT KILL HTE TEXTURES
//now load the new object into the same slot
	load_dyn_object(static_slot,dest_ocb,&the_pb,galactic_id,scale,-1,!DUST_BIT);	//the object, the position (-1=next free)

//copy old name
 pascal_copystr((*ocb_ptr).object_list[dest_ocb].Dyn_OCB_control_data.name_str,old_name); //to,from


 (*ocb_ptr).object_list[dest_ocb].dynamic_tex_ptr=tex_ptr_save;


 (*ocb_ptr).object_list[dest_ocb].targetted=targetted_save;
//And the old rotational info
  (*ocb_ptr).object_list[dest_ocb].torque_sum=torque_sum_saved;
  (*ocb_ptr).object_list[dest_ocb].Qor=Qor_saved;
  
//    set_blobbed_object_colour(dest_ocb,blobbed_red_save,blobbed_green_save,blobbed_blue_save);			
	 (*ocb_ptr).object_list[dest_ocb].blobbed_colour_red=blobbed_red_save;
	 (*ocb_ptr).object_list[dest_ocb].blobbed_colour_green=blobbed_green_save;
	 (*ocb_ptr).object_list[dest_ocb].blobbed_colour_blue=blobbed_blue_save;

	current_object_ptr=&ocb_ptr->object_list[dest_ocb];
//set up texture
//get mem ofr otexture

            //poke the texture into the object - we assume all polys are covered (they are)
  number_of_polys_in_object=current_object_ptr->number_of_polys;
  current_poly=current_object_ptr->polys_ptr;
  for (i=0;i<number_of_polys_in_object;i++)
  {
      (*current_poly).Pid=ogl_name_save;
      (*current_poly).draw_code=0;	//not transparent
       current_poly++;
  }
}


int find_vacant_dyn_poly_slot()	//returns an index into the dynamic_polys_array
{
extern _ZPoly_objs *dynamic_polys_ptr;
int i;

for (i=0;i<MAX_NUMBER_OF_DYN_POLY_SLOTS;i++)
	if ((*dynamic_polys_ptr)[i].in_use==0) return i;
return -1;
}

int find_vacant_dynamic_object_slot()	//returns an index into the dynamic_objects_array
{
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects
int i;

for (i=1;i<MAX_NUMBER_OF_DYN_OBJECTS;i++)
	if ((*ocb_ptr).object_list[i].in_use==0) return i;

add_to_text_display("Zex Exec> WARNING: find_vacant_dynamic_object_slot failed!",5);	
return -1;
}





int get_scaled_points_slot()	//returns an index into the dynamic_objects_array
{
int n;
extern scaled_points_array_type *scaled_points_ptr;

for (n=0;n<NUMBER_OF_SCALED_OBJECTS;n++)
 {
   if(scaled_points_ptr->scaled_points[n].in_use==0)
   {
    scaled_points_ptr->scaled_points[n].in_use=1;	//oops!
    return n;
   }
 }
 return -1;
}






void reset_objects()
{
extern int connection_list_index, points_list_index, world_points_list_index;
extern int dynamic_object_index, poly_list_index;
//extern _3D light_normal;

connection_list_index=points_list_index=world_points_list_index=0;
dynamic_object_index=poly_list_index=0;
clear_out_Fdyn();    //reset all fast objects
clear_out_dyn();	//reset all objects to not in use
init_Pdyns();
}


const unsigned char* object_loader_filename;

int load_object(const unsigned char *filename, int static_slot, unsigned long id,int colldetect, unsigned long texture_id, int translucent)
{
FSSpec the_file;
Handle the_file_data_H;
int Err;
unsigned char object_directory[]="\pO";
//int get_file_err;
//extern FSSpec Zex_FSSpec;
//FSSpec objects_folder_FSSpec;
//FSSpec the_object_FSSpec;

//CInfoPBRec pb;
//DirInfo *dpb=(DirInfo*) &pb;
//HFileInfo *fpb=(HFileInfo*) &pb;	//ptrs to the union (Finfo and DirInfo) held in CInfoPBRec
//int folder_DirID,return_value;
//Str255 the_file_name;


object_loader_filename=filename;	//global for error reporting


     if(make_data_fsspec(object_directory,filename,&the_file)) return  -2;

//now load it
	the_file_data_H=read_file(the_file);

	Err=MeshToZexDyn(*the_file_data_H,static_slot,id,colldetect,texture_id, translucent);	//stat slots 0
	DisposeHandle(the_file_data_H);
	
	if (Err) 
	{
	 report_error("load_object(): Error importing object",filename,Err);
	}
	
return Err;
}

