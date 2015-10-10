CC=gcc
GPP=g++
CFLAGS=-Wall -O3


EXE= var_in_spheres.x
OBJ= sphere.o

RAND_LIB= /Users/erikanson/Desktop/McQuinn/code/massFcn/gslRandUtils.o
RAND_INCL= /Users/erikanson/Desktop/McQuinn/code/massFcn

LIBFLAGS= -L/opt/local/lib $(RAND_LIB) -lgsl
INCFLAGS= -I $(RAND_INCL)

RM=/bin/rm -f

all: $(EXE) Makefile


var_in_spheres.x: var_in_spheres.c sphere.o
	$(GPP) $(CFLAGS) $(LIBFLAGS) -o $@ $^
	

sphere.o: sphere.c
	$(GPP) $(CFLAGS) $(INCFLAGS) -o sphere.o -c sphere.c


clean:
	$(RM) $(EXE) $(OBJ)