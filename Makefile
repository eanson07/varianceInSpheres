DEBUGFLAGS=

CC=gcc
GPP=g++ -std=gnu++0x
CFLAGS=-Wall -O3 $(DEBUGFLAGS)

GSL_INC= -I$(TACC_GSL_INC) -I$(TACC_GSL_INC)/gsl
GSL_LIB= -L$(TACC_GSL_LIB) 

EXE= var_in_spheres.x
OBJ= sphere.o gslRandUtils.o hack_hash.o


LIBFLAGS= $(GSL_LIB) -lgsl -lgslcblas
INCFLAGS= $(GSL_INC)

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