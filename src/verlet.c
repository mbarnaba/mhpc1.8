#include "verlet.h"
#include "force.h"
/* velocity verlet */
void velverlet(mdsys_t *sys)
{
    int i;

    /* first part: propagate velocities by half and positions by full step */
 //   #ifdef _OPENMP
  //  #pragma omp parallel for default(shared) private(i) schedule(auto)
  //  #endif
    for (i=0; i<sys->natoms; ++i) {
        sys->vx[i] += 0.5*sys->dt / mvsq2e * sys->fx[i] / sys->mass;
        sys->vy[i] += 0.5*sys->dt / mvsq2e * sys->fy[i] / sys->mass;
        sys->vz[i] += 0.5*sys->dt / mvsq2e * sys->fz[i] / sys->mass;
       // #ifdef _OPENMP
    	//#pragma omp critical
    	//{
    	//#endif    
        sys->rx[i] += sys->dt*sys->vx[i];
        sys->ry[i] += sys->dt*sys->vy[i];
        sys->rz[i] += sys->dt*sys->vz[i];
    }
    //}

    /* compute forces and potential energy */
    force(sys);
	
	/* Calls the second step from verlet2 */
	#ifdef _OPENMP
    	#pragma omp parallel for default(shared) private(i) schedule(auto)
    	#endif
	for (i=0; i<sys->natoms; ++i) {
        sys->vx[i] += 0.5*sys->dt / mvsq2e * sys->fx[i] / sys->mass;
        sys->vy[i] += 0.5*sys->dt / mvsq2e * sys->fy[i] / sys->mass;
        sys->vz[i] += 0.5*sys->dt / mvsq2e * sys->fz[i] / sys->mass;
    }
}
