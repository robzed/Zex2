// Robs distort routine, usually used for afine texture mapping.
////
////
//// All int's must be 32 bit.
////
////
////
////
//// Version 1.02 - RP - Wed 6/1/99 - Fixed slightly off horizontal distorting
//// Version 1.03 - RP - Mon 11/1/99 - Created light values in distort at request of Stu. Thanks Stu. (Welcome).
////
////
///* $Log: fdistort_mask.c,v $
///* Revision 1.0.0.1  2001/10/17 20:46:07  rob
///* First Imported.
///*
///* Revision 1.1.1.1  2001/01/01 21:13:39  rob
///* First Imported.
///*
///* Revision 1.1  2000/09/04 20:05:00  rob
///* Hopefully fixed little black line problem with software renderer.
///*
///* Revision 1.0.0.1  2000/08/21 22:09:32  stu
///* First Imported.
///*
// */
//
//#include "platform.h"
//
//
//#include "f_light.int"
//#include "fpoly4.ext"
//#include "genpoly.int"
//#include "fdistort.ext"
//
//#define WIDTH_FIXED_AT_128 1
//
//
////typedef struct {
////	    	    int linex; 
////	        FLOAT_TYPE bmpx; 
////	        FLOAT_TYPE bmpy;
////
////	    	    } border_type;	  	    
//
//border_type tex_startspace[1000];	   // 1000 of line start, bmpstartx, bmpstarty
//border_type tex_endspace[1000];		  // 1000 of line end, bmpendx, bmpendy
//
//
//#if LIGHT_SHADED_TEXTURES
//void scanoneline_mask(char *screen_line_address, int line_startx, int line_endx,
//	    char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width, char *light_colour_address);
//void scanoneline_t_mask(char *screen_line_address, int line_startx, int line_endx,
//	    char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width, char *light_colour_address);
//#else
//void scanoneline_mask(char *screen_line_address, int line_startx, int line_endx,
//	    char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width);
//void scanoneline_t_mask(char *screen_line_address, int line_startx, int line_endx,
//	    char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width);
//#endif
//
//
//#if LIGHT_SHADED_TEXTURES 
//void distort3_mask(char *saddr, int swidth, dcoord *point_list, char *texture_addr, int texture_width, int light_value, int transparent)
//#else
//void distort3_mask(char *saddr, int swidth, dcoord *point_list, char *texture_addr, int texture_width, int transparent)
//#endif
//{
//int x1,x2,y1,y2;
//border_type *starttemp;
//border_type *endtemp;
////int count;
//int ymin,ymax;
//int xmin,xmax,xminu,xmaxu,xminv,xmaxv;
//#if LIGHT_SHADED_TEXTURES
//char *light_address;
//#endif
//
//// first connection
//x1=point_list->x; y1=point_list->y; 
//ymin=y1; ymax=y1;
//xmin=x1; xmax=x1; xminu=point_list->u; xmaxu=point_list->u; xminv=point_list->v; xmaxv=point_list->v;
//point_list++; x2=point_list->x; y2=point_list->y;
//  
//  if(y1==y2)       // horizontal line
//    {		    
//        if(x2<xmin) { xmin=x2; xminu=(point_list->u); xminv=(point_list->v); }
//        if(x2>xmax) { xmax=x2; xmaxu=(point_list->u); xmaxv=(point_list->v); }
//    }          	/* ignore connection - but store for single height case*/
//      
//tex_draw2(tex_startspace,tex_endspace,x1,y1,x2,y2,(point_list-1)->u,(point_list-1)->v,point_list->u,point_list->v);
//
//// second connection    
//x1=x2; y1=y2;
//if(y1<ymin) { ymin=y1; }  if(y1>ymax) { ymax=y1; } 
//point_list++; x2=point_list->x; y2=point_list->y; 
//  
//  if(y1==y2)       // horizontal line
//    {		    
//        if(x2<xmin) { xmin=x2; xminu=(point_list->u); xminv=(point_list->v); }
//        if(x2>xmax) { xmax=x2; xmaxu=(point_list->u); xmaxv=(point_list->v); }
//    }          	/* ignore connection - but store for single height case*/
//
//tex_draw2(tex_startspace,tex_endspace,x1,y1,x2,y2,(point_list-1)->u,(point_list-1)->v,point_list->u,point_list->v);
//    
//    
//// third connection
//x1=x2; y1=y2; 
//if(y1<ymin) { ymin=y1; }  if(y1>ymax) { ymax=y1; } 
//point_list-=2; x2=point_list->x; y2=point_list->y;
//  
//  if(y1==y2)       // horizontal line
//    {		    
//        if(x2<xmin) { xmin=x2; xminu=(point_list->u); xminv=(point_list->v); }
//        if(x2>xmax) { xmax=x2; xmaxu=(point_list->u); xmaxv=(point_list->v); }
//    }          	/* ignore connection - but store for single height case*/
//
//tex_draw2(tex_startspace,tex_endspace,x1,y1,x2,y2,(point_list-1)->u,(point_list-1)->v,point_list->u,point_list->v);
//
//
//#if LIGHT_SHADED_TEXTURES
//{
//int temp;
//temp=(light_value>>(8+LIGHT256_SHIFT));
//temp=(NUM_LIGHT_LEVELS/2);
//light_address=light_table+(light_value>>(8+LIGHT256_SHIFT))+(NUM_LIGHT_LEVELS/2);
//}
//#endif
//
//if(!transparent)
//{
//// now draw the individual scan lines.
//
// starttemp=tex_startspace + ymin;
// endtemp=tex_endspace + ymin;
// saddr+=(ymin*swidth);
//
// if(ymin==ymax)
//   {
//#if LIGHT_SHADED_TEXTURES
//  scanoneline_mask(saddr, xmin,xmax, texture_addr,xminu,xminv,xmaxu,xmaxv,texture_width,light_address);
//#else
//  scanoneline_mask(saddr, xmin,xmax, texture_addr,xminu,xminv,xmaxu,xmaxv,texture_width);
//#endif
//   }
//
// while(ymin<=ymax)
//  { // scanoneline(s,startx,  endx,    bmp_address,  bmp_startx,  bmp_starty,   bmp_endx,      bmp_endy,   bmp_width)
//#if LIGHT_SHADED_TEXTURES
//  scanoneline_mask(saddr, starttemp->linex,endtemp->linex, texture_addr, starttemp->bmpx,starttemp->bmpy, endtemp->bmpx,endtemp->bmpy, texture_width,light_address);
//#else
//  scanoneline_mask(saddr, starttemp->linex,endtemp->linex, texture_addr, starttemp->bmpx,starttemp->bmpy, endtemp->bmpx,endtemp->bmpy, texture_width);
//#endif
//  saddr+=swidth;
//  
//  starttemp++;
//  endtemp++;
//  
//  ymin++;
//  }
// 
//}
//else // if tranparent
//{
//// now draw the individual scan lines.
//
// starttemp=tex_startspace + ymin;
// endtemp=tex_endspace + ymin;
// saddr+=(ymin*swidth);
//
// if(ymin==ymax)
//   {
//#if LIGHT_SHADED_TEXTURES
//  scanoneline_t_mask(saddr, xmin,xmax, texture_addr,xminu,xminv,xmaxu,xmaxv, texture_width,light_address);
//#else
//  scanoneline_t_mask(saddr, xmin,xmax, texture_addr,xminu,xminv,xmaxu,xmaxv, texture_width);
//#endif
//   }
//
// while(ymin<=ymax)
//  { // scanoneline(s,startx,  endx,    bmp_address,  bmp_startx,  bmp_starty,   bmp_endx,      bmp_endy,   bmp_width)
//#if LIGHT_SHADED_TEXTURES
//  scanoneline_t_mask(saddr, starttemp->linex,endtemp->linex, texture_addr, starttemp->bmpx,starttemp->bmpy, endtemp->bmpx,endtemp->bmpy, texture_width,light_address);
//#else
//  scanoneline_t_mask(saddr, starttemp->linex,endtemp->linex, texture_addr, starttemp->bmpx,starttemp->bmpy, endtemp->bmpx,endtemp->bmpy, texture_width);
//#endif
//  saddr+=swidth;
//  
//  starttemp++;
//  endtemp++;
//  
//  ymin++;
//  }
// 
//}
//
//
//}  // end of distort3()
//
//
//
//#define MASK_COLOUR 255
//
//
//#if LIGHT_SHADED_TEXTURES
//void scanoneline_mask(char *screen_line_address, int line_startx, int line_endx,
//	    char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width, char *light_colour_address)
//#else
//void scanoneline_mask(char *screen_line_address, int line_startx, int line_endx,
//	    char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width)
//#endif
//{
//int width;
//FLOAT_TYPE bmp_x_displacement,bmp_y_displacement;
//FLOAT_TYPE bmp_x_increment,bmp_y_increment;
//int temp;
//unsigned char chartemp;
//
//// lets find how many steps across, and where the address of the screen is
//width=line_endx-line_startx+1;
//screen_line_address+=line_startx;
//
//// we need to step diagonally across bmp. We do so in "width" number of steps.
//bmp_y_increment=bmp_endy-bmp_starty;
//bmp_y_increment/=width;
//// test //bmp_y_increment*=bmp_width;
//
//bmp_x_increment=bmp_endx-bmp_startx; 
//bmp_x_increment/=width;
// 
//// calculate the start point on the bitmap
//bmp_x_displacement=bmp_startx;
//bmp_y_displacement=bmp_starty; //test  // *bmp_width;
//
//// now the actual loop that does the drawing.
//while(width>0)
//  {
//#if WIDTH_FIXED_AT_128
//  temp=(F_TO_I(bmp_y_displacement) << 7);
//#else
//  temp=(F_TO_I(bmp_y_displacement) * bmp_width);
//#endif
////  temp=(F_TO_I(bmp_y_displacement) * bmp_width);   /// transfering bmp_width to the other side of this F_TO_I gives us problems - can we rely
//  	  	  	  	  	  	  	  	  	  	  	  	  	  // on the fact that bmp width will be 128????
//  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  
//  	    	    	    	    	    	    	    	    	    	    	    // also, if we do get this optimisation, perhaps we should save the original for
//  	  	  	  	  	  	  	  	  	  	  	  // 1/z texture mapping   
//
//  	    	    chartemp=*( bmp_address + F_TO_I(bmp_x_displacement) + temp);
//  
//  #if LIGHT_SHADED_TEXTURES
//  	  if(chartemp!=MASK_COLOUR)
//  	  	  {
//  	    	    *(  (  ((unsigned char) (chartemp))<<LIGHT_SHIFT  )+light_colour_address  );
//  	    	    //                        |------------------------------------------------|
//  	    	    //      |-------------| |---------------------------------------------------|
//  	    	    //     |---------------------------------------------------------------------|
//  	    	    //  |---------------------------------------------------------------------------------------|
//  	    	    *(screen_line_address) = chartemp;
//  	          }
//
//  	  	  
//  	            	    	    
//  #else
//    	    if(chartemp!=MASK_COLOUR)
//  	          {
//    	      	      *(screen_line_address) = chartemp;			
//    	  	  }
//
//
//  #endif
//  
//  screen_line_address++;
//  bmp_x_displacement+=bmp_x_increment;
//  bmp_y_displacement+=bmp_y_increment;
//  width--;
//  }
//
//}
//
//
//#if LIGHT_SHADED_TEXTURES
//void scanoneline_t_mask(char *screen_line_address, int line_startx, int line_endx,
//	    char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width, char *light_colour_address)
//#else
//void scanoneline_t_mask(char *screen_line_address, int line_startx, int line_endx,
//	    char *bmp_address, FLOAT_TYPE bmp_startx, FLOAT_TYPE bmp_starty, FLOAT_TYPE bmp_endx, FLOAT_TYPE bmp_endy, int bmp_width)
//#endif
//{
//int width;
//FLOAT_TYPE bmp_x_displacement,bmp_y_displacement;
//FLOAT_TYPE bmp_x_increment,bmp_y_increment;
//int temp;
//unsigned char chartemp, chartemp2;
//
//// lets find how many steps across, and where the address of the screen is
//width=line_endx-line_startx+1;
//screen_line_address+=line_startx;
//
//// we need to step diagonally across bmp. We do so in "width" number of steps.
//bmp_y_increment=bmp_endy-bmp_starty;
//bmp_y_increment/=width;
//// test //bmp_y_increment*=bmp_width;
//
//bmp_x_increment=bmp_endx-bmp_startx; 
//bmp_x_increment/=width;
// 
//// calculate the start point on the bitmap
//bmp_x_displacement=bmp_startx;
//bmp_y_displacement=bmp_starty; //test  // *bmp_width;
//
//// now the actual loop that does the drawing.
//while(width>0)
//  {
//#if WIDTH_FIXED_AT_128
//  temp=(F_TO_I(bmp_y_displacement) << 7);
//#else
//  temp=(F_TO_I(bmp_y_displacement) * bmp_width);
//#endif
////  temp=(F_TO_I(bmp_y_displacement) * bmp_width);   /// transfering bmp_width to the other side of this F_TO_I gives us problems - can we rely
//  	  	  	  	  	  	  	  	  	  	  	  	  	  // on the fact that bmp width will be 128????
//  	  	  	  	  
//   	  	  chartemp=*( bmp_address + F_TO_I(bmp_x_displacement) + temp);
//	  	        							
//  	  	  	  	  	  	  	  	  	  	  	  	  	  // also, if we do get this optimisation, perhaps we should save the original for
//  	  	  	  	  	  	  	  	  	  	  	  	  	  // 1/z texture mapping 
//  
//  	  	  if(chartemp!=MASK_COLOUR)
//  	  	  {
//  chartemp2 = transparency_table[((*(unsigned char *)screen_line_address)<<8) + *(unsigned char* )
// 
//  #if LIGHT_SHADED_TEXTURES
//  
//  	    	    (  (  ((unsigned char) (chartemp))<<LIGHT_SHIFT  )+light_colour_address  )
//  	       //                        |------------------------------------------------|
//  	    	   //      |-------------| |---------------------------------------------------|
//  	    	   //     |---------------------------------------------------------------------|
//  	    	   //  |---------------------------------------------------------------------------------------|
//  #else
//  	    	    chartemp
//  #endif
//	  	    ];
//	  	  
//	  	  
//	  	  *(screen_line_address) = chartemp2;
//	  	  
//	  	  }
//	  	  
//
//
//  screen_line_address++;    
//  bmp_x_displacement+=bmp_x_increment;
//  bmp_y_displacement+=bmp_y_increment;
//  width--;
//  }
//
//}
