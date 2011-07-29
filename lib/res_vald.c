/* $Id$

	Returns whether the passed-in file is a probably
	valid resource file.

	0 -> is valid
	1 -> no such file
	2 -> is a device
	3 -> not valid

*/

#include "../config.h"

#include <assert.h>
#include <fcntl.h>
#include <io.h>

#include "../include/command.h"
#include "../include/misc.h"
#include "../include/openf.h"
#include "../include/resource.h"
#include "../include/strings.typ"

/* Is called only, if the Strings resource had been found,
	hence, returning TRUE alsways. */

static int test_fct (res_majorid_t major,
		     res_minorid_t minor,
		     long length,
		     int fd,
		     void *const arg) {
	(void)major, (void)length, (void)fd, (void)arg;
	assert(major == RES_ID_STRINGS);
	return minor == STRING_RESOURCE_MINOR_ID? 1 : 2;
}

int validResFile(const char * const fnam)
{	int fd;

	assert(fnam);

	if((fd = dos_open(fnam, O_RDONLY)) < 0)
		return 1;

	if(isadev(fd)) {
		dos_close(fd);
		return 2;
	}

	dos_close(fd);
	if(enumFileResources(fnam
	 , RES_ID_STRINGS, test_fct, (void*)0) != 1)
		return 3;

	return 0;
}
