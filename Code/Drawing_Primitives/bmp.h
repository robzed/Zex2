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
