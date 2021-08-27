#if !defined(__LANG_TRANSLATION_H__)
#define __LANG_TRANSLATION_H__

#include "kosu.h"

/* language support pattern */



static SLanguageItem KOSUlang[] = {   
 	P(PANEL,				_noop("KOSU")),
    V(PANEL_MSG_TOTAL,		_noop("TOTAL")),
    V(PANEL_MSG_GOOD,		_noop("PASS")),
	V(PANEL_MSG_BAD,		_noop("FAIL")),
	//V(PANEL_MSG_PAUSE,		_noop("Pause")),
//	V(PANEL_MSG_PANEL,		_noop("Panels")), 
	//V(PANEL_MSG_OPERATOR,	_noop("OPERATEUR")), 
//	V(PANEL_MSG_END,		_noop("End")), 
	{0 , NULL, 0}
};
#endif    
