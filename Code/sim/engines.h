extern double total_fuel_consumption;
extern int can_consume_fuel;

extern int camera_object_total_fuel;
extern int camera_object_total_fuel_consumption;
extern int camera_object_orcs_fuel_consumption;
extern int camera_object_orcs_rot_fuel_consumption;
extern int camera_object_orcs_lin_fuel_consumption;


void run_engines(int object);

int is_engine1_available (int the_object);
int is_engine2_available (int the_object);
int is_rev1_available (int the_object);
int is_rev2_available (int the_object);

int is_engine2_fitted (int the_object);
