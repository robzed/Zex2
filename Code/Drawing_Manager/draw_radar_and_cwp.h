// ****************************
// Name:    		CPanel.C
// Description:		Controls Control Panel Updates
// 
// Author:			Stuart Ball
//
// ****************************

void display_control_panel();
void update_radar();
void run_cwp(void);

#define RADAR_RANGE_INIT  32e9 //  16000 KK's
#define RADAR_RANGE_SHORTEST  1e5 //  1 K's
#define RADAR_RANGE_MULTIPLIER 2 // 3.16227766	  // mulitply by this every press
#define RADAR_RANGE_LONGEST   4.294967296e14 // 5000 million km's (approx pluto orbit)

extern int radar_grat;
extern int do_throb;	//this is changed to 1 if aliens detected (Radar graticule)


typedef struct 
{
_3D the_vect;
int the_type;	//R'mparian, human, neutral
int is_target;	// temporary whilst target is zex centric
int Oid;        //object ID
int object_slot;
} radar_entry;

typedef struct {
int x1;
int y1;
int y2;
int the_colour;
} radar_display_entry;

