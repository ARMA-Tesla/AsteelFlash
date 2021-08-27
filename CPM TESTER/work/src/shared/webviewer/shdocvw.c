#include "shdocvw.h"
#include <userint.h>

static void CVIFUNC DWebBrwsrEvnts2RegOnStatusTextChange_EventVTableFunc (void *thisPtr,
                                                                          BSTR text);

static void CVIFUNC DWebBrwsrEvnts2RegOnProgressChange_EventVTableFunc (void *thisPtr,
                                                                        long progress,
                                                                        long progressMax);

static void CVIFUNC DWebBrwsrEvnts2RegOnCommandStateChange_EventVTableFunc (void *thisPtr,
                                                                            long command,
                                                                            VBOOL enable);

static void CVIFUNC DWebBrwsrEvnts2RegOnDownloadBegin_EventVTableFunc (void *thisPtr);

static void CVIFUNC DWebBrwsrEvnts2RegOnDownloadComplete_EventVTableFunc (void *thisPtr);

static void CVIFUNC DWebBrwsrEvnts2RegOnTitleChange_EventVTableFunc (void *thisPtr,
                                                                     BSTR text);

static void CVIFUNC DWebBrwsrEvnts2RegOnPropertyChange_EventVTableFunc (void *thisPtr,
                                                                        BSTR szProperty);

static void CVIFUNC DWebBrwsrEvnts2RegOnBeforeNavigate2_EventVTableFunc (void *thisPtr,
                                                                         LPDISPATCH pDisp,
                                                                         VARIANT *URL,
                                                                         VARIANT *flags,
                                                                         VARIANT *targetFrameName,
                                                                         VARIANT *postData,
                                                                         VARIANT *headers,
                                                                         VBOOL *cancel);

static void CVIFUNC DWebBrwsrEvnts2RegOnNewWindow2_EventVTableFunc (void *thisPtr,
                                                                    LPDISPATCH *ppDisp,
                                                                    VBOOL *cancel);

static void CVIFUNC DWebBrwsrEvnts2RegOnNavigateComplete2_EventVTableFunc (void *thisPtr,
                                                                           LPDISPATCH pDisp,
                                                                           VARIANT *URL);

static void CVIFUNC DWebBrwsrEvnts2RegOnDocumentComplete_EventVTableFunc (void *thisPtr,
                                                                          LPDISPATCH pDisp,
                                                                          VARIANT *URL);

static void CVIFUNC DWebBrwsrEvnts2RegOnOnQuit_EventVTableFunc (void *thisPtr);

static void CVIFUNC DWebBrwsrEvnts2RegOnOnVisible_EventVTableFunc (void *thisPtr,
                                                                   VBOOL visible);

static void CVIFUNC DWebBrwsrEvnts2RegOnOnToolBar_EventVTableFunc (void *thisPtr,
                                                                   VBOOL toolBar);

static void CVIFUNC DWebBrwsrEvnts2RegOnOnMenuBar_EventVTableFunc (void *thisPtr,
                                                                   VBOOL menuBar);

static void CVIFUNC DWebBrwsrEvnts2RegOnOnStatusBar_EventVTableFunc (void *thisPtr,
                                                                     VBOOL statusBar);

static void CVIFUNC DWebBrwsrEvnts2RegOnOnFullScreen_EventVTableFunc (void *thisPtr,
                                                                      VBOOL fullScreen);

static void CVIFUNC DWebBrwsrEvnts2RegOnOnTheaterMode_EventVTableFunc (void *thisPtr,
                                                                       VBOOL theaterMode);

static void CVIFUNC DWebBrwsrEvnts2RegOnWindowSetResizable_EventVTableFunc (void *thisPtr,
                                                                            VBOOL resizable);

static void CVIFUNC DWebBrwsrEvnts2RegOnWindowSetLeft_EventVTableFunc (void *thisPtr,
                                                                       long left);

static void CVIFUNC DWebBrwsrEvnts2RegOnWindowSetTop_EventVTableFunc (void *thisPtr,
                                                                      long top);

static void CVIFUNC DWebBrwsrEvnts2RegOnWindowSetWidth_EventVTableFunc (void *thisPtr,
                                                                        long width);

static void CVIFUNC DWebBrwsrEvnts2RegOnWindowSetHeight_EventVTableFunc (void *thisPtr,
                                                                         long height);

static void CVIFUNC DWebBrwsrEvnts2RegOnWindowClosing_EventVTableFunc (void *thisPtr,
                                                                       VBOOL isChildWindow,
                                                                       VBOOL *cancel);

static void CVIFUNC DWebBrwsrEvnts2RegOnClientToHostWindow_EventVTableFunc (void *thisPtr,
                                                                            long *CX,
                                                                            long *CY);

static void CVIFUNC DWebBrwsrEvnts2RegOnSetSecureLockIcon_EventVTableFunc (void *thisPtr,
                                                                           long secureLockIcon);

static void CVIFUNC DWebBrwsrEvnts2RegOnFileDownload_EventVTableFunc (void *thisPtr,
                                                                      VBOOL *cancel);

static void CVIFUNC DWebBrwsrEvnts2RegOnNavigateError_EventVTableFunc (void *thisPtr,
                                                                       LPDISPATCH pDisp,
                                                                       VARIANT *URL,
                                                                       VARIANT *frame,
                                                                       VARIANT *statusCode,
                                                                       VBOOL *cancel);

static void CVIFUNC DWebBrwsrEvnts2RegOnPrintTemplateInstantiation_EventVTableFunc (void *thisPtr,
                                                                                    LPDISPATCH pDisp);

static void CVIFUNC DWebBrwsrEvnts2RegOnPrintTemplateTeardown_EventVTableFunc (void *thisPtr,
                                                                               LPDISPATCH pDisp);

static void CVIFUNC DWebBrwsrEvnts2RegOnUpdatePageStatus_EventVTableFunc (void *thisPtr,
                                                                          LPDISPATCH pDisp,
                                                                          VARIANT *nPage,
                                                                          VARIANT *fDone);

static void CVIFUNC DWebBrwsrEvnts2RegOnPrivacyImpactedStateChange_EventVTableFunc (void *thisPtr,
                                                                                    VBOOL bImpacted);

static void CVIFUNC DWebBrwsrEvnts2RegOnNewWindow3_EventVTableFunc (void *thisPtr,
                                                                    LPDISPATCH *ppDisp,
                                                                    VBOOL *cancel,
                                                                    long dwFlags,
                                                                    BSTR bstrUrlContext,
                                                                    BSTR bstrUrl);

static void CVIFUNC DWebBrwsrEvntsRegOnBeforeNavigate_EventVTableFunc (void *thisPtr,
                                                                       BSTR URL,
                                                                       long flags,
                                                                       BSTR targetFrameName,
                                                                       VARIANT *postData,
                                                                       BSTR headers,
                                                                       VBOOL *cancel);

static void CVIFUNC DWebBrwsrEvntsRegOnNavigateComplete_EventVTableFunc (void *thisPtr,
                                                                         BSTR URL);

static void CVIFUNC DWebBrwsrEvntsRegOnStatusTextChange_EventVTableFunc (void *thisPtr,
                                                                         BSTR text);

static void CVIFUNC DWebBrwsrEvntsRegOnProgressChange_EventVTableFunc (void *thisPtr,
                                                                       long progress,
                                                                       long progressMax);

static void CVIFUNC DWebBrwsrEvntsRegOnDownloadComplete_EventVTableFunc (void *thisPtr);

static void CVIFUNC DWebBrwsrEvntsRegOnCommandStateChange_EventVTableFunc (void *thisPtr,
                                                                           long command,
                                                                           VBOOL enable);

static void CVIFUNC DWebBrwsrEvntsRegOnDownloadBegin_EventVTableFunc (void *thisPtr);

static void CVIFUNC DWebBrwsrEvntsRegOnNewWindow_EventVTableFunc (void *thisPtr,
                                                                  BSTR URL,
                                                                  long flags,
                                                                  BSTR targetFrameName,
                                                                  VARIANT *postData,
                                                                  BSTR headers,
                                                                  VBOOL *processed);

static void CVIFUNC DWebBrwsrEvntsRegOnTitleChange_EventVTableFunc (void *thisPtr,
                                                                    BSTR text);

static void CVIFUNC DWebBrwsrEvntsRegOnFrameBeforeNavigate_EventVTableFunc (void *thisPtr,
                                                                            BSTR URL,
                                                                            long flags,
                                                                            BSTR targetFrameName,
                                                                            VARIANT *postData,
                                                                            BSTR headers,
                                                                            VBOOL *cancel);

static void CVIFUNC DWebBrwsrEvntsRegOnFrameNavigateComplete_EventVTableFunc (void *thisPtr,
                                                                              BSTR URL);

static void CVIFUNC DWebBrwsrEvntsRegOnFrameNewWindow_EventVTableFunc (void *thisPtr,
                                                                       BSTR URL,
                                                                       long flags,
                                                                       BSTR targetFrameName,
                                                                       VARIANT *postData,
                                                                       BSTR headers,
                                                                       VBOOL *processed);

static void CVIFUNC DWebBrwsrEvntsRegOnQuit_EventVTableFunc (void *thisPtr,
                                                             VBOOL *cancel);

static void CVIFUNC DWebBrwsrEvntsRegOnWindowMove_EventVTableFunc (void *thisPtr);

static void CVIFUNC DWebBrwsrEvntsRegOnWindowResize_EventVTableFunc (void *thisPtr);

static void CVIFUNC DWebBrwsrEvntsRegOnWindowActivate_EventVTableFunc (void *thisPtr);

static void CVIFUNC DWebBrwsrEvntsRegOnPropertyChange_EventVTableFunc (void *thisPtr,
                                                                       BSTR property);

static void CVIFUNC DShellWindowsEvntsRegOnWindowRegistered_EventVTableFunc (void *thisPtr,
                                                                             long lCookie);

static void CVIFUNC DShellWindowsEvntsRegOnWindowRevoked_EventVTableFunc (void *thisPtr,
                                                                          long lCookie);

static void CVIFUNC DShellNameSpaceEventsRegOnFavoritesSelectionChange_EventVTableFunc (void *thisPtr,
                                                                                        long cItems,
                                                                                        long hItem,
                                                                                        BSTR strName,
                                                                                        BSTR strUrl,
                                                                                        long cVisits,
                                                                                        BSTR strDate,
                                                                                        long fAvailableOffline);

static void CVIFUNC DShellNameSpaceEventsRegOnSelectionChange_EventVTableFunc (void *thisPtr);

static void CVIFUNC DShellNameSpaceEventsRegOnDoubleClick_EventVTableFunc (void *thisPtr);

static void CVIFUNC DShellNameSpaceEventsRegOnInitialized_EventVTableFunc (void *thisPtr);

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

typedef interface tagWebViewer_IWebBrowser_Interface WebViewer_IWebBrowser_Interface;

typedef struct tagWebViewer_IWebBrowser_VTable
{
	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( WebViewer_IWebBrowser_Interface __RPC_FAR * This, 
	                                                         REFIID riid, 
	                                                         void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( WebViewer_IWebBrowser_Interface __RPC_FAR * This);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( WebViewer_IWebBrowser_Interface __RPC_FAR * This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( WebViewer_IWebBrowser_Interface __RPC_FAR * This, 
	                                                           UINT __RPC_FAR *pctinfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( WebViewer_IWebBrowser_Interface __RPC_FAR * This, 
	                                                      UINT iTInfo, 
	                                                      LCID lcid, 
	                                                      ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( WebViewer_IWebBrowser_Interface __RPC_FAR * This, 
	                                                        REFIID riid, 
	                                                        LPOLESTR __RPC_FAR *rgszNames, 
	                                                        UINT cNames, 
	                                                        LCID lcid, 
	                                                        DISPID __RPC_FAR *rgDispId);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( WebViewer_IWebBrowser_Interface __RPC_FAR * This, 
	                                                 DISPID dispIdMember, 
	                                                 REFIID riid, 
	                                                 LCID lcid, 
	                                                 WORD wFlags, 
	                                                 DISPPARAMS __RPC_FAR *pDispParams, 
	                                                 VARIANT __RPC_FAR *pVarResult, 
	                                                 EXCEPINFO __RPC_FAR *pExcepInfo, 
	                                                 UINT __RPC_FAR *puArgErr);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GoBack_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GoForward_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GoHome_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GoSearch_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Navigate_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                   BSTR URL, 
	                                                   VARIANT *flags, 
	                                                   VARIANT *targetFrameName, 
	                                                   VARIANT *postData, 
	                                                   VARIANT *headers);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh2_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                   VARIANT *level);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetApplication_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                         LPDISPATCH *ppDisp);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetParent_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                    LPDISPATCH *ppDisp);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetContainer_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                       LPDISPATCH *ppDisp);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDocument_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                      LPDISPATCH *ppDisp);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTopLevelContainer_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                               VBOOL *pBool);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetType_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                  BSTR *type);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLeft_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                  long *pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLeft_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                  long pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTop_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                 long *pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTop_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                 long pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWidth_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                   long *pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetWidth_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                   long pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetHeight_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                    long *pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetHeight_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                    long pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLocationName_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                          BSTR *locationName);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLocationURL_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                         BSTR *locationURL);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBusy_) (WebViewer_IWebBrowser_Interface __RPC_FAR *This, 
	                                                  VBOOL *pBool);

} WebViewer_IWebBrowser_VTable;

typedef interface tagWebViewer_IWebBrowser_Interface
{
	CONST_VTBL WebViewer_IWebBrowser_VTable __RPC_FAR *lpVtbl;
} WebViewer_IWebBrowser_Interface;

typedef interface tagWebViewer_IWebBrowser2_Interface WebViewer_IWebBrowser2_Interface;

typedef struct tagWebViewer_IWebBrowser2_VTable
{
	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( WebViewer_IWebBrowser2_Interface __RPC_FAR * This, 
	                                                         REFIID riid, 
	                                                         void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( WebViewer_IWebBrowser2_Interface __RPC_FAR * This);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( WebViewer_IWebBrowser2_Interface __RPC_FAR * This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( WebViewer_IWebBrowser2_Interface __RPC_FAR * This, 
	                                                           UINT __RPC_FAR *pctinfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( WebViewer_IWebBrowser2_Interface __RPC_FAR * This, 
	                                                      UINT iTInfo, 
	                                                      LCID lcid, 
	                                                      ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( WebViewer_IWebBrowser2_Interface __RPC_FAR * This, 
	                                                        REFIID riid, 
	                                                        LPOLESTR __RPC_FAR *rgszNames, 
	                                                        UINT cNames, 
	                                                        LCID lcid, 
	                                                        DISPID __RPC_FAR *rgDispId);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( WebViewer_IWebBrowser2_Interface __RPC_FAR * This, 
	                                                 DISPID dispIdMember, 
	                                                 REFIID riid, 
	                                                 LCID lcid, 
	                                                 WORD wFlags, 
	                                                 DISPPARAMS __RPC_FAR *pDispParams, 
	                                                 VARIANT __RPC_FAR *pVarResult, 
	                                                 EXCEPINFO __RPC_FAR *pExcepInfo, 
	                                                 UINT __RPC_FAR *puArgErr);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GoBack_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GoForward_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GoHome_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GoSearch_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Navigate_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                   BSTR URL, 
	                                                   VARIANT *flags, 
	                                                   VARIANT *targetFrameName, 
	                                                   VARIANT *postData, 
	                                                   VARIANT *headers);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Refresh2_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                   VARIANT *level);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetApplication_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                         LPDISPATCH *ppDisp);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetParent_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                    LPDISPATCH *ppDisp);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetContainer_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                       LPDISPATCH *ppDisp);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDocument_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                      LPDISPATCH *ppDisp);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTopLevelContainer_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                               VBOOL *pBool);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetType_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                  BSTR *type);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLeft_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                  long *pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLeft_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                  long pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTop_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                 long *pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTop_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                 long pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWidth_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                   long *pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetWidth_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                   long pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetHeight_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                    long *pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetHeight_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                    long pl);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLocationName_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                          BSTR *locationName);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLocationURL_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                         BSTR *locationURL);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBusy_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                  VBOOL *pBool);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Quit_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClientToWindow_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                         long *pcx, 
	                                                         long *pcy);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PutProperty_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                      BSTR property, 
	                                                      VARIANT vtValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetProperty_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                      BSTR property, 
	                                                      VARIANT *pvtValue);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetName_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                  BSTR *name);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetHWND_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                  long *pHWND);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFullName_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                      BSTR *fullName);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPath_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                  BSTR *path);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVisible_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                     VBOOL *pBool);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetVisible_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                     VBOOL pBool);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStatusBar_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                       VBOOL *pBool);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetStatusBar_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                       VBOOL pBool);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStatusText_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                        BSTR *statusText);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetStatusText_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                        BSTR statusText);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetToolBar_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                     long *value);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetToolBar_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                     long value);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMenuBar_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                     VBOOL *value);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMenuBar_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                     VBOOL value);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFullScreen_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                        VBOOL *pbFullScreen);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFullScreen_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                        VBOOL pbFullScreen);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Navigate2_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                    VARIANT *URL, 
	                                                    VARIANT *flags, 
	                                                    VARIANT *targetFrameName, 
	                                                    VARIANT *postData, 
	                                                    VARIANT *headers);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryStatusWB_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                        long cmdID, 
	                                                        long *pcmdf);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ExecWB_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                 long cmdID, 
	                                                 long cmdexecopt, 
	                                                 VARIANT *pvaIn, 
	                                                 VARIANT *pvaOut);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowBrowserBar_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                         VARIANT *pvaClsid, 
	                                                         VARIANT *pvarShow, 
	                                                         VARIANT *pvarSize);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetReadyState_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                        long *plReadyState);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetOffline_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                     VBOOL *pbOffline);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetOffline_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                     VBOOL pbOffline);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSilent_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                    VBOOL *pbSilent);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSilent_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                    VBOOL pbSilent);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRegisterAsBrowser_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                               VBOOL *pbRegister);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetRegisterAsBrowser_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                               VBOOL pbRegister);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRegisterAsDropTarget_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                                  VBOOL *pbRegister);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetRegisterAsDropTarget_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                                  VBOOL pbRegister);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTheaterMode_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                         VBOOL *pbRegister);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTheaterMode_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                         VBOOL pbRegister);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAddressBar_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                        VBOOL *value);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAddressBar_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                        VBOOL value);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetResizable_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                       VBOOL *value);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetResizable_) (WebViewer_IWebBrowser2_Interface __RPC_FAR *This, 
	                                                       VBOOL value);

} WebViewer_IWebBrowser2_VTable;

typedef interface tagWebViewer_IWebBrowser2_Interface
{
	CONST_VTBL WebViewer_IWebBrowser2_VTable __RPC_FAR *lpVtbl;
} WebViewer_IWebBrowser2_Interface;

typedef interface tagWebViewer_IShellWindows_Interface WebViewer_IShellWindows_Interface;

typedef struct tagWebViewer_IShellWindows_VTable
{
	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( WebViewer_IShellWindows_Interface __RPC_FAR * This, 
	                                                         REFIID riid, 
	                                                         void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( WebViewer_IShellWindows_Interface __RPC_FAR * This);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( WebViewer_IShellWindows_Interface __RPC_FAR * This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( WebViewer_IShellWindows_Interface __RPC_FAR * This, 
	                                                           UINT __RPC_FAR *pctinfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( WebViewer_IShellWindows_Interface __RPC_FAR * This, 
	                                                      UINT iTInfo, 
	                                                      LCID lcid, 
	                                                      ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( WebViewer_IShellWindows_Interface __RPC_FAR * This, 
	                                                        REFIID riid, 
	                                                        LPOLESTR __RPC_FAR *rgszNames, 
	                                                        UINT cNames, 
	                                                        LCID lcid, 
	                                                        DISPID __RPC_FAR *rgDispId);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( WebViewer_IShellWindows_Interface __RPC_FAR * This, 
	                                                 DISPID dispIdMember, 
	                                                 REFIID riid, 
	                                                 LCID lcid, 
	                                                 WORD wFlags, 
	                                                 DISPPARAMS __RPC_FAR *pDispParams, 
	                                                 VARIANT __RPC_FAR *pVarResult, 
	                                                 EXCEPINFO __RPC_FAR *pExcepInfo, 
	                                                 UINT __RPC_FAR *puArgErr);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCount_) (WebViewer_IShellWindows_Interface __RPC_FAR *This, 
	                                                   long *count);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Item_) (WebViewer_IShellWindows_Interface __RPC_FAR *This, 
	                                               VARIANT index, 
	                                               LPDISPATCH *folder);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *_NewEnum_) (WebViewer_IShellWindows_Interface __RPC_FAR *This, 
	                                                   LPUNKNOWN *ppunk);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Register_) (WebViewer_IShellWindows_Interface __RPC_FAR *This, 
	                                                   LPDISPATCH pid, 
	                                                   long HWND, 
	                                                   long swClass, 
	                                                   long *plCookie);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterPending_) (WebViewer_IShellWindows_Interface __RPC_FAR *This, 
	                                                          long lThreadId, 
	                                                          VARIANT *pvarloc, 
	                                                          VARIANT *pvarlocRoot, 
	                                                          long swClass, 
	                                                          long *plCookie);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Revoke_) (WebViewer_IShellWindows_Interface __RPC_FAR *This, 
	                                                 long lCookie);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnNavigate_) (WebViewer_IShellWindows_Interface __RPC_FAR *This, 
	                                                     long lCookie, 
	                                                     VARIANT *pvarloc);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnActivated_) (WebViewer_IShellWindows_Interface __RPC_FAR *This, 
	                                                      long lCookie, 
	                                                      VBOOL fActive);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindWindowSW_) (WebViewer_IShellWindows_Interface __RPC_FAR *This, 
	                                                       VARIANT *pvarloc, 
	                                                       VARIANT *pvarlocRoot, 
	                                                       long swClass, 
	                                                       long *pHWND, 
	                                                       long swfwOptions, 
	                                                       LPDISPATCH *ppdispOut);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnCreated_) (WebViewer_IShellWindows_Interface __RPC_FAR *This, 
	                                                    long lCookie, 
	                                                    LPUNKNOWN punk);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessAttachDetch_) (WebViewer_IShellWindows_Interface __RPC_FAR *This, 
	                                                             VBOOL fAttach);

} WebViewer_IShellWindows_VTable;

typedef interface tagWebViewer_IShellWindows_Interface
{
	CONST_VTBL WebViewer_IShellWindows_VTable __RPC_FAR *lpVtbl;
} WebViewer_IShellWindows_Interface;

typedef interface tagWebViewer_IShellUIHelper_Interface WebViewer_IShellUIHelper_Interface;

typedef struct tagWebViewer_IShellUIHelper_VTable
{
	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( WebViewer_IShellUIHelper_Interface __RPC_FAR * This, 
	                                                         REFIID riid, 
	                                                         void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( WebViewer_IShellUIHelper_Interface __RPC_FAR * This);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( WebViewer_IShellUIHelper_Interface __RPC_FAR * This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( WebViewer_IShellUIHelper_Interface __RPC_FAR * This, 
	                                                           UINT __RPC_FAR *pctinfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( WebViewer_IShellUIHelper_Interface __RPC_FAR * This, 
	                                                      UINT iTInfo, 
	                                                      LCID lcid, 
	                                                      ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( WebViewer_IShellUIHelper_Interface __RPC_FAR * This, 
	                                                        REFIID riid, 
	                                                        LPOLESTR __RPC_FAR *rgszNames, 
	                                                        UINT cNames, 
	                                                        LCID lcid, 
	                                                        DISPID __RPC_FAR *rgDispId);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( WebViewer_IShellUIHelper_Interface __RPC_FAR * This, 
	                                                 DISPID dispIdMember, 
	                                                 REFIID riid, 
	                                                 LCID lcid, 
	                                                 WORD wFlags, 
	                                                 DISPPARAMS __RPC_FAR *pDispParams, 
	                                                 VARIANT __RPC_FAR *pVarResult, 
	                                                 EXCEPINFO __RPC_FAR *pExcepInfo, 
	                                                 UINT __RPC_FAR *puArgErr);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ResetFrstBootMode_) (WebViewer_IShellUIHelper_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ResetSafeMode_) (WebViewer_IShellUIHelper_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RefreshOfflnDsktp_) (WebViewer_IShellUIHelper_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddFavorite_) (WebViewer_IShellUIHelper_Interface __RPC_FAR *This, 
	                                                      BSTR URL, 
	                                                      VARIANT *title);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddChannel_) (WebViewer_IShellUIHelper_Interface __RPC_FAR *This, 
	                                                     BSTR URL);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddDsktpComponent_) (WebViewer_IShellUIHelper_Interface __RPC_FAR *This, 
	                                                            BSTR URL, 
	                                                            BSTR type, 
	                                                            VARIANT *left, 
	                                                            VARIANT *top, 
	                                                            VARIANT *width, 
	                                                            VARIANT *height);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsSubscribed_) (WebViewer_IShellUIHelper_Interface __RPC_FAR *This, 
	                                                       BSTR URL, 
	                                                       VBOOL *pBool);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NavigateAndFind_) (WebViewer_IShellUIHelper_Interface __RPC_FAR *This, 
	                                                          BSTR URL, 
	                                                          BSTR strQuery, 
	                                                          VARIANT *varTargetFrame);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ImportExportFavorites_) (WebViewer_IShellUIHelper_Interface __RPC_FAR *This, 
	                                                                VBOOL fImport, 
	                                                                BSTR strImpExpPath);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AutoCompleteSaveForm_) (WebViewer_IShellUIHelper_Interface __RPC_FAR *This, 
	                                                               VARIANT *form);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AutoScan_) (WebViewer_IShellUIHelper_Interface __RPC_FAR *This, 
	                                                   BSTR strSearch, 
	                                                   BSTR strFailureUrl, 
	                                                   VARIANT *pvarTargetFrame);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AutoCompleteAttach_) (WebViewer_IShellUIHelper_Interface __RPC_FAR *This, 
	                                                             VARIANT *reserved);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowBrowserUI_) (WebViewer_IShellUIHelper_Interface __RPC_FAR *This, 
	                                                        BSTR bstrName, 
	                                                        VARIANT *pvarIn, 
	                                                        VARIANT *pvarOut);

} WebViewer_IShellUIHelper_VTable;

typedef interface tagWebViewer_IShellUIHelper_Interface
{
	CONST_VTBL WebViewer_IShellUIHelper_VTable __RPC_FAR *lpVtbl;
} WebViewer_IShellUIHelper_Interface;

typedef interface tagWebViewer_IShellNameSpace_Interface WebViewer_IShellNameSpace_Interface;

typedef struct tagWebViewer_IShellNameSpace_VTable
{
	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( WebViewer_IShellNameSpace_Interface __RPC_FAR * This, 
	                                                         REFIID riid, 
	                                                         void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( WebViewer_IShellNameSpace_Interface __RPC_FAR * This);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( WebViewer_IShellNameSpace_Interface __RPC_FAR * This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( WebViewer_IShellNameSpace_Interface __RPC_FAR * This, 
	                                                           UINT __RPC_FAR *pctinfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( WebViewer_IShellNameSpace_Interface __RPC_FAR * This, 
	                                                      UINT iTInfo, 
	                                                      LCID lcid, 
	                                                      ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( WebViewer_IShellNameSpace_Interface __RPC_FAR * This, 
	                                                        REFIID riid, 
	                                                        LPOLESTR __RPC_FAR *rgszNames, 
	                                                        UINT cNames, 
	                                                        LCID lcid, 
	                                                        DISPID __RPC_FAR *rgDispId);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( WebViewer_IShellNameSpace_Interface __RPC_FAR * This, 
	                                                 DISPID dispIdMember, 
	                                                 REFIID riid, 
	                                                 LCID lcid, 
	                                                 WORD wFlags, 
	                                                 DISPPARAMS __RPC_FAR *pDispParams, 
	                                                 VARIANT __RPC_FAR *pVarResult, 
	                                                 EXCEPINFO __RPC_FAR *pExcepInfo, 
	                                                 UINT __RPC_FAR *puArgErr);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MoveSelectionUp_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MoveSelectionDown_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ResetSort_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NewFolder_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Synchronize_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Import_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Export_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeContextMenuCommand_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                                   BSTR strCommand);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MoveSelectionTo_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSubscriptionsEnabled_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                                  VBOOL *pBool);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateSubscriptionForSelection_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                                         VBOOL *pBool);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteSubscriptionForSelection_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                                         VBOOL *pBool);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetRoot_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                  BSTR bstrFullPath);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEnumOptions_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                         long *pgrfEnumFlags);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEnumOptions_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                         long pgrfEnumFlags);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSelectedItem_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                          LPDISPATCH *pItem);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSelectedItem_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                          LPDISPATCH pItem);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetRoot_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                  VARIANT *pvar);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetRootProp_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                      VARIANT pvar);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDepth_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                   long *piDepth);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDepth_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                   long piDepth);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMode_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                  long *puMode);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMode_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                  long puMode);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFlags_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                   long *pdwFlags);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFlags_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                   long pdwFlags);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTVFlags_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                     long dwFlags);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTVFlags_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                     long *dwFlags);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetColumns_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                     BSTR *bstrColumns);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetColumns_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                     BSTR bstrColumns);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCountViewTypes_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                            long *piTypes);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetViewType_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                      long iType);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectedItems_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                        LPDISPATCH *ppid);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Expand_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This, 
	                                                 VARIANT var, 
	                                                 long iDepth);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnselectAll_) (WebViewer_IShellNameSpace_Interface __RPC_FAR *This);

} WebViewer_IShellNameSpace_VTable;

typedef interface tagWebViewer_IShellNameSpace_Interface
{
	CONST_VTBL WebViewer_IShellNameSpace_VTable __RPC_FAR *lpVtbl;
} WebViewer_IShellNameSpace_Interface;

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnStatusTextChange_CA_PARAMDATA[] =
	{
		{"text", VT_BSTR}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnProgressChange_CA_PARAMDATA[] =
	{
		{"progress", VT_I4},
        {"progressMax", VT_I4}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnCommandStateChange_CA_PARAMDATA[] =
	{
		{"command", VT_I4},
        {"enable", VT_BOOL}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnTitleChange_CA_PARAMDATA[] =
	{
		{"text", VT_BSTR}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnPropertyChange_CA_PARAMDATA[] =
	{
		{"szProperty", VT_BSTR}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnBeforeNavigate2_CA_PARAMDATA[] =
	{
		{"pDisp", VT_DISPATCH},
        {"URL", VT_VARIANT | VT_BYREF},
        {"flags", VT_VARIANT | VT_BYREF},
        {"targetFrameName", VT_VARIANT | VT_BYREF},
        {"postData", VT_VARIANT | VT_BYREF},
        {"headers", VT_VARIANT | VT_BYREF},
        {"cancel", VT_BOOL | VT_BYREF}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnNewWindow2_CA_PARAMDATA[] =
	{
		{"ppDisp", VT_DISPATCH | VT_BYREF},
        {"cancel", VT_BOOL | VT_BYREF}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnNavigateComplete2_CA_PARAMDATA[] =
	{
		{"pDisp", VT_DISPATCH},
        {"URL", VT_VARIANT | VT_BYREF}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnDocumentComplete_CA_PARAMDATA[] =
	{
		{"pDisp", VT_DISPATCH},
        {"URL", VT_VARIANT | VT_BYREF}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnOnVisible_CA_PARAMDATA[] =
	{
		{"visible", VT_BOOL}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnOnToolBar_CA_PARAMDATA[] =
	{
		{"toolBar", VT_BOOL}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnOnMenuBar_CA_PARAMDATA[] =
	{
		{"menuBar", VT_BOOL}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnOnStatusBar_CA_PARAMDATA[] =
	{
		{"statusBar", VT_BOOL}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnOnFullScreen_CA_PARAMDATA[] =
	{
		{"fullScreen", VT_BOOL}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnOnTheaterMode_CA_PARAMDATA[] =
	{
		{"theaterMode", VT_BOOL}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnWindowSetResizable_CA_PARAMDATA[] =
	{
		{"resizable", VT_BOOL}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnWindowSetLeft_CA_PARAMDATA[] =
	{
		{"left", VT_I4}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnWindowSetTop_CA_PARAMDATA[] =
	{
		{"top", VT_I4}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnWindowSetWidth_CA_PARAMDATA[] =
	{
		{"width", VT_I4}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnWindowSetHeight_CA_PARAMDATA[] =
	{
		{"height", VT_I4}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnWindowClosing_CA_PARAMDATA[] =
	{
		{"isChildWindow", VT_BOOL},
        {"cancel", VT_BOOL | VT_BYREF}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnClientToHostWindow_CA_PARAMDATA[] =
	{
		{"CX", VT_I4 | VT_BYREF},
        {"CY", VT_I4 | VT_BYREF}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnSetSecureLockIcon_CA_PARAMDATA[] =
	{
		{"secureLockIcon", VT_I4}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnFileDownload_CA_PARAMDATA[] =
	{
		{"cancel", VT_BOOL | VT_BYREF}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnNavigateError_CA_PARAMDATA[] =
	{
		{"pDisp", VT_DISPATCH},
        {"URL", VT_VARIANT | VT_BYREF},
        {"frame", VT_VARIANT | VT_BYREF},
        {"statusCode", VT_VARIANT | VT_BYREF},
        {"cancel", VT_BOOL | VT_BYREF}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnPrintTemplateInstantiation_CA_PARAMDATA[] =
	{
		{"pDisp", VT_DISPATCH}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnPrintTemplateTeardown_CA_PARAMDATA[] =
	{
		{"pDisp", VT_DISPATCH}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnUpdatePageStatus_CA_PARAMDATA[] =
	{
		{"pDisp", VT_DISPATCH},
        {"nPage", VT_VARIANT | VT_BYREF},
        {"fDone", VT_VARIANT | VT_BYREF}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnPrivacyImpactedStateChange_CA_PARAMDATA[] =
	{
		{"bImpacted", VT_BOOL}
	};

static CA_PARAMDATA _DWebBrwsrEvnts2_RegOnNewWindow3_CA_PARAMDATA[] =
	{
		{"ppDisp", VT_DISPATCH | VT_BYREF},
        {"cancel", VT_BOOL | VT_BYREF},
        {"dwFlags", VT_I4},
        {"bstrUrlContext", VT_BSTR},
        {"bstrUrl", VT_BSTR}
	};

static CA_METHODDATA _DWebBrwsrEvnts2_CA_METHODDATA[] =
	{
		{"StatusTextChange", _DWebBrwsrEvnts2_RegOnStatusTextChange_CA_PARAMDATA, 102, 0, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"ProgressChange", _DWebBrwsrEvnts2_RegOnProgressChange_CA_PARAMDATA, 108, 1, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"CommandStateChange", _DWebBrwsrEvnts2_RegOnCommandStateChange_CA_PARAMDATA, 105, 2, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"DownloadBegin", NULL, 106, 3, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"DownloadComplete", NULL, 104, 4, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"TitleChange", _DWebBrwsrEvnts2_RegOnTitleChange_CA_PARAMDATA, 113, 5, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"PropertyChange", _DWebBrwsrEvnts2_RegOnPropertyChange_CA_PARAMDATA, 112, 6, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"BeforeNavigate2", _DWebBrwsrEvnts2_RegOnBeforeNavigate2_CA_PARAMDATA, 250, 7, CC_STDCALL, 7, DISPATCH_METHOD, VT_EMPTY},
        {"NewWindow2", _DWebBrwsrEvnts2_RegOnNewWindow2_CA_PARAMDATA, 251, 8, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"NavigateComplete2", _DWebBrwsrEvnts2_RegOnNavigateComplete2_CA_PARAMDATA, 252, 9, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"DocumentComplete", _DWebBrwsrEvnts2_RegOnDocumentComplete_CA_PARAMDATA, 259, 10, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"OnQuit", NULL, 253, 11, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"OnVisible", _DWebBrwsrEvnts2_RegOnOnVisible_CA_PARAMDATA, 254, 12, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"OnToolBar", _DWebBrwsrEvnts2_RegOnOnToolBar_CA_PARAMDATA, 255, 13, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"OnMenuBar", _DWebBrwsrEvnts2_RegOnOnMenuBar_CA_PARAMDATA, 256, 14, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"OnStatusBar", _DWebBrwsrEvnts2_RegOnOnStatusBar_CA_PARAMDATA, 257, 15, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"OnFullScreen", _DWebBrwsrEvnts2_RegOnOnFullScreen_CA_PARAMDATA, 258, 16, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"OnTheaterMode", _DWebBrwsrEvnts2_RegOnOnTheaterMode_CA_PARAMDATA, 260, 17, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"WindowSetResizable", _DWebBrwsrEvnts2_RegOnWindowSetResizable_CA_PARAMDATA, 262, 18, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"WindowSetLeft", _DWebBrwsrEvnts2_RegOnWindowSetLeft_CA_PARAMDATA, 264, 19, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"WindowSetTop", _DWebBrwsrEvnts2_RegOnWindowSetTop_CA_PARAMDATA, 265, 20, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"WindowSetWidth", _DWebBrwsrEvnts2_RegOnWindowSetWidth_CA_PARAMDATA, 266, 21, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"WindowSetHeight", _DWebBrwsrEvnts2_RegOnWindowSetHeight_CA_PARAMDATA, 267, 22, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"WindowClosing", _DWebBrwsrEvnts2_RegOnWindowClosing_CA_PARAMDATA, 263, 23, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"ClientToHostWindow", _DWebBrwsrEvnts2_RegOnClientToHostWindow_CA_PARAMDATA, 268, 24, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"SetSecureLockIcon", _DWebBrwsrEvnts2_RegOnSetSecureLockIcon_CA_PARAMDATA, 269, 25, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"FileDownload", _DWebBrwsrEvnts2_RegOnFileDownload_CA_PARAMDATA, 270, 26, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"NavigateError", _DWebBrwsrEvnts2_RegOnNavigateError_CA_PARAMDATA, 271, 27, CC_STDCALL, 5, DISPATCH_METHOD, VT_EMPTY},
        {"PrintTemplateInstantiation", _DWebBrwsrEvnts2_RegOnPrintTemplateInstantiation_CA_PARAMDATA, 225, 28, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"PrintTemplateTeardown", _DWebBrwsrEvnts2_RegOnPrintTemplateTeardown_CA_PARAMDATA, 226, 29, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"UpdatePageStatus", _DWebBrwsrEvnts2_RegOnUpdatePageStatus_CA_PARAMDATA, 227, 30, CC_STDCALL, 3, DISPATCH_METHOD, VT_EMPTY},
        {"PrivacyImpactedStateChange", _DWebBrwsrEvnts2_RegOnPrivacyImpactedStateChange_CA_PARAMDATA, 272, 31, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"NewWindow3", _DWebBrwsrEvnts2_RegOnNewWindow3_CA_PARAMDATA, 273, 32, CC_STDCALL, 5, DISPATCH_METHOD, VT_EMPTY}
	};

static CA_INTERFACEDATA _DWebBrwsrEvnts2_CA_INTERFACEDATA =
	{
		_DWebBrwsrEvnts2_CA_METHODDATA,
        sizeof (_DWebBrwsrEvnts2_CA_METHODDATA) / sizeof (*_DWebBrwsrEvnts2_CA_METHODDATA)
	};

static void * _DWebBrwsrEvnts2_EventVTable[] =
	{
		DWebBrwsrEvnts2RegOnStatusTextChange_EventVTableFunc,
        DWebBrwsrEvnts2RegOnProgressChange_EventVTableFunc,
        DWebBrwsrEvnts2RegOnCommandStateChange_EventVTableFunc,
        DWebBrwsrEvnts2RegOnDownloadBegin_EventVTableFunc,
        DWebBrwsrEvnts2RegOnDownloadComplete_EventVTableFunc,
        DWebBrwsrEvnts2RegOnTitleChange_EventVTableFunc,
        DWebBrwsrEvnts2RegOnPropertyChange_EventVTableFunc,
        DWebBrwsrEvnts2RegOnBeforeNavigate2_EventVTableFunc,
        DWebBrwsrEvnts2RegOnNewWindow2_EventVTableFunc,
        DWebBrwsrEvnts2RegOnNavigateComplete2_EventVTableFunc,
        DWebBrwsrEvnts2RegOnDocumentComplete_EventVTableFunc,
        DWebBrwsrEvnts2RegOnOnQuit_EventVTableFunc,
        DWebBrwsrEvnts2RegOnOnVisible_EventVTableFunc,
        DWebBrwsrEvnts2RegOnOnToolBar_EventVTableFunc,
        DWebBrwsrEvnts2RegOnOnMenuBar_EventVTableFunc,
        DWebBrwsrEvnts2RegOnOnStatusBar_EventVTableFunc,
        DWebBrwsrEvnts2RegOnOnFullScreen_EventVTableFunc,
        DWebBrwsrEvnts2RegOnOnTheaterMode_EventVTableFunc,
        DWebBrwsrEvnts2RegOnWindowSetResizable_EventVTableFunc,
        DWebBrwsrEvnts2RegOnWindowSetLeft_EventVTableFunc,
        DWebBrwsrEvnts2RegOnWindowSetTop_EventVTableFunc,
        DWebBrwsrEvnts2RegOnWindowSetWidth_EventVTableFunc,
        DWebBrwsrEvnts2RegOnWindowSetHeight_EventVTableFunc,
        DWebBrwsrEvnts2RegOnWindowClosing_EventVTableFunc,
        DWebBrwsrEvnts2RegOnClientToHostWindow_EventVTableFunc,
        DWebBrwsrEvnts2RegOnSetSecureLockIcon_EventVTableFunc,
        DWebBrwsrEvnts2RegOnFileDownload_EventVTableFunc,
        DWebBrwsrEvnts2RegOnNavigateError_EventVTableFunc,
        DWebBrwsrEvnts2RegOnPrintTemplateInstantiation_EventVTableFunc,
        DWebBrwsrEvnts2RegOnPrintTemplateTeardown_EventVTableFunc,
        DWebBrwsrEvnts2RegOnUpdatePageStatus_EventVTableFunc,
        DWebBrwsrEvnts2RegOnPrivacyImpactedStateChange_EventVTableFunc,
        DWebBrwsrEvnts2RegOnNewWindow3_EventVTableFunc
	};

static CAEventClassDefn _DWebBrwsrEvnts2_CAEventClassDefn =
	{
		20,
        &WebViewer_IID_DWebBrwsrEvnts2,
        _DWebBrwsrEvnts2_EventVTable,
        &_DWebBrwsrEvnts2_CA_INTERFACEDATA,
        0
	};

static CA_PARAMDATA _DWebBrwsrEvnts_RegOnBeforeNavigate_CA_PARAMDATA[] =
	{
		{"URL", VT_BSTR},
        {"flags", VT_I4},
        {"targetFrameName", VT_BSTR},
        {"postData", VT_VARIANT | VT_BYREF},
        {"headers", VT_BSTR},
        {"cancel", VT_BOOL | VT_BYREF}
	};

static CA_PARAMDATA _DWebBrwsrEvnts_RegOnNavigateComplete_CA_PARAMDATA[] =
	{
		{"URL", VT_BSTR}
	};

static CA_PARAMDATA _DWebBrwsrEvnts_RegOnStatusTextChange_CA_PARAMDATA[] =
	{
		{"text", VT_BSTR}
	};

static CA_PARAMDATA _DWebBrwsrEvnts_RegOnProgressChange_CA_PARAMDATA[] =
	{
		{"progress", VT_I4},
        {"progressMax", VT_I4}
	};

static CA_PARAMDATA _DWebBrwsrEvnts_RegOnCommandStateChange_CA_PARAMDATA[] =
	{
		{"command", VT_I4},
        {"enable", VT_BOOL}
	};

static CA_PARAMDATA _DWebBrwsrEvnts_RegOnNewWindow_CA_PARAMDATA[] =
	{
		{"URL", VT_BSTR},
        {"flags", VT_I4},
        {"targetFrameName", VT_BSTR},
        {"postData", VT_VARIANT | VT_BYREF},
        {"headers", VT_BSTR},
        {"processed", VT_BOOL | VT_BYREF}
	};

static CA_PARAMDATA _DWebBrwsrEvnts_RegOnTitleChange_CA_PARAMDATA[] =
	{
		{"text", VT_BSTR}
	};

static CA_PARAMDATA _DWebBrwsrEvnts_RegOnFrameBeforeNavigate_CA_PARAMDATA[] =
	{
		{"URL", VT_BSTR},
        {"flags", VT_I4},
        {"targetFrameName", VT_BSTR},
        {"postData", VT_VARIANT | VT_BYREF},
        {"headers", VT_BSTR},
        {"cancel", VT_BOOL | VT_BYREF}
	};

static CA_PARAMDATA _DWebBrwsrEvnts_RegOnFrameNavigateComplete_CA_PARAMDATA[] =
	{
		{"URL", VT_BSTR}
	};

static CA_PARAMDATA _DWebBrwsrEvnts_RegOnFrameNewWindow_CA_PARAMDATA[] =
	{
		{"URL", VT_BSTR},
        {"flags", VT_I4},
        {"targetFrameName", VT_BSTR},
        {"postData", VT_VARIANT | VT_BYREF},
        {"headers", VT_BSTR},
        {"processed", VT_BOOL | VT_BYREF}
	};

static CA_PARAMDATA _DWebBrwsrEvnts_RegOnQuit_CA_PARAMDATA[] =
	{
		{"cancel", VT_BOOL | VT_BYREF}
	};

static CA_PARAMDATA _DWebBrwsrEvnts_RegOnPropertyChange_CA_PARAMDATA[] =
	{
		{"property", VT_BSTR}
	};

static CA_METHODDATA _DWebBrwsrEvnts_CA_METHODDATA[] =
	{
		{"BeforeNavigate", _DWebBrwsrEvnts_RegOnBeforeNavigate_CA_PARAMDATA, 100, 0, CC_STDCALL, 6, DISPATCH_METHOD, VT_EMPTY},
        {"NavigateComplete", _DWebBrwsrEvnts_RegOnNavigateComplete_CA_PARAMDATA, 101, 1, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"StatusTextChange", _DWebBrwsrEvnts_RegOnStatusTextChange_CA_PARAMDATA, 102, 2, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"ProgressChange", _DWebBrwsrEvnts_RegOnProgressChange_CA_PARAMDATA, 108, 3, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"DownloadComplete", NULL, 104, 4, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"CommandStateChange", _DWebBrwsrEvnts_RegOnCommandStateChange_CA_PARAMDATA, 105, 5, CC_STDCALL, 2, DISPATCH_METHOD, VT_EMPTY},
        {"DownloadBegin", NULL, 106, 6, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"NewWindow", _DWebBrwsrEvnts_RegOnNewWindow_CA_PARAMDATA, 107, 7, CC_STDCALL, 6, DISPATCH_METHOD, VT_EMPTY},
        {"TitleChange", _DWebBrwsrEvnts_RegOnTitleChange_CA_PARAMDATA, 113, 8, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"FrameBeforeNavigate", _DWebBrwsrEvnts_RegOnFrameBeforeNavigate_CA_PARAMDATA, 200, 9, CC_STDCALL, 6, DISPATCH_METHOD, VT_EMPTY},
        {"FrameNavigateComplete", _DWebBrwsrEvnts_RegOnFrameNavigateComplete_CA_PARAMDATA, 201, 10, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"FrameNewWindow", _DWebBrwsrEvnts_RegOnFrameNewWindow_CA_PARAMDATA, 204, 11, CC_STDCALL, 6, DISPATCH_METHOD, VT_EMPTY},
        {"Quit", _DWebBrwsrEvnts_RegOnQuit_CA_PARAMDATA, 103, 12, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"WindowMove", NULL, 109, 13, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"WindowResize", NULL, 110, 14, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"WindowActivate", NULL, 111, 15, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"PropertyChange", _DWebBrwsrEvnts_RegOnPropertyChange_CA_PARAMDATA, 112, 16, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY}
	};

static CA_INTERFACEDATA _DWebBrwsrEvnts_CA_INTERFACEDATA =
	{
		_DWebBrwsrEvnts_CA_METHODDATA,
        sizeof (_DWebBrwsrEvnts_CA_METHODDATA) / sizeof (*_DWebBrwsrEvnts_CA_METHODDATA)
	};

static void * _DWebBrwsrEvnts_EventVTable[] =
	{
		DWebBrwsrEvntsRegOnBeforeNavigate_EventVTableFunc,
        DWebBrwsrEvntsRegOnNavigateComplete_EventVTableFunc,
        DWebBrwsrEvntsRegOnStatusTextChange_EventVTableFunc,
        DWebBrwsrEvntsRegOnProgressChange_EventVTableFunc,
        DWebBrwsrEvntsRegOnDownloadComplete_EventVTableFunc,
        DWebBrwsrEvntsRegOnCommandStateChange_EventVTableFunc,
        DWebBrwsrEvntsRegOnDownloadBegin_EventVTableFunc,
        DWebBrwsrEvntsRegOnNewWindow_EventVTableFunc,
        DWebBrwsrEvntsRegOnTitleChange_EventVTableFunc,
        DWebBrwsrEvntsRegOnFrameBeforeNavigate_EventVTableFunc,
        DWebBrwsrEvntsRegOnFrameNavigateComplete_EventVTableFunc,
        DWebBrwsrEvntsRegOnFrameNewWindow_EventVTableFunc,
        DWebBrwsrEvntsRegOnQuit_EventVTableFunc,
        DWebBrwsrEvntsRegOnWindowMove_EventVTableFunc,
        DWebBrwsrEvntsRegOnWindowResize_EventVTableFunc,
        DWebBrwsrEvntsRegOnWindowActivate_EventVTableFunc,
        DWebBrwsrEvntsRegOnPropertyChange_EventVTableFunc
	};

static CAEventClassDefn _DWebBrwsrEvnts_CAEventClassDefn =
	{
		20,
        &WebViewer_IID_DWebBrwsrEvnts,
        _DWebBrwsrEvnts_EventVTable,
        &_DWebBrwsrEvnts_CA_INTERFACEDATA,
        0
	};

static CA_PARAMDATA _DShellWindowsEvnts_RegOnWindowRegistered_CA_PARAMDATA[] =
	{
		{"lCookie", VT_I4}
	};

static CA_PARAMDATA _DShellWindowsEvnts_RegOnWindowRevoked_CA_PARAMDATA[] =
	{
		{"lCookie", VT_I4}
	};

static CA_METHODDATA _DShellWindowsEvnts_CA_METHODDATA[] =
	{
		{"WindowRegistered", _DShellWindowsEvnts_RegOnWindowRegistered_CA_PARAMDATA, 200, 0, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY},
        {"WindowRevoked", _DShellWindowsEvnts_RegOnWindowRevoked_CA_PARAMDATA, 201, 1, CC_STDCALL, 1, DISPATCH_METHOD, VT_EMPTY}
	};

static CA_INTERFACEDATA _DShellWindowsEvnts_CA_INTERFACEDATA =
	{
		_DShellWindowsEvnts_CA_METHODDATA,
        sizeof (_DShellWindowsEvnts_CA_METHODDATA) / sizeof (*_DShellWindowsEvnts_CA_METHODDATA)
	};

static void * _DShellWindowsEvnts_EventVTable[] =
	{
		DShellWindowsEvntsRegOnWindowRegistered_EventVTableFunc,
        DShellWindowsEvntsRegOnWindowRevoked_EventVTableFunc
	};

static CAEventClassDefn _DShellWindowsEvnts_CAEventClassDefn =
	{
		20,
        &WebViewer_IID_DShellWindowsEvnts,
        _DShellWindowsEvnts_EventVTable,
        &_DShellWindowsEvnts_CA_INTERFACEDATA,
        0
	};

static CA_PARAMDATA _DShellNameSpaceEvents_RegOnFavoritesSelectionChange_CA_PARAMDATA[] =
	{
		{"cItems", VT_I4},
        {"hItem", VT_I4},
        {"strName", VT_BSTR},
        {"strUrl", VT_BSTR},
        {"cVisits", VT_I4},
        {"strDate", VT_BSTR},
        {"fAvailableOffline", VT_I4}
	};

static CA_METHODDATA _DShellNameSpaceEvents_CA_METHODDATA[] =
	{
		{"FavoritesSelectionChange", _DShellNameSpaceEvents_RegOnFavoritesSelectionChange_CA_PARAMDATA, 1, 0, CC_STDCALL, 7, DISPATCH_METHOD, VT_EMPTY},
        {"SelectionChange", NULL, 2, 1, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"DoubleClick", NULL, 3, 2, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY},
        {"Initialized", NULL, 4, 3, CC_STDCALL, 0, DISPATCH_METHOD, VT_EMPTY}
	};

static CA_INTERFACEDATA _DShellNameSpaceEvents_CA_INTERFACEDATA =
	{
		_DShellNameSpaceEvents_CA_METHODDATA,
        sizeof (_DShellNameSpaceEvents_CA_METHODDATA) / sizeof (*_DShellNameSpaceEvents_CA_METHODDATA)
	};

static void * _DShellNameSpaceEvents_EventVTable[] =
	{
		DShellNameSpaceEventsRegOnFavoritesSelectionChange_EventVTableFunc,
        DShellNameSpaceEventsRegOnSelectionChange_EventVTableFunc,
        DShellNameSpaceEventsRegOnDoubleClick_EventVTableFunc,
        DShellNameSpaceEventsRegOnInitialized_EventVTableFunc
	};

static CAEventClassDefn _DShellNameSpaceEvents_CAEventClassDefn =
	{
		20,
        &WebViewer_IID_DShellNameSpaceEvents,
        _DShellNameSpaceEvents_EventVTable,
        &_DShellNameSpaceEvents_CA_INTERFACEDATA,
        0
	};

const IID WebViewer_IID_IWebBrowser =
	{
		0xEAB22AC1, 0x30C1, 0x11CF, 0xA7, 0xEB, 0x0, 0x0, 0xC0, 0x5B, 0xAE, 0xB
	};

const IID WebViewer_IID_IWebBrowser2 =
	{
		0xD30C1661, 0xCDAF, 0x11D0, 0x8A, 0x3E, 0x0, 0xC0, 0x4F, 0xC9, 0xE2, 0x6E
	};

const IID WebViewer_IID_IShellWindows =
	{
		0x85CB6900, 0x4D95, 0x11CF, 0x96, 0xC, 0x0, 0x80, 0xC7, 0xF4, 0xEE, 0x85
	};

const IID WebViewer_IID_IShellUIHelper =
	{
		0x729FE2F8, 0x1EA8, 0x11D1, 0x8F, 0x85, 0x0, 0xC0, 0x4F, 0xC2, 0xFB, 0xE1
	};

const IID WebViewer_IID_IShellNameSpace =
	{
		0xE572D3C9, 0x37BE, 0x4AE2, 0x82, 0x5D, 0xD5, 0x21, 0x76, 0x3E, 0x31, 0x8
	};

const IID WebViewer_IID_DWebBrwsrEvnts2 =
	{
		0x34A715A0, 0x6587, 0x11D0, 0x92, 0x4A, 0x0, 0x20, 0xAF, 0xC7, 0xAC, 0x4D
	};

const IID WebViewer_IID_DWebBrwsrEvnts =
	{
		0xEAB22AC2, 0x30C1, 0x11CF, 0xA7, 0xEB, 0x0, 0x0, 0xC0, 0x5B, 0xAE, 0xB
	};

const IID WebViewer_IID_DShellWindowsEvnts =
	{
		0xFE4106E0, 0x399A, 0x11D0, 0xA4, 0x8C, 0x0, 0xA0, 0xC9, 0xA, 0x8F, 0x39
	};

const IID WebViewer_IID_DShellNameSpaceEvents =
	{
		0x55136806, 0xB2DE, 0x11D1, 0xB9, 0xF2, 0x0, 0xA0, 0xC9, 0x8B, 0xC5, 0x47
	};

HRESULT CVIFUNC WebViewer_NewIWebBrowser (const char *server,
                                          int supportMultithreading, LCID locale,
                                          int reserved,
                                          CAObjHandle *objectHandle)
{
	HRESULT __result = S_OK;
	GUID clsid = {0xEAB22AC3, 0x30C1, 0x11CF, 0xA7, 0xEB, 0x0, 0x0, 0xC0, 0x5B,
	              0xAE, 0xB};

	__result = CA_CreateObjectByClassIdEx (&clsid, server,
	                                       &WebViewer_IID_IWebBrowser,
	                                       supportMultithreading, locale,
	                                       reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC WebViewer_OpenIWebBrowser (const char *fileName,
                                           const char *server,
                                           int supportMultithreading,
                                           LCID locale, int reserved,
                                           CAObjHandle *objectHandle)
{
	HRESULT __result = S_OK;
	GUID clsid = {0xEAB22AC3, 0x30C1, 0x11CF, 0xA7, 0xEB, 0x0, 0x0, 0xC0, 0x5B,
	              0xAE, 0xB};

	__result = CA_LoadObjectFromFileByClassIdEx (fileName, &clsid, server,
	                                             &WebViewer_IID_IWebBrowser,
	                                             supportMultithreading, locale,
	                                             reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC WebViewer_ActiveIWebBrowser (const char *server,
                                             int supportMultithreading,
                                             LCID locale, int reserved,
                                             CAObjHandle *objectHandle)
{
	HRESULT __result = S_OK;
	GUID clsid = {0xEAB22AC3, 0x30C1, 0x11CF, 0xA7, 0xEB, 0x0, 0x0, 0xC0, 0x5B,
	              0xAE, 0xB};

	__result = CA_GetActiveObjectByClassIdEx (&clsid, server,
	                                          &WebViewer_IID_IWebBrowser,
	                                          supportMultithreading, locale,
	                                          reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGoBack (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GoBack_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGoForward (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GoForward_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGoHome (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GoHome_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGoSearch (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GoSearch_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserNavigate (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               const char *URL, VARIANT flags,
                                               VARIANT targetFrameName,
                                               VARIANT postData, VARIANT headers)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR URL__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (URL, &URL__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Navigate_ (__vtblIFacePtr,
	                                               URL__AutoType, &flags,
	                                               &targetFrameName, &postData,
	                                               &headers));

Error:
	CA_FreeBSTR (URL__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserRefresh (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Refresh_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserRefresh2 (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VARIANT level)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Refresh2_ (__vtblIFacePtr, &level));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserStop (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Stop_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGetApplication (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     CAObjHandle *ppDisp)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppDisp__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (ppDisp)
		*ppDisp = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetApplication_ (__vtblIFacePtr,
	                                                     &ppDisp__AutoType));
	

	if (ppDisp)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (ppDisp__AutoType,
		                                             &IID_IDispatch,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, ppDisp));
		ppDisp__AutoType = 0;
		}

Error:
	if (ppDisp__AutoType)
		ppDisp__AutoType->lpVtbl->Release (ppDisp__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppDisp)
			{
			CA_DiscardObjHandle (*ppDisp);
			*ppDisp = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGetParent (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                CAObjHandle *ppDisp)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppDisp__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (ppDisp)
		*ppDisp = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetParent_ (__vtblIFacePtr,
	                                                &ppDisp__AutoType));
	

	if (ppDisp)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (ppDisp__AutoType,
		                                             &IID_IDispatch,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, ppDisp));
		ppDisp__AutoType = 0;
		}

Error:
	if (ppDisp__AutoType)
		ppDisp__AutoType->lpVtbl->Release (ppDisp__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppDisp)
			{
			CA_DiscardObjHandle (*ppDisp);
			*ppDisp = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGetContainer (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   CAObjHandle *ppDisp)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppDisp__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (ppDisp)
		*ppDisp = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetContainer_ (__vtblIFacePtr,
	                                                   &ppDisp__AutoType));
	

	if (ppDisp)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (ppDisp__AutoType,
		                                             &IID_IDispatch,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, ppDisp));
		ppDisp__AutoType = 0;
		}

Error:
	if (ppDisp__AutoType)
		ppDisp__AutoType->lpVtbl->Release (ppDisp__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppDisp)
			{
			CA_DiscardObjHandle (*ppDisp);
			*ppDisp = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGetDocument (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  CAObjHandle *ppDisp)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppDisp__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (ppDisp)
		*ppDisp = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetDocument_ (__vtblIFacePtr,
	                                                  &ppDisp__AutoType));
	

	if (ppDisp)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (ppDisp__AutoType,
		                                             &IID_IDispatch,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, ppDisp));
		ppDisp__AutoType = 0;
		}

Error:
	if (ppDisp__AutoType)
		ppDisp__AutoType->lpVtbl->Release (ppDisp__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppDisp)
			{
			CA_DiscardObjHandle (*ppDisp);
			*ppDisp = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGetTopLevelContainer (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           VBOOL *pBool)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pBool__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetTopLevelContainer_ (__vtblIFacePtr,
	                                                           &pBool__Temp));

	if (pBool)
		{
		*pBool = pBool__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGetType (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, char **type)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR type__AutoType = 0;

	if (type)
		*type = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetType_ (__vtblIFacePtr,
	                                              &type__AutoType));

	if (type)
		__caErrChk (CA_BSTRGetCString (type__AutoType, type));

Error:
	CA_FreeBSTR (type__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (type)
			{
			CA_FreeMemory (*type);
			*type = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGetLeft (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, long *pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long pl__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetLeft_ (__vtblIFacePtr, &pl__Temp));

	if (pl)
		{
		*pl = pl__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserSetLeft (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, long pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetLeft_ (__vtblIFacePtr, pl));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGetTop (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, long *pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long pl__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetTop_ (__vtblIFacePtr, &pl__Temp));

	if (pl)
		{
		*pl = pl__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserSetTop (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, long pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetTop_ (__vtblIFacePtr, pl));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGetWidth (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, long *pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long pl__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetWidth_ (__vtblIFacePtr, &pl__Temp));

	if (pl)
		{
		*pl = pl__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserSetWidth (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, long pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetWidth_ (__vtblIFacePtr, pl));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGetHeight (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo, long *pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long pl__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetHeight_ (__vtblIFacePtr, &pl__Temp));

	if (pl)
		{
		*pl = pl__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserSetHeight (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo, long pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetHeight_ (__vtblIFacePtr, pl));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGetLocationName (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      char **locationName)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR locationName__AutoType = 0;

	if (locationName)
		*locationName = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetLocationName_ (__vtblIFacePtr,
	                                                      &locationName__AutoType));

	if (locationName)
		__caErrChk (CA_BSTRGetCString (locationName__AutoType, locationName));

Error:
	CA_FreeBSTR (locationName__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (locationName)
			{
			CA_FreeMemory (*locationName);
			*locationName = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGetLocationURL (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     char **locationURL)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR locationURL__AutoType = 0;

	if (locationURL)
		*locationURL = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetLocationURL_ (__vtblIFacePtr,
	                                                     &locationURL__AutoType));

	if (locationURL)
		__caErrChk (CA_BSTRGetCString (locationURL__AutoType, locationURL));

Error:
	CA_FreeBSTR (locationURL__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (locationURL)
			{
			CA_FreeMemory (*locationURL);
			*locationURL = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowserGetBusy (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, VBOOL *pBool)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pBool__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetBusy_ (__vtblIFacePtr, &pBool__Temp));

	if (pBool)
		{
		*pBool = pBool__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_NewWebBrowserIWebBrowser2 (int panel,
                                                     const char *label, int top,
                                                     int left, int *controlID,
                                                     int *UILError)
{
	HRESULT __result = S_OK;
	int ctrlId;
	GUID clsid = {0x8856F961, 0x340A, 0x11D0, 0xA9, 0x6B, 0x0, 0xC0, 0x4F,
	              0xD7, 0x5, 0xA2};
	const char * licStr = NULL;

	ctrlId = NewActiveXCtrl (panel, label, top, left, &clsid,
	                         &WebViewer_IID_IWebBrowser2, licStr, &__result);

	__ActiveXCtrlErrorHandler();

	return __result;
}

HRESULT CVIFUNC WebViewer_NewInternetExplorerIWebBrowser2 (const char *server,
                                                           int supportMultithreading,
                                                           LCID locale,
                                                           int reserved,
                                                           CAObjHandle *objectHandle)
{
	HRESULT __result = S_OK;
	GUID clsid = {0x2DF01, 0x0, 0x0, 0xC0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x46};

	__result = CA_CreateObjectByClassIdEx (&clsid, server,
	                                       &WebViewer_IID_IWebBrowser2,
	                                       supportMultithreading, locale,
	                                       reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC WebViewer_OpenInternetExplorerIWebBrowser2 (const char *fileName,
                                                            const char *server,
                                                            int supportMultithreading,
                                                            LCID locale,
                                                            int reserved,
                                                            CAObjHandle *objectHandle)
{
	HRESULT __result = S_OK;
	GUID clsid = {0x2DF01, 0x0, 0x0, 0xC0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x46};

	__result = CA_LoadObjectFromFileByClassIdEx (fileName, &clsid, server,
	                                             &WebViewer_IID_IWebBrowser2,
	                                             supportMultithreading, locale,
	                                             reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC WebViewer_ActiveInternetExplorerIWebBrowser2 (const char *server,
                                                              int supportMultithreading,
                                                              LCID locale,
                                                              int reserved,
                                                              CAObjHandle *objectHandle)
{
	HRESULT __result = S_OK;
	GUID clsid = {0x2DF01, 0x0, 0x0, 0xC0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x46};

	__result = CA_GetActiveObjectByClassIdEx (&clsid, server,
	                                          &WebViewer_IID_IWebBrowser2,
	                                          supportMultithreading, locale,
	                                          reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GoBack (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GoBack_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GoForward (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GoForward_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GoHome (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GoHome_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GoSearch (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GoSearch_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2Navigate (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *URL, VARIANT flags,
                                                VARIANT targetFrameName,
                                                VARIANT postData,
                                                VARIANT headers)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR URL__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (URL, &URL__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Navigate_ (__vtblIFacePtr,
	                                               URL__AutoType, &flags,
	                                               &targetFrameName, &postData,
	                                               &headers));

Error:
	CA_FreeBSTR (URL__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2Refresh (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Refresh_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2Refresh2 (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VARIANT level)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Refresh2_ (__vtblIFacePtr, &level));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2Stop (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Stop_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetApplication (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      CAObjHandle *ppDisp)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppDisp__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (ppDisp)
		*ppDisp = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetApplication_ (__vtblIFacePtr,
	                                                     &ppDisp__AutoType));
	

	if (ppDisp)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (ppDisp__AutoType,
		                                             &IID_IDispatch,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, ppDisp));
		ppDisp__AutoType = 0;
		}

Error:
	if (ppDisp__AutoType)
		ppDisp__AutoType->lpVtbl->Release (ppDisp__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppDisp)
			{
			CA_DiscardObjHandle (*ppDisp);
			*ppDisp = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetParent (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 CAObjHandle *ppDisp)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppDisp__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (ppDisp)
		*ppDisp = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetParent_ (__vtblIFacePtr,
	                                                &ppDisp__AutoType));
	

	if (ppDisp)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (ppDisp__AutoType,
		                                             &IID_IDispatch,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, ppDisp));
		ppDisp__AutoType = 0;
		}

Error:
	if (ppDisp__AutoType)
		ppDisp__AutoType->lpVtbl->Release (ppDisp__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppDisp)
			{
			CA_DiscardObjHandle (*ppDisp);
			*ppDisp = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetContainer (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    CAObjHandle *ppDisp)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppDisp__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (ppDisp)
		*ppDisp = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetContainer_ (__vtblIFacePtr,
	                                                   &ppDisp__AutoType));
	

	if (ppDisp)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (ppDisp__AutoType,
		                                             &IID_IDispatch,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, ppDisp));
		ppDisp__AutoType = 0;
		}

Error:
	if (ppDisp__AutoType)
		ppDisp__AutoType->lpVtbl->Release (ppDisp__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppDisp)
			{
			CA_DiscardObjHandle (*ppDisp);
			*ppDisp = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetDocument (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   CAObjHandle *ppDisp)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppDisp__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (ppDisp)
		*ppDisp = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetDocument_ (__vtblIFacePtr,
	                                                  &ppDisp__AutoType));
	

	if (ppDisp)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (ppDisp__AutoType,
		                                             &IID_IDispatch,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, ppDisp));
		ppDisp__AutoType = 0;
		}

Error:
	if (ppDisp__AutoType)
		ppDisp__AutoType->lpVtbl->Release (ppDisp__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppDisp)
			{
			CA_DiscardObjHandle (*ppDisp);
			*ppDisp = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetTopLevelContainer (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VBOOL *pBool)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pBool__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetTopLevelContainer_ (__vtblIFacePtr,
	                                                           &pBool__Temp));

	if (pBool)
		{
		*pBool = pBool__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetType (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, char **type)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR type__AutoType = 0;

	if (type)
		*type = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetType_ (__vtblIFacePtr,
	                                              &type__AutoType));

	if (type)
		__caErrChk (CA_BSTRGetCString (type__AutoType, type));

Error:
	CA_FreeBSTR (type__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (type)
			{
			CA_FreeMemory (*type);
			*type = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetLeft (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, long *pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long pl__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetLeft_ (__vtblIFacePtr, &pl__Temp));

	if (pl)
		{
		*pl = pl__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetLeft (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, long pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetLeft_ (__vtblIFacePtr, pl));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetTop (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, long *pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long pl__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetTop_ (__vtblIFacePtr, &pl__Temp));

	if (pl)
		{
		*pl = pl__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetTop (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, long pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetTop_ (__vtblIFacePtr, pl));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetWidth (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo, long *pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long pl__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetWidth_ (__vtblIFacePtr, &pl__Temp));

	if (pl)
		{
		*pl = pl__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetWidth (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo, long pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetWidth_ (__vtblIFacePtr, pl));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetHeight (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo, long *pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long pl__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetHeight_ (__vtblIFacePtr, &pl__Temp));

	if (pl)
		{
		*pl = pl__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetHeight (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo, long pl)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetHeight_ (__vtblIFacePtr, pl));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetLocationName (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       char **locationName)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR locationName__AutoType = 0;

	if (locationName)
		*locationName = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetLocationName_ (__vtblIFacePtr,
	                                                      &locationName__AutoType));

	if (locationName)
		__caErrChk (CA_BSTRGetCString (locationName__AutoType, locationName));

Error:
	CA_FreeBSTR (locationName__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (locationName)
			{
			CA_FreeMemory (*locationName);
			*locationName = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetLocationURL (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      char **locationURL)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR locationURL__AutoType = 0;

	if (locationURL)
		*locationURL = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetLocationURL_ (__vtblIFacePtr,
	                                                     &locationURL__AutoType));

	if (locationURL)
		__caErrChk (CA_BSTRGetCString (locationURL__AutoType, locationURL));

Error:
	CA_FreeBSTR (locationURL__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (locationURL)
			{
			CA_FreeMemory (*locationURL);
			*locationURL = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetBusy (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *pBool)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pBool__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetBusy_ (__vtblIFacePtr, &pBool__Temp));

	if (pBool)
		{
		*pBool = pBool__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2Quit (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Quit_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2ClientToWindow (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      long *pcx, long *pcy)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->ClientToWindow_ (__vtblIFacePtr, pcx,
	                                                     pcy));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2PutProperty (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   const char *property,
                                                   VARIANT vtValue)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR property__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (property, &property__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->PutProperty_ (__vtblIFacePtr,
	                                                  property__AutoType,
	                                                  vtValue));

Error:
	CA_FreeBSTR (property__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetProperty (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   const char *property,
                                                   VARIANT *pvtValue)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR property__AutoType = 0;
	VARIANT pvtValue__Temp;

	if (pvtValue)
		CA_VariantSetEmpty (pvtValue);
	CA_VariantSetEmpty (&pvtValue__Temp);

	__caErrChk (CA_CStringToBSTR (property, &property__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetProperty_ (__vtblIFacePtr,
	                                                  property__AutoType,
	                                                  &pvtValue__Temp));

	if (pvtValue)
		{
		*pvtValue = pvtValue__Temp;
		CA_VariantSetEmpty (&pvtValue__Temp);
		}

Error:
	CA_FreeBSTR (property__AutoType);
	CA_VariantClear (&pvtValue__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvtValue)
			CA_VariantClear (pvtValue);
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetName (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, char **name)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR name__AutoType = 0;

	if (name)
		*name = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetName_ (__vtblIFacePtr,
	                                              &name__AutoType));

	if (name)
		__caErrChk (CA_BSTRGetCString (name__AutoType, name));

Error:
	CA_FreeBSTR (name__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (name)
			{
			CA_FreeMemory (*name);
			*name = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetHWND (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, long *pHWND)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long pHWND__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetHWND_ (__vtblIFacePtr, &pHWND__Temp));

	if (pHWND)
		{
		*pHWND = pHWND__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetFullName (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   char **fullName)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR fullName__AutoType = 0;

	if (fullName)
		*fullName = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetFullName_ (__vtblIFacePtr,
	                                                  &fullName__AutoType));

	if (fullName)
		__caErrChk (CA_BSTRGetCString (fullName__AutoType, fullName));

Error:
	CA_FreeBSTR (fullName__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (fullName)
			{
			CA_FreeMemory (*fullName);
			*fullName = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetPath (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, char **path)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR path__AutoType = 0;

	if (path)
		*path = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetPath_ (__vtblIFacePtr,
	                                              &path__AutoType));

	if (path)
		__caErrChk (CA_BSTRGetCString (path__AutoType, path));

Error:
	CA_FreeBSTR (path__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (path)
			{
			CA_FreeMemory (*path);
			*path = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetVisible (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL *pBool)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pBool__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetVisible_ (__vtblIFacePtr,
	                                                 &pBool__Temp));

	if (pBool)
		{
		*pBool = pBool__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetVisible (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL pBool)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetVisible_ (__vtblIFacePtr, pBool));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetStatusBar (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL *pBool)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pBool__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetStatusBar_ (__vtblIFacePtr,
	                                                   &pBool__Temp));

	if (pBool)
		{
		*pBool = pBool__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetStatusBar (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL pBool)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetStatusBar_ (__vtblIFacePtr, pBool));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetStatusText (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     char **statusText)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR statusText__AutoType = 0;

	if (statusText)
		*statusText = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetStatusText_ (__vtblIFacePtr,
	                                                    &statusText__AutoType));

	if (statusText)
		__caErrChk (CA_BSTRGetCString (statusText__AutoType, statusText));

Error:
	CA_FreeBSTR (statusText__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (statusText)
			{
			CA_FreeMemory (*statusText);
			*statusText = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetStatusText (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *statusText)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR statusText__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (statusText, &statusText__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetStatusText_ (__vtblIFacePtr,
	                                                    statusText__AutoType));

Error:
	CA_FreeBSTR (statusText__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetToolBar (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  long *value)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long value__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetToolBar_ (__vtblIFacePtr,
	                                                 &value__Temp));

	if (value)
		{
		*value = value__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetToolBar (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  long value)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetToolBar_ (__vtblIFacePtr, value));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetMenuBar (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL *value)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL value__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMenuBar_ (__vtblIFacePtr,
	                                                 &value__Temp));

	if (value)
		{
		*value = value__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetMenuBar (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL value)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMenuBar_ (__vtblIFacePtr, value));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetFullScreen (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL *pbFullScreen)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbFullScreen__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetFullScreen_ (__vtblIFacePtr,
	                                                    &pbFullScreen__Temp));

	if (pbFullScreen)
		{
		*pbFullScreen = pbFullScreen__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetFullScreen (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL pbFullScreen)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetFullScreen_ (__vtblIFacePtr,
	                                                    pbFullScreen));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2Navigate2 (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VARIANT URL, VARIANT flags,
                                                 VARIANT targetFrameName,
                                                 VARIANT postData,
                                                 VARIANT headers)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Navigate2_ (__vtblIFacePtr, &URL,
	                                                &flags, &targetFrameName,
	                                                &postData, &headers));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2QueryStatusWB (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     enum WebViewerEnum_OLECMDID cmdID,
                                                     enum WebViewerEnum_OLECMDF *pcmdf)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long pcmdf__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->QueryStatusWB_ (__vtblIFacePtr, cmdID,
	                                                    (long *)&pcmdf__Temp));

	if (pcmdf)
		{
		*pcmdf = pcmdf__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2ExecWB (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              enum WebViewerEnum_OLECMDID cmdID,
                                              enum WebViewerEnum_OLECMDEXECOPT cmdexecopt,
                                              VARIANT pvaIn, VARIANT *pvaOut)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->ExecWB_ (__vtblIFacePtr, cmdID,
	                                             cmdexecopt, &pvaIn, pvaOut));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvaOut)
			CA_VariantClear (pvaOut);
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2ShowBrowserBar (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      VARIANT pvaClsid,
                                                      VARIANT pvarShow,
                                                      VARIANT pvarSize)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->ShowBrowserBar_ (__vtblIFacePtr,
	                                                     &pvaClsid, &pvarShow,
	                                                     &pvarSize));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetReadyState (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     enum WebViewerEnum_tagREADYSTATE *plReadyState)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long plReadyState__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetReadyState_ (__vtblIFacePtr,
	                                                    (long *)&plReadyState__Temp));

	if (plReadyState)
		{
		*plReadyState = plReadyState__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetOffline (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL *pbOffline)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbOffline__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetOffline_ (__vtblIFacePtr,
	                                                 &pbOffline__Temp));

	if (pbOffline)
		{
		*pbOffline = pbOffline__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetOffline (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL pbOffline)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetOffline_ (__vtblIFacePtr, pbOffline));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetSilent (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VBOOL *pbSilent)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbSilent__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetSilent_ (__vtblIFacePtr,
	                                                &pbSilent__Temp));

	if (pbSilent)
		{
		*pbSilent = pbSilent__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetSilent (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VBOOL pbSilent)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetSilent_ (__vtblIFacePtr, pbSilent));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetRegisterAsBrowser (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VBOOL *pbRegister)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbRegister__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetRegisterAsBrowser_ (__vtblIFacePtr,
	                                                           &pbRegister__Temp));

	if (pbRegister)
		{
		*pbRegister = pbRegister__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetRegisterAsBrowser (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VBOOL pbRegister)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetRegisterAsBrowser_ (__vtblIFacePtr,
	                                                           pbRegister));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetRegisterAsDropTarget (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               VBOOL *pbRegister)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbRegister__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetRegisterAsDropTarget_ (__vtblIFacePtr,
	                                                              &pbRegister__Temp));

	if (pbRegister)
		{
		*pbRegister = pbRegister__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetRegisterAsDropTarget (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               VBOOL pbRegister)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetRegisterAsDropTarget_ (__vtblIFacePtr,
	                                                              pbRegister));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetTheaterMode (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      VBOOL *pbRegister)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pbRegister__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetTheaterMode_ (__vtblIFacePtr,
	                                                     &pbRegister__Temp));

	if (pbRegister)
		{
		*pbRegister = pbRegister__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetTheaterMode (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      VBOOL pbRegister)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetTheaterMode_ (__vtblIFacePtr,
	                                                     pbRegister));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetAddressBar (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL *value)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL value__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetAddressBar_ (__vtblIFacePtr,
	                                                    &value__Temp));

	if (value)
		{
		*value = value__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetAddressBar (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL value)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetAddressBar_ (__vtblIFacePtr, value));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2GetResizable (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL *value)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL value__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetResizable_ (__vtblIFacePtr,
	                                                   &value__Temp));

	if (value)
		{
		*value = value__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IWebBrowser2SetResizable (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL value)
{
	HRESULT __result = S_OK;
	WebViewer_IWebBrowser2_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IWebBrowser2, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetResizable_ (__vtblIFacePtr, value));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IWebBrowser2, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_NewIShellWindows (const char *server,
                                            int supportMultithreading,
                                            LCID locale, int reserved,
                                            CAObjHandle *objectHandle)
{
	HRESULT __result = S_OK;
	GUID clsid = {0x9BA05972, 0xF6A8, 0x11CF, 0xA4, 0x42, 0x0, 0xA0, 0xC9, 0xA,
	              0x8F, 0x39};

	__result = CA_CreateObjectByClassIdEx (&clsid, server,
	                                       &WebViewer_IID_IShellWindows,
	                                       supportMultithreading, locale,
	                                       reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC WebViewer_OpenIShellWindows (const char *fileName,
                                             const char *server,
                                             int supportMultithreading,
                                             LCID locale, int reserved,
                                             CAObjHandle *objectHandle)
{
	HRESULT __result = S_OK;
	GUID clsid = {0x9BA05972, 0xF6A8, 0x11CF, 0xA4, 0x42, 0x0, 0xA0, 0xC9, 0xA,
	              0x8F, 0x39};

	__result = CA_LoadObjectFromFileByClassIdEx (fileName, &clsid, server,
	                                             &WebViewer_IID_IShellWindows,
	                                             supportMultithreading, locale,
	                                             reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC WebViewer_ActiveIShellWindows (const char *server,
                                               int supportMultithreading,
                                               LCID locale, int reserved,
                                               CAObjHandle *objectHandle)
{
	HRESULT __result = S_OK;
	GUID clsid = {0x9BA05972, 0xF6A8, 0x11CF, 0xA4, 0x42, 0x0, 0xA0, 0xC9, 0xA,
	              0x8F, 0x39};

	__result = CA_GetActiveObjectByClassIdEx (&clsid, server,
	                                          &WebViewer_IID_IShellWindows,
	                                          supportMultithreading, locale,
	                                          reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC WebViewer_IShellWindowsGetCount (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 long *count)
{
	HRESULT __result = S_OK;
	WebViewer_IShellWindows_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long count__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellWindows, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetCount_ (__vtblIFacePtr,
	                                               &count__Temp));

	if (count)
		{
		*count = count__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellWindows, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellWindowsItem (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, VARIANT index,
                                             CAObjHandle *folder)
{
	HRESULT __result = S_OK;
	WebViewer_IShellWindows_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH folder__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (folder)
		*folder = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellWindows, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Item_ (__vtblIFacePtr, index,
	                                           &folder__AutoType));
	

	if (folder)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (folder__AutoType,
		                                             &IID_IDispatch,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, folder));
		folder__AutoType = 0;
		}

Error:
	if (folder__AutoType)
		folder__AutoType->lpVtbl->Release (folder__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (folder)
			{
			CA_DiscardObjHandle (*folder);
			*folder = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellWindows, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellWindows_NewEnum (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 LPUNKNOWN *ppunk)
{
	HRESULT __result = S_OK;
	WebViewer_IShellWindows_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPUNKNOWN ppunk__Temp = 0;

	if (ppunk)
		*ppunk = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellWindows, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->_NewEnum_ (__vtblIFacePtr,
	                                               &ppunk__Temp));

	if (ppunk)
		{
		*ppunk = ppunk__Temp;
		ppunk__Temp = 0;
		}

Error:
	if (ppunk__Temp)
		ppunk__Temp->lpVtbl->Release (ppunk__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppunk)
			{
			if (*ppunk)
				(*ppunk)->lpVtbl->Release (*ppunk);
			*ppunk = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellWindows, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellWindowsRegister (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 CAObjHandle pid, long HWND,
                                                 long swClass, long *plCookie)
{
	HRESULT __result = S_OK;
	WebViewer_IShellWindows_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH pid__AutoType = 0;
	long plCookie__Temp;

	if (pid)
		{
		__caErrChk (CA_GetInterfaceFromObjHandle (pid, &IID_IDispatch, 1,
	                                          &pid__AutoType, NULL));
		}

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellWindows, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Register_ (__vtblIFacePtr,
	                                               pid__AutoType, HWND,
	                                               swClass, &plCookie__Temp));

	if (plCookie)
		{
		*plCookie = plCookie__Temp;
		}

Error:
	if (pid__AutoType)
		pid__AutoType->lpVtbl->Release (pid__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellWindows, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellWindowsRegisterPending (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        long lThreadId,
                                                        VARIANT pvarloc,
                                                        VARIANT pvarlocRoot,
                                                        long swClass,
                                                        long *plCookie)
{
	HRESULT __result = S_OK;
	WebViewer_IShellWindows_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long plCookie__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellWindows, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->RegisterPending_ (__vtblIFacePtr,
	                                                      lThreadId, &pvarloc,
	                                                      &pvarlocRoot,
	                                                      swClass,
	                                                      &plCookie__Temp));

	if (plCookie)
		{
		*plCookie = plCookie__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellWindows, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellWindowsRevoke (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               long lCookie)
{
	HRESULT __result = S_OK;
	WebViewer_IShellWindows_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellWindows, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Revoke_ (__vtblIFacePtr, lCookie));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellWindows, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellWindowsOnNavigate (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   long lCookie, VARIANT pvarloc)
{
	HRESULT __result = S_OK;
	WebViewer_IShellWindows_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellWindows, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->OnNavigate_ (__vtblIFacePtr, lCookie,
	                                                 &pvarloc));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellWindows, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellWindowsOnActivated (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    long lCookie, VBOOL fActive)
{
	HRESULT __result = S_OK;
	WebViewer_IShellWindows_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellWindows, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->OnActivated_ (__vtblIFacePtr, lCookie,
	                                                  fActive));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellWindows, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellWindowsFindWindowSW (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VARIANT pvarloc,
                                                     VARIANT pvarlocRoot,
                                                     long swClass, long *pHWND,
                                                     long swfwOptions,
                                                     CAObjHandle *ppdispOut)
{
	HRESULT __result = S_OK;
	WebViewer_IShellWindows_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long pHWND__Temp;
	LPDISPATCH ppdispOut__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (ppdispOut)
		*ppdispOut = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellWindows, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->FindWindowSW_ (__vtblIFacePtr,
	                                                   &pvarloc, &pvarlocRoot,
	                                                   swClass, &pHWND__Temp,
	                                                   swfwOptions,
	                                                   &ppdispOut__AutoType));

	if (pHWND)
		{
		*pHWND = pHWND__Temp;
		}

	if (ppdispOut)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (ppdispOut__AutoType,
		                                             &IID_IDispatch,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, ppdispOut));
		ppdispOut__AutoType = 0;
		}

Error:
	if (ppdispOut__AutoType)
		ppdispOut__AutoType->lpVtbl->Release (ppdispOut__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppdispOut)
			{
			CA_DiscardObjHandle (*ppdispOut);
			*ppdispOut = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellWindows, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellWindowsOnCreated (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  long lCookie, LPUNKNOWN punk)
{
	HRESULT __result = S_OK;
	WebViewer_IShellWindows_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellWindows, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->OnCreated_ (__vtblIFacePtr, lCookie,
	                                                punk));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellWindows, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellWindowsProcessAttachDetch (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           VBOOL fAttach)
{
	HRESULT __result = S_OK;
	WebViewer_IShellWindows_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellWindows, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->ProcessAttachDetch_ (__vtblIFacePtr,
	                                                         fAttach));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellWindows, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_NewIShellUIHelper (const char *server,
                                             int supportMultithreading,
                                             LCID locale, int reserved,
                                             CAObjHandle *objectHandle)
{
	HRESULT __result = S_OK;
	GUID clsid = {0x64AB4BB7, 0x111E, 0x11D1, 0x8F, 0x79, 0x0, 0xC0, 0x4F,
	              0xC2, 0xFB, 0xE1};

	__result = CA_CreateObjectByClassIdEx (&clsid, server,
	                                       &WebViewer_IID_IShellUIHelper,
	                                       supportMultithreading, locale,
	                                       reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC WebViewer_OpenIShellUIHelper (const char *fileName,
                                              const char *server,
                                              int supportMultithreading,
                                              LCID locale, int reserved,
                                              CAObjHandle *objectHandle)
{
	HRESULT __result = S_OK;
	GUID clsid = {0x64AB4BB7, 0x111E, 0x11D1, 0x8F, 0x79, 0x0, 0xC0, 0x4F,
	              0xC2, 0xFB, 0xE1};

	__result = CA_LoadObjectFromFileByClassIdEx (fileName, &clsid, server,
	                                             &WebViewer_IID_IShellUIHelper,
	                                             supportMultithreading, locale,
	                                             reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC WebViewer_ActiveIShellUIHelper (const char *server,
                                                int supportMultithreading,
                                                LCID locale, int reserved,
                                                CAObjHandle *objectHandle)
{
	HRESULT __result = S_OK;
	GUID clsid = {0x64AB4BB7, 0x111E, 0x11D1, 0x8F, 0x79, 0x0, 0xC0, 0x4F,
	              0xC2, 0xFB, 0xE1};

	__result = CA_GetActiveObjectByClassIdEx (&clsid, server,
	                                          &WebViewer_IID_IShellUIHelper,
	                                          supportMultithreading, locale,
	                                          reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC WebViewer_IShellUIHelperResetFrstBootMode (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IShellUIHelper_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellUIHelper, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->ResetFrstBootMode_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellUIHelper, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellUIHelperResetSafeMode (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IShellUIHelper_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellUIHelper, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->ResetSafeMode_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellUIHelper, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellUIHelperRefreshOfflnDsktp (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IShellUIHelper_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellUIHelper, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->RefreshOfflnDsktp_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellUIHelper, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellUIHelperAddFavorite (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *URL,
                                                     VARIANT title)
{
	HRESULT __result = S_OK;
	WebViewer_IShellUIHelper_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR URL__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (URL, &URL__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellUIHelper, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->AddFavorite_ (__vtblIFacePtr,
	                                                  URL__AutoType, &title));

Error:
	CA_FreeBSTR (URL__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellUIHelper, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellUIHelperAddChannel (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    const char *URL)
{
	HRESULT __result = S_OK;
	WebViewer_IShellUIHelper_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR URL__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (URL, &URL__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellUIHelper, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->AddChannel_ (__vtblIFacePtr,
	                                                 URL__AutoType));

Error:
	CA_FreeBSTR (URL__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellUIHelper, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellUIHelperAddDsktpComponent (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           const char *URL,
                                                           const char *type,
                                                           VARIANT left,
                                                           VARIANT top,
                                                           VARIANT width,
                                                           VARIANT height)
{
	HRESULT __result = S_OK;
	WebViewer_IShellUIHelper_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR URL__AutoType = 0;
	BSTR type__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (URL, &URL__AutoType));
	__caErrChk (CA_CStringToBSTR (type, &type__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellUIHelper, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->AddDsktpComponent_ (__vtblIFacePtr,
	                                                        URL__AutoType,
	                                                        type__AutoType,
	                                                        &left, &top,
	                                                        &width, &height));

Error:
	CA_FreeBSTR (URL__AutoType);
	CA_FreeBSTR (type__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellUIHelper, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellUIHelperIsSubscribed (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      const char *URL,
                                                      VBOOL *pBool)
{
	HRESULT __result = S_OK;
	WebViewer_IShellUIHelper_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR URL__AutoType = 0;
	VBOOL pBool__Temp;

	__caErrChk (CA_CStringToBSTR (URL, &URL__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellUIHelper, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->IsSubscribed_ (__vtblIFacePtr,
	                                                   URL__AutoType,
	                                                   &pBool__Temp));

	if (pBool)
		{
		*pBool = pBool__Temp;
		}

Error:
	CA_FreeBSTR (URL__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellUIHelper, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellUIHelperNavigateAndFind (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         const char *URL,
                                                         const char *strQuery,
                                                         VARIANT varTargetFrame)
{
	HRESULT __result = S_OK;
	WebViewer_IShellUIHelper_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR URL__AutoType = 0;
	BSTR strQuery__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (URL, &URL__AutoType));
	__caErrChk (CA_CStringToBSTR (strQuery, &strQuery__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellUIHelper, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->NavigateAndFind_ (__vtblIFacePtr,
	                                                      URL__AutoType,
	                                                      strQuery__AutoType,
	                                                      &varTargetFrame));

Error:
	CA_FreeBSTR (URL__AutoType);
	CA_FreeBSTR (strQuery__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellUIHelper, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellUIHelperImportExportFavorites (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               VBOOL fImport,
                                                               const char *strImpExpPath)
{
	HRESULT __result = S_OK;
	WebViewer_IShellUIHelper_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR strImpExpPath__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (strImpExpPath, &strImpExpPath__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellUIHelper, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->ImportExportFavorites_ (__vtblIFacePtr,
	                                                            fImport,
	                                                            strImpExpPath__AutoType));

Error:
	CA_FreeBSTR (strImpExpPath__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellUIHelper, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellUIHelperAutoCompleteSaveForm (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              VARIANT form)
{
	HRESULT __result = S_OK;
	WebViewer_IShellUIHelper_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellUIHelper, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->AutoCompleteSaveForm_ (__vtblIFacePtr,
	                                                           &form));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellUIHelper, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellUIHelperAutoScan (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *strSearch,
                                                  const char *strFailureUrl,
                                                  VARIANT pvarTargetFrame)
{
	HRESULT __result = S_OK;
	WebViewer_IShellUIHelper_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR strSearch__AutoType = 0;
	BSTR strFailureUrl__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (strSearch, &strSearch__AutoType));
	__caErrChk (CA_CStringToBSTR (strFailureUrl, &strFailureUrl__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellUIHelper, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->AutoScan_ (__vtblIFacePtr,
	                                               strSearch__AutoType,
	                                               strFailureUrl__AutoType,
	                                               &pvarTargetFrame));

Error:
	CA_FreeBSTR (strSearch__AutoType);
	CA_FreeBSTR (strFailureUrl__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellUIHelper, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellUIHelperAutoCompleteAttach (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VARIANT reserved)
{
	HRESULT __result = S_OK;
	WebViewer_IShellUIHelper_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellUIHelper, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->AutoCompleteAttach_ (__vtblIFacePtr,
	                                                         &reserved));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellUIHelper, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellUIHelperShowBrowserUI (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       const char *bstrName,
                                                       VARIANT pvarIn,
                                                       VARIANT *pvarOut)
{
	HRESULT __result = S_OK;
	WebViewer_IShellUIHelper_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR bstrName__AutoType = 0;
	VARIANT pvarOut__Temp;

	if (pvarOut)
		CA_VariantSetEmpty (pvarOut);
	CA_VariantSetEmpty (&pvarOut__Temp);

	__caErrChk (CA_CStringToBSTR (bstrName, &bstrName__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellUIHelper, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->ShowBrowserUI_ (__vtblIFacePtr,
	                                                    bstrName__AutoType,
	                                                    &pvarIn,
	                                                    &pvarOut__Temp));

	if (pvarOut)
		{
		*pvarOut = pvarOut__Temp;
		CA_VariantSetEmpty (&pvarOut__Temp);
		}

Error:
	CA_FreeBSTR (bstrName__AutoType);
	CA_VariantClear (&pvarOut__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvarOut)
			CA_VariantClear (pvarOut);
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellUIHelper, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_NewIShellNameSpace (const char *server,
                                              int supportMultithreading,
                                              LCID locale, int reserved,
                                              CAObjHandle *objectHandle)
{
	HRESULT __result = S_OK;
	GUID clsid = {0x55136805, 0xB2DE, 0x11D1, 0xB9, 0xF2, 0x0, 0xA0, 0xC9,
	              0x8B, 0xC5, 0x47};

	__result = CA_CreateObjectByClassIdEx (&clsid, server,
	                                       &WebViewer_IID_IShellNameSpace,
	                                       supportMultithreading, locale,
	                                       reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC WebViewer_OpenIShellNameSpace (const char *fileName,
                                               const char *server,
                                               int supportMultithreading,
                                               LCID locale, int reserved,
                                               CAObjHandle *objectHandle)
{
	HRESULT __result = S_OK;
	GUID clsid = {0x55136805, 0xB2DE, 0x11D1, 0xB9, 0xF2, 0x0, 0xA0, 0xC9,
	              0x8B, 0xC5, 0x47};

	__result = CA_LoadObjectFromFileByClassIdEx (fileName, &clsid, server,
	                                             &WebViewer_IID_IShellNameSpace,
	                                             supportMultithreading, locale,
	                                             reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC WebViewer_ActiveIShellNameSpace (const char *server,
                                                 int supportMultithreading,
                                                 LCID locale, int reserved,
                                                 CAObjHandle *objectHandle)
{
	HRESULT __result = S_OK;
	GUID clsid = {0x55136805, 0xB2DE, 0x11D1, 0xB9, 0xF2, 0x0, 0xA0, 0xC9,
	              0x8B, 0xC5, 0x47};

	__result = CA_GetActiveObjectByClassIdEx (&clsid, server,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          supportMultithreading, locale,
	                                          reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceMoveSelectionUp (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->MoveSelectionUp_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceMoveSelectionDown (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->MoveSelectionDown_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceResetSort (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->ResetSort_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceNewFolder (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->NewFolder_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceSynchronize (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Synchronize_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceImport (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Import_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceExport (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Export_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceInvokeContextMenuCommand (CAObjHandle objectHandle,
                                                                   ERRORINFO *errorInfo,
                                                                   const char *strCommand)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR strCommand__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (strCommand, &strCommand__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->InvokeContextMenuCommand_ (__vtblIFacePtr,
	                                                               strCommand__AutoType));

Error:
	CA_FreeBSTR (strCommand__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceMoveSelectionTo (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->MoveSelectionTo_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetSubscriptionsEnabled (CAObjHandle objectHandle,
                                                                  ERRORINFO *errorInfo,
                                                                  VBOOL *pBool)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pBool__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetSubscriptionsEnabled_ (__vtblIFacePtr,
	                                                              &pBool__Temp));

	if (pBool)
		{
		*pBool = pBool__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceCreateSubscriptionForSelection (CAObjHandle objectHandle,
                                                                         ERRORINFO *errorInfo,
                                                                         VBOOL *pBool)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pBool__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->CreateSubscriptionForSelection_ (__vtblIFacePtr,
	                                                                     &pBool__Temp));

	if (pBool)
		{
		*pBool = pBool__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceDeleteSubscriptionForSelection (CAObjHandle objectHandle,
                                                                         ERRORINFO *errorInfo,
                                                                         VBOOL *pBool)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VBOOL pBool__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->DeleteSubscriptionForSelection_ (__vtblIFacePtr,
	                                                                     &pBool__Temp));

	if (pBool)
		{
		*pBool = pBool__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetRoot (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *bstrFullPath)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR bstrFullPath__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (bstrFullPath, &bstrFullPath__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetRoot_ (__vtblIFacePtr,
	                                              bstrFullPath__AutoType));

Error:
	CA_FreeBSTR (bstrFullPath__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetEnumOptions (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         long *pgrfEnumFlags)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long pgrfEnumFlags__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetEnumOptions_ (__vtblIFacePtr,
	                                                     &pgrfEnumFlags__Temp));

	if (pgrfEnumFlags)
		{
		*pgrfEnumFlags = pgrfEnumFlags__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetEnumOptions (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         long pgrfEnumFlags)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetEnumOptions_ (__vtblIFacePtr,
	                                                     pgrfEnumFlags));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetSelectedItem (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          CAObjHandle *pItem)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH pItem__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (pItem)
		*pItem = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetSelectedItem_ (__vtblIFacePtr,
	                                                      &pItem__AutoType));
	

	if (pItem)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (pItem__AutoType,
		                                             &IID_IDispatch,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, pItem));
		pItem__AutoType = 0;
		}

Error:
	if (pItem__AutoType)
		pItem__AutoType->lpVtbl->Release (pItem__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pItem)
			{
			CA_DiscardObjHandle (*pItem);
			*pItem = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetSelectedItem (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          CAObjHandle pItem)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH pItem__AutoType = 0;

	if (pItem)
		{
		__caErrChk (CA_GetInterfaceFromObjHandle (pItem, &IID_IDispatch, 1,
	                                          &pItem__AutoType, NULL));
		}

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetSelectedItem_ (__vtblIFacePtr,
	                                                      pItem__AutoType));

Error:
	if (pItem__AutoType)
		pItem__AutoType->lpVtbl->Release (pItem__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetRoot (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VARIANT *pvar)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	VARIANT pvar__Temp;

	if (pvar)
		CA_VariantSetEmpty (pvar);
	CA_VariantSetEmpty (&pvar__Temp);

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetRoot_ (__vtblIFacePtr, &pvar__Temp));

	if (pvar)
		{
		*pvar = pvar__Temp;
		CA_VariantSetEmpty (&pvar__Temp);
		}

Error:
	CA_VariantClear (&pvar__Temp);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (pvar)
			CA_VariantClear (pvar);
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetRootProp (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      VARIANT pvar)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetRootProp_ (__vtblIFacePtr, pvar));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetDepth (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   long *piDepth)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long piDepth__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetDepth_ (__vtblIFacePtr,
	                                               &piDepth__Temp));

	if (piDepth)
		{
		*piDepth = piDepth__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetDepth (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   long piDepth)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetDepth_ (__vtblIFacePtr, piDepth));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetMode (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  long *puMode)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long puMode__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetMode_ (__vtblIFacePtr,
	                                              &puMode__Temp));

	if (puMode)
		{
		*puMode = puMode__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetMode (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  long puMode)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetMode_ (__vtblIFacePtr, puMode));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetFlags (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   long *pdwFlags)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long pdwFlags__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetFlags_ (__vtblIFacePtr,
	                                               &pdwFlags__Temp));

	if (pdwFlags)
		{
		*pdwFlags = pdwFlags__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetFlags (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   long pdwFlags)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetFlags_ (__vtblIFacePtr, pdwFlags));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetTVFlags (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     long dwFlags)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetTVFlags_ (__vtblIFacePtr, dwFlags));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetTVFlags (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     long *dwFlags)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long dwFlags__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetTVFlags_ (__vtblIFacePtr,
	                                                 &dwFlags__Temp));

	if (dwFlags)
		{
		*dwFlags = dwFlags__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetColumns (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     char **bstrColumns)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR bstrColumns__AutoType = 0;

	if (bstrColumns)
		*bstrColumns = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetColumns_ (__vtblIFacePtr,
	                                                 &bstrColumns__AutoType));

	if (bstrColumns)
		__caErrChk (CA_BSTRGetCString (bstrColumns__AutoType, bstrColumns));

Error:
	CA_FreeBSTR (bstrColumns__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (bstrColumns)
			{
			CA_FreeMemory (*bstrColumns);
			*bstrColumns = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetColumns (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *bstrColumns)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR bstrColumns__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (bstrColumns, &bstrColumns__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetColumns_ (__vtblIFacePtr,
	                                                 bstrColumns__AutoType));

Error:
	CA_FreeBSTR (bstrColumns__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetCountViewTypes (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            long *piTypes)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	long piTypes__Temp;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->GetCountViewTypes_ (__vtblIFacePtr,
	                                                        &piTypes__Temp));

	if (piTypes)
		{
		*piTypes = piTypes__Temp;
		}

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetViewType (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      long iType)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SetViewType_ (__vtblIFacePtr, iType));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceSelectedItems (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        CAObjHandle *ppid)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	LPDISPATCH ppid__AutoType = 0;
	LCID __locale;
	int __supportMultithreading;

	if (ppid)
		*ppid = 0;

	__caErrChk (CA_GetLocale (objectHandle, &__locale));
	__caErrChk (CA_GetSupportForMultithreading (objectHandle,
	                                            &__supportMultithreading));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SelectedItems_ (__vtblIFacePtr,
	                                                    &ppid__AutoType));
	

	if (ppid)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (ppid__AutoType,
		                                             &IID_IDispatch,
		                                             __supportMultithreading,
		                                             __locale, 0, 0, ppid));
		ppid__AutoType = 0;
		}

Error:
	if (ppid__AutoType)
		ppid__AutoType->lpVtbl->Release (ppid__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		if (ppid)
			{
			CA_DiscardObjHandle (*ppid);
			*ppid = 0;
			}
		}
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceExpand (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VARIANT var, long iDepth)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Expand_ (__vtblIFacePtr, var, iDepth));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC WebViewer_IShellNameSpaceUnselectAll (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo)
{
	HRESULT __result = S_OK;
	WebViewer_IShellNameSpace_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &WebViewer_IID_IShellNameSpace,
	                                          0, &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->UnselectAll_ (__vtblIFacePtr));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &WebViewer_IID_IShellNameSpace, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnStatusTextChange_EventVTableFunc (void *thisPtr,
                                                                          BSTR text)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnStatusTextChange_CallbackType __callbackFunction;
	char * text__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 0, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (text, &text__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, text__CType);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (text__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnProgressChange_EventVTableFunc (void *thisPtr,
                                                                        long progress,
                                                                        long progressMax)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnProgressChange_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 1, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, progress,
		                               progressMax);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnCommandStateChange_EventVTableFunc (void *thisPtr,
                                                                            long command,
                                                                            VBOOL enable)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnCommandStateChange_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 2, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, command,
		                               enable);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnDownloadBegin_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnDownloadBegin_CallbackType __callbackFunction;
	
	

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

static void CVIFUNC DWebBrwsrEvnts2RegOnDownloadComplete_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnDownloadComplete_CallbackType __callbackFunction;
	
	

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

static void CVIFUNC DWebBrwsrEvnts2RegOnTitleChange_EventVTableFunc (void *thisPtr,
                                                                     BSTR text)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnTitleChange_CallbackType __callbackFunction;
	char * text__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 5, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (text, &text__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, text__CType);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (text__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnPropertyChange_EventVTableFunc (void *thisPtr,
                                                                        BSTR szProperty)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnPropertyChange_CallbackType __callbackFunction;
	char * szProperty__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 6, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (szProperty, &szProperty__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData,
		                               szProperty__CType);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (szProperty__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnBeforeNavigate2_EventVTableFunc (void *thisPtr,
                                                                         LPDISPATCH pDisp,
                                                                         VARIANT *URL,
                                                                         VARIANT *flags,
                                                                         VARIANT *targetFrameName,
                                                                         VARIANT *postData,
                                                                         VARIANT *headers,
                                                                         VBOOL *cancel)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnBeforeNavigate2_CallbackType __callbackFunction;
	CAObjHandle pDisp__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 7, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (pDisp, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 1,
		                                             &pDisp__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, pDisp__CType,
		                               URL, flags, targetFrameName, postData, headers,
		                               cancel);
	
		__caErrChk (__result);
		
		}
Error:
	CA_DiscardObjHandle (pDisp__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnNewWindow2_EventVTableFunc (void *thisPtr,
                                                                    LPDISPATCH *ppDisp,
                                                                    VBOOL *cancel)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnNewWindow2_CallbackType __callbackFunction;
	CAObjHandle ppDisp__CType = 0;
	LPDISPATCH ppDisp__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 8, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*ppDisp, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &ppDisp__CType));
		*ppDisp = 0;
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &ppDisp__CType,
		                               cancel);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (ppDisp__CType, &IID_IDispatch, 1,
		                                          &ppDisp__AutoType, NULL));
		*ppDisp = ppDisp__AutoType;
		ppDisp__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (ppDisp__CType);
	if (ppDisp__AutoType)
		ppDisp__AutoType->lpVtbl->Release (ppDisp__AutoType);

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnNavigateComplete2_EventVTableFunc (void *thisPtr,
                                                                           LPDISPATCH pDisp,
                                                                           VARIANT *URL)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnNavigateComplete2_CallbackType __callbackFunction;
	CAObjHandle pDisp__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 9, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (pDisp, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 1,
		                                             &pDisp__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, pDisp__CType,
		                               URL);
	
		__caErrChk (__result);
		
		}
Error:
	CA_DiscardObjHandle (pDisp__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnDocumentComplete_EventVTableFunc (void *thisPtr,
                                                                          LPDISPATCH pDisp,
                                                                          VARIANT *URL)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnDocumentComplete_CallbackType __callbackFunction;
	CAObjHandle pDisp__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 10, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (pDisp, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 1,
		                                             &pDisp__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, pDisp__CType,
		                               URL);
	
		__caErrChk (__result);
		
		}
Error:
	CA_DiscardObjHandle (pDisp__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnOnQuit_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnOnQuit_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 11, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnOnVisible_EventVTableFunc (void *thisPtr,
                                                                   VBOOL visible)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnOnVisible_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 12, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, visible);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnOnToolBar_EventVTableFunc (void *thisPtr,
                                                                   VBOOL toolBar)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnOnToolBar_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 13, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, toolBar);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnOnMenuBar_EventVTableFunc (void *thisPtr,
                                                                   VBOOL menuBar)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnOnMenuBar_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 14, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, menuBar);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnOnStatusBar_EventVTableFunc (void *thisPtr,
                                                                     VBOOL statusBar)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnOnStatusBar_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 15, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, statusBar);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnOnFullScreen_EventVTableFunc (void *thisPtr,
                                                                      VBOOL fullScreen)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnOnFullScreen_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 16, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, fullScreen);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnOnTheaterMode_EventVTableFunc (void *thisPtr,
                                                                       VBOOL theaterMode)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnOnTheaterMode_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 17, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, theaterMode);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnWindowSetResizable_EventVTableFunc (void *thisPtr,
                                                                            VBOOL resizable)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnWindowSetResizable_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 18, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, resizable);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnWindowSetLeft_EventVTableFunc (void *thisPtr,
                                                                       long left)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnWindowSetLeft_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 19, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, left);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnWindowSetTop_EventVTableFunc (void *thisPtr,
                                                                      long top)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnWindowSetTop_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 20, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, top);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnWindowSetWidth_EventVTableFunc (void *thisPtr,
                                                                        long width)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnWindowSetWidth_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 21, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, width);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnWindowSetHeight_EventVTableFunc (void *thisPtr,
                                                                         long height)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnWindowSetHeight_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 22, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, height);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnWindowClosing_EventVTableFunc (void *thisPtr,
                                                                       VBOOL isChildWindow,
                                                                       VBOOL *cancel)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnWindowClosing_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 23, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, isChildWindow,
		                               cancel);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnClientToHostWindow_EventVTableFunc (void *thisPtr,
                                                                            long *CX,
                                                                            long *CY)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnClientToHostWindow_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 24, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, CX, CY);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnSetSecureLockIcon_EventVTableFunc (void *thisPtr,
                                                                           long secureLockIcon)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnSetSecureLockIcon_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 25, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, secureLockIcon);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnFileDownload_EventVTableFunc (void *thisPtr,
                                                                      VBOOL *cancel)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnFileDownload_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 26, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, cancel);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnNavigateError_EventVTableFunc (void *thisPtr,
                                                                       LPDISPATCH pDisp,
                                                                       VARIANT *URL,
                                                                       VARIANT *frame,
                                                                       VARIANT *statusCode,
                                                                       VBOOL *cancel)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnNavigateError_CallbackType __callbackFunction;
	CAObjHandle pDisp__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 27, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (pDisp, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 1,
		                                             &pDisp__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, pDisp__CType,
		                               URL, frame, statusCode, cancel);
	
		__caErrChk (__result);
		
		}
Error:
	CA_DiscardObjHandle (pDisp__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnPrintTemplateInstantiation_EventVTableFunc (void *thisPtr,
                                                                                    LPDISPATCH pDisp)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnPrintTemplateInstantiation_CallbackType __callbackFunction;
	CAObjHandle pDisp__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 28, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (pDisp, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 1,
		                                             &pDisp__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, pDisp__CType);
	
		__caErrChk (__result);
		
		}
Error:
	CA_DiscardObjHandle (pDisp__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnPrintTemplateTeardown_EventVTableFunc (void *thisPtr,
                                                                               LPDISPATCH pDisp)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnPrintTemplateTeardown_CallbackType __callbackFunction;
	CAObjHandle pDisp__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 29, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (pDisp, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 1,
		                                             &pDisp__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, pDisp__CType);
	
		__caErrChk (__result);
		
		}
Error:
	CA_DiscardObjHandle (pDisp__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnUpdatePageStatus_EventVTableFunc (void *thisPtr,
                                                                          LPDISPATCH pDisp,
                                                                          VARIANT *nPage,
                                                                          VARIANT *fDone)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnUpdatePageStatus_CallbackType __callbackFunction;
	CAObjHandle pDisp__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 30, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (pDisp, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 1,
		                                             &pDisp__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, pDisp__CType,
		                               nPage, fDone);
	
		__caErrChk (__result);
		
		}
Error:
	CA_DiscardObjHandle (pDisp__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnPrivacyImpactedStateChange_EventVTableFunc (void *thisPtr,
                                                                                    VBOOL bImpacted)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnPrivacyImpactedStateChange_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 31, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, bImpacted);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvnts2RegOnNewWindow3_EventVTableFunc (void *thisPtr,
                                                                    LPDISPATCH *ppDisp,
                                                                    VBOOL *cancel,
                                                                    long dwFlags,
                                                                    BSTR bstrUrlContext,
                                                                    BSTR bstrUrl)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvnts2RegOnNewWindow3_CallbackType __callbackFunction;
	CAObjHandle ppDisp__CType = 0;
	char * bstrUrlContext__CType = 0;
	char * bstrUrl__CType = 0;
	LPDISPATCH ppDisp__AutoType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 32, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_CreateObjHandleFromInterface (*ppDisp, &IID_IDispatch, 0,
		                                             LOCALE_NEUTRAL, 0, 0,
		                                             &ppDisp__CType));
		*ppDisp = 0;
		__caErrChk (CA_BSTRGetCString (bstrUrlContext, &bstrUrlContext__CType));
		__caErrChk (CA_BSTRGetCString (bstrUrl, &bstrUrl__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, &ppDisp__CType,
		                               cancel, dwFlags, bstrUrlContext__CType,
		                               bstrUrl__CType);
	
		__caErrChk (__result);
	
		__caErrChk (CA_GetInterfaceFromObjHandle (ppDisp__CType, &IID_IDispatch, 1,
		                                          &ppDisp__AutoType, NULL));
		*ppDisp = ppDisp__AutoType;
		ppDisp__AutoType = 0;
		
		}
Error:
	CA_DiscardObjHandle (ppDisp__CType);
	if (ppDisp__AutoType)
		ppDisp__AutoType->lpVtbl->Release (ppDisp__AutoType);
	CA_FreeMemory (bstrUrlContext__CType);
	CA_FreeMemory (bstrUrl__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvntsRegOnBeforeNavigate_EventVTableFunc (void *thisPtr,
                                                                       BSTR URL,
                                                                       long flags,
                                                                       BSTR targetFrameName,
                                                                       VARIANT *postData,
                                                                       BSTR headers,
                                                                       VBOOL *cancel)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnBeforeNavigate_CallbackType __callbackFunction;
	char * URL__CType = 0;
	char * targetFrameName__CType = 0;
	char * headers__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 0, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (URL, &URL__CType));
		__caErrChk (CA_BSTRGetCString (targetFrameName, &targetFrameName__CType));
		__caErrChk (CA_BSTRGetCString (headers, &headers__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, URL__CType,
		                               flags, targetFrameName__CType, postData,
		                               headers__CType, cancel);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (URL__CType);
	CA_FreeMemory (targetFrameName__CType);
	CA_FreeMemory (headers__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvntsRegOnNavigateComplete_EventVTableFunc (void *thisPtr,
                                                                         BSTR URL)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnNavigateComplete_CallbackType __callbackFunction;
	char * URL__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 1, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (URL, &URL__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, URL__CType);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (URL__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvntsRegOnStatusTextChange_EventVTableFunc (void *thisPtr,
                                                                         BSTR text)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnStatusTextChange_CallbackType __callbackFunction;
	char * text__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 2, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (text, &text__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, text__CType);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (text__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvntsRegOnProgressChange_EventVTableFunc (void *thisPtr,
                                                                       long progress,
                                                                       long progressMax)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnProgressChange_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 3, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, progress,
		                               progressMax);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvntsRegOnDownloadComplete_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnDownloadComplete_CallbackType __callbackFunction;
	
	

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

static void CVIFUNC DWebBrwsrEvntsRegOnCommandStateChange_EventVTableFunc (void *thisPtr,
                                                                           long command,
                                                                           VBOOL enable)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnCommandStateChange_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 5, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, command,
		                               enable);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvntsRegOnDownloadBegin_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnDownloadBegin_CallbackType __callbackFunction;
	
	

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

static void CVIFUNC DWebBrwsrEvntsRegOnNewWindow_EventVTableFunc (void *thisPtr,
                                                                  BSTR URL,
                                                                  long flags,
                                                                  BSTR targetFrameName,
                                                                  VARIANT *postData,
                                                                  BSTR headers,
                                                                  VBOOL *processed)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnNewWindow_CallbackType __callbackFunction;
	char * URL__CType = 0;
	char * targetFrameName__CType = 0;
	char * headers__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 7, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (URL, &URL__CType));
		__caErrChk (CA_BSTRGetCString (targetFrameName, &targetFrameName__CType));
		__caErrChk (CA_BSTRGetCString (headers, &headers__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, URL__CType,
		                               flags, targetFrameName__CType, postData,
		                               headers__CType, processed);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (URL__CType);
	CA_FreeMemory (targetFrameName__CType);
	CA_FreeMemory (headers__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvntsRegOnTitleChange_EventVTableFunc (void *thisPtr,
                                                                    BSTR text)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnTitleChange_CallbackType __callbackFunction;
	char * text__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 8, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (text, &text__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, text__CType);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (text__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvntsRegOnFrameBeforeNavigate_EventVTableFunc (void *thisPtr,
                                                                            BSTR URL,
                                                                            long flags,
                                                                            BSTR targetFrameName,
                                                                            VARIANT *postData,
                                                                            BSTR headers,
                                                                            VBOOL *cancel)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnFrameBeforeNavigate_CallbackType __callbackFunction;
	char * URL__CType = 0;
	char * targetFrameName__CType = 0;
	char * headers__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 9, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (URL, &URL__CType));
		__caErrChk (CA_BSTRGetCString (targetFrameName, &targetFrameName__CType));
		__caErrChk (CA_BSTRGetCString (headers, &headers__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, URL__CType,
		                               flags, targetFrameName__CType, postData,
		                               headers__CType, cancel);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (URL__CType);
	CA_FreeMemory (targetFrameName__CType);
	CA_FreeMemory (headers__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvntsRegOnFrameNavigateComplete_EventVTableFunc (void *thisPtr,
                                                                              BSTR URL)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnFrameNavigateComplete_CallbackType __callbackFunction;
	char * URL__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 10, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (URL, &URL__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, URL__CType);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (URL__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvntsRegOnFrameNewWindow_EventVTableFunc (void *thisPtr,
                                                                       BSTR URL,
                                                                       long flags,
                                                                       BSTR targetFrameName,
                                                                       VARIANT *postData,
                                                                       BSTR headers,
                                                                       VBOOL *processed)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnFrameNewWindow_CallbackType __callbackFunction;
	char * URL__CType = 0;
	char * targetFrameName__CType = 0;
	char * headers__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 11, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (URL, &URL__CType));
		__caErrChk (CA_BSTRGetCString (targetFrameName, &targetFrameName__CType));
		__caErrChk (CA_BSTRGetCString (headers, &headers__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, URL__CType,
		                               flags, targetFrameName__CType, postData,
		                               headers__CType, processed);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (URL__CType);
	CA_FreeMemory (targetFrameName__CType);
	CA_FreeMemory (headers__CType);

	return;
}

static void CVIFUNC DWebBrwsrEvntsRegOnQuit_EventVTableFunc (void *thisPtr,
                                                             VBOOL *cancel)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnQuit_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 12, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, cancel);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvntsRegOnWindowMove_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnWindowMove_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 13, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvntsRegOnWindowResize_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnWindowResize_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 14, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvntsRegOnWindowActivate_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnWindowActivate_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 15, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DWebBrwsrEvntsRegOnPropertyChange_EventVTableFunc (void *thisPtr,
                                                                       BSTR property)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DWebBrwsrEvntsRegOnPropertyChange_CallbackType __callbackFunction;
	char * property__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 16, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (property, &property__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData,
		                               property__CType);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (property__CType);

	return;
}

static void CVIFUNC DShellWindowsEvntsRegOnWindowRegistered_EventVTableFunc (void *thisPtr,
                                                                             long lCookie)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DShellWindowsEvntsRegOnWindowRegistered_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 0, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, lCookie);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DShellWindowsEvntsRegOnWindowRevoked_EventVTableFunc (void *thisPtr,
                                                                          long lCookie)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DShellWindowsEvntsRegOnWindowRevoked_CallbackType __callbackFunction;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 1, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, lCookie);
	
		__caErrChk (__result);
		
		}
Error:

	return;
}

static void CVIFUNC DShellNameSpaceEventsRegOnFavoritesSelectionChange_EventVTableFunc (void *thisPtr,
                                                                                        long cItems,
                                                                                        long hItem,
                                                                                        BSTR strName,
                                                                                        BSTR strUrl,
                                                                                        long cVisits,
                                                                                        BSTR strDate,
                                                                                        long fAvailableOffline)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DShellNameSpaceEventsRegOnFavoritesSelectionChange_CallbackType __callbackFunction;
	char * strName__CType = 0;
	char * strUrl__CType = 0;
	char * strDate__CType = 0;
	
	

	__caErrChk (CA_GetEventCallback (thisPtr, 0, &__callbackFunction,
	                                 &__callbackData, &__serverObjHandle));

	if (__callbackFunction != NULL)
		{
		__caErrChk (CA_BSTRGetCString (strName, &strName__CType));
		__caErrChk (CA_BSTRGetCString (strUrl, &strUrl__CType));
		__caErrChk (CA_BSTRGetCString (strDate, &strDate__CType));
	
		__result = __callbackFunction (__serverObjHandle, __callbackData, cItems, hItem,
		                               strName__CType, strUrl__CType, cVisits,
		                               strDate__CType, fAvailableOffline);
	
		__caErrChk (__result);
		
		}
Error:
	CA_FreeMemory (strName__CType);
	CA_FreeMemory (strUrl__CType);
	CA_FreeMemory (strDate__CType);

	return;
}

static void CVIFUNC DShellNameSpaceEventsRegOnSelectionChange_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DShellNameSpaceEventsRegOnSelectionChange_CallbackType __callbackFunction;
	
	

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

static void CVIFUNC DShellNameSpaceEventsRegOnDoubleClick_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DShellNameSpaceEventsRegOnDoubleClick_CallbackType __callbackFunction;
	
	

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

static void CVIFUNC DShellNameSpaceEventsRegOnInitialized_EventVTableFunc (void *thisPtr)
{
	HRESULT __result = S_OK;
	void * __callbackData;
	CAObjHandle __serverObjHandle;
	DShellNameSpaceEventsRegOnInitialized_CallbackType __callbackFunction;
	
	

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

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnStatusTextChange (CAObjHandle serverObject,
                                                                DWebBrwsrEvnts2RegOnStatusTextChange_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn, 0,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnProgressChange (CAObjHandle serverObject,
                                                              DWebBrwsrEvnts2RegOnProgressChange_CallbackType callbackFunction,
                                                              void *callbackData,
                                                              int enableCallbacks,
                                                              int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn, 1,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnCommandStateChange (CAObjHandle serverObject,
                                                                  DWebBrwsrEvnts2RegOnCommandStateChange_CallbackType callbackFunction,
                                                                  void *callbackData,
                                                                  int enableCallbacks,
                                                                  int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn, 2,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnDownloadBegin (CAObjHandle serverObject,
                                                             DWebBrwsrEvnts2RegOnDownloadBegin_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn, 3,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnDownloadComplete (CAObjHandle serverObject,
                                                                DWebBrwsrEvnts2RegOnDownloadComplete_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn, 4,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnTitleChange (CAObjHandle serverObject,
                                                           DWebBrwsrEvnts2RegOnTitleChange_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn, 5,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnPropertyChange (CAObjHandle serverObject,
                                                              DWebBrwsrEvnts2RegOnPropertyChange_CallbackType callbackFunction,
                                                              void *callbackData,
                                                              int enableCallbacks,
                                                              int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn, 6,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnBeforeNavigate2 (CAObjHandle serverObject,
                                                               DWebBrwsrEvnts2RegOnBeforeNavigate2_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn, 7,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnNewWindow2 (CAObjHandle serverObject,
                                                          DWebBrwsrEvnts2RegOnNewWindow2_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn, 8,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnNavigateComplete2 (CAObjHandle serverObject,
                                                                 DWebBrwsrEvnts2RegOnNavigateComplete2_CallbackType callbackFunction,
                                                                 void *callbackData,
                                                                 int enableCallbacks,
                                                                 int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn, 9,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnDocumentComplete (CAObjHandle serverObject,
                                                                DWebBrwsrEvnts2RegOnDocumentComplete_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     10, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnOnQuit (CAObjHandle serverObject,
                                                      DWebBrwsrEvnts2RegOnOnQuit_CallbackType callbackFunction,
                                                      void *callbackData,
                                                      int enableCallbacks,
                                                      int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     11, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnOnVisible (CAObjHandle serverObject,
                                                         DWebBrwsrEvnts2RegOnOnVisible_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     12, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnOnToolBar (CAObjHandle serverObject,
                                                         DWebBrwsrEvnts2RegOnOnToolBar_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     13, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnOnMenuBar (CAObjHandle serverObject,
                                                         DWebBrwsrEvnts2RegOnOnMenuBar_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     14, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnOnStatusBar (CAObjHandle serverObject,
                                                           DWebBrwsrEvnts2RegOnOnStatusBar_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     15, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnOnFullScreen (CAObjHandle serverObject,
                                                            DWebBrwsrEvnts2RegOnOnFullScreen_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     16, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnOnTheaterMode (CAObjHandle serverObject,
                                                             DWebBrwsrEvnts2RegOnOnTheaterMode_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     17, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnWindowSetResizable (CAObjHandle serverObject,
                                                                  DWebBrwsrEvnts2RegOnWindowSetResizable_CallbackType callbackFunction,
                                                                  void *callbackData,
                                                                  int enableCallbacks,
                                                                  int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     18, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnWindowSetLeft (CAObjHandle serverObject,
                                                             DWebBrwsrEvnts2RegOnWindowSetLeft_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     19, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnWindowSetTop (CAObjHandle serverObject,
                                                            DWebBrwsrEvnts2RegOnWindowSetTop_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     20, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnWindowSetWidth (CAObjHandle serverObject,
                                                              DWebBrwsrEvnts2RegOnWindowSetWidth_CallbackType callbackFunction,
                                                              void *callbackData,
                                                              int enableCallbacks,
                                                              int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     21, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnWindowSetHeight (CAObjHandle serverObject,
                                                               DWebBrwsrEvnts2RegOnWindowSetHeight_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     22, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnWindowClosing (CAObjHandle serverObject,
                                                             DWebBrwsrEvnts2RegOnWindowClosing_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     23, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnClientToHostWindow (CAObjHandle serverObject,
                                                                  DWebBrwsrEvnts2RegOnClientToHostWindow_CallbackType callbackFunction,
                                                                  void *callbackData,
                                                                  int enableCallbacks,
                                                                  int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     24, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnSetSecureLockIcon (CAObjHandle serverObject,
                                                                 DWebBrwsrEvnts2RegOnSetSecureLockIcon_CallbackType callbackFunction,
                                                                 void *callbackData,
                                                                 int enableCallbacks,
                                                                 int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     25, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnFileDownload (CAObjHandle serverObject,
                                                            DWebBrwsrEvnts2RegOnFileDownload_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     26, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnNavigateError (CAObjHandle serverObject,
                                                             DWebBrwsrEvnts2RegOnNavigateError_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     27, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnPrintTemplateInstantiation (CAObjHandle serverObject,
                                                                          DWebBrwsrEvnts2RegOnPrintTemplateInstantiation_CallbackType callbackFunction,
                                                                          void *callbackData,
                                                                          int enableCallbacks,
                                                                          int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     28, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnPrintTemplateTeardown (CAObjHandle serverObject,
                                                                     DWebBrwsrEvnts2RegOnPrintTemplateTeardown_CallbackType callbackFunction,
                                                                     void *callbackData,
                                                                     int enableCallbacks,
                                                                     int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     29, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnUpdatePageStatus (CAObjHandle serverObject,
                                                                DWebBrwsrEvnts2RegOnUpdatePageStatus_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     30, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnPrivacyImpactedStateChange (CAObjHandle serverObject,
                                                                          DWebBrwsrEvnts2RegOnPrivacyImpactedStateChange_CallbackType callbackFunction,
                                                                          void *callbackData,
                                                                          int enableCallbacks,
                                                                          int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     31, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnNewWindow3 (CAObjHandle serverObject,
                                                          DWebBrwsrEvnts2RegOnNewWindow3_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts2_CAEventClassDefn,
	                                     32, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnBeforeNavigate (CAObjHandle serverObject,
                                                             DWebBrwsrEvntsRegOnBeforeNavigate_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 0,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnNavigateComplete (CAObjHandle serverObject,
                                                               DWebBrwsrEvntsRegOnNavigateComplete_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 1,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnStatusTextChange (CAObjHandle serverObject,
                                                               DWebBrwsrEvntsRegOnStatusTextChange_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 2,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnProgressChange (CAObjHandle serverObject,
                                                             DWebBrwsrEvntsRegOnProgressChange_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 3,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnDownloadComplete (CAObjHandle serverObject,
                                                               DWebBrwsrEvntsRegOnDownloadComplete_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 4,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnCommandStateChange (CAObjHandle serverObject,
                                                                 DWebBrwsrEvntsRegOnCommandStateChange_CallbackType callbackFunction,
                                                                 void *callbackData,
                                                                 int enableCallbacks,
                                                                 int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 5,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnDownloadBegin (CAObjHandle serverObject,
                                                            DWebBrwsrEvntsRegOnDownloadBegin_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 6,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnNewWindow (CAObjHandle serverObject,
                                                        DWebBrwsrEvntsRegOnNewWindow_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 7,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnTitleChange (CAObjHandle serverObject,
                                                          DWebBrwsrEvntsRegOnTitleChange_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 8,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnFrameBeforeNavigate (CAObjHandle serverObject,
                                                                  DWebBrwsrEvntsRegOnFrameBeforeNavigate_CallbackType callbackFunction,
                                                                  void *callbackData,
                                                                  int enableCallbacks,
                                                                  int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 9,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnFrameNavigateComplete (CAObjHandle serverObject,
                                                                    DWebBrwsrEvntsRegOnFrameNavigateComplete_CallbackType callbackFunction,
                                                                    void *callbackData,
                                                                    int enableCallbacks,
                                                                    int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 10,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnFrameNewWindow (CAObjHandle serverObject,
                                                             DWebBrwsrEvntsRegOnFrameNewWindow_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 11,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnQuit (CAObjHandle serverObject,
                                                   DWebBrwsrEvntsRegOnQuit_CallbackType callbackFunction,
                                                   void *callbackData,
                                                   int enableCallbacks,
                                                   int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 12,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnWindowMove (CAObjHandle serverObject,
                                                         DWebBrwsrEvntsRegOnWindowMove_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 13,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnWindowResize (CAObjHandle serverObject,
                                                           DWebBrwsrEvntsRegOnWindowResize_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 14,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnWindowActivate (CAObjHandle serverObject,
                                                             DWebBrwsrEvntsRegOnWindowActivate_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 15,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnPropertyChange (CAObjHandle serverObject,
                                                             DWebBrwsrEvntsRegOnPropertyChange_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DWebBrwsrEvnts_CAEventClassDefn, 16,
	                                     callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DShellWindowsEvntsRegOnWindowRegistered (CAObjHandle serverObject,
                                                                   DShellWindowsEvntsRegOnWindowRegistered_CallbackType callbackFunction,
                                                                   void *callbackData,
                                                                   int enableCallbacks,
                                                                   int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DShellWindowsEvnts_CAEventClassDefn,
	                                     0, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DShellWindowsEvntsRegOnWindowRevoked (CAObjHandle serverObject,
                                                                DShellWindowsEvntsRegOnWindowRevoked_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DShellWindowsEvnts_CAEventClassDefn,
	                                     1, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DShellNameSpaceEventsRegOnFavoritesSelectionChange (CAObjHandle serverObject,
                                                                              DShellNameSpaceEventsRegOnFavoritesSelectionChange_CallbackType callbackFunction,
                                                                              void *callbackData,
                                                                              int enableCallbacks,
                                                                              int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DShellNameSpaceEvents_CAEventClassDefn,
	                                     0, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DShellNameSpaceEventsRegOnSelectionChange (CAObjHandle serverObject,
                                                                     DShellNameSpaceEventsRegOnSelectionChange_CallbackType callbackFunction,
                                                                     void *callbackData,
                                                                     int enableCallbacks,
                                                                     int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DShellNameSpaceEvents_CAEventClassDefn,
	                                     1, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DShellNameSpaceEventsRegOnDoubleClick (CAObjHandle serverObject,
                                                                 DShellNameSpaceEventsRegOnDoubleClick_CallbackType callbackFunction,
                                                                 void *callbackData,
                                                                 int enableCallbacks,
                                                                 int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DShellNameSpaceEvents_CAEventClassDefn,
	                                     2, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}

HRESULT CVIFUNC WebViewer_DShellNameSpaceEventsRegOnInitialized (CAObjHandle serverObject,
                                                                 DShellNameSpaceEventsRegOnInitialized_CallbackType callbackFunction,
                                                                 void *callbackData,
                                                                 int enableCallbacks,
                                                                 int *callbackId)
{
	HRESULT __result = S_OK;

	__result = CA_RegisterEventCallback (serverObject,
	                                     &_DShellNameSpaceEvents_CAEventClassDefn,
	                                     3, callbackFunction, callbackData,
	                                     enableCallbacks, callbackId);

	return __result;
}
