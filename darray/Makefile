# makefile for the newton3 example 
# Here we use the macros CXX, CXXLAGS, and general rules for compiling 
# .cpp files.
# We also use the macro OBJECTS to not have to keep track of the 
# object files in more than one place.
# The PHONY target clean removes non-source fils.

CXX = g++
LD  = g++
LDFLAGS = -pg -g
CXXFLAGS = -g -O3 -DBZ_DEBUG
# CXXFLAGS = -O3 

AR = ar

OBJECTS = main.o Darray.o
EXEC = ./gl.x

OBJECTS_TT = timing_test.o Darray.o
EXEC_TT = ./timing_test.x

OBJECTS_2D = test2D.o Darrays.o
EXEC_2D = ./test2D.x

.PHONY: clean darrayslib darrayslib_dbg

exe: $(EXEC_2D)

run_2D: $(EXEC_2D)
	$(EXEC_2D)   

run_tt: $(EXEC_TT)
	$(EXEC_TT)   

run_it: $(EXEC)
	$(EXEC)   

$(EXEC): $(OBJECTS)
	$(LD) $(OBJECTS) -o $(EXEC)

$(EXEC_TT): $(OBJECTS_TT)
	$(LD) $(OBJECTS_TT) -o $(EXEC_TT)

$(EXEC_2D): $(OBJECTS_2D)
	$(LD) $(OBJECTS_2D) -o $(EXEC_2D)

darrayslib: 
	g++ -O3 -c Darrays.cpp
	$(AR) rcs libdarray.a Darrays.o

darrayslib_dbg: Darrays.o
	g++ -g -DBZ_DEBUG -c Darrays.cpp
	$(AR) rcs libdarray_dbg.a Darrays.o


%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(EXEC) $(OBJECTS_2D) $(EXEC_2D)
