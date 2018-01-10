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
// * FILENAME           : main.c
// * MODULE TITLE       : main
// * TABS: 				: Set to 4 please.
// * PROJECT            : LSResToFile
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 23:22:05 on 21st May 2000
// * FIRST AUTHOR       : Stuart Ball
// * ENVIRONMENT:		: Anvil 3
// *
// * COPYRIGHT (c)        Lightsoft Software 2000.
// * DISTRIBUTION: 		  This software may be distributed freely as long as the
// *                      source and other project files remain intact. If you modify the source and
// *                      redistribute then you may only do so as long as all source (original and modified)
// *                      accompanies the distribution and Lightsoft is given a copy of the project to maintain
// *                      source integrity. Lightsofdt can be contacted at: http://www.lightsoft.co.uk
// *
// ***********************************************************************************
// * HISTORY:
// * Date   Initial       Descrip.
// * 210500 SBB           Program to extract all resources from a file to individual
// *                      data files. Typically useful when porting a Mac App to other platforms
// *                      Useage: Double click the App. A file selector allows you to pick a
// *                      file to extract the resources from. Select the file and wait for it to end.
// *                      The extracted resources typically appear in the same folder as LSResToFile.
// *                      NOTE: No UI! This is a quick hack I wrote in 1 hour and 10 minutes. There
// *                      is error checking but no feedback to user. Just silent fails.
// *                      Can be easily modified to do whatever you want. i.e. all resources in one file 
// *                      preceded with header for example. If you do improve it please let us have a copy 
// *                      (with source) for redistribution.
// *					  I wrote it because some guy wanted 5 bucks for a shareware thing with less(!) 
// *                      functionality.
// *
// *
// ***********************************************************************************


// ***********************************************************************************
// * INCLUDED FILES
// *

#include <processes.h>	//my standard bunch of includes
#include <DiskInit.h>
#include <Dialogs.h>
#include <Files.h>
#include <Fonts.h>
#include <Lists.h>
#include <Menus.h>
#include <Resources.h>
#include <Memory.h>
#include <OSUtils.h>
#include <Events.h>
#include <Packages.h>
#include <Scrap.h>
#include <Script.h>
#include <SegLoad.h>
#include <ToolUtils.h>
#include <NumberFormatting.h>
#include <StandardFile.h>


// ***********************************************************************************
// * CONSTANTS 
// *



/* Standard Useful constants */
#ifndef NIL
#define NIL ((void *)0)
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

QDGlobals qd;


// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

void InitToolbox(void);
void main(void);
void ExtractResources(FSSpec *TheFSSpec);
void resource_to_file(Handle the_resource);
void write_file(Handle the_resource, short rID, ResType rType);
void ZNumToString(unsigned short number, unsigned char *string);

//Init the Mac
void InitToolbox (void)
{
	InitGraf(&qd.thePort);		// Initialize QuickDraw variables for our program.
	InitFonts();				// Initialize fonts.
	FlushEvents(everyEvent, 0);	// Clear event queue of any pending events.
	InitWindows();				// Initialize the Window Manager.
	InitMenus();				// Ditto for the Menu Manager.
	TEInit();					// blah, blah Text Edit.
	InitDialogs(0L);			// blah, blah Dialog Manager.
	InitCursor();				// Set the cursor to the arrow cursor and init.
	
	MaxApplZone();				// Maximum heap size
}

//еееееееееееProgram starts here
void main(void)
{
StandardFileReply SFReply;

  InitToolbox();
//show file selector to get source res file
	StandardGetFile(0, -1, 0, &SFReply);
	if (SFReply.sfGood==1)
	 { 
	   ExtractResources(&SFReply.sfFile);
	 }
ExitToShell();

}


//ееееееееееееееееееееееееееееееееееееееееееееееее
//Given an FSSpec, extract all resources to individual data files
void ExtractResources(FSSpec *TheFSSpec)
{
short fRefNum;
int res_type_index=1;	//resource type index
int res_ID_index;		//resource index
int Typeend=0;	//tells us when we've scanned all resources
int IDend;
Handle hRsrc;

ResType rType;

//open res file
  fRefNum = FSpOpenResFile(TheFSSpec, fsRdPerm);
  if (fRefNum!=-1)
     {
     UseResFile(fRefNum);
     
     //scan the resource map. Get each one and save it as a file
       while (Typeend==0)	//outer loop is by type
       {
        Get1IndType(&rType, res_type_index);
        res_type_index++;
        if (rType==0) Typeend=1;	//if resource type not found then end this loop
        else	//walk through each res ID and extract it
         {
         res_ID_index=1;	IDend=0;
          while(IDend==0)	//inner loop by index
            {
            hRsrc = Get1IndResource(rType, res_ID_index);
            res_ID_index++;
            if (hRsrc==0) IDend=1;
            else
            resource_to_file(hRsrc);
            }	//end of IDend!=0
         
         }	//end of else walk through each ID
       }    //end of outer loop (Types) 
     }	//end of good file

}

//еееееееееееееееееее
//write the resource to a data file
void resource_to_file(Handle the_resource)
{
short rID;
ResType rType;
Str255 rName;
	
	GetResInfo(the_resource, &rID, &rType, rName);	//get id, type and name - we dont use the name
	
	DetachResource (the_resource);	//make it not a resource anymore
	write_file(the_resource, rID, rType);	//write the resource to a data file
	DisposeHandle( the_resource);	//claw the memory back it was using
}


void write_file(Handle the_resource, short rID, ResType rType)
{
OSErr iErr;	//our error variable
Str255 numberstr;	//build filename number as ascii here
Str255 filename;	//build complete filename here
short fRefNum;	//file reference number
long inOutCount;	//size of the data to write
Ptr bufPtr;		//ptr to the data to write

//make the filename
 ZNumToString(rID,numberstr);	//convert the resource ID to 5 difit number as ascii
 filename[0]=9;					//it's a pascal string of length 9
 filename[1]=rType>>24;			//store resource type - 4 chars
 filename[2]=rType>>16;
 filename[3]=rType>>8;
 filename[4]=rType;
 filename[5]=numberstr[0];		//store resource ID as five ascii digits representing the number
 filename[6]=numberstr[1];
 filename[7]=numberstr[2];
 filename[8]=numberstr[3];
 filename[9]=numberstr[4];
 
//create a data file (no resource fork)
iErr = Create(filename, 0, 'LsRt', 'DATA');

//open the file
if (!iErr)
iErr = FSOpen(filename, 0, &fRefNum);

//lock the data handle
HLock (the_resource);

//deref the handle to a ptr
bufPtr=*the_resource;

//get size of data
inOutCount=GetHandleSize(the_resource);

//write the data
if (!iErr)
iErr = FSWrite(fRefNum, &inOutCount, bufPtr);

//close the file
if (!iErr)
FSClose(fRefNum);

//bugger off
}

//ееееееееееееееееееееееееееееееееееееееееееееееееее
//convert unsigned short to 5 ascii digits - e.g. 128 comes out as 00128
void ZNumToString(unsigned short number, unsigned char *string)
{
int temp,temp1;

temp=number/10000;	//do 10000's
temp1=temp+'0';
string[0]=temp1;
number=number-(temp*10000);

temp=number/1000;	//do 1000's
temp1=temp+'0';
string[1]=temp1;
number=number-(temp*1000);

temp=number/100;	//you get the gist...
temp1=temp+'0';
string[2]=temp1;
number=number-(temp*100);

temp=number/10;
temp1=temp+'0';
string[3]=temp1;
number=number-(temp*10);

temp1=number+'0';
string[4]=temp1;

}