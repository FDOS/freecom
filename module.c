/*
 *	Special FreeCOM defuinitions for the module management
 *
 * 2000/07/12 ska
 * started
 */

#include <string.h>
#include <mcb.h>
#include <suppl.h>

#include "debug.h"
#include "module.h"
#include "res.h"
#include "misc.h"

#define NONE (unsigned long)-1
const char mcbToken[] = "FCOM_ERR";

typedef struct {
	WORD *segm;
		/* Usually the CRITER module has separated code and string portion */
	unsigned long code;		/* CRITER code */
	unsigned long strings;	/* CRITER strings */
	unsigned codeLength, stringsLength;
	FILE *f;		/* file the resources reside in */
} critData;

#pragma argsused
static int loadModule(res_majorid_t major
	, res_minorid_t minor
	, unsigned long length
	, FILE* f
	, void *arg)
{	critData *d;
	WORD segm, far*p;
	int i;

	d = arg;
	if(d->f != f) {
		/* File has been changed */
		d->code = d->strings = NONE;
		d->f = f;
	}
	if(length > 0xfffful) {
		dprintf(("[CRITER resource too large.]\n"));
		return 0;
	}
	switch(minor) {
	case 0x00:		/* CRITER code */
		d->code = ftell(f);
		d->codeLength = (unsigned)length;
		if(d->strings == NONE)
			break;
	loadSeparated:		/* load a separated code/data CRITER module */
		if((long)d->codeLength + (long)d->stringsLength > 0xffffl) {
			dprintf(("[CRITER module too large.]\n"));
			return 0;
		}
		if((segm = allocBlk(d->codeLength + d->stringsLength, 0x82)) == 0) {
			dprintf(("[Out of memory loading CRITER module.]\n"));
			return 0;
		}
		if(fseek(f, d->code, SEEK_SET)
		 || farread(MK_FP(segm, 0), d->codeLength, f) != d->codeLength
		 || fseek(f, d->strings, SEEK_SET)
		 || farread(MK_FP(segm, d->codeLength), d->stringsLength, f)
		  != d->stringsLength) {
		  	dprintf(("[Error reading CRITER module.]\n"));
		  	freeBlk(segm);
			return 0;
		}
		/* The CRITER strings resource includes offsets relative to its
			beginning, though, the code resource requires real offsets. */
		i = *(BYTE far*)MK_FP(segm, d->codeLength);
		p = (WORD far*)MK_FP(segm, d->codeLength + 1);
		while(i--)
			*p++ += d->codeLength;
		/* Modify the MCB name to include the magic token of FreeCOM's
			CRITER module */
		_fmemcpy(MK_FP(SEG2MCB(segm), 8), (char far*)mcbToken, 8);
		((struct MCB far*)MK_FP(SEG2MCB(segm), 0))->mcb_ownerPSP = 8;
		/* Both resources loaded --> done */
		*d->segm = segm;
		return 1;
	case 0x01:		/* CRITER strings */
		d->strings = ftell(f);
		d->stringsLength = (unsigned)length;
		if(d->code != NONE)		/* both found already? */
			goto loadSeparated;
		break;
	case 0x02:		/* CRITER code and strings merged together */
		if((segm = allocBlk((unsigned)length, 0x82)) == 0) {
			dprintf(("[Out of memory loading CRITER module.]\n"));
			return 0;
		}
		if(farread(MK_FP(segm, 0), (unsigned)length, f) != (unsigned)length) {
		  	dprintf(("[Error reading CRITER module.]\n"));
		  	freeBlk(segm);
			return 0;
		}
		/* Modify the MCB name to include the magic token of FreeCOM's
			CRITER module */
		_fmemcpy(MK_FP(SEG2MCB(segm), 8), (char far*)mcbToken, 8);
		((struct MCB far*)MK_FP(SEG2MCB(segm), 0))->mcb_ownerPSP = 8;
		/* Both resource loaded --> done */
		*d->segm = segm;
		return 1;
	}

	return 0;		/* Other minor IDs are happily ignored */
}

static int isModule(void *arg, unsigned segm)
{	if(_fmemcmp((char far*)arg, MK_FP(segm, 8), 8) == 0
	 && ((struct MCB far*)MK_FP(SEG2MCB(segm), 0))->mcb_ownerPSP == 8)
		return segm;
	return 0;
}

/* Returns the pointer to the context pointer of the Critical Error
	handler and, if not loaded so far, loads it */
context_t far* far* modCriter(void)
{	WORD segm;

	/* Locate an already loaded CRITER module */
	if((segm = mcb_walk(0, isModule, mcbToken)) == 0) {
		/* Load it into memory */
		critData d;

		d.segm = &segm;
		d.f = NULL;
		enumResources("CRITER", RES_ID_CRITER, loadModule, &d);
#ifdef DEBUG
		if(segm)
			dprintf(("[CRITER loaded to segment 0x%04x.]\n", segm));
#endif

	}
#ifdef DEBUG
	else
		dprintf(("[Re-using CRITER located at segment 0x%04x.]\n", segm));
#endif
	return (context_t far* far*)MK_FP(segm, 0);
}
