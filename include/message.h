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
char *getMessage(unsigned id);
char *defaultMessage(unsigned id);
char *getString(unsigned id);
void unloadMsgs(void);
unsigned msgSegment(void);

#endif
