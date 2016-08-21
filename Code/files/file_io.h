

//prototypes
Handle read_file (FSSpec the_file);
void get_zex_fsspec();
void read_prefs();
void write_prefs();
#if PORTABLE_FILESYSTEM
void *ZGetResource(unsigned int Type, short ID, int *loaded_size);	// loaded_size can be NULL.
void ZReleaseResource(void *resource_ptr);
void *ZSetResourceSize(void *resource_ptr, int new_size);		// returns pointer to new location, or NULL if failed. Space up 
#else
Handle ZGetResource(unsigned int Type, short ID);
#endif
typedef struct {
int score;
char name[63];
} hiscore;

typedef struct {
int Change_res;
int Use_VBL;
int Use_SpeedLimit;
int Use_Mode320;
int Use_Interlace;
int sound_volume;
int music_volume;
int registered;
int version;

int camera_zoom;
int camera_mode;

int ticks_at_installation_time;
int random_number_at_installation_time;	//from ticks - show to user for registration
int key_from_random_number_at_installation_time;	//user sends this to get a registration kye
int registration_key;

hiscore the_scores[10];
} Zprefs;

extern FSSpec Zex_FSSpec;


void save_game();
int load_game();
OSErr WriteZexFile( FSSpec fileSpec, Ptr buffer, long bufferLen );
OSErr ReadZexFile( FSSpec fileSpec, Ptr buffer, long bufferLen );
int check_file_extension (char * filename, char* extension);
