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

/*
 *  system_interface.cpp
 *  Z_dungeon / Zex
 *
 *  Created by Rob Probin on Thu Nov 07 2002.
 *  Copyright (c) 2002 Lightsoft. All rights reserved.
 *
 * $Author: robp $
 * $Date: 2003/09/22 22:01:33 $
 * $Revision: 1.1 $
 *
 * For log see end of file...
 *
 */

//#include "SDL.h"
#include "string.h"

//#include "general_include.h"

#include "system_interface.h"


// +--------------------------------+-------------------------+-----------------------
// | TITLE: make_filename           | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 03
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

BOOL make_filename(char output[FILENAME_LENGTH], const char *filename, const char *pathname, const char *typename_for_error)
{
int filename_len;

filename_len = strlen(filename);
if(filename_len==0)
  {
  fprintf(OUTPUT_FP,"No %s Filename!\n",typename_for_error);
  return FALSE;
  }

if((filename_len+strlen(pathname)+1) >= FILENAME_LENGTH)
  {
  fprintf(OUTPUT_FP,"%s filename plus path too long for %s\n",typename_for_error,filename);
  return FALSE;
  }
  
strcpy(output,pathname);		// path first
strcat(output,filename);			// followed by filename

return TRUE;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: copy_c_string_filename  | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 03
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void copy_c_string_filename(char *target, const char *source)
{
int length;

  length=0;
  while(*source && length < (FILENAME_LENGTH-1))
    {
    *target=*source;
    target++; source++;
    length++;
    }
    
  *target='\0';
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: c_strings_identical     | AUTHOR(s): Rob Probin   | DATE STARTED: 16 Mar 03
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

BOOL c_strings_identical(const char *source1, const char *source2)
{
if(*source1 != *source2) return FALSE;

while(*source1)
  {
  source1++; source2++;
  if(*source1 != *source2) return FALSE;
  }
  
return TRUE;
}

/* CVS LOG
 *
 * $Log: system_interface.cpp,v $
 * Revision 1.1  2003/09/22 22:01:33  robp
 * Work in progress: portable file system, stage 1 - load resources via C standard library calls. THIS INCLUDES A PROJECT FILE UPDATE.
 *
 * Revision 1.9  2003/09/20 18:02:11  robp
 * *** NAME CHANGE OF ALL C FILES FROM .c TO .cpp *** 
Before this point you will need to rename the files back to .c (i.e. remove the pp from .cpp) to build older versions.
 *
 * Revision 1.8  2003/07/06 20:14:56  robp
 * Got rid of unsigned/signed int/char compare warnings for gcc3.3, and added cvs header and footers to all files that didn't have them.
 *
 *
 *
 *
 */
