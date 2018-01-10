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

//items in the hold
#define MAX_HOLD_ITEMS 4
#define ORES 10  //agri
#define FOOD 11  //agri
#define TECH 12 //tech
#define LUXURY 13 //tech
#define FIREARMS 14 //tech
#define DRINKS 15  //agri
#define LIVESTOCK 16 //agri

//Race encodings for trading
#define MFR_HUM 1 //human
#define MFR_RMP 2 //rmparian
#define MFR_ALY 4 //alycian
#define MFR_KIS 8 //kisinte
#define MFR_XEN 16 //xenifor
#define MFR_KRY 32 //Kryptonil
#define MFR_QUA 64 //Quabriel
#define MFR_SEL 128 //Selenial
#define MFR_TRO 256 //Trondent
#define MFR_OUT 512 //Outsider


typedef struct
{
char string [36];
short dofq;	//1=tonnes, 4=grammes	
short _class;	//10-ores, 11-food, 12-tech, 13-luxury/textiles, 14-firearms
short base_price;
short transport_risk,pirate_risk;
short MFR; //manufacturer: 1=human, 2=alien
short markup; 
short stock; //spare 3
short buy_price; //spare 6
short sell_price; //spare 7
short id;	//serial number (index - 0 based)
} hold_desc;


void buy(int slot_docked_at);
void setup_stock(int slot_number);	//of mothership
void load_up_stock_strings();
void do_yer_actual_buy(int stock_item_id, int screen_class);
void draw_string_with_cr(int x, int y, Str255 the_string);
int do_trading_computer(int actual_price, int stock_index);
void update_ship(int item_id);
int check_needed(int item_id);






