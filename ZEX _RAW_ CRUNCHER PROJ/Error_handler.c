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

//Error_handler.c
#include <Dialogs.h>
#include <Processes.h>
#include <TextUtils.h>
#include <memory.h>

#include <strings.h>

#include <InputSprocket.h>

#include	"error_handler.ext"
#define kALRTFatal 128

void report_error(char *error_string)
{
unsigned char	errorString[256];
char i;
	i=0;
	while ((errorString[i+1]=error_string[i]) !='\0')	i++;
	errorString[0]=i;	//make pascal
	
//	ISpSuspend();

	ParamText((unsigned char*)errorString, "\p", "\p", "\p");
	if (Alert(kALRTFatal, nil) == cancel)
		Debugger();
	
	ExitToShell();

}
