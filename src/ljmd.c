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

#include "prototypes.h"
#include "input.h"
#include "utilities.h"
#include "force.h"
#include "output.h"
#include "verlet.h"

#ifdef _MPI 
#include "mympi.h"
#include <mpi.h>
#endif 


static double wallclock(void) {
    struct timeval t;
    gettimeofday( &t, 0 );
    const double seconds = ( (double) t.tv_sec) + 1.0e-6 * ( (double) t.tv_usec );
    return seconds; 
}


/* a few physical constants */
const double kboltz=0.0019872067;     /* boltzman constant in kcal/mol/K */
const double mvsq2e=2390.05736153349; /* m*v^2 in kcal/mol */


/* main */
#ifndef MAKING_LIB
int main(int argc, char **argv) {
    int nprint;
    char restfile[BLEN], trajfile[BLEN], ergfile[BLEN], line[BLEN];
    FILE *traj = NULL;
    FILE *erg = NULL;
    mdsys_t sys;
    int mpirank = 0; // this is true even without MPI 
    
    double seconds = 0; 

    read_input( &sys, &nprint, restfile, trajfile, ergfile, line );

    #ifdef _MPI 
    mpi_initialize( &sys ); 
    mpirank = sys.mpirank; 
    #endif 

    /* initialize forces and energies.*/
    sys.nfi = 0;
    seconds -= wallclock(); 
    force( &sys );
    ekin( &sys );
    seconds += wallclock(); 

    if (mpirank == 0) {
        erg = fopen(ergfile,"w");
        traj = fopen(trajfile,"w");

        printf("Starting simulation with %d atoms for %d steps.\n",sys.natoms, sys.nsteps);
        printf("     NFI            TEMP            EKIN                 EPOT              ETOT\n");
        output( &sys, erg, traj );
    }
    
    /**************************************************/
    /* main MD loop */
    for(sys.nfi=1; sys.nfi <= sys.nsteps; ++sys.nfi) {

        /* write output, if requested. Only the master rank will write output */
        if ((sys.nfi % nprint) == 0 && mpirank == 0) {
            output( &sys, erg, traj );
        }

        /* propagate system and recompute energies */
        seconds -= wallclock(); 
        velverlet( &sys );
        ekin( &sys );
        seconds += wallclock(); 
    }
    /**************************************************/

    /* clean up: close files. Only the master rank */
    if (mpirank == 0) {
        printf("Simulation Done.\n");
        fclose(erg);
        fclose(traj);
    }

    /* free memory */
    free(sys.rx);
    free(sys.ry);
    free(sys.rz);
    free(sys.vx);
    free(sys.vy);
    free(sys.vz);
    free(sys.fx);
    free(sys.fy);
    free(sys.fz);
    
    #ifdef _MPI 
    mpi_finalize( &sys ); 
    #endif 

    printf( "Execution time (seconds): %lf\n", seconds ); 
    return 0;
}
#endif // MAKING_LIB
