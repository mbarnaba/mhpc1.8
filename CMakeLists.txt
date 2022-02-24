cmake_minimum_required(VERSION 3.10)

set( target "ljmd.x" )

project( ${target} LANGUAGES C )


add_compile_definitions( LJMD_VERSION=0.1 )


file( GLOB_RECURSE SRC_FILES src/ljmd.c )
#file( GLOB_RECURSE SRC_FILES src/ljmd.c src/force.c )
add_executable( ${target} ${SRC_FILES} )

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

target_link_libraries( ${target} PUBLIC ${EXTRA_LIBS} OpenMP::OpenMP_C )
#target_link_libraries( ${target} PUBLIC ${EXTRA_LIBS} )

target_compile_options( 
    ${target} PRIVATE 
    -Wall -g -O3 -ffast-math -fomit-frame-pointer -fopenmp
)


# let's enable this by default cause we are in a early dev stage
set(ENABLE_TESTING true)
if(ENABLE_TESTING)
    enable_testing()

    add_test(
        NAME run
        COMMAND ${CMAKE_SOURCE_DIR}/tests.sh ${CMAKE_SOURCE_DIR} run
    )
    
    add_test(
        NAME writes
        COMMAND ${CMAKE_SOURCE_DIR}/tests.sh ${CMAKE_SOURCE_DIR} writes
    )
    
    add_test(
        NAME output
        COMMAND ${CMAKE_SOURCE_DIR}/tests.sh ${CMAKE_SOURCE_DIR} output
    )
endif()
