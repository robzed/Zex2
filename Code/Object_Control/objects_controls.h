//object_controls.h
//object controller definitions
#define zex	0
#define zexp2 1
#define CAMERA_CONTROL	2
#define PLAYER_BULLET_NOT_SO_WEENY 3
#define DUST 4
#define dust2 5
#define exploding_object 6
#define ejection_cap 7	
#define ship_laser1	8
#define explosion 9	//the ball we explode when something gets destroyed
#define burning_particle 10	//these change colour over time from white to red to orange to yellow
#define missile1 11
#define missile1b 12
#define missile1c 13
#define missile1d 14
#define LARGE_SHIELD 15
#define zex_shield 16
//upto 19 are reserved slots for things that are used on every level such as weapons

#define monster1 20
#define monster2 21
#define monster3 22
#define gun1 23
#define monster4 24
#define asteroid1	25
#define star_for_story 26	//only used in story/credits screen
#define space_cow 27
#define space_cow_child 28
#define MOTHERSHIP_CONTROL 29
#define planet_rot_y 30
#define planet_rot_x 31
#define planet_rot_z 32
#define dlp_exploding_object 33
#define monster6 34
#define boid 35
#define exploding_zex 36
#define player_ejection_pod 37
#define REAR_SHIP_LASER 38
#define wingman1 39
#define wingman2 40
#define warper 41
#define static_laser1 42
#define tumbling_to_explode_object 43	//sometimes before exploding an object we switch it to this which causes
                                        //it to tumble before ulitmately exploding
#define persistent_asteroid 44	//same as asteroid but doesnt die when out of range

#define DOOR_VERTICAL_SLIDING_DOWN_SHOT 45

#define EXCALABUR 46
#define SAUCER 47
#define DUST_FOR_END 48	//appears in front of camera as we move out on death

#define DOOR_VERTICAL_SLIDING_DOWN_SWITCHED 49
#define DOOR_SWITCH 50
#define DUST_FOR_HOLE 51	//warphole dust
#define STAR_FOR_WARPHOLE 52
#define CARGO_CONTAINER 53
#define ALIEN_ACTIVE_MOTHERSHIP	54 //these generate nasty ships
#define EARTH_ACTIVE_MOTHERSHIP	55 //these generate earth ships
#define SHUTTLE 56
#define ALIEN_TRANSPORTER 58
#define EARTH_TRANSPORTER 59
#define NPC_CONTROL 60
#define GAME_MOTHERSHIP	61 //these generate nasty ships
#define WINGMAN_FOR_GAME 62
#define PERSISTENT_CARGO_CONTAINER 63
#define SUN 64
#define PLANET_CONTROL 65
#define VV_IND 66
#define CLOUD_CONTROL 67	//far off dust cloud
#define FAR_STAR 68	//far off star
#define NO_CONTROLLER 69
#define MOTHERSHIP_PART 70
#define FALCON 71
//
#define PARTICLE_GENERATOR_1 80 //laser
#define PARTICLE_GENERATOR_CANNON 81 //laser
#define PARTICLE_GENERATOR_EXHAUST 82

#define LASER_CONTROL_1   160 //individual particle controller for lasers
#define CANNON_CONTROL_1   161 //individual particle controller for cannons
#define PARTICLE_CONTROL_EXHAUST   162 //individual particle controller for cannons




#define FALCON_CONTROL  500


 
