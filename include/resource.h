/*
 *	Declarations of resource management of FreeCOM
 *
 *	See DOCS\RESOURCE.TXT
 *
 *	enumFileResources()	enumerates all resources of the specified major ID
 *		and invokes a callback function
 *	startResource() prepares a stream that a new resources is about to
 *		be written into it
 *	endResource() cleans up the resource prepared by startResource()
 *
 */

#ifndef RESOURCE_H
#define RESOURCE_H

#include <stdio.h>

	/* one-byte alignment */
#pragma -a-
typedef unsigned res_minorid_t;	/* must be exactly unsigned 16bit */
		/* force 16bit enum */
#pragma -b
typedef enum {
	RES_ID_NONE = -0x7ffe,
	RES_ID_ANY = -0x7fff,
	RES_ID_STRINGS = 0,
	RES_ID_CRITER,
	RES_ID_INFO
} res_majorid_t;
#pragma -b.

typedef struct {		/* type of a control area */
	unsigned long res_length;
	res_majorid_t res_majorID;
	res_minorid_t res_minorID;
	unsigned char res_cookie[8];	/* NOT '\0' terminated! */
} resource_t;
	/* standard alignment */
#pragma -a.

typedef int (*res_callbackp_t)(res_majorid_t, res_minorid_t
 , unsigned long, FILE *, void *);

#define RES_COOKIE "FREECOM "


/****************************
 ****** Prototypes **********
 ****************************/


int enumFileResources(const char * const fnam, res_majorid_t id
 , int (*fct)(), void * const arg);
void startResource(FILE *f, res_majorid_t majorID, res_minorid_t minorID);
void endResource(FILE *f);

#if sizeof(res_majorid_t) != 2
#error "res_id_t must be exactly 16bit!"
#endif
#if sizeof(res_minorid_t) != 2
#error "res_id_t must be exactly 16bit!"
#endif

#endif
