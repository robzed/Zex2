// *************************************MODULE HEADER*********************************
// * FILENAME           : 3d_depth_of_field_manager.c
// * MODULE TITLE       : Handles setting up the screen correctly
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 23rd February 2002
// * FIRST AUTHOR       : Rob
// *
// * COPYRIGHT (c)        Lightsoft 2002
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************

#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <MacTypes.h>
    #include <MacMemory.h>
    #include <fp.h>
  #endif

  #include "3d_structures_def.h"
  #include "Error_handler.h"
#endif //pch

#include "3d_top.h"
#include "3d_depth_of_field_manager.h"

#define WIDEST_ANGLE_OF_VIEW 77		// in degrees

static double angle_of_view=WIDEST_ANGLE_OF_VIEW;	// private variable
double cos_angle_of_view;

extern int window_w, window_h;

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 23
// +
// | DESCRIPTION: Initialises the depth of field so that all view window resolutions
// |              get the same depth of field. Must be called after the view window
// |              size is know, but before any 3d calcs/calls etc
// +----------------------------------------------------------------ROUTINE HEADER----

void init_depth_of_field(void)
{
double largest_edge;


if(angle_of_view >= 180 || angle_of_view <= 0 )
  {
  MyDebugStr(__LINE__,"3d_depth_of_field_manager.c","Illegal angle of view");
  }
  
if(window_w<=50 || window_h<=50)
  {
  MyDebugStr(__LINE__,"3d_depth_of_field_manager.c","Window Too Small?");
  }
  
if(window_w > window_h)
  { largest_edge = window_w;}
else 
  { largest_edge = window_h; }
  
  
// We now have:
//
//
//     w
//   _________
//   \   |   /
//    \  |x /
//     \ | /
//      \|/
//       a       
//
// Where a is the total field of view angle, and is known (angle_of_view)
//       x is the unknown, and needs to be in the variable "depth_of_field"
//       w is the width of the window, and is inside the variable "largest_edge"
//
// tan a = w/x
//
// Therefore
//
//	w = x * tan (a/2), or x = w/tan (a/2)

depth_of_field = largest_edge / tan( (angle_of_view/2)*(PI/180) );

// also need to calculate ZMin - which is front clipping
ZMin = 0.8 * depth_of_field;
if(ZMin<=0) ZMin = 1;		// just incase - Zmin of zero would probably be a bad thing.

cos_angle_of_view=COS(angle_of_view);
}


void set_angle_of_view(int angle)
{
angle_of_view=angle;
}


int get_angle_of_view(void)
{
return angle_of_view;
}


double get_cos_of_angle_of_view(void)
{
return COS(angle_of_view);
}
