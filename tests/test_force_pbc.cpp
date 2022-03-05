// unit test example with test fixture
#include "gtest/gtest.h"
#include <math.h>
extern "C" {
#include "force.h"
}


class TestForce_PBC: public ::testing::Test {
    protected:
    mdsys_t* sys;

    void SetUp() {
        sys = new mdsys_t;
        sys->natoms = 2;
        sys->mass = 1.0;
	sys->mass = 1.0;
	sys->rcut = 10.;
	sys->box = 10.;
	sys->epsilon = 1.;
	sys->sigma = 1.;
        sys->rx = new double[2];
	sys->ry = new double[2];
	sys->rz = new double[2];
        sys->fx = new double[2];
	sys->fy = new double[2];
	sys->fz = new double[2];
        sys->rx[0] = -1.0;
        sys->rx[1] = 101.0;
	sys->ry[0] = 0.0;
        sys->ry[1] = 0.0;
	sys->rz[0] = 0.0;
        sys->rz[1] = 0.0;
        sys->fx[0] = 0.0;
        sys->fx[1] = 0.0;
	sys->fy[0] = 0.0;
        sys->fy[1] = 0.0;
	sys->fz[0] = 0.0;
        sys->fz[1] = 0.0;
    }

    void TearDown() {
        delete[] sys->rx;
	delete[] sys->ry;
	delete[] sys->rz;
        delete[] sys->fx;
	delete[] sys->fy;
	delete[] sys->fz;

        delete sys;
    }
};

TEST_F(TestForce_PBC, force_pbc) {
  // Here we test the force between particles in two different boxes,
  // i.e. considering periodic boundary conditions
  double rx, ry, rz, r;
  double ffac;
  int i,j;
  double * fx = new double[2];
  double * fy = new double[2];
  double * fz = new double[2];
  fx[0] = 0;
  fx[1] = 0;
  fy[0] = 0;
  fy[1] = 0;
  fz[0] = 0;
  fz[1] = 0;
  for(i=0; i < (sys->natoms); ++i) {
    for(j=0; j < (sys->natoms); ++j) {
      if (i==j) continue;
      if(sys->rx[i] > sys->rx[j])
	rx = 2;
      if(sys->rx[i] < sys->rx[j])
	rx = -2;
      // Particles are located ad -1 and 101 but box is 10
      // so the distance between the images taking into account pbc is 2
      ry = sys->ry[i] - sys->ry[j];
      rz = sys->rz[i] - sys->rz[j];
      r = sqrt(rx*rx + ry*ry + rz*rz);
      if (r < sys->rcut) {
	ffac = -4.0*sys->epsilon*(-12.0*pow(sys->sigma/r,12.0)/r+6*pow(sys->sigma/r,6.0)/r);
	fx[i] += rx/r*ffac;
	fy[i] += ry/r*ffac;
	fz[i] += rz/r*ffac;
      }
    }
  }
  force(sys);
  ASSERT_DOUBLE_EQ( sys->fx[0], fx[0] );
  ASSERT_DOUBLE_EQ( sys->fx[1], fx[1] );
  ASSERT_DOUBLE_EQ( sys->fy[0], fy[0] );
  ASSERT_DOUBLE_EQ( sys->fy[1], fy[1] );
  ASSERT_DOUBLE_EQ( sys->fz[0], fz[0] );
  ASSERT_DOUBLE_EQ( sys->fz[1], fz[1] );
  delete[] fx;
  delete[] fy;
  delete[] fz;
}

