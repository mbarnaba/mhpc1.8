#! /bin/bash 

# a simple script that automates the compilation process 
# for any build directory, give as first argument.
# It requires cmake 


builddir="$1"
[[ -z "${builddir}" ]] && builddir='./build'

echo "builddir = ${builddir}"


[[ -d "${builddir}" ]] && rm -r "${builddir}"
mkdir -p "${builddir}"

cwd="$( pwd )"
cd "${builddir}" 

cmake "${cwd}" 
cmake --build '.'
