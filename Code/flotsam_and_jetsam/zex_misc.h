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

//zex_misc.ext
unsigned short RangedRdm( unsigned short min, unsigned short max );
unsigned short NDRangedRdm( unsigned short min, unsigned short max );
unsigned short ZexRandom();
void init_zex_random();

//void print_raw_resource(ResType the_resource, int theID, int x,int y);
void user_delay(int delay_ticks);
void num_to_string9(int value, Ptr string);
void num_to_string99(int value, Ptr string);
void num_to_string99_suppress_leading_zero(int value, Ptr string);
void num_to_string999(int value, Ptr string);
void show_loading_screen();
void show_text_in_loading_screen(const char *the_text);
void show_text_in_loading_screen_nocr(Str255 the_text);
void show_text_in_loading_screen_fcr(Str255 the_text);
void append_text_to_loading_screen(Str255 the_text);

void show_error_in_loading_screen(Str255 the_text);
void kill_loading_screen();
int Zstrlen(char *str);

void CToPascal(const char *str, char* dest_str);
void PascalToC (const unsigned char *str, char *dest_str);
void copystr (char *to, const char*from);
void pascal_copystr(unsigned char *to, const unsigned char *from);

void Zstrcpy(char *s, char *t);
int Zget_Cstring_len(char* the_string);
float Zconvert_to_float(char* the_word);
int Zconvert_string_to_int(char* the_string, int len);
void get_system_ticks();
unsigned int  ZexTickCount();

float Zraise_by_10(int number);


int ZNumToStringBig(double number,char * string);
void ZexNumToStringPascal(int number,unsigned char * string);
void ZNumToString(unsigned short number, unsigned char *string);
void ZNumToString4(int number, char *string);
void ZNumToString3(int number, char *string);
void ZNumToString2(int number, char *string);
void ZNumToString1(int number, char *string);

//standardlib funcs
void Zsrand(int seed);
short Zrand(void);

#if ZEX_ANVIL
void strcpy(const char *s, char *t);
int strlen (const char *str);
void strcat(const char *s, char* t);
#endif

