//3d_top.ext

#ifndef __3d_structures__
#include "3d_structures_def.h"
#endif

extern int backface_cull;

void do_3d ();
void do_pers3(int width, int height);
void calc_normal(int the_object, int poly_select);
void do_culling2();
void do_zsort();
void calc_normal(int the_object, int poly_select);
void insert_to_z_buffer(_ZPoly *the_poly);
void reset_zsort_insert();
void do_gourad_polys();
_2Dp get_screen_center_coord_with_bflag(int object_slot);
_2D get_screen_front_coord(int object_slot);
_2D get_screen_rear_coord(int object_slot);
_2D point_to_screen(double x, double y, double z, int width, int h);
_2Dp point_to_screen_with_bflag(double x, double y, double z, int width, int h);
_2D get_any_screen_coord(int object_slot);

int is_in_frustrum(int object, bool fast_object);
_3D generate_point_in_frustrum(int object);


extern double depth_of_field;
extern double ZMin;

