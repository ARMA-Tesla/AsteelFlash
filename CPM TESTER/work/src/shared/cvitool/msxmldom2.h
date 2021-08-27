#if !defined(__MSXMLDOM2_H__) 
#define __MSXMLDOM2_H__

#include <msxmldom.h>

#define XML_GET_DOC(xml_file_path, doc) \
	{ \
	HRESULT hr = 0; \
	VBOOL is_xml_success; \
	VARIANT pfile_name_var = CA_VariantEmpty(); \
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &doc)); \
	EXCCHECKCVI( CA_VariantSetCString(&pfile_name_var, xml_file_path)); \
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (doc, NULL, pfile_name_var, &is_xml_success)); \
	CA_VariantClear(&pfile_name_var); \
	}

#define XML_GET_DOC_NODES(xml_file_path, node_name, node_list) \
	{ \
	HRESULT hr = 0; \
	CAObjHandle pxml = 0; \
	XML_GET_DOC(xml_file_path, pxml); \
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectNodes (pxml, NULL, node_name, &node_list)); \
	CA_DiscardObjHandle(pxml); \
	}

#define XML_GET_DOC_NODE(xml_file_path, node_name, node) \
	{ \
	HRESULT hr = 0; \
	CAObjHandle pxml = 0; \
	XML_GET_DOC(xml_file_path, pxml); \
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, node_name, &node)); \
	CA_DiscardObjHandle(pxml); \
	}

#define XML_GET_NODE_STRING(node_xml, node_name, node_text, length) \
	{ \
	HRESULT hr = 0; \
	CAObjHandle pparameter = 0; \
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode(node_xml, NULL, node_name, &pparameter)); \
	if(pparameter) { \
		char* ptext = NULL; \
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext(pparameter, NULL, &ptext)); \
		if(ptext) { \
			strncpy(node_text, ptext, length); \
			CA_FreeMemory(ptext); \
			ptext = NULL; } \
		CA_DiscardObjHandle(pparameter); \
		pparameter = 0; } \
	}

#endif /* __MSXMLDOM2_H__ */
