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
// * FILENAME           : coffee_file_interface.cpp
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 04:27:20 on 1st July 2000
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
/* ***********************************************************************************
 * $Log: coffee_file_interface.cpp,v $
 * Revision 1.3  2003/09/27 08:58:32  robp
 * Removal of Invalid Conversion Warning for C++ (I hate unsigned/signed chars)
 *
 * Revision 1.2  2003/09/20 12:57:07  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:35:23  stu_c
 * First Imported.
 *
 * Revision 1.8  2002/09/08 02:17:04  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.7  2002/01/02 19:23:45  rob
 * Added key setup code
 *
 * Revision 1.6  2001/12/29 01:11:40  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.5  2001/11/05 20:04:22  stu
 * More OSX work
 *
 * Revision 1.4  2001/11/03 22:30:05  stu
 * Bundles
 *
 * Revision 1.3  2001/11/02 19:32:48  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:29:18  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:08:53  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:04  rob
 * First Imported.
 *
 * Revision 1.3  2001/05/28 01:55:10  stu
 * 280501
 *
 * Revision 1.2  2001/04/08 20:24:11  rob
 * changed init around
 *
 * Revision 1.1.1.1  2001/01/01 21:13:35  rob
 * First Imported.
 *
 * Revision 1.0.0.1  2000/08/21 22:01:32  stu
 * First Imported.
 *
 */


// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "3d_structures_def.h"
  #include "error_handler.h"
#endif //pch

#include "coffee_file_interface.h"



coffee_file_spec perm_fspec;
long words_folder_DirID;
short words_folder_drive_id;


//How to correctly read a file:
//1. Use coffee_get_file_spec(index) to get its name
//2. Get the file size using coffee_get_file_size
//3. Allocate storage for the file from the returned size
//4. Use coffee_read_file to actually read the file data into the space
//cfi automatically closes the file so you don't need to worry about it.


//writing a file is accomplished with coffee_write_file; it does everything needed.
//Check the errors!



//test function
void cfi_test(void)
{
coffee_file_spec * the_spec_ptr;
int n,size;
char* data_read;

n=0;
while (n!=10)
	{
	the_spec_ptr=coffee_get_file_spec(n);
	if (the_spec_ptr->error==0)
	{
 		size=coffee_get_file_size((char*)the_spec_ptr->name);
		data_read=NewPtr(size);
		if (data_read!=0)
		{
			coffee_read_file((char*)the_spec_ptr->name, data_read);
			coffee_write_file((char*)the_spec_ptr->name, data_read, size);
			DisposePtr(data_read);
		}

	}
		
	n++;
	}
	
}

unsigned char directory_string[] = "\pW";
unsigned char *directory = directory_string;		// removed const - we pass to an OS file routine that doesn't have a const parameter


// **************Actual routines
//returns a pointer to the file spec of the W folder with file index index.
//if the caller is not using the data immediately then it should copy the return data
//If error then the error field of the file spec is set to non 0
//Never interfaces to the user.
//perm_fspec error filled in as follows:
//NOERR
//FOLDERERR
//INDEXERR 


coffee_file_spec* coffee_get_file_spec(int file_index)
{
FSSpec texture_folder_FSSpec;


extern FSSpec Zex_FSSpec;
CInfoPBRec pb;
DirInfo *dpb=(DirInfo*) &pb;
HFileInfo *fpb=(HFileInfo*) &pb;	//ptrs to the union (Finfo and DirInfo) held in CInfoPBRec
int folder_DirID;
int return_value,get_file_err;

if (file_index<1)
{
      perm_fspec.error=INDEXERR;	//fnf
      perm_fspec.name[0]=0;
	return &perm_fspec;
}


//get the words folder
	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,DirStr,&texture_folder_FSSpec);
	
//get zd3's dirctory ID
     dpb->ioVRefNum = texture_folder_FSSpec.vRefNum;
     dpb->ioDrDirID = texture_folder_FSSpec.parID;
     dpb->ioNamePtr = texture_folder_FSSpec.name;
     dpb->ioFDirIndex=0;	//query
     return_value=PBGetCatInfo(&pb,0);
     folder_DirID=dpb->ioDrDirID;

return_value=0;

//find sub folder
//      fpb->ioVRefNum = texture_folder_FSSpec.vRefNum;
     fpb->ioDirID = folder_DirID;
     fpb->ioNamePtr = directory;
     fpb->ioFDirIndex=0;	//query the file name

     return_value=PBGetCatInfo(&pb,0);
     if (return_value) 
     {
    	perm_fspec.error=FOLDERERR;	//folder error - W missing
      perm_fspec.name[0]=0;
      return &perm_fspec;
     }
     folder_DirID=dpb->ioDrDirID;
     
//now get the file name that corresponds to file index
      fpb->ioVRefNum = texture_folder_FSSpec.vRefNum;
      fpb->ioDirID= folder_DirID;
     fpb->ioNamePtr = perm_fspec.name;
     fpb->ioFDirIndex=file_index;	//gimmee the file name
     get_file_err=PBGetCatInfo(&pb,0);
     if(get_file_err) 	
     { 
      perm_fspec.error=INDEXERR;	//fnf
      perm_fspec.name[0]=0;
     }
     else
{
     perm_fspec.error=0;	//no error
      words_folder_DirID=folder_DirID;
      words_folder_drive_id=dpb->ioVRefNum;

}	
	return &perm_fspec;
} 

//returns file size or -error
int coffee_get_file_size(char* name)
{
FSSpec temp;
int Err;
char permission=fsRdPerm;
short refnum;
long curEOF;

	temp.parID=words_folder_DirID;
	pstrcopy((char*) name, (char*)temp.name);
	temp.vRefNum=words_folder_drive_id;	//defualt

Err=FSpOpenDF(&temp,permission,&refnum);
	if (Err) return OPENERR;

Err=GetEOF(refnum,&curEOF);	//get eof
	if (Err) return EOFERR;
Err=FSClose(refnum);
	if (Err) return CLOSEERR;

return curEOF;
}


//Read a file returns 0 if no err else err
int coffee_read_file(char* name, char* data_goes_here)
{
FSSpec temp;
int Err;
char permission=fsRdPerm;
short refnum;
long curEOF;

	temp.parID=words_folder_DirID;
	pstrcopy((char*) name, (char*)temp.name);
	temp.vRefNum=words_folder_drive_id;	//defualt

Err=FSpOpenDF(&temp,permission,&refnum);
	if (Err) return OPENERR;

Err=GetEOF(refnum,&curEOF);	//get eof
	if (Err) return EOFERR;

Err=FSRead(refnum,&curEOF,data_goes_here);
	if (Err) return READERR;
		
Err=FSClose(refnum);
	if (Err) return CLOSEERR;
		
return 0;

}

//write a file to the W folder
int coffee_write_file(char* name, char* data_to_write, int length)
{
	short fileRefNum;
	OSErr err;

	FSSpec temp;
	
	temp.parID=words_folder_DirID;
	pstrcopy((char*) name, (char*)temp.name);
	temp.vRefNum=words_folder_drive_id;	//defualt

	
	// create our file
	err = FSpCreate( &temp, 'Lide', 'TEXT', -1 );
	if (err && err != -48) return CREATERR;
                
	// open it for writing
	err = FSpOpenDF( &temp, fsWrPerm, &fileRefNum);
	if (err) return OPENERR;

	err = SetEOF( fileRefNum, 0);
	if (err) return WRITEERR;

	// write the data
	err = FSWrite( fileRefNum, (long*)&length, data_to_write );
	if (err) return WRITEERR;
	

	FSClose( fileRefNum );
	if (err) return CLOSEERR;


return 0;

}

//generic routines follow...
void pstrcopy(char* from, char* to)
{
int len;

len=from[0];
len++;

while (len>-1) 
 {
 to[len]=from[len];
 len--;
 }
}