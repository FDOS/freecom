#!perl

# Check the lengths of the individual segments of the MAP file(s)
# and report differences


# 00000H 05805H 05806H _TEXT              CODE
# 05806H 058FDH 000F8H BATCH_TEXT         CODE
# 058FEH 058FEH 00000H CMDTABLE_TEXT      CODE

if($#ARGV == 0) {
	$ARGV[1] = 'shell/com.map';
}

die "Cannot open old MAP file: $ARGV[0]: $!\n"
	unless open(IN, $ARGV[0]);

while(<IN>) {
	if(/Publics by/) {
		last;
		next;
	}

	@f = split(/\s+/, $_);
	next if $f[5] ne 'CODE';

	$name = $f[4];
	$length = hex($f[3]);
	$len{$name} = $length;
}

close IN;

die "Cannot open new MAP file: $ARGV[1]: $!\n"
	unless open(IN, $ARGV[1]);

$diff = 0;
while(<IN>) {
	if(/Publics by/) {
		last;
		next;
	}

	@f = split(/\s+/, $_);
	next if $f[5] ne 'CODE';

	$name = $f[4];
	$length = hex($f[3]);

	if(defined($len{$name})) {
		if($len{$name} < $length) {
			printf "= $name\t+%05xH\n", $length - $len{$name};
		} elsif($len{$name} > $length) {
			printf "= $name\t-%05xH\n", $len{$name} - $length;
		}
		$diff += $length - $len{$name};
		delete $len{$name};
	} else {
		printf "+ $name\t+%05xH\n", $length;
		$diff += $length;
	}
}

while( ($name, $length) = each %len ) {
	printf "- $name\t-%05xH\n", $length;
	$diff -= $length;
}
close IN;

print "  overall change: $diff bytes\n";

exit 0;
