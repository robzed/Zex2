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



