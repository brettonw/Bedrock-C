use warnings;
use strict;

package Web;

# set up the perl WWW mechanize
use WWW::Mechanize;
use WWW::Mechanize::TreeBuilder;

my $web = WWW::Mechanize->new();
WWW::Mechanize::TreeBuilder->meta->apply($web);

sub fetch {
    my ($url, $filename) = @_;

    # try to load a cached file
    if (-e $filename) {
        #print STDERR "load saved page from $filename\n";
        eval {$web->get("file://$filename");};
        #print STDERR "    done.\n";
        if ($web->res->is_success) {
            # check that it's a valid HTML page
            my @tags = $web->root()->look_down(_tag => "body");
            if ((scalar(@tags) == 1) && ($tags[0]->tag() eq "body")) {
                print STDERR "load from cache ($url) <= $filename\n";
                return 1;
            } else {
                print STDERR "bad cache entry ($filename)\n";
                unlink($filename);
                eval {$web->back();};
            }
        }
    }

    print STDERR "fetch ($url)";
    eval {$web->get($url);};
    if ($web->res->is_success && (open my $file, '>:encoding(UTF-8)', $filename)) {
        #print STDERR "    save page to $filename\n";
        print $file $web->content;
        close $file;
        #print STDERR "    done.\n";
        print STDERR " => $filename\n";
        return 1;
    }

    print STDERR "\n    FAILED ($!)\n";
    return 0;
}

sub getImage {
    my ($url, $filename) = @_;
    if (! -e $filename) {
        print STDERR "fetch image ($url) => $filename\n";
        eval {
            $web->get($url, ":content_file" => $filename);
            $web->back();
        };
        #print STDERR "    done.\n";
    }
    return(-e $filename);
}

sub find {
    my ($tag, $matchParams) = @_;
    my $result = [];
    #print STDERR "mechFind (" . $mech->base () . ") " . hashToString ($matchParams) . "\n";
    my @tags = $web->root()->look_down(_tag => $tag);
    for my $tag (@tags) {
        #print STDERR $tag->as_text () . "\n\n";
        # loop over all the keys in the matchParams
        my $matchCount = 0;
        my %attr = $tag->all_external_attr();
        #print STDERR "Attr: " . hashToString(\%attr) . "\n\n";
        for my $matchParam (keys %$matchParams) {
            #print STDERR "MATCH KEY: $matchParam\n";
            if (exists($attr{$matchParam}) && ($attr{$matchParam} =~ /$matchParams->{$matchParam}/)) {
                # it matches
                #print STDERR $attr{$matchParam} . " MATCHES /" . $matchParams->{$matchParam} . "/\n";
                $matchCount++;
            }
            else {
                #print STDERR $attr{$matchParam} . " DOES NOT MATCH /" . $matchParams->{$matchParam} . "/\n";
            }
        }
        if ($matchCount == scalar(%$matchParams)) {
            #print STDERR "Matches: " . hashToString (\%attr) . "\n";
            push(@$result, $tag);
        }
    }
    return $result;
}

sub findLinks {
    my ($matchParams) = @_;
    my $links = find("a", $matchParams);
    my $result = [];
    for my $link (@$links) {
        push(@$result, {
            url  => $link->attr("href"),
            text => $link->as_text()
        });
    }
    return $result;
}

sub get {
    return $web;
}

sub qualifyUrl {
    my ($url, $base) = @_;
    if ($url =~ /^http/) {
        return $url;
    }
    $base =~ s/\/$//;
    $url =~ s/^\///;
    return "$base/$url";
}

sub qualifyLinkUrls {
    my ($links, $base) = @_;
    for my $link (@$links) {
    	$link->{url} = qualifyUrl ($link->{url}, $base);
    }
    return $links;
}

sub goBack {
    eval {$web->back();};
}


1;
