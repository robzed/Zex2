// *************************************MODULE HEADER*********************************
// * FILENAME           : Untitled 2
// * MODULE TITLE       : Untitled 2
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 00:21:58 on 26th August 2000
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
/* CVS bits
$Log: hud.h,v $
Revision 1.2  2002/02/17 06:20:34  stu
Added radar targetting

Revision 1.1  2001/12/14 19:43:42  stu
Docking

Revision 1.0.0.1  2001/10/17 20:46:06  rob
First Imported.

Revision 1.8  2001/10/04 22:47:15  stu
LAst set of changes before 2.2

Revision 1.7  2001/10/02 22:11:56  rob
Added Zex title so could locate in solar system
Made the distance print go up to large numbers
Added bounding boxes on solar system view
Added limits for scale on solar system view

Revision 1.6  2001/09/18 20:40:49  stu
More Sim/ hud numbers print

Revision 1.5  2001/09/04 20:36:42  stu
Sim stuff

Revision 1.4  2001/05/28 01:49:05  stu
280501

Revision 1.3  2001/04/02 17:28:30  rob
made various variables switch on and off-able from forth. These include dust, asteriods, hud, zbuffer.

Revision 1.2  2001/01/07 18:46:43  rob
Really poor code for hiding hud when doing long sessions from console. Needs checking and correcting, because I think the draw routines don't just draw....

Revision 1.1.1.1  2001/01/01 21:13:36  rob
First Imported.

Revision 1.6  2000/12/03 23:01:34  rob
fixed radar

Revision 1.5  2000/11/26 22:57:23  stu
Various.

Revision 1.4  2000/10/19 19:08:40  stu
Relative velocities, turbulence, planet names.

Revision 1.3  2000/09/20 23:54:15  stu
HUD/AP/Scaling

Revision 1.2  2000/09/13 20:36:58  stu
Changes for hud targetting and sun corona effects.

Revision 1.1  2000/08/27 01:27:24  stu
Initial Issue

*/
void run_hud(int the_object);
void init_HUD(int the_object);
void do_target(int the_object);
void do_radar_target(void);

void check_autopilotup(int object);
void check_autopilotdown(int object);
void disengage_AP(int object);

void calc_zexs_relative_velocity(void);
void check_target(void);
void print_distance_in_metres_3_digit_just(int x, int y, float value, float red, float green, float blue, int just);
int make_3_digit(double value, unsigned char *number_string);

extern int hud_drawn;
extern float misc_text_red, misc_text_green, misc_text_blue;

void set_targetted_for_object(int object, int targetted);

