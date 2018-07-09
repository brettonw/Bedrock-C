#! /usr/bin/env perl

use strict;
use warnings FATAL => 'all';

binmode STDOUT, ":utf8";
binmode STDERR, ":utf8";
use utf8;

use File::Path qw(make_path);
use File::Basename;
use File::Glob qw(:glob csh_glob);
use Cwd qw(abs_path);

# search my script directory location for the "my" lib
use lib dirname (abs_path(__FILE__));
use my::JsonFile;
use my::Web;

# configurations -
#   files:
#           build configuration files in json format, "build.json", are read at multiple levels. the
#           first default is in the build script binary directory and contains global defaults. the
#           second default is at the root of the project directory, and the final build
#           configuration can be found inside each individual source path. each variable requested
#           is processed to the lowest level configuration in which it is found, so that most
#           defaults can be specified at the root config level, and overridden at the lower levels
#           if necessary, possibly using the parent value in its redefinition. configuration
#           variables can also be redefined at the command line, as well.
#
#           example build.json:
#           {
#               "type": "library",
#               "configurations": {
#                   "debug": {
#                       "compilerOptions": "-g -D_DEBUG_",
#                       "linkerOptions": ""
#                   },
#                   "release": {
#                       "compilerOptions": "-O3 -D_NDEBUG_",
#                       "linkerOptions": ""
#                   }
#               }
#           }
#
#   names:
#           source directories are named for the final product name (e.g. directory "math" builds a
#           library called "math", but this can be overridden with a build configuration option
#           called "name".
#   variables:
#           configuration variables are mostly defined statically, but can be specified to be
#           interpreted by a shell or by variable replacement, by preceding the value with "*" or
#           "$", respectively.
#   dependencies:
#           binaries (library or application) might depend on another library being built first, so
#           dependencies can be named in the variable "dependencies".
#
# tree structure -
# i was originally going to try to mimic a maven project and allow other languages to be present
# (java, etc.), but decided that a separate directory is ultimately cleaner.
#
# source -+- (lib)
#         +- (lib)
#         +- (app)   --- (app.cpp)
#         |
#         +- (app)   -+- (app.cpp)
#                     +- resources
#
# target -+- (lib) -+- (config) -+- objects -+- *.o
#         |         |            |           +- *.d
#         |         |            +- (built)
#         |         |
#         |         +- (config) -+- objects -+- *.o
#         |                      |           +- *.d
#         |                      +- (built)
#         |
#         +- (lib) -+- (config) -+- objects -+- *.o
#         |         |            |           +- *.d
#         |         |            +- (built)
#         |         |
#         |         +- (config) -+- objects -+- *.o
#         |                      |           +- *.d
#         |                      +- (built)
#         |
#         +- (app) -+- (config) -+- objects -+- *.o
#         |         |            |           +- *.d
#         |         |            +- (built)
#         |         |
#         |         +- (config) -+- objects -+- *.o
#         |                      |           +- *.d
#         |                      +- (built)
#         |
#         +- (app) -+- (config) -+- objects -+- *.o
#                   |            |           +- *.d
#                   |            +- (built)
#                   |            +- (copied resources)
#                   |
#                   +- (config) -+- objects -+- *.o
#                                |           +- *.d
#                                +- (built)
#                                +- (copied resources)
#

my $buildFileName = "build.json";

# get the build script source directory
my $buildScriptSourceDirectory = dirname(abs_path(__FILE__));
#print STDERR "Executing: $buildScriptSourceDirectory\n";

# load a default global configuration, and the project configuration if it exists
my $buildConfigurationStack = [];
push (@$buildConfigurationStack, JsonFile::read("$buildScriptSourceDirectory/$buildFileName"));

#my $globalBuildConfiguration = JsonFile::read("$buildScriptSourceDirectory/$buildFileName");
#my $projectBuildConfiguration = {};
#my $sourcePathBuildConfiguration = {};
#my $commandLineBuildConfiguration = {};

sub getConfigurationVariable {
    my ($variableName) = @_;

    # loop over the build configuration stack
    my $value = "";
    for my $buildConfiguration (@$buildConfigurationStack) {
        if (exists ($buildConfiguration->{$variableName})) {
            my $existingValue = $value;
            $value = $buildConfiguration->{$variableName};
            if (ref($value) ne "HASH") {
                # do variable substitution if the existing value should be used in the replacement
                $value =~ s/\$$variableName/$existingValue/g;

                # do a substitution with a shell command if one is requested
                $value =~ s/\$\(([^\)]*)\)/my $x = qx%$1%; chomp $x; $x/e;
            }
        }
    }
    return $value;
}

# process the command line options into a configuration hash
my $commandLineBuildConfiguration = {};
foreach my $argument (@ARGV) {
    if ($argument =~ /^([^=]*)=([^=]*)$/) {
        my $key = $1;
        my $value = $2;
        if (getConfigurationVariable($1) ne "") {
            $commandLineBuildConfiguration->{$1} = $2;
            print STDERR "$1 = $2\n";
        } else {
            print STDERR "Ignoring unknown build configuration variable: $key\n";
        }
    }
}
push (@$buildConfigurationStack, $commandLineBuildConfiguration);
push (@$buildConfigurationStack, JsonFile::read(getConfigurationVariable ("buildConfigurationFileName")));

# read the source path looking for subdirs
my @sourcePaths;
my $sourcePath = getConfigurationVariable ("sourcePath");
#print STDERR "SourcePath: $sourcePath\n";
if (opendir(SOURCEDIR, $sourcePath)) {
    while (my $file = readdir(SOURCEDIR)) {
        next unless (($file !~ /^\./) && (-d "$sourcePath/$file"));
        #print "$sourcePath/$file\n";

        # look inside the potential sourcepath for a build.json file
        push(@$buildConfigurationStack, JsonFile::read("$sourcePath/$file/" . getConfigurationVariable("buildConfigurationFileName")));

        print STDERR "Source Path: $file\n";
        push(@sourcePaths, $file);

    }
    closedir(SOURCEDIR);
} else {
    print STDERR "Can't open source directory ($sourcePath), $!\n";
}

# recreate
