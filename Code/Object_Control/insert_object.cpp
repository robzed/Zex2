//insert object
/*
 * $Log: insert\040object.c,v $
 * Revision 1.1.1.1  2003/09/05 22:36:01  stu_c
 * First Imported.
 *
 * Revision 1.11  2002/09/08 02:15:12  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.10  2002/07/28 17:16:34  stu_c
 * Exhaust particles and further work
 *
 * Revision 1.9  2002/05/31 23:54:21  stu
 * Removed all warnings from the code
 *
 * Revision 1.8  2002/04/13 15:33:29  stu
 * Rotation, Alycians, behaviour
 *
 * Revision 1.7  2001/12/04 23:36:44  stu
 * Motherships work
 *
 * Revision 1.6  2001/11/05 22:45:12  stu
 * Object thrust initialised correctly so engines doesn't crash on frame #1
 *
 * Revision 1.5  2001/11/05 20:02:34  stu
 * More OSX work
 *
 * Revision 1.4  2001/11/02 23:20:06  stu
 * Further OS X work. Enabled syscon logging.
 *
 * Revision 1.3  2001/11/02 19:32:27  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:27:57  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:04:52  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:06  rob
 * First Imported.
 *
 * Revision 1.6  2001/09/04 20:37:29  stu
 * Sim stuff
 *
 * Revision 1.5  2001/06/23 20:02:09  stu
 * 0.66 checkin
 *
 * Revision 1.4  2001/06/03 03:20:32  stu
 * OCB structure now contains a sub structure called:
ZObjectControl
Which contains all the control data for an object.

Added accessor in access_ocbs which returns a pointer to type ZObjectControl
 *
 * Revision 1.3  2001/05/28 01:46:28  stu
 * 280501
 *
 * Revision 1.2  2001/04/01 18:27:52  stu
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2001/01/01 21:13:38  rob
 * First Imported.
 *
 * Revision 1.1  2000/10/08 21:45:06  stu
 * Engine conversion to 64 bit. Bitch of a job.
 *
 * Revision 1.0.0.1  2000/08/21 22:07:04  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "3d_structures_def.h"
  #include "error_handler.h"
#endif

#include "3d_top.h"
#include "insert object.h"
#include "texture_management.h"
#include "matrix_primitives.h"
#include "sds.h"
#include "game_constants.h"

void update_object_with_ship_data (mount_pb_type* mounting_info, ZObject* dest_obj_ptr, float bounds_sphere);


//еееееееееееееееееееееееееееееееееееееееееееее

//Inserts an object into the static definitions
void  insert_object(int object_serial,_3D *vertices,textured_connections *connections, int number_of_vertices, int number_of_connections, 
              int pos_x, unsigned long the_id,int is_crashable,float bounds_sphere,int texture_id, int translucent)
{
//set up DynObjectFixedSize
extern Zex_Colour pcol[9];
extern	StatObjectsFixedSize *ocb_Sptr;
extern _ZPoly *Spoly_list_ptr_reset;

extern int	static_object_index;	//how many static objects

extern _3D *points_list_ptr;
extern textured_connections *connection_list_ptr;
extern	_ZPoly *Spoly_list_ptr;
extern	textured_connections* connection_list_top;
extern textured_connections *connection_list_ptr_reset;
extern _ZPoly * Spoly_list_top;

_ZPoly *object_poly_list_ptr;
int object_number_of_points;
int object_number_of_polys;
//int	ocb_place=0;
int i,material_count,points_count;
int j,k;
float minx,maxx,miny,maxy,minz,maxz;

ZObject test;
_ZPoly test_poly = {0,	//connections ptr
                    { 0,0,0, },	//face normal
                    { 0,0,0, }, 	//face normal translated
                    { 0x0,0x0,0x0, }, 	//color
                    0,	//average z
                    0,	//light value
                    0,	//offset to points for renderer
                    0	//texture ptr (0=none)
                    };
                    
textured_connections *oconnections;	//for touching polys test
textured_connections *tconnections;	//same
int touching_polys;
_ZPoly *the_polys;
_ZPoly *our_poly;
_ZPoly *touching_test_poly;

texture_slot* the_texture_slot;
Ptr texture_ptr0 = 0;
unsigned int texture_name=0;

mount_pb_type* mounting_info;
ZObject* dest_obj_ptr;

if (texture_id!=0)
 {
 
 the_texture_slot=find_texture_ptr(texture_id);

 if (the_texture_slot==0) report_error_id ("insert_object: Texture not found!",texture_id);
 texture_ptr0=the_texture_slot->the_raw_data;
 texture_name=the_texture_slot->gl_text_name;
 }


object_number_of_polys=number_of_connections/3;	//1 for number of connections and 6 for connections
if (object_number_of_polys>MAX_NUMBER_POLYS_PER_OBJ) report_error_id_number ("Object has too many polys!",the_id, object_number_of_polys);
object_number_of_points=number_of_vertices;
                                
//copy connections to connection_list_ptr
BlockMoveData	(connections,connection_list_ptr,number_of_connections*sizeof(textured_connections));
//copy points to points_list_pt

BlockMoveData	(vertices,points_list_ptr,number_of_vertices*(sizeof(double)*3));
object_poly_list_ptr=Spoly_list_ptr;
//set up ptrs to connections
material_count=0;
points_count=0;

  if (Spoly_list_ptr<Spoly_list_ptr_reset) report_error_id ("insert_object: Static poly ptr too low!!!!",-1);
  if (connection_list_ptr<connection_list_ptr_reset) report_error_id ("insert_object: static connections ptr too low!!!!",-1);


for (i=0;i<object_number_of_polys;i++)
{
//set up first set of colours
//skip unused material blocks
  while (pcol[material_count].points_start==pcol[material_count+1].points_start) material_count++;

  if (pcol[material_count].points_start==points_count)
  {
    test_poly.Prgb=pcol[material_count].colour;
    material_count++;
  }	//set up colour for these polys
  test_poly.Pid = texture_name;	//the OGL "name"
  test_poly.poly_connections_ptr = connection_list_ptr;
  test_poly.draw_code=translucent;	//not translucent
  connection_list_ptr+=3;	//6 ints make up a triag
  if (connection_list_ptr>connection_list_top) report_error_id ("insert_object: Too many static connections!!!!",-1);
  
  BlockMoveData	(&test_poly,Spoly_list_ptr,sizeof(test_poly));
  Spoly_list_ptr+=1;	//another poly in the static list
  if (Spoly_list_ptr>Spoly_list_top) report_error_id ("insert_object: Too many static polys!!!!",-1);
  points_count++;
}
//create the poly normals - this is calculated from three points on the poly


//test_polys[0].face_normal



//copy the polys to polylist_ptr
//(*poly_list_ptr).poly_list[0]=test_polys;
test.kill_flags=0;
test.Dyn_OCB_control_data.request_dock=0;
test.eng_invisible=0;
test.Oid=the_id;
test.Orotx=0;
test.Oroty=0;
test.Orotz=0;
//test.Orotdx=0;
//test.Orotdy=0;
//test.Orotdz=0;
//test.Omovedx=test.Omovedy=test.Omovedz=0;
test.hit_by_slot=-1;
//test.Dyn_OCB_control_data.attack_what=-1;	//not attacking anything

//change 050299
//if colldet is not zero then we set crashable and colltype
if (is_crashable!=0)
{
test.crashable=1;
test.colltype=is_crashable;
}
else
{
test.crashable=0;
test.colltype=0;
}
//test.number_owned=0;
test.in_use=1;

test.Oworldx=pos_x;
test.Oworldy=0;
test.Oworldz=0;

//bounds sphere
test.radius=bounds_sphere*1.5;

test.Dyn_OCB_control_data.controller=0;
test.Dyn_OCB_control_data.control_use_1 = test.Dyn_OCB_control_data.control_use_2 = test.Dyn_OCB_control_data.control_use_3 = test.Dyn_OCB_control_data.control_use_4 =0;
test.Dyn_OCB_control_data.control_use_1_float=test.Dyn_OCB_control_data.control_use_2_float=0;

test.number_of_polys=object_number_of_polys;
test.number_of_points=object_number_of_points;
test.points_ptr=points_list_ptr;
test.polys_ptr=object_poly_list_ptr;
//calc bounding box (x and y only)
minx=maxx=miny=maxy=minz=maxz=0;

for (i=0;i<object_number_of_points;i++)
	{
	if ((*points_list_ptr).x<minx) minx=(*points_list_ptr).x;
	if ((*points_list_ptr).x>maxx) maxx=(*points_list_ptr).x;
	if ((*points_list_ptr).y<miny) miny=(*points_list_ptr).y;
	if ((*points_list_ptr).y>maxy) maxy=(*points_list_ptr).y;
	if ((*points_list_ptr).z<minz) minz=(*points_list_ptr).z;
	if ((*points_list_ptr).z>maxz) maxz=(*points_list_ptr).z;

	points_list_ptr++;
	}
	
//if (the_id!='ZEXS')
//test.complex_control=0;
//else
//test.complex_control=1;


test.box_min.x=minx;
test.box_min.y=miny;
test.box_min.z=minz;

test.box_max.x=maxx;
test.box_max.y=maxy;
test.box_max.z=maxz;


//test.front_left.x=minx;
//test.front_left.y=0;
//test.front_left.z=maxz;
//
//test.front_right.x=maxx;
//test.front_right.y=0;
//test.front_right.z=maxz;
//
//
//test.front_center.x=0;
//test.front_center.y=miny*2;	  //from below (exagerrated)
//test.front_center.z=maxz;

//set up orientation quaternion

//test.Qor.w=-1;
//test.Qor.x=1;	//pointing along x
//test.Qor.y=0;
//test.Qor.z=0;

//set up polys pointer
(*ocb_Sptr).object_list[object_serial]=test;	//copy into master static object array


//load in mounting points

//do we have mount info for this object?

mounting_info=mount_exists(the_id);
dest_obj_ptr=&(*ocb_Sptr).object_list[object_serial];

//update_object_with_ship_data(mounting_info, dest_obj_ptr, bounds_sphere);
update_object_with_ship_data(mounting_info, dest_obj_ptr, 1);




//BlockMoveData	(&test,ocb_ptr,sizeof(Zobject);

//ocb_ptr=+dynamic_object_index;	//point to right place for header

//calculate polys normals
for(i=0;i<(*ocb_Sptr).object_list[object_serial].number_of_polys;i++)
calc_normal(object_serial,i);	//object,poly

points_list_ptr+=object_number_of_points+10;		//ready for next object (that +10 was causing all kinds of bugs 13/1/99 SB)
//poly_list_ptr+=object_number_of_polys;

static_object_index++;

//d11 - gourad shading
//now calc touching polys for every poly [i] in this object - crunch crunch time.


//do point 1 of all polys
for(i=0;i<(*ocb_Sptr).object_list[object_serial].number_of_polys;i++)
{
 touching_polys=0;	//our index into Ptouching_polys;
 the_polys=(*ocb_Sptr).object_list[object_serial].polys_ptr;	//pointing to static definition
 our_poly=the_polys+i;	//poly we're working on

 for (j=0;j<MAX_TOUCHING_POLYS;j++)
 {
  (*our_poly).Ptouching1[j]=-1;
  (*our_poly).Ptouching2[j]=-1;
  (*our_poly).Ptouching3[j]=-1;
 }	//set all touching to not

 for (j=0;j<(*ocb_Sptr).object_list[object_serial].number_of_polys;j++)
  {
  if (j!=i)	//dont include this poly in the check
    {
      oconnections=(*our_poly).poly_connections_ptr;	//points in our poly reset	
      //see if this poly shares points with poly[i]
      touching_test_poly=the_polys+j;	//poly we're examining
      tconnections=(*touching_test_poly).poly_connections_ptr;		//points in test poly
      
      
      
      for (k=0;k<3;k++)	//loop around test poly
       {
       if ((*tconnections).the_node==(*oconnections).the_node)
          {
          if (touching_polys<MAX_TOUCHING_POLYS) (*our_poly).Ptouching1[touching_polys]=j;	 //store poly id
          touching_polys++;
          break; //k=69; from k	- we have found a point on this poly touching so get out
          }
        tconnections++;	//inc test poly point
       }	//end of k
       
       
    
    }	//end of if j<>i
  }	//end of j
  
}	//end of i


//do point 2 of all polys
for(i=0;i<(*ocb_Sptr).object_list[object_serial].number_of_polys;i++)
{
 touching_polys=0;	//our index into Ptouching_polys;
 the_polys=(*ocb_Sptr).object_list[object_serial].polys_ptr;	//pointing to static definition
 our_poly=the_polys+i;	//poly we're working on

 for (j=0;j<(*ocb_Sptr).object_list[object_serial].number_of_polys;j++)
  {
  if (j!=i)	//dont include this poly in the check
    {
      oconnections=(*our_poly).poly_connections_ptr;	//points in our poly reset	
      oconnections++;	//point 2
      //see if this poly shares points with poly[i]
      touching_test_poly=the_polys+j;	//poly we're examining
      tconnections=(*touching_test_poly).poly_connections_ptr;		//points in test poly
      for (k=0;k<3;k++)	//loop around test poly
       {
       if ((*tconnections).the_node==(*oconnections).the_node)
          {
          if (touching_polys<MAX_TOUCHING_POLYS) (*our_poly).Ptouching2[touching_polys]=j;	 //store poly id
          touching_polys++;
          break; //k=69;	//have to get out of these loops
          }
        tconnections++;	//inc test poly point
       }	//end of k
    
    }	//end of if j<>i
  }	//end of j
  
}	//end of i



//do point 3 of all polys
for(i=0;i<(*ocb_Sptr).object_list[object_serial].number_of_polys;i++)
{
 touching_polys=0;	//our index into Ptouching_polys;
 the_polys=(*ocb_Sptr).object_list[object_serial].polys_ptr;	//pointing to static definition
 our_poly=the_polys+i;	//poly we're working on

 for (j=0;j<(*ocb_Sptr).object_list[object_serial].number_of_polys;j++)
  {
  if (j!=i)	//dont include this poly in the check
    {
      oconnections=(*our_poly).poly_connections_ptr;	//points in our poly reset	
      oconnections+=2;	//point 3
      //see if this poly shares points with poly[i]
      touching_test_poly=the_polys+j;	//poly we're examining
      tconnections=(*touching_test_poly).poly_connections_ptr;		//points in test poly
      for (k=0;k<3;k++)	//loop around test poly
       {
       if ((*tconnections).the_node==(*oconnections).the_node)
          {
          if (touching_polys<MAX_TOUCHING_POLYS) (*our_poly).Ptouching3[touching_polys]=j;	 //store poly id
          touching_polys++;
          break; //k=69;	//have to get out of these loops
          }
        tconnections++;	//inc test poly point
       }	//end of k
    
    }	//end of if j<>i
  }	//end of j
  
}	//end of i



}	//end of new_insert_object





//Inserts an object into the static definitions
void  dmf_insert_object(int object_serial,_3D *vertices,dmf_textured_connections *connections, int number_of_vertices, int number_of_connections, 
               unsigned long the_id,int is_crashable,float bounds_sphere)
{
//set up DynObjectFixedSize
//extern Zex_Colour pcol[9];
extern	StatObjectsFixedSize *ocb_Sptr;

extern int	static_object_index;	//how many static objects

extern _3D *points_list_ptr;
extern textured_connections *connection_list_ptr;
extern	_ZPoly *Spoly_list_ptr;
extern	textured_connections* connection_list_top;

_ZPoly *object_poly_list_ptr;
int object_number_of_points;
int object_number_of_polys;
//int	ocb_place=0;
int i,material_count;
int j,k;
float minx,maxx,miny,maxy,minz,maxz;

ZObject test;
_ZPoly test_poly = {0,	//connections ptr
                    { 0,0,0, },	//face normal
                    { 0,0,0, },	//face normal translated
                    { 0x0,0x0,0x0, },	//color
                    0,	//average z
                    0,	//light value
                    0,	//offset to points for renderer
                    0	//texture ptr (0=none)
                    };
                    
textured_connections *oconnections;	//for touching polys test
textured_connections *tconnections;	//same
int touching_polys;
_ZPoly *the_polys;
_ZPoly *our_poly;
_ZPoly *touching_test_poly;

//texture_slot* the_texture_slot;
//Ptr texture_ptr0 = 0;
//unsigned int texture_name;

mount_pb_type* mounting_info;
ZObject* dest_obj_ptr;
extern _ZPoly *Spoly_list_ptr_reset;
extern textured_connections *connection_list_ptr_reset;
extern _ZPoly * Spoly_list_top;



object_number_of_polys=number_of_connections/3;	//1 for number of connections and 6 for connections
if (object_number_of_polys>MAX_NUMBER_POLYS_PER_OBJ) report_error_id_number ("Object has too many polys!",the_id, object_number_of_polys);
object_number_of_points=number_of_vertices;
                                
//copy connections to connection_list_ptr
BlockMoveData	(connections,connection_list_ptr,number_of_connections*sizeof(dmf_textured_connections));
//copy points to points_list_pt

BlockMoveData	(vertices,points_list_ptr,number_of_vertices*(sizeof(double)*3));
object_poly_list_ptr=Spoly_list_ptr;
//set up ptrs to connections
material_count=0;

  if (Spoly_list_ptr<Spoly_list_ptr_reset) report_error_id ("insert_object: Static poly ptr too low!!!!",-1);
  if (connection_list_ptr<connection_list_ptr_reset) report_error_id ("insert_object: static connections ptr too low!!!!",-1);

//walk the connections getting attributes for each poly
for (i=0;i<number_of_connections;i++)
{
int temp_red, temp_green, temp_blue;
  
  temp_red=connections[i].colour.red*65535.0;
  temp_green=connections[i].colour.green*65535.0;
  temp_blue=connections[i].colour.blue*65535.0;
  
  test_poly.Prgb.red=temp_red;
  test_poly.Prgb.green=temp_green;
  test_poly.Prgb.blue=temp_blue;
  test_poly.Pid =connections[i].OGL_name ;	//the OGL "name"
  test_poly.poly_connections_ptr = connection_list_ptr;
  
  i+=2;
  
  connection_list_ptr+=3;	//6 ints make up a triag
  if (connection_list_ptr>connection_list_top) report_error_id ("insert_object: Too many static connections!!!!",-1);
  
  BlockMoveData	(&test_poly,Spoly_list_ptr,sizeof(test_poly));
  Spoly_list_ptr+=1;	//another poly in the static list
  if (Spoly_list_ptr>Spoly_list_top) report_error_id ("insert_object: Too many static polys!!!!",-1);
}
//create the poly normals - this is calculated from three points on the poly


//test_polys[0].face_normal



//copy the polys to polylist_ptr
//(*poly_list_ptr).poly_list[0]=test_polys;
test.kill_flags=0;
test.Dyn_OCB_control_data.request_dock=0;
test.eng_invisible=0;
test.Oid=the_id;
test.Orotx=0;
test.Oroty=0;
test.Orotz=0;
//test.Orotdx=0;
//test.Orotdy=0;
//test.Orotdz=0;
//test.Omovedx=test.Omovedy=test.Omovedz=0;
test.hit_by_slot=-1;
//test.Dyn_OCB_control_data.attack_what=-1;	//not attacking anything

//change 050299
//if colldet is not zero then we set crashable and colltype
if (is_crashable!=0)
{
test.crashable=1;
test.colltype=is_crashable;
}
else
{
test.crashable=0;
test.colltype=0;
}
//test.number_owned=0;
test.in_use=1;

test.Oworldx=0;
test.Oworldy=0;
test.Oworldz=0;

//bounds sphere
test.radius=bounds_sphere*1.5;

test.Dyn_OCB_control_data.controller=0;
test.Dyn_OCB_control_data.control_use_1 = test.Dyn_OCB_control_data.control_use_2 = test.Dyn_OCB_control_data.control_use_3 = test.Dyn_OCB_control_data.control_use_4 =0;
//test.Oreference=1;
//test.immediate_child=NULL;
test.number_of_polys=object_number_of_polys;
test.number_of_points=object_number_of_points;
test.points_ptr=points_list_ptr;
test.polys_ptr=object_poly_list_ptr;
//calc bounding box (x and y only)
minx=maxx=miny=maxy=minz=maxz=0;

for (i=0;i<object_number_of_points;i++)
	{
	if ((*points_list_ptr).x<minx) minx=(*points_list_ptr).x;
	if ((*points_list_ptr).x>maxx) maxx=(*points_list_ptr).x;
	if ((*points_list_ptr).y<miny) miny=(*points_list_ptr).y;
	if ((*points_list_ptr).y>maxy) maxy=(*points_list_ptr).y;
	if ((*points_list_ptr).z<minz) minz=(*points_list_ptr).z;
	if ((*points_list_ptr).z>maxz) maxz=(*points_list_ptr).z;

	points_list_ptr++;
	}
	
//if (the_id!='ZEXS')
//test.complex_control=0;
//else
//test.complex_control=1;


test.box_min.x=minx;
test.box_min.y=miny;
test.box_min.z=minz;

test.box_max.x=maxx;
test.box_max.y=maxy;
test.box_max.z=maxz;

//test.front_left.x=minx;
//test.front_left.y=0;
//test.front_left.z=maxz;
//
//test.front_right.x=maxx;
//test.front_right.y=0;
//test.front_right.z=maxz;
//
//
//test.front_center.x=0;
//test.front_center.y=-170;
//test.front_right.z=maxz;

//set up orientation quaternion

//test.Qor.w=-1;
//test.Qor.x=1;	//pointing along x
//test.Qor.y=0;
//test.Qor.z=0;

//set up polys pointer
(*ocb_Sptr).object_list[object_serial]=test;	//copy into master static object array
//BlockMoveData	(&test,ocb_ptr,sizeof(Zobject);
//load in mounting points

//do we have mount info for this object?
dest_obj_ptr=&(*ocb_Sptr).object_list[object_serial];

mounting_info=mount_exists(the_id);

//update_object_with_ship_data(mounting_info, dest_obj_ptr, bounds_sphere);
update_object_with_ship_data(mounting_info, dest_obj_ptr, 1);


//ocb_ptr=+dynamic_object_index;	//point to right place for header

//calculate polys normals
for(i=0;i<(*ocb_Sptr).object_list[object_serial].number_of_polys;i++)
calc_normal(object_serial,i);	//object,poly

points_list_ptr+=object_number_of_points+10;		//ready for next object (that +10 was causing all kinds of bugs 13/1/99 SB)
//poly_list_ptr+=object_number_of_polys;

static_object_index++;

//d11 - gourad shading
//now calc touching polys for every poly [i] in this object - crunch crunch time.


//do point 1 of all polys
for(i=0;i<(*ocb_Sptr).object_list[object_serial].number_of_polys;i++)
{
 touching_polys=0;	//our index into Ptouching_polys;
 the_polys=(*ocb_Sptr).object_list[object_serial].polys_ptr;	//pointing to static definition
 our_poly=the_polys+i;	//poly we're working on

 for (j=0;j<MAX_TOUCHING_POLYS;j++)
 {
  (*our_poly).Ptouching1[j]=-1;
  (*our_poly).Ptouching2[j]=-1;
  (*our_poly).Ptouching3[j]=-1;
 }	//set all touching to not

 for (j=0;j<(*ocb_Sptr).object_list[object_serial].number_of_polys;j++)
  {
  if (j!=i)	//dont include this poly in the check
    {
      oconnections=(*our_poly).poly_connections_ptr;	//points in our poly reset	
      //see if this poly shares points with poly[i]
      touching_test_poly=the_polys+j;	//poly we're examining
      tconnections=(*touching_test_poly).poly_connections_ptr;		//points in test poly
      
      
      
      for (k=0;k<3;k++)	//loop around test poly
       {
       if ((*tconnections).the_node==(*oconnections).the_node)
          {
          if (touching_polys<MAX_TOUCHING_POLYS) (*our_poly).Ptouching1[touching_polys]=j;	 //store poly id
          touching_polys++;
          break; //k=69; from k	- we have found a point on this poly touching so get out
          }
        tconnections++;	//inc test poly point
       }	//end of k
       
       
    
    }	//end of if j<>i
  }	//end of j
  
}	//end of i


//do point 2 of all polys
for(i=0;i<(*ocb_Sptr).object_list[object_serial].number_of_polys;i++)
{
 touching_polys=0;	//our index into Ptouching_polys;
 the_polys=(*ocb_Sptr).object_list[object_serial].polys_ptr;	//pointing to static definition
 our_poly=the_polys+i;	//poly we're working on

 for (j=0;j<(*ocb_Sptr).object_list[object_serial].number_of_polys;j++)
  {
  if (j!=i)	//dont include this poly in the check
    {
      oconnections=(*our_poly).poly_connections_ptr;	//points in our poly reset	
      oconnections++;	//point 2
      //see if this poly shares points with poly[i]
      touching_test_poly=the_polys+j;	//poly we're examining
      tconnections=(*touching_test_poly).poly_connections_ptr;		//points in test poly
      for (k=0;k<3;k++)	//loop around test poly
       {
       if ((*tconnections).the_node==(*oconnections).the_node)
          {
          if (touching_polys<MAX_TOUCHING_POLYS) (*our_poly).Ptouching2[touching_polys]=j;	 //store poly id
          touching_polys++;
          break; //k=69;	//have to get out of these loops
          }
        tconnections++;	//inc test poly point
       }	//end of k
    
    }	//end of if j<>i
  }	//end of j
  
}	//end of i



//do point 3 of all polys
for(i=0;i<(*ocb_Sptr).object_list[object_serial].number_of_polys;i++)
{
 touching_polys=0;	//our index into Ptouching_polys;
 the_polys=(*ocb_Sptr).object_list[object_serial].polys_ptr;	//pointing to static definition
 our_poly=the_polys+i;	//poly we're working on

 for (j=0;j<(*ocb_Sptr).object_list[object_serial].number_of_polys;j++)
  {
  if (j!=i)	//dont include this poly in the check
    {
      oconnections=(*our_poly).poly_connections_ptr;	//points in our poly reset	
      oconnections+=2;	//point 3
      //see if this poly shares points with poly[i]
      touching_test_poly=the_polys+j;	//poly we're examining
      tconnections=(*touching_test_poly).poly_connections_ptr;		//points in test poly
      for (k=0;k<3;k++)	//loop around test poly
       {
       if ((*tconnections).the_node==(*oconnections).the_node)
          {
          if (touching_polys<MAX_TOUCHING_POLYS) (*our_poly).Ptouching3[touching_polys]=j;	 //store poly id
          touching_polys++;
          break; //k=69;	//have to get out of these loops
          }
        tconnections++;	//inc test poly point
       }	//end of k
    
    }	//end of if j<>i
  }	//end of j
  
}	//end of i



}	//end of new_insert_object



void update_object_with_ship_data (mount_pb_type* mounting_info, ZObject* dest_obj_ptr, float bounds_sphere)
{
_3D mount_init = { 0,0,0 };

 if(mounting_info==0)
 {
 
 	 dest_obj_ptr->mthr1=mount_init;
 	 dest_obj_ptr->mthr2=mount_init;
 	 dest_obj_ptr->mrev1=mount_init;
 	 dest_obj_ptr->mrev2=mount_init;
 	 dest_obj_ptr->vthr1=mount_init;
 	 dest_obj_ptr->vthr2=mount_init;
 	 dest_obj_ptr->rvth1=mount_init;
 	 dest_obj_ptr->rvth2=mount_init;
 
 	 dest_obj_ptr->gun01=mount_init;
 	 dest_obj_ptr->gun02=mount_init;
 	 dest_obj_ptr->gun03=mount_init;
 	 dest_obj_ptr->gun04=mount_init;
 
 	 dest_obj_ptr->las01=mount_init;
 	 dest_obj_ptr->las02=mount_init;
 	 dest_obj_ptr->las03=mount_init;
 	 dest_obj_ptr->las04=mount_init;
 
 	 dest_obj_ptr->mis01=mount_init;
 	 dest_obj_ptr->mis02=mount_init;
 	 dest_obj_ptr->mis03=mount_init;
 
 	 dest_obj_ptr->eject=mount_init;
 
 
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mthr1=BK_MISSING;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mthr2=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_mrev1=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_mrev2=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_vthr1=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_vthr2=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_rvth1=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_rvth2=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_gun01=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_gun02=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_gun03=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_gun04=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_las01=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_las02=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_las01=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_las04=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_mis01=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_mis02=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_mis03=BK_MISSING;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_eject=BK_MISSING;
 	 
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_mass_tonnes=1;
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_fuel_capacity_kg=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.cargo_capacity_tonnes=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.hull_class=1;
 	 
	dest_obj_ptr->Dyn_OCB_control_data.fuel_tank_value_in_KGS=0;	//no fuel if no SDS data

 	 dest_obj_ptr->Dyn_OCB_control_data.rotational_pitch=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.rotational_roll=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.rotational_yaw=0;

 	 dest_obj_ptr->Dyn_OCB_control_data.shields1_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.shields2_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.shields3_class=0;

 	 dest_obj_ptr->Dyn_OCB_control_data.AP_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.radar_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.batt_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.ORCS=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.repair_system_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.shockwave_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.ECM_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.missfact_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.fastdock_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.insurance_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.fuel_maximiser_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.retrorocks_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.navcomp_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.trading_comp_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.hull_repair_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.trailhook=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.telnet=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.IFF=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.radiomon=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.cloak_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.FTL=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.weapon_bay_cooling_class=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.scoop=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.freedock=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.mines=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.hypertransport_count=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.hypertransport_max_count=0;
 	 dest_obj_ptr->Dyn_OCB_control_data.tempmult_class=0;
         dest_obj_ptr->Dyn_OCB_control_data.actual_main_thrust=0; 
 	 dest_obj_ptr->Dyn_OCB_control_data.was_loaded_from_sds=0;
 	 
 }
 else
 {
 	 Vmultiply (&mounting_info->mthr1,bounds_sphere,&dest_obj_ptr->mthr1);
 	 Vmultiply (&mounting_info->mthr2,bounds_sphere,&dest_obj_ptr->mthr2);
 	 Vmultiply (&mounting_info->mrev1,bounds_sphere,&dest_obj_ptr->mrev1);
 	 Vmultiply (&mounting_info->mrev2,bounds_sphere,&dest_obj_ptr->mrev2);
 	 Vmultiply (&mounting_info->vthr1,bounds_sphere,&dest_obj_ptr->vthr1);
 	 Vmultiply (&mounting_info->vthr2,bounds_sphere,&dest_obj_ptr->vthr2);
 	 Vmultiply (&mounting_info->rvth1,bounds_sphere,&dest_obj_ptr->rvth1);
 	 Vmultiply (&mounting_info->rvth2,bounds_sphere,&dest_obj_ptr->rvth2);
 	 Vmultiply (&mounting_info->gun01,bounds_sphere,&dest_obj_ptr->gun01);
 	 Vmultiply (&mounting_info->gun02,bounds_sphere,&dest_obj_ptr->gun02);
 	 Vmultiply (&mounting_info->gun03,bounds_sphere,&dest_obj_ptr->gun03);
 	 Vmultiply (&mounting_info->gun04,bounds_sphere,&dest_obj_ptr->gun04);
 	 Vmultiply (&mounting_info->las01,bounds_sphere,&dest_obj_ptr->las01);
 	 Vmultiply (&mounting_info->las02,bounds_sphere,&dest_obj_ptr->las02);
 	 Vmultiply (&mounting_info->las03,bounds_sphere,&dest_obj_ptr->las03);
 	 Vmultiply (&mounting_info->las04,bounds_sphere,&dest_obj_ptr->las04);
 	 Vmultiply (&mounting_info->eject,bounds_sphere,&dest_obj_ptr->eject);
 
 	 //set serviceable bits
 	 //layout of these fields is 31 - 3 = value, 2 = can be fitted, 1,0 = 00=not fitted
 	 if(mounting_info->mthr1v==0)
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mthr1=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mthr1=(UInt32)mounting_info->mthr1v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mthr1|=BK_SERVICEABLE+BK_CAN_FIT;
         }
 	 if((mounting_info->mthr2v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mthr2=BK_MISSING;
 	 else
         {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mthr2=(UInt32)mounting_info->mthr2v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mthr2|=BK_SERVICEABLE+BK_CAN_FIT;
         }
 	 if((mounting_info->mrev1v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mrev1=BK_MISSING;
 	 else
         {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mrev1=(UInt32)mounting_info->mrev1v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mrev1|=BK_SERVICEABLE+BK_CAN_FIT;
          }
 	 if((mounting_info->mrev2v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mrev2=BK_MISSING;
 	 else
         {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mrev2=(UInt32)mounting_info->mrev2v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mrev2|=BK_SERVICEABLE+BK_CAN_FIT;
         }
 	 if((mounting_info->vthr1v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_vthr1=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_vthr1=(UInt32)mounting_info->vthr1v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_vthr1|=BK_SERVICEABLE+BK_CAN_FIT;
         }
 	 if(mounting_info->vthr2v==0)
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_vthr2=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_vthr2=(UInt32)mounting_info->vthr2v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_vthr2|=BK_SERVICEABLE+BK_CAN_FIT;
         } 
 	 if((mounting_info->rvth1v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_rvth1=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_rvth1=(UInt32)mounting_info->rvth1v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_rvth1|=BK_SERVICEABLE+BK_CAN_FIT;
         } 
         
 	 if((mounting_info->rvth2v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_rvth2=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_rvth2=(UInt32)mounting_info->rvth2v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_rvth2|=BK_SERVICEABLE+BK_CAN_FIT;
         } 
         
 	 if((mounting_info->gun01v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_gun01=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_gun01=(UInt32)mounting_info->gun01v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_gun01|=BK_SERVICEABLE+BK_CAN_FIT;
         } 
 	 if((mounting_info->gun02v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_gun02=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_gun02=(UInt32)mounting_info->gun02v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_gun02|=BK_SERVICEABLE+BK_CAN_FIT;
         } 
         
 	 if((mounting_info->gun03v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_gun03=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_gun03=(UInt32)mounting_info->gun03v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_gun03|=BK_SERVICEABLE+BK_CAN_FIT;
         } 
 
 	 if((mounting_info->gun04v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_gun04=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_gun04=(UInt32)mounting_info->gun04v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_gun04|=BK_SERVICEABLE+BK_CAN_FIT;
         } 
 
 
 	 if((mounting_info->las01v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_las01=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_las01=(UInt32)mounting_info->las01v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_las01=BK_SERVICEABLE+BK_CAN_FIT;
         } 
 
 	 if((mounting_info->las02v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_las02=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_las02=(UInt32)mounting_info->las02v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_las02=BK_SERVICEABLE+BK_CAN_FIT;
         } 
 
 	 if((mounting_info->las03v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_las03=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_las03=(UInt32)mounting_info->las03v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_las03=BK_SERVICEABLE+BK_CAN_FIT;
         } 
 
 	 if((mounting_info->las04v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_las04=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_las04=(UInt32)mounting_info->las04v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_las04=BK_SERVICEABLE+BK_CAN_FIT;
         } 
 
 
 	 if((mounting_info->mis01v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mis01=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mis01=(UInt32)mounting_info->mis01v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mis01|=BK_SERVICEABLE+BK_CAN_FIT;
         }
          
 	 if((mounting_info->mis02v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mis02=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mis02=(UInt32)mounting_info->mis02v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mis02|=BK_SERVICEABLE+BK_CAN_FIT;
         }
 
 	 if((mounting_info->mis03v==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mis03=BK_MISSING;
 	 else
 	 {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mis03=(UInt32)mounting_info->mis03v<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_mis03|=BK_SERVICEABLE+BK_CAN_FIT;
         }
 
 	 if((mounting_info->ejectv==0))
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_eject=BK_MISSING;
 	 else
         {
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_eject=(UInt32)mounting_info->ejectv<<3;
 	    dest_obj_ptr->Dyn_OCB_control_data.bk_eject|=BK_SERVICEABLE+BK_CAN_FIT;
         }
         
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_mass_tonnes=mounting_info->mass_tonnes;
 	 dest_obj_ptr->mass=mounting_info->mass_tonnes;
 	 
 	 dest_obj_ptr->Dyn_OCB_control_data.bk_fuel_capacity_kg=(UInt32)mounting_info->fuel_capacity_kgs;
 	 dest_obj_ptr->Dyn_OCB_control_data.cargo_capacity_tonnes=(UInt8)mounting_info->cargo_capacity_tonnes;
 	 dest_obj_ptr->Dyn_OCB_control_data.hull_class=(UInt8)mounting_info->hull_class;
 	 
 	 dest_obj_ptr->Dyn_OCB_control_data.rotational_pitch=mounting_info->rotational_pitch;
 	 dest_obj_ptr->Dyn_OCB_control_data.rotational_roll=mounting_info->rotational_roll;
 	 dest_obj_ptr->Dyn_OCB_control_data.rotational_yaw=mounting_info->rotational_yaw;

 	 dest_obj_ptr->Dyn_OCB_control_data.shields1_class=(UInt8)mounting_info->shields1_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.shields2_class=(UInt8)mounting_info->shields2_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.shields3_class=(UInt8)mounting_info->shields3_class;

   
 	 dest_obj_ptr->Dyn_OCB_control_data.AP_class=(UInt8)mounting_info->AP_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.radar_class=(UInt8)mounting_info->radar_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.batt_class=(UInt8)mounting_info->batt_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.ORCS=(UInt8)mounting_info->ORCS;
 	 dest_obj_ptr->Dyn_OCB_control_data.repair_system_class=(UInt8)mounting_info->repair_system_class;;
 	 dest_obj_ptr->Dyn_OCB_control_data.shockwave_class=(UInt8)mounting_info->shockwave_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.ECM_class=(UInt8)mounting_info->ECM_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.missfact_class=(UInt8)mounting_info->missfact_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.fastdock_class=(UInt8)mounting_info->fastdock_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.insurance_class=(UInt8)mounting_info->insurance_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.fuel_maximiser_class=(UInt8)mounting_info->fuel_maximiser_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.retrorocks_class=(UInt8)mounting_info->retrorocks_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.navcomp_class=(UInt8)mounting_info->navcomp_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.trading_comp_class=(UInt8)mounting_info->trading_comp_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.hull_repair_class=(UInt8)mounting_info->hull_repair_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.trailhook=(UInt8)mounting_info->trailhook;
 	 dest_obj_ptr->Dyn_OCB_control_data.telnet=(UInt8)mounting_info->telnet;
 	 dest_obj_ptr->Dyn_OCB_control_data.IFF=(UInt8)mounting_info->IFF;
 	 dest_obj_ptr->Dyn_OCB_control_data.radiomon=(UInt8)mounting_info->radiomon;
 	 dest_obj_ptr->Dyn_OCB_control_data.cloak_class=(UInt8)mounting_info->cloak_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.FTL=(UInt8)mounting_info->FTL;
 	 dest_obj_ptr->Dyn_OCB_control_data.weapon_bay_cooling_class=(UInt8)mounting_info->weapon_bay_cooling_class;
 	 dest_obj_ptr->Dyn_OCB_control_data.scoop=(UInt8)mounting_info->scoop;
 	 dest_obj_ptr->Dyn_OCB_control_data.freedock=(UInt8)mounting_info->freedock;
 	 dest_obj_ptr->Dyn_OCB_control_data.mines=(UInt8)mounting_info->mines;
 	 dest_obj_ptr->Dyn_OCB_control_data.hypertransport_count=(UInt8)mounting_info->hypertransport_count;
 	 dest_obj_ptr->Dyn_OCB_control_data.hypertransport_max_count=(UInt8)mounting_info->hypertransport_max_count;
 	 dest_obj_ptr->Dyn_OCB_control_data.tempmult_class=(UInt8)mounting_info->tempmult_class;
//update fuel tank and usage vars
         dest_obj_ptr->Dyn_OCB_control_data.fuel_tank_value_in_KGS=mounting_info->fuel_capacity_kgs;	//start with a full tank
         dest_obj_ptr->Dyn_OCB_control_data.reverse_thrust_consumption_per_second_in_KGS
         =(UInt32)mounting_info->mrev1v;	//one reverse engine

         dest_obj_ptr->Dyn_OCB_control_data.main_thrust_consumption_per_second_in_KGS
         =(UInt32)mounting_info->mthr1v;	//one main engine
         
         dest_obj_ptr->Dyn_OCB_control_data.reverse_thrust_consumption_per_second_in_KGS*=STANDARD_FUEL_USAGE_PER_NEWTONSECOND_IN_KGS;
         dest_obj_ptr->Dyn_OCB_control_data.main_thrust_consumption_per_second_in_KGS*=STANDARD_FUEL_USAGE_PER_NEWTONSECOND_IN_KGS;
     dest_obj_ptr->Dyn_OCB_control_data.actual_main_thrust=0; 
         
 	 dest_obj_ptr->Dyn_OCB_control_data.was_loaded_from_sds=1;

 }

}