#! /usr/bin/env perl

use strict;
use warnings FATAL => 'all';
use diagnostics;

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

# build.pl, a comprehensive build script in perl for C++ on UNIX systems (MacOS and Linux).
# written by Bretton Wade, July 2018
#
# configurations -
#   files:
#           build configuration files in json format, "build.json", are read at multiple levels. the
#           first default is in the build script binary directory and contains global defaults. the
#           second default is at the root of the project directory, and the final build
#           configuration can be found inside each individual target. each variable requested is
#           processed to the lowest level configuration in which it is found, so that most defaults
#           can be specified at the root config level, and overridden at the lower levels if
#           necessary, possibly using the parent value in its redefinition. configuration variables
#           can also be redefined at the command line.
#
#           example build.json:
#           {
#               "type": "library",
#               "dependencies":[],
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
#           interpreted by a shell or by variable replacement, by wrapping the value with "$()" or
#           prepending with "$", respectively.
#   dependencies:
#           targets (library or application) might depend on another target being built first, so
#           dependencies can be named in the array variable "dependencies".
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

# get the build script source directory
my $buildScriptSourceDirectory = dirname(abs_path(__FILE__));
#print STDERR "Executing: $buildScriptSourceDirectory\n";

# load a default global configuration into a configuration stack
my $buildConfigurationStack = [];
push (@$buildConfigurationStack, JsonFile::read("$buildScriptSourceDirectory/build.json"));

# function to traverse the configuration stack to retrieve a defined value
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

# process the command line options into a configuration hash in the configuration stack
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

# now load the project local build configuration file into the configuration stack
push (@$buildConfigurationStack, JsonFile::read(getConfigurationVariable ("buildConfigurationFileName")));

# read the source path looking for subdirs, and loading their build configurations
my $targets = {};
my $sourcePath = getConfigurationVariable ("sourcePath");
if (opendir(SOURCEDIR, $sourcePath)) {
    while (my $file = readdir(SOURCEDIR)) {
        next unless (($file !~ /^\./) && (-d "$sourcePath/$file"));
        $targets->{$file} = JsonFile::read("$sourcePath/$file/" . getConfigurationVariable("buildConfigurationFileName"));
    }
    closedir(SOURCEDIR);
} else {
    print STDERR "Can't open source directory ($sourcePath), $!\n";
}

# identify the targets to build, and the dependency order to do it. the dependency graph is implicit
# in the dependencies array for each target, so we traverse it in depth first order, emitting build
# targets on return (marking them as visited).
my $targetsInOrder = [];
sub traverseDependencies {
    my ($target) = @_;
    if (exists ($targets->{$target})) {
        if (!exists($targets->{$target}->{touched})) {
            $targets->{$target}->{touched} = 1;
            my $dependencies = exists($targets->{$target}->{dependencies}) ? $targets->{$target}->{dependencies} : [];
            for my $dependency (sort (@$dependencies)) {
                traverseDependencies($dependency);
            }
            push (@$targetsInOrder, $target);
        }
    } else {
        print STDERR "Unknown target: $target\n";
    }
}

my $targetsToBuild = getConfigurationVariable("target");
$targetsToBuild = (ref $targetsToBuild eq "ARRAY") ? $targetsToBuild : (($targetsToBuild ne "*") ? [split (/, ?/, $targetsToBuild)] : [sort keys (%$targets)]);
for my $target (@$targetsToBuild) {
    traverseDependencies ($target);
}

# now walk the targets in dependency order
for my $target (@$targetsInOrder) {
    push (@$buildConfigurationStack, $targets->{$target});
    my $targetPath = getConfigurationVariable("targetPath");

    # determine what configurations to build
    my $configurations = getConfigurationVariable("configurations");
    my $configurationToBuild = getConfigurationVariable("configuration");
    $configurationToBuild = (ref $configurationToBuild eq "ARRAY") ? $configurationToBuild : (($configurationToBuild ne "*") ? [ split(/, ?/, $configurationToBuild) ] : [ sort keys (%$configurations) ]);
    for my $configuration (@$configurationToBuild) {
        print STDERR "BUILD $target/$configuration\n";
        my $objectPath = "$targetPath/$target/$configuration/objects";
        make_path ($objectPath);

        ### XXX TODO
    }

    pop (@$buildConfigurationStack);
}
