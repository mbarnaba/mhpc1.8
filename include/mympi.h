#ifndef MPI_HEADER_GUARD
#define MPI_HEADER_GUARD

#include <mpi.h>

#include "prototypes.h"


void mpi_initialize(mdsys_t* sys); 
void mpi_finalize(mdsys_t* sys); 


#endif // MPI_HEADER_GUARD
