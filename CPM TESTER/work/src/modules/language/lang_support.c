#include "lang_support.h"

char* planguageGettext(SLanguagePtr me, const char *msgid);
char* planguageDgettext(SLanguagePtr me, const char *domainname, const char *msgid);
char* planguageTextdomain(SLanguagePtr me, const char *domainname);
char* planguageBindtextdomain(SLanguagePtr me, const char *domainname, const char *dirname);

	
/*---------------------------------------------------------------------------*/	
#undef __FUNC__
#define __FUNC__ "language_new"
SElExceptionPtr language_new(
	SLanguagePtr* pLanguage
)
{
	SElExceptionPtr     pexception = NULL; 
	int32_t				error = 0;
	SLanguagePtr		me = NULL; 
	
	me = (SLanguagePtr)calloc( 1, sizeof(SLanguage));
	EXCCHECKALLOC( me );

	*pLanguage = me;
	
	me->_hinstLib = LoadLibrary("intl.dll");
	
	if(me->_hinstLib != NULL)
	{
		me->_addGettext			= (fncGettext)GetProcAddress(me->_hinstLib, "gettext"); 
		me->_addDgettext  		= (fncDgettext)GetProcAddress(me->_hinstLib, "dgettext"); 
		me->_addTextdomain     	= (fncTextdomain)GetProcAddress(me->_hinstLib, "textdomain"); 
		me->_addBindtextdomain   = (fncBindtextdomain)GetProcAddress(me->_hinstLib, "bindtextdomain"); 
	}
	else
	{
		EXCTHROW(-1, "iconv.dll and intl.dll do not exist in windows\\system32 directory!");
	}
	
	me->Gettext 			= planguageGettext;
	me->dGettext  		= planguageDgettext; 
	me->textdomain     	= planguageTextdomain; 
	me->bindtextdomain   = planguageBindtextdomain; 
	
Error:
   EXCRETHROW( pexception);	
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "language_delete"
SElExceptionPtr	language_delete(
	SLanguagePtr* pLanguage
)
{
	int32_t      	error = 0;
	SLanguagePtr	planguage = *pLanguage;
    
	if ( pLanguage && *pLanguage )
    {
		if(planguage->_hinstLib != NULL)
			FreeLibrary(planguage->_hinstLib);   

        free( *pLanguage);
        *pLanguage = NULL;
    }
	
/* Error: */
   return NULL;	
}

/*---------------------------------------------------------------------------*/
char* planguageGettext(SLanguagePtr me, const char *msgid)
{
	return ( (me->_addGettext)(msgid) );
}
/*---------------------------------------------------------------------------*/
char* planguageDgettext(SLanguagePtr me, const char *domainname, const char *msgid)
{
	return ( (me->_addDgettext)(domainname, msgid) );
}

/*---------------------------------------------------------------------------*/
char* planguageTextdomain(SLanguagePtr me, const char *domainname)
{
	return ( (me->_addTextdomain)(domainname) );  
}
/*---------------------------------------------------------------------------*/
char* planguageBindtextdomain(SLanguagePtr me, const char *domainname, const char *dirname)
{
	return ( (me->_addBindtextdomain)(domainname, dirname) ); 
}

