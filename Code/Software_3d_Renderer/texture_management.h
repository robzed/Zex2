//texture_management.ext

#define MAX_DYN_TEXTURES 90


typedef struct
{
Handle the_handle;	//so we can dispose of it
Handle the_rgb_handle;
char* the_raw_data;	//past the RAW header
char* the_raw_rgba_data;
unsigned long id;	//the ID (from first 4 chars of filename), ogl's name also
int gl_text_name;
} texture_slot;


int load_textures();
void load_texture(FSSpec the_texture_file);
void unload_textures();
texture_slot* find_texture_ptr(unsigned long wanted_texture_id);



void init_dynamic_textures();
int kill_dynamic_texture(dynamic_texture * the_tex_ptr);
dynamic_texture * get_memory_for_dyn_tex();
void load_ogl_textures();
int get_unique_texture_name();
void make_ogl_dynamic_texture(dynamic_texture* dyn_tex_block_ptr);
void load_ogl_dynamic_textures();
void load_texture32(FSSpec the_texture_file);
int load_direct_texture32(int* the_texture);

void convert_to_rgba(char * raw_data, char * raw_rgba_data, int draw_code);
void convert_to_rgba_trans(char * raw_data, char * raw_rgba_data);
void convert_to_rgba_from_rgb(char * raw_data, char * raw_rgba_data, int draw_code);


void convert_to_rgba_wh(char * raw_data, char * raw_rgba_data, int draw_code,int w, int h);

