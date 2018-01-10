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

//bmp.h
//sb/Lightsoft 8/12/98


typedef struct {
   UInt32  biSize;
   UInt32  biWidth;
   UInt32  biHeight;
   UInt16   biPlanes;
   UInt16   biBitCount;
   UInt32  biCompression;
   UInt32  biSizeImage;
   UInt32  biXPelsPerMeter;
   UInt32  biYPelsPerMeter;
   UInt32  biClrUsed;
   UInt32  biClrImportant;
} BITMAPINFOHEADER;

typedef struct  {
        UInt16   bfType;
        UInt32   bfSize;
        UInt16   bfReserved1;
        UInt16   bfReserved2;
        UInt32   bfOffBits;	
} BITMAPFILEHEADER;

typedef struct  {
   unsigned char    rgbBlue;
   unsigned char    rgbGreen;
   unsigned char    rgbRed;
   unsigned char    rgbReserved;
} RGBQUAD;

#if !ZEX_CORE_CARBON

typedef struct  { 
   BITMAPINFOHEADER    bmiHeader;
   RGBQUAD             bmiColors[1];
} BITMAPINFO;

typedef struct {
BITMAPFILEHEADER bm_fileheader;
BITMAPINFO bm_info;
char BMP_data[];
 } bmp_header;

void expand_bmp (bmp_header *bmp_src,char *dest);
#endif
