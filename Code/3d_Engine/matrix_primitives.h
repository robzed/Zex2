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

//matrix_primitives.ext

#ifndef __3d_structures__
#include "3d_structures_def.h"
#endif


void MAT_Copy(double source[4][4], double dest[4][4]);
void MAT_Mult(double mat1[4][4], double mat2[4][4], double dest[4][4]);
void VEC_MultMatrix(_3D *Source,double mat[4][4],_3D *Dest);
void MAT_Identity(double mat[4][4]);
void TR_Translate(double matrix[4][4],double tx,double ty,double tz);
void TR_Scale(double matrix[4][4],double sx,double sy, double sz);
void TR_Rotate(double matrix[4][4],int ax,int ay,int az);
void TR_Qrot(double matrix[4][4],_quat Qor);
void TR_Qrot2(double matrix[4][4],_quat Qor);
void NewQuatToMat(_quat * quat, double m[4][4]);
void QuatSetValue(_quat *quat, double x, double y, double z, int angle);
void QuatSetValueNormalised(_quat *quat, double x, double y, double z, int angle_in_degrees);
void QuatNormalize(_quat *quat);
void QuatAddValue(_quat *quat, double x, double y, double z, int angle);

double vectorMod(vector *v);
void VmultiplyBy(vector *q1, double constant);
void VdivideBy(vector *q1, double constant);
void Vcrossproduct(vector *v1, vector *v2, vector *r);
void Vmultiply(vector *q1, double constant, vector *result);
void Vdivide(vector *v1,vector *v2, vector* r); //divide v1 by v2
void Vinvert(vector *v1);
void Vaddto(vector* v1, vector* v2);	//add v2 to v1
void Vsub(vector *v1,vector *v2, vector* r);	//sub v2 from v1, result in r
void Vsubfrom(vector *v1, vector *v2); //sub v2 from v1
void Vcopy( vector * v1, vector * v2);	//copy v2 to v1

void divide(_quat *a,double b, _quat* r);
void quaternionMultiply(_quat* q1,_quat* q2, _quat* qr);
double dotProduct(double x1,double y1, double z1, double x2, double y2, double z2 );

//primitives
double Vdotproduct(vector* Vector1, vector* Vector2);
void Vnormalise(vector * Vector);
void VEC_multiply(vector *a, vector *b, vector* r) ;

void Z3D_TrigInit();


void calculate_normal(vector *n, _3D* p1, _3D* p2, _3D* p3);


double RA_line_len (double x, double y, double z); //Rational Approx line length algorithm

