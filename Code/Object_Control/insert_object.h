//insert object.ext
//pos_x,y etc should really be floats....
void 
insert_object(int object_serial,_3D *vertices,textured_connections *connections, int number_of_vertices, int number_of_connections, 
              int pos_x, unsigned long the_id, int is_crashable,float bounds_sphere,int texture_id, int translucent);


void  dmf_insert_object(int object_serial,_3D *vertices,dmf_textured_connections *connections, int number_of_vertices, int number_of_connections, 
               unsigned long the_id,int is_crashable,float bounds_sphere);