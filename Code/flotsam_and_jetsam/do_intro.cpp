/* $Log: do_intro.c,v $
/* Revision 1.2  2001/11/02 19:33:02  rob
/* Build variants for interface lib and carbon.
Coffee fixes after changes for X.
/*
/* Revision 1.1  2001/10/26 22:40:13  rob
/* header changes for new source set.
/*
/* Revision 1.0.0.1  2001/10/17 20:46:04  rob
/* First Imported.
/*
/* Revision 1.2  2001/06/03 00:25:44  stu
/* Fix for dialogs not showing. Also affects finder and Anvil console commands
/*
/* Revision 1.1.1.1  2001/01/01 21:13:35  rob
/* First Imported.
/*
/* Revision 1.1  2000/09/20 23:52:25  stu
/* HUD/AP/Scaling
/*
/* Revision 1.0.0.1  2000/08/21 22:01:55  stu
/* First Imported.
/*
 */
 
#include "zselector.h"		// this selects which build we will be doing
#include "zplatform.h"

#include <files.h>
#include <memory.h>
#include <events.h>

#include "file_io.h"
#include "do_intro.h"
#include "zsound.h"
#include "drawing.h"
#include "pictures.h"
#include	"splat_control.h"
#include	"txtsplat.h"
#include "ship_control.h"
#include "zex_misc.h"
#include "Error_handler.h"
#include "keyboard.h"

//do intro
void do_intro()
{

//FSSpec the_file;
Handle the_file_data_H;
LSRAW *the_picture;
extern FSSpec Zex_FSSpec;
int timer_end,wait_flag;
CGrafPtr the_drawing_buffer;
extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;
extern int music_volume;
extern GDHandle				mZexGDH;

int yaccel=30;
int ypos=0;
int xpos=680;
int memsize,i,j,y_correct,x_correct,new_x,new_y;
float mag;
LSRAW* pictbuffer;
GDHandle	saveGDH;
GrafPtr		savePort;
int frame,movie_index,mdelay;

typedef struct
{
int the_frame,the_delay;
} stumovie;

stumovie frames [] = 
{
128,2,200,2,129,2,200,2,130,2,200,2,131,2,200,2,132,2,200,2,133,2,200,2,134,2,200,2,
135,1,200,1,136,1,135,1,134,1,133,1,132,1,
131,1,130,1,129,1,128,1,
137,2,139,2,140,2,141,2,142,3,143,3,144,3,145,50,
146,2,147,2,148,2,149,2,150,2,151,2,152,2,153,180,154,100
-1,-1,-1,-1,-1
};

int top,bottom;
int n;
UInt32 picture_w,picture_h;
int picture_x,picture_y;

	music_set_vol(music_volume);

    play_tune(129);

//play intro movie
//    frame=128;
//    movie_index=0;
//    while (frame!=153)
//    {
//    if (game_fire_button()) break;


    frame=153;	//frames[movie_index].the_frame;
//    mdelay=frames[movie_index].the_delay;
//    movie_index++;
//    if (frame==137) simple_play_sound(2000);
    erase_zex_rect(monitor_w,monitor_h);
    the_file_data_H=GetZexPicture ('RCZM',frame);	//lsg    
	the_picture = (LSRAW*)*the_file_data_H;
	print_crunched_trans_pict(the_picture,off_640_x+0,off_480_y+0);
	Show_it();
    DisposeHandle(the_file_data_H);
    for(n=0;n<200;n++)
    {
     if(Button()) break;
     user_delay(1);    
    }    
	
//    }
    simple_stop_sound();
	
    play_tune(131);	
//show zex screen
	clearGL();	
    the_file_data_H=GetZexPicture ('RCZ ',130);	//zex logo   
    HLock(the_file_data_H);

   	the_picture = (LSRAW*)*the_file_data_H;
//center the picture
picture_w=(*the_picture).pwidth;
picture_h=(*the_picture).pheight;	//now pointing to data
picture_x=(monitor_w-picture_w)/2;
picture_y=(monitor_h-picture_h)/2;

	print_crunched_trans_pict(the_picture,picture_x,picture_y);
	Show_it();
    DisposeHandle(the_file_data_H);
    
    for(n=0;n<70;n++)
    {
     if(Button()) break;
     user_delay(1);    
    }

    disable_inputs();

}


void do_exit(void)
{

//    fade_out_music(32);

}