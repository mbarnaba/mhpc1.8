## Group 1:

- Celoria, Marco &rarr; marco-celoria
- Hasanuzzaman, Mohammad &rarr; hasanictp
- Barnaba, Matteo &rarr; mbarnaba

## to compile and run the code
cmake is required! 
```
./compile.sh [build_dir]
```
where build_dir is optional (if not given it will default to './build') and can be 
any directory. 


## testing 

Testing is enabled by default and you run the tests simply by running "ctest" from inside the build directory (build_dir). 
Both native testing and tests via Google Tests can be implemented and run. 
To add add new test simply add it to the relevant section inside CMakeLists.txt. 
```
if(ENABLE_TESTING)
    enable_testing()

    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/examples/argon_108.inp ${CMAKE_CURRENT_BINARY_DIR}
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/examples/argon_108.rest ${CMAKE_CURRENT_BINARY_DIR}
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/examples/argon_2916.inp ${CMAKE_CURRENT_BINARY_DIR}
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/examples/argon_2916.rest ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    add_test(
        NAME RunTest 
        COMMAND /bin/sh -c "${CMAKE_CURRENT_BINARY_DIR}/${target} < argon_108.inp && test -f argon_108.dat && test -f argon_108.xyz"
    )
    set_tests_properties( RunTest PROPERTIES PASS_REGULAR_EXPRESSION ".*Simulation Done.*" )
    ...
```
or 
```
if (RUN_GTEST) 
    set( GSRC_FILES ${SRC_FILES} )
    list( REMOVE_ITEM GSRC_FILES ${CMAKE_SOURCE_DIR}/src/mympi.c )

    add_library( mdlib ${GSRC_FILES} )
    target_compile_definitions( mdlib PRIVATE MAKING_LIB )
    
    enable_language( CXX )
    set( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} )
    include( GTest )
    
    add_executable( 
        test_kinetic ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_kinetic.cpp 
    )
    target_include_directories( test_kinetic PRIVATE ${CMAKE_SOURCE_DIR}/include )
    target_compile_definitions( test_kinetic PRIVATE MAKING_LIB )
...
```
Please add the "MAKING_LIB" definition to your test (as show above) because MPI and Google Tests do not get along and otherwise your test will not compile 
(sorry, I will fix this).


# MPI benchmark 


A benchmark for MPI has been performed measuring computation times for [1, 48] cores for both argon_108.inp and argon_2916.inp. 
The benchmark took place on 2 computational nodes with 24 Intel(R) Xeon(R) CPU E5-2697 v2 @ 2.70GHz processors. 
The executable was compiler with gcc-8.2.0 and openmpi-4.0.1.

The results are presented below, both in graphical and tabular form. 
The scaling is not bad but could be much better. 
It is however encouraging to see that the speedup improves with larger problem sizes. 


![MPI scaling](./mpi.svg)


```
	argon_108		argon_2916	
ranks	seconds	speedup	seconds	speedup
1.00	1.49	1.00	38.47	1.00
2.00	0.85	1.76	21.14	1.82
3.00	0.63	2.39	14.56	2.64
4.00	0.56	2.67	11.14	3.45
5.00	0.50	3.01	9.33	4.12
6.00	0.46	3.23	7.98	4.82
7.00	0.44	3.42	6.98	5.51
8.00	0.41	3.65	6.34	6.07
9.00	0.39	3.78	5.68	6.77
10.00	0.40	3.73	5.31	7.24
11.00	0.39	3.80	4.95	7.76
12.00	0.39	3.83	4.54	8.48
13.00	0.38	3.88	4.28	8.99
14.00	0.38	3.89	4.02	9.58
15.00	0.39	3.83	3.80	10.12
16.00	0.37	3.99	3.61	10.65
17.00	0.39	3.82	3.50	11.00
18.00	0.39	3.81	3.30	11.66
19.00	0.40	3.75	3.21	11.97
20.00	0.40	3.77	3.08	12.51
21.00	0.39	3.81	2.96	13.01
22.00	0.40	3.72	2.86	13.47
23.00	0.40	3.76	2.74	14.04
24.00	0.40	3.71	2.71	14.19
25.00	0.60	2.50	2.97	12.96
26.00	0.61	2.45	2.91	13.23
27.00	0.61	2.46	2.78	13.85
28.00	0.63	2.37	2.80	13.71
29.00	0.63	2.38	2.71	14.20
30.00	0.65	2.30	2.72	14.15
31.00	0.64	2.32	2.64	14.56
32.00	0.68	2.21	2.67	14.42
33.00	0.69	2.17	2.64	14.56
34.00	0.71	2.11	2.64	14.55
35.00	0.69	2.16	2.59	14.85
36.00	0.73	2.04	2.55	15.09
37.00	0.72	2.07	2.56	15.02
38.00	0.73	2.05	2.56	15.01
39.00	0.74	2.02	2.52	15.26
40.00	0.76	1.97	2.52	15.26
41.00	0.76	1.97	2.51	15.33
42.00	0.77	1.93	2.51	15.33
43.00	0.77	1.94	2.52	15.26
44.00	0.79	1.89	2.51	15.31
45.00	0.82	1.83	2.48	15.50
46.00	0.82	1.82	2.49	15.46
47.00	0.81	1.84	2.51	15.30
48.00	0.86	1.74	2.52	15.28
```
