#!perl

# Generate Xref from all local *.OBJ files and the C standard library

##### Environmental settings

# How to dump .OBJ/.LIB files
## '%f%' is substituted by the source filename
$dump = 'tdump %f%';

# Name of C standard library
$clib = 'd:\bc5\lib\cs.lib';

# Name of SUPPL library
$suppl = 'suppl_s.lib';

# Names of files to be ignored while gener‘ted the dependency
%ignoreFile = ('c0.asm' => 1);


##### End of user-configurable settings

open(LOG, ">mkxref.log")
	or die "Cannot create mkxfer.log: $!";

@files = <*.dmp>;

# Read in all local source files

$mkextref = sub {
			$ext{$name} .= ',' . $file;
			$extf{$file} .= ',' . $name;
		};

foreach $file (@files) {
	print STDERR "Processing $file ...\n";
	$file =~ tr/A-Z/a-z/;
	next if $file =~ /^(suppl|clib)_.\.dmp$/;
	&scan($file, sub { $file = $name; }
	 , sub {
			if(defined $pub{$name}) {
				&warning("$name multiply defined: $pub{$name} and $file");
			} else {
				$pub{$name} = $file;
				$pubf{$file} .= ",$name";
			}
		}
	 , $mkextref);
}

print STDERR "Processing SUPPL library...\n";
&scan('suppl_s.dmp', sub { $file = "Suppl<$name>"; }
 , sub {
		if(defined $pub{$name}) {
			&warning("$name in $file overridden by $pub{$name}");
			--$warning;
		} else {
			$pub{$name} = $file;
			$pubf{$file} .= ",$name";
		}
	}
 , $mkextref);


print STDERR "Processing C library...\n";
&scan('clib_s.dmp', sub { $file = "Clib<$name>"; }
 , sub {
		if(defined $pub{$name}) {
			&warning("$name in $file overridden by $pub{$name}");
			--$warning;
		} else {
			$pub{$name} = $file;
			$pubf{$file} .= ",$name";
		}
	}
 , $mkextref);

die "Solve the warnings before proceed" if $warning;


print STDERR "Generating file dependency information...\n";
# "What files reference a given symbol"
foreach $symbol (keys %pubf) {
	$reftbl{$symbol} = '';
	%refby = ('' => -1, $symbol => -1);
	# $refSyms: symbols exports the given file and can be
	#	referenced by other files
	$refSyms = $pubf{$symbol};
	$depth = 0;
	while($refSyms) {
		++$depth;
		# Aquire where these symbols are located
		$files = '';
		foreach $name (split(',', $refSyms)) {
			# The exported symbols of the files referenced at this depth are
			# referenced are referenced themselves by these files
			$files .= $ext{$name};
		}
		$refSyms = '';
		# Aquire which symbols are externally referenced by the new files
		$sym = '|';
		foreach $file (split(',', $files)) {
			if(!defined $refby{$file} && !defined $ignoreFile{$file}) {
				$reftbl{$symbol} .= $sym . $file;
				$sym = ',';
				$refby{$file} = $depth;
				# the $file references the current file, now determine
				# which files reference symbols exported by this one
				# to get a recursive list
				$refSyms .= $pubf{$file};
			}
		}
	}
}

# "What files a given symbol references"
foreach $symbol (keys %extf) {
	$needtbl{$symbol} = '';
	%needs = ('' => -1, $symbol => -1);
	$needSyms = $extf{$symbol};
	$depth = 0;
	while($needSyms) {
		++$depth;
		# Aquire which files the imported symbols are located in
		$newneedSyms = '';
		$sym = '|';
		foreach $name (split(',', $needSyms)) {
			$file = $pub{$name};
			if(!defined $needs{$file} && !defined $ignoreFile{$file}) {
				$needs{$file} = $depth;
				$needtbl{$symbol} .= $sym . $file;
				$sym = ',';
				# $file is referenced by the current file, now determine
				# which symbols are referenced by this one
				$newneedSyms .= $extf{$file};
			}
		}
		$needSyms = $newneedSyms;
	}
}


print STDERR "Processing MAP file...\n";
@ARGV = <*.map> unless @ARGV;

while(<>) {
	last if /Publics by Value/;
}

$lastSegm = $lastAddr = '';
@names = ();
@files = ();
while(<>) {
	next unless /^\s*([0-9a-fA-F]+):([0-9a-fA-F]+)\s+(\S+)\s*$/;
	$segm = $1;
	$addr = $2;
	$name = $3;

	push(@names, $name);

	if(defined $pub{$name}) {
		$file = $pub{$name};
		$files{$file} = 1;
	}

	if($lastSegm eq $segm) {
		$len{$lastName} = hex($addr) - hex($lastAddr);
	}

	$lastSegm = $segm;
	$lastAddr = $addr;
	$lastName = $name;
}

print "Imported symbols:\n\t" . join("\n\t", sort @names) . "\n";
print "Linked files:\n\t" . join("\n\t", sort keys %files) . "\n";


foreach $name (sort @names) {
	print "\n$name: ";
	if(defined $pub{$name}) {
		$file = $pub{$name};
		print "PUBDEF in $file\n";
	} else {
		print "no PUBDEF\n";
	}
	if(defined $len{$name}) {
		print "\testimated length: $len{$name} byte\n";
	}

	&printRec("references", 'references nothing', $needtbl{$pub{$name}});
	&printRec("referenced by", 'never referenced', $reftbl{$pub{$name}});

}

sub files {
	local($files) = @_;
	local($file);
	local($h) = '';

	foreach $file (split(',', $files)) {
		if($file && defined $files{$file}) {
			$h .= ", $file";
		}
	}

	return substr($h, 2);
}

sub warning {
	++$warning;
	print LOG join("\n", @_) . "\n";
	print STDERR join("\n", @_) . "\n";
}

sub scan {
	local($file, $thecode, $pubcode, $extcode) = @_;

	open(IN, "$file")
		or die "Cannot open $file";
	while(<IN>) {
		$code = '' if /^\S/;
		if(/\bTHEADR\s+(\S+)\s*$/) {
			$name = $1;
			$name =~ tr/A-Z/a-z/;
			&$thecode;
		} elsif(!$code && /\bPUBDEF\b/) {
			$_ = $';
			$code = $pubcode;
		} elsif(!$code && /\bEXTDEF\b/) {
			$_ = $';
			$code = $extcode;
		}
		&processList($code) if $code;
	}
	close IN;
}

sub processList {
	local($code) = @_;

	if(/\'([^\']+)\'/) {
		$name = $1;
		&$code;
	}
}

sub printRec {
	local($yesStr, $noStr, $files) = @_;
	local(@files, $depth, $prompt, $fil, $dep);

	if(length($files) > 1) {
		@files = split(/\|/, $files);
		$prompt = "==$yesStr";
		$dep = 0;
		foreach $depth ($[..$#files) {
			$fil = &files($files[$depth])
			 and (print "$prompt: $fil\n"), $prompt = "  depth #" . ++$dep;
		}
	} else {
		$noStr and print "==$noStr\n";
	}
}
