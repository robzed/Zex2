void load_level (unsigned char* the_filename);
void interpret_file(Ptr the_level);
void show_level_text();
void GetBGRAW(FSSpec* the_file);
int make_data_fsspec(Str255 the_folder_name,Str255 the_filename, FSSpec* the_fsspec);	//make fsspec of file name in zd3 folder

int load_player_ship(double x_in_m, double y_in_m, double z_in_m, short galactic_id );
int load_player_vv(double x_in_m, double y_in_m, double z_in_m, short galactic_id );
int load_camera(double x_in_m, double y_in_m, double z_in_m, short galactic_id );





