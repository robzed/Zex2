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

//splat_control.ext

extern int monitor_w, monitor_h;
extern int screen_cent_x, screen_cent_y;
extern int off_640_x,off_480_y, win_off_x, win_off_y;


extern int remove_opengl_context_on_switch;


//Splat_Control.int
//extern	DSpContextReference		  mDisplayContext;
//extern DSpContextAttributes	mDisplayAttributes;
//extern GDHandle				mZexGDH;
extern CTabHandle			   mZexCLUT;
//extern CGrafPtr	backBuff;

extern const RGBColor	rgbBlack;
extern const RGBColor	rgbWhite;
extern const RGBColor	rgbRed;
extern const RGBColor	rgbOrange;
extern const RGBColor	rgbBlue;
extern const RGBColor	rgbYellow;
extern const RGBColor	rgbPurple;
extern const RGBColor	rgbGreen;
extern const RGBColor	rgbLightBlue;
extern const RGBColor	rgbDarkBlue;
extern const RGBColor	rgbGrey;
extern const RGBColor	rgbDarkGrey;

int init_screen();
int init_screen();
//sw splat functions
//void create_blackout_window();
//void create_game_window();
//void switch_to_256();
//void create_virtual_screen();
//void start_modifying_picture();
//void stop_modifying_picture();

//dsp and our functions
void screen_quit();
void GraphicsSelectContext();
//void GraphicsInitAttributes(DSpContextAttributes *inAttributes);
void GraphicsActive(void);
void GraphicsInactive(void);
void GraphicsPaused(void);

int run_ogl(void);
void TestDrawGL(void);
void clearGL();

void Show_it();

void splat_screen();
void deferred_vbl_proc();

Boolean ZexPseudoVBLProc (void);


LS_CGrafPtr Get_back_buffer(void);
void display_wait();

extern int use_zbuffer;


void vid_to_8_bit();
void vid_to_16_bit();
//show_ready();
void load_screen_splat();

//watchdog
void reset_watchdog();
void enable_watchdog();
void disable_watchdog();

// is drawsprockets running?
int drawsprockets_is_running(void);
