#include "out.h"

extern int oPage;				/* Active console page;
									if == -1, no console is attached */
extern int oColMax;				/* Forced maximum column;
									if == 0, unlimited */
extern int oRowMax;				/* Forced maximum row;
									if == 0, unlimited */

#define iPage oPage
#define oColMax iColMax
#define oRowMax iRowMax

