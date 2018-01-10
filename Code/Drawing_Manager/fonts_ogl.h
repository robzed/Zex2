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

/*
 *  fonts.h
 *  Z_dungeon
 *
 *  Created by Stuart Ball on Sat Dec 27 2003.
 *  Copyright (c) 2003 Lightsoft Software (Games). All rights reserved.
 *
 */


/*
How the fonts work.
There are template fonts identified as SMALL_FIXED_FONT, SMALL_PROP_FONT, LARGE_MENU_FONT etc.
These are loaded with init_fonts. These fonts are not useable in the game but are used to build 'prepared fonts'.
Prepared fonts are built with create_font, which returns a font reference. These fonts are usable in the game.

create_font takes a template font ID (which template to create the prepared font from) along with the foreground pixel
attributes, the background pixel attributes and a drawing style.
This allows us to create fonts of any colour, alpha and drawing style.

Text is drawn with a call to print_string, which takes a font reference, coordinates and a C string.

The font system takes care of all aspects of font memory management. Be sure when the game is finished to call 
kill_fonts to release the storage used by the fonts.


Important Constants:
--------------------
NUMBER_OF_GLYPHS - The font template bitmaps consist of a horizontal strips of font images running from ASCII 
32 through to ASCII 127. Glyphs higher than this are currently not catered for but can be by changing 
NUMBER_OF_GLYPHS in fonts.cpp and creating new template bitmaps.

BIGGEST_GLYPH_WIDTH - The largest glyph width catered for is BIGGEST_GLYPH_WIDTH, again in fonts.cpp

MAX_NUMBER_OF_PREPARED_FONTS - The system can cope with MAX_NUMBER_OF_PREPARED_FONTS (found in this file).


Errors
-------
Errors are returned from create_font as per this file. 


Examples
--------
  FGColour.r=0xff; FGColour.g=0x0; FGColour.b=0x0; FGColour.alpha=0xff;
  BGColour.r=0x0; BGColour.g=0xFF; BGColour.b=0x0; BGColour.alpha=0xf;
  my_font_ref=create_font (&FGColour, &BGColour, SMALL_FIXED_FONT, FONT_NORMAL_DRAWING_STYLE);

Creates a font with a red fg colour, a green background colour in the normal style.
NOTE: If you want the bg colour to show you must set the bg alpha to something other than 0

  print_string(my_font_ref, 200,520, "Hello");
  
Prints "Hello" in red on green at 200 (x),500 (y)

*/


// ------Game specific defines

// These are the prepared font defines for ZDungeon
enum
{
 SMALL_PROP_WHITE_NO_BG,
 SMALL_FIXED_WHITE_NO_BG,
 SMALL_FIXED_WHITE_FG_BLACK_BG,
 SMALL_PROP_RED_NO_BG,
 SMALL_PROP_YELLOW_NO_BG,
 LARGE_MENU_WHITE_NO_BG,
 LARGE_MENU_GREEN_NO_BG,
 LARGE_MENU_ORANGE_NO_BG,
 LARGE_MENU_RED_NO_BG,
 LARGE_MENU_BLUE_NO_BG,
 LARGE_MENU_YELLOW_NO_BG,
 MEDIUM_MENU_WHITE_NO_BG,
 MEDIUM_MENU_GREEN_NO_BG,
 MEDIUM_MENU_RED_NO_BG,
 MEDIUM_MENU_ORANGE_NO_BG,
 SMALL_MENU_WHITE_NO_BG,
};


#define NOT_USED_FONT_ID -1
// ------End of game specific defines


enum
{
 SMALL_FIXED_FONT,
 SMALL_PROP_FONT,
 LARGE_MENU_FONT,  //36 point
 MEDIUM_MENU_FONT, //24 point
 SMALL_MENU_FONT,  //16 point 
 NUMBER_OF_FONTS=SMALL_MENU_FONT+1,
};                  //Template font ID's used in create_font


typedef int template_font_id; //template font ID's type
typedef int prepared_font_ref; //when drawing a string, pass in a prepared font reference to draw with

//drawing styles in create_font
#define FONT_NORMAL_DRAWING_STYLE 0


//how many prepared fonts can we deal with?
#define MAX_NUMBER_OF_PREPARED_FONTS 100


// Public errors from create_font
#define FONT_INVALID_TEMPLATE_FONT_ID -10  //template font id not one of SMALL_FIXED_FONT, SMALL_PROP_FONT etc
#define FONT_INVALID_DRAWING_STYLE -11     //Unknown drawing style passed to create_font
#define FONT_TOO_MANY_PREPARED_FONTS -12   //increase MAX_NUMBER_OF_PREPARED_FONTS



typedef struct
{
  unsigned char r;      //red
  unsigned char g;      //green
  unsigned char b;		//blue
  unsigned char alpha;  //range from SDL_ALPHA_TRANSPARENT (0) to SDL_ALPHA_OPAQUE (255)
} FColourSpec;          //basic colour spec used when creating fonts (note, set alpha >0 for bg colour to show)
 
 

// ---------------- API -----------------
//Call before drawing any text - loads all known template fonts
void init_fonts(void); 

//build a font and return a reference that should be used to draw with this font
//returns either a font ref>=0 or -ve error as above if font couldn't be built
prepared_font_ref create_font (FColourSpec* FGColour, FColourSpec* BGColour, template_font_id which_font, int drawing_style);

//Returns the font height
int get_font_height(prepared_font_ref which_font);

//print a formated string
void print_formatted_string(int x,int y, int string_name);

//print a string with a given prepared font 
void print_string(prepared_font_ref which_font, int x, int y, char* the_string);   

//print a string with a given prepared font relative to the last printing location 
void print_string_relative(prepared_font_ref which_font, int x_off, int y_off, char* the_string);

//print a string with a length of count with a given prepared font 
void print_counted_string(prepared_font_ref which_font, int x, int y, char* the_string, int count);

//Call to release the font storage
void kill_fonts(void); 

//<EOF>
