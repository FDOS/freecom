CFG_DEPENDENCIES = strings.mak

TOP=../..
!include "$(TOP)/config.mak"

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
	str000A.obj\
	str000B.obj\
	str000C.obj\
	str000D.obj\
	str000E.obj\
	str000F.obj
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
	str001A.obj\
	str001B.obj\
	str001C.obj\
	str001D.obj\
	str001E.obj\
	str001F.obj
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
	str002A.obj\
	str002B.obj\
	str002C.obj\
	str002D.obj\
	str002E.obj\
	str002F.obj
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
	str003A.obj\
	str003B.obj\
	str003C.obj\
	str003D.obj\
	str003E.obj\
	str003F.obj
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
	str004A.obj\
	str004B.obj\
	str004C.obj\
	str004D.obj\
	str004E.obj\
	str004F.obj
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
	str005A.obj\
	str005B.obj\
	str005C.obj\
	str005D.obj\
	str005E.obj\
	str005F.obj
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
	str006A.obj\
	str006B.obj\
	str006C.obj\
	str006D.obj\
	str006E.obj\
	str006F.obj
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
	str007A.obj\
	str007B.obj\
	str007C.obj\
	str007D.obj\
	str007E.obj\
	str007F.obj
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
	str008A.obj\
	str008B.obj\
	str008C.obj\
	str008D.obj\
	str008E.obj\
	str008F.obj
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
	str009A.obj\
	str009B.obj\
	str009C.obj\
	str009D.obj\
	str009E.obj\
	str009F.obj
OBJ21 =	str00A0.obj\
	str00A1.obj\
	str00A2.obj\
	str00A3.obj\
	str00A4.obj\
	str00A5.obj\
	str00A6.obj\
	str00A7.obj
OBJ22 =	str00A8.obj\
	str00A9.obj\
	str00AA.obj\
	str00AB.obj\
	str00AC.obj\
	str00AD.obj\
	str00AE.obj\
	str00AF.obj
OBJ23 =	str00B0.obj\
	str00B1.obj\
	str00B2.obj\
	str00B3.obj\
	str00B4.obj\
	str00B5.obj\
	str00B6.obj\
	str00B7.obj
OBJ24 =	str00B8.obj\
	str00B9.obj\
	str00BA.obj\
	str00BB.obj\
	str00BC.obj\
	str00BD.obj\
	str00BE.obj\
	str00BF.obj
OBJ25 =	str00C0.obj\
	str00C1.obj\
	str00C2.obj\
	str00C3.obj\
	str00C4.obj\
	str00C5.obj\
	str00C6.obj\
	str00C7.obj
OBJ26 =	str00C8.obj\
	str00C9.obj\
	str00CA.obj\
	str00CB.obj\
	str00CC.obj\
	str00CD.obj\
	str00CE.obj\
	str00CF.obj
OBJ27 =	str00D0.obj\
	str00D1.obj\
	str00D2.obj\
	str00D3.obj\
	str00D4.obj\
	str00D5.obj\
	str00D6.obj\
	str00D7.obj
OBJ28 =	str00D8.obj\
	str00D9.obj\
	str00DA.obj\
	str00DB.obj\
	str00DC.obj\
	str00DD.obj\
	str00DE.obj\
	str00DF.obj
OBJ29 =	str00E0.obj\
	str00E1.obj\
	str00E2.obj\
	str00E3.obj\
	str00E4.obj\
	str00E5.obj\
	str00E6.obj\
	str00E7.obj
OBJ30 =	str00E8.obj\
	str00E9.obj\
	str00EA.obj\
	str00EB.obj\
	str00EC.obj\
	str00ED.obj\
	str00EE.obj\
	str00EF.obj
OBJ31 =	str00F0.obj\
	str00F1.obj\
	str00F2.obj\
	str00F3.obj\
	str00F4.obj\
	str00F5.obj\
	str00F6.obj\
	str00F7.obj
OBJ32 =	str00F8.obj\
	str00F9.obj\
	str00FA.obj\
	str00FB.obj\
	str00FC.obj\
	str00FD.obj\
	str00FE.obj

strings.lib: ../strings.h ../strings.err $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) \
$(OBJ6) $(OBJ7) $(OBJ8) $(OBJ9) $(OBJ10) $(OBJ11) $(OBJ12) $(OBJ13) $(OBJ14) $(OBJ15) \
$(OBJ16) $(OBJ17) $(OBJ18) $(OBJ19) $(OBJ20) $(OBJ21) $(OBJ22) $(OBJ23) $(OBJ24) $(OBJ25) \
$(OBJ26) $(OBJ27) $(OBJ28) $(OBJ29) $(OBJ30) $(OBJ31) $(OBJ32)
	echo Making STRINGS library
	ECHO $(CL) -c *.c
	$(RMFILES2) strings.lib
	$(AR) strings.lib @strings.rsp $(LIBLIST) strings.lst
	$(CP) strings.lib ..
	$(CP) strings.lst ..
