#!perl

# Compare a language file of two different ages

# Useage: $0 new old

die "Useage: $0 new old\n"
	unless $ARGV[0] && $ARGV[1];

die "Cannot open $ARGV[1]: $!\n"
	unless open(IN, $ARGV[1]);
while(<IN>) {
	next unless /^:/;
	$name = $';
	$name = $` if $name =~ /\#/;
	$name =~ s/\s+//g;
	$body = '';
	while(<IN>) {
		$body .= $_;
		last if /^[\.\,]\s*$/;
	}
	$string{$name} = $body;
}
close IN;

unlink("$ARGV[0].old");
die "Cannot backup $ARGV[0]: $!\n"
	unless rename($ARGV[0], "$ARGV[0].old");
die "Cannot open backup $ARGV[0].old: $!\n"
	unless open(IN, "$ARGV[0].old");
die "Cannot create $ARGV[0]: $!\n"
	unless open(OUT, ">$ARGV[0]");
while(<IN>) {
	if(/^:/) {
		$name = $';
		$name = $` if $name =~ /\#/;
		$name =~ s/\s+//g;
		$body = '';
		while(<IN>) {
			$body .= $_;
			last if /^[\.\,]\s*$/;
		}
		$name .= '#1' unless $string{$name} eq $body;
		print OUT ":$name\n", $body;
	} else {
		print OUT;
	}
}
close IN;
close OUT;

