/*
 *  system_independant_file.h
 *  Z_dungeon
 *
 *  Created by Rob Probin on Sat Dec 21 2002.
 *  Copyright (c) 2002 Lightsoft. All rights reserved.
 *
 * $Author: robp $
 * $Date: 2003/09/21 13:13:10 $
 * $Revision: 1.1 $
 *
 * For log see end of file...
 *
 */

//#include "general_include.h"		// Z_dungeon include not available...
#include "stdio.h"

// ***************************************************************************
// * Definitions
// ***************************************************************************

typedef int ls_file_t;		// this is the type lsf_open_file() returns - and represents a file
typedef BOOL file_error;	// this represents TRUE=error FALSE=no_error (e.g. error can be end of file)

// ***************************************************************************
// * Public Function prototypes
// ***************************************************************************

ls_file_t lsf_open_file(const char *filename, const char *mode);		// return 0 for failure
void lsf_close_file(ls_file_t the_file);
char *lsf_getline(ls_file_t the_file, ls_file_t *actual_read);
int lsf_write_string(ls_file_t the_file, const char *string_to_write);		// returns EOF for error or non-negative otherwise
FILE *lsf_get_system_file_pointer(ls_file_t the_file); // Returns the system file pointer. This allows you to use fprintf with 
                                                       // the Lightsoft file routines. Don't use on read (messes up internal indexes).

// Added for Zex. Notice: With neither of these routines do you have access to the error/eof info.
int lsf_read_bytes_from_file(ls_file_t the_file, char *destination, int maximum_length); // returns length actually read
int lsf_write_bytes_to_file(ls_file_t the_file, char *source, int length);		 // return length actually written
long lsf_get_file_size(ls_file_t the_file);

//
// *** the following are NOT TESTED ***
//
file_error lsf_read_line(ls_file_t the_file);
file_error lsf_get_word(ls_file_t the_file, int max, char *destination);
file_error lsf_skip_whitespace(ls_file_t the_file);
file_error lsf_get_until_character(ls_file_t the_file, char the_character, int max, char *destination);

/* CVS LOG
 *
 * $Log: system_independant_file.h,v $
 * Revision 1.1  2003/09/21 13:13:10  robp
 * First phase of system independant file system - add Z_dungeons file system and expand.
 *
 * Revision 1.5  2003/08/23 15:04:05  robp
 * Added start of save code
 *
 * Revision 1.4  2003/07/06 10:17:56  robp
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

