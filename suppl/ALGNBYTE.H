/* one byte alignment */
/* do not use with Micro-C ! */

#if defined(__WATCOMC__)
#if __WATCOMC__ > 1000
#pragma pack(__push,1);
#else
#pragma pack(1);
#endif
#elif defined(_MSC_VER) || defined(_QC)
    #pragma pack(1)
#elif defined(__ZTC__)
    #pragma ZTC align 1
#elif defined(__TURBOC__) && (__TURBOC__ > 0x202)
    #pragma option -a-
#endif
