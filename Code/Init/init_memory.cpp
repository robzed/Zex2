//init_memory.cpp
/*
 * $Log: init_memory.cpp,v $
 * Revision 1.3  2003/09/27 08:58:34  robp
 * Removal of Invalid Conversion Warning for C++ (I hate unsigned/signed chars)
 *
 * Revision 1.2  2003/09/20 12:56:58  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:35:52  stu_c
 * First Imported.
 *
 * Revision 1.7  2002/09/08 02:03:49  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.6  2002/06/16 23:19:33  stu
 * Particles booked in for backup purposes - not working yet or complete.
 *
 * Revision 1.5  2002/05/31 23:52:07  stu
 * Removed all warnings from the code
 *
 * Revision 1.4  2002/02/10 21:43:51  stu
 * fast dyn objects
 *
 * Revision 1.3  2001/11/02 19:31:20  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:28:37  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:02:10  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:06  rob
 * First Imported.
 *
 * Revision 1.2  2001/04/29 20:57:52  stu
 * Z Clipping
 *
 * Revision 1.1.1.1  2001/01/01 21:13:36  rob
 * First Imported.
 *
 * Revision 1.2  2000/10/08 21:49:06  stu
 * Engine conversion to 64 bit. Bitch of a job.
 *
 * Revision 1.1  2000/09/20 23:47:07  stu
 * HUD/AP/Scaling
 *
 * Revision 1.0.0.1  2000/08/21 22:03:21  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "3d_structures_def.h"
  #include "error_handler.h"
#endif

//#include "3d_top.int"
#include "3d_top.h"
#include "init_memory.h"
#include "fcircle.h"

extern cmap_objects *cmap_ptr_base;
extern _3D *points_list_ptr;
extern visible_poly *visible_points_list_ptr;
extern int visible_points_list_top;
extern _2DPoly *view_points_list_ptr;
extern _3D *world_points_list_ptr;
extern visible_poly *visible_points_list_base;

int init_ocbs()
{
//galactic_map_handle thegmHdl;
//galactic_map_ptr thegmPtr;

DynObjectsFixedSizeHandle theHdl;
DynObjectsFixedSizePtr thePtr;
DynFObjectsFixedSizeHandle theFHdl;
DynFObjectsFixedSizePtr theFPtr;
DynPObjectsFixedSizeHandle thePHdl;
DynPObjectsFixedSizePtr thePPtr;
StatObjectsFixedSizeHandle theSHdl;
StatObjectsFixedSizePtr theSPtr;
//short count;
//extern	int *the_ocbs
	Handle temp_h;
	void *temp_ptr;
	int return_val=0;
	//int temp_val;
        int i;
	extern	DynObjectsFixedSize *ocb_ptr;
	extern	StatObjectsFixedSize *ocb_Sptr;
	extern  DynFObjectsFixedSize *Focb_ptr;
	extern  DynPObjectsFixedSize *Pocb_ptr;
	
//	extern  galactic_map *GM_ptr;
//	extern solar_system_map *SS_ptr
	//extern	_3D *pointslist_ptr; //current index
	extern  _3D *points_list_ptr_reset;
	//extern	_3D *visible_pointslist_ptr; //current index

	extern	_3D *points_list_ptr_base;	//they start here
	//extern	textured_connections *connectionlist_ptr;
	//extern	_ZPoly *poly_list_ptr;
	extern	_ZPoly *Spoly_list_ptr;
	extern	_ZPoly *Spoly_list_ptr_reset;
	extern  _ZPoly * Spoly_list_top;	
	extern	_ZPoly_objs *dynamic_polys_ptr;
        extern  _clipped_poly* clipped_polys_ptr;
        
	extern	Polygon **Polygon_handle;
	extern	Polygon *Polygon_ptr;

	extern	textured_connections* connection_list_top;
	extern  int *points_offsets_ptr; //array of ints specifying offset to points for object.


extern int points_list_top;
extern int view_points_list_top;
extern int world_points_list_top;
extern int dynamic_object_top;
//extern int poly_list_top;
//extern int Spoly_list_top;
extern int points_offsets_top;	
extern int dynamic_object_index;
extern int static_object_index;
extern textured_connections *connection_list_ptr;	//global connection list ptr
extern textured_connections *connection_list_ptr_reset;	//global connection list ptr
extern scaled_points_array_type *scaled_points_ptr;
int dyn_object_size;
ZObject test_obj;

//int size_of_GM;

_ZPoly_objs *temp_dynamic_polys_ptr;

	dyn_object_size=sizeof(test_obj);
	
//memory for static objects
	theSHdl = (StatObjectsFixedSizeHandle) NewHandleClear(sizeof(StatObjectsFixedSize));
	if (theSHdl!=0)
		{ 
		HLock((Handle)theSHdl);
		theSPtr=*theSHdl;
		ocb_Sptr =theSPtr;
		dynamic_object_top =MAX_NUMBER_OF_STAT_OBJECTS;
		}
	else
		report_error("Not enough memory (init_ocbs: StatObjectFixedSize).","\p",3);
//memory for fast dynamic objects
//SHow size of fast objects
	 #if ZEX_PROJECT_BUILDER==1 && SHOW_OCB_SIZE==1
	  {
	  int size_of_object=sizeof(DynFObjectsFixedSize);
       fprintf(stderr, " \n");
       fprintf(stderr, "Zex: FOCB storage is %d bytes\n",size_of_object);
      }
	#endif
	
	theFHdl = (DynFObjectsFixedSizeHandle) NewHandleClear(sizeof(DynFObjectsFixedSize));
	if (theFHdl!=0)
		{ 
		HLock((Handle)theFHdl);
		theFPtr=*theFHdl;
		Focb_ptr =theFPtr;
		for (i=0;i<MAX_NUMBER_OF_FAST_DYN_OBJECTS;i++)
		{
		(*Focb_ptr).Fobject_list[i].in_use=0;	//all not in use
		}
		}
	else
		report_error("Not enough memory (init_ocbs: DynFObjectsFixedSize).","\p",4);



//memory for particle dynamic objects
//SHow size of paticle objects
	 #if ZEX_PROJECT_BUILDER==1 && SHOW_OCB_SIZE==1
	  {
	  int size_of_object=sizeof(DynPObjectsFixedSize);
       fprintf(stderr, " \n");
       fprintf(stderr, "Zex: POCB storage is %d bytes\n",size_of_object);
      }
	#endif
	
	thePHdl = (DynPObjectsFixedSizeHandle) NewHandleClear(sizeof(DynPObjectsFixedSize));
	if (thePHdl!=0)
		{ 
		HLock((Handle)thePHdl);
		thePPtr=*thePHdl;
		Pocb_ptr =thePPtr;
		for (i=0;i<MAX_NUMBER_OF_PARTICLE_DYN_OBJECTS;i++)
		{
		(*Pocb_ptr).Pobject_list[i].previous=-1;	//all not in use
		(*Pocb_ptr).Pobject_list[i].next=-1;	//all not in use
		}
		}
	else
		report_error("Not enough memory (init_ocbs: DynFObjectsFixedSize).","\p",4);


//memory for dynamic objects
//SHow size of normal objects
	 #if ZEX_PROJECT_BUILDER==1 && SHOW_OCB_SIZE==1
	  {
	  int size_of_object=sizeof(DynObjectsFixedSize);
       fprintf(stderr, "Zex: OCB storage is %d bytes\n",size_of_object);
      }
	#endif
	theHdl = (DynObjectsFixedSizeHandle) NewHandleClear(sizeof(DynObjectsFixedSize));
	if (theHdl!=0)
		{ 
		HLock((Handle)theHdl);
		thePtr=*theHdl;
		ocb_ptr =thePtr;
		dynamic_object_top =MAX_NUMBER_OF_DYN_OBJECTS;
		for (i=0;i<MAX_NUMBER_OF_DYN_OBJECTS;i++)
		{
		(*ocb_ptr).object_list[i].in_use=0;	//all not in use
		}
		}
	else
		report_error("Not enough memory (init_ocbs: DynObjectFixedSize).","\p",3);

//memory for solar system
//	  size_of_SS=sizeof(solar_system);
//	  theSSHdl = (solar_system_handle) NewHandleClear(size_of_SS);
//	  if (theSSHdl!=0)
//	  	  { 
//	  	  HLock((Handle)theSSHdl);
//	  	  theSSPtr=*theSSHdl;
//	          SS_ptr =theSSPtr;
//	  	  }
//	  else
//	  	  report_error("Not enough memory (init_memory: solar system).","\p",3);

//memory for cmap
	temp_h=NewHandleClear(sizeof(cmap_objects));
	if (temp_h!=0)
		{ 
		HLock(temp_h);
		temp_ptr=*temp_h;
                #if IF_CPP_COMPILER__REMOVE_ME_AFTER_CPP_CONVERSION_COMPLETE
		cmap_ptr_base = static_cast<cmap_objects *>(temp_ptr);
                #else
		cmap_ptr_base =temp_ptr;
		#endif
		}
	else
		report_error("Not enough memory (init_ocbs: cmap).","\p",3);



//memory for scaled points
	temp_h=NewHandleClear(sizeof(scaled_points_array_type));
	if (temp_h!=0)
		{ 
		HLock(temp_h);
		temp_ptr=*temp_h;
                #if IF_CPP_COMPILER__REMOVE_ME_AFTER_CPP_CONVERSION_COMPLETE
		scaled_points_ptr= static_cast<scaled_points_array_type *>(temp_ptr);
                #else
		scaled_points_ptr=temp_ptr;
                #endif
		
		}
	else
		report_error("Not enough memory (init_ocbs: scaled points).","\p",3);	
		
		
		init_circle();			// do circle memory

//memory for points
	temp_h=(Handle)NewHandleClear(MAX_NUMBER_OF_STAT_OBJECTS*MAX_NUMBER_POLYS_PER_OBJ*3);
	if (temp_h!=0)
		{ 
		HLock(temp_h);
		temp_ptr=*temp_h;
                #if IF_CPP_COMPILER__REMOVE_ME_AFTER_CPP_CONVERSION_COMPLETE
		points_list_ptr = static_cast<_3D*>(temp_ptr);
                #else
		points_list_ptr =temp_ptr;
                #endif
		points_list_ptr_reset=points_list_ptr;	//so we can reset it
		points_list_top=(MAX_NUMBER_OF_STAT_OBJECTS*MAX_NUMBER_POLYS_PER_OBJ*3)/sizeof(_3D);
		}
	else
		report_error("Not enough memory (init_ocbs: points_list).","\p",3);

//memory for a polygon - for sys poly routine
	temp_h=(Handle)NewHandleClear(100);
	if (temp_h!=0)
		{ 
		Polygon_handle=(Polygon**)temp_h;
		HLock(temp_h);
		temp_ptr=*temp_h;
                #if IF_CPP_COMPILER__REMOVE_ME_AFTER_CPP_CONVERSION_COMPLETE
		Polygon_ptr = static_cast<Polygon*>(temp_ptr);
                #else
		Polygon_ptr =temp_ptr;
                #endif
		}
	else
		report_error("Not enough memory (init_ocbs: Polygon).","\p",3);

//memory for VISIBLE points
	temp_h=(Handle)NewHandleClear(MAX_NUMBER_OF_DYN_OBJECTS*MAX_NUMBER_POLYS_PER_OBJ*3);
	if (temp_h!=0)
		{ 
		HLock(temp_h);
		temp_ptr=*temp_h;
                #if IF_CPP_COMPILER__REMOVE_ME_AFTER_CPP_CONVERSION_COMPLETE
		visible_points_list_ptr = static_cast<visible_poly*>(temp_ptr);
                #else
		visible_points_list_ptr =temp_ptr;
                #endif
		visible_points_list_top=(MAX_NUMBER_OF_DYN_OBJECTS*MAX_NUMBER_POLYS_PER_OBJ*3)/sizeof(_3D);
		}
	else
		report_error("Not enough memory (init_ocbs: visible_points_list).","\p",3);


//memory for vieww points
	temp_h=(Handle)NewHandleClear(MAX_NUMBER_OF_DYN_OBJECTS*MAX_NUMBER_POLYS_PER_OBJ*3);
	if (temp_h!=0)
		{ 
		HLock(temp_h);
		temp_ptr=*temp_h;
                #if IF_CPP_COMPILER__REMOVE_ME_AFTER_CPP_CONVERSION_COMPLETE
		view_points_list_ptr = static_cast<_2DPoly*>(temp_ptr);
                #else
		view_points_list_ptr =temp_ptr;
                #endif
		view_points_list_top=(MAX_NUMBER_OF_DYN_OBJECTS*MAX_NUMBER_POLYS_PER_OBJ*3)/sizeof(_2D);
		}
	else
		report_error("Not enough memory (init_ocbs: view_points).","\p",3);

//memory for points_offsets
	temp_h=(Handle)NewHandleClear(300000);
	if (temp_h!=0)
		{ 
		HLock(temp_h);
		temp_ptr=*temp_h;
                #if IF_CPP_COMPILER__REMOVE_ME_AFTER_CPP_CONVERSION_COMPLETE
		points_offsets_ptr = static_cast<int*>(temp_ptr);
                #else
		points_offsets_ptr =(int*)temp_ptr;
                #endif
		points_offsets_top=300000/sizeof(int);
		}
	else
		report_error("Not enough memory (init_ocbs: points_offsets).","\p",3);

//memory for world points
	temp_h=(Handle)NewHandleClear(MAX_NUMBER_OF_DYN_OBJECTS*MAX_NUMBER_POLYS_PER_OBJ*3);
	if (temp_h!=0)
		{ 
		HLock(temp_h);
		temp_ptr=*temp_h;
                #if IF_CPP_COMPILER__REMOVE_ME_AFTER_CPP_CONVERSION_COMPLETE
		world_points_list_ptr = static_cast<_3D*>(temp_ptr);
                #else
		world_points_list_ptr =temp_ptr;
                #endif
		world_points_list_top=(MAX_NUMBER_OF_DYN_OBJECTS*MAX_NUMBER_POLYS_PER_OBJ*3)/sizeof(_3D);
		}
	else
		report_error("Not enough memory (init_ocbs: world_points_list).","\p",3);

//memory for temp points
//	temp_h=(Handle)NewHandleClear(300000);
//	if (temp_h!=0)
//		{ 
//		HLock(temp_h);
//		temp_ptr=*temp_h;
//		temp_points_list_ptr =temp_ptr;
////		temp_points_list_top=300000/sizeof(_3D);
//		}
//	else
//		report_error("Not enough memory (init_ocbs: temp_points_list).","\p",3);

//memory for connections
	temp_h=(Handle)NewHandleClear(300000);
	if (temp_h!=0)
		{ 
		HLock(temp_h);
		temp_ptr=*temp_h;
                #if IF_CPP_COMPILER__REMOVE_ME_AFTER_CPP_CONVERSION_COMPLETE
		connection_list_ptr = static_cast<textured_connections*>(temp_ptr);
                #else
		connection_list_ptr =temp_ptr;
                #endif
		connection_list_ptr_reset=connection_list_ptr;
		connection_list_top=connection_list_ptr+(300000/sizeof(textured_connections));
		}
	else
		report_error("Not enough memory (init_ocbs: connection_list).","\p",3);

//memory for static polys
	temp_h=(Handle)NewHandleClear(sizeof(_ZPoly)*MAX_NUMBER_OF_POLYS);
	if (temp_h!=0)
		{ 
		int temp;
		
		HLock(temp_h);
		temp_ptr=*temp_h;
                #if IF_CPP_COMPILER__REMOVE_ME_AFTER_CPP_CONVERSION_COMPLETE
		Spoly_list_ptr = static_cast<_ZPoly*>(temp_ptr);
                #else
		Spoly_list_ptr =temp_ptr;
                #endif
		Spoly_list_ptr_reset=Spoly_list_ptr;
		
		Spoly_list_top=Spoly_list_ptr+MAX_NUMBER_OF_POLYS;
		
		temp=Spoly_list_top-Spoly_list_ptr;
		}
	else
		report_error("Not enough memory (init_ocbs: poly_list).","\p",3);

//memory for dynamic polys
//SHow size of dynamic polys
	 #if ZEX_PROJECT_BUILDER==1 && SHOW_OCB_SIZE==1
	  {
	  int size_of_object=sizeof(_ZPoly_objs);
       fprintf(stderr, "Zex: Dyn Polys storage is %d bytes\n",size_of_object);
      }
	#endif

	temp_h=(Handle)NewHandleClear(sizeof(_ZPoly_objs));
	if (temp_h!=0)
		{ 
		HLock(temp_h);
		temp_ptr=*temp_h;
                #if IF_CPP_COMPILER__REMOVE_ME_AFTER_CPP_CONVERSION_COMPLETE
		dynamic_polys_ptr = static_cast<_ZPoly_objs *>(temp_ptr);
                #else
		dynamic_polys_ptr =temp_ptr;
                #endif
//set em all empty
		temp_dynamic_polys_ptr=dynamic_polys_ptr;

		for (i=0;i<MAX_NUMBER_OF_DYN_POLY_SLOTS;i++)
		{
		(*temp_dynamic_polys_ptr)[i].in_use=0;
		}

		}
	else
		report_error("Not enough memory (init_ocbs: dynamic_polys).","\p",3);



//memory for clipped polys
	temp_h=(Handle)NewHandleClear(sizeof(_clipped_poly)*MAX_NUMBER_OF_CLIPPED_POLYS);
	if (temp_h!=0)
		{ 
		//int temp;
		
		HLock(temp_h);
		temp_ptr=*temp_h;
                #if IF_CPP_COMPILER__REMOVE_ME_AFTER_CPP_CONVERSION_COMPLETE
		clipped_polys_ptr = static_cast<_clipped_poly*>(temp_ptr);
                #else
		clipped_polys_ptr =temp_ptr;
                #endif
		}
	else
		report_error("Not enough memory (init_ocbs: _clipped_polys).","\p",3);
	


points_list_ptr_base=points_list_ptr;
visible_points_list_base=visible_points_list_ptr;
dynamic_object_index=0;
static_object_index=0;

/*
insert_test_object(0,-90,-30,80,2,2,1);
insert_test_object(1,-30,30,120,1,1,2);
insert_test_object(2,30,-30,160,2,0,3);
insert_test_object(3,90,30,200,2,3,4);
insert_test_object(4,0,130,40,-1,-1,-1);
insert_test_object(5,0,-130,40,0,0,-4);

insert_test_object(6,-150,120,40,-1,-1,-1);
insert_test_object(7,150,-120,40,0,0,-4);

insert_test_object(8,150,-20,40,-3,-1,2);
insert_test_object(9,-150,20,40,0,5,0);

insert_test_object(10,150,120,40,-3,-1,2);
insert_test_object(11,-150,-120,40,0,5,0);

insert_test_object(12,100,180,140,0,0,-2);
insert_test_object(13,-100,-220,340,-1,-1,0);

insert_test_object(14,100,-220,240,4,1,-2);
insert_test_object(15,-150,220,240,3,3,1);
 */
return return_val;
}

