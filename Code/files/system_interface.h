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
 
 
