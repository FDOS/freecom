
#include <stdarg.h>

#define EC_CMD_HIDDEN 1
#define EC_CMD_SILENT 2
#define EC_CMD_IGNORE 4

int ecMkvcmd(const int mode, const char * const str, va_list ap);
