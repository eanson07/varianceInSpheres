#ifndef SPHERE_H
#define SPHERE_H

#include "particle.h"

struct pos
{
    float x, y, z;
};



/* SPHERE */

struct sphere;


typedef struct pos pos_t;
typedef struct sphere * sphere_t;


// offers a particle's position to a sphere, to be "accepted" if it's within the sphere
// returns 1 if accepted, 0 if rejected or if s is NULL
int sphere_offer_pos(sphere_t s, pos_t p);


// offers a particle to a sphere, to be "accepted" if it's within the sphere
void sphere_offer_part(sphere_t s, particle_t p);


// returns the radius of the sphere
float sphere_radius(sphere_t s);

// returns the number of particles "accepted" by the sphere so far
int sphere_nPoints(sphere_t s);

// initializes a new sphere at the given location with the given radius
sphere_t sphere_new(pos_t center, float radius);

// frees memory for the given sphere
void sphere_free(sphere_t s);




/* SPHERE FAMILY (for statistical analysis of multiple equal-sized spheres) */

struct sphere_family;

typedef struct sphere_family * family_t;


// create a family of "nSpheres" spheres of radius "radius"
// randomly distributed in a cube of length boxL
family_t family_new(float boxL, float radius, int nSpheres);


// frees resources for family
void family_free(family_t fam);

// returns the mean number of particles per sphere
float family_count_mu(family_t fam);

// returns the variance in the number of particles per sphere
float family_count_sig2(family_t fam);

// returns the estimated uncertainty in the variance in the number of particles per sphere
float family_count_sig2_err(family_t fam);

// returns the mean number density of particles in the spheres
float family_num_density_mu(family_t fam);

// returns the variance in the number density of particles in the spheres
float family_num_density_sig2(family_t fam);

// returns the estimated error in the variance in the number density of particles in the spheres
float family_num_density_sig2_err(family_t fam);

// returns the number of spheres
int family_n_spheres(family_t fam);

// returns the radius of the spheres in the family
float family_radius(family_t fam);

// offers a particle's position to a sphere family,
// to be "accepted" by each sphere if it's within that sphere
void family_offer_pos(family_t fam, pos_t p);



#endif