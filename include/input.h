#ifndef INPUT_H
#define INPUT_H


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#ifdef _MPI
#include <mpi.h>
#endif //_MPI


#include "prototypes.h"
#include "utilities.h"

int get_a_line(FILE *fp, char *buf);
int read_input(mdsys_t* sys, int * nprint, char * restfile, char * trajfile, char * ergfile, char * line);

#ifdef _MPI
void mpi_finalize(mdsys_t* sys); 
#endif

#endif
