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