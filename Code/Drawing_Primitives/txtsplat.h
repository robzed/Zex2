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

// Name:   		TxtSplat.h
// Date Started:	25/11/98
// Author(s):		ID/RP/SB
//
// Copyright(c) Lightsoft Software   		



//The base address you want the charcters draw to and the screen width
void InitText(void);  

//The string (80 max but increade it if you want more), X/Y co-ords on screen			
void SplatText(char splat_str[80], int x_pos, int y_pos, unsigned char fg_colour, unsigned char bg_colour);
void SplatText_Small(char splat_str[16], int x_pos, int y_pos, unsigned char fg_colour);
void SplatText_Small_rgb(char splat_str[16], int x_pos, int y_pos, float red, float green, float blue );
 void SplatText_mon_fast(char splat_str[16], int x_pos, int y_pos);


void SplatText_Thrust(char splat_str[30], int x_pos, int y_pos);
//As above but with the colour you want transparent
void SplatText_Transparent(char splat_str[80], int x_pos, int y_pos, int colour);
void Large_Draw_to_Double_Buffer_Transparent(char *char_ptr, int x_pos, int y_pos); // Colour is the colour that needs masking
void SplatTextLarge_Transparent(char splat_str[80], int x_pos, int y_pos);
void SplatTextLarge_Treplace(char splat_str[80], int x_pos, int y_pos, int colour, int replace);
void Large_Draw_to_Double_Buffer_Treplace(char *char_ptr, int x_pos, int y_pos, int colour, int replace);
void SplatTextLarge_weird(char splat_str[80], int x_pos, int y_pos);
void Large_Draw_to_Double_Buffer_weird(char *char_ptr, int x_pos, int y_pos);
void SplatTextLarge_weirdB(char splat_str[80], int x_pos, int y_pos);
void Large_Draw_to_Double_Buffer_weirdB(char *char_ptr, int x_pos, int y_pos);
void SplatText_Transparent_Fade(char splat_str[80], int x_pos, int y_pos, unsigned char colour);
void Draw_to_Double_Buffer_TransparentFade(char *char_ptr, int x_pos, int y_pos, unsigned char colour); // Colour is the colour that needs masking
void SplatText_Fade(char splat_str[80], int x_pos, int y_pos, unsigned char fg_colour, unsigned char  bg_colour);
void Draw_to_Double_Buffer_Fade(char *char_ptr, int x_pos, int y_pos , unsigned char fg_colour,unsigned char bg_colour);
void Draw_to_Double_Buffer_Small_Transparent(char *char_ptr, int x_pos, int y_pos , unsigned char fg_colour);
 void SplatText_Small_Transparent(char splat_str[16], int x_pos, int y_pos, unsigned char fg_colour);
 
void SplatTextLarge_TransparentC(char splat_str[80], int x_pos, int y_pos, int colour);

void Large_Draw_to_Double_Buffer_TransparentC(char *char_ptr, int x_pos, int y_pos,int colour); // Colour is the colour that needs masking


void SplatText_Console(char splat_str[80], int x_pos, int y_pos, int colour, char attrib_str[80]);
void Draw_to_Double_Buffer_Console(char *char_ptr, int x_pos, int y_pos, unsigned char colour, unsigned char attrib);
