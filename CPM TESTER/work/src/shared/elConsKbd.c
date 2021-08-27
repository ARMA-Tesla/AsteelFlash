/*
 *  "elConsKbd.c" - Console keyboard routines.
 *
 *	(c) 2007 ELCOM, a.s.
 *		All Rights Reserved.
 *
 *	The information  in this file  is provided for the exclusive use of
 *	the  licensees  of  ELCOM, a.s.  Such users have the right  to use,
 *	modify,  and incorporate this code into other products for purposes
 *	authorized  by the license  agreement  provided they  include  this
 *	notice  and the associated copyright notice  with any such product.
 *	The information in this file  is provided "AS IS" without warranty.
 *
 *
 * Revision History:
 * -----------------
 *	2007-08-04	Radomir Tomis (Radomir.Tomis@dvi.elcom.cz)
 *		Created from "cons_kbd.c".
 *	2002-08-10	Radomir Tomis
 *		Created/extracted from "rs232.c".
 */

#include <elBase.h>
#include <elLog.h>
#include <elConsKbd.h>

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elConsKbdHandle"
HANDLE elConsKbdHandle(void)
{
	static HANDLE fh = INVALID_HANDLE_VALUE;

	if ( fh == INVALID_HANDLE_VALUE )
	{
		fh = CreateFile("CONIN$",
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,			// pointer to security attributes 
 			OPEN_EXISTING,	// how to create 
			0,				// file attributes 
			0				// handle to file with attributes to copy 
		); 
		if ( fh == INVALID_HANDLE_VALUE )
			elLog("%s(): CreateFile(\"CONIN$\"): %W",__FUNC__);
	}
	return fh;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elConsKbdGetKey"
elConsKbdKey_t elConsKbdGetKey(void)
{
	static struct {
		int vkey;		/* WIN virtual key */
		elConsKbdKey_t kbd_key;
	} kmap[] = {
#define K(id)	EL_CONS_KBD_KEY_##id
		VK_INSERT,		K(INSERT),
		VK_DELETE,		K(DELETE),
		VK_HOME,		K(HOME),
		VK_END,			K(END),
		VK_NEXT,		K(PGDN),
		VK_PRIOR,		K(PGUP),
		VK_UP,			K(UP),
		VK_DOWN,		K(DOWN),
		VK_LEFT,		K(LEFT),
		VK_RIGHT,		K(RIGHT),
#if 0
		VK_BTAB,		K(BTAB),
#endif
		/*
		 * The following entries are defined in order to process
		 * 'Shift+Ctrl+<key>' combinations which returns zero value
		 * in 'uChar.AsciiChar':
		 */
		VK_SPACE,		' ',
		VK_RETURN,		K(ENTER),
#undef K
	};
	INPUT_RECORD rec;
	KEY_EVENT_RECORD *k;
	DWORD cnt;
	elConsKbdKey_t key;
	elConsKbdKeyState_t ks;
	register int i;
	
	if ( !ReadConsoleInput(elConsKbdHandle(),&rec,1,&cnt) )
	{
		elLog("%s(): ReadConsoleInput(): %M",__FUNC__);
		return (elConsKbdKey_t) -1;
	}
	if ( rec.EventType != KEY_EVENT )
		return 0;		/* not a keyboard event */
	if ( rec.Event.KeyEvent.bKeyDown != TRUE )
		return 0;		/* key release */
	k = &rec.Event.KeyEvent;
	key = k->uChar.AsciiChar;
#if 0
	// LogD("ReadConsoleInput(): key = %d\n",key);
	LogD(
"AsciiChar: %d, VirtualKeyCode: %d, VirtualScanCode: %d, ControlKeyState: %lu",
		key,k->wVirtualKeyCode,k->wVirtualScanCode,k->dwControlKeyState);
#endif
	/*
	 * <Shift|Ctrl|Alt>+<Enter> must be mapped to 'KBD_KEY_ENTER + 'KBD_KS_...':
	 */
	if ( !key || k->wVirtualKeyCode == VK_RETURN )
	{
		for(i = 0;  i < EL_ARRAY_CNT(kmap) && kmap[i].vkey != k->wVirtualKeyCode; i++);
		if ( i < EL_ARRAY_CNT(kmap) )
			key = kmap[i].kbd_key;
		else if ( k->wVirtualKeyCode >= VK_F1 && k->wVirtualKeyCode <= VK_F12 )
			key = EL_CONS_KBD_KEY_F(k->wVirtualKeyCode - VK_F1 + 1);
	}
	
	/*
	 * Shift/Ctrl/Alt state and LED indicators Num/Caps/Scroll:
	 */
	ks = 0;

	if ( k->dwControlKeyState & SHIFT_PRESSED )
		ks |= EL_CONS_KBD_KS_SHIFT;
	if ( k->dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED) )
		ks |= EL_CONS_KBD_KS_CTRL;
	/*
	 * NOTICE:
	 *
	 *	If the czech keyboard layout is turned on, then the right <Alt> acts
	 *	as left <Ctrl> + left <Alt> pressed together and it cannot be
	 *	distinguished.
	 */
	if ( k->dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED) )
		ks |= EL_CONS_KBD_KS_ALT;

	if ( k->dwControlKeyState & CAPSLOCK_ON )
		ks |= EL_CONS_KBD_LS_CAPS;
	if ( k->dwControlKeyState & NUMLOCK_ON )
		ks |= EL_CONS_KBD_LS_NUM;
	if ( k->dwControlKeyState & SCROLLLOCK_ON )
		ks |= EL_CONS_KBD_LS_SCROLL;

	return key | ks;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elConsKbdGetKeyPeek"
elConsKbdKey_t elConsKbdGetKeyPeek(void)
{
	INPUT_RECORD rec;
	DWORD cnt;

	if ( !PeekConsoleInput(elConsKbdHandle(),&rec,1,&cnt) )
	{
		elLog("%s(): PeekConsoleInput(): %M",__FUNC__);
		return (elConsKbdKey_t) -1;
	}
#if 0
	if ( cnt )
		LogD("%s(): cnt: %lu",__FUNC__,cnt);
#endif
	if ( cnt
#if 0
		&& rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown
#endif
	)
		return elConsKbdGetKey();
	else
		return 0;	/* no key press */
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elConsKbdKeyStr"
char *elConsKbdKeyStr(elConsKbdKey_t keyval)
{
	static byte keycode[] = {
		EL_CONS_KBD_KEY_SPACE, EL_CONS_KBD_KEY_BACKSPACE, EL_CONS_KBD_KEY_TAB,
		EL_CONS_KBD_KEY_ENTER, EL_CONS_KBD_KEY_ESC,
		0
	};
	static char *keystr[] = {
		"Space", "BackSpace", "Tab", "Enter", "Esc",
	};
	static char *fkeystr[] = { EL_CONS_KBD_KEY_STRs };
	static struct {
		elConsKbdKeyState_t id;
		char *str;
	} key_ks[] = {	/* key state */
		EL_CONS_KBD_KS_SHIFT,	"Shift",
		EL_CONS_KBD_KS_CTRL,	"Ctrl",
		EL_CONS_KBD_KS_ALT,		"Alt",
	};
	static char str[40+EL_ST];
	elConsKbdKeyState_t ks = keyval & EL_CONS_KBD_KS_MASK;
	register char *p;
	register int i;

	keyval &= EL_CONS_KBD_KEY_MASK;

	p = NULL;
	if ( !keyval )
		p = "Nul";
	else if ( keyval < EL_CONS_KBD_KEY_first
		&& (p=strchr((char *)keycode,keyval)) != NULL )
		p = keystr[p - (char *)keycode];
	else if ( keyval < EL_CONS_KBD_KEY_first )
	{
		if ( keyval < EL_CONS_KBD_KEY_SPACE )
		{
			keyval |= 0x40;
			if ( ks & EL_CONS_KBD_KS_CTRL )
				p = elStrTmpFmt("%c",keyval);
			else
				p = elStrTmpFmt("^%c",keyval);
		}
		else
		{
			p = elStrTmpFmt("%c",keyval);
			ks &= ~EL_CONS_KBD_KS_SHIFT;		/* zrušit "Shift+" */
			if ( keyval > 127 )
				ks &= ~EL_CONS_KBD_KS_ALT;	/* zrušit "Alt+" (napø. "Alt+è") */
		}
	}
	else if ( keyval >= EL_CONS_KBD_KEY_F(0) )
		p = elStrTmpFmt("F%d",keyval - EL_CONS_KBD_KEY_F(0));
	else
	{
		int	idx = keyval - EL_CONS_KBD_KEY_first;

		if ( idx < EL_ARRAY_CNT(fkeystr) )
			p = fkeystr[idx];
	}
	*str = '\0';
	for(i = 0; i < EL_ARRAY_CNT(key_ks); i++)
	{
		if ( ks & key_ks[i].id )
		{
			strcat(str,key_ks[i].str);
			strcat(str,"+");
		}
	}
	if ( p )
		strcat(str,p);

	return str;
}

/******************************************************************************/
