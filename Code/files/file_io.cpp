//Zex file io
//SB 14/11/98
/*
$Log: file_io.c,v $
Revision 1.2  2003/09/22 22:01:33  robp
Work in progress: portable file system, stage 1 - load resources via C standard library calls. THIS INCLUDES A PROJECT FILE UPDATE.

Revision 1.1.1.1  2003/09/05 22:35:24  stu_c
First Imported.

Revision 1.14  2002/09/08 02:03:13  stu_c
Precompiled header adjustments

Revision 1.13  2002/08/31 19:26:18  stu_c
Callsigns

Revision 1.12  2002/08/18 01:23:26  stu_c
GetZexFSSpec now works if Zex is launched into BG

Revision 1.11  2002/05/31 23:51:19  stu
Removed all warnings from the code

Revision 1.10  2002/04/13 15:33:42  stu
Rotation, Alycians, behaviour

Revision 1.9  2002/01/02 19:23:21  rob
Added key setup code

Revision 1.8  2001/12/29 01:09:47  stu
Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.

Revision 1.7  2001/11/05 20:04:34  stu
More OSX work

Revision 1.6  2001/11/02 23:18:35  stu
Further OS X work. Enabled syscon logging.

Revision 1.5  2001/11/02 19:31:15  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.4  2001/10/22 21:27:54  rob
Carbon warning changes

Revision 1.3  2001/10/21 12:37:54  rob
File changes to make compile under carbon

Revision 1.2  2001/10/21 00:57:32  stu
Initial port work

Revision 1.1  2001/10/20 23:59:52  rob
Various carbonisation fixes.

Revision 1.0.0.1  2001/10/17 20:46:04  rob
First Imported.

Revision 1.6  2001/06/24 21:47:22  stu
Removed some old camera prefs

Revision 1.5  2001/06/23 20:07:16  stu
0.66 checkin

Revision 1.4  2001/06/03 03:17:42  stu
OCB structure now contains a sub structure called:
ZObjectControl
Which contains all the control data for an object.

Added accessor in access_ocbs which returns a pointer to type ZObjectControl

Revision 1.3  2001/05/28 21:40:20  stu
Major commit 280501

Revision 1.2  2001/05/28 01:43:55  stu
280501

Revision 1.1.1.1  2001/01/01 21:13:35  rob
First Imported.

Revision 1.2  2000/11/21 19:19:51  stu
Lurkers clean up

Revision 1.1  2000/09/30 17:24:13  rob
Made resolution selectable again

Revision 1.0.0.1  2000/08/21 22:01:38  stu
First Imported.

 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <files.h>
    #include <processes.h>
    #include <resources.h>
    #include <folders.h>
    #include <script.h>
  #endif
  #include "Error_handler.h"
  #include "3d_structures_def.h"
  #include "zex_misc.h"
#endif //pch


#include "file_io.h"

#include "PrefsFile.h"
#include	"splat_control.h"
#include "instrum.h"
#include "ship_kit.h"
#include "camera_control.h"
#include "docked.h"

#include "system_independant_file.h"
#include "system_interface.h"

//read file
//needs an fsspec
//returns a handle to the file data
//Zex file io.int
FSSpec Zex_FSSpec;

Zprefs gzex_prefs;
int just_installed=0;	//set to 1 if prefs created
Str255 prefs_name="\pZEX Preferences";
#define PREFS_VERSION 24
void read_prefs()
{
extern int sound_volume;
extern int music_volume;
//extern int missile_flip;
extern int Use_SpeedLimit, Use_VBL, Change_res;
//extern int stop_time,start_time,accumulated_game_time;

//extern int camera_mode;

Zprefs reset_prefs = { 1,1,1, 0,0,40,40,0,PREFS_VERSION,DEFAULT_CAMERA_ZOOM,0,	//change res, vbl, speedlimiter, lores, sound volume, music volume, registered, version                   
                   0,0,0,0,	//ticks,random,key from random,regi key
                   { { 1000000,"ANVIL", }, 
                   { 750000,"CLAIRE", }, 	//HISCORES
                   { 500000,"PATCH", }, 
                   { 200000,"FLUFFY", },
                   { 100000,"STU", }, 
                   { 50000,"JESS", }, 
                   { 30000,"ROB", }, 
                   { 15000,"CATH", }, 
                   { 5000,"JULIE", },
                   { 1000,"IAN", } }
                   };


FSSpec prefs_fsspec;
	short foundVRefNum;
	long foundDirID;
	OSErr err;
	
	// find the Preferences folder
	err = FindFolder(	kOnSystemDisk,
						kPreferencesFolderType,
						kCreateFolder,
						&foundVRefNum,&foundDirID);
	if (!err)
		// make it into a FSSpec (given the file name)
	err =	FSMakeFSSpec(foundVRefNum, foundDirID, prefs_name, &prefs_fsspec);
    if (err) 
    {
    err=WritePrefsFile(prefs_fsspec,(char*)&reset_prefs,sizeof(gzex_prefs));
    just_installed=1;
    }
    if (!err) err = ReadPrefsFile(prefs_fsspec,(char*)&gzex_prefs,sizeof(gzex_prefs));

//check prefs version and rewrite if wrong version
    if (gzex_prefs.version!=PREFS_VERSION)
    {
     just_installed=1;
     WritePrefsFile(prefs_fsspec,(char*)&reset_prefs,sizeof(gzex_prefs));
     ReadPrefsFile(prefs_fsspec,(char*)&gzex_prefs,sizeof(gzex_prefs));
    }
    sound_volume=gzex_prefs.sound_volume;
    music_volume=gzex_prefs.music_volume;
//    Use_Mode320=gzex_prefs.Use_Mode320;
    Use_SpeedLimit=gzex_prefs.Use_SpeedLimit;
    Use_VBL=gzex_prefs.Use_VBL;
//    #if OGL==1
//    Change_res=0;
//    #else
    Change_res=gzex_prefs.Change_res;
//    #endif
//    camera_zoom=gzex_prefs.camera_zoom;
//    camera_mode=gzex_prefs.camera_mode;

}


void write_prefs()
{
extern Zprefs gzex_prefs;
extern int Use_SpeedLimit, Use_VBL, Change_res;
//extern int stop_time,start_time,accumulated_game_time;

FSSpec prefs_fsspec;
	short foundVRefNum;
	long foundDirID;
	OSErr err;
//	gzex_prefs.Use_Mode320=Use_Mode320;
	gzex_prefs.Use_SpeedLimit=Use_SpeedLimit;
	gzex_prefs.Use_VBL=Use_VBL;
	gzex_prefs.Change_res=Change_res;
	gzex_prefs.camera_zoom=0;
	gzex_prefs.camera_mode=1;
	// find the Preferences folder
	err = FindFolder(	kOnSystemDisk,
						kPreferencesFolderType,
						kCreateFolder,
						&foundVRefNum,&foundDirID);
	if (!err)
		// make it into a FSSpec (given the file name)
	err =	FSMakeFSSpec(foundVRefNum, foundDirID, prefs_name, &prefs_fsspec);
    WritePrefsFile(prefs_fsspec,(char*)&gzex_prefs,sizeof(gzex_prefs));
}

#if !REMOVE_OLD_FILESYSTEM_JUNK
Handle read_file (FSSpec the_file)
{
short refnum;
char permission=fsRdPerm;
OSErr Err;
long curEOF;
Handle hData;	//we return this

Err=FSpOpenDF(&the_file,permission,&refnum);
	if (Err)
		report_error("read_file(FSSpec) had an error on Open.",the_file.name,Err);

Err=GetEOF(refnum,&curEOF);	//get eof
	if (Err)
		report_error("read_file(FSSpec) had an error on GetEOF.",the_file.name,Err);

hData=NewHandleClear(curEOF+4);	//ensure zero terminated file!
	if (hData==0)
		report_error("read_file(FSSpec) had an error: No Memory.",the_file.name,3);

HLock(hData);

Err=FSRead(refnum,&curEOF,*hData);
	if (Err)
		report_error("read_file(FSSpec) had an error on Read.",the_file.name,Err);
		
Err=FSClose(refnum);
	if (Err)
		report_error("read_file(FSSpec) had an error on Close.",the_file.name,Err);
		
		
return hData;
}
#endif

// +--------------------------------+-------------------------+-----------------------
// | TITLE: read_file_into_memory   | AUTHOR(s): Rob Probin   | DATE STARTED: 22 Sept 03
// +
// | DESCRIPTION: Read a file into memory. Must be freed by release file read into memory.
// |
// | loaded_size can be NULL.
// +----------------------------------------------------------------ROUTINE HEADER----
void *read_file_into_memory(char *directory, char *filename, int *loaded_size)
{
ls_file_t my_file;
char path_filename[FILENAME_LENGTH];
void *data_storage;
int actual_bytes_read;
long file_size;

if(!make_filename(path_filename, filename, directory, "read_file_into_memory()"))	// make path first
    { report_error("read_file_into_memory(filename) had an error on path create.",filename,1100); }
  
my_file = lsf_open_file(path_filename, "rb");
if(my_file == NULL) { report_error("read_file_into_memory(filename) had an error on Open.",filename,1101); }

file_size = lsf_get_file_size(my_file);		//find size
if(file_size == -1) { report_error("read_file_into_memory(filename) had an error on get file size.",filename,1101); }

data_storage = calloc(1, file_size+4);		// ensure a zero terminated file (zero space and put 4 on the end)
if(data_storage == NULL) { report_error("read_file_into_memory(filename) had an error: No memory",filename,1102); }

actual_bytes_read = lsf_read_bytes_from_file(my_file, data_storage, file_size);
if(actual_bytes_read != file_size) { report_error("read_file_into_memory(filename) had an error on matching bytes read.",filename,1103); }

if(loaded_size !=NULL)
    {
    *loaded_size = file_size;
    }

lsf_close_file(my_file);

return data_storage;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: read_file_into_memory   | AUTHOR(s): Rob Probin   | DATE STARTED: 22 Sept 03
// +
// | DESCRIPTION: Abstraction for file previously read into memory (so memory mechanism can change)
// |
// +----------------------------------------------------------------ROUTINE HEADER----
void release_file_read_into_memory(void *file_memory_pointer)
{
free(file_memory_pointer);
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: resize_a_file_read_into_memory | AUTHOR(s): Rob Probin   | DATE STARTED: 24 Sept 03
// +
// | DESCRIPTION: Change the size of a file read into memory. Returns pointer to 
// | new space, or NULL if not possible. Space up to minimum of old and new is unchanged.
// +----------------------------------------------------------------ROUTINE HEADER----
void *resize_a_file_read_into_memory(void *file_memory_ptr, int new_size)
{
return realloc(file_memory_ptr, new_size);
}


short zex_res_file;
//fills Zex_fsspec with the fsspec of Zex
void get_zex_fsspec()
{
ProcessSerialNumber zex_psn;
OSErr Err;
ProcessInfoRec proc_info_rec;
CInfoPBRec pb;
FSSpec resource_folder_FSSpec;
Str255 dirname;
DirInfo *dpb=(DirInfo*) &pb;
int return_value;

zex_res_file=CurResFile();

//get zex psn
Err=GetCurrentProcess (&zex_psn);
proc_info_rec.processAppSpec=&Zex_FSSpec;
proc_info_rec.processName=NULL;
proc_info_rec.processInfoLength = sizeof(ProcessInfoRec);

Err=GetProcessInformation(&zex_psn, &proc_info_rec);	//get zex's fsspec in Zex_FSSpec

//now if PB build, traverse up the bundle and fix up the fsspec
     #if ZEX_PROJECT_BUILDER==1
	 resource_folder_FSSpec=Zex_FSSpec; //for debug

     dpb->ioVRefNum = resource_folder_FSSpec.vRefNum;
     dpb->ioDrDirID = resource_folder_FSSpec.parID;
     dpb->ioNamePtr = dirname;
     dpb->ioFDirIndex=-1;	//query
     return_value=PBGetCatInfo(&pb,0); //into MacOS
//now move on up into Contents
//     dpb->ioVRefNum = resource_folder_FSSpec.vRefNum;
     dpb->ioDrDirID = dpb->ioDrParID;
     dpb->ioNamePtr = dirname;
     dpb->ioFDirIndex=-1;	//query
     return_value=PBGetCatInfo(&pb,0);
//now move on up into top level of the app
     dpb->ioDrDirID = dpb->ioDrParID;
     dpb->ioNamePtr = dirname;
     dpb->ioFDirIndex=-1;	//query
     return_value=PBGetCatInfo(&pb,0);
//now move on up into LSG-ZEX
//     dpb->ioDrDirID = dpb->ioDrParID;
//     dpb->ioNamePtr = dirname;
//     dpb->ioFDirIndex=-1;	//query
//     return_value=PBGetCatInfo(&pb,0);

	FSMakeFSSpec(Zex_FSSpec.vRefNum,dpb->ioDrParID,DirStr,&Zex_FSSpec);
    
#endif

}

//еееееееееееееееееееееееееееее
OSErr WriteZexFile( FSSpec fileSpec, Ptr buffer, long bufferLen ) 
{
	short fileRefNum;
	OSErr err;
	extern OSType gPrefsCreator;
	
	// create our prefs file
	err = FSpCreate( &fileSpec, gPrefsCreator, 'GaMe', smSystemScript );
	if (err && err != -48) return err;

	// open it for writing
	err = FSpOpenDF( &fileSpec, fsWrPerm, &fileRefNum);
	if (err) return err;

	err = SetEOF( fileRefNum, 0);
	if (err) return err;

	// write the data
	err = FSWrite( fileRefNum, &bufferLen, buffer );
	if (err) return err;
	
	
	err = FSClose( fileRefNum );

	return err;
}


//еееееееееееееееееееееееееееее
OSErr ReadZexFile( FSSpec fileSpec, Ptr buffer, long bufferLen ) 
{
	short fileRefNum;
	OSErr err;
	//extern OSType gPrefsCreator;
	
	// open it for writing
	err = FSpOpenDF( &fileSpec, fsRdPerm, &fileRefNum);
	if (err) return err;

	// write the data
	err = FSRead( fileRefNum, &bufferLen, buffer );
	FSClose( fileRefNum );

	return err;
}

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
//ZEX save/load game
extern int gCheckWatchDog;
extern kit_desc ship_items[ITEMS_COUNT];
extern int score,credits,docked_at;
extern	DynObjectsFixedSize *ocb_ptr;

#define SAVE_VERS 25	//testers will hate me!

//-----Save Game Structure--------//
typedef struct {
UInt32 platform;	//'MAC ', 'PC  ' etc
UInt32 vers;
UInt32 cstring0; // йLightsoft-Stuart Ball-Robert Probin 1999.";
UInt32 cstring1; // йLightsoft-Stuart Ball-Robert Probin 1999.";
UInt32 cstring2; // йLightsoft-Stuart Ball-Robert Probin 1999.";
//galactic map selection
int selected_item_type;
int selected_item_index;
int selected_item_screen_x;
int selected_item_screen_y;
//general stuff
int score,credits,kills,rank,promotion_points,selected_pylon;
int level;
int hull;
int accumulated_time;
int difficulty;	
unsigned long docked_at_mothership_id;
int quad,section;
float target_x,target_y;
int sleep_timer,done_salary;

//experience etc.
int experience, laser_experience, hit_power,player_skill,luck;
int player_rank,total_level;
unsigned long wingman_id_0;	
unsigned long wingman_id_1;
unsigned long wingman_id_2;
unsigned long wingman_id_3;
unsigned long wingman_id_4;
unsigned long wingman_id_5;


kit_desc save_items[ITEMS_COUNT];


int in_level_save;

char for_level_use	[4000];	//level overlays a structure onto this
UInt32 pad [200];
} ZEX_save_file;

typedef struct
{
ZEX_save_file my_save_file;
//galactic_map the_gmap;
} saved_file_structure;

//------------------------------//



void save_game()
{
extern kit_desc ship_items[ITEMS_COUNT];
extern int current_level,difficulty;
extern int stop_time,start_time,accumulated_game_time;

extern int rank, kills, promotion_points,sleep_timer,done_salary;
extern int missile_flip;

//ZEX_save_file my_save_file;
saved_file_structure * saved_file_data_ptr;

//Str255 my_prompt="\pSave game as...";
char my_name[64];	//make save file name here
char my_pname[64];	//as pascal
	int i;
	short fileRefNum=0;
#if ZEX_CORE_CARBON
  MyDebugStr(__LINE__,__FILE__,"Carbon Conversion not finished");
#else
  StandardFileReply my_reply;
#endif

   gCheckWatchDog=0;	//disable the watchdog
GraphicsPaused();
FlushEvents(-1,0);	//flush all
//get memory for saved game. bout 200K at last check
saved_file_data_ptr=(saved_file_structure*) NewPtr(sizeof(saved_file_structure));
if (saved_file_data_ptr==0) 
      report_error_id_non_fatal("Unable to save game - not enough memory.", MemError());

//we save by simply writing out the structure
//saved_file_data_ptr->my_save_file.docked_at_mothership_id=(*ocb_ptr).object_list[docked_at].Dyn_OCB_control_data.mship_ptr->callsign;
//saved_file_data_ptr->my_save_file.quad=current_quad;
//saved_file_data_ptr->my_save_file.section=get_section(current_quad, (*ocb_ptr).object_list[docked_at].Oworldx, (*ocb_ptr).object_list[docked_at].Oworldy);
//saved_file_data_ptr->my_save_file.quad
saved_file_data_ptr->my_save_file.sleep_timer=sleep_timer;

saved_file_data_ptr->my_save_file.platform='MAC ';
saved_file_data_ptr->my_save_file.vers=SAVE_VERS;
saved_file_data_ptr->my_save_file.cstring0='Zex ';
saved_file_data_ptr->my_save_file.cstring1='йLSG';
saved_file_data_ptr->my_save_file.cstring2='1999';

//saved_file_data_ptr->my_save_file.selected_item_type=selected_item_type;
//saved_file_data_ptr->my_save_file.selected_item_index=selected_item_index;
//saved_file_data_ptr->my_save_file.selected_item_screen_x=selected_item_screen_x;
//saved_file_data_ptr->my_save_file.selected_item_screen_y=selected_item_screen_y;

saved_file_data_ptr->my_save_file.rank=rank;
saved_file_data_ptr->my_save_file.kills=kills;
saved_file_data_ptr->my_save_file.promotion_points=promotion_points;
saved_file_data_ptr->my_save_file.done_salary=done_salary;	//have we credited today?

saved_file_data_ptr->my_save_file.level=current_level;
//saved_file_data_ptr->my_save_file.hull=hull_strength;
saved_file_data_ptr->my_save_file.score=score;
saved_file_data_ptr->my_save_file.credits=credits;
saved_file_data_ptr->my_save_file.selected_pylon=missile_flip;
saved_file_data_ptr->my_save_file.accumulated_time=(stop_time-start_time)+accumulated_game_time;
saved_file_data_ptr->my_save_file.difficulty=difficulty;
saved_file_data_ptr->my_save_file.target_x=0;
saved_file_data_ptr->my_save_file.target_y=0;	//not used in z2

for (i=0;i<ITEMS_COUNT;i++)
saved_file_data_ptr->my_save_file.save_items[i]=ship_items[i];


//copy galactic map
//BlockMoveData(GM_ptr,&saved_file_data_ptr->the_gmap,sizeof (galactic_map));
FlushEvents(everyEvent,0);
//get_callsign_and_name(my_name,docked_at);
CToPascal(my_name,my_pname);

#if ZEX_CORE_CARBON
  MyDebugStr(__LINE__,__FILE__,"Carbon Conversion not finished");
#else
  StandardPutFile(my_prompt,(unsigned char const*) my_pname,&my_reply);	//talk about finickety!
//	err =	FSMakeFSSpec(my_reply.sVRefNum, my_reply.DirID, my_reply.name, &prefs_fsspec);
    WriteZexFile(my_reply.sfFile,(char*)saved_file_data_ptr, sizeof (saved_file_structure));
#endif


	FSClose( fileRefNum );
    
FlushEvents(-1,0);	//flush all
GraphicsActive();
   DisposePtr((Ptr)saved_file_data_ptr);
   
   gCheckWatchDog=1;	//enable the watchdog

}



//this is a tuffy, we have to load up the data and get zex
//into the mothership
int load_game()
{
extern kit_desc ship_items[ITEMS_COUNT];
//extern int current_level;
extern int next_level,difficulty,tutorial;
extern int do_next_level,docked_at;
extern int global_open_doors;
extern int sleep_timer,done_salary,rank,kills,promotion_points;
extern int done_salary;
extern int missile_flip;
//extern int Use_SpeedLimit, Use_VBL, Change_res;
//extern int stop_time;
extern int start_time,accumulated_game_time;

//Str255 my_prompt="\pLoad game...";
//OSType typeList; //[]= 'GaMe';
	int i,return_value;
	short fileRefNum;
saved_file_structure * saved_file_data_ptr;
unsigned long callsign;

#if ZEX_CORE_CARBON
  MyDebugStr(__LINE__,__FILE__,"Carbon Conversion not finished");
#else
  StandardFileReply my_reply;
#endif

     global_open_doors=1;

   gCheckWatchDog=0;	//disable the watchdog
GraphicsPaused();
FlushEvents(-1,0);	//flush all
//get memory for file

saved_file_data_ptr=(saved_file_structure*) NewPtr(sizeof(saved_file_structure));
if (saved_file_data_ptr==0) 
      {
      report_error_id_non_fatal("Unable to load game - not enough memory.", MemError());
      goto abort_load;
      }
      
FlushEvents(everyEvent,0);
#if ZEX_CORE_CARBON
  MyDebugStr(__LINE__,__FILE__,"Carbon Conversion not finished");
  if(FALSE)
    {
#else
  StandardGetFile(0,1,typeList,&my_reply);
if (my_reply.sfGood==1) 
 {
 return_value=1;	//OK

    ReadZexFile(my_reply.sfFile,(char*)saved_file_data_ptr,sizeof(saved_file_structure));
#endif

	FSClose( fileRefNum );

//we save by simply writing out the structure
 if (saved_file_data_ptr->my_save_file.platform!='MAC ') 
 {
 report_error_id_non_fatal("Not a Mac saved game.",'FAIL');
 DisposePtr((Ptr)saved_file_data_ptr);
 goto abort_load;
 }
 if (saved_file_data_ptr->my_save_file.vers!=SAVE_VERS) 
 {
 report_error_id_non_fatal("Saved Game is wrong version.",'FAIL');
 DisposePtr((Ptr)saved_file_data_ptr);
 goto abort_load;
 }

 next_level=saved_file_data_ptr->my_save_file.level+1;
// hull_strength=saved_file_data_ptr->my_save_file.hull;
sleep_timer=saved_file_data_ptr->my_save_file.sleep_timer;
 
rank=saved_file_data_ptr->my_save_file.rank;
kills=saved_file_data_ptr->my_save_file.kills;
promotion_points=saved_file_data_ptr->my_save_file.promotion_points;

//targ_x=saved_file_data_ptr->my_save_file.target_x;
//targ_y=saved_file_data_ptr->my_save_file.target_y;

//selected_item_type=saved_file_data_ptr->my_save_file.selected_item_type;
//selected_item_index=saved_file_data_ptr->my_save_file.selected_item_index;
//selected_item_screen_x=saved_file_data_ptr->my_save_file.selected_item_screen_x;
//selected_item_screen_y=saved_file_data_ptr->my_save_file.selected_item_screen_y;

 score=saved_file_data_ptr->my_save_file.score;
 credits=saved_file_data_ptr->my_save_file.credits;
 difficulty=saved_file_data_ptr->my_save_file.difficulty;
 done_salary=saved_file_data_ptr->my_save_file.done_salary;	//have we credited today?
 tutorial=0;
 missile_flip=saved_file_data_ptr->my_save_file.selected_pylon;
 
accumulated_game_time=saved_file_data_ptr->my_save_file.accumulated_time;
start_time=TickCount();

 for (i=0;i<ITEMS_COUNT;i++)
 ship_items[i]=saved_file_data_ptr->my_save_file.save_items[i];


//now we need to load up the OCB's from the galactic map then find the right mothership in the ocb's from the callsign
//we are all ready to run (all objects loaded, so we should be able to go docked...
//copy galactic map

//BlockMoveData(&saved_file_data_ptr->the_gmap,GM_ptr,sizeof (galactic_map));

//load OCB's
//init_map_control();
//current_quad=saved_file_data_ptr->my_save_file.quad;
//load_ocbs(saved_file_data_ptr->my_save_file.quad,saved_file_data_ptr->my_save_file.section);

do_next_level=0;
//find mothership in OCB's

callsign=saved_file_data_ptr->my_save_file.docked_at_mothership_id;

//the worst bit of C you're ever likely to see follows...
for(i=0;i<MAX_NUMBER_OF_DYN_OBJECTS;i++)
 {
// if ((*ocb_ptr).object_list[i].Dyn_OCB_control_data.mship_ptr->callsign==callsign) goto got_ms;
 }
report_error_id_non_fatal("Couldn't load game. Docked at not found. Saved File corrupt?.",'FAIL');
return_value=0;
DisposePtr((Ptr)saved_file_data_ptr);
goto end_load;

/*got_ms:*/ docked_at=i;
//docked_at_shields=(*ocb_ptr).object_list[i].shield_value;

(*ocb_ptr).object_list[0].Oworldx=(*ocb_ptr).object_list[i].Oworldx;
(*ocb_ptr).object_list[0].Oworldy=(*ocb_ptr).object_list[i].Oworldy;

//mothership_cat=(*ocb_ptr).object_list[i].object_category;	//that should allow the main loop to dock us!
DisposePtr((Ptr)saved_file_data_ptr);


}
else
abort_load: return_value=0;	//cancel

    
end_load: FlushEvents(-1,0);	//flush all
GraphicsActive();
   gCheckWatchDog=1;	//enable the watchdog

return return_value;
}



// +--------------------------------+-------------------------+-----------------------
// | TITLE: ZReleaseResource        | AUTHOR(s): Rob Probin   | DATE STARTED: 24 Sept 03
// +
// | DESCRIPTION: Release a resource read into memory.
// +----------------------------------------------------------------ROUTINE HEADER----
void ZReleaseResource(void *resource_ptr)
{
release_file_read_into_memory(resource_ptr);
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: ZReleaseResource        | AUTHOR(s): Rob Probin   | DATE STARTED: 24 Sept 03
// +
// | DESCRIPTION: Change the size of a resource. Returns pointer to 
// | new space, or NULL if not possible. Space up to minimum of old and new is unchanged.
// +----------------------------------------------------------------ROUTINE HEADER----
void *ZSetResourceSize(void *resource_ptr, int new_size)
{
return resize_a_file_read_into_memory(resource_ptr, new_size);
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: ZGetResource            | AUTHOR(s): Rob Probin   | DATE STARTED: 24 Sept 03
// +
// | DESCRIPTION: Load a resource previous read into memory.
// |
// | loaded_size can be NULL.
// +----------------------------------------------------------------ROUTINE HEADER----

#if PORTABLE_FILESYSTEM
void *ZGetResource(unsigned int Type, short ID, int *loaded_size)
#else
Handle ZGetResource(unsigned int Type, short ID)
#endif
{
#if PORTABLE_FILESYSTEM
unsigned char filename[FILENAME_LENGTH];
unsigned char numberstr[FILENAME_LENGTH];
#else
Str255 filename;
FSSpec the_fsspec;
FSSpec resource_folder_FSSpec;
//FSSpec the_resource_FSSpec;

Str255 numberstr;	//build filename number as ascii here

extern FSSpec Zex_FSSpec;
CInfoPBRec pb;
DirInfo *dpb=(DirInfo*) &pb;
HFileInfo *fpb=(HFileInfo*) &pb;	//ptrs to the union (Finfo and DirInfo) held in CInfoPBRec
int folder_DirID;
int return_value;
int get_file_err = 0;
//short file_index=1;
#endif


//create filename
//make the filename
 ZNumToString(ID,numberstr);	//convert the resource ID to 5 digit number as ascii
 filename[0]=9;					//it's a pascal string of length 9
 filename[1]=Type>>24;			//store resource type - 4 chars
 filename[2]=Type>>16;
 filename[3]=Type>>8;
 filename[4]=Type;
 filename[5]=numberstr[0];		//store resource ID as five ascii digits representing the number
 filename[6]=numberstr[1];
 filename[7]=numberstr[2];
 filename[8]=numberstr[3];
 filename[9]=numberstr[4];


#if PORTABLE_FILESYSTEM

//  1. im working here adding the allocate/open/read/close... (need to decide what size buffer to allocate)
//  2. also rest of program wants me to return a handle? Do we do this or do we return a pointer and alter
//     the rest of the program. Also do they ever de-allocate this space? With what call?
     
  return read_file_into_memory(RESOURCE_DIRECTORY,filename, loaded_size);
#else
//get the objects' folder
// If osx build then we need to climb out of the bundle to get to the zd3 folder
// We know the directory ID that holds Zex, so if we get info on that, we should get the parent directory
// Package = zex/contents/MacOS
// get directory ID of contents
	resource_folder_FSSpec=Zex_FSSpec; //for debug

	return_value=FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,DirStr,&resource_folder_FSSpec);
//get zd3's dirctory ID
     dpb->ioVRefNum = resource_folder_FSSpec.vRefNum;
     dpb->ioDrDirID = resource_folder_FSSpec.parID;
     dpb->ioNamePtr = resource_folder_FSSpec.name;
     dpb->ioFDirIndex=0;	//query
     return_value=PBGetCatInfo(&pb,0);
     folder_DirID=dpb->ioDrDirID;

     if (return_value) 
     {
     report_error ("ZGetResource couldn't find the folder named:",DirStr,return_value);
     return 0;
     }

return_value=0;

//find sub folder
//      fpb->ioVRefNum = texture_folder_FSSpec.vRefNum;
     fpb->ioDirID = folder_DirID;
     fpb->ioNamePtr = "\pR";	//resources folder
     fpb->ioFDirIndex=0;	//query the file name

     return_value=PBGetCatInfo(&pb,0);
     if (return_value) 
     {
     report_error ("ZGetResource couldn't find the folder named:","\pR",return_value);
     return 0;
     }
     folder_DirID=dpb->ioDrDirID;

return_value=0;

      fpb->ioVRefNum = resource_folder_FSSpec.vRefNum;
      fpb->ioDirID= folder_DirID;
//     fpb->ioDirID = folder_DirID;
     fpb->ioNamePtr = filename;
     fpb->ioFDirIndex=0;	//gimmee the file name
     get_file_err=PBGetCatInfo(&pb,0);
     if(get_file_err) return 0;
//now load it
      FSMakeFSSpec (fpb->ioVRefNum, folder_DirID,filename,&the_fsspec);

	return read_file(the_fsspec);	//handles errors locally; kills the game on error
#endif
}

//given a filename (p1) and an extension (p2) returns 1 if this file does have that
//extension else 0
//extension (p2) should be passed in in lower case
int check_file_extension (unsigned char * filename, unsigned char* extension)
{
 int length;
 length=strlen((char*) filename);
 length--;	//for 0 offset
 
 if ((filename[length-2]==extension[0]) &&
     (filename[length-1]==extension[1]) &&
     (filename[length-0]==extension[2])) return 1;
     //try for upper case
     extension[0]-=0x20;
     extension[1]-=0x20;
     extension[2]-=0x20;
     
     
 if ((filename[length-2]==extension[0]) &&
     (filename[length-1]==extension[1]) &&
     (filename[length-0]==extension[2])) 
     {
     extension[0]+=0x20;	//put extension back to as found just in case of compiler optimisations!
     extension[1]+=0x20;
     extension[2]+=0x20;
     return 1;     
     }
     else 
     {
     extension[0]+=0x20;
     extension[1]+=0x20;
     extension[2]+=0x20;
     return 0;
     }
}