void do_game_pause(void);
void do_game_dead();
void do_game_alert(Ptr the_string);
int do_game_restore();

void setup_screen(LSRAW* the_picture, LSRAW* the_button);
int handle_pause_mouse(LSRAW* button_down, LSRAW* button_up);

void check_balls();
void get_level();

