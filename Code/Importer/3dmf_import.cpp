// *************************************MODULE HEADER*********************************
// * FILENAME           : 3dmf_import.cpp
// * MODULE TITLE       : Untitled 1
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 21:45:08 on 17th May 2001
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************
/* CVS bits
$Log: 3dmf_import.cpp,v $
Revision 1.5  2003/09/28 17:30:07  robp
Changed files from .c to .cpp and removed spaces out of a couple of filenames.

Revision 1.4  2003/09/28 10:36:02  robp
Signed/Unsigned comparison fixes from last night, plus collision fix.

Revision 1.3  2003/09/27 20:41:38  robp
Fixed float->int warnings.

Revision 1.2  2003/09/19 20:54:52  robp
Removed items causing warnings.

Revision 1.1.1.1  2003/09/05 22:35:51  stu_c
First Imported.

Revision 1.11  2002/09/08 02:14:31  stu_c
Precompiled header adjustments

Revision 1.10  2002/05/31 23:53:09  stu
Removed all warnings from the code

Revision 1.9  2002/04/01 19:24:03  stu
Vertices with an invalid ogl name are set to the first texture in the file.

Revision 1.8  2002/03/09 15:08:52  stu
Now support RIGHT_HANDED (which is what we need) or left handed importing - set RIGHT_HANDED in 3d_structures to 1 for correct operation.

Revision 1.7  2002/02/24 15:42:22  stu
More error checking - this time on the number of connections not exceeding MAX_CONNECTIONS

Revision 1.6  2002/02/24 04:54:20  stu
kQ3AttributeTypeSurfaceUV is now ignored by importer so we can import meshworks 3dmf's

Revision 1.5  2001/12/29 01:11:35  stu
Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.

Revision 1.4  2001/11/05 19:59:18  stu
More OSX work

Revision 1.3  2001/11/02 19:32:22  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.2  2001/10/27 22:39:12  stu
zplatform included

Revision 1.1  2001/10/22 21:28:25  rob
Carbon warning changes

Revision 1.0.0.1  2001/10/17 20:46:06  rob
First Imported.

Revision 1.6  2001/09/10 21:42:56  stu
100901
More Sim, AP broken.

Revision 1.5  2001/07/08 20:23:55  stu
0.66 release 080701

Revision 1.4  2001/06/23 23:06:34  stu
0.66

Revision 1.3  2001/06/13 17:09:41  rob
No physical change: Allows debugging of errors.

Revision 1.2  2001/05/28 21:40:14  stu
Major commit 280501

Revision 1.1  2001/05/28 01:40:22  stu
Checkin 280501

*/

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #include "3d_structures_def.h"
  #include "Error_handler.h"
  #include "processor.h"
#endif

#include "3dmf_import.h"
#include "MeshToZex.h"
#include "insert_object.h"
#include "pictures.h"
#include "texture_management.h"


// ***********************************************************************************
// * CONSTANTS 
// *

#define TAB 9
#define CR 13
#define LF 10
#define SPACE 32
#define COMMA ','

#define MAXCONNECTIONS 1800
#define MAXVERTICES 1800
#define MAXUVS 1800
#define MAX_TEXTURES 8
#define MAX_COLOURS 32
#define MAX_NUM_TRIMESHES 32

// ***********************************************************************************
// * TYPE DEFINITIONS
// *

typedef struct {
float u, v;
} uv;

typedef struct {
unsigned long connections_index;
unsigned long number_of_connections;
unsigned long vertex_index;
unsigned long number_of_vertices;
unsigned long uv_index;
unsigned long number_of_uvs;
unsigned long texture_name;
fRGB diffuse_colour;
} trimesh_struct;

// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *
module_private _3D vertices[MAXVERTICES];	//can cope with up to this many vertices in any one object
module_private dmf_textured_connections connections[MAXCONNECTIONS]; //and this many connections
module_private uv uv_array[MAXUVS];			//and this many uv's
module_private int starting_vertex_per_texture;

//colours
module_private fRGB base_object_colour;
module_private fRGB connections_colours[MAX_COLOURS];
module_private int connections_colours_indices[MAX_COLOURS];
module_private int connections_colours_index;
//vertices
module_private int number_of_vertices;
module_private int connections_index;
module_private int uv_index;

module_private int gourad_shading;

module_private int texture_names [MAX_TEXTURES];
//module_private float transps [MAX_TEXTURES];	//transparencies for each texture. 0=transparent,1=opaque

module_private int numTriangles;
module_private int numTrianglesAttributeTypes;
module_private int numEdges;
module_private int numEdgeAttributeTypes;
module_private int numPoints;
module_private int numPointAttributeTypes;
module_private int current_texture_count;
module_private int TMnumTriangles;	//per trimesh
module_private int TMnumPoints;		//per trimesh
module_private int there_was_a_texture;
module_private int container_count;
module_private int did_colour;

extern unsigned char* object_loader_filename;

module_private trimesh_struct the_trimeshes [MAX_NUM_TRIMESHES];
module_private int trimesh_count;
// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

module_private /* char * */ void decode_3dmf(char *source, int object_number, unsigned long the_id, int colldet);

module_private char * decode_container(char *source);
module_private char * decode_trimesh(char *source);
module_private char * decode_attributearray(char* source);
//module_private char * decode_texture(char* source);
module_private char * decode_diffusecolor(char *source);
module_private char * decode_transparency_colour(char *source);
module_private char * decode_mipmap_texture(char *source);
module_private char * decode_pixmap_texture(char* source);

module_private char * find_end_of_keyword(char* source);
module_private int find_word (char* the_word,char** source);
module_private int find_next_word(char** source, char* the_word);
module_private int onezerostrcmp(char* s, char* t);
module_private int eat_white_space(char** where);
module_private int find_eol(char** source);
module_private float str_to_num(char* string);
//module_private void skip_word (char* the_word,char** source);
module_private char * find_end_of_attrib(char* source);
void post_process_trimeshes(void);
fRGB find_colour_for_vertex(int m);
void init_trimesh_diffuse_colours(void);
int is_this_connection_textured(int c);

//this routine reads a 3dmf text file into zex.
//pg1259 of qd3d154.pdf
//The minimum requirements for Zex are:
//We must have the connections.
//We must have the vertexes
//We must have at least a base colour for each poly (if not, defaults to white).
//
//We ignore the surface and vertex normals as we calculate this internally at object load time.
//A Texture Mapped poly also needs the image(s) along with UV coords for each vertex.



//a 3dmf text file has the following format:

//3DMetafile ( 1 6 Normal tableofcontents0> )
//   ^         ^ ^   ^                   ^
//Identifier  major  file type       ptr to toc
//              minor version
                
//	  trimesh2:             //label
//	  Container (           //object start
//	  	  TriMesh (	//must be trimesh! pg 1348

//	  	  	  12 1 0 0 8 2 // numTriangles numTriangleAttributeTypes numEdges numEdgeAttributeTypes numPoints numVertexAttributeTypes
//	  	  	  0 1 2 
//	  	  	  3 0 2 
//	  	  	  2 1 4 
//	  	  	  5 0 3 
//	  	  	  6 1 0 
//	  	  	  7 3 2 
//	  	  	  4 7 2 
//	  	  	  7 5 3 
//	  	  	  5 6 0 
//	  	  	  1 6 4 
//	  	  	  6 5 7 
//	  	  	  4 6 7  //connection list - numTriangles entries

//			-25 25 25 -25 25 -25 -25 -25 -25 
//	  	  	  -25 -25 25 25 -25 -25 25 25 25 
//	  	  	  25 25 -25 25 -25 25   //points

//	  	  	  -25 -25 -25 25 25 25 False ) //object bounding box

//attribute types from d3dsets.h
enum TQ3AttributeTypes {
                                                                /* Data Type          */
    kQ3AttributeTypeNone        = 0,                            /* ---------          */
    kQ3AttributeTypeSurfaceUV   = 1,                            /* TQ3Param2D          */
    kQ3AttributeTypeShadingUV   = 2,                            /* TQ3Param2D           */
    kQ3AttributeTypeNormal      = 3,                            /* TQ3Vector3D           */
    kQ3AttributeTypeAmbientCoefficient = 4,                     /* float            */
    kQ3AttributeTypeDiffuseColor = 5,                           /* TQ3ColorRGB          */
    kQ3AttributeTypeSpecularColor = 6,                          /* TQ3ColorRGB          */
    kQ3AttributeTypeSpecularControl = 7,                        /* float            */
    kQ3AttributeTypeTransparencyColor = 8,                      /* TQ3ColorRGB          */
    kQ3AttributeTypeSurfaceTangent = 9,                         /* TQ3Tangent2D          */
    kQ3AttributeTypeHighlightState = 10,                        /* TQ3Switch           */
    kQ3AttributeTypeSurfaceShader = 11,                         /* TQ3SurfaceShaderObject */
    kQ3AttributeTypeNumTypes    = 12
};

//		AttributeArray (

//	  	  	  3 = Normals
//            0 = reserved.
//            0 = 0=triangle attribs, 1= edge attribs, 2=vertex attribs
//            0 = PositionInArray (zero based)
//            0 = flag
//            3 = ???

//	  	  	  -1 0 0 //triangle normals - 12 of them for 12 triangles
//	  	  	  -1 0 0 
//	  	  	  0 0 -1 
//	  	  	  0 0 1 
//	  	  	  0 1 0 
//	  	  	  0 -1 0 
//	  	  	  0 -1 0 
//	  	  	  0 0 1 
//	  	  	  0 1 0 
//	  	  	  0 0 -1 
//	  	  	  1 0 0 
//	  	  	  1 0 0 
//	  	  )
//	  	  AttributeArray (
//	  	  	  3 0 2 0 0 = vertex normals - 8 of them for 8 points
//	  	  	  -0.270598 0.9238796 0.270598 
//	  	  	  -0.2886751 0.4082483 -0.8660254 
//	  	  	  -0.270598 -0.9238796 -0.270598 
//	  	  	  -0.2886751 -0.4082483 0.8660254 
//	  	  	  0.7071068 -0.2705981 -0.6532815 
//	  	  	  0.7071068 0.5 0.5 
//	  	  	  0.9238796 0.270598 -0.270598 
//	  	  	  0.9238796 -0.270598 0.270598 
//	  	  )
//	  	  AttributeArray (
//	  	  	  2 0 2 1 0 //2=shading uv of vertices - 8 of them for 8 vertices
//	  	  	  0.007812493 0.9921879 
//	  	  	  0.005208871 0.005208341 
//	  	  	  0.002604163 0.9947918 
//	  	  	  0 0 
//	  	  	  0.9921874 0.9921874 
//	  	  	  0.9921878 0.9908857 
//	  	  	  0.9973959 0.002604165 
//	  	  	  1 0 
//	  	  )
//	  	  attributeset3:
//	  	  Container (
//	  	  	  AttributeSet ( )
//	  	  	  textureshader4:
//	  	  	  Container (
//	  	  	  	  TextureShader ( )
//	  	  	  	  mipmaptexture5:
//	  	  	  	  MipmapTexture (
//	  	  	  	  	  False RGB16 BigEndian BigEndian 128 128 256 0 
//	  	  	  	  	  0x71836943610369426D83654361036102
//	  	  	  	  	  0x6D8371A369436102696371A371C36D83
//	  	  	  	  	  0x694365436D83694371A371A369436102
//	  	  	  	  	  0x6D6371C47646654265236D6371A371E4
//	  	  	  	  	  0x71C471A36D836D636943610260E36542
//                    etc


//In all error conditions Zex will throw a fatal error terminating the program. The error will need correcting!
void import_3dmf (char *the_file_data, int object_number, unsigned long the_id, int colldet)
{
char the_line[512];	//max line size in a mesh file
char the_word[512];	//max length of a word from a line
char *source;		//walk through the file with this
int line_len=0, line_pos=0, word_len=0;		//loop control
int n;



init_trimesh_diffuse_colours();
gourad_shading=1;	//Lambert can set this to 0
trimesh_count=0;
container_count=0;
number_of_vertices=0;
connections_index=0;
uv_index=0;
starting_vertex_per_texture=0;
connections_colours_index=1;
connections_colours_indices[0]=0;
//init colour indices
for(n=1;n<MAX_COLOURS;n++) connections_colours_indices[n]=-1;


for (n=0; n<MAXUVS; n++)
{
uv_array[n].u=0xDEAD;
uv_array[n].v=0xDEAD;

}

numTriangles=numTrianglesAttributeTypes=numEdges=0;	//filled in trimesh
numEdgeAttributeTypes=numPoints=numPointAttributeTypes=0;

TMnumTriangles=0;
TMnumPoints=0;

current_texture_count=0;	//inc'd once per pix/mipmap texture
texture_names[0]=-1;	//array of texture names

source=the_file_data;	//source is incremented through the file

base_object_colour.red=base_object_colour.green=base_object_colour.blue=1.0;	//set default object colour to white


//Step 1 - look for a line starting with 3DMetafile
line_len=get_line(source,the_line);
if (line_len!=-1)
{
    source+=line_len+1;	//point to next line (accounting for CR)
}


line_pos=0;
word_len=get_word(the_word,&the_line[line_pos]);	//should be 3DMetafile
line_pos+=word_len;

if(Zstrcmp(the_word,"3DMetafile\0"))
{
   report_error("import_3dmf: 3DMetafile expected.",object_loader_filename,667);
   return;
}
//skip open brackets
word_len=get_word(the_word,&the_line[line_pos]);	
line_pos+=word_len;

//check major and minor versions
word_len=get_word(the_word,&the_line[line_pos]);	//should be 1
line_pos+=word_len;
if (the_word[0]!='1')    report_error("import_3dmf: Version 1.6 expected",object_loader_filename,668);

word_len=get_word(the_word,&the_line[line_pos]);	//should be 6
line_pos+=word_len;
if (the_word[0]!='6')    report_error("import_3dmf: Version 1.6 expected.",object_loader_filename,669);

//check file type is Normal
word_len=get_word(the_word,&the_line[line_pos]);	//should be Normal
line_pos+=word_len;
if (Zstrcmp(the_word,"Normal\0"))    report_error("import_3dmf: Only Normal file types allowed..",object_loader_filename,670);

//check for toc offset 0
word_len=get_word(the_word,&the_line[line_pos]);	//should be tableofcontents0
line_pos+=word_len;
if (Zstrcmp(the_word,"tableofcontents0>\0"))    report_error("import_3dmf: Only TOC offset of 0 allowed.",object_loader_filename,671);

//OK, we've validated the file, now decode it... source is pointing to the next line

 decode_3dmf(source, object_number, the_id, colldet);

}





//This routine takes a validated 3dmf file ptr and decodes it to internal zex structure
//within a file we can have more than one trimesh, so after decoding the parent container we need to keep going until we have all the
//data

void decode_3dmf(char *source, int object_number, unsigned long the_id, int colldet)
{
char the_line[512];	//max line size in a mesh file
char the_word[512];	//max length of a word from a line
int line_len=0, line_pos=0, word_len=0, error;	
int open_bracks=0;
int n;
float bounds_sphere;

//first set all connections to opaque!
for(n=0; n<MAXCONNECTIONS;n++)
{
       connections[n].transparency=0; 
       connections[n].OGL_name=-1;
}


//Step 1 - look for a line starting with 3DMetafile
line_len=get_line(source,the_line);
if (line_len!=-1)
{
    source+=line_len+1;	//point to next line (accounting for CR)
}

line_pos=0;
word_len=get_word(the_word,&the_line[line_pos]);
line_pos+=word_len;

open_bracks=0;

 for(EVER)
 {
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) break;	//EOF
   
   if(!Zstrcmp(the_word,"(\0")) 
      {
        open_bracks++;
      }
   else   
   if(!Zstrcmp(the_word,")\0")) 
      {
        open_bracks--;
        if(open_bracks==0) break;
      }
   else
   if(!Zstrcmp(the_word,"BeginGroup\0")) 
      {
        source=find_end_of_keyword(source); //we skip begingroup completely - not interested in groups at the moment. We may need to be later.
      }
   else
   if(!Zstrcmp(the_word,"EndGroup\0")) 
      {
        find_word(")",&source); //find the close brackets of EndGroup
      }
   else
   if(!Zstrcmp(the_word,"Container\0")) 
      {
        source=decode_container(source);
      }

  }	//end of EVER

//Now we should have all the data we need to build the object...
if(trimesh_count==0) report_error("import_3dmf: No Trimesh in object???",object_loader_filename,1500);

post_process_trimeshes();	//fill in counts in trimesh array


//set vertex colours; run trhough connections_colours_indices setting the connection colours
 
 for( n=0; n<connections_index;n++)
 {
      connections[n].colour=find_colour_for_vertex(n); 
      connections[n].type=GOURAD+DMF;	//set all to gourad initially
   
 }	//eoN

//calculate bounds_sphere
 bounds_sphere=0;
 
 for(n=0; n<connections_index; n++)
 {
 float coord;
 
 coord=vertices[connections[n].vertex].x;
 coord=abs_nobranch(STATIC_CAST_TO_LONG(coord));
 if (coord > bounds_sphere) bounds_sphere=coord; 

 coord=vertices[connections[n].vertex].y;
 coord=abs_nobranch(STATIC_CAST_TO_LONG(coord));
 if (coord > bounds_sphere) bounds_sphere=coord; 

 coord=vertices[connections[n].vertex].z;
 coord=abs_nobranch(STATIC_CAST_TO_LONG(coord));
 if (coord > bounds_sphere) bounds_sphere=coord; 

 
 }


//now find the uv's for each connection

 
 if(uv_index!=0) //texture map?
 {
  if(texture_names[0]==-1) report_error("import_3dmf: Corrupt 3dmf file? Got UV's but no texture???",object_loader_filename,673);


  for(n=0; n<connections_index; n++)
  {
  short the_vertex;
  float t;
  //int local_texture_names[MAX_TEXTURES];
  //dmf_textured_connections local_connections[MAXCONNECTIONS]; //and this many connections

/* Debug code  
  for(m=0;m<MAX_TEXTURES;m++) local_texture_names[m]=texture_names[m];	//copy for debug
  for(m=0;m<MAXCONNECTIONS;m++) local_connections[m]=connections[m];	//copy for debug

  if(object_loader_filename[1]=='p')
  {
   int a;
   a=32;
  } 
*/
  
     if(is_this_connection_textured(n)!=0)
     {


/* ***HACK*** */
//if something has referenced a previous texture we set it to index 0 - this is to fix the planets which have two materials
 	  if(connections[n].OGL_name>=current_texture_count)
	  {
	   connections[n].OGL_name=0;
//	   report_error("import_3dmf: OGL name index > number of textures read. Connection=",object_loader_filename,connections_index);
	  }
/* End Hack */


     connections[n].OGL_name=texture_names[connections[n].OGL_name]; 
     connections[n].type=GOURADTEXTURE+DMF;	//set all to textured
//for each connection, find and store it's u and v
     
     the_vertex=connections[n].vertex;
     t=uv_array[the_vertex].u;
     connections[n].u=STATIC_CAST_TO_INT((128*t));
          
//     connections[n].v=128*(uv_array[connections[n].vertex].v);

     the_vertex=connections[n].vertex;
     t=uv_array[the_vertex].v;
     connections[n].v=STATIC_CAST_TO_INT(128-(128*t));
     } 
     else
     {
     connections[n].u=0;
     connections[n].v=0;
     }   
  }
  
  
//check for wrapped uv's
//we do this in triangles through the connection list
  for(n=0; n<connections_index; n+=3)
  {
   float min_u, min_v;
   //float u0,v0;
   
   min_u=connections[n].u;  
   
   if(connections[n+1].u<min_u) min_u=connections[n+1].u;
   if(connections[n+2].u<min_u) min_u=connections[n+2].u;
   
   if(min_u>127)
   {
    connections[n].u-=STATIC_CAST_TO_INT(min_u);
    connections[n+1].u-=STATIC_CAST_TO_INT(min_u);
    connections[n+2].u-=STATIC_CAST_TO_INT(min_u);
   }


   min_v=connections[n].v;  
   
   if(connections[n+1].v<min_v) min_v=connections[n+1].v;
   if(connections[n+2].v<min_v) min_v=connections[n+2].v;
   
   if(min_v>127)
   {
   connections[n].v-=STATIC_CAST_TO_INT(min_v);
   connections[n+1].v-=STATIC_CAST_TO_INT(min_v);
   connections[n+2].v-=STATIC_CAST_TO_INT(min_v);
   }
          
  }


 }
 
 dmf_insert_object(object_number, vertices, connections, number_of_vertices, connections_index,
                   the_id, colldet, bounds_sphere);
}


void init_trimesh_diffuse_colours(void)	//init colours to white
{
int n;	//n should be a C intrinsic surely!

	for (n=0;n<MAX_NUM_TRIMESHES;n++)
	{
	 the_trimeshes[n].diffuse_colour.red=1.0;
	 the_trimeshes[n].diffuse_colour.green=1.0;
	 the_trimeshes[n].diffuse_colour.blue=1.0;
	}

}

int is_this_connection_textured(int c)
{
//int n;
int the_vertex;

the_vertex=connections[c].vertex;

if(uv_array[the_vertex].u==0xDEAD) return 0;
else
return -1;
}


//search trimesh arrays for colour
fRGB find_colour_for_vertex(int m)
{
int n;


	for (n=0;n<trimesh_count;n++)
	{
	 if (STATIC_CAST_TO_UNSIGNED(m)<(the_trimeshes[n].connections_index+the_trimeshes[n].number_of_connections)) return the_trimeshes[n].diffuse_colour;
	}
	
	
report_error("import_3dmf: Couldn't find trimesh colour!",object_loader_filename,1700);	
return the_trimeshes[n].diffuse_colour;; // never gets here
}



void post_process_trimeshes(void)
{
//typedef struct {
//unsigned long connections_index;
//unsigned long number_of_connections;
//unsigned long vertex_index;
//unsigned long number_of_vertices;
//unsigned long uv_index;
//unsigned long number_of_uvs;
//unsigned long texture_name;
//fRGB diffuse_colour;
//} trimesh_struct;

//fill in the number_ofs_field

if (trimesh_count==1)
{
 the_trimeshes[trimesh_count-1].number_of_connections=connections_index;
 the_trimeshes[trimesh_count-1].number_of_vertices=number_of_vertices;
 the_trimeshes[trimesh_count-1].number_of_uvs=uv_index;
 
}
else
{
int n;

  for(n=0;n<trimesh_count;n++)
  {
  	if(n==trimesh_count-1) //last trimesh?
  	{
          the_trimeshes[n].number_of_connections=connections_index-the_trimeshes[n].connections_index;
          the_trimeshes[n].number_of_vertices=number_of_vertices-the_trimeshes[n].vertex_index;
          the_trimeshes[n].number_of_uvs=uv_index-the_trimeshes[n].uv_index;
  	
  	}
  	else	//not last trimesh
  	{
          the_trimeshes[n].number_of_connections=the_trimeshes[n+1].connections_index-the_trimeshes[n].connections_index;
          the_trimeshes[n].number_of_vertices=the_trimeshes[n+1].vertex_index-the_trimeshes[n].vertex_index;
          the_trimeshes[n].number_of_uvs=the_trimeshes[n+1].uv_index-the_trimeshes[n].uv_index;
  		
  	}
  }
}
}


char* decode_container(char *source)
{
int error;
//int result;
int open_bracks=0;	//so we know when we get to the end of the container definition
char the_word[512];	//max length of a word from a line

container_count++;
there_was_a_texture=0;
did_colour=0;	//set in decode_diffuse_colour

 for(EVER)
 {
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in container.",object_loader_filename,673); return NULL; /* never gets to return */ }
   
   if(!Zstrcmp(the_word,"(\0")) 
      {
        open_bracks++;
      }
   else   
   if(!Zstrcmp(the_word,")\0")) 
      {
       open_bracks--;
       if(!open_bracks) 
        {
              if(there_was_a_texture==0 && container_count==1 && did_colour==1)
              {
                //update uv index as there was no texture
               uv_index+=number_of_vertices;	//no uv's for this trimesh
              }
        container_count--;
        return source;
        }
      }
   else
   if(!Zstrcmp(the_word,"TriMesh\0")) 
      {
       source=decode_trimesh(source);     
      }
   else
   if(!Zstrcmp(the_word,"AttributeArray\0")) 
      {
       source=decode_attributearray(source);     
      }
   else
   if(!Zstrcmp(the_word,"DiffuseColor\0")) 
      {
        source=decode_diffusecolor(source);	//set the module level object_colour var.      
      }
   else   
   if(!Zstrcmp(the_word,"Container\0")) 
      {
       source=decode_container(source);
      }
   else   
   if(!Zstrcmp(the_word,"MipmapTexture\0")) 
      {
       source=decode_mipmap_texture(source);
      }
   else   
   if(!Zstrcmp(the_word,"PixmapTexture\0")) 
      {
       source=decode_pixmap_texture(source);
      }
   else   
   if(!Zstrcmp(the_word,"TransparencyColor\0")) 
      {
       source=decode_transparency_colour(source);	//will be after the attribs
      }


 }
 

return source;
}




//inputs data to the global connections and vertices
char* decode_trimesh(char *source)
{
int error;
//int result;
int open_bracks=0;	//so we know when we get to the end of the trimesh definition
char the_word[512];	//max length of a word from a line
int phase=0;
int current_num_points;

int n;	//good old n
#define READTRIHEADER 1
#define READCONNECTIONLIST 2
#define READPOINTSLIST 4
#define NUMEDGEATTRIBUTES 8
#define NUMPOINTS 16
#define NUMPOINTATTRIBS 32


trimesh_count++;	

the_trimeshes[trimesh_count-1].connections_index=connections_index;	//connections start for this trimesh
the_trimeshes[trimesh_count-1].vertex_index=number_of_vertices;	//vertices start for this trimesh
the_trimeshes[trimesh_count-1].uv_index=uv_index;	//uv's start for this trimesh



current_num_points=numPoints;	//get current total number of points so we can add it on to these trimesh local connections

 for(EVER)
 {
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in container.",object_loader_filename,673);  }
   
   if(!Zstrcmp(the_word,"(\0")) 
      {
        open_bracks++;
      }
   else   
   if(!Zstrcmp(the_word,")\0")) 
      {
      open_bracks--;
      if(!open_bracks) return source;
      }
   else   
   if(!Zstrcmp(the_word,"Container\0")) 
      {
       source=decode_container(source);	//RECURSIVE call to our caller
      }
   else   //try for attributes
   if((!(phase&READTRIHEADER)) && (open_bracks==1))	//we're in the trimesh declaration, so first entry is number of triangles
   {

      phase|=READTRIHEADER;	//set that we've done the trimesh header
      TMnumTriangles=STATIC_CAST_TO_INT(str_to_num(the_word));	//convert from string to float
      numTriangles+=TMnumTriangles;
        
      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: Number of Triangle Attributes expected.",object_loader_filename,674); }
      numTrianglesAttributeTypes=STATIC_CAST_TO_INT(str_to_num(the_word));	//convert from string to float
      
      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: Number of edges expected.",object_loader_filename,675);  } 
      numEdges=STATIC_CAST_TO_INT(str_to_num(the_word));	//convert from string to float
  
      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: Number of Edge Attributes expected.",object_loader_filename,676);  }
      numEdgeAttributeTypes=STATIC_CAST_TO_INT(str_to_num(the_word));	//convert from string to float
  
      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: Number of vertices expected.",object_loader_filename,677);  }
      TMnumPoints=STATIC_CAST_TO_INT(str_to_num(the_word));	//convert from string to float
      numPoints+=TMnumPoints;  
      
      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: Number of Vertex Attributes expected.",object_loader_filename,678);  }
      numPointAttributeTypes+=STATIC_CAST_TO_INT(str_to_num(the_word));	//convert from string to float
      
      if(find_eol(&source))	//skip any comments
      { report_error("import_3dmf: Unexpected End Of File after trimesh header.",object_loader_filename,679);  }
 
 
     //get connections
     for(n=0;n<TMnumTriangles;n++)
     {
     //int connection;
     float connection_arrray[3];     
      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: Connection expected.",object_loader_filename,680); }
      connection_arrray[2]=str_to_num(the_word);	//convert from string to float
      connection_arrray[2]+=current_num_points;	//make global

      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: Connection expected.",object_loader_filename,680); }
      connection_arrray[1]=str_to_num(the_word);	//convert from string to float
      connection_arrray[1]+=current_num_points;

      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: Connection expected.",object_loader_filename,680); }
      connection_arrray[0]=str_to_num(the_word);	//convert from string to float
      connection_arrray[0]+=current_num_points;

      connections[connections_index].vertex=STATIC_CAST_TO_INT(connection_arrray[0]);
      connections[connections_index].OGL_name=current_texture_count;	//save texture id
      connections_index++;

      connections[connections_index].vertex=STATIC_CAST_TO_INT(connection_arrray[1]);
      connections[connections_index].OGL_name=current_texture_count;	//which is inc'd in pixmap/mipmap
      connections_index++;
      
      connections[connections_index].vertex=STATIC_CAST_TO_INT(connection_arrray[2]);
      connections[connections_index].OGL_name=current_texture_count;      
      connections_index++;
      
	  
	  if (connections_index>MAXCONNECTIONS) report_error("import_3dmf: Too many connections. ",object_loader_filename,TMnumTriangles); 
     }  
     
     //get vertices
     for(n=0;n<TMnumPoints;n++)
     {
      float vertex;
           
      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: Vertex component X expected.",object_loader_filename,681); }
      vertex=str_to_num(the_word);	//convert from string to float
      
	  #ifndef RIGHT_HANDED 
		#error "RIGHT_HANDED not defined!"
	  #endif
	  #if RIGHT_HANDED==1
 	    vertices[number_of_vertices].x=-vertex;
      #else
	    vertices[number_of_vertices].x=vertex;
	  #endif
      
	  error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: Vertex component Y expected.",object_loader_filename,682);  }
      vertex=str_to_num(the_word);	//convert from string to float
      vertices[number_of_vertices].y=vertex;

      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: Vertex component Z expected.",object_loader_filename,683);  }
      vertex=str_to_num(the_word);	//convert from string to float
      vertices[number_of_vertices].z=vertex;
      
      number_of_vertices++;
      
     }  
  
   }


 }

}

//needs source pointing to open bracks of attribarray
char* decode_attributearray(char* source)
{
int error,type,n;
char the_word[512];	//max length of a word from a line
//int u,v;
float uf,vf;

   
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in container.",object_loader_filename,700);  }

   if(Zstrcmp(the_word,"(\0")) 
   {
     report_error("import_3dmf: Open brackets expected.",object_loader_filename,701);
   }
   
   
//check out the type of attribs, we don't need the normals, just UV
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in container.",object_loader_filename,702);  }
   type=STATIC_CAST_TO_INT(str_to_num(the_word));	//convert from string to float
   
   if(type==kQ3AttributeTypeNormal)
   {
    source=find_end_of_attrib(source); //we skip begingroup completely - not interested in groups at the moment. We may need to be later.
   }
   else
   if(type==kQ3AttributeTypeSurfaceUV)	//we ignore these attributes
   {
    source=find_end_of_attrib(source); //we skip begingroup completely - not interested in groups at the moment. We may need to be later.
   }
   else
   
   if(type==kQ3AttributeTypeShadingUV)	//texture mapped
   {
   //these go into connections, with index uv_index
   //skip reserved
   there_was_a_texture=1;
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in container.",object_loader_filename,703);  }
   //get tri/edge/vertex selector
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in container.",object_loader_filename,704);  }
   type=STATIC_CAST_TO_INT(str_to_num(the_word));	//convert from string to float
   if(type!=2)  report_error("import_3dmf: Can only accept vertices with uv.",object_loader_filename,705);
   //skip position in array and flag
   if(find_eol(&source))	//skip any comments
   { report_error("import_3dmf: Unexpected End Of File in AttributeArray.",object_loader_filename,706);  }

   if(TMnumPoints==0) report_error("import_3dmf: UV attribute array but no points in trimesh.",object_loader_filename,1000);

   //if(TMnumPoints%3!=0) report_error("import_3dmf: UV attribute array length is not MOD 3.",object_loader_filename,TMnumPoints);
   for(n=0;n<TMnumPoints;n++)
   {
      //get the uv's
      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: Unexpected end of file at u in AttributeArray.",object_loader_filename,704);  }
      uf=str_to_num(the_word);	//convert from string to float
      uv_array[uv_index].u=uf;
      
      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: Unexpected end of file at v in AttributeArray.",object_loader_filename,704);  }
      vf=str_to_num(the_word);	//convert from string to float      
      uv_array[uv_index].v=vf;
      
      
      uv_index++;
   }  
   source=find_end_of_attrib(source); //skip to end of attributearray.

   }
   else 
   {
    report_error("import_3dmf: Unknown type in uv AttributeArray header..",object_loader_filename,705);
   }
   
	     
return source;
}




//needs source pointing to open bracks of mipmaptexture
char* decode_mipmap_texture(char* source)
{
int error;
char the_word[512];	//max length of a word from a line
int pix_format=0;
int* picture_ptr;

   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in mipmap texture.",object_loader_filename,800);  }
   if(Zstrcmp(the_word,"(\0"))  report_error("import_3dmf: Open brackets expected.",object_loader_filename,801);
//check for False - we can't handle mipmapped pixmaps
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in mipmap texture.",object_loader_filename,802);  }
   if(Zstrcmp(the_word,"False\0"))  report_error("import_3dmf: Can't handle mipmaps.",object_loader_filename,803);
//check for data format
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in mipmap texture.",object_loader_filename,802);  }
  
   if(!Zstrcmp(the_word,"RGB16\0")) pix_format=1;
   else
  if(!Zstrcmp(the_word,"RGB32\0")) pix_format=2;
   else 
   report_error("import_3dmf: Invalid data format in mipmap - accept RGB16 or RGB32.",object_loader_filename,804);
//check for BigEndian * 2
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in mipmap texture.",object_loader_filename,804);  }
   if(Zstrcmp(the_word,"BigEndian\0"))    report_error("import_3dmf: Invalid data format in mipmap - BigEndian only.",object_loader_filename,803);

   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Invalid 3dmf file.","\pUnexpected end of file in mipmap texture.",804);  }
   if(Zstrcmp(the_word,"BigEndian\0"))    report_error("import_3dmf: Invalid data format in mipmap - BigEndian only.",object_loader_filename,803);
//check width and height
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Invalid 3dmf file.","\pUnexpected end of file in mipmap texture.",804);  }
   if(Zstrcmp(the_word,"128\0"))    report_error("import_3dmf: Invalid data format in mipmap - 128 pixels wide only.",object_loader_filename,803);

   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Invalid 3dmf file.","\pUnexpected end of file in mipmap texture.",804);  }
   if(Zstrcmp(the_word,"128\0"))    report_error("import_3dmf: Invalid data format in mipmap - 128 pixels wide only.",object_loader_filename,803);
   //ok, now skip to EOL
   if(find_eol(&source))	//skip any comments
   { report_error("import_3dmf: Unexpected End Of File in mipmap.",object_loader_filename,805);  }
   
   picture_ptr=(int*) NewPtrClear((128*128*4)+16);
   
   if(pix_format==1)
   convert_rgb16text_to_rgba(source,picture_ptr);
   else
   if(pix_format==2)
   convert_rgb32text_to_rgba(source,picture_ptr);
   
   texture_names[current_texture_count]=load_direct_texture32(picture_ptr);	//save OGL name in array
   the_trimeshes[trimesh_count-1].texture_name=texture_names[current_texture_count];

   current_texture_count++;	//got a texture, so inc index for next texture

   if(current_texture_count>MAX_TEXTURES-1) report_error("import_3dmf: Too many textures in object.",object_loader_filename,806);
   find_word(")",&source); //skip to end to mipmap data.
   
   return source;

}

//needs source pointing to open bracks of mipmaptexture
char* decode_pixmap_texture(char* source)
{
int error;
char the_word[512];	//max length of a word from a line
int pix_format=0;
int* picture_ptr;

   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in pixmap texture.",object_loader_filename,800);  }
   if(Zstrcmp(the_word,"(\0"))  report_error("import_3dmf: Invalid 3dmf file.","\pOpen brackets expected.",801);
//check for width=128
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in pixmap texture.",object_loader_filename,802);  }
   if(Zstrcmp(the_word,"128\0"))  report_error("import_3dmf: Pixmap width must be 128.",object_loader_filename,803);
//check for h=128
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in pixmap texture.",object_loader_filename,802);  }
   if(Zstrcmp(the_word,"128\0"))  report_error("import_3dmf: Pixmap height must be 128.",object_loader_filename,803);

//ignore rowbytes
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in pixmap texture.",object_loader_filename,802);  }
//ignore bits per pixel
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in pixmap texture.",object_loader_filename,802);  }

//check for data format
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in pixmap texture.",object_loader_filename,802);  }
  
   if(!Zstrcmp(the_word,"ARGB32\0")) pix_format=1;
   else
   if(!Zstrcmp(the_word,"RGB32\0")) pix_format=2;
   else 
   if(!Zstrcmp(the_word,"RGB16\0")) pix_format=3;
   else 
   report_error("import_3dmf: Invalid data format in pixmap - accept ARGB32, RGB32 or RGB16.",object_loader_filename,804);
//check for BigEndian * 2
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in pixmap texture.",object_loader_filename,804);  }
   if(Zstrcmp(the_word,"BigEndian\0"))    report_error("import_3dmf: Invalid data format in pixmap - BigEndian only.",object_loader_filename,803);

   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in pixmap texture.",object_loader_filename,804);  }
   if(Zstrcmp(the_word,"BigEndian\0"))    report_error("import_3dmf: Invalid data format in pixmap - BigEndian only.",object_loader_filename,803);

   //ok, now skip to EOL
   if(find_eol(&source))	//skip any comments
   { report_error("import_3dmf: Unexpected End Of File in pixmap.",object_loader_filename,805);  }
   
   picture_ptr=(int*) NewPtrClear((128*128*4)+16);
   
   if(pix_format==1)
   convert_argb32text_to_rgba(source,picture_ptr);
   else
   if(pix_format==2)
   convert_rgb32text_to_rgba(source,picture_ptr);
   else
   if(pix_format==3)
   convert_rgb16text_to_rgba(source,picture_ptr);

//      report_error("import_3dmf: Invalid 3dmf file.","\pStu needs to fix the importer!",807);

   texture_names[current_texture_count]=load_direct_texture32(picture_ptr);
   current_texture_count++;
   if(current_texture_count>MAX_TEXTURES-1) report_error("import_3dmf: Too many textures in object.",object_loader_filename,806);

   find_word(")",&source); //skip to end to pixmap data.
   
   return source;

}


char* decode_diffusecolor(char *source)
{
int error;
//int result;
char the_word[512];	//max length of a word from a line
int open_bracks=0;	//so we know when we get to the end of the container definition
 
 did_colour=1;	//set global flag to let UV management know there is not a texture for this trimesh
 
 for(EVER)
 {
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in DiffuseColor.",object_loader_filename,691);  }
   
   if(!Zstrcmp(the_word,"(\0")) 
      {
        open_bracks++;
      }
   else   
   if(!Zstrcmp(the_word,")\0")) 
      {
      open_bracks--;
      if(!open_bracks) return source;
      }
   else
   if(open_bracks==1)	//in ( r g b) - assumes we have the red value in word
   {
      base_object_colour.red=str_to_num(the_word);	//convert from string to float
   	
      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: DiffuseColour green expected.",object_loader_filename,693); }
      base_object_colour.green=str_to_num(the_word);	//convert from string to float

      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: DiffuseColour blue expected.",object_loader_filename,694); }
      base_object_colour.blue=str_to_num(the_word);	//convert from string to float
      //now poke it into connections_colours[MAX_COLOURS]
      connections_colours[connections_colours_index-1]=base_object_colour;
      connections_colours_indices[connections_colours_index]=connections_index;
      
      connections_colours_index++;
      if(connections_colours_index>MAX_COLOURS) report_error("import_3dmf: Too Many Colours in Object.",object_loader_filename,1235);
      
      the_trimeshes[trimesh_count-1].diffuse_colour=base_object_colour;
   }   
 }	//end of EVER. Hmmm.


}


char* decode_transparency_colour(char *source)
{
int error;
//int result;
int n;
char the_word[512];	//max length of a word from a line
int open_bracks=0;	//so we know when we get to the end of the container definition
float red, green, blue, trans;
 
 red=green=blue=0;
 for(EVER)
 {
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file in TransparentColor.",object_loader_filename,691);  }
   
   if(!Zstrcmp(the_word,"(\0")) 
      {
        open_bracks++;
      }
   else   
   if(!Zstrcmp(the_word,")\0")) 
      {
      open_bracks--;
      if(!open_bracks) break;
      }
   else
   if(open_bracks==1)	//in ( r g b) - assumes we have the red value in word
   {
      red=str_to_num(the_word);	//convert from string to float
   	
      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: TransparentColor green expected..",object_loader_filename,1193); }
      green=str_to_num(the_word);	//convert from string to float

      error=find_next_word(&source,the_word);	//get next word in the word and update source
      if(error) { report_error("import_3dmf: TransparentColor blue expected..",object_loader_filename,1194); }
      blue=str_to_num(the_word);	//convert from string to float

   }   
 }	//end of EVER. Hmmm.
//now calculate the base transparency as an average.
the_trimeshes[trimesh_count-1].diffuse_colour.red=red;
the_trimeshes[trimesh_count-1].diffuse_colour.green=green;
the_trimeshes[trimesh_count-1].diffuse_colour.blue=blue;

trans=(red+green+blue)/3;
//now poke the connections
for(n=starting_vertex_per_texture; n<connections_index;n++)
{
       connections[n].transparency=trans; 
}

starting_vertex_per_texture=connections_index;	//for next trimesh

return source;

}



char* find_end_of_keyword(char* source)
{
int open_bracks=0;	//so we know when we get to the end of the container definition
int error;
char the_word[512];	//max length of a word from a line

 for(EVER)
 {
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file.",object_loader_filename,6955); return NULL; /* never gets here */ }
   
   if(!Zstrcmp(the_word,"(\0")) 
      {
        open_bracks++;
      }
   else   
   if(!Zstrcmp(the_word,")\0")) 
      {
       open_bracks--;
       if(!open_bracks) return source;
      }
 }	//end of EVER
}



char* find_end_of_attrib(char* source)
{
int error;
char the_word[512];	//max length of a word from a line

 for(EVER)
 {
   error=find_next_word(&source,the_word);	//get next word in the word and update source
   if(error) { report_error("import_3dmf: Unexpected end of file.",object_loader_filename,6956); return NULL; /* never gets here */ }
   
   if(!Zstrcmp(the_word,")\0")) 
   {
        return source;
   }
 }	//end of EVER
}






// ***************************************************
//MISC ROUTINES

float str_to_num(char* string)
{
//float dectof_result;
return      Zconvert_to_float(string);
}
//runs through file to eol
int find_eol(char** source)
{
char* source_ptr=*source;
//int error;

  //copy to white space
  for(EVER)
  {
  	if(*source_ptr==0) return !0;	//EOF
 
    if( (*source_ptr==CR) || (*source_ptr==LF) )
     {
       *source=source_ptr;
       return 0;
  	 }
    source_ptr++;
  }

}

//returns the next word in the file. returns 0=not eof
int find_next_word(char** source, char* the_word)
{
char* source_ptr=*source;
int error;

  for (EVER) //find start of nect word that isn't a comment
  {
        error =  eat_white_space(&source_ptr);
        if (error) return error; //eof etc
        
        if (*source_ptr=='#') //comment?
        {
            error = find_eol(&source_ptr);
            if (error) return error; //eof etc
        }
        else
        {
            break;
        }
  }

  //copy to white space
  for(EVER)
  {
    if(*source_ptr==0) return !0;	//EOF
 
    if((*source_ptr==SPACE) || (*source_ptr==TAB) || (*source_ptr==CR) || (*source_ptr==LF)) //end of word
     {
       *source=source_ptr;
       *the_word=0;
       return 0;
     }
    *the_word=*source_ptr;	//copy char and keep going
    source_ptr++; the_word++;
  }

}

//eats white space, returns !0 if EOF
int eat_white_space(char** where)
{
 char* source=*where;
   for(EVER)
   {
   if(*source==0) return !0;	//EOF?
   
   if((*source!=SPACE) && (*source!=TAB) && (*source!=CR) && (*source!=LF) )
     {
      *where=source; return 0;
     }
   source++;
   }
}





//finds the next occurence of a given word and updates source. returns 0=ok
int find_word (char* the_word,char** source)
{
char* source_ptr=*source;

//just walk the file looking for word. File terminates in 0
for(EVER)	//but not really obviously...
{
  if (*source_ptr==0) return -1;
  if(onezerostrcmp(the_word,source_ptr))
  {
    source_ptr++;
  }
  else break;
}

*source=source_ptr;	//return new position in file
return 0;
}




//expects only first param to be zero terminated. returns 0=match
int onezerostrcmp(char* s, char* t)
{
  for (EVER)
  {
   if (*s==0) return 0;
   if (*s!=*t) return -1;
   s++; t++;
  }
}