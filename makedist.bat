if "%1" == "" goto noparam

mkdir temp

rem first make executable distribution
mkdir temp\bin
mkdir temp\doc
mkdir temp\doc\%1

copy %1\command.com temp\bin
copy %1\docs\*.* temp\doc\%1

cd temp
pkzip -mpre0 ..\binaries.zip
cd ..
pkzip -mex %1x.zip binaries.zip

rem now make source distribution

mkdir temp\source
mkdir temp\source\%1
xcopy /s /e %1 temp\source\%1

cd temp
pkzip -mpre0 ..\source.zip
cd ..
pkzip -mex %1s.zip source.zip

rmdir temp

goto done

:noparam
echo you must specify the distribution (i.e. com075e).


:done
