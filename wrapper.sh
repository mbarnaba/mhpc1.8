#! /bin/bash 


rank="${OMPI_COMM_WORLD_RANK}"


if (( rank == 0 )); then 
    ./ljmd.x < ./argon_108.inp
else 
    ./ljmd.x < ./argon_108.inp 1>/dev/null 2>/dev/null
fi      
