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
 *  fonts_ogl.cpp
 *  
 *
 *  Created by Stuart Ball on Sat Jan 17 2004.
 *  Copyright (c) 2004 Lightsoft Software (Games). All rights reserved.
 *
 * $Author: stu_c $
 * $Date:$
 * $Revision:$
 */
 
//Please see fonts.h for description and usage notes
//See fonts.c/h for an SDL font system
//This is based on the SDL implementation, but because OGL is so vastly different
//it wasn't possible to hack it about to fit.
//The OGL version is based on the same template fonts structure as the SDL version
//but doesn't suffer from the limitations of SDL. Hence we don't need to pre-compile 
//font colours, we can do that at drawing time.
//
//The way this works is we generate a display list from the template font as a base texture,  
//as a quad for each glyph, then use glCallListsScalar to draw a string. Should be as fast as the 
//HW can do it as with any luck the lists will live on the vid card.
//We lose as we can't assume SDL is present with its nice SDL_LoadBMP calls.
//The API will hopefully be the same as the SDL version.

// ***********************************************************************************
// * PRIVATE DEFINES
// *
#define LOWER_GLYPH 30
#define UPPER_GLYPH 227
#define BIGGEST_GLYPH_WIDTH 96 //biggest glyph width we can deal with
#define NO_DATA 0x01 //what we consider no data in the template font bitmap (pallette entry, white)
#define DATA 0x00    //what we consider data in the template font  bitmap (pallette entry, black)
#define DATA256 0xFF //clut 255 is black in 8 bit greyscale, 0 is white
#define NUMBER_OF_GLYPHS (UPPER_GLYPH-LOWER_GLYPH)+1 //how many glyphs in a font

//Internal errors, not publicly available 
#define NOERR 0
#define FONT_ERR_NOT_ENOUGH_DATA -1
#define FONT_ERR_GLYPH_WIDTH_TOO_BIG -2
#define FONT_ERR_GLYPH_WIDTH_TOO_SMALL -3

//For formatted strings
#define WHITE 0
#define RED 1
#define ORANGE 2
#define GREEN 3
#define BLUE 4
#define YELLOW 5

#define LARGE 2
#define MEDIUM 1
#define SMALL 0

#define SUBSTRING_MAX 512

// ***********************************************************************************
// * PRIVATE TYPE DEFS
// *

typedef struct {
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 alpha;
} LS_Colour;

typedef struct {
	int       ncolours;
	LS_Colour *colours;
} LS_Palette;

/* Everything in the pixel format structure is read-only */
typedef struct  {
	SDL_Palette *palette;
	Uint8  BitsPerPixel;
	Uint8  BytesPerPixel;
} LS_PixelFormat;

typedef struct {
	SDL_PixelFormat *format;
	int w, h;				
	Uint16 pitch;			
	void *pixels;			
} LS_Surface;

typedef struct
{
 int offset;
 int width;
} basic_font_entry;

typedef struct
{
  LS_Surface* the_SDL_surface;
  basic_font_entry character_data [NUMBER_OF_GLYPHS];
} template_font_t;


typedef struct
{
  LS_Surface* font_surface;
  basic_font_entry character_data [NUMBER_OF_GLYPHS];
  int h;
} prepared_font;


// ***********************************************************************************
// * PRIVATE VARIABLE DECLARATIONS 
// *
static int last_drawn_x, last_drawn_y; //where we are drawing from if relative

static template_font_t fonts[NUMBER_OF_FONTS]; //small fixed game, small_prop_game, large menu and small menu

static prepared_font* prepared_fonts[MAX_NUMBER_OF_PREPARED_FONTS];

static prepared_font_ref new_font_ref;;

static int fixed_index; //if a fixed width font, here's the width (used when initting template fonts)

static int current_colour; //for formatted strings
static int current_size=LARGE;

// ***********************************************************************************
// * PRIVATE FUNCTION PROTOTYPES
// *
static bool init_template_font(template_font_t* where, char* filename);
static int build_template_font_info(template_font_t* where);
static int build_template_proportional_font(template_font_t* where);
static int build_template_fixed_width_font(template_font_t* where);
static bool is_font_proportional(template_font_t* where);

//Font drawing style functions used in create_font
static void create_normal_font(int w, int h, unsigned char* source_ptr, unsigned char* dest_ptr, 
                               FColourSpec* FGColour, FColourSpec* BGColour );

static void create_normal_font_AA(int rgba_w, int rgba_h, template_font_t* where, unsigned char* dest_ptr,
                                  FColourSpec* FGColour, FColourSpec* BGColour );

static prepared_font_ref process_escape(char current_char);

// ***********************************************************************************
// * ACTUAL CODE
// * 



// ***********************************************************************************
// * PUBLIC ROUTINES
// * 


// +--------------------------------+-------------------------+-----------------------
// | TITLE: init_fonts              | AUTHOR(s): Stuart Ball  | DATE STARTED: 27 Dec 2003
// +
// | DESCRIPTION: Initialises our fonts
// +----------------------------------------------------------------ROUTINE HEADER----
void init_fonts(void)
{
FColourSpec FGColour, BGColour;

 current_colour=WHITE; //for formatted strings
 current_size=LARGE;


 last_drawn_x=0; last_drawn_y=0; new_font_ref=0;
 
 fonts[SMALL_FIXED_FONT].the_SDL_surface=0;
 fonts[SMALL_PROP_FONT].the_SDL_surface=0;
 fonts[LARGE_MENU_FONT].the_SDL_surface=0;
 fonts[MEDIUM_MENU_FONT].the_SDL_surface=0;
 fonts[SMALL_MENU_FONT].the_SDL_surface=0;

 if (!init_template_font(&fonts[SMALL_FIXED_FONT],FONTS_PATH"small_game_font.bmp"))
 {
	fprintf(OUTPUT_FP, "FATAL: init_fonts - Unable to read small game font! Please re-install the game\n");
    request_quit(); //quit if there's a problem here
    return;         //Fatal, do no proceed
 }

 if (!init_template_font(&fonts[SMALL_PROP_FONT],FONTS_PATH"small_prop_font.bmp"))
 {
	fprintf(OUTPUT_FP, "FATAL: init_fonts - Unable to read small prop font! Please re-install the game\n");
    request_quit(); //quit if there's a problem here
    return;         //Fatal, do no proceed
 }
 
 if (!init_template_font(&fonts[LARGE_MENU_FONT],FONTS_PATH"large_menu_font256.bmp"))
 {
	fprintf(OUTPUT_FP, "FATAL: init_fonts - Unable to read large menu font! Please re-install the game\n");
    request_quit(); //quit if there's a problem here
    return;         //Fatal, do no proceed
 }
 
 if (!init_template_font(&fonts[MEDIUM_MENU_FONT],FONTS_PATH"medium_menu_font256.bmp"))
 {
	fprintf(OUTPUT_FP, "FATAL: init_fonts - Unable to read medium menu font! Please re-install the game\n");
    request_quit(); //quit if there's a problem here
    return;         //Fatal, do no proceed
 }

 if (!init_template_font(&fonts[SMALL_MENU_FONT],FONTS_PATH"small_menu_font256.bmp"))
 {
	fprintf(OUTPUT_FP, "FATAL: init_fonts - Unable to read small menu font! Please re-install the game\n");
    request_quit(); //quit if there's a problem here
    return;         //Fatal, do no proceed
 }

// --------------------------------------------------------------------------
// The following is game specific, create your prepared fonts here

/*
//Create the fonts we need for ZDungeon - these tie up with enums in header
  FGColour.r=0xff; FGColour.g=0xFF; FGColour.b=0xFF; FGColour.alpha=0xff;
  BGColour.r=0x0; BGColour.g=0xFF; BGColour.b=0x0; BGColour.alpha=0x0;
  create_font (&FGColour, &BGColour, SMALL_PROP_FONT, FONT_NORMAL_DRAWING_STYLE); //SMALL_PROP_WHITE_NO_BG

  FGColour.r=0xff; FGColour.g=0xFF; FGColour.b=0xFF; FGColour.alpha=0xff;
  BGColour.r=0x0; BGColour.g=0xFF; BGColour.b=0x0; BGColour.alpha=0x0;
  create_font (&FGColour, &BGColour, SMALL_FIXED_FONT, FONT_NORMAL_DRAWING_STYLE); //SMALL_FIXED_WHITE_NO_BG

  FGColour.r=0xff; FGColour.g=0xFF; FGColour.b=0xFF; FGColour.alpha=0xff;
  BGColour.r=0x0; BGColour.g=0x0; BGColour.b=0x0; BGColour.alpha=0x2f;
  create_font (&FGColour, &BGColour, SMALL_FIXED_FONT, FONT_NORMAL_DRAWING_STYLE); //SMALL_FIXED_WHITE_FG_BLACK_BG

  FGColour.r=0xff; FGColour.g=0x33; FGColour.b=0x33; FGColour.alpha=0xff;
  BGColour.r=0x0; BGColour.g=0x0; BGColour.b=0x0; BGColour.alpha=0x0;
  create_font (&FGColour, &BGColour, SMALL_PROP_FONT, FONT_NORMAL_DRAWING_STYLE); //SMALL_PROP_RED_NO_BG

  FGColour.r=0xff; FGColour.g=0xFF; FGColour.b=0x0; FGColour.alpha=0xff;
  BGColour.r=0x0; BGColour.g=0x0; BGColour.b=0x0; BGColour.alpha=0x0;
  create_font (&FGColour, &BGColour, SMALL_PROP_FONT, FONT_NORMAL_DRAWING_STYLE); //SMALL_PROP_YELLOW_NO_BG

  FGColour.r=0xff; FGColour.g=0xff; FGColour.b=0xff; FGColour.alpha=0xff;
  BGColour.r=0x0; BGColour.g=0x0; BGColour.b=0x0; BGColour.alpha=0x0;
  create_font (&FGColour, &BGColour, LARGE_MENU_FONT, FONT_NORMAL_DRAWING_STYLE); //LARGE_MENU_WHITE_NO_BG

  FGColour.r=0x3f; FGColour.g=0xff; FGColour.b=0x3f; FGColour.alpha=0xdf;
  BGColour.r=0x0; BGColour.g=0x0; BGColour.b=0x0; BGColour.alpha=0x0;
  create_font (&FGColour, &BGColour, LARGE_MENU_FONT, FONT_NORMAL_DRAWING_STYLE); //LARGE_MENU_GREEN_NO_BG

  FGColour.r=0xff; FGColour.g=0x99; FGColour.b=0x00; FGColour.alpha=0xdf;
  BGColour.r=0xff; BGColour.g=0xFf; BGColour.b=0xff; BGColour.alpha=0x0;
  create_font (&FGColour, &BGColour, LARGE_MENU_FONT, FONT_NORMAL_DRAWING_STYLE); //LARGE_MENU_ORANGE_NO_BG

  FGColour.r=0xff; FGColour.g=0x33; FGColour.b=0x33; FGColour.alpha=0xdf;
  BGColour.r=0xff; BGColour.g=0xFf; BGColour.b=0xff; BGColour.alpha=0x0;
  create_font (&FGColour, &BGColour, LARGE_MENU_FONT, FONT_NORMAL_DRAWING_STYLE); //LARGE_MENU_RED_NO_BG

  FGColour.r=0x20; FGColour.g=0x20; FGColour.b=0xff; FGColour.alpha=0xdf;
  BGColour.r=0xff; BGColour.g=0xFf; BGColour.b=0xff; BGColour.alpha=0x0;
  create_font (&FGColour, &BGColour, LARGE_MENU_FONT, FONT_NORMAL_DRAWING_STYLE); //LARGE_MENU_BLUE_NO_BG

  FGColour.r=0xFF; FGColour.g=0xFF; FGColour.b=0x00; FGColour.alpha=0xdf;
  BGColour.r=0xff; BGColour.g=0xFf; BGColour.b=0xff; BGColour.alpha=0x0;
  create_font (&FGColour, &BGColour, LARGE_MENU_FONT, FONT_NORMAL_DRAWING_STYLE); //LARGE_MENU_YELLOW_NO_BG
 
  FGColour.r=0xff; FGColour.g=0xff; FGColour.b=0xff; FGColour.alpha=0xff;
  BGColour.r=0xff; BGColour.g=0xFf; BGColour.b=0xff; BGColour.alpha=0x0;
  create_font (&FGColour, &BGColour, MEDIUM_MENU_FONT, FONT_NORMAL_DRAWING_STYLE); //MEDIUM_MENU_WHITE_NO_BG
 
  FGColour.r=0x3f; FGColour.g=0xff; FGColour.b=0x3f; FGColour.alpha=0xff;
  BGColour.r=0xff; BGColour.g=0xFf; BGColour.b=0xff; BGColour.alpha=0x0;
  create_font (&FGColour, &BGColour, MEDIUM_MENU_FONT, FONT_NORMAL_DRAWING_STYLE); //MEDIUM_MENU_GREEN_NO_BG

  FGColour.r=0xff; FGColour.g=0x33; FGColour.b=0x33; FGColour.alpha=0xff;
  BGColour.r=0xff; BGColour.g=0xFf; BGColour.b=0xff; BGColour.alpha=0x0;
  create_font (&FGColour, &BGColour, MEDIUM_MENU_FONT, FONT_NORMAL_DRAWING_STYLE); //MEDIUM_MENU_RED_NO_BG

  FGColour.r=0xff; FGColour.g=0x99; FGColour.b=0x00; FGColour.alpha=0xff;
  BGColour.r=0xff; BGColour.g=0xFf; BGColour.b=0xff; BGColour.alpha=0x0;
  create_font (&FGColour, &BGColour, MEDIUM_MENU_FONT, FONT_NORMAL_DRAWING_STYLE); //MEDIUM_MENU_ORANGE_NO_BG
  
  FGColour.r=0xff; FGColour.g=0xff; FGColour.b=0xff; FGColour.alpha=0xff;
  BGColour.r=0xff; BGColour.g=0xFf; BGColour.b=0xff; BGColour.alpha=0x0;
  create_font (&FGColour, &BGColour, SMALL_MENU_FONT, FONT_NORMAL_DRAWING_STYLE); //SMALL_MENU_WHITE_NO_BG

*/

// End of game specific code
// --------------------------------------------------------------------------

}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: create_font             | AUTHOR(s): Stuart Ball  | DATE STARTED: 1 Jan 2004
// +
// | DESCRIPTION: creates a font the game can use, return a font ref used in print_string
// +----------------------------------------------------------------ROUTINE HEADER----
prepared_font_ref create_font (FColourSpec* FGColour, FColourSpec* BGColour, template_font_id which_font, int drawing_style)
{
 prepared_font_ref return_val;
 prepared_font* built_font;
 unsigned char* dest_ptr;
 int rgba_w, rgba_h;
 template_font_t* source;
 int source_h;
 unsigned char* source_ptr;
 int n; 
 SDL_Surface *temp_surface;
 BOOL anti_alias=FALSE;
 
 if ((which_font<0) || (which_font>NUMBER_OF_FONTS)) { return FONT_INVALID_TEMPLATE_FONT_ID; }
 if (new_font_ref==MAX_NUMBER_OF_PREPARED_FONTS)  { return FONT_TOO_MANY_PREPARED_FONTS; } //Too many prepared fonts. Increase MAX_NUMBER_OF_PREPARED_FONTS
 
 source=&fonts[which_font];
 if (source->the_SDL_surface->format->palette->ncolors==256) //if 8 bit greyscale then change what we're looking for accordingly
 {
  anti_alias=TRUE;
 }
 
 source_ptr=(unsigned char*)source->the_SDL_surface->pixels;
 //get storage for the font
 built_font= new prepared_font;
 
 //get actual size rather than that contained in the bmp
 source_h=source->the_SDL_surface->h;
 rgba_w=source->the_SDL_surface->w; rgba_h=source_h-1;
 built_font->h=rgba_h;
 //get memory for temp rgba buffer
 dest_ptr = new unsigned char[rgba_w*rgba_h*4];
 
 //create rgba image as per the drawing style
 if (drawing_style==FONT_NORMAL_DRAWING_STYLE)
 {
     if (anti_alias==FALSE)
	 {
	   create_normal_font(rgba_w, rgba_h,source_ptr, dest_ptr, FGColour, BGColour);
	 }
	 else
	 {
	   create_normal_font_AA(rgba_w, rgba_h, source, dest_ptr, FGColour, BGColour);
     }
 } //end of normal drawing style
 else
 {
   //Other drawing styles can go here as function calls similar to above
   return FONT_INVALID_DRAWING_STYLE; //we only deal with normal drawing styles for now
 }
 
 //copy the widths and offsets (we could refer to the original font if memory was really tight)
 
 for (n=0;n<NUMBER_OF_GLYPHS;n++)
 {
  built_font->character_data[n].offset=source->character_data[n].offset;
  built_font->character_data[n].width=source->character_data[n].width;
 }
  
 //now create the SDL surface and set the alpha
   Uint32 rmask,gmask,bmask,amask;
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
  #else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
  #endif
  temp_surface = SDL_CreateRGBSurfaceFrom(dest_ptr, rgba_w, rgba_h, 32, rgba_w*4, rmask, gmask, bmask, amask);
  built_font->font_surface = SDL_DisplayFormatAlpha(temp_surface); //convert it for optimal blitting
  //built_font->font_surface=temp_surface;
  SDL_FreeSurface(temp_surface);
  delete dest_ptr;
  
  
 prepared_fonts[new_font_ref]=built_font; 
 return_val=new_font_ref;
 new_font_ref++; //inc for next call
 return return_val;
}



// +---------------------------------+-------------------------+-----------------------
// | TITLE: print_formatted_string_id| AUTHOR(s): Stuart Ball  | DATE STARTED: 10 Jan 2004
// +
// | DESCRIPTION: prints a formatted string_id  at a given location
// | Formatting as follows:
// | %L, %M, %S - font size, large, medium or small
// | %R - red, %W - white, %G - Green, %O - Orange
// | %X - 2 digit hex character code follows
// | NB: Not all colours are available in all sizes
// +-----------------------------------------------------------------ROUTINE HEADER----

void print_formatted_string(int x,int y, int string_id)
{
 char* source_string;
 char substring[SUBSTRING_MAX];
 int substring_index=0;
 char current_char;
 prepared_font_ref the_font=LARGE_MENU_WHITE_NO_BG; //default value
 BOOL escape;
 BOOL first_substring;
 source_string=get_text(string_id); //our formatted string
 first_substring=TRUE;
 escape=FALSE;
 
 for(;;)
 {
  current_char=*source_string;
  source_string++;
  if (current_char==0) break; //finished
  
  if (escape==TRUE)
  {
		//blow out the previous string
		if (substring_index>0) //do we have substring to print?
		{
			  substring[substring_index]=0; //terminate
		      
			  if (the_font==NOT_USED_FONT_ID) //check font is OK to print with
		      {
         	    fprintf(OUTPUT_FP, "ERROR: print_formatted_string - unknown font attributes in basic_text.zsl string_id %i. Probably font isn't available in the colour you have specified. \n");
		        return; //lets not crash
			   }
			  if (first_substring==TRUE)
			  {
					print_string(the_font,x,y,substring);
					first_substring=FALSE;
			  }
			  else
			  {
					print_string_relative(the_font, 0, 0, substring); //continue from last internal x,y
			  }
			  substring_index=0; //reset substring after we've printed it
		}
		
		//now process the escape sequence
		the_font=process_escape(current_char); //this may be invalid at this time, s'ok

		
		escape=FALSE;
		continue;
  } //end of escape==TRUE
  
  if (current_char=='%') 
  { 
    if (*source_string=='%') //escaped %?
	{
       source_string++;
	}
	else
	if ((*source_string=='X') || (*source_string=='x') ) //escaped %?
	{
	char hex_char;
	BOOL hex_error=FALSE;
	 source_string++;
	 hex_char=(*source_string); source_string++;
	 if (hex_char>='A') { hex_char=hex_char-'A'+10; }
	 else
	 if (hex_char>='0') { hex_char-='0'; }
     if (hex_char<0 || hex_char>15)
	 {
         	fprintf(OUTPUT_FP, "ERROR: print_formatted_string - unknown hex value in basic_text.zsl string_id %i.\n",string_id);
	        hex_error=TRUE;
	 }	 

	 current_char=hex_char<<4;

	 hex_char=(*source_string); source_string++;
	 if (hex_char>='A') { hex_char=hex_char-'A'+10; }
	 else
	 if (hex_char>='0') { hex_char-='0'; }
	 current_char+=hex_char;
     if (hex_char<0 || hex_char>15)
	 {
         	fprintf(OUTPUT_FP, "ERROR: print_formatted_string - unknown hex value in basic_text.zsl string_id %i.\n",string_id);
	        hex_error=TRUE;
	 }	 
	 
	 if (hex_error==TRUE) { current_char='!'; }
	 
	}
	else
	{
      escape=TRUE; 
  	  continue;
    }
  } //end of % check
  
  //otherwise just store the character in the substring and move on
  substring[substring_index]=current_char;
  substring_index++;
  
  //can't put this check on debug only as user may be changing the strings
  if (substring_index>=SUBSTRING_MAX)
  {
	fprintf(OUTPUT_FP, "ERROR: print_formatted_string - substring too long. Maximum of %i characters. basic_text.zsl string_id=%i\n",SUBSTRING_MAX, string_id);
	break;
  }
 } //end of loop
 
//finally blow out the last substring
substring[substring_index]=0; //terminate

if (first_substring==TRUE)
{
	print_string(the_font,x,y,substring);
}
else
{
	print_string_relative(the_font, 0, 0, substring); //continue from last internal x,y
}

}




// +--------------------------------+-------------------------+-----------------------
// | TITLE: print_string            | AUTHOR(s): Stuart Ball  | DATE STARTED: 2 Jan 2004
// +
// | DESCRIPTION: prints a string with a given prepared font at a given location
// +----------------------------------------------------------------ROUTINE HEADER----

void print_string(prepared_font_ref which_font, int x, int y, char* the_string)
{
 prepared_font* built_font;
 unsigned char c;	// unsigned so that we can have characters >= 128 for international sets
 int foffset,fwidth;
 SDL_Rect dest_rect, source_rect;
 SDL_Surface *screen;
 
 built_font=prepared_fonts[which_font];
 screen = the_screen::get_instance()->get_screen_ref();

 dest_rect.x=x; dest_rect.y=y;
 
 
 while((c=*the_string) >= LOWER_GLYPH)
  {
	  c-=LOWER_GLYPH;
	  foffset=built_font->character_data[c].offset;
	  fwidth=built_font->character_data[c].width;
	  source_rect.x=foffset;
	  source_rect.w=fwidth;
	  source_rect.y=0;
	  source_rect.h=built_font->h;
	  dest_rect.w=fwidth; dest_rect.h=source_rect.h;
	   
	  SDL_BlitSurface(built_font->font_surface, &source_rect, screen, &dest_rect);

	  dest_rect.x += fwidth;
	  the_string++;
  }
last_drawn_x=dest_rect.x;
last_drawn_y=y;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: print_counted_string    | AUTHOR(s): Stuart Ball  | DATE STARTED: 4 Jan 2004
// +
// | DESCRIPTION: prints a string of length count with a given prepared font at a given location
// +----------------------------------------------------------------ROUTINE HEADER----

void print_counted_string(prepared_font_ref which_font, int x, int y, char* the_string, int count)
{
 prepared_font* built_font;
 unsigned char c;	// unsigned so that we can have characters >= 128 for international sets
 int foffset,fwidth;
 SDL_Rect dest_rect, source_rect;
 SDL_Surface *screen;
 
 built_font=prepared_fonts[which_font];
 screen = the_screen::get_instance()->get_screen_ref();

 dest_rect.x=x; dest_rect.y=y;
 
 
 while((c=*the_string) >= LOWER_GLYPH && count>0)
  {
	  c-=LOWER_GLYPH;
	  foffset=built_font->character_data[c].offset;
	  fwidth=built_font->character_data[c].width;
	  source_rect.x=foffset;
	  source_rect.w=fwidth;
	  source_rect.y=0;
	  source_rect.h=built_font->h;
	  dest_rect.w=fwidth; dest_rect.h=source_rect.h;
	   
	  SDL_BlitSurface(built_font->font_surface, &source_rect, screen, &dest_rect);

	  dest_rect.x += fwidth;
	  the_string++;
	  count--;
  }

last_drawn_x=dest_rect.x;
last_drawn_y=y;

}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: print_string_relative   | AUTHOR(s): Stuart Ball  | DATE STARTED: 10 Jan 2004
// +
// | DESCRIPTION: prints a string with a given prepared font relative to the last drawing location
// +----------------------------------------------------------------ROUTINE HEADER----

void print_string_relative(prepared_font_ref which_font, int x_off, int y_off, char* the_string)
{
 prepared_font* built_font;
 unsigned char c;	// unsigned so that we can have characters >= 128 for international sets
 int foffset,fwidth;
 SDL_Rect dest_rect, source_rect;
 SDL_Surface *screen;
 
 built_font=prepared_fonts[which_font];
 screen = the_screen::get_instance()->get_screen_ref();

 dest_rect.x=last_drawn_x+x_off; 
 dest_rect.y=last_drawn_y+y_off;
 
 while((c=*the_string) >= LOWER_GLYPH)
  {
	  c-=LOWER_GLYPH;
	  foffset=built_font->character_data[c].offset;
	  fwidth=built_font->character_data[c].width;
	  source_rect.x=foffset;
	  source_rect.w=fwidth;
	  source_rect.y=0;
	  source_rect.h=built_font->h;
	  dest_rect.w=fwidth; dest_rect.h=source_rect.h;
	   
	  SDL_BlitSurface(built_font->font_surface, &source_rect, screen, &dest_rect);

	  dest_rect.x += fwidth;
	  the_string++;
  }
  
last_drawn_x=dest_rect.x;
last_drawn_y=dest_rect.y;
}




// +--------------------------------+-------------------------+-----------------------
// | TITLE: get_font_height         | AUTHOR(s): Stuart Ball  | DATE STARTED: 27 Dec 2003
// +
// | DESCRIPTION: Returns the height of a given prepared_font
// +----------------------------------------------------------------ROUTINE HEADER----
int get_font_height(prepared_font_ref which_font)
{
 return prepared_fonts[which_font]->h;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: kill_fonts              | AUTHOR(s): Stuart Ball  | DATE STARTED: 27 Dec 2003
// +
// | DESCRIPTION: Release the storage used for the font bitmap data
// +----------------------------------------------------------------ROUTINE HEADER----
void kill_fonts(void)
{
int n;
prepared_font* built_font;

//kill template fonts
  for (n=0;n<NUMBER_OF_FONTS;n++)
  {
   if (fonts[n].the_SDL_surface!=NULL)
   {
     //dispose of the storage
	 SDL_FreeSurface(fonts[n].the_SDL_surface);
   }
  }
  
//kill prepared fonts
  for (n=0;n<(new_font_ref-1);n++)
  {
     built_font=prepared_fonts[n];
	 SDL_FreeSurface(built_font->font_surface);
	 delete built_font;
  }

}

// ***********************************************************************************
// * PRIVATE ROUTINES
// * 


// -------------------FONT DRAWING STYLES - Non AA
// +--------------------------------+-------------------------+-----------------------
// | TITLE: create_normal_font      | AUTHOR(s): Stuart Ball  | DATE STARTED: 2 Jan 2004
// +
// | DESCRIPTION: draws the RGBA font in NORMAL style
// +----------------------------------------------------------------ROUTINE HEADER----
static void create_normal_font(int rgba_w, int rgba_h, unsigned char* source_ptr, unsigned char* dest_ptr, FColourSpec* FGColour, FColourSpec* BGColour )
{
int looph, loopw;
 unsigned char source_pix;

	 for (looph=0; looph<rgba_h; looph++)
	 {
	   for (loopw=0;loopw<rgba_w; loopw++)
	   {
		source_pix=*source_ptr;
		source_ptr++;
		if (source_pix==DATA)
		{
		 *dest_ptr=FGColour->r; dest_ptr++; *dest_ptr=FGColour->g; dest_ptr++;
		 *dest_ptr=FGColour->b; dest_ptr++; *dest_ptr=FGColour->alpha; dest_ptr++;
		}
		else
		{
		 *dest_ptr=BGColour->r; dest_ptr++; *dest_ptr=BGColour->g; dest_ptr++;
		 *dest_ptr=BGColour->b; dest_ptr++; *dest_ptr=BGColour->alpha; dest_ptr++;
		}    
	   }
	 } //scanned the font

}


// -------------------FONT DRAWING STYLES - AA
// +--------------------------------+-------------------------+-----------------------
// | TITLE: create_normal_font_AA   | AUTHOR(s): Stuart Ball  | DATE STARTED: 2 Jan 2004
// +
// | DESCRIPTION: draws the RGBA font in NORMAL AA style
// +----------------------------------------------------------------ROUTINE HEADER----
static void create_normal_font_AA(int rgba_w, int rgba_h, template_font_t* where, unsigned char* dest_ptr, FColourSpec* FGColour, FColourSpec* BGColour )
{
 int looph, loopw;
 unsigned char source_pix;
 unsigned char* source_ptr;
 SDL_Color* clut_start;
 SDL_Colour clut_entry;
 
 source_ptr=(unsigned char*)where->the_SDL_surface->pixels;
 clut_start=where->the_SDL_surface->format->palette->colors;
 
	 for (looph=0; looph<rgba_h; looph++)
	 {
	   for (loopw=0;loopw<rgba_w; loopw++)
	   {
		source_pix=*source_ptr; //this is a clut reference - each entry in the CLUT is an SDL_Color
		source_ptr++;
		clut_entry=*(clut_start+source_pix);
		//now we can take any entry in the colour (lets say red) and multiply it by the components 
		//if it's anything but the BG colour
		if (clut_entry.r!=255) //not BG colour
		{
		 int red,green,blue, alpha;
		 red=green=blue=alpha=255-clut_entry.r;
		 red*=FGColour->r; red>>=8;
		 green*=FGColour->g; green>>=8;
		 blue*=FGColour->b; blue>>=8;
		 alpha*=FGColour->alpha; alpha>>=8;
		 *dest_ptr=red; dest_ptr++; *dest_ptr=green; dest_ptr++;
		 *dest_ptr=blue; dest_ptr++; *dest_ptr=alpha; dest_ptr++;
		}
		else //go for the BG colour
		{
		 *dest_ptr=BGColour->r; dest_ptr++; *dest_ptr=BGColour->g; dest_ptr++;
		 *dest_ptr=BGColour->b; dest_ptr++; *dest_ptr=BGColour->alpha; dest_ptr++;
		}    
	   }
	 } //scanned the font

}



// -----------Internal private functions


// +--------------------------------+-------------------------+-----------------------
// | TITLE: process_escape          | AUTHOR(s): Stuart Ball  | DATE STARTED: 10 Jan 2004
// +
// | DESCRIPTION: Handles escape sequences from formatted string
// | Formatting as follows:
// | %L, %M, %S - font size, large, medium or small
// | %R - red, %W - white, %G - Green, %O - Orange
// | NB: Not all colours are available in all sizes
// |		      
// +----------------------------------------------------------------ROUTINE HEADER----
prepared_font_ref process_escape(char current_char)
{
 if (current_char=='L') { current_size=LARGE; }
 else
 if (current_char=='M') { current_size=MEDIUM; }
 else
 if (current_char=='S') { current_size=SMALL; }
 else
 if (current_char=='R') { current_colour=RED; }
 else
 if (current_char=='W') { current_colour=WHITE; }
 else
 if (current_char=='G') { current_colour=GREEN; }
 else
 if (current_char=='O') { current_colour=ORANGE; }
 else
 if (current_char=='B') { current_colour=BLUE; }
 else
 if (current_char=='Y') { current_colour=YELLOW; }
 else
 {
  	fprintf(OUTPUT_FP, "ERROR: process_escape - Unknown escape sequence in string\n");
	return NOT_USED_FONT_ID;

 }
 
 //calc return font
 if (current_size==LARGE)
 {
   if (current_colour==WHITE) { return LARGE_MENU_WHITE_NO_BG; }
   if (current_colour==RED) { return LARGE_MENU_RED_NO_BG; }
   if (current_colour==ORANGE) { return LARGE_MENU_ORANGE_NO_BG; }
   if (current_colour==GREEN) { return LARGE_MENU_GREEN_NO_BG; }
   if (current_colour==BLUE) { return LARGE_MENU_BLUE_NO_BG; }
   if (current_colour==YELLOW) { return LARGE_MENU_YELLOW_NO_BG; }
 }
 else
 if (current_size==MEDIUM)
 {
   if (current_colour==WHITE) { return MEDIUM_MENU_WHITE_NO_BG; }
   if (current_colour==RED) { return MEDIUM_MENU_RED_NO_BG; }
   if (current_colour==ORANGE) { return MEDIUM_MENU_ORANGE_NO_BG; }
   if (current_colour==GREEN) { return MEDIUM_MENU_GREEN_NO_BG; }
 }
 else
 {
   if (current_colour==WHITE) { return SMALL_MENU_WHITE_NO_BG; }
   if (current_colour==RED) { return NOT_USED_FONT_ID; }
   if (current_colour==ORANGE) { return NOT_USED_FONT_ID; }
   if (current_colour==GREEN) { return NOT_USED_FONT_ID; }
 }

return NOT_USED_FONT_ID;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: init_template_font      | AUTHOR(s): Stuart Ball  | DATE STARTED: 27 Dec 2003
// +
// | DESCRIPTION: Initialises a specific font from the 1 bit bmp file
// |		      Returns TRUE if initialised OK, else FALSE
// +----------------------------------------------------------------ROUTINE HEADER----
static bool init_template_font(template_font_t* where, char* filename)
{
 int err;
 where->the_SDL_surface=SDL_LoadBMP(filename);
 
 if (where->the_SDL_surface==NULL) { return FALSE; } //no file or wrong format.
 err=build_template_font_info(where);
 
 if (NOERR==err) return TRUE;
 //All font errors are fatal
 switch (err)
        {
			case FONT_ERR_NOT_ENOUGH_DATA:
			{
				fprintf(OUTPUT_FP, "FATAL: init_font - not enough data in bmp - %s\n",filename);
		        break;
			}
			case FONT_ERR_GLYPH_WIDTH_TOO_BIG:
			{
				fprintf(OUTPUT_FP, "FATAL: init_font - Not enough index marks OR one of the glyphs is too wide - %s\n",filename);
			    break;
			}
			case FONT_ERR_GLYPH_WIDTH_TOO_SMALL:
			{
				fprintf(OUTPUT_FP, "FATAL: init_font - one of the glyphs is too narrow - %s\n",filename);
			    break;
			}
		}
		
 request_quit(); //quit
 return FALSE;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: build_template_font_info| AUTHOR(s): Stuart Ball  | DATE STARTED: 27 Dec 2003
// +
// | DESCRIPTION: Builds all necessary font info
// +----------------------------------------------------------------ROUTINE HEADER----

int build_template_font_info(template_font_t* where)
{
int err; 

 SDL_LockSurface(where->the_SDL_surface);
 
 if (is_font_proportional(where))
 {
   err=build_template_proportional_font(where);
 } 
 else
 {
   err=build_template_fixed_width_font(where);
 }
 
 SDL_UnlockSurface(where->the_SDL_surface); 
 return err;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: build_fixed_width_font  | AUTHOR(s): Stuart Ball  | DATE STARTED: 28 Dec 2003
// +
// | DESCRIPTION: Builds the data for a fixed width font
// +----------------------------------------------------------------ROUTINE HEADER----
static int build_template_fixed_width_font(template_font_t* where)
{
 //Here the glyphs are all the same width, so we can go off fixed index
 //The data is a byte per pixel
 //We need to fill in offset and width for every glyph
 int n, offset;
 offset=0;
 for (n=0;n<NUMBER_OF_GLYPHS;n++)
 {
  where->character_data[n].offset=offset;
  where->character_data[n].width=fixed_index;
  offset+=fixed_index;

 }
 return NOERR;
}



// +--------------------------------+-------------------------+-----------------------
// | TITLE: build_proportional_font | AUTHOR(s): Stuart Ball  | DATE STARTED: 28 Dec 2003
// +
// | DESCRIPTION: Builds the data for a proportional font
// +----------------------------------------------------------------ROUTINE HEADER----
static int build_template_proportional_font(template_font_t* where)
{
int h,w,cwidth,x_off;
char* data;
int glyph_count;
unsigned char the_byte;
unsigned char index_data=DATA;

 if (where->the_SDL_surface->format->palette->ncolors==256) //if 8 bit greyscale then change what we're looking for accordingly
 {
  index_data=DATA256;
 }
 
 h=where->the_SDL_surface->h; w=where->the_SDL_surface->w;
 //skip all data to get at the index mark(s)
 data=(char*)where->the_SDL_surface->pixels;
 data+=w*(h-1); //point to bottom row of bmp at index marks
 
 glyph_count=0;
 x_off=0;

 while(glyph_count<NUMBER_OF_GLYPHS) //for each character
 {
	   for (cwidth=1;cwidth<BIGGEST_GLYPH_WIDTH;cwidth++) //search for index mark
	   {
			 the_byte=*data;
			 data++;
			 x_off++; //update running total of x offset
			 if (the_byte==index_data) { break; } //found an index mark
			 
			 
			 if (x_off==(w-1)) //about to run off the end of the data?
			 {
			  //fatal, out of data - not enough index marks, don't read any further
			  return FONT_ERR_NOT_ENOUGH_DATA;
			 }
	   }
	   
	   if (cwidth==BIGGEST_GLYPH_WIDTH) //error - index mark not found
	   {
	   //fatal - font width too big
		  return FONT_ERR_GLYPH_WIDTH_TOO_BIG;
	   }
	   
	   if (cwidth<=2)
	   {
		//fatal - width too small
		  return FONT_ERR_GLYPH_WIDTH_TOO_SMALL;
	   }
	   //store this offset
	   where->character_data[glyph_count].width=cwidth;
	   where->character_data[glyph_count].offset=(x_off-cwidth); //x_off is the right hand side of the char
	   glyph_count++; //next char
 }
 
 return NOERR;
 
}




// +--------------------------------+-------------------------+-----------------------
// | TITLE: is_font_proportional    | AUTHOR(s): Stuart Ball  | DATE STARTED: 27 Dec 2003
// +
// | DESCRIPTION: Looks at the data and decides if proportional or not.
// |			  If there's just the one index mark on the bottom line then it isn't
// |              Can't generate fatal errors
// +----------------------------------------------------------------ROUTINE HEADER----
static bool is_font_proportional(template_font_t* where)
{
int h,w,n; 
char* data;
int index2;
unsigned char the_byte;
unsigned char index_data=DATA;

 if (where->the_SDL_surface->format->palette->ncolors==256) //if 8 bit greyscale then change what we're looking for accordingly
 {
  index_data=DATA256;
 }
 
 h=where->the_SDL_surface->h; w=where->the_SDL_surface->w;
 //skip all data to get at the index mark(s)
 data=(char*)where->the_SDL_surface->pixels;
 data+=w*(h-1); //point to bottom row

 fixed_index=index2=0;
 
 for (n=0;n<BIGGEST_GLYPH_WIDTH;n++)
 {
   the_byte=*data;
   data++;
   if (the_byte==index_data) { break; }
 }
 
 //We have an index at n. Is it the only one?
 fixed_index=n+1; //make a note incase it's a fixed width font
 for (;n<BIGGEST_GLYPH_WIDTH*2;n++)
 {
   the_byte=*data;
   data++;
   if (the_byte==index_data) 
   {
    index2=n;
    break;
   }
 }
 
 if (index2!=0) //we have proportional font
 { return TRUE; }
 else
 { return FALSE; }
 
}




//<EOF>



