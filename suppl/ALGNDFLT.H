
/* set structure alignment to default */
/* do not use with Micro-C ! */

#if defined(__WATCOMC__)
#if __WATCOMC__ > 1000
#pragma pack(__pop);
#else
#pragma pack();
#endif
#elif defined (_MSC_VER) || defined(_QC)
 #pragma pack()
#elif defined (__ZTC__)
 #pragma ZTC align
#elif defined(__TURBOC__) && (__TURBOC__ > 0x202)
 #pragma option -a.
#endif
