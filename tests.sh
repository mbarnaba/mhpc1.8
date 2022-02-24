#! /bin/bash 


__clean__() {
    rm argon_108.*
    rm run.out run.err 
}

# does the executable run?
test_run() {        
    cp ${CMAKE_SOURCE_DIR}/examples/argon_108.inp .
    cp ${CMAKE_SOURCE_DIR}/examples/argon_108.rest .    
    
    ./ljmd.x < argon_108.inp 1>run.out 2>run.err 
    retval=$?
    
    # if not argument is given the files are cleaned 
    [[ -z "$1" ]] &&  __clean__

    return $retval
}


# does the executable create argon_108.dat and argon_108.xyz?
test_writes() {
    # I pass an argument so test_run so it will not clean up the output files
    test_run 'dummy_argument'
 
    retval=1
    [[ -f 'argon_108.dat' ]] && [[ -f argon_108.xyz ]] && retval=0
    
    # we have to clean here 
    __clean__

    return $retval 
}

# does the executable produce a 'good' output 
test_output() {
    # I pass an argument so test_run so it will not clean up the output files
    test_run 'dummy_argument'
 
    retval=1
    tail -n 1 'run.out' |grep 'Simulation Done. Run time:' && retval=0
    
    # we have to clean here 
    __clean__

    return $retval 
} 



# these two variables are expected as arguments 
CMAKE_SOURCE_DIR="$1"
testname="$2"


# the corresponding test is executed and its retval returned to ctest
test_${testname}
exit "$?"
