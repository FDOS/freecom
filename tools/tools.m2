
error:
	errlvl 127

icmd : load_icd.exe icmd_1.icd icmd_2.icd icmd_3.icd

#		*Implicit Rules*
.asm.com:
	$(NASM) $(NASMFLAGS) -f bin -o $*.com  $<
#.asm.obj:
#	$(NASM) $(NASMFLAGS) -f obj $<

icmd_1.icd : icmd_inc.inc icmd_1.nas
	$(NASM) $(NASMFLAGS) -o icmd_1.icd icmd_1.nas

icmd_2.icd : icmd_inc.inc icmd_2.nas
	$(NASM) $(NASMFLAGS) -o icmd_2.icd icmd_2.nas

icmd_3.icd : icmd_inc.inc icmd_3.nas
	$(NASM) $(NASMFLAGS) -o icmd_3.icd icmd_3.nas
