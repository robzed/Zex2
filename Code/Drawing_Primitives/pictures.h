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

typedef struct {
UInt16 pwidth;
UInt16 pheight;
char *raw_data;
} LSRAW;


typedef struct {
UInt16 pwidth;
UInt16 pheight;
char *raw_data;	//rgba quads actually
} LSRAW_RGBA;

void  convert_rgb16text_to_rgba(char* source, int* picture_ptr);
void  convert_argb32text_to_rgba(char* source, int* picture_ptr);
void  convert_rgb32text_to_rgba(char* source, int* picture_ptr);

char* getbyte(char* source, unsigned char * where);


void print_pict(LSRAW *the_picture, UInt32 x, UInt32 y);
void print_crunched_pict(LSRAW *the_picture, UInt32 x, UInt32 y);
void print_crunched_pict_C(LSRAW *the_picture, UInt32 x, UInt32 y, UInt32 top, UInt32 bott);
void print_crunched_trans_pict(LSRAW *the_picture, UInt32 x, UInt32 y);
void print_crunched32_trans_pict(LSRAW *the_picture, UInt32 x, UInt32 y);
void print_crunched_translucent_pict(LSRAW *the_picture, UInt32 x, UInt32 y);

#if PORTABLE_FILESYSTEM


class ZexPicture
{

public:
    ZexPicture(unsigned long res_type, short resid);	// normal constructor
    ~ZexPicture();					// our destructor
    LSRAW* GetPictureRef() const;			// how the user gets a reference to the picture (const means doesn't modify class data)

private:
    LSRAW* pic_pointer;					// a pointer to the memory
        
    // prohibit copy and assignment - since these are invalid operations
    ZexPicture(const ZexPicture &);			// copy constructor (used on definition from another type)
    ZexPicture& operator= (const ZexPicture&);		// copy assignment
    // (since this class has a pointer, it needs a assignment and copy constructor ... see The C++ Programming Language Page 10.4.4.1)
};

//LSRAW* GetZexPicture (unsigned long res_type, short resid);
//void DisposeZexPicture(LSRAW *pic_pointer);
#else
Handle GetZexPicture (unsigned long res_type, short resid);
#endif

void decrunch_pict(LSRAW *the_picture, LSRAW *where);
void print_pict_scaled(LSRAW *the_picture, UInt32 x, UInt32 y, UInt32 sf, UInt32 trans_on);
void print_pict_magnified(LSRAW *the_picture, UInt32 x, UInt32 y, int y_correction, int x_correction, float magnification);

void print_pict_quick(LSRAW *the_picture, int x, int y);

void erase_zex_rect(int w, int h);	//erase to black (0xff)

void erase_zex_rect_colour(int w, int h, unsigned char colour_8);	//erase to a given colour

void erase_zex_rect_rgb(float red, float green, float blue);

void set_erase_colour(unsigned char colour_8);

void invert_y_pict(LSRAW *the_picture);