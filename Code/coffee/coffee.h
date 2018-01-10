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

void get_coffee(void);
void run_coffee(void);
void coffee_start_new_game(void);
void coffee_end_game(void);
void coffee_object_created(int id);
void coffee_object_deleted(int id);

void complete_coffee_setup(void);

int interpret(char *string);
void declare_variable(char *name, void *var_name);
void declare_constant(char *name, int const_name);

#define declare_function(name,function,parameters,return_param) coffee5(declare_c_function,(long)name,(long)function,parameters,return_param)
#define declare_advanced_function(name,function,parameters,return_param) coffee5(declare_func_advanced,(long)name,(long)function,parameters,return_param)
