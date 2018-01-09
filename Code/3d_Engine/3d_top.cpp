//3d_top.cpp
/*
$Log: 3d_top.cpp,v $
Revision 1.5  2003/09/28 17:30:10  robp
Changed files from .c to .cpp and removed spaces out of a couple of filenames.

Revision 1.4  2003/09/28 10:36:03  robp
Signed/Unsigned comparison fixes from last night, plus collision fix.

Revision 1.3  2003/09/27 21:52:38  robp
Fixed places where we were loading an unsigned with -1.

Revision 1.2  2003/09/27 20:41:52  robp
Fixed float->int warnings.

Revision 1.1.1.1  2003/09/05 22:35:09  stu_c
First Imported.

Revision 1.36  2002/09/08 02:11:36  stu_c
Precompiled header adjustments

Revision 1.35  2002/06/24 21:27:54  stu_c
cos_angle_of_view declared extern but not working yet....

Revision 1.34  2002/06/16 23:19:28  stu
Particles booked in for backup purposes - not working yet or complete.

Revision 1.33  2002/05/31 23:55:14  stu
Removed all warnings from the code

Revision 1.32  2002/05/05 22:26:28  stu
Further developement - hull temperature now correct for multiple suns and atmosphere.

All calls to add_to_kill_list check the return

Revision 1.31  2002/04/14 15:58:10  stu
Further work

Revision 1.30  2002/03/19 23:18:26  stu
Work so objects are controlled when not in view

Revision 1.29  2002/03/16 00:43:30  stu
Continued development

Revision 1.28  2002/03/09 15:03:44  stu
Can now be right handed or left handed - RIGHT_HANDED is defined as 1 in 3d_structures

Revision 1.27  2002/02/28 00:00:32  stu
Used MOTHERSHIP_CONTROL instead of 29

Revision 1.26  2002/02/27 23:02:13  stu
Fixed motherships not geting taken out of view - too many visible polys error.

Revision 1.25  2002/02/24 19:11:47  rob
basically no changes - space changes due to depth of field work.

Revision 1.24  2002/02/24 03:39:06  stu
Removed a whole bunch of commented out code

Revision 1.23  2002/02/23 01:13:19  stu
Sun distance now initialised to a respectable value. Slight radar mod to remove a big conditional in radar display - we now take the range to be the radius rather than the diameter of the radar scan

Revision 1.22  2002/02/22 00:23:31  stu
A bit more poly filtering in do_culling2. If all points are -ve z we don't process the poly further

Revision 1.21  2002/02/21 22:43:47  stu
Docking

Revision 1.20  2002/02/17 06:19:26  stu
Added radar targetting

Revision 1.19  2002/02/16 19:01:10  stu
Radar range decrease key and rationalisation of dlp colours

Revision 1.18  2002/02/10 21:37:25  stu
fast dyn objects

Revision 1.17  2002/02/10 13:27:20  rob
Made binary suns both have flare...

Revision 1.16  2002/02/02 23:08:50  stu
eng_3d debug pane support and some LOD mods

Revision 1.15  2002/01/27 18:03:33  stu
Removed used variable vis

Revision 1.14  2002/01/20 16:46:48  stu
Optimised light path a little, black level adjustment.

Revision 1.13  2001/12/29 23:02:43  stu
Added check for Goutside in do_sun_lens_flares

Revision 1.12  2001/12/29 01:09:34  stu
Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.

Revision 1.11  2001/12/14 19:40:57  stu
Docking

Revision 1.10  2001/12/04 23:35:19  stu
Motherships work

Revision 1.9  2001/12/02 19:31:48  stu
Radar runs every fifth frame. Now draws bottom, graticule, top items.

Revision 1.8  2001/11/28 21:40:35  stu
Finish Eclipse so works on Non os x carbon as well.

Revision 1.7  2001/11/24 19:27:02  stu
RA_line_len in place of sqrt

Revision 1.6  2001/11/05 19:59:53  stu
More OSX work

Revision 1.5  2001/11/02 19:32:37  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.4  2001/10/22 22:37:10  rob
Carbon warning changes
(last one)

Revision 1.3  2001/10/22 22:12:40  stu
More Carbon work

Revision 1.2  2001/10/22 21:27:52  rob
Carbon warning changes

Revision 1.1  2001/10/21 01:07:20  stu
Initial porting work

Revision 1.0.0.1  2001/10/17 20:46:04  rob
First Imported.

 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #include "3d_structures_def.h"
  #include "Error_handler.h"
  #include "options.h"
  #include "processor.h"
  #include "zex_misc.h"
  #include "access_ocbs.h"
#endif //pch

#include "matrix_primitives.h"
#include "3d_top.h"
#include "camera_control.h"
#include "insert_object.h"
#include "debug_timing.h"
#include "Physics.h"
#include	"objects_controls.h"	//controller definitions
#include "dlp.h"
#include "Zsound.h"
#include "light_control.h"
#include "splat_control.h"
#include "draw_radar_and_cwp.h"
#include "pobs.h"

//debug info to coffee
int debug_dynamic_objects;
int debug_radar_objects;
int debug_radius_objects;
int debug_in_frustum_objects;
int debug_polys_translated;
//3d_top.int
//view debug pane
int view_fast_object;
int view_sun_object;
int view_planet_object;
int view_far_star_object;
int view_dust_object;
int view_other_object;
int view_moon_object;
int view_mship_object;

 
DynObjectsFixedSize *ocb_ptr;
DynFObjectsFixedSize *Focb_ptr;
DynPObjectsFixedSize *Pocb_ptr;

StatObjectsFixedSize *ocb_Sptr;
cmap_objects *cmap_ptr_base;
scaled_points_array_type *scaled_points_ptr;


_3D *points_list_ptr;
visible_poly *visible_points_list_ptr;
_3D *points_list_ptr_base;
_3D *points_list_ptr_reset;	//reset value for points_list_pointer
visible_poly *visible_points_list_base;
_3D *world_points_list_ptr;
//_3D *temp_points_list_ptr;

_2DPoly *view_points_list_ptr;
_3D light_normal;	//the light vecterre
_3D camera_pos;	//+z inwards remember
_3D camera_pos_delta;	//change to enable auto camera movement - dont do it anymore!
//

textured_connections fixed_triag_connection [3] = 
{  { { 0, 0xffff,0xffff,}, {0,0,0} ,0,0,0,0,0,0 },
   { { 1, 0xffff,0xffff,}, {0,0,0} ,0,0,0,0,0,0 },
   { { 2, 0xffff,0xffff,}, {0,0,0} ,0,0,0,0,0,0 } }; //used to create polys in explode
_3D fixed_triag_points[3] = { { 0,50,0, },
                             { -50,-50,0, },
                            { 50,-50,0 } };
                             





Polygon **Polygon_handle;	
Polygon *Polygon_ptr;	//these two are temp for drawing with system poly routine

textured_connections *connection_list_ptr;	 //global connection list ptr
textured_connections *connection_list_ptr_reset;	//global connection list ptr
textured_connections* connection_list_top;

_ZPoly *Spoly_list_ptr;	//static objects holding area, compiled from Mesh input
_ZPoly *Spoly_list_ptr_reset;
_ZPoly * Spoly_list_top;

_ZPoly *poly_list_ptr;	//dynamic working area
_ZPoly_objs *dynamic_polys_ptr;


_clipped_poly* clipped_polys_ptr;
int *points_offsets_ptr; //array of ints specifying offset to points for object.

//index into various storages
int connection_list_index;
int points_list_index;
int world_points_list_index;
int dynamic_object_index;
int Fdynamic_object_index;

int poly_list_index;
int freeze;	//if 1 3d does not add deltas
//extent of various storages

int points_list_top;
int visible_points_list_top;
int view_points_list_top;
int world_points_list_top;
int dynamic_object_top;
int poly_list_top;
//int Spoly_list_top;

int points_offsets_top;
int	total_points_counter;
//ptrs to polys for z sort
_ZPolySort zsort_list[MAX_NUMBER_OF_POLYS];	//max of 2000 polys in system at any on time
int zsort_index; //should not really be here
int zsort_start_index,zsort_count,zbuffer_next_free;
double last_z;



module_private void do_sun_lens_flares(void);
bool radius_filter_for_planets(int the_object);

module_private void z_clip(_ZPoly* the_poly, visible_poly* input_points, _clipped_poly* output_poly);
module_private int front_clip(_clipped_poly* output_poly,int vertex_index, _3D V1, _3D V2,
               int L1, int L2, texture_coord T1, texture_coord T2);

//#ifndef __zex_defs__
//#include "Zex_defs.h"
//#endif
//this is the top level function of the transformation engine
//it takes a list of points
//do_3d (int number_of_points,float *points_list)

//Note: Typically, anything in this code-set begining with a capital 'Q' means quternion and vv is velocity vector
float Qor_delta=0.01;

int backface_cull=-1;	//backface culling on/off
int zbuffer_divisor;
extern int can_do_radar_display_calcs;



// -------------------------------------------------------------------

void do_3d ()
{
int	number_of_points;
_3D *the_points;

extern	int	total_points_counter;
extern int dynamic_object_index;

int points_offset=0;	//how many points we have transformed (ultimately added to connections)
int object_counter;
int *offsets;
_3D *output_points3;
_ZPoly *the_polys;
int i,number_of_polys,active_object_counter;
extern int *points_offsets_ptr; //array of ints specifying offset to points for object.
extern _3D ship_rotated_normal; //from engine
extern int objs_displayed;
extern int camera_mode,radar_points_counter;
ZObject *source_object;
ZFObject *Fsource_object;
register ZObject * current_object_ptr;
register ZFObject * current_Fobject_ptr;
double temp_matrix[4][4],cam_orient_matrix[4][4], global_matrix[4][4];
extern radar_entry radar_list[];
_3D object_center;
static signed int radar_display_delay=0;
extern int can_do_radar_display_calcs;
extern int	Fdynamic_object_index;	//how many fast dynamic objects
int scaled_points_slot;
extern scaled_points_array_type *scaled_points_ptr;
vector ship,vect;
float distance;
extern float gradar_range;

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
//goto debug;
//radar code
active_object_counter=0;
object_counter=0;
objs_displayed=0;
object_center.x=0;
object_center.y=0;
object_center.z=0;

//fov stats collection
view_fast_object=0;
view_sun_object=0;
view_planet_object=0;
view_far_star_object=0;
view_dust_object=0;
view_other_object=0;
view_moon_object=0;
view_mship_object=0;
view_moon_object=0;

if(camera_mode==1)
{
_quat inv_camera;
int camera_object=0;

 inv_camera.w=(*ocb_ptr).object_list[camera_object].Qor.w;
 inv_camera.x=-(*ocb_ptr).object_list[camera_object].Qor.x;
 inv_camera.y=-(*ocb_ptr).object_list[camera_object].Qor.y;
 inv_camera.z=-(*ocb_ptr).object_list[camera_object].Qor.z;

TR_Qrot(cam_orient_matrix,inv_camera);	//get camera orientation matrix from obj 0

}

//loop through all active visible objects

offsets =points_offsets_ptr;	//point to offsets arrray

world_points_list_index=0;	//our transformed points go here

//output_points =radar_list;
active_object_counter=2;
object_counter=2;	//0 is zex, 1 is vv (vv is velocity vector)
		#if ENG_DEBUG_EXPORT==1
         debug_radius_objects=0;
		 debug_in_frustum_objects=0;
		 debug_polys_translated=0;
		#endif

// --------------RADAR PROCESSING - RUNS every 5th frame


radar_display_delay--;
if (radar_display_delay>0)
{
 can_do_radar_display_calcs=0;
}
else
{
 radar_display_delay=4;
 can_do_radar_display_calcs=1;
}


#if ENG_DEBUG_EXPORT==1
debug_dynamic_objects=dynamic_object_index;
#endif

if (can_do_radar_display_calcs)
{

ship.x=(*ocb_ptr).object_list[0].Oworldx;
ship.y=(*ocb_ptr).object_list[0].Oworldy;
ship.z=(*ocb_ptr).object_list[0].Oworldz;

radar_points_counter=0;	///for subsequent function - pers for example

while (active_object_counter<dynamic_object_index)
{
    current_object_ptr=&ocb_ptr->object_list[object_counter];

 if ((current_object_ptr->in_use==1))	//zex or vv
  {
//е object transform/rotate one point to radar list
    if (current_object_ptr->Dyn_OCB_control_data.controller!=DUST 
        && current_object_ptr->Dyn_OCB_control_data.controller!=FAR_STAR
        && current_object_ptr->Dyn_OCB_control_data.allegiance!=NO_RADAR)
    {
    if((current_object_ptr->bolted_to==0) || (current_object_ptr->targetted==1))	//when targetting moved to weapons, get targetted item and its parent
                                                                                        //allow the targetted object through but not its parent.
                                                                                        //disallow all none -parent objects.
    {
	//find out if in radar range
	
	    vect.x=(*ocb_ptr).object_list[object_counter].Oworldx-ship.x;
        vect.y=(*ocb_ptr).object_list[object_counter].Oworldy-ship.y;
		vect.z=(*ocb_ptr).object_list[object_counter].Oworldz-ship.z;
//vect is now a vector to the object, we have to project it onto a sphere...
//get length of vector
        distance=vectorMod(&vect);	//yuck, sqrt
        if (distance<(gradar_range/2))	    //is object in range of radar - global var
        {
	
         source_object=&(*ocb_ptr).object_list[object_counter];
 
         radar_list[radar_points_counter].the_type=(*source_object).Dyn_OCB_control_data.allegiance;
         radar_list[radar_points_counter].is_target=(*source_object).targetted;
         radar_list[radar_points_counter].Oid=(*source_object).Oid;
	     radar_list[radar_points_counter].object_slot=object_counter;
         TR_Qrot(global_matrix,source_object->Qor);	//apply rotation as per orinetation quaternion
         TR_Translate(global_matrix,(*source_object).Oworldx,(*source_object).Oworldy,(*source_object).Oworldz);
         TR_Translate(global_matrix,camera_pos.x,camera_pos.y,camera_pos.z);
         MAT_Mult(global_matrix,cam_orient_matrix,temp_matrix);
 
 
 //transform one point from the object into the radar points
         if (radar_points_counter<150)
		{
         // if (object_counter!=1)
          {
           VEC_MultMatrix(&object_center,temp_matrix,&radar_list[radar_points_counter].the_vect);
           radar_points_counter++;
          }
        }
	  
	  }//end of in range
	  
     } //end of bolted
     
     }	     //end of not dust
   
  active_object_counter+=1;

 } //end of if in_use
 
object_counter++;

//е do next object
}	//end of while
#if ENG_DEBUG_EXPORT==1
debug_radar_objects=radar_points_counter;
#endif
}//end of radar processing




//object brick wall fov filter - SB 01/12/98
//trivial rejection




//debug:



active_object_counter=0;
object_counter=0;
objs_displayed=0;
// ------------------------ OBJECT FOV FILTERS -------------------------------

while (active_object_counter<dynamic_object_index)
 {
//     _3D p1,p2;
     int vis;
//     double distance_to;
//     extern _3D camera_pos;


  current_object_ptr=&ocb_ptr->object_list[object_counter];

  if (current_object_ptr->in_use==1 ) //if object is alive
   {
   vis=0;

   if (object_counter<2 ) 
   {
     vis=1;	//Zex and VV has to be visible so we update it's position etc.
     goto got_vis;
   }
   
// taken out 270703 - now taken care of by frustum   
//   if (radius_filter_for_planets(object_counter))
//   {
//   		 #if ENG_DEBUG_EXPORT==1
//         debug_radius_objects++;
//		#endif
//		vis=1; goto got_vis;
//   }

     //else check if the object is in view
   if (is_in_frustrum(object_counter,FALSE)==1)
   {
    vis=1;
		 #if ENG_DEBUG_EXPORT==1
         debug_in_frustum_objects++;
		#endif   
   }
got_vis:
//vis=1;	//debug
    if (vis==1)
    {
      current_object_ptr->is_visible=1;    
      objs_displayed++;
    } 
    else
    {
     current_object_ptr->is_visible=0;
    }
	 
	active_object_counter++;
   } //end of in_use

  object_counter++;  
  }	//end of while	
//end of new code



//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
//Transformation/rotation on visible objects - SB 02/11/98



//if(camera_mode==1)
//{
//_quat inv_camera;
//int camera_object=get_main_camera_object();
//
//inv_camera.w=(*ocb_ptr).object_list[camera_object].Qor.w;
//inv_camera.x=-(*ocb_ptr).object_list[camera_object].Qor.x;
//inv_camera.y=-(*ocb_ptr).object_list[camera_object].Qor.y;
//inv_camera.z=-(*ocb_ptr).object_list[camera_object].Qor.z;
//TR_Qrot(cam_orient_matrix,inv_camera);	//get camera orientation matrix from obj 0
//
//}

if(camera_mode==1)
{
_quat inv_camera;
int camera_object=CAMERA_OBJECT; //camera

 inv_camera.w=(*ocb_ptr).object_list[camera_object].Qor.w;
 inv_camera.x=-(*ocb_ptr).object_list[camera_object].Qor.x;
 inv_camera.y=-(*ocb_ptr).object_list[camera_object].Qor.y;
 inv_camera.z=-(*ocb_ptr).object_list[camera_object].Qor.z;

TR_Qrot(cam_orient_matrix,inv_camera);	//get camera orientation matrix from obj 0

}

//loop through all active visible objects
total_points_counter=0;	///for subsequent function - pers for example

offsets =points_offsets_ptr;	//point to offsets arrray

world_points_list_index=0;	//our transformed points go here

output_points3 =world_points_list_ptr;
active_object_counter=0;
object_counter=0;


while (active_object_counter<dynamic_object_index)
{

 current_object_ptr=&ocb_ptr->object_list[object_counter];

 if (current_object_ptr->in_use==1)
  {
     source_object=&(*ocb_ptr).object_list[object_counter];
     apply_extern_forces(&source_object->Qor, &source_object->torque_sum, source_object->mass);

   if (current_object_ptr->is_visible==0)
   {
	TR_Qrot(global_matrix,source_object->Qor);	//apply rotation as per orinetation quaternion
    
    VEC_MultMatrix(&source_object->box_min,global_matrix,&source_object->box_min_translated);
    VEC_MultMatrix(&source_object->box_max,global_matrix,&source_object->box_max_translated);
//do mounting points
    VEC_MultMatrix(&source_object->mthr1,global_matrix,&source_object->mthr1_translated);
    VEC_MultMatrix(&source_object->mthr2,global_matrix,&source_object->mthr2_translated);
    VEC_MultMatrix(&source_object->mrev1,global_matrix,&source_object->mrev1_translated);
    VEC_MultMatrix(&source_object->mrev2,global_matrix,&source_object->mrev2_translated);

    VEC_MultMatrix(&source_object->vthr1,global_matrix,&source_object->vthr1_translated);
    VEC_MultMatrix(&source_object->vthr2,global_matrix,&source_object->vthr2_translated);
    VEC_MultMatrix(&source_object->rvth1,global_matrix,&source_object->rvth1_translated);
    VEC_MultMatrix(&source_object->rvth2,global_matrix,&source_object->rvth2_translated);

    VEC_MultMatrix(&source_object->gun01,global_matrix,&source_object->gun01_translated);
    VEC_MultMatrix(&source_object->gun02,global_matrix,&source_object->gun02_translated);
    VEC_MultMatrix(&source_object->gun03,global_matrix,&source_object->gun03_translated);
    VEC_MultMatrix(&source_object->gun04,global_matrix,&source_object->gun04_translated);

    VEC_MultMatrix(&source_object->las01,global_matrix,&source_object->las01_translated);
    VEC_MultMatrix(&source_object->las02,global_matrix,&source_object->las02_translated);
    VEC_MultMatrix(&source_object->las03,global_matrix,&source_object->las03_translated);
    VEC_MultMatrix(&source_object->las04,global_matrix,&source_object->las04_translated);

    VEC_MultMatrix(&source_object->mis01,global_matrix,&source_object->mis01_translated);
    VEC_MultMatrix(&source_object->mis02,global_matrix,&source_object->mis02_translated);
    VEC_MultMatrix(&source_object->mis03,global_matrix,&source_object->mis03_translated);

    VEC_MultMatrix(&source_object->eject,global_matrix,&source_object->eject_translated);

//	TR_Qrot(global_matrix,source_object->Qor);	//apply rotation as per orinetation quaternion

//translate axis normals
      VEC_MultMatrix(&(*ocb_ptr).object_list[object_counter].n,global_matrix,&(*ocb_ptr).object_list[object_counter].n_translated);
      VEC_MultMatrix(&(*ocb_ptr).object_list[object_counter].s,global_matrix,&(*ocb_ptr).object_list[object_counter].s_translated);
      VEC_MultMatrix(&(*ocb_ptr).object_list[object_counter].v,global_matrix,&(*ocb_ptr).object_list[object_counter].v_translated);
   }
   else //is visible
   
   {
//е object transform/rotate loop
//     this_object_points_offset =points_offset;	//the offset applied to this objects connections


     number_of_points=(*source_object).number_of_points;

     number_of_polys=(*source_object).number_of_polys;
	#if ENG_DEBUG_EXPORT==1
        if(current_object_ptr->is_blobbed==0)
        { 
         debug_polys_translated+=number_of_polys;
	    }
		else
		{
         debug_polys_translated+=1;
  
		}
	#endif   
     
     
     the_polys=(*source_object).polys_ptr;

//pick right set of source points.
     scaled_points_slot=(*source_object).scaled_points_entry;
     if(scaled_points_slot==-1)
     the_points=(*source_object).points_ptr;
     else
     the_points=scaled_points_ptr->scaled_points[scaled_points_slot].scaled_points_start;
     
//store trasnformed points ptr
     (*source_object).world_points_ptr=output_points3;
    
//     apply_extern_forces(&source_object->Qor, &source_object->torque_sum, source_object->mass);
     TR_Qrot(global_matrix,source_object->Qor);	//apply rotation as per orinetation quaternion

     TR_Translate(global_matrix,(*source_object).Oworldx,(*source_object).Oworldy,(*source_object).Oworldz);

     TR_Translate(global_matrix,camera_pos.x,camera_pos.y,camera_pos.z);
     MAT_Mult(global_matrix,cam_orient_matrix,temp_matrix);


//Now transform/rotate the points.

       for (i=0; i<number_of_points; i++)
       {

          VEC_MultMatrix(&(*the_points),temp_matrix,output_points3);
  	      the_points++;
	      output_points3++;
          points_offset++;	//done another point

        }
    
//    } //end of not blobbed
	
	TR_Qrot(global_matrix,source_object->Qor);	//apply rotation as per orinetation quaternion
    
    VEC_MultMatrix(&source_object->box_min,global_matrix,&source_object->box_min_translated);
    VEC_MultMatrix(&source_object->box_max,global_matrix,&source_object->box_max_translated);
//do mounting points
    VEC_MultMatrix(&source_object->mthr1,global_matrix,&source_object->mthr1_translated);
    VEC_MultMatrix(&source_object->mthr2,global_matrix,&source_object->mthr2_translated);
    VEC_MultMatrix(&source_object->mrev1,global_matrix,&source_object->mrev1_translated);
    VEC_MultMatrix(&source_object->mrev2,global_matrix,&source_object->mrev2_translated);

    VEC_MultMatrix(&source_object->vthr1,global_matrix,&source_object->vthr1_translated);
    VEC_MultMatrix(&source_object->vthr2,global_matrix,&source_object->vthr2_translated);
    VEC_MultMatrix(&source_object->rvth1,global_matrix,&source_object->rvth1_translated);
    VEC_MultMatrix(&source_object->rvth2,global_matrix,&source_object->rvth2_translated);

    VEC_MultMatrix(&source_object->gun01,global_matrix,&source_object->gun01_translated);
    VEC_MultMatrix(&source_object->gun02,global_matrix,&source_object->gun02_translated);
    VEC_MultMatrix(&source_object->gun03,global_matrix,&source_object->gun03_translated);
    VEC_MultMatrix(&source_object->gun04,global_matrix,&source_object->gun04_translated);

    VEC_MultMatrix(&source_object->las01,global_matrix,&source_object->las01_translated);
    VEC_MultMatrix(&source_object->las02,global_matrix,&source_object->las02_translated);
    VEC_MultMatrix(&source_object->las03,global_matrix,&source_object->las03_translated);
    VEC_MultMatrix(&source_object->las04,global_matrix,&source_object->las04_translated);

    VEC_MultMatrix(&source_object->mis01,global_matrix,&source_object->mis01_translated);
    VEC_MultMatrix(&source_object->mis02,global_matrix,&source_object->mis02_translated);
    VEC_MultMatrix(&source_object->mis03,global_matrix,&source_object->mis03_translated);

    VEC_MultMatrix(&source_object->eject,global_matrix,&source_object->eject_translated);

//	TR_Qrot(global_matrix,source_object->Qor);	//apply rotation as per orinetation quaternion

//translate axis normals
      VEC_MultMatrix(&(*ocb_ptr).object_list[object_counter].n,global_matrix,&(*ocb_ptr).object_list[object_counter].n_translated);
      VEC_MultMatrix(&(*ocb_ptr).object_list[object_counter].s,global_matrix,&(*ocb_ptr).object_list[object_counter].s_translated);
      VEC_MultMatrix(&(*ocb_ptr).object_list[object_counter].v,global_matrix,&(*ocb_ptr).object_list[object_counter].v_translated);


//	TR_Qrot(global_matrix,source_object->Qor);	//apply rotation as per orinetation quaternion
//        MAT_Mult(global_matrix,cam_orient_matrix,temp_matrix);
//    #if (0)

//if is_blobbed is set we only copy the first poly
   if(current_object_ptr->is_blobbed==1)
   {
      VEC_MultMatrix(&(*the_polys).face_normal,global_matrix,&(*the_polys).face_normal_translated);      
      the_polys++;	//next poly
   }
   else
   {
    for (i=0; i<number_of_polys; i++)
     {
      VEC_MultMatrix(&(*the_polys).face_normal,global_matrix,&(*the_polys).face_normal_translated);      
      the_polys++;	//next poly

     }
   }	//end of blobbed
//     #endif

   } //end of is_visible
  active_object_counter+=1;

 } //end of if in_use
object_counter++;

//е do next object
}	//end of while



//------------------------ now do fast objects ---------------------------
//FAST VISIBILITY
active_object_counter=0;
object_counter=0;


while (active_object_counter<Fdynamic_object_index)
 {
     int vis;

  current_Fobject_ptr=&Focb_ptr->Fobject_list[object_counter];
  if (current_Fobject_ptr->in_use==1 ) //if object is alive
   {
   vis=0;
   if (is_in_frustrum(object_counter,TRUE)==1)
   {
    vis=1;
		 #if ENG_DEBUG_EXPORT==1
         debug_in_frustum_objects++;
		#endif   
   }

    if (vis==1)
    {
      current_Fobject_ptr->is_visible=1;    
      objs_displayed++;
    } 
    else
    {
     current_Fobject_ptr->is_visible=0;
    }
	active_object_counter++;
   } //end of in_use
  object_counter++;  
  }	//end of while	


active_object_counter=0;
object_counter=0;



//FAST TRANSFORMATION
while (active_object_counter<Fdynamic_object_index)
{

 current_Fobject_ptr=&Focb_ptr->Fobject_list[object_counter];

 if (current_Fobject_ptr->in_use==1)
  {
   if (current_Fobject_ptr->is_visible==1)
    {
//е object transform/rotate loop
//     this_object_points_offset =points_offset;	//the offset applied to this objects connections

     Fsource_object=&(*Focb_ptr).Fobject_list[object_counter];

     number_of_points=(*Fsource_object).number_of_points;

     number_of_polys=(*Fsource_object).number_of_polys;
	#if ENG_DEBUG_EXPORT==1
         debug_polys_translated+=1;
	#endif   
     
     the_polys=(*Fsource_object).polys_ptr;

//pick right set of source points.
     scaled_points_slot=(*Fsource_object).scaled_points_entry;
     if(scaled_points_slot==-1)
     the_points=(*Fsource_object).points_ptr;
     else
     the_points=scaled_points_ptr->scaled_points[scaled_points_slot].scaled_points_start;
     
//store trasnformed points ptr
     (*Fsource_object).world_points_ptr=output_points3;
    
     TR_Qrot(global_matrix,Fsource_object->Qor);	//apply rotation as per orinetation quaternion

     TR_Translate(global_matrix,(*Fsource_object).Oworldx,(*Fsource_object).Oworldy,(*Fsource_object).Oworldz);

     TR_Translate(global_matrix,camera_pos.x,camera_pos.y,camera_pos.z);
     MAT_Mult(global_matrix,cam_orient_matrix,temp_matrix);

//Now transform/rotate the points.
       for (i=0; i<number_of_points; i++)
       {
          VEC_MultMatrix(&(*the_points),temp_matrix,output_points3);
  	      the_points++;
	      output_points3++;
          points_offset++;	//done another point
        }
    	    
    for (i=0; i<number_of_polys; i++)
     {
      VEC_MultMatrix(&(*the_polys).face_normal,global_matrix,&(*the_polys).face_normal_translated);      
      the_polys++;	//next poly

     }
   } //end of is_visible
  active_object_counter+=1;
 } //end of if in_use
object_counter++;

//е do next object
}	//end of while



//end of fast objects
 ship_rotated_normal=(*ocb_ptr).object_list[2].n_translated; //camera
}	//end



//checks if an object is visible

bool radius_filter_for_planets(int the_object)
{
register ZObject * current_object_ptr;
double distance_to;
double fraction_of_radius;

// This routine first applies a radius test and if htat passes as wide frustum test
//Needed for really big objects like planets.
//THis code can be replaced with a smarter frustum test that takes into account the object radius...

  current_object_ptr=&ocb_ptr->object_list[the_object];
  
 	 if ( current_object_ptr->Dyn_OCB_control_data.controller!=PLANET_CONTROL) return FALSE; //currently only handles planets

         #define DISTANCE_TO_RADIUS_CONST 1.3	//radius filter correction factor

         fraction_of_radius=current_object_ptr->radius*DISTANCE_TO_RADIUS_CONST;
         distance_to=get_distance_to_from(the_object,0);

         if (distance_to<fraction_of_radius) 	//then it's visible
         { 
		  //now do wide angle frustum test
          _3D p1,p2;
          extern _3D ship_rotated_normal; //from engine

     	  p1.x=ocb_ptr->object_list[0].Oworldx;
     	  p1.y=ocb_ptr->object_list[0].Oworldy;
     	  p1.z=ocb_ptr->object_list[0].Oworldz;

       	p2.x=ocb_ptr->object_list[the_object].Oworldx;
     	  p2.y=ocb_ptr->object_list[the_object].Oworldy;
     	  p2.z=ocb_ptr->object_list[the_object].Oworldz;
     
     	  p1.x=p2.x-p1.x; p1.y=p2.y-p1.y; p1.z=p2.z-p1.z;	//get vector to object
        
//		distance_to=RA_line_len(p1.x, p1.y, p1.z);
          distance_to=(sqrt(p1.x*p1.x+p1.y*p1.y+p1.z*p1.z));
          if (distance_to<0) distance_to=-distance_to;
	 
	      Vnormalise(&p1);	 //normalised vector to object
	
	//compare normal to ship_rotated_normal and if         
		  p1.x=p1.x-ship_rotated_normal.x;
          p1.y=p1.y-ship_rotated_normal.y;
          p1.z=p1.z-ship_rotated_normal.z;
        
          if (p1.x<0) p1.x=-p1.x;
          if (p1.y<0) p1.y=-p1.y;
          if (p1.z<0) p1.z=-p1.z;
         

          if (p1.x<0.90 && p1.y<0.90 && p1.z <0.90)  return TRUE; //was 0.53
          else return FALSE;      
		  
 	     }	//end of radius test
		 else
		 {
		  return FALSE;
		 }

}



int is_in_frustrum(int object, bool fast_object)
{
_3D p1,p2;
//int distance_to;
double distance_to;
extern _3D ship_rotated_normal; //from engine
//extern double cos_angle_of_view; //calculated in init_depth_of_field

     	p1.x=ocb_ptr->object_list[0].Oworldx;
     	p1.y=ocb_ptr->object_list[0].Oworldy;
     	p1.z=ocb_ptr->object_list[0].Oworldz;

if (fast_object)
{

     	p2.x=Focb_ptr->Fobject_list[object].Oworldx;
     	p2.y=Focb_ptr->Fobject_list[object].Oworldy;
     	p2.z=Focb_ptr->Fobject_list[object].Oworldz;
     
     	p1.x=p2.x-p1.x; p1.y=p2.y-p1.y; p1.z=p2.z-p1.z;	//get vector to object
        
//		distance_to=RA_line_len(p1.x, p1.y, p1.z);
        distance_to=(sqrt(p1.x*p1.x+p1.y*p1.y+p1.z*p1.z));
        if (distance_to<0) distance_to=-distance_to;
	 
	 Vnormalise(&p1);	 //normalised vector to object
	
	//compare normal to ship_rotated_normal and if         
         p1.x=p1.x-ship_rotated_normal.x; //actualy the camera
         p1.y=p1.y-ship_rotated_normal.y;
         p1.z=p1.z-ship_rotated_normal.z;
        
         if (p1.x<0) p1.x=-p1.x;
         if (p1.y<0) p1.y=-p1.y;
         if (p1.z<0) p1.z=-p1.z;
         

          if (p1.x<0.4 && p1.y<0.4 && p1.z <0.4) 
		  {
           view_fast_object++;
		   return 1;	//0.4 is ok
		  }
		  else return 0;      

	  

 
} //end of fast object




         if (ocb_ptr->object_list[object].Dyn_OCB_control_data.controller==SUN)	//suns always visible
         {
           view_sun_object++;

            return 1;
	 }


     	p2.x=ocb_ptr->object_list[object].Oworldx;
     	p2.y=ocb_ptr->object_list[object].Oworldy;
     	p2.z=ocb_ptr->object_list[object].Oworldz;
     
     	p1.x=p2.x-p1.x; p1.y=p2.y-p1.y; p1.z=p2.z-p1.z;	//get vector to object
        
		distance_to=RA_line_len(p1.x, p1.y, p1.z);
	 
	 Vnormalise(&p1);	 //normalised vector to object
	
	//compare normal to ship_rotated_normal and if         
         p1.x=p1.x-ship_rotated_normal.x; //actually the camera
         p1.y=p1.y-ship_rotated_normal.y;
         p1.z=p1.z-ship_rotated_normal.z;
        
         if (p1.x<0) p1.x=-p1.x;
         if (p1.y<0) p1.y=-p1.y;
         if (p1.z<0) p1.z=-p1.z;
         

 	 if ( ocb_ptr->object_list[object].Dyn_OCB_control_data.controller==PLANET_CONTROL)
	 {
//          if (p1.x<0.43 && p1.y<0.43 && p1.z <0.43)  
          if (1)  //planets are always visible
		  {
		  int galactic_id=ocb_ptr->object_list[object].Dyn_OCB_control_data.galactic_id;
		   if (POBs_is_this_a_moon(galactic_id))
		   {
		    view_moon_object++;
		   }
		   else
		   {
                    view_planet_object++;
		   }
		  return 1; //was 0.53
          }
		  else return 0;      
	 }


/*
 	 if ( ocb_ptr->object_list[object].Dyn_OCB_control_data.controller==CLOUD_CONTROL)
	 {
	  //return 1;
	  
          if (p1.x<0.73 && p1.y<0.73 && p1.z <0.73)  return 1;
          else return 0;      
	 }
*/

 	 if ( ocb_ptr->object_list[object].Dyn_OCB_control_data.controller==FAR_STAR)
	 {
          if (p1.x<0.40 && p1.y<0.40 && p1.z <0.40)  
		  {
           view_far_star_object++;
		  return 1;	//0.4 is ok
          }
		  else return 0;      

	  
	 }

//        distance_to=(sqrt(p1.x*p1.x+p1.y*p1.y+p1.z*p1.z));
        if (distance_to<0) distance_to=-distance_to;
	
	 if (ocb_ptr->object_list[object].Dyn_OCB_control_data.controller==DUST)
	 {
	  if (p1.x<0.73 && p1.y<0.73 && p1.z <0.73 && distance_to<50000)      
	  {
           view_dust_object++;
	    return 1;
 	  }
	  else return 0;
	 }
	 else
	{
        _ZPoly *the_polys;
        
            if (p1.x<0.72 && p1.y<0.72 && p1.z <0.72 )  
            {

             if ( distance_to<15000000)
             {

	        if (ocb_ptr->object_list[object].Dyn_OCB_control_data.controller==MOTHERSHIP_CONTROL)
			{
			 view_mship_object++;
                         ocb_ptr->object_list[object].is_blobbed=0;
                         the_polys=ocb_ptr->object_list[object].polys_ptr;
                         if (the_polys->polyflags&AUTOBLOB_BIT)
                         {
                          (*the_polys).polyflags=0;	//clear blob first poly
                         }
                         return 1;
			}
			else
			{
	  		 view_other_object++;
                         return 1;
	                }
              }
              
              else
              
              if ( distance_to<100000000000.0)
              {
                
	        if (ocb_ptr->object_list[object].Dyn_OCB_control_data.controller==MOTHERSHIP_CONTROL)
			{
			 view_mship_object++;
                         ocb_ptr->object_list[object].is_blobbed=1;
                         the_polys=ocb_ptr->object_list[object].polys_ptr;
                         (*the_polys).polyflags|=AUTOBLOB_BIT;
                         return 1;
			}
			else
			{
                        return 0; //if not a mothership it's not visible past this range
	                }
               }
            return 0;   
            }
          
 	    else return 0;
	 }
	 
}



_3D generate_point_in_frustrum(int object)
{
//int x,y;
float z;
_3D p1;
extern _3D ship_rotated_normal; //from engine
//generate z
	z=RangedRdm(0,30000)*2;
//	x=RangedRdm(0,z)-(z/2);
//	y=RangedRdm(0,z)-(z/2);
	p1.x=z*ship_rotated_normal.x; p1.y=z*ship_rotated_normal.y;p1.z=z*ship_rotated_normal.z;	//this should of course be object
	p1.x+=(*ocb_ptr).object_list[object].Oworldx;
	p1.y+=(*ocb_ptr).object_list[object].Oworldy;
	p1.z+=(*ocb_ptr).object_list[object].Oworldz;
	
return p1; 
}


//еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
//this routine looks at each "on" object.
//for each poly in the object, it determins if visible and if so
//copies visible points (and the poly def) to visible_points_list[] for further work by pers2
double zex_z;	 //zex's z so we know ehn to draw the dlp

void do_culling2()
{
extern	DynObjectsFixedSize *ocb_ptr;
extern	int number_of_polys_displayed;
extern visible_poly *visible_points_list_ptr;
extern visible_poly *visible_points_list_base;
//extern int zsort_index;

//_3D Camera_normal;	//we have to calc this on every frame :(
_ZPoly *the_polys;
//int	points_offset;	//offset to points for the polys in the object
//float view_angle;
int poly_count;
int	how_many_polys;
//_3D poly_plane;
//int the_object=0;
//int frontface;
//_3D *world_points;
textured_connections *pconnections;
_3D p1,p2,p3;	//our three points and the z point for gourad
_3D *this_object_points;
int conn,D,object_count,active_object_count;
double average_z,light_value;
int poly_type;	//flat, gourad or texture mapped
int light_amplitude, self_lit;	//new lights 101200 - SB
extern int zsort_count, black_level;
register ZObject * current_object_ptr;
bool any_point_in_front;

zsort_index=0;
reset_zsort_insert();

//dynamic_object_index=1;
number_of_polys_displayed=0;
visible_points_list_ptr=visible_points_list_base;	//we output visible points here

object_count=active_object_count=0;
while (active_object_count<dynamic_object_index)
{
  current_object_ptr=&ocb_ptr->object_list[object_count];
  if (current_object_ptr->in_use==1)
  {
   if (current_object_ptr->is_visible==1 && current_object_ptr->eng_invisible==0)
    {
//Distance to the object
//     distance_to_object=get_distance_to_from(the_object,0);

//point to rotated world points
  this_object_points=current_object_ptr->world_points_ptr;	//the points for this object
//  this_object_object_points=(*ocb_ptr).object_list[object_count].points_ptr;	  //the points for this object

  if(current_object_ptr->is_blobbed==1)
  how_many_polys=1;	//THE mother of all optimisations!
  else
  how_many_polys=current_object_ptr->number_of_polys;
  the_polys=current_object_ptr->polys_ptr;


//calculate dynamic light normal 101200 Stu
//in lighting.c

   if( ((current_object_ptr->Oid)&0xff) == '&' )
   self_lit=1;
   else
   {
    self_lit=0;
    calculate_light_normal(object_count,&light_normal, &light_amplitude);	
   }

  
  for (poly_count=0;poly_count<how_many_polys;poly_count++)
  {
//     vis=1;
	 
   if( self_lit==0 /*&& any_point_in_front */)
   {

        light_value=(light_normal.x*(*the_polys).face_normal_translated.x)+
                    (light_normal.y*(*the_polys).face_normal_translated.y)+
                    (light_normal.z*(*the_polys).face_normal_translated.z);
	
        light_value*=light_amplitude;
		
		//black level clamping
		if (light_value<black_level) light_value=black_level;
		
        (*the_polys).Plight_value=STATIC_CAST_TO_INT(light_value);
    }
    else
    {
      (*the_polys).Plight_value=32760;  // self lighting objects - stars typically
    }


    pconnections=(*the_polys).poly_connections_ptr;	
    poly_type=(*pconnections).type;	//flat, gourad or textured

    conn=(*pconnections).the_node;
    pconnections++;
    p1=(*(this_object_points+conn));
    
    conn=(*pconnections).the_node;
    pconnections++;
    p2=(*(this_object_points+conn));
    
    conn=(*pconnections).the_node;	//e.g. 1 to 2, 2 to 3
    pconnections++;
    p3=(*(this_object_points+conn));
  
any_point_in_front=FALSE;

if ((p1.z>0 || p2.z>0 || p3.z>0)) any_point_in_front=TRUE;

//D=-x1*(y2*z3-y3*z2)-x2*(y3*z1-y1*z3)-x3*(y1*z2-y2*z1);
//calc poly lighting
//looks like a lot of maths, but is comparable to rotating the surface normal (3 muls less actually!) 
//Calc normal relative to camera
    D=STATIC_CAST_TO_INT(-p1.x*(p2.y*p3.z-p3.y*p2.z) - p2.x*(p3.y*p1.z-p1.y*p3.z) - p3.x*(p1.y*p2.z-p2.y*p1.z));
//check if poly is in frustum   

 #ifndef RIGHT_HANDED 
		#error "RIGHT_HANDED not defined!"
 #endif
 #if RIGHT_HANDED==1
    if (poly_count==0) D=1; //First poly ALWAYS visible as planet label is tagged to it :)
    if(!backface_cull) D=1;	//stutest
    if (any_point_in_front && (D>0 || (*the_polys).polyflags&DUST_BIT) )	//the poly is visible or is blobbed
 #else
    if (poly_count==0) D=-1;
    if(!backface_cull) D=-1;	//stutest
    if (any_point_in_front && (D<0 || (*the_polys).polyflags&DUST_BIT) )	//the poly is visible or is blobbed
 #endif
 
	{       
       
         average_z=p1.z+p2.z+p3.z;	//for debugging
         (*the_polys).average_z=average_z/3;	//needed fro z sort
         (*the_polys).PPoints_offset=number_of_polys_displayed; //points offset for renderer
	  insert_to_z_buffer(the_polys);
          zsort_count++;
         if (zsort_count>MAX_NUMBER_OF_POLYS-2)
       		report_error("Too many visible triags (Do_Culling2).","\p",zsort_count);

         (*visible_points_list_ptr).poly_ptr=the_polys;
         (*visible_points_list_ptr).point1=p1;	//becomes input points in pers
         (*visible_points_list_ptr).point2=p2;
         (*visible_points_list_ptr).point3=p3;
         visible_points_list_ptr++;	//ready for next visible poly
         (*the_polys).p_visible=1;	//for gourad if so
         number_of_polys_displayed+=1;

      }	//D<0 etc
     else	//not visiblew
     {
       pconnections-=3;	//back to first connection
       

//       if ((*pconnections).type&GOURAD)
//        {
//        light_value=(light_normal.x*(*the_polys).face_normal_translated.x)+(light_normal.y*(*the_polys).face_normal_translated.y)+(light_normal.z*(*the_polys).face_normal_translated.z);
//	  light_value*=light_amplitude;
//    (*the_polys).Plight_value=255;	  //light_value;
//        }
        
      (*the_polys).p_visible=0;
     }
//    } //end of visible poly
	
	
   
    the_polys+=1;	//(*ocb_ptr).object_list[0].polys_ptr;


    }	 //end of poly loop
    }	//end of if vis
   active_object_count++;
   } //end of if on
   object_count++;
  }	//next object - end of while
  
//now fast objects
  
  
{
ZFObject* current_object_ptr;

object_count=active_object_count=0;
while (active_object_count<Fdynamic_object_index)
{
  current_object_ptr=&Focb_ptr->Fobject_list[object_count];
  if (current_object_ptr->in_use==1)
  {
   if (current_object_ptr->is_visible==1 )
    {
    
//point to rotated world points
  this_object_points=current_object_ptr->world_points_ptr;	//the points for this object
//  this_object_object_points=(*ocb_ptr).object_list[object_count].points_ptr;	  //the points for this object

  how_many_polys=current_object_ptr->number_of_polys;
  the_polys=current_object_ptr->polys_ptr;

//calculate dynamic light normal 101200 Stu
//in lighting.c
//All fast objects are self lit
/*
   if( ((current_object_ptr->Oid)&0xff) == '&' )
   self_lit=1;
   else
   {
    self_lit=0;
    calculate_light_normal(object_count,&light_normal, &light_amplitude);	
   }
*/

  
  for (poly_count=0;poly_count<how_many_polys;poly_count++)
  {
	 
/*
   if( self_lit==0 )
   {

        light_value=(light_normal.x*(*the_polys).face_normal_translated.x)+
                    (light_normal.y*(*the_polys).face_normal_translated.y)+
                    (light_normal.z*(*the_polys).face_normal_translated.z);
	
        light_value*=light_amplitude;
		
		//black level clamping
		if (light_value<black_level) light_value=black_level;
		
        (*the_polys).Plight_value=light_value;
    }
    else
    {
*/
      (*the_polys).Plight_value=42760;  // self lighting objects - stars typically
//    }


  
    pconnections=(*the_polys).poly_connections_ptr;	
    poly_type=(*pconnections).type;	//flat, gourad or textured

    conn=(*pconnections).the_node;
    pconnections++;
    p1=(*(this_object_points+conn));
    
    conn=(*pconnections).the_node;
    pconnections++;
    p2=(*(this_object_points+conn));
    
    conn=(*pconnections).the_node;	//e.g. 1 to 2, 2 to 3
    pconnections++;
    p3=(*(this_object_points+conn));

//D=-x1*(y2*z3-y3*z2)-x2*(y3*z1-y1*z3)-x3*(y1*z2-y2*z1);
//calc poly lighting
//looks like a lot of maths, but is comparable to rotating the surface normal (3 muls less actually!) 
//Calc normal relative to camera
//    D=-p1.x*(p2.y*p3.z-p3.y*p2.z) - p2.x*(p3.y*p1.z-p1.y*p3.z) - p3.x*(p1.y*p2.z-p2.y*p1.z);
    
//    if(!backface_cull) D=-1;	//stutest
//    D=-1;
	  
//    if (D<0 )	//the poly is visible or is blobbed
     {       
       
         average_z=p1.z+p2.z+p3.z;	//for debugging
         (*the_polys).average_z=average_z /*/3*/;	//needed fro z sort
         (*the_polys).PPoints_offset=number_of_polys_displayed; //points offset for renderer
	  insert_to_z_buffer(the_polys);
          zsort_count++;
         if (zsort_count>MAX_NUMBER_OF_POLYS-2)
       		report_error("Too many visible triags (Do_Culling2).","\p",zsort_count);

         (*visible_points_list_ptr).poly_ptr=the_polys;
         (*visible_points_list_ptr).point1=p1;	//becomes input points in pers
         (*visible_points_list_ptr).point2=p2;
         (*visible_points_list_ptr).point3=p3;
         visible_points_list_ptr++;	//ready for next visible poly
         (*the_polys).p_visible=1;	//for gourad if so
         number_of_polys_displayed+=1;

      }	//D<0 etc
/*
     else	//not visiblew
     {
       pconnections-=3;	//back to first connection
      (*the_polys).p_visible=0;
     }
*/

    the_polys+=1;	//(*ocb_ptr).object_list[0].polys_ptr;
    }	 //end of poly loop
    }	//end of if vis
   active_object_count++;
   } //end of if on
   object_count++;
  }	//next object - end of while


} 
//end of fast objects  
  
  
  

//var_a=zsort_count;

do_gourad_polys();

do_sun_lens_flares();

}

// **********************END OF MAIN 3D PROCESSING ************************

extern int game_running;
extern int sun_slot1;
extern int sun_slot2;
int flare_brightness;
int done_corona_message;

double light_dist1, light_dist2;

void do_sun_lens_flares(void)
{
ZObject *source_object1;
ZObject *source_object2=0;

//_3D *world_points_ptr;
_2Dp sun_coord1, sun_coord2;
extern int Goutside;
//_3D vd;
int ulength;
int flength;
int fx,fy;	
                                // BIG NOTE
int binary_suns;		// currently binaries are only handled for lense FLARES not for screen  
                                // brightness or more importantly corona....
 if (Goutside==0) return;
 
 if (game_running==1)
 {

 int flare_radius;
 
 
 #if TEST_SYSTEM==1
 return;
 #endif

 
//wot this bit of code does is:
//1. Draws a lens flare if sun is in front of player.
//2. Whites out the screen depending on sun position.
//3. Attenuates both of the above depending on distance.
//4. No way crashes if you get too close to the sun.
//5. Could handle two light source but currently only does one....
//flare brightness is taken into account in drawing.c
 
    //calc suns 2d position
	source_object1=&(*ocb_ptr).object_list[sun_slot1];	 //vv object
	
        sun_coord1=get_screen_center_coord_with_bflag(sun_slot1);
        if(sun_slot2==-1) { binary_suns=0; } else { binary_suns=1; }
//        binary_suns=0;
        
        if(binary_suns) { source_object2=&(*ocb_ptr).object_list[sun_slot2];
                          sun_coord2=get_screen_center_coord_with_bflag(sun_slot2); }

	#define MINIMUM_FLARE_RADIUS 160	//at maximum attenuation is 160 screen pixels
	 #define FLARE_STEPS 20			//how many steps to white out the screen
	 #define MAX_LIGHT_DISTANCE 50000000000000.0  //30000000	//minimum attenuation at this range (Zex units)
                                                                // increased from 100 million km to 500 million km - more realistic
	 //get distance to sun
	 light_dist1=get_distance_to_zex(sun_slot1);
	 light_dist1-=(*source_object1).radius;
	 if(binary_suns) { light_dist2=get_distance_to_zex(sun_slot2);
	                   light_dist2-=(*source_object2).radius;  }
	 
         if (light_dist1>MAX_LIGHT_DISTANCE) light_dist1=MAX_LIGHT_DISTANCE;
         if (light_dist2>MAX_LIGHT_DISTANCE) light_dist2=MAX_LIGHT_DISTANCE;
         
         if (light_dist1<10000000000.0)
         {
            if( done_corona_message==0)
            {
             add_to_text_display("OBC> Danger. Entering Star's corona",DLP_RED); 
             done_corona_message=1;
             play_zsound(danger, sound_low_pri, speech_chan, sound_vol_7);
            fade_and_play_zex_resource(132);	//main theme - starshine by Purple Motion

            }
         }
         else 
         {
           if (done_corona_message==1)
           {
           add_to_text_display("OBC> Cleared corona",DLP_GREEN); 
           done_corona_message=0;
           fade_out_music();
          }
         }
         
         light_dist1=MAX_LIGHT_DISTANCE-light_dist1;	 //invert - minimum brightness does not decrease > 10M	         
         //get in range of 0 to 20 - attenuation resolution
         light_dist1/=(MAX_LIGHT_DISTANCE/50);
         
         if(binary_suns)
           {
           light_dist2=MAX_LIGHT_DISTANCE-light_dist2;	 //invert - minimum brightness does not decrease > 10M	         
           //get in range of 0 to 20 - attenuation resolution
           light_dist2/=(MAX_LIGHT_DISTANCE/50);
           }
           
         	//calc screen brightness
         	//max distance is 320
         	fx=  abs_nobranch (screen_cent_x-sun_coord1.x);
         	fy= abs_nobranch  (screen_cent_y-sun_coord1.y);
         	flare_radius=STATIC_CAST_TO_INT(MINIMUM_FLARE_RADIUS+(light_dist1));
         	flength=STATIC_CAST_TO_INT(sqrt(fx*fx+fy*fy));
         	ulength=flength;
         	if (ulength>flare_radius) ulength=flare_radius;
                flare_brightness=STATIC_CAST_TO_INT(light_dist1);	// *1.9; 	 //Adjust this line for general glare at distance
                //flare_brightness/=3;	//at max attenuation

        if ( sun_coord1.behind==0 )	//only draw frare if in front
         {
        	if (light_dist1>4)
        	{
        	//calc sun center
        	add_to_dlp(DRAW_OUTER_FLARE,sun_coord1.x, sun_coord1.y, STATIC_CAST_TO_INT(light_dist1), 1,0,0,0,0,0);
         	}
         	flare_brightness+=FLARE_STEPS-(ulength/(flare_radius/FLARE_STEPS));	//adjust this line for glare at angle
         	
         }



if(binary_suns)
  {
        if ( sun_coord2.behind==0 )	//only draw frare if in front
         {
        	if (light_dist2>4)
        	{
        	//calc sun center
        	add_to_dlp(DRAW_OUTER_FLARE,sun_coord2.x, sun_coord2.y, STATIC_CAST_TO_INT(light_dist2), 1,0,0,0,0,0);
         	}
//         	flare_brightness+=FLARE_STEPS-(ulength/(flare_radius/FLARE_STEPS));	//adjust this line for glare at angle
         	
         }

}
 

//         }
//	else flare_brightness=0;
 }
}


int gourad_polys;
//runs through the visible polys setting the light values for gouraded polys
void do_gourad_polys()
{
extern int dynamic_object_index;
int active_object_counter,object_counter;
int how_many_polys,poly_count;
textured_connections *pconnections;
_ZPoly *the_polys;
short poly_type;
int gourad_light;
int touching_count,i,touching_poly;
//int light_value;
//int light_amplitude;	//new lights 101200 SB
_ZPoly *temp_poly;
register ZObject * current_object_ptr;
//_3D vertex_av;

//extern UInt32 gCannon;


        gourad_polys=0;
//int debug_loop_count=0;
//now if gourad, we calc the light value of the polys points via it's neighbours and this (averaging) 
//(some from last frame, some from this no doubt but it should be OK) 
active_object_counter=0;
object_counter=0;
while (active_object_counter<dynamic_object_index)
{
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("\pdo_gourad_polys looped");
     current_object_ptr=&ocb_ptr->object_list[object_counter];


//The fault is somewhere in this code!
 if (current_object_ptr->in_use==1)
  {
   if (current_object_ptr->is_visible==1)
    {
	
//run through the polys of this object
    the_polys=current_object_ptr->polys_ptr;



    if(current_object_ptr->is_blobbed==1)
    {
      how_many_polys=1;	//THE mother of all optimisations!
    }
    else
    {
      how_many_polys=current_object_ptr->number_of_polys;    
      for (poly_count=0;poly_count<how_many_polys;poly_count++)
      { 
        pconnections=(*the_polys).poly_connections_ptr;	
        poly_type=(*pconnections).type;	//flat, gourad or textured
  
         if (poly_type&GOURAD  && (*the_polys).p_visible==1)
         {
          gourad_polys++;	
          touching_count=1;
  
          gourad_light=(*the_polys).Plight_value;
          	  
          for (i=0;i<MAX_TOUCHING_POLYS;i++)
           {
             if((*the_polys).Ptouching1[i]!=-1)
              {
               touching_poly=(*the_polys).Ptouching1[i];
               temp_poly=current_object_ptr->polys_ptr;	//point to polys for object
               temp_poly+=touching_poly;	//point to poly that is touching this point
  
               gourad_light+=(*temp_poly).Plight_value;
               
               touching_count++;
             }
           }
     
  	  gourad_light/=touching_count;
  
         if (gourad_light<-32000) gourad_light=-32000;
         else
         if (gourad_light>32000) gourad_light=32000;
         (*the_polys).Pvertex_light[0]=gourad_light;
  
     
  //calc p2
         touching_count=1;
          gourad_light=(*the_polys).Plight_value;
         for (i=0;i<MAX_TOUCHING_POLYS;i++)
           {
            if((*the_polys).Ptouching2[i]!=-1)
             {
              touching_poly=(*the_polys).Ptouching2[i];
              temp_poly=current_object_ptr->polys_ptr;	  //point to polys for object
              temp_poly+=touching_poly;	//point to poly that is touching this point
              gourad_light+=(*temp_poly).Plight_value;
              touching_count++;
             }
           }
      
  	  gourad_light/=touching_count;
  
         if (gourad_light<-32000) gourad_light=-32000;
         else
         if (gourad_light>32000) gourad_light=32000;
         (*the_polys).Pvertex_light[1]=gourad_light;
     
  //calc p3
         touching_count=1;
          gourad_light=(*the_polys).Plight_value;
         for (i=0;i<MAX_TOUCHING_POLYS;i++)
         {
          if((*the_polys).Ptouching3[i]!=-1)
           {
            touching_poly=(*the_polys).Ptouching3[i];
            temp_poly=current_object_ptr->polys_ptr;	  //point to polys for object
            temp_poly+=touching_poly;	  //point to poly that is touching this point
            gourad_light+=(*temp_poly).Plight_value;
            touching_count++;
           }
          }
      
  	  gourad_light/=touching_count;
  
         if (gourad_light<-32000) gourad_light=-32000;
         else
         if (gourad_light>32000) gourad_light=32000;
          (*the_polys).Pvertex_light[2]=gourad_light;
         }	 //end of gourad poly
       the_polys+=1;	 //(*ocb_ptr).object_list[0].polys_ptr;
  
       }	//for loop next poly
    
    } //end of is not blobbed
    
   } //end of if visible
   active_object_counter++;
    
  } //end of if in_use
 object_counter++;

 //е check next object visible
 }	//end of while

}




//inserts the poly into a linked list. Position depends on average z - we want the furthest (greatest)
//at the top of the list and the furthest (least) at the  end
//SB 16/12/98
void insert_to_z_buffer(_ZPoly *the_poly)
{
extern _ZPolySort zsort_list[MAX_NUMBER_OF_POLYS];	//max of 2000 polys
//extern int zsort_count;
extern int zbuffer_next_free;
extern int zsort_start_index;	//holds index of start of list.
//extern double last_z;
_ZPoly *current_poly;
register double the_z;
register int current_index,inserted,previous_slot,was_pointing_to;
//register _ZPolySort * zbuff_next_free_ptr;


the_z=(*the_poly).average_z;
current_index=zsort_start_index;	//top of the list - always max z
inserted=0;
previous_slot=-1;
//find out where this z fits in the list
//first see if it's greater than the start value!



 current_poly=zsort_list[current_index].poly_data;
 if (current_poly!=0)	//prevent those annoying access exceptions..

 {
  
  if (the_z>zsort_list[current_index].z_value)
  {
  //insert poly
  
  zsort_list[zbuffer_next_free].previous_slot=-1;	//new head of list
  zsort_list[zbuffer_next_free].next_slot=zsort_start_index;	//point to next, which is old start
  zsort_list[zsort_start_index].previous_slot=zbuffer_next_free;	//patch old start to previous this new one

  zsort_list[zbuffer_next_free].poly_data=the_poly;
  zsort_list[zbuffer_next_free].z_value=the_z;	//store z
  zsort_start_index=zbuffer_next_free;	//new start
  zbuffer_next_free++;
  inserted=1;	
  zsort_list[zbuffer_next_free].poly_data=0; //make end of list

  }
 }	//end of poly!=0;
 
//search loop
while(inserted==0)
 {
 //check if we have have reached the end of the line
 if (zsort_list[current_index].poly_data==0)
  {
  zsort_list[current_index].poly_data=the_poly;
  zsort_list[current_index].previous_slot=previous_slot;
  zsort_list[current_index].next_slot=zbuffer_next_free;
  zsort_list[current_index].z_value=the_z;	//store z
  zsort_list[zbuffer_next_free].poly_data=0;	//make end of list
  zbuffer_next_free++;
  inserted=1;
  }
 else //if not end of list

  {

  //see if our z is > than the current z 
  //if so this is where we need to insert
  if (the_z>zsort_list[current_index].z_value)
    {
    //insert our poly
    //patch previous to point to our new
    previous_slot=zsort_list[current_index].previous_slot;
    was_pointing_to=zsort_list[previous_slot].next_slot;
    zsort_list[previous_slot].next_slot=zbuffer_next_free;	//make previous point to our new one
    
    //insert new
    zsort_list[zbuffer_next_free].poly_data=the_poly;
    zsort_list[zbuffer_next_free].previous_slot=previous_slot;	//tie this one back to old
    zsort_list[zbuffer_next_free].next_slot=was_pointing_to;	//and our new one points to the old next
    zsort_list[zbuffer_next_free].z_value=the_z;	//store z
    zsort_list[was_pointing_to].previous_slot=zbuffer_next_free;	//follower points to new
    zbuffer_next_free++;
    zsort_list[zbuffer_next_free].poly_data=0;	//make end of list
    inserted=1;
   }
  else	//follow the chain
    {
    previous_slot=current_index;
    current_index=zsort_list[current_index].next_slot;
    }
  }

 } //end of while inserted==0
// last_z=the_z;	//save for next run in-case they're the same
}

//ееroutine resets the z buffer insert
void reset_zsort_insert()
{
extern int zsort_start_index,zsort_count,zbuffer_next_free;
extern _ZPolySort zsort_list[MAX_NUMBER_OF_POLYS];	//max of 2000 polys
zbuffer_next_free=1;	//next free slot
zsort_start_index=0;	//used in insert_to_z_buffer - slot of the start of the list  - max z
zsort_count=0;	//counter of items inserted
//clear top of list
zsort_list[0].previous_slot=-1;	//indicate start of list
zsort_list[0].next_slot=-1;	//indicate end of list
zsort_list[0].poly_data=0;
}
//еееееееееееееееееееее


//еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
//Perspective version 3 - SB 130401
//produces a list of z-clipped and perspectively transformed polys in clipped_output_ptr
//and number_of_clipped_polys so drawing can draw 'em

//float depth_of_field=400;	//400 is good, larger is longer lens; smaller is shorter lens; 200 is fisheye
//#define depth_of_field 1000	  //400 is good, larger is longer lens; smaller is shorter lens; 200 is fisheye
//600=56 degrees H and 44 degrees v

#define depth_of_field_default 1000

double depth_of_field=depth_of_field_default;		// initialiser is just in case init never runs...

clipped_polys_drawing_info_type clipped_polys_drawing_info;

int number_of_clipped_polys;
int use_zbuffer_flag;
void do_pers3(int width, int height)
//this is the rwritten version used in conjunction with do_culling2.
{
//extern _2DPoly *view_points_list_ptr;
extern visible_poly *visible_points_list_base;
//extern int OGL_active;

//extern	int	total_points_counter;
#define TO ,

//visible_poly *input_points;
//_2DPoly *view_points;
double pz,pdiv,ooz;
//int i;
float center_x,center_y;
//double fcenter_x, fcenter_y;
_clipped_poly* clipped_output_ptr;

int poly_index;
_ZPoly *in_poly;

//extern float zbuffer_range;
float zdepth;
int zbuff_on_poly=0;
short object_owner;
double obj_depth;


zbuffer_divisor=0;


number_of_clipped_polys=0;

center_x=width/2;
center_y=height/2;

//input_points = visible_points_list_base;	  //from culling
//view_points=view_points_list_ptr;
pdiv=depth_of_field;

//here we are transforming from three points per poly in input points
//to three or four points per poly in view_points
clipped_output_ptr=clipped_polys_ptr;
clipped_polys_drawing_info.clipped_polys_ptr=clipped_polys_ptr;
clipped_polys_drawing_info.switch_zbuffer_on_at_poly_number=-1;

//run through zsort_list, processing the polys to clipped_output
poly_index=zsort_start_index;

object_owner=-1;

in_poly=zsort_list[poly_index].poly_data;	//get us going

use_zbuffer_flag=0;
zdepth=0;

while (in_poly!=0)
{    
    in_poly=zsort_list[poly_index].poly_data;	//this poly
    if (in_poly!=0)	//prevent those pesky access faults
    {
       poly_index=zsort_list[poly_index].next_slot;	//next poly
  

       //here we send three input points to the z clipper and the address of a clipstruct
       //the z clipper fills this with either three or four points. We then perspective transform them
   
       if (object_owner!=in_poly->owner)	//new object, so note poly number
       {
        object_owner=in_poly->owner;
        zbuff_on_poly=number_of_clipped_polys;
       }

       z_clip(in_poly, visible_points_list_base, clipped_output_ptr);

       //new perspective (version 3, combines the field of view with the reciprocal of the z to 
       //replace one multiply and one divide with one multiply per vertex)
       //transform vert1,2 and 3 to x1,y1,x2,y2 etx
       // pz=(*input_points).point1.z;
       
       //check if we need to switch on the z buffer
       
        obj_depth=clipped_output_ptr->average_z+(*ocb_ptr).object_list[clipped_output_ptr->owner].radius;
        
        if(use_zbuffer!=0 && use_zbuffer_flag==0 && obj_depth<200000*2 && obj_depth>0)	//only do this once per frame (Stu!)
        {
        
         use_zbuffer_flag=1;
         //280401 - goota find the furthest z point on this object and set z_divisor and poly number 
                           
         //calc furthest point of this object which is our zbuffer divisor for 0-1
          clipped_polys_drawing_info.z_divisor_for_zbuffer=obj_depth;	//clipped_output_ptr->average_z + radius;

         
         //switch z buffer on at poly #                                                 
         clipped_polys_drawing_info.switch_zbuffer_on_at_poly_number=zbuff_on_poly;
         zbuffer_divisor=STATIC_CAST_TO_INT(clipped_polys_drawing_info.z_divisor_for_zbuffer);
         
        }

       pz=clipped_output_ptr->vertices[0].z;
              
       ooz=1.0/pz;	//divide by z
       ooz*=pdiv;	//times field of view
       clipped_output_ptr->screen_pts[0].x=center_x+(clipped_output_ptr->vertices[0].x*ooz);
       clipped_output_ptr->screen_pts[0].y=center_y-(clipped_output_ptr->vertices[0].y*ooz);
       
       
       pz=clipped_output_ptr->vertices[1].z;
       ooz=1.0/pz;
       ooz*=pdiv;
       clipped_output_ptr->screen_pts[1].x=center_x+(clipped_output_ptr->vertices[1].x*ooz);
       clipped_output_ptr->screen_pts[1].y=center_y-(clipped_output_ptr->vertices[1].y*ooz);
       
       pz=clipped_output_ptr->vertices[2].z;
       ooz=1.0/pz; 
       ooz*=pdiv;
       clipped_output_ptr->screen_pts[2].x=center_x+(clipped_output_ptr->vertices[2].x*ooz);
       clipped_output_ptr->screen_pts[2].y=center_y-(clipped_output_ptr->vertices[2].y*ooz);
       
       //check for fourth vertex out of zclip
       if(clipped_output_ptr->number_of_verts==4)
       {
       pz=clipped_output_ptr->vertices[3].z;
       ooz=1.0/pz; 
       ooz*=pdiv;
       clipped_output_ptr->screen_pts[3].x=center_x+(clipped_output_ptr->vertices[3].x*ooz);
       clipped_output_ptr->screen_pts[3].y=center_y-(clipped_output_ptr->vertices[3].y*ooz);
       }
      
       //
       clipped_output_ptr++;	//point to next clipped_poly storage
       number_of_clipped_polys++;

       if (number_of_clipped_polys==MAX_NUMBER_OF_CLIPPED_POLYS)
             		      report_error("Too many clipped polys (do_pers3).","\p",number_of_clipped_polys);



    }
 
 }	//end of the_poly_info
 
clipped_polys_drawing_info.number_of_clipped_polys_to_draw=number_of_clipped_polys;
 return;
 
 
}





//coming into this routine we have a poly with a fixed three vertices.
//going out we have a _clipped_poly containing up to 4 z clipped vertices
void z_clip(_ZPoly* input_poly, visible_poly* input_points, _clipped_poly* output_poly)
{
textured_connections *pconnections;
int vertex_index;
_3D V1,V2,V3;
int L1,L2,L3;
texture_coord T1,T2,T3;

//this looks like we're going to slow down, but it saves the back end doing various
//indirections multiple times.

output_poly->Plight_value=input_poly->Plight_value;
output_poly->OGL_name=input_poly->Pid;
output_poly->draw_code=input_poly->draw_code;
output_poly->polyflags=input_poly->polyflags;
output_poly->owner=input_poly->owner;
output_poly->Prgb=input_poly->Prgb;
output_poly->average_z=input_poly->average_z;


//stuff from the connections
//get type
pconnections=input_poly->poly_connections_ptr; 
output_poly->type=(*pconnections).type;
output_poly->transparency=(*pconnections).transparency;


//do the z clipping
vertex_index=0;

input_points+=input_poly->PPoints_offset;

//points 1 to 2
V1=input_points->point1;
V2=input_points->point2;
V3=input_points->point3;

L1=input_poly->Pvertex_light[0];
L2=input_poly->Pvertex_light[1];
L3=input_poly->Pvertex_light[2];

T1.u=(*pconnections).u;
T1.v=(*pconnections).v;
pconnections++;
T2.u=(*pconnections).u;
T2.v=(*pconnections).v;
pconnections++;
T3.u=(*pconnections).u;
T3.v=(*pconnections).v;


#define RUN_ZCLIP 1

#if RUN_ZCLIP
vertex_index+=front_clip(output_poly,vertex_index,V1,V2,L1,L2,T1,T2);
vertex_index+=front_clip(output_poly,vertex_index,V2,V3,L2,L3,T2,T3);
vertex_index+=front_clip(output_poly,vertex_index,V3,V1,L3,L1,T3,T1);
#else
output_poly->vertices[0]=V1;
output_poly->vertices[1]=V2;
output_poly->vertices[2]=V3;

output_poly->texture_pts[0]=T1;
output_poly->texture_pts[1]=T2;
output_poly->texture_pts[2]=T3;

output_poly->Pvertex_light[0]=L1;
output_poly->Pvertex_light[1]=L2;
output_poly->Pvertex_light[2]=L3;


vertex_index=3;
#endif

output_poly->number_of_verts=vertex_index;


}

//this is a Z clipper - it returns the number of vertices inserted into _clipped_poly
//which is added to vertex_index for the next call
//   #define ZMin depth_of_field-200	  //dont change this stu! Its important

#define ZMin_default (depth_of_field_default-200)

double ZMin = ZMin_default;


int front_clip(_clipped_poly* output_poly,int vertex_index, _3D V1, _3D V2,
               int L1, int L2, texture_coord T1, texture_coord T2)
{
   double dold,dnew, m=1;
   
   //both points in front of camera
   if ( (V1.z>=ZMin) && (V2.z>=ZMin) )
   {
    //copy v2 data
    output_poly->vertices[vertex_index]=V2;
    output_poly->Pvertex_light[vertex_index]=L2;
    output_poly->texture_pts[vertex_index]=T2;
      return 1;
   }
  
   dold=V2.z-V1.z; dnew=ZMin-V1.z;	//calc slope
   if (dold) m=dnew/dold;
   
//leaving the viewport
   if ( (V1.z>=ZMin) && (V2.z<ZMin) )
   {
      output_poly->vertices[vertex_index].x=V1.x + (V2.x-V1.x)*m;
      output_poly->vertices[vertex_index].y=V1.y + (V2.y-V1.y)*m;
      output_poly->vertices[vertex_index].z=depth_of_field;
      
      output_poly->Pvertex_light[vertex_index]=L1;
      output_poly->texture_pts[vertex_index]=T1;
      
      return 1;
   }

//entering the viewport: 


   if ( (V1.z<ZMin) && (V2.z>=ZMin) )
   {
      output_poly->vertices[vertex_index].x=V1.x + (V2.x-V1.x)*m;
      output_poly->vertices[vertex_index].y=V1.y + (V2.y-V1.y)*m;
      output_poly->vertices[vertex_index].z=depth_of_field;
      output_poly->vertices[vertex_index+1]=V2;

      output_poly->Pvertex_light[vertex_index]=L1;
      output_poly->Pvertex_light[vertex_index+1]=L2;
      
      output_poly->texture_pts[vertex_index]=T1;
      output_poly->texture_pts[vertex_index+1]=T2;
      
      return 2;
   }

//take care of default both points behind camera
return 0;	//both points are behind
}



//
//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
//calculate face normal as
//               Xn:=(Z2-Z1)*(Y3-Y1)-(Y2-Y1)*(Z3-Z1);
//               Yn:=(Z2-Z1)*(X3-X1)-(X2-X1)*(Z3-Z1);
//               Zn:=(Y2-Y1)*(X3-X1)-(X2-X1)*(Y3-Y1);
//               { Now Normalize the normals }
//               Module:=Sqrt(Sqr(Xn)+Sqr(Yn)+Sqr(Zn));
//               Xn:=Xn/Module;
//               Yn:=Yn/Module;
//               Zn:=Zn/Module;
	//calculate a poly normal in static objects
	

void calc_normal(int the_object, int poly_select)
{
extern	StatObjectsFixedSize *ocb_Sptr;
textured_connections *pconnections;
int	conn;	//offset to points for the polys in the object
_3D p1,p2,p3;	//our three points
_3D *world_points;
_3D point_normal;
_ZPoly *the_polys;
_ZPoly *the_poly;
double Module;	//normalising normals
//float test_point;	//used for calcing poly point normals for Gourad.
//vector np;

the_polys=(*ocb_Sptr).object_list[the_object].polys_ptr;	//pointing to static definition
the_poly=the_polys+poly_select;

pconnections=(*the_poly).poly_connections_ptr;	

world_points=(*ocb_Sptr).object_list[the_object].points_ptr;	//the points for this object
conn=(*pconnections).the_node;
if ((*pconnections).type==1) (*pconnections).u=-32767;	//frig for gourad test - remove
    pconnections++;

  p1=(*(world_points+conn));
conn=(*pconnections).the_node;
if ((*pconnections).type==1) (*pconnections).u=0;	//frig for gourad test - remove
    pconnections++;
  p2=(*(world_points+conn));
conn=(*pconnections).the_node;	//e.g. 1 to 2, 2 to 3
if ((*pconnections).type==1) (*pconnections).u=32767;
    pconnections++;
  p3=(*(world_points+conn));

//get the normals
point_normal.x=(p2.z-p1.z)*(p3.y-p1.y)-(p2.y-p1.y)*(p3.z-p1.z);
point_normal.y=(p2.z-p1.z)*(p3.x-p1.x)-(p2.x-p1.x)*(p3.z-p1.z);
point_normal.z=(p2.y-p1.y)*(p3.x-p1.x)-(p2.x-p1.x)*(p3.y-p1.y);

//point_normal.x=(p3.z-p1.z)*(p2.y-p1.y)-(p3.y-p1.y)*(p2.z-p1.z);
//point_normal.y=(p3.z-p1.z)*(p2.x-p1.x)-(p3.x-p1.x)*(p2.z-p1.z);
//point_normal.z=(p3.y-p1.y)*(p2.x-p1.x)-(p3.x-p1.x)*(p2.y-p1.y);

//normalize the normals
Module=sqrt(point_normal.x*point_normal.x+point_normal.y*point_normal.y+point_normal.z*point_normal.z);
//store normal vector (poly 0,0,0 to x,y,z where 1.0=max)



#ifndef RIGHT_HANDED 
  			  #error "RIGHT_HANDED not defined!"
#endif
#if RIGHT_HANDED==0
 (*the_poly).face_normal.x=point_normal.x/Module;	//this is working in the static def
 (*the_poly).face_normal.y=-point_normal.y/Module;
 (*the_poly).face_normal.z=point_normal.z/Module;	//TaaDaah!
#else
 (*the_poly).face_normal.x=-point_normal.x/Module;	//this is working in the static def
 (*the_poly).face_normal.y=point_normal.y/Module;
 (*the_poly).face_normal.z=-point_normal.z/Module;	//TaaDaah!
#endif

}



// get a random point from an object, simply as a place holder
_2D get_any_screen_coord(int object_slot)
{
_3D *this_object_points;
double x,y,z;

     this_object_points=(*ocb_ptr).object_list[object_slot].world_points_ptr;	//the points for this object

     x=(*this_object_points).x;
     y=(*this_object_points).y;
     z=(*this_object_points).z;
     
     return point_to_screen(x,y,z,monitor_w,monitor_h);
//   }

	
}




//only call during game as it assumes a fixed window size
//returns the 2d point of the center of the object
_2Dp get_screen_center_coord_with_bflag(int object_slot)
{

//_ZPoly *the_polys;
//textured_connections *pconnections;
_3D *this_object_points;
_2Dp return_val;
_2D return1;

int i,number_of_points;
double x,y,z;
     if ((*ocb_ptr).object_list[object_slot].is_visible==0)
     {
      return_val.x=0; return_val.y=0; return_val.behind=1;
      return return_val;
     }
//find the center of the object
     this_object_points=(*ocb_ptr).object_list[object_slot].world_points_ptr;	//the points for this object
     x=y=z=0;
     number_of_points=(*ocb_ptr).object_list[object_slot].number_of_points;
     for (i=0;i<number_of_points;i++)
     {
       x+=(*this_object_points).x;
       y+=(*this_object_points).y;
       z+=(*this_object_points).z;
       this_object_points++;
     }
     x/=number_of_points;
     y/=number_of_points;
     z/=number_of_points;
     

     return1 = point_to_screen(x,y,z,monitor_w,monitor_h);
     return_val.x=return1.x;
     return_val.y=return1.y;
     if (z<0) return_val.behind=1;
     else return_val.behind=0;
     
     return return_val;
}


//only call during game as it assumes a fixed window size
//returns the 2d point of the FRONT of the object
_2D get_screen_front_coord(int object_slot)
{
//_ZPoly *the_polys;
//textured_connections *pconnections;
_3D *this_object_points;
_3D *this_object_original_points;
int point_index=0;	//which point has greatest x extent
int i,number_of_points;
double x,y,z;

//find the front of the object
///find the rightmost point in the non-rotated points, making a note of it;s point number
//and then persing that point out of the world points

     this_object_points=(*ocb_ptr).object_list[object_slot].world_points_ptr;	//the world points for this object
     this_object_original_points=(*ocb_ptr).object_list[object_slot].points_ptr;	//the original points
     x=-500000;
     number_of_points=(*ocb_ptr).object_list[object_slot].number_of_points;
//find rightmost point
     for (i=0;i<number_of_points;i++)
     {
       if ((*this_object_original_points).x>x)
       {
        x=(*this_object_original_points).x; 
        point_index=i;
       }
       this_object_original_points++;
     }
     
     
     x=this_object_points[point_index].x;
     y=this_object_points[point_index].y;
     z=this_object_points[point_index].z;
     return point_to_screen(x,y,z,monitor_w,monitor_h);
//   }

   
//     return point_to_screen(x,y,z,640,CONTROL_PANEL_TOP);
}

//only call during game as it assumes a fixed window size
//returns the 2d point of the FRONT of the object
_2D get_screen_rear_coord(int object_slot)
{
//_ZPoly *the_polys;
//textured_connections *pconnections;
_3D *this_object_points;
_3D *this_object_original_points;
int point_index=0;	//which point has greatest x extent
int i,number_of_points;
double x,y,z;

//find the front of the object
///find the rightmost point in the non-rotated points, making a note of it;s point number
//and then persing that point out of the world points

     this_object_points=(*ocb_ptr).object_list[object_slot].world_points_ptr;	//the world points for this object
     this_object_original_points=(*ocb_ptr).object_list[object_slot].points_ptr;	//the original points
     x=500000;
     number_of_points=(*ocb_ptr).object_list[object_slot].number_of_points;
//find rightmost point
     for (i=0;i<number_of_points;i++)
     {
       if ((*this_object_original_points).x<x)
       {
        x=(*this_object_original_points).x; 
        point_index=i;
       }
       this_object_original_points++;
     }
     
     
     x=this_object_points[point_index].x;
     y=this_object_points[point_index].y;
     z=this_object_points[point_index].z;
 

     return point_to_screen(x,y,z,monitor_w,monitor_h);

   
//     return point_to_screen(x,y,z,640,CONTROL_PANEL_TOP);
}


//convert a 3d point to screen coords given screen width and height
_2D point_to_screen(double x, double y, double z, int width, int height)
{
_2D return_val;
double pdiv;
int center_x,center_y;

if (0)
//if (z<camera_pos.z)
{
return_val.x=-1; return_val.y=-1;
return return_val;
}
else
{
 center_x=width>>1;
 center_y=height>>1;
 
pdiv=depth_of_field;
 //{
  return_val.x=STATIC_CAST_TO_INT(center_x+(x*pdiv/z));
  return_val.y=STATIC_CAST_TO_INT(center_y-(y*pdiv/z));
 //}
 //else
 //{
 // return_val.x=center_x+(x*256/z);
 // return_val.y=center_y-(y*256/z);
 //}
 return return_val;
}
}



_2Dp point_to_screen_with_bflag(double x, double y, double z, int width, int height)
{

float pdiv;
int center_x,center_y;
_2Dp return_val;

 center_x=width>>1;
 center_y=height>>1;
 

pdiv=depth_of_field;
 //{
  return_val.x=STATIC_CAST_TO_INT(center_x+(x*pdiv/z));
  return_val.y=STATIC_CAST_TO_INT(center_y-(y*pdiv/z));
     if (z<0) return_val.behind=1;
     else return_val.behind=0;

 return return_val;
}

