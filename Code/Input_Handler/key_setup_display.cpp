// *************************************MODULE HEADER*********************************
// * FILENAME           : key_setup_display.cpp
// * MODULE TITLE       : key_setup_display
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 20:33 on 17th December 2001
// * FIRST AUTHOR       : Rob
// *
// * COPYRIGHT (c)        LS 2001
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************


// ***********************************************************************************
// * INCLUDED FILES
// *

#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

  #include "Error_handler.h"
  #include "processor.h" 
  #include "zex_misc.h"
#endif /pch

#include "keyboard.h"


#include "pictures.h"
#include "OGL.h"
#include "key_setting.h"

// ***********************************************************************************
// * CONSTANTS 
// *
#define module_private static

#define NUMBER_OF_STARS 200

#define random_x() (RangedRdm(0,window_w))
#define random_y() (RangedRdm(0,window_h))
#define center_x() ((window_w-1)/2)
#define center_y() ((window_h-1)/2)

// ***********************************************************************************
// * TYPE DEFINITIONS
// *

typedef struct {
               float x,y;
               float speed_x,speed_y;
               float accel_x,accel_y;
               } star_type;

// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

Handle star_master_handle;

int star_direction;
int star_subtype;

// left/right/up/down must be first 4
#define STAR_LEFT 0
#define STAR_RIGHT 1
#define STAR_UP 2
#define STAR_DOWN 3
#define STAR_IN 4
#define STAR_IN2 5
#define STAR_IN3 6
#define STAR_OUT 7
#define STAR_OUT2 8
#define STAR_OUT3 9

#define NUM_STAR_TYPES 10

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *


// private functions
module_private Boolean is_star_off_screen(star_type *star_ptr);
module_private void plot_star(star_type *star_ptr);
module_private void move_star(star_type *star_ptr);
module_private void create_new_star(star_type *star_ptr);
module_private void reset_star(star_type *star_ptr);
module_private void set_random_velocity(star_type *star_ptr);
module_private void set_random_direction(void);


// ***********************************************************************************
// * ACTUAL CODE
// *

//
// done ---------- (a) Time equalise stars
// done ---------- (b) Randoms & Limits for stars - banding wrong
// (c) zoom in & out stars
// done ------------ (d) resolution independant speed

// +--------------------------------+-------------------------+-----------------------
// | TITLE: init_star_starts        | AUTHOR(s): Rob Probin   | DATE STARTED: 17/12/01
// +
// | DESCRIPTION:  Sets up the initial positions and directions of the stars.
// +----------------------------------------------------------------ROUTINE HEADER----

void init_screen_stars(void)
{
star_type *star_master_ptr;
int i;

star_master_handle=NewHandle(sizeof(star_type) * NUMBER_OF_STARS);
if(star_master_handle==0) MyDebugStr(__LINE__,"key_setup_display.c","Couldn't get any memory for stars");
i=MemError(); if(i) MyDebugStr(i,"MemError value = ","Memory Error for stars");

HLock(star_master_handle);

star_master_ptr=(star_type *)*star_master_handle;

set_random_direction();

for(i=0; i<NUMBER_OF_STARS; i++)
  {
  create_new_star(star_master_ptr+i);
  }

HUnlock(star_master_handle);
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: destroy_screen_stars    | AUTHOR(s): Rob Probin   | DATE STARTED: 17/12/01
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void destroy_screen_stars(void)
{
DisposeHandle(star_master_handle);
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: show_screen_stars       | AUTHOR(s): Rob Probin   | DATE STARTED: 17/12/01
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void show_screen_stars(void)
{
int i;
star_type *star_master_ptr;

HLock(star_master_handle);
star_master_ptr=(star_type *)*star_master_handle;

glBegin(GL_POINTS);

for(i=0; i<NUMBER_OF_STARS; i++)
  {
  move_star(star_master_ptr+i);
  
  if(is_star_off_screen(star_master_ptr+i))
    {
    reset_star(star_master_ptr+i);
    }
    
  plot_star(star_master_ptr+i);
  }


glEnd();
  
HUnlock(star_master_handle);
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: is_star_off_screen      | AUTHOR(s): Rob Probin   | DATE STARTED: 17/12/01
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

extern int window_w, window_h;

module_private Boolean is_star_off_screen(star_type *star_ptr)
{
float x; float y;

x=star_ptr->x; y=star_ptr->y;

  if(x>=window_w || x<0) return TRUE;
  if(y>=window_h || y<0) return TRUE;

if(star_direction==STAR_IN || star_direction==STAR_IN2 || star_direction==STAR_IN3)
  {
  if(x>=(center_x()-10) && x<=(center_x()+10)
     && y>=(center_y()-10) && y<=(center_y()+10))
    return TRUE;
  }
  
return FALSE;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: plot_star               | AUTHOR(s): Rob Probin   | DATE STARTED: 17/12/01
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

module_private void plot_star(star_type *star_ptr)
{
OGL_plot_rgb_quick((int)star_ptr->x,(int)star_ptr->y,1.0,1.0,1.0);
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: move_star               | AUTHOR(s): Rob Probin   | DATE STARTED: 17/12/01
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

module_private void move_star(star_type *star_ptr)
{
double actual_speed_per_frame_x;
double actual_speed_per_frame_y;

star_ptr->speed_x += star_ptr->accel_x * real_frame_time_in_secs;
star_ptr->speed_y += star_ptr->accel_y * real_frame_time_in_secs;

actual_speed_per_frame_x=(star_ptr->speed_x) * real_frame_time_in_secs;	// actual speed goes down as framerate goes up
actual_speed_per_frame_y=(star_ptr->speed_y) * real_frame_time_in_secs;	// actual speed goes down as framerate goes up

star_ptr->x += actual_speed_per_frame_x;
star_ptr->y += actual_speed_per_frame_y;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: create_new_star         | AUTHOR(s): Rob Probin   | DATE STARTED: 17/12/01
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----


module_private void create_new_star(star_type *star_ptr)
{
int x,y;

x=random_x(); y=random_y();
star_ptr->x=x; star_ptr->y=y;

set_random_velocity(star_ptr);
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: reset_star              | AUTHOR(s): Rob Probin   | DATE STARTED: 17/12/01
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

module_private void reset_star(star_type *star_ptr)
{
int temp_star_direction;
int x,y;


if(star_direction==STAR_OUT || star_direction==STAR_OUT2 || star_direction==STAR_OUT3)	// center stars
  {
  star_ptr->x=center_x()-10+RangedRdm(0,21);
  star_ptr->y=center_y()-10+RangedRdm(0,21);
  set_random_velocity(star_ptr);
  return;
  }
  
if(star_direction==STAR_IN || star_direction==STAR_IN2 || star_direction==STAR_IN3)
  {
  temp_star_direction=RangedRdm(0,4);	// stars must start at edges
  }
else	// any other type
  {
  temp_star_direction=star_direction;
  }

switch(temp_star_direction)
  {
  case STAR_LEFT:
                                        x=(window_w-1); y=random_y();
                                        star_ptr->x=x; star_ptr->y=y;
  					break;
  case STAR_RIGHT:
                                        x=0; y=random_y();
                                        star_ptr->x=x; star_ptr->y=y;
  					break;
  case STAR_UP:
                                        x=random_x(); y=(window_h-1);
                                        star_ptr->x=x; star_ptr->y=y;
  					break;
  case STAR_DOWN:
                                        x=random_x(); y=0;
                                        star_ptr->x=x; star_ptr->y=y;
  					break;
  }


set_random_velocity(star_ptr);  
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: set_random_velocity     | AUTHOR(s): Rob Probin   | DATE STARTED: 17/12/01
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

#define STAR_SPEED 10.0

module_private void set_random_velocity(star_type *star_ptr)
{
float speed;
float x,y,mag;

speed=((float)(1+RangedRdm(0,1000)))/STAR_SPEED;

speed *= ((float)window_w)/640.0;	// increase speed for bigger windows

// speed in pixels per second
// minimum is 1/10th of a pixel per second
// maximum is 100 pixels per second

switch(star_direction)
  {
  case STAR_LEFT:
                    star_ptr->speed_x = -speed;
                    star_ptr->speed_y = 0;
                    break;
  					
  case STAR_RIGHT:
                    star_ptr->speed_x = speed;
                    star_ptr->speed_y = 0;
                    break;
  					
  case STAR_UP:
                    star_ptr->speed_x = 0;
                    star_ptr->speed_y = -speed;
                    break;
  					
  case STAR_DOWN:
                    star_ptr->speed_x = 0;
                    star_ptr->speed_y = speed;
                    break;
                                        		
  case STAR_IN:
                    x=star_ptr->x-center_x(); y=star_ptr->y-center_y();
                    mag=sqrt(x*x+y*y);
                    star_ptr->speed_x = -speed * (x/mag);
                    star_ptr->speed_y = -speed * (y/mag);
                    star_ptr->accel_x = -0.3 * star_ptr-> speed_x;
                    star_ptr->accel_y = -0.3 * star_ptr-> speed_y;
                    break;
  case STAR_IN2:
                    x=star_ptr->x-center_x(); y=star_ptr->y-center_y();
                    mag=sqrt(x*x+y*y);
                    star_ptr->speed_x = -speed * (x/mag);
                    star_ptr->speed_y = -speed * (y/mag);
                    star_ptr->accel_x = 0.2 * star_ptr-> speed_x;
                    star_ptr->accel_y = 0.2 * star_ptr-> speed_y;
                    break;
  case STAR_IN3:
                    x=star_ptr->x-center_x(); y=star_ptr->y-center_y();
                    mag=sqrt(x*x+y*y);
                    star_ptr->speed_x = -speed * (x/mag);
                    star_ptr->speed_y = -speed * (y/mag);
                    star_ptr->accel_x = 0; // -0.1 * star_ptr-> speed_x;
                    star_ptr->accel_y = 0; // -0.1 * star_ptr-> speed_y;
                    break;

// We start with a positional vector, divide by the magnitude to normalise it, then multiply each 
// axis vector by the speed.
  case STAR_OUT:
                    x = star_ptr->x-center_x();  y = star_ptr->y-center_y();
                    if(x==0 && y==0) 
                        { x=random_x()-center_x(); 
                          y=random_y()-center_y(); }
                    if(x==0 && y==0) { x=10; }	// make sure not null vector ever
                    mag=sqrt(x*x+y*y);
                    star_ptr->speed_x = speed * (x/mag);
                    star_ptr->speed_y = speed * (y/mag);
                    star_ptr->accel_x = 0.1 * star_ptr->speed_x;
                    star_ptr->accel_y = 0.1 * star_ptr->speed_y;
                    break;

  case STAR_OUT2:
                    x = star_ptr->x-center_x();  y = star_ptr->y-center_y();
                    if(x==0 && y==0) 
                        { x=random_x()-center_x(); 
                          y=random_y()-center_y(); }
                    if(x==0 && y==0) { x=10; }	// make sure not null vector ever
                    mag=sqrt(x*x+y*y);
                    star_ptr->speed_x = speed * (x/mag);
                    star_ptr->speed_y = speed * (y/mag);
                    star_ptr->accel_x = -0.1 * star_ptr->speed_x;
                    star_ptr->accel_y = -0.1 * star_ptr->speed_y;
                    break;

  case STAR_OUT3:
                    x = star_ptr->x-center_x();  y = star_ptr->y-center_y();
                    if(x==0 && y==0) 
                        { x=random_x()-center_x(); 
                          y=random_y()-center_y(); }
                    if(x==0 && y==0) { x=10; }	// make sure not null vector ever
                    mag=sqrt(x*x+y*y);
                    star_ptr->speed_x = speed * (x/mag);
                    star_ptr->speed_y = speed * (y/mag);
                    star_ptr->accel_x = -0.3 * star_ptr->speed_x;
                    star_ptr->accel_y = -0.3 * star_ptr->speed_y;
                    break;

  }
  
  if(star_direction == STAR_LEFT || star_direction == STAR_RIGHT || 
     star_direction == STAR_UP   || star_direction == STAR_DOWN)
    {
    switch(star_subtype)
      {
      case 0:
        star_ptr->accel_x = 0.1 * star_ptr->speed_y;
        star_ptr->accel_y = 0.1 * star_ptr->speed_x;
        break;
      case 1:
        star_ptr->accel_x = -0.1 * star_ptr->speed_y;
        star_ptr->accel_y = -0.1 * star_ptr->speed_x;
        break;
      case 2:
        star_ptr->accel_x = 0.6 * star_ptr->speed_x;
        star_ptr->accel_y = 0.6 * star_ptr->speed_y;
        break;
      case 3:
        star_ptr->accel_x = -0.2 * star_ptr->speed_x;
        star_ptr->accel_y = -0.2 * star_ptr->speed_y;
        break;

      default:  // default = no accel set
        star_ptr->accel_x=0;
        star_ptr->accel_y=0;
        break;
      }
      
    }

}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: set_random_direction    | AUTHOR(s): Rob Probin   | DATE STARTED: 17/12/01
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

module_private void set_random_direction(void)
{
star_direction=RangedRdm(0,NUM_STAR_TYPES);
//star_direction = STAR_IN3;
star_subtype=RangedRdm(0,5);
}



// ***********************************************************************************
// * DESIGN NOTES
// *

// star scroll features
// ====================
// parallax scroll
// up/down, left/right, 
//
// from/to center - with or without rotate
// different distances
// 3d zooming - i.e. slow far away, fast near
//
// do we need all this detail just for a backdrop?
// probably not
//
//

//
// TECHNIQUES
//
// use fp positions and speeds.
//
//
