//LS_Standard.h
#define PI 	((float)3.1415926535898)
#define ABS(x)	((x)>0?(x):-(x))
#define NO 0
#define YES 1
#define GOOD 1
#define BAD 1
#define TRUE 1
#define FALSE 0

//Below this line aa project specific things that should really go in another file
#define no_colldet NO
#define yes_colldet YES	//sphere, fast and inaccurate
#define BOX_COLLDETECT 2	//box - slow
#define BS_COLLDETECT 3	//box and sphere - fast and accurate
#define no_texture 0
#define fpoly	//use fpoly if defined else use system poly

#define EVER ;;	//as in for (EVER)



#define MONSTER_Z -200

//texture mapping constants
#define BITMAP_SIZE 128

typedef struct
{
int top,left,bottom,right;
} LSRect;

typedef struct
{
float top,left,bottom,right;
} LSFloatRect;

//
// These were added to support the Z_dungeon code-set.
//
#ifdef  __cplusplus
typedef bool BOOL;
#else
typedef char BOOL;
#endif

#ifdef __cplusplus
  #define STATIC_CAST_TO_INT(x) static_cast<int>(x)
  #define STATIC_CAST_TO_UINT(x) static_cast<unsigned int>(x)
  #define STATIC_CAST_TO_LONG(x) static_cast<long>(x)
  #define STATIC_CAST_TO_UNSIGNED(x) static_cast<unsigned>(x)

// for testing - remove the casts
//  #define STATIC_CAST_TO_INT(x) (x)
//  #define STATIC_CAST_TO_UINT(x) (x)
//  #define STATIC_CAST_TO_LONG(x) (x)
//  #define STATIC_CAST_TO_UNSIGNED(x) (x)

#else
  #define STATIC_CAST_TO_INT(x) (int)(x)
  #define STATIC_CAST_TO_UINT(x) (unsigned int)(x)
  #define STATIC_CAST_TO_LONG(x) (long)(x)
  #define STATIC_CAST_TO_UNSIGNED(x) (unsigned)(x)
#endif


// this is the output channel for printf
#define OUTPUT_FP stderr		// this was stderr in SDL documentation

#define DEBUG_GENERAL_STATUS	1	// general debug info to console


#define FLUSH_ALL_EVENTS 0xffff		// mac specific constant ... what's it doing here? dunno.

