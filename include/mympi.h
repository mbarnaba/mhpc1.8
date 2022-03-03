#ifndef MPI_HEADER_GUARD
#define MPI_HEADER_GUARD

#ifdef USE_MPI
#include <mpi.h>
#endif

#include "prototypes.h"


void mpi_initialize(mdsys_t* sys); 
void mpi_finalize(mdsys_t* sys); 


#endif // MPI_HEADER_GUARD
