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

//dlp.ext
#define DLP_NOP	0
#define DRAW_LINE 1	//p1=start x, p2=start y, p3=end x, p4=end y,p5=colour
#define DRAW_TEXT 2	//p1=x, p2=y, p3=ptr to c string
#define DRAW_LASER1 3	//p1=start x, p2=start y, p3=end x, p4=end y
#define DRAW_LASER2 4	//p1=start x, p2=start y, p3=end x, p4=end y
#define DRAW_LASER3 5	//p1=start x, p2=start y, p3=end x, p4=end y
#define DRAW_LINE_NO_CLIP 6	//p1=start x, p2=start y, p3=end x, p4=end y,p5=colour
                    //this can crash if passed bad params!
#define DRAW_TRACTOR 7
#define DRAW_DLP_EXPLOSION 8
#define DRAW_OUTER_FLARE 9
#define DRAW_FILLED_BOX  10	//p1=top, p2=left, p3=bott, p4=right, p5=colour
#define FLOATING_DIALOG 11	//p1=x, p2=y. p6 has the string
#define DRAW_RADAR_LINE 12	//line at text layer
#define SLEEP_DIALOG 13	////p1=x, p2=y. p6 has the string
#define DLP_RED 1  //text colours for text message area
#define DLP_PURP 2
#define DLP_YELLOW 3
#define DLP_GREEN 4
#define DLP_WHITE 5


void add_to_dlp(int command,int param1,int param2, int param3, int param4, int param5, char* param6,
                    float param7, float param8, float param9);
void clear_dlp();
void run_dlp();
void run_dlp_text();
void add_to_text_display(Ptr text_in,int colour);
void display_text_display();
void init_dlp();
void set_dlp_clip_bounds(Rect the_bounds);	//140899


void reset_dlp_text_display();
void dlp_draw_box(int top, int left, int bottom, int right, int colour);
void dlp_draw_unfilled_box(int top, int left, int bottom, int right, int colour);
void dlp_draw_rounded_box(int top, int left, int bottom, int right, int colour);

void draw_line(int x1, int y1, int x3, int y3, int colour);
void draw_line_no_clip(int x1, int y1, int x3, int y3, int colour);
void Zline_rgb(int x1, int y1, int x2, int y2,float red, float green, float blue);
