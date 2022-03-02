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

add_compile_definitions( LJMD_VERSION=0.1 )
file( GLOB_RECURSE SRC_FILES src/ljmd.c src/input.c src/output.c src/force.c src/utilities.c src/verlet.c )
#file( GLOB_RECURSE SRC_FILES src/ljmd.c src/force.c )
include( CheckLibraryExists )
CHECK_LIBRARY_EXISTS( m sin "" HAVE_LIB_M )                                                                                                
if (HAVE_LIB_M)                                                                                                                          
    set( EXTRA_LIBS ${EXTRA_LIBS} m )                                                                                                      
endif (HAVE_LIB_M)

# look for OpenMP and OpenMP library
find_package(OpenMP REQUIRED)
include(CheckIncludeFile)
check_include_file(omp.h HAVE_OMP_H)
if(!HAVE_OMP_H)
    message(FATAL_ERROR "Must have OpenMP runtime and omp.h header file")
endif()

# look for mpi library
find_package(MPI REQUIRED)
include(CheckIncludeFile)
check_include_file(mpi.h HAVE_MPI_H)
if(HAVE_MPI_H)
    add_compile_definitions( _MPI=yes )
    message( "compiling with MPI" )
    list( APPEND SRC_FILES ${PROJECT_SOURCE_DIR}/src/mympi.c )
    list( APPEND EXTRA_LIBS ${MPI_C_LIBRARIES} )
else()
    message(FATAL_ERROR "Must have mpi.h header file")
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
target_include_directories( ${target} PRIVATE ${YOUR_DIRECTORY})
target_compile_options( 
    ${target} PRIVATE 
    -Wall -g -O3 -ffast-math -fomit-frame-pointer -fopenmp
)


# let's enable this by default cause we are in a early dev stage
set(ENABLE_TESTING true)
if(ENABLE_TESTING)
    enable_testing()

    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/examples/argon_108.inp ${CMAKE_CURRENT_BINARY_DIR}
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/examples/argon_108.rest ${CMAKE_CURRENT_BINARY_DIR}
    )
    add_test(
        NAME RunTest 
        COMMAND /bin/sh -c "${CMAKE_CURRENT_BINARY_DIR}/${target} < argon_108.inp && test -f argon_108.dat && test -f argon_108.xyz"
    )
    set_tests_properties( RunTest PROPERTIES PASS_REGULAR_EXPRESSION ".*Simulation Done.*" )
endif()
