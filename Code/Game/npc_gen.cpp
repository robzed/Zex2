/*
 *  npc_gen.cpp
 *  zex23xcode
 *
 *  Created by Stuart Ball on Sat Sep 13 2003.
 *  Copyright (c) 2003 Lightsoft. All rights reserved.
 *
 */

#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <fp.h>
  #endif

  #include "3d_structures_def.h"
  #include	"zex_misc.h"
  #include "processor.h"
  #include "access_ocbs.h"
  #include "error_handler.h"
  #include "real_time.h"
#endif


#include "npc_gen.h"

//•••••••••••••••••••••••••••••••••••••••••••••••••••••
//run_npc_gen looks at the current situation and decides if
//we should generate an NPC and if so what it should be

void run_npc_gen(void)
{
 //if hyper transport is active
 
}