
die "Useage: $0 this_map previous_map\n" unless $#ARGV == 1;

die "Cannot open this map file $ARGV[0]" unless open(IN, $ARGV[0]);
LOOP1: while(<IN>) {
	s/^\s+//;
	s/\s+$//;
	@F = split(/\s+/, $_);
	next if $#F < $[;
	last LOOP1 if $#F != 4;
	next if $F[4] eq 'Class';
	$segs{$F[3]} = hex(substr($F[2], 0, length($F[2]) - 1));
}
close IN;

die "Cannot open previous map file $ARGV[1]" unless open(IN, $ARGV[1]);
LOOP2: while(<IN>) {
	s/^\s+//;
	s/\s+$//;
	@F = split(/\s+/, $_);
	next if $#F < $[;
	last LOOP2 if $#F != 4;
	next if $F[4] eq 'Class';
	$len = hex(substr($F[2], 0, length($F[2]) - 1));
	if(defined $segs{$F[3]}) {
		$len = $segs{$F[3]} - $len;
		delete $segs{$F[3]};
	}
	$total += $len;
	$len = "+" . $len if $len > 0;
	(print("$F[3]: $len\n"), $disp = 1) if $len;
}
close IN;
$total = "+" . $total if $total > 0;
print "Total: $total\n" if $disp;
