shell .SEQUENTIAL : utils err_fcts.h strings context lib cmd shell_dir

shell_dir .SETDIR=shell :
	@echo Entering $(PWD)
	$(RUNMAKE) all
	@echo Leaving $(PWD)

CLEAN_DEPENDENCIES += shell_clean
shell_clean .SETDIR=shell :
	@echo Entering  $(PWD)
	$(RUNMAKE) clean
	@echo Leaving  $(PWD)

CLOBBER_DEPENDENCIES += shell_clobber
shell_clobber .SETDIR=shell :
	@echo Entering  $(PWD)
	$(RUNMAKE) clobber
	@echo Leaving  $(PWD)

DIST_DEPENDENCIES += shell_dist
shell_dist .SETDIR=shell :
	@echo Entering  $(PWD)
	$(RUNMAKE) dist
	@echo Leaving  $(PWD)
