#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sphere.h"

const int POS_BUF_SIZE = 128*1024, PARTICLE_INDEX = 1;
const int DEFAULT_N_FAMILIES = 10, DEFAULT_N_SPHERES = 1000;
const int MIN_PART_PER_MEAN_SPHERE = 40;
const int MIN_SPHERES_PER_BOX_VOL = 200;


struct snap_head
{
    int      blockSize1;
    int      npart[6];
    double   mass[6];
    double   time;
    double   redshift;
    int      flag_sfr;
    int      flag_feedback;
    int      npartTotal[6];
    int      flag_cooling;
    int      num_files;
    double   BoxSize;
    double   Omega0;
    double   OmegaLambda;
    double   HubbleParam;
    char     fill[256- 6*sizeof(int)- 6*sizeof(double)- 2*sizeof(double)- 2*sizeof(int)- 6*sizeof(int)- 2*sizeof(int) - 4*sizeof(double)];  /* fills to 256 Bytes */
    int      blockSize2;
};

typedef struct pos pos_t;
typedef struct snap_head head_t;


int main(int args, char *argv[])
{
    int nread, unread, i, block_size, nFamilies = DEFAULT_N_FAMILIES;
    float rmin, rmax, logrmin, logrmax, dlogr;
    float mass;
    FILE *file = stdin;
    pos_t pos_buf[POS_BUF_SIZE];
    head_t head;
    family_t *families;
    
    if(args > 1)
    {
        file = fopen(argv[1],"r");
        if(file == NULL)
        {
            fprintf(stderr, "ERROR: couldn't open %s for reading.\n", argv[1]);
            return 1;
        }
    }
    
    fread(&head, sizeof(head), 1, file);
    mass = head.mass[PARTICLE_INDEX];
    
    fprintf(stderr, "Constructing spheres...\n");
    // construct spheres
    families = (family_t *)malloc(nFamilies * sizeof(family_t));
    rmin = head.BoxSize * pow( 3. * MIN_PART_PER_MEAN_SPHERE / (4. * M_PI * head.npart[PARTICLE_INDEX]), 1./3 );
    rmax = head.BoxSize * pow( 3./(4. * M_PI * MIN_SPHERES_PER_BOX_VOL), 1./3);
    logrmin = log(rmin);
    logrmax = log(rmax);
    dlogr = (logrmax - logrmin)/(nFamilies - 1);
    for(i = 0; i < nFamilies; ++i)
    {
        float logr = logrmin + i * dlogr;
        float r = exp(logr);
        int nSpheres = DEFAULT_N_SPHERES; // can adjust this by sphere/box size
        families[i] = family_new(head.BoxSize, r, nSpheres);
        if(families[i] == NULL)
        {
            fprintf(stderr, "ERROR: failed to allocate sphere family %d of %d at %s: %d\n",
                    i, nFamilies, __FILE__, __LINE__);
            return 1;
        }
    }
    
    
    // read the particle positions and offer them to the spheres
    fread(&block_size, sizeof(block_size), 1, file);
    for(i = 0; i < PARTICLE_INDEX; ++i)
    {
        if(head.npart[i] != 0)
            fseek(file, head.npart[i] * sizeof(pos_t), SEEK_CUR);
    }
    unread = head.npart[PARTICLE_INDEX];
    fprintf(stderr, "Particles to read: %d\n", unread);
    while(unread > 0)
    {
        int toRead = POS_BUF_SIZE < unread ? POS_BUF_SIZE : unread;
	fprintf(stderr, "Processing %d particles of %d remaining...", toRead, unread);
        nread = fread(&pos_buf, sizeof(pos_t), toRead, file);
        if(nread != toRead)
        {
            fprintf(stderr, "Reading error, read %d of %d requested with %d advertized.\nLocation: %s: %d\n", nread, toRead, unread, __FILE__, __LINE__);
            return 1;
        }
        unread -= nread;
        for(i = 0; i < nFamilies; ++i)
        {
            int j;
            family_t fam = families[i];
            for(j = 0; j < nread; ++j)
                family_offer_pos(fam, pos_buf[j]);
        }
	fprintf(stderr, "done\n");
    }
    
    fprintf(stderr, "Printing statistics\n");
    // print statistics
    for(i = 0; i < nFamilies; ++i)
    {
        float r, rho_mu, rho_mu_eps, rho_var, rho_var_eps;
        float delta_var, delta_var_eps, delta_var_err;
        family_t fam = families[i];
        
        int n = family_n_spheres(fam);
        r = family_radius(fam);
        
        rho_mu = family_num_density_mu(fam) * mass;
        rho_var = family_num_density_sig2(fam) * mass * mass;
        rho_mu_eps = sqrt( rho_var / n ) / rho_mu;
        rho_var_eps = sqrt( 2. / (n-1) );
        
        delta_var = rho_var / (rho_mu * rho_mu);
        delta_var_eps = sqrt( rho_var_eps * rho_var_eps + 4. * rho_mu_eps * rho_mu_eps );
        delta_var_err = delta_var_eps * delta_var;
        
        printf("%g\t%g\t%g\t%g\t%g\n", r, delta_var, delta_var_err, rho_mu, rho_mu * rho_mu_eps);
        
    }
    
    return 0;
}
