/*
 *	Defines keys without ASCII representation
 *
 *	Internally a key is an (int) value, the low byte encodes the keycode
 *	the high byte the mode the keycode has to be interpreted by.
 *	mode	meaning of keycode
 *	0		ASCII code
 *	1		scan code
 */

#ifndef KEYS__H
#define KEYS__H

#define SCANCODE(key)	(key | 256)
#define ASCIICODE(key)	(key)

#define KEY_F1	SCANCODE(59)
#define KEY_F2	SCANCODE(60)
#define KEY_F3	SCANCODE(61)
#define KEY_F4	SCANCODE(62)
#define KEY_F5	SCANCODE(63)
#define KEY_F6	SCANCODE(64)
#define KEY_F7	SCANCODE(65)
#define KEY_F8	SCANCODE(66)
#define KEY_F9	SCANCODE(67)
#define KEY_F10	SCANCODE(68)
#define KEY_F11	SCANCODE(133)
#define KEY_F12	SCANCODE(134)

#define KEY_LEFT	SCANCODE(75)
#define KEY_CTRL_LEFT	SCANCODE(0x73)
#define KEY_RIGHT	SCANCODE(77)
#define KEY_CTRL_RIGHT	SCANCODE(0x74)
#define KEY_UP	SCANCODE(72)
#define KEY_DOWN	SCANCODE(80)
#define KEY_INS	SCANCODE(82)
#define KEY_INSERT	SCANCODE(82)
#define KEY_DEL	SCANCODE(83)
#define KEY_DELETE	SCANCODE(83)
#define KEY_HOME	SCANCODE(71)
#define KEY_END	SCANCODE(79)
#define KEY_PUP	SCANCODE(73)
#define KEY_PDOWN	SCANCODE(81)

#define KEY_TAB	ASCIICODE('\t')
#define KEY_NL	ASCIICODE('\n')
#define KEY_CR	ASCIICODE('\r')
#define KEY_ENTER	ASCIICODE('\r')
#define KEY_BS	ASCIICODE('\b')
#define KEY_ESC	ASCIICODE(27)

#define KEY_CTL_A	ASCIICODE(1)
#define KEY_CTL_B	ASCIICODE(2)
#define KEY_CTL_C	ASCIICODE(3)
#define KEY_CTL_D	ASCIICODE(4)
#define KEY_CTL_E	ASCIICODE(5)
#define KEY_CTL_F	ASCIICODE(6)
#define KEY_CTL_G	ASCIICODE(7)
#define KEY_CTL_H	ASCIICODE(8)
#define KEY_CTL_I	ASCIICODE(9)
#define KEY_CTL_J	ASCIICODE(10)
#define KEY_CTL_K	ASCIICODE(11)
#define KEY_CTL_L	ASCIICODE(12)
#define KEY_CTL_M	ASCIICODE(13)
#define KEY_CTL_N	ASCIICODE(14)
#define KEY_CTL_O	ASCIICODE(15)
#define KEY_CTL_P	ASCIICODE(16)
#define KEY_CTL_Q	ASCIICODE(17)
#define KEY_CTL_R	ASCIICODE(18)
#define KEY_CTL_S	ASCIICODE(19)
#define KEY_CTL_T	ASCIICODE(20)
#define KEY_CTL_U	ASCIICODE(21)
#define KEY_CTL_V	ASCIICODE(22)
#define KEY_CTL_W	ASCIICODE(23)
#define KEY_CTL_X	ASCIICODE(24)
#define KEY_CTL_Y	ASCIICODE(25)
#define KEY_CTL_Z	ASCIICODE(26)

#endif
