//item types enum list for SHIP systems, not for buy

enum {
//these can be damaged and repaired
KRADAR,	//NORMAL RADAR
KLONG_RANGE_RADAR, 
KCANNON,
KLASER,	//FRONT LASER
KMISSILES,	//MISSILE TARGETTING COMPUTER
KREPAIR_SYSTEMS,	//CAN BE 1 OR 2
KFLIGHT_COMPUTER,	//FLIGHT COMPUTER - ORientation Control System - ORCS
KENGINE,
//these cant be damaged
KMISSILE_PYLON1,KMISSILE_PYLON2,KMISSILE_PYLON3,	//3 MISSILE PYLONS
KSHIELDS,	//SHIELDS
KNORMAL_FUEL_TANK,	//NORMAL FUEL

//things from here on can be LOST
KREAR_LASER,
K360,	//360 DEGREES TARGETTING
KSHOCKWAVE,KECM,KMISSILE_FACTORY,

KFAST_DOCK, KFLATPACK, KFUEL_MAXIMISER,KRETRO_ROCKETS,
KHYPERSPACE,
KNAV_COMPUTER,	//RADAR VECTORS
KEJECTION,	//EJECTION POD

KTRADING_COMPUTER, KAUTO_HULL_REPAIR, KBOID_ADDITION,
KASTEROID_ADDITION,	//weapons do more damage to asteroids
KLAST,	//not used but used as a count of items for array dimensioning
};

#define ITEMS_COUNT KLAST

//kit_description
//status=
//0=not fitted
//1 fitted and serviceable
//2, 3 Unserviceable
//type is used to indicate what the item is as defined in the item types enum list in buy.int
//value is a controller specific thing - for example fuel quantity or missile type
//damage is a number between 0 and 100 - where 100 is not damaged and 0 is completely knackered!
//and wil lresult in status becoming zero.
//the class is the basic class - weapons ,equipment etc.
//modifier for example sheilds strength 3 - the subclass contains 3
typedef struct
{
int status;
float  value1;
unsigned char damage,_class,modifier,pad1; } kit_desc;

//this next structure is updated from ship_control
//and defines various power variables
typedef struct
{
	float battery_charge_state;	//0-100%
	float DC_voltage;		  //0-28VDC
	Boolean DC_on;	    	    //yes/no
	
	float AC_voltage;		  //0-400VAC - needs fuel
	Boolean AC_on;	    	    //yes/no
	
	Boolean fuel_status;	    //have fuel/no fuel
	
	Boolean engine_running;	//yes/no
} ship_power_status_type;





#define SERVICEABLE 1
#define UNSERVICEABLE 2
#define LOST 0

