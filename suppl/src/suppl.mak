.AUTODEPEND

CFG_DEPENDENCIES = suppl.mak

!include "..\..\config.mak"

all : suppl_$(SHELL_MMODEL).lib

OBJ1 = addu.obj byte2par.obj cntry.obj dfndeli2.obj dfndelim.obj dfnexpan.obj \
dfnfnam.obj
OBJ2 = dfnfullp.obj dfnmerge.obj dfnpath.obj dfnsplit.obj dfnsquee.obj \
dfnstat.obj
OBJ3 = dfntruen.obj dmemcmpf.obj dosalloc.obj dosfree.obj dossize.obj \
dstrchar.obj
OBJ4 = dstrfupr.obj dstrleft.obj dstrrepl.obj dstrtrim.obj dstrupr.obj \
enoallc.obj
OBJ5 = enoreal.obj enosdup.obj enosetos.obj env_chg.obj env_del.obj \
env_dvar.obj env_find.obj
OBJ6 = env_fora.obj env_free.obj env_insv.obj env_len.obj env_mtch.obj \
env_new.obj
OBJ7 = env_nost.obj env_ovrw.obj env_repl.obj env_rlsg.obj env_scnt.obj \
env_size.obj
OBJ8 = env_strg.obj env_sub.obj env_var1.obj env_var2.obj ffmaxbuf.obj \
fgetpos.obj
OBJ9 = fmemchr.obj fmemcmp.obj fmemcpy.obj fmemove.obj fnorm.obj fputmc.obj \
fstrcpy.obj
OBJ10= fstrdup.obj fstrlen.obj _getdcwd.obj mcb_1st.obj mcb_is.obj \
mcb_leng.obj  mcb_nxt.obj
OBJ11= mcb_walk.obj stpcat.obj stpcpy.obj toupperx.obj

CFLAGS = $(CFLAGS) -I..

suppl_$(SHELL_MMODEL).lib: $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) \
$(OBJ7) $(OBJ8) $(OBJ9) $(OBJ10) $(OBJ11)
	cd ..
	echo SET CFG=$(CFG)>vars.bat
	echo SET SUPPL=suppl_$(SHELL_MMODEL)>>vars.bat
	echo SET CC=$(CC)>>vars.bat
	echo SET CFLAGS=$(CFLAGS)>>vars.bat
	echo SET AR=$(AR)>>vars.bat
    echo >compile.me

clean :
	cd ..
	clnsuppl.bat
