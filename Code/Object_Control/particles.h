/*
 *  particles.h
 *  zex22
 *
 *  Created by Stuart Ball on Tue May 28 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

void handle_particle_generator_1(int it);
void handle_particle_generator_cannon(int it);
void handle_particle_type_laser(int it);
void handle_particle_type_shell(int it);
void handle_particle_generator_exhaust(int vt);
void handle_particle_type_exhaust(int it);

void kill_gun_particles(int vt, int parent, int particle_bunch_id);