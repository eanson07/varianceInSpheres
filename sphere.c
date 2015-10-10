#include <stdlib.h>
#include <math.h>
#include "sphere.h"
#include "randUtils.h"


struct sphere
{
    pos_t center;
    float r, r2;
    int count;
};


// returns the radius of the sphere
float sphere_radius(sphere_t s)
{
    return s->r;
}

// offers a particle's position to a sphere, to be "accepted" if it's within the sphere
int sphere_offer_pos(sphere_t s, pos_t p)
{
    float dx, dy, dz;
    if(s == NULL)
        return 0;
    dx = s->center.x - p.x;
    dy = s->center.y - p.y;
    dz = s->center.z - p.z;
    
    if(dx*dx + dy*dy + dz*dz <= s->r2)
    {
        s->count++;
        return 1;
    }
    return 0;
}

// offers a particle position to a sphere, to be "accepted" if it's within the sphere
// applies periodic boundaries at length boxL
static int sphere_offer_periodic(sphere_t s, pos_t p, float boxL)
{
    float dx, dy, dz;
    if(s == NULL)
        return 0;
    dx = abs(s->center.x - p.x);
    dy = abs(s->center.y - p.y);
    dz = abs(s->center.z - p.z);
    
    if(dx > boxL - dx)
        dx = boxL - dx;
    if(dy > boxL - dy)
        dy = boxL - dy;
    if(dz > boxL - dz)
        dz = boxL - dz;
    
    if(dx*dx + dy*dy + dz*dz <= s->r2)
    {
        s->count++;
        return 1;
    }
    return 0;
}



// returns the number of particles "accepted" by the sphere so far
int sphere_nPoints(sphere_t s)
{
    if(s == NULL)
        return -1;
    return s->count;
}

// initializes a new sphere at the given location with the given radius
sphere_t sphere_new(pos_t center, float radius)
{
    sphere_t ret = (sphere_t)malloc(sizeof(struct sphere));
    if(ret == NULL)
        return NULL;
    ret->center = center;
    ret->r = radius;
    ret->r2 = radius * radius;
    ret->count = 0;
    return ret;
}

// frees memory for the given sphere
void sphere_free(sphere_t s)
{
    if(s != NULL)
        free(s);
}


/**************************************/


/* SPHERE FAMILY (for statistical analysis of multiple equal-sized spheres) */

static pos_t random_pos(float boxL)
{
    pos_t ret;
    ret.x = urand() * boxL;
    ret.y = urand() * boxL;
    ret.z = urand() * boxL;
    return ret;
}


struct sphere_family
{
    float boxL, r, V;
    int nSpheres;
    int sum, sum2;
    sphere_t *spheres;
};


// create a family of "nSpheres" spheres of radius "radius"
// randomly distributed in a cube of length boxL
family_t family_new(float boxL, float radius, int nSpheres)
{
    int i;
    family_t ret = (family_t)malloc(sizeof(struct sphere_family));
    if(ret == NULL)
        return NULL;
    ret->boxL = boxL;
    ret->r = radius;
    ret->V = 4./3 * M_PI * radius * radius * radius;
    ret->nSpheres = nSpheres;
    ret->sum = ret->sum2 = 0;
    ret->spheres = (sphere_t *)malloc(nSpheres * sizeof(sphere_t));
    if(ret->spheres == NULL)
    {
        free(ret);
        return NULL;
    }
    for(i = 0; i < nSpheres; ++i)
    {
        pos_t center = random_pos(boxL);
        sphere_t s = sphere_new(center, radius);
        if( s == NULL )
        {
            int j;
            for(j = 0; j < i; ++j)
                sphere_free(ret->spheres[j]);
            free(ret->spheres);
            free(ret);
            return NULL;
        }
        ret->spheres[i] = s;
    }
    return ret;
}


// frees resources for family
void family_free(family_t fam)
{
    int i;
    if(fam == NULL)
        return;
    for(i = 0; i < fam->nSpheres; ++i)
        sphere_free(fam->spheres[i]);
    free(fam->spheres);
    free(fam);
}

// returns the mean number of particles per sphere
float family_count_mu(family_t fam)
{
    return (float)fam->sum / fam->nSpheres;
}

// returns the variance in the number of particles per sphere
float family_count_sig2(family_t fam)
{
    int n = fam->nSpheres;
    float mu = (float)fam->sum / n;
    float mu2 = (float)fam->sum2 / n;
    return (mu2 - mu*mu)*n/(n-1);
}

// returns the estimated uncertainty in the variance in the number of particles per sphere
float family_count_sig2_err(family_t fam)
{
    int n = fam->nSpheres;
    return family_count_sig2(fam) * sqrt(2./(n-1));
}

// returns the mean number density of particles in the spheres
float family_num_density_mu(family_t fam)
{
    return (float)fam->sum / fam->nSpheres / fam->V;
}

// returns the variance in the number density of particles in the spheres
float family_num_density_sig2(family_t fam)
{
    return family_count_sig2(fam) / (fam->V * fam->V);
}

// returns the estimated error in the variance in the number density of particles in the spheres
float family_num_density_sig2_err(family_t fam)
{
    int n = fam->nSpheres;
    return family_num_density_sig2(fam) * sqrt(2./(n-1));
}


// returns the number of spheres
int family_n_spheres(family_t fam)
{
    return fam->nSpheres;
}

// returns the radius of the spheres in the family
float family_radius(family_t fam)
{
    return fam->r;
}

// offers a particle's position to a sphere family,
// to be "accepted" by each sphere if it's within that sphere
void family_offer_pos(family_t fam, pos_t p)
{
    int i;
    for(i = 0; i < fam->nSpheres; ++i)
    {
        sphere_t s = fam->spheres[i];
        if( sphere_offer_periodic(s, p, fam->boxL) )
        {
            int newCount = s->count;
            fam->sum++;
            fam->sum2 += 2 * newCount - 1;
        }
    }
}

