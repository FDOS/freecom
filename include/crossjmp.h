/*
 * Allows to longjmp through FreeCOM at abort-type error states.
 *
 */

#ifndef FREECOM_CROSSJMP_H
#define FREECOM_CROSSJMP_H

#include <setjmp.h>

extern jmp_buf jmp_beginning;

#define jmp_fatal(err) longjmp(jmp_beginning, (err))

#endif
