# Two
C++ Code Project

TODO
- Standardize makefile to support debug and release compilation options
- makefile to support src/test/resources

NOTES
- The "standardized" version of the makefile now supports making tests and a standalone executable, 
and requires the developer to use the src/main/cpp and src/test/cpp structure - the Main.cpp file
is the only standard requirement, and Test.cpp is not really special but a main implementation should
be present, with the understanding that its body will run after all the tests have loaded and run.
