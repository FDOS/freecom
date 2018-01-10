/* $Id$
 *  VERIFY.C - verify command.
 *
 *  Comments:
 *
 * 31 Jul 1998 (John P Price)
 *   started.
 *
 *
 */

#include "../config.h"

#include <assert.h>
#include <dos.h>

#include "../err_fcts.h"
#include "../include/command.h"
#include "../strings.h"

#ifdef __WATCOMC__
void setverify(char a);
#pragma aux setverify = \
	"mov ah,0x2e" \
	"mov dl,0" \
	"int 0x21" \
	parm [al] modify exact [ah dl]
char getverify(void);
#pragma aux getverify = \
	"mov ah,0x54" \
	"int 0x21" \
	value [al] modify exact [ah]
#endif

#ifdef __GNUC__
static inline void setverify(char a)
{
	asm volatile("int $0x21":: "Rah"((char)0x2e), "Rdl"((char)0), "Ral"(a));
}

static inline char getverify(void)
{
	char ret;
	asm volatile("int $0x21": "=Ral"(ret): "Rah"((char)0x54));
	return ret;
}
#endif

int cmd_verify(char *param)
{
  switch(onoffStr(param)) {
  	default:
		error_on_off();
		return 1;
	case OO_Null:	case OO_Empty:
		displayString(TEXT_MSG_VERIFY_STATE, getverify() ? D_ON : D_OFF);
		break;
  	case OO_Off:	setverify(0);	break;
  	case OO_On:		setverify(1);	break;
	}
  return 0;
}
