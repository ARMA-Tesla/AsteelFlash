/****************************************************************************
 *                         O L E    S T R I N G
 *---------------------------------------------------------------------------
 *    Copyright   2007 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       olestr.c                                                    
 * Purpose:     OLE String conversions
 * Version:     1.0  
 ****************************************************************************/

#include "olestr.h"

#undef __FUNC__
#define __FUNC__ "CStrintToBSTR"
SElExceptionPtr CStrintToBSTR( const char* string, BSTR* pbStr)
{
	SElExceptionPtr pexception = NULL;
	int				size = 0;
	int32_t			error;
	BSTR			pbstr = NULL;
	
	size = MultiByteToWideChar(CP_ACP, 0, string, -1, NULL, 0);	

	if ( pbStr )
	{
		pbstr = SysAllocStringLen(NULL, size);
		EXCCHECKWIN( MultiByteToWideChar( CP_ACP, 0, string, -1,
										  pbstr, size));
	}

Error:
	if ( pbStr ) *pbStr = pbstr;
	EXCRETHROW( pexception);	
}


#undef __FUNC__
#define __FUNC__ "BSTRtoCStrintBuffer"
SElExceptionPtr BSTRtoCStrintBuffer( BSTR string, char* buffer, int size)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error;
	int				length, converted_length = -1;
	
	length = WideCharToMultiByte(CP_ACP, 0, string, -1, NULL, 0, NULL, NULL);
	if ( length >= size )
	{
		converted_length = (size > 0) ? size - 1 : 0;
		buffer[size-1] = '\0';
	}
	if ( converted_length && buffer )
	{
		EXCCHECKWIN( WideCharToMultiByte( CP_ACP, 0, string, converted_length,
									      buffer, size, NULL, NULL ));
	}
	if ( length >= size )
	{
		EXCTHROW( length+1, "Required Buffer Size");
	}
	
Error:
	EXCRETHROW( pexception);	
}

#undef __FUNC__
#define __FUNC__ "msxml_GetSingleNodeText"
SElExceptionPtr msxml_GetSingleNodeText( 
	IXMLDOMNode*	pRoot,
	const char* 	nodeName,
	char* 			buffer,
	int32_t			size
)
{
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	BSTR			bstr = NULL;
	BSTR			str  = NULL;
	IXMLDOMNode*	pnode = NULL;
	int32_t			error;

	EXCCHECK( CStrintToBSTR(nodeName, &bstr));
	EXCCHECKCOM( IXMLDOMNode_selectSingleNode( pRoot, bstr, &pnode));
	EXCCHECKCOM( IXMLDOMNode_get_text( pnode, &str));
	EXCCHECK( BSTRtoCStrintBuffer( str, buffer, size));

Error:
    if ( str ) SysFreeString( str);
	if ( bstr ) SysFreeString( bstr);
	if ( pnode ) IXMLDOMNode_Release( pnode );
	EXCRETHROW( pexception);
}
