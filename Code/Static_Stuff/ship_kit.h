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

//buy.int
//defs for buy
//basic classes
#define REPAIRS 1
#define EQUIPMENT 2
#define WEAPONS 3

//item types for buy, not ships systems - their in instrum.int
enum {
MISSILE1, MISSILE2, MISSILE3,
LASER1,LASER2,LASER3,
TASER1,TASER2,TASER3,	//like lasers except twin and tri shot
REAR_LASER1,REAR_LASER2,REAR_LASER3,
CANNON1,CANNON2,CANNON3,
SHOCKWAVE1,SHOCKWAVE2,
ECM,
MISSILE_FACTORY,
TARGETTER_360,

FAST_DOCK,FLATPACK,EFFICIENT_ENGINE,
RETRO_ROCKETS,HYPERSPACE,FLIGHT_COMPUTER1,
BULLET_PROOF_EJECTION,
SHIELDS1,SHIELDS2,SHIELDS3,SHIELDS4,SHIELDS5,

HULL_REPAIR, FUEL, 
REPAIR_SYSTEMS1, REPAIR_SYSTEMS2, REPAIR_SYSTEMS3,
EJECTION,FLIGHT_COMPUTER2, NAV_COMPUTER1, NAV_COMPUTER2,
TRADING_COMPUTER1,TRADING_COMPUTER2,TRADING_COMPUTER3,
AUTO_HULL_REPAIR,BOID_ADDITION,ASTEROID_ADDITION,
NUMBER_OF_GOODS,	//end of list marker
};