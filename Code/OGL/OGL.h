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



typedef struct {
		float x;
		float y;
		float z;
		} fcoord;
		
typedef struct {
		float x;
		float y;
		float z;
		float u;
		float v;
	        float l;	//light
		} fdcoord;
			
int init_ogl();

void ogl_active();
void ogl_inactive();
void show_ogl_info(void);
int	SufficientVideoMemory( AGLPixelFormat fmt ); 

void OGL_show_it(void);
void cleanupAGL();
void OGL_polygon(int numb_verts,fcoord *point_list, float r, float g, float b);
void OGL_polygonZ(int numb_verts,fcoord *point_list, float r, float g, float b);
void OGL_Gpolygon(int numb_verts,fdcoord *point_list,float r, float g, float b, int translucent);
void OGL_GpolygonZ(int numb_verts,fdcoord *point_list,float r, float g, float b, int translucent);
void OGL_Tpolygon(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, float r, float g, float b, int translucent);
void OGL_TpolygonZ(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, float r, float g, float b, int translucent);
void OGL_GTpolygon(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, int translucent);
void OGL_GTpolygonZ(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, int translucent);

void OGL_TPpolygon(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, float r, float g, float b, int translucent);
void OGL_TPpolygonZ(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, float r, float g, float b, int translucent);

void OGL_GTpolygon3(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, float translucent);
void OGL_GTpolygonZ3(int numb_verts,char* taddr, fdcoord *point_list,unsigned long name, float translucent);


void OGL_line_rgb(int x1, int y1, int x3, int y3, float red, float green, float blue);
void OGL_line_rgb_quick(int x1, int y1, int x3, int y3, float red, float green, float blue);
void OGL_line(int x1, int y1, int x3, int y3, int colour);
void OGL_line_no_colour(int x1, int y1, int x3, int y3);

void OGL_plot(int x1, int y1, int colour);
void OGL_plot_rgb(int x1, int y1, float red, float green, float blue);
void OGL_plot_rgb_quick(int x1, int y1, float red, float green, float blue);


void OGL_set_stencil_for_all();
void OGL_set_stencil_for_above();


void OGL_draw_pict(int x, int y,LSRAW* pictbuffer_rgba, int picture_w, int picture_h);
void OGL_set_pen_colour_from_index(unsigned int colour);
void OGL_moveto(float x, float y);
void OGL_move_and_set_colour(float x, float y, int colour);


// CI calls
void init_constant_image(void);

int set_as_constant_image(LSRAW *_rgba_image, int _x1, int _y1, int _x2, int _y2);
int set_as_constant_image1(LSRAW *_rgba_image, int _x1, int _y1, int _x2, int _y2, int unique_id);
void set_constant_image_pos(int constant_image_id, int x, int y);
void draw_constant_images(void);
void delete_constant_image(int constant_image_id);
void delete_all_constant_images(void);

void swap_context(int context);
int get_context(void);
int is_a_factor_of_2(int number);

// instructions for use:- constant images
//
// (a) Must call init_constant_image() at start up.
// (b) Call set_as_constant_image() to set graphics to routines
// (c) Call draw_constant_images() each frame to display 
// (d) to remove one image use delete_constant_image
// (e) to remove all images use delete_all_constant_images()
//
// There are a maximum of 30 images on the screen at once.
// These can all be used as above without below "swap_context".
//
// swap_context() alters the current context. This context is used for
// all calls, and is only reset by init_constant_image (which should only be 
// called once!)
//
// get_context() fetches the value.
//
// if you draw in one context, then change to another, all your piccies
// will be hidden until you change back. You can use these as multiple
// drawing boards, thereby displaying something different until you change back.
//
void ogl_show_mouse_cursor();
void ogl_hide_mouse_cursor();
void ogl_update_mouse_cursor();


void ogl_load_control_panel();
void ogl_hide_control_panel();

void ogl_hide_radar_reticules();