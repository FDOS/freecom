#!perl

# Scan the err*.c files and create the ERROR.H include file.

$src = shift;

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

if($src) {
	die "Cannot open file $src: $!\n"
		unless open(SRC, $src);
	$i = rindex($src, "\\");
	$j = rindex($src, '/');
	if($i < 0 && $j < 0) {
		$dir = '.';
	} else {
		$dir = substr($src, 0, $i > $j? $i: $j);
	}
	$cnt = 0;
	die "Cannot create Makefile include file: $dir/err_fcts.mk: $!\n"
		unless open(MK, ">$dir/err_fcts.mk");
	while(<SRC>) {
		chop;
		die "Syntax error"
			unless /:=/;
		$header = $`;
		$body = "displayError($');";
		if($header =~ /\(/) {
			$fctname = $`;
		} else {
			$fctname = $header;
			$header .= '(void)';
		}
		$fnam = "$dir/err" . ++$cnt . '.c';
		die "Cannot create $fnam: $!\n"
			unless open(OUT, ">$fnam");
		print "void $header;\n";
		print OUT "/*	This is an automatic generated file

	DO NOT EDIT! SEE ERR_FCTS.SRC and SCANERR.PL.

	Error printing function providing a wrapper for STRINGS
 */

#include \"../config.h\"

#include \"../include/misc.h\"
#include \"../err_fcts.h\"
#include \"../strings.h\"

#undef $fctname
void $header
{	$body
}
";
		close OUT;
	}
	print MK "SRC_ERR = ";
	foreach $i (1..$cnt) {
		print MK " \\\n\terr$i.c"
	}
	print MK "\n\nOBJ_ERR = ";
	foreach $i (1..$cnt) {
		print MK " \\\n\terr$i.obj"
	}
	print MK "\n\n";
	foreach $i (1..$cnt) {
		print MK "err$i.obj : err$i.c ../config.h ../strings.h ../err_fcts.h ../include/misc.h\n"
	}
	close MK;
}
