//misc functions

/*
$Log: zex_misc.c,v $
Revision 1.4  2003/09/27 20:41:47  robp
Fixed float->int warnings.

Revision 1.3  2003/09/27 09:00:28  robp
Removal of Invalid Conversion Warning for C++ (I hate unsigned/signed chars)

Revision 1.2  2003/09/26 19:20:50  robp
Alteration for C++ const means internal unless specified extern. Made header extern so that C compiler wouldn't complain.

Revision 1.1.1.1  2003/09/05 22:35:26  stu_c
First Imported.

Revision 1.17  2002/09/08 02:03:37  stu_c
Precompiled header adjustments

Revision 1.16  2002/08/31 19:24:44  stu_c
Callsigns

Revision 1.15  2002/06/24 22:04:41  stu_c
Bunch of old code removed.

Revision 1.14  2002/04/14 15:59:26  stu
Further work

Revision 1.13  2002/02/04 23:10:23  stu
No diffs - bloody lurkers

Revision 1.12  2002/01/29 22:57:37  rob
pobs change

Revision 1.11  2001/12/29 01:10:35  stu
Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.

Revision 1.10  2001/12/04 23:36:01  stu
Motherships work

Revision 1.9  2001/11/24 19:30:23  stu
*** empty log message ***

Revision 1.8  2001/11/07 21:35:15  stu
Both X and IB now use same RAndom Gems.

Revision 1.7  2001/11/03 14:26:09  rob
Anvil build problems corrected.

Revision 1.6  2001/11/02 23:18:27  stu
Further OS X work. Enabled syscon logging.

Revision 1.5  2001/11/02 19:31:19  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.4  2001/10/26 22:43:53  rob
Anvil build changes

Revision 1.3  2001/10/23 20:45:15  stu
More carbonising

Revision 1.2  2001/10/22 21:28:35  rob
Carbon warning changes

Revision 1.1  2001/10/21 01:01:07  stu
Initial porting work

Revision 1.0.0.1  2001/10/17 20:46:04  rob
First Imported.

Revision 1.3  2001/05/28 01:55:02  stu
280501

Revision 1.2  2001/01/14 17:12:55  stu
Gourad Fix

Revision 1.1.1.1  2001/01/01 21:13:35  rob
First Imported.

Revision 1.4  2000/11/28 22:30:36  stu
Dunno.

Revision 1.3  2000/10/19 19:07:18  stu
Relative velocities, turbulence, planet names.

Revision 1.2  2000/09/13 20:36:46  stu
Changes for hud targetting and sun corona effects.

Revision 1.1  2000/08/27 01:36:39  stu
Hud changes

Revision 1.0.0.1  2000/08/21 22:02:05  stu
First Imported.

 */
/* example function returns value between min and max */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <Windows.h>
    #include <Quickdraw.h>
  #endif

  #include "3d_structures_def.h"
  #include "options.h"
  #include "zex_misc.h"
  #include	"error_handler.h"
  #include "game_defs.h"
#endif //pch


#include	"splat_control.h"
#include "olg_text.h"
//internal protos
void show_vers();
//extracts either a string before a decimal point or the string after 
void extract_string(char* source_string, char* dest_string,int selector);


//Zex's random function simply pulls a number out of an array of random numbers
//generated at init time. Will wrap after max_random_index calls.

#define max_random_index 90000
unsigned short random_array[max_random_index+1];	//filled at init time
int random_array_pos=0;



unsigned short RangedRdm( unsigned short min, unsigned short max )
/* assume that min is less than max */
{
	unsigned short	qdRdm;	/* treat return value as 0-65536 */
	long	range, t;
extern unsigned short random_array[max_random_index+1];
extern int random_array_pos;

//	qdRdm = ZexRandom();
    random_array_pos++;
    if (random_array_pos>max_random_index) random_array_pos=0;
    qdRdm=random_array[random_array_pos];

	range = max - min;
	t = (qdRdm * range) / 65536; 	/* now 0 <= t <= range */
	return( t+min );
}



//Non deterministic random - doesn't bugger with RangedRdm results
unsigned short NDRangedRdm( unsigned short min, unsigned short max )
/* assume that min is less than max */
{
	unsigned short	qdRdm;	/* treat return value as 0-65536 */
	long	range, t;

	qdRdm = Random();

	range = max - min;
	t = (qdRdm * range) / 65536; 	/* now 0 <= t <= range */
	return( t+min );
}


//returns a number between 0 and 65535
unsigned short ZexRandom()
{
random_array_pos++;
if (random_array_pos>max_random_index) random_array_pos=0;
return random_array[random_array_pos];
}

void init_zex_random()
{
extern unsigned short random_array[max_random_index+1];
extern int random_array_pos;
int i;

random_array_pos=0;
for (i=0;i<max_random_index;i++)
{ 
 random_array[i]=Random();
 
}
}


//standardlib functions

 void Zsrand(int seed)
 {
 SetQDGlobalsRandomSeed(seed);
 }

 short Zrand(void)
 {
 return Random();
 }
 
 #define EX_INT 0
 #define EX_FLOAT 1
 #define EX_EXP 2
 
 //Convert a string to a float value
 //SB 310703
float Zconvert_to_float(char* the_word)
{
char int_string[32];
char float_string[32];
char exp_string[32];
float float_part=0;
float int_part=0;
float exp_part=0;
float f_multiplier,e_multiplier;
int string_len;
int sign_total=1;
int exp_sign=1;
//This is a six stage process.
//1. get integer part
extract_string(the_word,int_string,EX_INT);
//2. get float part
extract_string(the_word,float_string,EX_FLOAT);
//3. get exp part - might be something like -3.8e-06
extract_string(the_word, exp_string,EX_EXP);

//4. convert float part to int number
string_len=Zget_Cstring_len(float_string);

if(string_len!=0) //I wanted to do this with no conditionals nad it does work, but this is a nice optimisation
{
        float_part=Zconvert_string_to_int(float_string, string_len);
        //4. Multiply by suitable divisor - e.g. .1 if 1 decimal place, .01 if 2
        f_multiplier=Zraise_by_10(string_len);
        f_multiplier=1/f_multiplier;
        float_part*=f_multiplier;
}
//5. convert integer part
string_len=Zget_Cstring_len(int_string);
int_part=Zconvert_string_to_int(int_string, string_len);
//6. Add float part
if (int_part<0) sign_total=-sign_total;
int_part=fabs(int_part);
int_part+=float_part;
//7 multiply by exponent
string_len=Zget_Cstring_len(exp_string);
if (string_len!=0)
{
	exp_part=Zconvert_string_to_int(exp_string, string_len);
	
	if (exp_part<0.0) exp_sign=-exp_sign;
	exp_part=fabs(exp_part);
	
		e_multiplier=Zraise_by_10(STATIC_CAST_TO_INT(fabs(exp_part)));
		if (exp_sign<0)
		{
		    e_multiplier=1/e_multiplier;
		}
	int_part*=e_multiplier;
}

if (sign_total<0) int_part=-int_part;
//8. return result
return int_part; //taadaah
}
 
//Given a number raise it to the power of 10 (roughly)
float Zraise_by_10(int number)
{
 float multiplier=1;
 while(number>0)
 {
  multiplier*=10;
  number--;
 }
return multiplier; 
}


//Given a sstring and a length have a go at converting it to a string...
int Zconvert_string_to_int(char* the_string, int len)
{
 int index=len-1;
 int multiplier=1;
 int result=0;
 char the_char;
 int sign=1;
 
 while(index>=0)
 {
   the_char=the_string[index];
   if (the_char=='-') { sign=-sign; break; }
   result+=(the_char-'0')*multiplier;
   index--;   multiplier*=10;
 }
 result*=sign;
 return result;
}



int Zget_Cstring_len(char* the_string)
{
 int index=0;
 char the_char;
    while (1)
    {
      the_char=the_string[index];
      if (the_char==0) break;
      if (the_char=='\n') break;
      if (the_char=='\r') break;
      index++;
    }
return index;
} 
//extracts either a string before a decimal point or the string after 
void extract_string(char* source_string, char* dest_string,int selector)
{
int src_index=0;
int dest_index=0;
char the_char;

  dest_string[0]=0; //just in case
  
  if (selector==EX_INT)
  {
    while (1)
    {
      the_char=source_string[src_index];
      if (the_char=='\n') break;
      if (the_char=='\r') break;
      if (the_char==0) break;
      if (the_char=='.') break;
      dest_string[src_index]=the_char;
      dest_string[src_index+1]=0;
      src_index++;
    }
  }
  else
  if (selector==EX_FLOAT)
  {
    while (1) //find '.'
    {
      the_char=source_string[src_index];
      if (the_char=='\n') return;
      if (the_char=='\r') return;
      if (the_char==0) return;
      if (the_char=='.') break;
      src_index++;
    }
   src_index++;
   //now extract the float part
    while (1)
    {
      the_char=source_string[src_index];
      if (the_char=='\n') break;
      if (the_char=='\r') break;
      if (the_char==0) break;
      if (the_char=='e') break;
      if (the_char=='E') break;
      if (the_char=='.') break;
      dest_string[dest_index]=the_char;
      src_index++; dest_index++;
    }
    dest_string[dest_index]=0;

  }
  else
  if (selector==EX_EXP) //some thing like  -3.8e-06
  {
    while (1) //find 'e' or 'E'
    {
      the_char=source_string[src_index];
      if (the_char=='\n') return;
      if (the_char=='\r') return;
      if (the_char==0) return;
      if (the_char=='e') break;
      if (the_char=='E') break;
      src_index++;
    }
   src_index++;
   //now extract the exp part
    while (1)
    {
      the_char=source_string[src_index];
      if (the_char=='\n') break;
      if (the_char=='\r') break;
      if (the_char==0) break;
      if (the_char=='.') break;
      dest_string[dest_index]=the_char;
      src_index++; dest_index++;
    }
    dest_string[dest_index]=0;

  }
  
} 

////ееThis routine prints an 8 bit RAW file with the width and height in the header
////at x,y in screen addr
//void print_raw_resource(ResType the_resource, int theID, int x, int y)
//{
//extern Ptr screenaddr;
//extern int screenwidth;
//
//int raw_w,raw_h;
//Handle the_raw;
//Ptr the_raw_ptr;
//
//the_raw=Get1Resource(the_resource,theID);
//if (!the_raw) report_error("Print_raw_resource: Resource missing");
//the_raw_ptr=*the_raw;
//
//}


//wait for delay_ticks ticks
void user_delay(int delay_ticks)
{
unsigned int stop_ticks;

stop_ticks=TickCount()+delay_ticks;
while (TickCount()<stop_ticks);
}


void num_to_string9(int value, Ptr string)
{
int units;
units=value;
(*string)=units+'0';
string++;
(*string)=0;
}

void num_to_string99(int value, Ptr string)
{
int tens,units;
tens=value/10;
units=value-(tens*10);
(*string)=tens+'0';
string++;
(*string)=units+'0';
string++;
(*string)=0;
}


void num_to_string99_suppress_leading_zero(int value, Ptr string)
{
int tens,units;
tens=value/10;
units=value-(tens*10);
if(tens==0)
(*string)=' ';
else
(*string)=tens+'0';


string++;
(*string)=units+'0';
string++;
(*string)=0;
}

void num_to_string999(int value, Ptr string)
{
int hundreds,tens,units;
hundreds=value/100;
value=value-hundreds*100;
tens=value/10;
value=value-tens*10;
units=value;

(*string)=hundreds+'0';
string++;
(*string)=tens+'0';
string++;
(*string)=units+'0';
string++;
(*string)=0;
}



WindowPtr our_window=0;

void show_loading_screen()
{



our_window=GetNewCWindow(128,0,(WindowPtr)-1);
SetPort(GetWindowPort(our_window));
RGBForeColor(&rgbGreen);
TextSize (9);
TextMode (0);

}

int boot_log;

void show_text_in_loading_screen(char *the_text)
{
extern int OGL_active;
int n;

#if ZEX_PROJECT_BUILDER==1 && PB_PRINT_TO_LOG_ENABLED
fprintf(stderr,the_text);
#endif

if (OGL_active==1)
{

   for (n=0;n<6; n++)
   {

   TestDrawGL();
   }
}
return;


}

void show_text_in_loading_screen_fcr(Str255 the_text)
{
Rect my_rect={44,0,440,620};
//int i;
if (our_window==0) return;

SetPort(GetWindowPort(our_window));
RGBForeColor(&rgbWhite);


//MoveTo(260,20);
//DrawString("\pZEXк йLSG1999.");
//MoveTo(250,40);
//DrawString("\pPlease wait. Loading...");

//RGBForeColor(&rgbWhite);

DrawString(the_text);

//for (i=0;i<15;i++)
{
ScrollRect(&my_rect,0,-12,0);
//user_delay(1);
}

user_delay(1);

}

void show_text_in_loading_screen_nocr(Str255 the_text)
{
//Rect my_rect={44,0,440,620};
//int i;
if (our_window==0) return;

SetPort(GetWindowPort(our_window));
RGBForeColor(&rgbWhite);


MoveTo(260,20);
DrawString("\pZEXк йLSG2000.");
MoveTo(250,40);
DrawString("\pPlease wait. Loading...");

//RGBForeColor(&rgbWhite);

MoveTo(30,420);
DrawString(the_text);


//user_delay(1);
}

void append_text_to_loading_screen(Str255 the_text)
{
//Rect my_rect={44,0,440,620};
//int i;
if (our_window==0) return;

SetPort(GetWindowPort(our_window));
RGBForeColor(&rgbWhite);
DrawString(the_text);
//user_delay(1);
}

void show_error_in_loading_screen(Str255 the_text)
{
Rect my_rect={0,0,440,620};
int i;
if (our_window==0) return;

SetPort(GetWindowPort(our_window));
TextSize (9);
RGBForeColor(&rgbRed);
TextMode (0);
MoveTo(30,420);
DrawString(the_text);

RGBForeColor(&rgbWhite);
MoveTo(230,450);
show_vers();

for (i=0;i<12;i++)
{
ScrollRect(&my_rect,0,-1,0);
//user_delay(1);
}


}

void show_vers()
{
DrawString("\pZex Exec v");
DrawString(VERS_STRING);
DrawString(DATE_STRING);
}


void kill_loading_screen()
{
DisposeWindow(our_window);
our_window=0;
}

#if ZEX_ANVIL

void strcat(const char *s, char *t)
{
// find end of destination
while(s!='\0')
  { s++; }

// copy string
while(*t!='\0')
  { *s=*t; s++; t++; }
  
*s='\0'; // put zero terminator on end
}


//copy t to s
void strcpy(const char *s, char *t)
{
while((*s=*t) != '\0')
  {
  s++;
  t++;
  }
}

// C string length routine

int strlen (const char *str)
	{
		char *p;

		p = str;
		while (*p++) ;
		return((long)(--p - str));
	}
#endif


//convert a C string to pascal
void CToPascal(const char *str, char *dest_str)
	{
	//char *p,*q;
        char temp;
        unsigned char len;

                temp=1;
		len=0;
		while (temp!=0)
		{
		temp=str[len];
		dest_str[len+1]=temp;
		len++;
		}
	        dest_str[0]=len-1;
	}


//convert pascal string to c
void PascalToC (const unsigned char *str, char *dest_str)
{
int len,n;
	len=str[0];
	for (n=0;n<len;n++) dest_str[n]=str[n+1];
	dest_str[n]=0;
}

unsigned int zex_ticks;

//called once per game frame to set up zex_ticks
void get_system_ticks()
{
zex_ticks=TickCount();
}

unsigned int  ZexTickCount()
{
return zex_ticks;
}

//converts an unsigned integer to a Pascal string
void ZexNumToStringPascal(int number,unsigned char * string)
{
unsigned char char_count;
unsigned int div;
char_count=1;
if (number<10)  goto units;
if (number<100) goto tens;
if (number<1000) goto hundreds;
if (number<10000) goto thousands;
if (number<100000) goto tenthousands;
if (number<1000000) goto hunthousands;
if (number<10000000) goto millions;
if (number<100000000) goto tenmillions;
if (number<1000000000) goto hunmillions;
string[1]='x';
string[0]=1;
return;

hunmillions: div=number/100000000;
number=number-(div*100000000);
string[char_count]=div+'0';
char_count++;

tenmillions: div=number/10000000;
number=number-(div*10000000);
string[char_count]=div+'0';
char_count++;

millions: div=number/1000000;
number=number-(div*1000000);
string[char_count]=div+'0';
char_count++;


hunthousands: div=number/100000;
number=number-(div*100000);
string[char_count]=div+'0';
char_count++;


tenthousands: div=number/10000;
number=number-(div*10000);
string[char_count]=div+'0';
char_count++;


thousands: div=number/1000;
number=number-(div*1000);
string[char_count]=div+'0';
char_count++;

hundreds: div=number/100;
number=number-(div*100);
string[char_count]=div+'0';
char_count++;


tens: div=number/10;
number=number-(div*10);
string[char_count]=div+'0';
char_count++;

units: string[char_count]=number+'0';
string[0]=char_count;
return;
}



//converts an unsigned integer to a C string. retruns string pos
int ZNumToStringBig(double number,char * string)
{
unsigned int div, units, char_count;

if (number==0)
{
 string[0]='N'; string[1]='o'; string[2]='n'; string[3]='e'; string[4]=0; 
 return 4;
}
char_count=0;
units=0;
if (number<10)  goto units;
if (number<100) goto tens;
if (number<1000) goto hundreds;
if (number<10000) goto thousands;
if (number<100000) goto tenthousands;
if (number<1000000) goto hunthousands;
if (number<10000000) goto millions;
units=1;
string[0]='~';
char_count=1;
number/=1000000;
if (number<10) goto units;
if (number<100) goto tens;
if (number<1000) goto hundreds;
units=2;
number/=1000;
if (number<10) goto units;
if (number<100) goto tens;
if (number<1000) goto hundreds;
if (number<10000) goto thousands;
if (number<100000) goto tenthousands;
if (number<1000000) goto hunthousands;
if (number<10000000) goto millions;

string[0]='x';
string[1]=0;
return 1;


millions: div=STATIC_CAST_TO_UINT(number/1000000);
number=number-(div*1000000);
string[char_count]=div+'0';
char_count++;


hunthousands: div=STATIC_CAST_TO_UINT(number/100000);
number=number-(div*100000);
string[char_count]=div+'0';
char_count++;


tenthousands: div=STATIC_CAST_TO_UINT(number/10000);
number=number-(div*10000);
string[char_count]=div+'0';
char_count++;


thousands: div=STATIC_CAST_TO_UINT(number/1000);
number=number-(div*1000);
string[char_count]=div+'0';
char_count++;

hundreds: div=STATIC_CAST_TO_UINT(number/100);
number=number-(div*100);
string[char_count]=div+'0';
char_count++;


tens: div=STATIC_CAST_TO_UINT(number/10);
number=number-(div*10);
string[char_count]=div+'0';
char_count++;

units: string[char_count]=STATIC_CAST_TO_INT(number+'0');

if (units==1)
{
  string[char_count+1]='M';
  string[char_count+2]=0;
}
else
if (units==2)
{
  char_count++;
  string[char_count]='B';
  char_count++;
  string[char_count]=0;
}
else
{
  char_count++;
  string[char_count]=0;
}
return char_count;
}



//ееееееееееееееееееееееееееееееееееееееееееееееееее
//convert unsigned short to 5 ascii digits - e.g. 128 comes out as 00128
void ZNumToString(unsigned short number, unsigned char *string)
{
int temp,temp1;

temp=number/10000;	//do 10000's
temp1=temp+'0';
string[0]=temp1;
number=number-(temp*10000);

temp=number/1000;	//do 1000's
temp1=temp+'0';
string[1]=temp1;
number=number-(temp*1000);

temp=number/100;	//you get the gist...
temp1=temp+'0';
string[2]=temp1;
number=number-(temp*100);

temp=number/10;
temp1=temp+'0';
string[3]=temp1;
number=number-(temp*10);

temp1=number+'0';
string[4]=temp1;

}
//ееееееееееееееееееееееееееееееееееееееееееееееееее
//convert unsigned short to 4 ascii digits - e.g. 128 comes out as 128
void ZNumToString4(int number, char *string)
{
int temp,temp1;
temp=number/1000;
temp1=temp+'0';
string[0]=temp1;
number=number-(temp*1000);

temp=number/100;	//you get the gist...
temp1=temp+'0';
string[1]=temp1;
number=number-(temp*100);

temp=number/10;
temp1=temp+'0';
string[2]=temp1;
number=number-(temp*10);

temp1=number+'0';
string[3]=temp1;

string[4]=0;

}

//ееееееееееееееееееееееееееееееееееееееееееееееееее
//convert unsigned short to 3 ascii digits - e.g. 128 comes out as 128
void ZNumToString3(int number, char *string)
{
int temp,temp1;

temp=number/100;	//you get the gist...
temp1=temp+'0';
string[0]=temp1;
number=number-(temp*100);

temp=number/10;
temp1=temp+'0';
string[1]=temp1;
number=number-(temp*10);

temp1=number+'0';
string[2]=temp1;

string[3]=0;

}

void ZNumToString2(int number, char *string)
{
int temp,temp1;


temp=number/10;
temp1=temp+'0';
string[0]=temp1;
number=number-(temp*10);

temp1=number+'0';
string[1]=temp1;

string[2]=0;

}


void ZNumToString1(int number, char *string)
{
int temp1;

temp1=number+'0';
string[0]=temp1;

string[1]=0;

}

void pascal_copystr(unsigned char *to, const unsigned char *from)
{
unsigned char length;

length = *from;		// length is first byte in pascal strings
*to = length;		// move length across
to++;
from++;

while(length!=0)	// while not empty
  {
  *to=*from;		// move byte across
  to++;			// move to next bytes
  from++;
  length--;		// one less to move
  }

}

void copystr (char *to, const char*from)
{
char temp;
temp=-1;
 while (temp!=0)
  {
  temp=*from;
  *to=temp;
  to++;
  from++;
  }
}