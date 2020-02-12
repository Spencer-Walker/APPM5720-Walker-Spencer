To run the code just type make and it should run immediately after compiling as the old code did. If you don't want output to bo displayed remove the -D VERBOSE from the CXXFLAGS variable in the makefile. 

From the 512x512 case its clear that having the j loop outside is better than the i loop on the outside.

+ i outer j inner
T = 29578ms
* i outer j inner
T = 29955ms
+ j outer i inner
T = 17004ms
* j outer i inner
T = 17226ms

