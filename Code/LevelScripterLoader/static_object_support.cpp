/*
 * $Log: static_object_support.c,v $
 * Revision 1.1.1.1  2003/09/05 22:35:57  stu_c
 * First Imported.
 *
 * Revision 1.5  2002/09/08 02:13:33  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.4  2002/03/16 00:45:11  stu
 * Continued development
 *
 * Revision 1.3  2001/12/29 01:12:12  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.2  2001/11/02 19:32:15  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.1  2001/10/21 01:03:11  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:06  rob
 * First Imported.
 *
 * Revision 1.2  2001/04/01 18:30:04  stu
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2001/01/01 21:13:36  rob
 * First Imported.
 *
 * Revision 1.3  2000/12/03 23:52:14  rob
 * changed constants for object type to get uniformity
 *
 * Revision 1.2  2000/11/16 20:35:13  rob
 * orbit
 *
 * Revision 1.1  2000/09/20 23:47:54  stu
 * HUD/AP/Scaling
 *
 * Revision 1.0.0.1  2000/08/21 22:03:37  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <fp.h>
  #endif

  #include "3d_structures_def.h"
  #include "Error_handler.h"
#endif //pch

#include "static_object_support.h"
#include "meshtozex.h"
#include "load_objects.h"


//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//searches static objects for an id, if found returns the address of the object else 0
ZObject * get_static_object_pointer(unsigned long the_id)
{
extern	StatObjectsFixedSize *ocb_Sptr;
extern int	static_object_index;	//how many static objects
ZObject *return_val;

int match=0,index=0;
return_val=0;

while (match==0 && index<static_object_index)
 {
 if ((*ocb_Sptr).object_list[index].Oid==the_id) match=1;
 index++;
 }

if (match==1) return &(*ocb_Sptr).object_list[index-1];	//address of static definition of bg object
else return return_val;

}

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//searches static objects for an id, if found returns the slot of the object else -1
int get_static_object_slot(unsigned long the_id)
{
extern	StatObjectsFixedSize *ocb_Sptr;
extern int	static_object_index;	//how many static objects

int match=0,active_object_count, object_count;

object_count=active_object_count=0;
while (active_object_count<static_object_index)
{
  if ((*ocb_Sptr).object_list[object_count].in_use==1)
  {
   if ((*ocb_Sptr).object_list[object_count].Oid==the_id)
   {
     match=1;
     break;
   }
   active_object_count++;
 }
 object_count++;
}
 
 if (match==1) return object_count;	//slot of static definition of bg object
 else 
 return -1;

}


