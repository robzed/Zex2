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
#define INTERFACE_LIB_BUILD 1		// interface lib build - using carbonaccessors.o
#define ZEX_ANVIL 1		  	  // 0=PB build, 1=Anvil build
