strings .SEQUENTIAL : utils strings_dir

strings_dir .SETDIR=strings :
	@echo Entering $(PWD)
	$(RUNMAKE) all
	@echo Leaving $(PWD)

CLEAN_DEPENDENCIES += strings_clean
strings_clean .SETDIR=strings :
	@echo Entering  $(PWD)
	$(RUNMAKE) clean
	@echo Leaving  $(PWD)

CLOBBER_DEPENDENCIES += strings_clobber
strings_clobber .SETDIR=strings :
	@echo Entering  $(PWD)
	$(RUNMAKE) clobber
	@echo Leaving  $(PWD)

DIST_DEPENDENCIES += strings_dist
strings_dist .SETDIR=strings :
	@echo Entering  $(PWD)
	$(RUNMAKE) dist
	@echo Leaving  $(PWD)
