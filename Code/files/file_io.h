#ifndef __FILES__
#include <files.h>
#endif

#ifndef __MACMEMORY__
#include <macmemory.h>
#endif


//prototypes
Handle read_file (FSSpec the_file);
void get_zex_fsspec();
void read_prefs();
void write_prefs();
Handle ZGetResource(unsigned int Type, short ID);
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
int check_file_extension (unsigned char * filename, unsigned char* extension);