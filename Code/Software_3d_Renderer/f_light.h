// f_light.h


#define NUM_LIGHT_LEVELS 16

#if NUM_LIGHT_LEVELS==16
  #define LIGHT256_SHIFT 4
  #define LIGHT_SHIFT 4
#elif NUM_LIGHT_LEVELS==32
  #define LIGHT256_SHIFT 3
  #define LIGHT_SHIFT 5
#elif NUM_LIGHT_LEVELS==64
  #define LIGHT256_SHIFT 2
  #define LIGHT_SHIFT 6
#elif NUM_LIGHT_LEVELS==128
  #define LIGHT256_SHIFT 1
  #define LIGHT_SHIFT 7
#endif

extern char light_table[256*NUM_LIGHT_LEVELS];
extern char transparency_table[256*256];

#define LIGHT_SHADED_TEXTURES 1
