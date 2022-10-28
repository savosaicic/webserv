#!/usr/bin/env perl

use strict;
use IO::Socket;
use Test::More tests => 3;
use lib ("./lib");
use WebservTest;

my $tf = WebservTest->new();
my $t;

$tf->{PORT} = 8080;
ok($tf->start_proc == 0, "Starting webserv") or die();

#ok($tf->handle_tcp_connection() == 0, 'Valid TCP connection') or die();
my @ports = (9080..9085);
ok($tf->multiple_tcp_connection(@ports) == 0, 'Valid mutiple TCP connection');

#$t->{REQUEST}  = "Bonjour Monde";
#$t->{RESPONSE}  = "Bonjour Monde\r\n\r\n";
#ok($tf->handle_tcp_echo($t) == 0, 'Valid echo request') or die();

ok($tf->stop_proc == 0, "Stoping webserv") or die();
