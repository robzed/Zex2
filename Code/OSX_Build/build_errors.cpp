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



