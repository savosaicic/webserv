package WebservTest;

use strict;
use IO::Socket ();
use IO::Select;
use Test::More; # diag()
use Socket;
use threads;
use Cwd 'abs_path';

sub find_program {
	my @DEFAULT_PATHS = ('/usr/bin/', '/usr/local/bin/');
	my ($envname, $program) = @_;
	my $location;

	if (defined $ENV{$envname}) {
		$location = $ENV{$envname};
	} else {
		$location = `which "$program" 2>/dev/null`;
		chomp $location;
		if (! -x $location) {
			for my $path (@DEFAULT_PATHS) {
				$location = $path . $program;
				last if -x $location;
			}
		}
	}

	if (-x $location) {
		$ENV{$envname} = $location;
		return 1;
	} else {
		delete $ENV{$envname};
		return 0;
	}
}

BEGIN {
	our $HAVE_PERL = find_program('PERL', 'perl');
	if (!$HAVE_PERL) {
		die "Couldn't find path to perl, but it obviously seems to be running";
	}
}

sub mtime {
	my $file = shift;
	my @stat = stat $file;
	return @stat ? $stat[9] : 0;
}

sub new {
	my $class = shift;
	my $self = {};
	my $lpath;

	$lpath = (defined $ENV{'top_builddir'} ? $ENV{'top_builddir'} : '..');
	$self->{BASEDIR} = abs_path($lpath);
	if (exists $ENV{WEBSERV_EXE_PATH}) {
		$self->{WEBSERV_PATH} = $ENV{WEBSERV_EXE_PATH};
	}
	else {
		$self->{WEBSERV_PATH} = '/Users/savosaicic/webserv/webserv';
	}

	my ($name, $aliases, $addrtype, $net) = gethostbyaddr(inet_aton("127.0.0.1"), AF_INET);

	$self->{HOSTNAME} = $name;

	bless($self, $class);

	return $self;
}

sub listening_on {
	my $self = shift;
	my $port = shift;

	local $@;
	local $SIG{ALRM} = sub { };
    eval {
	local $SIG{ALRM} = sub { die 'alarm()'; };
	alarm(1);
	my $remote = IO::Socket::INET->new(
		Timeout  => 1,
		Proto    => "tcp",
		PeerAddr => "127.0.0.1",
		PeerPort => $port) || do { alarm(0); die 'socket()'; };

	close $remote;
	alarm(0);
    };
	alarm(0);
	return (defined($@) && $@ eq "");
}

sub stop_proc {
	my $self = shift;

	my $pid = $self->{WEBSERV_PID};
	if (defined $pid && $pid != -1) {
		kill('USR1', $pid) if (($ENV{"TRACEME"}||'') eq 'strace');
		kill('TERM', $pid) or return -1;
		return -1 if ($pid != waitpid($pid, 0));
	} else {
		diag("\nProcess not started, nothing to stop");
		return -1;
	}

	return 0;
}

sub wait_for_port_with_proc {
	my $self = shift;
	my $port = shift;
	my $child = shift;
	my $timeout = 10*100; # 10 secs (valgrind might take a while), select waits 0.01 s

	while (0 == $self->listening_on($port)) {
		select(undef, undef, undef, 0.01);
		$timeout--;

		# the process is gone, we failed
		require POSIX;
		if (0 != waitpid($child, POSIX::WNOHANG())) {
			return -1;
		}
		if (0 >= $timeout) {
			diag("\nTimeout while trying to connect; killing child");
			kill('TERM', $child);
			return -1;
		}
	}

	return 0;
}

sub start_proc {
	my $self = shift;
	# kill old proc if necessary
	#$self->stop_proc;

	$ENV{'SRCDIR'} = $self->{BASEDIR}.'/tests';
	$ENV{'PORT'} = 8080;
	$self->{PORT} = 8080;

	my @cmdline = ($self->{WEBSERV_PATH});

	if (defined $ENV{"TRACEME"} && $ENV{"TRACEME"} eq 'valgrind') {
		@cmdline = (qw(valgrind --tool=memcheck --track-origins=yes --show-reachable=yes --leak-check=yes --log-file=valgrind.%p), @cmdline);
	}

	diag("\nstarting webserv process at :".$self->{PORT}.", cmdline: @cmdline");
	my $child = fork();
	if (not defined $child) {
		diag("\nFork failed");
		return -1;
	}
	if ($child == 0) {
		# set up systemd socket activation env vars
		$ENV{LISTEN_FDS} = "1";
		$ENV{LISTEN_PID} = $$;
		if (defined($ENV{"TRACEME"}) && $ENV{"TRACEME"} ne "valgrind") {
			$ENV{LISTEN_PID} = "parent:$$"; # lighttpd extension
		}
		exec @cmdline or die($?);
	}

	if (0 != $self->wait_for_port_with_proc($self->{PORT}, $child)) {
		diag(sprintf("\nThe process %i is not up", $child));
		return -1;
	}

	$self->{WEBSERV_PID} = $child;

	0;
}

sub multiple_tcp_connection {
	my $self = shift;
  my @ports = @_;
	my $host = "127.0.0.1";

	my $is_debug = $ENV{"TRACE_HTTP"};

  for (@ports) {
    diag("\nlaunching threads with $_ port") if $is_debug;
    my $thr = threads->create(\&tcp_connection, $_);
    my $ReturnData = $thr->join();
    if ($ReturnData == -1) {
      return -1;
    }
  }
  return 0;
}

sub handle_tcp_echo {
	my $self = shift;
	my $t = shift;
	my $EOL = "\015\012";
	my $BLANK = $EOL x 2;
	my $host = "127.0.0.1";

	my @request = $t->{REQUEST};
	my @response = $t->{RESPONSE};
	my $slow = defined $t->{SLOWREQUEST};
	my $is_debug = $ENV{"TRACE_HTTP"};

	my $remote =
		IO::Socket::INET->new(
			Proto    => "tcp",
			PeerAddr => $host,
			PeerPort => $self->{PORT});

	if (not defined $remote) {
		diag("\nconnect failed: $!");
		return -1;
	}

	$remote->autoflush(1);

	if (!$slow) {
		diag("\nFIRST ONE") if $is_debug;
		diag("\nsending request header to ".$host.":".$self->{PORT}) if $is_debug;
		foreach(@request) {
			# pipeline requests
			s/\r//g;
			s/\n/$EOL/g;

			print $remote $_.$BLANK;
			diag("\n<< ".$_) if $is_debug;
		}
		# I've stopped writing data
		shutdown($remote, 1);
	} else {
		diag("\nSENDING request header to ".$host.":".$self->{PORT}) if $is_debug;
		foreach(@request) {
			# pipeline requests
			chomp;
			s/\r//g;
			s/\n/$EOL/g;

			print $remote $_;
			diag("<< ".$_."\n") if $is_debug;
			select(undef, undef, undef, 0.0001);
			print $remote "\015";
			select(undef, undef, undef, 0.0001);
			print $remote "\012";
			select(undef, undef, undef, 0.0001);
			print $remote "\015";
			select(undef, undef, undef, 0.0001);
			print $remote "\012";
			select(undef, undef, undef, 0.0001);
		}

	}
	diag("\n... done") if $is_debug;

	my $lines = "";

	diag("\nreceiving response") if $is_debug;
	# read everything
	while(<$remote>) {
		$lines .= $_;
		diag(">> ".$_) if $is_debug;
	}
	diag("\n... done") if $is_debug;

	close $remote;

	my $full_response = $lines;

	if ($full_response eq $response[0]) {
		return 0;
	}

	return -1;
}

sub tcp_connection {
	my $port = shift;
	my $host = "127.0.0.1";
	my $is_debug = $ENV{"TRACE_HTTP"};

	my $remote =
		IO::Socket::INET->new(
			Proto    => "tcp",
			PeerAddr => $host,
			PeerPort => $port);

	if (not defined $remote) {
		diag("\nconnect failed: $!");
		return -1;
	}
	$remote->autoflush(1);

	return 0;

#  my $select = IO::Select->new();
#  $select->add($remote);
#  my @ready = $select->can_read(2);
#  if (!@ready) {
#    $select->remove($remote);
#    close($remote);
#    return -1;
#  }
#  $select->remove($remote);
#	
#	my $buf;
#	my $bytes_read = $remote->sysread($buf, 8);
#	if (not defined $bytes_read) {
#    	diag("\nerror on the socket");
#	}
#	elsif ($bytes_read == 0) {
#		$remote->close();
#		return 0;
#	}
#	close($remote);
#	return -1;
}

sub handle_tcp_connection {
	
	my $self = shift;
	# my $t = shift;
	my $host = "127.0.0.1";

	my $is_debug = $ENV{"TRACE_HTTP"};

	my $remote =
		IO::Socket::INET->new(
			Proto    => "tcp",
			PeerAddr => $host,
			PeerPort => $self->{PORT});

	if (not defined $remote) {
		diag("\nconnect failed: $!");
		return -1;
	}
	$remote->autoflush(1);
	
	my $buf;
	my $bytes_read = $remote->sysread($buf, 8);
	if (not defined $bytes_read) {
    	diag("\nerror on the socket");
	}
	elsif ($bytes_read == 0) {
		$remote->close();
		return 0;
	}
	close($remote);
	return -1;
}

sub handle_http {
	my $self = shift;
	my $t = shift;
	my $EOL = "\015\012";
	my $BLANK = $EOL x 2;
	my $host = "127.0.0.1";

	my @request = $t->{REQUEST};
	my @response = $t->{RESPONSE};
	my $slow = defined $t->{SLOWREQUEST};
	my $is_debug = $ENV{"TRACE_HTTP"};

	my $remote =
		IO::Socket::INET->new(
			Proto    => "tcp",
			PeerAddr => $host,
			PeerPort => $self->{PORT});

	if (not defined $remote) {
		diag("\nconnect failed: $!");
		return -1;
	}

	$remote->autoflush(1);

	if (!$slow) {
		diag("\nFIRST ONE") if $is_debug;
		diag("\nsending request header to ".$host.":".$self->{PORT}) if $is_debug;
		foreach(@request) {
			# pipeline requests
			s/\r//g;
			s/\n/$EOL/g;

			print $remote $_.$BLANK;
			diag("\n<< ".$_) if $is_debug;
		}
		# I've stopped writing data
		shutdown($remote, 1);
	} else {
		diag("\nsending request header to ".$host.":".$self->{PORT}) if $is_debug;
		foreach(@request) {
			# pipeline requests
			chomp;
			s/\r//g;
			s/\n/$EOL/g;

			print $remote $_;
			diag("<< ".$_."\n") if $is_debug;
			select(undef, undef, undef, 0.0001);
			print $remote "\015";
			select(undef, undef, undef, 0.0001);
			print $remote "\012";
			select(undef, undef, undef, 0.0001);
			print $remote "\015";
			select(undef, undef, undef, 0.0001);
			print $remote "\012";
			select(undef, undef, undef, 0.0001);
		}

	}
	diag("\n... done") if $is_debug;

	my $lines = "";

	diag("\nreceiving response") if $is_debug;
	# read everything
	while(<$remote>) {
		$lines .= $_;
		diag(">> ".$_) if $is_debug;
	}
	diag("\n... done") if $is_debug;

	close $remote;

	my $full_response = $lines;

	my $href;
	foreach $href ( @{ $t->{RESPONSE} }) {
		# first line is always response header
		my %resp_hdr;
		my $resp_body;
		my $resp_line;
		my $conditions = $_;

		for (my $ln = 0; defined $lines; $ln++) {
			(my $line, $lines) = split($EOL, $lines, 2);

			# header finished
			last if(!defined $line or length($line) == 0);

			if ($ln == 0) {
				# response header
				$resp_line = $line;
			} else {
				# response vars

				if ($line =~ /^([^:]+):\s*(.+)$/) {
					(my $h = $1) =~ tr/[A-Z]/[a-z]/;

					if (defined $resp_hdr{$h}) {
#						diag(sprintf("\nheader '%s' is duplicated: '%s' and '%s'\n",
#						             $h, $resp_hdr{$h}, $2));
						$resp_hdr{$h} .= ', '.$2;
					} else {
						$resp_hdr{$h} = $2;
					}
				} else {
					diag(sprintf("\nunexpected line '%s'", $line));
					return -1;
				}
			}
		}

		if (not defined($resp_line)) {
			diag(sprintf("\nempty response"));
			return -1;
		}

		$t->{etag} = $resp_hdr{'etag'};
		$t->{date} = $resp_hdr{'date'};

		# check length
		if (defined $resp_hdr{"content-length"}) {
			$resp_body = substr($lines, 0, $resp_hdr{"content-length"});
			if (length($lines) < $resp_hdr{"content-length"}) {
				$lines = "";
			} else {
				$lines = substr($lines, $resp_hdr{"content-length"});
			}
			undef $lines if (length($lines) == 0);
		} else {
			$resp_body = $lines;
			undef $lines;
		}

		# check conditions
		if ($resp_line =~ /^(HTTP\/1\.[01]) ([0-9]{3}) .+$/) {
			if ($href->{'HTTP-Protocol'} ne $1) {
				diag(sprintf("\nproto failed: expected '%s', got '%s'", $href->{'HTTP-Protocol'}, $1));
				return -1;
			}
			if ($href->{'HTTP-Status'} ne $2) {
				diag(sprintf("\nstatus failed: expected '%s', got '%s'", $href->{'HTTP-Status'}, $2));
				return -1;
			}
		} else {
			diag(sprintf("\nunexpected resp_line '%s'", $resp_line));
			return -1;
		}

		if (defined $href->{'HTTP-Content'}) {
			$resp_body = "" unless defined $resp_body;
			if ($href->{'HTTP-Content'} ne $resp_body) {
				diag(sprintf("\nbody failed: expected '%s', got '%s'", $href->{'HTTP-Content'}, $resp_body));
				return -1;
			}
		}

		if (defined $href->{'-HTTP-Content'}) {
			if (defined $resp_body && $resp_body ne '') {
				diag(sprintf("\nbody failed: expected empty body, got '%s'", $resp_body));
				return -1;
			}
		}

		foreach (keys %{ $href }) {
			next if $_ eq 'HTTP-Protocol';
			next if $_ eq 'HTTP-Status';
			next if $_ eq 'HTTP-Content';
			next if $_ eq '-HTTP-Content';

			(my $k = $_) =~ tr/[A-Z]/[a-z]/;

			my $verify_value = 1;
			my $key_inverted = 0;

			if (substr($k, 0, 1) eq '+') {
				$k = substr($k, 1);
				$verify_value = 0;
			} elsif (substr($k, 0, 1) eq '-') {
				## the key should NOT exist
				$k = substr($k, 1);
				$key_inverted = 1;
				$verify_value = 0; ## skip the value check
			}

			if ($key_inverted) {
				if (defined $resp_hdr{$k}) {
					diag(sprintf("\nheader '%s' MUST not be set", $k));
					return -1;
				}
			} else {
				if (not defined $resp_hdr{$k}) {
					diag(sprintf("\nrequired header '%s' is missing", $k));
					return -1;
				}
			}

			if ($verify_value) {
				if ($href->{$_} =~ /^\/(.+)\/$/) {
					if ($resp_hdr{$k} !~ /$1/) {
						diag(sprintf(
							"\nresponse-header failed: expected '%s', got '%s', regex: %s",
							$href->{$_}, $resp_hdr{$k}, $1));
						return -1;
					}
				} elsif ($href->{$_} ne $resp_hdr{$k}) {
					diag(sprintf(
						"\nresponse-header failed: expected '%s', got '%s'",
						$href->{$_}, $resp_hdr{$k}));
					return -1;
				}
			}
		}
	}

	# we should have sucked up everything
	if (defined $lines) {
		diag(sprintf("\nunexpected lines '%s'", $lines));
		return -1;
	}

	return 0;
}

1;
