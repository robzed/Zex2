/*
 *  system_independant_file.cpp
 *  Z_dungeon / Zex
 *
 *  Created by Rob Probin on Sat Dec 21 2002.
 *  Copyright (c) 2002 Lightsoft. All rights reserved.
 *
 *        ---------------------x-----------------------
 *
 * This is a module specifically for loading text files for parsing as
 * command input files.
 *
 * It also handles different types of newlines well:
 * Either a CR or a LF will terminate a line, and if a CR is found, 
 * an LF is looked for and removed if found. Notice a LF CR pair will
 * generate two lines - but this is OK mostly - and hopefully 
 * exceptionally rare.
 *
 * $Author: robp $
 * $Date: 2003/09/22 22:01:32 $
 * $Revision: 1.2 $
 *
 * For log see end of file...
 *
 */

#include "system_independant_file.h"


// ***********************************************************************************
// * CONSTANTS 
// *

#define MAX_LINE 512	 	 // maximum size of a line
#define MAX_OPEN_FILES 10	// could use FOPEN_MAX files - but probably not needed
#define LSF_ls_file_OFFSET 13335	// shouldn't be zero (currently just "magic number" to spot errors)

// ***********************************************************************************
// * MACROS 
// *

// ***********************************************************************************
// * TYPE DEFS
// *

// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

typedef struct {

    FILE *file_ptr;
    char line_array[MAX_LINE];
    short int line_position;
    } lsf_structure_t;
    
lsf_structure_t lsf_file_array[MAX_OPEN_FILES];

int lsf_file_counter=0;
// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

static int lsf_getstring(char *string, int max, FILE *file);


// ***********************************************************************************
// * ACTUAL CODE
// *



// +--------------------------------+-------------------------+-----------------------
// | TITLE: lsf_open_file           | AUTHOR(s): Rob Probin   | DATE STARTED: 21 Dec 02
// +
// | DESCRIPTION: Opens a file and record basic information about it.
// +----------------------------------------------------------------ROUTINE HEADER----

ls_file_t lsf_open_file(const char *filename, const char *mode)
{
FILE *input_f;
int opened_file_ref;
short int find_slot;

if(lsf_file_counter>=MAX_OPEN_FILES)
  {
  opened_file_ref = -1;
  for(find_slot=0; find_slot<lsf_file_counter; find_slot++)
    {
    if(lsf_file_array[find_slot].file_ptr == NULL)
      {
      opened_file_ref=find_slot;
      break;
      }
    }

  if(opened_file_ref == -1)
    {
    fprintf(OUTPUT_FP, "lsf> No more file space!\n");
    return 0;
    }
  }
else	// we are not at the max yet
  {
  opened_file_ref = lsf_file_counter;	// make a record of the current file ref
  }
  
input_f = fopen(filename,mode);
if(input_f==NULL) 
  {
  #if DEBUG_GENERAL_STATUS
    fprintf(OUTPUT_FP, "lsf> Couldn't open file! (%s)\n",filename);
  #endif
  return 0;
  }

lsf_file_array[opened_file_ref].file_ptr = input_f;	// record file pointer

lsf_file_array[opened_file_ref].line_position=0;  // set position back to start
lsf_file_array[opened_file_ref].line_array[0]='\0';	// zero terminate initially (for error cases)


lsf_file_counter++;		// increment counter to next one
return opened_file_ref+LSF_ls_file_OFFSET;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: lsf_close_file          | AUTHOR(s): Rob Probin   | DATE STARTED: 21 Dec 02
// +
// | DESCRIPTION: Closes Previously opened file
// +----------------------------------------------------------------ROUTINE HEADER----

void lsf_close_file(ls_file_t the_file)
{
the_file-=LSF_ls_file_OFFSET;	// change back to local offset
if(the_file<0 || the_file>=MAX_OPEN_FILES) { fprintf(OUTPUT_FP, "lsf> File ref error!\n"); return; }

fclose(lsf_file_array[the_file].file_ptr);	// close the file
lsf_file_array[the_file].file_ptr = NULL;	// mark slot as empty
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: lsf_read_line           | AUTHOR(s): Rob Probin   | DATE STARTED: 21 Dec 02
// +
// | DESCRIPTION:  Reads a line of text info the files line buffer
// +----------------------------------------------------------------ROUTINE HEADER----
// *** NOT TESTED ***

file_error lsf_read_line(ls_file_t the_file)
{
the_file-=LSF_ls_file_OFFSET;

lsf_file_array[the_file].line_position=0;  // set position back to start
lsf_file_array[the_file].line_array[0]='\0';	// zero terminate initially (for error cases)

if(the_file<0 || the_file>=MAX_OPEN_FILES) // look for an error
    { 
    fprintf(OUTPUT_FP, "lsf> File ref error!\n"); 
    return FALSE; 
    }

if(lsf_getstring(lsf_file_array[the_file].line_array,MAX_LINE,lsf_file_array[the_file].file_ptr)==-1)
  {
  return FALSE;
  }

return TRUE;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: lsf_getline             | AUTHOR(s): Rob Probin   | DATE STARTED: 09 Mar 03
// +
// | DESCRIPTION: Returns a line of text to the caller. 
// | If the string is NULL, actual chars = 0 for file error, and actual chars = -1 for EOF
// +----------------------------------------------------------------ROUTINE HEADER----

char *lsf_getline(ls_file_t the_file, int *actual_chars_read)
{
the_file-=LSF_ls_file_OFFSET;

lsf_file_array[the_file].line_position=0;  // set position back to start
lsf_file_array[the_file].line_array[0]='\0';	// zero terminate initially (for error cases)

if(the_file<0 || the_file>=MAX_OPEN_FILES) // look for an error
    { 
    *actual_chars_read=0;
    fprintf(OUTPUT_FP, "lsf> File ref error!\n"); 
    return NULL; 
    }

*actual_chars_read = lsf_getstring(lsf_file_array[the_file].line_array,MAX_LINE,lsf_file_array[the_file].file_ptr);
if(*actual_chars_read==-1)
  {
  return NULL;
  }

return lsf_file_array[the_file].line_array;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: lsf_write_string        | AUTHOR(s): Rob Probin   | DATE STARTED: 07 June 03
// +
// | DESCRIPTION: Writes a string to a file
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

int lsf_write_string(ls_file_t the_file, const char *string_to_write)
{
FILE *file;
int error;

the_file-=LSF_ls_file_OFFSET;

lsf_file_array[the_file].line_position=0;  // set position back to start
lsf_file_array[the_file].line_array[0]='\0';	// zero terminate initially (for error cases)

if(the_file<0 || the_file>=MAX_OPEN_FILES) // look for an error
    { 
    fprintf(OUTPUT_FP, "lsf> File ref error in lsf_write_string!\n"); 
    return EOF; 
    }

file = lsf_file_array[the_file].file_ptr;
error = fputs(string_to_write, file);

return error;
}



// +--------------------------------+-------------------------+-----------------------
// | TITLE: lsf_get_system_file_pointer        | AUTHOR(s): Rob Probin   | DATE STARTED: 07 June 03
// +
// | DESCRIPTION: Returns the system file pointer. This allows you to use fprintf with
// |  the Lightsoft file routines.
// +----------------------------------------------------------------ROUTINE HEADER----
FILE *lsf_get_system_file_pointer(ls_file_t the_file)
{
FILE *file;

the_file-=LSF_ls_file_OFFSET;

lsf_file_array[the_file].line_position=0;  // set position back to start
lsf_file_array[the_file].line_array[0]='\0';	// zero terminate initially (for error cases)

if(the_file<0 || the_file>=MAX_OPEN_FILES) // look for an error
    { 
    fprintf(OUTPUT_FP, "lsf> File ref error in lsf_get_system_file_pointer!\n"); 
    return 0; 
    }

file = lsf_file_array[the_file].file_ptr;

return file;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: lsf_gets                | AUTHOR(s): Rob Probin   | DATE STARTED: 21 Dec 02
// +
// | DESCRIPTION: Similar to standard libraries fgets, except handles different types
// | of newline. 
// |
// | lsf_getstring reads at most the next n-1 characters into the array s, stopping 
// | if a newline is encountered; the newline is NOT included in the array, which is 
// | terminated by '\0'. lsf_getstring returns the number of characters read, or -1 for EOF
// | at the beginning of a new line
// |
// +----------------------------------------------------------------ROUTINE HEADER----

static int lsf_getstring(char *string, int max, FILE *file)
{
  int c;
  int count=0;		// count of characters
  
  c=getc(file);		// get first character
  if(c==EOF) return -1;	// End of file at start of a line
  
  max--;		// n-1 characters read into array (not including zero terminator)
  while(count < max)
    {
    if(c == 13 || c == 10) 
        {
        if(c == 13)	// CR followed by LF?
          {
          c=getc(file);
          if(c != 10) { ungetc(c,file); }
          }
        break;
        }

    *string = c;	// write the character into the string
    string++;		// next location
    count++;    	// one more chatacters read and accepted

    c=getc(file);	// get next character
    if(c==EOF) break;	// End of file at end of a line
    }
    
  *string='\0';		// zero terminate output string

  return count;		// return number of characters read in
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: lsf_read_file           | AUTHOR(s): Rob Probin   | DATE STARTED: 20 Sept 03
// +
// | DESCRIPTION:  Read a block of data from a file. New for Zex2.3
// +----------------------------------------------------------------ROUTINE HEADER----
// Notice: With neither of these routines do you have access to the error/eof info.

int lsf_read_bytes_from_file(ls_file_t the_file, char *destination, int maximum_length)
{
FILE *file;
int bytes_read;

the_file-=LSF_ls_file_OFFSET;

lsf_file_array[the_file].line_position=0;  // set position back to start
lsf_file_array[the_file].line_array[0]='\0';	// zero terminate initially (for error cases)

if(the_file<0 || the_file>=MAX_OPEN_FILES) // look for an error
    { 
    fprintf(OUTPUT_FP, "lsf> File ref error in lsf_write_string!\n"); 
    return 0; 
    }

file = lsf_file_array[the_file].file_ptr;
bytes_read = fread(destination, 1, maximum_length, file);

return bytes_read;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: lsf_write_file          | AUTHOR(s): Rob Probin   | DATE STARTED: 20 Sept 03
// +
// | DESCRIPTION:  Write a block of data to a file. New for Zex2.3.
// +----------------------------------------------------------------ROUTINE HEADER----
// Notice: With neither of these routines do you have access to the error/eof info.

int lsf_write_bytes_to_file(ls_file_t the_file, char *source, int length)
{
FILE *file;
int bytes_written;

the_file-=LSF_ls_file_OFFSET;

lsf_file_array[the_file].line_position=0;  // set position back to start
lsf_file_array[the_file].line_array[0]='\0';	// zero terminate initially (for error cases)

if(the_file<0 || the_file>=MAX_OPEN_FILES) // look for an error
    { 
    fprintf(OUTPUT_FP, "lsf> File ref error in lsf_write_string!\n"); 
    return 0; 
    }

file = lsf_file_array[the_file].file_ptr;
bytes_written = fwrite(source, 1, length, file);

return bytes_written;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: lsf_get_file_size       | AUTHOR(s): Rob Probin   | DATE STARTED: 22 Sept 03
// +
// | DESCRIPTION: This finds the length of a file. Returns -1 for error.
// | 
// | This may not be overly portable in C since:-
// |	(a) does ftell return the length after a SEEK_END?
// | 	(b) not all systems support SEEK_END.
// |
// | An alternative method is to read successive bytes into a small array, adding up
// | how many have been read. If this method is going to be used, and performance
// | of reading the twice is an issue, then it would be better to allocate a 64K buffer
// | read up to 64K, then extend (if too small) and read the rest or shrink the buffer if
// | too much has been allocated.
// |
// | Also see notes after routine.
//
// +----------------------------------------------------------------ROUTINE HEADER----

long lsf_get_file_size(ls_file_t the_file)
{
FILE *file;
long current_position;
long end_position;

the_file-=LSF_ls_file_OFFSET;

if(the_file<0 || the_file>=MAX_OPEN_FILES) // look for an error
    { 
    fprintf(OUTPUT_FP, "lsf> File ref error in lsf_get_file_size!\n"); 
    return -1; 
    }
    
file = lsf_file_array[the_file].file_ptr;


current_position = ftell(file);		// get the current file position
if(current_position == -1)
    { 
    fprintf(OUTPUT_FP, "lsf> ftell_1 error in lsf_get_file_size!\n"); 
    return -1; 
    }

if(fseek(file, 0, SEEK_END))		// set to the end of the file
    { 
    fprintf(OUTPUT_FP, "lsf>seek end error in lsf_get_file_size!\n"); 
    return -1; 
    }

end_position = ftell(file);		// this might not be a valid method for determining the length in C??
if(end_position == -1)
    { 
    fprintf(OUTPUT_FP, "lsf> ftell_2 error in lsf_get_file_size!\n"); 
    return -1; 
    }

if(fseek(file, current_position, SEEK_SET))	// set back to where we were.
    { 
    fprintf(OUTPUT_FP, "lsf> ftell_1 error in lsf_get_file_size!\n"); 
    return -1; 
    }

return end_position;
}

// from http://open-systems.ufl.edu/mirrors/ftp.isc.org/pub/usenet/comp.sources.unix/volume26/funnelweb/part08
//
// comment from the FunnelWeb Source 
//
/* Finding The Length of a File                                               */
/* ----------------------------                                               */
/* We have to be able to find out the length of a file before reading it in   */
/* because, in this version of FunnelWeb, the entire file must be read into   */
/* one contiguous block of memory.                                            */
/*                                                                            */
/* As it turns out, finding the length of a file in portable C turns out to   */
/* be a very hard problem. Here are some possible solutions:                  */
/*                                                                            */
/*    1. Read the entire file in and see how long it is.                      */
/*    2. Use fseek to move to the end of the file and then use ftell.         */
/*    3. Use the Unix 'stat' call.                                            */
/*                                                                            */
/* Of these, only the first is portable. The second alternative is            */
/* non-portable because some environments do not support the SEEK_END symbol  */
/* required to perform a seek to the end of file.                             */
/* Alternatives to needing the length are as follows:                         */
/*                                                                            */
/*    4. Read the file/allocate memory in 64K blocks.                         */
/*    5. Read the file in 64K blocks and then copy them to a contiguous one.  */
/*                                                                            */
/* Perhaps options 4 or 5 could be implemented later. However, right now I    */
/* haven't got the time to do anything except strive for portability, so      */
/* option 1 it is.                                                            */


// +--------------------------------+-------------------------+-----------------------
// | TITLE: lsf_get_word            | AUTHOR(s): Rob Probin   | DATE STARTED: 21 Dec 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----
// *** NOT TESTED ***

file_error lsf_get_word(ls_file_t the_file, int max, char *destination)
{
int c;
int pos=lsf_file_array[the_file].line_position;
char *str=&lsf_file_array[the_file].line_array[pos];

if(lsf_skip_whitespace(the_file)) return TRUE;

while(--max > 0)
  {
  c=*str;
  if(c==' ' || c=='\t') break;
  if(c=='\0') break;
  *destination = c;
  destination++;
  str++;
  pos++;
  }

*destination='\0';

lsf_file_array[the_file].line_position = pos;
  
return FALSE;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: lsf_skip_whitespace     | AUTHOR(s): Rob Probin   | DATE STARTED: 21 Dec 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----
// *** NOT TESTED ***

file_error lsf_skip_whitespace(ls_file_t the_file)
{
int pos=lsf_file_array[the_file].line_position;
char *str=&lsf_file_array[the_file].line_array[pos];


while(1)
  {
  while(*str=='\0') // carry on skipping forward until we have a non-empty line
    {
    if(lsf_read_line(the_file)) return TRUE;
    pos = lsf_file_array[the_file].line_position;
    str = &lsf_file_array[the_file].line_array[pos];
    }
    
  while(*str==' ' || *str=='\t')
    {
    str++; pos++;
    }
    
  if(*str!=' ' && *str!='\t') return FALSE;
  }

lsf_file_array[the_file].line_position = pos;
return FALSE;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: lsf_get_until_character | AUTHOR(s): Rob Probin   | DATE STARTED: 21 Dec 02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----
// *** NOT TESTED ***

file_error lsf_get_until_character(ls_file_t the_file, char the_character, int max, char *destination)
{
int c;
int pos=lsf_file_array[the_file].line_position;
char *str=&lsf_file_array[the_file].line_array[pos];

while(*str=='\0') // carry on skipping forward until we have a non-empty line
  {
  if(lsf_read_line(the_file)) return TRUE;
  pos = lsf_file_array[the_file].line_position;
  str = &lsf_file_array[the_file].line_array[pos];
  }

while(--max > 0 && (c=*str)!=the_character)
  {
  if(c=='\0') break;
  *destination = c;
  destination++;
  }

*destination='\0';
  
return FALSE;
}


/* CVS LOG
 *
 * $Log: system_independant_file.cpp,v $
 * Revision 1.2  2003/09/22 22:01:32  robp
 * Work in progress: portable file system, stage 1 - load resources via C standard library calls. THIS INCLUDES A PROJECT FILE UPDATE.
 *
 * Revision 1.1  2003/09/21 13:13:10  robp
 * First phase of system independant file system - add Z_dungeons file system and expand.
 *
 * Revision 1.9  2003/09/20 18:02:10  robp
 * *** NAME CHANGE OF ALL C FILES FROM .c TO .cpp *** 
Before this point you will need to rename the files back to .c (i.e. remove the pp from .cpp) to build older versions.
 *
 * Revision 1.8  2003/08/23 15:03:58  robp
 * Added start of save code
 *
 * Revision 1.7  2003/07/06 10:17:52  robp
 * 1. Added user definable keys
2. Added debug panes
3. Added loadable text strings
4. Added support for more fonts
5. Added more debug for graphic loading
6. Fixed problem with star initialisation
7. Fix bug with key action in menu latching icon
8. Fixed bug in unload of player interface & fonts
 *
 *
 *
 */

