void init_input_device();
void configure_input_device();
void process_input_device();
OSStatus InitNeeds();
void SetKeyboardMouseActivation(Boolean active);
void poll_input_device();
void kill_inputs();
void enable_inputs();
void disable_inputs();
void take_mouse_keyboard_as_input();
void untake_mouse_keyboard_as_input();
int game_fire_button();
void ZexGetMouse (Point *the_point);
short isPressed(unsigned short k );	//k is keycode
void init_ip_globs();


