/*
 * simple lennard-jones potential MD code with velocity verlet.
 * units: Length=Angstrom, Mass=amu; Energy=kcal
 *
 * baseline c version.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

/////////////////////////////////////////////////////////////////////////
#if defined(_OPENMP)
#include <omp.h>
#endif
/////////////////////////////////////////////////////////////////////////



/* generic file- or pathname buffer length */
#define BLEN 200

/* a few physical constants */
const double kboltz=0.0019872067;     /* boltzman constant in kcal/mol/K */
const double mvsq2e=2390.05736153349; /* m*v^2 in kcal/mol */


/* compute forces */
static void force(mdsys_t *sys)
{
    double r,ffac;
    double rx,ry,rz;
    double c6, c12, rcsq;
    double sigma, sigma6;
    int i,j;

    /* zero energy and forces */
    sys->epot=0.0;
    azzero(sys->fx,sys->natoms);
    azzero(sys->fy,sys->natoms);
    azzero(sys->fz,sys->natoms);

    sigma = sys->sigma ;
    sigma6  = sigma * sigma * sigma * sigma * sigma * sigma;
    c6 =4.0 * sys->epsilon * sigma6;
    c12=4.0 * sys->epsilon * sigma6*sigma6;
    rcsq = sys->rcut * sys->rcut;
    
    
//////////////////////////////////////////////////////////////////////////////////////
//  We will always foucus on this part for mpi and openmp calculation
//#if defined(_OPENMP)
//#pragma omp unroll partial(4)
# pragma omp parallel for default(shared) private(i, j, rx, ry, rz, r) //reduction(+:epot)
//#endif
//////////////////////////////////////////////////////////////////////////////////////  
    for(i=0; i < (sys->natoms); ++i) {
        for(j=0; j < (sys->natoms); ++j) {

            /* particles have no interactions with themselves */
            if (i==j) continue;

            /* get distance between particle i and j */
            rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
            ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
            rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
            r = sqrt(rx*rx + ry*ry + rz*rz);
            double rsq;
            rsq = rx*rx + ry*ry + rz*rz;
            /* compute force and energy if within cutoff */
            if (r < sys->rcut) {
            // if (rsq < rcsq) {
                // double r6, rinv; rinv = 1.0/rsq; r6=rinv*rinv*rinv;
                // ffac = ( 12.0 * c12 * r6 - 6.0 * c6 ) * r6 * rinv ;
                // sys->epot += 0.5 * r6 * ( c12 * r6 - c6 );
                // sys->fx[i] += rx*ffac;
                // sys->fy[i] += ry*ffac;
                // sys->fz[i] += rz*ffac;
                ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r,12.0)/r
                                         +6*pow(sys->sigma/r,6.0)/r);

                sys->epot += 0.5*4.0*sys->epsilon*(pow(sys->sigma/r,12.0)
                                               -pow(sys->sigma/r,6.0));

                sys->fx[i] += rx/r*ffac;
                sys->fy[i] += ry/r*ffac;
                sys->fz[i] += rz/r*ffac;
            }
        }
    }
}



/* compute forces */
static void force_3law(mdsys_t *sys)
{
    double ffac;
    double rx,ry,rz;
    double c6, c12, rcsq;
    double sigma, sigma6;
    int i,j;

    /* zero energy and forces */
    sys->epot=0.0;
    azzero(sys->fx,sys->natoms);
    azzero(sys->fy,sys->natoms);
    azzero(sys->fz,sys->natoms);
    sigma = sys->sigma ;
    sigma6  = sigma * sigma * sigma * sigma * sigma * sigma;
    c6 =4.0 * sys->epsilon * sigma6;
    c12=4.0 * sys->epsilon * sigma6*sigma6;
    rcsq = sys->rcut * sys->rcut;
    
    //#if defined(_OPENMP)
    //#pragma omp unroll partial(4)
   // # pragma omp parallel for default(shared) private(i, j, rx, ry, rz, r, ffac,fx, fy, fz,ffac) reduction(+:epot)
    //#endif
    for(i=0; i < (sys->natoms)-1; ++i) {
        for(j=i+1; j < (sys->natoms); ++j) {
            /* get distance between particle i and j */
            rx=pbc(sys->rx[i] - sys->rx[j], 0.5*sys->box);
            ry=pbc(sys->ry[i] - sys->ry[j], 0.5*sys->box);
            rz=pbc(sys->rz[i] - sys->rz[j], 0.5*sys->box);
            double rsq;
            rsq = rx*rx + ry*ry + rz*rz;
            /* compute force and energy if within cutoff */
             if (rsq < rcsq) {
                double r6, rinv;
                rinv = 1.0/rsq; // 1/(r^2)
                r6=rinv*rinv*rinv; // 1/(r^6)
                ffac = ( 12.0 * c12 * r6 - 6.0 * c6 ) * r6 * rinv ;
                sys->epot += r6 * ( c12 * r6 - c6 );
                sys->fx[i] += rx*ffac;
                sys->fy[i] += ry*ffac;
                sys->fz[i] += rz*ffac;
                sys->fx[j] -= rx*ffac;
                sys->fy[j] -= ry*ffac;
                sys->fz[j] -= rz*ffac;
            }
        }
    }
}


}
