#include "force.h"
#include <math.h>
#include <ctype.h>

#ifdef _OPENMP
#include <omp.h>
#endif //_OPENMP

/* helper function: apply minimum image convention */
double pbc(double x, const double boxby2,const double twice_boxby2)
{
    while (x >  boxby2) x -= twice_boxby2;
    while (x < -boxby2) x += twice_boxby2;
    return x;
}


/* compute kinetic energy */
void ekin(mdsys_t *sys)
{   
    int i;

    sys->ekin=0.0;
    for (i=0; i<sys->natoms; ++i) {
        sys->ekin += 0.5*mvsq2e*sys->mass*(sys->vx[i]*sys->vx[i] + sys->vy[i]*sys->vy[i] + sys->vz[i]*sys->vz[i]);
    }
    sys->temp = 2.0*sys->ekin/(3.0*sys->natoms-3.0)/kboltz;
}


#ifndef _MPI 
/* force using Newton's third law + avoid costly math operations + reorganization variables */
#if defined (_MATH)
void force(mdsys_t *sys)
{
    double ffac;
    double rx,ry,rz;
    int i,j;
    const double half_box = 0.5*sys->box;
    const double twice_boxby2 = sys->box;
    /* zero energy and forces */
    sys->epot=0.0;
    azzero(sys->fx,sys->natoms);
    azzero(sys->fy,sys->natoms);
    azzero(sys->fz,sys->natoms);
    //Constants added to avoid computing pow,sqrt etc.
    const double sigma  = sys->sigma ;
    const double sigma6 = sigma * sigma * sigma * sigma * sigma * sigma ;
    const double c6  = 4.0 * sys->epsilon * sigma6 ;
    const double c12 = c6 * sigma6 ;
    const double  rcsq = sys->rcut * sys->rcut;
    double rsq, rinv, r6;
    double rxffac, ryffac, rzffac;
#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic) private(i, j, rsq, rx, ry, rz, ffac) reduction(+ : epot)
#endif //_OPENMP
    for(i=0; i < (sys->natoms - 1); ++i) {
        for(j=i+1; j < (sys->natoms); ++j) {
            /* particles have no interactions with themselves */
            //if (i==j) continue; //With 3rd law we can eliminate this
            /* get distance between particle i and j */
            rx=pbc(sys->rx[i] - sys->rx[j], half_box,twice_boxby2);
            ry=pbc(sys->ry[i] - sys->ry[j], half_box,twice_boxby2);
            rz=pbc(sys->rz[i] - sys->rz[j], half_box,twice_boxby2);
            rsq = rx*rx + ry*ry + rz*rz;
            if (rsq < rcsq){
                rinv=1.0/rsq;
                r6 = rinv*rinv*rinv;
                ffac = (12.0*c12*r6 - 6.0*c6)*r6*rinv;
                sys->epot  += r6*(c12*r6 - c6);
                rxffac = rx*ffac;
                sys->fx[i] += rxffac;
                sys->fx[j] -= rxffac;
                ryffac = ry*ffac;
                sys->fy[i] += ryffac;
                sys->fy[j] -= ryffac;
                rzffac = rz*ffac;
                sys->fz[i] += rzffac;
                sys->fz[j] -= rzffac;
            }
        }
    }
}
#elif defined(_NEWTON)
/* force using Newton's third law */
void force(mdsys_t *sys)
{
    double r,ffac;
    double rx,ry,rz;
    int i,j;
    const double half_box = 0.5*sys->box;
    const double twice_boxby2 = sys->box;
    /* zero energy and forces */
    sys->epot=0.0;
    azzero(sys->fx,sys->natoms);
    azzero(sys->fy,sys->natoms);
    azzero(sys->fz,sys->natoms);
    //Constants added to avoid computing pow,sqrt etc.
    for(i=0; i < (sys->natoms - 1); ++i) {
        for(j=i+1; j < (sys->natoms); ++j) {
            /* particles have no interactions with themselves */
            //if (i==j) continue; //With 3rd law we can eliminate this
            /* get distance between particle i and j */
            rx=pbc(sys->rx[i] - sys->rx[j], half_box,twice_boxby2);
            ry=pbc(sys->ry[i] - sys->ry[j], half_box,twice_boxby2);
            rz=pbc(sys->rz[i] - sys->rz[j], half_box,twice_boxby2);
            r = sqrt(rx*rx + ry*ry + rz*rz);
            /* compute force and energy if within cutoff */
            if (r < sys->rcut) {
                ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r,12.0)/r+6*pow(sys->sigma/r,6.0)/r);
                sys->epot += 4.0*sys->epsilon*(pow(sys->sigma/r,12.0)-pow(sys->sigma/r,6.0));
                sys->fx[i] += rx/r*ffac;
                sys->fx[j] -= rx/r*ffac;
                sys->fy[i] += ry/r*ffac;
                sys->fy[j] -= ry/r*ffac;
                sys->fz[i] += rz/r*ffac;
                sys->fz[j] -= rz/r*ffac;
            }
        }
    }
}
#else
/* force baseline */
void force(mdsys_t *sys)
{
    double r,ffac;
    double rx,ry,rz;
    int i,j;
    const double half_box = 0.5*sys->box;
    const double twice_boxby2 = sys->box;
    /* zero energy and forces */
    sys->epot=0.0;
    azzero(sys->fx,sys->natoms);
    azzero(sys->fy,sys->natoms);
    azzero(sys->fz,sys->natoms);
    for(i=0; i < (sys->natoms); ++i) {
        for(j=0; j < (sys->natoms); ++j) {
            /* particles have no interactions with themselves */
            if (i==j) continue;
            /* get distance between particle i and j */
            rx=pbc(sys->rx[i] - sys->rx[j], half_box,twice_boxby2);
            ry=pbc(sys->ry[i] - sys->ry[j], half_box,twice_boxby2);
            rz=pbc(sys->rz[i] - sys->rz[j], half_box,twice_boxby2);
            r = sqrt(rx*rx + ry*ry + rz*rz);
            /* compute force and energy if within cutoff */
            if (r < sys->rcut) {
                ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r,12.0)/r+6*pow(sys->sigma/r,6.0)/r);
                sys->epot += 0.5*4.0*sys->epsilon*(pow(sys->sigma/r,12.0)-pow(sys->sigma/r,6.0));
                sys->fx[i] += rx/r*ffac;
                sys->fy[i] += ry/r*ffac;
                sys->fz[i] += rz/r*ffac;
            }
        }
    }
}
#endif
#else // _MPI 

/* compute forces MPI version */
void force(mdsys_t *sys) {
    double ffac;
    double rx,ry,rz;
    int i,j;
    int ii; 
    const double half_box = 0.5*sys->box;
    const double twice_boxby2 = sys->box;

    /* zero energy and forces */
    double rank_epot = 0.0; // each rank will add to its local variable
    azzero( sys->rank_fx, sys->natoms );
    azzero( sys->rank_fy, sys->natoms );
    azzero( sys->rank_fz, sys->natoms );
    
    MPI_Bcast( sys->rx, sys->natoms, MPI_DOUBLE, 0, sys->mpicomm );
    MPI_Bcast( sys->ry, sys->natoms, MPI_DOUBLE, 0, sys->mpicomm );
    MPI_Bcast( sys->rz, sys->natoms, MPI_DOUBLE, 0, sys->mpicomm );
	
	//Constants added to avoid computing pow,sqrt etc.
	
    const double sigma  = sys->sigma ;
    const double sigma6 = sigma * sigma * sigma * sigma * sigma * sigma ;
    const double c6  = 4.0 * sys->epsilon * sigma6 ;
    const double c12 = c6 * sigma6 ;
	const double rcsq = sys->rcut * sys->rcut;
	double rsq, rinv, r6;
    double rxffac, ryffac, rzffac;
    for(i=0; i < (sys->natoms - 1); i += sys->mpiranks) {
        ii = i + sys->mpirank; 
        if (ii >= (sys->natoms - 1)) break;

        for(j=i + 1; j < (sys->natoms); ++j) {

            /* particles have no interactions with themselves */
            //if (i==j) continue; //With 3rd law we can eliminate this
            
            /* get distance between particle i and j */
            rx=pbc(sys->rx[i] - sys->rx[j], half_box,twice_boxby2);
            ry=pbc(sys->ry[i] - sys->ry[j], half_box,twice_boxby2);
            rz=pbc(sys->rz[i] - sys->rz[j], half_box,twice_boxby2);
            
			rsq = rx*rx + ry*ry + rz*rz;
			if (rsq < rcsq){
				
				rinv=1.0/rsq;
				r6 = rinv*rinv*rinv;
				
				ffac = (12.0*c12*r6 - 6.0*c6)*r6*rinv;

				rank_epot += r6*(c12*r6 - c6);
				
                rxffac = rx*ffac;
                sys->rank_fx[i] += rxffac;
                sys->rank_fx[j] -= rxffac;
                
                ryffac = ry*ffac;
                sys->rank_fy[i] += ryffac;
                sys->rank_fy[j] -= ryffac;
                
                rzffac = rz*ffac;
                sys->rank_fz[i] += rzffac;
                sys->rank_fz[j] -= rzffac;
            }
        }
    }

    MPI_Reduce( sys->rank_fx, sys->fx, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm );
    MPI_Reduce( sys->rank_fy, sys->fy, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm );
    MPI_Reduce( sys->rank_fz, sys->fz, sys->natoms, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm );
    MPI_Reduce( &rank_epot, &sys->epot, 1, MPI_DOUBLE, MPI_SUM, 0, sys->mpicomm );
}
#endif // _MPI
