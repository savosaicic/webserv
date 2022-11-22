#!/usr/bin/perl

print "content-type: text/html\n\n";
print "<html><head><title>exemple en perl</title></head>\n";
print "<body>\n";
foreach my $line ( <STDIN> ) {
    chomp( $line );
    print "$line\n";
}
print "les cgi s'ex�cutent bien sur votre serveur !\n";
print "</body></html>\n";
