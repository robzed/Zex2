#define DC_NOM 28 //volts DC
#define AC_NOM 400 //volts AC
void apply_control_inputs();
void run_ORCS();


short check_key(unsigned short k);
void ship_init();
void do_player_fire();
void reset_ship();
void reset_ship_with_carry_over();
short fire_button();
void do_player_laser();
void do_player_missile();
void kill_zex();


int get_pylon();
void select_next_pylon();
void reset_screen_from_status();
void clear_down_ship_on_eject();
void do_ejection();
void do_player_rear_laser();

void end_game_right_here_right_now(int EOLCode, char * the_reason_string);


float get_ship_mass_in_kgs(void);

extern int selectGalacticMap;
extern int selectSolarMap;

#define ZEX2 1	//mainly debug


