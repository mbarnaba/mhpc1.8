
## to compile and run the code
cmake is required! 
```
./compile.sh [build_dir]
```
where build_dir is optional (if not given it will default to './build') and can be 
any directory. 


## testing 

Testing is enabled by default and you run the tests simply by running 'ctest' from inside the build directory (build_dir). 

The tested are collected (as functions) inside 'tests.sh'. 
You can add simply by adding a test in the 'CMakeLists.txt' and creating a corresponding function in 'tests.sh'. 

For example this is the line to add a test in CMakeLists.txt 

```
add_test(
    NAME run
    COMMAND ${CMAKE_SOURCE_DIR}/tests.sh ${CMAKE_SOURCE_DIR} run
)

```

and this is the implementation of the corresponding function in tests.sh.


```
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

```
