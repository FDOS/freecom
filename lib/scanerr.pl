#!perl

# Scan the err*.c files and create the ERROR.H include file.

for (@ARGV) {
	push(@h, glob($_));
}
@ARGV = @h;

while(<>) {
	next unless /^void\s+[a-z]/;
	chomp;
	if(/^([^\(]+\([^\)]+\))\s*$/) {
		# simple case
		print "$1;\n";
	} else {
		$line = $_ . "\n";
		$cnt = 1;
		die "Syntax error in function definition"
			unless ($i = index($line, '(')) >= 0;
		$fnam = $ARGV;
		while($cnt) {
			if(++$i >= length($line)) {
				$_ = <>;
				die "Missing right parenthesis in file $fnam\n"
					if !$_ || $ARGV != $fnam;
				$line .= $_;
			}
			++$cnt if substr($line, $i, 1) eq '(';
			--$cnt if substr($line, $i, 1) eq ')';
		}
		print substr($line, 0, $i) . ";\n";
	}
}
