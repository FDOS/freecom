/* $Id$
	Declarations for messages

*/

#ifndef __MESSAGE_H_
#define __MESSAGE_H_

void displayString(unsigned id,...);
void displayError(unsigned id,...);
int userprompt(unsigned id,...);
int getPromptString(unsigned id, char ** const chars, char ** const fmt);
#define freePromptString(chars,fmt)	free(chars)
int mapMetakey(const char * const chars, int ch);
char *getMessage(const unsigned id);
char *defaultMessage(const unsigned id);
char *getString(const unsigned id);
void unloadMsgs(void);
unsigned msgSegment(void);

#endif
