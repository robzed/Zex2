#define AP_OFF 0
#define AP_STEER_CUE 1
#define AP_STEER 2
#define AP_TRANSPORT 3
#define AP_COMBAT 4
#define AP_HYPERTRANSPORT 5

#define HYPERTRANSPORT_DELAY 10.0 //seconds before engagement


extern int camera_object_quat_x,camera_object_quat_y,camera_object_quat_z,camera_object_quat_w;
extern int camera_quat_x,camera_quat_y,camera_quat_z,camera_quat_w;
extern int debug_time_to_stop, debug_time_to_target, debug_distance_to_target;

void run_AP(int object);
void run_flight_computations(int object);
void engage_ap(int the_object, int the_mode);
void disengage_AP(int object);
int find_nearest_planet(int object);
int is_AP_aligned(int object);
