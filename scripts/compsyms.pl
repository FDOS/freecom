
die "Useage: $0 this_map previous_map" unless $#ARGV == 1;

die "Cannot open this map $ARGV[0]" unless open(IN, $ARGV[0]);
while( ($_ = <IN>) && !/^\s*Address/ )
{}

LOOP1: while(<IN>) {
	s/^\s*//;
	s/\s*$//;
	@F = split(/\s+/, $_);
	last LOOP1 if $F[0] eq 'Address';

	if($#F == 1) {
		$sym{$F[1]} = 1;
	} elsif($#F == 2 && $F[1] eq 'Abs') {
		$sym{$F[2]} = 1;
	}
}
close IN;

die "Cannot open previous map $ARGV[1]" unless open(IN, $ARGV[1]);
while( ($_ = <IN>) && !/^\s*Address/ )
{}

LOOP1: while(<IN>) {
	s/^\s*//;
	s/\s*$//;
	@F = split(/\s+/, $_);
	last LOOP1 if $F[0] eq 'Address';

	if($#F == 1) {
		$nam = $F[1];
	} elsif($#F == 2 && $F[1] eq 'Abs') {
		$nam = $F[2];
	} else {
		next;
	}

	if(defined $sym{$nam}) {
		delete $sym{$nam};
	} else {
		print "- $nam\n";
	}
}
close IN;

foreach $nam (sort keys %sym) {
	print "+ $nam\n";
}
