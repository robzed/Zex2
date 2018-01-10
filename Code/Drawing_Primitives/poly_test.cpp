// Zex 2 - 3D Space Trading Game with in-cockpit view 
// Copyright (C) 1998-2018  Stuart Ball and Robert Probin
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

/*

poly test


*/

#include <types.h>
#include <quickdraw.h>
#include <windows.h>
#include <menus.h>
#include <fonts.h>
#include <dialogs.h>
#include <textedit.h>
#include <Events.h>
#include <stdio.h>

void init_mac(void);

typedef struct {
		int x;
		int y;
		} coord;
		
void polygon(int num_lines, coord *point_list, int colour, char *screen_address, int screen_width);
void init_polygon(void);


void main(void)
{
Ptr screenaddr;
int screenwidth;
GDHandle mygdhandle;
//int a;
EventRecord	theEvent;
WindowPtr ww;
Rect my_rect;
char my_text[80];
coord *point_list;
coord points[10];

init_mac();

mygdhandle=GetMainDevice();

screenaddr=(*((*mygdhandle)->gdPMap))->baseAddr;
screenwidth=0x3fff & (*((*mygdhandle)->gdPMap))->rowBytes;


SetRect(&my_rect,0,0,400,400);


ww=NewCWindow(0,&my_rect,"\ptitle",1,0,(struct GrafPort *)-1,0,0);
if(ww==0) { return; }


SetPort(ww);
MoveTo(20,20);

sprintf(my_text," %x %x ...             ",screenaddr,screenwidth);
DrawText(my_text,0,20);

//for(a=0; a<250; a++)
//  {
//	pline(a, 600-a, screenaddr+(screenwidth*a), a);
 // *(screenaddr+(screenwidth*a))=a;
//  }

init_polygon();


#if 1
/*-----------------------------------------------------*/
point_list=points;

point_list->x=10; point_list->y=10; point_list++;
point_list->x=20; point_list->y=80; point_list++;
point_list->x=100; point_list->y=100; point_list++;

polygon(3,points,45,screenaddr,screenwidth);

/*-----------------------------------------------------*/
point_list=points;

point_list->x=110; point_list->y=110; point_list++;
point_list->x=180; point_list->y=120; point_list++;
point_list->x=200; point_list->y=200; point_list++;

polygon(3,points,87,screenaddr,screenwidth);


/*-----------------------------------------------------*/
point_list=points;

point_list->x=10; point_list->y=210; point_list++;
point_list->x=10; point_list->y=210; point_list++;
point_list->x=80; point_list->y=260; point_list++;

polygon(3,points,190,screenaddr,screenwidth);


/*-----------------------------------------------------*/
point_list=points;

point_list->x=10; point_list->y=150; point_list++;
point_list->x=55; point_list->y=100; point_list++;
point_list->x=100; point_list->y=150; point_list++;

polygon(3,points,255,screenaddr,screenwidth); 
/*-----------------------------------------------------*/
point_list=points;

point_list->x=150; point_list->y=50; point_list++;
point_list->x=165; point_list->y=50; point_list++;
point_list->x=100; point_list->y=50; point_list++;

polygon(3,points,255,screenaddr,screenwidth);  
/*-----------------------------------------------------*/
point_list=points;

point_list->x=200; point_list->y=50; point_list++;
point_list->x=200; point_list->y=100; point_list++;
point_list->x=250; point_list->y=100; point_list++;
point_list->x=250; point_list->y=50; point_list++;

polygon(4,points,255,screenaddr,screenwidth);  
/*-----------------------------------------------------*/
#endif

point_list=points;

point_list->x=300; point_list->y=50; point_list++;
point_list->x=301; point_list->y=100; point_list++;
point_list->x=350; point_list->y=102; point_list++;
point_list->x=350; point_list->y=50; point_list++;

polygon(4,points,255,screenaddr,screenwidth);  
/*-----------------------------------------------------*/


do {
	WaitNextEvent(everyEvent, &theEvent, 0xffffffff, 0);
	} while(theEvent.what!=mouseDown);




}



#if !(MAC68K)
QDGlobals qd;
#endif

void	init_mac()
{
	InitGraf(&qd.thePort);
	InitFonts();
	FlushEvents(everyEvent, 0);
	InitWindows();				
	InitMenus();
	TEInit();
	InitDialogs(0L);
	InitCursor();
	MaxApplZone();

}
