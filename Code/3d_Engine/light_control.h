#define MAX_NUMBER_OF_OMNILIGHTS 2	// how many suns Rob's allowed to have...

//zex 2 - 101200 Stu
void init_lights(void);
int set_light_position(int light_number, double position_x, double position_y, double position_z);
int get_light_position(int light_number, _3D *return_val);
int get_light_brightness(int light_number, int *return_val);
int set_light_default_brightness(int light_number);
int set_light_brightness(int light_number, int brightness);

//3d functions
void	calculate_light_normal(int object, _3D* light_normal, int * light_amplitude);	
void   set_black_level(int the_black_level); //0=black, 32767=white


