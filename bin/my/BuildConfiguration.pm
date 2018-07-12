use strict;
use warnings FATAL => 'all';

# need JsonFile - the search lib path has already been adjusted by the calling perl script, so we
# don't need to do that again here.
use JsonFile;

package BuildConfiguration;

# export the 'conf' function so it can be used without qualification
use Exporter 'import';
our @EXPORT_OK = qw(conf);

my %buildConfigurations;

sub add {
    my ($name, $buildConfiguration) = @_;
    $buildConfigurations{$name} = $buildConfiguration;
}

sub get {
    my ($name) = @_;
    return $buildConfigurations{$name} || {};
}

# given a value and a context, reduce the value as much as possible by performing replacements
# XXX TODO - this needs to be a bit more fantastical
sub reduceValue {
    my ($value, $context) = @_;

    # do variable substitutions, .
    my $replacementCount;
    do {
        $replacementCount = 0;
        # this list is reverse sorted so that variable names that are substrings of longer variable
        # names will be replaced after the longer variable names are replaced, hopefully this isn't
        # just too clever by half.
        my @variableNames = reverse sort $value =~ /\$([a-z][A-Za-z0-9]*)/g;
        for my $variableName (@variableNames) {
            if ((exists($context->{$variableName})) && ($context->{$variableName} ne "*")) {
                $value =~ s/\$$variableName/$context->{$variableName}/ge;
                ++$replacementCount;
            }
        }
    } while ($replacementCount > 0);

    # do a substitution with a shell command if one is requested, until no more happen
    while ($value =~ s/\$\(([^\)]*)\)/my $x = qx%$1%; chomp $x; $x/e) {}

    # return the reduced value
    return $value;
}

# given a left context and a right context, create a new context that "applies" left to right,
# where right contains variable names needing substitution, and left possibly supplies values.
sub apply {
    my ($left, $right) = @_;

    my $new = {};
    for my $key (keys (%$right)) {
        # allow for "reduce" to result in undef
        #print STDERR "    RIGHT($key) = $right->{$key}\n";
        my $value = reduceValue ($right->{$key}, $left);
        if (defined $value) {
            #print STDERR "        NEW($key) = $value\n";
            $new->{$key} = $value;
        }
    }
    for my $key (keys (%$left)) {
        if (! exists ($new->{$key})) {
            #print STDERR "    LEFT($key) = $left->{$key}\n";
            my $value = reduceValue($left->{$key}, $left);
            if (defined $value) {
                #print STDERR "        NEW($key) = $value\n";
                $new->{$key} = $value;
            }
        }
    }
    return $new;
}

sub concatenate {
    my ($name, $leftName, $buildConfiguration) = @_;
    print STDERR "CONCATENATE ($leftName with XXX into $name)\n";

    # we assume the left build configuration has already been resolved as fully as possible.
    $buildConfigurations{$name} = apply (get ($leftName), $buildConfiguration);
}

sub copy {
    my ($buildConfiguration) = @_;
    my $new = {};
    for my $key (keys (%$buildConfiguration)) {
        $new->{$key} = $buildConfiguration->{$key};
    }
    return $new;
}

# reduce all the ariables within a build configuration using itself as the context
# this should be the final step in using a build context
sub reduce {
    my ($buildConfiguration) = @_;
    my $new = copy ($buildConfiguration);
    for my $key (keys (%$new)) {
        # allow for "reduce" to result in undef
        print STDERR "    VALUE($key) = $new->{$key}\n";
        my $value = reduceValue ($new->{$key}, $new);
        if ($value ne $new->{$key}) {
            print STDERR "        NEW($key) = $value\n";
            $new->{$key} = $value;
        }
    }
    return $new;
}

# load a build configuration file
sub load {
    return JsonFile::read(@_);
}

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
