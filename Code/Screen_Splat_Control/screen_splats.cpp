// *************************************MODULE HEADER*********************************
// * FILENAME           : screen_splats.c
// * MODULE TITLE       : screen_splats.c
// * PROJECT            : Zex
// * DATE STARTED       : 06:36:10 on 14th June 1999
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************
/* $Log: screen_splats.c,v $
/* Revision 1.5  2002/09/08 02:06:56  stu_c
/* Precompiled header adjustments
/*
/* Revision 1.4  2001/12/29 01:11:02  stu
/* Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
/*
/* Revision 1.3  2001/11/02 19:31:37  rob
/* Build variants for interface lib and carbon.
Coffee fixes after changes for X.
/*
/* Revision 1.2  2001/10/22 21:28:30  rob
/* Carbon warning changes
/*
/* Revision 1.1  2001/10/21 01:06:20  stu
/* Initial porting work
/*
/* Revision 1.0.0.1  2001/10/17 20:46:07  rob
/* First Imported.
/*
/* Revision 1.1.1.1  2001/01/01 21:13:38  rob
/* First Imported.
/*
/* Revision 1.0.0.1  2000/08/21 22:09:01  stu
/* First Imported.
/*
 */


// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include	"LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <Palettes.h>
    #include <QDOffscreen.h>
    #include <mactypes.h>
    #include <fixmath.h>
  #endif
  #include	"Error_handler.h"
  #include	"zex_misc.h"

#endif //pch

#include	"splat_control.h"
#include "pictures.h"
#include "screen_splats.h"


// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *



int gupdate_cp;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *
//¥¥¥¥¥¥¥Private bits
void	pixel_doubling_blit_8(
					const void * source_bitmap,
					int          source_rowbytes,
					int          source_width,
					int          source_height,
					void *       dest_bitmap,
					int          dest_rowbytes);




// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE              : 
// | AUTHOR(s)          :
// | DATE STARTED       : 
// +----------------------------------------------------------------------------------
// | DESCRIPTION:  
// | 
// | 
// |
// |
// +----------------------------------------------------------------------------------
// | PARAM IN   : none
// |
// | PARAM OUT  : none
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial                Descrip.
// |
// |
// +----------------------------------------------------------------------------------
// | ADDITIONAL NOTES:
// | 









//splats.c






//SB 170599
//This is about 8% faster than drawsprockets best effort :)
//
void splat_low_level(char * src, int src_width, char * dest, int dest_width)
{
//CGrafPtr	frontBuff;
//extern	      DSpContextReference		              mDisplayContext;
//extern Ptr screenaddr;
//extern int screenwidth;
//char *src_now,*dest_now;
Float64 *src_float,*dest_float;
int line_count,line,pix;
int pix_w;

//where are we splatting to
//    DSpContext_GetFrontBuffer(mDisplayContext, &frontBuff);
//    dest=((*(frontBuff)->portPixMap))->baseAddr;
//    dest_width=0x3fff & ((*(frontBuff)->portPixMap))->rowBytes;

//from
//src=screenaddr;

//dest_now=dest;	
//src_now=src;
src_float=(Float64 *) src;
dest_float=(Float64 *) dest;

//how many lines
line_count=monitor_h;
pix_w=monitor_w/8;
//while(vbl_now==0);	//wait for vbl

//the below code has been tweaked so optimiser outputs
//mtctr 640/8
//lfdu
//stfdu
//bdnz 
//cmp 480
//add
//add
//ble
//for the inner loop
//so don't touch.
//on my 8500 this is twice as fast as copybits
for (line=0;line<line_count;line++)
    {
     for (pix=0;pix<pix_w;pix++)
         *dest_float++=*src_float++;

     src = src+src_width;	//point to next src line;
     dest=dest+dest_width;

     src_float=(Float64 *) src;
     dest_float=(Float64 *) dest;

    }	//do next line

}



//SB 170599
//This is about 8% faster than drawsprockets best effort :)
//hardwired for 640 * 480
void splat_control_panel(Ptr dest_vram, int dest_width)
{
//CGrafPtr	  frontBuff;
//extern	DSpContextReference		                mDisplayContext;
//extern Ptr screenaddr;
//extern int screenwidth;
//char *src,*dest,*src_now,*dest_now;
//Float64 *src_float,*dest_float;
//int line_count,line,pix;
//
//
////where are we splatting to
////    DSpContext_GetFrontBuffer(mDisplayContext, &frontBuff);
////    dest=((*(frontBuff)->portPixMap))->baseAddr;
////    dest_width=0x3fff & ((*(frontBuff)->portPixMap))->rowBytes;
//dest=dest_vram;
//
////from
//src=screenaddr;
//src=src+(screenwidth*CONTROL_PANEL_TOP);
//dest=dest+(dest_width*CONTROL_PANEL_TOP);
////dest_now=dest;	  
////src_now=src;
//src_float=(Float64 *) src;
//dest_float=(Float64 *) dest;
//
////how many lines
//line_count=480-CONTROL_PANEL_TOP;
////the below code has been tweaked so optimiser outputs
////mtctr 640/8
////lfdu
////stfdu
////bdnz 
////cmp 480
////add
////add
////ble
////for the inner loop
////so don't touch.
////on my 8500 this is twice as fast as copybits
//for (line=0;line<line_count;line++)
//    {
//     for (pix=0;pix<640/8;pix++)
//         *dest_float++=*src_float++;
//
//     src = src+screenwidth;	  //point to next src line;
//     dest=dest+dest_width;
//
//     src_float=(Float64 *) src;
//     dest_float=(Float64 *) dest;
//
//    }	//do next line
//
}



