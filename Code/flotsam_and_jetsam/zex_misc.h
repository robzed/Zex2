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
void show_text_in_loading_screen(char *the_text);
void show_text_in_loading_screen_nocr(Str255 the_text);
void show_text_in_loading_screen_fcr(Str255 the_text);
void append_text_to_loading_screen(Str255 the_text);

void show_error_in_loading_screen(Str255 the_text);
void kill_loading_screen();
int Zstrlen(char *str);

void CToPascal(char *str, char* dest_str);
void PascalToC (unsigned char *str, unsigned char *dest_str);
void copystr (char *to, char*from);

void Zstrcpy(char *s, char *t);
int Zget_Cstring_len(char* the_string);
float Zconvert_to_float(char* the_word);
int Zconvert_string_to_int(char* the_string, int len);
void get_system_ticks();
unsigned int  ZexTickCount();

float Zraise_by_10(int number);


int ZNumToStringBig(double number,unsigned char * string);
void ZexNumToStringPascal(int number,unsigned char * string);
void ZNumToString(unsigned short number, unsigned char *string);
void ZNumToString4(int number, unsigned char *string);
void ZNumToString3(int number, unsigned char *string);
void ZNumToString2(int number, unsigned char *string);
void ZNumToString1(int number, unsigned char *string);

//standardlib funcs
void Zsrand(int seed);
short Zrand(void);

#if ZEX_ANVIL
void strcpy(char *s, char *t);
int strlen (char *str);
void strcat(char *s, char* t);
#endif

