
//camera_control.ext


#define DEFAULT_CAMERA_ZOOM 10000
#define MIN_CAMERA_ZOOM 3500
#define MAX_CAMERA_ZOOM 17500

#define CAMERA_OBJECT 2

void run_camera_control(void);
void reset_camera();
int get_main_camera_object(void);
int set_main_camera_object(int dynamic_slot);

void camera_set_front_view();
void camera_set_right_view();
void camera_set_left_view();
void camera_set_rear_view();

int has_camera_switched_view();

int camera_get_view(); //returns 1=front, !1=panned
float camera_get_pan(); //returns camera pan force




