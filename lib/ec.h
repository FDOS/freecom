
#include <stdarg.h>

#define EC_CMD_HIDDEN	(EC_CMD_FORCE_INTERNAL | EC_CMD_NO_TRACE	\
						 | EC_CMD_NONINTERACTIVE)

int ecMkvcmd(const int mode, const char * const str, va_list ap);
