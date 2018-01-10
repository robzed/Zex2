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

// genpoly.int

extern int screen_x_min, screen_x_max, screen_y_min, screen_y_max;	//module vars for width and height - sb 181298

#define FLOAT_TEXTURE_MAP 0		// gourand can't use float yet


#if FLOAT_TEXTURE_MAP
  #define F_TO_I(x) ((int)(x))
  #define I_TO_F(x) ((float)(x))
  #define FLOAT_TYPE float
#else
  #define I_TO_F(x) ((x)<<16)
  #define F_TO_I(x) ((x)>>16)
  #define FLOAT_TYPE int
#endif

typedef struct {
		int linex; 
	    FLOAT_TYPE bmpx; 
	    FLOAT_TYPE bmpy;

		} border_type;

//protos
void tex_draw2(border_type *start_dest, border_type *end_dest, int x1,int y1,int x2,int y2, int from_a, int from_b, int to_a, int to_b);
//void tex_draw_rev(border_type *data_dest,int x1,int y1,int x2,int y2, int from_a, int from_b, int to_a, int to_b);

//module_private void scanoneline(char *screen_line_address, int line_startx, int line_endx, char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width, char *light_colour_address);
int convert_to_palette(int r, int g, int b);

