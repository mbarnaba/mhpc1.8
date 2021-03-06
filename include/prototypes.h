#ifndef PROT_H
#define PROT_H

#include <stdlib.h>
#include <stdio.h>

#ifdef MAKING_LIB 
#undef _MPI
#endif

#ifdef _MPI
#include <mpi.h>
#endif  

#ifdef _OPENMP
#include <omp.h>
#endif //_OPENMP

/* generic file- or pathname buffer length */
#define BLEN 200
/* a few physical constants */
extern const double kboltz;     /* boltzman constant in kcal/mol/K */
extern const double mvsq2e; /* m*v^2 in kcal/mol */
/* structure to hold the complete information
 * about the MD system */
struct _mdsys {
    int natoms,nfi,nsteps;
    double dt, mass, epsilon, sigma, box, rcut;
    double ekin, epot, temp;
    double *rx, *ry, *rz;
    double *vx, *vy, *vz;
    double *fx, *fy, *fz;

#ifdef _MPI
    MPI_Comm mpicomm; 
    int mpiranks; 
    int mpirank; 
    double *rank_fx, *rank_fy, *rank_fz; 
#endif 
};
typedef struct _mdsys mdsys_t;

#endif
