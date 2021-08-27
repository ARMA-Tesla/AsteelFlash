/*
 *  <elConsKbd.h> - Definitions for module "elConsKbd.c".
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

#if _MSC_VER >= 1000
#pragma once
#endif 

#if !defined(_EL_CONS_KBD_H)
#define _EL_CONS_KBD_H

/* MS Visual Studio 2005: #ident ignored; Borland C/C++: Unknown preprocessor directive: 'ident' */
#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_
#if 1
#ident "@(#) elConsKbd.h 1.0 2002/08/10"
#else
#endif
#endif

#include <elBase.h>

/*
 * 'EL_CONS_KBD_...': Console keyboard key constants.
 */
enum EelConsKbdKey {
#define K(id)	EL_CONS_KBD_KEY_##id
	K(MASK)			= 0xFFFF,		/* 'EL_CONS_KBD_KEY_MASK' */

	K(SPACE)		=	' ',
	K(BACKSPACE)	=	'\b',
	K(TAB)			=	'\t',
	K(ENTER)		=	'\n',
	K(ESC)			=	0x1B,

	K(first)		= 1000,
	K(INSERT)		= K(first),
	K(DELETE),
	K(HOME),
	K(END),
	K(PGUP),
	K(PGDN),
	K(UP),
	K(DOWN),
	K(LEFT),
	K(RIGHT),
	K(BTAB),
	K(F0),
	K(F1),
	K(F2),
	K(F3),
	K(F4),
	K(F5),
	K(F6),
	K(F7),
	K(F8),
	K(F9),
	K(F10),
	K(F11),
	K(F12),
#undef K
};
#define EL_CONS_KBD_KEY_F(n)	(EL_CONS_KBD_KEY_F0 + (n))

#define	EL_CONS_KBD_KEY_STRs	\
	"Insert",	"Delete",	"Home",		"End",		"PgUp",		"PgDn",		\
	"Up",		"Down",		"Left",		"Right",	"BackTab"

enum EelConsKbdKeyState {
	EL_CONS_KBD_KS			= 0x10000L,
	EL_CONS_KBD_KS_MASK		= EL_CONS_KBD_KS * 0xF,

	EL_CONS_KBD_KS_SHIFT	= EL_CONS_KBD_KS * EL_FLAG(0),
	EL_CONS_KBD_KS_CTRL		= EL_CONS_KBD_KS * EL_FLAG(1),
	EL_CONS_KBD_KS_ALT		= EL_CONS_KBD_KS * EL_FLAG(2),
};

enum EelConsKbdLedState {
	EL_CONS_KBD_LS			= 0x100000L,
	EL_CONS_KBD_LS_MASK		= EL_CONS_KBD_LS * 0xF,

	EL_CONS_KBD_LS_NUM		= EL_CONS_KBD_LS * EL_FLAG(0),
	EL_CONS_KBD_LS_CAPS		= EL_CONS_KBD_LS * EL_FLAG(1),
	EL_CONS_KBD_LS_SCROLL	= EL_CONS_KBD_LS * EL_FLAG(2),
};

typedef ulong elConsKbdKey_t;
typedef ulong elConsKbdKeyState_t;
typedef ulong elConsKbdLedState_t;

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

HANDLE			 elConsKbdHandle		__P((void));
elConsKbdKey_t	 elConsKbdGetKey		__P((void));
elConsKbdKey_t	 elConsKbdGetKeyPeek	__P((void));
char			*elConsKbdKeyStr		__P((elConsKbdKey_t keyval));

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif 	/* _EL_CONS_KBD_H */
