.DEFAULT_GOAL := runTests

# compiler and linker
compiler := $(shell find /usr/local/bin | grep -p "\/g++-\d")
compilerSourceOptions := -std=c++14

# the target binary
target := main
testTarget := test

# variables
sourceDir := src/main/cpp
testDir := src/test/cpp
targetDir := target
sourceObjectsDir := $(targetDir)/objects
resourcesDir := src/main/resources
sourceFileExtension := cpp
dependencyFileExtension := d
objectFileExtension := o

# flags, libraries, and includes
compilerSourceOptions := -std=c++14 -Wall -g -D_DEBUG_
compilerTestOptions :=
linkerOptions := -lstdc++
additionalLibraries :=

# the rest of this file is common to all projects
sourceFiles := $(shell find $(sourceDir) -type f -name *.$(sourceFileExtension) ! -name "*Main.cpp")
sourceObjectFiles := $(patsubst $(sourceDir)/%,$(sourceObjectsDir)/%,$(sourceFiles:.$(sourceFileExtension)=.$(objectFileExtension)))
sourceFileMain := $(shell find $(sourceDir) -type f -name "*Main.cpp")
sourceObjectMain := $(patsubst $(sourceDir)/%,$(sourceObjectsDir)/%,$(sourceFileMain:.$(sourceFileExtension)=.$(objectFileExtension)))
testFiles := $(shell find $(testDir) -type f -name *.$(sourceFileExtension))
testObjectFiles := $(patsubst $(testDir)/%,$(sourceObjectsDir)/%,$(testFiles:.$(sourceFileExtension)=.$(objectFileExtension)))

# run
run: build
	@echo
	@$(targetDir)/$(target)

# basic build
build: resources $(target)

# run the tests
runTests: buildTests
	$(targetDir)/$(testTarget)

# build the tests
buildTests: resources $(testTarget)

# copy resources to the target directory
resources: directories
	@cp $(resourcesDir)/* $(targetDir)/

# make the directories
directories:
	@mkdir -p $(targetDir)
	@mkdir -p $(sourceObjectsDir)
	@mkdir -p $(sourceObjectsDir)

# completely remove the target directory
clean:
	@$(RM) -rf $(targetDir)

# pull in dependency info for *existing* .o files
-include $(sourceObjectFiles:.$(objectFileExtension)=.$(dependencyFileExtension))
-include $(testObjectFiles:.$(objectFileExtension)=.$(dependencyFileExtension))

# link
$(target): $(sourceObjectFiles) $(sourceObjectMain)
	$(compiler) -o $(targetDir)/$(target) $^ $(linkerOptions) $(additionalLibraries)

$(testTarget): $(sourceObjectFiles) $(testObjectFiles)
	$(compiler) -o $(targetDir)/$(testTarget) $^ $(linkerOptions) $(additionalLibraries)

# compile
$(sourceObjectsDir)/%.$(objectFileExtension): $(sourceDir)/%.$(sourceFileExtension)
	@mkdir -p $(dir $@)
	$(compiler) $(compilerSourceOptions) -c -o $@ $<
	@$(compiler) $(compilerSourceOptions) -MM $(sourceDir)/$*.$(sourceFileExtension) > $(sourceObjectsDir)/$*.$(dependencyFileExtension)
	@cp -f $(sourceObjectsDir)/$*.$(dependencyFileExtension) $(sourceObjectsDir)/$*.$(dependencyFileExtension).tmp
	@sed -e 's|.*:|$(sourceObjectsDir)/$*.$(objectFileExtension):|' < $(sourceObjectsDir)/$*.$(dependencyFileExtension).tmp > $(sourceObjectsDir)/$*.$(dependencyFileExtension)
	@sed -e 's/.*://' -e 's/\\$$//' < $(sourceObjectsDir)/$*.$(dependencyFileExtension).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(sourceObjectsDir)/$*.$(dependencyFileExtension)
	@rm -f $(sourceObjectsDir)/$*.$(dependencyFileExtension).tmp

$(sourceObjectsDir)/%.$(objectFileExtension): $(testDir)/%.$(sourceFileExtension)
	@mkdir -p $(dir $@)
	$(compiler) $(compilerSourceOptions) $(compilerTestOptions) -I$(sourceDir) -c -o $@ $<
	@$(compiler) $(compilerSourceOptions) $(compilerTestOptions) -I$(sourceDir) -MM $(testDir)/$*.$(sourceFileExtension) > $(sourceObjectsDir)/$*.$(dependencyFileExtension)
	@cp -f $(sourceObjectsDir)/$*.$(dependencyFileExtension) $(sourceObjectsDir)/$*.$(dependencyFileExtension).tmp
	@sed -e 's|.*:|$(sourceObjectsDir)/$*.$(objectFileExtension):|' < $(sourceObjectsDir)/$*.$(dependencyFileExtension).tmp > $(sourceObjectsDir)/$*.$(dependencyFileExtension)
	@sed -e 's/.*://' -e 's/\\$$//' < $(sourceObjectsDir)/$*.$(dependencyFileExtension).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(sourceObjectsDir)/$*.$(dependencyFileExtension)
	@rm -f $(sourceObjectsDir)/$*.$(dependencyFileExtension).tmp

# non-file targets
.PHONY: run build runTests buildTests clean resources
