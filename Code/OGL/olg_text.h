
int init_ogl_text();
void OGL_draw_monaco_bold(char* the_string);
void OGL_draw_helvetica(char* the_string);
void OGL_draw_helvetica_small(char* the_string);

void OGL_draw_helveticaP(unsigned char* the_string);	//passcal string version


void OGL_DrawStr(char *str,int the_len,int x, int y);
void ZDrawString(unsigned char * the_string);
void ZMoveTo(int x, int y);
void ZLineTo(int x, int y);
void ZRGBForeColor (const RGBColor * RGB_Colour);
void ZRGBBackColor (const RGBColor * RGB_Colour); //used exclusively in ZEraseRect
void ZEraseRect(Rect * the_rect);