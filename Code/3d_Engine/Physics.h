#define ZEX_UNIT_SIZE_IN_METERS 0.01	//1 cms size

#define HALF_SEC 0.5
#define QUARTER_SEC 0.25
#define ONE_SEC 1
#define TENTH_SEC 0.1
#define FIFTH_SEC 0.2
#define ATMOSPHERE_PERCENTAGE 0.07	//percentage of a planets radius is atmosphere 'depth'



#define UNIT_RADIUS 0.5


void apply_initial_force (int object, float magnitude, float for_how_long_in_secs, vector in_p /*oint*/ , vector in_d /*irection*/);
void apply_a_force(int object, float magnitude, vector in_p /*oint*/ , vector in_d /*irection*/) ;
void apply_extern_forces(_quat* QrotObj, vector* TorqueSum, float mass);

//Fixed times for camera control (see handle_camera in object control.c)
void apply_a_force_1sec(int object, float magnitude, vector in_p /*oint*/ , vector in_d /*irection*/);
void apply_extern_forces_1sec(_quat* QrotObj, vector* TorqueSum, float mass);

void apply_a_linear_force_world_reference(int object, vector force);
void reset_angular_torque(int dyn_object);

//distance calcs
double get_distance_to_from(int Oto, int Ofrom);
double get_distance_to_zex(int object);
double get_distance_to_from_point(double from_point_x,double from_point_y,double from_point_z,
                               double to_point_x,double to_point_y,double to_point_z);

double get_distance_to_from_in_m(int Oto, int Ofrom);
double get_distance_apart(_3D a, _3D b);

double get_length_of_vector(vector a);

void get_vector_to(vector* v, int Oto, int Ofrom);

