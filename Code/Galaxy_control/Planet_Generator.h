void gen_planet_name(unsigned int seed, unsigned char * output_string, int type);

void planet_gen(int seed, int is_inhabited, unsigned char *scratch_address, unsigned char *output_address);
//void galaxy_gen(int seed, char *output_address);
void galaxy_gen(int seed, unsigned char *output_address, int squish_flag);

void init_name_generator(void);	//must be called before generating names


// new planetgen routines
void flat_planet_gen(int seed, int POBs_id, unsigned char *scratch_address, unsigned char *output_address);
