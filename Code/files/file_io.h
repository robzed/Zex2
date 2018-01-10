// Zex 2 - 3D Space Trading Game with in-cockpit view 
// Copyright (C) 1998-2018  Stuart Ball and Robert Probin
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.



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
