//if this structure changes then you MUST update ship_data_system.zsl as it is that which fills this data
//this data eventually ends up in the object control data and the mounting points in the OCB


 
//selectors for get_UInt32_SDS_value
enum {
//mounting points
MTHR1V,MTHR2V,	//main thrust and value
MREV1V, MREV2V, //main reverse thrust and value
VTHR1V, VTHR2V, //vertical thrust and value
RVTH1V, RVTH2V, //reverse vertical thrust and value
GUN01V, GUN02V, GUN03V, GUN04V,	//guns and class
LAS01V, LAS02V, LAS03V, LAS04V, //lasers and class
MIS01V, MIS02V,MIS03V, 	//missile pylons and class
EJECTV,			//ejection point and class

//basic data values - 5 bits. These MUST be defined consecutively else it'll break!
SHIELD1V, SHIELD2V, SHIELD3V, //shield class
AP_CLASSV, RADAR_CLASSV, BATT_CLASSV, ORCSV,	//ap,radar,batt and orcs class
REPAIRV, SHOCKV, ECMV, MISSFACTV,	//repair systems, shockwave, ecm, missile factory class
FASTDOCKV, INSURANCEV, FMAXV, RETROROCKSV,	//fastdock, insurance, fuel maximiser, retrorockets class
NAVCOMPV, TRADINGCOMPV, HULLV, 	//nav computer, tading computer, hull repair class
CARGOLIFEV, TRAILHV,TELNETV, IFFV, RADIOV,	//cargo life support, trailer hook, telnet, iff and radio class
CLOAKV, FTLV, WCOOLINGV, 	//cloak, ftl and weapons cooling class
SCOOPV, FREEDOCKV, MINESV,HTRNV, HTRNMAX,TEMPM, 	//scoop, freedock, mines class and number of hyper transports

// non status values - no status bits on these
MASSV, FUELCAPV, 	//mass in tonnes, fuel capacity in kgs
ROTPITCHV, ROTROLLV, ROTYAWV,	//rotational thrusters in newtons/sec
CARGOCAPV, HULLCLASSV,	//cargo cvapacity, hull class


};


//for basic 8 bit data
typedef struct {
unsigned char the_value;
unsigned char   the_bits;
} sds_attrib_data_type;

//for mounting points
typedef struct {
_3D             the_position;
float           the_value;
unsigned char   the_bits; //2=can be fitted   1,0 = 00=not fitted, 01=damaged, 10= serviceable
} sds_mount_data_type;

//non status values return as a float



typedef struct {
unsigned long id;
//  point         value
_3D mthr1; Float64 mthr1v;	 //( main thrust 1 )
_3D mthr2; Float64 mthr2v;	 //( main thrust 2 )
_3D mrev1; Float64 mrev1v;  //( main reverse thrust )
_3D mrev2; Float64 mrev2v;  //( main reverse thrust 2 )
_3D vthr1; Float64 vthr1v;  //( vertical thruster 1 )
_3D vthr2; Float64 vthr2v;  //( vertical thrust 2 )
_3D rvth1; Float64 rvth1v;  //( inverse vertical thrust 1 )
_3D rvth2; Float64 rvth2v;  //( inverse vertical thrust 2 )
_3D gun01; Float64 gun01v;  //( gun 1 )
_3D gun02; Float64 gun02v;  //( gun 2 )
_3D gun03; Float64 gun03v;  //( gun 3 )
_3D gun04; Float64 gun04v;  
_3D las01; Float64 las01v; //( laser 1 )
_3D las02; Float64 las02v;
_3D las03; Float64 las03v;
_3D las04; Float64 las04v;
_3D mis01; Float64 mis01v;	 //( missile pylons )
_3D mis02; Float64 mis02v;	 //( missile pylons )
_3D mis03; Float64 mis03v;	 //( missile pylons )
_3D eject; Float64 ejectv; //( ejection point )

Float32 mass_tonnes;
Float32 fuel_capacity_kgs;

Float32 rotational_pitch;
Float32 rotational_roll;
Float32 rotational_yaw;

Float32 cargo_capacity_tonnes;
Float32   hull_class;

//lower bit of the following is can be fitted bit
//integral items with hull
UInt32   shields1_class;
UInt32   shields2_class; 
UInt32   shields3_class; 
UInt32   AP_class; 
UInt32   radar_class; 
UInt32   batt_class ;
UInt32   ORCS ;

UInt32   repair_system_class; 
UInt32   shockwave_class ;
UInt32   ECM_class ;
UInt32   missfact_class; 
UInt32   fastdock_class ;
UInt32   insurance_class ;
UInt32   fuel_maximiser_class; 
UInt32   retrorocks_class ;
UInt32   navcomp_class ;
UInt32   ejection_class ;
UInt32   trading_comp_class ;
UInt32   hull_repair_class ;
UInt32   cargo_life_supp_class; 
UInt32   trailhook ;
UInt32   telnet ;
UInt32   IFF ;
UInt32   radiomon; 
UInt32   cloak_class; 
UInt32   FTL ;
UInt32   weapon_bay_cooling_class; 
UInt32   scoop ;
UInt32   freedock; 
UInt32   mines ;
UInt32   hypertransport_count;
UInt32   hypertransport_max_count;
UInt32   tempmult_class; 
} mount_pb_type;

extern ZObjectControl player_kit;
//this function initialises the mount info structure
//called from init and coffee
//exported to coffee
void init_ship_data (void);


//this function loads a new mount_pb into the next slot of the mount_info array
//and updates the index of mount pb's
//exported to coffee
void load_ship_data (mount_pb_type* mount_pb);

//this function updates all static definitions.
//it also updates all currently loaded dynamic ocbs so objects do not need to be dismissed
//and summoned again 
//exported to coffee
void sds_update(void);

//this function is called from load_object to see if mounting point info exists
//for a given id
//returns either 0 or a pointer to the mounting point info
mount_pb_type* mount_exists (unsigned long the_id);



//sds access
UInt32 get_main_thrust_eng_1(int object_slot_number);
UInt32 get_main_thrust_eng_2(int object_slot_number);
UInt32 get_main_thrust_total(int object_slot_number);



void get_ship_mount_data (int object_slot_number, unsigned long selector, sds_mount_data_type* return_data);
void get_ship_attrib_data (int object_slot_number, unsigned long selector, sds_attrib_data_type * return_data);
float get_ship_non_status_value (int object_slot_number, unsigned long selector);

void set_ship_thruster (int object_slot_number, unsigned long selector, UInt8 magnitude);
void set_ship_rotational_thruster (int object_slot_number, unsigned long selector, SInt8 magnitude);     //selector = ROTPITCHV, ROTROLLV, ROTYAWV

void set_ship_item_fired (int object_slot_number, UInt32 the_bit);	//one of COMMAND_LASER1_FIRED etc


//misc cumulative items for sim
float get_reverse_thruster_power_in_newtons (int the_object);
float get_main_thruster_power_in_newtons (int the_object);



void copy_kit_from_ship(int object_number);
void copy_kit_to_ship(int object_number);
void get_kit_attrib_data (unsigned long selector, sds_attrib_data_type * return_data);
void decrement_ship_attrib_data (int object_slot_number, unsigned long selector);
