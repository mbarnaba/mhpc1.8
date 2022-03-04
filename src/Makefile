IPATH=../include
OPATH=../Obj-para
SRCS = $(wildcard *.c)
OBJS_exe = $(SRCS:%.c=$(OPATH)/%.o)
OBJS_openmp = $(SRCS:%.c=$(OPATH)/%.openmp.o)
OBJS_mpi = $(SRCS:%.c=$(OPATH)/%.mpi.o)
HEADERS = $(wildcard $(IPATH)/*.h)
EXE = ../ljmd-para.x
CC = gcc
CFLAGS = -I $(IPATH) -Wall -Wextra -ffast-math -fomit-frame-pointer -O3 -DLJMD_VERSION=1.0 -std=c99
LDFLAGS=-lm -L. -g 
OMPFLAGS=-fopenmp #-D_OPENMP
MPIC = mpicc

default: serial

.PHONY: default
serial: $(OBJS_exe)
	$(CC) $^ -o $(EXE) $(LDFLAGS)

$(OPATH)/%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@
	
.PHONY: serial
		
openmp: $(OBJS_openmp)
	$(CC) $^ -o $(EXE) $(LDFLAGS) -fopenmp

$(OPATH)/%.openmp.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $(OMPFLAGS) $< -o $@
	
.PHONY: openmp

mpi: $(OBJS_mpi) $(HEADERS)
	$(MPIC) $^ -o $(EXE) $(LDFLAGS) -DUSE_MPI 

$(OPATH)/%.mpi.o: %.c $(HEADERS)
	$(MPIC) -c $(CFLAGS) -DUSE_MPI $< -o $@

.PHONY: mpi	

clean:
	rm -f $(OBJS_openmp)  
	
.PHONY: clean
