CFG_DEPENDENCIES = makefile.mak

TOP = ../..
!include "$(TOP)/config.mak"

CFG:=../$(CFG)

all : strings.lib

OBJ1 =	str0000.obj\
	str0001.obj\
	str0002.obj\
	str0003.obj\
	str0004.obj\
	str0005.obj\
	str0006.obj\
	str0007.obj
OBJ2 =	str0008.obj\
	str0009.obj\
	str000a.obj\
	str000b.obj\
	str000c.obj\
	str000d.obj\
	str000e.obj\
	str000f.obj
OBJ3 =	str0010.obj\
	str0011.obj\
	str0012.obj\
	str0013.obj\
	str0014.obj\
	str0015.obj\
	str0016.obj\
	str0017.obj
OBJ4 =	str0018.obj\
	str0019.obj\
	str001a.obj\
	str001b.obj\
	str001c.obj\
	str001d.obj\
	str001e.obj\
	str001f.obj
OBJ5 =	str0020.obj\
	str0021.obj\
	str0022.obj\
	str0023.obj\
	str0024.obj\
	str0025.obj\
	str0026.obj\
	str0027.obj
OBJ6 =	str0028.obj\
	str0029.obj\
	str002a.obj\
	str002b.obj\
	str002c.obj\
	str002d.obj\
	str002e.obj\
	str002f.obj
OBJ7 =	str0030.obj\
	str0031.obj\
	str0032.obj\
	str0033.obj\
	str0034.obj\
	str0035.obj\
	str0036.obj\
	str0037.obj
OBJ8 =	str0038.obj\
	str0039.obj\
	str003a.obj\
	str003b.obj\
	str003c.obj\
	str003d.obj\
	str003e.obj\
	str003f.obj
OBJ9 =	str0040.obj\
	str0041.obj\
	str0042.obj\
	str0043.obj\
	str0044.obj\
	str0045.obj\
	str0046.obj\
	str0047.obj
OBJ10 =	str0048.obj\
	str0049.obj\
	str004a.obj\
	str004b.obj\
	str004c.obj\
	str004d.obj\
	str004e.obj\
	str004f.obj
OBJ11 =	str0050.obj\
	str0051.obj\
	str0052.obj\
	str0053.obj\
	str0054.obj\
	str0055.obj\
	str0056.obj\
	str0057.obj
OBJ12 =	str0058.obj\
	str0059.obj\
	str005a.obj\
	str005b.obj\
	str005c.obj\
	str005d.obj\
	str005e.obj\
	str005f.obj
OBJ13 =	str0060.obj\
	str0061.obj\
	str0062.obj\
	str0063.obj\
	str0064.obj\
	str0065.obj\
	str0066.obj\
	str0067.obj
OBJ14 =	str0068.obj\
	str0069.obj\
	str006a.obj\
	str006b.obj\
	str006c.obj\
	str006d.obj\
	str006e.obj\
	str006f.obj
OBJ15 =	str0070.obj\
	str0071.obj\
	str0072.obj\
	str0073.obj\
	str0074.obj\
	str0075.obj\
	str0076.obj\
	str0077.obj
OBJ16 =	str0078.obj\
	str0079.obj\
	str007a.obj\
	str007b.obj\
	str007c.obj\
	str007d.obj\
	str007e.obj\
	str007f.obj
OBJ17 =	str0080.obj\
	str0081.obj\
	str0082.obj\
	str0083.obj\
	str0084.obj\
	str0085.obj\
	str0086.obj\
	str0087.obj
OBJ18 =	str0088.obj\
	str0089.obj\
	str008a.obj\
	str008b.obj\
	str008c.obj\
	str008d.obj\
	str008e.obj\
	str008f.obj
OBJ19 =	str0090.obj\
	str0091.obj\
	str0092.obj\
	str0093.obj\
	str0094.obj\
	str0095.obj\
	str0096.obj\
	str0097.obj
OBJ20 =	str0098.obj\
	str0099.obj\
	str009a.obj\
	str009b.obj\
	str009c.obj\
	str009d.obj\
	str009e.obj\
	str009f.obj
OBJ21 =	str00a0.obj\
	str00a1.obj\
	str00a2.obj\
	str00a3.obj\
	str00a4.obj\
	str00a5.obj\
	str00a6.obj\
	str00a7.obj
OBJ22 =	str00a8.obj\
	str00a9.obj\
	str00aa.obj\
	str00ab.obj\
	str00ac.obj\
	str00ad.obj\
	str00ae.obj\
	str00af.obj
OBJ23 =	str00b0.obj\
	str00b1.obj\
	str00b2.obj\
	str00b3.obj\
	str00b4.obj\
	str00b5.obj\
	str00b6.obj\
	str00b7.obj
OBJ24 =	str00b8.obj\
	str00b9.obj\
	str00ba.obj\
	str00bb.obj\
	str00bc.obj\
	str00bd.obj\
	str00be.obj\
	str00bf.obj
OBJ25 =	str00c0.obj\
	str00c1.obj\
	str00c2.obj\
	str00c3.obj\
	str00c4.obj\
	str00c5.obj\
	str00c6.obj\
	str00c7.obj
OBJ26 =	str00c8.obj\
	str00c9.obj\
	str00ca.obj\
	str00cb.obj\
	str00cc.obj\
	str00cd.obj\
	str00ce.obj\
	str00cf.obj
OBJ27 =	str00d0.obj\
	str00d1.obj\
	str00d2.obj\
	str00d3.obj\
	str00d4.obj\
	str00d5.obj\
	str00d6.obj\
	str00d7.obj
OBJ28 =	str00d8.obj\
	str00d9.obj\
	str00da.obj\
	str00db.obj\
	str00dc.obj\
	str00dd.obj\
	str00de.obj\
	str00df.obj
OBJ29 =	str00e0.obj\
	str00e1.obj\
	str00e2.obj\
	str00e3.obj\
	str00e4.obj\
	str00e5.obj\
	str00e6.obj\
	str00e7.obj
OBJ30 =	str00e8.obj\
	str00e9.obj\
	str00ea.obj\
	str00eb.obj\
	str00ec.obj\
	str00ed.obj\
	str00ee.obj\
	str00ef.obj
OBJ31 =	str00f0.obj\
	str00f1.obj\
	str00f2.obj\
	str00f3.obj\
	str00f4.obj\
	str00f5.obj\
	str00f6.obj\
	str00f7.obj
OBJ32 =	str00f8.obj\
	str00f9.obj\
	str00fa.obj\
	str00fb.obj\
	str00fc.obj\
	str00fd.obj\
	str00fe.obj

strings.lib: ../strings.h ../strings.err $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) \
$(OBJ6) $(OBJ7) $(OBJ8) $(OBJ9) $(OBJ10) $(OBJ11) $(OBJ12) $(OBJ13) $(OBJ14) $(OBJ15) \
$(OBJ16) $(OBJ17) $(OBJ18) $(OBJ19) $(OBJ20) $(OBJ21) $(OBJ22) $(OBJ23) $(OBJ24) $(OBJ25) \
$(OBJ26) $(OBJ27) $(OBJ28) $(OBJ29) $(OBJ30) $(OBJ31) $(OBJ32)
	echo Making STRINGS library
	$(RMFILES2) strings.lib
	$(AR) strings.lib @strings.rsp $(LIBLIST) strings.lst
	$(CP) strings.lib ..
	$(CP) strings.lst ..
