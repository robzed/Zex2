//FPOLY4.ext
typedef struct {
		int x;
		int y;
		} coord;


void polygon(int num_lines, coord *point_list, int colour, char *screen_address, int screen_width);
void init_polygon(void);
void window_clipper(int num_lines, coord *input_list, coord *temp_list, coord *output_list, int *output_lines);
void set_poly_clipping (int x_min, int x_max, int y_min, int y_max);	//sb 1801298
void pline(int x1, int x2, char *line_screen_address, int colour);
int single_line_clip (int *x1, int *y1, int *x2, int *y2);
