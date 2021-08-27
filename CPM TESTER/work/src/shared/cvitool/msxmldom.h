#ifndef _MSXML_H
#define _MSXML_H

#if defined(INCLUDE_AFTER_WINDOWS_H) && !defined(_INC_WINDOWS)
#error  This header must be included before utility.h and formatio.h
#error  because it includes cviauto.h which includes Windows SDK headers.
#endif /* INCLUDE_AFTER_WINDOWS_H */

#include <cviauto.h>

#ifdef __cplusplus
    extern "C" {
#endif
/* NICDBLD_BEGIN> Type Library Specific Types */

enum MSXMLEnum_tagDOMNodeType
{
    MSXMLConst_NODE_INVALID = 0,
    MSXMLConst_NODE_ELEMENT = 1,
    MSXMLConst_NODE_ATTRIBUTE = 2,
    MSXMLConst_NODE_TEXT = 3,
    MSXMLConst_NODE_CDATA_SECTION = 4,
    MSXMLConst_NODE_ENTITY_REFERENCE = 5,
    MSXMLConst_NODE_ENTITY = 6,
    MSXMLConst_NODE_PROCESSING_INSTRUCTION = 7,
    MSXMLConst_NODE_COMMENT = 8,
    MSXMLConst_NODE_DOCUMENT = 9,
    MSXMLConst_NODE_DOCUMENT_TYPE = 10,
    MSXMLConst_NODE_DOCUMENT_FRAGMENT = 11,
    MSXMLConst_NODE_NOTATION = 12,
    _MSXML_tagDOMNodeTypeForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSXMLEnum_tagXMLEMEM_TYPE
{
    MSXMLConst_XMLELEMTYPE_ELEMENT = 0,
    MSXMLConst_XMLELEMTYPE_TEXT = 1,
    MSXMLConst_XMLELEMTYPE_COMMENT = 2,
    MSXMLConst_XMLELEMTYPE_DOCUMENT = 3,
    MSXMLConst_XMLELEMTYPE_DTD = 4,
    MSXMLConst_XMLELEMTYPE_PI = 5,
    MSXMLConst_XMLELEMTYPE_OTHER = 6,
    _MSXML_tagXMLEMEM_TYPEForceSizeToFourBytes = 0xFFFFFFFF
};
typedef long MSXMLType_DOMNodeType;
typedef CAObjHandle MSXMLObj_IXMLDOMNode;
typedef CAObjHandle MSXMLObj_IXMLDOMNodeList;
typedef CAObjHandle MSXMLObj_IXMLDOMNamedNodeMap;
typedef CAObjHandle MSXMLObj_IXMLDOMDocument;
typedef CAObjHandle MSXMLObj_IXMLDOMDocumentType;
typedef CAObjHandle MSXMLObj_IXMLDOMImplementation;
typedef CAObjHandle MSXMLObj_IXMLDOMElement;
typedef CAObjHandle MSXMLObj_IXMLDOMDocumentFragment;
typedef CAObjHandle MSXMLObj_IXMLDOMText;
typedef CAObjHandle MSXMLObj_IXMLDOMComment;
typedef CAObjHandle MSXMLObj_IXMLDOMCDATASection;
typedef CAObjHandle MSXMLObj_IXMLDOMProcessingInstruction;
typedef CAObjHandle MSXMLObj_IXMLDOMAttribute;
typedef CAObjHandle MSXMLObj_IXMLDOMEntityReference;
typedef CAObjHandle MSXMLObj_IXMLDOMParseError;
typedef CAObjHandle MSXMLObj_IXMLElement2;
typedef long MSXMLType_XMLELEM_TYPE;
typedef HRESULT (CVICALLBACK *XMLDOMDocumentEventsRegOnondataavailable_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                      void *caCallbackData,
                                                                                      long *__returnValue);
typedef HRESULT (CVICALLBACK *XMLDOMDocumentEventsRegOnonreadystatechange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                         void *caCallbackData,
                                                                                         long *__returnValue);
/* NICDBLD_END> Type Library Specific Types */

extern const IID MSXML_IID_IXMLDOMDocument;
extern const IID MSXML_IID_IXMLHttpRequest;
extern const IID MSXML_IID_IXMLDSOControl;
extern const IID MSXML_IID_IXMLDocument2;
extern const IID MSXML_IID_XMLDOMDocumentEvents;
extern const IID MSXML_IID_IXMLDOMImplementation;
extern const IID MSXML_IID_IXMLDOMNode;
extern const IID MSXML_IID_IXMLDOMNodeList;
extern const IID MSXML_IID_IXMLDOMNamedNodeMap;
extern const IID MSXML_IID_IXMLDOMDocumentType;
extern const IID MSXML_IID_IXMLDOMElement;
extern const IID MSXML_IID_IXMLDOMAttribute;
extern const IID MSXML_IID_IXMLDOMDocumentFragment;
extern const IID MSXML_IID_IXMLDOMText;
extern const IID MSXML_IID_IXMLDOMCharacterData;
extern const IID MSXML_IID_IXMLDOMComment;
extern const IID MSXML_IID_IXMLDOMCDATASection;
extern const IID MSXML_IID_IXMLDOMProcessingInstruction;
extern const IID MSXML_IID_IXMLDOMEntityReference;
extern const IID MSXML_IID_IXMLDOMParseError;
extern const IID MSXML_IID_IXMLDOMNotation;
extern const IID MSXML_IID_IXMLDOMEntity;
extern const IID MSXML_IID_IXTLRuntime;

HRESULT CVIFUNC MSXML_NewDOMDocumentIXMLDOMDocument (const char *server,
                                                     int supportMultithreading,
                                                     LCID locale, int reserved,
                                                     CAObjHandle *objectHandle);

HRESULT CVIFUNC MSXML_OpenDOMDocumentIXMLDOMDocument (const char *fileName,
                                                      const char *server,
                                                      int supportMultithreading,
                                                      LCID locale, int reserved,
                                                      CAObjHandle *objectHandle);

HRESULT CVIFUNC MSXML_ActiveDOMDocumentIXMLDOMDocument (const char *server,
                                                        int supportMultithreading,
                                                        LCID locale,
                                                        int reserved,
                                                        CAObjHandle *objectHandle);

HRESULT CVIFUNC MSXML_NewDOMFreeThreadedDocumentIXMLDOMDocument (const char *server,
                                                                 int supportMultithreading,
                                                                 LCID locale,
                                                                 int reserved,
                                                                 CAObjHandle *objectHandle);

HRESULT CVIFUNC MSXML_OpenDOMFreeThreadedDocumentIXMLDOMDocument (const char *fileName,
                                                                  const char *server,
                                                                  int supportMultithreading,
                                                                  LCID locale,
                                                                  int reserved,
                                                                  CAObjHandle *objectHandle);

HRESULT CVIFUNC MSXML_ActiveDOMFreeThreadedDocumentIXMLDOMDocument (const char *server,
                                                                    int supportMultithreading,
                                                                    LCID locale,
                                                                    int reserved,
                                                                    CAObjHandle *objectHandle);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetnodeName (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  char **name);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetnodeValue (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VARIANT *value);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentSetnodeValue (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VARIANT value);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetnodeType (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLType_DOMNodeType *type);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetparentNode (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNode *parent);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetchildNodes (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNodeList *childList);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetfirstChild (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNode *firstChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetlastChild (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode *lastChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetpreviousSibling (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSXMLObj_IXMLDOMNode *previousSibling);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetnextSibling (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     MSXMLObj_IXMLDOMNode *nextSibling);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetattributes (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNamedNodeMap *attributeMap);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentinsertBefore (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode newChild,
                                                   VARIANT refChild,
                                                   MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentreplaceChild (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode newChild,
                                                   MSXMLObj_IXMLDOMNode oldChild,
                                                   MSXMLObj_IXMLDOMNode *outOldChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentremoveChild (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNode childNode,
                                                  MSXMLObj_IXMLDOMNode *oldChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentappendChild (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNode newChild,
                                                  MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumenthasChildNodes (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL *hasChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetownerDocument (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSXMLObj_IXMLDOMDocument *DOMDocument);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentcloneNode (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo, VBOOL deep,
                                                MSXMLObj_IXMLDOMNode *cloneRoot);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetnodeTypeString (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        char **nodeType);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGettext (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, char **text);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentSettext (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *text);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetspecified (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VBOOL *isSpecified);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetdefinition (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNode *definitionNode);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetnodeTypedValue (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VARIANT *typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentSetnodeTypedValue (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VARIANT typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetdataType (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VARIANT *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentSetdataType (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetxml (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMDocumenttransformNode (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNode stylesheet,
                                                    char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentselectNodes (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *queryString,
                                                  MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentselectSingleNode (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       const char *queryString,
                                                       MSXMLObj_IXMLDOMNode *resultNode);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetparsed (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL *isParsed);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetnamespaceURI (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      char **namespaceURI);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetprefix (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                char **prefixString);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetbaseName (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  char **nameString);

HRESULT CVIFUNC MSXML_IXMLDOMDocumenttransformNodeToObject (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            MSXMLObj_IXMLDOMNode stylesheet,
                                                            VARIANT outputObject);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetdoctype (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSXMLObj_IXMLDOMDocumentType *documentType);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetimplementation (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMImplementation *impl);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetdocumentElement (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSXMLObj_IXMLDOMElement *DOMElement);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentSetByRefdocumentElement (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              MSXMLObj_IXMLDOMElement DOMElement);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentcreateElement (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    const char *tagName,
                                                    MSXMLObj_IXMLDOMElement *element);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentcreateDocumentFragment (CAObjHandle objectHandle,
                                                             ERRORINFO *errorInfo,
                                                             MSXMLObj_IXMLDOMDocumentFragment *docFrag);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentcreateTextNode (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *data,
                                                     MSXMLObj_IXMLDOMText *text);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentcreateComment (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    const char *data,
                                                    MSXMLObj_IXMLDOMComment *comment);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentcreateCDATASection (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         const char *data,
                                                         MSXMLObj_IXMLDOMCDATASection *cdata);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentcreateProcessingInstruction (CAObjHandle objectHandle,
                                                                  ERRORINFO *errorInfo,
                                                                  const char *target,
                                                                  const char *data,
                                                                  MSXMLObj_IXMLDOMProcessingInstruction *pi);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentcreateAttribute (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      const char *name,
                                                      MSXMLObj_IXMLDOMAttribute *attribute);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentcreateEntityReference (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            const char *name,
                                                            MSXMLObj_IXMLDOMEntityReference *entityRef);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentgetElementsByTagName (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           const char *tagName,
                                                           MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentcreateNode (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VARIANT type, const char *name,
                                                 const char *namespaceURI,
                                                 MSXMLObj_IXMLDOMNode *node);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentnodeFromID (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *idString,
                                                 MSXMLObj_IXMLDOMNode *node);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentload (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo,
                                           VARIANT xmlSource,
                                           VBOOL *isSuccessful);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetreadyState (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    long *value);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetparseError (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMParseError *errorObj);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGeturl (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             char **urlString);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetasync (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *isAsync);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentSetasync (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL isAsync);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentabort (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentloadXML (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *bstrXML,
                                              VBOOL *isSuccessful);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentsave (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo,
                                           VARIANT destination);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetvalidateOnParse (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         VBOOL *isValidating);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentSetvalidateOnParse (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         VBOOL isValidating);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetresolveExternals (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          VBOOL *isResolving);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentSetresolveExternals (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          VBOOL isResolving);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentGetpreserveWhiteSpace (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VBOOL *isPreserving);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentSetpreserveWhiteSpace (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VBOOL isPreserving);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentSetonreadystatechange (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VARIANT newValue);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentSetondataavailable (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         VARIANT newValue);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentSetontransformnode (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         VARIANT newValue);

HRESULT CVIFUNC MSXML_NewIXMLHttpRequest (const char *server,
                                          int supportMultithreading, LCID locale,
                                          int reserved,
                                          CAObjHandle *objectHandle);

HRESULT CVIFUNC MSXML_OpenIXMLHttpRequest (const char *fileName,
                                           const char *server,
                                           int supportMultithreading,
                                           LCID locale, int reserved,
                                           CAObjHandle *objectHandle);

HRESULT CVIFUNC MSXML_ActiveIXMLHttpRequest (const char *server,
                                             int supportMultithreading,
                                             LCID locale, int reserved,
                                             CAObjHandle *objectHandle);

HRESULT CVIFUNC MSXML_IXMLHttpRequestopen (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo,
                                           const char *bstrMethod,
                                           const char *bstrUrl, VARIANT varAsync,
                                           VARIANT bstrUser,
                                           VARIANT bstrPassword);

HRESULT CVIFUNC MSXML_IXMLHttpRequestsetRequestHeader (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       const char *bstrHeader,
                                                       const char *bstrValue);

HRESULT CVIFUNC MSXML_IXMLHttpRequestgetResponseHeader (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        const char *bstrHeader,
                                                        char **pbstrValue);

HRESULT CVIFUNC MSXML_IXMLHttpRequestgetAllResponseHeaders (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            char **pbstrHeaders);

HRESULT CVIFUNC MSXML_IXMLHttpRequestsend (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, VARIANT varBody);

HRESULT CVIFUNC MSXML_IXMLHttpRequestabort (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo);

HRESULT CVIFUNC MSXML_IXMLHttpRequestGetstatus (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                long *plStatus);

HRESULT CVIFUNC MSXML_IXMLHttpRequestGetstatusText (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    char **pbstrStatus);

HRESULT CVIFUNC MSXML_IXMLHttpRequestGetresponseXML (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     CAObjHandle *ppBody);

HRESULT CVIFUNC MSXML_IXMLHttpRequestGetresponseText (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      char **pbstrBody);

HRESULT CVIFUNC MSXML_IXMLHttpRequestGetresponseBody (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      VARIANT *pvarBody);

HRESULT CVIFUNC MSXML_IXMLHttpRequestGetresponseStream (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VARIANT *pvarBody);

HRESULT CVIFUNC MSXML_IXMLHttpRequestGetreadyState (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    long *plState);

HRESULT CVIFUNC MSXML_IXMLHttpRequestSetonreadystatechange (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            CAObjHandle newValue);

HRESULT CVIFUNC MSXML_NewIXMLDSOControl (const char *server,
                                         int supportMultithreading, LCID locale,
                                         int reserved, CAObjHandle *objectHandle);

HRESULT CVIFUNC MSXML_OpenIXMLDSOControl (const char *fileName,
                                          const char *server,
                                          int supportMultithreading, LCID locale,
                                          int reserved,
                                          CAObjHandle *objectHandle);

HRESULT CVIFUNC MSXML_ActiveIXMLDSOControl (const char *server,
                                            int supportMultithreading,
                                            LCID locale, int reserved,
                                            CAObjHandle *objectHandle);

HRESULT CVIFUNC MSXML_IXMLDSOControlGetXMLDocument (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMDocument *ppDoc);

HRESULT CVIFUNC MSXML_IXMLDSOControlSetXMLDocument (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMDocument ppDoc);

HRESULT CVIFUNC MSXML_IXMLDSOControlGetJavaDSOCompatible (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          long *fJavaDSOCompatible);

HRESULT CVIFUNC MSXML_IXMLDSOControlSetJavaDSOCompatible (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          long fJavaDSOCompatible);

HRESULT CVIFUNC MSXML_IXMLDSOControlGetreadyState (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   long *state);

HRESULT CVIFUNC MSXML_NewIXMLDocument2 (const char *server,
                                        int supportMultithreading, LCID locale,
                                        int reserved, CAObjHandle *objectHandle);

HRESULT CVIFUNC MSXML_OpenIXMLDocument2 (const char *fileName,
                                         const char *server,
                                         int supportMultithreading, LCID locale,
                                         int reserved, CAObjHandle *objectHandle);

HRESULT CVIFUNC MSXML_ActiveIXMLDocument2 (const char *server,
                                           int supportMultithreading,
                                           LCID locale, int reserved,
                                           CAObjHandle *objectHandle);

HRESULT CVIFUNC MSXML_IXMLDocument2Getroot (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            MSXMLObj_IXMLElement2 *p);

HRESULT CVIFUNC MSXML_IXMLDocument2GetfileSize (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo, char **p);

HRESULT CVIFUNC MSXML_IXMLDocument2GetfileModifiedDate (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        char **p);

HRESULT CVIFUNC MSXML_IXMLDocument2GetfileUpdatedDate (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       char **p);

HRESULT CVIFUNC MSXML_IXMLDocument2Geturl (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, char **p);

HRESULT CVIFUNC MSXML_IXMLDocument2Seturl (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, const char *p);

HRESULT CVIFUNC MSXML_IXMLDocument2GetmimeType (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo, char **p);

HRESULT CVIFUNC MSXML_IXMLDocument2GetreadyState (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo, long *pl);

HRESULT CVIFUNC MSXML_IXMLDocument2Getcharset (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, char **p);

HRESULT CVIFUNC MSXML_IXMLDocument2Setcharset (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               const char *p);

HRESULT CVIFUNC MSXML_IXMLDocument2Getversion (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, char **p);

HRESULT CVIFUNC MSXML_IXMLDocument2Getdoctype (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, char **p);

HRESULT CVIFUNC MSXML_IXMLDocument2GetdtdURL (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, char **p);

HRESULT CVIFUNC MSXML_IXMLDocument2createElement (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VARIANT vType, VARIANT var1,
                                                  MSXMLObj_IXMLElement2 *ppElem);

HRESULT CVIFUNC MSXML_IXMLDocument2Getasync (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, VBOOL *pf);

HRESULT CVIFUNC MSXML_IXMLDocument2Setasync (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, VBOOL pf);

HRESULT CVIFUNC MSXML_IXMLDOMImplementationhasFeature (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       const char *feature,
                                                       const char *version,
                                                       VBOOL *hasFeature);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetnodeName (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, char **name);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetnodeValue (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VARIANT *value);

HRESULT CVIFUNC MSXML_IXMLDOMNodeSetnodeValue (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VARIANT value);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetnodeType (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              MSXMLType_DOMNodeType *type);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetparentNode (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNode *parent);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetchildNodes (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNodeList *childList);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetfirstChild (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNode *firstChild);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetlastChild (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               MSXMLObj_IXMLDOMNode *lastChild);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetpreviousSibling (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     MSXMLObj_IXMLDOMNode *previousSibling);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetnextSibling (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSXMLObj_IXMLDOMNode *nextSibling);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetattributes (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNamedNodeMap *attributeMap);

HRESULT CVIFUNC MSXML_IXMLDOMNodeinsertBefore (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               MSXMLObj_IXMLDOMNode newChild,
                                               VARIANT refChild,
                                               MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMNodereplaceChild (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               MSXMLObj_IXMLDOMNode newChild,
                                               MSXMLObj_IXMLDOMNode oldChild,
                                               MSXMLObj_IXMLDOMNode *outOldChild);

HRESULT CVIFUNC MSXML_IXMLDOMNoderemoveChild (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              MSXMLObj_IXMLDOMNode childNode,
                                              MSXMLObj_IXMLDOMNode *oldChild);

HRESULT CVIFUNC MSXML_IXMLDOMNodeappendChild (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              MSXMLObj_IXMLDOMNode newChild,
                                              MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMNodehasChildNodes (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL *hasChild);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetownerDocument (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMDocument *DOMDocument);

HRESULT CVIFUNC MSXML_IXMLDOMNodecloneNode (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, VBOOL deep,
                                            MSXMLObj_IXMLDOMNode *cloneRoot);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetnodeTypeString (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    char **nodeType);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGettext (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, char **text);

HRESULT CVIFUNC MSXML_IXMLDOMNodeSettext (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, const char *text);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetspecified (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *isSpecified);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetdefinition (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNode *definitionNode);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetnodeTypedValue (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VARIANT *typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMNodeSetnodeTypedValue (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VARIANT typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetdataType (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VARIANT *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMNodeSetdataType (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetxml (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMNodetransformNode (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNode stylesheet,
                                                char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMNodeselectNodes (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *queryString,
                                              MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMNodeselectSingleNode (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   const char *queryString,
                                                   MSXMLObj_IXMLDOMNode *resultNode);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetparsed (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            VBOOL *isParsed);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetnamespaceURI (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  char **namespaceURI);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetprefix (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            char **prefixString);

HRESULT CVIFUNC MSXML_IXMLDOMNodeGetbaseName (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              char **nameString);

HRESULT CVIFUNC MSXML_IXMLDOMNodetransformNodeToObject (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode stylesheet,
                                                        VARIANT outputObject);

HRESULT CVIFUNC MSXML_IXMLDOMNodeListGetitem (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, long index,
                                              MSXMLObj_IXMLDOMNode *listItem);

HRESULT CVIFUNC MSXML_IXMLDOMNodeListGetlength (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                long *listLength);

HRESULT CVIFUNC MSXML_IXMLDOMNodeListnextNode (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               MSXMLObj_IXMLDOMNode *nextItem);

HRESULT CVIFUNC MSXML_IXMLDOMNodeListreset (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo);

HRESULT CVIFUNC MSXML_IXMLDOMNodeListGet_newEnum (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  LPUNKNOWN *ppUnk);

HRESULT CVIFUNC MSXML_IXMLDOMNamedNodeMapgetNamedItem (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       const char *name,
                                                       MSXMLObj_IXMLDOMNode *namedItem);

HRESULT CVIFUNC MSXML_IXMLDOMNamedNodeMapsetNamedItem (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSXMLObj_IXMLDOMNode newItem,
                                                       MSXMLObj_IXMLDOMNode *nameItem);

HRESULT CVIFUNC MSXML_IXMLDOMNamedNodeMapremoveNamedItem (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          const char *name,
                                                          MSXMLObj_IXMLDOMNode *namedItem);

HRESULT CVIFUNC MSXML_IXMLDOMNamedNodeMapGetitem (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  long index,
                                                  MSXMLObj_IXMLDOMNode *listItem);

HRESULT CVIFUNC MSXML_IXMLDOMNamedNodeMapGetlength (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    long *listLength);

HRESULT CVIFUNC MSXML_IXMLDOMNamedNodeMapgetQualifiedItem (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           const char *baseName,
                                                           const char *namespaceURI,
                                                           MSXMLObj_IXMLDOMNode *qualifiedItem);

HRESULT CVIFUNC MSXML_IXMLDOMNamedNodeMapremoveQualifiedItem (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              const char *baseName,
                                                              const char *namespaceURI,
                                                              MSXMLObj_IXMLDOMNode *qualifiedItem);

HRESULT CVIFUNC MSXML_IXMLDOMNamedNodeMapnextNode (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode *nextItem);

HRESULT CVIFUNC MSXML_IXMLDOMNamedNodeMapreset (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo);

HRESULT CVIFUNC MSXML_IXMLDOMNamedNodeMapGet_newEnum (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      LPUNKNOWN *ppUnk);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetnodeName (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      char **name);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetnodeValue (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VARIANT *value);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeSetnodeValue (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VARIANT value);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetnodeType (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      MSXMLType_DOMNodeType *type);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetparentNode (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode *parent);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetchildNodes (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNodeList *childList);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetfirstChild (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode *firstChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetlastChild (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSXMLObj_IXMLDOMNode *lastChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetpreviousSibling (CAObjHandle objectHandle,
                                                             ERRORINFO *errorInfo,
                                                             MSXMLObj_IXMLDOMNode *previousSibling);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetnextSibling (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSXMLObj_IXMLDOMNode *nextSibling);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetattributes (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNamedNodeMap *attributeMap);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeinsertBefore (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSXMLObj_IXMLDOMNode newChild,
                                                       VARIANT refChild,
                                                       MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypereplaceChild (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSXMLObj_IXMLDOMNode newChild,
                                                       MSXMLObj_IXMLDOMNode oldChild,
                                                       MSXMLObj_IXMLDOMNode *outOldChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTyperemoveChild (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      MSXMLObj_IXMLDOMNode childNode,
                                                      MSXMLObj_IXMLDOMNode *oldChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeappendChild (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      MSXMLObj_IXMLDOMNode newChild,
                                                      MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypehasChildNodes (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL *hasChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetownerDocument (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           MSXMLObj_IXMLDOMDocument *DOMDocument);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypecloneNode (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL deep,
                                                    MSXMLObj_IXMLDOMNode *cloneRoot);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetnodeTypeString (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            char **nodeType);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGettext (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  char **text);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeSettext (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *text);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetspecified (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VBOOL *isSpecified);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetdefinition (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode *definitionNode);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetnodeTypedValue (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VARIANT *typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeSetnodeTypedValue (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VARIANT typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetdataType (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      VARIANT *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeSetdataType (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      const char *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetxml (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypetransformNode (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode stylesheet,
                                                        char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeselectNodes (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      const char *queryString,
                                                      MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeselectSingleNode (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           const char *queryString,
                                                           MSXMLObj_IXMLDOMNode *resultNode);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetparsed (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL *isParsed);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetnamespaceURI (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          char **namespaceURI);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetprefix (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    char **prefixString);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetbaseName (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      char **nameString);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypetransformNodeToObject (CAObjHandle objectHandle,
                                                                ERRORINFO *errorInfo,
                                                                MSXMLObj_IXMLDOMNode stylesheet,
                                                                VARIANT outputObject);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetname (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  char **rootName);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetentities (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      MSXMLObj_IXMLDOMNamedNodeMap *entityMap);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentTypeGetnotations (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSXMLObj_IXMLDOMNamedNodeMap *notationMap);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetnodeName (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 char **name);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetnodeValue (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VARIANT *value);

HRESULT CVIFUNC MSXML_IXMLDOMElementSetnodeValue (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VARIANT value);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetnodeType (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSXMLType_DOMNodeType *type);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetparentNode (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode *parent);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetchildNodes (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNodeList *childList);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetfirstChild (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode *firstChild);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetlastChild (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNode *lastChild);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetpreviousSibling (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode *previousSibling);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetnextSibling (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNode *nextSibling);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetattributes (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNamedNodeMap *attributeMap);

HRESULT CVIFUNC MSXML_IXMLDOMElementinsertBefore (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNode newChild,
                                                  VARIANT refChild,
                                                  MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMElementreplaceChild (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNode newChild,
                                                  MSXMLObj_IXMLDOMNode oldChild,
                                                  MSXMLObj_IXMLDOMNode *outOldChild);

HRESULT CVIFUNC MSXML_IXMLDOMElementremoveChild (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSXMLObj_IXMLDOMNode childNode,
                                                 MSXMLObj_IXMLDOMNode *oldChild);

HRESULT CVIFUNC MSXML_IXMLDOMElementappendChild (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSXMLObj_IXMLDOMNode newChild,
                                                 MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMElementhasChildNodes (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VBOOL *hasChild);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetownerDocument (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      MSXMLObj_IXMLDOMDocument *DOMDocument);

HRESULT CVIFUNC MSXML_IXMLDOMElementcloneNode (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, VBOOL deep,
                                               MSXMLObj_IXMLDOMNode *cloneRoot);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetnodeTypeString (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       char **nodeType);

HRESULT CVIFUNC MSXML_IXMLDOMElementGettext (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, char **text);

HRESULT CVIFUNC MSXML_IXMLDOMElementSettext (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             const char *text);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetspecified (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL *isSpecified);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetdefinition (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode *definitionNode);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetnodeTypedValue (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VARIANT *typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMElementSetnodeTypedValue (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VARIANT typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetdataType (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VARIANT *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMElementSetdataType (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetxml (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMElementtransformNode (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode stylesheet,
                                                   char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMElementselectNodes (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *queryString,
                                                 MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMElementselectSingleNode (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      const char *queryString,
                                                      MSXMLObj_IXMLDOMNode *resultNode);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetparsed (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *isParsed);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetnamespaceURI (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     char **namespaceURI);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetprefix (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               char **prefixString);

HRESULT CVIFUNC MSXML_IXMLDOMElementGetbaseName (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 char **nameString);

HRESULT CVIFUNC MSXML_IXMLDOMElementtransformNodeToObject (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           MSXMLObj_IXMLDOMNode stylesheet,
                                                           VARIANT outputObject);

HRESULT CVIFUNC MSXML_IXMLDOMElementGettagName (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                char **tagName);

HRESULT CVIFUNC MSXML_IXMLDOMElementgetAttribute (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *name,
                                                  VARIANT *value);

HRESULT CVIFUNC MSXML_IXMLDOMElementsetAttribute (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *name,
                                                  VARIANT value);

HRESULT CVIFUNC MSXML_IXMLDOMElementremoveAttribute (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *name);

HRESULT CVIFUNC MSXML_IXMLDOMElementgetAttributeNode (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      const char *name,
                                                      MSXMLObj_IXMLDOMAttribute *attributeNode);

HRESULT CVIFUNC MSXML_IXMLDOMElementsetAttributeNode (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      MSXMLObj_IXMLDOMAttribute DOMAttribute,
                                                      MSXMLObj_IXMLDOMAttribute *attributeNode);

HRESULT CVIFUNC MSXML_IXMLDOMElementremoveAttributeNode (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSXMLObj_IXMLDOMAttribute DOMAttribute,
                                                         MSXMLObj_IXMLDOMAttribute *attributeNode);

HRESULT CVIFUNC MSXML_IXMLDOMElementgetElementsByTagName (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          const char *tagName,
                                                          MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMElementnormalize (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetnodeName (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   char **name);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetnodeValue (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VARIANT *value);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeSetnodeValue (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VARIANT value);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetnodeType (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLType_DOMNodeType *type);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetparentNode (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     MSXMLObj_IXMLDOMNode *parent);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetchildNodes (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     MSXMLObj_IXMLDOMNodeList *childList);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetfirstChild (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     MSXMLObj_IXMLDOMNode *firstChild);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetlastChild (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNode *lastChild);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetpreviousSibling (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          MSXMLObj_IXMLDOMNode *previousSibling);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetnextSibling (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      MSXMLObj_IXMLDOMNode *nextSibling);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetattributes (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     MSXMLObj_IXMLDOMNamedNodeMap *attributeMap);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeinsertBefore (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNode newChild,
                                                    VARIANT refChild,
                                                    MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMAttributereplaceChild (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNode newChild,
                                                    MSXMLObj_IXMLDOMNode oldChild,
                                                    MSXMLObj_IXMLDOMNode *outOldChild);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeremoveChild (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode childNode,
                                                   MSXMLObj_IXMLDOMNode *oldChild);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeappendChild (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode newChild,
                                                   MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMAttributehasChildNodes (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL *hasChild);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetownerDocument (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMDocument *DOMDocument);

HRESULT CVIFUNC MSXML_IXMLDOMAttributecloneNode (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VBOOL deep,
                                                 MSXMLObj_IXMLDOMNode *cloneRoot);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetnodeTypeString (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         char **nodeType);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGettext (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, char **text);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeSettext (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               const char *text);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetspecified (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL *isSpecified);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetdefinition (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     MSXMLObj_IXMLDOMNode *definitionNode);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetnodeTypedValue (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         VARIANT *typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeSetnodeTypedValue (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         VARIANT typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetdataType (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VARIANT *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeSetdataType (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   const char *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetxml (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMAttributetransformNode (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     MSXMLObj_IXMLDOMNode stylesheet,
                                                     char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeselectNodes (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   const char *queryString,
                                                   MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeselectSingleNode (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        const char *queryString,
                                                        MSXMLObj_IXMLDOMNode *resultNode);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetparsed (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VBOOL *isParsed);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetnamespaceURI (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       char **namespaceURI);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetprefix (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 char **prefixString);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetbaseName (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   char **nameString);

HRESULT CVIFUNC MSXML_IXMLDOMAttributetransformNodeToObject (CAObjHandle objectHandle,
                                                             ERRORINFO *errorInfo,
                                                             MSXMLObj_IXMLDOMNode stylesheet,
                                                             VARIANT outputObject);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetname (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               char **attributeName);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeGetvalue (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VARIANT *attributeValue);

HRESULT CVIFUNC MSXML_IXMLDOMAttributeSetvalue (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VARIANT attributeValue);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetnodeName (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          char **name);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetnodeValue (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           VARIANT *value);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentSetnodeValue (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           VARIANT value);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetnodeType (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          MSXMLType_DOMNodeType *type);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetparentNode (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            MSXMLObj_IXMLDOMNode *parent);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetchildNodes (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            MSXMLObj_IXMLDOMNodeList *childList);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetfirstChild (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            MSXMLObj_IXMLDOMNode *firstChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetlastChild (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           MSXMLObj_IXMLDOMNode *lastChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetpreviousSibling (CAObjHandle objectHandle,
                                                                 ERRORINFO *errorInfo,
                                                                 MSXMLObj_IXMLDOMNode *previousSibling);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetnextSibling (CAObjHandle objectHandle,
                                                             ERRORINFO *errorInfo,
                                                             MSXMLObj_IXMLDOMNode *nextSibling);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetattributes (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            MSXMLObj_IXMLDOMNamedNodeMap *attributeMap);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentinsertBefore (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           MSXMLObj_IXMLDOMNode newChild,
                                                           VARIANT refChild,
                                                           MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentreplaceChild (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           MSXMLObj_IXMLDOMNode newChild,
                                                           MSXMLObj_IXMLDOMNode oldChild,
                                                           MSXMLObj_IXMLDOMNode *outOldChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentremoveChild (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          MSXMLObj_IXMLDOMNode childNode,
                                                          MSXMLObj_IXMLDOMNode *oldChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentappendChild (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          MSXMLObj_IXMLDOMNode newChild,
                                                          MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmenthasChildNodes (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VBOOL *hasChild);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetownerDocument (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               MSXMLObj_IXMLDOMDocument *DOMDocument);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentcloneNode (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL deep,
                                                        MSXMLObj_IXMLDOMNode *cloneRoot);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetnodeTypeString (CAObjHandle objectHandle,
                                                                ERRORINFO *errorInfo,
                                                                char **nodeType);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGettext (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      char **text);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentSettext (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      const char *text);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetspecified (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           VBOOL *isSpecified);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetdefinition (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            MSXMLObj_IXMLDOMNode *definitionNode);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetnodeTypedValue (CAObjHandle objectHandle,
                                                                ERRORINFO *errorInfo,
                                                                VARIANT *typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentSetnodeTypedValue (CAObjHandle objectHandle,
                                                                ERRORINFO *errorInfo,
                                                                VARIANT typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetdataType (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          VARIANT *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentSetdataType (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          const char *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetxml (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmenttransformNode (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            MSXMLObj_IXMLDOMNode stylesheet,
                                                            char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentselectNodes (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          const char *queryString,
                                                          MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentselectSingleNode (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               const char *queryString,
                                                               MSXMLObj_IXMLDOMNode *resultNode);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetparsed (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL *isParsed);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetnamespaceURI (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              char **namespaceURI);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetprefix (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        char **prefixString);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmentGetbaseName (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          char **nameString);

HRESULT CVIFUNC MSXML_IXMLDOMDocumentFragmenttransformNodeToObject (CAObjHandle objectHandle,
                                                                    ERRORINFO *errorInfo,
                                                                    MSXMLObj_IXMLDOMNode stylesheet,
                                                                    VARIANT outputObject);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetnodeName (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, char **name);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetnodeValue (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VARIANT *value);

HRESULT CVIFUNC MSXML_IXMLDOMTextSetnodeValue (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VARIANT value);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetnodeType (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              MSXMLType_DOMNodeType *type);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetparentNode (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNode *parent);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetchildNodes (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNodeList *childList);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetfirstChild (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNode *firstChild);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetlastChild (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               MSXMLObj_IXMLDOMNode *lastChild);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetpreviousSibling (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     MSXMLObj_IXMLDOMNode *previousSibling);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetnextSibling (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSXMLObj_IXMLDOMNode *nextSibling);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetattributes (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNamedNodeMap *attributeMap);

HRESULT CVIFUNC MSXML_IXMLDOMTextinsertBefore (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               MSXMLObj_IXMLDOMNode newChild,
                                               VARIANT refChild,
                                               MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMTextreplaceChild (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               MSXMLObj_IXMLDOMNode newChild,
                                               MSXMLObj_IXMLDOMNode oldChild,
                                               MSXMLObj_IXMLDOMNode *outOldChild);

HRESULT CVIFUNC MSXML_IXMLDOMTextremoveChild (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              MSXMLObj_IXMLDOMNode childNode,
                                              MSXMLObj_IXMLDOMNode *oldChild);

HRESULT CVIFUNC MSXML_IXMLDOMTextappendChild (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              MSXMLObj_IXMLDOMNode newChild,
                                              MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMTexthasChildNodes (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL *hasChild);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetownerDocument (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMDocument *DOMDocument);

HRESULT CVIFUNC MSXML_IXMLDOMTextcloneNode (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, VBOOL deep,
                                            MSXMLObj_IXMLDOMNode *cloneRoot);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetnodeTypeString (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    char **nodeType);

HRESULT CVIFUNC MSXML_IXMLDOMTextGettext (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, char **text);

HRESULT CVIFUNC MSXML_IXMLDOMTextSettext (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, const char *text);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetspecified (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *isSpecified);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetdefinition (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNode *definitionNode);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetnodeTypedValue (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VARIANT *typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMTextSetnodeTypedValue (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VARIANT typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetdataType (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VARIANT *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMTextSetdataType (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetxml (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMTexttransformNode (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNode stylesheet,
                                                char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMTextselectNodes (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *queryString,
                                              MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMTextselectSingleNode (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   const char *queryString,
                                                   MSXMLObj_IXMLDOMNode *resultNode);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetparsed (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            VBOOL *isParsed);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetnamespaceURI (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  char **namespaceURI);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetprefix (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            char **prefixString);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetbaseName (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              char **nameString);

HRESULT CVIFUNC MSXML_IXMLDOMTexttransformNodeToObject (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode stylesheet,
                                                        VARIANT outputObject);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetdata (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, char **data);

HRESULT CVIFUNC MSXML_IXMLDOMTextSetdata (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMTextGetlength (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            long *dataLength);

HRESULT CVIFUNC MSXML_IXMLDOMTextsubstringData (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                long offset, long count,
                                                char **data);

HRESULT CVIFUNC MSXML_IXMLDOMTextappendData (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMTextinsertData (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, long offset,
                                             const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMTextdeleteData (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, long offset,
                                             long count);

HRESULT CVIFUNC MSXML_IXMLDOMTextreplaceData (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, long offset,
                                              long count, const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMTextsplitText (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, long offset,
                                            MSXMLObj_IXMLDOMText *rightHandTextNode);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetnodeName (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       char **name);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetnodeValue (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VARIANT *value);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataSetnodeValue (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VARIANT value);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetnodeType (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSXMLType_DOMNodeType *type);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetparentNode (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSXMLObj_IXMLDOMNode *parent);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetchildNodes (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSXMLObj_IXMLDOMNodeList *childList);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetfirstChild (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSXMLObj_IXMLDOMNode *firstChild);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetlastChild (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode *lastChild);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetpreviousSibling (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              MSXMLObj_IXMLDOMNode *previousSibling);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetnextSibling (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          MSXMLObj_IXMLDOMNode *nextSibling);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetattributes (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSXMLObj_IXMLDOMNamedNodeMap *attributeMap);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDatainsertBefore (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode newChild,
                                                        VARIANT refChild,
                                                        MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDatareplaceChild (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode newChild,
                                                        MSXMLObj_IXMLDOMNode oldChild,
                                                        MSXMLObj_IXMLDOMNode *outOldChild);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataremoveChild (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSXMLObj_IXMLDOMNode childNode,
                                                       MSXMLObj_IXMLDOMNode *oldChild);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataappendChild (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSXMLObj_IXMLDOMNode newChild,
                                                       MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDatahasChildNodes (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         VBOOL *hasChild);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetownerDocument (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            MSXMLObj_IXMLDOMDocument *DOMDocument);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDatacloneNode (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL deep,
                                                     MSXMLObj_IXMLDOMNode *cloneRoot);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetnodeTypeString (CAObjHandle objectHandle,
                                                             ERRORINFO *errorInfo,
                                                             char **nodeType);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGettext (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   char **text);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataSettext (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   const char *text);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetspecified (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL *isSpecified);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetdefinition (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSXMLObj_IXMLDOMNode *definitionNode);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetnodeTypedValue (CAObjHandle objectHandle,
                                                             ERRORINFO *errorInfo,
                                                             VARIANT *typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataSetnodeTypedValue (CAObjHandle objectHandle,
                                                             ERRORINFO *errorInfo,
                                                             VARIANT typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetdataType (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VARIANT *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataSetdataType (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       const char *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetxml (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDatatransformNode (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSXMLObj_IXMLDOMNode stylesheet,
                                                         char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataselectNodes (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       const char *queryString,
                                                       MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataselectSingleNode (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            const char *queryString,
                                                            MSXMLObj_IXMLDOMNode *resultNode);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetparsed (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL *isParsed);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetnamespaceURI (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           char **namespaceURI);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetprefix (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     char **prefixString);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetbaseName (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       char **nameString);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDatatransformNodeToObject (CAObjHandle objectHandle,
                                                                 ERRORINFO *errorInfo,
                                                                 MSXMLObj_IXMLDOMNode stylesheet,
                                                                 VARIANT outputObject);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetdata (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   char **data);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataSetdata (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataGetlength (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     long *dataLength);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDatasubstringData (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         long offset, long count,
                                                         char **data);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDataappendData (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDatainsertData (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      long offset,
                                                      const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDatadeleteData (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      long offset, long count);

HRESULT CVIFUNC MSXML_IXMLDOMCharacterDatareplaceData (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       long offset, long count,
                                                       const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetnodeName (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 char **name);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetnodeValue (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VARIANT *value);

HRESULT CVIFUNC MSXML_IXMLDOMCommentSetnodeValue (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VARIANT value);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetnodeType (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSXMLType_DOMNodeType *type);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetparentNode (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode *parent);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetchildNodes (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNodeList *childList);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetfirstChild (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode *firstChild);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetlastChild (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNode *lastChild);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetpreviousSibling (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode *previousSibling);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetnextSibling (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNode *nextSibling);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetattributes (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNamedNodeMap *attributeMap);

HRESULT CVIFUNC MSXML_IXMLDOMCommentinsertBefore (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNode newChild,
                                                  VARIANT refChild,
                                                  MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMCommentreplaceChild (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNode newChild,
                                                  MSXMLObj_IXMLDOMNode oldChild,
                                                  MSXMLObj_IXMLDOMNode *outOldChild);

HRESULT CVIFUNC MSXML_IXMLDOMCommentremoveChild (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSXMLObj_IXMLDOMNode childNode,
                                                 MSXMLObj_IXMLDOMNode *oldChild);

HRESULT CVIFUNC MSXML_IXMLDOMCommentappendChild (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSXMLObj_IXMLDOMNode newChild,
                                                 MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMCommenthasChildNodes (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VBOOL *hasChild);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetownerDocument (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      MSXMLObj_IXMLDOMDocument *DOMDocument);

HRESULT CVIFUNC MSXML_IXMLDOMCommentcloneNode (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, VBOOL deep,
                                               MSXMLObj_IXMLDOMNode *cloneRoot);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetnodeTypeString (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       char **nodeType);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGettext (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, char **text);

HRESULT CVIFUNC MSXML_IXMLDOMCommentSettext (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             const char *text);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetspecified (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL *isSpecified);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetdefinition (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode *definitionNode);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetnodeTypedValue (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VARIANT *typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMCommentSetnodeTypedValue (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VARIANT typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetdataType (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VARIANT *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMCommentSetdataType (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetxml (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMCommenttransformNode (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode stylesheet,
                                                   char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMCommentselectNodes (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *queryString,
                                                 MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMCommentselectSingleNode (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      const char *queryString,
                                                      MSXMLObj_IXMLDOMNode *resultNode);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetparsed (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *isParsed);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetnamespaceURI (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     char **namespaceURI);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetprefix (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               char **prefixString);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetbaseName (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 char **nameString);

HRESULT CVIFUNC MSXML_IXMLDOMCommenttransformNodeToObject (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           MSXMLObj_IXMLDOMNode stylesheet,
                                                           VARIANT outputObject);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetdata (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, char **data);

HRESULT CVIFUNC MSXML_IXMLDOMCommentSetdata (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMCommentGetlength (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               long *dataLength);

HRESULT CVIFUNC MSXML_IXMLDOMCommentsubstringData (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   long offset, long count,
                                                   char **data);

HRESULT CVIFUNC MSXML_IXMLDOMCommentappendData (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMCommentinsertData (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                long offset, const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMCommentdeleteData (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                long offset, long count);

HRESULT CVIFUNC MSXML_IXMLDOMCommentreplaceData (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 long offset, long count,
                                                 const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetnodeName (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      char **name);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetnodeValue (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VARIANT *value);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionSetnodeValue (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VARIANT value);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetnodeType (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      MSXMLType_DOMNodeType *type);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetparentNode (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode *parent);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetchildNodes (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNodeList *childList);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetfirstChild (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode *firstChild);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetlastChild (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSXMLObj_IXMLDOMNode *lastChild);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetpreviousSibling (CAObjHandle objectHandle,
                                                             ERRORINFO *errorInfo,
                                                             MSXMLObj_IXMLDOMNode *previousSibling);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetnextSibling (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSXMLObj_IXMLDOMNode *nextSibling);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetattributes (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNamedNodeMap *attributeMap);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectioninsertBefore (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSXMLObj_IXMLDOMNode newChild,
                                                       VARIANT refChild,
                                                       MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionreplaceChild (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSXMLObj_IXMLDOMNode newChild,
                                                       MSXMLObj_IXMLDOMNode oldChild,
                                                       MSXMLObj_IXMLDOMNode *outOldChild);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionremoveChild (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      MSXMLObj_IXMLDOMNode childNode,
                                                      MSXMLObj_IXMLDOMNode *oldChild);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionappendChild (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      MSXMLObj_IXMLDOMNode newChild,
                                                      MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionhasChildNodes (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL *hasChild);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetownerDocument (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           MSXMLObj_IXMLDOMDocument *DOMDocument);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectioncloneNode (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL deep,
                                                    MSXMLObj_IXMLDOMNode *cloneRoot);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetnodeTypeString (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            char **nodeType);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGettext (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  char **text);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionSettext (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *text);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetspecified (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VBOOL *isSpecified);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetdefinition (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode *definitionNode);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetnodeTypedValue (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VARIANT *typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionSetnodeTypedValue (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VARIANT typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetdataType (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      VARIANT *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionSetdataType (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      const char *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetxml (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectiontransformNode (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode stylesheet,
                                                        char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionselectNodes (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      const char *queryString,
                                                      MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionselectSingleNode (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           const char *queryString,
                                                           MSXMLObj_IXMLDOMNode *resultNode);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetparsed (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL *isParsed);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetnamespaceURI (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          char **namespaceURI);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetprefix (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    char **prefixString);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetbaseName (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      char **nameString);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectiontransformNodeToObject (CAObjHandle objectHandle,
                                                                ERRORINFO *errorInfo,
                                                                MSXMLObj_IXMLDOMNode stylesheet,
                                                                VARIANT outputObject);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetdata (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  char **data);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionSetdata (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionGetlength (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    long *dataLength);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionsubstringData (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        long offset, long count,
                                                        char **data);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionappendData (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectioninsertData (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     long offset,
                                                     const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectiondeleteData (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     long offset, long count);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionreplaceData (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      long offset, long count,
                                                      const char *data);

HRESULT CVIFUNC MSXML_IXMLDOMCDATASectionsplitText (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    long offset,
                                                    MSXMLObj_IXMLDOMText *rightHandTextNode);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetnodeName (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               char **name);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetnodeValue (CAObjHandle objectHandle,
                                                                ERRORINFO *errorInfo,
                                                                VARIANT *value);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionSetnodeValue (CAObjHandle objectHandle,
                                                                ERRORINFO *errorInfo,
                                                                VARIANT value);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetnodeType (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               MSXMLType_DOMNodeType *type);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetparentNode (CAObjHandle objectHandle,
                                                                 ERRORINFO *errorInfo,
                                                                 MSXMLObj_IXMLDOMNode *parent);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetchildNodes (CAObjHandle objectHandle,
                                                                 ERRORINFO *errorInfo,
                                                                 MSXMLObj_IXMLDOMNodeList *childList);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetfirstChild (CAObjHandle objectHandle,
                                                                 ERRORINFO *errorInfo,
                                                                 MSXMLObj_IXMLDOMNode *firstChild);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetlastChild (CAObjHandle objectHandle,
                                                                ERRORINFO *errorInfo,
                                                                MSXMLObj_IXMLDOMNode *lastChild);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetpreviousSibling (CAObjHandle objectHandle,
                                                                      ERRORINFO *errorInfo,
                                                                      MSXMLObj_IXMLDOMNode *previousSibling);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetnextSibling (CAObjHandle objectHandle,
                                                                  ERRORINFO *errorInfo,
                                                                  MSXMLObj_IXMLDOMNode *nextSibling);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetattributes (CAObjHandle objectHandle,
                                                                 ERRORINFO *errorInfo,
                                                                 MSXMLObj_IXMLDOMNamedNodeMap *attributeMap);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructioninsertBefore (CAObjHandle objectHandle,
                                                                ERRORINFO *errorInfo,
                                                                MSXMLObj_IXMLDOMNode newChild,
                                                                VARIANT refChild,
                                                                MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionreplaceChild (CAObjHandle objectHandle,
                                                                ERRORINFO *errorInfo,
                                                                MSXMLObj_IXMLDOMNode newChild,
                                                                MSXMLObj_IXMLDOMNode oldChild,
                                                                MSXMLObj_IXMLDOMNode *outOldChild);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionremoveChild (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               MSXMLObj_IXMLDOMNode childNode,
                                                               MSXMLObj_IXMLDOMNode *oldChild);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionappendChild (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               MSXMLObj_IXMLDOMNode newChild,
                                                               MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionhasChildNodes (CAObjHandle objectHandle,
                                                                 ERRORINFO *errorInfo,
                                                                 VBOOL *hasChild);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetownerDocument (CAObjHandle objectHandle,
                                                                    ERRORINFO *errorInfo,
                                                                    MSXMLObj_IXMLDOMDocument *DOMDocument);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructioncloneNode (CAObjHandle objectHandle,
                                                             ERRORINFO *errorInfo,
                                                             VBOOL deep,
                                                             MSXMLObj_IXMLDOMNode *cloneRoot);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetnodeTypeString (CAObjHandle objectHandle,
                                                                     ERRORINFO *errorInfo,
                                                                     char **nodeType);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGettext (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           char **text);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionSettext (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           const char *text);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetspecified (CAObjHandle objectHandle,
                                                                ERRORINFO *errorInfo,
                                                                VBOOL *isSpecified);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetdefinition (CAObjHandle objectHandle,
                                                                 ERRORINFO *errorInfo,
                                                                 MSXMLObj_IXMLDOMNode *definitionNode);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetnodeTypedValue (CAObjHandle objectHandle,
                                                                     ERRORINFO *errorInfo,
                                                                     VARIANT *typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionSetnodeTypedValue (CAObjHandle objectHandle,
                                                                     ERRORINFO *errorInfo,
                                                                     VARIANT typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetdataType (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               VARIANT *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionSetdataType (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               const char *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetxml (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructiontransformNode (CAObjHandle objectHandle,
                                                                 ERRORINFO *errorInfo,
                                                                 MSXMLObj_IXMLDOMNode stylesheet,
                                                                 char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionselectNodes (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               const char *queryString,
                                                               MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionselectSingleNode (CAObjHandle objectHandle,
                                                                    ERRORINFO *errorInfo,
                                                                    const char *queryString,
                                                                    MSXMLObj_IXMLDOMNode *resultNode);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetparsed (CAObjHandle objectHandle,
                                                             ERRORINFO *errorInfo,
                                                             VBOOL *isParsed);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetnamespaceURI (CAObjHandle objectHandle,
                                                                   ERRORINFO *errorInfo,
                                                                   char **namespaceURI);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetprefix (CAObjHandle objectHandle,
                                                             ERRORINFO *errorInfo,
                                                             char **prefixString);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetbaseName (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               char **nameString);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructiontransformNodeToObject (CAObjHandle objectHandle,
                                                                         ERRORINFO *errorInfo,
                                                                         MSXMLObj_IXMLDOMNode stylesheet,
                                                                         VARIANT outputObject);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGettarget (CAObjHandle objectHandle,
                                                             ERRORINFO *errorInfo,
                                                             char **name);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionGetdata (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           char **value);

HRESULT CVIFUNC MSXML_IXMLDOMProcessingInstructionSetdata (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           const char *value);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetnodeName (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         char **name);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetnodeValue (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          VARIANT *value);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceSetnodeValue (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          VARIANT value);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetnodeType (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSXMLType_DOMNodeType *type);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetparentNode (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           MSXMLObj_IXMLDOMNode *parent);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetchildNodes (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           MSXMLObj_IXMLDOMNodeList *childList);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetfirstChild (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           MSXMLObj_IXMLDOMNode *firstChild);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetlastChild (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          MSXMLObj_IXMLDOMNode *lastChild);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetpreviousSibling (CAObjHandle objectHandle,
                                                                ERRORINFO *errorInfo,
                                                                MSXMLObj_IXMLDOMNode *previousSibling);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetnextSibling (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            MSXMLObj_IXMLDOMNode *nextSibling);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetattributes (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           MSXMLObj_IXMLDOMNamedNodeMap *attributeMap);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceinsertBefore (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          MSXMLObj_IXMLDOMNode newChild,
                                                          VARIANT refChild,
                                                          MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferencereplaceChild (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          MSXMLObj_IXMLDOMNode newChild,
                                                          MSXMLObj_IXMLDOMNode oldChild,
                                                          MSXMLObj_IXMLDOMNode *outOldChild);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceremoveChild (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSXMLObj_IXMLDOMNode childNode,
                                                         MSXMLObj_IXMLDOMNode *oldChild);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceappendChild (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSXMLObj_IXMLDOMNode newChild,
                                                         MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferencehasChildNodes (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           VBOOL *hasChild);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetownerDocument (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              MSXMLObj_IXMLDOMDocument *DOMDocument);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferencecloneNode (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VBOOL deep,
                                                       MSXMLObj_IXMLDOMNode *cloneRoot);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetnodeTypeString (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               char **nodeType);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGettext (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     char **text);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceSettext (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *text);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetspecified (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          VBOOL *isSpecified);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetdefinition (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           MSXMLObj_IXMLDOMNode *definitionNode);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetnodeTypedValue (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               VARIANT *typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceSetnodeTypedValue (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               VARIANT typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetdataType (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         VARIANT *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceSetdataType (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         const char *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetxml (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferencetransformNode (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           MSXMLObj_IXMLDOMNode stylesheet,
                                                           char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceselectNodes (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         const char *queryString,
                                                         MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceselectSingleNode (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              const char *queryString,
                                                              MSXMLObj_IXMLDOMNode *resultNode);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetparsed (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VBOOL *isParsed);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetnamespaceURI (CAObjHandle objectHandle,
                                                             ERRORINFO *errorInfo,
                                                             char **namespaceURI);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetprefix (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       char **prefixString);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferenceGetbaseName (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         char **nameString);

HRESULT CVIFUNC MSXML_IXMLDOMEntityReferencetransformNodeToObject (CAObjHandle objectHandle,
                                                                   ERRORINFO *errorInfo,
                                                                   MSXMLObj_IXMLDOMNode stylesheet,
                                                                   VARIANT outputObject);

HRESULT CVIFUNC MSXML_IXMLDOMParseErrorGeterrorCode (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     long *errorCode);

HRESULT CVIFUNC MSXML_IXMLDOMParseErrorGeturl (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               char **urlString);

HRESULT CVIFUNC MSXML_IXMLDOMParseErrorGetreason (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  char **reasonString);

HRESULT CVIFUNC MSXML_IXMLDOMParseErrorGetsrcText (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   char **sourceString);

HRESULT CVIFUNC MSXML_IXMLDOMParseErrorGetline (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                long *lineNumber);

HRESULT CVIFUNC MSXML_IXMLDOMParseErrorGetlinepos (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   long *linePosition);

HRESULT CVIFUNC MSXML_IXMLDOMParseErrorGetfilepos (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   long *filePosition);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetnodeName (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  char **name);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetnodeValue (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VARIANT *value);

HRESULT CVIFUNC MSXML_IXMLDOMNotationSetnodeValue (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VARIANT value);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetnodeType (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLType_DOMNodeType *type);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetparentNode (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNode *parent);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetchildNodes (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNodeList *childList);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetfirstChild (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNode *firstChild);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetlastChild (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode *lastChild);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetpreviousSibling (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSXMLObj_IXMLDOMNode *previousSibling);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetnextSibling (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     MSXMLObj_IXMLDOMNode *nextSibling);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetattributes (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNamedNodeMap *attributeMap);

HRESULT CVIFUNC MSXML_IXMLDOMNotationinsertBefore (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode newChild,
                                                   VARIANT refChild,
                                                   MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMNotationreplaceChild (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode newChild,
                                                   MSXMLObj_IXMLDOMNode oldChild,
                                                   MSXMLObj_IXMLDOMNode *outOldChild);

HRESULT CVIFUNC MSXML_IXMLDOMNotationremoveChild (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNode childNode,
                                                  MSXMLObj_IXMLDOMNode *oldChild);

HRESULT CVIFUNC MSXML_IXMLDOMNotationappendChild (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNode newChild,
                                                  MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMNotationhasChildNodes (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL *hasChild);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetownerDocument (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSXMLObj_IXMLDOMDocument *DOMDocument);

HRESULT CVIFUNC MSXML_IXMLDOMNotationcloneNode (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo, VBOOL deep,
                                                MSXMLObj_IXMLDOMNode *cloneRoot);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetnodeTypeString (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        char **nodeType);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGettext (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, char **text);

HRESULT CVIFUNC MSXML_IXMLDOMNotationSettext (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *text);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetspecified (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VBOOL *isSpecified);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetdefinition (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNode *definitionNode);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetnodeTypedValue (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VARIANT *typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMNotationSetnodeTypedValue (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VARIANT typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetdataType (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VARIANT *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMNotationSetdataType (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetxml (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMNotationtransformNode (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSXMLObj_IXMLDOMNode stylesheet,
                                                    char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMNotationselectNodes (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *queryString,
                                                  MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMNotationselectSingleNode (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       const char *queryString,
                                                       MSXMLObj_IXMLDOMNode *resultNode);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetparsed (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL *isParsed);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetnamespaceURI (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      char **namespaceURI);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetprefix (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                char **prefixString);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetbaseName (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  char **nameString);

HRESULT CVIFUNC MSXML_IXMLDOMNotationtransformNodeToObject (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            MSXMLObj_IXMLDOMNode stylesheet,
                                                            VARIANT outputObject);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetpublicId (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VARIANT *publicId);

HRESULT CVIFUNC MSXML_IXMLDOMNotationGetsystemId (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VARIANT *systemId);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetnodeName (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                char **name);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetnodeValue (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VARIANT *value);

HRESULT CVIFUNC MSXML_IXMLDOMEntitySetnodeValue (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VARIANT value);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetnodeType (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLType_DOMNodeType *type);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetparentNode (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNode *parent);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetchildNodes (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNodeList *childList);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetfirstChild (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNode *firstChild);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetlastChild (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSXMLObj_IXMLDOMNode *lastChild);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetpreviousSibling (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSXMLObj_IXMLDOMNode *previousSibling);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetnextSibling (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMNode *nextSibling);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetattributes (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNamedNodeMap *attributeMap);

HRESULT CVIFUNC MSXML_IXMLDOMEntityinsertBefore (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSXMLObj_IXMLDOMNode newChild,
                                                 VARIANT refChild,
                                                 MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMEntityreplaceChild (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSXMLObj_IXMLDOMNode newChild,
                                                 MSXMLObj_IXMLDOMNode oldChild,
                                                 MSXMLObj_IXMLDOMNode *outOldChild);

HRESULT CVIFUNC MSXML_IXMLDOMEntityremoveChild (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNode childNode,
                                                MSXMLObj_IXMLDOMNode *oldChild);

HRESULT CVIFUNC MSXML_IXMLDOMEntityappendChild (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNode newChild,
                                                MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXMLDOMEntityhasChildNodes (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL *hasChild);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetownerDocument (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     MSXMLObj_IXMLDOMDocument *DOMDocument);

HRESULT CVIFUNC MSXML_IXMLDOMEntitycloneNode (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, VBOOL deep,
                                              MSXMLObj_IXMLDOMNode *cloneRoot);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetnodeTypeString (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      char **nodeType);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGettext (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, char **text);

HRESULT CVIFUNC MSXML_IXMLDOMEntitySettext (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            const char *text);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetspecified (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VBOOL *isSpecified);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetdefinition (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNode *definitionNode);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetnodeTypedValue (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      VARIANT *typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMEntitySetnodeTypedValue (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      VARIANT typedValue);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetdataType (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VARIANT *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMEntitySetdataType (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *dataTypeName);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetxml (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo,
                                           char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMEntitytransformNode (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSXMLObj_IXMLDOMNode stylesheet,
                                                  char **xmlString);

HRESULT CVIFUNC MSXML_IXMLDOMEntityselectNodes (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *queryString,
                                                MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXMLDOMEntityselectSingleNode (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *queryString,
                                                     MSXMLObj_IXMLDOMNode *resultNode);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetparsed (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VBOOL *isParsed);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetnamespaceURI (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    char **namespaceURI);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetprefix (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              char **prefixString);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetbaseName (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                char **nameString);

HRESULT CVIFUNC MSXML_IXMLDOMEntitytransformNodeToObject (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          MSXMLObj_IXMLDOMNode stylesheet,
                                                          VARIANT outputObject);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetpublicId (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VARIANT *publicId);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetsystemId (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VARIANT *systemId);

HRESULT CVIFUNC MSXML_IXMLDOMEntityGetnotationName (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    char **name);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetnodeName (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, char **name);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetnodeValue (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VARIANT *value);

HRESULT CVIFUNC MSXML_IXTLRuntimeSetnodeValue (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VARIANT value);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetnodeType (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              MSXMLType_DOMNodeType *type);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetparentNode (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNode *parent);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetchildNodes (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNodeList *childList);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetfirstChild (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNode *firstChild);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetlastChild (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               MSXMLObj_IXMLDOMNode *lastChild);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetpreviousSibling (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     MSXMLObj_IXMLDOMNode *previousSibling);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetnextSibling (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSXMLObj_IXMLDOMNode *nextSibling);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetattributes (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNamedNodeMap *attributeMap);

HRESULT CVIFUNC MSXML_IXTLRuntimeinsertBefore (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               MSXMLObj_IXMLDOMNode newChild,
                                               VARIANT refChild,
                                               MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXTLRuntimereplaceChild (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               MSXMLObj_IXMLDOMNode newChild,
                                               MSXMLObj_IXMLDOMNode oldChild,
                                               MSXMLObj_IXMLDOMNode *outOldChild);

HRESULT CVIFUNC MSXML_IXTLRuntimeremoveChild (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              MSXMLObj_IXMLDOMNode childNode,
                                              MSXMLObj_IXMLDOMNode *oldChild);

HRESULT CVIFUNC MSXML_IXTLRuntimeappendChild (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              MSXMLObj_IXMLDOMNode newChild,
                                              MSXMLObj_IXMLDOMNode *outNewChild);

HRESULT CVIFUNC MSXML_IXTLRuntimehasChildNodes (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL *hasChild);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetownerDocument (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   MSXMLObj_IXMLDOMDocument *DOMDocument);

HRESULT CVIFUNC MSXML_IXTLRuntimecloneNode (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, VBOOL deep,
                                            MSXMLObj_IXMLDOMNode *cloneRoot);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetnodeTypeString (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    char **nodeType);

HRESULT CVIFUNC MSXML_IXTLRuntimeGettext (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, char **text);

HRESULT CVIFUNC MSXML_IXTLRuntimeSettext (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, const char *text);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetspecified (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *isSpecified);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetdefinition (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNode *definitionNode);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetnodeTypedValue (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VARIANT *typedValue);

HRESULT CVIFUNC MSXML_IXTLRuntimeSetnodeTypedValue (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VARIANT typedValue);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetdataType (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VARIANT *dataTypeName);

HRESULT CVIFUNC MSXML_IXTLRuntimeSetdataType (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *dataTypeName);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetxml (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, char **xmlString);

HRESULT CVIFUNC MSXML_IXTLRuntimetransformNode (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                MSXMLObj_IXMLDOMNode stylesheet,
                                                char **xmlString);

HRESULT CVIFUNC MSXML_IXTLRuntimeselectNodes (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *queryString,
                                              MSXMLObj_IXMLDOMNodeList *resultList);

HRESULT CVIFUNC MSXML_IXTLRuntimeselectSingleNode (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   const char *queryString,
                                                   MSXMLObj_IXMLDOMNode *resultNode);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetparsed (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            VBOOL *isParsed);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetnamespaceURI (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  char **namespaceURI);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetprefix (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            char **prefixString);

HRESULT CVIFUNC MSXML_IXTLRuntimeGetbaseName (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              char **nameString);

HRESULT CVIFUNC MSXML_IXTLRuntimetransformNodeToObject (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        MSXMLObj_IXMLDOMNode stylesheet,
                                                        VARIANT outputObject);

HRESULT CVIFUNC MSXML_IXTLRuntimeuniqueID (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo,
                                           MSXMLObj_IXMLDOMNode pNode, long *pID);

HRESULT CVIFUNC MSXML_IXTLRuntimedepth (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo,
                                        MSXMLObj_IXMLDOMNode pNode, long *pDepth);

HRESULT CVIFUNC MSXML_IXTLRuntimechildNumber (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              MSXMLObj_IXMLDOMNode pNode,
                                              long *pNumber);

HRESULT CVIFUNC MSXML_IXTLRuntimeancestorChildNumber (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      const char *bstrNodeName,
                                                      MSXMLObj_IXMLDOMNode pNode,
                                                      long *pNumber);

HRESULT CVIFUNC MSXML_IXTLRuntimeabsoluteChildNumber (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      MSXMLObj_IXMLDOMNode pNode,
                                                      long *pNumber);

HRESULT CVIFUNC MSXML_IXTLRuntimeformatIndex (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, long lIndex,
                                              const char *bstrFormat,
                                              char **pbstrFormattedString);

HRESULT CVIFUNC MSXML_IXTLRuntimeformatNumber (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               double dblNumber,
                                               const char *bstrFormat,
                                               char **pbstrFormattedString);

HRESULT CVIFUNC MSXML_IXTLRuntimeformatDate (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VARIANT varDate,
                                             const char *bstrFormat,
                                             VARIANT varDestLocale,
                                             char **pbstrFormattedString);

HRESULT CVIFUNC MSXML_IXTLRuntimeformatTime (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VARIANT varTime,
                                             const char *bstrFormat,
                                             VARIANT varDestLocale,
                                             char **pbstrFormattedString);

HRESULT CVIFUNC MSXML_XMLDOMDocumentEventsRegOnondataavailable (CAObjHandle serverObject,
                                                                XMLDOMDocumentEventsRegOnondataavailable_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId);

HRESULT CVIFUNC MSXML_XMLDOMDocumentEventsRegOnonreadystatechange (CAObjHandle serverObject,
                                                                   XMLDOMDocumentEventsRegOnonreadystatechange_CallbackType callbackFunction,
                                                                   void *callbackData,
                                                                   int enableCallbacks,
                                                                   int *callbackId);
#ifdef __cplusplus
    }
#endif
#endif /* _MSXML_H */
