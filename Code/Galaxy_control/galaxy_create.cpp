// *************************************MODULE HEADER*********************************
// * FILENAME           : galaxy_create.cpp
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * DATE STARTED       : 20:38:00 on 12th November 2001
// * FIRST AUTHOR       : Rob Probin
// *
// * COPYRIGHT (c)        2001 Lightsoft
// *
// ***********************************************************************************
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "options.h"
  #include "LS_Standard.h"

  #include "3d_structures_def.h"
  #include	"zex_misc.h"
  #include "Error_handler.h"
  #include "processor.h"
  #include "access_ocbs.h"
#endif //pch

#include "galaxy.h"

#include "objects_controls.h"
#include "game_defs.h"
#include "file_io.h"
#include "pictures.h"
#include	"splat_control.h"
#include	"txtsplat.h"

#include "ship_control.h"
#include "game_pause.h"
#include "object_control_top.h"
#include "camera_control.h"
#include "dlp.h"
#include "inventory.h"
#include    "fpoly4.h"
#include "genpoly.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "planet_generator.h"
#include "instrum.h"
#include "obj_control_misc_routines.h"
#include "load_objects.h"
#include "Zsound.h"
#include "physics.h"
#include "light_control.h"
#include "texture_management.h"
#include "physics_gravity.h"
#include "objects_misc.h"

#include "hud.h"
#include "dlp.h"

#include "olg_text.h"
#include "ogl.h"

#include "keyboard.h"
// ***********************************************************************************
// * CONSTANTS 
// *

#define NUMBER_OF_SYSTEMS 320
#define module_private static

#define MAX_NUM_MOTHERSHIPS 8

#define NO_MOTHERSHIP      0
#define HUMAN_MOTHERSHIP   1
#define RMP_MOTHERSHIP     2
#define NEUTRAL_MOTHERSHIP 3

// ***********************************************************************************
// * TYPE DEFINITIONS
// *

// Saved game: 32 bytes per solar system
// ==========
// 8*2bits: mothership status - 0:destroyed/doesn't exist, 1=human, 2=rmp, 3=neutral
//
// 8bits: number rmp raiders
// 8bits: number human raiders
// 8bits: number rmp traders
// 8bits: number human traders
//
// total 32 bytes

typedef struct
  {
  unsigned char rmp_raiders;
  unsigned char rmp_traders;
  unsigned char human_raiders;
  unsigned char human_traders;
  
  unsigned short mothership_status;	// 2 bits times 8 motherships (max)

  unsigned short more_spare;  
  long int sparea;
  long int spareb;
  
  // next 16 bytes
  long int spare1;
  long int spare2;
  long int spare3;
  long int spare4;
  } system_struct_type;

// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

// This variable contains the data stored as a saved file
module_private system_struct_type system_data[NUMBER_OF_SYSTEMS];


// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

void check_system_num(int system);
int total_number_of_systems(void);
void get_main_star_name(int system_num, char *name_string);
int fetch_mothership(int mothership_num,int system);
int get_number_of_motherships(int system_num);
int get_number_of_rmp_motherships(int system_num);



// ***********************************************************************************
// * NOTES
// *

//
// How many systems
//


// Seed System: 
// 1 seed generates: X systems.
//  Each system has a number of random characteristics associated with it...
//   (a) position x, y, z
//   (b) solar system construction - single, binary sun
//   (c) name of sun, planets and moons
//   (d) number of motherships, raiders and traders
//   (c) random size, texture, initial rotation around and distance away from body and 
//             mass of body.
// 

// Modules required:
//
// Easy access to galaxy variables either from saved files, or from
//
// All planets accessed off 
//

//
//
// want to make it so we can find specific items very easily.
//
//



// ***********************************************************************************
// * ACTUAL CODE
// *



/* check_system_num
 *
 * DESCRIPTION: 
 *
 */

void check_system_num(int system)
{
if(system<0 || system>=NUMBER_OF_SYSTEMS)
  {
  MyDebugStr(__LINE__,"galaxy_create.c/check_system_num","System Number out of range!");
  }
}

/* total_number_of_systems
 *
 * DESCRIPTION: 
 *
 */

int total_number_of_systems(void)
{
return NUMBER_OF_SYSTEMS;
}


/* get_main_star_name
 *
 * DESCRIPTION: 
 *
 */

void get_main_star_name(int system_num, char *name_string)
{
check_system_num(system_num);
*name_string='\0';
}



/* get_number_of_motherships
 *
 * DESCRIPTION: 
 *
 */

int fetch_mothership(int mothership_num,int system)
{
check_system_num(system);

if(mothership_num<0 || mothership_num>=MAX_NUM_MOTHERSHIPS)
  { MyDebugStr(__LINE__,"galaxy_create.c/check_system_num","System Number out of range!"); }


return 0x03 & ((system_data[system].mothership_status)>>(mothership_num<<1));
}

/* get_number_of_motherships
 *
 * DESCRIPTION: 
 *
 */

int get_number_of_motherships(int system_num)
{
int i;
int mothership_count;

mothership_count=0;

for( i=0; i<MAX_NUM_MOTHERSHIPS; i++)
  {
  if(fetch_mothership(i,system_num)) mothership_count++;
  }
  
return mothership_count;
}


/* get_number_of_rmp_motherships
 *
 * DESCRIPTION: 
 *
 */

int get_number_of_rmp_motherships(int system_num)
{
int i;
int mothership_count;

mothership_count=0;

for( i=0; i<MAX_NUM_MOTHERSHIPS; i++)
  {
  if(fetch_mothership(i,system_num)==RMP_MOTHERSHIP) mothership_count++;
  }
  
return mothership_count;
}



/* init_game_system_data
 *
 * DESCRIPTION: 
 *
 */

//void init_game_system_data(void)
//{
//  {
  
//  }
//}
