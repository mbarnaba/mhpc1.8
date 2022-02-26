#ifndef VERLET_H
#define VERLET_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#include "prototypes.h"
#include "force.h"


void velverlet(mdsys_t *sys);

#endif
