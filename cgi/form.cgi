#!/usr/local/bin/perl
#
#     INPUT data
$in_string = <STDIN>;
#
#     DEPLUS $in_string
$in_string =~ tr/\+/ /s; # translate and squeeze multiple spaces
#
#     TOKENIZE attr/val strings
@attr_val_strings = split (/&/, $in_string);
#
#     SPLIT attr/val strings and put into assoc. array
foreach $out_str (@attr_val_strings) {
@pair = split (/=/, $out_str);
$attr_value{$pair[0]} = $pair[1];
  }
#
#     DECODE special characters
foreach $key (keys %attr_value) {
  $key =~ s/%(..)/pack("c",hex($1))/ge;
  $attr_value{$key} =~ s/%(..)/pack("c",hex($1))/ge;
  }

#     OUTPUT section

#     generate header lines
print "200 ok\n";
print "content-type: text/html\n\n";

#     GENERATE report, in HTML
print "<HTML>\n";

print "<HEAD>\n";
print "<TITLE>stdin vars.</TITLE>\n";
print "<H1>Print CGI STDIN Variables</H1>\n";
print "</HEAD>\n";

print "<BODY>\n";
print "<HR>\n";
print "<H3>STDIN Variables</H3>\n";
print "<UL>\n";
foreach $key (keys %attr_value) {
  print "<LI>$key = $attr_value{$key}\n";
  }
print "</UL>\n";
print "</BODY>\n";

print "</HTML>\n";
