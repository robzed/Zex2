
//drawing.c
/*
 * $Log: drawing.c,v $
 * Revision 1.4  2003/09/26 19:20:46  robp
 * Alteration for C++ const means internal unless specified extern. Made header extern so that C compiler wouldn't complain.
 *
 * Revision 1.3  2003/09/22 21:44:22  robp
 * Box of Curios 1: Code tidy up things, which weren't strictly incorrect, just weird. No functional changes. Examples include a block of variables that were declared twice. These things were all thrown up by the C++ compiler.
 *
 * Revision 1.2  2003/09/20 12:56:56  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:35:17  stu_c
 * First Imported.
 *
 * Revision 1.15  2002/09/08 01:57:23  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.14  2002/06/24 21:36:13  stu_c
 * Removed screen coord to ogl +-1 mapping code.
 *
 * Revision 1.13  2002/03/16 00:45:24  stu
 * Continued development
 *
 * Revision 1.12  2002/02/21 22:51:14  stu
 * Background colour now black (not dark grey)
 *
 * Revision 1.11  2002/02/10 21:52:56  stu
 * texture binding counting
 *
 * Revision 1.10  2002/02/03 19:31:43  stu
 * Planet labels now in same colour as blobbed object
 *
 * Revision 1.9  2002/02/02 23:07:14  stu
 * Changes for LOD on planets/labels
 *
 * Revision 1.8  2002/01/29 23:15:18  stu
 * Planet names now aligned better
 *
 * Revision 1.7  2002/01/20 21:37:40  stu
 * Changes to get it building on Anvil
 *
 * Revision 1.6  2002/01/20 17:05:11  stu
 * erase colour now setable, planet labels.
 *
 * Revision 1.5  2001/12/29 01:11:16  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.4  2001/11/02 19:32:04  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.3  2001/10/23 21:56:15  stu
 * More carbon work
 *
 * Revision 1.2  2001/10/22 21:28:44  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:01:58  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:04  rob
 * First Imported.
 *
 * Revision 1.10  2001/06/23 20:05:11  stu
 * 0.66 checkin
 *
 * Revision 1.9  2001/06/01 21:51:20  stu
 * Lurkers clear up
 *
 * Revision 1.8  2001/05/29 20:43:16  stu
 * Removed zbuffer debugging code that was booked in on last commit.
D'oh!
 *
 * Revision 1.7  2001/05/28 20:35:46  stu
 * 280501
 *
 * Revision 1.6  2001/05/28 01:45:02  stu
 * 280501
 *
 * Revision 1.5  2001/04/29 20:55:43  stu
 * Z Clipping
 *
 * Revision 1.4  2001/04/02 17:28:25  rob
 * made various variables switch on and off-able from forth. These include dust, asteriods, hud, zbuffer.
 *
 * Revision 1.3  2001/04/01 18:30:15  stu
 * *** empty log message ***
 *
 * Revision 1.2  2001/01/14 17:12:05  stu
 * Gourad Fix
 *
 * Revision 1.1.1.1  2001/01/01 21:13:37  rob
 * First Imported.
 *
 * Revision 1.9  2000/12/11 22:28:15  stu
 * BIG BUG FIX For jerky redraws - the gamma fading
was geting called every bloody frame!!!!!!!

D'OH
 *
 * Revision 1.8  2000/11/28 22:29:33  stu
 * Dunno.
 *
 * Revision 1.7  2000/10/19 19:04:07  stu
 * Relative velocities, turbulence, planet names.
 *
 * Revision 1.6  2000/10/11 21:30:31  stu
 * Finished off 64 bit conversion. Made engines bigger.
AP tweaks.
 *
 * Revision 1.5  2000/10/08 21:44:24  stu
 * Engine conversion to 64 bit. Bitch of a job.
 *
 * Revision 1.4  2000/09/30 20:01:56  stu
 * Solar System Map changes and autopilot combat mode.
 *
 * Revision 1.3  2000/09/20 23:55:54  stu
 * HUD/AP/Scaling
 *
 * Revision 1.2  2000/09/13 20:31:44  stu
 * Changes for hud targetting and sun corona effects.
 *
 * Revision 1.1  2000/09/02 13:26:11  stu
 * Lurker clean up
 *
 * Revision 1.0.0.1  2000/08/21 22:06:20  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include    "LS_Standard.h"
  #include "options.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include	<types.h>
    #include    <QuickDraw.h>
  #endif
  
  #include "3d_structures_def.h"
  #include "zex_misc.h"
#endif //pch


#include	"splat_control.h"
#include	"Drawing.h"
#include "genpoly.h"
#include    "fpoly4.h"
#include "txtsplat.h"
#include "pictures.h"
#include "file_io.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "dlp.h"
#include "Physics.h"

#if OGL
#include "ogl.h"
#endif

Ptr screenaddr;
int screenwidth;

int old_flare_brightness=0;
int rendering = -1;

//CGrafPtr the_drawing_buffer;
extern int flare_brightness;
extern int heat_brightness;	//handle_planet in object_control_top
extern int binds;

RGBColor	rgbWhite1 = { 0xffff, 0xffff, 0xffff};
RGBColor	rgbRed1= { 0xffff, 0x0, 0x0};
//Main poly draw routine
void Draw_it(int width, int height) 
{
//extern Ptr screenaddr;
//extern int screenwidth;
//extern GDHandle displaydevice;
//extern LSRAW *bpic_ptr;
//extern int have_bpic;
extern int flash_screen;
extern int training_mission, pilot_asleep;
//extern int gShowGalacticMap;
extern int use_zbuffer;
//extern int zbuffer_active;
//extern float zbuffer_range;

//CGrafPtr the_drawing_buffer;

//Rect action_rect;	//the rect we draw into - the bottom of the screen is the control panel


//GrafPtr		savePort;
//GDHandle	saveGDH;


//extern LSRAW* raw_rgba_data_CP;	//ptr to rgba control panel data



    if (flash_screen==0 )
	
	{
	extern int ship_buffet;
	
	if (ship_buffet==0)
	 {
	 glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	//black
	 }
	 else
	 {
	 float blue;
	 
	 blue=(float)ship_buffet;
	 blue/=40000.0;
	 blue+=0.03;
	 glClearColor(0.03f, 0.03f, blue, 1.0f);	//black	
	 }
	}
//    erase_zex_rect(width,height);	//in pictures
    else
    if (flash_screen>1)
    {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//white
    flash_screen--;
    }
    else
    if (flash_screen==1)
    {
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);	//red
    flash_screen--;
    }

//glClear(GL_COLOR_BUFFER_BIT);

if(use_zbuffer!=0)
{
         glDisable(GL_DEPTH_TEST);	//enabled in plot_pts when z <1000000
         glDepthFunc (GL_LESS);	//accept pixel if z is less than current
	//glClearDepth(.01);

// glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

} 
else
    glClear(GL_COLOR_BUFFER_BIT);
  


if(!rendering) return;


if (training_mission==1 || pilot_asleep==0)
    {
    plot_pts();
//    OGL_draw_pict(0, CONTROL_PANEL_TOP+(480-CONTROL_PANEL_TOP), 
//            (LSRAW *) raw_rgba_data_CP, 640, 480-CONTROL_PANEL_TOP);
         glDisable(GL_DEPTH_TEST);

    }
        display_text_display();	//show any text message


}



//Main poly draw routine
void Draw_it_no_erase(int width, int height) 
{
//extern Ptr screenaddr;
//extern int screenwidth;
//extern GDHandle displaydevice;
//extern LSRAW *bpic_ptr;
//extern int have_bpic;

//CGrafPtr the_drawing_buffer;



    plot_pts();

    display_text_display();	//show any text message


}

//Main poly draw routine
void Draw_it_colour_bg(int width,int height, int colour) 
{
//extern Ptr screenaddr;
//extern int screenwidth;
//extern GDHandle displaydevice;
//extern LSRAW *bpic_ptr;
//extern int have_bpic;

//CGrafPtr the_drawing_buffer;



    
    erase_zex_rect_colour(width,height,colour);	//white

    plot_pts();

    display_text_display();	//show any text message


}

//еееееееееееееееееееееееееееееееееееееееееееееееееееееее
//heavy modification at 080399 to run dlp at ship z time
//so lasers and thingsdont overdraw other things and look all yucky
//furthest point is greatest z, nearest point is zero

//version 3 has to work from clipped_output_ptr for number_of_clipped_polys
#if OGL==1
void plot_pts()
{
//extern _2DPoly *view_points_list_ptr;
//extern	int number_of_polys_displayed;
//extern _ZPolySort zsort_list[MAX_NUMBER_OF_POLYS];	//max of 2000 polys
//extern int zsort_index;
//extern Ptr screenaddr;
//extern int screenwidth;
//extern double zex_z;	 //where z is so we know when to draw the dlp
extern DynObjectsFixedSize *ocb_ptr;

//_2DPoly *drawing_info;
//_2DPoly *view_points;
//int poly_count;
//int startY;
//_ZPoly *the_poly_info;
//_2D point1,point2,point3;
_2D *point_list;
_2D points[10];
//_2D temppoints[10];
//_2D outpoints[10];
_float2D foutpoints[10];

//texture mapping I/F - SB 020199
dcoord *tpoint_list;
dcoord tpoints[10];
//dcoord ttemp_points[10];
//dcoord toutput_points[30];
fdcoord ftoutput_points[30];	 //float temp out points (OGL)
//int tpolys;
//textured_connections *pconnections;
int point_u,point_v,point_l;

//fpoly I/F
int output_lines;
//short fpoly_colour;
//RGBColor RGB;
int light_value,temp_red,temp_blue,temp_green;
//extern int zsort_start_index;	//top of polys linked list
//int poly_index;
int poly_type, translucent;
//int done_dlp=0;
extern char * taddr;
//extern int use_zbuffer;
double z_div;
//extern  _clipped_poly* clipped_polys_ptr;
int number_of_clipped_polys;

//extern int OGL_active;
extern int zbuffer_active;
     float fpoly_colour_r,fpoly_colour_g,fpoly_colour_b;
	  dcoord * op;	      //x,y,u,v
	  fdcoord * fop;	//x,y,u,v
          int i;
int poly_count;

extern clipped_polys_drawing_info_type clipped_polys_drawing_info;

_clipped_poly* clipped_input_ptr;

//run through zsort_list, drawing the polys
//poly_index=zsort_start_index;

//the_poly_info=zsort_list[poly_index].poly_data;	//get us going

//if z buffer active, and Z is less  than zbuffer_range then we switch on the zbuffer
zbuffer_active=0;
z_div=1;

number_of_clipped_polys=clipped_polys_drawing_info.number_of_clipped_polys_to_draw;
clipped_input_ptr=clipped_polys_drawing_info.clipped_polys_ptr;

//        glMatrixMode(GL_PROJECTION);
//        glLoadIdentity();

//           glOrtho(-1.0f, (GLfloat) 1/*window_w*/, -1.0f, (GLfloat) 1 /*window_h*/, 0.0f, -1.0f);

if (number_of_clipped_polys==0) return;

#if ENG_DEBUG_EXPORT==1
binds=0;                   //for debugging
#endif

for (poly_count=0; poly_count<number_of_clipped_polys; poly_count++)
//while (the_poly_info!=0)
{    
//    the_poly_info=zsort_list[poly_index].poly_data;	  //this poly
//    if (the_poly_info!=0)	//prevent those pesky access faults
//    {
//     poly_index=zsort_list[poly_index].next_slot;
    
//     drawing_info=view_points_list_ptr+(*the_poly_info).PPoints_offset;
    
//      pconnections=(*the_poly_info).poly_connections_ptr;   
//      translucent=(*the_poly_info).draw_code;  
      translucent=clipped_input_ptr->draw_code;  

//      tpoint_list=tpoints;      

//      poly_type=(*pconnections).type;
     poly_type=clipped_input_ptr->type&3;	//mask off gourad and texture
     
//test for a z < 10Km's, if found turn on zbuffering
//     if(clipped_input_ptr->use_zbuffer!=0)
//     {

     
        if(poly_count==clipped_polys_drawing_info.switch_zbuffer_on_at_poly_number)	//only do this once per frame (Stu!)
        {
           z_div=clipped_polys_drawing_info.z_divisor_for_zbuffer;		     	//true average for three points
           zbuffer_active=1;
           glEnable(GL_DEPTH_TEST);	//switched off at the end of draw_it
           glDepthFunc (GL_LESS);	//accept pixel if z is less than current   
//        glMatrixMode(GL_PROJECTION);
//        glLoadIdentity();
//           glOrtho(-1.0f, (GLfloat) 1/*window_w*/, -1.0f, (GLfloat) 1 /*window_h*/, -0.01f, -1.0f);
    
        }
        
        //if((zbuffer_active==1) && (clipped_input_ptr->average_z>z_div)) Debugger();

//     }
//-----------START HARD CODE-----------//

   if((clipped_input_ptr->polyflags&DUST_BIT) || (clipped_input_ptr->polyflags&AUTOBLOB_BIT))	//optimised dust and blobbed planets
   {
   //get owner
   int poly_owner;
   poly_owner=clipped_input_ptr->owner;
   fpoly_colour_r=ocb_ptr->object_list[poly_owner].blobbed_colour_red;
   fpoly_colour_g=ocb_ptr->object_list[poly_owner].blobbed_colour_green;
   fpoly_colour_b=ocb_ptr->object_list[poly_owner].blobbed_colour_blue;
//     fpoly_colour_r=fpoly_colour_g=fpoly_colour_b=1;	  //0-1
     OGL_plot_rgb(clipped_input_ptr->screen_pts[0].x, clipped_input_ptr->screen_pts[0].y, fpoly_colour_r, fpoly_colour_g, fpoly_colour_b);
   }
   else
   if(clipped_input_ptr->polyflags&PLANET_BIT)	  //optimised dust and blobbed planets
   {
   //get owner
   int poly_owner;
   char temp_name_str[256];
			 int str_len, str_y;
   
   poly_owner=clipped_input_ptr->owner;
   fpoly_colour_r=ocb_ptr->object_list[poly_owner].blobbed_colour_red;
   fpoly_colour_g=ocb_ptr->object_list[poly_owner].blobbed_colour_green;
   fpoly_colour_b=ocb_ptr->object_list[poly_owner].blobbed_colour_blue;

     OGL_line_rgb(clipped_input_ptr->screen_pts[0].x, clipped_input_ptr->screen_pts[0].y-1,clipped_input_ptr->screen_pts[0].x+2, clipped_input_ptr->screen_pts[0].y-1,
                  fpoly_colour_r, fpoly_colour_g, fpoly_colour_b);
     OGL_line_rgb(clipped_input_ptr->screen_pts[0].x-1, clipped_input_ptr->screen_pts[0].y,clipped_input_ptr->screen_pts[0].x+3, clipped_input_ptr->screen_pts[0].y,
                  fpoly_colour_r, fpoly_colour_g, fpoly_colour_b);
     OGL_line_rgb(clipped_input_ptr->screen_pts[0].x, clipped_input_ptr->screen_pts[0].y+1,clipped_input_ptr->screen_pts[0].x+2, clipped_input_ptr->screen_pts[0].y+1,
                  fpoly_colour_r, fpoly_colour_g, fpoly_colour_b);

//draw planet name - 140102
               	//get name from object - stored as pascal string
 	         PascalToC ((*ocb_ptr).object_list[poly_owner].Dyn_OCB_control_data.name_str,temp_name_str);
	        str_len=(*ocb_ptr).object_list[poly_owner].Dyn_OCB_control_data.name_str[0];
			 if (str_len&1) str_y=-10; else str_y=10;
			 
			 if (monitor_w-clipped_input_ptr->screen_pts[0].x>100)
			 {
			             SplatText_Small_rgb(temp_name_str,clipped_input_ptr->screen_pts[0].x+7, clipped_input_ptr->screen_pts[0].y+str_y, fpoly_colour_r, fpoly_colour_g, fpoly_colour_b);
             }
			 else
             {
						 str_len*=7;
						 
			 			 SplatText_Small_rgb(temp_name_str,clipped_input_ptr->screen_pts[0].x-str_len, clipped_input_ptr->screen_pts[0].y+str_y, fpoly_colour_r, fpoly_colour_g, fpoly_colour_b);
			 }     
   }
   else

   {	//not blobbed

     if (poly_type==GOURADTEXTURE)	//Fuck, it's texture mapped. Don't Panic boys!!!
     {
     int dmf_flag;
     float translucency;
     
//      pconnections=(*the_poly_info).poly_connections_ptr;     
      tpoint_list=tpoints;
      dmf_flag=clipped_input_ptr->type&DMF;
      translucency=clipped_input_ptr->transparency;	//0=transparent, 1=opaque
      
      
      point_u=clipped_input_ptr->texture_pts[0].u;
      point_v=clipped_input_ptr->texture_pts[0].v;
      point_l=clipped_input_ptr->Pvertex_light[0];

      tpoint_list->x=clipped_input_ptr->screen_pts[0].x; tpoint_list->y=clipped_input_ptr->screen_pts[0].y; 
      tpoint_list->z=clipped_input_ptr->vertices[0].z; 
      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;
      tpoint_list->l=point_l;
      tpoint_list++;
//      pconnections++;
      
      point_u=clipped_input_ptr->texture_pts[1].u;
      point_v=clipped_input_ptr->texture_pts[1].v;
      point_l=clipped_input_ptr->Pvertex_light[1];

      tpoint_list->x=clipped_input_ptr->screen_pts[1].x; tpoint_list->y=clipped_input_ptr->screen_pts[1].y;
      tpoint_list->z=clipped_input_ptr->vertices[1].z; 
      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;
//      tpoint_list->u=point_u; tpoint_list->v=point_v;
      tpoint_list->l=point_l;
      
      
      tpoint_list++;
//      pconnections++;
      
      point_u=clipped_input_ptr->texture_pts[2].u;
      point_v=clipped_input_ptr->texture_pts[2].v;
      point_l=clipped_input_ptr->Pvertex_light[2];

      tpoint_list->x=clipped_input_ptr->screen_pts[2].x; tpoint_list->y=clipped_input_ptr->screen_pts[2].y; 
      tpoint_list->z=clipped_input_ptr->vertices[2].z; 

      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;
      tpoint_list->l=point_l;

      tpoint_list++;

      if(clipped_input_ptr->number_of_verts==4)
      {
      point_u=clipped_input_ptr->texture_pts[3].u;
      point_v=clipped_input_ptr->texture_pts[3].v;
      point_l=clipped_input_ptr->Pvertex_light[3];

      tpoint_list->x=clipped_input_ptr->screen_pts[3].x; tpoint_list->y=clipped_input_ptr->screen_pts[3].y; 
      tpoint_list->z=clipped_input_ptr->vertices[3].z; 

      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;
      tpoint_list->l=point_l;

      tpoint_list++;

      }
//distort_window_clipper3(tpoints,ttemp_points,toutput_points,&tpolys);
          tpoint_list=tpoints;

          op=tpoint_list;	//from clipper
          fop=ftoutput_points;
          
          for (i=0;i<clipped_input_ptr->number_of_verts;i++)
          {
          fop->x=op->x;

          fop->y=op->y;
          fop->z=op->z/z_div;	//get z in range 1 to 0 (0=nearest)
          
          
          fop->u=op->u>>16;	//texel x
          fop->u=fop->u /128;

          fop->v=op->v>>16;	//texel y
          fop->v=fop->v /128;
          fop->l=op->l*3;	//l+32767;
          if(fop->l>65000) fop->l=65000;

//          fop->v=op->v;
          op++;
          fop++;
          }

//  distort3(screenaddr,screenwidth, tpoint_list, taddr, BITMAP_SIZE,(*the_poly_info).Plight_value,translucent);
        if(dmf_flag)
        {
          if(zbuffer_active==0)
          OGL_GTpolygon3(  clipped_input_ptr->number_of_verts,taddr, (fdcoord *)ftoutput_points,clipped_input_ptr->OGL_name, translucency);
          else
          OGL_GTpolygonZ3(  clipped_input_ptr->number_of_verts,taddr, (fdcoord *)ftoutput_points,clipped_input_ptr->OGL_name, translucency);
        }
        else
        {
          if(zbuffer_active==0)
          OGL_GTpolygon(  clipped_input_ptr->number_of_verts,taddr, (fdcoord *)ftoutput_points,clipped_input_ptr->OGL_name, translucent);
          else
          OGL_GTpolygonZ(  clipped_input_ptr->number_of_verts,taddr, (fdcoord *)ftoutput_points,clipped_input_ptr->OGL_name, translucent);
        }
//  tpoint_list+=3;
//  toutput_points+=3;
//  tpolys--;
  
//  }

     }
     else	//this is a non-tm'd poly




     if (poly_type==TEXTURE)	//Fuck, it's texture mapped. Don't Panic boys!!!
     {

      tpoint_list=tpoints;
      
      point_u=clipped_input_ptr->texture_pts[0].u;
      point_v=clipped_input_ptr->texture_pts[0].v;
      tpoint_list->x=clipped_input_ptr->screen_pts[0].x; tpoint_list->y=clipped_input_ptr->screen_pts[0].y; 
      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;
      tpoint_list++;
      
      point_u=clipped_input_ptr->texture_pts[1].u;
      point_v=clipped_input_ptr->texture_pts[1].v;
      tpoint_list->x=clipped_input_ptr->screen_pts[1].x; tpoint_list->y=clipped_input_ptr->screen_pts[1].y;
      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;
      tpoint_list++;
      
      point_u=clipped_input_ptr->texture_pts[2].u;
      point_v=clipped_input_ptr->texture_pts[2].v;

      tpoint_list->x=clipped_input_ptr->screen_pts[2].x; tpoint_list->y=clipped_input_ptr->screen_pts[2].y; 
      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;

      tpoint_list++;
      
      if(clipped_input_ptr->number_of_verts==4)
      {
      point_u=clipped_input_ptr->texture_pts[3].u;
      point_v=clipped_input_ptr->texture_pts[3].v;

      tpoint_list->x=clipped_input_ptr->screen_pts[3].x; tpoint_list->y=clipped_input_ptr->screen_pts[3].y; 
      tpoint_list->u=point_u<<16; tpoint_list->v=point_v<<16;

      tpoint_list++;
      }

//distort_window_clipper3(tpoints,ttemp_points,toutput_points,&tpolys);


//clamp light
     if((clipped_input_ptr->draw_code==55) || (clipped_input_ptr->draw_code==56))
     {
       fpoly_colour_r=(float)clipped_input_ptr->Prgb.red/65535;;
       fpoly_colour_g=(float)clipped_input_ptr->Prgb.green/65535;;
       fpoly_colour_b=(float)clipped_input_ptr->Prgb.blue/65535;;
       
 
     }
     else
     {
      if (clipped_input_ptr->Plight_value>32760) clipped_input_ptr->Plight_value=32760; 
      else
      if (clipped_input_ptr->Plight_value<-32760) clipped_input_ptr->Plight_value=-32760;

     light_value=clipped_input_ptr->Plight_value;

	 temp_red=light_value+32767;

     if (temp_red>65535) temp_red=65535;
     else
     if (temp_red<0) temp_red=0;

    
     fpoly_colour_r=temp_red;
     
     fpoly_colour_r=fpoly_colour_r/65536;	//0-1
     fpoly_colour_g=fpoly_colour_r;	//0-1
     fpoly_colour_b=fpoly_colour_r;	//0-1
     }
     
     tpoint_list=tpoints;
	  
//ogl likes all points in the range -1 to 1, where 0 is center, so we have to convert our integer points to floating points in this range
//y is also reversed
//tpoint_list=toutput_points;

          op=tpoint_list;	//from clipper
          fop=ftoutput_points;
          
          for (i=0;i<clipped_input_ptr->number_of_verts;i++)
          {
          
          
          fop->z=op->z/z_div;

          fop->x=op->x;

          fop->y=op->y;
          
          fop->u=op->u>>16;	//texel x
          fop->u=fop->u /128;

          fop->v=op->v>>16;	//texel y
          fop->v=fop->v /128;
          op++;
          fop++;
          }


     if(clipped_input_ptr->draw_code==55)
     {	//draw particle
     
        if(zbuffer_active==1)
        {
        //switch off zbuffer to draw particles
        glDisable(GL_DEPTH_TEST);	//switched off at the end of draw_it <--write some logic to clear these up Stu.

        OGL_TPpolygon(  clipped_input_ptr->number_of_verts,taddr, (fdcoord *)ftoutput_points,clipped_input_ptr->OGL_name,fpoly_colour_r,fpoly_colour_g, fpoly_colour_b, translucent);
        glEnable(GL_DEPTH_TEST);	//switched off at the end of draw_it

        }
        else
        OGL_TPpolygon(  clipped_input_ptr->number_of_verts,taddr, (fdcoord *)ftoutput_points,clipped_input_ptr->OGL_name,fpoly_colour_r,fpoly_colour_g, fpoly_colour_b, translucent);
        
        //OGL_TPpolygonZ(  clipped_input_ptr->number_of_verts,taddr, (fdcoord *)ftoutput_points,clipped_input_ptr->OGL_name,fpoly_colour_r,fpoly_colour_g, fpoly_colour_b, translucent);
     }
     else
     if(clipped_input_ptr->draw_code==56)
     {	//draw particle
     
        if(zbuffer_active==1)
        {
        //switch off zbuffer to draw particles
        glDisable(GL_DEPTH_TEST);	//switched off at the end of draw_it <--write some logic to clear these up Stu.

        OGL_TPpolygon(  clipped_input_ptr->number_of_verts,taddr, (fdcoord *)ftoutput_points,clipped_input_ptr->OGL_name,fpoly_colour_r,fpoly_colour_g, fpoly_colour_b, 1);
        glEnable(GL_DEPTH_TEST);	//switched off at the end of draw_it

        }
        else
        OGL_TPpolygon(  clipped_input_ptr->number_of_verts,taddr, (fdcoord *)ftoutput_points,clipped_input_ptr->OGL_name,fpoly_colour_r,fpoly_colour_g, fpoly_colour_b, 1);
        
        //OGL_TPpolygonZ(  clipped_input_ptr->number_of_verts,taddr, (fdcoord *)ftoutput_points,clipped_input_ptr->OGL_name,fpoly_colour_r,fpoly_colour_g, fpoly_colour_b, translucent);
     }
     else
     {
        if(zbuffer_active==0)
        OGL_Tpolygon(  clipped_input_ptr->number_of_verts,taddr, (fdcoord *)ftoutput_points,clipped_input_ptr->OGL_name,fpoly_colour_r,fpoly_colour_g, fpoly_colour_b, translucent);
        else
        OGL_TpolygonZ(  clipped_input_ptr->number_of_verts,taddr, (fdcoord *)ftoutput_points,clipped_input_ptr->OGL_name,fpoly_colour_r,fpoly_colour_g, fpoly_colour_b, translucent);
     }  
//  tpoint_list+=3;

     }
     else	//this is a non-tm'd poly
     if (poly_type==GOURAD)	//Gourad flag in point v, light values in u
     {
     //Gourad polyy
     float translucency;
     int dmf_flag;
         
       translucency=clipped_input_ptr->transparency;	 //0=transparent, 1=opaque
      dmf_flag=clipped_input_ptr->type&DMF;
    
	 temp_red=clipped_input_ptr->Prgb.red;
	 temp_green=clipped_input_ptr->Prgb.green;
	 temp_blue=clipped_input_ptr->Prgb.blue;

     fpoly_colour_r=temp_red;
     fpoly_colour_g=temp_green;
     fpoly_colour_b=temp_blue;
           tpoint_list=tpoints;
         point_l=clipped_input_ptr->Pvertex_light[0];

      tpoint_list->x=clipped_input_ptr->screen_pts[0].x; tpoint_list->y=clipped_input_ptr->screen_pts[0].y; 
      tpoint_list->z=clipped_input_ptr->vertices[0].z;
      tpoint_list->l=point_l;	//light value

      tpoint_list++;
         point_l=clipped_input_ptr->Pvertex_light[1];

      tpoint_list->x=clipped_input_ptr->screen_pts[1].x; tpoint_list->y=clipped_input_ptr->screen_pts[1].y;
      tpoint_list->z=clipped_input_ptr->vertices[1].z;
      tpoint_list->l=point_l;

      tpoint_list++;
      point_l=clipped_input_ptr->Pvertex_light[2];

      tpoint_list->x=clipped_input_ptr->screen_pts[2].x; tpoint_list->y=clipped_input_ptr->screen_pts[2].y; 
      tpoint_list->z=clipped_input_ptr->vertices[2].z;
      tpoint_list->l=point_l;

      tpoint_list++;
      
      if(clipped_input_ptr->number_of_verts==4)
      {
      point_l=clipped_input_ptr->Pvertex_light[3];

      tpoint_list->x=clipped_input_ptr->screen_pts[3].x; tpoint_list->y=clipped_input_ptr->screen_pts[3].y; 
      tpoint_list->z=clipped_input_ptr->vertices[3].z;
      tpoint_list->l=point_l;

      tpoint_list++;
      }

          tpoint_list=tpoints;


          

          op=tpoint_list;
          fop=ftoutput_points;
          
          for (i=0;i<clipped_input_ptr->number_of_verts;i++)
          {

//          if(op->z>z_div) Debugger();

          fop->z=op->z/z_div;

          fop->x=op->x;

          fop->y=op->y;
          
          fop->u=op->l;	//lighting

          op++;
          fop++;
          }
       if(dmf_flag)
       {
        translucent=translucency;
       }
       
       translucent=0;
       
        if(clipped_input_ptr->draw_code==1) 
        {
         translucent=1;
        }
        
       if (zbuffer_active==0)
       OGL_Gpolygon(clipped_input_ptr->number_of_verts,(fdcoord *)ftoutput_points,fpoly_colour_r,fpoly_colour_g,fpoly_colour_b, translucent);
       else
       OGL_GpolygonZ(clipped_input_ptr->number_of_verts,(fdcoord *)ftoutput_points,fpoly_colour_r,fpoly_colour_g,fpoly_colour_b, translucent);
       
//  tpoint_list+=3;
     
     }
     else	//this is a flat coloured poly
     {
     
     light_value=clipped_input_ptr->Plight_value;

	temp_red=clipped_input_ptr->Prgb.red+light_value;
	 temp_green=clipped_input_ptr->Prgb.green+light_value;
	 temp_blue=clipped_input_ptr->Prgb.blue+light_value;
     if (temp_red>65535) temp_red=65535;
     else
     if (temp_red<0) temp_red=0;

     if (temp_blue>65535) temp_blue=65535;
	 else
	 if (temp_blue<0) temp_blue=0;

     if (temp_green>65535) temp_green=65535;
     else
     if (temp_green<0) temp_green=0;
     
     fpoly_colour_r=temp_red;
     fpoly_colour_g=temp_green;
     fpoly_colour_b=temp_blue;
     
     fpoly_colour_r=fpoly_colour_r/65536;	//0-1
     fpoly_colour_g=fpoly_colour_g/65536;	//0-1
     fpoly_colour_b=fpoly_colour_b/65536;	//0-1
     


//	if((*the_poly_info).polyflags&DUST_BIT)
//	{
//		OGL_plot_rgb((*(drawing_info)).x1, (*(drawing_info)).y1, fpoly_colour_r, fpoly_colour_g, fpoly_colour_b);
//	}
//	else	//dust bit not set
//	{
     
//we need to get these in the range 1..0

      point_list=points;
      point_list->x=clipped_input_ptr->screen_pts[0].x;  point_list->y=clipped_input_ptr->screen_pts[0].y; point_list->z=clipped_input_ptr->vertices[0].z; point_list++;
      point_list->x=clipped_input_ptr->screen_pts[1].x;  point_list->y=clipped_input_ptr->screen_pts[1].y; point_list->z=clipped_input_ptr->vertices[1].z; point_list++;
      point_list->x=clipped_input_ptr->screen_pts[2].x;  point_list->y=clipped_input_ptr->screen_pts[2].y; point_list->z=clipped_input_ptr->vertices[2].z; point_list++;
      if(clipped_input_ptr->number_of_verts==4)
      {
      point_list->x=clipped_input_ptr->screen_pts[3].x;  point_list->y=clipped_input_ptr->screen_pts[3].y; point_list->z=clipped_input_ptr->vertices[3].z; point_list++;
      }
      point_list=points;

output_lines=clipped_input_ptr->number_of_verts;
	
	if(1)
	{
	_2D * op;
	_float2D * fop;
	  
//ogl likes all points in the range -1 to 1, where 0 is center, so we have to convert our integer points to floating points in this range
//y is also reversed


          op=points;
          fop=foutpoints;

          
          for (i=0;i<output_lines;i++)
          {
          fop->z=op->z/z_div;

          fop->x=op->x;
          
          fop->y=op->y;
          op++;
          fop++;
          }
          if (zbuffer_active==0)
          OGL_polygon(clipped_input_ptr->number_of_verts,(fcoord *)foutpoints,fpoly_colour_r,fpoly_colour_g,fpoly_colour_b);
	  else
          OGL_polygonZ(clipped_input_ptr->number_of_verts,(fcoord *)foutpoints,fpoly_colour_r,fpoly_colour_g,fpoly_colour_b);
           }
//        }	//DUST_BIT
     }


     }	//end of not blobbed



//    } //end of if poly!=0
    clipped_input_ptr++;    
   }	// end of for loop

//        glMatrixMode(GL_PROJECTION);
//        glLoadIdentity();
//        glOrtho(-1.0f, (GLfloat) 1/*window_w*/, -1.0f, (GLfloat) 1 /*window_h*/, 0.0f, -1.0f);
}


#endif //ogl==1





 
//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
Ptr screenaddr1;
Ptr screenaddr2;
Ptr screenaddr3;

void reinit_screen()
{
//extern Ptr screenaddr;
//extern int screenwidth;
//LSRAW *the_picture;
//FSSpec the_file;
//Handle the_file_data_H;

//extern FSSpec Zex_FSSpec;

//CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;

}



