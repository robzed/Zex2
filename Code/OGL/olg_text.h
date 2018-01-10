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


int init_ogl_text();
void OGL_draw_monaco_bold(char* the_string);
void OGL_draw_helvetica(char* the_string);
void OGL_draw_helvetica_small(char* the_string);

void OGL_draw_helveticaP(const unsigned char* the_string);	//passcal string version


void OGL_DrawStr(char *str,int the_len,int x, int y);
void ZDrawString(const unsigned char * the_string);
void ZMoveTo(int x, int y);
void ZLineTo(int x, int y);
void ZRGBForeColor (const RGBColor * RGB_Colour);
void ZRGBBackColor (const RGBColor * RGB_Colour); //used exclusively in ZEraseRect
void ZEraseRect(Rect * the_rect);