/*
 * Allows to longjmp through FreeCOM at abort-type error states.
 *
 * 2000/12/10 ska
 *	started
 */

#ifndef FREECOM_CROSSJMP_H
#define FREECOM_CROSSJMP_H

#include <setjmp.h>

extern jmp_buf jmp_beginning;

#endif
