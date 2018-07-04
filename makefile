# compiler and linker
CPP := $(shell find /usr/local/bin | grep -p "\/g++-\d")
CPPFLAGS := -std=c++14

# the Target Binary Program
TARGET := main

# variables
SRCDIR := src/main/cpp
INCDIR := src/main/cpp
TARGETDIR := target
BUILDDIR := $(TARGETDIR)/objects
RESDIR := src/main/resources
SRCEXT := cpp
DEPEXT := d
OBJEXT := o

# flags, libraries, and encludes
CPPFLAGS := -std=c++14 -Wall -O3 -g
LDFLAGS := -lstdc++
LDLIBS :=
INC := -I$(INCDIR)
INCDEP := -I$(INCDIR)

#---------------------------------------------------------------------------------
# DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

# default make all
all: resources $(TARGET)

# remake
remake: clean all

# copy resources to the target directory
resources: directories
	@cp $(RESDIR)/* $(TARGETDIR)/

# make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

# clean, completely remove the target directory
clean:
	@$(RM) -rf $(TARGETDIR)

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

# link
$(TARGET): $(OBJECTS)
	$(CPP) -o $(TARGETDIR)/$(TARGET) $^ $(LDFLAGS) $(LDLIBS)

# compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $(INC) -c -o $@ $<
	@$(CPP) $(CPPFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

# non-file targets
.PHONY: all remake clean resources
