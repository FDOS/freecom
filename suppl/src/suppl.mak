CFG_DEPENDENCIES = suppl.mak

TOP=../..
!include "$(TOP)/config.mak"

SUPPL=suppl_$(SHELL_MMODEL)
CC = $(CC) -I..

all : ..\$(SUPPL).lib

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

DOBJ1 = app_ievx.obj app_ini_.obj app_name.obj app_namx.obj app_vars.obj \
dbgf_chg.obj
DOBJ2 = dbgf_cl.obj dbgf_cle.obj dbgf_clg.obj dbgf_dl.obj dbgf_dpl.obj \
dbgf_et.obj
DOBJ3 = dbgf_ext.obj dbgf_fl.obj dbgf_flg.obj dbgf_ien.obj dbgf_lgh.obj \
dbgf_lgi.obj
DOBJ4 = dbgf_lgt.obj dbgf_lk.obj dbgf_log.obj dbgf_lv.obj dbgf_mfi.obj \
dbgf_mi.obj
DOBJ5 = dbgf_mki.obj dbgf_ml.obj dbgf_pop.obj dbgf_prt.obj dbgf_psh.obj \
dbgf_var.obj
DOBJ6 = dbgm_chk.obj dbgv_s0.obj dbgv_s2.obj dbgv_s3.obj dbgv_s4.obj dbgv_s5.obj
DOBJ7 = dbgv_s6.obj dbgv_s7.obj dbgv_s8.obj dbgv_s10.obj dbgv_s13.obj \
dbgv_s14.obj
DOBJ8 = dbgv_s15.obj dbgv_s17.obj dbgv_s19.obj dbgv_s20.obj dbgv_s22.obj \
dbgv_s25.obj
DOBJ9 = eeopen.obj eestrcon.obj env_sdup.obj erfc_00f.obj erfc_015.obj \
gm_res.obj 
DOBJ10 = gm_dup.obj gm_chgm.obj gm_gtmem.obj nlstime.obj strnum.obj \
s_skipws.obj s_skipwd.obj

echolib.bat: ..\..\scripts\echolib.bat
	copy ..\..\scripts\echolib.bat

# Prepare Linker Response File
objlist.txt: echolib.bat suppl.mak
	..\..\scripts\rmfiles objlist.txt
	$(ECHOLIB) objlist.txt $(OBJ1)
	$(ECHOLIB) objlist.txt $(OBJ2)
	$(ECHOLIB) objlist.txt $(OBJ3)
	$(ECHOLIB) objlist.txt $(OBJ4)
	$(ECHOLIB) objlist.txt $(OBJ5)
	$(ECHOLIB) objlist.txt $(OBJ6)
	$(ECHOLIB) objlist.txt $(OBJ7)
	$(ECHOLIB) objlist.txt $(OBJ8)
	$(ECHOLIB) objlist.txt $(OBJ9)
	$(ECHOLIB) objlist.txt $(OBJ10)
	$(ECHOLIB) objlist.txt $(OBJ11)
	$(ECHOLIB) objlist.txt $(DOBJ1)
	$(ECHOLIB) objlist.txt $(DOBJ2)
	$(ECHOLIB) objlist.txt $(DOBJ3)
	$(ECHOLIB) objlist.txt $(DOBJ4)
	$(ECHOLIB) objlist.txt $(DOBJ5)
	$(ECHOLIB) objlist.txt $(DOBJ6)
	$(ECHOLIB) objlist.txt $(DOBJ7)
	$(ECHOLIB) objlist.txt $(DOBJ8)
	$(ECHOLIB) objlist.txt $(DOBJ9)
	$(ECHOLIB) objlist.txt $(DOBJ10)

# Create the library
..\$(SUPPL).lib: $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) \
$(OBJ7) $(OBJ8) $(OBJ9) $(OBJ10) $(OBJ11) $(DOBJ1) $(DOBJ2) $(DOBJ3) $(DOBJ4) \
$(DOBJ5) $(DOBJ6) $(DOBJ7) $(DOBJ8) $(DOBJ9) $(DOBJ10) objlist.txt
	..\..\scripts\rmfiles ..\$(SUPPL).lib
	$(AR) /C ..\$(SUPPL).LIB @objlist.txt $(LIBLIST)..\$(SUPPL).lst
