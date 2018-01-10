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
// * FILENAME           : ppsound.h
// * MODULE TITLE       : Player Pro Included Sound
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 3rd January 2002
// * FIRST AUTHOR       : Rob
// *
// * COPYRIGHT (c)        LS 2002
// *
// ***********************************************************************************

#if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
  #include <Carbon/Carbon.h>
#else
  #include <MacTypes.h>
  #include <Files.h>
  #include <Sound.h>
  #include <Resources.h>
  #include <StringCompare.h>
  #include <ToolUtils.h>
  #include <Gestalt.h>
  #include <Script.h>
  #include <mixedmode.h>
  #include <Aliases.h>
  #include <Files.h>
  #include <string.h>
  #include <CodeFragments.h>
  #include <SoundInput.h>
  #include <SoundComponents.h>
  #include <Notification.h>
  #include <OpenTransport.h>
  #include <OSUtils.h>
  #include <Types.h>
#endif

#if ZEX_ANVIL
  #define inline
#else
  #define macintosh	  1
#endif


// non-carbon data structures... included since zplatform.h selects for TARGET_API_MAC_CARBON
struct SndDoubleBuffer {
  long                dbNumFrames;
  long                dbFlags;
  long                dbUserInfo[2];
  SInt8               dbSoundData[1];
};
typedef struct SndDoubleBuffer          SndDoubleBuffer;
typedef SndDoubleBuffer *               SndDoubleBufferPtr;

typedef CALLBACK_API( void , SndDoubleBackProcPtr )(SndChannelPtr channel, SndDoubleBufferPtr doubleBufferPtr);
typedef STACK_UPP_TYPE(SndDoubleBackProcPtr)                    SndDoubleBackUPP;

struct SndDoubleBufferHeader {
  short               dbhNumChannels;
  short               dbhSampleSize;
  short               dbhCompressionID;
  short               dbhPacketSize;
  UnsignedFixed       dbhSampleRate;
  SndDoubleBufferPtr  dbhBufferPtr[2];
  SndDoubleBackUPP    dbhDoubleBack;
};

typedef struct SndDoubleBufferHeader    SndDoubleBufferHeader;
typedef SndDoubleBufferHeader *         SndDoubleBufferHeaderPtr;

struct SndDoubleBufferHeader2 {
  short               dbhNumChannels;
  short               dbhSampleSize;
  short               dbhCompressionID;
  short               dbhPacketSize;
  UnsignedFixed       dbhSampleRate;
  SndDoubleBufferPtr  dbhBufferPtr[2];
  SndDoubleBackUPP    dbhDoubleBack;
  OSType              dbhFormat;
};
typedef struct SndDoubleBufferHeader2   SndDoubleBufferHeader2;
typedef SndDoubleBufferHeader2 *        SndDoubleBufferHeader2Ptr;

enum {
  dbBufferReady                 = 0x00000001, /*double buffer is filled*/
  dbLastBuffer                  = 0x00000004 /*last double buffer to play*/
};



