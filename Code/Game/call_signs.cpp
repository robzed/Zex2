/*
 *  call_signs.cpp
 *  zex22
 *
 *  Created by Stuart Ball on Fri Aug 30 2002.
 *  Copyright (c) 2002 Lightsoft Games. All rights reserved.
 *
 */
 
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

  #include "3d_structures_def.h"
  #include "zex_misc.h"
#endif //pch
 

#include "behaviour.h"
#include "call_signs.h"



//This little routine examines an object and creates a pseudo random callsign for it
//Callsigns are displayed as RACE ID,num1,num2,num3,num4 - a string of four ascii digits
//Your IFF (hud) will try to display the race ID as R for Ramparian, H for human
// A for Alycian and U for unknown.

void gen_callsign(int dyn_slot)
{
  //object NEEDS to be a ship or mothership
  register ZObject * the_object;
  extern	DynObjectsFixedSize *ocb_ptr;
  
  the_object=&ocb_ptr->object_list[dyn_slot];
  UInt8 allegiance=the_object->Dyn_OCB_control_data.allegiance;  
  UInt8 ship_class=the_object->Dyn_OCB_control_data.NPC_class;
  
  if (allegiance==HUMAN) { the_object->Dyn_OCB_control_data.callsign[0]='H'; }
  else
  if (allegiance==ALIEN) { the_object->Dyn_OCB_control_data.callsign[0]='R'; }
  else
  if (allegiance==NEUTRAL) { the_object->Dyn_OCB_control_data.callsign[0]='A'; }
  else
  { the_object->Dyn_OCB_control_data.callsign[0]='U'; }

  
  if (ship_class==NPC_CLASS_FIGHTER || ship_class==NPC_CLASS_TRADER || ship_class==NPC_CLASS_GUARD)
  {
    UInt32 random=NDRangedRdm(0,9999);
    ZNumToString4(random,&the_object->Dyn_OCB_control_data.callsign[1]);
  }
  else
  if (ship_class==NPC_CLASS_PIRATE)
  {
    the_object->Dyn_OCB_control_data.callsign[1]='x'; 
    the_object->Dyn_OCB_control_data.callsign[2]='x'; 
    the_object->Dyn_OCB_control_data.callsign[3]='x'; 
    the_object->Dyn_OCB_control_data.callsign[4]='x'; 
  }
  else //you have no class madam!
  {
    the_object->Dyn_OCB_control_data.callsign[0]='\0';
  }

the_object->Dyn_OCB_control_data.callsign[5]='\0';
  
}




