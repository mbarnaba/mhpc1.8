#include "input.h"

/* generic file- or pathname buffer length */
#define BLEN 200

/* helper function: read a line and then return
   the first string with whitespace stripped off */
int get_a_line(FILE *fp, char *buf)	
{
    char tmp[BLEN], *ptr;

    /* read a line and cut of comments and blanks */
    if (fgets(tmp,BLEN,fp)) {
        int i;

        ptr=strchr(tmp,'#');
        if (ptr) *ptr= '\0';
        i=strlen(tmp); --i;
        while(isspace(tmp[i])) {
            tmp[i]='\0';
            --i;
        }
        ptr=tmp;
        while(isspace(*ptr)) {++ptr;}
        i=strlen(ptr);
        strcpy(buf,tmp);
        return 0;
    } else {
        perror("problem reading input");
        return -1;
    }
    return 0;
}


int read_input(mdsys_t* sys, int * nprint, char * restfile, char * trajfile, char * ergfile, char * line)
{
	FILE *fp;
	int i;
    /* read input file */
    if(get_a_line(stdin,line)) return 1;
    sys->natoms=atoi(line);
    if(get_a_line(stdin,line)) return 1;
    sys->mass=atof(line);
    if(get_a_line(stdin,line)) return 1;
    sys->epsilon=atof(line);
    if(get_a_line(stdin,line)) return 1;
    sys->sigma=atof(line);
    if(get_a_line(stdin,line)) return 1;
    sys->rcut=atof(line);
    if(get_a_line(stdin,line)) return 1;
    sys->box=atof(line);
    if(get_a_line(stdin,restfile)) return 1;
    if(get_a_line(stdin,trajfile)) return 1;
    if(get_a_line(stdin,ergfile)) return 1;
    if(get_a_line(stdin,line)) return 1;
    sys->nsteps=atoi(line);
    if(get_a_line(stdin,line)) return 1;
    sys->dt=atof(line);
    if(get_a_line(stdin,line)) return 1;
    *nprint=atoi(line);
    
    /* allocate memory */
    sys->rx=(double *)malloc(sys->natoms*sizeof(double));
    sys->ry=(double *)malloc(sys->natoms*sizeof(double));
    sys->rz=(double *)malloc(sys->natoms*sizeof(double));
    sys->vx=(double *)malloc(sys->natoms*sizeof(double));
    sys->vy=(double *)malloc(sys->natoms*sizeof(double));
    sys->vz=(double *)malloc(sys->natoms*sizeof(double));
    sys->fx=(double *)malloc(sys->natoms*sizeof(double));
    sys->fy=(double *)malloc(sys->natoms*sizeof(double));
    sys->fz=(double *)malloc(sys->natoms*sizeof(double));

    fp=fopen(restfile,"r");
    /* read restart */
    if(fp) {
        for (i=0; i<sys->natoms; ++i) {
            fscanf(fp,"%lf%lf%lf",sys->rx+i, sys->ry+i, sys->rz+i);
        }
        for (i=0; i<sys->natoms; ++i) {
            fscanf(fp,"%lf%lf%lf",sys->vx+i, sys->vy+i, sys->vz+i);
        }
        fclose(fp);
        azzero(sys->fx, sys->natoms);
        azzero(sys->fy, sys->natoms);
        azzero(sys->fz, sys->natoms);
    } else {
        perror("cannot read restart file");
        return 3;
    }

    return 0;
}
