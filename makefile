.DEFAULT_GOAL := runTest

# compiler and linker
compiler := $(shell find /usr/local/bin /usr/bin | grep -P "\/g\+\+" | tail -1)

# the target binary
target := main
testTarget := test

# variables
sourceDir := src/main/cpp
testDir := src/test/cpp
targetDir := target
objectsDir := $(targetDir)/objects
resourcesDir := src/main/resources
sourceFileExtension := cpp
dependencyFileExtension := d
objectFileExtension := o

# flags, libraries, and includes
compilerOptions := -std=c++11 -Wall -g -D_DEBUG_
linkerOptions := -lstdc++
additionalLibraries :=

# the rest of this file is common to all projects
sourceFiles := $(shell find $(sourceDir) -type f -name *.$(sourceFileExtension) ! -name "*Main.cpp")
sourceObjectFiles := $(patsubst $(sourceDir)/%,$(objectsDir)/%,$(sourceFiles:.$(sourceFileExtension)=.$(objectFileExtension)))
sourceFileMain := $(shell find $(sourceDir) -type f -name "*Main.cpp")
sourceObjectMain := $(patsubst $(sourceDir)/%,$(objectsDir)/%,$(sourceFileMain:.$(sourceFileExtension)=.$(objectFileExtension)))
testFiles := $(shell find $(testDir) -type f -name *.$(sourceFileExtension))
testObjectFiles := $(patsubst $(testDir)/%,$(objectsDir)/%,$(testFiles:.$(sourceFileExtension)=.$(objectFileExtension)))

# run
run: build
	@echo
	@$(targetDir)/$(target)

# basic build
build: resources $(target)

# run the tests
runTest: buildTest
	@echo
	@$(targetDir)/$(testTarget)

# build the tests
buildTest: resources $(testTarget)

# copy resources to the target directory
resources: directories
	@cp $(resourcesDir)/* $(targetDir)/

# make the directories
directories:
	@mkdir -p $(targetDir)
	@mkdir -p $(objectsDir)
	@mkdir -p $(objectsDir)

# completely remove the target directory
clean:
	@$(RM) -rf $(targetDir)

# pull in dependency info for *existing* .o files
-include $(sourceObjectFiles:.$(objectFileExtension)=.$(dependencyFileExtension))
-include $(testObjectFiles:.$(objectFileExtension)=.$(dependencyFileExtension))

# link
$(target): $(sourceObjectFiles) $(sourceObjectMain)
	@echo "Linking $(targetDir)/$(target)"
	@$(compiler) -o $(targetDir)/$(target) $^ $(linkerOptions) $(additionalLibraries)

$(testTarget): $(sourceObjectFiles) $(testObjectFiles)
	@echo "Linking $(targetDir)/$(target)"
	@$(compiler) -o $(targetDir)/$(testTarget) $^ $(linkerOptions) $(additionalLibraries)
	
# compile
$(objectsDir)/%.$(objectFileExtension): $(sourceDir)/%.$(sourceFileExtension)
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	@$(compiler) $(compilerOptions) -c -o $@ $<
	@$(compiler) $(compilerOptions) -MM $(sourceDir)/$*.$(sourceFileExtension) > $(objectsDir)/$*.$(dependencyFileExtension)
	@cp -f $(objectsDir)/$*.$(dependencyFileExtension) $(objectsDir)/$*.$(dependencyFileExtension).tmp
	@sed -e 's|.*:|$(objectsDir)/$*.$(objectFileExtension):|' < $(objectsDir)/$*.$(dependencyFileExtension).tmp > $(objectsDir)/$*.$(dependencyFileExtension)
	@sed -e 's/.*://' -e 's/\\$$//' < $(objectsDir)/$*.$(dependencyFileExtension).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(objectsDir)/$*.$(dependencyFileExtension)
	@rm -f $(objectsDir)/$*.$(dependencyFileExtension).tmp

$(objectsDir)/%.$(objectFileExtension): $(testDir)/%.$(sourceFileExtension)
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	@$(compiler) $(compilerOptions) -I$(sourceDir) -c -o $@ $<
	@$(compiler) $(compilerOptions) -I$(sourceDir) -MM $(testDir)/$*.$(sourceFileExtension) > $(objectsDir)/$*.$(dependencyFileExtension)
	@cp -f $(objectsDir)/$*.$(dependencyFileExtension) $(objectsDir)/$*.$(dependencyFileExtension).tmp
	@sed -e 's|.*:|$(objectsDir)/$*.$(objectFileExtension):|' < $(objectsDir)/$*.$(dependencyFileExtension).tmp > $(objectsDir)/$*.$(dependencyFileExtension)
	@sed -e 's/.*://' -e 's/\\$$//' < $(objectsDir)/$*.$(dependencyFileExtension).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(objectsDir)/$*.$(dependencyFileExtension)
	@rm -f $(objectsDir)/$*.$(dependencyFileExtension).tmp

# non-file targets
.PHONY: run build runTest buildTest clean resources
