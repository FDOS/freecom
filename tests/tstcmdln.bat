@ECHO OFF
mkdir "a long dir" > NUL
copy testenv.exe "a long dir\testenv.exe" > NUL
"a long dir"\testenv aaaaaaaaa realllly long command line to create a env variable that can pass the long name to it oh yeah a really long command line still going.  Oh yeah is is at the end yet, nope, lets keep going untilout of characters. 
ECHO CMDLINE should start with quoted "a long dir\testenv.exe"
del "a long dir\testenv.exe" > NUL
rmdir "a long dir" > NUL

