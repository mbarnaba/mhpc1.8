// unit test example with test fixture
#include "gtest/gtest.h"

extern "C" {
#include "force.h"
}


class TestPbc: public ::testing::Test {
    protected:
    mdsys_t* sys;

    void SetUp() {
        sys = new mdsys_t;
	sys->box = 10;
        sys->rx = new double[2];
        sys->rx[0] = -11.0;
        sys->rx[1] = 1.0;
    }

    void TearDown() {
        delete[] sys->rx;
        delete sys;
    }
};

TEST_F(TestPbc, pbc) {
  // Particles are located at x = 1 and x = -11 but box = 10
  // We thus expect that particles are effectively separated by delta x = 2
  // We test the module for simplicity and so we expect 2*2=4
  const double expected = 4;
  const double half_box = 0.5*sys->box;
  const double twice_boxby2 = sys->box;
  const double rx=pbc(sys->rx[0] - sys->rx[1], half_box, twice_boxby2);
  ASSERT_DOUBLE_EQ( rx*rx, expected ); 
}



