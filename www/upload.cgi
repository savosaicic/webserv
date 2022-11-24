#!/usr/bin/perl

use strict;
use warnings;
use CGI;

my $cgi = new CGI;
my $dir = $ENV{DOCUMENT_ROOT};
my $file = $cgi->param('file');

open(LOCAL, ">$dir/$file") or print 'error';
my $file_handle = $cgi->upload('file');
while(<$file_handle>) {
	print LOCAL $_;
}
close($file_handle);
close(LOCAL); 
print $cgi->header();
print "$file has been successfully uploaded\n";
