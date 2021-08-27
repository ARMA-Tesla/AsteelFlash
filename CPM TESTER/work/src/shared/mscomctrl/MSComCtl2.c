#include "MSComCtl2.h"
#include <userint.h>

static void CVIFUNC DAnimationEventsRegOnClick_EventVTableFunc (void *thisPtr);

static void CVIFUNC DAnimationEventsRegOnDblClick_EventVTableFunc (void *thisPtr);

static void CVIFUNC DAnimationEventsRegOnMouseDown_EventVTableFunc (void *thisPtr,
                                                                    short button,
                                                                    short shift,
                                                                    long x,
                                                                    long y);

static void CVIFUNC DAnimationEventsRegOnMouseMove_EventVTableFunc (void *thisPtr,
                                                                    short button,
                                                                    short shift,
                                                                    long x,
                                                                    long y);

static void CVIFUNC DAnimationEventsRegOnMouseUp_EventVTableFunc (void *thisPtr,
                                                                  short button,
                                                                  short shift,
                                                                  long x, long y);

static void CVIFUNC DAnimationEventsRegOnOLEStartDrag_EventVTableFunc (void *thisPtr,
                                                                       LPDISPATCH *data,
                                                                       long *allowedEffects);

static void CVIFUNC DAnimationEventsRegOnOLEGiveFeedback_EventVTableFunc (void *thisPtr,
                                                                          long *effect,
                                                                          VBOOL *defaultCursors);

static void CVIFUNC DAnimationEventsRegOnOLESetData_EventVTableFunc (void *thisPtr,
                                                                     LPDISPATCH *data,
                                                                     short *dataFormat);

static void CVIFUNC DAnimationEventsRegOnOLECompleteDrag_EventVTableFunc (void *thisPtr,
                                                                          long *effect);

static void CVIFUNC DAnimationEventsRegOnOLEDragOver_EventVTableFunc (void *thisPtr,
                                                                      LPDISPATCH *data,
                                                                      long *effect,
                                                                      short *button,
                                                                      short *shift,
                                                                      float *x,
                                                                      float *y,
                                                                      short *state);

static void CVIFUNC DAnimationEventsRegOnOLEDragDrop_EventVTableFunc (void *thisPtr,
                                                                      LPDISPATCH *data,
                                                                      long *effect,
                                                                      short *button,
                                                                      short *shift,
                                                                      float *x,
                                                                      float *y);

static void CVIFUNC DUpDownEventsRegOnChange_EventVTableFunc (void *thisPtr);

static void CVIFUNC DUpDownEventsRegOnDownClick_EventVTableFunc (void *thisPtr);

static void CVIFUNC DUpDownEventsRegOnUpClick_EventVTableFunc (void *thisPtr);

static void CVIFUNC DUpDownEventsRegOnMouseDown_EventVTableFunc (void *thisPtr,
                                                                 short button,
                                                                 short shift,
                                                                 long x, long y);

static void CVIFUNC DUpDownEventsRegOnMouseMove_EventVTableFunc (void *thisPtr,
                                                                 short button,
                                                                 short shift,
                                                                 long x, long y);

static void CVIFUNC DUpDownEventsRegOnMouseUp_EventVTableFunc (void *thisPtr,
                                                               short button,
                                                               short shift,
                                                               long x, long y);

static void CVIFUNC DUpDownEventsRegOnOLEStartDrag_EventVTableFunc (void *thisPtr,
                                                                    LPDISPATCH *data,
                                                                    long *allowedEffects);

static void CVIFUNC DUpDownEventsRegOnOLEGiveFeedback_EventVTableFunc (void *thisPtr,
                                                                       long *effect,
                                                                       VBOOL *defaultCursors);

static void CVIFUNC DUpDownEventsRegOnOLESetData_EventVTableFunc (void *thisPtr,
                                                                  LPDISPATCH *data,
                                                                  short *dataFormat);

static void CVIFUNC DUpDownEventsRegOnOLECompleteDrag_EventVTableFunc (void *thisPtr,
                                                                       long *effect);

static void CVIFUNC DUpDownEventsRegOnOLEDragOver_EventVTableFunc (void *thisPtr,
                                                                   LPDISPATCH *data,
                                                                   long *effect,
                                                                   short *button,
                                                                   short *shift,
                                                                   float *x,
                                                                   float *y,
                                                                   short *state);

static void CVIFUNC DUpDownEventsRegOnOLEDragDrop_EventVTableFunc (void *thisPtr,
                                                                   LPDISPATCH *data,
                                                                   long *effect,
                                                                   short *button,
                                                                   short *shift,
                                                                   float *x,
                                                                   float *y);

static void CVIFUNC DMonthViewEventsRegOnDateClick_EventVTableFunc (void *thisPtr,
                                                                    DATE dateClicked);

static void CVIFUNC DMonthViewEventsRegOnDateDblClick_EventVTableFunc (void *thisPtr,
                                                                       DATE dateDblClicked);

static void CVIFUNC DMonthViewEventsRegOnGetDayBold_EventVTableFunc (void *thisPtr,
                                                                     DATE startDate,
                                                                     short count,
                                                                     SAFEARRAY **state);

static void CVIFUNC DMonthViewEventsRegOnSelChange_EventVTableFunc (void *thisPtr,
                                                                    DATE startDate,
                                                                    DATE endDate,
                                                                    VBOOL *cancel);

static void CVIFUNC DMonthViewEventsRegOnClick_EventVTableFunc (void *thisPtr);

static void CVIFUNC DMonthViewEventsRegOnDblClick_EventVTableFunc (void *thisPtr);

static void CVIFUNC DMonthViewEventsRegOnKeyDown_EventVTableFunc (void *thisPtr,
                                                                  short *keyCode,
                                                                  short shift);

static void CVIFUNC DMonthViewEventsRegOnKeyUp_EventVTableFunc (void *thisPtr,
                                                                short *keyCode,
                                                                short shift);

static void CVIFUNC DMonthViewEventsRegOnKeyPress_EventVTableFunc (void *thisPtr,
                                                                   short *keyAscii);

static void CVIFUNC DMonthViewEventsRegOnMouseDown_EventVTableFunc (void *thisPtr,
                                                                    short button,
                                                                    short shift,
                                                                    long x,
                                                                    long y);

static void CVIFUNC DMonthViewEventsRegOnMouseMove_EventVTableFunc (void *thisPtr,
                                                                    short button,
                                                                    short shift,
                                                                    long x,
                                                                    long y);

static void CVIFUNC DMonthViewEventsRegOnMouseUp_EventVTableFunc (void *thisPtr,
                                                                  short button,
                                                                  short shift,
                                                                  long x, long y);

static void CVIFUNC DMonthViewEventsRegOnOLEStartDrag_EventVTableFunc (void *thisPtr,
                                                                       LPDISPATCH *data,
                                                                       long *allowedEffects);

static void CVIFUNC DMonthViewEventsRegOnOLEGiveFeedback_EventVTableFunc (void *thisPtr,
                                                                          long *effect,
                                                                          VBOOL *defaultCursors);

static void CVIFUNC DMonthViewEventsRegOnOLESetData_EventVTableFunc (void *thisPtr,
                                                                     LPDISPATCH *data,
                                                                     short *dataFormat);

static void CVIFUNC DMonthViewEventsRegOnOLECompleteDrag_EventVTableFunc (void *thisPtr,
                                                                          long *effect);

static void CVIFUNC DMonthViewEventsRegOnOLEDragOver_EventVTableFunc (void *thisPtr,
                                                                      LPDISPATCH *data,
                                                                      long *effect,
                                                                      short *button,
                                                                      short *shift,
                                                                      float *x,
                                                                      float *y,
                                                                      short *state);

static void CVIFUNC DMonthViewEventsRegOnOLEDragDrop_EventVTableFunc (void *thisPtr,
                                                                      LPDISPATCH *data,
                                                                      long *effect,
                                                                      short *button,
                                                                      short *shift,
                                                                      float *x,
                                                                      float *y);

static void CVIFUNC DDTPickerEventsRegOnCallbackKeyDown_EventVTableFunc (void *thisPtr,
                                                                         short keyCode,
                                                                         short shift,
                                                                         BSTR callbackField,
                                                                         DATE *callbackDate);

static void CVIFUNC DDTPickerEventsRegOnChange_EventVTableFunc (void *thisPtr);

static void CVIFUNC DDTPickerEventsRegOnCloseUp_EventVTableFunc (void *thisPtr);

static void CVIFUNC DDTPickerEventsRegOnDropDown_EventVTableFunc (void *thisPtr);

static void CVIFUNC DDTPickerEventsRegOnFormat_EventVTableFunc (void *thisPtr,
                                                                BSTR callbackField,
                                                                BSTR *formattedString);

static void CVIFUNC DDTPickerEventsRegOnFormatSize_EventVTableFunc (void *thisPtr,
                                                                    BSTR callbackField,
                                                                    short *size);

static void CVIFUNC DDTPickerEventsRegOnClick_EventVTableFunc (void *thisPtr);

static void CVIFUNC DDTPickerEventsRegOnDblClick_EventVTableFunc (void *thisPtr);

static void CVIFUNC DDTPickerEventsRegOnKeyDown_EventVTableFunc (void *thisPtr,
                                                                 short *keyCode,
                                                                 short shift);

static void CVIFUNC DDTPickerEventsRegOnKeyUp_EventVTableFunc (void *thisPtr,
                                                               short *keyCode,
                                                               short shift);

static void CVIFUNC DDTPickerEventsRegOnKeyPress_EventVTableFunc (void *thisPtr,
                                                                  short *keyAscii);

static void CVIFUNC DDTPickerEventsRegOnMouseDown_EventVTableFunc (void *thisPtr,
                                                                   short button,
                                                                   short shift,
                                                                   long x,
                                                                   long y);

static void CVIFUNC DDTPickerEventsRegOnMouseMove_EventVTableFunc (void *thisPtr,
                                                                   short button,
                                                                   short shift,
                                                                   long x,
                                                                   long y);

static void CVIFUNC DDTPickerEventsRegOnMouseUp_EventVTableFunc (void *thisPtr,
                                                                 short button,
                                                                 short shift,
                                                                 long x, long y);

static void CVIFUNC DDTPickerEventsRegOnOLEStartDrag_EventVTableFunc (void *thisPtr,
                                                                      LPDISPATCH *data,
                                                                      long *allowedEffects);

static void CVIFUNC DDTPickerEventsRegOnOLEGiveFeedback_EventVTableFunc (void *thisPtr,
                                                                         long *effect,
                                                                         VBOOL *defaultCursors);

static void CVIFUNC DDTPickerEventsRegOnOLESetData_EventVTableFunc (void *thisPtr,
                                                                    LPDISPATCH *data,
                                                                    short *dataFormat);

static void CVIFUNC DDTPickerEventsRegOnOLECompleteDrag_EventVTableFunc (void *thisPtr,
                                                                         long *effect);

static void CVIFUNC DDTPickerEventsRegOnOLEDragOver_EventVTableFunc (void *thisPtr,
                                                                     LPDISPATCH *data,
                                                                     long *effect,
                                                                     short *button,
                                                                     short *shift,
                                                                     float *x,
                                                                     float *y,
                                                                     short *state);

static void CVIFUNC DDTPickerEventsRegOnOLEDragDrop_EventVTableFunc (void *thisPtr,
                                                                     LPDISPATCH *data,
                                                                     long *effect,
                                                                     short *button,
                                                                     short *shift,
                                                                     float *x,
                                                                     float *y);

static void CVIFUNC DFlatSBEventsRegOnChange_EventVTableFunc (void *thisPtr);

static void CVIFUNC DFlatSBEventsRegOnScroll_EventVTableFunc (void *thisPtr);

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

typedef interface tagMSComCtl2_IVBDataObject_Interface MSComCtl2_IVBDataObject_Interface;

typedef struct tagMSComCtl2_IVBDataObject_VTable
{
	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( MSComCtl2_IVBDataObject_Interface __RPC_FAR * This, 
	                                                         REFIID riid, 
	                                                         void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( MSComCtl2_IVBDataObject_Interface __RPC_FAR * This);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( MSComCtl2_IVBDataObject_Interface __RPC_FAR * This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( MSComCtl2_IVBDataObject_Interface __RPC_FAR * This, 
	                                                           UINT __RPC_FAR *pctinfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( MSComCtl2_IVBDataObject_Interface __RPC_FAR * This, 
	                                                      UINT iTInfo, 
	                                                      LCID lcid, 
	                                                      ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( MSComCtl2_IVBDataObject_Interface __RPC_FAR * This, 
	                                                        REFIID riid, 
	                                                        LPOLESTR __RPC_FAR *rgszNames, 
	                                                        UINT cNames, 
	                                                        LCID lcid, 
	                                                        DISPID __RPC_FAR *rgDispId);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( MSComCtl2_IVBDataObject_Interface __RPC_FAR * This, 
	                                                 DISPID dispIdMember, 
	                                                 REFIID riid, 
	                                                 LCID lcid, 
	                                                 WORD wFlags, 
	                                                 DISPPARAMS __RPC_FAR *pDispParams, 
	                                                 VARIANT __RPC_FAR *pVarResult, 
	                                                 EXCEPINFO __RPC_FAR *pExcepInfo, 
	                                                 UINT __RPC_FAR *puArgErr);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clear_) (MSComCtl2_IVBDataObject_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetData_) (MSComCtl2_IVBDataObject_Interface __RPC_FAR *This, 
	                                                  short sFormat, 
	                                                  VARIANT *pvData);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFormat_) (MSComCtl2_IVBDataObject_Interface __RPC_FAR *This, 
	                                                    short sFormat, 
	                                                    VBOOL *pbFormatSupported);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetData_) (MSComCtl2_IVBDataObject_Interface __RPC_FAR *This, 
	                                                  VARIANT vValue, 
	                                                  VARIANT vFormat);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFiles_) (MSComCtl2_IVBDataObject_Interface __RPC_FAR *This, 
	                                                   LPDISPATCH *pFiles);

} MSComCtl2_IVBDataObject_VTable;

typedef interface tagMSComCtl2_IVBDataObject_Interface
{
	CONST_VTBL MSComCtl2_IVBDataObject_VTable __RPC_FAR *lpVtbl;
} MSComCtl2_IVBDataObject_Interface;

typedef interface tagMSComCtl2_IVBDataObjectFiles_Interface MSComCtl2_IVBDataObjectFiles_Interface;

typedef struct tagMSComCtl2_IVBDataObjectFiles_VTable
{
	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( MSComCtl2_IVBDataObjectFiles_Interface __RPC_FAR * This, 
	                                                         REFIID riid, 
	                                                         void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( MSComCtl2_IVBDataObjectFiles_Interface __RPC_FAR * This);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( MSComCtl2_IVBDataObjectFiles_Interface __RPC_FAR * This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( MSComCtl2_IVBDataObjectFiles_Interface __RPC_FAR * This, 
	                                                           UINT __RPC_FAR *pctinfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( MSComCtl2_IVBDataObjectFiles_Interface __RPC_FAR * This, 
	                                                      UINT iTInfo, 
	                                                      LCID lcid, 
	                                                      ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( MSComCtl2_IVBDataObjectFiles_Interface __RPC_FAR * This, 
	                                                        REFIID riid, 
	                                                        LPOLESTR __RPC_FAR *rgszNames, 
	                                                        UINT cNames, 
	                                                        LCID lcid, 
	                                                        DISPID __RPC_FAR *rgDispId);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( MSComCtl2_IVBDataObjectFiles_Interface __RPC_FAR * This, 
	                                                 DISPID dispIdMember, 
	                                                 REFIID riid, 
	                                                 LCID lcid, 
	                                                 WORD wFlags, 
	                                                 DISPPARAMS __RPC_FAR *pDispParams, 
	                                                 VARIANT __RPC_FAR *pVarResult, 
	                                                 EXCEPINFO __RPC_FAR *pExcepInfo, 
	                                                 UINT __RPC_FAR *puArgErr);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetItem_) (MSComCtl2_IVBDataObjectFiles_Interface __RPC_FAR *This, 
	                                                  long lIndex, 
	                                                  BSTR *bstrItem);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCount_) (MSComCtl2_IVBDataObjectFiles_Interface __RPC_FAR *This, 
	                                                   long *plCount);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add_) (MSComCtl2_IVBDataObjectFiles_Interface __RPC_FAR *This, 
	                                              BSTR bstrFilename, 
	                                              VARIANT vIndex);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clear_) (MSComCtl2_IVBDataObjectFiles_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Remove_) (MSComCtl2_IVBDataObjectFiles_Interface __RPC_FAR *This, 
	                                                 VARIANT vIndex);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *_NewEnum_) (MSComCtl2_IVBDataObjectFiles_Interface __RPC_FAR *This, 
	                                                   LPUNKNOWN *ppUnk);

} MSComCtl2_IVBDataObjectFiles_VTable;

typedef interface tagMSComCtl2_IVBDataObjectFiles_Interface
{
	CONST_VTBL MSComCtl2_IVBDataObjectFiles_VTable __RPC_FAR *lpVtbl;
} MSComCtl2_IVBDataObjectFiles_Interface;

typedef interface tagMSComCtl2_IAnimation_Interface MSComCtl2_IAnimation_Interface;

typedef struct tagMSComCtl2_IAnimation_VTable
{
	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( MSComCtl2_IAnimation_Interface __RPC_FAR * This, 
	                                                         REFIID riid, 
	                                                         void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( MSComCtl2_IAnimation_Interface __RPC_FAR * This);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( MSComCtl2_IAnimation_Interface __RPC_FAR * This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( MSComCtl2_IAnimation_Interface __RPC_FAR * This, 
	                                                           UINT __RPC_FAR *pctinfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( MSComCtl2_IAnimation_Interface __RPC_FAR * This, 
	                                                      UINT iTInfo, 
	                                                      LCID lcid, 
	                                                      ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( MSComCtl2_IAnimation_Interface __RPC_FAR * This, 
	                                                        REFIID riid, 
	                                                        LPOLESTR __RPC_FAR *rgszNames, 
	                                                        UINT cNames, 
	                                                        LCID lcid, 
	                                                        DISPID __RPC_FAR *rgDispId);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( MSComCtl2_IAnimation_Interface __RPC_FAR * This, 
	                                                 DISPID dispIdMember, 
	                                                 REFIID riid, 
	                                                 LCID lcid, 
	                                                 WORD wFlags, 
	                                                 DISPPARAMS __RPC_FAR *pDispParams, 
	                                                 VARIANT __RPC_FAR *pVarResult, 
	                                                 EXCEPINFO __RPC_FAR *pExcepInfo, 
	                                                 UINT __RPC_FAR *puArgErr);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAutoPlay_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This, 
	                                                      VBOOL pfAutoPlay);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAutoPlay_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This, 
	                                                      VBOOL *pfAutoPlay);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetBackStyle_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This, 
	                                                       long penumBackStyle);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBackStyle_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This, 
	                                                       long *penumBackStyle);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCenter_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This, 
	                                                    VBOOL pfCenter);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCenter_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This, 
	                                                    VBOOL *pfCenter);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEnabled_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This, 
	                                                     VBOOL pfEnable);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEnabled_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This, 
	                                                     VBOOL *pfEnable);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GethWnd_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This, 
	                                                  long *phWnd);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetBackColor_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This, 
	                                                       unsigned long pocBackColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBackColor_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This, 
	                                                       unsigned long *pocBackColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetOLEDropMode_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This, 
	                                                         long *psOLEDropMode);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOLEDropMode_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This, 
	                                                         long psOLEDropMode);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Close_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This, 
	                                               BSTR bstrFilename);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Play_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This, 
	                                               VARIANT varRepeatCount, 
	                                               VARIANT varStartFrame, 
	                                               VARIANT varEndFrame);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AboutBox_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OLEDrag_) (MSComCtl2_IAnimation_Interface __RPC_FAR *This);

} MSComCtl2_IAnimation_VTable;

typedef interface tagMSComCtl2_IAnimation_Interface
{
	CONST_VTBL MSComCtl2_IAnimation_VTable __RPC_FAR *lpVtbl;
} MSComCtl2_IAnimation_Interface;

typedef interface tagMSComCtl2_IUpDown_Interface MSComCtl2_IUpDown_Interface;

typedef struct tagMSComCtl2_IUpDown_VTable
{
	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( MSComCtl2_IUpDown_Interface __RPC_FAR * This, 
	                                                         REFIID riid, 
	                                                         void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( MSComCtl2_IUpDown_Interface __RPC_FAR * This);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( MSComCtl2_IUpDown_Interface __RPC_FAR * This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( MSComCtl2_IUpDown_Interface __RPC_FAR * This, 
	                                                           UINT __RPC_FAR *pctinfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( MSComCtl2_IUpDown_Interface __RPC_FAR * This, 
	                                                      UINT iTInfo, 
	                                                      LCID lcid, 
	                                                      ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( MSComCtl2_IUpDown_Interface __RPC_FAR * This, 
	                                                        REFIID riid, 
	                                                        LPOLESTR __RPC_FAR *rgszNames, 
	                                                        UINT cNames, 
	                                                        LCID lcid, 
	                                                        DISPID __RPC_FAR *rgDispId);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( MSComCtl2_IUpDown_Interface __RPC_FAR * This, 
	                                                 DISPID dispIdMember, 
	                                                 REFIID riid, 
	                                                 LCID lcid, 
	                                                 WORD wFlags, 
	                                                 DISPPARAMS __RPC_FAR *pDispParams, 
	                                                 VARIANT __RPC_FAR *pVarResult, 
	                                                 EXCEPINFO __RPC_FAR *pExcepInfo, 
	                                                 UINT __RPC_FAR *puArgErr);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetValue_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                   long plValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetValue_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                   long *plValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAcceleration_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                          short psAcceleration);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAcceleration_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                          short *psAcceleration);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAlignment_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                       long penumAlignment);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAlignment_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                       long *penumAlignment);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAutoBuddy_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                       VBOOL pfAutoBuddy);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAutoBuddy_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                       VBOOL *pfAutoBuddy);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetBuddyControl_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                          VARIANT pvarBuddyCtl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetByRefBuddyControl_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                               VARIANT pvarBuddyCtl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBuddyControl_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                          VARIANT *pvarBuddyCtl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetIncrement_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                       long plIncrement);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIncrement_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                       long *plIncrement);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMax_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                 long plMax);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMax_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                 long *plMax);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMin_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                 long plMin);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMin_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                 long *plMin);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOrientation_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                         long penumOrientation);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetOrientation_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                         long *penumOrientation);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSyncBuddy_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                       VBOOL pfSyncBuddy);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSyncBuddy_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                       VBOOL *pfSyncBuddy);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetWrap_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                  VBOOL pfWrap);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWrap_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                  VBOOL *pfWrap);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetBuddyProperty_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                           VARIANT pvarDispidBuddyProperty);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBuddyProperty_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                           VARIANT *pvarDispidBuddyProperty);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEnabled_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                     VBOOL pfEnable);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEnabled_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                     VBOOL *pfEnable);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GethWnd_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                  long *phWnd);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetOLEDropMode_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                         long *psOLEDropMode);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOLEDropMode_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This, 
	                                                         long psOLEDropMode);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AboutBox_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OLEDrag_) (MSComCtl2_IUpDown_Interface __RPC_FAR *This);

} MSComCtl2_IUpDown_VTable;

typedef interface tagMSComCtl2_IUpDown_Interface
{
	CONST_VTBL MSComCtl2_IUpDown_VTable __RPC_FAR *lpVtbl;
} MSComCtl2_IUpDown_Interface;

typedef interface tagMSComCtl2_IMonthView_Interface MSComCtl2_IMonthView_Interface;

typedef struct tagMSComCtl2_IMonthView_VTable
{
	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( MSComCtl2_IMonthView_Interface __RPC_FAR * This, 
	                                                         REFIID riid, 
	                                                         void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( MSComCtl2_IMonthView_Interface __RPC_FAR * This);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( MSComCtl2_IMonthView_Interface __RPC_FAR * This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( MSComCtl2_IMonthView_Interface __RPC_FAR * This, 
	                                                           UINT __RPC_FAR *pctinfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( MSComCtl2_IMonthView_Interface __RPC_FAR * This, 
	                                                      UINT iTInfo, 
	                                                      LCID lcid, 
	                                                      ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( MSComCtl2_IMonthView_Interface __RPC_FAR * This, 
	                                                        REFIID riid, 
	                                                        LPOLESTR __RPC_FAR *rgszNames, 
	                                                        UINT cNames, 
	                                                        LCID lcid, 
	                                                        DISPID __RPC_FAR *rgDispId);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( MSComCtl2_IMonthView_Interface __RPC_FAR * This, 
	                                                 DISPID dispIdMember, 
	                                                 REFIID riid, 
	                                                 LCID lcid, 
	                                                 WORD wFlags, 
	                                                 DISPPARAMS __RPC_FAR *pDispParams, 
	                                                 VARIANT __RPC_FAR *pVarResult, 
	                                                 EXCEPINFO __RPC_FAR *pExcepInfo, 
	                                                 UINT __RPC_FAR *puArgErr);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAppearance_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                        long *penumAppearances);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAppearance_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                        long penumAppearances);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBackColor_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                       unsigned long *pclrBackColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetBackColor_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                       unsigned long pclrBackColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBorderStyle_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                         long *psBorderStyle);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetBorderStyle_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                         long psBorderStyle);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDay_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                 short *psDay);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDay_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                 short psDay);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDayBold_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                     DATE dateIndex, 
	                                                     VBOOL *pbDayBold);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDayBold_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                     DATE dateIndex, 
	                                                     VBOOL pbDayBold);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDayOfWeek_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                       long *penumDayOfWeek);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDayOfWeek_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                       long penumDayOfWeek);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEnabled_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                     VBOOL *pbEnabled);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEnabled_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                     VBOOL pbEnabled);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFont_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                  LPDISPATCH *ppFont);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFont_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                  LPDISPATCH ppFont);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetByRefFont_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                       LPDISPATCH ppFont);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetForeColor_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                       unsigned long *pclrForeColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetForeColor_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                       unsigned long pclrForeColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GethWnd_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                  long *phWnd);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SethWnd_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                  long phWnd);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMaxDate_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                     DATE *pdateMaxDate);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMaxDate_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                     DATE pdateMaxDate);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMaxSelCount_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                         short *psMaxSelCount);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMaxSelCount_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                         short psMaxSelCount);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMinDate_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                     DATE *pdateMinDate);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMinDate_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                     DATE pdateMinDate);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMonth_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                   long *psMonth);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMonth_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                   long psMonth);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMonthBackColor_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                            unsigned long *pclrMonthBackColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMonthBackColor_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                            unsigned long pclrMonthBackColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMonthColumns_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                          short *psMonthColumns);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMonthColumns_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                          short psMonthColumns);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMonthRows_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                       short *psMonthRows);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMonthRows_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                       short psMonthRows);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMouseIcon_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                       LPDISPATCH *ppMouseIcon);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMouseIcon_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                       LPDISPATCH ppMouseIcon);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetByRefMouseIcon_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                            LPDISPATCH ppMouseIcon);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMousePointer_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                          long *psMousePointer);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMousePointer_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                          long psMousePointer);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMultiSelect_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                         VBOOL *pbMultiSelect);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMultiSelect_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                         VBOOL pbMultiSelect);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetOLEDropMode_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                         long *psOLEDropMode);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOLEDropMode_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                         long psOLEDropMode);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetScrollRate_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                        short *psScrollRate);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetScrollRate_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                        short psScrollRate);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSelEnd_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                    DATE *pdateSelEnd);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSelEnd_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                    DATE pdateSelEnd);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSelStart_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                      DATE *pdateSelStart);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSelStart_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                      DATE pdateSelStart);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetShowToday_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                       VBOOL *pbShowToday);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetShowToday_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                       VBOOL pbShowToday);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetShowWeekNumbers_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                             VBOOL *pbShowWeekNumbers);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetShowWeekNumbers_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                             VBOOL pbShowWeekNumbers);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStartOfWeek_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                         long *penumStartOfWeek);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetStartOfWeek_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                         long penumStartOfWeek);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTitleBackColor_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                            unsigned long *pclrTitleBackColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTitleBackColor_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                            unsigned long pclrTitleBackColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTitleForeColor_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                            unsigned long *pclrTitleForeColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTitleForeColor_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                            unsigned long pclrTitleForeColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTrailingForeColor_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                               unsigned long *pclrTrailingForeColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTrailingForeColor_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                               unsigned long pclrTrailingForeColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetValue_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                   DATE *pdateValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetValue_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                   DATE pdateValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Get_Value_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                    DATE *pdateValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Set_Value_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                    DATE pdateValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVisibleDays_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                         short sIndex, 
	                                                         DATE *pdateVisibleDays);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetVisibleDays_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                         short sIndex, 
	                                                         DATE pdateVisibleDays);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWeek_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                  short *psWeek);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetWeek_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                  short psWeek);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetYear_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                  short *psYear);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetYear_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                  short psYear);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AboutBox_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ComputeControlSize_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                             short rows, 
	                                                             short columns, 
	                                                             float *width, 
	                                                             float *height);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HitTest_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This, 
	                                                  long x, 
	                                                  long y, 
	                                                  DATE *date, 
	                                                  long *enumHitTestArea);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OLEDrag_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh_) (MSComCtl2_IMonthView_Interface __RPC_FAR *This);

} MSComCtl2_IMonthView_VTable;

typedef interface tagMSComCtl2_IMonthView_Interface
{
	CONST_VTBL MSComCtl2_IMonthView_VTable __RPC_FAR *lpVtbl;
} MSComCtl2_IMonthView_Interface;

typedef interface tagMSComCtl2_IDTPicker_Interface MSComCtl2_IDTPicker_Interface;

typedef struct tagMSComCtl2_IDTPicker_VTable
{
	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( MSComCtl2_IDTPicker_Interface __RPC_FAR * This, 
	                                                         REFIID riid, 
	                                                         void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( MSComCtl2_IDTPicker_Interface __RPC_FAR * This);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( MSComCtl2_IDTPicker_Interface __RPC_FAR * This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( MSComCtl2_IDTPicker_Interface __RPC_FAR * This, 
	                                                           UINT __RPC_FAR *pctinfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( MSComCtl2_IDTPicker_Interface __RPC_FAR * This, 
	                                                      UINT iTInfo, 
	                                                      LCID lcid, 
	                                                      ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( MSComCtl2_IDTPicker_Interface __RPC_FAR * This, 
	                                                        REFIID riid, 
	                                                        LPOLESTR __RPC_FAR *rgszNames, 
	                                                        UINT cNames, 
	                                                        LCID lcid, 
	                                                        DISPID __RPC_FAR *rgDispId);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( MSComCtl2_IDTPicker_Interface __RPC_FAR * This, 
	                                                 DISPID dispIdMember, 
	                                                 REFIID riid, 
	                                                 LCID lcid, 
	                                                 WORD wFlags, 
	                                                 DISPPARAMS __RPC_FAR *pDispParams, 
	                                                 VARIANT __RPC_FAR *pVarResult, 
	                                                 EXCEPINFO __RPC_FAR *pExcepInfo, 
	                                                 UINT __RPC_FAR *puArgErr);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFormat_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                    long *penumFormat);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFormat_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                    long penumFormat);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCalendarBackColor_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                               unsigned long *pclrCalendarBackColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCalendarBackColor_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                               unsigned long pclrCalendarBackColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCalendarForeColor_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                               unsigned long *pclrCalendarForeColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCalendarForeColor_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                               unsigned long pclrCalendarForeColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCalendarTitleBackColor_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                                    unsigned long *pclrTitleBackColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCalendarTitleBackColor_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                                    unsigned long pclrTitleBackColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCalendarTitleForeColor_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                                    unsigned long *pclrTitleForeColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCalendarTitleForeColor_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                                    unsigned long pclrTitleForeColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCalendarTrailingForeColor_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                                       unsigned long *pclrTrailingForeColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCalendarTrailingForeColor_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                                       unsigned long pclrTrailingForeColor);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCheckBox_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                      VBOOL *pbEnabled);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCheckBox_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                      VBOOL pbEnabled);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCustomFormat_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                          BSTR *pbstrCustomFormat);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCustomFormat_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                          BSTR pbstrCustomFormat);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDay_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                 VARIANT *pvDay);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDay_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                 VARIANT pvDay);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDayOfWeek_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                       VARIANT *pvDayOfWeek);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDayOfWeek_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                       VARIANT pvDayOfWeek);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEnabled_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                     VBOOL *pbEnabled);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEnabled_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                     VBOOL pbEnabled);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFont_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                  LPDISPATCH *ppFont);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFont_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                  LPDISPATCH ppFont);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetByRefFont_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                       LPDISPATCH ppFont);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetHour_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                  VARIANT *pvHour);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetHour_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                  VARIANT pvHour);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GethWnd_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                  long *phWnd);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SethWnd_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                  long phWnd);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMaxDate_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                     DATE *pdateMaxDate);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMaxDate_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                     DATE pdateMaxDate);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMinDate_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                     DATE *pdateMinDate);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMinDate_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                     DATE pdateMinDate);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMinute_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                    VARIANT *pvMinute);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMinute_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                    VARIANT pvMinute);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMonth_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                   VARIANT *pvMonth);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMonth_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                   VARIANT pvMonth);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMouseIcon_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                       LPDISPATCH *ppMouseIcon);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMouseIcon_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                       LPDISPATCH ppMouseIcon);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetByRefMouseIcon_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                            LPDISPATCH ppMouseIcon);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMousePointer_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                          long *psMousePointer);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMousePointer_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                          long psMousePointer);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetOLEDropMode_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                         long *psOLEDropMode);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOLEDropMode_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                         long psOLEDropMode);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSecond_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                    VARIANT *pvSecond);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSecond_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                    VARIANT pvSecond);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUpDown_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                    VBOOL *pbUpDown);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetUpDown_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                    VBOOL pbUpDown);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetValue_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                   VARIANT *pvdateValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetValue_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                   VARIANT pvdateValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Get_Value_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                    VARIANT *pvdateValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Set_Value_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                    VARIANT pvdateValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetYear_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                  VARIANT *pvYear);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetYear_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This, 
	                                                  VARIANT pvYear);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AboutBox_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OLEDrag_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh_) (MSComCtl2_IDTPicker_Interface __RPC_FAR *This);

} MSComCtl2_IDTPicker_VTable;

typedef interface tagMSComCtl2_IDTPicker_Interface
{
	CONST_VTBL MSComCtl2_IDTPicker_VTable __RPC_FAR *lpVtbl;
} MSComCtl2_IDTPicker_Interface;

typedef interface tagMSComCtl2_IFlatSB_Interface MSComCtl2_IFlatSB_Interface;

typedef struct tagMSComCtl2_IFlatSB_VTable
{
	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( MSComCtl2_IFlatSB_Interface __RPC_FAR * This, 
	                                                         REFIID riid, 
	                                                         void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( MSComCtl2_IFlatSB_Interface __RPC_FAR * This);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( MSComCtl2_IFlatSB_Interface __RPC_FAR * This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( MSComCtl2_IFlatSB_Interface __RPC_FAR * This, 
	                                                           UINT __RPC_FAR *pctinfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( MSComCtl2_IFlatSB_Interface __RPC_FAR * This, 
	                                                      UINT iTInfo, 
	                                                      LCID lcid, 
	                                                      ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( MSComCtl2_IFlatSB_Interface __RPC_FAR * This, 
	                                                        REFIID riid, 
	                                                        LPOLESTR __RPC_FAR *rgszNames, 
	                                                        UINT cNames, 
	                                                        LCID lcid, 
	                                                        DISPID __RPC_FAR *rgDispId);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( MSComCtl2_IFlatSB_Interface __RPC_FAR * This, 
	                                                 DISPID dispIdMember, 
	                                                 REFIID riid, 
	                                                 LCID lcid, 
	                                                 WORD wFlags, 
	                                                 DISPPARAMS __RPC_FAR *pDispParams, 
	                                                 VARIANT __RPC_FAR *pVarResult, 
	                                                 EXCEPINFO __RPC_FAR *pExcepInfo, 
	                                                 UINT __RPC_FAR *puArgErr);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAppearance_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                        long *penumAppearance);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAppearance_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                        long penumAppearance);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetArrows_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                    long *penumArrows);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetArrows_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                    long penumArrows);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEnabled_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                     VBOOL *pbEnabled);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEnabled_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                     VBOOL pbEnabled);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GethWnd_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                  long *phWnd);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SethWnd_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                  long phWnd);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLargeChange_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                         short *psLargeChange);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLargeChange_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                         short psLargeChange);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMin_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                 short *psMin);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMin_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                 short psMin);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMax_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                 short *psMax);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMax_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                 short psMax);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMouseIcon_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                       LPDISPATCH *ppMouseIcon);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMouseIcon_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                       LPDISPATCH ppMouseIcon);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetByRefMouseIcon_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                            LPDISPATCH ppMouseIcon);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMousePointer_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                          long *psMousePointer);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMousePointer_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                          long psMousePointer);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetOrientation_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                         long *penumOrientation);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOrientation_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                         long penumOrientation);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSmallChange_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                         short *psSmallChange);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSmallChange_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                         short psSmallChange);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetValue_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                   short *psValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetValue_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                   short psValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Get_Value_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                    short *psValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Set_Value_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This, 
	                                                    short psValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AboutBox_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh_) (MSComCtl2_IFlatSB_Interface __RPC_FAR *This);

} MSComCtl2_IFlatSB_VTable;

typedef interface tagMSComCtl2_IFlatSB_Interface
{
	CONST_VTBL MSComCtl2_IFlatSB_VTable __RPC_FAR *lpVtbl;
} MSComCtl2_IFlatSB_Interface;

static CA_PARAMDATA _DAnimationEvents_RegOnMouseDown_CA_PARAMDATA[] =
	{
		{"button", VT_I2},
        {"shift", VT_I2},
        {"x", VT_I4},
        {"y", VT_I4}
	};

static CA_PARAMDATA _DAnimationEvents_RegOnMouseMove_CA_PARAMDATA[] =
	{
		{"button", VT_I2},
        {"shift", VT_I2},
        {"x", VT_I4},
        {"y", VT_I4}
	};

static CA_PARAMDATA _DAnimationEvents_RegOnMouseUp_CA_PARAMDATA[] =
	{
		{"button", VT_I2},
        {"shift", VT_I2},
        {"x", VT_I4},
        {"y", VT_I4}
	};

static CA_PARAMDATA _DAnimationEvents_RegOnOLEStartDrag_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"allowedEffects", VT_I4 | VT_BYREF}
	};

static CA_PARAMDATA _DAnimationEvents_RegOnOLEGiveFeedback_CA_PARAMDATA[] =
	{
		{"effect", VT_I4 | VT_BYREF},
        {"defaultCursors", VT_BOOL | VT_BYREF}
	};

static CA_PARAMDATA _DAnimationEvents_RegOnOLESetData_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"dataFormat", VT_I2 | VT_BYREF}
	};

static CA_PARAMDATA _DAnimationEvents_RegOnOLECompleteDrag_CA_PARAMDATA[] =
	{
		{"effect", VT_I4 | VT_BYREF}
	};

static CA_PARAMDATA _DAnimationEvents_RegOnOLEDragOver_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"effect", VT_I4 | VT_BYREF},
        {"button", VT_I2 | VT_BYREF},
        {"shift", VT_I2 | VT_BYREF},
        {"x", VT_R4 | VT_BYREF},
        {"y", VT_R4 | VT_BYREF},
        {"state", VT_I2 | VT_BYREF}
	};

static CA_PARAMDATA _DAnimationEvents_RegOnOLEDragDrop_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"effect", VT_I4 | VT_BYREF},
        {"button", VT_I2 | VT_BYREF},
        {"shift", VT_I2 | VT_BYREF},
        {"x", VT_R4 | VT_BYREF},
        {"y", VT_R4 | VT_BYREF}
	};

static CA_METHODDATA _DAnimationEvents_CA_METHODDATA[] =
	{
		{"Click", NULL, -600, 0, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"DblClick", NULL, -601, 1, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"MouseDown", _DAnimationEvents_RegOnMouseDown_CA_PARAMDATA, -605, 2, CC_STDCALL, 4, DISPATCH_METHOD, VT_EMPTY},
        {"MouseMove", _DAnimationEvents_RegOnMouseMove_CA_PARAMDATA, -606, 3, CC_STDCALL, 4, DISPATCH_METHOD, VT_EMPTY},
        {"MouseUp", _DAnimationEvents_RegOnMouseUp_CA_PARAMDATA, -607, 4, CC_STDCALL, 4, DISPATCH_METHOD, VT_EMPTY},
        {"OLEStartDrag", _DAnimationEvents_RegOnOLEStartDrag_CA_PARAMDATA, 1550, 5, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"OLEGiveFeedback", _DAnimationEvents_RegOnOLEGiveFeedback_CA_PARAMDATA, 1551, 6, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"OLESetData", _DAnimationEvents_RegOnOLESetData_CA_PARAMDATA, 1552, 7, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"OLECompleteDrag", _DAnimationEvents_RegOnOLECompleteDrag_CA_PARAMDATA, 1553, 8, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"OLEDragOver", _DAnimationEvents_RegOnOLEDragOver_CA_PARAMDATA, 1554, 9, CC_STDCALL, 7, DISPATCH_METHOD, VT_EMPTY},
        {"OLEDragDrop", _DAnimationEvents_RegOnOLEDragDrop_CA_PARAMDATA, 1555, 10, CC_STDCALL, 6, DISPATCH_METHOD, VT_EMPTY}
	};

static CA_INTERFACEDATA _DAnimationEvents_CA_INTERFACEDATA =
	{
		_DAnimationEvents_CA_METHODDATA,
        sizeof (_DAnimationEvents_CA_METHODDATA) / sizeof (*_DAnimationEvents_CA_METHODDATA)
	};

static void * _DAnimationEvents_EventVTable[] =
	{
		DAnimationEventsRegOnClick_EventVTableFunc,
        DAnimationEventsRegOnDblClick_EventVTableFunc,
        DAnimationEventsRegOnMouseDown_EventVTableFunc,
        DAnimationEventsRegOnMouseMove_EventVTableFunc,
        DAnimationEventsRegOnMouseUp_EventVTableFunc,
        DAnimationEventsRegOnOLEStartDrag_EventVTableFunc,
        DAnimationEventsRegOnOLEGiveFeedback_EventVTableFunc,
        DAnimationEventsRegOnOLESetData_EventVTableFunc,
        DAnimationEventsRegOnOLECompleteDrag_EventVTableFunc,
        DAnimationEventsRegOnOLEDragOver_EventVTableFunc,
        DAnimationEventsRegOnOLEDragDrop_EventVTableFunc
	};

static CAEventClassDefn _DAnimationEvents_CAEventClassDefn =
	{
		20,
        &MSComCtl2_IID_DAnimationEvents,
        _DAnimationEvents_EventVTable,
        &_DAnimationEvents_CA_INTERFACEDATA,
        0
	};

static CA_PARAMDATA _DUpDownEvents_RegOnMouseDown_CA_PARAMDATA[] =
	{
		{"button", VT_I2},
        {"shift", VT_I2},
        {"x", VT_I4},
        {"y", VT_I4}
	};

static CA_PARAMDATA _DUpDownEvents_RegOnMouseMove_CA_PARAMDATA[] =
	{
		{"button", VT_I2},
        {"shift", VT_I2},
        {"x", VT_I4},
        {"y", VT_I4}
	};

static CA_PARAMDATA _DUpDownEvents_RegOnMouseUp_CA_PARAMDATA[] =
	{
		{"button", VT_I2},
        {"shift", VT_I2},
        {"x", VT_I4},
        {"y", VT_I4}
	};

static CA_PARAMDATA _DUpDownEvents_RegOnOLEStartDrag_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"allowedEffects", VT_I4 | VT_BYREF}
	};

static CA_PARAMDATA _DUpDownEvents_RegOnOLEGiveFeedback_CA_PARAMDATA[] =
	{
		{"effect", VT_I4 | VT_BYREF},
        {"defaultCursors", VT_BOOL | VT_BYREF}
	};

static CA_PARAMDATA _DUpDownEvents_RegOnOLESetData_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"dataFormat", VT_I2 | VT_BYREF}
	};

static CA_PARAMDATA _DUpDownEvents_RegOnOLECompleteDrag_CA_PARAMDATA[] =
	{
		{"effect", VT_I4 | VT_BYREF}
	};

static CA_PARAMDATA _DUpDownEvents_RegOnOLEDragOver_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"effect", VT_I4 | VT_BYREF},
        {"button", VT_I2 | VT_BYREF},
        {"shift", VT_I2 | VT_BYREF},
        {"x", VT_R4 | VT_BYREF},
        {"y", VT_R4 | VT_BYREF},
        {"state", VT_I2 | VT_BYREF}
	};

static CA_PARAMDATA _DUpDownEvents_RegOnOLEDragDrop_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"effect", VT_I4 | VT_BYREF},
        {"button", VT_I2 | VT_BYREF},
        {"shift", VT_I2 | VT_BYREF},
        {"x", VT_R4 | VT_BYREF},
        {"y", VT_R4 | VT_BYREF}
	};

static CA_METHODDATA _DUpDownEvents_CA_METHODDATA[] =
	{
		{"Change", NULL, 1, 0, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"DownClick", NULL, 2, 1, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"UpClick", NULL, 3, 2, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"MouseDown", _DUpDownEvents_RegOnMouseDown_CA_PARAMDATA, -605, 3, CC_STDCALL, 4, DISPATCH_METHOD, VT_EMPTY},
        {"MouseMove", _DUpDownEvents_RegOnMouseMove_CA_PARAMDATA, -606, 4, CC_STDCALL, 4, DISPATCH_METHOD, VT_EMPTY},
        {"MouseUp", _DUpDownEvents_RegOnMouseUp_CA_PARAMDATA, -607, 5, CC_STDCALL, 4, DISPATCH_METHOD, VT_EMPTY},
        {"OLEStartDrag", _DUpDownEvents_RegOnOLEStartDrag_CA_PARAMDATA, 1550, 6, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"OLEGiveFeedback", _DUpDownEvents_RegOnOLEGiveFeedback_CA_PARAMDATA, 1551, 7, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"OLESetData", _DUpDownEvents_RegOnOLESetData_CA_PARAMDATA, 1552, 8, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"OLECompleteDrag", _DUpDownEvents_RegOnOLECompleteDrag_CA_PARAMDATA, 1553, 9, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"OLEDragOver", _DUpDownEvents_RegOnOLEDragOver_CA_PARAMDATA, 1554, 10, CC_STDCALL, 7, DISPATCH_METHOD, VT_EMPTY},
        {"OLEDragDrop", _DUpDownEvents_RegOnOLEDragDrop_CA_PARAMDATA, 1555, 11, CC_STDCALL, 6, DISPATCH_METHOD, VT_EMPTY}
	};

static CA_INTERFACEDATA _DUpDownEvents_CA_INTERFACEDATA =
	{
		_DUpDownEvents_CA_METHODDATA,
        sizeof (_DUpDownEvents_CA_METHODDATA) / sizeof (*_DUpDownEvents_CA_METHODDATA)
	};

static void * _DUpDownEvents_EventVTable[] =
	{
		DUpDownEventsRegOnChange_EventVTableFunc,
        DUpDownEventsRegOnDownClick_EventVTableFunc,
        DUpDownEventsRegOnUpClick_EventVTableFunc,
        DUpDownEventsRegOnMouseDown_EventVTableFunc,
        DUpDownEventsRegOnMouseMove_EventVTableFunc,
        DUpDownEventsRegOnMouseUp_EventVTableFunc,
        DUpDownEventsRegOnOLEStartDrag_EventVTableFunc,
        DUpDownEventsRegOnOLEGiveFeedback_EventVTableFunc,
        DUpDownEventsRegOnOLESetData_EventVTableFunc,
        DUpDownEventsRegOnOLECompleteDrag_EventVTableFunc,
        DUpDownEventsRegOnOLEDragOver_EventVTableFunc,
        DUpDownEventsRegOnOLEDragDrop_EventVTableFunc
	};

static CAEventClassDefn _DUpDownEvents_CAEventClassDefn =
	{
		20,
        &MSComCtl2_IID_DUpDownEvents,
        _DUpDownEvents_EventVTable,
        &_DUpDownEvents_CA_INTERFACEDATA,
        0
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnDateClick_CA_PARAMDATA[] =
	{
		{"dateClicked", VT_DATE}
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnDateDblClick_CA_PARAMDATA[] =
	{
		{"dateDblClicked", VT_DATE}
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnGetDayBold_CA_PARAMDATA[] =
	{
		{"startDate", VT_DATE},
        {"count", VT_I2},
        {"state", VT_BOOL | VT_ARRAY | VT_BYREF}
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnSelChange_CA_PARAMDATA[] =
	{
		{"startDate", VT_DATE},
        {"endDate", VT_DATE},
        {"cancel", VT_BOOL | VT_BYREF}
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnKeyDown_CA_PARAMDATA[] =
	{
		{"keyCode", VT_I2 | VT_BYREF},
        {"shift", VT_I2}
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnKeyUp_CA_PARAMDATA[] =
	{
		{"keyCode", VT_I2 | VT_BYREF},
        {"shift", VT_I2}
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnKeyPress_CA_PARAMDATA[] =
	{
		{"keyAscii", VT_I2 | VT_BYREF}
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnMouseDown_CA_PARAMDATA[] =
	{
		{"button", VT_I2},
        {"shift", VT_I2},
        {"x", VT_I4},
        {"y", VT_I4}
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnMouseMove_CA_PARAMDATA[] =
	{
		{"button", VT_I2},
        {"shift", VT_I2},
        {"x", VT_I4},
        {"y", VT_I4}
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnMouseUp_CA_PARAMDATA[] =
	{
		{"button", VT_I2},
        {"shift", VT_I2},
        {"x", VT_I4},
        {"y", VT_I4}
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnOLEStartDrag_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"allowedEffects", VT_I4 | VT_BYREF}
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnOLEGiveFeedback_CA_PARAMDATA[] =
	{
		{"effect", VT_I4 | VT_BYREF},
        {"defaultCursors", VT_BOOL | VT_BYREF}
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnOLESetData_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"dataFormat", VT_I2 | VT_BYREF}
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnOLECompleteDrag_CA_PARAMDATA[] =
	{
		{"effect", VT_I4 | VT_BYREF}
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnOLEDragOver_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"effect", VT_I4 | VT_BYREF},
        {"button", VT_I2 | VT_BYREF},
        {"shift", VT_I2 | VT_BYREF},
        {"x", VT_R4 | VT_BYREF},
        {"y", VT_R4 | VT_BYREF},
        {"state", VT_I2 | VT_BYREF}
	};

static CA_PARAMDATA _DMonthViewEvents_RegOnOLEDragDrop_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"effect", VT_I4 | VT_BYREF},
        {"button", VT_I2 | VT_BYREF},
        {"shift", VT_I2 | VT_BYREF},
        {"x", VT_R4 | VT_BYREF},
        {"y", VT_R4 | VT_BYREF}
	};

static CA_METHODDATA _DMonthViewEvents_CA_METHODDATA[] =
	{
		{"DateClick", _DMonthViewEvents_RegOnDateClick_CA_PARAMDATA, 1, 0, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"DateDblClick", _DMonthViewEvents_RegOnDateDblClick_CA_PARAMDATA, 2, 1, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"GetDayBold", _DMonthViewEvents_RegOnGetDayBold_CA_PARAMDATA, 3, 2, CC_STDCALL, 3, DISPATCH_METHOD, VT_EMPTY},
        {"SelChange", _DMonthViewEvents_RegOnSelChange_CA_PARAMDATA, 4, 3, CC_STDCALL, 3, DISPATCH_METHOD, VT_EMPTY},
        {"Click", NULL, -600, 4, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"DblClick", NULL, -601, 5, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"KeyDown", _DMonthViewEvents_RegOnKeyDown_CA_PARAMDATA, -602, 6, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"KeyUp", _DMonthViewEvents_RegOnKeyUp_CA_PARAMDATA, -604, 7, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"KeyPress", _DMonthViewEvents_RegOnKeyPress_CA_PARAMDATA, -603, 8, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"MouseDown", _DMonthViewEvents_RegOnMouseDown_CA_PARAMDATA, -605, 9, CC_STDCALL, 4, DISPATCH_METHOD, VT_EMPTY},
        {"MouseMove", _DMonthViewEvents_RegOnMouseMove_CA_PARAMDATA, -606, 10, CC_STDCALL, 4, DISPATCH_METHOD, VT_EMPTY},
        {"MouseUp", _DMonthViewEvents_RegOnMouseUp_CA_PARAMDATA, -607, 11, CC_STDCALL, 4, DISPATCH_METHOD, VT_EMPTY},
        {"OLEStartDrag", _DMonthViewEvents_RegOnOLEStartDrag_CA_PARAMDATA, 1550, 12, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"OLEGiveFeedback", _DMonthViewEvents_RegOnOLEGiveFeedback_CA_PARAMDATA, 1551, 13, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"OLESetData", _DMonthViewEvents_RegOnOLESetData_CA_PARAMDATA, 1552, 14, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"OLECompleteDrag", _DMonthViewEvents_RegOnOLECompleteDrag_CA_PARAMDATA, 1553, 15, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"OLEDragOver", _DMonthViewEvents_RegOnOLEDragOver_CA_PARAMDATA, 1554, 16, CC_STDCALL, 7, DISPATCH_METHOD, VT_EMPTY},
        {"OLEDragDrop", _DMonthViewEvents_RegOnOLEDragDrop_CA_PARAMDATA, 1555, 17, CC_STDCALL, 6, DISPATCH_METHOD, VT_EMPTY}
	};

static CA_INTERFACEDATA _DMonthViewEvents_CA_INTERFACEDATA =
	{
		_DMonthViewEvents_CA_METHODDATA,
        sizeof (_DMonthViewEvents_CA_METHODDATA) / sizeof (*_DMonthViewEvents_CA_METHODDATA)
	};

static void * _DMonthViewEvents_EventVTable[] =
	{
		DMonthViewEventsRegOnDateClick_EventVTableFunc,
        DMonthViewEventsRegOnDateDblClick_EventVTableFunc,
        DMonthViewEventsRegOnGetDayBold_EventVTableFunc,
        DMonthViewEventsRegOnSelChange_EventVTableFunc,
        DMonthViewEventsRegOnClick_EventVTableFunc,
        DMonthViewEventsRegOnDblClick_EventVTableFunc,
        DMonthViewEventsRegOnKeyDown_EventVTableFunc,
        DMonthViewEventsRegOnKeyUp_EventVTableFunc,
        DMonthViewEventsRegOnKeyPress_EventVTableFunc,
        DMonthViewEventsRegOnMouseDown_EventVTableFunc,
        DMonthViewEventsRegOnMouseMove_EventVTableFunc,
        DMonthViewEventsRegOnMouseUp_EventVTableFunc,
        DMonthViewEventsRegOnOLEStartDrag_EventVTableFunc,
        DMonthViewEventsRegOnOLEGiveFeedback_EventVTableFunc,
        DMonthViewEventsRegOnOLESetData_EventVTableFunc,
        DMonthViewEventsRegOnOLECompleteDrag_EventVTableFunc,
        DMonthViewEventsRegOnOLEDragOver_EventVTableFunc,
        DMonthViewEventsRegOnOLEDragDrop_EventVTableFunc
	};

static CAEventClassDefn _DMonthViewEvents_CAEventClassDefn =
	{
		20,
        &MSComCtl2_IID_DMonthViewEvents,
        _DMonthViewEvents_EventVTable,
        &_DMonthViewEvents_CA_INTERFACEDATA,
        0
	};

static CA_PARAMDATA _DDTPickerEvents_RegOnCallbackKeyDown_CA_PARAMDATA[] =
	{
		{"keyCode", VT_I2},
        {"shift", VT_I2},
        {"callbackField", VT_BSTR},
        {"callbackDate", VT_DATE | VT_BYREF}
	};

static CA_PARAMDATA _DDTPickerEvents_RegOnFormat_CA_PARAMDATA[] =
	{
		{"callbackField", VT_BSTR},
        {"formattedString", VT_BSTR | VT_BYREF}
	};

static CA_PARAMDATA _DDTPickerEvents_RegOnFormatSize_CA_PARAMDATA[] =
	{
		{"callbackField", VT_BSTR},
        {"size", VT_I2 | VT_BYREF}
	};

static CA_PARAMDATA _DDTPickerEvents_RegOnKeyDown_CA_PARAMDATA[] =
	{
		{"keyCode", VT_I2 | VT_BYREF},
        {"shift", VT_I2}
	};

static CA_PARAMDATA _DDTPickerEvents_RegOnKeyUp_CA_PARAMDATA[] =
	{
		{"keyCode", VT_I2 | VT_BYREF},
        {"shift", VT_I2}
	};

static CA_PARAMDATA _DDTPickerEvents_RegOnKeyPress_CA_PARAMDATA[] =
	{
		{"keyAscii", VT_I2 | VT_BYREF}
	};

static CA_PARAMDATA _DDTPickerEvents_RegOnMouseDown_CA_PARAMDATA[] =
	{
		{"button", VT_I2},
        {"shift", VT_I2},
        {"x", VT_I4},
        {"y", VT_I4}
	};

static CA_PARAMDATA _DDTPickerEvents_RegOnMouseMove_CA_PARAMDATA[] =
	{
		{"button", VT_I2},
        {"shift", VT_I2},
        {"x", VT_I4},
        {"y", VT_I4}
	};

static CA_PARAMDATA _DDTPickerEvents_RegOnMouseUp_CA_PARAMDATA[] =
	{
		{"button", VT_I2},
        {"shift", VT_I2},
        {"x", VT_I4},
        {"y", VT_I4}
	};

static CA_PARAMDATA _DDTPickerEvents_RegOnOLEStartDrag_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"allowedEffects", VT_I4 | VT_BYREF}
	};

static CA_PARAMDATA _DDTPickerEvents_RegOnOLEGiveFeedback_CA_PARAMDATA[] =
	{
		{"effect", VT_I4 | VT_BYREF},
        {"defaultCursors", VT_BOOL | VT_BYREF}
	};

static CA_PARAMDATA _DDTPickerEvents_RegOnOLESetData_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"dataFormat", VT_I2 | VT_BYREF}
	};

static CA_PARAMDATA _DDTPickerEvents_RegOnOLECompleteDrag_CA_PARAMDATA[] =
	{
		{"effect", VT_I4 | VT_BYREF}
	};

static CA_PARAMDATA _DDTPickerEvents_RegOnOLEDragOver_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"effect", VT_I4 | VT_BYREF},
        {"button", VT_I2 | VT_BYREF},
        {"shift", VT_I2 | VT_BYREF},
        {"x", VT_R4 | VT_BYREF},
        {"y", VT_R4 | VT_BYREF},
        {"state", VT_I2 | VT_BYREF}
	};

static CA_PARAMDATA _DDTPickerEvents_RegOnOLEDragDrop_CA_PARAMDATA[] =
	{
		{"data", VT_DISPATCH | VT_BYREF},
        {"effect", VT_I4 | VT_BYREF},
        {"button", VT_I2 | VT_BYREF},
        {"shift", VT_I2 | VT_BYREF},
        {"x", VT_R4 | VT_BYREF},
        {"y", VT_R4 | VT_BYREF}
	};

static CA_METHODDATA _DDTPickerEvents_CA_METHODDATA[] =
	{
		{"CallbackKeyDown", _DDTPickerEvents_RegOnCallbackKeyDown_CA_PARAMDATA, 1, 0, CC_STDCALL, 4, DISPATCH_METHOD, VT_EMPTY},
        {"Change", NULL, 2, 1, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"CloseUp", NULL, 3, 2, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"DropDown", NULL, 4, 3, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"Format", _DDTPickerEvents_RegOnFormat_CA_PARAMDATA, 5, 4, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"FormatSize", _DDTPickerEvents_RegOnFormatSize_CA_PARAMDATA, 6, 5, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"Click", NULL, -600, 6, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"DblClick", NULL, -601, 7, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"KeyDown", _DDTPickerEvents_RegOnKeyDown_CA_PARAMDATA, -602, 8, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"KeyUp", _DDTPickerEvents_RegOnKeyUp_CA_PARAMDATA, -604, 9, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"KeyPress", _DDTPickerEvents_RegOnKeyPress_CA_PARAMDATA, -603, 10, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"MouseDown", _DDTPickerEvents_RegOnMouseDown_CA_PARAMDATA, -605, 11, CC_STDCALL, 4, DISPATCH_METHOD, VT_EMPTY},
        {"MouseMove", _DDTPickerEvents_RegOnMouseMove_CA_PARAMDATA, -606, 12, CC_STDCALL, 4, DISPATCH_METHOD, VT_EMPTY},
        {"MouseUp", _DDTPickerEvents_RegOnMouseUp_CA_PARAMDATA, -607, 13, CC_STDCALL, 4, DISPATCH_METHOD, VT_EMPTY},
        {"OLEStartDrag", _DDTPickerEvents_RegOnOLEStartDrag_CA_PARAMDATA, 1550, 14, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"OLEGiveFeedback", _DDTPickerEvents_RegOnOLEGiveFeedback_CA_PARAMDATA, 1551, 15, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"OLESetData", _DDTPickerEvents_RegOnOLESetData_CA_PARAMDATA, 1552, 16, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"OLECompleteDrag", _DDTPickerEvents_RegOnOLECompleteDrag_CA_PARAMDATA, 1553, 17, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"OLEDragOver", _DDTPickerEvents_RegOnOLEDragOver_CA_PARAMDATA, 1554, 18, CC_STDCALL, 7, DISPATCH_METHOD, VT_EMPTY},
        {"OLEDragDrop", _DDTPickerEvents_RegOnOLEDragDrop_CA_PARAMDATA, 1555, 19, CC_STDCALL, 6, DISPATCH_METHOD, VT_EMPTY}
	};

static CA_INTERFACEDATA _DDTPickerEvents_CA_INTERFACEDATA =
	{
		_DDTPickerEvents_CA_METHODDATA,
        sizeof (_DDTPickerEvents_CA_METHODDATA) / sizeof (*_DDTPickerEvents_CA_METHODDATA)
	};

static void * _DDTPickerEvents_EventVTable[] =
	{
		DDTPickerEventsRegOnCallbackKeyDown_EventVTableFunc,
        DDTPickerEventsRegOnChange_EventVTableFunc,
        DDTPickerEventsRegOnCloseUp_EventVTableFunc,
        DDTPickerEventsRegOnDropDown_EventVTableFunc,
        DDTPickerEventsRegOnFormat_EventVTableFunc,
        DDTPickerEventsRegOnFormatSize_EventVTableFunc,
        DDTPickerEventsRegOnClick_EventVTableFunc,
        DDTPickerEventsRegOnDblClick_EventVTableFunc,
        DDTPickerEventsRegOnKeyDown_EventVTableFunc,
        DDTPickerEventsRegOnKeyUp_EventVTableFunc,
        DDTPickerEventsRegOnKeyPress_EventVTableFunc,
        DDTPickerEventsRegOnMouseDown_EventVTableFunc,
        DDTPickerEventsRegOnMouseMove_EventVTableFunc,
        DDTPickerEventsRegOnMouseUp_EventVTableFunc,
        DDTPickerEventsRegOnOLEStartDrag_EventVTableFunc,
        DDTPickerEventsRegOnOLEGiveFeedback_EventVTableFunc,
        DDTPickerEventsRegOnOLESetData_EventVTableFunc,
        DDTPickerEventsRegOnOLECompleteDrag_EventVTableFunc,
        DDTPickerEventsRegOnOLEDragOver_EventVTableFunc,
        DDTPickerEventsRegOnOLEDragDrop_EventVTableFunc
	};

static CAEventClassDefn _DDTPickerEvents_CAEventClassDefn =
	{
		20,
        &MSComCtl2_IID_DDTPickerEvents,
        _DDTPickerEvents_EventVTable,
        &_DDTPickerEvents_CA_INTERFACEDATA,
        0
	};

static CA_METHODDATA _DFlatSBEvents_CA_METHODDATA[] =
	{
		{"Change", NULL, 1, 0, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"Scroll", NULL, 2, 1, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY}
	};

static CA_INTERFACEDATA _DFlatSBEvents_CA_INTERFACEDATA =
	{
		_DFlatSBEvents_CA_METHODDATA,
        sizeof (_DFlatSBEvents_CA_METHODDATA) / sizeof (*_DFlatSBEvents_CA_METHODDATA)
	};

static void * _DFlatSBEvents_EventVTable[] =
	{
		DFlatSBEventsRegOnChange_EventVTableFunc,
        DFlatSBEventsRegOnScroll_EventVTableFunc
	};

static CAEventClassDefn _DFlatSBEvents_CAEventClassDefn =
	{
		20,
        &MSComCtl2_IID_DFlatSBEvents,
        _DFlatSBEvents_EventVTable,
        &_DFlatSBEvents_CA_INTERFACEDATA,
        0
	};

const IID MSComCtl2_IID_IAnimation =
	{
		0xB09DE713, 0x87C1, 0x11D1, 0x8B, 0xE3, 0x0, 0x0, 0xF8, 0x75, 0x4D, 0xA1
	};

const IID MSComCtl2_IID_IUpDown =
	{
		0x603C7E7E, 0x87C2, 0x11D1, 0x8B, 0xE3, 0x0, 0x0, 0xF8, 0x75, 0x4D, 0xA1
	};

const IID MSComCtl2_IID_IMonthView =
	{
		0x232E4565, 0x87C3, 0x11D1, 0x8B, 0xE3, 0x0, 0x0, 0xF8, 0x75, 0x4D, 0xA1
	};

const IID MSComCtl2_IID_IDTPicker =
	{
		0x20DD1B9B, 0x87C4, 0x11D1, 0x8B, 0xE3, 0x0, 0x0, 0xF8, 0x75, 0x4D, 0xA1
	};

const IID MSComCtl2_IID_IFlatSB =
	{
		0xFE387538, 0x44A3, 0x11D1, 0xB5, 0xB7, 0x0, 0x0, 0xC0, 0x90, 0x0, 0xC4
	};

const IID MSComCtl2_IID_DAnimationEvents =
	{
		0xB09DE714, 0x87C1, 0x11D1, 0x8B, 0xE3, 0x0, 0x0, 0xF8, 0x75, 0x4D, 0xA1
	};

const IID MSComCtl2_IID_DUpDownEvents =
	{
		0x603C7E7F, 0x87C2, 0x11D1, 0x8B, 0xE3, 0x0, 0x0, 0xF8, 0x75, 0x4D, 0xA1
	};

const IID MSComCtl2_IID_DMonthViewEvents =
	{
		0x232E4569, 0x87C3, 0x11D1, 0x8B, 0xE3, 0x0, 0x0, 0xF8, 0x75, 0x4D, 0xA1
	};

const IID MSComCtl2_IID_DDTPickerEvents =
	{
		0x20DD1B9D, 0x87C4, 0x11D1, 0x8B, 0xE3, 0x0, 0x0, 0xF8, 0x75, 0x4D, 0xA1
	};

const IID MSComCtl2_IID_DFlatSBEvents =
	{
		0xFE387539, 0x44A3, 0x11D1, 0xB5, 0xB7, 0x0, 0x0, 0xC0, 0x90, 0x0, 0xC4
	};

const IID MSComCtl2_IID_IVBDataObject =
	{
		0x2334D2B1, 0x713E, 0x11CF, 0x8A, 0xE5, 0x0, 0xAA, 0x0, 0xC0, 0x9, 0x5
	};

const IID MSComCtl2_IID_IVBDataObjectFiles =
	{
		0x2334D2B3, 0x713E, 0x11CF, 0x8A, 0xE5, 0x0, 0xAA, 0x0, 0xC0, 0x9, 0x5
	};

const IID MSComCtl2_IID_Font =
	{
		0xBEF6E003, 0xA874, 0x101A, 0x8B, 0xBA, 0x0, 0xAA, 0x0, 0x30, 0xC, 0xAB
	};

const IID MSComCtl2_IID_Picture =
	{
		0x7BF80981, 0xBF32, 0x101A, 0x8B, 0xBB, 0x0, 0xAA, 0x0, 0x30, 0xC, 0xAB
	};

HRESULT CVIFUNC MSComCtl2_IVBDataObjectClear (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IVBDataObject_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IVBDataObject, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Clear_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IVBDataObject, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IVBDataObjectGetData (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                short sFormat, VARIANT *pvData)
{
	HRESULT __result = S_OK;
	MSComCtl2_IVBDataObject_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VARIANT pvData__Temp;

	if (pvData)
		CA_VariantSetEmpty (pvData);
	CA_VariantSetEmpty (&pvData__Temp);

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IVBDataObject, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetData_ (__vtblIFacePtr, sFormat,
	                                              &pvData__Temp));

	if (pvData)
		{
		*pvData = pvData__Temp;
		CA_VariantSetEmpty (&pvData__Temp);
		}

Error:
	CA_VariantClear (&pvData__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvData)
			CA_VariantClear (pvData);
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IVBDataObject, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IVBDataObjectGetFormat (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  short sFormat,
                                                  VBOOL *pbFormatSupported)
{
	HRESULT __result = S_OK;
	MSComCtl2_IVBDataObject_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbFormatSupported__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IVBDataObject, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetFormat_ (__vtblIFacePtr, sFormat,
	                                                &pbFormatSupported__Temp));

	if (pbFormatSupported)
		{
		*pbFormatSupported = pbFormatSupported__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IVBDataObject, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IVBDataObjectSetData (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VARIANT vValue, VARIANT vFormat)
{
	HRESULT __result = S_OK;
	MSComCtl2_IVBDataObject_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IVBDataObject, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetData_ (__vtblIFacePtr, vValue,
	                                              vFormat));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IVBDataObject, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IVBDataObjectGetFiles (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSComCtl2Obj_IVBDataObjectFiles *pFiles)
{
	HRESULT __result = S_OK;
	MSComCtl2_IVBDataObject_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH pFiles__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (pFiles)
		*pFiles = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IVBDataObject, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetFiles_ (__vtblIFacePtr,
	                                               &pFiles__AutoType));
	

	if (pFiles)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (pFiles__AutoType,
		                                             &MSComCtl2_IID_IVBDataObjectFiles,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, pFiles));
		pFiles__AutoType = 0;
		}

Error:
	if (pFiles__AutoType)
		pFiles__AutoType->lpVtbl->Release (pFiles__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pFiles)
			{
			CA_DiscardObjHandle (*pFiles);
			*pFiles = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IVBDataObject, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IVBDataObjectFilesGetItem (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     long lIndex,
                                                     char **bstrItem)
{
	HRESULT __result = S_OK;
	MSComCtl2_IVBDataObjectFiles_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR bstrItem__AutoType = 0;

	if (bstrItem)
		*bstrItem = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IVBDataObjectFiles,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetItem_ (__vtblIFacePtr, lIndex,
	                                              &bstrItem__AutoType));

	if (bstrItem)
		__caErrChk (CA_BSTRGetCString (bstrItem__AutoType, bstrItem));

Error:
	CA_FreeBSTR (bstrItem__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (bstrItem)
			{
			CA_FreeMemory (*bstrItem);
			*bstrItem = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IVBDataObjectFiles,
	                    __result, errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IVBDataObjectFilesGetCount (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      long *plCount)
{
	HRESULT __result = S_OK;
	MSComCtl2_IVBDataObjectFiles_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long plCount__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IVBDataObjectFiles,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetCount_ (__vtblIFacePtr,
	                                               &plCount__Temp));

	if (plCount)
		{
		*plCount = plCount__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IVBDataObjectFiles,
	                    __result, errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IVBDataObjectFilesAdd (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *bstrFilename,
                                                 VARIANT vIndex)
{
	HRESULT __result = S_OK;
	MSComCtl2_IVBDataObjectFiles_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR bstrFilename__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (bstrFilename, &bstrFilename__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IVBDataObjectFiles,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Add_ (__vtblIFacePtr,
	                                          bstrFilename__AutoType, vIndex));

Error:
	CA_FreeBSTR (bstrFilename__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IVBDataObjectFiles,
	                    __result, errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IVBDataObjectFilesClear (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IVBDataObjectFiles_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IVBDataObjectFiles,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Clear_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IVBDataObjectFiles,
	                    __result, errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IVBDataObjectFilesRemove (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VARIANT vIndex)
{
	HRESULT __result = S_OK;
	MSComCtl2_IVBDataObjectFiles_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IVBDataObjectFiles,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Remove_ (__vtblIFacePtr, vIndex));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IVBDataObjectFiles,
	                    __result, errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IVBDataObjectFiles_NewEnum (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      LPUNKNOWN *ppUnk)
{
	HRESULT __result = S_OK;
	MSComCtl2_IVBDataObjectFiles_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPUNKNOWN ppUnk__Temp = 0;

	if (ppUnk)
		*ppUnk = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IVBDataObjectFiles,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->_NewEnum_ (__vtblIFacePtr,
	                                               &ppUnk__Temp));

	if (ppUnk)
		{
		*ppUnk = ppUnk__Temp;
		ppUnk__Temp = 0;
		}

Error:
	if (ppUnk__Temp)
		ppUnk__Temp->lpVtbl->Release (ppUnk__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppUnk)
			{
			if (*ppUnk)
				(*ppUnk)->lpVtbl->Release (*ppUnk);
			*ppUnk = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IVBDataObjectFiles,
	                    __result, errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_NewIAnimation (int panel, const char *label, int top,
                                         int left, int *controlID, int *UILError)
{
	HRESULT __result = S_OK;
	int ctrlId;
	GUID clsid = {0xB09DE715, 0x87C1, 0x11D1, 0x8B, 0xE3, 0x0, 0x0, 0xF8, 0x75,
	              0x4D, 0xA1};
	const char * licStr = "651A8940-87C5-11d1-8BE3-0000F8754DA1";

	ctrlId = NewActiveXCtrl (panel, label, top, left, &clsid,
	                         &MSComCtl2_IID_IAnimation, licStr, &__result);

	__ActiveXCtrlErrorHandler();

	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationSetAutoPlay (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VBOOL pfAutoPlay)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetAutoPlay_ (__vtblIFacePtr,
	                                                  pfAutoPlay));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationGetAutoPlay (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VBOOL *pfAutoPlay)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pfAutoPlay__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetAutoPlay_ (__vtblIFacePtr,
	                                                  &pfAutoPlay__Temp));

	if (pfAutoPlay)
		{
		*pfAutoPlay = pfAutoPlay__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationSetBackStyle (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  enum MSComCtl2Enum_BackStyleConstants penumBackStyle)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetBackStyle_ (__vtblIFacePtr,
	                                                   penumBackStyle));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationGetBackStyle (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  enum MSComCtl2Enum_BackStyleConstants *penumBackStyle)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long penumBackStyle__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetBackStyle_ (__vtblIFacePtr,
	                                                   (long *)&penumBackStyle__Temp));

	if (penumBackStyle)
		{
		*penumBackStyle = penumBackStyle__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationSetCenter (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL pfCenter)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetCenter_ (__vtblIFacePtr, pfCenter));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationGetCenter (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *pfCenter)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pfCenter__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetCenter_ (__vtblIFacePtr,
	                                                &pfCenter__Temp));

	if (pfCenter)
		{
		*pfCenter = pfCenter__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationSetEnabled (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL pfEnable)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetEnabled_ (__vtblIFacePtr, pfEnable));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationGetEnabled (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL *pfEnable)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pfEnable__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetEnabled_ (__vtblIFacePtr,
	                                                 &pfEnable__Temp));

	if (pfEnable)
		{
		*pfEnable = pfEnable__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationGethWnd (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             MSComCtl2Type_OLE_HANDLE *phWnd)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long phWnd__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GethWnd_ (__vtblIFacePtr, &phWnd__Temp));

	if (phWnd)
		{
		*phWnd = phWnd__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationSetBackColor (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Type_OLE_COLOR pocBackColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetBackColor_ (__vtblIFacePtr,
	                                                   pocBackColor));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationGetBackColor (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Type_OLE_COLOR *pocBackColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	unsigned long pocBackColor__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetBackColor_ (__vtblIFacePtr,
	                                                   &pocBackColor__Temp));

	if (pocBackColor)
		{
		*pocBackColor = pocBackColor__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationGetOLEDropMode (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_OLEDropConstants *psOLEDropMode)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long psOLEDropMode__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetOLEDropMode_ (__vtblIFacePtr,
	                                                     (long *)&psOLEDropMode__Temp));

	if (psOLEDropMode)
		{
		*psOLEDropMode = psOLEDropMode__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationSetOLEDropMode (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_OLEDropConstants psOLEDropMode)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetOLEDropMode_ (__vtblIFacePtr,
	                                                     psOLEDropMode));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationClose (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Close_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationOpen (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          const char *bstrFilename)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR bstrFilename__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (bstrFilename, &bstrFilename__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Open_ (__vtblIFacePtr,
	                                           bstrFilename__AutoType));

Error:
	CA_FreeBSTR (bstrFilename__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationPlay (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          VARIANT varRepeatCount,
                                          VARIANT varStartFrame,
                                          VARIANT varEndFrame)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Play_ (__vtblIFacePtr, varRepeatCount,
	                                           varStartFrame, varEndFrame));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationStop (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Stop_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationAboutBox (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->AboutBox_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IAnimationOLEDrag (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IAnimation_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IAnimation, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->OLEDrag_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IAnimation, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_NewIUpDown (int panel, const char *label, int top,
                                      int left, int *controlID, int *UILError)
{
	HRESULT __result = S_OK;
	int ctrlId;
	GUID clsid = {0x603C7E80, 0x87C2, 0x11D1, 0x8B, 0xE3, 0x0, 0x0, 0xF8, 0x75,
	              0x4D, 0xA1};
	const char * licStr = "651A8940-87C5-11d1-8BE3-0000F8754DA1";

	ctrlId = NewActiveXCtrl (panel, label, top, left, &clsid,
	                         &MSComCtl2_IID_IUpDown, licStr, &__result);

	__ActiveXCtrlErrorHandler();

	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownSetValue (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, long plValue)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetValue_ (__vtblIFacePtr, plValue));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownGetValue (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, long *plValue)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long plValue__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetValue_ (__vtblIFacePtr,
	                                               &plValue__Temp));

	if (plValue)
		{
		*plValue = plValue__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownSetAcceleration (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  short psAcceleration)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetAcceleration_ (__vtblIFacePtr,
	                                                      psAcceleration));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownGetAcceleration (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  short *psAcceleration)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	short psAcceleration__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetAcceleration_ (__vtblIFacePtr,
	                                                      &psAcceleration__Temp));

	if (psAcceleration)
		{
		*psAcceleration = psAcceleration__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownSetAlignment (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               enum MSComCtl2Enum_AlignmentConstants penumAlignment)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetAlignment_ (__vtblIFacePtr,
	                                                   penumAlignment));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownGetAlignment (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               enum MSComCtl2Enum_AlignmentConstants *penumAlignment)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long penumAlignment__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetAlignment_ (__vtblIFacePtr,
	                                                   (long *)&penumAlignment__Temp));

	if (penumAlignment)
		{
		*penumAlignment = penumAlignment__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownSetAutoBuddy (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL pfAutoBuddy)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetAutoBuddy_ (__vtblIFacePtr,
	                                                   pfAutoBuddy));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownGetAutoBuddy (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *pfAutoBuddy)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pfAutoBuddy__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetAutoBuddy_ (__vtblIFacePtr,
	                                                   &pfAutoBuddy__Temp));

	if (pfAutoBuddy)
		{
		*pfAutoBuddy = pfAutoBuddy__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownSetBuddyControl (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VARIANT pvarBuddyCtl)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetBuddyControl_ (__vtblIFacePtr,
	                                                      pvarBuddyCtl));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownSetByRefBuddyControl (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VARIANT pvarBuddyCtl)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetByRefBuddyControl_ (__vtblIFacePtr,
	                                                           pvarBuddyCtl));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownGetBuddyControl (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VARIANT *pvarBuddyCtl)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VARIANT pvarBuddyCtl__Temp;

	if (pvarBuddyCtl)
		CA_VariantSetEmpty (pvarBuddyCtl);
	CA_VariantSetEmpty (&pvarBuddyCtl__Temp);

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetBuddyControl_ (__vtblIFacePtr,
	                                                      &pvarBuddyCtl__Temp));

	if (pvarBuddyCtl)
		{
		*pvarBuddyCtl = pvarBuddyCtl__Temp;
		CA_VariantSetEmpty (&pvarBuddyCtl__Temp);
		}

Error:
	CA_VariantClear (&pvarBuddyCtl__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvarBuddyCtl)
			CA_VariantClear (pvarBuddyCtl);
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownSetIncrement (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               long plIncrement)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetIncrement_ (__vtblIFacePtr,
	                                                   plIncrement));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownGetIncrement (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               long *plIncrement)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long plIncrement__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetIncrement_ (__vtblIFacePtr,
	                                                   &plIncrement__Temp));

	if (plIncrement)
		{
		*plIncrement = plIncrement__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownSetMax (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, long plMax)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMax_ (__vtblIFacePtr, plMax));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownGetMax (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, long *plMax)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long plMax__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMax_ (__vtblIFacePtr, &plMax__Temp));

	if (plMax)
		{
		*plMax = plMax__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownSetMin (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, long plMin)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMin_ (__vtblIFacePtr, plMin));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownGetMin (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, long *plMin)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long plMin__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMin_ (__vtblIFacePtr, &plMin__Temp));

	if (plMin)
		{
		*plMin = plMin__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownSetOrientation (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 enum MSComCtl2Enum_OrientationConstants penumOrientation)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetOrientation_ (__vtblIFacePtr,
	                                                     penumOrientation));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownGetOrientation (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 enum MSComCtl2Enum_OrientationConstants *penumOrientation)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long penumOrientation__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetOrientation_ (__vtblIFacePtr,
	                                                     (long *)&penumOrientation__Temp));

	if (penumOrientation)
		{
		*penumOrientation = penumOrientation__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownSetSyncBuddy (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL pfSyncBuddy)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetSyncBuddy_ (__vtblIFacePtr,
	                                                   pfSyncBuddy));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownGetSyncBuddy (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *pfSyncBuddy)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pfSyncBuddy__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetSyncBuddy_ (__vtblIFacePtr,
	                                                   &pfSyncBuddy__Temp));

	if (pfSyncBuddy)
		{
		*pfSyncBuddy = pfSyncBuddy__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownSetWrap (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, VBOOL pfWrap)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetWrap_ (__vtblIFacePtr, pfWrap));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownGetWrap (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, VBOOL *pfWrap)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pfWrap__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetWrap_ (__vtblIFacePtr,
	                                              &pfWrap__Temp));

	if (pfWrap)
		{
		*pfWrap = pfWrap__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownSetBuddyProperty (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VARIANT pvarDispidBuddyProperty)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetBuddyProperty_ (__vtblIFacePtr,
	                                                       pvarDispidBuddyProperty));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownGetBuddyProperty (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VARIANT *pvarDispidBuddyProperty)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VARIANT pvarDispidBuddyProperty__Temp;

	if (pvarDispidBuddyProperty)
		CA_VariantSetEmpty (pvarDispidBuddyProperty);
	CA_VariantSetEmpty (&pvarDispidBuddyProperty__Temp);

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetBuddyProperty_ (__vtblIFacePtr,
	                                                       &pvarDispidBuddyProperty__Temp));

	if (pvarDispidBuddyProperty)
		{
		*pvarDispidBuddyProperty = pvarDispidBuddyProperty__Temp;
		CA_VariantSetEmpty (&pvarDispidBuddyProperty__Temp);
		}

Error:
	CA_VariantClear (&pvarDispidBuddyProperty__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvarDispidBuddyProperty)
			CA_VariantClear (pvarDispidBuddyProperty);
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownSetEnabled (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VBOOL pfEnable)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetEnabled_ (__vtblIFacePtr, pfEnable));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownGetEnabled (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VBOOL *pfEnable)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pfEnable__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetEnabled_ (__vtblIFacePtr,
	                                                 &pfEnable__Temp));

	if (pfEnable)
		{
		*pfEnable = pfEnable__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownGethWnd (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          MSComCtl2Type_OLE_HANDLE *phWnd)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long phWnd__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GethWnd_ (__vtblIFacePtr, &phWnd__Temp));

	if (phWnd)
		{
		*phWnd = phWnd__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownGetOLEDropMode (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 enum MSComCtl2Enum_OLEDropConstants *psOLEDropMode)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long psOLEDropMode__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetOLEDropMode_ (__vtblIFacePtr,
	                                                     (long *)&psOLEDropMode__Temp));

	if (psOLEDropMode)
		{
		*psOLEDropMode = psOLEDropMode__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownSetOLEDropMode (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 enum MSComCtl2Enum_OLEDropConstants psOLEDropMode)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetOLEDropMode_ (__vtblIFacePtr,
	                                                     psOLEDropMode));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownAboutBox (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->AboutBox_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IUpDownOLEDrag (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IUpDown_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IUpDown, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->OLEDrag_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IUpDown, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_NewIMonthView (int panel, const char *label, int top,
                                         int left, int *controlID, int *UILError)
{
	HRESULT __result = S_OK;
	int ctrlId;
	GUID clsid = {0x232E456A, 0x87C3, 0x11D1, 0x8B, 0xE3, 0x0, 0x0, 0xF8, 0x75,
	              0x4D, 0xA1};
	const char * licStr = "651A8940-87C5-11d1-8BE3-0000F8754DA1";

	ctrlId = NewActiveXCtrl (panel, label, top, left, &clsid,
	                         &MSComCtl2_IID_IMonthView, licStr, &__result);

	__ActiveXCtrlErrorHandler();

	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetAppearance (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   enum MSComCtl2Enum_AppearanceConstants *penumAppearances)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long penumAppearances__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetAppearance_ (__vtblIFacePtr,
	                                                    (long *)&penumAppearances__Temp));

	if (penumAppearances)
		{
		*penumAppearances = penumAppearances__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetAppearance (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   enum MSComCtl2Enum_AppearanceConstants penumAppearances)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetAppearance_ (__vtblIFacePtr,
	                                                    penumAppearances));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetBackColor (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Type_OLE_COLOR *pclrBackColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	unsigned long pclrBackColor__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetBackColor_ (__vtblIFacePtr,
	                                                   &pclrBackColor__Temp));

	if (pclrBackColor)
		{
		*pclrBackColor = pclrBackColor__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetBackColor (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Type_OLE_COLOR pclrBackColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetBackColor_ (__vtblIFacePtr,
	                                                   pclrBackColor));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetBorderStyle (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_BorderStyleConstants *psBorderStyle)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long psBorderStyle__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetBorderStyle_ (__vtblIFacePtr,
	                                                     (long *)&psBorderStyle__Temp));

	if (psBorderStyle)
		{
		*psBorderStyle = psBorderStyle__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetBorderStyle (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_BorderStyleConstants psBorderStyle)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetBorderStyle_ (__vtblIFacePtr,
	                                                     psBorderStyle));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetDay (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, short *psDay)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	short psDay__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetDay_ (__vtblIFacePtr, &psDay__Temp));

	if (psDay)
		{
		*psDay = psDay__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetDay (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, short psDay)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetDay_ (__vtblIFacePtr, psDay));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetDayBold (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                DATE dateIndex, VBOOL *pbDayBold)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbDayBold__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetDayBold_ (__vtblIFacePtr, dateIndex,
	                                                 &pbDayBold__Temp));

	if (pbDayBold)
		{
		*pbDayBold = pbDayBold__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetDayBold (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                DATE dateIndex, VBOOL pbDayBold)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetDayBold_ (__vtblIFacePtr, dateIndex,
	                                                 pbDayBold));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetDayOfWeek (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  enum MSComCtl2Enum_DayConstants *penumDayOfWeek)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long penumDayOfWeek__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetDayOfWeek_ (__vtblIFacePtr,
	                                                   (long *)&penumDayOfWeek__Temp));

	if (penumDayOfWeek)
		{
		*penumDayOfWeek = penumDayOfWeek__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetDayOfWeek (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  enum MSComCtl2Enum_DayConstants penumDayOfWeek)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetDayOfWeek_ (__vtblIFacePtr,
	                                                   penumDayOfWeek));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetEnabled (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL *pbEnabled)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbEnabled__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetEnabled_ (__vtblIFacePtr,
	                                                 &pbEnabled__Temp));

	if (pbEnabled)
		{
		*pbEnabled = pbEnabled__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetEnabled (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL pbEnabled)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetEnabled_ (__vtblIFacePtr, pbEnabled));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetFont (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             MSComCtl2Obj_IFontDisp *ppFont)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppFont__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (ppFont)
		*ppFont = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetFont_ (__vtblIFacePtr,
	                                              &ppFont__AutoType));
	

	if (ppFont)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (ppFont__AutoType,
		                                             &MSComCtl2_IID_Font,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, ppFont));
		ppFont__AutoType = 0;
		}

Error:
	if (ppFont__AutoType)
		ppFont__AutoType->lpVtbl->Release (ppFont__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppFont)
			{
			CA_DiscardObjHandle (*ppFont);
			*ppFont = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetFont (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             MSComCtl2Obj_IFontDisp ppFont)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppFont__AutoType = 0;

	if (ppFont)
		{
		__caErrChk (CA_GetInterfaceFromObjHandle (ppFont, &MSComCtl2_IID_Font, 1,
	                                          &ppFont__AutoType, NULL));
		}

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetFont_ (__vtblIFacePtr,
	                                              ppFont__AutoType));

Error:
	if (ppFont__AutoType)
		ppFont__AutoType->lpVtbl->Release (ppFont__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetByRefFont (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Obj_IFontDisp ppFont)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppFont__AutoType = 0;

	if (ppFont)
		{
		__caErrChk (CA_GetInterfaceFromObjHandle (ppFont, &MSComCtl2_IID_Font, 1,
	                                          &ppFont__AutoType, NULL));
		}

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetByRefFont_ (__vtblIFacePtr,
	                                                   ppFont__AutoType));

Error:
	if (ppFont__AutoType)
		ppFont__AutoType->lpVtbl->Release (ppFont__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetForeColor (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Type_OLE_COLOR *pclrForeColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	unsigned long pclrForeColor__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetForeColor_ (__vtblIFacePtr,
	                                                   &pclrForeColor__Temp));

	if (pclrForeColor)
		{
		*pclrForeColor = pclrForeColor__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetForeColor (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Type_OLE_COLOR pclrForeColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetForeColor_ (__vtblIFacePtr,
	                                                   pclrForeColor));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGethWnd (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             MSComCtl2Type_OLE_HANDLE *phWnd)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long phWnd__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GethWnd_ (__vtblIFacePtr, &phWnd__Temp));

	if (phWnd)
		{
		*phWnd = phWnd__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSethWnd (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             MSComCtl2Type_OLE_HANDLE phWnd)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SethWnd_ (__vtblIFacePtr, phWnd));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMaxDate (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                DATE *pdateMaxDate)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	DATE pdateMaxDate__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMaxDate_ (__vtblIFacePtr,
	                                                 &pdateMaxDate__Temp));

	if (pdateMaxDate)
		{
		*pdateMaxDate = pdateMaxDate__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMaxDate (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                DATE pdateMaxDate)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMaxDate_ (__vtblIFacePtr,
	                                                 pdateMaxDate));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMaxSelCount (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    short *psMaxSelCount)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	short psMaxSelCount__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMaxSelCount_ (__vtblIFacePtr,
	                                                     &psMaxSelCount__Temp));

	if (psMaxSelCount)
		{
		*psMaxSelCount = psMaxSelCount__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMaxSelCount (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    short psMaxSelCount)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMaxSelCount_ (__vtblIFacePtr,
	                                                     psMaxSelCount));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMinDate (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                DATE *pdateMinDate)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	DATE pdateMinDate__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMinDate_ (__vtblIFacePtr,
	                                                 &pdateMinDate__Temp));

	if (pdateMinDate)
		{
		*pdateMinDate = pdateMinDate__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMinDate (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                DATE pdateMinDate)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMinDate_ (__vtblIFacePtr,
	                                                 pdateMinDate));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMonth (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              enum MSComCtl2Enum_MonthConstants *psMonth)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long psMonth__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMonth_ (__vtblIFacePtr,
	                                               (long *)&psMonth__Temp));

	if (psMonth)
		{
		*psMonth = psMonth__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMonth (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              enum MSComCtl2Enum_MonthConstants psMonth)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMonth_ (__vtblIFacePtr, psMonth));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMonthBackColor (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSComCtl2Type_OLE_COLOR *pclrMonthBackColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	unsigned long pclrMonthBackColor__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMonthBackColor_ (__vtblIFacePtr,
	                                                        &pclrMonthBackColor__Temp));

	if (pclrMonthBackColor)
		{
		*pclrMonthBackColor = pclrMonthBackColor__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMonthBackColor (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSComCtl2Type_OLE_COLOR pclrMonthBackColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMonthBackColor_ (__vtblIFacePtr,
	                                                        pclrMonthBackColor));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMonthColumns (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     short *psMonthColumns)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	short psMonthColumns__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMonthColumns_ (__vtblIFacePtr,
	                                                      &psMonthColumns__Temp));

	if (psMonthColumns)
		{
		*psMonthColumns = psMonthColumns__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMonthColumns (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     short psMonthColumns)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMonthColumns_ (__vtblIFacePtr,
	                                                      psMonthColumns));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMonthRows (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  short *psMonthRows)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	short psMonthRows__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMonthRows_ (__vtblIFacePtr,
	                                                   &psMonthRows__Temp));

	if (psMonthRows)
		{
		*psMonthRows = psMonthRows__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMonthRows (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  short psMonthRows)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMonthRows_ (__vtblIFacePtr,
	                                                   psMonthRows));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMouseIcon (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Obj_IPictureDisp *ppMouseIcon)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppMouseIcon__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (ppMouseIcon)
		*ppMouseIcon = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMouseIcon_ (__vtblIFacePtr,
	                                                   &ppMouseIcon__AutoType));
	

	if (ppMouseIcon)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (ppMouseIcon__AutoType,
		                                             &MSComCtl2_IID_Picture,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, ppMouseIcon));
		ppMouseIcon__AutoType = 0;
		}

Error:
	if (ppMouseIcon__AutoType)
		ppMouseIcon__AutoType->lpVtbl->Release (ppMouseIcon__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppMouseIcon)
			{
			CA_DiscardObjHandle (*ppMouseIcon);
			*ppMouseIcon = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMouseIcon (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Obj_IPictureDisp ppMouseIcon)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppMouseIcon__AutoType = 0;

	if (ppMouseIcon)
		{
		__caErrChk (CA_GetInterfaceFromObjHandle (ppMouseIcon,
	                                          &MSComCtl2_IID_Picture, 1,
	                                          &ppMouseIcon__AutoType, NULL));
		}

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMouseIcon_ (__vtblIFacePtr,
	                                                   ppMouseIcon__AutoType));

Error:
	if (ppMouseIcon__AutoType)
		ppMouseIcon__AutoType->lpVtbl->Release (ppMouseIcon__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetByRefMouseIcon (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSComCtl2Obj_IPictureDisp ppMouseIcon)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppMouseIcon__AutoType = 0;

	if (ppMouseIcon)
		{
		__caErrChk (CA_GetInterfaceFromObjHandle (ppMouseIcon,
	                                          &MSComCtl2_IID_Picture, 1,
	                                          &ppMouseIcon__AutoType, NULL));
		}

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetByRefMouseIcon_ (__vtblIFacePtr,
	                                                        ppMouseIcon__AutoType));

Error:
	if (ppMouseIcon__AutoType)
		ppMouseIcon__AutoType->lpVtbl->Release (ppMouseIcon__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMousePointer (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     enum MSComCtl2Enum_MousePointerConstants *psMousePointer)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long psMousePointer__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMousePointer_ (__vtblIFacePtr,
	                                                      (long *)&psMousePointer__Temp));

	if (psMousePointer)
		{
		*psMousePointer = psMousePointer__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMousePointer (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     enum MSComCtl2Enum_MousePointerConstants psMousePointer)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMousePointer_ (__vtblIFacePtr,
	                                                      psMousePointer));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMultiSelect (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL *pbMultiSelect)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbMultiSelect__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMultiSelect_ (__vtblIFacePtr,
	                                                     &pbMultiSelect__Temp));

	if (pbMultiSelect)
		{
		*pbMultiSelect = pbMultiSelect__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMultiSelect (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL pbMultiSelect)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMultiSelect_ (__vtblIFacePtr,
	                                                     pbMultiSelect));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetOLEDropMode (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_OLEDropConstants *psOLEDropMode)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long psOLEDropMode__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetOLEDropMode_ (__vtblIFacePtr,
	                                                     (long *)&psOLEDropMode__Temp));

	if (psOLEDropMode)
		{
		*psOLEDropMode = psOLEDropMode__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetOLEDropMode (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_OLEDropConstants psOLEDropMode)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetOLEDropMode_ (__vtblIFacePtr,
	                                                     psOLEDropMode));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetScrollRate (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   short *psScrollRate)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	short psScrollRate__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetScrollRate_ (__vtblIFacePtr,
	                                                    &psScrollRate__Temp));

	if (psScrollRate)
		{
		*psScrollRate = psScrollRate__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetScrollRate (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   short psScrollRate)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetScrollRate_ (__vtblIFacePtr,
	                                                    psScrollRate));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetSelEnd (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE *pdateSelEnd)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	DATE pdateSelEnd__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetSelEnd_ (__vtblIFacePtr,
	                                                &pdateSelEnd__Temp));

	if (pdateSelEnd)
		{
		*pdateSelEnd = pdateSelEnd__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetSelEnd (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE pdateSelEnd)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetSelEnd_ (__vtblIFacePtr,
	                                                pdateSelEnd));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetSelStart (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 DATE *pdateSelStart)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	DATE pdateSelStart__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetSelStart_ (__vtblIFacePtr,
	                                                  &pdateSelStart__Temp));

	if (pdateSelStart)
		{
		*pdateSelStart = pdateSelStart__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetSelStart (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 DATE pdateSelStart)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetSelStart_ (__vtblIFacePtr,
	                                                  pdateSelStart));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetShowToday (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL *pbShowToday)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbShowToday__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetShowToday_ (__vtblIFacePtr,
	                                                   &pbShowToday__Temp));

	if (pbShowToday)
		{
		*pbShowToday = pbShowToday__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetShowToday (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL pbShowToday)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetShowToday_ (__vtblIFacePtr,
	                                                   pbShowToday));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetShowWeekNumbers (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL *pbShowWeekNumbers)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbShowWeekNumbers__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetShowWeekNumbers_ (__vtblIFacePtr,
	                                                         &pbShowWeekNumbers__Temp));

	if (pbShowWeekNumbers)
		{
		*pbShowWeekNumbers = pbShowWeekNumbers__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetShowWeekNumbers (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL pbShowWeekNumbers)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetShowWeekNumbers_ (__vtblIFacePtr,
	                                                         pbShowWeekNumbers));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetStartOfWeek (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_DayConstants *penumStartOfWeek)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long penumStartOfWeek__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetStartOfWeek_ (__vtblIFacePtr,
	                                                     (long *)&penumStartOfWeek__Temp));

	if (penumStartOfWeek)
		{
		*penumStartOfWeek = penumStartOfWeek__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetStartOfWeek (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_DayConstants penumStartOfWeek)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetStartOfWeek_ (__vtblIFacePtr,
	                                                     penumStartOfWeek));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetTitleBackColor (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSComCtl2Type_OLE_COLOR *pclrTitleBackColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	unsigned long pclrTitleBackColor__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetTitleBackColor_ (__vtblIFacePtr,
	                                                        &pclrTitleBackColor__Temp));

	if (pclrTitleBackColor)
		{
		*pclrTitleBackColor = pclrTitleBackColor__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetTitleBackColor (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSComCtl2Type_OLE_COLOR pclrTitleBackColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetTitleBackColor_ (__vtblIFacePtr,
	                                                        pclrTitleBackColor));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetTitleForeColor (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSComCtl2Type_OLE_COLOR *pclrTitleForeColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	unsigned long pclrTitleForeColor__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetTitleForeColor_ (__vtblIFacePtr,
	                                                        &pclrTitleForeColor__Temp));

	if (pclrTitleForeColor)
		{
		*pclrTitleForeColor = pclrTitleForeColor__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetTitleForeColor (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSComCtl2Type_OLE_COLOR pclrTitleForeColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetTitleForeColor_ (__vtblIFacePtr,
	                                                        pclrTitleForeColor));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetTrailingForeColor (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          MSComCtl2Type_OLE_COLOR *pclrTrailingForeColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	unsigned long pclrTrailingForeColor__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetTrailingForeColor_ (__vtblIFacePtr,
	                                                           &pclrTrailingForeColor__Temp));

	if (pclrTrailingForeColor)
		{
		*pclrTrailingForeColor = pclrTrailingForeColor__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetTrailingForeColor (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          MSComCtl2Type_OLE_COLOR pclrTrailingForeColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetTrailingForeColor_ (__vtblIFacePtr,
	                                                           pclrTrailingForeColor));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetValue (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              DATE *pdateValue)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	DATE pdateValue__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetValue_ (__vtblIFacePtr,
	                                               &pdateValue__Temp));

	if (pdateValue)
		{
		*pdateValue = pdateValue__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetValue (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              DATE pdateValue)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetValue_ (__vtblIFacePtr, pdateValue));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGet_Value (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE *pdateValue)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	DATE pdateValue__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Get_Value_ (__vtblIFacePtr,
	                                                &pdateValue__Temp));

	if (pdateValue)
		{
		*pdateValue = pdateValue__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSet_Value (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE pdateValue)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Set_Value_ (__vtblIFacePtr, pdateValue));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetVisibleDays (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    short sIndex,
                                                    DATE *pdateVisibleDays)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	DATE pdateVisibleDays__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetVisibleDays_ (__vtblIFacePtr,
	                                                     sIndex,
	                                                     &pdateVisibleDays__Temp));

	if (pdateVisibleDays)
		{
		*pdateVisibleDays = pdateVisibleDays__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetVisibleDays (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    short sIndex,
                                                    DATE pdateVisibleDays)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetVisibleDays_ (__vtblIFacePtr,
	                                                     sIndex,
	                                                     pdateVisibleDays));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetWeek (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, short *psWeek)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	short psWeek__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetWeek_ (__vtblIFacePtr,
	                                              &psWeek__Temp));

	if (psWeek)
		{
		*psWeek = psWeek__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetWeek (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, short psWeek)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetWeek_ (__vtblIFacePtr, psWeek));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewGetYear (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, short *psYear)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	short psYear__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetYear_ (__vtblIFacePtr,
	                                              &psYear__Temp));

	if (psYear)
		{
		*psYear = psYear__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewSetYear (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, short psYear)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetYear_ (__vtblIFacePtr, psYear));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewAboutBox (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->AboutBox_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewComputeControlSize (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        short rows,
                                                        short columns,
                                                        float *width,
                                                        float *height)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	float width__Temp;
	float height__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->ComputeControlSize_ (__vtblIFacePtr,
	                                                         rows, columns,
	                                                         &width__Temp,
	                                                         &height__Temp));

	if (width)
		{
		*width = width__Temp;
		}
	if (height)
		{
		*height = height__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewHitTest (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, long x,
                                             long y, DATE *date,
                                             enum MSComCtl2Enum_MonthViewHitTestAreas *enumHitTestArea)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	DATE date__Temp;
	long enumHitTestArea__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->HitTest_ (__vtblIFacePtr, x, y,
	                                              &date__Temp,
	                                              (long *)&enumHitTestArea__Temp));

	if (date)
		{
		*date = date__Temp;
		}
	if (enumHitTestArea)
		{
		*enumHitTestArea = enumHitTestArea__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewOLEDrag (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->OLEDrag_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IMonthViewRefresh (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IMonthView_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IMonthView, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Refresh_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IMonthView, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_NewIDTPicker (int panel, const char *label, int top,
                                        int left, int *controlID, int *UILError)
{
	HRESULT __result = S_OK;
	int ctrlId;
	GUID clsid = {0x20DD1B9E, 0x87C4, 0x11D1, 0x8B, 0xE3, 0x0, 0x0, 0xF8, 0x75,
	              0x4D, 0xA1};
	const char * licStr = "651A8940-87C5-11d1-8BE3-0000F8754DA1";

	ctrlId = NewActiveXCtrl (panel, label, top, left, &clsid,
	                         &MSComCtl2_IID_IDTPicker, licStr, &__result);

	__ActiveXCtrlErrorHandler();

	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetFormat (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              enum MSComCtl2Enum_FormatConstants *penumFormat)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long penumFormat__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetFormat_ (__vtblIFacePtr,
	                                                (long *)&penumFormat__Temp));

	if (penumFormat)
		{
		*penumFormat = penumFormat__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetFormat (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              enum MSComCtl2Enum_FormatConstants penumFormat)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetFormat_ (__vtblIFacePtr,
	                                                penumFormat));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetCalendarBackColor (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSComCtl2Type_OLE_COLOR *pclrCalendarBackColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	unsigned long pclrCalendarBackColor__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetCalendarBackColor_ (__vtblIFacePtr,
	                                                           &pclrCalendarBackColor__Temp));

	if (pclrCalendarBackColor)
		{
		*pclrCalendarBackColor = pclrCalendarBackColor__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetCalendarBackColor (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSComCtl2Type_OLE_COLOR pclrCalendarBackColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetCalendarBackColor_ (__vtblIFacePtr,
	                                                           pclrCalendarBackColor));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetCalendarForeColor (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSComCtl2Type_OLE_COLOR *pclrCalendarForeColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	unsigned long pclrCalendarForeColor__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetCalendarForeColor_ (__vtblIFacePtr,
	                                                           &pclrCalendarForeColor__Temp));

	if (pclrCalendarForeColor)
		{
		*pclrCalendarForeColor = pclrCalendarForeColor__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetCalendarForeColor (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSComCtl2Type_OLE_COLOR pclrCalendarForeColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetCalendarForeColor_ (__vtblIFacePtr,
	                                                           pclrCalendarForeColor));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetCalendarTitleBackColor (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              MSComCtl2Type_OLE_COLOR *pclrTitleBackColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	unsigned long pclrTitleBackColor__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetCalendarTitleBackColor_ (__vtblIFacePtr,
	                                                                &pclrTitleBackColor__Temp));

	if (pclrTitleBackColor)
		{
		*pclrTitleBackColor = pclrTitleBackColor__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetCalendarTitleBackColor (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              MSComCtl2Type_OLE_COLOR pclrTitleBackColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetCalendarTitleBackColor_ (__vtblIFacePtr,
	                                                                pclrTitleBackColor));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetCalendarTitleForeColor (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              MSComCtl2Type_OLE_COLOR *pclrTitleForeColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	unsigned long pclrTitleForeColor__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetCalendarTitleForeColor_ (__vtblIFacePtr,
	                                                                &pclrTitleForeColor__Temp));

	if (pclrTitleForeColor)
		{
		*pclrTitleForeColor = pclrTitleForeColor__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetCalendarTitleForeColor (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              MSComCtl2Type_OLE_COLOR pclrTitleForeColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetCalendarTitleForeColor_ (__vtblIFacePtr,
	                                                                pclrTitleForeColor));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetCalendarTrailingForeColor (CAObjHandle objectHandle,
                                                                 ERRORINFO *errorInfo,
                                                                 MSComCtl2Type_OLE_COLOR *pclrTrailingForeColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	unsigned long pclrTrailingForeColor__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetCalendarTrailingForeColor_ (__vtblIFacePtr,
	                                                                   &pclrTrailingForeColor__Temp));

	if (pclrTrailingForeColor)
		{
		*pclrTrailingForeColor = pclrTrailingForeColor__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetCalendarTrailingForeColor (CAObjHandle objectHandle,
                                                                 ERRORINFO *errorInfo,
                                                                 MSComCtl2Type_OLE_COLOR pclrTrailingForeColor)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetCalendarTrailingForeColor_ (__vtblIFacePtr,
	                                                                   pclrTrailingForeColor));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetCheckBox (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL *pbEnabled)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbEnabled__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetCheckBox_ (__vtblIFacePtr,
	                                                  &pbEnabled__Temp));

	if (pbEnabled)
		{
		*pbEnabled = pbEnabled__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetCheckBox (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL pbEnabled)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetCheckBox_ (__vtblIFacePtr,
	                                                  pbEnabled));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetCustomFormat (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    char **pbstrCustomFormat)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR pbstrCustomFormat__AutoType = 0;

	if (pbstrCustomFormat)
		*pbstrCustomFormat = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetCustomFormat_ (__vtblIFacePtr,
	                                                      &pbstrCustomFormat__AutoType));

	if (pbstrCustomFormat)
		__caErrChk (CA_BSTRGetCString (pbstrCustomFormat__AutoType,
	                               pbstrCustomFormat));

Error:
	CA_FreeBSTR (pbstrCustomFormat__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pbstrCustomFormat)
			{
			CA_FreeMemory (*pbstrCustomFormat);
			*pbstrCustomFormat = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetCustomFormat (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    const char *pbstrCustomFormat)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR pbstrCustomFormat__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (pbstrCustomFormat,
	                              &pbstrCustomFormat__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetCustomFormat_ (__vtblIFacePtr,
	                                                      pbstrCustomFormat__AutoType));

Error:
	CA_FreeBSTR (pbstrCustomFormat__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetDay (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, VARIANT *pvDay)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VARIANT pvDay__Temp;

	if (pvDay)
		CA_VariantSetEmpty (pvDay);
	CA_VariantSetEmpty (&pvDay__Temp);

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetDay_ (__vtblIFacePtr, &pvDay__Temp));

	if (pvDay)
		{
		*pvDay = pvDay__Temp;
		CA_VariantSetEmpty (&pvDay__Temp);
		}

Error:
	CA_VariantClear (&pvDay__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvDay)
			CA_VariantClear (pvDay);
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetDay (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, VARIANT pvDay)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetDay_ (__vtblIFacePtr, pvDay));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetDayOfWeek (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VARIANT *pvDayOfWeek)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VARIANT pvDayOfWeek__Temp;

	if (pvDayOfWeek)
		CA_VariantSetEmpty (pvDayOfWeek);
	CA_VariantSetEmpty (&pvDayOfWeek__Temp);

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetDayOfWeek_ (__vtblIFacePtr,
	                                                   &pvDayOfWeek__Temp));

	if (pvDayOfWeek)
		{
		*pvDayOfWeek = pvDayOfWeek__Temp;
		CA_VariantSetEmpty (&pvDayOfWeek__Temp);
		}

Error:
	CA_VariantClear (&pvDayOfWeek__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvDayOfWeek)
			CA_VariantClear (pvDayOfWeek);
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetDayOfWeek (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VARIANT pvDayOfWeek)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetDayOfWeek_ (__vtblIFacePtr,
	                                                   pvDayOfWeek));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetEnabled (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *pbEnabled)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbEnabled__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetEnabled_ (__vtblIFacePtr,
	                                                 &pbEnabled__Temp));

	if (pbEnabled)
		{
		*pbEnabled = pbEnabled__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetEnabled (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL pbEnabled)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetEnabled_ (__vtblIFacePtr, pbEnabled));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetFont (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            MSComCtl2Obj_IFontDisp *ppFont)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppFont__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (ppFont)
		*ppFont = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetFont_ (__vtblIFacePtr,
	                                              &ppFont__AutoType));
	

	if (ppFont)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (ppFont__AutoType,
		                                             &MSComCtl2_IID_Font,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, ppFont));
		ppFont__AutoType = 0;
		}

Error:
	if (ppFont__AutoType)
		ppFont__AutoType->lpVtbl->Release (ppFont__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppFont)
			{
			CA_DiscardObjHandle (*ppFont);
			*ppFont = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetFont (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            MSComCtl2Obj_IFontDisp ppFont)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppFont__AutoType = 0;

	if (ppFont)
		{
		__caErrChk (CA_GetInterfaceFromObjHandle (ppFont, &MSComCtl2_IID_Font, 1,
	                                          &ppFont__AutoType, NULL));
		}

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetFont_ (__vtblIFacePtr,
	                                              ppFont__AutoType));

Error:
	if (ppFont__AutoType)
		ppFont__AutoType->lpVtbl->Release (ppFont__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetByRefFont (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSComCtl2Obj_IFontDisp ppFont)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppFont__AutoType = 0;

	if (ppFont)
		{
		__caErrChk (CA_GetInterfaceFromObjHandle (ppFont, &MSComCtl2_IID_Font, 1,
	                                          &ppFont__AutoType, NULL));
		}

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetByRefFont_ (__vtblIFacePtr,
	                                                   ppFont__AutoType));

Error:
	if (ppFont__AutoType)
		ppFont__AutoType->lpVtbl->Release (ppFont__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetHour (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            VARIANT *pvHour)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VARIANT pvHour__Temp;

	if (pvHour)
		CA_VariantSetEmpty (pvHour);
	CA_VariantSetEmpty (&pvHour__Temp);

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetHour_ (__vtblIFacePtr,
	                                              &pvHour__Temp));

	if (pvHour)
		{
		*pvHour = pvHour__Temp;
		CA_VariantSetEmpty (&pvHour__Temp);
		}

Error:
	CA_VariantClear (&pvHour__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvHour)
			CA_VariantClear (pvHour);
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetHour (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, VARIANT pvHour)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetHour_ (__vtblIFacePtr, pvHour));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGethWnd (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            MSComCtl2Type_OLE_HANDLE *phWnd)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long phWnd__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GethWnd_ (__vtblIFacePtr, &phWnd__Temp));

	if (phWnd)
		{
		*phWnd = phWnd__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSethWnd (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            MSComCtl2Type_OLE_HANDLE phWnd)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SethWnd_ (__vtblIFacePtr, phWnd));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetMaxDate (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE *pdateMaxDate)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	DATE pdateMaxDate__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMaxDate_ (__vtblIFacePtr,
	                                                 &pdateMaxDate__Temp));

	if (pdateMaxDate)
		{
		*pdateMaxDate = pdateMaxDate__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetMaxDate (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE pdateMaxDate)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMaxDate_ (__vtblIFacePtr,
	                                                 pdateMaxDate));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetMinDate (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE *pdateMinDate)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	DATE pdateMinDate__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMinDate_ (__vtblIFacePtr,
	                                                 &pdateMinDate__Temp));

	if (pdateMinDate)
		{
		*pdateMinDate = pdateMinDate__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetMinDate (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE pdateMinDate)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMinDate_ (__vtblIFacePtr,
	                                                 pdateMinDate));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetMinute (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VARIANT *pvMinute)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VARIANT pvMinute__Temp;

	if (pvMinute)
		CA_VariantSetEmpty (pvMinute);
	CA_VariantSetEmpty (&pvMinute__Temp);

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMinute_ (__vtblIFacePtr,
	                                                &pvMinute__Temp));

	if (pvMinute)
		{
		*pvMinute = pvMinute__Temp;
		CA_VariantSetEmpty (&pvMinute__Temp);
		}

Error:
	CA_VariantClear (&pvMinute__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvMinute)
			CA_VariantClear (pvMinute);
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetMinute (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VARIANT pvMinute)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMinute_ (__vtblIFacePtr, pvMinute));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetMonth (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VARIANT *pvMonth)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VARIANT pvMonth__Temp;

	if (pvMonth)
		CA_VariantSetEmpty (pvMonth);
	CA_VariantSetEmpty (&pvMonth__Temp);

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMonth_ (__vtblIFacePtr,
	                                               &pvMonth__Temp));

	if (pvMonth)
		{
		*pvMonth = pvMonth__Temp;
		CA_VariantSetEmpty (&pvMonth__Temp);
		}

Error:
	CA_VariantClear (&pvMonth__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvMonth)
			CA_VariantClear (pvMonth);
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetMonth (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VARIANT pvMonth)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMonth_ (__vtblIFacePtr, pvMonth));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetMouseIcon (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSComCtl2Obj_IPictureDisp *ppMouseIcon)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppMouseIcon__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (ppMouseIcon)
		*ppMouseIcon = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMouseIcon_ (__vtblIFacePtr,
	                                                   &ppMouseIcon__AutoType));
	

	if (ppMouseIcon)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (ppMouseIcon__AutoType,
		                                             &MSComCtl2_IID_Picture,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, ppMouseIcon));
		ppMouseIcon__AutoType = 0;
		}

Error:
	if (ppMouseIcon__AutoType)
		ppMouseIcon__AutoType->lpVtbl->Release (ppMouseIcon__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppMouseIcon)
			{
			CA_DiscardObjHandle (*ppMouseIcon);
			*ppMouseIcon = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetMouseIcon (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSComCtl2Obj_IPictureDisp ppMouseIcon)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppMouseIcon__AutoType = 0;

	if (ppMouseIcon)
		{
		__caErrChk (CA_GetInterfaceFromObjHandle (ppMouseIcon,
	                                          &MSComCtl2_IID_Picture, 1,
	                                          &ppMouseIcon__AutoType, NULL));
		}

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMouseIcon_ (__vtblIFacePtr,
	                                                   ppMouseIcon__AutoType));

Error:
	if (ppMouseIcon__AutoType)
		ppMouseIcon__AutoType->lpVtbl->Release (ppMouseIcon__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetByRefMouseIcon (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      MSComCtl2Obj_IPictureDisp ppMouseIcon)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppMouseIcon__AutoType = 0;

	if (ppMouseIcon)
		{
		__caErrChk (CA_GetInterfaceFromObjHandle (ppMouseIcon,
	                                          &MSComCtl2_IID_Picture, 1,
	                                          &ppMouseIcon__AutoType, NULL));
		}

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetByRefMouseIcon_ (__vtblIFacePtr,
	                                                        ppMouseIcon__AutoType));

Error:
	if (ppMouseIcon__AutoType)
		ppMouseIcon__AutoType->lpVtbl->Release (ppMouseIcon__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetMousePointer (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_MousePointerConstants *psMousePointer)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long psMousePointer__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMousePointer_ (__vtblIFacePtr,
	                                                      (long *)&psMousePointer__Temp));

	if (psMousePointer)
		{
		*psMousePointer = psMousePointer__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetMousePointer (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_MousePointerConstants psMousePointer)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMousePointer_ (__vtblIFacePtr,
	                                                      psMousePointer));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetOLEDropMode (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   enum MSComCtl2Enum_OLEDropConstants *psOLEDropMode)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long psOLEDropMode__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetOLEDropMode_ (__vtblIFacePtr,
	                                                     (long *)&psOLEDropMode__Temp));

	if (psOLEDropMode)
		{
		*psOLEDropMode = psOLEDropMode__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetOLEDropMode (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   enum MSComCtl2Enum_OLEDropConstants psOLEDropMode)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetOLEDropMode_ (__vtblIFacePtr,
	                                                     psOLEDropMode));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetSecond (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VARIANT *pvSecond)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VARIANT pvSecond__Temp;

	if (pvSecond)
		CA_VariantSetEmpty (pvSecond);
	CA_VariantSetEmpty (&pvSecond__Temp);

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetSecond_ (__vtblIFacePtr,
	                                                &pvSecond__Temp));

	if (pvSecond)
		{
		*pvSecond = pvSecond__Temp;
		CA_VariantSetEmpty (&pvSecond__Temp);
		}

Error:
	CA_VariantClear (&pvSecond__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvSecond)
			CA_VariantClear (pvSecond);
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetSecond (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VARIANT pvSecond)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetSecond_ (__vtblIFacePtr, pvSecond));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetUpDown (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VBOOL *pbUpDown)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbUpDown__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetUpDown_ (__vtblIFacePtr,
	                                                &pbUpDown__Temp));

	if (pbUpDown)
		{
		*pbUpDown = pbUpDown__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetUpDown (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VBOOL pbUpDown)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetUpDown_ (__vtblIFacePtr, pbUpDown));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetValue (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VARIANT *pvdateValue)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VARIANT pvdateValue__Temp;

	if (pvdateValue)
		CA_VariantSetEmpty (pvdateValue);
	CA_VariantSetEmpty (&pvdateValue__Temp);

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetValue_ (__vtblIFacePtr,
	                                               &pvdateValue__Temp));

	if (pvdateValue)
		{
		*pvdateValue = pvdateValue__Temp;
		CA_VariantSetEmpty (&pvdateValue__Temp);
		}

Error:
	CA_VariantClear (&pvdateValue__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvdateValue)
			CA_VariantClear (pvdateValue);
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetValue (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VARIANT pvdateValue)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetValue_ (__vtblIFacePtr, pvdateValue));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGet_Value (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VARIANT *pvdateValue)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VARIANT pvdateValue__Temp;

	if (pvdateValue)
		CA_VariantSetEmpty (pvdateValue);
	CA_VariantSetEmpty (&pvdateValue__Temp);

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Get_Value_ (__vtblIFacePtr,
	                                                &pvdateValue__Temp));

	if (pvdateValue)
		{
		*pvdateValue = pvdateValue__Temp;
		CA_VariantSetEmpty (&pvdateValue__Temp);
		}

Error:
	CA_VariantClear (&pvdateValue__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvdateValue)
			CA_VariantClear (pvdateValue);
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSet_Value (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VARIANT pvdateValue)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Set_Value_ (__vtblIFacePtr,
	                                                pvdateValue));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerGetYear (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            VARIANT *pvYear)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VARIANT pvYear__Temp;

	if (pvYear)
		CA_VariantSetEmpty (pvYear);
	CA_VariantSetEmpty (&pvYear__Temp);

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetYear_ (__vtblIFacePtr,
	                                              &pvYear__Temp));

	if (pvYear)
		{
		*pvYear = pvYear__Temp;
		CA_VariantSetEmpty (&pvYear__Temp);
		}

Error:
	CA_VariantClear (&pvYear__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvYear)
			CA_VariantClear (pvYear);
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerSetYear (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, VARIANT pvYear)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetYear_ (__vtblIFacePtr, pvYear));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerAboutBox (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->AboutBox_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerOLEDrag (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->OLEDrag_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IDTPickerRefresh (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IDTPicker_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IDTPicker, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Refresh_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IDTPicker, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_NewIFlatSB (int panel, const char *label, int top,
                                      int left, int *controlID, int *UILError)
{
	HRESULT __result = S_OK;
	int ctrlId;
	GUID clsid = {0xFE38753A, 0x44A3, 0x11D1, 0xB5, 0xB7, 0x0, 0x0, 0xC0, 0x90,
	              0x0, 0xC4};
	const char * licStr = "651A8940-87C5-11d1-8BE3-0000F8754DA1";

	ctrlId = NewActiveXCtrl (panel, label, top, left, &clsid,
	                         &MSComCtl2_IID_IFlatSB, licStr, &__result);

	__ActiveXCtrlErrorHandler();

	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBGetAppearance (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                enum MSComCtl2Enum_FlatScrollBarAppearanceConstants *penumAppearance)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long penumAppearance__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetAppearance_ (__vtblIFacePtr,
	                                                    (long *)&penumAppearance__Temp));

	if (penumAppearance)
		{
		*penumAppearance = penumAppearance__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBSetAppearance (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                enum MSComCtl2Enum_FlatScrollBarAppearanceConstants penumAppearance)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetAppearance_ (__vtblIFacePtr,
	                                                    penumAppearance));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBGetArrows (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            enum MSComCtl2Enum_ArrowsConstants *penumArrows)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long penumArrows__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetArrows_ (__vtblIFacePtr,
	                                                (long *)&penumArrows__Temp));

	if (penumArrows)
		{
		*penumArrows = penumArrows__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBSetArrows (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            enum MSComCtl2Enum_ArrowsConstants penumArrows)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetArrows_ (__vtblIFacePtr,
	                                                penumArrows));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBGetEnabled (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VBOOL *pbEnabled)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbEnabled__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetEnabled_ (__vtblIFacePtr,
	                                                 &pbEnabled__Temp));

	if (pbEnabled)
		{
		*pbEnabled = pbEnabled__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBSetEnabled (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VBOOL pbEnabled)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetEnabled_ (__vtblIFacePtr, pbEnabled));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBGethWnd (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          MSComCtl2Type_OLE_HANDLE *phWnd)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long phWnd__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GethWnd_ (__vtblIFacePtr, &phWnd__Temp));

	if (phWnd)
		{
		*phWnd = phWnd__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBSethWnd (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          MSComCtl2Type_OLE_HANDLE phWnd)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SethWnd_ (__vtblIFacePtr, phWnd));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBGetLargeChange (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 short *psLargeChange)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	short psLargeChange__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetLargeChange_ (__vtblIFacePtr,
	                                                     &psLargeChange__Temp));

	if (psLargeChange)
		{
		*psLargeChange = psLargeChange__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBSetLargeChange (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 short psLargeChange)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetLargeChange_ (__vtblIFacePtr,
	                                                     psLargeChange));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBGetMin (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, short *psMin)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	short psMin__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMin_ (__vtblIFacePtr, &psMin__Temp));

	if (psMin)
		{
		*psMin = psMin__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBSetMin (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, short psMin)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMin_ (__vtblIFacePtr, psMin));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBGetMax (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, short *psMax)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	short psMax__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMax_ (__vtblIFacePtr, &psMax__Temp));

	if (psMax)
		{
		*psMax = psMax__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBSetMax (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, short psMax)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMax_ (__vtblIFacePtr, psMax));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBGetMouseIcon (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               MSComCtl2Obj_IPictureDisp *ppMouseIcon)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppMouseIcon__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (ppMouseIcon)
		*ppMouseIcon = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMouseIcon_ (__vtblIFacePtr,
	                                                   &ppMouseIcon__AutoType));
	

	if (ppMouseIcon)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (ppMouseIcon__AutoType,
		                                             &MSComCtl2_IID_Picture,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, ppMouseIcon));
		ppMouseIcon__AutoType = 0;
		}

Error:
	if (ppMouseIcon__AutoType)
		ppMouseIcon__AutoType->lpVtbl->Release (ppMouseIcon__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppMouseIcon)
			{
			CA_DiscardObjHandle (*ppMouseIcon);
			*ppMouseIcon = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBSetMouseIcon (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               MSComCtl2Obj_IPictureDisp ppMouseIcon)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppMouseIcon__AutoType = 0;

	if (ppMouseIcon)
		{
		__caErrChk (CA_GetInterfaceFromObjHandle (ppMouseIcon,
	                                          &MSComCtl2_IID_Picture, 1,
	                                          &ppMouseIcon__AutoType, NULL));
		}

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMouseIcon_ (__vtblIFacePtr,
	                                                   ppMouseIcon__AutoType));

Error:
	if (ppMouseIcon__AutoType)
		ppMouseIcon__AutoType->lpVtbl->Release (ppMouseIcon__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBSetByRefMouseIcon (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSComCtl2Obj_IPictureDisp ppMouseIcon)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppMouseIcon__AutoType = 0;

	if (ppMouseIcon)
		{
		__caErrChk (CA_GetInterfaceFromObjHandle (ppMouseIcon,
	                                          &MSComCtl2_IID_Picture, 1,
	                                          &ppMouseIcon__AutoType, NULL));
		}

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetByRefMouseIcon_ (__vtblIFacePtr,
	                                                        ppMouseIcon__AutoType));

Error:
	if (ppMouseIcon__AutoType)
		ppMouseIcon__AutoType->lpVtbl->Release (ppMouseIcon__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBGetMousePointer (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  enum MSComCtl2Enum_MousePointerConstants *psMousePointer)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long psMousePointer__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMousePointer_ (__vtblIFacePtr,
	                                                      (long *)&psMousePointer__Temp));

	if (psMousePointer)
		{
		*psMousePointer = psMousePointer__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBSetMousePointer (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  enum MSComCtl2Enum_MousePointerConstants psMousePointer)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMousePointer_ (__vtblIFacePtr,
	                                                      psMousePointer));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBGetOrientation (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 enum MSComCtl2Enum_OrientationConstants *penumOrientation)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long penumOrientation__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetOrientation_ (__vtblIFacePtr,
	                                                     (long *)&penumOrientation__Temp));

	if (penumOrientation)
		{
		*penumOrientation = penumOrientation__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBSetOrientation (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 enum MSComCtl2Enum_OrientationConstants penumOrientation)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetOrientation_ (__vtblIFacePtr,
	                                                     penumOrientation));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBGetSmallChange (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 short *psSmallChange)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	short psSmallChange__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetSmallChange_ (__vtblIFacePtr,
	                                                     &psSmallChange__Temp));

	if (psSmallChange)
		{
		*psSmallChange = psSmallChange__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBSetSmallChange (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 short psSmallChange)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetSmallChange_ (__vtblIFacePtr,
	                                                     psSmallChange));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBGetValue (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, short *psValue)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	short psValue__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetValue_ (__vtblIFacePtr,
	                                               &psValue__Temp));

	if (psValue)
		{
		*psValue = psValue__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBSetValue (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, short psValue)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetValue_ (__vtblIFacePtr, psValue));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBGet_Value (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, short *psValue)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	short psValue__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Get_Value_ (__vtblIFacePtr,
	                                                &psValue__Temp));

	if (psValue)
		{
		*psValue = psValue__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBSet_Value (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, short psValue)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Set_Value_ (__vtblIFacePtr, psValue));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBAboutBox (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->AboutBox_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC MSComCtl2_IFlatSBRefresh (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	MSComCtl2_IFlatSB_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &MSComCtl2_IID_IFlatSB, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Refresh_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &MSComCtl2_IID_IFlatSB, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

static void CVIFUNC DAnimationEventsRegOnClick_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DAnimationEventsRegOnClick_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 0, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DAnimationEventsRegOnDblClick_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DAnimationEventsRegOnDblClick_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 1, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DAnimationEventsRegOnMouseDown_EventVTableFunc (void *thisPtr,
                                                                    short button,
                                                                    short shift,
                                                                    long x,
                                                                    long y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DAnimationEventsRegOnMouseDown_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 2, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, button, shift,
		                               x, y);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DAnimationEventsRegOnMouseMove_EventVTableFunc (void *thisPtr,
                                                                    short button,
                                                                    short shift,
                                                                    long x,
                                                                    long y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DAnimationEventsRegOnMouseMove_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 3, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, button, shift,
		                               x, y);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DAnimationEventsRegOnMouseUp_EventVTableFunc (void *thisPtr,
                                                                  short button,
                                                                  short shift,
                                                                  long x, long y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DAnimationEventsRegOnMouseUp_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 4, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, button, shift,
		                               x, y);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DAnimationEventsRegOnOLEStartDrag_EventVTableFunc (void *thisPtr,
                                                                       LPDISPATCH *data,
                                                                       long *allowedEffects)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DAnimationEventsRegOnOLEStartDrag_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 5, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               allowedEffects);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DAnimationEventsRegOnOLEGiveFeedback_EventVTableFunc (void *thisPtr,
                                                                          long *effect,
                                                                          VBOOL *defaultCursors)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DAnimationEventsRegOnOLEGiveFeedback_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 6, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, effect,
		                               defaultCursors);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DAnimationEventsRegOnOLESetData_EventVTableFunc (void *thisPtr,
                                                                     LPDISPATCH *data,
                                                                     short *dataFormat)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DAnimationEventsRegOnOLESetData_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 7, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               dataFormat);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DAnimationEventsRegOnOLECompleteDrag_EventVTableFunc (void *thisPtr,
                                                                          long *effect)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DAnimationEventsRegOnOLECompleteDrag_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 8, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, effect);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DAnimationEventsRegOnOLEDragOver_EventVTableFunc (void *thisPtr,
                                                                      LPDISPATCH *data,
                                                                      long *effect,
                                                                      short *button,
                                                                      short *shift,
                                                                      float *x,
                                                                      float *y,
                                                                      short *state)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DAnimationEventsRegOnOLEDragOver_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 9, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               effect, button, shift, x, y, state);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DAnimationEventsRegOnOLEDragDrop_EventVTableFunc (void *thisPtr,
                                                                      LPDISPATCH *data,
                                                                      long *effect,
                                                                      short *button,
                                                                      short *shift,
                                                                      float *x,
                                                                      float *y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DAnimationEventsRegOnOLEDragDrop_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 10, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               effect, button, shift, x, y);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DUpDownEventsRegOnChange_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DUpDownEventsRegOnChange_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 0, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DUpDownEventsRegOnDownClick_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DUpDownEventsRegOnDownClick_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 1, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DUpDownEventsRegOnUpClick_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DUpDownEventsRegOnUpClick_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 2, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DUpDownEventsRegOnMouseDown_EventVTableFunc (void *thisPtr,
                                                                 short button,
                                                                 short shift,
                                                                 long x, long y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DUpDownEventsRegOnMouseDown_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 3, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, button, shift,
		                               x, y);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DUpDownEventsRegOnMouseMove_EventVTableFunc (void *thisPtr,
                                                                 short button,
                                                                 short shift,
                                                                 long x, long y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DUpDownEventsRegOnMouseMove_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 4, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, button, shift,
		                               x, y);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DUpDownEventsRegOnMouseUp_EventVTableFunc (void *thisPtr,
                                                               short button,
                                                               short shift,
                                                               long x, long y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DUpDownEventsRegOnMouseUp_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 5, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, button, shift,
		                               x, y);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DUpDownEventsRegOnOLEStartDrag_EventVTableFunc (void *thisPtr,
                                                                    LPDISPATCH *data,
                                                                    long *allowedEffects)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DUpDownEventsRegOnOLEStartDrag_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 6, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               allowedEffects);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DUpDownEventsRegOnOLEGiveFeedback_EventVTableFunc (void *thisPtr,
                                                                       long *effect,
                                                                       VBOOL *defaultCursors)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DUpDownEventsRegOnOLEGiveFeedback_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 7, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, effect,
		                               defaultCursors);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DUpDownEventsRegOnOLESetData_EventVTableFunc (void *thisPtr,
                                                                  LPDISPATCH *data,
                                                                  short *dataFormat)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DUpDownEventsRegOnOLESetData_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 8, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               dataFormat);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DUpDownEventsRegOnOLECompleteDrag_EventVTableFunc (void *thisPtr,
                                                                       long *effect)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DUpDownEventsRegOnOLECompleteDrag_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 9, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, effect);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DUpDownEventsRegOnOLEDragOver_EventVTableFunc (void *thisPtr,
                                                                   LPDISPATCH *data,
                                                                   long *effect,
                                                                   short *button,
                                                                   short *shift,
                                                                   float *x,
                                                                   float *y,
                                                                   short *state)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DUpDownEventsRegOnOLEDragOver_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 10, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               effect, button, shift, x, y, state);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DUpDownEventsRegOnOLEDragDrop_EventVTableFunc (void *thisPtr,
                                                                   LPDISPATCH *data,
                                                                   long *effect,
                                                                   short *button,
                                                                   short *shift,
                                                                   float *x,
                                                                   float *y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DUpDownEventsRegOnOLEDragDrop_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 11, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               effect, button, shift, x, y);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DMonthViewEventsRegOnDateClick_EventVTableFunc (void *thisPtr,
                                                                    DATE dateClicked)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnDateClick_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 0, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, dateClicked);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DMonthViewEventsRegOnDateDblClick_EventVTableFunc (void *thisPtr,
                                                                       DATE dateDblClicked)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnDateDblClick_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 1, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, dateDblClicked);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DMonthViewEventsRegOnGetDayBold_EventVTableFunc (void *thisPtr,
                                                                     DATE startDate,
                                                                     short count,
                                                                     SAFEARRAY **state)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnGetDayBold_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 2, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, startDate,
		                               count, state);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DMonthViewEventsRegOnSelChange_EventVTableFunc (void *thisPtr,
                                                                    DATE startDate,
                                                                    DATE endDate,
                                                                    VBOOL *cancel)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnSelChange_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 3, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, startDate,
		                               endDate, cancel);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DMonthViewEventsRegOnClick_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnClick_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 4, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DMonthViewEventsRegOnDblClick_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnDblClick_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 5, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DMonthViewEventsRegOnKeyDown_EventVTableFunc (void *thisPtr,
                                                                  short *keyCode,
                                                                  short shift)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnKeyDown_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 6, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyCode, shift);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DMonthViewEventsRegOnKeyUp_EventVTableFunc (void *thisPtr,
                                                                short *keyCode,
                                                                short shift)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnKeyUp_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 7, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyCode, shift);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DMonthViewEventsRegOnKeyPress_EventVTableFunc (void *thisPtr,
                                                                   short *keyAscii)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnKeyPress_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 8, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyAscii);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DMonthViewEventsRegOnMouseDown_EventVTableFunc (void *thisPtr,
                                                                    short button,
                                                                    short shift,
                                                                    long x,
                                                                    long y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnMouseDown_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 9, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, button, shift,
		                               x, y);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DMonthViewEventsRegOnMouseMove_EventVTableFunc (void *thisPtr,
                                                                    short button,
                                                                    short shift,
                                                                    long x,
                                                                    long y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnMouseMove_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 10, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, button, shift,
		                               x, y);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DMonthViewEventsRegOnMouseUp_EventVTableFunc (void *thisPtr,
                                                                  short button,
                                                                  short shift,
                                                                  long x, long y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnMouseUp_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 11, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, button, shift,
		                               x, y);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DMonthViewEventsRegOnOLEStartDrag_EventVTableFunc (void *thisPtr,
                                                                       LPDISPATCH *data,
                                                                       long *allowedEffects)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnOLEStartDrag_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 12, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               allowedEffects);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DMonthViewEventsRegOnOLEGiveFeedback_EventVTableFunc (void *thisPtr,
                                                                          long *effect,
                                                                          VBOOL *defaultCursors)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnOLEGiveFeedback_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 13, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, effect,
		                               defaultCursors);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DMonthViewEventsRegOnOLESetData_EventVTableFunc (void *thisPtr,
                                                                     LPDISPATCH *data,
                                                                     short *dataFormat)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnOLESetData_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 14, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               dataFormat);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DMonthViewEventsRegOnOLECompleteDrag_EventVTableFunc (void *thisPtr,
                                                                          long *effect)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnOLECompleteDrag_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 15, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, effect);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DMonthViewEventsRegOnOLEDragOver_EventVTableFunc (void *thisPtr,
                                                                      LPDISPATCH *data,
                                                                      long *effect,
                                                                      short *button,
                                                                      short *shift,
                                                                      float *x,
                                                                      float *y,
                                                                      short *state)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnOLEDragOver_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 16, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               effect, button, shift, x, y, state);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DMonthViewEventsRegOnOLEDragDrop_EventVTableFunc (void *thisPtr,
                                                                      LPDISPATCH *data,
                                                                      long *effect,
                                                                      short *button,
                                                                      short *shift,
                                                                      float *x,
                                                                      float *y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DMonthViewEventsRegOnOLEDragDrop_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 17, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               effect, button, shift, x, y);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DDTPickerEventsRegOnCallbackKeyDown_EventVTableFunc (void *thisPtr,
                                                                         short keyCode,
                                                                         short shift,
                                                                         BSTR callbackField,
                                                                         DATE *callbackDate)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnCallbackKeyDown_CallbackType __callbackFunction;
	char * callbackField__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 0, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (callbackField, &callbackField__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyCode, shift,
		                               callbackField__CType, callbackDate);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (callbackField__CType);

	return;
}

static void CVIFUNC DDTPickerEventsRegOnChange_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnChange_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 1, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DDTPickerEventsRegOnCloseUp_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnCloseUp_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 2, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DDTPickerEventsRegOnDropDown_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnDropDown_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 3, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DDTPickerEventsRegOnFormat_EventVTableFunc (void *thisPtr,
                                                                BSTR callbackField,
                                                                BSTR *formattedString)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnFormat_CallbackType __callbackFunction;
	char * callbackField__CType = 0;
	char * formattedString__CType = 0;
	BSTR formattedString__AutoType = 0;
	

	*formattedString = 0;

	__caErrChk (CA_GetEventCallback (thisPtr, 4, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (callbackField, &callbackField__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData,
		                               callbackField__CType, &formattedString__CType);
	
		__caErrChk (__result);
	
		__caErrChk (CA_CStringToBSTR (formattedString__CType,
		                              &formattedString__AutoType));
		*formattedString = formattedString__AutoType;
		formattedString__AutoType = 0;
		}
Error:
	CA_FreeMemory (callbackField__CType);
	CA_FreeMemory (formattedString__CType);
	CA_FreeBSTR (formattedString__AutoType);

	return;
}

static void CVIFUNC DDTPickerEventsRegOnFormatSize_EventVTableFunc (void *thisPtr,
                                                                    BSTR callbackField,
                                                                    short *size)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnFormatSize_CallbackType __callbackFunction;
	char * callbackField__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 5, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (callbackField, &callbackField__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData,
		                               callbackField__CType, size);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (callbackField__CType);

	return;
}

static void CVIFUNC DDTPickerEventsRegOnClick_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnClick_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 6, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DDTPickerEventsRegOnDblClick_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnDblClick_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 7, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DDTPickerEventsRegOnKeyDown_EventVTableFunc (void *thisPtr,
                                                                 short *keyCode,
                                                                 short shift)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnKeyDown_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 8, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyCode, shift);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DDTPickerEventsRegOnKeyUp_EventVTableFunc (void *thisPtr,
                                                               short *keyCode,
                                                               short shift)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnKeyUp_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 9, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyCode, shift);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DDTPickerEventsRegOnKeyPress_EventVTableFunc (void *thisPtr,
                                                                  short *keyAscii)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnKeyPress_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 10, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, keyAscii);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DDTPickerEventsRegOnMouseDown_EventVTableFunc (void *thisPtr,
                                                                   short button,
                                                                   short shift,
                                                                   long x,
                                                                   long y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnMouseDown_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 11, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, button, shift,
		                               x, y);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DDTPickerEventsRegOnMouseMove_EventVTableFunc (void *thisPtr,
                                                                   short button,
                                                                   short shift,
                                                                   long x,
                                                                   long y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnMouseMove_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 12, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, button, shift,
		                               x, y);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DDTPickerEventsRegOnMouseUp_EventVTableFunc (void *thisPtr,
                                                                 short button,
                                                                 short shift,
                                                                 long x, long y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnMouseUp_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 13, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, button, shift,
		                               x, y);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DDTPickerEventsRegOnOLEStartDrag_EventVTableFunc (void *thisPtr,
                                                                      LPDISPATCH *data,
                                                                      long *allowedEffects)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnOLEStartDrag_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 14, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               allowedEffects);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DDTPickerEventsRegOnOLEGiveFeedback_EventVTableFunc (void *thisPtr,
                                                                         long *effect,
                                                                         VBOOL *defaultCursors)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnOLEGiveFeedback_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 15, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, effect,
		                               defaultCursors);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DDTPickerEventsRegOnOLESetData_EventVTableFunc (void *thisPtr,
                                                                    LPDISPATCH *data,
                                                                    short *dataFormat)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnOLESetData_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 16, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               dataFormat);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DDTPickerEventsRegOnOLECompleteDrag_EventVTableFunc (void *thisPtr,
                                                                         long *effect)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnOLECompleteDrag_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 17, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, effect);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DDTPickerEventsRegOnOLEDragOver_EventVTableFunc (void *thisPtr,
                                                                     LPDISPATCH *data,
                                                                     long *effect,
                                                                     short *button,
                                                                     short *shift,
                                                                     float *x,
                                                                     float *y,
                                                                     short *state)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnOLEDragOver_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 18, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               effect, button, shift, x, y, state);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DDTPickerEventsRegOnOLEDragDrop_EventVTableFunc (void *thisPtr,
                                                                     LPDISPATCH *data,
                                                                     long *effect,
                                                                     short *button,
                                                                     short *shift,
                                                                     float *x,
                                                                     float *y)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DDTPickerEventsRegOnOLEDragDrop_CallbackType __callbackFunction;
	CAObjHandle data__CType = 0;
	LPDISPATCH data__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 19, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*data, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &data__CType));
		*data = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &data__CType,
		                               effect, button, shift, x, y);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (data__CType, &IID_IDispatch, 1,
		                                          &data__AutoType, NULL));
		*data = data__AutoType;
		data__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (data__CType);
	if (data__AutoType)
		data__AutoType->lpVtbl->Release (data__AutoType);

	return;
}

static void CVIFUNC DFlatSBEventsRegOnChange_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DFlatSBEventsRegOnChange_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 0, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DFlatSBEventsRegOnScroll_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DFlatSBEventsRegOnScroll_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 1, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnClick (CAObjHandle serverObject,
                                                      DAnimationEventsRegOnClick_CallbackType callbackFunction,
                                                      void *callbackData,
                                                      int enableCallbacks,
                                                      int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DAnimationEvents_CAEventClassDefn,
	                                     0, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnDblClick (CAObjHandle serverObject,
                                                         DAnimationEventsRegOnDblClick_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DAnimationEvents_CAEventClassDefn,
	                                     1, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnMouseDown (CAObjHandle serverObject,
                                                          DAnimationEventsRegOnMouseDown_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DAnimationEvents_CAEventClassDefn,
	                                     2, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnMouseMove (CAObjHandle serverObject,
                                                          DAnimationEventsRegOnMouseMove_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DAnimationEvents_CAEventClassDefn,
	                                     3, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnMouseUp (CAObjHandle serverObject,
                                                        DAnimationEventsRegOnMouseUp_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DAnimationEvents_CAEventClassDefn,
	                                     4, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnOLEStartDrag (CAObjHandle serverObject,
                                                             DAnimationEventsRegOnOLEStartDrag_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DAnimationEvents_CAEventClassDefn,
	                                     5, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnOLEGiveFeedback (CAObjHandle serverObject,
                                                                DAnimationEventsRegOnOLEGiveFeedback_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DAnimationEvents_CAEventClassDefn,
	                                     6, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnOLESetData (CAObjHandle serverObject,
                                                           DAnimationEventsRegOnOLESetData_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DAnimationEvents_CAEventClassDefn,
	                                     7, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnOLECompleteDrag (CAObjHandle serverObject,
                                                                DAnimationEventsRegOnOLECompleteDrag_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DAnimationEvents_CAEventClassDefn,
	                                     8, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnOLEDragOver (CAObjHandle serverObject,
                                                            DAnimationEventsRegOnOLEDragOver_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DAnimationEvents_CAEventClassDefn,
	                                     9, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnOLEDragDrop (CAObjHandle serverObject,
                                                            DAnimationEventsRegOnOLEDragDrop_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DAnimationEvents_CAEventClassDefn,
	                                     10, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnChange (CAObjHandle serverObject,
                                                    DUpDownEventsRegOnChange_CallbackType callbackFunction,
                                                    void *callbackData,
                                                    int enableCallbacks,
                                                    int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DUpDownEvents_CAEventClassDefn, 0,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnDownClick (CAObjHandle serverObject,
                                                       DUpDownEventsRegOnDownClick_CallbackType callbackFunction,
                                                       void *callbackData,
                                                       int enableCallbacks,
                                                       int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DUpDownEvents_CAEventClassDefn, 1,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnUpClick (CAObjHandle serverObject,
                                                     DUpDownEventsRegOnUpClick_CallbackType callbackFunction,
                                                     void *callbackData,
                                                     int enableCallbacks,
                                                     int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DUpDownEvents_CAEventClassDefn, 2,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnMouseDown (CAObjHandle serverObject,
                                                       DUpDownEventsRegOnMouseDown_CallbackType callbackFunction,
                                                       void *callbackData,
                                                       int enableCallbacks,
                                                       int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DUpDownEvents_CAEventClassDefn, 3,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnMouseMove (CAObjHandle serverObject,
                                                       DUpDownEventsRegOnMouseMove_CallbackType callbackFunction,
                                                       void *callbackData,
                                                       int enableCallbacks,
                                                       int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DUpDownEvents_CAEventClassDefn, 4,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnMouseUp (CAObjHandle serverObject,
                                                     DUpDownEventsRegOnMouseUp_CallbackType callbackFunction,
                                                     void *callbackData,
                                                     int enableCallbacks,
                                                     int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DUpDownEvents_CAEventClassDefn, 5,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnOLEStartDrag (CAObjHandle serverObject,
                                                          DUpDownEventsRegOnOLEStartDrag_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DUpDownEvents_CAEventClassDefn, 6,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnOLEGiveFeedback (CAObjHandle serverObject,
                                                             DUpDownEventsRegOnOLEGiveFeedback_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DUpDownEvents_CAEventClassDefn, 7,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnOLESetData (CAObjHandle serverObject,
                                                        DUpDownEventsRegOnOLESetData_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DUpDownEvents_CAEventClassDefn, 8,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnOLECompleteDrag (CAObjHandle serverObject,
                                                             DUpDownEventsRegOnOLECompleteDrag_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DUpDownEvents_CAEventClassDefn, 9,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnOLEDragOver (CAObjHandle serverObject,
                                                         DUpDownEventsRegOnOLEDragOver_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DUpDownEvents_CAEventClassDefn, 10,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnOLEDragDrop (CAObjHandle serverObject,
                                                         DUpDownEventsRegOnOLEDragDrop_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DUpDownEvents_CAEventClassDefn, 11,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnDateClick (CAObjHandle serverObject,
                                                          DMonthViewEventsRegOnDateClick_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     0, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnDateDblClick (CAObjHandle serverObject,
                                                             DMonthViewEventsRegOnDateDblClick_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     1, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnGetDayBold (CAObjHandle serverObject,
                                                           DMonthViewEventsRegOnGetDayBold_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     2, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnSelChange (CAObjHandle serverObject,
                                                          DMonthViewEventsRegOnSelChange_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     3, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnClick (CAObjHandle serverObject,
                                                      DMonthViewEventsRegOnClick_CallbackType callbackFunction,
                                                      void *callbackData,
                                                      int enableCallbacks,
                                                      int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     4, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnDblClick (CAObjHandle serverObject,
                                                         DMonthViewEventsRegOnDblClick_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     5, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnKeyDown (CAObjHandle serverObject,
                                                        DMonthViewEventsRegOnKeyDown_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     6, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnKeyUp (CAObjHandle serverObject,
                                                      DMonthViewEventsRegOnKeyUp_CallbackType callbackFunction,
                                                      void *callbackData,
                                                      int enableCallbacks,
                                                      int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     7, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnKeyPress (CAObjHandle serverObject,
                                                         DMonthViewEventsRegOnKeyPress_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     8, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnMouseDown (CAObjHandle serverObject,
                                                          DMonthViewEventsRegOnMouseDown_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     9, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnMouseMove (CAObjHandle serverObject,
                                                          DMonthViewEventsRegOnMouseMove_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     10, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnMouseUp (CAObjHandle serverObject,
                                                        DMonthViewEventsRegOnMouseUp_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     11, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnOLEStartDrag (CAObjHandle serverObject,
                                                             DMonthViewEventsRegOnOLEStartDrag_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     12, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnOLEGiveFeedback (CAObjHandle serverObject,
                                                                DMonthViewEventsRegOnOLEGiveFeedback_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     13, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnOLESetData (CAObjHandle serverObject,
                                                           DMonthViewEventsRegOnOLESetData_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     14, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnOLECompleteDrag (CAObjHandle serverObject,
                                                                DMonthViewEventsRegOnOLECompleteDrag_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     15, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnOLEDragOver (CAObjHandle serverObject,
                                                            DMonthViewEventsRegOnOLEDragOver_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     16, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnOLEDragDrop (CAObjHandle serverObject,
                                                            DMonthViewEventsRegOnOLEDragDrop_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DMonthViewEvents_CAEventClassDefn,
	                                     17, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnCallbackKeyDown (CAObjHandle serverObject,
                                                               DDTPickerEventsRegOnCallbackKeyDown_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn, 0,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnChange (CAObjHandle serverObject,
                                                      DDTPickerEventsRegOnChange_CallbackType callbackFunction,
                                                      void *callbackData,
                                                      int enableCallbacks,
                                                      int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn, 1,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnCloseUp (CAObjHandle serverObject,
                                                       DDTPickerEventsRegOnCloseUp_CallbackType callbackFunction,
                                                       void *callbackData,
                                                       int enableCallbacks,
                                                       int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn, 2,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnDropDown (CAObjHandle serverObject,
                                                        DDTPickerEventsRegOnDropDown_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn, 3,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnFormat (CAObjHandle serverObject,
                                                      DDTPickerEventsRegOnFormat_CallbackType callbackFunction,
                                                      void *callbackData,
                                                      int enableCallbacks,
                                                      int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn, 4,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnFormatSize (CAObjHandle serverObject,
                                                          DDTPickerEventsRegOnFormatSize_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn, 5,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnClick (CAObjHandle serverObject,
                                                     DDTPickerEventsRegOnClick_CallbackType callbackFunction,
                                                     void *callbackData,
                                                     int enableCallbacks,
                                                     int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn, 6,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnDblClick (CAObjHandle serverObject,
                                                        DDTPickerEventsRegOnDblClick_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn, 7,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnKeyDown (CAObjHandle serverObject,
                                                       DDTPickerEventsRegOnKeyDown_CallbackType callbackFunction,
                                                       void *callbackData,
                                                       int enableCallbacks,
                                                       int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn, 8,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnKeyUp (CAObjHandle serverObject,
                                                     DDTPickerEventsRegOnKeyUp_CallbackType callbackFunction,
                                                     void *callbackData,
                                                     int enableCallbacks,
                                                     int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn, 9,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnKeyPress (CAObjHandle serverObject,
                                                        DDTPickerEventsRegOnKeyPress_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn,
	                                     10, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnMouseDown (CAObjHandle serverObject,
                                                         DDTPickerEventsRegOnMouseDown_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn,
	                                     11, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnMouseMove (CAObjHandle serverObject,
                                                         DDTPickerEventsRegOnMouseMove_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn,
	                                     12, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnMouseUp (CAObjHandle serverObject,
                                                       DDTPickerEventsRegOnMouseUp_CallbackType callbackFunction,
                                                       void *callbackData,
                                                       int enableCallbacks,
                                                       int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn,
	                                     13, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnOLEStartDrag (CAObjHandle serverObject,
                                                            DDTPickerEventsRegOnOLEStartDrag_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn,
	                                     14, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnOLEGiveFeedback (CAObjHandle serverObject,
                                                               DDTPickerEventsRegOnOLEGiveFeedback_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn,
	                                     15, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnOLESetData (CAObjHandle serverObject,
                                                          DDTPickerEventsRegOnOLESetData_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn,
	                                     16, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnOLECompleteDrag (CAObjHandle serverObject,
                                                               DDTPickerEventsRegOnOLECompleteDrag_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn,
	                                     17, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnOLEDragOver (CAObjHandle serverObject,
                                                           DDTPickerEventsRegOnOLEDragOver_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn,
	                                     18, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnOLEDragDrop (CAObjHandle serverObject,
                                                           DDTPickerEventsRegOnOLEDragDrop_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DDTPickerEvents_CAEventClassDefn,
	                                     19, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DFlatSBEventsRegOnChange (CAObjHandle serverObject,
                                                    DFlatSBEventsRegOnChange_CallbackType callbackFunction,
                                                    void *callbackData,
                                                    int enableCallbacks,
                                                    int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DFlatSBEvents_CAEventClassDefn, 0,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC MSComCtl2_DFlatSBEventsRegOnScroll (CAObjHandle serverObject,
                                                    DFlatSBEventsRegOnScroll_CallbackType callbackFunction,
                                                    void *callbackData,
                                                    int enableCallbacks,
                                                    int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DFlatSBEvents_CAEventClassDefn, 1,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}
