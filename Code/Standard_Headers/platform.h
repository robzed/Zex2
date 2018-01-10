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

// *************************************MODULE HEADER*********************************
// * FILENAME           : Platform.h
// * MODULE TITLE       : Platform defs
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 00:04:36 on 21st August 2000
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *      SB              Tells Zex which platform it is building for
// *
// *
// ***********************************************************************************

/* CVS bits
$Log: platform.h,v $
Revision 1.3  2001/10/25 21:10:42  rob
pb and anvil equates

Revision 1.2  2001/10/25 21:04:52  rob
Added project environment defines.

Revision 1.1  2001/10/20 15:19:34  rob
Added CORE_CARBON

Revision 1.0.0.1  2001/10/17 20:46:08  rob
First Imported.

Revision 1.2  2001/05/20 17:15:14  rob
Carbon specific includes

Revision 1.1.1.1  2001/01/01 21:13:39  rob
First Imported.

Revision 1.1  2000/10/08 21:47:18  stu
Engine conversion to 64 bit. Huge pain of a job.

Revision 1.0.0.1  2000/08/21 22:10:20  stu
First Imported.

Revision 1.1  2000/08/20 23:24:43  stu
Initial issue.

            */

//enable only one of these!!!!!
#define MAC 1
#define WIN 0
#define LIN 0	//not used



// mac specific defines

#define CARBONIZE_ZEX 1


// sub-defines for mac specific defines
#define CARBONIZE_COFFEE_INTERFACE CARBONIZE_ZEX
#define CARBONIZE_MUSIC CARBONIZE_ZEX

// these are carbonise only
#define CORE_CARBON 1		// effects things like init and MaxApplZone

// ************************************************************************
// ************************************************************************
// ************************************************************************
// Initial Carbon builds are seperated by source sets on Anvil (MrC/MPW) and
// Project Builder.

#define ZEX_ANVIL 1		  	  // 0=PB build, 1=Anvil build
#define ZEX_PROJECT_BUILDER (!ZEX_ANVIL)

// ************************************************************************
// ************************************************************************
// ************************************************************************
