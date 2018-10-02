#! /usr/bin/env perl

use strict;
use warnings;

# function to load a csv format file
sub readHtmlFile {
    my ($htmlFileName) = @_;

    my $headers = [];
    my $data = [];
    if (open (my $htmlFile, "<:encoding(UTF-8)", $htmlFileName)) {
        my $rowData = {};
        my $gotHeaders = 0;
        while (my $row = <$htmlFile>) {
            chomp $row;
            $row =~ s/^\s*//g;
            $row =~ s/\s*$//g;
            $row =~ s/[\n\r]//g;
            #print STDERR "$row\n";
            if (length ($row) > 0) {
                #print STDERR "ROW: $row\n";
                
                # the format of these files is html, one entry per line, except where it's not
                if ($gotHeaders == 0) {
                    if ($row =~ /<th>([^<]*)<\/th>/i) {
                        #print STDERR "HEADER: $1\n";
                        push (@$headers, $1);
                    } elsif ($row =~ /^<\/thead>/i) {
                        if (scalar (@$headers) > 1) {
                            #print STDERR "HEADERS: @$headers\n";
                            $gotHeaders = 1;
                        } else {
                            print STDERR "NO HEADERS\n";
                        }
                    }
                } else {
                    if ($row =~ /<tr>/i) {
                        $rowData = {};
                        #print STDERR "Begin row\n";
                    } elsif ($row =~ /^<\/tr>/i) {
                        # complete the data row by saving it
                        push (@$data, $rowData);
                        #print STDERR "End row\n";
                    } elsif ($row =~ /<td[^>]*>(.*)<\/td>/i) {
                        # it's a compelte row data entry
                        my $headerName = $headers->[scalar(keys %$rowData)];
                        #print STDERR "    $headerName = $1\n";
                        $rowData->{$headerName} = $1;                        
                    } elsif ($row =~ /<td[^>]*>(.*)/i) {
                        # it's the beginning of an incomplete row data entry
                        my $headerName = $headers->[scalar(keys %$rowData)];
                        #print STDERR "    $headerName = $1\n";
                        $rowData->{$headerName} = $1;                        
                    } elsif ($row =~ /(.*)<\/td>/i) {
                        # it's the end of an incomplete row data entry
                        my $headerName = $headers->[scalar(keys %$rowData) - 1];
                        $rowData->{$headerName} .= " $1";                        
                        print STDERR "    $headerName = " . $rowData->{$headerName} . "\n";
                    } else {
                        # it's the continuation of an incomplete row data entry
                        my $headerName = $headers->[scalar(keys %$rowData) - 1];
                        $rowData->{$headerName} .= " $row";                        
                        #print STDERR "    $headerName = " . $rowData->{$headerName} . "\n";
                    }
                }
            }
        }
        close $htmlFile;
    } else {
        print STDERR "Can't open $htmlFileName ($!)\n";
    }
    return {"headers" => $headers, "data" => $data};
}

print STDERR "$ARGV[0]\n";
my $data = readHtmlFile ($ARGV[0])->{data};

my $outputFileName = $ARGV[0];
$outputFileName =~ s/\.html$/.json/i;
if (open (my $outputFile, ">:encoding(UTF-8)", $outputFileName)) {
    my $rowSeparator = "[\n";
    for my $row (@$data) {
        print $outputFile $rowSeparator;
        $rowSeparator = ",\n";
        
        my $separator = "    {";
        for my $key (keys (%$row)) {
            my $value = exists ($row->{$key}) ? $row->{$key} : "";
            print $outputFile "$separator\"$key\":\"$value\"";
            $separator = ",";
        }
        print $outputFile "}";
    }
    print $outputFile "\n]\n";
    close $outputFile;
}



