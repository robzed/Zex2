//load level
//SB 18/12/98
//See 3D engine design for notes
/*
 * $Log: load_game.c,v $
 * Revision 1.7  2003/09/27 20:41:41  robp
 * Fixed float->int warnings.
 *
 * Revision 1.6  2003/09/27 08:58:30  robp
 * Removal of Invalid Conversion Warning for C++ (I hate unsigned/signed chars)
 *
 * Revision 1.5  2003/09/26 19:20:49  robp
 * Alteration for C++ const means internal unless specified extern. Made header extern so that C compiler wouldn't complain.
 *
 * Revision 1.4  2003/09/22 20:52:02  stu_c
 * Zex:Weapons:Laser Bay temperature and overheat (in progress)
 *
 * Revision 1.3  2003/09/20 12:56:58  robp
 * Removed nested comments
 *
 * Revision 1.2  2003/09/14 12:18:46  stu_c
 * Removed some unused commented out code
 *
 * Revision 1.1.1.1  2003/09/05 22:35:57  stu_c
 * First Imported.
 *
 * Revision 1.25  2002/09/08 02:01:41  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.24  2002/08/27 21:00:07  stu_c
 * Colliding shells
 *
 * Revision 1.23  2002/07/28 17:11:09  stu_c
 * Exhausts and further particle work
 *
 * Revision 1.22  2002/04/13 15:32:22  stu
 * Rotation, Alycians, behaviour
 *
 * Revision 1.21  2002/03/10 02:47:22  stu
 * Fix for suns/particles not being rendered
 *
 * Revision 1.20  2002/03/09 15:16:11  stu
 * load the parts of alien motherships
 *
 * Revision 1.19  2002/02/24 23:58:12  rob
 * LMSP command for level loader
 *
 * Revision 1.18  2002/02/24 03:37:55  stu
 * TEST_SYSTEM around pobs
 *
 * Revision 1.17  2002/02/13 00:50:40  rob
 * POBs now manages player ship and velocity vector
 *
 * Revision 1.16  2002/02/02 23:47:59  rob
 * pre-load moon
 *
 * Revision 1.15  2002/01/29 22:55:38  rob
 * pobs change
 *
 * Revision 1.14  2002/01/27 18:05:41  stu
 * Unused variables removed
 *
 * Revision 1.13  2002/01/20 17:09:08  stu
 * Loading of new stars and new sun.
 *
 * Revision 1.12  2002/01/02 19:26:36  rob
 * Added key setup code
 *
 * Revision 1.11  2001/12/31 20:04:51  stu
 * load_level now takes pointer to filename rather than level number
 *
 * Revision 1.10  2001/12/29 01:10:17  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.9  2001/12/28 23:08:13  stu
 * Now include texture_management.h but doesn't use it.
 *
 * Revision 1.8  2001/12/27 19:48:28  stu
 * Additional development work for docking
 *
 * Revision 1.7  2001/11/24 19:29:29  stu
 * *** empty log message ***
 *
 * Revision 1.6  2001/11/05 20:01:50  stu
 * More OSX work
 *
 * Revision 1.5  2001/11/03 22:29:51  stu
 * Bundles
 *
 * Revision 1.4  2001/11/02 23:20:19  stu
 * Further OS X work. Enabled syscon logging.
 *
 * Revision 1.3  2001/11/02 19:31:02  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:28:38  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:02:22  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:06  rob
 * First Imported.
 *
 * Revision 1.10  2001/09/23 19:01:39  stu
 * AP work, improved number print, engines.
 *
 * Revision 1.9  2001/06/23 23:10:32  stu
 * 0.66
 *
 * Revision 1.8  2001/06/21 20:31:13  rob
 * make so bad load doesn't quit
 *
 * Revision 1.7  2001/06/13 18:19:57  rob
 * Clear pb stuff
 *
 * Revision 1.6  2001/06/01 21:50:10  stu
 * Changes so we only need one run-time folder for both test and real environments
 *
 * Revision 1.5  2001/05/28 21:40:22  stu
 * Major commit 280501
 *
 * Revision 1.4  2001/05/28 01:44:28  stu
 * 280501
 *
 * Revision 1.3  2001/04/01 18:34:35  stu
 * *** empty log message ***
 *
 * Revision 1.2  2001/01/15 19:56:29  stu
 * Bolts, fuel freeze
 *
 * Revision 1.1.1.1  2001/01/01 21:13:36  rob
 * First Imported.
 *
 * Revision 1.10  2000/12/11 19:47:51  stu
 * Light fix in 3d_top - calc_normal
 *
 * Revision 1.9  2000/12/10 03:31:41  stu
 * Calls to kill replaced with either calls to add_to_kill_list OR
i_know_what_im_doing_kill
 *
 * Revision 1.8  2000/12/03 23:52:22  rob
 * changed constants for object type to get uniformity
 *
 * Revision 1.7  2000/11/28 22:27:51  stu
 * ASteroid loaded
 *
 * Revision 1.6  2000/11/21 19:17:38  stu
 * Lurkers clean up
 *
 * Revision 1.5  2000/11/16 20:35:57  rob
 * orbit
 *
 * Revision 1.4  2000/09/20 23:49:38  stu
 * HUD/AP/Scaling
 *
 * Revision 1.3  2000/09/02 13:23:36  stu
 * Added velocity vector display to HUD. Uses new object HUD_VV.z.
 *
 * Revision 1.2  2000/08/27 21:41:52  stu
 * Lurkers clear up...
 *
 * Revision 1.1  2000/08/27 16:25:22  stu
 * Zex Mass changed to 1 tonne.
Changes for movement decoupling in time.
 *
 * Revision 1.0.0.1  2000/08/21 22:04:24  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <files.h>
    #include <fp.h>
  #endif
#include "3d_structures_def.h"
#include "Error_handler.h"
#include "zex_misc.h"
#include "options.h"
#endif //pch


#include "Zsound.h"
#include "meshtozex.h"
#include "load_objects.h"
#include "load_game.h"
#include "file_io.h"
#include "objects_controls.h"
#include "z_init.h"
#include	"splat_control.h"

#include	"txtsplat.h"
#include "ship_control.h"
#include "pictures.h"
#include "camera_control.h"

#include "galaxy.h"
#include "physics_gravity.h"
#include "obj_control_misc_routines.h"
#include "static_object_support.h"
#include "texture_management.h"

#include "POBs.h"
#include "Physics.h"

#include "keyboard.h"

int level_text_read_index;
char level_text[2048];	//level description goes here
int level_start_time_ticks,current_level;
int SOLR_executed;

//change 010601
//The game filename is game.zlv, the test filename is 99.zlv
void load_level (unsigned char* the_filename)
{
//extern FSSpec Zex_FSSpec;
//extern int difficulty;
extern int	static_object_index;	//how many static objects
//these following 5 are in level_control.c
//extern int music_volume;
extern int reset_brightness;
//int tens;
FSSpec the_file;
Handle the_file_data_H;
Ptr the_level_file;
unsigned char level_directory[]="\pL";
show_text_in_loading_screen("Zex: Load game\n");


//     current_level=the_level;
     
    level_start_time_ticks=TickCount();	//make a note of the start time of the level

//set difficulty to default 1 in case user forgets
reset_brightness=25000;	//default daytime mission

//can_do_next_level=0;
static_object_index=0;
reset_static_slots();	//clear out old static objects
unload_textures();
load_textures(); //now done in load_level

reset_objects();	//clear out old dynamic objects

	make_data_fsspec(level_directory,the_filename,&the_file);

	the_file_data_H=read_file(the_file);
    HLock(the_file_data_H);
    the_level_file=*the_file_data_H;
    SOLR_executed=0;	//Set in interpret file if we get SOLR command
    interpret_file(the_level_file);
	DisposeHandle(the_file_data_H);
    show_text_in_loading_screen("Zex: Running POBS\n");	

#if TEST_SYSTEM==0
    if (SOLR_executed==1)
	{
    force_object_orbit(0, my_planet() );	// force zex to orbit planet
    }
#endif
    reset_camera();

#if TEST_SYSTEM==0
    check_all_POBS();
#endif
	
}


int have_bpic;
Handle bpic;
LSRAW *bpic_ptr;

void GetBGRAW(FSSpec* the_file)
{
extern Handle bpic;
//short fid;

 have_bpic=0;
 return;
 
//remove old
 if (have_bpic==1) DisposeHandle(bpic);
 have_bpic=0;
 bpic=0;
//open res file
// fid=FSpOpenResFile(the_file,fsRdPerm);
// 
// bpic = (Handle) Get1Resource('BGIM',128);  //Get the Handle to the Resource 
// if (bpic==0 || ResError()!=0) have_bpic=0;
// else
// {
//  have_bpic=1;
//  DetachResource (bpic);
//  HLock ((Handle) bpic);
//  bpic_ptr = (LSRAW*)*bpic;
// }
////close res file
// CloseResFile(fid);
}

int load_player_ship(double x_in_m, double y_in_m, double z_in_m, short galactic_id )
{
load_dyn_object_pb the_params;

//set up ship
	clear_pb(&the_params);
	the_params.world_x = x_in_m/ZEX_UNIT_SIZE_IN_METERS;
	the_params.world_y = y_in_m/ZEX_UNIT_SIZE_IN_METERS;
	the_params.world_z = z_in_m/ZEX_UNIT_SIZE_IN_METERS;
	the_params.rot_z=270;
	the_params.controller_ref=zex;
	the_params.shield_value=100;
	the_params.shields_class=4;
	the_params.mass=1;	//basic ship weighs 1 tonne
	the_params.object_category=HUMAN;
	 the_params.laser_cat=1;
	 the_params.cannon_cat=1;
	 the_params.missile_cat=1;
	 the_params.number_of_missiles=3;

	load_dyn_object(zex,0,&the_params,galactic_id,8,-1,!DUST_BIT);	  //the object, the position (-1=next free)
        return 0; // dyn slot of player ship is ALWAYS 0
}

int load_player_vv(double x_in_m, double y_in_m, double z_in_m, short galactic_id )
{
load_dyn_object_pb the_params;
	
//set up hud VV indicator
	clear_pb(&the_params);
	the_params.world_x = x_in_m/ZEX_UNIT_SIZE_IN_METERS;
	the_params.world_y = y_in_m/ZEX_UNIT_SIZE_IN_METERS;
	the_params.world_z = z_in_m/ZEX_UNIT_SIZE_IN_METERS;
	the_params.rot_z=270;
	the_params.controller_ref=VV_IND;
	the_params.shield_value=0;
	the_params.mass=1;	//basic ship weighs 1 tonne
		 	 the_params.dq=0;
	the_params.object_category=HUMAN;

	load_dyn_object(31,1,&the_params,galactic_id,1,-1,!DUST_BIT);	  //the object, the position (-1=next free)
        return 1; // dyn slot of player velocity vector is ALWAYS 1
}


int load_camera(double x_in_m, double y_in_m, double z_in_m, short galactic_id )
{
load_dyn_object_pb the_params;
	
//set up hud VV indicator
	clear_pb(&the_params);
	the_params.world_x = x_in_m/ZEX_UNIT_SIZE_IN_METERS;
	the_params.world_y = y_in_m/ZEX_UNIT_SIZE_IN_METERS;
	the_params.world_z = z_in_m/ZEX_UNIT_SIZE_IN_METERS;
	the_params.rot_z=270;
	the_params.controller_ref=CAMERA_CONTROL; //2
	the_params.shield_value=0;
	the_params.mass=1;	//basic ship weighs 1 tonne
		 	 the_params.dq=0;
	the_params.object_category=HUMAN;

	load_dyn_object(31,2,&the_params,galactic_id,1,-1,!DUST_BIT);	  //the object, the position (-1=next free)
        return 2; // dyn slot of camera is ALWAYS 2
}


void interpret_file(Ptr the_level)
{
extern	  DynObjectsFixedSize *ocb_ptr;
//extern FSSpec Zex_FSSpec;
extern _3D camera_pos;
extern int monsters_can_fire;
extern int score_this_level,credits_this_level,time_this_level,monsters_killed_this_level;
extern int reset_brightness;

//Handle the_map_data_H;
//FSSpec the_file;
char the_line[512];	//max line size in a mesh file
char the_word[512];	//max length of a word from a line
char *source;		//walk through the file with this
int line_len=0;
//int line_pos=0;
int word_len=0,stop;		//loop control
decimal my_dec;
short ix;
short vp;
float dectof_result;	//for the OS routine numtostring
unsigned long command;
load_dyn_object_pb the_params;
anim_pb anim;
int fname_length,i,line_number=1;
Str255 filename;
int static_slot,frames_counter;
unsigned long id;
int colldetect;
unsigned long texture_id;
int dest_ocb,temp_int;
//float bg_x,bg_y;
//float delta_y;
int frame_rate,anim_state,anim_control;
char character;
int line_index;

//Rect fuel;
extern int score,credits,accumulated_game_time;
//copy score etc to this_level so we calc items this level
    score_this_level=score,
    credits_this_level=credits;
    time_this_level=accumulated_game_time;
    monsters_killed_this_level=0;


   show_text_in_loading_screen("load_game: Loading fixed objects\n");	


//stuff we always have to do - load in weapons 'n' things to our reserved slots (<40)
    load_object ("\pzex_game.z",zex,'ZEXS',1,'T009',IS_NOT_TRANSLUCENT);
    
    load_object ("\pstar2.z",DUST,'DST&',no_colldet,0,IS_NOT_TRANSLUCENT);	//earth dust - was DST2
//    load_object ("\pstar3.z",dust2,'DST1',no_colldet,0,IS_NOT_TRANSLUCENT);	  //rmparian dust
//    load_object ("\pejectcap.z",ejection_cap,'EC01',yes_colldet,'TB00',IS_NOT_TRANSLUCENT);
//    load_object ("\pSE_object.z",explosion,'EX01',no_colldet,0,IS_NOT_TRANSLUCENT);	  //our exploding ball

//Alien mship 1
 load_object ("\pring3Op.3dmf",14,'ST01',yes_colldet,0,IS_NOT_TRANSLUCENT); //3dmf's should NOT be assigned a static meshworks texture!
 load_object ("\pvert_tube32000Op.3dmf",15,'TUB1',yes_colldet,0,IS_NOT_TRANSLUCENT); //3dmf's should NOT be assigned a static meshworks texture!
 load_object ("\pring6Op.3dmf",16,'ST02',yes_colldet,0,IS_NOT_TRANSLUCENT); //3dmf's should NOT be assigned a static meshworks texture!

    //load_object ("\pcarg1.z",17,'CAR1',yes_colldet,0,IS_NOT_TRANSLUCENT);
    load_object ("\pcarg2.z",18,'CAR2',yes_colldet,0,IS_NOT_TRANSLUCENT);
    load_object ("\pcarg3.z",19,'CAR3',yes_colldet,0,IS_NOT_TRANSLUCENT);
    load_object ("\pcarg4.z",20,'CAR4',yes_colldet,0,IS_NOT_TRANSLUCENT);
    load_object ("\pCLOU.z",21,'CL1&',no_colldet,'NB&1',IS_TRANSLUCENT);	//dust cloud, always faces player. A long way away!
    load_object ("\pCLOU.z",22,'CL2&',no_colldet,'NB&2',IS_TRANSLUCENT);	//dust cloud, always faces player. A long way away!
    load_object ("\pCLOU.z",23,'CL3&',no_colldet,'NB&3',IS_TRANSLUCENT);	//dust cloud, always faces player. A long way away!
    load_object ("\pCLOU.z",24,'CL4&',no_colldet,'NB&4',IS_TRANSLUCENT);	//dust cloud, always faces player. A long way away!
    load_object ("\pPAR1.z",25,'PAR1',no_colldet,'PT&1',55);	//particle, note special draw code so engine uses colour from poly
    load_object ("\pPRC1.z",26,'PRC1',no_colldet,0,IS_NOT_TRANSLUCENT);	//dummy particle controller #1
//Particle with collision detection
    load_object ("\pPAR1.z",27,'PARC',yes_colldet,'PT&1',55);	//particle, note special draw code so         
//100 unit shield
    load_object ("\pshield100.z",17,'SHL1',no_colldet,0,IS_TRANSLUCENT);

//    load_object ("\pzex_docked.z",27,'ZEXD',no_colldet,'CP02',0);
   // load_object ("\pstar2OGL.z",30,'DST2',no_colldet,0,IS_NOT_TRANSLUCENT);	  //earth dust
    load_object ("\pHUD_VV.z",31,'SPH1',no_colldet,0,IS_NOT_TRANSLUCENT);	//HUD vv indicator
    load_object ("\past2.z",32,'AST1',yes_colldet,'T002',IS_NOT_TRANSLUCENT);	//asteroid
    load_object ("\pstar10.z",33,'ST1&',no_colldet,'STR1',56);	//star - note star drawing code
    load_object ("\pstar10.z",34,'ST2&',no_colldet,'STR2',56);	//star - note star drawing code
    load_object ("\pstar10.z",35,'ST3&',no_colldet,'STR3',56);	//star - note star drawing code
    load_object ("\pBDSC.z",36,'SUN&',no_colldet,'STR2',56);	//sun - note star drawing code
   

//Load up our ship, VV and camera objects into 0-2 respectively
POBs_load_player_ship(0.0, 0.0, MONSTER_Z*ZEX_UNIT_SIZE_IN_METERS);
POBs_load_player_vv(0.0, 0.0, MONSTER_Z*ZEX_UNIT_SIZE_IN_METERS);
POBs_load_camera(0.0, 0.0, MONSTER_Z*ZEX_UNIT_SIZE_IN_METERS);

//set up DEBUG Particle generator - laser 1
//	  the_params.world_x=0;
//	  the_params.world_y=0;
//	  the_params.world_z=0;
//	  the_params.delta_rot_x=0;
//	  the_params.delta_rot_y=0;
//	  the_params.delta_rot_z=0;
//	  the_params.rot_x=0;
//	  the_params.rot_y=0;
//	  the_params.rot_z=0;
//	  the_params.controller_ref=PARTICLE_GENERATOR_1;
//	  the_params.control_use_1=1000;	//gen a particle every 100mS
//	  the_params.control_use_2=0;
//	  the_params.control_use_3=0;
//	  the_params.control_use_4=0;
//	  the_params.control_use_7=0;
//	  the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	  the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	  the_params.shield_value=0;
//	  the_params.mass=1;	  //basic ship weighs 1 tonne
//	  	   	the_params.dq=0;
//	  the_params.object_category=HUMAN;
//	   the_params.laser_cat=0;
//	   the_params.cannon_cat=0;
//	   the_params.missile_cat=0;
//	   the_params.number_of_missiles=0;
//
//	   the_params.unique_id=0;
//     the_params.who_spawned_this_object=0;	  //Zex owns this object
//	  load_dyn_object(26,2,&the_params,-1,1,-1,!DUST_BIT);	  //the object, the position (-1=next free)	


//fred: 
   show_text_in_loading_screen("load_game: Interpreting level file\n");	
        level_text_read_index=0;

//now process level file
source=the_level;	//source is incremented through the file

stop=0;	//loop term

while (stop==0)
{
 line_len=get_line(source,the_line);
 
 if (line_len!=-1)
    {
     word_len+=get_word(the_word,&the_line[word_len]);
     if (word_len!=-1) //-1=cr
     {
      command=(the_word[0]<<24) + (the_word[1]<<16) +
      (the_word[2]<<8) + the_word[3];
//ееееееееееееcommand processing, all cut 'n' pasted - no apologies.
      switch (command)
      {
       case '****': break;
       
//       case 'DIFF':	  //difficulty level
//          {
//            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
//            ix=0;
//            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
//            dectof_result=dec2f (&my_dec);	  //number of objects in a column
//            difficulty=dectof_result;
//            break;
//          }
 
       case 'SOLR':
       	{
		   show_text_in_loading_screen("Zex: Setting up solar system\n");	
			SOLR_executed=1;
            set_up_solar_system();
            
       	break;
       	}
       	
       case 'LITE':	//brightness
          {
            reset_brightness=30000;
            break;
          }

       case 'NITE':	//brightness
          {
            reset_brightness=65500;
            break;
          }
         
       case 'FUEL':	//a refuel rectangle
          {

          }

       case 'LTXT': //level descriptor text
       character=13;
       line_index=0;
       while (character!=0 && level_text_read_index<1022)
       {
       character=the_line[word_len+line_index];	//get char from line
       if (character!=0)
       level_text[level_text_read_index]=character;
       else
       level_text[level_text_read_index]=13;
       line_index++;
       level_text_read_index++;
       }
            break;

       case 'SCRD':	//ship start coords
          {
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);	//number of objects in a column
            (*ocb_ptr).object_list[zex].Oworldx=dectof_result;
            camera_pos.x=dectof_result;	//set camera too
 
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);	//number of objects in a column
            (*ocb_ptr).object_list[zex].Oworldy=dectof_result;
            camera_pos.y=dectof_result;	//set camera too

            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);	//number of objects in a column
            (*ocb_ptr).object_list[zex].Oworldz=dectof_result;
            camera_pos.y=dectof_result;	//set camera too

            break;
          }

       case 'MONF':	//monsters can fire - 1 they do, 0 they dont
          {
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);	//number of objects in a column
            (*ocb_ptr).object_list[zex].Oworldx=dectof_result;
            monsters_can_fire=STATIC_CAST_TO_INT(dectof_result);	//set camera too
             break;
          }

       case 'TARG':	//target of level
          {
		//not used in zex 2
            break;
          }

       case 'DEST':	//destination once level is complete for nav
          {
//            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
//            ix=0;
//            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
//            dectof_result=dec2f (&my_dec);	  //number of objects in a column
//            dest_x=dectof_result;	//set camera too
//            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
//            ix=0;
//            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
//            dectof_result=dec2f (&my_dec);	  //number of objects in a column
//            dest_y=dectof_result;	//set camera too
            break;
          }


       case 'RFIL':	//load an object
          {
            //get iflename as pascal string
            fname_length=get_word(the_word,&the_line[word_len]);
            word_len+=fname_length;
            fname_length--;	//account for param separator
            for (i=0;i<fname_length;i++)
             {
             filename[i+1]=the_word[i];
             }
             filename[0]=fname_length;
              
            //get ID
            word_len+=get_word(the_word,&the_line[word_len]);
            id=(the_word[0]<<24) + (the_word[1]<<16) +
            (the_word[2]<<8) + the_word[3];
            //get static_object slot
            word_len+=get_word(the_word,&the_line[word_len]);
            dectof_result=Zconvert_to_float(the_word);

//            ix=0;
//            vp=0;
//            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
//            //dectof_result=dec2f (&my_dec);
//            dectof_result=dec2num (&my_dec);
            static_slot=STATIC_CAST_TO_INT(dectof_result);
            //get colldet
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            ix=0;
            vp=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
            colldetect=STATIC_CAST_TO_INT(dectof_result);	//0-3
            //get texture ID
            the_word[0]=0;the_word[1]=0;the_word[2]=0;the_word[3]=0;
            fname_length=get_word(the_word,&the_line[word_len]);
            word_len+=fname_length;

            texture_id=(the_word[0]<<24) + (the_word[1]<<16) +
            (the_word[2]<<8) + the_word[3];
            if (the_word[0]==0 || the_word[1]==0 || the_word[2]==0 || the_word[3]==0 || the_word[4]!=0) 
            report_error ("load_level: RFIL: Bad texture ID in object.","\p(Must be 4 chars or 0000). Line number below.",line_number);
            if (texture_id=='0000') texture_id=0;
            if (static_slot<40) report_error("load_level: RFIL: Can't read object into slots below 20 (Reserved slots)","\p",100);
            load_object(filename,static_slot,id,colldetect,texture_id,IS_NOT_TRANSLUCENT);
            break;
          }
       case 'LMON':	//insert object to dyn ocbs
          {
             int scale=1;
             
            clear_pb(&the_params);
            
          //read object id
            word_len+=get_word(the_word,&the_line[word_len]);
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting Object ID code",line_number);

            id=(the_word[0]<<24) + (the_word[1]<<16) +
            (the_word[2]<<8) + the_word[3];
          //find id
             static_slot=get_static_object_slot(id);	//go fetch fido (the object)
             if (static_slot==-1) report_error_id ("load_level: Can't find object. ID is: ",id);
          //get world x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting start pos x",line_number);

            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.world_x=dectof_result;

          //get world y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting start pos y",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.world_y=dectof_result;
         
          //get world z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting start pos z",line_number);

            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.world_z=dectof_result;

          //get movement delta x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting delta x",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	    //    the_params.Omovedx=dectof_result;

          //get movement delta y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting delta y",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	    //    the_params.Omovedy=dectof_result;
          //get movement delta z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting delta z",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	    //    the_params.Omovedz=dectof_result;

          //get delta rot x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting delta rot x",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.delta_rot_x=dectof_result;

          //get delta rot y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting delta rot y",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.delta_rot_y=dectof_result;

          //get delta rot z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting delta rot z",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.delta_rot_z=dectof_result;

          //get control_use_1
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting Control Use 1 Value",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.control_use_1=STATIC_CAST_TO_INT(dectof_result);


          //get control_use_2
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting Control Use 2 Value",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.control_use_2=STATIC_CAST_TO_INT(dectof_result);

          //get control_use_3
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting Control Use 3 value",line_number);

            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.control_use_3=STATIC_CAST_TO_INT(dectof_result);

          //get controller ref
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting Controller reference number",line_number);

            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.controller_ref=STATIC_CAST_TO_INT(dectof_result);
          
          //get shields
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting shield value",line_number);

            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.shield_value=STATIC_CAST_TO_INT(dectof_result);

          //get mass
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting Mass",line_number);

            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.mass=dectof_result;
            if (the_params.mass<0) report_error("load_level: LMON: Negative mass not allowed.","\pLine number below.",line_number);

          //get category
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting object category",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
            temp_int=STATIC_CAST_TO_INT(dectof_result);
  	        the_params.object_category=temp_int;

          //get trivial rejection
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting No trivial rejection",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
            temp_int=STATIC_CAST_TO_INT(dectof_result);
  	        the_params.no_trivial_rejection=temp_int;

          //get scale
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting object scale",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
            temp_int=STATIC_CAST_TO_INT(dectof_result);
  	     scale=temp_int;
            if(scale<=0) report_error("load_level: LMON: Invalid scale","\pMust be greater than 0",line_number);

          //get unique ID
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting Bolt ID",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
            temp_int=STATIC_CAST_TO_INT(dectof_result);
  	        the_params.unique_id=temp_int;



	      the_params.attack_mode =  0;	//attacking
	 the_params.cannon_cat=1;
	 the_params.missile_cat=1;
	 
	      
	      dest_ocb=find_vacant_dynamic_object_slot();	//get us a slot to put it in

      if (dest_ocb==-1) report_error_id ("load_level: Cant get dynamic slot for LMON", 0);

          the_params.parent=-1;
 	  load_dyn_object(static_slot,dest_ocb,&the_params,-1,scale,-1,!DUST_BIT);	//load it
          #if TEST_SYSTEM==0
		   if (SOLR_executed==1)
		   force_object_orbit(dest_ocb,my_planet());
                   
          #endif
           break;
          }  


       case 'LMSP':	// load a mothership into game
          {
          double x,y,z;
          int seed,alleg;
          _3D start_pos;
          
            //get world x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMSP: Not enough parameters.","\pExpecting start pos x",line_number);

            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        x=dectof_result;

          //get world y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMSP: Not enough parameters.","\pExpecting start pos y",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        y=dectof_result;
         
          // get world z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMSP: Not enough parameters.","\pExpecting start pos z",line_number);

            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        z=dectof_result;

          // seed
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMSP: Not enough parameters.","\pExpecting Control Use 1 Value",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        seed=STATIC_CAST_TO_INT(dectof_result);

          // alignment
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LMSP: Not enough parameters.","\pExpecting Control Use 2 Value",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        alleg=STATIC_CAST_TO_INT(dectof_result);
           
                start_pos.x=0; start_pos.y=0; start_pos.z=0;		// just in case we are a test system, then no offset
          #if TEST_SYSTEM==0
		   if (SOLR_executed==1)
                        { get_basic_orbit_position(my_planet(), &start_pos); }
          #endif
                POBs_load_spacestation((x*ZEX_UNIT_SIZE_IN_METERS)+start_pos.x, (y*ZEX_UNIT_SIZE_IN_METERS)+start_pos.y, 
                                        (z*ZEX_UNIT_SIZE_IN_METERS)+start_pos.z, seed,alleg);
          
          break;
          }  


       case 'LAMN':	//insert animated object to dyn ocbs
          {
           frames_counter=0;
           anim.object_id[0]=0xffffffff;	//we allow up to 6 frames
           anim.object_id[1]=0xffffffff;
           anim.object_id[2]=0xffffffff;
           anim.object_id[3]=0xffffffff;
           anim.object_id[4]=0xffffffff;
           anim.object_id[5]=0xffffffff;
          //read object ids - '0000'=null
           for (i=0;i<6;i++)
           {
            word_len+=get_word(the_word,&the_line[word_len]);
            if (the_word[0]==0) report_error("load_level: LMON: Not enough parameters.","\pExpecting Object ID code",line_number);

            id=(the_word[0]<<24) + (the_word[1]<<16) +
            (the_word[2]<<8) + the_word[3];
             if (id!='0000') 
             {
              frames_counter++;
              anim.object_id[i]=id;
          //find id
              static_slot=get_static_object_slot(id);	//go fetch fido (the object)
              if (static_slot==-1) report_error_id ("load_level: LAMN: Can't find object. ID is: ",id);
             }
             
           }

          //get frame rate
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting anim frame rate",line_number);

            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        frame_rate=STATIC_CAST_TO_INT(dectof_result);

          //get animation control
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting anim control",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        anim_control=STATIC_CAST_TO_INT(dectof_result);

          //get animation state
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting anim state",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        anim_state=STATIC_CAST_TO_INT(dectof_result);
          
          //get world x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting start pos x",line_number);

            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.world_x=dectof_result;

          //get world y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting start pos y",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.world_y=dectof_result;
         
          //get world z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting start pos z",line_number);

            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.world_z=dectof_result;

          //get movement delta x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting delta x",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	    //    the_params.Omovedx=dectof_result;

          //get movement delta y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting delta y",line_number);
            dectof_result=Zconvert_to_float(the_word);  	    //    the_params.Omovedy=dectof_result;
          //get movement delta z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting delta z",line_number);
            dectof_result=Zconvert_to_float(the_word);  	    //    the_params.Omovedz=dectof_result;

          //get delta rot x
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting delta rot x",line_number);
            dectof_result=Zconvert_to_float(the_word);
            the_params.delta_rot_x=dectof_result;

          //get delta rot y
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting delta rot y",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.delta_rot_y=dectof_result;

          //get delta rot z
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting delta rot z",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.delta_rot_z=dectof_result;

          //get control_use_1
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting Control Use 1 Value",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.control_use_1=STATIC_CAST_TO_INT(dectof_result);


          //get control_use_2
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting Control Use 2 Value",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.control_use_2=STATIC_CAST_TO_INT(dectof_result);

          //get control_use_3
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting Control Use 3 value",line_number);

            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.control_use_3=STATIC_CAST_TO_INT(dectof_result);

          //get controller ref
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting Controller reference number",line_number);

            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.controller_ref=STATIC_CAST_TO_INT(dectof_result);
          
          //get shields
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting shield value",line_number);

            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.shield_value=STATIC_CAST_TO_INT(dectof_result);

          //get mass
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting Mass",line_number);

            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
  	        the_params.mass=dectof_result;
            if (dectof_result<0) report_error("load_level: LAMN: Negative mass not allowed.","\pLine number below.",line_number);

          //get category
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting object category",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
            temp_int=STATIC_CAST_TO_INT(dectof_result);
  	        the_params.object_category=temp_int;

          //get trivial rejection
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting control if not visible",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
            temp_int=STATIC_CAST_TO_INT(dectof_result);
  	        the_params.no_trivial_rejection=temp_int;

          //get number of missiles
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting number of missiles",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
            temp_int=STATIC_CAST_TO_INT(dectof_result);
  	        the_params.number_of_missiles=temp_int;
          //get laser category
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting laser category",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
            temp_int=STATIC_CAST_TO_INT(dectof_result);
  	        the_params.laser_cat=temp_int;
          //get has roll
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: LAMN: Not enough parameters.","\pExpecting has roll",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	  //convert from string to float
            dectof_result=Zconvert_to_float (the_word);
            temp_int=STATIC_CAST_TO_INT(dectof_result);
//  	          the_params.has_roll=temp_int;

//     the_params.has_roll=0;

	      the_params.rot_x=0;
	      the_params.rot_y=0;
	      the_params.rot_z=270;
	      the_params.control_use_4=0;
	the_params.control_use_5=0;
	      the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	      the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	      the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	      the_params.attack_mode =  0;	//attacking

	 the_params.cannon_cat=1;
	 the_params.missile_cat=1;
	
//	dest_ocb=find_vacant_dynamic_object_slot();	//get us a slot to put it in


     the_params.parent=-1;
 	  load_animated_object(&anim,frames_counter,frame_rate,anim_control,anim_state,&the_params,-1);	//load it
          
           break;
          }  


       case 'BOLT':	//bolt an object to another object
          {  
          int bolted, boltee, slot, bolted_to;
          float offset_x, offset_y, offset_z;
                      
            //get the object we wish to bolt onto another object
            word_len+=get_word(the_word,&the_line[word_len]);
            if (the_word[0]==0) report_error("load_level: BOLT: Not enough parameters.","\pExpecting object to be bolted to",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
            bolted=STATIC_CAST_TO_INT(dectof_result);
            
            //get object to bolt on to
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: BOLT: Not enough parameters.","\pExpecting object to be bolted",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
            boltee=STATIC_CAST_TO_INT(dectof_result);	//0-3

            //get x offset
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: BOLT: Not enough parameters.","\pExpecting bolted x offset",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
            offset_x=dectof_result;	//0-3

            //get y offset
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: BOLT: Not enough parameters.","\pExpecting bolted y offset",line_number);
            ix=0;
            str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
            dectof_result=Zconvert_to_float (the_word);
            offset_y=dectof_result;	//0-3

            //get z offset
            word_len+=get_word(the_word,&the_line[word_len]);	//get difficulty rating
            if (the_word[0]==0) report_error("load_level: BOLT: Not enough parameters.","\pExpecting bolted z offset",line_number);
            dectof_result=Zconvert_to_float(the_word);            offset_z=dectof_result;	//0-3

	    //find the boltee object
	    slot = find_unique_id(bolted);
	    if (slot==-1) report_error("load_level: BOLT: Bolted not loaded.","\pLoad object you are trying to bolt",line_number);
            
            bolted_to=find_unique_id(boltee);
	    if (bolted_to==-1) report_error("load_level: BOLT: Boltee not loaded.","\pLoad object you are trying to bolt to",line_number);
            
            (*ocb_ptr).object_list[slot].bolted_to=bolted_to;

            (*ocb_ptr).object_list[slot].bolt_x_offset=offset_x;
            (*ocb_ptr).object_list[slot].bolt_y_offset=offset_y;
            (*ocb_ptr).object_list[slot].bolt_z_offset=offset_z;
            
            
            
            break;
          }

      case 'BMAP':	 //load bg map
          {
            break;
          }          
       }	//end of switch
//ееееееееееееend command processing
      word_len=0;
      source+=line_len+1;	//point to next line (accounting for CR)
      line_number++;
     }
	}
	else stop=1;
 }
level_text[level_text_read_index]=0;
		   show_text_in_loading_screen("Zex: Game data read OK\n");	

}

char good_luck[]="GOOD LUCK CAPTAIN.";
char be_lucky[]="BE LUCKY CAPTAIN.";
char watch_ass[]="REMEMBER - WATCH YOUR BUTT!";
char counting[]="WE ARE COUNTING ON YOU.";

//еееееееееееееееееееееееее
//display the LTXT text
void show_level_text()
{
extern int level_text_read_index;
extern char level_text[2048];	//level description goes here

extern Ptr screenaddr;
extern int screenwidth;


//CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;

int level_text_pos=0;
int text_line_index;
int curs_y;
char text_line[1024];
char the_char;
int temp,temp_y;
typedef struct {
int x,y,c,delta;
}star_object;


star_object stars[612];	//array of bg stars

curs_y=monitor_h-80;


    gLaser=0;
//    user_delay(10);
//fill the star array
    init_zex_random();
    for (temp=0;temp<612;temp++)
    {
     //Ptr pix;
    
     stars[temp].x=RangedRdm(10,monitor_w-1);
     stars[temp].y=RangedRdm(10,monitor_h-1);
     stars[temp].delta=RangedRdm(0,4);	//movement frame rate
     stars[temp].c=RangedRdm(245,252);
//     stars[temp].c=255-(5-stars[temp].delta);	//slower=darker=further away
    }
//this is a vertical scrolly
while (game_fire_button()==0)
 {
    level_text_pos=0;
    if (curs_y>100) curs_y-=2;	//stop at line 100
//	  SetPort((GrafPtr)the_drawing_buffer);
//    GetPort (&savePort);
//	  saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//	  SetGDevice(mZexGDH);
//	  RGBBackColor (&rgbBlack);
//	  EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect(monitor_w,monitor_h);	//in pictures
//	  SetGDevice(saveGDH);	  //these are here so I can SEE them!
//	  SetPort(savePort);
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;
//lets do some stars as teh background image
    for (temp=0;temp<612;temp++)
    {
     int x,y,c;
     Ptr pix;
    
     x=stars[temp].x;
     y=stars[temp].y;
     c=stars[temp].c;
     x+=stars[temp].delta;
      if (x>monitor_w-20) x=0;
      stars[temp].x=x;
     pix=(char*)screenaddr+(y*screenwidth)+x;	 //screenaddr
     *pix=c;

    }
//    init_zex_random();

    temp_y=curs_y;    
    
    while(level_text_pos<level_text_read_index)
    {

    text_line_index=0;
    the_char=0;
     while (the_char!=13 && text_line_index<1023)
      {
      the_char=level_text[level_text_pos];
      text_line[text_line_index]=the_char;
      level_text_pos++;
      text_line_index++;
      }
      text_line[text_line_index-1]=0;	//make c string
    
     if (temp_y<monitor_h-20) SplatText_Transparent (text_line,off_640_x+120,temp_y,247);
    temp_y+=12;
    
     }
	Show_it();

 }	//end of while
 
 
//now scroll text down and off
  while(curs_y<monitor_h-20)	
 {
    level_text_pos=0;
    if (curs_y<monitor_h-18) curs_y+=6;	//stop at line 100
//	SetPort((GrafPtr)the_drawing_buffer);
//    GetPort (&savePort);
//	  saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//	  SetGDevice(mZexGDH);
//	  RGBBackColor (&rgbBlack);
//	  EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect(monitor_w,monitor_h);	//in pictures
//	  SetGDevice(saveGDH);	  //these are here so I can SEE them!
//	  SetPort(savePort);
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;
//lets do some stars as teh background image
    for (temp=0;temp<612;temp++)
    {
     int x,y,c;
     Ptr pix;
    
     x=stars[temp].x;
     y=stars[temp].y;
     c=stars[temp].c;
     x+=stars[temp].delta;
      if (x>monitor_w-21) x=0;
      stars[temp].x=x;
     pix=(char*)screenaddr+(y*screenwidth)+x;	 //screenaddr
     *pix=c;

    }
//    init_zex_random();

    temp_y=curs_y;    
    
    while(level_text_pos<level_text_read_index)
    {

    text_line_index=0;
    the_char=0;
     while (the_char!=13 && text_line_index<1023)
      {
      the_char=level_text[level_text_pos];
      text_line[text_line_index]=the_char;
      level_text_pos++;
      text_line_index++;
      }
      text_line[text_line_index-1]=0;	//make c string
    
     if (temp_y<monitor_h-20) SplatText_Transparent (text_line,off_640_x+120,temp_y,247);
    temp_y+=12;
    
     }
	Show_it();

 }	//end of while
 

    gCannon=0; 
    gLaser=0;
}


int FILE_LOAD_ERROR_QUITS=1;

//looks for a file in ZD3/folder name/
int make_data_fsspec(unsigned char*the_folder_name,const unsigned char*the_filename, FSSpec* the_fsspec)	//make fsspec of file name in zd3 folder
{
extern FSSpec Zex_FSSpec;
FSSpec data_folder_FSSpec;
CInfoPBRec pb;
DirInfo *dpb=(DirInfo*) &pb;
HFileInfo *fpb=(HFileInfo*) &pb;	//ptrs to the union (Finfo and DirInfo) held in CInfoPBRec
int folder_DirID;
int return_value;
short get_file_err;
Str255 modifyable_target_filename;

    pascal_copystr(modifyable_target_filename, the_filename);
    
//get the objects' folder
	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,DirStr,&data_folder_FSSpec);
	
//get zd3's dirctory ID
     dpb->ioVRefNum = data_folder_FSSpec.vRefNum;
     dpb->ioDrDirID = data_folder_FSSpec.parID;
     dpb->ioNamePtr = data_folder_FSSpec.name;
     dpb->ioFDirIndex=0;	//query
     return_value=PBGetCatInfo(&pb,0);
     if (return_value) 
     {
     report_error ("Zex couldn't read a file:",the_filename,return_value);
     return return_value;
     }
     folder_DirID=dpb->ioDrDirID;

return_value=0;

//find sub folder
      fpb->ioVRefNum = data_folder_FSSpec.vRefNum;
     fpb->ioDirID = folder_DirID;
     fpb->ioNamePtr = the_folder_name;
     fpb->ioFDirIndex=0;	//query the file name

     return_value=PBGetCatInfo(&pb,0);
     if (return_value) 
     {
     report_error ("load_level couldn't find a folder:",the_folder_name,return_value);
     return return_value;
     }
     folder_DirID=dpb->ioDrDirID;

//now load it
//find file
      fpb->ioVRefNum = data_folder_FSSpec.vRefNum;
     fpb->ioDirID = folder_DirID;
     fpb->ioNamePtr = modifyable_target_filename;
     fpb->ioFDirIndex=0;	//query the file name
     get_file_err=PBGetCatInfo(&pb,0);
     if(FILE_LOAD_ERROR_QUITS)
       {
       if (get_file_err) report_error ("make_data_fsspec couldn't read a file:",the_filename,get_file_err);
       }
     if(get_file_err) return get_file_err;

     FSMakeFSSpec (fpb->ioVRefNum, folder_DirID,the_filename,the_fsspec);
     
return 0;
}

//int make_data_fsspec(Str255 the_folder_name,Str255 the_filename, FSSpec* the_fsspec)	  //make fsspec of file name in zd3 folder
//{
//extern FSSpec Zex_FSSpec;
//FSSpec data_folder_FSSpec;
//CInfoPBRec pb;
//DirInfo *dpb=(DirInfo*) &pb;
//HFileInfo *fpb=(HFileInfo*) &pb;	  //ptrs to the union (Finfo and DirInfo) held in CInfoPBRec
//int folder_DirID;
//int return_value;
//short get_file_err;
//
////get the objects' folder
//	  FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"\pZD3",&data_folder_FSSpec);
////get the folders dirctory ID
//     dpb->ioVRefNum = data_folder_FSSpec.vRefNum;
//     dpb->ioDrDirID = data_folder_FSSpec.parID;
//     dpb->ioNamePtr = data_folder_FSSpec.name;
//     dpb->ioFDirIndex=0;	  //query
//     return_value=PBGetCatInfo(&pb,0);
//     if (return_value) 
//     {
//     report_error ("load_level couldn't read a file:",the_filename,return_value);
//     return return_value;
//     }
//     folder_DirID=dpb->ioDrDirID;
//
//return_value=0;
//
//
//      fpb->ioVRefNum = data_folder_FSSpec.vRefNum;
//     fpb->ioDirID = folder_DirID;
//     fpb->ioNamePtr = the_filename;
//     fpb->ioFDirIndex=0;	  //query the file name
//     get_file_err=PBGetCatInfo(&pb,0);
//     if (get_file_err) report_error ("make_data_fsspec couldn't read a file:",the_filename,get_file_err);
//     if(get_file_err) return return_value;
////
////now load it
//     FSMakeFSSpec (fpb->ioVRefNum, folder_DirID,the_filename,the_fsspec);
//}