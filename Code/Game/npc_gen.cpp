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