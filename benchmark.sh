#! /bin/bash 

uses="COMPILER NEWTON MATH MPI"

flags=''
for use in ${uses}; do
    flags="${flags} -DUSE_${use}=OFF"
done

builddir=BASE
rm -r $builddir
mkdir $builddir
cd $builddir
cmake ${flags} ..
make
ctest
(time ./ljmd.x < argon_108.inp ; ) 2> argon_108.time
(time ./ljmd.x < argon_2916.inp ; ) 2> argon_2916.time
cd ..

for use in ${uses}; do
    flags="${flags} -DUSE_${use}=ON"
    echo $flags
    builddir="${use}"
    rm -r $builddir
    mkdir $builddir
    cd $builddir
    cmake ${flags} ..
    make
    ctest
    (time ./ljmd.x < argon_108.inp ; ) 2> argon_108.time
    (time ./ljmd.x < argon_2916.inp ; ) 2> argon_2916.time
    cd ..
done
