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

//Error_handler.ext
void report_error(const char *error_string, const Str255 error_string2, int error_number);

// not implemented yet for Zex2
void report_error_c(const char *error_string, const char * error_string2, int error_number);

void report_error_id(const char *error_String,unsigned long id);
void report_error_id_number(const char *error_string, unsigned long id, int number);
void report_error_id_non_fatal(const char *error_string, unsigned long id);


void MyDebugStr( short line, const char* file, const char* text);

