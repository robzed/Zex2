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
 *  main.h
 *  Zex
 *
 *  Created by Rob Probin on 04/06/2005.
 *  Copyright 2005 Lightsoft. All rights reserved.
 *
 * $Author: rob $
 * $Date: 2005/12/10 10:51:28 $
 * $Revision: 1.3 $
 *
 * For log see end of file...
 *
 */


#ifndef _MAIN_H_
#define _MAIN_H_

void full_screen_video();
void windowed_video();
void toggle_video();

bool are_we_in_full_screen_mode();

#endif // _MAIN_H_






/* CVS LOG
*
* $Log: main.h,v $
* Revision 1.3  2005/12/10 10:51:28  rob
* Zex2005: Fixed full screen cursor flicker (hopefully) by implementing our own cursor.
*
* Revision 1.2  2005/10/10 21:27:29  rob
* Zex2005: Added newline at end to stop compiler whinging.
*
* Revision 1.1  2005/06/04 13:11:06  rob
* Zex2005: Basic full screen / windowed mode change.
*
*
*
*/

