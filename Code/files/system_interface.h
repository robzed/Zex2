/*
 *  system_interface.h
 *  Z_dungeon
 *
 *  Created by Rob Probin on Thu Nov 07 2002.
 *  Copyright (c) 2002 Lightsoft. All rights reserved.
 *
 * $Author: robp $
 * $Date: 2003/07/06 20:14:55 $
 * $Revision: 1.7 $
 *
 * For log see end of file...
 *
 */

BOOL make_filename(char output[FILENAME_LENGTH], const char *filename, const char *pathname, const char *typename_for_error);

void copy_c_string_filename(char *target, const char *source);
BOOL c_strings_identical(const char *source1, const char *source2);

/* CVS LOG
 *
 * $Log: system_interface.h,v $
 * Revision 1.7  2003/07/06 20:14:55  robp
 * Got rid of unsigned/signed int/char compare warnings for gcc3.3, and added cvs header and footers to all files that didn't have them.
 *
 *
 *
 *
 */
 
 
