/*
 *	Special FreeCOM declaration for the resource management
 *
 *	enumResources()	enumerates all resources of the specified major ID
 *		and invokes a callback function; if the primary resource file
 *		is not avilable, the passe din alternate file is tried.
 */

#ifndef RES_H
#define RES_H

#include "../include/resource.h"

int enumResources(res_majorid_t id, int (*fct)(), void * const arg);

#endif
