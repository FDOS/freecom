/*
  Session management

  When an external command is to be executed the internal state
  of FreeCom changes:
    + internally cached values could be changed by the program,
      e.g. environment variables, dimension of the screen ...
    + some memory could have to be swapped in/out
    + the message segment is to be freed/reloaded

 * 1999/07/07 ska
 *  started
 */

#include "config.h"

#include "command.h"
#include "session.h"

  /* Shall the message block remain in memory when an external
    program is executed */
int persistentMSGs = 0;


/*
 *  Called before the external program is called
 */
int saveSession(void)
{
  if(!persistentMSGs)
    unloadMsgs();

  return 1;
}

int restoreSession(void)
{
  /* The messages are loaded automatically */

  return 1;
}
