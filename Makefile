DEBUGFLAGS=

CC=gcc
GPP=g++ -std=gnu++0x
CFLAGS=-Wall -O3 $(DEBUGFLAGS)

RAND_LIB= -L/Users/erikanson/Desktop/McQuinn/code/massFcn
RAND_INC= -I/Users/erikanson/Desktop/McQuinn/code/massFcn

GSL_INC=
GSL_LIB= -L/opt/local/lib -lgsl

EXE= var_in_spheres.x
OBJ= sphere.o gslRandUtils.o


LIBFLAGS= $(GSL_LIB) $(RAND_LIB)
INCFLAGS= $(GSL_INC) $(RAND_INC)

RM=/bin/rm -f

all: $(EXE) Makefile


var_in_spheres.x: var_in_spheres.c $(OBJ)
	$(GPP) $(CFLAGS) $(LIBFLAGS) -o $@ $^


.cc.o: Makefile
	$(GPP) $(CFLAGS) $(INCFLAGS) -o $*.o -c $*.cc

.c.o: Makefile
	$(GPP) $(CFLAGS) $(INCFLAGS) -o $*.o -c $*.c


clean:
	$(RM) $(EXE) $(OBJ)