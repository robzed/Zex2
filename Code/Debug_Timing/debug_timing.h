// *****************************************************************MODULE HEADER*****
// * FILENAME		: debug_timing.ext
// * MODULE TITLE	: debug timing external (public) definitions.
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


extern double timemin[];
extern double timemax[];
extern double timetotal[];
extern double timeav[];
extern double timepercent[];

extern int game_timing;
extern int frames_to_reset_after;

extern int timed_frames;
extern int total_frames;


void T(int routine_code);


#ifndef PCI_TIMING_DEBUG
#error "options.def" not included where "debug_timing.ext" is included!!
#endif


#define T_COFFEE 0
#define T_MISC 1
#define T_SWAP 2
#define T_MATRIX 3

#define T_CULLING 4
#define T_PERS 5
#define T_POLYDRAW 6
#define T_DLP 7



#define T_MIN 0
#define T_MAX 7

