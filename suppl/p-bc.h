/* Portable include file for Turbo/Borland C */
#ifdef __TURBOC__


/* Global argc/argv variables */
#define G_ARGV _argv		/* global char **argv	variable */
#define G_ARGC _argc		/* global int argc	variable */
#define G_ARG_DEF			/* definition of global variables */
#define G_ARG_INIT			/* initialization of global variables */

/* byte/word/dword type */
typedef unsigned char byte;
typedef unsigned word;
typedef int FLAG;	/* no special 8-bit handling */
typedef unsigned FLAG8;	/* no special 8-bit handling */
typedef unsigned long dword;

/* peekb() && pokeb() exist */
#define peekw peek
#define pokew poke

#define _osversion MK_OSVERS(_osmajor, _osminor)

/* get/set interrupt vecotr */
typedef void interrupt (far *isr)();
#define set_isrfct(num,fct) setvect((num), (isr)(fct))
#define get_isr(num,var) ((var) = getvect(num))
#define set_isr(num,var) setvect((num), (var))

/* calling interrupt */
typedef struct REGPACK IREGS;
#define intrpt intr


#endif
