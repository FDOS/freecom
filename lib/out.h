#include "../include/output.h"

extern unsigned oColumn;		/* current output column; == 0 -> none */
extern unsigned oPadSpaces;		/* how many padding spaces are pending */

extern int oPage;				/* Active console page;
									if == -1, no console is attached */
extern int oColMax;				/* Forced maximum column;
									if == 0, unlimited */
extern int oRowMax;				/* Forced maximum row;
									if == 0, unlimited */

	/* How many columns are left til line break */
int oColumnsLeft(void);
	/* Return if stdout is connected to the monitor */
int oConsole(void);
	/* Re-calculate oPage, oColMax, oRowMax */
void oGetScreenDimensions(void);
	/* Get the position of the cursor */
void oGetCursor(int * const x, int * const y);

#define oRowIncrement()
