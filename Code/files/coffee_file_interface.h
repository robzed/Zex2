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

typedef struct {
int error;
Str63 name;
} coffee_file_spec;	// if changed please change root.zsl

//errors
#define NOERR 0	
#define FOLDERERR -1	//W folder missing
#define INDEXERR -2	//index error (runs 1 to max) - file not found

#define OPENERR -3
#define EOFERR -4
#define READERR -5
#define CLOSEERR -6
#define WRITEERR -7
#define CREATERR -8


//stu test routine 
void cfi_test(void);

extern unsigned char *directory;		// removed const - we pass to an OS file routine that doesn't have a const parameter

//given an index return a pointer to a coffee_file_spec that gives the name of the file
coffee_file_spec* coffee_get_file_spec(int index);
//given a file name returns the size of it or -error
int coffee_get_file_size(char* name);
//given a file name and data to space reads a file
int coffee_read_file(char* name, char* data_goes_here);
//write a file to the W folder
int coffee_write_file(char* name, char* data_to_write, int length);

void pstrcopy(char* from, char* to);