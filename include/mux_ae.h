/*
	Declaration for Installable COMMAND extensions (MUX-AE)

	2000/12/10 ska
	started
*/

#ifndef FREECOM_MUX_AE_H
#define FREECOM_MUX_AE_H

#include "../include/command.h"

/* minimal size of buffer passed into runExtension() for both
	command and line. Also, command[-2] & command[-1] must be available as
	well as line[-2], line[-1] and line[BUFFER_SIZE_MUX_AE] */
#if MAX_INTERNAL_COMMAND_SIZE > 255
#define BUFFER_SIZE_MUX_AE 255
#else
#define BUFFER_SIZE_MUX_AE MAX_INTERNAL_COMMAND_SIZE
#endif

/* Check for extensions and perform it, on success return != 0 */
/* Also: The contents of both buffers is monitored in order to
	check if they got overflowed during the MUX call. If so,
	the executation is aborted. */
int runExtension(char * const command, char * const line);

#endif
