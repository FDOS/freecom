utils .SETDIR=utils :
	@echo Entering $(PWD)
	$(RUNMAKE) all
	@echo Leaving $(PWD)

CLEAN_DEPENDENCIES += utils_clean
utils_clean .SETDIR=utils :
	@echo Entering  $(PWD)
	$(RUNMAKE) clean
	@echo Leaving  $(PWD)

CLOBBER_DEPENDENCIES += utils_clobber
utils_clobber .SETDIR=utils :
	@echo Entering  $(PWD)
	$(RUNMAKE) clobber
	@echo Leaving  $(PWD)

DIST_DEPENDENCIES += utils_dist
utils_dist .SETDIR=utils :
	@echo Entering  $(PWD)
	$(RUNMAKE) dist
	@echo Leaving  $(PWD)
