CFG_DEPENDENCIES = makefile.mak

TOP2LVL = 1
TOP=../..
!include "$(TOP)/config.mak"

SUPPL=suppl_$(SHELL_MMODEL)
CC = $(CC) -I..

all : ../$(SUPPL).lib

OBJS1 = addu.obj byte2par.obj cntry.obj dfndeli2.obj dfndelim.obj dfnexpan.obj dfnfnam.obj
OBJS2 = dfnfullp.obj dfnmerge.obj dfnpath.obj dfnsplit.obj dfnsquee.obj dfnstat.obj
OBJS3 = dfntruen.obj dmemcmpf.obj dosalloc.obj dosfree.obj dossize.obj dstrchar.obj
OBJS4 = dstrfupr.obj dstrleft.obj dstrrepl.obj dstrtrim.obj dstrupr.obj enoallc.obj
OBJS5 = enoreal.obj enosdup.obj enosetos.obj env_chg.obj env_del.obj env_dvar.obj env_find.obj
OBJS6 = env_fora.obj env_free.obj env_insv.obj env_len.obj env_mtch.obj env_new.obj
OBJS7 = env_nost.obj env_ovrw.obj env_repl.obj env_rlsg.obj env_scnt.obj env_size.obj
OBJS8 = env_strg.obj env_sub.obj env_var1.obj env_var2.obj ffmaxbuf.obj fgetpos.obj
OBJS9 = fmemchr.obj fmemcmp.obj fmemcpy.obj fmemove.obj fnorm.obj fputmc.obj fstrcpy.obj
OBJS10 = fstrdup.obj fstrlen.obj _getdcwd.obj mcb_1st.obj mcb_is.obj mcb_leng.obj  mcb_nxt.obj
OBJS11 = mcb_walk.obj stpcat.obj stpcpy.obj toupperx.obj filefind.obj invokedo.obj intr.obj

DOBJS1 = app_ievx.obj app_ini_.obj app_name.obj app_namx.obj app_vars.obj dbgf_chg.obj
DOBJS2 = dbgf_cl.obj dbgf_cle.obj dbgf_clg.obj dbgf_dl.obj dbgf_dpl.obj dbgf_et.obj
DOBJS3 = dbgf_ext.obj dbgf_fl.obj dbgf_flg.obj dbgf_ien.obj dbgf_lgh.obj dbgf_lgi.obj
DOBJS4 = dbgf_lgt.obj dbgf_lk.obj dbgf_log.obj dbgf_lv.obj dbgf_mfi.obj dbgf_mi.obj
DOBJS5 = dbgf_mki.obj dbgf_ml.obj dbgf_pop.obj dbgf_prt.obj dbgf_psh.obj dbgf_var.obj
DOBJS6 = dbgm_chk.obj dbgv_s0.obj dbgv_s2.obj dbgv_s3.obj dbgv_s4.obj dbgv_s5.obj
DOBJS7 = dbgv_s6.obj dbgv_s7.obj dbgv_s8.obj dbgv_s10.obj dbgv_s13.obj dbgv_s14.obj
DOBJS8 = dbgv_s15.obj dbgv_s17.obj dbgv_s19.obj dbgv_s20.obj dbgv_s22.obj dbgv_s25.obj
DOBJS9 = eeopen.obj eestrcon.obj env_sdup.obj erfc_00f.obj erfc_015.obj gm_res.obj 
DOBJS10 = gm_dup.obj gm_chgm.obj gm_gtmem.obj nlstime.obj strnum.obj s_skipws.obj s_skipwd.obj

# Prepare Linker Response File
objlist.txt: $(ECHOLIBDEP) makefile.mak
	$(RMFILES) objlist.txt
	$(ECHOLIB) objlist.txt $(OBJS1)
	$(ECHOLIB) objlist.txt $(OBJS2)
	$(ECHOLIB) objlist.txt $(OBJS3)
	$(ECHOLIB) objlist.txt $(OBJS4)
	$(ECHOLIB) objlist.txt $(OBJS5)
	$(ECHOLIB) objlist.txt $(OBJS6)
	$(ECHOLIB) objlist.txt $(OBJS7)
	$(ECHOLIB) objlist.txt $(OBJS8)
	$(ECHOLIB) objlist.txt $(OBJS9)
	$(ECHOLIB) objlist.txt $(OBJS10)
	$(ECHOLIB) objlist.txt $(OBJS11)
	$(ECHOLIB) objlist.txt $(DOBJS1)
	$(ECHOLIB) objlist.txt $(DOBJS2)
	$(ECHOLIB) objlist.txt $(DOBJS3)
	$(ECHOLIB) objlist.txt $(DOBJS4)
	$(ECHOLIB) objlist.txt $(DOBJS5)
	$(ECHOLIB) objlist.txt $(DOBJS6)
	$(ECHOLIB) objlist.txt $(DOBJS7)
	$(ECHOLIB) objlist.txt $(DOBJS8)
	$(ECHOLIB) objlist.txt $(DOBJS9)
	$(ECHOLIB) objlist.txt $(DOBJS10)

# Create the library
../$(SUPPL).lib: $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6) \
$(OBJS7) $(OBJS8) $(OBJS9) $(OBJS10) $(OBJS11) $(DOBJS1) $(DOBJS2) $(DOBJS3) $(DOBJS4) \
$(DOBJS5) $(DOBJS6) $(DOBJS7) $(DOBJS8) $(DOBJS9) $(DOBJS10) objlist.txt
	$(RMFILES) ..$(DIRSEP)$(SUPPL).lib
	$(AR) ..$(DIRSEP)$(SUPPL).lib @objlist.txt $(LIBLIST)..$(DIRSEP)$(SUPPL).lst
