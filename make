#!/usr/bin/perl -w
use JSON;


my %config = from_json(cat(q(config.json)))->%*;
my %requirment = from_json(cat(q(requirment.json)))->%*;




push $config{resedent}->@*, qw(requirment.json config.json make);
$ENV{CFLAGS} .= qq( -I$config{path}{include} );


&Task::process_tasks(@ARGV);



















package Task {


	sub AUTOLOAD
	{
		my $this = ($AUTOLOAD =~ /::(.*)$/)[0];
		my $cmd;

		push $requirment{$this}->@*, &process_tasks($requirment{$this}->@*) if $requirment{$this};
		if ( -e qq($config{path}{main}/$this.c) ) {
			my $req_str = $requirment{$this} ? (join q( ), map qq($config{path}{obj}/$_.o), $requirment{$this}->@*) : q();
			$cmd = qq(gcc $ENV{CFLAGS} $req_str $config{path}{main}/$this.c -o $config{path}{bin}/$this);
		} elsif ( -e qq($config{path}{module}/$this.c) ) {
			$cmd = qq(gcc -c $ENV{CFLAGS} $config{path}{module}/$this.c -o $config{path}{obj}/$this.o);
		} else {
			warn qq(;; can't found $this);
			return;
		}

		print STDERR qq($cmd\n);
		print STDERR `$cmd`;

		return $requirment{$this} ? $requirment{$this}->@* : ();
	}

	sub process_tasks
	{
		map &{qq(Task::$_)}, @_;
	}

	sub make_main
	{
		my $this = shift;
		my $src = qq($config{path}{main}/$this.c);				warn qq(Can't found $this in main) and return;
		my $res = qq($config{path}{bin}/$this);
		my $src_time = -C $src;
		my $res_time = (-e $res) ? -C $res : 0;

		my $req_str = $requirment{$this} ? (join q( ), map qq($config{path}{obj}/$_.o), $requirment{$this}->@*) : q();
		$cmd = qq(gcc $ENV{CFLAGS} $req_str $config{path}{main}/$this.c -o $config{path}{bin}/$this);

		return $cmd;
	}

	sub make_module
	{
	}

	sub clean
	{
		unlink $_ foreach <$config{path}{bin}/*>;
		unlink $_ foreach <$config{path}{obj}/*>;
		foreach my $file (<*>)
		{
			next		if -d $file;
			next		if grep { $_ eq $file } $config{resedent}->@*;
			unlink $file;
		}
	}


}








sub cat
{
	my $txt;

	foreach my $filename (@_)
	{
		open my $d, qq(<$filename) or warn qq(;; can't open $filename: $!) and next;
			$txt .= join q(), <$d>;
		close $d;
	}

	return $txt;
}





