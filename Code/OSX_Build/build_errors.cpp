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
 *  build_errors.c
 *  zex22
 *
 *  Created by stu on Mon Oct 22 2001.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
 */



#include	"3d_structures_def.h"	     
#include "txtsplat.h"
#include "Error_handler.h"

#include "olg_text.h"
#include "fpoly4.h"
#include "genpoly.h"
#include "fdistort.h"
#include "fgouraud.h"
#include "pictures.h"
#include "ogl.h"
#include "olg_text.h"
#include 	"options.h"
#include "file_io.h"
#include "console.h"

Ptr double_buffer;
int screen_width;
Handle hpic;
Ptr medium_font;
Ptr tiny_font;
Handle small_font_h;
Handle large_font_h;
Ptr large_font;

void InitText()
 {
// hpic = (Handle) ZGetResource('ZRAW',128);  //Get the Handle to the Resource 
 hpic = (Handle) ZGetResource('JESS',129);  //Get the Handle to the Resource 
 if (hpic==0) report_error("Resource missing: ZRAW 128","\p.",4);
 HLock(hpic);
 medium_font=*hpic;
 
 large_font_h = (Handle) ZGetResource('JESS',128);  //Get the Handle to the Large font Resource 
 if (large_font_h==0) report_error("Resource missing: JESS 128","\p.",4);
 HLock(large_font_h);
 large_font=*large_font_h; 

 small_font_h = (Handle) ZGetResource('JESS',130);  //Get the Handle to the Large font Resource 
 if (small_font_h==0) report_error("Resource missing: JESS 130","\p.",4);
 HLock(small_font_h);
 tiny_font=*small_font_h; 

 }



