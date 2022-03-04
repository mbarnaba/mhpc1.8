cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set( target "ljmd.x" )

project( ${target} LANGUAGES C )

# print custom message depending on the operating system
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  message(STATUS "Configuring on/for Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  message(STATUS "Configuring on/for macOS")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
 message(STATUS "Configuring on/for Windows")
else()
  message(STATUS "Configuring on/for ${CMAKE_SYSTEM_NAME}")
endif()


option( USE_COMPILER "Build with compilar optimizations enabled I support" ON )
option( USE_NEWTON "Build with Newton's 3rd law (halvs the calculations)" ON )
option( USE_MATH "Build with fast math operations and better organization of data" ON )
option( USE_MPI "Build with MPI support" OFF )
option( USE_OMP "Build with Open MP support" OFF )
option( ENABLE_TESTING "Enable Testing" ON )
option( RUN_GTEST "Enable Google tests" ON )


add_compile_definitions( LJMD_VERSION=0.1 )
file( GLOB_RECURSE SRC_FILES src/ljmd.c src/input.c src/output.c src/force.c src/utilities.c src/verlet.c )
#file( GLOB_RECURSE SRC_FILES src/ljmd.c src/force.c )
include( CheckLibraryExists )
CHECK_LIBRARY_EXISTS( m sin "" HAVE_LIB_M )                                                                                                
if (HAVE_LIB_M)                                                                                                                          
    set( EXTRA_LIBS ${EXTRA_LIBS} m )                                                                                                      
endif (HAVE_LIB_M)


# look for OpenMP and OpenMP library
if (USE_OMP) 
    find_package(OpenMP REQUIRED)
    include(CheckIncludeFile)
    check_include_file( omp.h HAVE_OMP_H )
    if(!HAVE_OMP_H)
        add_compile_definitions( _OMP=yes )
        target_compile_options( 
            ${target} PRIVATE 
            -Wall -g -O3 -ffast-math -fomit-frame-pointer -fopenmp
        )
        list( APPEND EXTRA_LIBS ${OMP_C_LIBRARIES} )
    else()
        message( FATAL_ERROR "Must have OpenMP runtime and omp.h header file" )
    endif()
endif()


# look for mpi library
if (USE_MPI) 
    find_package( MPI REQUIRED )
    include(CheckIncludeFile)
    check_include_file( mpi.h HAVE_MPI_H )
    message( "building with MPI" )
    if(HAVE_MPI_H)
        add_compile_definitions( _MPI=yes )
        list( APPEND SRC_FILES ${PROJECT_SOURCE_DIR}/src/mympi.c )
        list( APPEND EXTRA_LIBS ${MPI_C_LIBRARIES} )
    else()
        message( FATAL_ERROR "Must have mpi.h header file" )
    endif()
endif()



set(HEADER_FILES ${CMAKE_SOURCE_DIR}/include )
include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable( ${target} ${SRC_FILES} )


# let the preprocessor know about the host processor architecture
if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i386")
  message(STATUS "i386 architecture detected")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i686")
  message(STATUS "i686 architecture detected")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
  message(STATUS "x86_64 architecture detected")
else()
  message(STATUS "host processor architecture is unknown")
endif()

#target_link_libraries( ${target} PUBLIC ${EXTRA_LIBS} OpenMP::OpenMP_C MPI::MPI_C )

target_link_libraries( ${target} PUBLIC ${EXTRA_LIBS}  )

if (USE_COMPILER) 
    message( "using compiler opitmizations" )
    target_compile_options( 
        ${target} PRIVATE 
        -Wall -O3 -ffast-math -fomit-frame-pointer -fexpensive-optimizations -msse3 
    )
endif()

if (USE_NEWTON) 
    message( "using Newton's 3rd law" )
    add_compile_definitions( _NEWTON=yes )
endif()

if (USE_MATH) 
    message( "using fast math operations and better organization of data" )
    add_compile_definitions( _MATH=yes )
endif()


# let's enable this by default cause we are in a early dev stage
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

    
    # Google Test
    if (RUN_GTEST) 
        enable_language( CXX )
        set( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} )
        include( GTest )
        
        add_library( mdlib ${SRC_FILES} )
        target_compile_definitions( mdlib PRIVATE MAKING_LIB )
        
        add_executable( 
            test_kinetic ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_kinetic.cpp 
        )
        target_include_directories( test_kinetic PRIVATE ${CMAKE_SOURCE_DIR}/include )

        target_link_libraries( 
            test_kinetic PRIVATE GTest::GTestMain GTest::GTest mdlib
        )
        add_test(
            NAME Kinetic
            COMMAND test_kinetic
        )


	add_executable( 
            test_pbc ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_pbc.cpp 
        )
        target_include_directories( test_pbc PRIVATE ${CMAKE_SOURCE_DIR}/include )

        target_link_libraries( 
            test_pbc PRIVATE GTest::GTestMain GTest::GTest mdlib
        )
        add_test(
            NAME PBC
            COMMAND test_pbc
        )
    endif(RUN_GTEST)
    

    if (USE_MPI) 
        set( nranks 2 )

        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/wrapper.sh ${CMAKE_CURRENT_BINARY_DIR}
        )
        add_test(
            NAME RunTestMPI
            COMMAND 
            /bin/sh -c "mpirun -np ${nranks} ${CMAKE_CURRENT_BINARY_DIR}/wrapper.sh && test -f argon_108.dat && test -f argon_108.xyz"
        )
        set_tests_properties( 
            RunTestMPI 
            PROPERTIES PASS_REGULAR_EXPRESSION ".*MPI successfuly finalized with ${nranks} ranks*."
        )
    endif(USE_MPI)
endif(ENABLE_TESTING)
