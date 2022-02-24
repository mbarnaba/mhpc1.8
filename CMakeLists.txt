cmake_minimum_required(VERSION 3.10)

set( target "ljmd.x" )

project(${target} LANGUAGES C)


add_compile_definitions(LJMD_VERSION=0.1)


file(GLOB_RECURSE SRC_FILES src/ljmd.c )
add_executable(${target} ${SRC_FILES})

include(CheckLibraryExists)

CHECK_LIBRARY_EXISTS(m sin "" HAVE_LIB_M)                                                                                                
if (HAVE_LIB_M)                                                                                                                          
    set(EXTRA_LIBS ${EXTRA_LIBS} m)                                                                                                      
endif (HAVE_LIB_M)


target_link_libraries(${target} PUBLIC ${EXTRA_LIBS})

target_compile_options( 
    ${target} PRIVATE 
    -Wall -g -O3 -ffast-math -fomit-frame-pointer
)


if(ENABLE_TESTING)
  enable_testing()

  # does the application run?
  add_test(NAME Runs COMMAND ${target})

  #does it create the file write.ppm?
  #file(WRITE ${CMAKE_BINARY_DIR}/test_write.sh "rm -f *.ppm && ./ljmd-serial.x && test -f white.ppm")
  #add_test(NAME Writes COMMAND bash test_write.sh 5)
endif()
