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

//raw cruncher
//asks user for a raw file, crunches it using simple rle, and saves as
//.zrc file
//SB 18/12/98
#include <files.h>
#include <standardfile.h>
#include <ConditionalMacros.h>
#include <Dialogs.h>
#include <Types.h>
#include <Quickdraw.h>
#include	<fonts.h>
#include "file_io.ext"

void init_mac(void);

#if !(MAC68K)
QDGlobals qd;
#endif

typedef struct {
UInt16 pwidth;
UInt16 pheight;
char *raw_data;
} LSRAW;

main()
{
Handle the_file_h;
LSRAW *output_file;

LSRAW *the_file;
StandardFileReply sf_reply;
char name_len;
int length_of_file,length_of_output_file=0,input_file_pos=0;
char * src;
char * dest;

unsigned char the_char,start_char,char_pos;
FSSpec the_file_fsspec;
init_mac();

StandardGetFile(0,-1,0,&sf_reply);
the_file_h=read_file(sf_reply.sfFile);
HLock (the_file_h);
the_file=(LSRAW*)*the_file_h;
length_of_file=GetHandleSize(the_file_h);
output_file=(LSRAW*)NewPtr(length_of_file*2);

//at the start of the data we have two shorts indicatiing rect. so copy those
(*output_file).pwidth=(*the_file).pwidth;
(*output_file).pheight=(*the_file).pheight;
length_of_output_file+=4;
input_file_pos+=4;
src=(char *)&the_file->raw_data;
dest=(char *)&output_file->raw_data;

//now code the picture using 8 bit rle
while (input_file_pos<length_of_file)
 {
 //crunch up some data - we output pairs of bytes, the first byte is the count, the second is the data
 start_char=(*src);
// src++;
 char_pos=0;
 the_char=(*src);
 while (the_char==start_char && char_pos<254)	//we use 255 as eof marker 
  {
  the_char=(*src);
  if (the_char==start_char)
  {  src++; 
  char_pos++; }
  }
 //store count and char
 *dest=char_pos;
 dest++;
 *dest=start_char;
 dest++;
 input_file_pos+=char_pos;
 length_of_output_file+=2;
}	//keep crunching

*dest=255;
dest++;
*dest=255;	//mark eof
dest++;
length_of_output_file+=2;

//now save the output file
//modify filename so last three chars become RCZ
name_len=sf_reply.sfFile.name[0];
sf_reply.sfFile.name[name_len]='Z';
sf_reply.sfFile.name[name_len-1]='C';
sf_reply.sfFile.name[name_len-2]='R';
write_file((char*) output_file,length_of_output_file, sf_reply.sfFile);

ExitToShell();
}

void init_mac(void)
{
extern QDGlobals qd;
	MaxApplZone();				// Grab application memory.

	InitGraf(&qd.thePort);		// Initialize QuickDraw variables for our program.
	InitFonts();				// Initialize fonts.
	InitWindows();				// Initialize the Window Manager.
	InitMenus();				// Ditto for the Menu Manager.
	TEInit();					// blah, blah Text Edit.
	InitDialogs(0L);			// blah, blah Dialog Manager.
	InitCursor();				// Set the cursor to the arrow cursor and init.
	
	
//	MoreMasters();				// Allocate a block of master pointers.
//	MoreMasters();				// And allocate more.
	
	GetDateTime((unsigned long *)&qd.randSeed);		// Randomize random seed.
//	FlushEvents(everyEvent, 0);	// Clear event queue of any pending events.
	FlushEvents(mDownMask | keyDownMask | autoKeyMask, 0);
}
