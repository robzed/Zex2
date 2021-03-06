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

//cruncher file io
//Zex file io
//SB 14/11/98
#include <files.h>
#include <processes.h>
#include "file_io.ext"
#include "file_io.int"
#include "Error_handler.ext"

//read file
//needs an fsspec
//returns a handle to the file data

Handle read_file (FSSpec the_file)
{
short refnum;
char permission=fsRdPerm;
OSErr Err;
long curEOF;
Handle hData;	//we return this

Err=FSpOpenDF(&the_file,permission,&refnum);
	if (Err)
		report_error("read_file(FSSpec) had an error on Open.");

Err=GetEOF(refnum,&curEOF);	//get eof
	if (Err)
		report_error("read_file(FSSpec) had an error on GetEOF.");

hData=NewHandleClear(curEOF+4);	//ensure zero terminated file!
	if (hData==0)
		report_error("read_file(FSSpec) had an error: No Memory.");

HLock(hData);

Err=FSRead(refnum,&curEOF,*hData);
	if (Err)
		report_error("read_file(FSSpec) had an error on Read.");

Err=FSClose(refnum);
	if (Err)
		report_error("read_file(FSSpec) had an error on Close.");
		
return hData;
}

//fills Zex_fsspec with the fsspec of Zex
get_zex_fsspec()
{
ProcessSerialNumber zex_psn;
OSErr Err;
ProcessInfoRec proc_info_rec;

//get zex psn
Err=GetFrontProcess (&zex_psn);
proc_info_rec.processAppSpec=&Zex_FSSpec;
proc_info_rec.processName=NULL;
proc_info_rec.processInfoLength = sizeof(ProcessInfoRec);

Err=GetProcessInformation(&zex_psn, &proc_info_rec);	//get zex's fsspec

}

//note - little error checking!
write_file(char * the_data, int data_length, FSSpec the_file)
{
short refnum;
char permission=fsWrPerm;
OSErr Err;

FSpCreate (&the_file, 'LsZx', 'RCW ',0);

Err=FSpOpenDF(&the_file,permission,&refnum);
	if (Err)
		report_error("write_file(FSSpec) had an error on Open.");

Err=SetEOF(refnum,0);	//get eof
	if (Err)
		report_error("rite_file(FSSpec) had an error on SetEOF.");


Err=FSWrite(refnum, (long *) &data_length,the_data);
	if (Err)
		report_error("write_file(FSSpec) had an error on Write.");

Err=FSClose(refnum);
	if (Err)
		report_error("write_file(FSSpec) had an error on Close.");

}