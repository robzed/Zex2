// *****************************************************************MODULE HEADER*****
// * FILENAME		: processor.ext
// * MODULE TITLE	: processor related functions external (public) definitions.
//
// * PROJECT		: Zex
// *
// * COPYRIGHT (c) 1999 Lightsoft
// *           (c) 1999 Robert Probin and Stuart Ball
// *		   http://www.lightsoft.co.uk/
// *
// * Lightsoft is a trading name of Robert Probin and Stuart Ball.
// *
// ***********************************************************************************



void init_cache_line_size(char *big_buffer);
int get_cache_line_size(void);
int InRange(int value, int min, int max);
long abs_nobranch (long i);
void reset_microsecs_timer(void);
void update_microsecs_timer(void);

void freeze_physics(void);
void unfreeze_physics(void);
int physics_frozen(void);


extern double time_in_secs;	// this is "game_frame_time_in_secs"
extern double real_frame_time_in_secs;

void InitThreads();
void SetHighPriority();
void SetNormalPriority();
void thread_yield();
