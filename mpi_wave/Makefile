#
# Makefile for timing examples
#

CXX = mpicxx
LD  = mpicxx
LDFLAGS = 
CXXINCLUDE = 
CXXFLAGS = -O3

OBJECTS = cg_mpi.o Darrays.o
EXEC = cg_mpi.x	

.PHONY: clean

exe: $(EXEC)


$(EXEC): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(EXEC)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(CXXINCLUDE) -c $<

clean:
	rm -f $(OBJECTS) $(EXEC) 
