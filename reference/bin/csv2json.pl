#! /usr/bin/env perl

use strict;
use warnings;

# function to load a csv format file
sub readCsvFile {
    my ($csvFileName) = @_;

    my $headers = [];
    my $data = [];
    if (open (my $csvFile, "<:encoding(UTF-8)", $csvFileName)) {
        my $gotHeaders = 0;
        while (my $row = <$csvFile>) {
            chomp $row;
            $row =~ s/[\n\r]//g;
            if ($row !~ /"/) {
                #print STDERR "$row\n";
                if (length ($row) > 0) {
                    #print STDERR "ROW: $row\n";
                    
                    # the format of these files is that the first two lines are the 
                    # filename and the header row, respectively
                    if ($gotHeaders == 0) {
                        if ($row =~ /^#?(.*)/) {
                            @{$headers} = split (/,\s*/, $1);
                            if (scalar (@$headers) > 1) {
                                #print STDERR "HEADERS: @$headers\n";
                                $gotHeaders = 1;
                            } else {
                                print STDERR "SKIPPING LINE WITH NO HEADERS\n";
                            }
                        }
                    } else {
                        # this is a data row - the approach could be more efficient, but for 
                        # expedience I'm just going to iterate over the header array and 
                        # store each element as a hash in the rows array
                        my @rowEntries = split (/,\s*/, $row);
                        my $rowData = {};
                        for (my $i = 0; $i < scalar (@$headers); ++$i) {
                            if ($i < scalar (@rowEntries)) {
                                if (length($rowEntries[$i]) > 0) {
                                    $rowData->{$headers->[$i]} = $rowEntries[$i];
                                    #print STDERR "    STORE - " . $headers->[$i] . ": " . $rowEntries[$i] . "\n";
                                }
                            }
                        }
                        if (scalar (keys %$rowData) > 0) {
                            push (@$data, $rowData);
                        } else {
                            print STDERR "Empty row for ($row)\n";
                        }
                    }
                }
            } else {
                print STDERR "Skipping row with quotes...\n    $row\n";
            }
        }
        close $csvFile;
    } else {
        print STDERR "Can't open $csvFileName ($!)\n";
    }
    return {"headers" => $headers, "data" => $data};
}

for my $arg (@ARGV) {
    print STDERR "$arg\n";
    my $data = readCsvFile ($arg)->{data};
    
    my $outputFileName = $arg;
    $outputFileName =~ s/\.csv$/.json/i;
    if (open (my $outputFile, ">:encoding(UTF-8)", $outputFileName)) {
        my $rowSeparator = "[\n";
        for my $row (@$data) {
            if (scalar keys (%$row) > 0) {
            print $outputFile $rowSeparator;
            $rowSeparator = ",\n";
            
            my $separator = "    {";
            for my $key (keys (%$row)) {
                my $value = exists ($row->{$key}) ? $row->{$key} : "";
                print $outputFile "$separator\"$key\":\"$value\"";
                $separator = ",";
            }
            print $outputFile "}";
            } else {
                print STDERR "EMPTY ROW\n";
            }
        }
        print $outputFile "\n]\n";
        close $outputFile;
    }
}

