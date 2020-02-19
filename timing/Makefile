#
# Makefile for timing examples
#

CXX = g++
LD  = g++
LDFLAGS = -L../darray -ldarray -lblas -llapack
CXXINCLUDE = -I../darray
CXXFLAGS = -g -O3 -pg

OBJECTS = timing_mm.o
EXEC = ex.x	

.PHONY: clean

exe: $(EXEC)


$(EXEC): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(EXEC)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(CXXINCLUDE) -c $<

clean:
	rm -f $(OBJECTS) $(EXEC) 
