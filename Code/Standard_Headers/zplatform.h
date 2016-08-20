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
$Log: zplatform.h,v $
Revision 1.2  2003/09/21 21:08:13  robp
Partial source check-in  - should have no build issues, just update to include new PORTABLE switches.

Revision 1.1.1.1  2003/09/05 22:37:05  stu_c
First Imported.

Revision 1.10  2002/09/08 02:21:14  stu_c
Precompiled header adjustments

Revision 1.9  2001/11/02 23:19:06  stu
Further OS X work. Enabled syscon logging.

Revision 1.8  2001/11/02 19:33:14  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.7  2001/10/27 22:39:34  stu
zplatform included

Revision 1.6  2001/10/27 18:59:40  rob
dual interface lib / carbon changes

Revision 1.5  2001/10/26 22:44:50  rob
Anvil changes

Revision 1.4  2001/10/25 21:10:41  rob
pb and anvil equates

Revision 1.3  2001/10/21 01:14:10  stu
Initial porting work

Revision 1.2  2001/10/20 23:32:15  rob
changed core selection label

Revision 1.1  2001/10/20 23:24:38  rob
Baseline cause projectbuilder is a pile of........

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


//
// depreciated - currently unused. The system headers may well contain these types of linkage...
//
// enable only one of these!!!!!
//#define MAC 1
//#define WIN 0
//#define LIN 0	//not used



// mac specific defines

#define CARBONIZE_ZEX 1

// sub-defines for mac specific defines (can also be used for InterfaceLib app)
#define CARBONIZE_COFFEE_INTERFACE CARBONIZE_ZEX
#define CARBONIZE_MUSIC CARBONIZE_ZEX

// these are carbonise only things.
#define ZEX_CORE_CARBON 1		// effects things like init and MaxApplZone

// ************************************************************************
// ************************************************************************
// ************************************************************************
// Initial Carbon builds are seperated by source sets on Anvil (MrC/MPW) and
// Project Builder.
#ifndef ZEX_PROJECT_BUILDER
#define ZEX_PROJECT_BUILDER (!ZEX_ANVIL)
#endif

// ************************************************************************
// ************************************************************************
// ************************************************************************

#if !INTERFACE_LIB_BUILD
  #define TARGET_API_MAC_CARBON 1		// for a pure carbon project
#else
  #define ACCESSOR_CALLS_ARE_FUNCTIONS 1		  // for an interface lib project
  #define OPAQUE_TOOLBOX_STRUCTS 1	  	  // for an interface lib project
#endif

//
// For some reason what KeyMap points to is changed ... (used in GetKey())
//
// 26-9-2003 Rob - For some reason it's been changed back <sigh>...
//#if ZEX_PROJECT_BUILDER || INTERFACE_LIB_BUILD
//  typedef unsigned long* keymap_ptr;
//#else
  typedef long* keymap_ptr;
//#endif

#undef __CARBON__

