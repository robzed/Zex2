//MeshToZex
//SB 14/11/98
//
//Note to Stu:
//Get Anvil to encrypt this source in some way so nobody can ever see
//what crap code you turn out some days.

//Updated 27/11/98 to accept a four character object ID
//
//NB's
//Mesh (great prog!) files use a -z axis (righthanded, a la original fant).
//Zex really likes a +z axis (a la Zex!), so all vertices' z points are negated during 
//import.
//This import routine is for Mesh 0.9
//
//Mesh files are text files. The ultimate in cross platform ability?
//

//Good Zex choones notes as noted when progging:
//Deep.s3m
//Blue Valley (Final part only?)
//Dubu99
//Hyperdrv
//12th Warrior (a cycle free choon?)

//Thought of the day: Have pride in your comments

//Update 2/1/99
//Change to handle mapping extents in material blocks
//Update 9/1/99
//Change to handle "smooth" polys (Gourad shaded)
/*
 * $Log: MeshToZex.c,v $
 * Revision 1.1.1.1  2003/09/05 22:35:52  stu_c
 * First Imported.
 *
 * Revision 1.8  2002/09/08 02:14:46  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.7  2002/05/31 23:53:45  stu
 * Removed all warnings from the code
 *
 * Revision 1.6  2002/03/09 15:09:44  stu
 * Can now import as RIGHT_HANDED or left.
 *
 * Revision 1.5  2001/12/29 01:11:47  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.4  2001/11/02 23:20:40  stu
 * Further OS X work. Enabled syscon logging.
 *
 * Revision 1.3  2001/11/02 19:32:25  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:29:21  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:12:52  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:06  rob
 * First Imported.
 *
 * Revision 1.3  2001/05/28 01:52:27  stu
 * 280501
 *
 * Revision 1.2  2001/01/14 17:17:29  stu
 * Gourad Fix
 *
 * Revision 1.1.1.1  2001/01/01 21:13:38  rob
 * First Imported.
 *
 * Revision 1.1  2000/10/08 21:45:20  stu
 * Engine conversion to 64 bit. Bitch of a job.
 *
 * Revision 1.0.0.1  2000/08/21 22:07:29  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <fp.h>
  #endif
  #include "3d_structures_def.h"
  #include "Error_handler.h"
#endif //pch


#include "MeshToZex.h"
#include "insert object.h"
#include "texture_management.h"
#include "3dmf_import.h"

Zex_Colour pcol[9];



int MeshToZexDyn (char *the_file_data, int object_number, unsigned long the_id,int colldet, int texture_id, int translucent)
{
char the_line[512];	//max line size in a mesh file
char the_word[512];	//max length of a word from a line
char *source;		//walk through the file with this
int line_len=0, line_pos=0, word_len=0;		//loop control
int vert_index;

decimal my_dec;
short ix;
short vp;
float dectof_result;
float vertex[3];
_3D vertices[1000];	//can cope with up to 1000 vertices in any one object
textured_connections connections[2000]; //and 2000 connections
int number_of_vertices=0;
int connections_index=0;
int the_connection;
int connection_count,material;	//used for points colours	

double minx=0;
double maxx=0;
double miny=0;
double maxy=0;
//double minz=0;
//double maxz=0;	//for bounding box creation
double max_radius;	 //greatest of all of them


//zex d9 - texture mapping
int smoothing=0;	//1 if gouraded (from material block)
int map_type=0; //must be 1 for tm zex else 0
int map_axis=0;
double map_center_x, map_center_y, map_center_z;
double map_extent_x, map_extent_y, map_extent_z;
double map_min_x=0,map_max_x,map_min_y=0,map_max_y;
double step_size_u=0,step_size_v=0;
double unknown_param;

float px,py;
int version;

//RGBColor ObjColor;	//the colour of the object from the first material line


connections_index=0;

source=the_file_data;	//source is incremented through the file

//check for 3dmetafile
get_line(source,the_line);
line_pos=0;
word_len=get_word(the_word,the_line);
if(!Zstrcmp(the_word,"3DMetafile\0"))
{
   import_3dmf (the_file_data, object_number, the_id, colldet);
   return 0;	// return no error
}
//get version 
line_pos+=word_len; //next word
word_len=get_word(the_word,&the_line[line_pos]); //version major
ix=0;
str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
dectof_result=Zconvert_to_float (the_word);
version=dectof_result*10;

line_pos+=word_len; //next word
word_len=get_word(the_word,&the_line[line_pos]); //version minor
ix=0;
str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
dectof_result=Zconvert_to_float (the_word);
version+=dectof_result;

line_pos+=word_len;

//Step 1 - look for a line starting with VERTICES
    the_word[0]=0;
    while (line_len!=-1 && (Zstrcmp(the_word,"VERTICES\0")))	//keep going till eof or material
      { //4
       line_len=get_line(source,the_line);
       if (line_len!=-1)
         { //5
          source+=line_len+1;	//point to next line (accounting for CR)
          get_word(the_word,the_line);

         } //5
      }      //end of edges - 4
//while (line_len!=-1)	  //keep going till eof - actually, if we get an eof it's an error!
// {	//2
  line_len=get_line(source,the_line);
  if (line_len!=-1)
  { //3
//    source+=line_len+1;	//point to next line (+1 for CR)
    number_of_vertices=0;

//¥¥¥¥¥Copy vertices until EDGES
//    get_word(the_word,the_line);
//    if (!Zstrcmp(the_word,"VERTICES\0"))
//    { //4
    //Get vertices
    //serial x  y  z
    the_word[0]=0;
    line_len=get_line(source,the_line);
    while (line_len!=-1 && (Zstrcmp(the_word,"EDGES\0")) )	 //keep going till eof or edges
      { //5
       if (line_len!=-1)
         { //6
         line_pos=0;
         word_len=0;
         //get words until cr
          vert_index=0;
          while (word_len!=-1)
           { //7
           word_len=get_word(the_word,&the_line[line_pos]);
           line_pos+=word_len;
           if (vert_index!=0)	//pos 0 is the vertex ID
           //convert to a single float
             { //8
             ix=0;
             str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
             dectof_result=Zconvert_to_float (the_word);
			 
			 #ifndef RIGHT_HANDED 
  			  #error "RIGHT_HANDED not defined!"
			 #endif
			 #if RIGHT_HANDED==0
			  if (vert_index==3) dectof_result=-dectof_result;	 //negate z
             #endif
			 
			 vertex[vert_index-1]=dectof_result;	//build x,y,z
             } //8
           vert_index++;

           } //7
           vertices[number_of_vertices].x=vertex[0];
           vertices[number_of_vertices].y=vertex[1];
           vertices[number_of_vertices].z=vertex[2];	//copy to array
//update min and max
           if (vertex[0]<minx) minx=vertex[0];
           if (vertex[0]>maxx) maxx=vertex[0];

           if (vertex[1]<miny) miny=vertex[1];
           if (vertex[1]>maxy) maxy=vertex[1];

//           if (vertex[2]<minz) minz=vertex[2];
//           if (vertex[2]>maxz) maxz=vertex[2];	//we no longer take z into account 12/1298 SB

           number_of_vertices++;
           if (number_of_vertices>999)
		   report_error("MeshToZex() Object too complex - > 999 vertices.","\p",666);
          //next line
          
          source+=line_len+1;	//point to next line (accounting for CR)
	      get_word(the_word,the_line);
         } //6
       line_len=get_line(source,the_line);	//for the while loop
	   get_word(the_word,the_line);

      }    //end of vertices - 5
//calc max_radius
		max_radius=ABS(minx);
		if (ABS(maxx)>max_radius) max_radius=ABS(maxx);
		if (ABS(miny)>max_radius) max_radius=ABS(miny);
		if (ABS(maxy)>max_radius) max_radius=ABS(maxy);
//		if (ABS(minz)>max_radius) max_radius=ABS(minz);
//		if (ABS(maxz)>max_radius) max_radius=ABS(maxz);
		

//    } //4 - end of line_len
//   } //end of look for vertices
//2. look for edges - we currently dont use these, so we just look for the materials

    the_word[0]=0;
    while (line_len!=-1 && (Zstrcmp(the_word,"MATERIAL\0")))	//keep going till eof or material
      { //4
       line_len=get_line(source,the_line);
       if (line_len!=-1)
         { //5
          source+=line_len+1;	//point to next line (accounting for CR)
          get_word(the_word,the_line);

         } //5
      }      //end of edges - 4
    
          source-=line_len+1;	//point to next line (accounting for CR)

//3. materials (connections or triangle list)
//The materials blocks are how Mesh (Have I said it's a great program?) defines it's 
//triags and we define our polys from them
//From Zex0.01d2 onwards all polys are simple triags.
//When this import works, Zex will go to d3 and Zex will be a living entity at last
//cause then I can get my best ship designers hat on and design one (if I can stay awake that long..)
//get the colour of the material
//we are pointing at "MATERIAL x,x,x"
//The next line is either a list of connections for this material
//or the next material block
connection_count=0;
for (material=0;material<8;material++)
{

	line_pos=0;
    word_len=get_word(the_word,&the_line[line_pos]);	//skip material
           line_pos+=word_len;
            pcol[material].points_start=connection_count;	//points from zero coloured as
    word_len=get_word(the_word,&the_line[line_pos]);	//red value
           line_pos+=word_len;
           ix=0;
           str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
           pcol[material].colour.red=(int)Zconvert_to_float (the_word);
    word_len=get_word(the_word,&the_line[line_pos]);	//red value
           line_pos+=word_len;
           ix=0;
           str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
           pcol[material].colour.green=(int)Zconvert_to_float (the_word);
    word_len=get_word(the_word,&the_line[line_pos]);	//red value
           line_pos+=word_len;
           ix=0;
           str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
           pcol[material].colour.blue=(int)Zconvert_to_float (the_word);
//Zex d9 - following the rgb values are:
/*
4 - smoothing type - if 1 it's gourad
5 - map type, 0 if none, else 1 for planar, 2 for cyndrical, 3 for spherical
6 - 0 (reserved) or solid colour transparancy
7.map axis: 0 for x-axis, 1 for y-axis, 2 for z-axis 
8.map x: x-coordinate of map center 
9.map y: y-coordinate of map center 
10.map z: z-coordinate of map center 
11.map x-size: extent of mapping in x direction 
12.map y-size: extent of mapping in y direction 
13.map z-size: extent of mapping in z direction 
14. 1.1 only - map transparency mode, 0 for opaque, 1 for white transparent, 2 for alpha channel
*/
//get smoothing - 9/1/99
    word_len=get_word(the_word,&the_line[line_pos]);	//red value
           line_pos+=word_len;

           ix=0;
           str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float

    smoothing=(int)Zconvert_to_float (the_word);
//get map type
    word_len=get_word(the_word,&the_line[line_pos]);	//red value
           line_pos+=word_len;
           ix=0;
           str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float

    map_type=(int)Zconvert_to_float (the_word);
   
    if (map_type!=0)
    {
 // read in rest of gumf
 //skip reserved
     word_len=get_word(the_word,&the_line[line_pos]);	 //reserved
           line_pos+=word_len;
//get mapaxis
    word_len=get_word(the_word,&the_line[line_pos]);	//map axis
           line_pos+=word_len;
           ix=0;
           str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float

    map_axis=(int)Zconvert_to_float (the_word);

//get center_x
    word_len=get_word(the_word,&the_line[line_pos]);	//map center
           line_pos+=word_len;
           ix=0;
           str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float

    map_center_x=Zconvert_to_float (the_word);

//get center_y
    word_len=get_word(the_word,&the_line[line_pos]);	//map center
           line_pos+=word_len;
           ix=0;
           str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float

    map_center_y=Zconvert_to_float (the_word);
//get center_z
    word_len=get_word(the_word,&the_line[line_pos]);	//map center
           line_pos+=word_len;
           ix=0;
           str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float

    map_center_z=Zconvert_to_float (the_word);

//get extent_x
    word_len=get_word(the_word,&the_line[line_pos]);	//map center
           line_pos+=word_len;
           ix=0;
           str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float

    map_extent_x=Zconvert_to_float (the_word);

//get extent_y
    word_len=get_word(the_word,&the_line[line_pos]);	//map center
           line_pos+=word_len;
           ix=0;
           str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float

    map_extent_y=Zconvert_to_float (the_word);
//get extent_z
    word_len=get_word(the_word,&the_line[line_pos]);	//map center
           line_pos+=word_len;
           ix=0;
           str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float

    map_extent_z=Zconvert_to_float (the_word);
    
    if (version>10)
    {
     //get  solid colour transparancy
     
      word_len=get_word(the_word,&the_line[line_pos]);	//map center
      line_pos+=word_len;
      ix=0;
      str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float

      unknown_param=Zconvert_to_float (the_word);
    }
     /* ****** we only support planar mapping ******  */
    if(map_axis==2)  /* z axis  (the original zex mapping)  */
      {
//calc map min_x,max_x, min_y and max_y
//max y is numerically greater than min y
			map_min_x=map_extent_x/2;
			map_min_x=map_center_x-map_min_x;

			map_max_x=map_extent_x/2;
			map_max_x=map_center_x+map_max_x;

			map_min_y=map_extent_y/2;
			map_min_y=map_center_y-map_min_y;

			map_max_y=map_extent_y/2;
			map_max_y=map_center_y+map_max_y;
            step_size_u=map_extent_x/BITMAP_SIZE;
            step_size_v=map_extent_y/BITMAP_SIZE;
            
      }
    else if(map_axis==1)  /* y axis */
      {
			map_min_x=map_extent_x/2;
			map_min_x=map_center_x-map_min_x;

			map_max_x=map_extent_x/2;
			map_max_x=map_center_x+map_max_x;

			map_min_y=map_extent_z/2;		// changed
			map_min_y=map_center_z-map_min_y;	// changed

			map_max_y=map_extent_z/2;		// changed
			map_max_y=map_center_z+map_max_y;	// changed

			step_size_u=map_extent_x/BITMAP_SIZE;
			step_size_v=map_extent_z/BITMAP_SIZE;	// changed
      
      }
    else if(map_axis==0) /* x axis */
      {
			map_min_x=map_extent_z/2;		// changed
			map_min_x=map_center_z-map_min_x;	// changed

			map_max_x=map_extent_z/2;		// changed
			map_max_x=map_center_z+map_max_x;	// changed

			map_min_y=map_extent_y/2;
			map_min_y=map_center_y-map_min_y;

			map_max_y=map_extent_y/2;
			map_max_y=map_center_y+map_max_y;

			step_size_u=map_extent_z/BITMAP_SIZE;	// changed
			step_size_v=map_extent_y/BITMAP_SIZE;
      
      }
      
    } //end of map type not 0
    else
    {
      if (version>10)
      {
       //get solid colour transparency
       word_len=get_word(the_word,&the_line[line_pos]);	//map center
       line_pos+=word_len;
       ix=0;
       str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float

       unknown_param=Zconvert_to_float (the_word);
      }
    }
    the_word[0]=0;
    line_pos=0;
    source+=line_len+1;	//point to next line (accounting for CR)


    line_len=get_line(source,the_line);
    word_len=get_word(the_word,&the_line[line_pos]);	//red value
 
    if (Zstrcmp(the_word,"END\0")!=0)
    {
     while (line_len!=-1 && (Zstrcmp(the_word,"MATERIAL\0")))	 //keep going till eof or second Material
      { //4
       if (line_len!=-1)
         { //5 - get 3 connections for a triag
//           connections[connections_index]=3;	  //number of lines
//           connections_index++;
           source+=line_len+1;	//point to next line (accounting for CR)
           word_len=get_word(the_word,&the_line[line_pos]);
           ix=0;
           str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
           the_connection=(int)Zconvert_to_float (the_word);



//           first_connection=the_connection;	//so we can connect last to first
           connections[connections_index].the_node=the_connection;
//from the node, get the points x and y and translate to bitmaps u and v...
//            map_x_step=(map_max_x-map_min_x)/bitmap_size;
//            map_y_step=(map_max_y-map_min_y)/bitmap_size;		  	  
           
           if (map_type!=0)	//textured?
           {
            if(map_axis==0)  /* x axis */
              {
              px=vertices[the_connection].z;
              }
            else
              {
              px=vertices[the_connection].x;
              }
            px=px-map_min_x;	//map to logical coord
            px=px/step_size_u;	//translate to u
            
            if(map_axis==1) /* y axis */
              {
              py=vertices[the_connection].z;
              }
            else
              {
              py=vertices[the_connection].y;	//we need to invert the sign of y
              }
            py=py-map_min_y;	//map to logical coord
            py=py/step_size_v;	//translate to v
            py=BITMAP_SIZE-py;
//some paranoia for meshworks innacuracies
            if (py<0) py=0;
            if (py>BITMAP_SIZE-1) py=BITMAP_SIZE-1;
            if (px<0) px=0;
            if (px>BITMAP_SIZE-1) px=BITMAP_SIZE-1;

            connections[connections_index].u=px;
            connections[connections_index].v=py;
            connections[connections_index].type=TEXTURE;
            if(smoothing==1)
            {
             connections[connections_index].type=GOURADTEXTURE;	//030101 - gourad textue mapping SB
            }
           }
           else
           if(smoothing==1)
           {
            connections[connections_index].type=GOURAD;
           }
	    else
           {
            connections[connections_index].type=FLAT;
           }

           connections_index++; //first point
           line_pos+=word_len;
           word_len=get_word(the_word,&the_line[line_pos]);
           ix=0;
           str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
           the_connection=(int)Zconvert_to_float (the_word);

           connections[connections_index].the_node=the_connection;
           if (map_type!=0)	//texture mapped
           {

            if(map_axis==0)  /* x axis */
              {
              px=vertices[the_connection].z;
              }
            else
              {
              px=vertices[the_connection].x;
              }
            px=px-map_min_x;	//map to logical coord
            px=px/step_size_u;	//translate to u
            
            if(map_axis==1) /* y axis */
              {
              py=vertices[the_connection].z;
              }
            else
              {
              py=vertices[the_connection].y;	
              }


            py=py-map_min_y;	//map to logical coord
            py=py/step_size_v;	//translate to v
            py=BITMAP_SIZE-py;

//some paranoia for meshworks innacuracies
            if (py<0) py=0;
            if (py>BITMAP_SIZE-1) py=BITMAP_SIZE-1;
            if (px<0) px=0;
            if (px>BITMAP_SIZE-1) px=BITMAP_SIZE-1;

            connections[connections_index].u=px;
            connections[connections_index].v=py;
            connections[connections_index].type=TEXTURE;
            if(smoothing==1)
            {
             connections[connections_index].type=GOURADTEXTURE;	//030101 - gourad textue mapping SB
            }
           
           }
           else
           if(smoothing==1)
           {
            connections[connections_index].type=GOURAD;
           }
			else
           {
            connections[connections_index].type=FLAT;
           }

           connections_index++;	//second point
//           connections[connections_index]=the_connection;
//           connections_index++;		  //zex like 0-1,1-2 etc.

           line_pos+=word_len;
           word_len=get_word(the_word,&the_line[line_pos]);
           ix=0;
           str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
           the_connection=(int)Zconvert_to_float (the_word);

           connections[connections_index].the_node=the_connection;
           if (map_type!=0)
           {
//           px=vertices[the_connection].x;
//           px=px-map_min_x;	 //map to logical coord
//           px=px/step_size_u;	//translate to u
//           py=vertices[the_connection].y;

            if(map_axis==0)  /* x axis */
              {
              px=vertices[the_connection].z;
              }
            else
              {
              px=vertices[the_connection].x;
              }
            px=px-map_min_x;	//map to logical coord
            px=px/step_size_u;	//translate to u
            
            if(map_axis==1) /* y axis */
              {
              py=vertices[the_connection].z;
              }
            else
              {
              py=vertices[the_connection].y;	//we need to invert the sign of y
              }


            py=py-map_min_y;	//map to logical coord
            py=py/step_size_v;	//translate to v
            py=BITMAP_SIZE-py;
//some paranoia for meshworks innacuracies
            if (py<0) py=0;
            else if (py>BITMAP_SIZE-1) py=BITMAP_SIZE-1;
            if (px<0) px=0;
            else if (px>BITMAP_SIZE-1) px=BITMAP_SIZE-1;

            connections[connections_index].u=px;
            connections[connections_index].v=py;
            connections[connections_index].type=TEXTURE;
            if(smoothing==1)
            {
             connections[connections_index].type=GOURADTEXTURE;	//030101 - gourad textue mapping SB
            }
           }
           else
           if(smoothing==1)
           {
            connections[connections_index].type=GOURAD;
           }
			else
           {
            connections[connections_index].type=FLAT;
           }
           connections_index++;	//third point
//connect last to first
//           connections[connections_index]=the_connection;
//           connections_index++;
//           connections[connections_index]=first_connection;
//           connections_index++;
           if (connections_index>1999)
		   report_error("MeshToZex() Object too complex - > 1999 connections.","\p",666);
           connection_count++;
           line_pos+=word_len;
         } //5
       line_len=get_line(source,the_line);
       line_pos=0;
       word_len=get_word(the_word,&the_line[line_pos]);

      }	//end of while MATERIAL (connections) - 4
     }	//end of !END

  } //for material

//NOW, we have the points and the connections - we can call insert_obj (Yeehaw!)
      insert_object(object_number,vertices,connections,number_of_vertices,connections_index,
                    0, the_id, colldet,max_radius, texture_id , translucent);
//      insert_object(1,vertices,connections,number_of_vertices,connections_index,-80);
//      insert_object(2,vertices,connections,number_of_vertices,connections_index,0);
       return 0;	//get out after doing triag connections

    } //end of found materials - 3

//  } //2
// } //3
 
return -1;	//never executed??? But if we do, and just in case, it's an init failure.

}	//end of MeshToZexDyn - 1


//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//Subfuncs of meshtozex
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥

//get a line from the source file
//returns number of characters copied or -1

int get_line(char *source,char *line)
{
char the_char;
int i;

 for (i=0;i<511;i++)
 {
   the_char=*source++;
   if (the_char==0) return -1;	//eof
   
   if (the_char==10) the_char=13;	//fix to handle unix eol's
   if (the_char!=13)
      *line++=the_char;
 else 
   {
    *line=0;	//terminate
    return i;
   }
 }
 
 return -1;
 
}	//end of get_line


//get a word from &source
int get_word (char *word, char *source)
{
char the_char;
int i;

 for (i=1;i<511;i++)
 {

   the_char=*source++;
   switch (the_char)
   {
   case 9: case 32: case ',':
        {
         *word=0;
         return i;
        }
   case 13: case 10: case 0:
        {
        *word=0;
        return -1;	//ended in cr/lf or eof
        }
   default:
      *word++=the_char;
   }
 }	//end of for i

MyDebugStr(__LINE__,__FILE__,"Malformed object - line too long");
return 0;	// word of greater than 511 must be an error?
}


//Standard C strcmp
int Zstrcmp(char *s, char *t)
{
for ( ; *s == *t; s++, t++)
    if (*s == '\0') 
        return 0;
return *s - *t;	//if != then return diff
}