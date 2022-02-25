#include "force.h"

#include <math.h>
#include <ctype.h>

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

/* compute forces */
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
	
	double c12 = 4.0*sys->epsilon*pow(sys->sigma,12.0);
	double c6 = 4.0*sys->epsilon*pow(sys->sigma,6.0);
	const double rcsq = sys->rcut * sys->rcut;
	double rsq,rinv,r6;
	
    for(i=0; i < (sys->natoms)-1; ++i) {
        for(j=i+1; j < (sys->natoms); ++j) {

            /* particles have no interactions with themselves */
            //if (i==j) continue; //With 3rd law we can eliminate this
            
            /* get distance between particle i and j */
            rx=pbc(sys->rx[i] - sys->rx[j], half_box,twice_boxby2);
            ry=pbc(sys->ry[i] - sys->ry[j], half_box,twice_boxby2);
            rz=pbc(sys->rz[i] - sys->rz[j], half_box,twice_boxby2);
            //r = sqrt(rx*rx + ry*ry + rz*rz);
      
            /* compute force and energy if within cutoff */
            /**if (r < sys->rcut) {
                ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r,12.0)/r
                                         +6*pow(sys->sigma/r,6.0)/r);
                
                //We use Netwon third law: the following is used twice
                //So it's not multiplied by 0.5 anymore
                sys->epot += 4.0*sys->epsilon*(pow(sys->sigma/r,12.0)
                                               -pow(sys->sigma/r,6.0));
			**/
			rsq = rx*rx + ry*ry + rz*rz;
			if (rsq < rcsq){
				
				rinv=1.0/rsq;
				r6 = rinv*rinv*rinv;
				
				ffac = (12.0*c12*r6 - 6.0*c6)*r6*rinv;
				sys->epot += r6*(c12*r6 - c6);
				
                sys->fx[i] += rx*ffac;
                sys->fx[j] -= rx*ffac;
                
                sys->fy[i] += ry*ffac;
                sys->fy[j] -= ry*ffac;
                
                sys->fz[i] += rz*ffac;
                sys->fz[j] -= rz*ffac;
            }
        }
    }
}
