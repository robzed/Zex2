/*
 *  fast_dyn.c
 *  zex22
 *
 *  Created by Stuart Ball on Sat Feb 09 2002.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <files.h>
  #endif
  #include "3d_structures_def.h"
  #include "Error_handler.h"
  #include "zex_misc.h"
#endif //pch

#include "file_io.h"
#include "MeshToZex.h"
#include "load_objects.h"
#include "objects_controls.h"
#include "z_init.h"
#include "pictures.h"
#include "texture_management.h"
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

module_private void FZkill(int the_object, int kill_ogl_textures);


int load_Fdyn_object(int static_slot,int dest_focb, load_dyn_object_pb *the_pb, float scale, int parent_slot)
{
extern	StatObjectsFixedSize *ocb_Sptr;
extern	DynFObjectsFixedSize *Focb_ptr;
extern _ZPoly_objs *dynamic_polys_ptr;
extern int	Fdynamic_object_index;	//how many fast dynamic objects
int poly_slot,size_of_polys;
_ZPoly *new_polys_address;
_ZPoly *old_polys_address;
register ZFObject * current_object_ptr;
extern scaled_points_array_type *scaled_points_ptr;

//for dust bit in polys
_ZPoly *the_polys;
int how_many_polys,poly_count;


if(scale<1)  report_error ("load_Fdyn_object: Scale less than 1.","\p Static slot =",static_slot);



//check if mass=0 and error if so
if (the_pb->mass<=0) report_error ("load_dyn_object: Mass=0 or -ve! Static slot =","\p",static_slot);
if (dest_focb!=-1)
	{
     current_object_ptr=&Focb_ptr->Fobject_list[dest_focb];
     if (current_object_ptr->in_use!=0)  report_error ("load_Fdyn_object: FDynamic Object already in use. Object must be dead.","\p Static slot =",static_slot);
//stuff from the static declaration
    current_object_ptr->number_of_polys=(*ocb_Sptr).object_list[static_slot].number_of_polys;	
	
	
	current_object_ptr->in_use=1;	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.
	current_object_ptr->is_visible=1;
	current_object_ptr->Oid=(*ocb_Sptr).object_list[static_slot].Oid;
	QuatSetValue(&current_object_ptr->Qor,0,0,1,0);	//pointing into z
	current_object_ptr->Oworldx=the_pb->world_x;
	current_object_ptr->Oworldy=the_pb->world_y;
	current_object_ptr->Oworldz=the_pb->world_z;
	current_object_ptr->scale=scale;
	current_object_ptr->number_of_polys=(*ocb_Sptr).object_list[static_slot].number_of_polys;
	current_object_ptr->number_of_points=(*ocb_Sptr).object_list[static_slot].number_of_points;
	current_object_ptr->points_ptr=(*ocb_Sptr).object_list[static_slot].points_ptr;
	current_object_ptr->polys_ptr=(*ocb_Sptr).object_list[static_slot].polys_ptr;
	current_object_ptr->dynamic_tex_ptr=0;


	current_object_ptr->controller=the_pb->controller_ref;
	current_object_ptr->mass=the_pb->mass;
	current_object_ptr->radius=(*ocb_Sptr).object_list[static_slot].radius;
	
//now copy polys out of static into vacant dynamic
	poly_slot=find_vacant_dyn_poly_slot();
	if	(poly_slot!=-1)
	{
	(*dynamic_polys_ptr)[poly_slot].in_use=1;	//prevent a crash - this poly slot is in use!
	//copy the polys across
	new_polys_address=&(*dynamic_polys_ptr)[poly_slot].the_obj_polys[0];	//so we can poke the new object
	old_polys_address=(*Focb_ptr).Fobject_list[dest_focb].polys_ptr;
	
	size_of_polys=(*ocb_Sptr).object_list[static_slot].number_of_polys;
	size_of_polys=size_of_polys*sizeof(_ZPoly);
	
	BlockMoveData	(old_polys_address,new_polys_address,size_of_polys);
	//poke the address of the polys into the new object
	(*Focb_ptr).Fobject_list[dest_focb].polys_ptr=new_polys_address;
	(*Focb_ptr).Fobject_list[dest_focb].active_polys_slot=poly_slot;	//so we can quickly kill these polys
	
	

	} else { report_error ("load_Fdyn_object: Couldn't find vacant poly slot =","\p",(int)dynamic_polys_ptr); }

        
          how_many_polys=current_object_ptr->number_of_polys;
          the_polys=current_object_ptr->polys_ptr;

          for (poly_count=0;poly_count<how_many_polys;poly_count++)
          {
  		    (*the_polys).owner=dest_focb;	//so we can back track from a poly to an object
		    the_polys++;	//next poly please
          }
//change as of version 0.53
//if scale!=1 then we need to copy and scale the points and update ocb[n].points_ptr to point to the new points
//the maximum number of points we can store in an object is 500
//and the maximum number of caled objects is 30. This is defined in scaled_object_points.
//when an object is killed we need to check the scaled flag and kill the scaled points object
//if necessary.
	 (*Focb_ptr).Fobject_list[dest_focb].scaled_points_entry=-1;
	 
	 if (scale!=1)
	 {
	 int slot,number_of_points,n;
	 _3D the_point;
	 _3D *source_point_ptr,*dest_point_ptr;
	 //scale radius
	 (*Focb_ptr).Fobject_list[dest_focb].radius*=scale;
	 
	 //scale points and redirect source points pointer
	  slot=get_scaled_points_slot();
	  if (slot==-1) report_error ("load_dyn_object: Couldn't find empty scaled object points slot","\p",static_slot);
	  (*Focb_ptr).Fobject_list[dest_focb].scaled_points_entry=slot;	//so we can kill em
	  number_of_points=(*Focb_ptr).Fobject_list[dest_focb].number_of_points;

	  if(number_of_points>NUMBER_OF_SCALED_POINTS)
	     report_error ("load_Fdyn_object: Too many points in object to scale","\p",static_slot);
	  //now copy the points and scale em
          source_point_ptr=(*Focb_ptr).Fobject_list[dest_focb].points_ptr;
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


	Fdynamic_object_index++;	//we've added an object that needs to be processed

	// call the forth controller new object controller for this object - just in case
	// it has a forth controller. Forth routines handle bounds checking
        return 0;
	} // end of if(dest_ocb!=-1)
	
report_error ("load_Fdyn_object: Destination slot not empty!.","\p Static slot =",static_slot);
return -1;
} // end of load_dyn_object


// -----------------------------------------------

//called from reset_objects
void clear_out_Fdyn()
{
extern int	Fdynamic_object_index;
int dest_ocb;

for (dest_ocb=0;dest_ocb<MAX_NUMBER_OF_FAST_DYN_OBJECTS;dest_ocb++)
	{
	Fi_know_what_im_doing_kill(dest_ocb,TRUE);	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.
	}

Fdynamic_object_index=0;
}
// -----------------------------------------------

//this routine should only be called if you really know what you are doing
//because Fdynamic_object_index will get out of sync otherwise 
//it also overrides the checking of an already killed object!
//It is called from above to kill all the old fast objects.
public void Fi_know_what_im_doing_kill( int the_object, int kill_textures)
{

	FZkill(the_object,kill_textures);
}
// -----------------------------------------------

module_private void FZkill(int the_object, int kill_ogl_textures)	//this should be called kill_obj, but this way I can say "kill(it)"
{
extern	DynFObjectsFixedSize *Focb_ptr;
extern _ZPoly_objs *dynamic_polys_ptr;
extern int	Fdynamic_object_index;
register ZFObject * current_object_ptr;
extern scaled_points_array_type *scaled_points_ptr;

int polys_slot,scaled_points_slot;

//mod 210599 - SB
//we check if the object is alive before killing it to prevent dynamic_object-index being erroneously
//decremented.

// check that we aren't killing dead objects
if (Focb_ptr->Fobject_list[the_object].in_use==0)
  {
   return;     //we now silently ignore the kill request
   }


     current_object_ptr=&Focb_ptr->Fobject_list[the_object];
     
//check for scaled points
     scaled_points_slot=current_object_ptr->scaled_points_entry;
     
     if (scaled_points_slot!=-1)
     {
      scaled_points_ptr->scaled_points[scaled_points_slot].in_use=0;
     }
     
     
	current_object_ptr->in_use=0;	//deaded
	
	//if has_kill_method set then call the controller
	//if (current_object_ptr->has_kill_method!=0) dispatch_control(the_object);
	
	// call the forth controller kill controller for this object - just in case it has
	// a forth controller. Forth routines handle bounds checking
	//coffee_object_deleted(the_object);
	
	
	polys_slot=current_object_ptr->active_polys_slot;	 //point to the polys
	(*dynamic_polys_ptr)[polys_slot].in_use=0;	//this poly slot is not in us
//if dynamic texture, release the memory
    if (current_object_ptr->dynamic_tex_ptr!=0)
	{
     if(kill_ogl_textures!=0)
	  {
	     kill_dynamic_texture(current_object_ptr->dynamic_tex_ptr);
      }
	}

	if (Fdynamic_object_index>0)
	Fdynamic_object_index--;
// }
}

// -----------------------------------------------
int find_vacant_Fdynamic_object_slot()	//returns an index into the Fdynamic_objects_array
{
extern	DynFObjectsFixedSize *Focb_ptr;	//the dynamic objects
int i;

for (i=1;i<MAX_NUMBER_OF_FAST_DYN_OBJECTS;i++)
	if ((*Focb_ptr).Fobject_list[i].in_use==0) return i;

add_to_text_display("Zex Exec> WARNING: find_vacant_fast_dynamic_object_slot failed!",5);	
return -1;
}

