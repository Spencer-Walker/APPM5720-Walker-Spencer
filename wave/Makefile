#
# Makefile for timing examples
#

CXX = g++
LD  = g++
SILO_LIB = /home/becker/spwa4419/Documents/silo-4.10.2/lib
SILO_INC = /home/becker/spwa4419/Documents/silo-4.10.2/include
DARRAY_LIB = .
DARRAY_INC = .
LDFLAGS = -L$(SILO_LIB) -lsilo -lblas -llapack
CXXINCLUDE = -I$(DARRAY_INC) -I$(SILO_INC)
# CXXFLAGS = -g -O0 -pg -DBZ_DEBUG 
CXXFLAGS = -O3

OBJECTS = main.o WaveSimulation.o Darrays.o BoundaryConditions.o
EXEC = ex.x	

.PHONY: clean

exe: $(EXEC)


$(EXEC): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(EXEC)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(CXXINCLUDE) -c $<

clean:
	rm -f $(OBJECTS) $(EXEC) 
