void do_object_control();
void handle_weeny_bullet(int it);
void handle_explode(int it);
void handle_monster1(int it);
void handle_not_so_weeny_bullet(int it);
void handle_ejection(int it);
void kill_kill_list();
void reset_kill_list();
int add_to_kill_list (int current_object);
void handle_laser_weeny (int it, int rear);
void handle_gun1(int it);
void handle_persistent_asteroid(int it);
void handle_burning_particle(int it);
void handle_missile1(int it);
void handle_space_cow(int it);
void handle_earth_mothership(int it);
void handle_planet_roty(int it);	//rotate slowly every control_1 frames
void handle_planet_rotz(int it);	//rotate slowly every control_1 frames
void do_mothership_hit(int object_slot, int mothership_slot);
void handle_static_laser1(int it);	//static laser cointroller

void i_know_what_im_doing_kill( int the_object, int kill_ogl_textures);

void reset_torque(ZObject* current_object_ptr);

void kill_children(int parent);



