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
// * FILENAME           : zselector.h
// * MODULE TITLE       : Environment selector
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 00:19:36 on 30th October 2001
// * FIRST AUTHOR       : Rob Probin
// *
// * COPYRIGHT (c)        2001 Lightsoft
// *
// ***********************************************************************************

//
// These two include tell us which environment we are building for
//
#define INTERFACE_LIB_BUILD 0		// interface lib build - using carbonaccessors.o
#define ZEX_ANVIL 1		  	  // 0=PB build, 1=Anvil build
