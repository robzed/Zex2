// fgourand.ext

void setup_fgouraud(void);
void gouraud3(char *saddr, int swidth, dcoord *point_list, int r, int g, int b, int transparent);

int convert_to_palette(int r, int g, int b);
void convert_to_rgb(int colour, int *r, int *g, int *b);

