typedef struct
{
int slot;
int allegiance;
float x,y;
} mothership_nav_entry;


void handle_earth_mothership(int it);	//tractor and laser - stationary position
void do_mothership_hit(int object_slot, int mothership_slot);
void cancel_dock(int object_being_docked);

void handle_active_mothership(int it);	  //a live mothership, gens bad guys
void load_motherships_into_nav();
void update_motherships();
void handle_game_mothership(int it);
int is_a_mothership(int slot);
//mammary

mothership_block * mship_get_mem();
int mship_release_mem(mothership_block * the_block);
void init_mship_mem();