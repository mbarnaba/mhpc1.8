### !!! we need pro version for branch protection
### !!! so always work with your branch and merge it to the main branch

* clone the repo from matteo website
* create your branch with branch_name
* switch to that branch and set upstream


### Daily routine followed by each member
```
$ git pull                  // main branch
$ git checkout user_name
$ git merge                 //from user branch

the user branch will sync with the main branch
work from your branch after that 

$ git add .                 //add the files
$ git commit -m "comment"
$ git push                  //if u have set the $ git push --set-upstream origin branch_name
$ git checkout main 
$ git merge user_branch
$ git push
```

### User Notes
# HASAN (hasanictp)
```
dumped initial file as per assign4 pdf
added git ignore
```
### User Notes
# MARCO (marco-celoria)
```
abcdef
adasdasd
adadas
```

### User Notes
# MATTEO (mbarnaba)
```
modified the readme as "git switch" is not provided by older git versions

modified gitignore so that the build directory wont be tracked

added a simple script that automates compilation for any build
directory, given as first argument, './build' is the default one

added our git usernames to the readme. 

```


## to compile and run the code
cmake is required!

```
mkdir build 
cd build 
cmake .. 
cmake --build .
make
cd examples
make
```

Or simply: 
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
