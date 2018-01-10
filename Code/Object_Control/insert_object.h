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

//insert object.ext
//pos_x,y etc should really be floats....
void 
insert_object(int object_serial,_3D *vertices,textured_connections *connections, int number_of_vertices, int number_of_connections, 
              int pos_x, unsigned long the_id, int is_crashable,float bounds_sphere,int texture_id, int translucent);


void  dmf_insert_object(int object_serial,_3D *vertices,dmf_textured_connections *connections, int number_of_vertices, int number_of_connections, 
               unsigned long the_id,int is_crashable,float bounds_sphere);