#!perl

# Searches for string codes in sources

$lng = shift;

die "Useage: $0 lng-file {source-files}\n"
	unless $lng;

foreach $pattern (@ARGV) {
	if(substr($pattern, 0, 1) eq '-') {
		$pattern = substr($pattern ,1);
		$negate = 1;
	} else {
		$negate = 0;
	}
	foreach $file (glob($pattern)) {
		if(-f $file) {
			$ofile = lc($file);
			if($negate) {
				delete $files{$ofile};
			} else {
				$files{$ofile} = $file;
			}
		}
	}
}

foreach $ofile (sort keys %files) {
	$file = $files{$ofile};
	warn "Cannot open file $file: $!\n"
		unless open(IN, $file);
	while(<IN>) {
		while(/\b(TEXT|PROMPT)_[A-Z_]+\b/) {
			$string{$&} = 1;
			$_ = $';
		}
	}
	close IN;
}


die "Cannot open lng file: $!\n"
	unless open(IN, $lng);
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
	print $name . "\n" unless $string{$name};
}
close IN;
