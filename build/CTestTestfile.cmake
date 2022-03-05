# CMake generated Testfile for 
# Source directory: /mnt/120CT/github1/mhpc1.8
# Build directory: /mnt/120CT/github1/mhpc1.8/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(RunTest "/bin/sh" "-c" "/mnt/120CT/github1/mhpc1.8/build/ljmd.x < argon_108.inp && test -f argon_108.dat && test -f argon_108.xyz")
set_tests_properties(RunTest PROPERTIES  PASS_REGULAR_EXPRESSION ".*Simulation Done.*" _BACKTRACE_TRIPLES "/mnt/120CT/github1/mhpc1.8/CMakeLists.txt;130;add_test;/mnt/120CT/github1/mhpc1.8/CMakeLists.txt;0;")
add_test(Kinetic "/mnt/120CT/github1/mhpc1.8/build/test_kinetic")
set_tests_properties(Kinetic PROPERTIES  _BACKTRACE_TRIPLES "/mnt/120CT/github1/mhpc1.8/CMakeLists.txt;154;add_test;/mnt/120CT/github1/mhpc1.8/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
