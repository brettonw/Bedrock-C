#! /usr/bin/env perl

use strict;
use warnings FATAL => 'all';
#use diagnostics;

# utf8 everything...
binmode STDOUT, ":utf8";
binmode STDERR, ":utf8";
use utf8;

use File::Path qw(make_path);
use File::Basename;
use Cwd qw(abs_path);

# search under the script directory location for the "my" libs
use lib dirname (abs_path(__FILE__)) . "/my";
use Context qw(conf %ContextType);
use Slurp qw(slurp);

# root configs
Context::load ("root", dirname(abs_path(__FILE__)));
#Context::displayTypeNamed("root", $ContextType{VALUES});
#Context::displayTypeNamed("root", $ContextType{CONFIGURATIONS});
#Context::displayTypeNamed("root", $ContextType{TYPES});

# project configs
Context::load ("project", ".");
#Context::displayTypeNamed("project", $ContextType{VALUES});
#Context::displayTypeNamed("project", $ContextType{CONFIGURATIONS});
#Context::displayTypeNamed("project", $ContextType{TYPES});

# process the command line options into a values configuration and store it
my $commandLineContext = {};
foreach my $argument (@ARGV) {
    if ($argument =~ /^([^=]*)=([^=]*)$/) {
        my $key = $1;
        my $value = $2;
        if (conf($1) ne "") {
            $commandLineContext->{$1} = $2;
            print STDERR "$1 = $2\n";
        } else {
            print STDERR "Ignoring unknown build configuration variable: $key\n";
        }
    }
}
Context::addTypeNamed("commandline", $ContextType{VALUES}, $commandLineContext);
#Context::displayTypeNamed("commandline", $ContextType{VALUES});

# finalize project-values, project-types, and project-configurations
Context::addTypeNamed("project", $ContextType{VALUES}, Context::concatenateNamed ("root-" . $ContextType{VALUES}, "project-" . $ContextType{VALUES}, "commandline-" . $ContextType{VALUES}));
Context::displayTypeNamed("project", $ContextType{VALUES});
#Context::addTypeNamed("project", $ContextType{CONFIGURATIONS}, Context::concatenateNamed ("root-$ContextType{CONFIGURATIONS}", "project-$ContextType{CONFIGURATIONS}"));
#Context::displayTypeNamed("project", $ContextType{CONFIGURATIONS});
#Context::addTypeNamed("project", $ContextType{TYPES}, Context::concatenateNamed ("root-$ContextType{TYPES}", "project-$ContextType{TYPES}"));
#Context::displayTypeNamed("project", $ContextType{TYPES});

#---------------------------------------------------------------------------------------------------
# read the source path looking for subdirs, and loading their values context
my $targets = {};
my $targetPrefix = "#";
my $sourcePath = Context::confType ("project", $ContextType{VALUES}, "sourcePath");
if (opendir(SOURCE_PATH, $sourcePath)) {
    while (my $target = readdir(SOURCE_PATH)) {
        next unless (($target !~ /^\./) && (-d "$sourcePath/$target"));
        Context::load ("$targetPrefix$target", "$sourcePath/$target/");
        my $targetContext = Context::concatenateNamed ("project-" . $ContextType{VALUES}, "$targetPrefix$target-" . $ContextType{VALUES});
        $targetContext->{target} = $target;

        # XXX TODO need to concatenate configurations, then types
        #Context::concatenate (
        #    Context::getTypeNamed("root", $ContextType{TYPES})->{$targetContext->{type}},
        #    Context::getTypeNamed("root", $ContextType{TYPES})->{$targetContext->{type}},
        #    Context::getTypeNamed("target-$target", $ContextType{TYPES})->{$targetContext->{type}}
        #);

        $targets->{$target} = $targetContext;

        Context::display($targetContext);
    }
    closedir(SOURCE_PATH);
} else {
    print STDERR "Can't open source directory ($sourcePath), $!\n";
}

#---------------------------------------------------------------------------------------------------
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

my $targetsToBuild = Context::confType ("project", $ContextType{VALUES}, "target");
$targetsToBuild = (ref $targetsToBuild eq "ARRAY") ? $targetsToBuild : (($targetsToBuild ne "*") ? [split (/, ?/, $targetsToBuild)] : [sort keys (%$targets)]);
for my $target (@$targetsToBuild) {
    traverseTargetDependencies ($target);
}

for my $target (@$targetsInDependencyOrder) {
    print STDERR "TARGET: $target\n";
}
exit (0);
#---------------------------------------------------------------------------------------------------
sub outputFileName {
    my ($sourceTargetFile, $outputExtensionName) = @_;
    my $sourceExtension = conf ("sourceExtension");
    my $outputExtension = conf ($outputExtensionName);
    return $sourceTargetFile =~ s/$sourceExtension$/$outputExtension/r;
}

sub targetPath {
    my ($target, $configuration) = @_;
    return conf ("targetPath") . "/$target/$configuration";
}

sub objectPath {
    my ($target, $configuration) = @_;
    return targetPath ($target, $configuration) . "/" . conf ("objectsPath");
}

#---------------------------------------------------------------------------------------------------
# functions to deal with object dependencies
sub readObjectDependencies {
    my ($target, $configuration, $sourceTargetFile) = @_;
    my $dependencyFile = objectPath ($target, $configuration) . "/" . outputFileName ($sourceTargetFile, "dependencyExtension");
    my $dependencies = slurp ($dependencyFile) || "$sourcePath/$target/$sourceTargetFile";
    $dependencies = ((($dependencies =~ s/\\//gr) =~ s/\s+/ /gr) =~ s/.*: +//gr);
    #print STDERR "        DEPENDENCIES: $dependencies\n";
    return [split (/ /, $dependencies)];
}

sub checkObjectDependencies {
    my ($target, $configuration, $sourceTargetFile) = @_;

    # if the object file exists, compare its age to the dependencies
    my $objectFile = objectPath ($target, $configuration) . "/" . outputFileName ($sourceTargetFile, "objectExtension");
    if (-e $objectFile) {
        my $objectAgeDelta = (-M $objectFile);
        my $dependencies = readObjectDependencies ($target, $configuration, $sourceTargetFile);
        for my $dependency (@$dependencies) {
            if ((-M $dependency) < $objectAgeDelta) {
                # this dependency is younger than the object file, so the file should be rebuilt
                return 1;
            }
        }
        # none of the dependencies is younger than this file, so it does not need to be rebuilt
        return 0;
    } else {
        # the object doesn't exist, it should be built
        return 1;
    }
}

sub writeObjectDependencies {
    my ($target, $configuration, $sourceTargetFile, $includes) = @_;
    my $dependencyFile = objectPath ($target, $configuration) . "/" . outputFileName ($sourceTargetFile, "dependencyExtension");
    my $dependsGenerator = conf ("compiler") . " $includes " . conf ("configurations")->{$configuration}->{compilerOptions} . " " . conf ("configurations")->{$configuration}->{dependerOptions} . " $sourcePath/$target/$sourceTargetFile > $dependencyFile";
    #print STDERR "$dependsGenerator\n";
    qx/$dependsGenerator/;
}

#---------------------------------------------------------------------------------------------------
sub compileObject {
    my ($target, $configuration, $sourceTargetFile, $includes) = @_;
    my $objectFile = objectPath ($target, $configuration) . "/" . outputFileName ($sourceTargetFile, "objectExtension");
    my $objectGenerator = conf ("compiler") . " $includes " . conf ("configurations")->{$configuration}->{compilerOptions} . " -c $sourcePath/$target/$sourceTargetFile  -o $objectFile";
    print STDERR "    COMPILE $sourceTargetFile: $objectGenerator\n";
    qx/$objectGenerator/;
}

#---------------------------------------------------------------------------------------------------
# now walk the targets in dependency order
for my $target (@$targetsInDependencyOrder) {
    # setup the build configuration for this target (we loaded this in a previous step)
    Context::begin ($targets->{$target});

    # determine what configurations to build
    my $configurations = conf ("configurations");
    my $configurationToBuild = conf ("configuration");
    $configurationToBuild = (ref $configurationToBuild eq "ARRAY") ? $configurationToBuild : (($configurationToBuild ne "*") ? [ split(/, ?/, $configurationToBuild) ] : [ sort keys (%$configurations) ]);
    for my $configuration (@$configurationToBuild) {
        print STDERR "BUILD $target/$configuration\n";

        # ensure the target directory is present
    	make_path (objectPath ($target, $configuration));
       	
       	# gather up all the source files in the source path, and check to see if they need to be
        # compiled by comparing the modification dates of the dependencies
       	my $sourceTargetPath = "$sourcePath/$target";
       	my $sourceExtension = conf ("sourceExtension");
       	my @sourceTargetFiles;
        if (opendir(SOURCE_TARGET_DIR, $sourceTargetPath)) {
            while (my $sourceTargetFile = readdir(SOURCE_TARGET_DIR)) {
                if ($sourceTargetFile =~ /$sourceExtension$/) {
                    #print STDERR "    Source: $sourceTargetFile\n";
                    if (checkObjectDependencies ($target, $configuration, $sourceTargetFile)) {
                        push (@sourceTargetFiles, $sourceTargetFile);
                    }
                }
            }
            closedir(SOURCE_TARGET_DIR);
        } else {
            print STDERR "Can't open target source directory ($sourceTargetPath), $!\n";
        }

        # build out the "includes" string to be passed with the compilation steps
        my $includes = "-I$sourcePath";
        my $dependencies = exists($targets->{$target}->{dependencies}) ? $targets->{$target}->{dependencies} : [];
        for my $dependency (@$dependencies) {
            $includes .= " -I$sourcePath/$dependency"
        }
        #print STDERR "    INCLUDES: $includes\n";
       	
        # for all the sources that are "dirty", we need to rewrite their dependencies and then
        # compile them into object files... In the future, do this with multiple threads...
        for my $sourceTargetFile (sort @sourceTargetFiles) {
            writeObjectDependencies ($target, $configuration, $sourceTargetFile, $includes);
            compileObject($target, $configuration, $sourceTargetFile, $includes);
        }

        # we want to link the targets according to their type, so do a little work to get ready to
        # do that
        my $targetFile;
        my $linkOptions = conf ("configurations")->{$configuration}->{linkerOptions};
        my $type = conf ("type");
        if ($type eq "application") {
            $targetFile = targetPath ($target, $configuration) . "/" . $target;
            $linkOptions .= " " .  conf ("configurations")->{$configuration}->{applicationOptions};
        } elsif ($type eq "library") {
            $targetFile = targetPath ($target, $configuration) . "/lib" . $target . conf("libraryExtension");
            $linkOptions .= " " .  conf ("configurations")->{$configuration}->{libraryOptions};
        } else {
            die "invalid target type";
        }

        # build out the "libraries" string to be passed with the linking step
        # -Lbin/shared -ltq84
        my $libraries = "";
        for my $dependency (@$dependencies) {
            $libraries .= " -L" . targetPath($dependency, $configuration) . " -l$dependency";
        }
        #print STDERR "    INCLUDES: $includes\n";

        # check to see if the final target needs to be built
        if ((! -e $targetFile) || (scalar (@sourceTargetFiles) > 0)) {
            my $targetGenerator = conf ("compiler") . " $linkOptions -o $targetFile $libraries " . objectPath ($target, $configuration) . "/*" . conf ("objectExtension");
            print STDERR "    LINK $target: $targetGenerator\n";
            qx/$targetGenerator/;
        }

        ### XXX TODO - gather the built files into a library or an application
    }

    Context::end ();
}
