void run_map_control();

void init_map_control();

void set_map_update_rate(int rate);
int get_section(int quad, int x, int y);
//void load_ocbs(int quad, int section);
void load_ocbs(int quad, int section);
void force_ocb_update();
void calc_current_quad();
