#! /bin/bash 


uses="COMPILER NEWTON MATH"

flags=''
for use in ${uses}; do
    flags="${flags} -DUSE_${use}=OFF"
done


for_use() {
    local use="$1"
    
    local builddir=$use
    rm -r $builddir
    mkdir $builddir
    cd $builddir
    cmake ${flags} ..
    make
    ctest
    
    for inp in argon_108 argon_2916; do
        for i in $( seq 5 ); do
            ./ljmd.x < "${inp}.inp" |grep "Execution time" |cut -d':' -f2
        done > "${inp}.seconds"
    done

    cd ..
}


for_use 'BASE'

for use in ${uses}; do
    flags="${flags} -DUSE_${use}=ON"
    echo $flags

    for_use "${use}" 
done
