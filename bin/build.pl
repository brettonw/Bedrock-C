#! /usr/bin/env perl

use strict;
use warnings FATAL => 'all';
use diagnostics;

binmode STDOUT, ":utf8";
binmode STDERR, ":utf8";
use utf8;

use File::Path qw(make_path);
use File::Basename;
use Cwd qw(abs_path);

# search my script directory location for the "my" lib
use lib dirname (abs_path(__FILE__));
use my::JsonFile;

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
sub conf {
    return getConfigurationVariable (@_);
}

# process the command line options into a configuration hash in the configuration stack
my $commandLineBuildConfiguration = {};
foreach my $argument (@ARGV) {
    if ($argument =~ /^([^=]*)=([^=]*)$/) {
        my $key = $1;
        my $value = $2;
        if (conf($1) ne "") {
            $commandLineBuildConfiguration->{$1} = $2;
            print STDERR "$1 = $2\n";
        } else {
            print STDERR "Ignoring unknown build configuration variable: $key\n";
        }
    }
}
push (@$buildConfigurationStack, $commandLineBuildConfiguration);

# now load the project local build configuration file into the configuration stack
push (@$buildConfigurationStack, JsonFile::read(conf ("buildConfigurationFileName")));

# read the source path looking for subdirs, and loading their build configurations
my $targets = {};
my $sourcePath = conf ("sourcePath");
if (opendir(SOURCE_PATH, $sourcePath)) {
    while (my $file = readdir(SOURCE_PATH)) {
        next unless (($file !~ /^\./) && (-d "$sourcePath/$file"));
        $targets->{$file} = JsonFile::read("$sourcePath/$file/" . conf("buildConfigurationFileName"));
    }
    closedir(SOURCE_PATH);
} else {
    print STDERR "Can't open source directory ($sourcePath), $!\n";
}

# identify the targets to build, and the dependency order to do it. the dependency graph is implicit
# in the dependencies array for each target, so we traverse it in depth first order, emitting build
# targets on return (marking them as visited).
my $targetsInDependencyOrder = [];
sub traverseTargetDependencies {
    my ($target) = @_;
    if (exists ($targets->{$target})) {
        if (!exists($targets->{$target}->{touched})) {
            $targets->{$target}->{touched} = 1;
            my $dependencies = exists($targets->{$target}->{dependencies}) ? $targets->{$target}->{dependencies} : [];
            for my $dependency (sort (@$dependencies)) {
                traverseTargetDependencies($dependency);
            }
            push (@$targetsInDependencyOrder, $target);
        }
    } else {
        print STDERR "Unknown target: $target\n";
    }
}

my $targetsToBuild = conf ("target");
$targetsToBuild = (ref $targetsToBuild eq "ARRAY") ? $targetsToBuild : (($targetsToBuild ne "*") ? [split (/, ?/, $targetsToBuild)] : [sort keys (%$targets)]);
for my $target (@$targetsToBuild) {
    traverseTargetDependencies ($target);
}

# function to read a dependencies file
sub readObjectDependencies {
    my ($sourceTargetPath, $sourceTargetFile, $objectPath) = @_;
    my $sourceExtension = conf ("sourceExtension");
    my $dependencyExtension = conf ("dependencyExtension");
    my $dependencyFile = $sourceTargetFile;
    $dependencyFile =~ s/$sourceExtension$/$dependencyExtension/;
    if (open (DEPENDENCY_FILE, "$objectPath/$dependencyFile")) {
        my $dependencyLine = "";
        while (<DEPENDENCY_FILE>) {
            chomp;
            $dependencyLine .= $_;
            last if ($dependencyLine !~ /\\$/);
        }
        close (DEPENDENCY_FILE);
        
        # process the dependency line into an array of dependencies
        return [split (/ /, $dependencyLine)];
    }
    return [$sourceTargetFile];
}

# now walk the targets in dependency order
for my $target (@$targetsInDependencyOrder) {
    push (@$buildConfigurationStack, $targets->{$target});
    my $targetPath = conf ("targetPath");

    # determine what configurations to build
    my $configurations = conf ("configurations");
    my $configurationToBuild = conf ("configuration");
    $configurationToBuild = (ref $configurationToBuild eq "ARRAY") ? $configurationToBuild : (($configurationToBuild ne "*") ? [ split(/, ?/, $configurationToBuild) ] : [ sort keys (%$configurations) ]);
    for my $configuration (@$configurationToBuild) {
        print STDERR "BUILD $target/$configuration\n";
        my $objectPath = "$targetPath/$target/$configuration/objects";
    	make_path ($objectPath);
       	
       	# gather up all the source files in the source path
       	my $sourceTargetPath = "$sourcePath/$target";
       	my $sourceExtension = conf ("sourceExtension");
       	my $sourceTargetFiles = {};
        if (opendir(SOURCE_TARGET_DIR, $sourceTargetPath)) {
            while (my $sourceTargetFile = readdir(SOURCE_TARGET_DIR)) {
                if ($sourceTargetFile =~ /$sourceExtension$/) {
                    $sourceTargetFiles->{$sourceTargetFile} = readObjectDependencies ($sourceTargetPath, $sourceTargetFile, $objectPath);
                    print STDERR "    Source: $sourceTargetFile\n";
                }
            }
            closedir(SOURCE_TARGET_DIR);
        } else {
            print STDERR "Can't open target source directory ($sourceTargetPath), $!\n";
        }
       	
        # now the sourceTargetFiles have a dependency line, we only need to compare the dates of 
        # those files to the date of the object file, if it exists.
        for my $sourceTargetFile (sort keys %$sourceTargetFiles) {
            my $dependsGenerator = conf ("compiler") . " " . conf ("compilerOptions") . " " . conf ("configurations")->{$configuration}->{dependerOptions} . " $sourceTargetPath/$sourceTargetFile > $objectPath/$sourceTargetFile";
            print STDERR "$dependsGenerator\n"; 
        }
        ### XXX TODO
    }

    pop (@$buildConfigurationStack);
}
