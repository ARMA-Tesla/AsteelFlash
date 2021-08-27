#if !defined(__LANG_TRANSLATION_H__)
#define __LANG_TRANSLATION_H__

#include "messaging.h"

/* language support pattern */


static SLanguageItem PANELlang[] = {   
 	P(PANEL,				_noop("Panel")),
	V(PANEL_SAP_ERR_LABEL,	_noop("Errors:")),
	{0 , NULL, 0}
};

static SLanguageItem MSG_ERRORlang[] = {   
 	P(MSG_ERROR,			_noop("Error Panel")),
	{0 , NULL, 0}
};

static SLanguageItem MSG_SAPlang[] = {   
 	P(MSG_SAP,			_noop("SAP Link Configuration")),
	L(MSG_SAP_BARCODE,	_noop("Read Barcode")),
	L(MSG_SAP_TRANSLATED_BARCODE,	_noop("Translated Barcode")),
	{0 , NULL, 0}
};


static SLanguageItem SAP_ERRORlang[] = {   
 	P(SAP_ERROR,			_noop("SAP Barcode Error")),
	{0 , NULL, 0}
};




#endif    
