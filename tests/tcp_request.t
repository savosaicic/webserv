#!/usr/bin/env perl

use strict;
use IO::Socket;
use Test::More tests => 2;
use lib ("./lib");
use WebservTest;

my $tf = WebservTest->new();
my $t;

ok($tf->start_proc == 0, "Starting webserv") or die();

ok($tf->handle_tcp_connection() == 0, 'Valid TCP connection') or die();

#$t->{REQUEST}  = "Bonjour Monde";
#$t->{RESPONSE}  = "Bonjour Monde\r\n\r\n";
#ok($tf->handle_tcp_echo($t) == 0, 'Valid echo request') or die();
