#if !defined(__LANG_SUPPORT_H__)
#define __LANG_SUPPORT_H__

#include <elerror.h>
#include <eltypes.h>
#include <exception/exception.h>   
#include <windows.h>

#define _(string)             	(LANGUAGE_PTR!=NULL)? LANGUAGE_PTR->dGettext(LANGUAGE_PTR, PACKAGE, string):string
#define gettext(string)         (LANGUAGE_PTR!=NULL)? LANGUAGE_PTR->dGettext(LANGUAGE_PTR, PACKAGE, string):string
#define gettext_noop(string)   	string
#define _noop(string)         	string

#define LIBINTL_DLL_EXPORTED __declspec(dllexport)

typedef char*  ( *fncGettext)(const char *msgid);
typedef char*  ( *fncDgettext)(const char *domainname, const char *msgid);
typedef char*  ( *fncTextdomain)(const char *domainname);
typedef char*  ( *fncBindtextdomain)(const char *domainname, const char *dirname);

typedef struct _SLanguage
{
	char* (*Gettext)(struct _SLanguage* me, const char *msgid);
	char* (*dGettext)(struct _SLanguage* me, const char *domainname, const char *msgid);
	char* (*textdomain)(struct _SLanguage* me, const char *domainname);
	char* (*bindtextdomain)(struct _SLanguage* me, const char *domainname, const char *dirname);
	
	HINSTANCE       	_hinstLib;
	fncGettext 			_addGettext;
	fncDgettext			_addDgettext;
	fncTextdomain		_addTextdomain;
	fncBindtextdomain   _addBindtextdomain;
										  
} SLanguage, *SLanguagePtr;

SElExceptionPtr	language_new(SLanguagePtr* pLanguage);
SElExceptionPtr language_delete(SLanguagePtr* pLanguage);

#endif

