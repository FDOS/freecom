lib .SEQUENTIAL : utils err_fcts.h strings context lib_dir

lib_dir .SETDIR=lib :
	@echo Entering $(PWD)
	$(RUNMAKE) all
	@echo Leaving $(PWD)

CLEAN_DEPENDENCIES += lib_clean
lib_clean .SETDIR=lib :
	@echo Entering  $(PWD)
	$(RUNMAKE) clean
	@echo Leaving  $(PWD)

CLOBBER_DEPENDENCIES += lib_clobber
lib_clobber .SETDIR=lib :
	@echo Entering  $(PWD)
	$(RUNMAKE) clobber
	@echo Leaving  $(PWD)

DIST_DEPENDENCIES += lib_dist
lib_dist .SETDIR=lib :
	@echo Entering  $(PWD)
	$(RUNMAKE) dist
	@echo Leaving  $(PWD)
