#define MAX_AWAKE 38*60*60*13

typedef struct {
int days,hours,minutes,seconds;
} real_time_struct;


void get_real_time_struct(real_time_struct* time_struct);
void update_real_time();
void draw_real_time(int x, int y, int colour);
void init_game_time();
void add_a_minute_to_real_time();
void check_sleep();
int get_TOD_hours();
int get_TOD_mins();
int get_TOD_secs();

void flip_time_multiplier_up(void);
void flip_time_multiplier_down(void);
void set_time_multiplier_to_1(void);
void set_time_multiplier_to_5(void);
void set_time_multiplier_to_2(void);
void set_time_multiplier_to_10(void);


void set_time_multiplier_to_1000(void);
void set_time_multiplier_to_500(void);
void set_time_multiplier_to_200(void);
int get_time_multiplier(void);

