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

//FPOLY4.ext
typedef struct {
		int x;
		int y;
		} coord;


void polygon(int num_lines, coord *point_list, int colour, char *screen_address, int screen_width);
void init_polygon(void);
void window_clipper(int num_lines, coord *input_list, coord *temp_list, coord *output_list, int *output_lines);
void set_poly_clipping (int x_min, int x_max, int y_min, int y_max);	//sb 1801298
void pline(int x1, int x2, char *line_screen_address, int colour);
int single_line_clip (int *x1, int *y1, int *x2, int *y2);
