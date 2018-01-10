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

// *************************************MODULE HEADER*********************************
// * FILENAME           : electrics.cpp
// * MODULE TITLE       : electrics
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 19:14:08 on 3rd September 2001
// * FIRST AUTHOR       : Stu
// *
// * COPYRIGHT (c)        LS
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************
/* CVS bits
$Log: electrics.cpp,v $
Revision 1.1.1.1  2003/09/05 22:36:20  stu_c
First Imported.

Revision 1.6  2002/09/08 02:09:54  stu_c
Precompiled header adjustments

Revision 1.5  2002/04/28 06:23:19  stu
This checkin adds more to the HUD and brings out some sim variables to the player.

Revision 1.4  2001/12/29 01:11:29  stu
Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.

Revision 1.3  2001/11/24 19:30:19  stu
*** empty log message ***

Revision 1.2  2001/11/02 19:32:07  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.1  2001/10/27 22:39:04  stu
zplatform included

Revision 1.0.0.1  2001/10/17 20:46:07  rob
First Imported.

Revision 1.1  2001/09/04 20:32:10  stu
Initial issue

*/

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "LS_Standard.h"
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

  #include "3d_structures_def.h"
  #include "access_ocbs.h"
#endif //pch

#include "electrics.h"

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *
extern DynObjectsFixedSize *ocb_ptr;


// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *



int is_dc_good (int the_object)
{
 if((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.power_flags&DC_GOOD)
 return 1;
 else
 return  0;
}

int is_ac_good (int the_object)
{
 if((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.power_flags&AC_GOOD)
 return 1;
 else
 return 0;
}


void run_electrics (int the_object)
{
  (*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.power_flags=DC_GOOD+AC_GOOD;
}


float get_dc_volts(int the_object)
{

 if ((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.power_flags&DC_GOOD)
  {
  float flow;
   //dc volts varies between 27 and 33 as a function of fuel flow; where 2400mg/s is max
   flow=get_total_fuel_consumption(the_object)*1000000.0;
   if (flow>2400) flow=2400;
   flow/=400;
   return 27+flow;
  }
  else 
  return 0.0; 
}

float get_ac_volts(int the_object)
{

 if ((*ocb_ptr).object_list[the_object].Dyn_OCB_control_data.power_flags&AC_GOOD)
  {
  float flow;
   //ac volts varies between 413 and 427 as a function of fuel flow; where 2400mg/s is max (ish)
   flow=get_total_fuel_consumption(the_object)*1000000.0;
   if (flow>2400) flow=2400;
   flow/=180;
   return 413+flow;
  }
  else 
  return 0.0; 
}