cmd .SEQUENTIAL : utils err_fcts.h strings context lib cmd_dir

cmd_dir .SETDIR=cmd :
	@echo Entering $(PWD)
	$(RUNMAKE) all
	@echo Leaving $(PWD)

CLEAN_DEPENDENCIES += cmd_clean
cmd_clean .SETDIR=cmd :
	@echo Entering  $(PWD)
	$(RUNMAKE) clean
	@echo Leaving  $(PWD)

CLOBBER_DEPENDENCIES += cmd_clobber
cmd_clobber .SETDIR=cmd :
	@echo Entering  $(PWD)
	$(RUNMAKE) clobber
	@echo Leaving  $(PWD)

DIST_DEPENDENCIES += cmd_dist
cmd_dist .SETDIR=cmd :
	@echo Entering  $(PWD)
	$(RUNMAKE) dist
	@echo Leaving  $(PWD)
