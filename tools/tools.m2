
!include "..\config.mak"

error:
	errlvl 127

icmd : load_icd.exe icmd_1.icd icmd_2.icd icmd_3.icd

#		*Implicit Rules*
.c.exe:
  $(CC) $(CFLAGS) $<  $(SUPPL_LIB_PATH)\SUPPL_S.LIB
.c.obj:
  $(CC) $(CFLAGS) -c {$< }
.asm.com:
	$(NASM) $(NASMFLAGS) -f bin -o $*.com  $<
#.asm.obj:
#	$(NASM) $(NASMFLAGS) -f obj $<
.nas.icd:
	$(NASM) $(NASMFLAGS) -o $&.icd $<

icmd_1.icd : icmd_inc.inc icmd_1.nas

icmd_2.icd : icmd_inc.inc icmd_2.nas

icmd_3.icd : icmd_inc.inc icmd_3.nas
