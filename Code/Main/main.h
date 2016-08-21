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

