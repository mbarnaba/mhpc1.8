#ifndef FORCE_H
#define FORCE_H

#include "prototypes.h"
#include "utilities.h"

#ifdef _OPENMP
#include <omp.h>
#endif //_OPENMP



double pbc(double x, const double boxby2, const double twice_boxby2);

void ekin(mdsys_t *sys);

void force(mdsys_t *sys);

#endif

