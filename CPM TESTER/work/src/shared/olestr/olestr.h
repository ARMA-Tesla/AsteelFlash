
#if !defined(__EL_OLE_STRING_SUPPORT_H__)
#define __EL_OLE_STRING_SUPPORT_H__

#include <Exception.h>
#define COBJMACROS
#include "windows.h"
#include "ole2.h"
#include <msxml2.h>


#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif



SElExceptionPtr CStrintToBSTR( const char* string, BSTR* pbStr);
SElExceptionPtr BSTRtoCStrintBuffer( BSTR string, char* buffer, int32_t size);

SElExceptionPtr msxml_GetSingleNodeText( 
	IXMLDOMNode*	pRoot,
	const char* 	nodeName,
	char* 			buffer,
	int32_t			size
);


#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif

#endif /* __EL_OLE_STRING_SUPPORT_H__ */
