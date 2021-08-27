#include "bklib2.h"
#include <userint.h>

static void CVIFUNC _DBKTreeEventsRegOnNodeClick_EventVTableFunc (void *thisPtr,
                                                                  BSTR keyNode);

static void CVIFUNC _DBKTreeEventsRegOnExpand_EventVTableFunc (void *thisPtr,
                                                               BSTR keyNode);

static void CVIFUNC _DBKTreeEventsRegOnCollapse_EventVTableFunc (void *thisPtr,
                                                                 BSTR keyNode);

static void CVIFUNC _DBKTreeEventsRegOnPopupMenuCommand_EventVTableFunc (void *thisPtr,
                                                                         BSTR keyNode,
                                                                         BSTR keyCol,
                                                                         long menuItem);

static void CVIFUNC _DBKTreeEventsRegOnMouseOverCell_EventVTableFunc (void *thisPtr,
                                                                      BSTR keyNode,
                                                                      BSTR keyCol);

static void CVIFUNC _DBKTreeEventsRegOnModify_EventVTableFunc (void *thisPtr,
                                                               BSTR keyNode,
                                                               BSTR keyCol,
                                                               long modifyType);

static void CVIFUNC _DBKTreeEventsRegOnPrePopupMenuCallback_EventVTableFunc (void *thisPtr,
                                                                             BSTR keyNode,
                                                                             BSTR keyCol,
                                                                             BSTR *pMenuItem);

static void CVIFUNC _DBKTreeEventsRegOnDragOver_EventVTableFunc (void *thisPtr,
                                                                 BSTR keyNode,
                                                                 short nPosition,
                                                                 short state);

static void CVIFUNC _DBKTreeEventsRegOnDragDrop_EventVTableFunc (void *thisPtr,
                                                                 BSTR keyNode,
                                                                 short nPosition);

#define __ActiveXCtrlErrorHandler() \
if ((ctrlId) > 0) \
	{ \
	if (controlID) \
		*controlID = (ctrlId); \
	if (UILError) \
		*UILError = 0; \
	__result = S_OK; \
	} \
else if ((ctrlId) == UIEActiveXError) \
	{ \
	if (controlID) \
		*controlID = 0; \
	if (UILError) \
		*UILError = 0; \
	} \
else \
	{ \
	if (controlID) \
		*controlID = 0; \
	if (UILError) \
		*UILError = (ctrlId); \
	__result = E_CVIAUTO_CVI_UI_ERROR; \
	}

static CA_PARAMDATA __DBKTreeEvents_RegOnNodeClick_CA_PARAMDATA[] =
	{
		{"keyNode", VT_BSTR}
	};

static CA_PARAMDATA __DBKTreeEvents_RegOnExpand_CA_PARAMDATA[] =
	{
		{"keyNode", VT_BSTR}
	};

static CA_PARAMDATA __DBKTreeEvents_RegOnCollapse_CA_PARAMDATA[] =
	{
		{"keyNode", VT_BSTR}
	};

static CA_PARAMDATA __DBKTreeEvents_RegOnPopupMenuCommand_CA_PARAMDATA[] =
	{
		{"keyNode", VT_BSTR},
        {"keyCol", VT_BSTR},
        {"menuItem", VT_I4}
	};

static CA_PARAMDATA __DBKTreeEvents_RegOnMouseOverCell_CA_PARAMDATA[] =
	{
		{"keyNode", VT_BSTR},
        {"keyCol", VT_BSTR}
	};

static CA_PARAMDATA __DBKTreeEvents_RegOnModify_CA_PARAMDATA[] =
	{
		{"keyNode", VT_BSTR},
        {"keyCol", VT_BSTR},
        {"modifyType", VT_I4}
	};

static CA_PARAMDATA __DBKTreeEvents_RegOnPrePopupMenuCallback_CA_PARAMDATA[] =
	{
		{"keyNode", VT_BSTR},
        {"keyCol", VT_BSTR},
        {"pMenuItem", VT_BSTR | VT_BYREF}
	};

static CA_PARAMDATA __DBKTreeEvents_RegOnDragOver_CA_PARAMDATA[] =
	{
		{"keyNode", VT_BSTR},
        {"nPosition", VT_I2},
        {"state", VT_I2}
	};

static CA_PARAMDATA __DBKTreeEvents_RegOnDragDrop_CA_PARAMDATA[] =
	{
		{"keyNode", VT_BSTR},
        {"nPosition", VT_I2}
	};

static CA_METHODDATA __DBKTreeEvents_CA_METHODDATA[] =
	{
		{"NodeClick", __DBKTreeEvents_RegOnNodeClick_CA_PARAMDATA, 1, 0, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"Expand", __DBKTreeEvents_RegOnExpand_CA_PARAMDATA, 2, 1, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"Collapse", __DBKTreeEvents_RegOnCollapse_CA_PARAMDATA, 3, 2, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"PopupMenuCommand", __DBKTreeEvents_RegOnPopupMenuCommand_CA_PARAMDATA, 4, 3, CC_STDCALL, 3, DISPATCH_METHOD, VT_EMPTY},
        {"MouseOverCell", __DBKTreeEvents_RegOnMouseOverCell_CA_PARAMDATA, 5, 4, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"Modify", __DBKTreeEvents_RegOnModify_CA_PARAMDATA, 6, 5, CC_STDCALL, 3, DISPATCH_METHOD, VT_EMPTY},
        {"PrePopupMenuCallback", __DBKTreeEvents_RegOnPrePopupMenuCallback_CA_PARAMDATA, 7, 6, CC_STDCALL, 3, DISPATCH_METHOD, VT_EMPTY},
        {"DragOver", __DBKTreeEvents_RegOnDragOver_CA_PARAMDATA, 8, 7, CC_STDCALL, 3, DISPATCH_METHOD, VT_EMPTY},
        {"DragDrop", __DBKTreeEvents_RegOnDragDrop_CA_PARAMDATA, 9, 8, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY}
	};

static CA_INTERFACEDATA __DBKTreeEvents_CA_INTERFACEDATA =
	{
		__DBKTreeEvents_CA_METHODDATA,
        sizeof (__DBKTreeEvents_CA_METHODDATA) / sizeof (*__DBKTreeEvents_CA_METHODDATA)
	};

static void * __DBKTreeEvents_EventVTable[] =
	{
		_DBKTreeEventsRegOnNodeClick_EventVTableFunc,
        _DBKTreeEventsRegOnExpand_EventVTableFunc,
        _DBKTreeEventsRegOnCollapse_EventVTableFunc,
        _DBKTreeEventsRegOnPopupMenuCommand_EventVTableFunc,
        _DBKTreeEventsRegOnMouseOverCell_EventVTableFunc,
        _DBKTreeEventsRegOnModify_EventVTableFunc,
        _DBKTreeEventsRegOnPrePopupMenuCallback_EventVTableFunc,
        _DBKTreeEventsRegOnDragOver_EventVTableFunc,
        _DBKTreeEventsRegOnDragDrop_EventVTableFunc
	};

static CAEventClassDefn __DBKTreeEvents_CAEventClassDefn =
	{
		20,
        &BKLib2_IID__DBKTreeEvents,
        __DBKTreeEvents_EventVTable,
        &__DBKTreeEvents_CA_INTERFACEDATA,
        0
	};

const IID BKLib2_IID__DBKTree =
	{
		0x315BFB14, 0x49DC, 0x11D5, 0x84, 0x5, 0x0, 0xE0, 0x18, 0x8E, 0x24, 0xAC
	};

const IID BKLib2_IID__DBKGrid =
	{
		0x315BFB14, 0x49DC, 0x11D6, 0x84, 0x5, 0x0, 0xE0, 0x18, 0x8E, 0x24, 0xAC
	};

const IID BKLib2_IID__DBKTreeEvents =
	{
		0x315BFB15, 0x49DC, 0x11D5, 0x84, 0x5, 0x0, 0xE0, 0x18, 0x8E, 0x24, 0xAC
	};

const IID BKLib2_IID__DBKGridEvents =
	{
		0x315BFB15, 0x49DC, 0x11D6, 0x84, 0x5, 0x0, 0xE0, 0x18, 0x8E, 0x24, 0xAC
	};

const IID BKLib2_IID_Font =
	{
		0xBEF6E003, 0xA874, 0x101A, 0x8B, 0xBA, 0x0, 0xAA, 0x0, 0x30, 0xC, 0xAB
	};

HRESULT CVIFUNC BKLib2_New_DBKTree (int panel, const char *label, int top,
                                    int left, int *controlID, int *UILError)
{
	HRESULT __result = S_OK;
	int ctrlId;
	GUID clsid = {0x315BFB16, 0x49DC, 0x11D5, 0x84, 0x5, 0x0, 0xE0, 0x18, 0x8E,
	              0x24, 0xAC};
	const char * licStr = "Copyright (c) 2005 Elcom a.s. BKTreeCtrl (2.1)";

	ctrlId = NewActiveXCtrl (panel, label, top, left, &clsid,
	                         &BKLib2_IID__DBKTree, licStr, &__result);

	__ActiveXCtrlErrorHandler();

	return __result;
}

HRESULT CVIFUNC BKLib2_Open_DBKTree (const char *fileName, int panel,
                                     const char *label, int top, int left,
                                     int *controlID, int *UILError)
{
	HRESULT __result = S_OK;
	int ctrlId;

	ctrlId = NewActiveXCtrlFromFile (panel, label, top, left, fileName,
	                                 &BKLib2_IID__DBKTree, &__result);

	__ActiveXCtrlErrorHandler();

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeAddColumn (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          const char *keyCol, const char *name,
                                          short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x18, CAVT_SHORT,
	                              returnValue, 2, __paramTypes, keyCol, name);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnName (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *keyCol,
                                              char **returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x19, CAVT_CSTRING,
	                              returnValue, 1, __paramTypes, keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnCount (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               short *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x1A, CAVT_SHORT,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeRemoveColumn (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             const char *keyCol,
                                             short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x1B, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeResetContent (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             short *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x1C, CAVT_SHORT,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetColumnName (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *keyCol,
                                              const char *name,
                                              short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x1D, CAVT_SHORT,
	                              returnValue, 2, __paramTypes, keyCol, name);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeAddNode (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo,
                                        const char *keyParent,
                                        const char *keyNode, const char *text,
                                        short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x1E, CAVT_SHORT,
	                              returnValue, 3, __paramTypes, keyParent,
	                              keyNode, text);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeRemoveNode (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo,
                                           const char *keyNode,
                                           short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x1F, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, keyNode);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemText (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *keyNode,
                                                const char *keyCol,
                                                const char *text,
                                                short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x20, CAVT_SHORT,
	                              returnValue, 3, __paramTypes, keyNode,
	                              keyCol, text);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemText (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *keyNode,
                                                const char *keyCol,
                                                char **returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x21, CAVT_CSTRING,
	                              returnValue, 2, __paramTypes, keyNode,
	                              keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemTextEx (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *keyNode,
                                                  const char *keyCol,
                                                  char **returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x22, CAVT_CSTRING,
	                              returnValue, 2, __paramTypes, keyNode,
	                              keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetColumnWidth (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               const char *keyCol, long nWidth,
                                               short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_LONG};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x23, CAVT_SHORT,
	                              returnValue, 2, __paramTypes, keyCol, nWidth);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemImage (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *keyNode,
                                                 const char *keyCol,
                                                 short nImage)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING, CAVT_SHORT};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x24, CAVT_EMPTY, NULL,
	                              3, __paramTypes, keyNode, keyCol, nImage);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetRedraw (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, short bRedraw,
                                          short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_SHORT};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x25, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, bRedraw);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeExpand (CAObjHandle objectHandle,
                                       ERRORINFO *errorInfo, const char *keyNode,
                                       short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x26, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, keyNode);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeCollapse (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo,
                                         const char *keyNode, short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x27, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, keyNode);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeExpandLevel (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, long nLevel,
                                            short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_LONG};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x28, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, nLevel);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemFormat (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *keyNode,
                                                  const char *keyCol,
                                                  short nodeFormat)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING, CAVT_SHORT};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x29, CAVT_EMPTY, NULL,
	                              3, __paramTypes, keyNode, keyCol, nodeFormat);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemFormat (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *keyNode,
                                                  const char *keyCol,
                                                  short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x2A, CAVT_SHORT,
	                              returnValue, 2, __paramTypes, keyNode,
	                              keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeFontStyle (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *keyNode,
                                                 short nodeStyle)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_SHORT};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x2B, CAVT_EMPTY, NULL,
	                              2, __paramTypes, keyNode, nodeStyle);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeFontStyle (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *keyNode,
                                                 short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x2C, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, keyNode);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetParentNode (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *keyNode,
                                              char **returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x2D, CAVT_CSTRING,
	                              returnValue, 1, __paramTypes, keyNode);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetScrollBarVisible (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    short nBar, short nNewValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_SHORT, CAVT_SHORT};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x2E, CAVT_EMPTY, NULL,
	                              2, __paramTypes, nBar, nNewValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetScrollBarVisible (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    short nBar,
                                                    short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_SHORT};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x2F, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, nBar);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemFontStyle (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *keyNode,
                                                     const char *keyCol,
                                                     short nodeStyle)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING, CAVT_SHORT};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x30, CAVT_EMPTY, NULL,
	                              3, __paramTypes, keyNode, keyCol, nodeStyle);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemFontStyle (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *keyNode,
                                                     const char *keyCol,
                                                     short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x31, CAVT_SHORT,
	                              returnValue, 2, __paramTypes, keyNode,
	                              keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemTextColor (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *keyNode,
                                                     const char *keyCol,
                                                     BKLib2Type_OLE_COLOR nNewValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING, CAVT_ULONG};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x32, CAVT_EMPTY, NULL,
	                              3, __paramTypes, keyNode, keyCol, nNewValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemTextColor (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *keyNode,
                                                     const char *keyCol,
                                                     BKLib2Type_OLE_COLOR *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x33, CAVT_ULONG,
	                              returnValue, 2, __paramTypes, keyNode,
	                              keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemBackColor (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *keyNode,
                                                     const char *keyCol,
                                                     BKLib2Type_OLE_COLOR nNewValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING, CAVT_ULONG};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x34, CAVT_EMPTY, NULL,
	                              3, __paramTypes, keyNode, keyCol, nNewValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemBackColor (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *keyNode,
                                                     const char *keyCol,
                                                     BKLib2Type_OLE_COLOR *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x35, CAVT_ULONG,
	                              returnValue, 2, __paramTypes, keyNode,
	                              keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeIsExpanded (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo,
                                           const char *keyNode,
                                           short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x36, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, keyNode);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeMoveNode (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo,
                                         const char *keyNode,
                                         const char *keyTargetNode,
                                         short nPosition, short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING, CAVT_SHORT};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x37, CAVT_SHORT,
	                              returnValue, 3, __paramTypes, keyNode,
	                              keyTargetNode, nPosition);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetPopupMenu (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             const char *menuItems,
                                             short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x38, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, menuItems);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemEditable (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    const char *keyNode,
                                                    const char *keyCol,
                                                    short bEditable)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING, CAVT_SHORT};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x39, CAVT_EMPTY, NULL,
	                              3, __paramTypes, keyNode, keyCol, bEditable);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemEditable (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    const char *keyNode,
                                                    const char *keyCol,
                                                    short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x3A, CAVT_SHORT,
	                              returnValue, 2, __paramTypes, keyNode,
	                              keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeFromArray (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, VARIANT *vArray,
                                          short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_VARIANT | CAVT_BYREFI};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x3B, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, vArray);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeToArray (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo,
                                        VARIANT *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x3C, CAVT_VARIANT,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeFromString (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo,
                                           const char *strString,
                                           const char *chSeparator,
                                           const char *chLine,
                                           short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x3D, CAVT_SHORT,
	                              returnValue, 3, __paramTypes, strString,
	                              chSeparator, chLine);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeToString (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo,
                                         const char *chSeparator,
                                         const char *chLine, char **returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x3E, CAVT_CSTRING,
	                              returnValue, 2, __paramTypes, chSeparator,
	                              chLine);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeToString2 (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          const char *chSeparator,
                                          const char *chLine, char **returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x3F, CAVT_CSTRING,
	                              returnValue, 2, __paramTypes, chSeparator,
	                              chLine);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnNext (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *keyCol,
                                              char **returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x40, CAVT_CSTRING,
	                              returnValue, 1, __paramTypes, keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeNext (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            const char *keyNode, short position,
                                            char **returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_SHORT};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x41, CAVT_CSTRING,
	                              returnValue, 2, __paramTypes, keyNode,
	                              position);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeHasChildren (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   const char *keyNode,
                                                   short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x42, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, keyNode);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemImage (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *keyNode,
                                                 const char *keyCol,
                                                 short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x43, CAVT_SHORT,
	                              returnValue, 2, __paramTypes, keyNode,
	                              keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemTextEx (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *keyNode,
                                                  const char *keyCol,
                                                  const char *text,
                                                  short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x44, CAVT_SHORT,
	                              returnValue, 3, __paramTypes, keyNode,
	                              keyCol, text);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeEnsureVisible (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *keyNode)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x45, CAVT_EMPTY, NULL,
	                              1, __paramTypes, keyNode);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeRenameNodeKey (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *keyNode,
                                              const char *keyNew,
                                              short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x46, CAVT_SHORT,
	                              returnValue, 2, __paramTypes, keyNode,
	                              keyNew);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeIsNodeExist (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            const char *keyNode,
                                            short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x47, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, keyNode);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetColumnHeadFontStyle (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       const char *keyCol,
                                                       short headStyle)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_SHORT};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x48, CAVT_EMPTY, NULL,
	                              2, __paramTypes, keyCol, headStyle);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnHeadFontStyle (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       const char *keyCol,
                                                       short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x49, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetColumnHeadFormat (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    const char *keyCol,
                                                    short headFormat)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_SHORT};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x4A, CAVT_EMPTY, NULL,
	                              2, __paramTypes, keyCol, headFormat);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnHeadFormat (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    const char *keyCol,
                                                    short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x4B, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetVersion (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo,
                                           char **returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x4C, CAVT_CSTRING,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetFrameBorder (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               long nNewStyle, long nNewStyleEx)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_LONG, CAVT_LONG};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x4D, CAVT_EMPTY, NULL,
	                              2, __paramTypes, nNewStyle, nNewStyleEx);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetFrameBorder (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               long *pStyle, long *pStyleEx)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_LONG | CAVT_BYREFI,
	                               CAVT_LONG | CAVT_BYREFI};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x4E, CAVT_EMPTY, NULL,
	                              2, __paramTypes, pStyle, pStyleEx);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeFromXML (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo, const char *strXML,
                                        short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x4F, CAVT_SHORT,
	                              returnValue, 1, __paramTypes, strXML);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeToXML (CAObjHandle objectHandle,
                                      ERRORINFO *errorInfo, char **returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x50, CAVT_CSTRING,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeData (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            const char *keyNode, long lData)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_LONG};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x51, CAVT_EMPTY, NULL,
	                              2, __paramTypes, keyNode, lData);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeData (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            const char *keyNode,
                                            long *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x52, CAVT_LONG,
	                              returnValue, 1, __paramTypes, keyNode);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemData (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *keyNode,
                                                const char *keyCol, long lData)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING, CAVT_LONG};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x53, CAVT_EMPTY, NULL,
	                              3, __paramTypes, keyNode, keyCol, lData);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemData (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *keyNode,
                                                const char *keyCol,
                                                long *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x54, CAVT_LONG,
	                              returnValue, 2, __paramTypes, keyNode,
	                              keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnWidth (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               const char *keyCol,
                                               long *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x55, CAVT_LONG,
	                              returnValue, 1, __paramTypes, keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemTipTextEnable (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         const char *keyNode,
                                                         const char *keyCol,
                                                         short bEnable)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING, CAVT_SHORT};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x56, CAVT_EMPTY, NULL,
	                              3, __paramTypes, keyNode, keyCol, bEnable);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemTipTextEnable (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         const char *keyNode,
                                                         const char *keyCol,
                                                         short *returnValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING, CAVT_CSTRING};

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x57, CAVT_SHORT,
	                              returnValue, 2, __paramTypes, keyNode,
	                              keyCol);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeRefresh (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xFFFFFDDA, CAVT_EMPTY,
	                              NULL, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeAboutBox (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xFFFFFDD8, CAVT_EMPTY,
	                              NULL, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetSelectedItem (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                char **returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x1,
	                              DISPATCH_PROPERTYGET, CAVT_CSTRING,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetSelectedItem (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x1,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefSelectedItem (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     char **newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_CSTRING | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x1,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetBackColor (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             BKLib2Type_OLE_COLOR *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x2,
	                              DISPATCH_PROPERTYGET, CAVT_ULONG,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetBackColor (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             BKLib2Type_OLE_COLOR newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_ULONG};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x2,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefBackColor (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  BKLib2Type_OLE_COLOR *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_ULONG | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x2,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetBackTextColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x3,
	                              DISPATCH_PROPERTYGET, CAVT_ULONG,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetBackTextColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_ULONG};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x3,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefBackTextColor (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      BKLib2Type_OLE_COLOR *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_ULONG | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x3,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetForeTextColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x4,
	                              DISPATCH_PROPERTYGET, CAVT_ULONG,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetForeTextColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_ULONG};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x4,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefForeTextColor (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      BKLib2Type_OLE_COLOR *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_ULONG | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x4,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnHeadingVisible (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x5,
	                              DISPATCH_PROPERTYGET, CAVT_BOOL, returnValue,
	                              0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetColumnHeadingVisible (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x5,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefColumnHeadingVisible (CAObjHandle objectHandle,
                                                             ERRORINFO *errorInfo,
                                                             VBOOL *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x5,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetGridLines (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             short *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x6,
	                              DISPATCH_PROPERTYGET, CAVT_SHORT,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetGridLines (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             short newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_SHORT};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x6,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefGridLines (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  short *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_SHORT | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x6,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetGridLineColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x7,
	                              DISPATCH_PROPERTYGET, CAVT_ULONG,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetGridLineColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_ULONG};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x7,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefGridLineColor (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      BKLib2Type_OLE_COLOR *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_ULONG | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x7,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetImageList (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             CAObjHandle *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x8,
	                              DISPATCH_PROPERTYGET, CAVT_OBJHANDLE,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetImageList (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             CAObjHandle newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_OBJHANDLE};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x8,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefImageList (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  CAObjHandle *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_OBJHANDLE | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x8,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetFont (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo,
                                        BKLib2Obj_Font *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x9,
	                              DISPATCH_PROPERTYGET, CAVT_OBJHANDLE,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetFont (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo,
                                        BKLib2Obj_Font newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_OBJHANDLE};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x9,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefFont (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             BKLib2Obj_Font *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_OBJHANDLE | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x9,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnResizing (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xA,
	                              DISPATCH_PROPERTYGET, CAVT_BOOL, returnValue,
	                              0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetColumnResizing (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xA,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefColumnResizing (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VBOOL *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xA,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetAutoImages (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VBOOL *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xB,
	                              DISPATCH_PROPERTYGET, CAVT_BOOL, returnValue,
	                              0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetAutoImages (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VBOOL newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xB,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefAutoImages (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VBOOL *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xB,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetTreeLineColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xC,
	                              DISPATCH_PROPERTYGET, CAVT_ULONG,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetTreeLineColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_ULONG};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xC,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefTreeLineColor (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      BKLib2Type_OLE_COLOR *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_ULONG | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xC,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetTreeLines (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             short *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xD,
	                              DISPATCH_PROPERTYGET, CAVT_SHORT,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetTreeLines (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             short newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_SHORT};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xD,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefTreeLines (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  short *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_SHORT | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xD,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetEnableEventMouseOverCell (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VBOOL *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xE,
	                              DISPATCH_PROPERTYGET, CAVT_BOOL, returnValue,
	                              0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetEnableEventMouseOverCell (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VBOOL newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xE,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefEnableEventMouseOverCell (CAObjHandle objectHandle,
                                                                 ERRORINFO *errorInfo,
                                                                 VBOOL *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xE,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetEnableEventModify (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xF,
	                              DISPATCH_PROPERTYGET, CAVT_BOOL, returnValue,
	                              0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetEnableEventModify (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xF,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefEnableEventModify (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          VBOOL *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0xF,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetEditable (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            VBOOL *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x10,
	                              DISPATCH_PROPERTYGET, CAVT_BOOL, returnValue,
	                              0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetEditable (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, VBOOL newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x10,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefEditable (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VBOOL *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x10,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetEnableDragAndDrop (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x11,
	                              DISPATCH_PROPERTYGET, CAVT_BOOL, returnValue,
	                              0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetEnableDragAndDrop (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x11,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefEnableDragAndDrop (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          VBOOL *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x11,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetDragMode (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            short *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x12,
	                              DISPATCH_PROPERTYGET, CAVT_SHORT,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetDragMode (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, short newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_SHORT};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x12,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefDragMode (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 short *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_SHORT | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x12,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetDragIcon (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            short *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x13,
	                              DISPATCH_PROPERTYGET, CAVT_SHORT,
	                              returnValue, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetDragIcon (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, short newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_SHORT};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x13,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefDragIcon (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 short *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_SHORT | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x13,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetEnableSelection (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VBOOL *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x14,
	                              DISPATCH_PROPERTYGET, CAVT_BOOL, returnValue,
	                              0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetEnableSelection (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VBOOL newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x14,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefEnableSelection (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x14,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetEnableFocusCell (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VBOOL *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x15,
	                              DISPATCH_PROPERTYGET, CAVT_BOOL, returnValue,
	                              0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetEnableFocusCell (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VBOOL newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x15,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefEnableFocusCell (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x15,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetEnableTreeKeyDirection (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          VBOOL *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x16,
	                              DISPATCH_PROPERTYGET, CAVT_BOOL, returnValue,
	                              0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetEnableTreeKeyDirection (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          VBOOL newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x16,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefEnableTreeKeyDirection (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               VBOOL *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x16,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeGetListMode (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            VBOOL *returnValue)
{
	HRESULT __result = S_OK;

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x17,
	                              DISPATCH_PROPERTYGET, CAVT_BOOL, returnValue,
	                              0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetListMode (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, VBOOL newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x17,
	                              DISPATCH_PROPERTYPUT, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefListMode (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VBOOL *newValue)
{
	HRESULT __result = S_OK;
	unsigned int __paramTypes[] = {CAVT_BOOL | CAVT_BYREFI};

	__result = CA_InvokeHelperEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKTree, 0x17,
	                              DISPATCH_PROPERTYPUTREF, CAVT_EMPTY, NULL, 1,
	                              __paramTypes, newValue);

	return __result;
}

HRESULT CVIFUNC BKLib2_New_DBKGrid (int panel, const char *label, int top,
                                    int left, int *controlID, int *UILError)
{
	HRESULT __result = S_OK;
	int ctrlId;
	GUID clsid = {0x315BFB16, 0x49DC, 0x11D6, 0x84, 0x5, 0x0, 0xE0, 0x18, 0x8E,
	              0x24, 0xAC};
	const char * licStr = "Copyright (c) 2005 Elcom a.s. BKTreeCtrl (2.1)";

	ctrlId = NewActiveXCtrl (panel, label, top, left, &clsid,
	                         &BKLib2_IID__DBKGrid, licStr, &__result);

	__ActiveXCtrlErrorHandler();

	return __result;
}

HRESULT CVIFUNC BKLib2_Open_DBKGrid (const char *fileName, int panel,
                                     const char *label, int top, int left,
                                     int *controlID, int *UILError)
{
	HRESULT __result = S_OK;
	int ctrlId;

	ctrlId = NewActiveXCtrlFromFile (panel, label, top, left, fileName,
	                                 &BKLib2_IID__DBKGrid, &__result);

	__ActiveXCtrlErrorHandler();

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKGridRefresh (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKGrid, 0xFFFFFDDA, CAVT_EMPTY,
	                              NULL, 0, NULL);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKGridAboutBox (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;

	__result = CA_MethodInvokeEx (objectHandle, errorInfo,
	                              &BKLib2_IID__DBKGrid, 0xFFFFFDD8, CAVT_EMPTY,
	                              NULL, 0, NULL);

	return __result;
}

static void CVIFUNC _DBKTreeEventsRegOnNodeClick_EventVTableFunc (void *thisPtr,
                                                                  BSTR keyNode)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	_DBKTreeEventsRegOnNodeClick_CallbackType __callbackFunction;
	char * keyNode__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 0, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (keyNode, &keyNode__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyNode__CType);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (keyNode__CType);

	return;
}

static void CVIFUNC _DBKTreeEventsRegOnExpand_EventVTableFunc (void *thisPtr,
                                                               BSTR keyNode)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	_DBKTreeEventsRegOnExpand_CallbackType __callbackFunction;
	char * keyNode__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 1, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (keyNode, &keyNode__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyNode__CType);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (keyNode__CType);

	return;
}

static void CVIFUNC _DBKTreeEventsRegOnCollapse_EventVTableFunc (void *thisPtr,
                                                                 BSTR keyNode)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	_DBKTreeEventsRegOnCollapse_CallbackType __callbackFunction;
	char * keyNode__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 2, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (keyNode, &keyNode__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyNode__CType);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (keyNode__CType);

	return;
}

static void CVIFUNC _DBKTreeEventsRegOnPopupMenuCommand_EventVTableFunc (void *thisPtr,
                                                                         BSTR keyNode,
                                                                         BSTR keyCol,
                                                                         long menuItem)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	_DBKTreeEventsRegOnPopupMenuCommand_CallbackType __callbackFunction;
	char * keyNode__CType = 0;
	char * keyCol__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 3, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (keyNode, &keyNode__CType));
		__caErrChk (CA_BSTRGetCString (keyCol, &keyCol__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyNode__CType,
		                               keyCol__CType, menuItem);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (keyNode__CType);
	CA_FreeMemory (keyCol__CType);

	return;
}

static void CVIFUNC _DBKTreeEventsRegOnMouseOverCell_EventVTableFunc (void *thisPtr,
                                                                      BSTR keyNode,
                                                                      BSTR keyCol)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	_DBKTreeEventsRegOnMouseOverCell_CallbackType __callbackFunction;
	char * keyNode__CType = 0;
	char * keyCol__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 4, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (keyNode, &keyNode__CType));
		__caErrChk (CA_BSTRGetCString (keyCol, &keyCol__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyNode__CType,
		                               keyCol__CType);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (keyNode__CType);
	CA_FreeMemory (keyCol__CType);

	return;
}

static void CVIFUNC _DBKTreeEventsRegOnModify_EventVTableFunc (void *thisPtr,
                                                               BSTR keyNode,
                                                               BSTR keyCol,
                                                               long modifyType)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	_DBKTreeEventsRegOnModify_CallbackType __callbackFunction;
	char * keyNode__CType = 0;
	char * keyCol__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 5, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (keyNode, &keyNode__CType));
		__caErrChk (CA_BSTRGetCString (keyCol, &keyCol__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyNode__CType,
		                               keyCol__CType, modifyType);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (keyNode__CType);
	CA_FreeMemory (keyCol__CType);

	return;
}

static void CVIFUNC _DBKTreeEventsRegOnPrePopupMenuCallback_EventVTableFunc (void *thisPtr,
                                                                             BSTR keyNode,
                                                                             BSTR keyCol,
                                                                             BSTR *pMenuItem)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	_DBKTreeEventsRegOnPrePopupMenuCallback_CallbackType __callbackFunction;
	char * keyNode__CType = 0;
	char * keyCol__CType = 0;
	char * pMenuItem__CType = 0;
	BSTR pMenuItem__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 6, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (keyNode, &keyNode__CType));
		__caErrChk (CA_BSTRGetCString (keyCol, &keyCol__CType));
		__caErrChk (CA_BSTRGetCString (*pMenuItem, &pMenuItem__CType));
		CA_FreeBSTR (*pMenuItem);
		*pMenuItem = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyNode__CType,
		                               keyCol__CType, &pMenuItem__CType);
	
		__caErrChk (__result);
	
		__caErrChk (CA_CStringToBSTR (pMenuItem__CType, &pMenuItem__AutoType));
		*pMenuItem = pMenuItem__AutoType;
		pMenuItem__AutoType = 0;
		}
Error:
	CA_FreeMemory (keyNode__CType);
	CA_FreeMemory (keyCol__CType);
	CA_FreeMemory (pMenuItem__CType);
	CA_FreeBSTR (pMenuItem__AutoType);

	return;
}

static void CVIFUNC _DBKTreeEventsRegOnDragOver_EventVTableFunc (void *thisPtr,
                                                                 BSTR keyNode,
                                                                 short nPosition,
                                                                 short state)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	_DBKTreeEventsRegOnDragOver_CallbackType __callbackFunction;
	char * keyNode__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 7, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (keyNode, &keyNode__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyNode__CType,
		                               nPosition, state);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (keyNode__CType);

	return;
}

static void CVIFUNC _DBKTreeEventsRegOnDragDrop_EventVTableFunc (void *thisPtr,
                                                                 BSTR keyNode,
                                                                 short nPosition)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	_DBKTreeEventsRegOnDragDrop_CallbackType __callbackFunction;
	char * keyNode__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 8, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (keyNode, &keyNode__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyNode__CType,
		                               nPosition);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (keyNode__CType);

	return;
}

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnNodeClick (CAObjHandle serverObject,
                                                     _DBKTreeEventsRegOnNodeClick_CallbackType callbackFunction,
                                                     void *callbackData,
                                                     int enableCallbacks,
                                                     int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &__DBKTreeEvents_CAEventClassDefn, 0,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnExpand (CAObjHandle serverObject,
                                                  _DBKTreeEventsRegOnExpand_CallbackType callbackFunction,
                                                  void *callbackData,
                                                  int enableCallbacks,
                                                  int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &__DBKTreeEvents_CAEventClassDefn, 1,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnCollapse (CAObjHandle serverObject,
                                                    _DBKTreeEventsRegOnCollapse_CallbackType callbackFunction,
                                                    void *callbackData,
                                                    int enableCallbacks,
                                                    int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &__DBKTreeEvents_CAEventClassDefn, 2,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnPopupMenuCommand (CAObjHandle serverObject,
                                                            _DBKTreeEventsRegOnPopupMenuCommand_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &__DBKTreeEvents_CAEventClassDefn, 3,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnMouseOverCell (CAObjHandle serverObject,
                                                         _DBKTreeEventsRegOnMouseOverCell_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &__DBKTreeEvents_CAEventClassDefn, 4,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnModify (CAObjHandle serverObject,
                                                  _DBKTreeEventsRegOnModify_CallbackType callbackFunction,
                                                  void *callbackData,
                                                  int enableCallbacks,
                                                  int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &__DBKTreeEvents_CAEventClassDefn, 5,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnPrePopupMenuCallback (CAObjHandle serverObject,
                                                                _DBKTreeEventsRegOnPrePopupMenuCallback_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &__DBKTreeEvents_CAEventClassDefn, 6,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnDragOver (CAObjHandle serverObject,
                                                    _DBKTreeEventsRegOnDragOver_CallbackType callbackFunction,
                                                    void *callbackData,
                                                    int enableCallbacks,
                                                    int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &__DBKTreeEvents_CAEventClassDefn, 7,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnDragDrop (CAObjHandle serverObject,
                                                    _DBKTreeEventsRegOnDragDrop_CallbackType callbackFunction,
                                                    void *callbackData,
                                                    int enableCallbacks,
                                                    int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &__DBKTreeEvents_CAEventClassDefn, 8,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}
