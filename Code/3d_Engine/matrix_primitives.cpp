// matrix_primitives.cpp
//matrix functions (primitives) and low level trig
/*
$Log: matrix_primitives.cpp,v $
Revision 1.1.1.1  2003/09/05 22:35:11  stu_c
First Imported.

Revision 1.11  2002/09/08 02:11:53  stu_c
Precompiled header adjustments

Revision 1.10  2002/01/27 18:06:05  stu
Unused variables removed

Revision 1.9  2001/12/29 22:59:46  stu
Added QuatAddValue

Revision 1.8  2001/12/29 01:11:44  stu
Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.

Revision 1.7  2001/12/28 18:37:46  stu
Added QuatSetValueNormalised

Revision 1.6  2001/12/04 22:58:07  rob
sin tab proto removed

Revision 1.5  2001/11/24 19:27:22  stu
RA_line_len in place of sqrt

Revision 1.4  2001/11/02 19:32:39  rob
Build variants for interface lib and carbon.
Coffee fixes after changes for X.

Revision 1.3  2001/10/24 21:39:54  stu
First set of source to build on pb for carbon.

Revision 1.2  2001/10/22 21:27:59  rob
Carbon warning changes

Revision 1.1  2001/10/21 01:10:09  stu
Initial porting work

Revision 1.0.0.1  2001/10/17 20:46:04  rob
First Imported.

Revision 1.3  2001/09/04 20:39:08  stu
Sim stuff

Revision 1.2  2001/05/28 01:54:09  stu
280501

Revision 1.1.1.1  2001/01/01 21:13:34  rob
First Imported.

Revision 1.7  2000/12/11 19:46:17  stu
Light fix in 3d_top - calc_normal

Revision 1.6  2000/12/10 03:30:55  stu
Nothing changed. Honest.

Revision 1.5  2000/11/26 22:55:47  stu
Various.

Revision 1.4  2000/10/08 21:41:51  stu
Engine conversion to 64 bit. Huge pain of a job.

Revision 1.3  2000/10/08 13:23:37  rob
calculate normal updated

Revision 1.2  2000/10/07 17:01:51  rob
Created common normal routine

Revision 1.1  2000/09/20 23:54:23  stu
HUD/AP/Scaling

Revision 1.0.0.1  2000/08/21 21:59:45  stu
First Imported.

Revision 1.0.0.1  2000/08/11 23:19:30  stu
First Imported.

Revision 1.0.0.1  2000/08/11 23:08:32  stu
First Imported.

Revision 1.0.0.1  2000/08/11 22:34:19  stu
First Imported.

Revision 1.2  2000/04/07 01:59:17  stu
Ongoing camera in cockpit

Revision 1.1.1.1  2000/03/23 22:43:29  stu
First Imported.

Revision 1.2  1999/05/06 17:40:04  stu
*** empty log message ***

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
#endif //pch

#include "matrix_primitives.h"

//matrix_primitives.int
double SinTable[360], CosTable[360];
double mat1[4][4], mat2[4][4];	//these are module global matricies used to rotate and
                                //transform an object.

//identity defined in global storage for a quick copy...
double the_identity [4][4] =
{ { 1,0,0,0, },
  { 0,1,0,0, }, 
  { 0,0,1,0, }, 
  { 0,0,0,1 } };

//Our module static data
static  double zmat[4][4];	//={0,0,0,0,
//                          0,0,0,0,
//                          0,0,1,0,
//                          0,0,0,1};

static double ymat[4][4];	//={0,0,0,0,
//                         0,1,0,0,
//                         0,0,0,0,
//                         0,0,0,1};
                         
static double xmat[4][4];	//={1,0,0,0,	 //was 1,0,0,0 SB 170400
//                         0,0,0,0,
//                         0,0,0,0,
//                         0,0,0,1};
                         
static double tmat[4][4];	 //translation matrix

                         
//   tmat[0][0]=1;  tmat[0][1]=0;  tmat[0][2]=0;  tmat[0][3]=0;
//   tmat[1][0]=0;  tmat[1][1]=1;  tmat[1][2]=0;  tmat[1][3]=0;
//   tmat[2][0]=0;  tmat[2][1]=0;  tmat[2][2]=1;  tmat[2][3]=0;
//   tmat[3][0]=tx; tmat[3][1]=ty; tmat[3][2]=tz; tmat[3][3]=1;
              
//   xmat[0][0]=1;        xmat[0][1]=0;        xmat[0][2]=0;        xmat[0][3]=0;
//   xmat[1][0]=0;        xmat[1][1]=Cax;      xmat[1][2]=Sax;      xmat[1][3]=0;
//   xmat[2][0]=0;        xmat[2][1]=-Sax;     xmat[2][2]=Cax;      xmat[2][3]=0;
//   xmat[3][0]=0;        xmat[3][1]=0;        xmat[3][2]=0;	    xmat[3][3]=1;
                   
//   ymat[0][0]=Cay;      ymat[0][1]=0;        ymat[0][2]=-Say;     ymat[0][3]=0;
//   ymat[1][0]=0;        ymat[1][1]=1;        ymat[1][2]=0;        ymat[1][3]=0;
//   ymat[2][0]=Say;      ymat[2][1]=0;        ymat[2][2]=Cay;      ymat[2][3]=0;
//   ymat[3][0]=0;        ymat[3][1]=0;        ymat[3][2]=0;        ymat[3][3]=1;

//   zmat[0][0]=Caz;      zmat[0][1]=Saz;      zmat[0][2]=0;        zmat[0][3]=0;
//   zmat[1][0]=-Saz;     zmat[1][1]=Caz;      zmat[1][2]=0;        zmat[1][3]=0;
//   zmat[2][0]=0;        zmat[2][1]=0;        zmat[2][2]=1;        zmat[2][3]=0;
//   zmat[3][0]=0;        zmat[3][1]=0;        zmat[3][2]=0;        zmat[3][3]=1;






//SB 6/11/98
void MAT_Copy(double source[4][4], double dest[4][4])
{
    int i,j;
    for(i=0; i<4; i++)
      for(j=0; j<4; j++)
         dest[i][j]=source[i][j];
}





//mult 2 matricies and store in dest matrix
void MAT_Mult(double mat1[4][4], double mat2[4][4], double dest[4][4])
{
   int i,j;

   for(i=0; i<4; i++)
      for(j=0; j<4; j++)
         dest[i][j]=mat1[i][0]*mat2[0][j]+
                    mat1[i][1]*mat2[1][j]+
                    mat1[i][2]*mat2[2][j]+
                    mat1[i][3]*mat2[3][j];

//   for(j=0; j<4; j++)
//      for(i=0; i<4; i++)
//         dest[i][j]=mat1[j][0]*mat2[0][i]+
//                    mat1[j][1]*mat2[1][i]+
//                    mat1[j][2]*mat2[2][i]+
//                    mat1[j][3]*mat2[3][i];

}







//matrix product
void VEC_MultMatrix(_3D *Source, double mat[4][4],_3D *Dest)
{
    Dest->x=Source->x*mat[0][0]+
            Source->y*mat[1][0]+
            Source->z*mat[2][0]	+
                      mat[3][0]; 
                                             
    Dest->y=Source->x*mat[0][1]+
            Source->y*mat[1][1]+
            Source->z*mat[2][1]+
                      mat[3][1];
                       
    Dest->z=Source->x*mat[0][2]+
            Source->y*mat[1][2]+
            Source->z*mat[2][2]+
                      mat[3][2];
                      
//    Dest->x=Source->x*mat[0][0]+
//            Source->y*mat[0][1]+
//            Source->z*mat[0][2]+
//                      mat[0][3];
//                       
//    Dest->y=Source->x*mat[1][0]+
//            Source->y*mat[1][1]+
//            Source->z*mat[1][2]+
//                      mat[1][3];
//                       
//    Dest->z=Source->x*mat[2][0]+
//            Source->y*mat[2][1]+
//            Source->z*mat[2][2]+
//                      mat[2][3]; 

// Dest->x = (Source->x * mat[0][0]) + (Source->y * mat[1][0]) + (Source->z * mat[2][0]); //THIS IS *SO* NOT HOW TO FIX THE X COORDINATE
// Dest->y = (Source->x * mat[0][1]) + (Source->y * mat[1][1]) + (Source->z * mat[2][1]);
// Dest->z = (Source->x * mat[0][2]) + (Source->y * mat[1][2]) + (Source->z * mat[2][2]);


}


//fills in an identity matrix
void MAT_Identity(double mat[4][4])
{
//using blockmove might offer a slightt speed increase
//extern Float64 the_identity[4][4];	    //a predefined identity so hopefully the compiler can do a quick copy
//BlockMoveData(&the_identity,mat,sizeof(the_identity));

    mat[0][0]=1; mat[0][1]=0; mat[0][2]=0; mat[0][3]=0;
    mat[1][0]=0; mat[1][1]=1; mat[1][2]=0; mat[1][3]=0;
    mat[2][0]=0; mat[2][1]=0; mat[2][2]=1; mat[2][3]=0;
    mat[3][0]=0; mat[3][1]=0; mat[3][2]=0; mat[3][3]=1;


}


void TR_Translate(double matrix[4][4],double tx,double ty,double tz)
{
//extern double tmat[4][4];
   tmat[0][0]=1;  tmat[0][1]=0;  tmat[0][2]=0;  tmat[0][3]=0;
   tmat[1][0]=0;  tmat[1][1]=1;  tmat[1][2]=0;  tmat[1][3]=0;
   tmat[2][0]=0;  tmat[2][1]=0;  tmat[2][2]=1;  tmat[2][3]=0;
   tmat[3][0]=tx; tmat[3][1]=ty; tmat[3][2]=tz; tmat[3][3]=1;
   MAT_Mult(matrix,tmat,mat1);
   MAT_Copy(mat1,matrix);
}

void TR_Scale(double matrix[4][4],double sx,double sy, double sz)
{
   double smat[4][4];
   smat[0][0]=sx; smat[0][1]=0;  smat[0][2]=0;  smat[0][3]=0;
   smat[1][0]=0;  smat[1][1]=sy; smat[1][2]=0;  smat[1][3]=0;
   smat[2][0]=0;  smat[2][1]=0;  smat[2][2]=sz; smat[2][3]=0;
   smat[3][0]=0;  smat[3][1]=0;  smat[3][2]=0;  smat[3][3]=1;
   MAT_Mult(matrix,smat,mat1);
   MAT_Copy(mat1,matrix);
}



//convert a Quat to an orientation matrix - overwrites the matrix
void TR_Qrot(double matrix[4][4],_quat Qor)
{
//translate Qor to orientation matrix
NewQuatToMat(&Qor,matrix);
}	

//same as above but multiplies existing matrix with result - used to rotate around camera
void TR_Qrot2(double matrix[4][4],_quat Qor)
{
   double Or_mat[4][4];
   double r_mat[4][4];
   
//translate Qor to orientation matrix
NewQuatToMat(&Qor,Or_mat);
MAT_Mult(matrix,Or_mat,r_mat);	//<-Commutation - had Or_mat*matrix which is wrong (causes objects to rotate around 
				//their axis rather than around the camera. D'oh! - two days.
MAT_Copy(r_mat,matrix);
}


void TR_Rotate(double matrix[4][4],int ax,int ay,int az)
{
   double Cax,Sax,Cay,Say,Caz,Saz;
//   float xmat[4][4];
//   extern double zmat[4][4];
//   extern double ymat[4][4],xmat[4][4];
Sax=SIN(ax);
Say=SIN(ay);
Saz=SIN(az);

Cax=COS(ax);
Cay=COS(ay);
Caz=COS(az);

//   xmat[0][0]=1;        xmat[0][1]=0;        xmat[0][2]=0;        xmat[0][3]=0;
//   xmat[1][0]=0;        
   xmat[1][1]=Cax;      xmat[1][2]=Sax;      //xmat[1][3]=0;
//   xmat[2][0]=0;        
   xmat[2][1]=-Sax;     xmat[2][2]=Cax;      //xmat[2][3]=0;
//   xmat[3][0]=0;        xmat[3][1]=0;        xmat[3][2]=0;	    xmat[3][3]=1;

   ymat[0][0]=Cay;      
//   ymat[0][1]=0;        
   ymat[0][2]=-Say;
//   ymat[0][3]=0;
//   ymat[1][0]=0;        ymat[1][1]=1;        ymat[1][2]=0;        ymat[1][3]=0;
     ymat[2][0]=Say;      //ymat[2][1]=0;        
     ymat[2][2]=Cay;      //ymat[2][3]=0;
//   ymat[3][0]=0;        ymat[3][1]=0;        ymat[3][2]=0;        ymat[3][3]=1;

   zmat[0][0]=Caz;      zmat[0][1]=Saz;      //zmat[0][2]=0;        zmat[0][3]=0;
   zmat[1][0]=-Saz;     zmat[1][1]=Caz;      //zmat[1][2]=0;        zmat[1][3]=0;
//   zmat[2][0]=0;        zmat[2][1]=0;        zmat[2][2]=1;        zmat[2][3]=0;
//   zmat[3][0]=0;        zmat[3][1]=0;        zmat[3][2]=0;        zmat[3][3]=1;

   MAT_Mult(matrix,ymat,mat1);	//is this the right order?
   MAT_Mult(mat1,xmat,mat2);
   MAT_Mult(mat2,zmat,matrix);
}



///*SDOC***********************************************************************
//
//  Name:		  QuatToMat
//
//  Action:   Converts quaternion representation of a rotation to a matrix
//	  	  	  representation
//
//  Params:   GL_QUAT* (our quaternion), GLfloat (4x4 matrix)
//
//  Returns:  nothing
//
//  Comments: remember matrix (in OGL) is represented in COLUMN major form
//
//***********************************************************************EDOC*/
//
//

void NewQuatToMat(_quat * quat, double m[4][4]) 
{
   double xx = quat->x * quat->x;
   double xy = quat->x * quat->y;
   double xz = quat->x * quat->z;
   double xw = quat->x * quat->w;

   double yy = quat->y * quat->y;
   double yz = quat->y * quat->z;
   double yw = quat->y * quat->w;

   double zz = quat->z * quat->z;
   double zw = quat->z * quat->w;



   m[0][0]  = 1 - 2 * ( yy + zz ); m[1][0]  =     2 * ( xy - zw ); m[2][0]  =     2 * ( xz + yw );
   m[0][1]  =     2 * ( xy + zw ); m[1][1]  = 1 - 2 * ( xx + zz ); m[2][1]  =     2 * ( yz - xw );
   m[0][2]  =     2 * ( xz - yw ); m[1][2]  =     2 * ( yz + xw ); m[2][2]  = 1 - 2 * ( xx + yy );
   m[3][0]=0;
   m[3][1]=0;
   m[3][2]=0;

   m[3][3]=1;
   m[2][3]=0;
   m[1][3]=0;
   m[0][3]=0;

}



double vectorMod(vector *v) 
{
 return RA_line_len(v->x,v->y,v->z);
 //return sqrt( (v->x*v->x)+(v->y*v->y)+(v->z*v->z) );
}


double dotProduct(double x1,double y1, double z1, double x2, double y2, double z2 ) 
{
   return (x1 * x2) + (y1 * y2) + (z1 * z2);
}

void quaternionMultiply(_quat* q1,_quat* q2, _quat* qr) 
{
   qr->x = (q1->w * q2->x) + (q2->w * q1->x) + (q1->y * q2->z) - (q1->z * q2->y);
   qr->y = (q1->w * q2->y) + (q2->w * q1->y) + (q1->z * q2->x) - (q1->x * q2->z);
   qr->z = (q1->w * q2->z) + (q2->w * q1->z) + (q1->x * q2->y) - (q1->y * q2->x);
   qr->w = (q1->w * q2->w) - dotProduct(q1->x,q1->y,q1->z,q2->x,q2->y,q2->z);
}

void Vcrossproduct(vector *v1, vector *v2, vector *r) 
{
   r->x = ((v1->y * v2->z) - (v1->z * v2->y));
   r->y = ((v1->z * v2->x) - (v1->x * v2->z));
   r->z = ((v1->x * v2->y) - (v1->y * v2->x));
}

void VmultiplyBy(vector *q1, double constant) 
{      //multiply this vector by a scalar
   q1->x *= constant;
   q1->y *= constant;
   q1->z *= constant;
  };

void VdivideBy(vector *q1, double constant) 
{      //divide this vector by a scalar
   q1->x /= constant;
   q1->y /= constant;
   q1->z /= constant;
  };


void Vmultiply(vector *q1, double constant, vector *result) 
{      //mult this vector by a scalar
   result->x=q1->x * constant;
   result->y=q1->y * constant;
   result->z=q1->z * constant;
  };

void Vdivide(vector *v1, vector *v2, vector * r) //divide v1 by v2
{ //Perform the given op, and store the result in r
    r->x = v1->x / v2->x;
    r->y = v1->y / v2->y;
    r->z = v1->z / v2->z;
  };


void divide(_quat *a,double b, _quat* r) 
{ //Perform the given op, and store the result in r
    r->x = a->x / b;
    r->y = a->y / b;
    r->z = a->z / b;
  };
 
void Vsub(vector *v1, vector *v2, vector * r) //sub v2 from v1
{ //Perform the given op, and store the result in r
    r->x = v1->x - v2->x;
    r->y = v1->y - v2->y;
    r->z = v1->z - v2->z;
  };
 
void Vsubfrom(vector *v1, vector *v2) //sub v2 from v1
{ //Perform the given op, and store the result in r
    v1->x = v1->x - v2->x;
    v1->y = v1->y - v2->y;
    v1->z = v1->z - v2->z;
  };

void Vinvert( vector * v1)
{
   v1->x = -v1->x;
   v1->y = -v1->y;
   v1->z = -v1->z;

}


void Vaddto( vector * v1, vector * v2)	   //add v2 to v1
{
   v1->x = v1->x+v2->x;
   v1->y = v1->y+v2->y;
   v1->z = v1->z+v2->z;

}

void Vcopy( vector * v1, vector * v2)	  //copy v2 to v1
{
   v1->x = v2->x;
   v1->y = v2->y;
   v1->z = v2->z;

}

///*SDOC***********************************************************************
//
//  Name:		  gluQuatSetValue_EXT
//
//  Action:   Assembles quaternion from an axis and an angle
//
//  Params:   GL_QUAT* (quaternion), GLfloat (x, y, z - axis), GLfloat (angle)
//
//  Returns:  nothing
//
//  Comments: NOTE: vector has been split into x, y, z so that you do not have
//	  	  	  to change your vector library (i.e. greater portability)
//
//	  	  	  NOTE2: angle has to be in Degrees
//
//***********************************************************************EDOC*/
void QuatSetValue(_quat *quat, double x, double y, double z, int angle)
{
float sin_a, cos_a;
	sin_a=SIN(angle/2);
	cos_a=COS(angle/2);
	quat->x=x*sin_a;
	quat->y=y*sin_a;
	quat->z=z*sin_a;
	quat->w=cos_a;
	QuatNormalize(quat);
	
//code below is Chris Heckers original (bollox) code
//	float temp, dist;
//
//	  // normalize
//	  temp = x*x + y*y + z*z;
//
//    dist = (float)(1.0 / sqrt(temp));
//
//    x *= dist;
//    y *= dist;
//    z *= dist;
//
//	  quat->x = x;
//	  quat->y = y;
//	  quat->z = z;
//
//	  quat->w = (float)COS(angle / 2);
	
}




//Same as above but DANGEROUS because it assumes the vector passed in IS normalised!
//Obviously faster
//281201
void QuatSetValueNormalised(_quat *quat, double x, double y, double z, int angle_in_degrees)
{
float sin_a, cos_a;
	sin_a=SIN(angle_in_degrees/2);
	cos_a=COS(angle_in_degrees/2);
	quat->x=x*sin_a;
	quat->y=y*sin_a;
	quat->z=z*sin_a;
	quat->w=cos_a;	
	
}


void QuatAddValue(_quat *quat, double x, double y, double z, int angle)
{
float sin_a, cos_a;
	sin_a=SIN(angle);
	cos_a=COS(angle);
	quat->x+=x*sin_a;
	quat->y+=y*sin_a;
	quat->z+=z*sin_a;
	quat->w+=cos_a;
	QuatNormalize(quat);
	
	
}

///*SDOC***********************************************************************
//
//  Name:		  gluQuatNormalize_EXT
//
//  Action:   Normalizes quaternion (i.e. w^2 + x^2 + y^2 + z^2 = 1)
//
//  Params:   GL_QUAT* (quaternion)
//
//  Returns:  nothing
//
//  Comments: none
//
//***********************************************************************EDOC*/
void QuatNormalize(_quat *quat)
{
    double	 dist, square;

	square = quat->x * quat->x + quat->y * quat->y + quat->z * quat->z
		+ quat->w * quat->w;
	
	if (square > 0.0)
		dist = (double)(1.0 / sqrt(square));
	else dist = 1;

    quat->x *= dist;
    quat->y *= dist;
    quat->z *= dist;
    quat->w *= dist;
}

//=------------------






//some vector operations

double Vdotproduct(vector* Vector1, vector* Vector2)
{
    return (Vector1->x*Vector2->x+Vector1->y*Vector2->y+Vector1->z*Vector2->z);
}




void Vnormalise(vector * Vector)
{
   double m=sqrt(Vector->x*Vector->x+Vector->y*Vector->y+Vector->z*Vector->z);
//   double m=RA_line_len(Vector->x,Vector->y,Vector->z);
   Vector->x/=m;
   Vector->y/=m;
   Vector->z/=m;
}


void VEC_multiply(vector *a, vector *b, vector* r) 
{ //Perform the given op, and store the result in this object
    r->x = a->x * b->x;
    r->y = a->y * b->y;
    r->z = a->z * b->z;
  };

//init functions

//fill sin and cos tables

void Z3D_TrigInit()
{
   int d;
   for(d=0; d<360; d++)
   {
       SinTable[d]=sin(d*PI/180.0);
       CosTable[d]=cos(d*PI/180.0);
   }
}





void calculate_normal(vector *n, _3D* p1, _3D* p2, _3D* p3)
{                      
//get the normals
n->x=(p2->z-p1->z)*(p3->y-p1->y)-(p2->y-p1->y)*(p3->z-p1->z);
n->y=(p2->z-p1->z)*(p3->x-p1->x)-(p2->x-p1->x)*(p3->z-p1->z);
n->z=(p2->y-p1->y)*(p3->x-p1->x)-(p2->x-p1->x)*(p3->y-p1->y);

//normalize the vector
Vnormalise(n);
}


//Rational approximation line length function
//Performs sqrt (x*x+y*y+z*z)
double RA_line_len (double x, double y, double z)
{
double R,x_0,x_1,x_2;
/* stuchat 36; Stuart Ball, Ranko Bojanic
First compute R = a^2 + b^2 + c^2. Then

set x_0 = a+b+c and then compute

x_1 = 0.5*(x_0 + R/x_0)
x_2 = 0.5*(x_1 + R/x_1)
x_3 = 0.5*(x_2 + R/x_2)
*/

 x=fabs(x); y=fabs(y); z=fabs(z);
  
 R=(x*x)+(y*y)+(z*z);
 x_0=x+y+z;
 
 x_1=0.5*(x_0 + R/x_0);
 x_2 = 0.5*(x_1 + R/x_1);
// x_3 = 0.5*(x_2 + R/x_2);

 return x_2;
}
