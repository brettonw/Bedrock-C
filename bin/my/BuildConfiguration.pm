use strict;
use warnings FATAL => 'all';

# need JsonFile - the search lib path has already been adjusted by the calling perl script, so we
# don't need to do that again here.
use JsonFile;

package BuildConfiguration;

# export the 'conf' function so it can be used without qualification
use Exporter 'import';
our @EXPORT_OK = qw(conf);

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
#   variables:
#           configuration variables are mostly defined statically, but can be specified to be
#           interpreted by a shell or by variable replacement, by wrapping the value with "$()" or
#           prepending with "$", respectively.

# establish the build configuration stack
my @buildConfigurationStack;

# function to traverse the configuration stack from the root to the tail to retrieve a defined value
# for the requested variable name - note this differs from most stack-based dictionary schemes in
# programming languages like PostScript or Javascript in that it evaluates all the dictionaries,
# rather than stopping after the first definition. this allows us to override and extend the
# definitions given at higher levels.
sub conf {
    my ($variableName) = @_;

    # loop over the build configuration stack
    my $value = undef;
    for my $buildConfiguration (@buildConfigurationStack) {
        if (exists ($buildConfiguration->{$variableName})) {
            my $existingValue = $value;
            $value = $buildConfiguration->{$variableName};
            if (ref($value) ne "HASH") {
                # do variable substitution if the existing value should be used in the replacement
                $value =~ s/\$$variableName/$existingValue/g;
                
                # do variable substitutions using any variable name already defined at the root

                # do a substitution with a shell command if one is requested
                $value =~ s/\$\(([^\)]*)\)/my $x = qx%$1%; chomp $x; $x/e;
            }
        }
    }
    return $value;
}

# function to push a new configuration on the stack and return it (for chaining)
sub begin {
    my ($buildConfiguration) = @_;
    if ($buildConfiguration) {
        push (@buildConfigurationStack, $buildConfiguration);
    }
    return $buildConfiguration;
}

# function to read a build configuration file in the specified directory - if the 
# buildConfigurationName is not already defined, it will default to "build.json".
sub read {
    my ($buildConfigurationDirectory) = @_;
    my $buildConfigurationFileName = conf ("buildConfigurationFileName") || "build.json";
    my $buildConfiguration = JsonFile::read("$buildConfigurationDirectory/$buildConfigurationFileName");
    return $buildConfiguration;
}

# function to read a build configuration file in the specified directory and push it on the 
# configuration stack, returns the read configuration;
sub enter {
return begin (&read (@_));
}

# function to pop a configuration off the stack
sub end {
    pop (@buildConfigurationStack);
}

1;
