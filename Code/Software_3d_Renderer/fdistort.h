//fdistort.ext

typedef struct {
		int x;
		int y;
		int z;
		FLOAT_TYPE u;
		FLOAT_TYPE v;
		FLOAT_TYPE l;	//light value
		} dcoord;



//void distort3(char *saddr, int swidth, dcoord *point_list, char *texture_addr, int texture_width);
void distort3(char *saddr, int swidth, dcoord *point_list, char *texture_addr, int texture_width, int light_value, int transparent);
void distort_window_clipper3(dcoord *input_list, dcoord *temp_list, dcoord *output_list,int *polys_out);

void distort3_mask(char *saddr, int swidth, dcoord *point_list, char *texture_addr, int texture_width, int light_value, int transparent);

void distortm(char *saddr, int swidth, dcoord *point_list, char *texture_addr, int texture_width, int light_value, int transparent, int num_connections);
void distort_window_clipper_m(dcoord *input_list, dcoord *temp_list, dcoord *output_list,int *points_out);

