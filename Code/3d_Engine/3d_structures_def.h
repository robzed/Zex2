//3d structures defs



#if ZEX_PROJECT_BUILDER
  #include <DrawSprocket/DrawSprocket.h>
#else
  #include <DrawSprocket.h>
#endif

#ifndef __LOGGING__
 #include "logging.h"
#endif

#define __3d_structures__


//Important Engine Defs
#define MAX_NUMBER_OF_FAST_DYN_OBJECTS 1350
#define MAX_NUMBER_OF_PARTICLE_DYN_OBJECTS 7
#define MAX_NUMBER_OF_DYN_OBJECTS 300

#define MAX_NUMBER_OF_DYN_POLY_SLOTS MAX_NUMBER_OF_DYN_OBJECTS+MAX_NUMBER_OF_FAST_DYN_OBJECTS 
#define MAX_NUMBER_OF_STAT_OBJECTS 140
#define MAX_NUMBER_OF_POLYS 5000
#define MAX_NUMBER_POLYS_PER_OBJ 520
#define MAX_NUMBER_OF_CLIPPED_POLYS 4000

#define NUMBER_OF_SCALED_POINTS 500	//max number of points in a scaled object
#define NUMBER_OF_SCALED_OBJECTS 1600 //max number of scaled objects

#define NUMBER_OF_CMAP_OBJS 1000	//this can be wanged up - it's just a list of ints
#define MAX_TOUCHING_POLYS 6	//number of polys touching a point for gourad
//shading types for poly structure
#define FLAT 0
#define GOURAD 1
#define TEXTURE 2
#define GOURADTEXTURE 3	//141200 SB
#define DMF    4	//270501 SB

#define IS_NOT_TRANSLUCENT 0
#define IS_TRANSLUCENT 1


#define HUMAN 0
#define ALIEN 1
#define NEUTRAL 2
#define SCENARY 3
#define MASSIVE_SCENARY 4
#define NO_RADAR 5



#define MAX_MOUNTS 30


//this define is used to set up a system when testing - i.e. no solar system,
//no dust, asteroids etc

#define TEST_SYSTEM 0	//after changing this you should completely rebuild the project! Neither Anvil nor PB can cope with the dependencies correctly
#define RIGHT_HANDED 1  //Should be 1 for right handed operation. Zex was until 080202 left handed

#define REMOVE_GRAVITY_FROM_GAME  1   // remove gravity and orbits from game


typedef struct
{
   float red, green, blue;
} fRGB;



//typedef struct
//{
//   double x, y, z;
//} _3D;

typedef struct
{
   double x, y, z;
} vector;

typedef vector _3D;

typedef struct
{
int x,y,z;
} _2D;

typedef struct
{
short u,v;
} texture_coord;

typedef struct
{
float x,y;
} screen_coord;

typedef struct
{
int x,y,behind;
} _2Dp;


typedef struct
{
float x,y,z;
} _float2D;

typedef struct
{
double w,x,y,z;
} _quat;




typedef short  _Connection;

typedef struct
{
RGBColor color;
short pad[3];	//to align with fRGB in dmf_textured_connections
short the_node;
short u;	//texture coords
short v;
short type;	//0=normal, 1=gourad, 2=texture
int pad1;	//OGL_name
float transparency;	//transparency
} textured_connections;

typedef struct
{
fRGB colour;	    //the vertex colour
short vertex;
short u;	//texture coords
short v;
short type;	//0=normal, 1=gourad, 2=texture, 4=3dmf connection
int OGL_name;
float transparency;	//0=transparent, 1=opaque
} dmf_textured_connections;


typedef struct
{
RGBColor colour;
int points_start;
} Zex_Colour;


//bit positions in a poly flag
#define BLOBIT 128	//draw with no texture
#define AUTOBLOB_BIT 64	//plot a point
#define DUST_BIT 32	//just plot a point if set
#define PLANET_BIT 16	//just plot a big point if set



#define module_private static
#define public 


typedef struct
{
_quat camera_quat;	//as it says really
vector camera_torque_sum;	//rotational vector

} camera_type;


typedef struct
{
textured_connections *poly_connections_ptr;	//ptr to the connection list for this poly
_3D face_normal;	//the face DIRECTION vector
_3D face_normal_translated;	//after rotation
RGBColor Prgb;	  //the colour
double average_z;	 //average z of poly
int Plight_value;	//the lighting value
int PPoints_offset;	//offset to points for 2dpoly draw
unsigned long Pid;	//the name for OGL
short Ptouching1[MAX_TOUCHING_POLYS];	  //for true gourad - a list of up to 6 polys touching this one point
short Ptouching2[MAX_TOUCHING_POLYS];	  //for true gourad - a list of up to 6 polys touching this one point
short Ptouching3[MAX_TOUCHING_POLYS];	  //for true gourad - a list of up to 6 polys touching this one point
short Pvertex_light[4];			//calculated in do_gourad and (4) in z-clip
//unsigned char  PTranslucent;	  //the translucency
char p_visible;	//poly is visible?
char draw_code;	//0=normal, 1=translucent, 2 ff=transparent - use different filler
unsigned char polyflags;	//bit 7=blob it
unsigned short owner;		//which dynamic object this poly belongs to. If blobbed we draw here.
//int special_1, special_2;	//???
//int eng_culled;	//if 0, do not backface cull this poly
} _ZPoly;


typedef struct
{
_ZPoly *_2DPoly_ptr;
int number_of_clipped_points;	//set in z-clip to either 3 or 4
int x1,y1,z1,x2,y2,z2,x3,y3,z3,y4,x4;	//4 calc'd in z-clip
} _2DPoly;



typedef struct	//output from z_clip - everything needed to draw a poly
{
int number_of_verts;
_3D vertices[4]; //3d coords
double average_z;
texture_coord texture_pts [4];  //texture coords
int Plight_value;               //the lighting value
short Pvertex_light[4];         //calculated in do_gourad and (4) in z-clip
short type;                     //0=normal, 1=gourad, 2=texture
unsigned long OGL_name;         //the texture name for OGL binding
char draw_code;                 //0=normal, 1=translucent, 2 ff=transparent - use different filler
unsigned char polyflags;        //bit 7=blob it
unsigned short owner;           //which dynamic object this poly belongs to
RGBColor Prgb;                  //the colour
float transparency;
//the 2d drawing coords
screen_coord screen_pts[4];
} _clipped_poly;	//170 bytes - 210401


//clipped_polys_drawing_info is what plot_pts uses to draw the polys
typedef struct {
_clipped_poly* clipped_polys_ptr;
int number_of_clipped_polys_to_draw;
int switch_zbuffer_on_at_poly_number;	//-1 if zbuffer not in use
float z_divisor_for_zbuffer;
} clipped_polys_drawing_info_type;



typedef struct
{
int in_use;
_ZPoly the_obj_polys[MAX_NUMBER_POLYS_PER_OBJ];
} _ZPoly_objs[MAX_NUMBER_OF_DYN_POLY_SLOTS];

typedef struct {
_3D *points_ptr;		//ptr to the points for this object
_ZPoly *polys_ptr;	//ptr to the polys for this object
int polys_slot;		//the polys slot
} anim_obj_bits;

//dynamic textures
typedef struct {
char tex_data [128*128+40];	//+40 just to make me feel [a lot] better.
int in_use;
int name;	//ogl name
} dynamic_texture;


enum {
TYPE_ZEX,
TYPE_ESHUTTLE,
TYPE_FALCON,
TYPE_ECLIPSE,
TYPE_SMALL,
TYPE_BLUE,
TYPE_GREEN,
TYPE_ASHUTTLE,
TYPE_PHOENIX,
TYPE_PYTHON,
TYPE_EXCALIBUR,
TYPE_DONT_CARE,
};




typedef struct
{
unsigned char in_use;
unsigned char laser_type;
unsigned char cannon_type;
unsigned char shields_type;
short craft_type;	//see enum above
short  time_docked;	//when this item was docked
} docked_item_type;

//mothership memory block 
typedef struct
{
                        //0-100
float production_rating;	//how quickly it can turn a new ship out - based on supply
float max_power_rating;	//max possible (0-100)
float power_rating;	//current raw power available - drops charge shields and as production goes up (0-100)
                         //of raw materials 0-100
float supply_rating;	 //affects above 3 - filled in from closeness to planet
                       //0 - 100
unsigned char in_use;	 //1 if so, else 0=free
unsigned char like_zex;	//used in alien mships - 1 initially, incremented if zex fires on mship
                       
unsigned char allegiance;
unsigned char visited;	//1 if we have actually docked here  

int supply_planet_index;	//which planet this MS is supplied from
int shield_value;	//copied into and out of OCB's
int callsign;

_float2D real_coords;

int max_ships_in_dock;
int ships_in_dock;	//how many in dock
docked_item_type docked_queue[8];	//max number of ships docked
int surrender_timer;	//if >0 can't surrender (surrender sets to 1000)
short galactic_id;

} mothership_block;


typedef struct
{
int in_use;	//0=no
_3D scaled_points_start[NUMBER_OF_SCALED_POINTS];
} scaled_points_type;


typedef struct
{
scaled_points_type scaled_points[NUMBER_OF_SCALED_OBJECTS];
} scaled_points_array_type;




//defines for basic_kit
#define BK_MISSING 0
#define BK_INREPAIR 1
#define BK_SERVICEABLE 2
#define BK_CAN_FIT 4
#define MAX_CARGO_BAY_SIZE 32
//From this data we can recreated a rotated and transformed bounding box in little space.
//we can do collision detection from this data in the tradsitional way.
#define _MEMBER ;
#define START_STRUCT_DEF typedef struct {
#define END_STRUCT_DEF }
#define END_DEF ;
#define PTR *
#define ARRAY_ [
#define _ARRAY ]
#define MEMBER_ 

//#include "zobject_struct.h"

#define BSTACK_SIZE 15
#define BSTACK_FRAME_SIZE_IN_INTS 11
//control data

START_STRUCT_DEF

// see 3d engine design - controllers for these
int MEMBER_ controller _MEMBER	 // the main controller for this object - typically NPC
int MEMBER_ no_control_this_frame _MEMBER

UInt8 MEMBER_ NPC_class _MEMBER // fighter, traveller, guard
UInt8 MEMBER_ allegiance _MEMBER	 // 2 or 3 if a scenary object, 1 if a monster object, 0 if a zex object - used in coll det
UInt8 MEMBER_ rank _MEMBER		     // the rank of the pilot
char MEMBER_ callsign[6] _MEMBER // E.G. 'H1234\0'
int MEMBER_ craft_type _MEMBER	 // see enum above mship_block - not used in training, only game from gen_NPC
int MEMBER_ laser_trigger_slot _MEMBER //for firing weapons
int MEMBER_ cannon_trigger_slot _MEMBER
int MEMBER_ exhaust_trigger_slot _MEMBER

int MEMBER_ control_use_1 _MEMBER
int MEMBER_ control_use_2 _MEMBER
int MEMBER_ control_use_3 _MEMBER
int MEMBER_ control_use_4 _MEMBER	 // for controller use
int MEMBER_ control_use_5 _MEMBER
int MEMBER_ control_use_6 _MEMBER

float MEMBER_ control_use_1_float _MEMBER
float MEMBER_ control_use_2_float _MEMBER
float MEMBER_ control_use_3_float _MEMBER
float MEMBER_ control_use_4_float _MEMBER

int MEMBER_ behaviour _MEMBER
unsigned char MEMBER_ BSTACK_ptr _MEMBER
int MEMBER_ bstack [BSTACK_FRAME_SIZE_IN_INTS*BSTACK_SIZE] _MEMBER	//stack for pushing 10 control vars per behaviour

unsigned char MEMBER_ name_str ARRAY_ 12 _ARRAY _MEMBER	//planet name etc as pascal string
int MEMBER_ galactic_id _MEMBER	// unique id from map so we can tell if an object is already present when we try to load it
long MEMBER_ docked_object_slot _MEMBER	// motherships slot number, -1 if not docked
int MEMBER_ shield_value _MEMBER	// when zero if collision then die (unless ZEX which has hull protection as well)
int MEMBER_ hull_value _MEMBER	      // when zero if collision (or damage) then die 
int MEMBER_ dq _MEMBER	  // how much damage this thing does when hit - used for weapons (not ships!)
UInt8 MEMBER_ request_dock _MEMBER	 // >0 if so - causes a mothership to consider docking object
UInt8 MEMBER_ docked _MEMBER	// 1 if so

// these are purely for motherships
int MEMBER_ docking_what _MEMBER	// what object is the mothership docking

//basic_kit
//each of these is a bit field as follows
// 31.....3         2               1 0
// control value    can be fitted   00=not fitted, 01=damaged, 10= serviceable
UInt32 MEMBER_ bk_mthr1 _MEMBER
UInt32 MEMBER_ bk_mthr2 _MEMBER
UInt32 MEMBER_ bk_mrev1 _MEMBER
UInt32 MEMBER_ bk_mrev2 _MEMBER
UInt32 MEMBER_ bk_vthr1 _MEMBER
UInt32 MEMBER_ bk_vthr2 _MEMBER
UInt32 MEMBER_ bk_rvth1 _MEMBER
UInt32 MEMBER_ bk_rvth2 _MEMBER
UInt32 MEMBER_ bk_gun01 _MEMBER
UInt32 MEMBER_ bk_gun02 _MEMBER
UInt32 MEMBER_ bk_gun03 _MEMBER
UInt32 MEMBER_ bk_gun04 _MEMBER
UInt32 MEMBER_ bk_las01 _MEMBER
UInt32 MEMBER_ bk_las02 _MEMBER
UInt32 MEMBER_ bk_las03 _MEMBER
UInt32 MEMBER_ bk_las04 _MEMBER
UInt32 MEMBER_ bk_mis01 _MEMBER
UInt32 MEMBER_ bk_mis02 _MEMBER
UInt32 MEMBER_ bk_mis03 _MEMBER
UInt32 MEMBER_ bk_eject _MEMBER


//now the misc ship data
Float32 MEMBER_ bk_mass_tonnes _MEMBER	//in tonnes
UInt32 MEMBER_ bk_fuel_capacity_kg _MEMBER
Float32 MEMBER_ rotational_pitch _MEMBER
Float32 MEMBER_ rotational_roll _MEMBER
Float32 MEMBER_ rotational_yaw _MEMBER

UInt8 MEMBER_ cargo_capacity_tonnes _MEMBER	//in 1 tonne containers - no bigger than 32!
UInt8  MEMBER_ hull_class _MEMBER

//lower three bits of the following is state as above
//integral items with hull
UInt8  MEMBER_ shields1_class _MEMBER
UInt8  MEMBER_ shields2_class _MEMBER
UInt8  MEMBER_ shields3_class _MEMBER
UInt8  MEMBER_ AP_class _MEMBER
UInt8  MEMBER_ radar_class _MEMBER
UInt8  MEMBER_ batt_class _MEMBER
UInt8  MEMBER_ ORCS _MEMBER

UInt8  MEMBER_ repair_system_class _MEMBER
UInt8  MEMBER_ shockwave_class _MEMBER
UInt8  MEMBER_ ECM_class _MEMBER
UInt8  MEMBER_ missfact_class _MEMBER
UInt8  MEMBER_ fastdock_class _MEMBER
UInt8  MEMBER_ insurance_class _MEMBER
UInt8  MEMBER_ fuel_maximiser_class _MEMBER
UInt8  MEMBER_ retrorocks_class _MEMBER
UInt8  MEMBER_ navcomp_class _MEMBER
UInt8  MEMBER_ trading_comp_class _MEMBER
UInt8  MEMBER_ hull_repair_class _MEMBER
UInt8  MEMBER_ cargo_life_supp_class _MEMBER
UInt8  MEMBER_ trailhook _MEMBER
UInt8  MEMBER_ telnet _MEMBER
UInt8  MEMBER_ IFF _MEMBER
UInt8  MEMBER_ radiomon _MEMBER
UInt8  MEMBER_ cloak_class _MEMBER
UInt8  MEMBER_ FTL _MEMBER
UInt8  MEMBER_ weapon_bay_cooling_class _MEMBER
UInt8  MEMBER_ scoop _MEMBER
UInt8  MEMBER_ freedock _MEMBER
UInt8  MEMBER_ mines _MEMBER
UInt8  MEMBER_ hypertransport_count _MEMBER
UInt8  MEMBER_ hypertransport_max_count _MEMBER
UInt8  MEMBER_ tempmult_class _MEMBER

SInt8   cargo_bay [MAX_CARGO_BAY_SIZE] ;
UInt8  MEMBER_ was_loaded_from_sds _MEMBER	//tell load_dyn_object not to override params loaded from file -i.e. mass

// CONTROL INPUTS

//these next lot are the thrusters that can be controled 'from the stick'
//can range from 0 (off) to 255 (max)
//When controllers change the thrust of a thruster they should update
//these so the engine can update the low level physics and draw the exhausts correctly
UInt8 MEMBER_ command_mthr1 _MEMBER
UInt8 MEMBER_ command_mthr2 _MEMBER
UInt8 MEMBER_ command_mrev1 _MEMBER
UInt8 MEMBER_ command_mrev2 _MEMBER
UInt8 MEMBER_ command_vthr1 _MEMBER
UInt8 MEMBER_ command_vthr2 _MEMBER
UInt8 MEMBER_ command_rvth1 _MEMBER
UInt8 MEMBER_ command_rvth2 _MEMBER

SInt8 MEMBER_ command_pitch _MEMBER	//+ is up, - is down
SInt8 MEMBER_ command_roll _MEMBER      //+ is roll starboard, - is port (left)
SInt8 MEMBER_ command_yaw _MEMBER       //+ is yaw starboard, - is port

#define ORCS_DAMPER_ROT 1
#define ORCS_DAMPER_LIN 2

UInt8 command_ORCS_damper;	// ORCS applies linear/rotational drag if bits set

//When an object fires a laser, cannon or ejection pod it should set one of these flags 
// in command_fired_flags
#define COMMAND_LASER1_FIRED 1
#define COMMAND_LASER2_FIRED 2
#define COMMAND_LASER3_FIRED 4
#define COMMAND_LASER4_FIRED 8
#define COMMAND_GUN1_FIRED 16
#define COMMAND_GUN2_FIRED 32
#define COMMAND_GUN3_FIRED 64
#define COMMAND_GUN4_FIRED 128
#define COMMAND_EJECT_FIRED 256

#define COMMAND_MISSILE1_FIRED 512
#define COMMAND_MISSILE2_FIRED 1024
#define COMMAND_MISSILE3_FIRED 2048
#define COMMAND_MISSILE4_FIRED 4096	//not used

UInt32 MEMBER_ command_fired_flags _MEMBER

//Laser temp
float laser_bay_temperature_in_C;

//fuel control variables
double fuel_tank_value_in_KGS;
Float32 reverse_thrust_consumption_per_second_in_KGS; //per engine
Float32 main_thrust_consumption_per_second_in_KGS;	//per engine

//private ORCS data - do not twiddle with this; tis for the ORCS 
//and AP use exclusively
float  ORCS_thrust_up;
float ORCS_thrust_down;

float ORCS_extern_roll;
float ORCS_extern_pitch;
float ORCS_extern_yaw;

float yaw_channel;
float roll_channel;
float pitch_channel;

//AP data - calculated in autopilot.c
UInt32 command_autopilot_engaged;	//from controller
float AP_timer_1;	//used to calculate course of object
float ap_ht_engage_timer;
float AP_thr_lock_out_timer;	//no thrust until this is <=0

_3D AP_old_target_pos_in_m;	//where the target was when we started AP_timer_1
_3D AP_target_end_position_in_m;	//where we predict the target to be at end of flight


float time_to_target_in_secs_as_crow_flies;	//from flight
float AP_brake_time;	//at what distance we should start braking
float AP_brake_distance;
float stop_time_in_secs;
float current_relative_velocity;
float current_abs_velocity;
float last_distance_to_target;	//for velocity sign calc
float AP_pull_up_timer;	//combat pull up down timer 

float ht_accel; //this value is multiplied by the distance to target to give an incrementing accel

UInt8 AP_roll_lock_out;
#define AP_WAITING_FOR_TIMER 1
#define AP_LOCKED 2
#define AP_CALCD_FLAG 4
#define AP_BRAKE_FLAG 8
#define AP_DROP_TO_STEER_FLAG 16
#define AP_BRAKING_PHASE_FLAG 32
#define AP_THRUST_LOCK_OUT_FLAG 64
#define AP_ORBITAL_STAB_FLAG 128
UInt8 AP_flags;
//Power systems status - calculate in power.c
UInt8 DC_power_volts;
UInt8 AC_power_volts;
UInt8 DC_power_amps;
UInt8 AC_power_amps;
#define AC_GOOD 1
#define DC_GOOD 2
#define POWER_ON 4
UInt8 power_flags;


//Engines data
float actual_main_thrust;
int eclipse_engine_sound_ref;	//either a sound ref or -1 if too far away to be heard
float current_total_fuel_consumption;    //so we can calc what the thrust volume should be
float eng_temperature_in_C;

//Hull data
float hull_temperature_in_C;

END_STRUCT_DEF ZObjectControl END_DEF



// object def

START_STRUCT_DEF
int	MEMBER_ in_use  _MEMBER	// 1 if so, else 0
int MEMBER_ is_visible _MEMBER	            // 1 if so, else 0 from obj fov filter prior to main 3d calcs
int MEMBER_ eng_invisible _MEMBER	            // if 1, this objects polys go right the way through until final draw and then stop
                                            // used to make something invisible
int MEMBER_ targetted _MEMBER		//zex has targetted this object
OSType MEMBER_ Oid _MEMBER	                      // E.G. B001 - four character generaic identifier
int MEMBER_ Orotx _MEMBER
int MEMBER_ Oroty _MEMBER
int MEMBER_ Orotz _MEMBER	        // object rotation from static def
// the following specify constant rotations around an axis as thrust around the axis in newtons per second
// object control top looks at these and calls apply thrust as appropriate
// this saves us reams of space in the OCB (otherwise we'd need three of:
// vector point, vector direction, loat magnitude

float MEMBER_ Orotdx _MEMBER
float MEMBER_ Orotdy _MEMBER
float MEMBER_ Orotdz _MEMBER	   // object delta rotation in newtons per second
_quat MEMBER_ Qor _MEMBER	                       // orientation quaternion

vector MEMBER_ torque_sum _MEMBER	               // rotational vector
vector MEMBER_ abs_velocity_vector _MEMBER       	// linear acceleration vector
vector MEMBER_ rel_velocity_vector _MEMBER       	// linear acceleration vector reloative to 
                                                       //target or nearest planet
int MEMBER_ rotate_around_object _MEMBER	//if not -1, engine rotates this object around this 
						// objects' parents center - e.g. a missile mounted on a wing is bolted to 
						//the parent
int MEMBER_ object_targetted _MEMBER
double MEMBER_ Oworldx _MEMBER
double MEMBER_ Oworldy _MEMBER
double MEMBER_ Oworldz _MEMBER // where the object is to go in the world

double MEMBER_ Ostartx _MEMBER
double MEMBER_ Ostarty _MEMBER // where the object started in the world - used by ssome monster controllers
float MEMBER_ scale _MEMBER
int MEMBER_ is_blobbed _MEMBER	//if 1 we just plot a point for this object
float MEMBER_ blobbed_colour_red _MEMBER	//the colour if blobbed 0-1
float MEMBER_ blobbed_colour_green _MEMBER	  //the colour if blobbed
float MEMBER_ blobbed_colour_blue _MEMBER	 //the colour if blobbed

int MEMBER_ parent _MEMBER	// who fired it etc
int MEMBER_ number_of_polys _MEMBER	// how many polys make up this object
int MEMBER_ number_of_points _MEMBER	// how many points
int MEMBER_ scaled_points_entry _MEMBER	// -1 if not scaled
_3D PTR MEMBER_ points_ptr _MEMBER	  	  // ptr to the points for this object
_3D PTR MEMBER_ world_points_ptr _MEMBER	// ptr to the transformed world points for this object
_ZPoly PTR MEMBER_ polys_ptr _MEMBER	// ptr to the polygon definitions
int MEMBER_ active_polys_slot _MEMBER	 // the index in the dynamic active polys array so we can kill them quickly
dynamic_texture PTR MEMBER_ dynamic_tex_ptr _MEMBER	// if object is using a dynamic texture this is the ptr else 0

_3D MEMBER_ n _MEMBER	 // the object normal
_3D MEMBER_ n_translated _MEMBER // translated object normal (which way it's facing)
_3D MEMBER_ v _MEMBER	 // the object normal vertical
_3D MEMBER_ v_translated _MEMBER // translated object normal (which way it's facing)
_3D MEMBER_ s _MEMBER	 // the object normal vertical
_3D MEMBER_ s_translated _MEMBER // translated object normal (which way it's facing)

//BOLTS - these allow us to construct complex objects from many other objects
//bolt_ref comes from LMON and is a unique reference for this object
//See 01.zlv for usage
unsigned int MEMBER_ unique_id _MEMBER	             // an id from LMON
//bolted_to comes from BOLT and is set to 1 if an object is bolted onto something else
//it stops the normal gravity moving an object
unsigned int MEMBER_ bolted_to _MEMBER	      //object this object is bolted to
float MEMBER_ bolt_x_offset _MEMBER
float MEMBER_ bolt_y_offset _MEMBER
float MEMBER_ bolt_z_offset _MEMBER

// various control items

float MEMBER_ radius _MEMBER	// bounds sphere of object


//These next lot are rotated BUT NOT translated; so to get the real bounding
//box you need to add on the objects Oworldx,y and z coords!
_3D MEMBER_ box_min _MEMBER	// collision box
_3D MEMBER_ box_max _MEMBER
_3D MEMBER_ box_min_translated _MEMBER	 // collision box
_3D MEMBER_ box_max_translated _MEMBER

//main engines
_3D MEMBER_ mthr1 _MEMBER
_3D MEMBER_ mthr1_translated _MEMBER
_3D MEMBER_ mthr2 _MEMBER
_3D MEMBER_ mthr2_translated _MEMBER
_3D MEMBER_ mrev1 _MEMBER
_3D MEMBER_ mrev1_translated _MEMBER
_3D MEMBER_ mrev2 _MEMBER
_3D MEMBER_ mrev2_translated _MEMBER

//vertical thrusters
_3D MEMBER_ vthr1 _MEMBER
_3D MEMBER_ vthr1_translated _MEMBER
_3D MEMBER_ vthr2 _MEMBER
_3D MEMBER_ vthr2_translated _MEMBER
_3D MEMBER_ rvth1 _MEMBER
_3D MEMBER_ rvth1_translated _MEMBER
_3D MEMBER_ rvth2 _MEMBER
_3D MEMBER_ rvth2_translated _MEMBER

//guns
_3D MEMBER_ gun01 _MEMBER
_3D MEMBER_ gun01_translated _MEMBER
_3D MEMBER_ gun02 _MEMBER
_3D MEMBER_ gun02_translated _MEMBER
_3D MEMBER_ gun03 _MEMBER
_3D MEMBER_ gun03_translated _MEMBER
_3D MEMBER_ gun04 _MEMBER
_3D MEMBER_ gun04_translated _MEMBER

//lasers
_3D MEMBER_ las01 _MEMBER
_3D MEMBER_ las01_translated _MEMBER
_3D MEMBER_ las02 _MEMBER
_3D MEMBER_ las02_translated _MEMBER
_3D MEMBER_ las03 _MEMBER
_3D MEMBER_ las03_translated _MEMBER
_3D MEMBER_ las04 _MEMBER
_3D MEMBER_ las04_translated _MEMBER

//missiles
_3D MEMBER_ mis01 _MEMBER
_3D MEMBER_ mis01_translated _MEMBER
_3D MEMBER_ mis02 _MEMBER
_3D MEMBER_ mis02_translated _MEMBER
_3D MEMBER_ mis03 _MEMBER
_3D MEMBER_ mis03_translated _MEMBER

//ejection point
_3D MEMBER_ eject _MEMBER
_3D MEMBER_ eject_translated _MEMBER


int MEMBER_ invincible_count _MEMBER	// if not zero, object is invincible
float MEMBER_ mass _MEMBER	  // in Tonnes
long MEMBER_ hit_by_slot _MEMBER	 // dynamic slot of object that hit this object else -1, so we can find info about what hit this object

UInt8 MEMBER_ crashable _MEMBER // if 1 then object is collidable
UInt8 MEMBER_ colltype _MEMBER	 // collision detection type. 3=default (sphere and box 1=sphere, 2=box
UInt8 MEMBER_ just_hit _MEMBER	 // object controllers use this to take evasive action. Inc'd in coll_det, dec'd by controller
UInt8 MEMBER_ no_trivial_rejection _MEMBER	 // should be set by monsters that must run when not in view
UInt8 MEMBER_ showing_shield _MEMBER //1 if so
//kill processing
#define KILL_METHOD 1
#define HAS_CHILDREN 2
UInt8 MEMBER_ kill_flags _MEMBER	 // 1 if so, kill sets in_use to 0 and calls controller one last time


// animation control - d10 - see 3d engine design under ANIMATION for details.
UInt8 MEMBER_ anim_control _MEMBER	 // 0=stop at end of cycle, 1=continuous
UInt8 MEMBER_ anim_state _MEMBER	// 0=stopped, 1=running
UInt8 MEMBER_ anim_frame_rate _MEMBER	 // how often objects are switched.
UInt8 MEMBER_ anim_counter _MEMBER	 // used in conjunction with anim-frame_rate
UInt8 MEMBER_ anim_frame _MEMBER
int MEMBER_ anim_max_object _MEMBER	// slot number of next object in sequence
anim_obj_bits MEMBER_ the_anim_ptrs ARRAY_ 6 _ARRAY _MEMBER


ZObjectControl MEMBER_ Dyn_OCB_control_data _MEMBER

END_STRUCT_DEF ZObject END_DEF





//Dynamic objects allocation
typedef struct
{
ZObject object_list[MAX_NUMBER_OF_DYN_OBJECTS];
} DynObjectsFixedSize,*DynObjectsFixedSizePtr,**DynObjectsFixedSizeHandle;

//Static Objects allocation
typedef struct
{
ZObject object_list[MAX_NUMBER_OF_STAT_OBJECTS];
} StatObjectsFixedSize,*StatObjectsFixedSizePtr,**StatObjectsFixedSizeHandle;

typedef struct
{
_ZPoly *poly_ptr;	//the poly
_3D point1,point2,point3; //the points
} visible_poly;

typedef struct
{
int previous_slot;	//z before
int next_slot;	//next z (double linked list)
double z_value;

_ZPoly *poly_data;
} _ZPolySort;

//typedef struct // if you modify this, you will need to modify the forth version
//{
//double world_x,world_y,world_z;	//the position of the object
//float delta_rot_x,delta_rot_y,delta_rot_z;
//int rot_x,rot_y,rot_z;
//int controller_ref,control_use_1,control_use_2;
//int control_use_3,control_use_4,control_use_7;
//int attack_mode,who_spawned_this_object;
//float mass;
//short shield_value;
//short max_vel,min_vel;	//maximum and minimum velocities
//
//UInt16 dq;	  //how much damage this thing does when hit - 080499
//UInt8 no_trivial_rejection,object_category;
//
//UInt8 aggressive;
//UInt8 _class; //fighter, traveller, guard
//UInt8 shields_class;	  //0 not rechargeable, >0 recharge rate per frame
//
/////following 4 added 160199
//UInt8 laser_cat;
//UInt8 cannon_cat;
//UInt8 missile_cat;
//UInt8 number_of_missiles;
//UInt8 has_kill_method;
//UInt8 invisible_flag;
//_3D normal;
//int anim_frame_rate;	  //how often objects are switched.
//int anim_max_object;	  //slot number of next object in sequence
//int unique_id;	//an id from the level file; generally zero but set for doors that need to be found by switches 
////anim_object_ptrs anim_points_polys;	//ptrs to points and polys for eaach object
//anim_obj_bits the_anim_ptrs[3];
//} load_dyn_object_pb;


// REMEMBER - if you modify this, you will need to copy
// it over to the Forth version in load_dyn_obj_pb.zsl
START_STRUCT_DEF

double MEMBER_ world_x _MEMBER
double MEMBER_ world_y _MEMBER
double MEMBER_ world_z _MEMBER	// the position of the object
float  MEMBER_ delta_rot_x _MEMBER
float  MEMBER_ delta_rot_y _MEMBER
float  MEMBER_ delta_rot_z _MEMBER

int    MEMBER_ rot_x _MEMBER
int    MEMBER_ rot_y _MEMBER
int    MEMBER_ rot_z _MEMBER
int    MEMBER_ controller_ref _MEMBER

int    MEMBER_ control_use_1 _MEMBER
int    MEMBER_ control_use_2 _MEMBER
int    MEMBER_ control_use_3 _MEMBER
int    MEMBER_ control_use_4 _MEMBER
int    MEMBER_ control_use_5 _MEMBER
float  MEMBER_ control_use_1_float _MEMBER
int    MEMBER_ attack_mode _MEMBER
int    MEMBER_ parent _MEMBER

float MEMBER_ mass _MEMBER
short MEMBER_ shield_value _MEMBER
short MEMBER_ max_vel _MEMBER
short MEMBER_ min_vel _MEMBER	 // maximum and minimum velocities

UInt16  MEMBER_ dq _MEMBER	// how much damage this thing does when hit - 080499
UInt8 MEMBER_ no_trivial_rejection _MEMBER
UInt8 MEMBER_ object_category _MEMBER

UInt8 MEMBER_ aggressive _MEMBER
UInt8 MEMBER_ NPC_class _MEMBER          // fighter, traveller, guard
UInt8 MEMBER_ shields_class _MEMBER   // 0 not rechargeable, >0 recharge rate per frame

// following 4 added 160199
UInt8 MEMBER_ laser_cat _MEMBER
UInt8 MEMBER_ cannon_cat _MEMBER
UInt8 MEMBER_ missile_cat _MEMBER
UInt8 MEMBER_ number_of_missiles _MEMBER

UInt8 MEMBER_ kill_flags _MEMBER
UInt8 MEMBER_ invisible_flag _MEMBER

_3D MEMBER_ normal _MEMBER
int MEMBER_ anim_frame_rate _MEMBER	// how often objects are switched.
int MEMBER_ anim_max_object _MEMBER	// slot number of next object in sequence
int MEMBER_ unique_id _MEMBER	// an id from the level file; generally zero but set for doors that need to be found by switches 

anim_obj_bits MEMBER_ the_anim_ptrs ARRAY_ 3 _ARRAY _MEMBER

int MEMBER_ init_check _MEMBER

END_STRUCT_DEF load_dyn_object_pb END_DEF

// *********************FAST OCBS FOR STARS*******************************

//Fast, simple objects - These are designed for very simple objects. These are drawn first before the
//normal OCBS - use for background graphics - stars for instance.
//Load an object from static to these with load_fdyn_object(blah blah);
//Limitations on these objects:
//Not z-buffered
//Don't appear on the radar
//Cant be targetted
//Don't have names
//Cant move
//Are drawn before all other objects

typedef struct {
int in_use; int is_visible;
OSType Oid;
_quat MEMBER_ Qor _MEMBER	                       // orientation quaternion
double MEMBER_ Oworldx _MEMBER
double MEMBER_ Oworldy _MEMBER
double MEMBER_ Oworldz _MEMBER	// the position of the object
float MEMBER_ scale _MEMBER
int MEMBER_ number_of_polys _MEMBER	// how many polys make up this object
int MEMBER_ number_of_points _MEMBER	// how many points
int MEMBER_ scaled_points_entry _MEMBER	// -1 if not scaled
_3D PTR MEMBER_ points_ptr _MEMBER	  	  // ptr to the points for this object
_3D PTR MEMBER_ world_points_ptr _MEMBER	// ptr to the transformed world points for this object
_ZPoly PTR MEMBER_ polys_ptr _MEMBER	// ptr to the polygon definitions
int MEMBER_ active_polys_slot _MEMBER	 // the index in the dynamic active polys array so we can kill them quickly
dynamic_texture PTR MEMBER_ dynamic_tex_ptr _MEMBER	// if object is using a dynamic texture this is the ptr else 0

float MEMBER_ radius _MEMBER	// bounds sphere of object
float MEMBER_ mass _MEMBER
int controller;
int pad1;
float control_use_1_float;
UInt8 MEMBER_ kill_flags _MEMBER	 // 1 if so, kill sets in_use to 0 and calls controller one last time


} ZFObject;


typedef struct
{
ZFObject Fobject_list[MAX_NUMBER_OF_FAST_DYN_OBJECTS];
} DynFObjectsFixedSize,*DynFObjectsFixedSizePtr,**DynFObjectsFixedSizeHandle;

// *********************PARTICLES OCBS *******************************
//Particles OCB
typedef struct {
short previous; 
short next;	//Linked list of active particle objects

short is_visible;
OSType Oid;
_quat MEMBER_ Qor _MEMBER	                       // orientation quaternion
double MEMBER_ Oworldx _MEMBER
double MEMBER_ Oworldy _MEMBER
double MEMBER_ Oworldz _MEMBER	// the position of the object

vector MEMBER_ abs_velocity_vector _MEMBER       	// linear acceleration vector

int MEMBER_ number_of_polys _MEMBER	// how many polys make up this object
int MEMBER_ number_of_points _MEMBER	// how many points
_3D PTR MEMBER_ points_ptr _MEMBER	  	  // ptr to the points for this object
_3D PTR MEMBER_ world_points_ptr _MEMBER	// ptr to the transformed world points for this object
_ZPoly PTR MEMBER_ polys_ptr _MEMBER	// ptr to the polygon definitions
int MEMBER_ active_polys_slot _MEMBER	 // the index in the dynamic active polys array so we can kill them quickly
dynamic_texture PTR MEMBER_ dynamic_tex_ptr _MEMBER	// if object is using a dynamic texture this is the ptr else 0

float MEMBER_ radius _MEMBER	// bounds sphere of object
float MEMBER_ mass _MEMBER
int controller;
float control_use_1_float;
UInt8 MEMBER_ kill_flags _MEMBER	 // 1 if so, kill sets in_use to 0 and calls controller one last time

} ZPObject;


typedef struct
{
int head_index;
int tail_index;
ZPObject Pobject_list[MAX_NUMBER_OF_PARTICLE_DYN_OBJECTS];
} DynPObjectsFixedSize,*DynPObjectsFixedSizePtr,**DynPObjectsFixedSizeHandle;



typedef struct
{
unsigned int object_id[20];
} anim_pb;


typedef struct
{
int cmap_objs[NUMBER_OF_CMAP_OBJS];
} cmap_objects;

//some macros to get sin and cos of an angle between 0 and 359
//#define SIN(x) SinTable[((UInt32)x&255)]
//#define COS(x) CosTable[((UInt32)x&255)]	//safe (slower) accessors

#define SIN(x) SinTable[((UInt32)x)]
#define COS(x) CosTable[((UInt32)x)]	//unsafe accesors

extern	double SinTable[360], CosTable[360];
