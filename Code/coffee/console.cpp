// *************************************MODULE HEADER*********************************
// * FILENAME           : console.c
// * MODULE TITLE       : Untitled 1
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 05:38:16 on 2nd July 2000
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
/* ***********************************************************************************
 * $Log: console.c,v $
 * Revision 1.1.1.1  2003/09/05 22:35:15  stu_c
 * First Imported.
 *
 * Revision 1.10  2002/09/08 02:17:32  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.9  2002/02/15 23:25:49  stu
 * Just removed some commented out code
 *
 * Revision 1.8  2001/12/29 01:11:42  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.7  2001/11/07 22:21:55  rob
 * No game input from keyboard when in input mode.
 *
 * Revision 1.6  2001/11/06 20:11:15  rob
 * Key input changes
 *
 * Revision 1.5  2001/11/03 10:33:32  rob
 * Fix for console window not working under X.
 *
 * Revision 1.4  2001/11/02 23:18:43  stu
 * Further OS X work. Enabled syscon logging.
 *
 * Revision 1.3  2001/11/02 19:32:50  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:29:06  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:09:41  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:04  rob
 * First Imported.
 *
 * Revision 1.8  2001/09/09 21:30:54  rob
 * Added ability to see console screen if something goes wrong during boot sequence by clicking on the Debugger button.
 *
 * Revision 1.7  2001/09/07 20:40:57  rob
 * Added console debug_print
Added instant_console flag
 *
 * Revision 1.6  2001/09/04 20:40:53  stu
 * SDS/Sim etc
 *
 * Revision 1.5  2001/08/07 19:56:05  rob
 * 1. Added support for KEY? into console.
2. Added check for LSGF core version into c coffee loader.
 *
 * Revision 1.4  2001/01/15 19:52:02  stu
 * Bolts, fuel freeze
 *
 * Revision 1.3  2001/01/07 20:10:43  rob
 * Scroll was filling with blanks
 *
 * Revision 1.2  2001/01/02 18:57:17  stu
 * CLS fills with zeros. Various OGL calls in-lined
 *
 * Revision 1.1.1.1  2001/01/01 21:13:34  rob
 * First Imported.
 *
 * Revision 1.0.0.1  2000/08/21 22:00:12  stu
 * First Imported.
 *
 */


// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"


  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <Dialogs.h>		// for show console window
    #include <Types.h>
    #include <NumberFormatting.h>
    #include <Events.h>
  #endif
#include "zex_misc.h"
#endif //pch

#include "console.h"
#include "txtsplat.h"
#include "pictures.h"
#include "ogl.h"
#include "olg_text.h"
#include "keyboard.h"

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *
extern UInt32 gConsole;
extern int fps;
extern unsigned char number_string[12];

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *
int console_check_key(void);




int gShowConsole=0;
int last_console_frame=0;
int console_start_line=0;
int console_x,console_y;
int running_down,fully_down,running_up,fully_up;
int cursor_char_x, cursor_char_y, cursor_flash_count;
int cursor_state;
int instant_console=0;

int console_colour;
int cursor_colour;



char console_buffer [CONSOLE_LENGTH+80];	//text buffer of 1024 64 char lines
char console_attrib_buffer [CONSOLE_LENGTH+80];	//text attribs buffer of 1024 64 char lines




void init_console(void)
{
//int m,n;
	cursor_char_x=CURSOR_START_X;
	cursor_char_y=CURSOR_START_Y;
	cursor_flash_count=FLASH_RATE;
	cursor_state=1;
	
	console_cls(); 
/*	for (n=0;n<CONSOLE_ROWS;n++)
	{
	for (m=0;m<CONSOLE_COLUMNS;m++)
		{
		console_buffer[(n*CONSOLE_COLUMNS)+m]=RangedRdm(31,150);
		}
	} */
	console_x=90;
	console_y=CONSOLE_START_Y;
	
	console_colour=10;
	cursor_colour=10;
}


void console_cls (void)
{
int n,m;

for (n=0;n<CONSOLE_ROWS;n++)
{
for (m=0;m<CONSOLE_COLUMNS;m++)
	{
	console_buffer[(n*CONSOLE_COLUMNS)+m]=0;
	}
}
}

char curs_on[]= {'_',0};



char cursor_attrib_str[] = { 1 , 0 };
 
void run_console(void)
{
int x,y,n;
//int m;



if (gConsole && ZexTickCount()>last_console_frame)	//F1 key
  {

  last_console_frame=ZexTickCount()+10;
  gShowConsole^=1;
  gConsole=0;	//because isp is disabled and we yoyo otherwise
  if(gShowConsole==1)
    {
    disable_inputs();	// a shown console disables the inputs
    }
  else
    {
    enable_inputs();    // a hidden or hiding console enables the game inputs
    }
	
	
	//check if was running up or down
	if (gShowConsole==1)
	{
	if (running_up==1) 
	  {
	  running_up=0;
	  fully_up=0;
	  }
	}
	
	if (gShowConsole==0)
	{
	if (running_down==1) 
	    {
		running_down=0;
		fully_down=0;
	    }
	}
	
  }
  
if ((running_down==0) && (fully_down==0) && (gShowConsole==1) )
      { 
      running_down=1;	//start console slide down
      FlushEvents(everyEvent,0);
      }


if (running_down==1)
{
	fully_up=0;
	console_y+=LINE_SPACING;
	
	if(instant_console)
	  {
	  console_y=26;
	  }
	  
	if (console_y>25)		// how far down to scroll to (+/- 10 pixels)
	{
		fully_down=1;
		running_down=0;
	}
}

if ((running_up==0) && (fully_up==0) && (gShowConsole==0) )
running_up=1;	//start console slide up

if (running_up==1)
{
	fully_down=0;
	console_y-=LINE_SPACING;
	
	if(instant_console)
	  {
	  console_y=CONSOLE_START_Y-1;
	  }

	if (console_y<CONSOLE_START_Y)
	{
		fully_up=1;
		running_up=0;
	}
}


if (fully_up==0 )
{  

 x=10; y=10;
 //if (!running_up) SplatText_Transparent_Fade ("Zex: Coffee Console V1.00 (C)Lightsoft Software (tools) 2000",x,y,225);
 x=500;
 NumToString(fps,number_string);
 number_string[number_string[0]+1]=0;	//quick p to c hack
 number_string[0]=' ';
 SplatText_Transparent ("FPS:",x,y,10);
 SplatText_Transparent ((char *)number_string,x+70,y,10);
//display console text
//garbage fill

y=console_y;
x=console_x;

//for (n=0;n<CONSOLE_ROWS;n++)
//{
//for (m=0;m<CONSOLE_COLUMNS;m++)
//	  {
//	  console_buffer[(n*CONSOLE_COLUMNS)+m]=RangedRdm(32,128);
//	  }
//}

     OGL_set_pen_colour_from_index(console_colour);


for (n=0;n<CONSOLE_ROWS;n++)
 {
 	console_buffer[(n*CONSOLE_COLUMNS)+(CONSOLE_COLUMNS-1)]=0;	//just make sure we don't ufck up
// 	console_attrib_buffer[(n*CONSOLE_COLUMNS)+(CONSOLE_COLUMNS-1)]=0;	//just make sure we don't ufck up
 	
// 	SplatText_Console((char*)&console_buffer[n*CONSOLE_COLUMNS],x,y,console_colour,(char*)&console_attrib_buffer[n*CONSOLE_COLUMNS]);
     OGL_moveto(x,y);
     OGL_draw_monaco_bold((char*)&console_buffer[n*CONSOLE_COLUMNS]);

 	y+=LINE_SPACING;
 }

cursor_flash_count--;
if (cursor_flash_count<0)
{
 cursor_state^=1;
 cursor_flash_count=FLASH_RATE;
}

//overlay the cursor
if (cursor_state==1)
 	SplatText_Console(curs_on,(cursor_char_x*6)+console_x,cursor_char_y*LINE_SPACING+console_y,cursor_colour,cursor_attrib_str);

  }
}


int stored_key=0;

int console_check_for_key(void)
{
if((fully_up==0) && (!running_up))
  {
  if(stored_key==0)
    {
    stored_key=console_return_any_key();
    }

  if(stored_key)
    {
    return -1;
    }
  else
    {
    return 0;
    }
  }
else
  {
  return 0;
  }
}


int console_return_any_key(void)
{
EventRecord my_event_record;
unsigned char ascii_code;


ascii_code=0;

if((fully_up==0) && (!running_up))
{
if(stored_key)
  {
  ascii_code=stored_key;
  stored_key=0;
  return ascii_code;
  }
  
//check for cursor arrow keys
if ( (GetNextEvent(0xffff,&my_event_record)))
{
	if ((my_event_record.what==keyDown) || (my_event_record.what==autoKey))
	{
       cursor_state=1;
 	   cursor_flash_count=FLASH_RATE;		//make cursor visible  
   	  
	   ascii_code=my_event_record.message;
  	  

  	   if (ascii_code==ESCAPE)
  	    {
	    gConsole=1;
	    run_console();
	    ascii_code=0;
	   }

	}
  }	
}  // end of running up/fully up
return ascii_code;
}




void do_scroll(void);

//console_buffer
//CONSOLE_ROWS CONSOLE_COLUMNS
//cursor_char_x, cursor_char_y


void console_emit(char the_char)
{
console_buffer[(cursor_char_y*CONSOLE_COLUMNS)+cursor_char_x]=the_char;

cursor_char_x++;
if(cursor_char_x>=(CONSOLE_COLUMNS-1))		// allow for zero terminated space
	{
	cursor_char_y++;
	cursor_char_x=0;
	}
if (cursor_char_y>=CONSOLE_ROWS)
	{
	cursor_char_y--;
	do_scroll();
	}

}


void console_cr(void)
{
cursor_char_x=0;
cursor_char_y++;

if (cursor_char_y>=CONSOLE_ROWS)
	{
	cursor_char_y--;
	do_scroll();
	}

}


void do_scroll(void)
{
int dest,source;
int n,m;

dest=0; source=CONSOLE_COLUMNS;
for(n=0;n<CONSOLE_ROWS-1;n++)
	{
		for (m=0;m<CONSOLE_COLUMNS;m++)
			{
			console_buffer[dest]=console_buffer[source];
			//console_attrib_buffer[dest]=console_buffer[source];
			dest++;source++;
		  }
	}
//clear last line
for (n=0;n<CONSOLE_COLUMNS;n++)
	{
	 console_buffer[dest]=0;	// was ' ';
	 console_attrib_buffer[dest]=0;
	 dest++;
	}
}




// ************************************
static int debug_print_called_count;
void reset_debug()
{
debug_print_called_count=0;
}


void print_debug_blank()
{
 debug_print_called_count++;
}

#define MAXCOL 15

void print_debug(int number,char* string)
{
int x,y;

if (gShowConsole==1) return;

if (debug_print_called_count<MAXCOL) 
{
 x=10;
 y=debug_print_called_count*15+15;
}
else
if (debug_print_called_count<MAXCOL*2) 
{
 x=220;
 y=(debug_print_called_count-MAXCOL)*15;
}

else 
{
 x=440;
 y=(debug_print_called_count-MAXCOL*2)*15;
}

debug_print_called_count++;

 NumToString(number,number_string);
 number_string[number_string[0]+1]=0;	//quick p to c hack
 number_string[0]=' ';
 SplatText_Transparent (string,x,y,10);
 SplatText_Transparent ((char *)number_string,x+80,y,10);
}


// Special Routine that creates a window, and displays the contents
// of the console window. For use out of DSp/OGL modes.
//
void show_console_window(void)
{
	//variables we use for the window
	WindowPtr my_window;
	unsigned long finalTicks;
	GrafPtr oldPort;
	Rect	windRect;
	//variables we use for drawing the console
	unsigned char consoleString[256];
	int i,n;


	// setup a window
	SetRect(&windRect, 20, 50, 520, 420);
	my_window = NewCWindow(nil, &windRect, "\pConsole Copy", true, documentProc, 
						(WindowPtr) -1, false, 0);
        if(my_window!=0)
          {
	  GetPort(&oldPort);	//save current port
	  SetPort(GetWindowPort(my_window));	//make our port current
          
	  for (n=0;n<CONSOLE_ROWS;n++)
             {
	     console_buffer[(n*CONSOLE_COLUMNS)+(CONSOLE_COLUMNS-1)]=0;	//just make sure we don't ufck up
             
             i=0;
	     while ((consoleString[i+1]=console_buffer[(n*CONSOLE_COLUMNS)+i]) !='\0')	      
	     	{
	     	i++;
	     	}
	     consoleString[0]=i;	//make pascal
             
	     MoveTo(10,4+((n+1)*11));	//move to correct line (ish...)
	     DrawString(consoleString);	//print console line , note : Pascal string
	     
	     }
          
          #if !INTERFACE_LIB_BUILD
            QDFlushPortBuffer(GetWindowPort(my_window),NULL);
          #endif
	  
          //wait a second then clean up and quit
	  Delay (600,&finalTicks);	//wait 5 seconds
          

	  SetPort(oldPort);	//restore the old port
	  DisposeWindow(my_window);	//dispose of the window
          }
}
