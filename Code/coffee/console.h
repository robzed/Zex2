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

void init_console();
void run_console(void);
void console_cls(void);

int console_return_any_key(void);
int console_check_for_key(void);

void console_emit(char the_char);
void console_cr(void);

void reset_debug();
void print_debug(int number,char* string);
void print_debug_blank();

// make variables public, so coffee to make then visible to the forth

extern int console_colour;
extern int cursor_colour;
extern char console_buffer[];
extern char console_attrib_buffer[];
extern int cursor_char_x, cursor_char_y;

extern int instant_console;

void show_console_window(void);



#define CONSOLE_COLUMNS 64
#define CONSOLE_ROWS 32
#define CONSOLE_START_Y -340
#define CURSOR_START_X 0
#define CURSOR_START_Y 0
#define FLASH_RATE 7
#define ARROW_UP 0x1e
#define ARROW_DOWN 0x1f
#define ARROW_RIGHT 0x1d
#define ARROW_LEFT 0x1c
#define ESCAPE 0x1b
#define LINE_SPACING 11
#define CONSOLE_LENGTH CONSOLE_COLUMNS*CONSOLE_ROWS

