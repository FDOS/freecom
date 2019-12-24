#ifndef ISKANJI_H
#define ISKANJI_H

#ifdef JAPANESE
 #define iskanji(c)		(((unsigned char)(c) >= 0x81 && (unsigned char)(c) <= 0x9f) || ((unsigned char)(c) >= 0xe0 && (unsigned char)(c) <= 0xfc))
 #define iskanji2(c)	((unsigned char)(c) >= 0x40 && (unsigned char)(c) <= 0xfc && (unsigned char)(c) != 0x7f)
#else
 #define iskanji(c)		(0)
 #define iskanji2(c)	(0)
#endif

#endif
