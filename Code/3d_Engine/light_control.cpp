// *************************************MODULE HEADER*********************************
// * FILENAME           : light_control.c
// * MODULE TITLE       : Lighting
// * PROJECT            : Zex
// * DATE STARTED       : 22:55:43 on 22nd June 1999
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************
/*
 * $Log: light_control.c,v $
 * Revision 1.1.1.1  2003/09/05 22:35:11  stu_c
 * First Imported.
 *
 * Revision 1.5  2002/09/08 02:12:31  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.4  2002/01/29 22:53:11  rob
 * pobs change
 *
 * Revision 1.3  2002/01/20 16:47:20  stu
 * Added set_black_level function.
 *
 * Revision 1.2  2001/11/02 19:32:45  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.1  2001/10/21 01:05:06  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:04  rob
 * First Imported.
 *
 * Revision 1.2  2001/01/14 17:17:00  stu
 * Gourad Fix
 *
 * Revision 1.1.1.1  2001/01/01 21:13:34  rob
 * First Imported.
 *
 * Revision 1.6  2000/12/11 19:45:24  stu
 * Light fix in 3d_top - calc_normal
 *
 * Revision 1.5  2000/12/10 03:30:34  stu
 * New dynamic light. Work in progress
 *
 * Revision 1.4  2000/09/20 23:54:31  stu
 * HUD/AP/Scaling
 *
 * Revision 1.3  2000/08/27 21:33:42  stu
 * module replaced with vec_normalize
 *
 * Revision 1.2  2000/08/27 19:51:00  rob
 * Messed up file.... opps
 *
 * Revision 1.1  2000/08/27 19:41:22  rob
 * Removed light movement code
 *
 * Revision 1.0.0.1  2000/08/21 21:59:41  stu
 * First Imported.
 *
 */


/*
Zex lighting. 220699 SB
An array of omni lights
*/


// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "3d_structures_def.h"
#endif //pch

#include "light_control.h"
#include "3d_top.h"
#include "matrix_primitives.h"
// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *
int reset_brightness=25000;


typedef struct 
{
int in_use;
double x,y,z;
int red,green,blue;
int brightness;
} light;

light lights[MAX_NUMBER_OF_OMNILIGHTS];



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *



// ***********************************************************************************
// * Code
// *


//how to set up a light in Zex.
//1. Set the light position
//2. Either call set_light_default_brightness or call set_light_brightness


//NOTE Turn of LIGHTS_PARAM_CHECK when happy your code runs Okay
//All functions (except init) will return OK when this define is 0 else -1 on error
#define LIGHTS_PARAMS_CHECK 1


void init_lights(void)
{
int n;

for (n=0;n<MAX_NUMBER_OF_OMNILIGHTS;n++)
 {
  lights[n].in_use=0;
  lights[n].brightness=25000;
 }
}



//new 101200
//light number runs 0 to MAX_NUMBER_OF_OMNILIGHTS-1
int set_light_position(int light_number, double position_x, double position_y, double position_z)
{
#if LIGHTS_PARAMS_CHECK==1
if (light_number>MAX_NUMBER_OF_OMNILIGHTS-1) return -1;
#endif

lights[light_number].x=position_x;
lights[light_number].y=position_y;
lights[light_number].z=position_z;
lights[light_number].in_use=1;

return 0;
}



int get_light_position(int light_number, _3D *return_val)
{
#if LIGHTS_PARAMS_CHECK==1
if (light_number>MAX_NUMBER_OF_OMNILIGHTS-1) return -1;
#endif
return_val->x=lights[light_number].x;
return_val->y=lights[light_number].y;
return_val->z=lights[light_number].z;

return 0;
}


int get_light_brightness(int light_number, int *return_val)
{
#if LIGHTS_PARAMS_CHECK==1
if (light_number>MAX_NUMBER_OF_OMNILIGHTS-1) return -1;
#endif
*return_val=lights[light_number].brightness;
return 0;
}

int set_light_brightness(int light_number, int brightness)
{
#if LIGHTS_PARAMS_CHECK==1
if (light_number>MAX_NUMBER_OF_OMNILIGHTS-1) return -1;
#endif
lights[light_number].brightness=brightness;
return 0;
}


int set_light_default_brightness(int light_number)
{
#if LIGHTS_PARAMS_CHECK==1
if (light_number>MAX_NUMBER_OF_OMNILIGHTS-1) return -1;
#endif
lights[light_number].brightness=45000;
return 0;
}


//3d functions
//does all the whizzy bits. Returns two things
//1. The effective light normal to this object. If there is more than 1 light source
//   this is averaged out (for speed - should be good enough)
//2. The effective light amplitude. If there's more than 1 light source
//   this is the addition of the light amplitudes.
//   NOTE: There is no attenuation for distance yet.

void calculate_light_normal(int object, _3D* light_normal, int *light_amplitude)
{
extern DynObjectsFixedSize *ocb_ptr;
register ZObject * current_object_ptr;
vector light_vector;
//get ptr to object
current_object_ptr=&ocb_ptr->object_list[object];

//calculate vector from light source to object
light_vector.x=lights[0].x-current_object_ptr->Oworldx;
light_vector.y=lights[0].y-current_object_ptr->Oworldy;
light_vector.z=lights[0].z-current_object_ptr->Oworldz;

//normalise
Vnormalise (&light_vector);

//return
light_normal->x= light_vector.x;
light_normal->y= light_vector.y;
light_normal->z= light_vector.z;
*light_amplitude=lights[0].brightness;
}	

int black_level=-32700;
void set_black_level(int the_black_level)	//0=black, 32767=white
{
 black_level=the_black_level;
 #if ZEX_PROJECT_BUILDER==1 && PB_PRINT_TO_LOG_ENABLED
 fprintf(stderr,"Lighting: Black Level set to %i\n",the_black_level);
 #endif
}
