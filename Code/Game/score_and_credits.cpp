//score and credits
/*
 * $Log: score_and_credits.c,v $
 * Revision 1.1.1.1  2003/09/05 22:35:47  stu_c
 * First Imported.
 *
 * Revision 1.7  2002/09/08 02:02:35  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.6  2002/07/28 17:12:16  stu_c
 * Exhausts and further particle work
 *
 * Revision 1.5  2002/02/16 19:03:06  stu
 * Radar range decrease key and rationalisation of dlp colours
 *
 * Revision 1.4  2001/12/29 01:10:31  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.3  2001/11/02 19:31:08  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.2  2001/10/22 21:28:28  rob
 * Carbon warning changes
 *
 * Revision 1.1  2001/10/21 01:03:05  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:06  rob
 * First Imported.
 *
 * Revision 1.1.1.1  2001/01/01 21:13:35  rob
 * First Imported.
 *
 * Revision 1.0.0.1  2000/08/21 22:02:02  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <NumberFormatting.h>
  #endif
  #include "3d_structures_def.h"
  #include	"zex_misc.h"
#endif //pch


#include "score_and_credits.h"
#include "dlp.h"
#include "cargo.h"

void add_to_score_and_credits(int object_just_destroyed)
{
extern int score,credits,showing_stats;
extern int difficulty,promotion_points,kills;
extern	DynObjectsFixedSize *ocb_ptr;

int id;
//int wingman;
int earn;
int hit_by,owner,zex_shot;
unsigned long owner_id;	//who fired the shot?



if (showing_stats==1) return;	//no score when stats or docked

zex_shot=0;

hit_by=(*ocb_ptr).object_list[object_just_destroyed].hit_by_slot;
if (hit_by!=-1)
{
 owner=(*ocb_ptr).object_list[hit_by].parent;
 if (owner!=-1)
 {
  owner_id=(*ocb_ptr).object_list[owner].Oid;
  if (owner_id=='ZEXS') zex_shot=1;
  else zex_shot=0;
 }
 else zex_shot=0;
}

if (zex_shot==0) return;

kills++;

id=(*ocb_ptr).object_list[object_just_destroyed].Oid;
if (id=='MIS1' || id=='MIS2' || id=='MIS3' || id=='EX01') return; 	  //dont do for cannons being destroyed
															//or explosion
switch (id)
 {
  case 'MON1':
                score+=RangedRdm(10,18);
                earn=RangedRdm(14,40);
                credits+=earn;
                show_earnings(earn);
		gen_cargo(object_just_destroyed,difficulty);	//possibly throw out a cargo container
		promotion_points+=2;
                break;
  case 'WH01':
                score+=RangedRdm(10,18);
                break;

  case 'MON2':
                score+=RangedRdm(20,40);
                earn=RangedRdm(13,32);
                credits+=earn;
                show_earnings(earn);
		gen_cargo(object_just_destroyed,difficulty);	//possibly throw out a cargo container
		promotion_points+=3;
                break;
  case 'MON3':
                score+=RangedRdm(75,152);
                earn=RangedRdm(35,60);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
//                wingman=find_a_wingman();
//                wingman_diealienbyboss_text(wingman);	//eh42: nice shot boss! etc
		}
				promotion_points+=3;
		gen_cargo(object_just_destroyed,2+difficulty);	//possibly throw out a cargo container
                break;

  case 'GRN1':
                score+=RangedRdm(85,172);
                earn=RangedRdm(40,70);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
//                wingman=find_a_wingman();
//                wingman_diealienbyboss_text(wingman);	//eh42: nice shot boss! etc
		}
				promotion_points+=4;
		gen_cargo(object_just_destroyed,3+difficulty);	//possibly throw out a cargo container
                break;
  case 'ASH1':
                score+=RangedRdm(85,172);
                earn=RangedRdm(50,90);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
//                wingman=find_a_wingman();
//                wingman_diealienbyboss_text(wingman);	//eh42: nice shot boss! etc
		}
                promotion_points+=10;
		gen_cargo(object_just_destroyed,90+difficulty);	//possibly throw out a cargo container
		gen_cargo(object_just_destroyed,90+difficulty);	//possibly throw out a cargo container
                break;


  case 'MON4':
                score+=RangedRdm(80,152);
                earn=RangedRdm(60,92);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
//                wingman=find_a_wingman();
//                wingman_diealienbyboss_text(wingman);	//eh42: nice shot boss! etc
                }
		gen_cargo(object_just_destroyed,10+difficulty);	//possibly throw out a cargo container
		promotion_points+=4;
                break;

  case 'MON6':
                score+=RangedRdm(280,352);
                earn=RangedRdm(95,152);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
//                wingman=find_a_wingman();
//                wingman_diealienbyboss_text(wingman);	//eh42: nice shot boss! etc
		}
		gen_cargo(object_just_destroyed,5+difficulty);	//possibly throw out a cargo container
               		promotion_points+=6;
                break;
                
  case 'EXC1':
                score+=RangedRdm(290,392);
                earn=RangedRdm(150,300);
                credits+=earn;
                show_earnings(earn);
               if (zex_shot==1)
                {
//                wingman=find_a_wingman();
//                wingman_diealienbyboss_text(wingman);	//eh42: nice shot boss! etc
		}
		gen_cargo(object_just_destroyed,10+difficulty);	//possibly throw out a cargo container
		promotion_points+=12;
                break;
                
               
  case 'SGN1':
                score+=RangedRdm(35,62);
                earn=RangedRdm(45,85);
                credits+=earn;
                show_earnings(earn);
                		promotion_points+=2;
                break;
  case 'IOD1':
                score+=RangedRdm(150*difficulty,300*difficulty);
                earn=RangedRdm(350*difficulty,450*difficulty);
                credits+=earn;
                show_earnings(earn);
                		promotion_points+=2;
                break;
  
  case 'EC01':
                score+=0;
                earn=-30;
                credits+=earn;
                if (credits<0) credits=0;
                show_earnings(earn);
                
                		promotion_points-=1;
                if (promotion_points<0) promotion_points=0;
                break;
//phoenix
  case 'MN31': case 'MN32': case 'MN33': case 'MN34':
                score+=RangedRdm(80,200);
                earn=RangedRdm(100,200);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
//                wingman=find_a_wingman();
//                wingman_diealienbyboss_text(wingman);	//eh42: nice shot boss! etc
		}
		gen_cargo(object_just_destroyed,6+difficulty);	//possibly throw out a cargo container
	promotion_points+=5;
                break;


//static laser
  case 'SLS1': case 'SLS2': case 'SLS3':
                score+=RangedRdm(80,200);
                earn=RangedRdm(100,200);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
//                wingman=find_a_wingman();
//                wingman_diealienbyboss_text(wingman);	//eh42: nice shot boss! etc
		}
	        promotion_points+=9;
                break;

                
//warper
  case 'WARP':
                score+=RangedRdm(200,400);
                earn=RangedRdm(120,260);
                credits+=earn;
                show_earnings(earn);
                if (zex_shot==1)
                {
//                wingman=find_a_wingman();
//                wingman_diealienbyboss_text(wingman);	//eh42: nice shot boss! etc
		}
		gen_cargo(object_just_destroyed,10+difficulty);	//possibly throw out a cargo container
	promotion_points+=6;
                break;

//boids
  case 'BID1': case 'BID2': case 'BID3': case 'BID4':
                score+=RangedRdm(500,1500);
                earn=RangedRdm(230,400);
                credits+=earn;
                show_earnings(earn);
                	promotion_points+=20;
                break;

  case 'AST1':
                score+=RangedRdm(100,500);
                earn=RangedRdm(120,200);
                credits+=earn;
                show_earnings(earn);
                	promotion_points+=10;
                break;


  case 'ZZ1A': case 'ZZ1B': case 'ZZ1C':
                score+=RangedRdm(900,1700);
                break;
  
  default :
                break;

 }
}

char credits_string[]=" CREDITS";
char deduct_string[]="DESTROYING THE ESCAPE CAPSULE HAS JUST COST YOU 30 CREDITS.";
char deduct_all_string[]="DESTROYING THE ESCAPE CAPSULE HAS COST YOU ALL YOUR CREDITS.";

void show_earnings(int amount)
{
unsigned char no_string[24];
int end_of_number_string,index;
unsigned char the_char=13;
extern int credits;

 if (amount<0)
  {
  if (credits==0)
  add_to_text_display((char *) deduct_all_string,DLP_GREEN);
  else
  add_to_text_display((char *) deduct_string,DLP_GREEN);
  
  return;
  }
 NumToString(amount,no_string);
//tag on credits string
 end_of_number_string=no_string[0]+1;	 //quick p to c hack
 no_string[0]=' ';
 index=0;
//copy from credits_string to no_string
 while(the_char!=0)
 {
 the_char=credits_string[index];
 no_string[end_of_number_string+index]=the_char;
 index++;
 }
add_to_text_display((char *) &no_string[1],DLP_GREEN);
}

