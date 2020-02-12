# makefile for the newton3 example 
# Here we use the macros CXX, CXXLAGS, and general rules for compiling 
# .cpp files.
# We also use the macro OBJECTS to not have to keep track of the 
# object files in more than one place.
# The PHONY target clean removes non-source fils.

CXX = g++
LD  = g++
LDFLAGS = 
CXXFLAGS = -O0 -DBZ_DEBUG
EXEC = ./gl.x

OBJECTS = main.o Darray.o
.PHONY: clean

run_it: $(EXEC)
	$(EXEC)   

$(EXEC): $(OBJECTS)
	$(LD) $(OBJECTS) -o $(EXEC)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(EXEC)
