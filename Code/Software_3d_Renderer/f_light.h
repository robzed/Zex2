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

// f_light.h


#define NUM_LIGHT_LEVELS 16

#if NUM_LIGHT_LEVELS==16
  #define LIGHT256_SHIFT 4
  #define LIGHT_SHIFT 4
#elif NUM_LIGHT_LEVELS==32
  #define LIGHT256_SHIFT 3
  #define LIGHT_SHIFT 5
#elif NUM_LIGHT_LEVELS==64
  #define LIGHT256_SHIFT 2
  #define LIGHT_SHIFT 6
#elif NUM_LIGHT_LEVELS==128
  #define LIGHT256_SHIFT 1
  #define LIGHT_SHIFT 7
#endif

extern char light_table[256*NUM_LIGHT_LEVELS];
extern char transparency_table[256*256];

#define LIGHT_SHADED_TEXTURES 1
