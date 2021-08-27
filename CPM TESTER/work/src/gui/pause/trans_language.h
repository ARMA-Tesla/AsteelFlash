#if !defined(__LANG_TRANSLATION_H__)
#define __LANG_TRANSLATION_H__

#include "pause.h"

/* language support pattern */



static SLanguageItem PAUSElang[] = {   
 	P(PAUSE,				_noop("Pause")),
	V(PAUSE_MSG_PAUSE,		_noop("Pause")),
    L(PAUSE_BUTTON_1,		_noop("Operators Pause")),
	L(PAUSE_BUTTON_2,		_noop("Planned Downtime")),
	L(PAUSE_BUTTON_3,		_noop("Quality")),
	L(PAUSE_BUTTON_4,		_noop("Maintenance/Fault")),
   {0 , NULL, 0}
};
#endif    
