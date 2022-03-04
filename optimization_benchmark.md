Marco Celoria
# Optimization benchmark

In this brief note we report our benchmark for the LJMD code.
Our main focus is the optimization of the serial code without considering parallelization aspects, specifically we optimized the force computation.

We have proceed incrementally:

Starting from the baseline code, we first improve performances by exploiting the compiler, using optimization flags.

Then, we have improved the algorithm by means of our knowledge of the physics of the system, specifically by taking advantage of Newton's third law.

Finally, we have considered additional optimization such as substituting costly mathematical operations with faster ones and improving the data structure.

Clearly, we have implemented these improvements having in mind that these shall be mergeable with MPI and OpenMP.
Tests have been performed by running the `benchmark.sh` script on node with 12 cpu cores

```
Intel(R) Xeon(R) CPU E5-2697 v2 @ 2.70GHz
```

Particularly, we have measured the computational time, without counting the times it takes for input/output operations.
Two cases have been considered the first with 108 argon atoms with 10000 steps (`argon_108.inp`), the second with 2916 argon atoms and 1000 steps (`argon_2916.inp`).
We have performed 10 runs and take the average time.

Results are summarized in the following graphs 

![optimization_time.png](img/optimization_time.png)
![optimization_speedup.png](img/optimization_speedup.png)


The baseline time for 108 argon case is 21.2740526 seconds, while for the 2916 atoms we have 353.4211562 seconds, compiled with gcc version 8.2.0.

By compiling the same baseline code with the following optimization flags 

```
-Wall -O3 -ffast-math -fomit-frame-pointer -fexpensive-optimizations -msse3 
```

we get 3.0546928 and 93.6310114 seconds for the 108 and 2916 atoms cases, respectively, corresponding to a speedup of roughly 7x and 4x.

By changing the algorithm to exploint Newton's third law we halve the computation and the computation time.
Specifically, we get 1.4319414 seconds for the 108 atom case and 40.519560 seconds for 2916 atoms, i.e. roughly 2x speedup.

Finally, by avoiding complex and costly mathematical operations (like powers, square roots and divisions) with simpler and more efficient ones together with a better use of data structure we get 1.2019493 seconds (1.2x) for the 108 atom cases and 39.6687816 seconds for 2916 atoms (only 1.02x indicating low performance improvement with system size).

