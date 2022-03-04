// unit test example with test fixture
#include "gtest/gtest.h"

extern "C" {
#include "force.h"
}


class TestKinetic: public ::testing::Test {
    protected:
    mdsys_t* sys;

    void SetUp() {
        sys = new mdsys_t;
        sys->natoms = 2;
        sys->mass = 1.0;
        sys->rx = new double[2];
        sys->vx = new double[2];
	sys->vy = new double[2];
	sys->vz = new double[2];
        sys->fx = new double[2];
        sys->rx[0] = -1.0;
        sys->rx[1] = 1.0;
        sys->vx[0] = 1.0;
        sys->vx[1] = 0.0;
	sys->vy[0] = 0.0;
        sys->vy[1] = 0.0;
	sys->vz[0] = 0.0;
        sys->vz[1] = 0.0;
        sys->fx[0] = 1.0;
        sys->fx[1] = 0.2;
    }

    void TearDown() {
        delete[] sys->rx;
        delete[] sys->vx;
        delete[] sys->fx;

        delete sys;
    }
};

TEST_F(TestKinetic, kinetic) {
    const double expected{ 0.5 * sys->mass * sys->vx[ 0 ] * sys->vx[ 0 ] * mvsq2e };
    ekin( sys );  
    ASSERT_DOUBLE_EQ( sys->ekin, expected ); 
}
