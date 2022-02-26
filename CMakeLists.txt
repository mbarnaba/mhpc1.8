cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set( target "ljmd.x" )

project( ${target} LANGUAGES C )

#if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
#  message(STATUS "Configuring on/for Linux")
#elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
#  message(STATUS "Configuring on/for macOS")
#elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
# message(STATUS "Configuring on/for Windows")
#else()
#  message(STATUS "Configuring on/for ${CMAKE_SYSTEM_NAME}")
#ndif()

add_compile_definitions( LJMD_VERSION=0.1 )


file( GLOB_RECURSE SRC_FILES src/ljmd.c src/input.c src/output.c src/force.c src/utilities.c src/verlet.c )
#file( GLOB_RECURSE SRC_FILES src/ljmd.c src/force.c )

set(HEADER_FILES ${YOUR_DIRECTORY}include/force.h )

include( CheckLibraryExists )

CHECK_LIBRARY_EXISTS( m sin "" HAVE_LIB_M )                                                                                                
if (HAVE_LIB_M)                                                                                                                          
    set( EXTRA_LIBS ${EXTRA_LIBS} m )                                                                                                      
endif (HAVE_LIB_M)

# look for OpenMP and OpenMP library
#find_package(OpenMP REQUIRED)
#include(CheckIncludeFile)
#check_include_file(omp.h HAVE_OMP_H)
#if(!HAVE_OMP_H)
#    message(FATAL_ERROR "Must have OpenMP runtime and omp.h header file")
#endif()
include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable( ${target} ${SRC_FILES} )

#target_compile_definitions(${target} PUBLIC "COMPILER_NAME=\"${CMAKE_C_COMPILER_ID}\"")

#if(CMAKE_C_COMPILER_ID MATCHES Intel)
#  target_compile_definitions(${target}PUBLIC "IS_INTEL_C_COMPILER")
#endif()
#if(CMAKE_C_COMPILER_ID MATCHES GNU)
#  target_compile_definitions(${target} PUBLIC "IS_GNU_C_COMPILER")
#endif()


#target_link_libraries( ${target} PUBLIC ${EXTRA_LIBS} OpenMP::OpenMP_C )
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
