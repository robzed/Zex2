//texture_management.c
//SB 3/1/99
//manages Zex's textures for me cause I'm too lazy to do it myself.
/* $Log: texture_management.c,v $
/* Revision 1.0.0.1  2001/10/17 20:46:08  rob
/* First Imported.
/*
/* Revision 1.4  2001/09/10 21:40:52  stu
/* 100901
More Sim, AP broken.
/*
/* Revision 1.3  2001/05/28 01:45:31  stu
/* 280501
/*
/* Revision 1.2  2001/04/01 18:25:53  stu
/* *** empty log message ***
/*
/* Revision 1.1.1.1  2001/01/01 21:13:39  rob
/* First Imported.
/*
/* Revision 1.0.0.1  2000/08/21 22:10:02  stu
/* First Imported.
/*
 */

#include "platform.h"

#include <Files.h>

#include "3d_structures_def.h"
#include "texture_management.ext"
#include "error_handler.ext"
#include "pictures.ext"
#include "file_io.ext"
#include "load_game.ext"
#include "options.h"
#include "ogl.ext"
#include    "fpoly4.ext"
#include "genpoly.int"
#include "fdistort.ext"
#include "fgouraud.ext"
#include "dlp.ext"
#include "zex_misc.ext"

#define MAX_TEXTURES 100
texture_slot the_textures [MAX_TEXTURES];	//our array of texture ptrs and ID's

int textures_index = 0;	//index into our array of textures
static GLuint texture_name = 0;



//returns an ogl name for a texture
int get_unique_texture_name()
{
texture_name++;
return texture_name;
}


//load all textures from the textures folder/resource
int load_textures()
{
FSSpec texture_folder_FSSpec;
FSSpec the_texture_FSSpec;

extern FSSpec Zex_FSSpec;
CInfoPBRec pb;
DirInfo *dpb=(DirInfo*) &pb;
HFileInfo *fpb=(HFileInfo*) &pb;	//ptrs to the union (Finfo and DirInfo) held in CInfoPBRec
int folder_DirID;
Str255 the_file_name;
int return_value;
int get_file_err = 0;
short file_index=1;
unsigned char temp_str[256];

        texture_name=1;
//get the objects' folder
	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,DirStr,&texture_folder_FSSpec);
//get zd3's dirctory ID
     dpb->ioVRefNum = texture_folder_FSSpec.vRefNum;
     dpb->ioDrDirID = texture_folder_FSSpec.parID;
     dpb->ioNamePtr = texture_folder_FSSpec.name;
     dpb->ioFDirIndex=0;	//query
     return_value=PBGetCatInfo(&pb,0);
     folder_DirID=dpb->ioDrDirID;

return_value=0;

//find sub folder
//      fpb->ioVRefNum = texture_folder_FSSpec.vRefNum;
     fpb->ioDirID = folder_DirID;
     fpb->ioNamePtr = "\pT";
     fpb->ioFDirIndex=0;	//query the file name

     return_value=PBGetCatInfo(&pb,0);
     if (return_value) 
     {
     report_error ("load_textures couldn't find the folder named:","\pT",return_value);
     return return_value;
     }
     folder_DirID=dpb->ioDrDirID;

return_value=0;
while (get_file_err==0)
 {
      fpb->ioVRefNum = texture_folder_FSSpec.vRefNum;
      fpb->ioDirID= folder_DirID;
//     fpb->ioDirID = folder_DirID;
     fpb->ioNamePtr = the_file_name;
     fpb->ioFDirIndex=file_index;	//gimmee the file name
     get_file_err=PBGetCatInfo(&pb,0);
     if(get_file_err) return return_value;
//now load it
//for x platform requirements instead of checking the Mac file type I have to check the extension
     PascalToC(fpb->ioNamePtr,temp_str);
     if (check_file_extension(temp_str,(unsigned char *)"raw"))	
     {
      FSMakeFSSpec (fpb->ioVRefNum, folder_DirID,the_file_name,&the_texture_FSSpec);
      load_texture(the_texture_FSSpec);	//in texture_management.c funnily enough    
      texture_name++;
     }
     else
     if (check_file_extension(temp_str,(unsigned char *)"r32"))	
     {
      FSMakeFSSpec (fpb->ioVRefNum, folder_DirID,the_file_name,&the_texture_FSSpec);
      load_texture32(the_texture_FSSpec);	//in texture_management.c funnily enough    
      texture_name++;
     }
     
     file_index++;	 //darn't ferget this un Stu (like you did...)
 }
 
}



//load a texture given the fsspec of the file containing the texture in RAW format

//ogl version which gets an OGL name for each texture
void load_texture(FSSpec the_texture_file)
{
unsigned long insert_id;
Handle temp_handle;
Handle tempb_handle;
LSRAW * the_raw;
LSRAW_RGBA * the_raw_rgba;
Ptr raw_data;
Ptr raw_rgba_data;
int err;
int draw_code;
extern int OGL_active;

//make ID from first 4 chars as a UInt32
insert_id=(the_texture_file.name[1]<<24) + (the_texture_file.name[2]<<16) +
(the_texture_file.name[3]<<8) + the_texture_file.name[4]; 
if (the_texture_file.name[3]=='&') draw_code=1;
else draw_code=0;

//load file
temp_handle=read_file(the_texture_file);
if (temp_handle==0) report_error ("Texture Management: Load_Texture: Zero Handle returned from read_file. Out of Memory.","\p",8);
HLock(temp_handle);	//bit important this...
the_raw = (LSRAW*)*temp_handle;	//dereference
//now convert from 8 bit clut to rgba
tempb_handle=NewHandleClear((128*128*4)+8);
if (tempb_handle==0) report_error ("Texture Management: Load_Texture: Zero Handle returned from read_file. Out of Memory.","\p",8);
HLock(tempb_handle);	//bit important this...
the_raw_rgba = (LSRAW_RGBA*)*tempb_handle;	//dereference

raw_data=(char *)&the_raw->raw_data;
raw_rgba_data=(char *)&the_raw_rgba->raw_data;


//check for OGL 32 bit textures
//if (the_texture_file.name[1]=='!')
//convert_to_rgba_from_rgbp(raw_data,raw_rgba_data,draw_code);
//else
convert_to_rgba(raw_data,raw_rgba_data,draw_code);

raw_data=(char *)&the_raw->raw_data;
raw_rgba_data=(char *)&the_raw_rgba->raw_data;


the_textures[textures_index].the_raw_data=raw_data;	//8 bit data for static screens
the_textures[textures_index].the_raw_rgba_data=raw_rgba_data;	//rgba data for OGL
the_textures[textures_index].the_handle=temp_handle;
the_textures[textures_index].the_rgb_handle=tempb_handle;
the_textures[textures_index].id=insert_id;

glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
glPixelStorei(GL_PACK_ALIGNMENT, 1); 
//glGenTextures(1,&texture_name);
the_textures[textures_index].gl_text_name=texture_name;


// DisposeHandle(temp_handle);	  //delete our temp mem - ogl has the texture now

textures_index++;
if (textures_index>MAX_TEXTURES) report_error ("Texture_Management:Load_Texture: Too many textures.","\p",7);
}




void load_texture32(FSSpec the_texture_file)
{
unsigned long insert_id;
Handle temp_handle;
Handle tempb_handle;
LSRAW * the_raw;
LSRAW_RGBA * the_raw_rgba;
Ptr raw_data;
Ptr raw_rgba_data;
int err;
int draw_code;
extern int OGL_active;

//make ID from first 4 chars as a UInt32
insert_id=(the_texture_file.name[1]<<24) + (the_texture_file.name[2]<<16) +
(the_texture_file.name[3]<<8) + the_texture_file.name[4]; 
if (the_texture_file.name[3]=='&') draw_code=1;
else draw_code=0;

//load file
temp_handle=read_file(the_texture_file);
if (temp_handle==0) report_error ("Texture Management: Load_Texture: Zero Handle returned from read_file. Out of Memory.","\p",8);
HLock(temp_handle);	//bit important this...
the_raw = (LSRAW*)*temp_handle;	//dereference
//now convert from 8 bit clut to rgba
tempb_handle=NewHandleClear((128*128*4)+8);
if (tempb_handle==0) report_error ("Texture Management: Load_Texture: Zero Handle returned from read_file. Out of Memory.","\p",8);
HLock(tempb_handle);	//bit important this...
the_raw_rgba = (LSRAW_RGBA*)*tempb_handle;	//dereference

raw_data=(char *)&the_raw->raw_data;
raw_rgba_data=(char *)&the_raw_rgba->raw_data;


//check for OGL 32 bit textures
//if (the_texture_file.name[1]=='!')
//convert_to_rgba_from_rgbp(raw_data,raw_rgba_data,draw_code);
//else
convert_to_rgba_from_rgb(raw_data,raw_rgba_data,draw_code);

raw_data=(char *)&the_raw->raw_data;
raw_rgba_data=(char *)&the_raw_rgba->raw_data;


the_textures[textures_index].the_raw_data=raw_data;	//8 bit data for static screens
the_textures[textures_index].the_raw_rgba_data=raw_rgba_data;	//rgba data for OGL
the_textures[textures_index].the_handle=temp_handle;
the_textures[textures_index].the_rgb_handle=tempb_handle;
the_textures[textures_index].id=insert_id;

glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
glPixelStorei(GL_PACK_ALIGNMENT, 1); 
//glGenTextures(1,&texture_name);
the_textures[textures_index].gl_text_name=texture_name;


// DisposeHandle(temp_handle);	  //delete our temp mem - ogl has the texture now

textures_index++;
if (textures_index>MAX_TEXTURES) report_error ("Texture_Management:Load_Texture: Too many textures.","\p",7);
}


//Load a texture from a 3dmf file
int load_direct_texture32(int* the_texture)
{
unsigned long insert_id;
Handle temp_handle;
Handle tempb_handle;
LSRAW * the_raw;
LSRAW_RGBA * the_raw_rgba;
Ptr raw_data;
Ptr raw_rgba_data;
int err;
int draw_code;
int OGL_name;

extern int OGL_active;

raw_rgba_data=(char *)the_texture;

the_textures[textures_index].the_raw_rgba_data=raw_rgba_data;	//rgba data for OGL
//the_textures[textures_index].the_raw_rgba_data=the_textures[4].the_raw_rgba_data;	//rgba data for OGL
the_textures[textures_index].the_handle=0;
the_textures[textures_index].the_rgb_handle=0;

the_textures[textures_index].id=0;

glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
glPixelStorei(GL_PACK_ALIGNMENT, 1); 
//glGenTextures(1,&texture_name);
OGL_name=get_unique_texture_name();
the_textures[textures_index].gl_text_name=OGL_name;


// DisposeHandle(temp_handle);	  //delete our temp mem - ogl has the texture now

textures_index++;
if (textures_index>MAX_TEXTURES) report_error ("Texture_Management:Load_Texture: Too many textures.","\p",7);

return OGL_name;
}







#if OGL==1

void load_ogl_textures()
{
int i,err;

  for (i=0;i<textures_index;i++)
  {
  
  glBindTexture(GL_TEXTURE_2D, the_textures[i].gl_text_name);	//we supply the name...
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 128);
    glPixelStorei(GL_PACK_ROW_LENGTH, 128);

     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
     glPixelStorei(GL_PACK_ALIGNMENT, 1); 

glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (float)GL_LINEAR);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (float)GL_LINEAR);

//  if(the_textures[i].gl_text_name==0x1e)
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA,
//  GL_UNSIGNED_BYTE, the_textures[4].the_raw_rgba_data);
//  else
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA,
  GL_UNSIGNED_BYTE, the_textures[i].the_raw_rgba_data);
 
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   err=aglGetError();
   if (err!=0) report_error ("Texture_Management:load_ogl_textures: AGL error.","\p",err);
  }
 
}
#endif

//converts 256 colour raw to rgba quad format
//draw code=0=normal
//1=255 is transparent and all others alpha is 50%
//2=255 is transparent
void convert_to_rgba(char * raw_data, char * raw_rgba_data, int draw_code)
{
int line_count, pixel_count;
unsigned char the_pixel;
int red,green,blue;

for (line_count=0; line_count<128; line_count++)
  {
   for (pixel_count=0; pixel_count<128; pixel_count++)
     {
     the_pixel=*raw_data;
     raw_data++;
     convert_to_rgb(the_pixel, &red, &green, &blue);
     *raw_rgba_data=red;
     raw_rgba_data++;
     *raw_rgba_data=green;
     raw_rgba_data++;
     *raw_rgba_data=blue;
     raw_rgba_data++;
     if (draw_code==0)
     {
      *raw_rgba_data=255;	//opaque
     }
     else     
     if (draw_code==2)
     {
      if (the_pixel==255)
      *raw_rgba_data=0;	//transparent
      else
      *raw_rgba_data=255;	//opaque
      
     }
     else
     if (draw_code==1)
       {
        if (the_pixel==255)
        *raw_rgba_data=0;	//transparent
        else
        *raw_rgba_data=150;	//translucent
       }
     
     raw_rgba_data++;

     }
  }
}

void convert_to_rgba_from_rgb(char * raw_data, char * raw_rgba_data, int draw_code)
{
int line_count, pixel_count;
unsigned char the_pixel;
int red,green,blue;

for (line_count=0; line_count<128; line_count++)
  {
   for (pixel_count=0; pixel_count<128; pixel_count++)
     {
     int colour;
     
     colour=0;
     
     the_pixel=*raw_data;
     colour+=the_pixel;
     raw_data++;
     *raw_rgba_data=the_pixel;
     raw_rgba_data++;

     the_pixel=*raw_data;
     colour+=the_pixel;
     raw_data++;
     *raw_rgba_data=the_pixel;
     raw_rgba_data++;

     the_pixel=*raw_data;
     colour+=the_pixel;
     raw_data++;
     *raw_rgba_data=the_pixel;
     raw_rgba_data++;
     
     if (draw_code==0)
     {
      *raw_rgba_data=255;	//opaque
     }
     else     
     if (draw_code==2)
     {
      if (colour<100)
      *raw_rgba_data=0;	//transparent
      else
      *raw_rgba_data=255;	//opaque
      
     }
     else
     if (draw_code==1)
       {
        if (colour<100)
        *raw_rgba_data=0;	//transparent
        else
        *raw_rgba_data=230;	//translucent
       }
     
     raw_rgba_data++;

     }
  }
}


void convert_to_rgba_wh(char * raw_data, char * raw_rgba_data, int draw_code,int w, int h)
{
int line_count, pixel_count;
unsigned char the_pixel;
int red,green,blue;

for (line_count=0; line_count<h; line_count++)
  {
   for (pixel_count=0; pixel_count<w; pixel_count++)
     {
     the_pixel=*raw_data;
     raw_data++;
     convert_to_rgb(the_pixel, &red, &green, &blue);
     *raw_rgba_data=red;
     raw_rgba_data++;
     *raw_rgba_data=green;
     raw_rgba_data++;
     *raw_rgba_data=blue;
     raw_rgba_data++;
     if (draw_code==0)
     {
      *raw_rgba_data=255;	//opaque
     }
     else     
     if (draw_code==2)
     {
      if (the_pixel==255)
      *raw_rgba_data=0;	//transparent
      else
      *raw_rgba_data=255;	//opaque
      
     }
     else
     if (draw_code==1)
       {
        if (the_pixel==255)
        *raw_rgba_data=0;	//transparent
        else
        *raw_rgba_data=128;	//translucent
       }
     
     raw_rgba_data++;

     }
  }
}


//converts 256 colour raw to rgba quad format
//colour 255 is transparent
void convert_to_rgba_trans(char * raw_data, char * raw_rgba_data)
{
int line_count, pixel_count;
unsigned char the_pixel;
int red,green,blue;

for (line_count=0; line_count<128; line_count++)
  {
   for (pixel_count=0; pixel_count<128; pixel_count++)
     {
     the_pixel=*raw_data;
     raw_data++;
     convert_to_rgb(the_pixel, &red, &green, &blue);
     
     *raw_rgba_data=red;
     raw_rgba_data++;
     *raw_rgba_data=green;
     raw_rgba_data++;
     *raw_rgba_data=blue;
     raw_rgba_data++;

     if (the_pixel==255)
     *raw_rgba_data=0;	//alpha
     else
     *raw_rgba_data=255;	//alpha
     raw_rgba_data++;

     }
  }
}

//unloads all the textures
void unload_textures()
{
while (textures_index>=0)
  {
  textures_index--;
  
  if(the_textures[textures_index].the_handle!=0)
  DisposeHandle (the_textures[textures_index].the_handle);

#if OGL==1
  if(the_textures[textures_index].the_rgb_handle!=0)
  DisposeHandle (the_textures[textures_index].the_rgb_handle);
#endif
   
  }
#if OGL==1
 
if (OGL_active==1)
 {
 GLuint the_name;
 int i;
 
 for (i=0;i<textures_index;i++)
  {
   the_name=the_textures[i].gl_text_name;
   glDeleteTextures(1,&the_name);
  }
  
 }	//end of OGL active
#endif

textures_index=0;	//reset
}

//given an id, returns the ptr to the raw data OR 0
texture_slot* find_texture_ptr(unsigned long wanted_texture_id)
{
int i=0;
while (i<textures_index)
  {
  if (the_textures[i].id==wanted_texture_id) 
   {
   return &the_textures[i];

   }
  i++;
  }
return 0;
}



//memory for dynamic textures for planets etc



dynamic_texture dynamic_textures_block [MAX_DYN_TEXTURES];

void init_dynamic_textures()
{
int i;
for (i=0;i<MAX_DYN_TEXTURES;i++) 
 {
 dynamic_textures_block[i].in_use=0;
 dynamic_textures_block[i].name=-1;	//for ogl
 }
}




int kill_dynamic_texture(dynamic_texture * the_tex_ptr)
{
int i;
GLuint the_name;

for (i=0;i<MAX_DYN_TEXTURES;i++) 
  {
        if (&dynamic_textures_block[i]==the_tex_ptr) 
         {
#if OGL==1
        
         //tell ogl to ditch the texture
         the_name=dynamic_textures_block[i].name;
         glDeleteTextures(1,&the_name);
#endif
         dynamic_textures_block[i].in_use=0;
         dynamic_textures_block[i].name=-1;
         return 0;
         }
  }

return -1;
}

//returns either the ptr to storage or 0
dynamic_texture * get_memory_for_dyn_tex()
{
int i;
for (i=0;i<MAX_DYN_TEXTURES;i++) 
  {
        if (dynamic_textures_block[i].in_use==0) 
        {
          dynamic_textures_block[i].in_use=1;
          dynamic_textures_block[i].name=get_unique_texture_name();	//for ogl
          return &dynamic_textures_block[i];
        }
  }

return 0;

}

//011199 SB
void load_ogl_dynamic_textures()
{
int i;
for (i=0;i<MAX_DYN_TEXTURES;i++) 
  {
        if (dynamic_textures_block[i].in_use!=0)	//found one in use 
        {
           make_ogl_dynamic_texture(&dynamic_textures_block[i]);
        }
  }

}

void make_ogl_dynamic_texture(dynamic_texture* dyn_tex_block_ptr)
{
Handle temp_rgbaH;
LSRAW * the_raw;
LSRAW_RGBA * the_raw_rgba;
Ptr raw_data;
Ptr raw_rgba_data;
int err;

   err=aglGetError();
   if (err!=0) report_error ("Texture_Management:make_ogl_dynamic_texture(1): AGL error.","\p",err);

//from the 8 bit data make rgba data and bind it to name
temp_rgbaH=NewHandle(128*128*4);



if (temp_rgbaH==0)
   {
          add_to_text_display("Zex Exec> WARNING: OGL: no memory for RGBA dyn_texture",1);
          dyn_tex_block_ptr->name=0;	//not a valid name
   }
else
   {
     HLock(temp_rgbaH);
    


     raw_rgba_data = *temp_rgbaH;	//dereference

     raw_data=(char *)&dyn_tex_block_ptr->tex_data;
//     draw_code=dyn_tex_block_ptr->draw_code;

     convert_to_rgba_trans(raw_data,raw_rgba_data);

     raw_rgba_data = *temp_rgbaH;	//dereference


     glBindTexture(GL_TEXTURE_2D, dyn_tex_block_ptr->name);	//we supply the name...

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 128);
    glPixelStorei(GL_PACK_ROW_LENGTH, 128);

     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
     glPixelStorei(GL_PACK_ALIGNMENT, 1); 

     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA,
     GL_UNSIGNED_BYTE, raw_rgba_data);
 
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   err=aglGetError();
   if (err!=0) report_error ("Texture_Management:make_ogl_dynamic_texture(2): AGL error.","\p",err);


     
     DisposeHandle(temp_rgbaH);
   }
   
}