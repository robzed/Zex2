// *************************************MODULE HEADER*********************************
// * FILENAME           : make_solar_system.c
// * MODULE TITLE       : 
// * PROJECT            : Zex
// * DATE STARTED       : Sat 17:09:10 on 30th Sept 2000
// * FIRST AUTHOR       : Rob Probin
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************

// $Log: make_solar_system.c,v $
// Revision 1.1.1.1  2003/09/05 22:35:31  stu_c
// First Imported.
//
//
//

// ***********************************************************************************
// * INCLUDED FILES
// *
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"
  #include "LS_Standard.h"

  #if (!defined(__CARBON__)) && ZEX_PROJECT_BUILDER
    #include <Carbon/Carbon.h>
  #else
    #include <fp.h>
  #endif
  #include "zex_misc.h"
  #include "error_handler.h"
#endif //pch

#include "galaxy.h"
#include "matrix_primitives.h"
#include "Physics.h"
#include "physics_gravity.h"
#include "planet_generator.h"
#include "POBS.h"


// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

/*

NAME:        log_ranged_random(min,max)

DESCRIPTION: Given a range, produces a distribution with more down the bottom.
 */


int MSS_min_planet_size=200;		// in units of 10km - slightly smaller than pluto
//int MSS_small_planet_min_size=50;	// below 500km moon is just a brown rock
int MSS_medium_planet_min_size=600;	// 6,000km
int MSS_large_planet_min_size=1500;	// 15,000km
int MSS_gas_giant_min_size=3000;	// 30,000km
int MSS_massive_gas_min_size=8000;	// 80,000km
int MSS_max_planet_size=15000;		// in units of 10km - slightly bigger than jupiter

// moon = 3470, mercury = 4870, mars 6700, earth 12000,

double random_planet_size_in_m(void)
{
int size, band_selector;

band_selector=RangedRdm(0,100);

// planets 10% chance - rock/small planet < 
//         40% chance - medium planet     <
//         10% chance - large planet      <
//         20% chance - gas giant         <
//         20% change - large gas giant   <
if(band_selector < 10)		// small planet
    { size= 1e4*RangedRdm(MSS_min_planet_size,MSS_medium_planet_min_size); }
else if(band_selector < 50)	// medium size
    { size= 1e4*RangedRdm(MSS_medium_planet_min_size,MSS_large_planet_min_size); }
else if(band_selector < 60)	// large size
    { size= 1e4*RangedRdm(MSS_large_planet_min_size,MSS_gas_giant_min_size); }
else if(band_selector < 80)	// gas
    { size= 1e4*RangedRdm(MSS_gas_giant_min_size,MSS_massive_gas_min_size); }
else 				// massive
    { size= 1e4*RangedRdm(MSS_massive_gas_min_size,MSS_max_planet_size); }

return (double)size;
}



int MSS_min_rotation_hours=12;
int MSS_max_average_rotation_hours=72;
int MSS_max_rotation_hours=1920;

int random_planet_rotation_in_hours(void)
{
int hours, band_selector;
 
band_selector=RangedRdm(0,100);
// 80% planets rotate in between 0 and 3 days
// 20% others take up to 80 hours
if(band_selector < 80)
  { hours = RangedRdm(MSS_min_rotation_hours, MSS_max_average_rotation_hours); }
else
  { hours = RangedRdm(MSS_max_average_rotation_hours, MSS_max_rotation_hours); }
  
return hours;
}



/* ---- solar system basics ----

SUN

mass: between 0.5 and 10 solar masses. 
size: between x and y

how does mass and size relate?:

position always at 0,0


no binary systems yet, although binary systems represent more than 50% of star systems.


PLANETS

random mass
random size - are size and mass tied?
what about gas-giants and rocky planets?


nearest planet sun-size + n %
number of planets between 0 to 20

Each planet calculated from start, and we add on next distance, therefore creating non-colliding orbits.

Planets require random start angle around planet, but have a rotational vector to keep orbiting around sun. (centri-pedalforce)

Planets also have random rotation, but currently all in same plane as sun orbit.

MOONS

Each planet has the possiblity for 0 to 20 moons, depending upon mass, and some randomness

Moons require random start angle around planet, but they gain planet velocity vector + rotational vector to keep orbiting around planet. (centri-pedalforce)

Each moon calculated from start, and we add on next distance, therefore creating non-colliding orbits.

Moons have random rotation, but currently in same plane as sun orbit.

Currently no non- disk orbits for planets or moons.


Other things to be considered: (A) planet days, (B) asteroids

*/



/* ==================================================================
 * NAME:        Solar System Setup Constants
 *
 * DESCRIPTION: These constants setup the basic parameters for how
 *              planets, moons, etc. can be generated from the random
 *              solar system generator. This ensures we only generate
 *              fairly balanced solar systems. 
 *
 *              If we had a complete galaxy, some of these parameters
 *              could be relaxed - since an empty solar system might 
 *              be quite good, but since we only generate one solar 
 *              system, we need some balance.
 *
 */

int MSS_sun_seperation_min=0;		// in units of 100,000 km
int MSS_sun_seperation_max=500;		// in units of 100,000 km

int MSS_sun_size_min=500;		// in units of 1000 km
int MSS_sun_size_max=10000;		// in units of 1000 km

int MSS_base_planet_seperation=40;	// place the first planet a minimum of X million km away from the outer sun orbit
int MSS_planet_seperation_min=30;	// in units of million km
int MSS_planet_seperation_max=80;	// in units of million km

int MSS_min_planets=9;			// cannot be 0 - will break
int MSS_max_planets=30;
                         
double MSS_fixed_mass_figure=5.98e24;	   // lets give all planets the same mass as earth

double MSS_planet_distance_scale=1.2;	// how much the distance gets bigger the further you 
int MSS_pds_effect=4;			// start this effect after the fourth

// moon data
double MSS_min_moon_distance=0.2;	// fraction of planets radius above surface
int MSS_min_moons=0;
int MSS_max_moons=30;
int MSS_planet_size_number_of_moons_ratio=2100e3;

double MSS_max_moon_to_planet_ratio=0.29;		// how big compared with planet
double MSS_max_moon_to_planet_ratio_for_gas_giants=0.1; // except for gas giants that have smaller moons
int MSS_minimum_moon_size_of_moon=100;	// 100m is about the smallest moon we want.

int MSS_moon_seperation_min=50;		// units of 1000km
int MSS_moon_seperation_max=600;	// units of 1000km

// spacestation stuff
int MSS_SPACESTATION_SEED=1000;		// give a number between 0 to this one to the spacestation generator
//int MSS_SPACESTATION_DENSITY=6;	// 1 in 6 things have a spacestation
int MSS_SPACESTATION_DENSITY=1;		// 1 in 1 things have a spacestation

int MSS_NUM_SPACESTATION_ALLEGIANCES=9;	// 10 types: human, rmparian, alycian, Kisinte, Xenifor, Kryptonil, Quabriel, Sellenial, Trondent, Outsider

double MSS_min_planet_spacestation_distance=0.15;	// fraction of orbitted radius above surface


// ==================================================================


/*
 * NAME:        load_suns
 *
 * DESCRIPTION: This load up either a unary or binary sun.
 *
 */
// (1) All suns (and planets) orbit 0,0,0
// (2) We only create either unary or binary suns at the moment, since anything more
// complicated gives me a serious brain overload problem. As it is the system really isn't set
// up to handle more than one sun!
double load_suns(void)
{
double base1,size1,mass1;
double base2,size2,mass2;
double seperation_distance,x1,z1,x2,z2;
double orbit_dist1,orbit_dist2;
int angle;
extern int sun_slot1,sun_slot2;


if(RangedRdm(0,2)==0)	// randomise production of suns 
  {
  // ONE SUN		(our sun...  load_sun(150e9,0,0,1.392e9 /*6.96e8*/,1.99e30))
  base1=RangedRdm(MSS_sun_size_min,MSS_sun_size_max);
  size1=base1 * 1e7;			// sun diameter can be between 0.5 million km, and 20 million km
  mass1=base1 * 1.43e27;		// mass can be between 0.7e30kg and 28.6e30kg
  POBs_load_sun(0,0,0,size1,mass1,321,1);	// load Sun1 up. 
  return size1;				// allow _at_least_ radius between surface and next body.
  }
else
  {
  // BINARY SUN
  // Of the three types of binary system, we support only one - the suns in the inside, with the
  //   planets orbiting outside
  base1=RangedRdm(MSS_sun_size_min,MSS_sun_size_max);
  size1=base1 * 1e6;			// sun diameter can be between 0.5 million km, and 20 million km
  mass1=base1 * 1.43e27;		// mass can be between 0.7e30kg and 28.6e30kg
  base2=RangedRdm(MSS_sun_size_min,MSS_sun_size_max);
  size2=base2 * 1e6;			// sun diameter can be between 0.5 million km, and 20 million km
  mass2=base2 * 1.43e27;		// mass can be between 0.7e30kg and 28.6e30kg

// how far apart are they?
  seperation_distance = ((size1+size2)/2) + (RangedRdm(MSS_sun_seperation_min,MSS_sun_seperation_max) * 1e8);	// minimum distance is both radi, max for zex is saturn orbit

// what is the center of gravity? - square of the mass ratio (see calcs below)

  orbit_dist2 = seperation_distance / ((sqrt(mass1/mass2)) +1);
  orbit_dist1 = seperation_distance - orbit_dist2;

// random angle is the same+180 degrees
  angle=RangedRdm(0,360); 
  x1=orbit_dist1*(COS(angle)); z1=orbit_dist1*(SIN(angle));

  angle+=180; if(angle>=360) angle-=360;
  x2=orbit_dist2*(COS(angle)); z2=orbit_dist2*(SIN(angle));
  
// load up the suns into the game
  POBs_load_sun(x1,0,z1,size1,mass1,321,1);		// load Sun1 up. 
  POBs_load_sun(x2,0,z2,size2,mass2,322,2);		// load Sun2 up. 

// (for zex) rotation period is identical - they rotate around each other (?)
// use (nearly) standard orbit routines
  STATIC_set_stable_orbit_vector(sun_slot1,sun_slot2);
  STATIC_set_stable_orbit_vector(sun_slot2,sun_slot1);
  
  if( (orbit_dist1+size1) > (orbit_dist2+size2) )	// find which is further out...
    {
    return orbit_dist1+size1;				// allow _at_least_ radius between surface and next body.
    }
  else
    {
    return orbit_dist2+size2;
    }
    
  } // end of if(single/binary)

} // end of load_suns()


//  randomised sun masses/sizes/colours/light in solar systems (pointless for 1 system)
//
//
// New to do
// =============
//
// PRIMARIES:
//
// ship / space-station placement ---- **** TEST **** (using planets)
// not all planets are appearing - are we running out of slots? throw error + fix
//
// differentiate planets and moons in load_dyn object
// do we need to make some moons into rocks?
//
// SECONARIES:
//
// reduce distance between planets so that they appear bigger in the sky
// make it so stuff appears around a random planet/moon 
// 
// GAME INFO:
//
// antigravity drives - time compression drive. but makes all objects appear virtually still - apart from other ships
// anything larger than 30000km diameter is a gas giant (most planets are gas giants
// 


/*
 * NAME:        my_place_spacestation
 *
 * DESCRIPTION: possibility place a spacestation around a planet
 *
 */
double my_place_spacestation(double orbited_x, double orbited_y, double orbited_z, double object_diameter);
double my_place_spacestation(double orbited_x, double orbited_y, double orbited_z, double object_diameter)
{
double x1,y1,z1,angle,orbit_dist;


//if(RangedRdm(0,MSS_SPACESTATION_DENSITY)==0)	// spacestations are fairly rare - caller now deterimines
//    {
    orbit_dist=(object_diameter/2) * (1+MSS_min_planet_spacestation_distance);

    angle=RangedRdm(0,360); x1=orbit_dist*(COS(angle)); z1=orbit_dist*(SIN(angle));
    x1+=orbited_x; y1=orbited_y; z1+=orbited_z;
    POBs_load_spacestation(x1,y1,z1,RangedRdm(0,MSS_SPACESTATION_SEED),RangedRdm(0,MSS_NUM_SPACESTATION_ALLEGIANCES));
    return (object_diameter/2)*MSS_min_planet_spacestation_distance;	 // allow for object
//    }
//else  // no place spacestation
//    {
//    return 0; // no change in distance
//    }
}

/*
 * NAME:        my_make_moons
 *
 * DESCRIPTION: Make a set of moons (and spacestations) around planet. Also handles
 *		other satellites, e.g. Spacestations.
 */
double my_make_moons(double planet_x, double planet_y, double planet_z, double planet_diameter);
double my_make_moons(double planet_x, double planet_y, double planet_z, double planet_diameter)
{
int n,num_moons;
int seed;
double cd,diameter;	// center distance or cummulative distance
double rotation_time_in_hrs;
double orbit_dist,x1,z1; // x and z rotation around sun
int angle;		 // angle rotated around sun
int max_moons;
double random_fraction,max_moon_size;
int number_of_planet_motherships;
extern int difficulty;

//On higher levels there may be no motherships!
//1=hard, 16=easy
   if (difficulty==1)
   {
     number_of_planet_motherships=RangedRdm(0,4);
   }
   else
   if (difficulty==8)
   {
     number_of_planet_motherships=RangedRdm(3,6);
   }
   else
   if (difficulty==16)
   {
     number_of_planet_motherships=RangedRdm(3,8);
   }
   else
   {
   report_error("Unknown difficulty in my_make_moons","\p FATAL ",4);
   }


cd=planet_diameter/2;

for (n=1;n<number_of_planet_motherships;n++)
{
  cd+=my_place_spacestation(planet_x,planet_y,planet_z,planet_diameter);
}

// cd += my_place_spacestation(planet_x,planet_y,planet_z,planet_diameter);

cd += (planet_diameter/2) * MSS_min_moon_distance;

max_moons=planet_diameter/MSS_planet_size_number_of_moons_ratio;
if(max_moons>MSS_max_moons)	// can't have more than so many moons
  {
  max_moons=MSS_max_moons;
  }
if(max_moons==0) return 0;
num_moons=RangedRdm(MSS_min_moons,max_moons+1);
if(num_moons==0) return 0;

for(n=0; n<num_moons; n++)
  {
  cd+=1e6*RangedRdm(MSS_moon_seperation_min,MSS_moon_seperation_max);	// allow gap between planets

  // lets calculate the possible moon size
  if(planet_diameter < MSS_gas_giant_min_size)
    {
    max_moon_size = planet_diameter * MSS_max_moon_to_planet_ratio; // maximum size
    }
  else
    {
    max_moon_size = planet_diameter * MSS_max_moon_to_planet_ratio_for_gas_giants; // maximum size for gas giants
    }
  
  random_fraction=((double)RangedRdm(0,60000))/60000;
  diameter=((max_moon_size-MSS_minimum_moon_size_of_moon)*random_fraction)+MSS_minimum_moon_size_of_moon;
  cd+=diameter/2;	// ensure we add on half the moon, cause we want to place it at the center
  
  rotation_time_in_hrs=random_planet_rotation_in_hours();

  seed=RangedRdm(0,65535);
  orbit_dist=cd; angle=RangedRdm(0,360); x1=orbit_dist*(COS(angle)); z1=orbit_dist*(SIN(angle));
  //x1=cd; z1=0; // remove the comment on line to make all moons appear in a line
  x1+=planet_x; z1+=planet_z;
  POBs_load_moon(x1,planet_y,z1,seed,diameter,MSS_fixed_mass_figure, rotation_time_in_hrs*60*60);
  cd+=my_place_spacestation(x1,planet_y,z1,diameter);
  cd+=diameter/2;
    
  } // end of for
 
return cd;
} // end of my_make_moons()



/*
 * NAME:        my_make_planets
 *
 * DESCRIPTION: make a set of planets
 *
 */
void my_make_planets(double minimum_distance_away_from_center_in_m)
{
int n,num_planets;
int seed;
double cd,diameter;	// center distance
double orbit_dist,x1,z1; // x and z rotation around sun
int angle;		 // angle rotated around sun
double rotation_time_in_hrs;

#if TEST_SYSTEM==1
return;
#endif


cd=minimum_distance_away_from_center_in_m;
cd+=MSS_base_planet_seperation;

if(MSS_min_planets==0) return;
num_planets=RangedRdm(MSS_min_planets,MSS_max_planets+1);


for(n=0; n<num_planets; n++)
  {
  cd+=1e9*RangedRdm(MSS_planet_seperation_min,MSS_planet_seperation_max);	// allow gap between planets
  diameter = random_planet_size_in_m();
  cd+=diameter/2;

  rotation_time_in_hrs=random_planet_rotation_in_hours();

  seed=RangedRdm(0,65535);
  orbit_dist=cd; angle=RangedRdm(0,360); x1=orbit_dist*(COS(angle)); z1=orbit_dist*(SIN(angle));
  // x1=cd; z1=0; // remove the comment on line to make all planets appear in a line
  POBs_load_planet(x1,0,z1,seed,diameter,MSS_fixed_mass_figure, rotation_time_in_hrs*60*60);
  cd+=my_make_moons(x1,0,z1,diameter);
  
  cd+=diameter/2;
  if(n>MSS_pds_effect)
    {
    cd*=MSS_planet_distance_scale;	// more planets we add, the greater the distance between planets
    }
  }


#if 0
// standard solar system for testing
//            load_planet(    x_in_m, y_in_m, z_in_m, seed, galactic_id, is_inhabited, planet_diameter_in_m, planet_mass_in_kg);
              load_planet(0,0,58e9+cd                     ,1    ,1,1,  4878e3,  3.3e23); // mercury
              load_planet(0,0,108e9+cd                     ,2    ,1,1, 12104e3, 4.87e24); // venus
              load_planet(0,0,150e9+cd                     ,222  ,1,1, 12756e3, 5.98e24); // earth
//              load_planet(0,0,150379e6+cd                     ,100  ,1,1, 3476e3,  7.35e22); // moon
              load_planet(0,0,227e9+cd                     ,4    ,1,1, 6787e3,  6.42e23); // mars
              load_planet(0,0,778e9+cd                     ,5    ,1,1, 142800e3,1.90e27); // jupiter
              load_planet(0,0,1427e9+cd                     ,6    ,1,1, 120660e3,5.69e26); // saturn
              load_planet(0,0,2870e9+cd                     ,7    ,1,1,  51118e3,8.68e25); // uranus
              load_planet(0,0,4497e9+cd                     ,8    ,1,1,  49528e3,1.02e26); // neptune
              load_planet(0,0,5900e9+cd                     ,9    ,1,1,   2300e3,1.29e22); // pluto
#endif



}   // end of my_make_planets()









// Calculations for Central Orbits of Suns
// =======================================
//
// m1 = test mass
//
//         d1    m1      d2
//   o <--------> x <-------------> O
//  Mm                              Me
//     <-------------------------->
//               d3
// 
// Therefore       d3=d1+d2
//
//  and          Fm = (G.m1.Mm)/d1^2    Fe = (G.m1.Me)/d2^2
//
// Since we want to find the point where balance occurs (m1 feels no force)
//
//  Fm = Fe,
//
//  (G.m1.Mm)/d1^2  = (G.m1.Me)/d2^2
//
// G and m1 cancel out.
//
// Mm/d1^2 = Me/d2^2
//
// Multiplying both sides by d1^2 and dividing both sides my Me...
//
// Mm/Me = d1^2/d2^2
//
// sqrt both sizes
//
// sqrt(Mm/Me) = d1/d2
//
// ----------------------------------------
// Since d3=d1+d2, or d2=d3-d1
//
// sqrt(Mm/Me) = d1/(d3-d1)
//
// 1/(sqrt(Mm/Me)) = (d3-d1)/d1
//
// 1/(sqrt(Mm/Me)) = d3/d1 - d1/d1
//
//                 = d3/d1 - 1
//
//               +1 = d3/d1
//
// 1/(sqrt(Mm/Me)) + 1 = d3/d1
// 
// 1/d3 ((1/Sqrt(Mm/Me)) +1 )  = 1/d1
//
// d1 = d3 / (  (1/sqrt(Mm/Me)) +1 )
//
// ----------------------------------------
// Since d1=d3-d2
//
// sqrt(Mm/Me) = (d3-d2)/d2
//
// sqrt(Mm/Me) = d3/d2 - 1
//
// sqrt(Mm/Me) +1 = d3/d2
//
// d2 = d3 / (   (sqrt(Mm/Me)) +1 )
//
// d2 is distance away from earth
//
// d1 is distance away from moon
//


/*

ORBITS
======

geostationary orbit: A circular orbit in the equatorial plane, any point on which revolves about the Earth in the same direction and with the same period
as the Earth's rotation. (188) Note: An object in a geostationary orbit will remain directly above a fixed point on the equator at a distance of approximately
42,164 km from the center of the Earth, i.e. , approximately 35,787 km above mean sea level.


Low Earth Orbit 
When a satellite circles close to Earth we say it's in Low Earth Orbit (LEO). Satellites in LEO are just
200 - 500 miles (320 - 800 kilometers) high. Because they orbit so close to Earth, they must travel
very fast so gravity won't pull them back into the atmosphere. Satellites in LEO speed along at
17,000 miles per hour (27,359 kilometers per hour)! They can circle Earth in about 90 minutes. 


This from http://www.badastronomy.com/bad/movies/spacecowboys2.html

The highest the Shuttle can get is about 900 kilometers (560 miles),
but that's a highly elliptical orbit and a tough one to manage. Its usual height off the Earth's surface is
about 300 kilometers or a bit more. 
                  
Hubble ... orbits at roughly 600 kilometers     

Earth Diameter 12756km, Earth radius 6378km.


  The Moon is the only natural satellite of Earth: 

        orbit:    384,400 km from Earth
        diameter: 3476 km
        mass:     7.35e22 kg
        
        
        
        

Sun
radius:               6.96 x 10^5 km
mass:                 1.99 x 10^30 kg

av density:			  
length of day:  	  26-35 days



Mercury

Mass (kg)............................................3.3 x 10^23
Diameter (km)........................................4878
Mean density (kg/m^3) ...............................5420

Average distance from Sun (AU).......................0.387
Rotation period (length of day in Earth days)........58.65
Revolution period (length of year in Earth days).....87.97




Venus

Mass (kg)............................................4.87 x 10^24
Diameter (km)........................................12104
Mean density (kg/m^3) ...............................5250

Average distance from Sun (AU).......................0.723
Rotation period (length of day in Earth days)........243.0 (retrograde)
Revolution period (length of year in Earth days).....224.7



Earth

Mass (kg)............................................5.98 x 10^24
Diameter (km)........................................12756
Mean density (kg/m^3) ...............................5520

Average distance from Sun (AU).......................1
Rotation period (length of day in Earth hours).......23.93
Revolution period (length of year in Earth days).....365.26



Mars

Mass (kg)............................................6.42 x 10^23
Diameter (km)........................................6787
Mean density (kg/m^3) ...............................3940

Average distance from Sun (AU).......................1.524
Rotation period (length of day in Earth days)........1.026
Revolution period (length of year in Earth days).....686.98
                                                     



Jupiter

Mass (kg)............................................1.90 x 10^27
Diameter (km)........................................142,800
Mean density (kg/m^3) ...............................1314

Average distance from Sun (AU).......................5.203
Rotation period (length of day in Earth hours).......9.8
Revolution period (length of year in Earth years)....11.86

Rings................................................Faint ring.
                      Infrared spectra imply dark rock fragments.
                      
                      
Saturn

Mass (kg)............................................5.69 x 10^26
Diameter (km)........................................120660
Mean density (kg/m^3) ...............................690

Average distance from Sun (AU).......................9.539
Rotation period (length of day in Earth hours).......10.2
Revolution period (length of year in Earth years)....29.46

Rings................................................Rings are 
        270,000 km in diameter, but only a few hundred meters thick. 
        Particles are centimeters to decameters in size and are ice (some
        may be covered with ice); there are traces of silicate 
        and carbon minerals. There are four main ring groups and three
        more faint, narrow ring groups separated by gaps called divisions.


Uranus

Mass (kg)............................................8.68 x 10^25
Diameter (km)........................................51118
Mean density (kg/m^3) ...............................1290

Average distance from Sun (AU).......................19.18
Rotation period (length of day in Earth hours).......17.9
Revolution period (length of year in Earth years)....84

Rings................................................Uranus has 
        a system of narrow, faint rings. Ring particles are dark, 
        and could consist of rocky or carbonaceous material.
        
        




Neptune

Mass (kg)............................................1.02 x 10^26
Diameter (km)........................................49528
Mean density (kg/m^3) ...............................1640

Average distance from Sun (AU).......................30.06
Rotation period (length of day in Earth hours).......19.1
Revolution period (length of year in Earth years)....164.8

Rings................................................Rings are 
                          narrow, and contain concentrations of 
                          particles called ring arcs.
                          
                          




Pluto

Mass (kg)............................................1.29 x 10^22
Diameter (km)........................................2300
Mean density (kg/m^3) ...............................2030

Average distance from Sun (AU).......................39.53
Rotation period (length of day in Earth days)........6.39
Revolution period (length of year in Earth years)....247.7



                                                                                                          





                             
                            for a Sun-sized star (1 solar mass):
                            1.

                                 The star will eventually expand up to one hundred times its size, becoming a
                                 "red giant."
                            2.

                                 As the helium in its core is used up, the star begins to shrink.
                            3.

                                 The gravitational force in its core becomes so great that it crushes the star,
                                 creating a "white dwarf."
                            4.

                                 The white dwarf continues for a long period of time to give off excess light.
                             
                            for a small star:
                            1.

                                 The star exhausts its fuel.
                            2.

                                 The gravitational force from within its center overpowers the star and it is
                                 crushed into a "brown dwarf."
                             
                            for a large star (10 solar masses):
                            1.

                                 The star exhausts its fuel and begins to expand.
                            2.

                                 The star expands into a huge fireball - a "supergiant."
                            3.

                                 The supergiant explodes violently, radiating an immense amount of energy. This
                                 explosion is the "supernova." Many nebulae come from supernovae - the Crab
                                 Nebula is one of these. Four supernovae have been observed in the last
                                 thousand years: The Chinese witnessed one in 1054 AD, Tycho Brahe in 1572,
                                 Kepler in 1604, and the most recent occurred in 1987.
                            4.

                                 The reamining part of the star is crushed by gravity and supercompressed into an
                                 extremely dense ball of matter - a "neutron star." Neutron stars are so named
                                 because they are compressed so much that the protons and electrons within the
                                 atoms are crushed together into neutrons. Pulsars are a type of neutron star that
                                 rotates and emit radio waves, sometimes resulting in a bit of unnecessary
                                 excitement by astronomers on Earth. (The regular radio pulse of the pulsar was
                                 initially thought to be a signal from an alien race.) The Crab pulsar is a neutron
                                 star in the Crab Nebula.
                             
                            for an extremely large star (30 solar masses):
                            1.

                                 The star exhausts its fuel and begins to expand.
                            2.

                                 The star expands into a supergiant and experiences a supernova explosion.
                            3.

                                 The huge mass of the star causes such powerful gravitational forces from the
                                 center that it becomes infinitely compressed.
                            4.

                                 The resulting object is called a "black hole," from which nothing can escape. A
                                 black hole is also known as a singularity, from which scientists believe the
                                 universe was created. Theoretically, the universe could compress itself back into
                                 a singularity and explode, beginning the process again and repeating forever.



Sun
 radius:               6.96 x 10^5 km

 mass:                 1.99 x 10^30 kg

                                      density:
                                            
                                            

                                               convection zone: 0.1 g/cm3 
                                               radiative zone: 1 g/cm3 
                                               core: 100 g/cm3

                                  length of day:

                                               26-35 days

                                   temperature:
                                            

                                               surface: 5.5 x 10^3¡ C 
                                               core: 1.5 x 10^7¡ C

                                   composition:

                                               H2 (75%), He (25%)



Mercury



Planet Profile

Mass (kg)............................................3.3 x 10^23
Diameter (km)........................................4878
Mean density (kg/m^3) ...............................5420
Escape velocity (m/sec)..............................4300

Average distance from Sun (AU).......................0.387
Rotation period (length of day in Earth days)........58.65
Revolution period (length of year in Earth days).....87.97

Obliquity (tilt of axis in degrees)..................0
Orbit inclination (degrees)..........................7
Orbit eccentricity (deviation from circular).........0.206

Mean surface temperature (K).........................452
Maximum surface temperature (K)......................700
Minimum surface temperature (K)......................100

Visual geometric albedo (reflectivity)...............0.12
Largest known surface feature........................Caloris Basin 
                                                (1350 km diameter)
Atmospheric components...............................trace amounts of 
                                                     hydrogen and 
                                                     helium
                                                     




Venus



Planet Profile

Mass (kg)............................................4.87 x 10^24
Diameter (km)........................................12104
Mean density (kg/m^3) ...............................5250
Escape velocity (m/sec)..............................10400

Average distance from Sun (AU).......................0.723
Rotation period (length of day in Earth days)........243.0 (retrograde)
Revolution period (length of year in Earth days).....224.7

Obliquity (tilt of axis degrees).....................178
Orbit inclination (degrees)..........................3.39
Orbit eccentricity (deviation from circular).........0.007

Mean surface temperature (K).........................726

Visual geometric albedo (reflectivity)...............0.59
Highest point on surface.............................Maxwell Montes 
                                (17 km above mean planetary radius)
Atmospheric components...............................96% carbon dioxide, 
                                                      3% nitrogen,  
                                                      0.003% water vapor
Surface materials....................................basaltic rock and 
                                                     altered materials


Earth



Planet Profile

Mass (kg)............................................5.98 x 10^24
Diameter (km)........................................12756
Mean density (kg/m^3) ...............................5520
Escape velocity (m/sec)..............................11200

Average distance from Sun (AU).......................1
Rotation period (length of day in Earth hours).......23.93
Revolution period (length of year in Earth days).....365.26

Obliquity (tilt of axis in degrees)..................23.4
Orbit inclination (degrees)..........................0
Orbit eccentricity (deviation from circular).........0.017

Mean surface temperature (K).........................281
Maximum surface temperature (K)......................310
Minimum surface temperature (K)......................260

Visual geometric albedo (reflectivity)...............0.39
Highest point on surface.............................Mount Everest 
                                       (over 8 km above sea-level)
Atmospheric components...............................78% nitrogen, 
                                                     21% oxygen, 
                                                      1% argon
Surface materials....................................basaltic and 
                                                granitic rock and 
                                                altered materials




Mars



Planet Profile

Mass (kg)............................................6.42 x 10^23
Diameter (km)........................................6787
Mean density (kg/m^3) ...............................3940
Escape velocity (m/sec)..............................5000

Average distance from Sun (AU).......................1.524
Rotation period (length of day in Earth days)........1.026
Revolution period (length of year in Earth days).....686.98

Obliquity (tilt of axis in degrees)..................25
Orbit inclination (degrees)..........................1.85
Orbit eccentricity (deviation from circular).........0.093

Maximum surface temperature (K)......................310
Minimum surface temperature (K)......................150

Visual geometric albedo (reflectivity)...............0.15
Highest point on surface.............................Olympus Mons 
                       (about 24 km above surrounding lava plains)
Atmospheric components...............................95% carbon dioxide, 
                                                      3% nitrogen, 
                                                      1.6% argon
Surface materials....................................basaltic rock and 
                                                     altered materials
                                                     
                                                     



Jupiter



Planet Profile

Mass (kg)............................................1.90 x 10^27
Diameter (km)........................................142,800
Mean density (kg/m^3) ...............................1314
Escape velocity (m/sec)..............................59500

Average distance from Sun (AU).......................5.203
Rotation period (length of day in Earth hours).......9.8
Revolution period (length of year in Earth years)....11.86

Obliquity (tilt of axis in degrees)..................3.08
Orbit inclination (degrees)..........................1.3
Orbit eccentricity (deviation from circular).........0.048

Mean surface temperature (K).........................120 (cloud tops)

Visual geometric albedo (reflectivity)...............0.44
Atmospheric components...............................90% hydrogen,
                                                     10% helium, 
                                                       .07% methane
Rings................................................Faint ring.
                      Infrared spectra imply dark rock fragments.
                      
                      
                      
                      



Saturn



Planet Profile

Mass (kg)............................................5.69 x 10^26
Diameter (km)........................................120660
Mean density (kg/m^3) ...............................690
Escape velocity (m/sec)..............................35600

Average distance from Sun (AU).......................9.539
Rotation period (length of day in Earth hours).......10.2
Revolution period (length of year in Earth years)....29.46

Obliquity (tilt of axis in degrees)..................26.7
Orbit inclination (degrees)..........................2.49
Orbit eccentricity (deviation from circular).........0.056

Mean temperature (K).................................88 K (1 bar level)

Visual geometric albedo (reflectivity)...............0.46
Atmospheric components...............................97% hydrogen, 
                                                      3% helium, 
                                                       .05% methane
Rings................................................Rings are 
        270,000 km in diameter, but only a few hundred meters thick. 
        Particles are centimeters to decameters in size and are ice (some
        may be covered with ice); there are traces of silicate 
        and carbon minerals. There are four main ring groups and three
        more faint, narrow ring groups separated by gaps called divisions.






Uranus



Planet Profile

Mass (kg)............................................8.68 x 10^25
Diameter (km)........................................51118
Mean density (kg/m^3) ...............................1290
Escape velocity (m/sec)..............................21300

Average distance from Sun (AU).......................19.18
Rotation period (length of day in Earth hours).......17.9
Revolution period (length of year in Earth years)....84

Obliquity (tilt of axis in degrees)..................97.9
Orbit inclination (degrees)..........................0.77
Orbit eccentricity (deviation from circular).........0.047

Mean temperature (K).................................59

Visual geometric albedo (reflectivity)...............0.56
Atmospheric components...............................83% hydrogen, 
                                                     15% helium, 
                                                      2% methane (at depth)
Rings................................................Uranus has 
        a system of narrow, faint rings. Ring particles are dark, 
        and could consist of rocky or carbonaceous material.
        
        




Neptune



Planet Profile

Mass (kg)............................................1.02 x 10^26
Diameter (km)........................................49528
Mean density (kg/m^3) ...............................1640
Escape velocity (m/sec)..............................23300

Average distance from Sun (AU).......................30.06
Rotation period (length of day in Earth hours).......19.1
Revolution period (length of year in Earth years)....164.8

Obliquity (tilt of axis in degrees)..................29.6
Orbit inclination (degrees)..........................1.77
Orbit eccentricity (deviation from circular).........0.009

Mean temperature (K).................................48

Visual geometric albedo (reflectivity)...............0.51
Atmospheric components...............................74% hydrogen, 
                                                     25% helium,
                                                      1% methane (at depth)
Rings................................................Rings are 
                          narrow, and contain concentrations of 
                          particles called ring arcs.
                          
                          




Pluto



Planet Profile

Mass (kg)............................................1.29 x 10^22
Diameter (km)........................................2300
Mean density (kg/m^3) ...............................2030
Escape velocity (m/sec)..............................1100

Average distance from Sun (AU).......................39.53
Rotation period (length of day in Earth days)........6.39
Revolution period (length of year in Earth years)....247.7

Obliquity (tilt of axis in degrees)..................122.5
Orbit inclination (degrees)..........................17.15
Orbit eccentricity (deviation from circular).........0.248

Mean temperature (K).................................37

Visual geometric albedo (reflectivity)...............about 0.5

Atmospheric components...............................perhaps 
                                                     methane and 
                                                     nitrogen 
Surface materials....................................perhaps 
                                                     methane ice







Masses and Orbital Characteristics of Extrasolar Planets

     Star Name

                                    Msini 
                                    (Mjup)

                                                 Period 
                                                 (d)

                                                               Semimajor 
                                                               Axis (AU)

                                                                                    Eccentricity

                                                                                                          K 
                                                                                                          (m/s)

 1
     HD 187123
                                    0.52
                                                 3.097
                                                               0.042
                                                                                    0.00 
                                                                                                          72 
 2
     Tau Bootis
                                    3.64
                                                 3.3126
                                                               0.042
                                                                                    0.00 
                                                                                                          469 
 3
     HD 75289
                                    0.42
                                                 3.508
                                                               0.047 
                                                                                    0.00 
                                                                                                          54 
 4
     51 Pegasi
                                    0.44
                                                 4.2308
                                                               0.051
                                                                                    0.01
                                                                                                          56. 
 5
     Upsilon Andromedae (b)
                                    0.69
                                                 4.617
                                                               0.059
                                                                                    0.04 
                                                                                                          73. 
 6
     Upsilon Andromedae (c)
                                    2.0
                                                 241.3
                                                               0.82
                                                                                    0.23 
                                                                                                          54. 
 7
     Upsilon Andromedae (d)
                                    4.1
                                                 1280.6
                                                               2.4
                                                                                    0.31 
                                                                                                          67. 
 8
     HD 217107
                                    1.28
                                                 7.11
                                                               0.07
                                                                                    0.14 
                                                                                                          140 
 9
     Rho1 55 Cancri
                                    0.85
                                                 14.656
                                                               0.12
                                                                                    0.03 
                                                                                                          75.8 
 10
     Gliese 86
                                    3.6
                                                 15.8
                                                               0.11
                                                                                    0.04 
                                                                                                          379 
 11
     HD 195019
                                    3.43
                                                 18.3
                                                               0.14
                                                                                    0.05 
                                                                                                          268 
 12
     Rho Corona Borealis
                                    1.1
                                                 39.6
                                                               0.23
                                                                                    0.1 
                                                                                                          67. 
 13
     HD 168443
                                    5.04
                                                 58
                                                               0.28
                                                                                    0.54 
                                                                                                          330 
 14
     Gliese 876
                                    2.1
                                                 60.9
                                                               0.21
                                                                                    0.27 
                                                                                                          239. 
 15
     HD114762
                                    11.0
                                                 84
                                                               0.41
                                                                                    0.33 
                                                                                                          619 
 16
     70 Virginis
                                    7.4
                                                 116.7
                                                               0.47
                                                                                    0.40 
                                                                                                          316.8 
 17
     HD 210277
                                    1.36
                                                 437
                                                               1.15
                                                                                    0.45 
                                                                                                          41 
 18
     16 Cygni B
                                    1.74
                                                 802.8
                                                               1.70
                                                                                    0.68 
                                                                                                          52.2 
 19
     47 Ursae Majoris
                                    2.42
                                                 1093
                                                               2.08
                                                                                    0.10 
                                                                                                          47.2 
 20
     14 Herculis
                                    4
                                                 ~2000
                                                               ~3
                                                                                    ~0.35 
                                                                                                          80 
                                                                                                          
                                                                                                          
                                                                                                          




Bad: It becomes clear that Blake will die, and no one can save him. He knows his wife will try, even
                  though it might kill her too. To save her, he kills himself by taking his helmet off. Instantly, he freezes,
                  and his face becomes distorted and icky. 

                  Good: Again, he wouldn't freeze that fast. The human body holds a lot of heat, and it would take a long
                  time to freeze. Have you ever found yourself literally freezing solid when you go outside on a winter's
                  day? No, of course not. Yet losing heat by transferring it to cold air is far faster than doing so by simply
                  radiating it away, as you would in space. If it takes a long time in frigid air, it would take even longer in
                  frigid space. 

                  His face would not get icky, either; at least, not right away. There is no air in space. Despite what movies
                  like ``Outland'' would have you believe, you wouldn't explode if exposed to space. The air would rush
                  out of your lungs explosively, which could damage your lungs and throat. But your skin and vascular
                  system would contain your blood fairly well, protecting it against the vacuum, so your blood would not
                  boil. After a long time, the cells would freeze, and the expanding ice crystals (water expands when it
                  freezes) would rupture the cells, making you look a pretty good mess. But that would take hours, or
                  longer, not seconds. Oddly though, a human can survive several seconds of pure vacuum. ``2001: A
                  Space Odyssey'' had a scene like this; the astronaut had to go from his pod to the airlock without his
                  helmet. I don't think he would look as nicly coiffed after the incident as he did in the movie, but the scene
                  was largely accurate. Ironically, as Arthur C. Clarke has pointed out, in Earth orbit, your biggest problem
                  is sunburn! Exposure to the UV light of the Sun would fry your skin in seconds, giving you a very bad
                  sunburn. I'd be satisfied to live with that, given the alternative! For more info about what happens when a
                  human is exposed to a vacuum, read the Imagine the Universe! website. They have real-life examples
                  discussed too. Very cool. 
                  
                  
                  
http://imagine.gsfc.nasa.gov/docs/ask_astro/answers/970603.html



                                                                 
                                              enter search text 


           Imagine Home  |   Ask A High Energy Astronomer  |   Current page 



             The Question

             How would the unprotected human body react to the vacuum of outer space? Would it inflate to bursting? or would it not?
             or would just the interior gases hyperinflate? We are also relating this to short-term exposure only. This question
             primarily relates to the pressure differential problems. Temperature or radiation considerations would be interesting as
             well. 

             The question arose out of a discussion of the movie 2001. When Dave "blew" himself into the airlock from the pod
             without a helmet, should he have "blown up" or is there "no difference" as shown in the movie correct? 

             The Answer

             From the now extinct page http://medlib/jsc.nasa.gov/intro/vacuum.html: 

             How long can a human live unprotected in space? 

             If you don't try to hold your breath, exposure to space for half a minute or so is unlikely to produce permanent injury.
             Holding your breath is likely to damage your lungs, something scuba divers have to watch out for when ascending, and
             you'll have eardrum trouble if your Eustachian tubes are badly plugged up, but theory predicts -- and animal experiments
             confirm -- that otherwise, exposure to vacuum causes no immediate injury. You do not explode. Your blood does not
             boil. You do not freeze. You do not instantly lose consciousness. 

             Various minor problems (sunburn, possibly "the bends", certainly some [mild, reversible, painless] swelling of skin and
             underlying tissue) start after ten seconds or so. At some point you lose consciousness from lack of oxygen. Injuries
             accumulate. After perhaps one or two minutes, you're dying. The limits are not really known. 

             You do not explode and your blood does not boil because of the containing effect of your skin and circulatory system.
             You do not instantly freeze because, although the space environment is typically very cold, heat does not transfer away
             from a body quickly. Loss of conciousness occurs only after the body has depleted the supply of oxygen in the blood. If
             your skin is exposed to direct sunlight without any protection from its intense ultraviolet radiation, you can get a very bad
             sunburn. 

             At NASA's Manned Spacecraft Center (now renamed Johnson Space Center) we had a test subject accidentally exposed
             to a near vacuum (less than 1 psi) in an incident involving a leaking space suit in a vacuum chamber back in '65. He
             remained concious for about 14 seconds, which is about the time it takes for O2 deprived blood to go from the lungs to
             the brain. The suit probably did not reach a hard vacuum, and we began repressurizing the chamber within 15 seconds.
             The subject regained conciousness at around 15,000 feet equivalent altitude. The subject later reported that he could feel
             and hear the air leaking out, and his last concious memory was of the water on his tongue beginning to boil. 

             Aviation Week and Space Technology (02/13/95) printed a letter by Leonard Gordon which reported another
             vacuum-packed anecdote: 

             "The experiment of exposing an unpressurized hand to near vacuum for a significant time while the pilot went about his
             business occurred in real life on Aug. 16, 1960. Joe Kittinger, during his ascent to 102,800 ft (19.5 miles) in an open
             gondola, lost pressurization of his right hand. He decided to continue the mission, and the hand became painful and
             useless as you would expect. However, once back to lower altitudes following his record-breaking parachute jump, the
             hand returned to normal." 

             References: 

             Frequently Asked Questions on sci.space. sci.astro 

             The Effect on the Chimpanzee of Rapid Decompression to a Near Vacuum, Alfred G. Koestler ed., NASA CR-329 (Nov
             1965). 

             Experimental Animal Decompression to a Near Vacuum Environment, R.W. Bancroft, J.E. Dunn, eds, Report
             SAM-TR-65-48 (June 1965), USAF School of Aerospace Medicine, Brooks AFB, Texas. 

             Survival Under Near-Vacuum Conditions in the article "Barometric Pressure," by C.E. Billings, Chapter 1 of
             Bioastronautics Data Book, Second edition, NASA SP-3006, edited by James F. Parker Jr. and Vita R. West, 1973. 

             Personal communication, James Skipper, NASA/JSC Crew Systems Division, December 14, 1994. 



             Henry Spencer wrote the following for the sci.space FAQ: 

             How Long Can a Human Live Unprotected in Space? 

             If you *don't* try to hold your breath, exposure to space for half a minute of so is unlikely to produce permanent injury.
             Holding your breath is likely to damage your lungs, something scuba divers have to watch out for when ascending, and
             you'll have eardrum trouble if your Eustachian tubes are badly plugged up, but theory predicts -- and animal experiments
             confirm -- that otherwise, exposure to vacuum causes no immediate injury. You do not explode. Your blood does not
             boil. You do not freeze. You do not instantly lose consciousness. 

             Various minor problems (sunburn, possibly "the bends", certainly some [mild, reversible, painless] swelling of skin and
             underlying tissue) start after 10 seconds or so. At some point you lose consciousness from lack of oxygen. Injuries
             accumulate. After pehaps one or two minutes you're dying. The limits are not really known. 

             References: 

             The Effect on the Chimpanzee of Rapid Decompression to a Near Vacuum, Alfred G. Koestler ed., NASA CR-329 (Nov.
             1965) 

             Experimental Animal Decompression to a Near Vacuum Environment, R.W. Bancroft, J.E. Dunn, eds, Report
             SAM-TR-65-48 (June 1965), USAF School of Aerospace Medicine, Brooks AFB, Texas. 



             You would probably pass out in around 15 seconds because your lungs are now exchanging oxygen out of the blood.
             The reason that a human does not burst is that our skin has some strength. For instance compressed oxygen in a steel tank
             may be at several hundreds times the pressure of the air outside and the strength of the steel keeps the cylinder from
             breaking. Although our skin is not steel, it still is strong enough to keep our bodies from bursting in space. 

             Also, the vapor pressure of water at 37 C is 47 mm Hg. As long as you keep your blood-pressure above that (which you
             will unless you go deep into shock) your blood will not boil. My guess is that the body seems to regulate blood pressure
             as a gauge, rather than absolute pressure (e.g. your blood vessels don't collapse when you dive 10 feet into a pool). 

             The saliva on your tongue might boil, however. 

             For more information and references, see http://www.sff.net/people/geoffrey.landis/vacuum.html 

             Hope this helps!
             The Ask a High-Energy Astronomer Team 

             Questions on this topic are no longer responded to by the "Ask a High-Energy Astronomer" service. See
             http://imagine.gsfc.nasa.gov/docs/ask_astro/ask_an_astronomer.html for help on other astronomy Q&A services.


                                                     Prev

                                                           Main

                                                                Next


           Home
                           Imagine Science
                                                      InterActivites
                                                                                 Satellites and Data
           Dictionary
                           Resources
                                                      Teacher's Corner
                                                                                 Feedback
                                                Ask a High Energy Astronomer


          Having trouble printing?

          Imagine the Universe is a service of the High Energy Astrophysics Science Archive Research Center (HEASARC), Dr. Nicholas
          White (Director), within the Laboratory for High Energy Astrophysics at NASA's Goddard Space Flight Center.

          The Imagine Team
          Project Leader: Dr. Jim Lochner

          NASA's privacy statement

          Do you have a question, problem or comment about this web site? Please let us know.



*/