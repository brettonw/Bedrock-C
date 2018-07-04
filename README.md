# Two
C++ Code Project

- .o files need to be built into target
- src/main files should all be compiled together into a lib, for each subdir
- src/test files should each be compiled and linked against the found libs

steps:
1. scan src/main for subdirs
2. analyze dependencies
3. for each subdir, make a copy under $PROJECT_DIR/target, compile each file that is newer
   than the corresponding object file - or whose dependency tree indicates recompilation.
   drop the resulting .lib files in target.
4. for each file under src/test - compile and link as an executable against all the libs.
   then run it, and report success or failure
5. if there is a program.cpp or $PROJECT_NAME.cpp file under src/main - compile and link it
   as an executable against all the libs under target_

	src
	 +-	lib
	     +- com/brettonw/common
	     +- com/brettonw/scanner
	 +- app
	     +- editor
	     +- blahblah
	 +- test
	     + lib/common/brettonw/common/text
	     + lib/common/brettonw/common/rawtext
