CFG_DEPENDENCIES = makefile.mak

TOP=../..
!include "$(TOP)/config.mak"

SUPPL=suppl_$(SHELL_MMODEL)
CC = $(CC) -I..

all : ../$(SUPPL).lib

OBJS1 = addu.obj byte2par.obj cntry.obj dfndeli2.obj dfndelim.obj dfnexpan.obj dfnfnam.obj
LOBJS1 = $(LIBPLUS)addu.obj $(LIBPLUS)byte2par.obj $(LIBPLUS)cntry.obj $(LIBPLUS)dfndeli2.obj $(LIBPLUS)dfndelim.obj $(LIBPLUS)dfnexpan.obj $(LIBPLUS)dfnfnam.obj
OBJS2 = dfnfullp.obj dfnmerge.obj dfnpath.obj dfnsplit.obj dfnsquee.obj dfnstat.obj
LOBJS2 = $(LIBPLUS)dfnfullp.obj $(LIBPLUS)dfnmerge.obj $(LIBPLUS)dfnpath.obj $(LIBPLUS)dfnsplit.obj $(LIBPLUS)dfnsquee.obj $(LIBPLUS)dfnstat.obj
OBJS3 = dfntruen.obj dmemcmpf.obj dosalloc.obj dosfree.obj dossize.obj dstrchar.obj
LOBJS3 = $(LIBPLUS)dfntruen.obj $(LIBPLUS)dmemcmpf.obj $(LIBPLUS)dosalloc.obj $(LIBPLUS)dosfree.obj $(LIBPLUS)dossize.obj $(LIBPLUS)dstrchar.obj
OBJS4 = dstrfupr.obj dstrleft.obj dstrrepl.obj dstrtrim.obj dstrupr.obj enoallc.obj
LOBJS4 = $(LIBPLUS)dstrfupr.obj $(LIBPLUS)dstrleft.obj $(LIBPLUS)dstrrepl.obj $(LIBPLUS)dstrtrim.obj $(LIBPLUS)dstrupr.obj $(LIBPLUS)enoallc.obj
OBJS5 = enoreal.obj enosdup.obj enosetos.obj env_chg.obj env_del.obj env_dvar.obj env_find.obj
LOBJS5 = $(LIBPLUS)enoreal.obj $(LIBPLUS)enosdup.obj $(LIBPLUS)enosetos.obj $(LIBPLUS)env_chg.obj $(LIBPLUS)env_del.obj $(LIBPLUS)env_dvar.obj $(LIBPLUS)env_find.obj
OBJS6 = env_fora.obj env_free.obj env_insv.obj env_len.obj env_mtch.obj env_new.obj
LOBJS6 = $(LIBPLUS)env_fora.obj $(LIBPLUS)env_free.obj $(LIBPLUS)env_insv.obj $(LIBPLUS)env_len.obj $(LIBPLUS)env_mtch.obj $(LIBPLUS)env_new.obj
OBJS7 = env_nost.obj env_ovrw.obj env_repl.obj env_rlsg.obj env_scnt.obj env_size.obj
LOBJS7 = $(LIBPLUS)env_nost.obj $(LIBPLUS)env_ovrw.obj $(LIBPLUS)env_repl.obj $(LIBPLUS)env_rlsg.obj $(LIBPLUS)env_scnt.obj $(LIBPLUS)env_size.obj
OBJS8 = env_strg.obj env_sub.obj env_var1.obj env_var2.obj ffmaxbuf.obj fgetpos.obj
LOBJS8 = $(LIBPLUS)env_strg.obj $(LIBPLUS)env_sub.obj $(LIBPLUS)env_var1.obj $(LIBPLUS)env_var2.obj $(LIBPLUS)ffmaxbuf.obj $(LIBPLUS)fgetpos.obj
OBJS9 = fmemchr.obj fmemcmp.obj fmemcpy.obj fmemove.obj fnorm.obj fputmc.obj fstrcpy.obj
LOBJS9 = $(LIBPLUS)fmemchr.obj $(LIBPLUS)fmemcmp.obj $(LIBPLUS)fmemcpy.obj $(LIBPLUS)fmemove.obj $(LIBPLUS)fnorm.obj $(LIBPLUS)fputmc.obj $(LIBPLUS)fstrcpy.obj
OBJS10 = fstrdup.obj fstrlen.obj _getdcwd.obj mcb_1st.obj mcb_is.obj mcb_leng.obj  mcb_nxt.obj
LOBJS10 = $(LIBPLUS)fstrdup.obj $(LIBPLUS)fstrlen.obj $(LIBPLUS)_getdcwd.obj $(LIBPLUS)mcb_1st.obj $(LIBPLUS)mcb_is.obj $(LIBPLUS)mcb_leng.obj $(LIBPLUS)mcb_nxt.obj
OBJS11 = mcb_walk.obj stpcat.obj stpcpy.obj toupperx.obj filefind.obj invokedo.obj intr.obj
LOBJS11 = $(LIBPLUS)mcb_walk.obj $(LIBPLUS)stpcat.obj $(LIBPLUS)stpcpy.obj $(LIBPLUS)toupperx.obj $(LIBPLUS)filefind.obj $(LIBPLUS)invokedo.obj $(LIBPLUS)intr.obj

DOBJS1 = app_ievx.obj app_ini_.obj app_name.obj app_namx.obj app_vars.obj dbgf_chg.obj
LDOBJS1 = $(LIBPLUS)app_ievx.obj $(LIBPLUS)app_ini_.obj $(LIBPLUS)app_name.obj $(LIBPLUS)app_namx.obj $(LIBPLUS)app_vars.obj $(LIBPLUS)dbgf_chg.obj
DOBJS2 = dbgf_cl.obj dbgf_cle.obj dbgf_clg.obj dbgf_dl.obj dbgf_dpl.obj dbgf_et.obj
LDOBJS2 = $(LIBPLUS)dbgf_cl.obj $(LIBPLUS)dbgf_cle.obj $(LIBPLUS)dbgf_clg.obj $(LIBPLUS)dbgf_dl.obj $(LIBPLUS)dbgf_dpl.obj $(LIBPLUS)dbgf_et.obj
DOBJS3 = dbgf_ext.obj dbgf_fl.obj dbgf_flg.obj dbgf_ien.obj dbgf_lgh.obj dbgf_lgi.obj
LDOBJS3 = $(LIBPLUS)dbgf_ext.obj $(LIBPLUS)dbgf_fl.obj $(LIBPLUS)dbgf_flg.obj $(LIBPLUS)dbgf_ien.obj $(LIBPLUS)dbgf_lgh.obj $(LIBPLUS)dbgf_lgi.obj
DOBJS4 = dbgf_lgt.obj dbgf_lk.obj dbgf_log.obj dbgf_lv.obj dbgf_mfi.obj dbgf_mi.obj
LDOBJS4 = $(LIBPLUS)dbgf_lgt.obj $(LIBPLUS)dbgf_lk.obj $(LIBPLUS)dbgf_log.obj $(LIBPLUS)dbgf_lv.obj $(LIBPLUS)dbgf_mfi.obj $(LIBPLUS)dbgf_mi.obj
DOBJS5 = dbgf_mki.obj dbgf_ml.obj dbgf_pop.obj dbgf_prt.obj dbgf_psh.obj dbgf_var.obj
LDOBJS5 = $(LIBPLUS)dbgf_mki.obj $(LIBPLUS)dbgf_ml.obj $(LIBPLUS)dbgf_pop.obj $(LIBPLUS)dbgf_prt.obj $(LIBPLUS)dbgf_psh.obj $(LIBPLUS)dbgf_var.obj
DOBJS6 = dbgm_chk.obj dbgv_s0.obj dbgv_s2.obj dbgv_s3.obj dbgv_s4.obj dbgv_s5.obj
LDOBJS6 = $(LIBPLUS)dbgm_chk.obj $(LIBPLUS)dbgv_s0.obj $(LIBPLUS)dbgv_s2.obj $(LIBPLUS)dbgv_s3.obj $(LIBPLUS)dbgv_s4.obj $(LIBPLUS)dbgv_s5.obj
DOBJS7 = dbgv_s6.obj dbgv_s7.obj dbgv_s8.obj dbgv_s10.obj dbgv_s13.obj dbgv_s14.obj
LDOBJS7 = $(LIBPLUS)dbgv_s6.obj $(LIBPLUS)dbgv_s7.obj $(LIBPLUS)dbgv_s8.obj $(LIBPLUS)dbgv_s10.obj $(LIBPLUS)dbgv_s13.obj $(LIBPLUS)dbgv_s14.obj
DOBJS8 = dbgv_s15.obj dbgv_s17.obj dbgv_s19.obj dbgv_s20.obj dbgv_s22.obj dbgv_s25.obj
LDOBJS8 = $(LIBPLUS)dbgv_s15.obj $(LIBPLUS)dbgv_s17.obj $(LIBPLUS)dbgv_s19.obj $(LIBPLUS)dbgv_s20.obj $(LIBPLUS)dbgv_s22.obj $(LIBPLUS)dbgv_s25.obj
DOBJS9 = eeopen.obj eestrcon.obj env_sdup.obj erfc_00f.obj erfc_015.obj gm_res.obj 
LDOBJS9 = $(LIBPLUS)eeopen.obj $(LIBPLUS)eestrcon.obj $(LIBPLUS)env_sdup.obj $(LIBPLUS)erfc_00f.obj $(LIBPLUS)erfc_015.obj $(LIBPLUS)gm_res.obj 
DOBJS10 = gm_dup.obj gm_chgm.obj gm_gtmem.obj nlstime.obj strnum.obj s_skipws.obj s_skipwd.obj
LDOBJS10 = $(LIBPLUS)gm_dup.obj $(LIBPLUS)gm_chgm.obj $(LIBPLUS)gm_gtmem.obj $(LIBPLUS)nlstime.obj $(LIBPLUS)strnum.obj $(LIBPLUS)s_skipws.obj $(LIBPLUS)s_skipwd.obj

echolib.bat: ../../scripts/echolib.bat
	$(CP) ..$(DIRSEP)..$(DIRSEP)scripts$(DIRSEP)echolib.bat .

# Prepare Linker Response File
objlist.txt: echolib.bat makefile.mak
	$(RMFILES2) objlist.txt
	$(ECHOLIB) objlist.txt $(LOBJS1) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LOBJS2) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LOBJS3) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LOBJS4) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LOBJS5) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LOBJS6) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LOBJS7) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LOBJS8) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LOBJS9) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LOBJS10) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LOBJS11) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LDOBJS1) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LDOBJS2) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LDOBJS3) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LDOBJS4) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LDOBJS5) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LDOBJS6) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LDOBJS7) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LDOBJS8) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LDOBJS9) $(LIBCONT)
	$(ECHOLIB) objlist.txt $(LDOBJS10)

# Create the library
../$(SUPPL).lib: $(OBJS1) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5) $(OBJS6) \
$(OBJS7) $(OBJS8) $(OBJS9) $(OBJS10) $(OBJS11) $(DOBJS1) $(DOBJS2) $(DOBJS3) $(DOBJS4) \
$(DOBJS5) $(DOBJS6) $(DOBJS7) $(DOBJS8) $(DOBJS9) $(DOBJS10) objlist.txt
	$(RMFILES2) ..$(DIRSEP)$(SUPPL).lib
	$(AR) ..$(DIRSEP)$(SUPPL).lib @objlist.txt $(LIBLIST)..$(DIRSEP)$(SUPPL).lst
