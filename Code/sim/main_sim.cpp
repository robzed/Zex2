// *************************************MODULE HEADER*********************************
// * FILENAME           : Untitled 4
// * MODULE TITLE       : Untitled 4
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 22:38:22 on 15th August 2001
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
$Log: main_sim.c,v $
Revision 1.5  2002/09/08 02:10:05  stu_c
Precompiled header adjustments

Revision 1.4  2002/04/28 06:22:45  stu
This checkin adds more to the HUD and brings out some sim variables to the player.

Revision 1.3  2001/11/05 22:54:09  stu
Removal of debugging code for frame #1 crash on PB

Revision 1.2  2001/11/05 19:57:38  stu
More OSX work

Revision 1.1  2001/11/02 19:32:08  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.0.0.1  2001/10/17 20:46:07  rob
First Imported.

Revision 1.1  2001/09/04 20:32:32  stu
Initial issue

*/

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "3d_structures_def.h"
#endif //pch

#include "main_sim.h"
#include "engines.h"
#include "autopilot.h"
#include "electrics.h"
#include "obj_control_misc_routines.h"


// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *




//This top level routine controls the simulation of all ships systems fitted
//as well as engine calcs and firing of weapons.
//In more detail:
//1. Thrust calcs
//2. Fuel calcs
//3. Handles items fired (from command_fired_flags
//4. Handles ships power calcs
//5. Handles other simulation of all other ships items fitted

void object_systems_sim(int object)
{
  update_mass(object);
  run_electrics(object);
  run_flight_computations(object);	//in AP
  run_AP(object);
  run_engines(object);	//thrust and fuel

}