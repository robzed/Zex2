// *************************************MODULE HEADER*********************************
// * FILENAME           : Planet_Generator.c
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * DATE STARTED       : 19:37:44 on 29th August 1999
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************

/* $Log: Planet_Generator.c,v $
/* Revision 1.8  2002/09/08 02:04:16  stu_c
/* Precompiled header adjustments
/*
/* Revision 1.7  2002/05/31 23:52:34  stu
/* Removed all warnings from the code
/*
/* Revision 1.6  2002/02/24 03:42:02  stu
/* (Some) compiler warnings cleaned up
/*
/* Revision 1.5  2002/02/02 23:05:18  stu
/* Now takes a param that causes [S] 0, [P] 1, [M] 2 to be appended to the string
/*
/* Revision 1.4  2002/01/20 19:16:02  stu
/* Fixed bug whereby if first char of name was Q second letter would be U, is now u.
/*
/* Revision 1.3  2001/11/02 19:31:25  rob
/* Build variants for interface lib and carbon.
Coffee fixes after changes for X.
/*
/* Revision 1.2  2001/10/22 21:28:57  rob
/* Carbon warning changes
/*
/* Revision 1.1  2001/10/21 01:01:16  stu
/* Initial porting work
/*
/* Revision 1.0.0.1  2001/10/17 20:46:05  rob
/* First Imported.
/*
/* Revision 1.2  2001/01/14 17:17:57  stu
/* Gourad Fix
/*
/* Revision 1.1.1.1  2001/01/01 21:13:36  rob
/* First Imported.
/*
/* Revision 1.1  2000/10/19 19:04:28  stu
/* Relative velocities, turbulence, planet names.
/*
/* Revision 1.0.0.1  2000/08/21 22:02:59  stu
/* First Imported.
/*
 */

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "3d_structures_def.h"
  #include	"zex_misc.h"
#endif //pch


#include "Planet_Generator.h"


// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *
unsigned char get_consonant(int seed);
unsigned char get_vowel(int seed);
int is_vowel(unsigned char test);
void gen_char(int char_pos, int seed, char * where);
unsigned char random_vowel();
int is_cons(unsigned char test);
unsigned char get_capital_consonant(int seed);
//random pool for name generator
//we get a seed and must return a pascal string
//derived from the see

#if ZEX_CORE_CARBON==0
extern QDGlobals qd;
#endif

int seed_add=3;

void gen_planet_name(unsigned int seed, unsigned char * output_string, int input_type) //0=planet, 1=moon
{
int char_count,consonant;
unsigned int temp;
unsigned char temp_char=0;
int number_of_chars;
//char gened_char [3];	//pascal string of new char
int i;



if (seed_add>17) seed_add=3;	//reset odd add

//clac number of chars
number_of_chars=seed%10;

temp=RangedRdm(0,100);


if (number_of_chars<4) 
{
 if (temp>60)
 number_of_chars=4;
 else if (temp>30)
 number_of_chars=5;
 else
 number_of_chars=3;
}

char_count=0;

//for (i=0;i<number_of_chars;i++)
// {
//  gen_char(i,seed/(i+1), gened_char);
//  output_string[i+1]=gened_char[1];
//  char_count++;
// }
// 
//first part can be any char
temp=seed%25+'A';

if (temp=='X')	//dont want X
  temp=(seed/2)%25+'A';
  
if (temp=='X') temp=get_capital_consonant(seed);	//last ditch effort

output_string[1]=temp;
char_count=1;
//if it was a vowel, the second must be a consonant

//2nd char
if (temp=='Q')
output_string[2]='u';

else
{
if (is_vowel(temp)==1) consonant=1;
else
consonant=0;

//second char depends on consonant, and must be an even letter...
if (consonant==1) temp_char=get_consonant(seed/3);
else temp_char=get_vowel(seed/3);

output_string[2]=temp_char;
}
char_count++;
if (number_of_chars==2) goto done;

//3rd char
if (temp_char=='q')
output_string[3]='u';
else
{
temp=(seed/3)%25+seed_add;
if (temp>25) temp-=25;
temp+='a';
seed_add+=2;

output_string[3]=temp;;
}
char_count++;
if (number_of_chars==3) goto done;

//4th char
if (temp=='q')
output_string[4]='u';
else
{
temp=(seed/6)%25+seed_add;
if (temp>25) temp-=25;
temp+='a';
seed_add+=2;


output_string[4]=temp;;
}
char_count++;
if (number_of_chars==4) goto done;

//5th char
if (temp=='q')
output_string[5]='u';
else
{
temp=(seed/5)%25+seed_add;
if (temp>25) temp-=25;
temp+='a';
seed_add+=2;

output_string[5]=temp;;
}
char_count++;
if (number_of_chars==5) goto done;

//char 6
if (temp=='q')
output_string[6]='u';
else
{
if (is_vowel(temp)==1) consonant=1;
else
consonant=0;

//second char depends on consonant, and must be an even letter...
if (consonant==1) temp_char=get_consonant(seed);
else temp_char=get_vowel(seed/4);
}

output_string[6]=temp;;

char_count++;
if (number_of_chars==6) goto done;


//7th char
if (temp=='q')
output_string[7]='u';
else
{
temp=(seed/5)%25+seed_add;
if (temp>25) temp-=25;
temp+='a';
seed_add+=2;

output_string[7]=temp;;
}
char_count++;
if (number_of_chars==7) goto done;

//8th char
if (temp=='q')
output_string[8]='u';
else
{
temp=(seed/9)%25+seed_add;
if (temp>25) temp-=25;
temp+='a';
seed_add+=2;

output_string[8]=temp;;
}
char_count++;
if (number_of_chars==8) goto done;

done: 
output_string[0]=char_count+3; //for [P]

//now check the string for chars that can't be next to each other
//e.g. m followed by g etc

for (i=1;i<char_count;i++)
 {
 char next_char;
 char this_char;
  
 next_char=output_string[i+1];
 this_char=output_string[i];
 if (this_char>='A' && this_char<='Z') this_char+=32;	//to lower case 
//this switch determines the rules of the word composition and is tweaked
//so bad language can't come out.
//faves seen to date include:
//Qunt and God. 
  	    switch(this_char) {
	    case 'a':
			  if(next_char=='a') output_string[i+1]='b';
			  if(next_char=='o') output_string[i+1]='p';
				break;

				
            case 'b':
			  if(next_char=='c') output_string[i+1]='d';
			  if(next_char=='f') output_string[i+1]='b';
			  if(next_char=='g') output_string[i+1]='e';
			  if(next_char=='m') output_string[i+1]='a';
			  if(next_char=='n') output_string[i+1]='i';
			  if(next_char=='p') output_string[i+1]='e';
			  if(next_char=='q') output_string[i+1]='i';
			  if(next_char=='v') output_string[i+1]='a';
			  if(next_char=='w') output_string[i+1]='i';
			  if(next_char=='x') output_string[i+1]='b';
			  if(next_char=='y') output_string[i+1]='a';
			  if(next_char=='z') output_string[i+1]='e';
			  if(next_char=='d') output_string[i+1]=random_vowel();;
                break;
				
            case 'c':
			  if(next_char=='c') output_string[i+1]='i';	//dont allow double c's
			  if(next_char=='d') output_string[i+1]='c';
			  if(next_char=='f') output_string[i+1]='a';
			  if(next_char=='j') output_string[i+1]='a';
			  if(next_char=='m') output_string[i+1]='k';
			  if(next_char=='n') output_string[i+1]='i';
			  if(next_char=='p') output_string[i+1]='e';
			  if(next_char=='q') output_string[i+1]='l';
			  if(next_char=='v') output_string[i+1]='a';
			  if(next_char=='w') output_string[i+1]='l';
			  if(next_char=='x') output_string[i+1]='e';
			  if(next_char=='y') output_string[i+1]='a';
			  if(next_char=='z') output_string[i+1]='a';
			  if(next_char=='t') output_string[i+1]='o';

                break;
				
            case 'd':
			  if(next_char=='b') output_string[i+1]='a';
			  if(next_char=='c') output_string[i+1]='e';
			  if(next_char=='f') output_string[i+1]='i';
			  if(next_char=='g') output_string[i+1]='o';
			  if(next_char=='h') output_string[i+1]='u';
			  if(next_char=='j') output_string[i+1]='a';
			  if(next_char=='k') output_string[i+1]='e';
			  if(next_char=='m') output_string[i+1]='d';
			  if(next_char=='n') output_string[i+1]='d';
			  if(next_char=='p') output_string[i+1]='e';
			  if(next_char=='q') output_string[i+1]='a';

                break;
				
            case 'e':
                break;
				
            case 'f':
			  if(next_char=='b') output_string[i+1]='f';
			  if(next_char=='c') output_string[i+1]='a';
			  if(next_char=='d') output_string[i+1]='e';
			  if(next_char=='g') output_string[i+1]='o';
			  if(next_char=='h') output_string[i+1]='a';
			  if(next_char=='j') output_string[i+1]='e';
			  if(next_char=='k') output_string[i+1]='i';
			  if(next_char=='m') output_string[i+1]='l';
			  if(next_char=='n') output_string[i+1]='f';
			  if(next_char=='p') output_string[i+1]='e';
			  if(next_char=='s') output_string[i+1]='i';
			  if(next_char=='t') output_string[i+1]='o';
			  if(next_char=='v') output_string[i+1]='a';
			  if(next_char=='w') output_string[i+1]='e';
			  if(next_char=='x') output_string[i+1]='f';
			  if(next_char=='z') output_string[i+1]='f';
			  if(next_char=='u') output_string[i+1]='i';

                break;
				
            case 'g':
			  if(next_char=='b') output_string[i+1]='a';
			  if(next_char=='c') output_string[i+1]='g';
			  if(next_char=='d') output_string[i+1]='e';
			  if(next_char=='f') output_string[i+1]='i';
			  if(next_char=='j') output_string[i+1]='o';
			  if(next_char=='k') output_string[i+1]='h';
			  if(next_char=='l') output_string[i+1]='e';
			  if(next_char=='m') output_string[i+1]='a';
			  if(next_char=='n') output_string[i+1]='u';
			  if(next_char=='p') output_string[i+1]='g';
			  if(next_char=='t') output_string[i+1]='h';
			  if(next_char=='v') output_string[i+1]='i';
			  if(next_char=='w') output_string[i+1]='o';
			  if(next_char=='x') output_string[i+1]='i';
			  if(next_char=='z') output_string[i+1]='o';

                break;
				
            case 'h':
			  if(next_char=='b') output_string[i+1]='a';
			  if(next_char=='c') output_string[i+1]='o';
			  if(next_char=='d') output_string[i+1]='e';
			  if(next_char=='f') output_string[i+1]='i';
			  if(next_char=='g') output_string[i+1]='u';
			  if(next_char=='j') output_string[i+1]='a';
			  if(next_char=='l') output_string[i+1]='e';
			  if(next_char=='m') output_string[i+1]='o';
			  if(next_char=='n') output_string[i+1]='i';
			  if(next_char=='p') output_string[i+1]='a';
			  if(next_char=='q') output_string[i+1]='i';
			  if(next_char=='r') output_string[i+1]='a';
			  if(next_char=='s') output_string[i+1]='u';
			  if(next_char=='t') output_string[i+1]='o';
			  if(next_char=='v') output_string[i+1]='a';
			  if(next_char=='w') output_string[i+1]='u';
			  if(next_char=='x') output_string[i+1]='i';
			  if(next_char=='y') output_string[i+1]='o';
			  if(next_char=='z') output_string[i+1]='u';

                break;
				
            case 'i':
			  if(next_char=='i') output_string[i+1]='n';
                          if(next_char=='j') output_string[i+1]='l';
                          if(next_char=='w') output_string[i+1]='s';

                break;
				
            case 'j':
			  if(next_char=='b') output_string[i+1]='a';
			  if(next_char=='c') output_string[i+1]=random_vowel();
			  if(next_char=='d') output_string[i+1]='u';
			  if(next_char=='f') output_string[i+1]='s';
			  if(next_char=='g') output_string[i+1]='u';
			  if(next_char=='k') output_string[i+1]='i';
			  if(next_char=='l') output_string[i+1]='o';
			  if(next_char=='m') output_string[i+1]=random_vowel();
			  if(next_char=='p') output_string[i+1]='s';
			  if(next_char=='q') output_string[i+1]='e';
			  if(next_char=='r') output_string[i+1]='o';
			  if(next_char=='s') output_string[i+1]=random_vowel();
			  if(next_char=='t') output_string[i+1]='i';
			  if(next_char=='v') output_string[i+1]='s';
			  if(next_char=='w') output_string[i+1]='u';
			  if(next_char=='x') output_string[i+1]='i';
			  if(next_char=='z') output_string[i+1]='u';

                break;
				
            case 'k':
			  if(next_char=='b') output_string[i+1]='a';
			  if(next_char=='c') output_string[i+1]='s';
			  if(next_char=='d') output_string[i+1]='a';
			  if(next_char=='g') output_string[i+1]='e';
			  if(next_char=='j') output_string[i+1]='o';
              if(next_char=='n') output_string[i+1]='o';
              if(next_char=='m') output_string[i+1]='o';
              if(next_char=='r') output_string[i+1]='a';
              if(next_char=='f') output_string[i+1]='o';
                break;
				
            case 'l':
	      if(next_char=='b') output_string[i+1]='l';
              if(next_char=='j') output_string[i+1]='a';
              if(next_char=='c') output_string[i+1]='u';
              if(next_char=='d') output_string[i+1]='a';
              if(next_char=='n') output_string[i+1]='o';
              if(next_char=='m') output_string[i+1]='o';
              if(next_char=='r') output_string[i+1]='a';
              if(next_char=='f') output_string[i+1]='o';
              if(next_char=='h') output_string[i+1]='u';
              if(next_char=='v') output_string[i+1]='t';
			  if(next_char=='w') output_string[i+1]='u';
			  if(next_char=='x') output_string[i+1]='i';
                break;
				
            case 'm':
			  if(next_char=='b') output_string[i+1]='a';
              if(next_char=='j') output_string[i+1]='e';
			  if(next_char=='g') output_string[i+1]='o';
			  if(next_char=='c') output_string[i+1]='i';
                break;
				
            case 'n':
              if(next_char=='b') output_string[i+1]='o';
              if(next_char=='j') output_string[i+1]='n';
			  if(next_char=='v') output_string[i+1]='s';
			  if(next_char=='w') output_string[i+1]='u';
			  if(next_char=='x') output_string[i+1]='i';
			  if(next_char=='z') output_string[i+1]='u';

                break;
				
            case 'o':
              if(next_char=='e') output_string[i+1]='n';

                break;
				
            case 'p':
              if(next_char=='b') output_string[i+1]='a';
              if(next_char=='j') output_string[i+1]='p';
			  if(next_char=='v') output_string[i+1]='p';
			  if(next_char=='w') output_string[i+1]='u';
			  if(next_char=='x') output_string[i+1]='i';
			  if(next_char=='z') output_string[i+1]='u';
                break;
				
            case 'q':
                break;
				
            case 'r':
              if(next_char=='b') output_string[i+1]='s';
              if(next_char=='j') output_string[i+1]='o';
			  if(next_char=='r') output_string[i+1]='a';
              if(next_char=='c') output_string[i+1]='t';
              if(next_char=='d') output_string[i+1]='o';
              if(next_char=='j') output_string[i+1]='a';
			  if(next_char=='v') output_string[i+1]='t';
			  if(next_char=='n') output_string[i+1]='o';
			  if(next_char=='m') output_string[i+1]='u';
			  if(next_char=='w') output_string[i+1]='u';
			  if(next_char=='x') output_string[i+1]='i';
			  if(next_char=='z') output_string[i+1]='a';


                break;
				
            case 's':
              if(next_char=='b') output_string[i+1]='s';
              if(next_char=='j') output_string[i+1]='s';

                break;
				
            case 't':
              if(next_char=='b') output_string[i+1]='t';
              if(next_char=='c') output_string[i+1]='t';
              if(next_char=='d') output_string[i+1]='o';
              if(next_char=='f') output_string[i+1]= random_vowel();
              if(next_char=='j') output_string[i+1]='a';
			  if(next_char=='v') output_string[i+1]='t';
			  if(next_char=='n') output_string[i+1]='o';
			  if(next_char=='m') output_string[i+1]='u';
			  if(next_char=='w') output_string[i+1]='u';
			  if(next_char=='x') output_string[i+1]='i';
			  if(next_char=='z') output_string[i+1]='a';

                break;
				
            case 'u':
              if(next_char=='u') output_string[i+1]='s';
              if(next_char=='n') output_string[i+1]='a';
              if(next_char=='q') output_string[i+1]='d';

                break;
				
            case 'v':
              if(next_char=='b') output_string[i+1]='u';
              if(next_char=='j') output_string[i+1]='a';
              if(next_char=='k') output_string[i+1]='a';
			  if(next_char=='v') output_string[i+1]='e';
			  if(next_char=='w') output_string[i+1]='o';
			  if(next_char=='x') output_string[i+1]='u';
			  if(next_char=='s') output_string[i+1]='u';
			  if(next_char=='c') output_string[i+1]='o';
			  if(next_char=='z') output_string[i+1]='i';

                break;
				
            case 'w':
              if(next_char=='b') output_string[i+1]='u';
              if(next_char=='c') output_string[i+1]='a';
              if(next_char=='d') output_string[i+1]='i';
              if(next_char=='f') output_string[i+1]='u';
              if(next_char=='g') output_string[i+1]='o';
              if(next_char=='k') output_string[i+1]='i';
              if(next_char=='l') output_string[i+1]='u';
              if(next_char=='m') output_string[i+1]='o';
              if(next_char=='n') output_string[i+1]='e';
              if(next_char=='p') output_string[i+1]='o';
              if(next_char=='q') output_string[i+1]='a';
              if(next_char=='s') output_string[i+1]='i';
              if(next_char=='t') output_string[i+1]='e';

              if(next_char=='j') output_string[i+1]='a';
			  if(next_char=='v') output_string[i+1]='e';
			  if(next_char=='w') output_string[i+1]='o';
			  if(next_char=='x') output_string[i+1]='u';
			  if(next_char=='z') output_string[i+1]='i';

                break;
				
            case 'x':

              if(next_char=='b') output_string[i+1]='u';
              if(next_char=='c') output_string[i+1]='a';
              if(next_char=='d') output_string[i+1]='i';
              if(next_char=='f') output_string[i+1]='u';
              if(next_char=='g') output_string[i+1]='o';
              if(next_char=='h') output_string[i+1]='i';
              if(next_char=='j') output_string[i+1]='a';
              if(next_char=='k') output_string[i+1]='i';
              if(next_char=='l') output_string[i+1]='u';
      	      if(next_char=='m') output_string[i+1]='i';
	      if(next_char=='n') output_string[i+1]='a';
              if(next_char=='p') output_string[i+1]='o';
              if(next_char=='q') output_string[i+1]='a';
              if(next_char=='r') output_string[i+1]='a';
              if(next_char=='s') output_string[i+1]='i';
              if(next_char=='t') output_string[i+1]='e';
			  if(next_char=='v') output_string[i+1]='e';

			  if(next_char=='w') output_string[i+1]='o';
			  if(next_char=='x') output_string[i+1]='u';
			  if(next_char=='z') output_string[i+1]='i';


                break;
				
            case 'y':
              if(next_char=='b') output_string[i+1]='u';
              if(next_char=='c') output_string[i+1]='a';
              if(next_char=='d') output_string[i+1]='i';
              if(next_char=='f') output_string[i+1]='u';
              if(next_char=='g') output_string[i+1]='o';
              if(next_char=='k') output_string[i+1]='i';
              if(next_char=='l') output_string[i+1]='u';
			  if(next_char=='e') output_string[i+1]='s';
			  if(next_char=='v') output_string[i+1]='m';
			  if(next_char=='w') output_string[i+1]='n';
			  if(next_char=='x') output_string[i+1]='p';
			  if(next_char=='z') output_string[i+1]='d';
			  if(next_char=='y') output_string[i+1]='e';
                break;
				
            case 'z':
			  if(next_char=='v') output_string[i+1]='e';
			  if(next_char=='w') output_string[i+1]='o';
			  if(next_char=='x') output_string[i+1]='u';
			  if(next_char=='z') output_string[i+1]='i';
			  if(next_char=='p') output_string[i+1]='i';
			  if(next_char=='m') output_string[i+1]='o';
			  if(next_char=='n') output_string[i+1]='a';
			  if(next_char=='t') output_string[i+1]='i';
			  if(next_char=='c') output_string[i+1]='o';
			  if(next_char=='d') output_string[i+1]='a';
			  if(next_char=='f') output_string[i+1]='i';
			  if(next_char=='g') output_string[i+1]='o';
                break;
   }
 }

//now what we're gping to do, is run through the string looking
//for runs of three characters the same
if (char_count>3)
 {
  for (i=1;i<char_count-1;i++)
  {
  char next_char;
  char next_char2;
  char this_char;
  next_char2=output_string[i+2];  
  next_char=output_string[i+1];
  this_char=output_string[i];
  if (this_char==next_char && next_char==next_char2)
      {
       output_string[i+1]=random_vowel();	//replace middle with a
      }
  }
 }

//now run through the string looking
//for runs of three consonants and replace the middle witha vowel
if (char_count>3)
 {
  for (i=1;i<char_count-1;i++)
  {
  char next_char;
  char next_char2;
  char this_char;
  next_char2=output_string[i+2];  
  next_char=output_string[i+1];
  this_char=output_string[i];
  if (is_cons(this_char)==1 && is_cons(next_char)==1 && is_cons(next_char2)==1)
      {
       output_string[i+1]=random_vowel();	//replace middle with a
      }
  }
 }

//now run through the string looking
//for runs of three vowels and replace the middle witha cons
if (char_count>3)
 {
  for (i=1;i<char_count-1;i++)
  {
  char next_char;
  char next_char2;
  char this_char;
  next_char2=output_string[i+2];  
  next_char=output_string[i+1];
  this_char=output_string[i];
  if (is_vowel(this_char)==1 && is_vowel(next_char)==1 && is_vowel(next_char2)==1)
      {
       output_string[i+1]=get_consonant(seed/RangedRdm(1,5));	//replace middle with a
      }
  }
 }

if (input_type==1) //planet
{
 output_string[char_count+1]='[';
 output_string[char_count+2]='P';
 output_string[char_count+3]=']';

}
else
if (input_type==2) //moon
{
 output_string[char_count+1]='[';
 output_string[char_count+2]='M';
 output_string[char_count+3]=']';

}
else
if (input_type==0) //SUN
{
 output_string[char_count+1]='[';
 output_string[char_count+2]='S';
 output_string[char_count+3]=']';

}

//finally, convert to upper case
//  for (i=1;i<char_count+1;i++)
//  {
//   if(output_string[i]>='a' &&  output_string[i]<='z')
//      output_string[i]-=0x20;
//  }
}

//==========================================================

//is it a consonant (opposite of vowel)
int is_cons(unsigned char test)
{
if (test=='A' || test=='E' || test=='I' || test=='O' || test=='U' ||
    test=='a' || test=='e' || test=='i' || test=='o' || test=='u'
    ) return 0;
else
return 1;
}

int is_vowel(unsigned char test)
{
if (test=='A' || test=='E' || test=='I' || test=='O' || test=='U' ||
    test=='a' || test=='e' || test=='i' || test=='o' || test=='u'
    ) return 1;
else
return 0;
}

unsigned char random_vowel()
{
int temp;
 temp=RangedRdm(0,4);
 if (temp==0) return 'a';
 if (temp==1) return 'e';
 if (temp==2) return 'i';
 if (temp==3) return 'o';
 if (temp==4) return 'u';

return 0;
}

unsigned char get_consonant(int seed)
{
unsigned char temp_char;
temp_char=seed%25+'a';
if (is_vowel(temp_char)) temp_char++;
if (temp_char>'z') temp_char='z';
return temp_char;
}


unsigned char get_capital_consonant(int seed)
{
unsigned char temp_char;
temp_char=seed%25+'A';
if (is_vowel(temp_char)) temp_char++;
if (temp_char>'Z') temp_char='Z';
return temp_char;
}

unsigned char get_vowel(int seed)
{
int temp;
temp=seed%5;

if (temp==0) return 'a';
if (temp==1) return 'e';
if (temp==2) return 'i';
if (temp==3) return 'o';
if (temp==4) return 'u';
return 0;
}

//this routine generates either one or two characters, depending on the seed.
//it also sets the global variables vowel and consonant accordingly, 
//and stores the last character generated in last_char
//and takes a flag specifying whether the first character generated should be
//a vowel or consonant 
//void gen_char(int vc_flag, int seed, char * where)
int last_char,last2_char;
void gen_char(int char_pos, int seed, char * where)
{
int temp,seeda;
extern int last_char;
//extern int last_char2;

//if last was a q then return u
if (last_char=='q')
    {
	temp='u';
	goto end;
    }

///if this is char 2 then we must gen the alternate
if (char_pos==1)
 {
  if (is_vowel(last_char)==1)
     {
     temp=get_consonant(seed);
     goto end;
     }
  else
     {
     temp=get_vowel(seed);
     goto end;
     }
 }   
//gen char
temp=seed%26;
if (temp>25) temp=25;;
temp+='a';

//check for double vowels
if (is_vowel(last_char)==1)
 {
  if (is_vowel(temp)==1)
    {
    if(last_char==temp)
         {
         if (temp=='a' || temp=='i' || temp=='u')	//dont allow double aa ii uu
             {
              temp=get_consonant(seed);
             }
         }
    }
 }
//check for triple letters
seeda=seed;
while ((temp & last_char & last2_char)==temp)	//while they're all the same
    {
              seeda++;	//prevent posssible loop
              temp=get_consonant(seeda);
    }
    

end: last2_char=last_char;
last_char=temp;
where[0]=1;
where[1]=temp;

}


//must be called before we start generating names else names will vary!!!
void init_name_generator(void)
{
seed_add=3;
#if ZEX_CORE_CARBON==0
qd.randSeed=0x69006931;
#endif

init_zex_random();
}

// *********************************GRAPHICS ARE IN planet_gen_main.c


