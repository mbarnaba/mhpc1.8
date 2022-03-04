
#include "prototypes.h"
#include "utilities.h"

#ifdef _MPI
#include <mpi.h>
#include "mympi.h"
#endif 

void mpi_initialize(mdsys_t* sys) {

#ifdef _MPI

    MPI_Init( NULL, NULL ); 

    sys->mpicomm = MPI_COMM_WORLD; 

    MPI_Comm_size( sys->mpicomm, &sys->mpiranks );
    MPI_Comm_rank( sys->mpicomm, &sys->mpirank );


    sys->rank_fx = (double*) malloc( sys->natoms * sizeof(double) );
    sys->rank_fy = (double*) malloc( sys->natoms * sizeof(double) );
    sys->rank_fz = (double*) malloc( sys->natoms * sizeof(double) );

    #ifndef NDEBUG
    printf( "MPI successfuly initialized with %d ranks\n", sys->mpiranks ); 
    #endif
#endif 
}

void mpi_finalize(mdsys_t* sys) {
    #ifdef _MPI
    free( sys->rank_fx ); 
    free( sys->rank_fy ); 
    free( sys->rank_fz ); 

    MPI_Finalize();
    #ifndef NDEBUG
    printf( "MPI successfuly finalized with %d ranks\n", sys->mpiranks ); 
    #endif
#endif //_MPI 
}
