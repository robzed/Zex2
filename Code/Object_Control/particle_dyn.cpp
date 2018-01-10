// Zex 2 - 3D Space Trading Game with in-cockpit view 
// Copyright (C) 1998-2018  Stuart Ball and Robert Probin
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

/*
 *  particle_dyn.cpp
 *  zex22
 *
 *  Created by Stuart Ball on Sun Jun 02 2002.
 *  Copyright (c) 2002 Lightsoft. All rights reserved.
 *
 */



#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #include "3d_structures_def.h"
  #include "zex_misc.h"
  #include "Error_handler.h"
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

#include "particle_dyn.h"




module_private void PZkill(int the_object, int kill_ogl_textures);
/*
How does this work then?

OK,we have primary and secondary particles.
Primary particles are processed like normal objects (except they have a reduced OCB)
Secondary particles are linked to primary particles and consist of simply a link and a 
vector specifying which primary they are linked to.

Very often what looks like a single particle on screen is actually a collection
of particles; each gun bullet for example is actually three objects. The lasers are
a collection of objects strung out along a line.


Particles are held in a linked list of POCB's (particle ocb's)

The structure looks like:
 int head_index;
 int tail_index;
 array of ZPobjects

This structure is called DynPObjectsFixedSize

(See 3d_structures_def.h for details)

The head index tells the software where in the array of POCBS the list starts. The tail tells 
it where it ends.

Each POCB contains a previous and next entry; this is how the list is formed. When we add a particle
to the system it gets added to the tail. When we kill a particle, we take the next POCB and join it
to the killed particle's previous.
*/



// -----------------------------------------------

void init_Pdyns(void)
{
int dest_ocb;
extern	DynPObjectsFixedSize *Pocb_ptr;

 Pocb_ptr->head_index=-1;
 Pocb_ptr->tail_index=-1; //All empty
 for (dest_ocb=0; dest_ocb<MAX_NUMBER_OF_PARTICLE_DYN_OBJECTS; dest_ocb++)
 {
  Pocb_ptr->Pobject_list[dest_ocb].previous=-1; //mark as free 
 }
}

// -----------------------------------------------

//this routine should only be called if you really know what you are doing
//because Fdynamic_object_index will get out of sync otherwise 
//it also overrides the checking of an already killed object!
//It is called from above to kill all the old fast objects.
zex_public void Pi_know_what_im_doing_kill( int the_object, int kill_textures)
{

	PZkill(the_object,kill_textures);
}
// -----------------------------------------------

module_private void PZkill(int the_object, int kill_ogl_textures)	//this should be called kill_obj, but this way I can say "kill(it)"
{
}

// -----------------------------------------------

//Load a particle, will return either the particles slot number or -1 if we couldn't load it
int load_Pdyn_object(int static_slot, load_dyn_object_pb *the_pb, int parent_slot)
{
extern	DynPObjectsFixedSize *Pocb_ptr;	//the particle slots
int free_slot=0;	//we return the slot the object was loaded into
int end_slot,temp_slot;
  
if (Pocb_ptr->head_index==-1)
{
 free_slot=0;
 Pocb_ptr->head_index=0;
 Pocb_ptr->tail_index=0;
}
else //not an empty array
{
 //go to end
 end_slot=Pocb_ptr->tail_index;
 
 //now find a free slot
 temp_slot=0;
 for (EVER)
 {
   if (Pocb_ptr->Pobject_list[temp_slot].previous==-1)
   {
     break;
   } 
   temp_slot++;
   if (temp_slot==MAX_NUMBER_OF_PARTICLE_DYN_OBJECTS)
   {
     return -1; //couldn't find a free slot!
   }
 
 }
 
} //end of not an empty array


  //OK, now we're patched in we can load the data
  
	
  return free_slot;
		  
}
