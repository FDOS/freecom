tools .SEQUENTIAL : utils err_fcts.h strings context lib tools_dir

default.mk : ../config.h
	..\utils\mktools.exe >$@

tools_dir .SETDIR=tools : default.mk
	@echo Entering $(PWD)
	$(RUNMAKE) all
	@echo Leaving $(PWD)

CLEAN_DEPENDENCIES += tools_clean
tools_clean .SETDIR=tools :
	@echo Entering  $(PWD)
	$(RUNMAKE) clean
	@echo Leaving  $(PWD)

CLOBBER_DEPENDENCIES += tools_clobber
tools_clobber .SETDIR=tools :
	@echo Entering  $(PWD)
	$(RUNMAKE) clobber
	@echo Leaving  $(PWD)

DIST_DEPENDENCIES += tools_dist
tools_dist .SETDIR=tools :
	@echo Entering  $(PWD)
	$(RUNMAKE) dist
	@echo Leaving  $(PWD)
