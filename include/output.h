/* $id$

	Header file centralizing all I/O functions

*/

#ifndef __OUTPUT_H_
#define __OUTPUT_H_

#define OUT_MODE_BRACKETS		0x8000	/* enclose in brackets */
#define OUT_MODE_JUSTIFY_LEFT	0
#define OUT_MODE_JUSTIFY_RIGHT	0x0001
#define OUT_MODE_JUSTIFY_CENTER	0x0002
/*#define OUT_MODE_JUSTIFY_STRETCH	0x0003	not implemented */
#define OUT_MODE_JUSTIFY_MASK	0x0003
#define OUT_MODE_PAD_BLANK		0
#define OUT_MODE_LINE_WRAP		0
#define OUT_MODE_NO_WRAP		0x0100	/* Try to not wrap output */
#define OUT_MODE_BLOCK_ENLARGE	0		/* if string > size, output full */
#define OUT_MODE_BLOCK_CUT		0x0080	/* cut after size characters */
#define OUT_MODE_BLOCK_CHUNK	0x0040	/* place string in N*size chunks */
#define OUT_MODE_BLOCK_MASK		0x00c0

	/* Reset all internal states */
void oInit(void);
	/* Flush any pending requests */
void oFlush(void);
	/* Make sure the current output line gets a \n */
void oFlushNL(void);
	/* Issue a \n if less than size characters are left on line */
void oNeedSpace(const int size);
	/* Issue a \n */
void oNL(void);
	/* Output a string, optionally enclosed in brackets, chunked in
		blocks of size characters. If the line gets filled, the string
		is not wrapped, but fully transferred to the next line. */
void oBlockString(const unsigned mode, const int size, const char * const str);
	/* Display a number of pad bytes according mode; spaces may be
		held back to be ignored if a NL follows immediately. */
void oPadChars(const unsigned mode, const int size);
	/* Add a pending blank */
void oBlank(void);
	/* Display a single character */
void oCh(const int ch);
	/* Display a sequence of characters */
void oMem(const char * const line, const int size);
	/* Display a string */
void oStr(const char * const line);
	/* Display a string from the resource */
void oID(const unsigned mode, const int size, const unsigned strID);

	/* Return if the input stream is connected to the console */
int iConsole(void);

#endif
