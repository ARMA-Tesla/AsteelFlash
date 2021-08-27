#ifndef _WEBVIEWER_H
#define _WEBVIEWER_H

#if defined(INCLUDE_AFTER_WINDOWS_H) && !defined(_INC_WINDOWS)
#error  This header must be included before utility.h and formatio.h
#error  because it includes cviauto.h which includes Windows SDK headers.
#endif /* INCLUDE_AFTER_WINDOWS_H */

#include <cviauto.h>

#ifdef __cplusplus
    extern "C" {
#endif
/* NICDBLD_BEGIN> Type Library Specific Types */

enum WebViewerEnum_OLECMDID
{
	WebViewerConst_OLECMDID_OPEN = 1,
	WebViewerConst_OLECMDID_NEW = 2,
	WebViewerConst_OLECMDID_SAVE = 3,
	WebViewerConst_OLECMDID_SAVEAS = 4,
	WebViewerConst_OLECMDID_SAVECOPYAS = 5,
	WebViewerConst_OLECMDID_PRINT = 6,
	WebViewerConst_OLECMDID_PRINTPREVIEW = 7,
	WebViewerConst_OLECMDID_PAGESETUP = 8,
	WebViewerConst_OLECMDID_SPELL = 9,
	WebViewerConst_OLECMDID_PROPERTIES = 10,
	WebViewerConst_OLECMDID_CUT = 11,
	WebViewerConst_OLECMDID_COPY = 12,
	WebViewerConst_OLECMDID_PASTE = 13,
	WebViewerConst_OLECMDID_PASTESPECIAL = 14,
	WebViewerConst_OLECMDID_UNDO = 15,
	WebViewerConst_OLECMDID_REDO = 16,
	WebViewerConst_OLECMDID_SELECTALL = 17,
	WebViewerConst_OLECMDID_CLEARSELECTION = 18,
	WebViewerConst_OLECMDID_ZOOM = 19,
	WebViewerConst_OLECMDID_GETZOOMRANGE = 20,
	WebViewerConst_OLECMDID_UPDATECOMMANDS = 21,
	WebViewerConst_OLECMDID_REFRESH = 22,
	WebViewerConst_OLECMDID_STOP = 23,
	WebViewerConst_OLECMDID_HIDETOOLBARS = 24,
	WebViewerConst_OLECMDID_SETPROGRESSMAX = 25,
	WebViewerConst_OLECMDID_SETPROGRESSPOS = 26,
	WebViewerConst_OLECMDID_SETPROGRESSTEXT = 27,
	WebViewerConst_OLECMDID_SETTITLE = 28,
	WebViewerConst_OLECMDID_SETDOWNLOADSTATE = 29,
	WebViewerConst_OLECMDID_STOPDOWNLOAD = 30,
	WebViewerConst_OLECMDID_ONTOOLBARACTIVATED = 31,
	WebViewerConst_OLECMDID_FIND = 32,
	WebViewerConst_OLECMDID_DELETE = 33,
	WebViewerConst_OLECMDID_HTTPEQUIV = 34,
	WebViewerConst_OLECMDID_HTTPEQUIV_DONE = 35,
	WebViewerConst_OLECMDID_ENABLE_INTERACTION = 36,
	WebViewerConst_OLECMDID_ONUNLOAD = 37,
	WebViewerConst_OLECMDID_PROPERTYBAG2 = 38,
	WebViewerConst_OLECMDID_PREREFRESH = 39,
	WebViewerConst_OLECMDID_SHOWSCRIPTERROR = 40,
	WebViewerConst_OLECMDID_SHOWMESSAGE = 41,
	WebViewerConst_OLECMDID_SHOWFIND = 42,
	WebViewerConst_OLECMDID_SHOWPAGESETUP = 43,
	WebViewerConst_OLECMDID_SHOWPRINT = 44,
	WebViewerConst_OLECMDID_CLOSE = 45,
	WebViewerConst_OLECMDID_ALLOWUILESSSAVEAS = 46,
	WebViewerConst_OLECMDID_DONTDOWNLOADCSS = 47,
	WebViewerConst_OLECMDID_UPDATEPAGESTATUS = 48,
	WebViewerConst_OLECMDID_PRINT2 = 49,
	WebViewerConst_OLECMDID_PRINTPREVIEW2 = 50,
	WebViewerConst_OLECMDID_SETPRINTTEMPLATE = 51,
	WebViewerConst_OLECMDID_GETPRINTTEMPLATE = 52,
	WebViewerConst_OLECMDID_PAGEACTIONBLOCKED = 55,
	WebViewerConst_OLECMDID_PAGEACTIONUIQUERY = 56,
	WebViewerConst_OLECMDID_FOCUSVIEWCONTROLS = 57,
	WebViewerConst_OLECMDID_FOCUSVIEWCONTROLSQUERY = 58,
	WebViewerConst_OLECMDID_SHOWPAGEACTIONMENU = 59,
	_WebViewer_OLECMDIDForceSizeToFourBytes = 0xFFFFFFFF
};
enum WebViewerEnum_OLECMDF
{
	WebViewerConst_OLECMDF_SUPPORTED = 1,
	WebViewerConst_OLECMDF_ENABLED = 2,
	WebViewerConst_OLECMDF_LATCHED = 4,
	WebViewerConst_OLECMDF_NINCHED = 8,
	WebViewerConst_OLECMDF_INVISIBLE = 16,
	WebViewerConst_OLECMDF_DEFHIDEONCTXTMENU = 32,
	_WebViewer_OLECMDFForceSizeToFourBytes = 0xFFFFFFFF
};
enum WebViewerEnum_OLECMDEXECOPT
{
	WebViewerConst_OLECMDEXECOPT_DODEFAULT = 0,
	WebViewerConst_OLECMDEXECOPT_PROMPTUSER = 1,
	WebViewerConst_OLECMDEXECOPT_DONTPROMPTUSER = 2,
	WebViewerConst_OLECMDEXECOPT_SHOWHELP = 3,
	_WebViewer_OLECMDEXECOPTForceSizeToFourBytes = 0xFFFFFFFF
};
enum WebViewerEnum_tagREADYSTATE
{
	WebViewerConst_READYSTATE_UNINITIALIZED = 0,
	WebViewerConst_READYSTATE_LOADING = 1,
	WebViewerConst_READYSTATE_LOADED = 2,
	WebViewerConst_READYSTATE_INTERACTIVE = 3,
	WebViewerConst_READYSTATE_COMPLETE = 4,
	_WebViewer_tagREADYSTATEForceSizeToFourBytes = 0xFFFFFFFF
};
enum WebViewerEnum_CommandStateChangeConstants
{
	WebViewerConst_CSC_UPDATECOMMANDS = -1,
	WebViewerConst_CSC_NAVIGATEFORWARD = 1,
	WebViewerConst_CSC_NAVIGATEBACK = 2,
	_WebViewer_CommandStateChangeConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum WebViewerEnum_SecureLockIconConstants
{
	WebViewerConst_secureLockIconUnsecure = 0,
	WebViewerConst_secureLockIconMixed = 1,
	WebViewerConst_secureLockIconSecureUnknownBits = 2,
	WebViewerConst_secureLockIconSecure40Bit = 3,
	WebViewerConst_secureLockIconSecure56Bit = 4,
	WebViewerConst_secureLockIconSecureFortezza = 5,
	WebViewerConst_secureLockIconSecure128Bit = 6,
	_WebViewer_SecureLockIconConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum WebViewerEnum_ShellWindowTypeConstants
{
	WebViewerConst_SWC_EXPLORER = 0,
	WebViewerConst_SWC_BROWSER = 1,
	WebViewerConst_SWC_3RDPARTY = 2,
	WebViewerConst_SWC_CALLBACK = 4,
	_WebViewer_ShellWindowTypeConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum WebViewerEnum_ShellWindowFindWindowOptions
{
	WebViewerConst_SWFO_NEEDDISPATCH = 1,
	WebViewerConst_SWFO_INCLUDEPENDING = 2,
	WebViewerConst_SWFO_COOKIEPASSED = 4,
	_WebViewer_ShellWindowFindWindowOptionsForceSizeToFourBytes = 0xFFFFFFFF
};
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnStatusTextChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                  void *caCallbackData,
                                                                                  char *text);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnProgressChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                void *caCallbackData,
                                                                                long  progress,
                                                                                long  progressMax);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnCommandStateChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                    void *caCallbackData,
                                                                                    long  command,
                                                                                    VBOOL  enable);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnDownloadBegin_CallbackType) (CAObjHandle caServerObjHandle,
                                                                               void *caCallbackData);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnDownloadComplete_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                  void *caCallbackData);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnTitleChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                             void *caCallbackData,
                                                                             char *text);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnPropertyChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                void *caCallbackData,
                                                                                char *szProperty);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnBeforeNavigate2_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                 void *caCallbackData,
                                                                                 CAObjHandle  pDisp,
                                                                                 VARIANT *URL,
                                                                                 VARIANT *flags,
                                                                                 VARIANT *targetFrameName,
                                                                                 VARIANT *postData,
                                                                                 VARIANT *headers,
                                                                                 VBOOL *cancel);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnNewWindow2_CallbackType) (CAObjHandle caServerObjHandle,
                                                                            void *caCallbackData,
                                                                            CAObjHandle *ppDisp,
                                                                            VBOOL *cancel);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnNavigateComplete2_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                   void *caCallbackData,
                                                                                   CAObjHandle  pDisp,
                                                                                   VARIANT *URL);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnDocumentComplete_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                  void *caCallbackData,
                                                                                  CAObjHandle  pDisp,
                                                                                  VARIANT *URL);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnOnQuit_CallbackType) (CAObjHandle caServerObjHandle,
                                                                        void *caCallbackData);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnOnVisible_CallbackType) (CAObjHandle caServerObjHandle,
                                                                           void *caCallbackData,
                                                                           VBOOL  visible);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnOnToolBar_CallbackType) (CAObjHandle caServerObjHandle,
                                                                           void *caCallbackData,
                                                                           VBOOL  toolBar);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnOnMenuBar_CallbackType) (CAObjHandle caServerObjHandle,
                                                                           void *caCallbackData,
                                                                           VBOOL  menuBar);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnOnStatusBar_CallbackType) (CAObjHandle caServerObjHandle,
                                                                             void *caCallbackData,
                                                                             VBOOL  statusBar);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnOnFullScreen_CallbackType) (CAObjHandle caServerObjHandle,
                                                                              void *caCallbackData,
                                                                              VBOOL  fullScreen);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnOnTheaterMode_CallbackType) (CAObjHandle caServerObjHandle,
                                                                               void *caCallbackData,
                                                                               VBOOL  theaterMode);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnWindowSetResizable_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                    void *caCallbackData,
                                                                                    VBOOL  resizable);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnWindowSetLeft_CallbackType) (CAObjHandle caServerObjHandle,
                                                                               void *caCallbackData,
                                                                               long  left);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnWindowSetTop_CallbackType) (CAObjHandle caServerObjHandle,
                                                                              void *caCallbackData,
                                                                              long  top);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnWindowSetWidth_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                void *caCallbackData,
                                                                                long  width);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnWindowSetHeight_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                 void *caCallbackData,
                                                                                 long  height);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnWindowClosing_CallbackType) (CAObjHandle caServerObjHandle,
                                                                               void *caCallbackData,
                                                                               VBOOL  isChildWindow,
                                                                               VBOOL *cancel);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnClientToHostWindow_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                    void *caCallbackData,
                                                                                    long *CX,
                                                                                    long *CY);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnSetSecureLockIcon_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                   void *caCallbackData,
                                                                                   long  secureLockIcon);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnFileDownload_CallbackType) (CAObjHandle caServerObjHandle,
                                                                              void *caCallbackData,
                                                                              VBOOL *cancel);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnNavigateError_CallbackType) (CAObjHandle caServerObjHandle,
                                                                               void *caCallbackData,
                                                                               CAObjHandle  pDisp,
                                                                               VARIANT *URL,
                                                                               VARIANT *frame,
                                                                               VARIANT *statusCode,
                                                                               VBOOL *cancel);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnPrintTemplateInstantiation_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                            void *caCallbackData,
                                                                                            CAObjHandle  pDisp);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnPrintTemplateTeardown_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                       void *caCallbackData,
                                                                                       CAObjHandle  pDisp);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnUpdatePageStatus_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                  void *caCallbackData,
                                                                                  CAObjHandle  pDisp,
                                                                                  VARIANT *nPage,
                                                                                  VARIANT *fDone);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnPrivacyImpactedStateChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                            void *caCallbackData,
                                                                                            VBOOL  bImpacted);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvnts2RegOnNewWindow3_CallbackType) (CAObjHandle caServerObjHandle,
                                                                            void *caCallbackData,
                                                                            CAObjHandle *ppDisp,
                                                                            VBOOL *cancel,
                                                                            long  dwFlags,
                                                                            char *bstrUrlContext,
                                                                            char *bstrUrl);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnBeforeNavigate_CallbackType) (CAObjHandle caServerObjHandle,
                                                                               void *caCallbackData,
                                                                               char *URL,
                                                                               long  flags,
                                                                               char *targetFrameName,
                                                                               VARIANT *postData,
                                                                               char *headers,
                                                                               VBOOL *cancel);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnNavigateComplete_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                 void *caCallbackData,
                                                                                 char *URL);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnStatusTextChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                 void *caCallbackData,
                                                                                 char *text);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnProgressChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                               void *caCallbackData,
                                                                               long  progress,
                                                                               long  progressMax);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnDownloadComplete_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                 void *caCallbackData);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnCommandStateChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                   void *caCallbackData,
                                                                                   long  command,
                                                                                   VBOOL  enable);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnDownloadBegin_CallbackType) (CAObjHandle caServerObjHandle,
                                                                              void *caCallbackData);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnNewWindow_CallbackType) (CAObjHandle caServerObjHandle,
                                                                          void *caCallbackData,
                                                                          char *URL,
                                                                          long  flags,
                                                                          char *targetFrameName,
                                                                          VARIANT *postData,
                                                                          char *headers,
                                                                          VBOOL *processed);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnTitleChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                            void *caCallbackData,
                                                                            char *text);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnFrameBeforeNavigate_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                    void *caCallbackData,
                                                                                    char *URL,
                                                                                    long  flags,
                                                                                    char *targetFrameName,
                                                                                    VARIANT *postData,
                                                                                    char *headers,
                                                                                    VBOOL *cancel);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnFrameNavigateComplete_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                      void *caCallbackData,
                                                                                      char *URL);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnFrameNewWindow_CallbackType) (CAObjHandle caServerObjHandle,
                                                                               void *caCallbackData,
                                                                               char *URL,
                                                                               long  flags,
                                                                               char *targetFrameName,
                                                                               VARIANT *postData,
                                                                               char *headers,
                                                                               VBOOL *processed);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnQuit_CallbackType) (CAObjHandle caServerObjHandle,
                                                                     void *caCallbackData,
                                                                     VBOOL *cancel);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnWindowMove_CallbackType) (CAObjHandle caServerObjHandle,
                                                                           void *caCallbackData);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnWindowResize_CallbackType) (CAObjHandle caServerObjHandle,
                                                                             void *caCallbackData);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnWindowActivate_CallbackType) (CAObjHandle caServerObjHandle,
                                                                               void *caCallbackData);
typedef HRESULT (CVICALLBACK *DWebBrwsrEvntsRegOnPropertyChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                               void *caCallbackData,
                                                                               char *property);
typedef HRESULT (CVICALLBACK *DShellWindowsEvntsRegOnWindowRegistered_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                     void *caCallbackData,
                                                                                     long  lCookie);
typedef HRESULT (CVICALLBACK *DShellWindowsEvntsRegOnWindowRevoked_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                  void *caCallbackData,
                                                                                  long  lCookie);
typedef HRESULT (CVICALLBACK *DShellNameSpaceEventsRegOnFavoritesSelectionChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                                void *caCallbackData,
                                                                                                long  cItems,
                                                                                                long  hItem,
                                                                                                char *strName,
                                                                                                char *strUrl,
                                                                                                long  cVisits,
                                                                                                char *strDate,
                                                                                                long  fAvailableOffline);
typedef HRESULT (CVICALLBACK *DShellNameSpaceEventsRegOnSelectionChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                       void *caCallbackData);
typedef HRESULT (CVICALLBACK *DShellNameSpaceEventsRegOnDoubleClick_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                   void *caCallbackData);
typedef HRESULT (CVICALLBACK *DShellNameSpaceEventsRegOnInitialized_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                   void *caCallbackData);
/* NICDBLD_END> Type Library Specific Types */

extern const IID WebViewer_IID_IWebBrowser;
extern const IID WebViewer_IID_IWebBrowser2;
extern const IID WebViewer_IID_IShellWindows;
extern const IID WebViewer_IID_IShellUIHelper;
extern const IID WebViewer_IID_IShellNameSpace;
extern const IID WebViewer_IID_DWebBrwsrEvnts2;
extern const IID WebViewer_IID_DWebBrwsrEvnts;
extern const IID WebViewer_IID_DShellWindowsEvnts;
extern const IID WebViewer_IID_DShellNameSpaceEvents;

HRESULT CVIFUNC WebViewer_NewIWebBrowser (const char *server,
                                          int supportMultithreading, LCID locale,
                                          int reserved,
                                          CAObjHandle *objectHandle);

HRESULT CVIFUNC WebViewer_OpenIWebBrowser (const char *fileName,
                                           const char *server,
                                           int supportMultithreading,
                                           LCID locale, int reserved,
                                           CAObjHandle *objectHandle);

HRESULT CVIFUNC WebViewer_ActiveIWebBrowser (const char *server,
                                             int supportMultithreading,
                                             LCID locale, int reserved,
                                             CAObjHandle *objectHandle);

HRESULT CVIFUNC WebViewer_IWebBrowserGoBack (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IWebBrowserGoForward (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IWebBrowserGoHome (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IWebBrowserGoSearch (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IWebBrowserNavigate (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               const char *URL, VARIANT flags,
                                               VARIANT targetFrameName,
                                               VARIANT postData, VARIANT headers);

HRESULT CVIFUNC WebViewer_IWebBrowserRefresh (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IWebBrowserRefresh2 (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VARIANT level);

HRESULT CVIFUNC WebViewer_IWebBrowserStop (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IWebBrowserGetApplication (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     CAObjHandle *ppDisp);

HRESULT CVIFUNC WebViewer_IWebBrowserGetParent (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                CAObjHandle *ppDisp);

HRESULT CVIFUNC WebViewer_IWebBrowserGetContainer (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   CAObjHandle *ppDisp);

HRESULT CVIFUNC WebViewer_IWebBrowserGetDocument (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  CAObjHandle *ppDisp);

HRESULT CVIFUNC WebViewer_IWebBrowserGetTopLevelContainer (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           VBOOL *pBool);

HRESULT CVIFUNC WebViewer_IWebBrowserGetType (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, char **type);

HRESULT CVIFUNC WebViewer_IWebBrowserGetLeft (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, long *pl);

HRESULT CVIFUNC WebViewer_IWebBrowserSetLeft (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, long pl);

HRESULT CVIFUNC WebViewer_IWebBrowserGetTop (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, long *pl);

HRESULT CVIFUNC WebViewer_IWebBrowserSetTop (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, long pl);

HRESULT CVIFUNC WebViewer_IWebBrowserGetWidth (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, long *pl);

HRESULT CVIFUNC WebViewer_IWebBrowserSetWidth (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, long pl);

HRESULT CVIFUNC WebViewer_IWebBrowserGetHeight (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo, long *pl);

HRESULT CVIFUNC WebViewer_IWebBrowserSetHeight (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo, long pl);

HRESULT CVIFUNC WebViewer_IWebBrowserGetLocationName (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      char **locationName);

HRESULT CVIFUNC WebViewer_IWebBrowserGetLocationURL (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     char **locationURL);

HRESULT CVIFUNC WebViewer_IWebBrowserGetBusy (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, VBOOL *pBool);

HRESULT CVIFUNC WebViewer_NewWebBrowserIWebBrowser2 (int panel,
                                                     const char *label, int top,
                                                     int left, int *controlID,
                                                     int *UILError);

HRESULT CVIFUNC WebViewer_NewInternetExplorerIWebBrowser2 (const char *server,
                                                           int supportMultithreading,
                                                           LCID locale,
                                                           int reserved,
                                                           CAObjHandle *objectHandle);

HRESULT CVIFUNC WebViewer_OpenInternetExplorerIWebBrowser2 (const char *fileName,
                                                            const char *server,
                                                            int supportMultithreading,
                                                            LCID locale,
                                                            int reserved,
                                                            CAObjHandle *objectHandle);

HRESULT CVIFUNC WebViewer_ActiveInternetExplorerIWebBrowser2 (const char *server,
                                                              int supportMultithreading,
                                                              LCID locale,
                                                              int reserved,
                                                              CAObjHandle *objectHandle);

HRESULT CVIFUNC WebViewer_IWebBrowser2GoBack (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IWebBrowser2GoForward (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IWebBrowser2GoHome (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IWebBrowser2GoSearch (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IWebBrowser2Navigate (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *URL, VARIANT flags,
                                                VARIANT targetFrameName,
                                                VARIANT postData,
                                                VARIANT headers);

HRESULT CVIFUNC WebViewer_IWebBrowser2Refresh (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IWebBrowser2Refresh2 (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VARIANT level);

HRESULT CVIFUNC WebViewer_IWebBrowser2Stop (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetApplication (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      CAObjHandle *ppDisp);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetParent (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 CAObjHandle *ppDisp);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetContainer (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    CAObjHandle *ppDisp);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetDocument (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   CAObjHandle *ppDisp);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetTopLevelContainer (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VBOOL *pBool);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetType (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, char **type);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetLeft (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, long *pl);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetLeft (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, long pl);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetTop (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, long *pl);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetTop (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo, long pl);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetWidth (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo, long *pl);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetWidth (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo, long pl);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetHeight (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo, long *pl);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetHeight (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo, long pl);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetLocationName (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       char **locationName);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetLocationURL (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      char **locationURL);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetBusy (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *pBool);

HRESULT CVIFUNC WebViewer_IWebBrowser2Quit (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IWebBrowser2ClientToWindow (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      long *pcx, long *pcy);

HRESULT CVIFUNC WebViewer_IWebBrowser2PutProperty (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   const char *property,
                                                   VARIANT vtValue);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetProperty (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   const char *property,
                                                   VARIANT *pvtValue);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetName (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, char **name);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetHWND (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, long *pHWND);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetFullName (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   char **fullName);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetPath (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo, char **path);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetVisible (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL *pBool);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetVisible (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL pBool);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetStatusBar (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL *pBool);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetStatusBar (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL pBool);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetStatusText (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     char **statusText);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetStatusText (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *statusText);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetToolBar (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  long *value);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetToolBar (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  long value);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetMenuBar (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL *value);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetMenuBar (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL value);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetFullScreen (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL *pbFullScreen);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetFullScreen (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL pbFullScreen);

HRESULT CVIFUNC WebViewer_IWebBrowser2Navigate2 (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VARIANT URL, VARIANT flags,
                                                 VARIANT targetFrameName,
                                                 VARIANT postData,
                                                 VARIANT headers);

HRESULT CVIFUNC WebViewer_IWebBrowser2QueryStatusWB (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     enum WebViewerEnum_OLECMDID cmdID,
                                                     enum WebViewerEnum_OLECMDF *pcmdf);

HRESULT CVIFUNC WebViewer_IWebBrowser2ExecWB (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              enum WebViewerEnum_OLECMDID cmdID,
                                              enum WebViewerEnum_OLECMDEXECOPT cmdexecopt,
                                              VARIANT pvaIn, VARIANT *pvaOut);

HRESULT CVIFUNC WebViewer_IWebBrowser2ShowBrowserBar (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      VARIANT pvaClsid,
                                                      VARIANT pvarShow,
                                                      VARIANT pvarSize);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetReadyState (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     enum WebViewerEnum_tagREADYSTATE *plReadyState);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetOffline (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL *pbOffline);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetOffline (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL pbOffline);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetSilent (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VBOOL *pbSilent);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetSilent (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VBOOL pbSilent);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetRegisterAsBrowser (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VBOOL *pbRegister);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetRegisterAsBrowser (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VBOOL pbRegister);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetRegisterAsDropTarget (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               VBOOL *pbRegister);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetRegisterAsDropTarget (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               VBOOL pbRegister);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetTheaterMode (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      VBOOL *pbRegister);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetTheaterMode (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      VBOOL pbRegister);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetAddressBar (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL *value);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetAddressBar (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL value);

HRESULT CVIFUNC WebViewer_IWebBrowser2GetResizable (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL *value);

HRESULT CVIFUNC WebViewer_IWebBrowser2SetResizable (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL value);

HRESULT CVIFUNC WebViewer_NewIShellWindows (const char *server,
                                            int supportMultithreading,
                                            LCID locale, int reserved,
                                            CAObjHandle *objectHandle);

HRESULT CVIFUNC WebViewer_OpenIShellWindows (const char *fileName,
                                             const char *server,
                                             int supportMultithreading,
                                             LCID locale, int reserved,
                                             CAObjHandle *objectHandle);

HRESULT CVIFUNC WebViewer_ActiveIShellWindows (const char *server,
                                               int supportMultithreading,
                                               LCID locale, int reserved,
                                               CAObjHandle *objectHandle);

HRESULT CVIFUNC WebViewer_IShellWindowsGetCount (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 long *count);

HRESULT CVIFUNC WebViewer_IShellWindowsItem (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, VARIANT index,
                                             CAObjHandle *folder);

HRESULT CVIFUNC WebViewer_IShellWindows_NewEnum (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 LPUNKNOWN *ppunk);

HRESULT CVIFUNC WebViewer_IShellWindowsRegister (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 CAObjHandle pid, long HWND,
                                                 long swClass, long *plCookie);

HRESULT CVIFUNC WebViewer_IShellWindowsRegisterPending (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        long lThreadId,
                                                        VARIANT pvarloc,
                                                        VARIANT pvarlocRoot,
                                                        long swClass,
                                                        long *plCookie);

HRESULT CVIFUNC WebViewer_IShellWindowsRevoke (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               long lCookie);

HRESULT CVIFUNC WebViewer_IShellWindowsOnNavigate (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   long lCookie, VARIANT pvarloc);

HRESULT CVIFUNC WebViewer_IShellWindowsOnActivated (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    long lCookie, VBOOL fActive);

HRESULT CVIFUNC WebViewer_IShellWindowsFindWindowSW (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VARIANT pvarloc,
                                                     VARIANT pvarlocRoot,
                                                     long swClass, long *pHWND,
                                                     long swfwOptions,
                                                     CAObjHandle *ppdispOut);

HRESULT CVIFUNC WebViewer_IShellWindowsOnCreated (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  long lCookie, LPUNKNOWN punk);

HRESULT CVIFUNC WebViewer_IShellWindowsProcessAttachDetch (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           VBOOL fAttach);

HRESULT CVIFUNC WebViewer_NewIShellUIHelper (const char *server,
                                             int supportMultithreading,
                                             LCID locale, int reserved,
                                             CAObjHandle *objectHandle);

HRESULT CVIFUNC WebViewer_OpenIShellUIHelper (const char *fileName,
                                              const char *server,
                                              int supportMultithreading,
                                              LCID locale, int reserved,
                                              CAObjHandle *objectHandle);

HRESULT CVIFUNC WebViewer_ActiveIShellUIHelper (const char *server,
                                                int supportMultithreading,
                                                LCID locale, int reserved,
                                                CAObjHandle *objectHandle);

HRESULT CVIFUNC WebViewer_IShellUIHelperResetFrstBootMode (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IShellUIHelperResetSafeMode (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IShellUIHelperRefreshOfflnDsktp (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IShellUIHelperAddFavorite (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *URL,
                                                     VARIANT title);

HRESULT CVIFUNC WebViewer_IShellUIHelperAddChannel (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    const char *URL);

HRESULT CVIFUNC WebViewer_IShellUIHelperAddDsktpComponent (CAObjHandle objectHandle,
                                                           ERRORINFO *errorInfo,
                                                           const char *URL,
                                                           const char *type,
                                                           VARIANT left,
                                                           VARIANT top,
                                                           VARIANT width,
                                                           VARIANT height);

HRESULT CVIFUNC WebViewer_IShellUIHelperIsSubscribed (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      const char *URL,
                                                      VBOOL *pBool);

HRESULT CVIFUNC WebViewer_IShellUIHelperNavigateAndFind (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         const char *URL,
                                                         const char *strQuery,
                                                         VARIANT varTargetFrame);

HRESULT CVIFUNC WebViewer_IShellUIHelperImportExportFavorites (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               VBOOL fImport,
                                                               const char *strImpExpPath);

HRESULT CVIFUNC WebViewer_IShellUIHelperAutoCompleteSaveForm (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              VARIANT form);

HRESULT CVIFUNC WebViewer_IShellUIHelperAutoScan (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *strSearch,
                                                  const char *strFailureUrl,
                                                  VARIANT pvarTargetFrame);

HRESULT CVIFUNC WebViewer_IShellUIHelperAutoCompleteAttach (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VARIANT reserved);

HRESULT CVIFUNC WebViewer_IShellUIHelperShowBrowserUI (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       const char *bstrName,
                                                       VARIANT pvarIn,
                                                       VARIANT *pvarOut);

HRESULT CVIFUNC WebViewer_NewIShellNameSpace (const char *server,
                                              int supportMultithreading,
                                              LCID locale, int reserved,
                                              CAObjHandle *objectHandle);

HRESULT CVIFUNC WebViewer_OpenIShellNameSpace (const char *fileName,
                                               const char *server,
                                               int supportMultithreading,
                                               LCID locale, int reserved,
                                               CAObjHandle *objectHandle);

HRESULT CVIFUNC WebViewer_ActiveIShellNameSpace (const char *server,
                                                 int supportMultithreading,
                                                 LCID locale, int reserved,
                                                 CAObjHandle *objectHandle);

HRESULT CVIFUNC WebViewer_IShellNameSpaceMoveSelectionUp (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IShellNameSpaceMoveSelectionDown (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IShellNameSpaceResetSort (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IShellNameSpaceNewFolder (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IShellNameSpaceSynchronize (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IShellNameSpaceImport (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IShellNameSpaceExport (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IShellNameSpaceInvokeContextMenuCommand (CAObjHandle objectHandle,
                                                                   ERRORINFO *errorInfo,
                                                                   const char *strCommand);

HRESULT CVIFUNC WebViewer_IShellNameSpaceMoveSelectionTo (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetSubscriptionsEnabled (CAObjHandle objectHandle,
                                                                  ERRORINFO *errorInfo,
                                                                  VBOOL *pBool);

HRESULT CVIFUNC WebViewer_IShellNameSpaceCreateSubscriptionForSelection (CAObjHandle objectHandle,
                                                                         ERRORINFO *errorInfo,
                                                                         VBOOL *pBool);

HRESULT CVIFUNC WebViewer_IShellNameSpaceDeleteSubscriptionForSelection (CAObjHandle objectHandle,
                                                                         ERRORINFO *errorInfo,
                                                                         VBOOL *pBool);

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetRoot (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *bstrFullPath);

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetEnumOptions (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         long *pgrfEnumFlags);

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetEnumOptions (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         long pgrfEnumFlags);

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetSelectedItem (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          CAObjHandle *pItem);

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetSelectedItem (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          CAObjHandle pItem);

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetRoot (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VARIANT *pvar);

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetRootProp (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      VARIANT pvar);

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetDepth (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   long *piDepth);

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetDepth (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   long piDepth);

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetMode (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  long *puMode);

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetMode (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  long puMode);

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetFlags (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   long *pdwFlags);

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetFlags (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   long pdwFlags);

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetTVFlags (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     long dwFlags);

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetTVFlags (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     long *dwFlags);

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetColumns (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     char **bstrColumns);

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetColumns (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *bstrColumns);

HRESULT CVIFUNC WebViewer_IShellNameSpaceGetCountViewTypes (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            long *piTypes);

HRESULT CVIFUNC WebViewer_IShellNameSpaceSetViewType (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      long iType);

HRESULT CVIFUNC WebViewer_IShellNameSpaceSelectedItems (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        CAObjHandle *ppid);

HRESULT CVIFUNC WebViewer_IShellNameSpaceExpand (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VARIANT var, long iDepth);

HRESULT CVIFUNC WebViewer_IShellNameSpaceUnselectAll (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnStatusTextChange (CAObjHandle serverObject,
                                                                DWebBrwsrEvnts2RegOnStatusTextChange_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnProgressChange (CAObjHandle serverObject,
                                                              DWebBrwsrEvnts2RegOnProgressChange_CallbackType callbackFunction,
                                                              void *callbackData,
                                                              int enableCallbacks,
                                                              int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnCommandStateChange (CAObjHandle serverObject,
                                                                  DWebBrwsrEvnts2RegOnCommandStateChange_CallbackType callbackFunction,
                                                                  void *callbackData,
                                                                  int enableCallbacks,
                                                                  int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnDownloadBegin (CAObjHandle serverObject,
                                                             DWebBrwsrEvnts2RegOnDownloadBegin_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnDownloadComplete (CAObjHandle serverObject,
                                                                DWebBrwsrEvnts2RegOnDownloadComplete_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnTitleChange (CAObjHandle serverObject,
                                                           DWebBrwsrEvnts2RegOnTitleChange_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnPropertyChange (CAObjHandle serverObject,
                                                              DWebBrwsrEvnts2RegOnPropertyChange_CallbackType callbackFunction,
                                                              void *callbackData,
                                                              int enableCallbacks,
                                                              int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnBeforeNavigate2 (CAObjHandle serverObject,
                                                               DWebBrwsrEvnts2RegOnBeforeNavigate2_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnNewWindow2 (CAObjHandle serverObject,
                                                          DWebBrwsrEvnts2RegOnNewWindow2_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnNavigateComplete2 (CAObjHandle serverObject,
                                                                 DWebBrwsrEvnts2RegOnNavigateComplete2_CallbackType callbackFunction,
                                                                 void *callbackData,
                                                                 int enableCallbacks,
                                                                 int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnDocumentComplete (CAObjHandle serverObject,
                                                                DWebBrwsrEvnts2RegOnDocumentComplete_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnOnQuit (CAObjHandle serverObject,
                                                      DWebBrwsrEvnts2RegOnOnQuit_CallbackType callbackFunction,
                                                      void *callbackData,
                                                      int enableCallbacks,
                                                      int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnOnVisible (CAObjHandle serverObject,
                                                         DWebBrwsrEvnts2RegOnOnVisible_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnOnToolBar (CAObjHandle serverObject,
                                                         DWebBrwsrEvnts2RegOnOnToolBar_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnOnMenuBar (CAObjHandle serverObject,
                                                         DWebBrwsrEvnts2RegOnOnMenuBar_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnOnStatusBar (CAObjHandle serverObject,
                                                           DWebBrwsrEvnts2RegOnOnStatusBar_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnOnFullScreen (CAObjHandle serverObject,
                                                            DWebBrwsrEvnts2RegOnOnFullScreen_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnOnTheaterMode (CAObjHandle serverObject,
                                                             DWebBrwsrEvnts2RegOnOnTheaterMode_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnWindowSetResizable (CAObjHandle serverObject,
                                                                  DWebBrwsrEvnts2RegOnWindowSetResizable_CallbackType callbackFunction,
                                                                  void *callbackData,
                                                                  int enableCallbacks,
                                                                  int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnWindowSetLeft (CAObjHandle serverObject,
                                                             DWebBrwsrEvnts2RegOnWindowSetLeft_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnWindowSetTop (CAObjHandle serverObject,
                                                            DWebBrwsrEvnts2RegOnWindowSetTop_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnWindowSetWidth (CAObjHandle serverObject,
                                                              DWebBrwsrEvnts2RegOnWindowSetWidth_CallbackType callbackFunction,
                                                              void *callbackData,
                                                              int enableCallbacks,
                                                              int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnWindowSetHeight (CAObjHandle serverObject,
                                                               DWebBrwsrEvnts2RegOnWindowSetHeight_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnWindowClosing (CAObjHandle serverObject,
                                                             DWebBrwsrEvnts2RegOnWindowClosing_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnClientToHostWindow (CAObjHandle serverObject,
                                                                  DWebBrwsrEvnts2RegOnClientToHostWindow_CallbackType callbackFunction,
                                                                  void *callbackData,
                                                                  int enableCallbacks,
                                                                  int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnSetSecureLockIcon (CAObjHandle serverObject,
                                                                 DWebBrwsrEvnts2RegOnSetSecureLockIcon_CallbackType callbackFunction,
                                                                 void *callbackData,
                                                                 int enableCallbacks,
                                                                 int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnFileDownload (CAObjHandle serverObject,
                                                            DWebBrwsrEvnts2RegOnFileDownload_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnNavigateError (CAObjHandle serverObject,
                                                             DWebBrwsrEvnts2RegOnNavigateError_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnPrintTemplateInstantiation (CAObjHandle serverObject,
                                                                          DWebBrwsrEvnts2RegOnPrintTemplateInstantiation_CallbackType callbackFunction,
                                                                          void *callbackData,
                                                                          int enableCallbacks,
                                                                          int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnPrintTemplateTeardown (CAObjHandle serverObject,
                                                                     DWebBrwsrEvnts2RegOnPrintTemplateTeardown_CallbackType callbackFunction,
                                                                     void *callbackData,
                                                                     int enableCallbacks,
                                                                     int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnUpdatePageStatus (CAObjHandle serverObject,
                                                                DWebBrwsrEvnts2RegOnUpdatePageStatus_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnPrivacyImpactedStateChange (CAObjHandle serverObject,
                                                                          DWebBrwsrEvnts2RegOnPrivacyImpactedStateChange_CallbackType callbackFunction,
                                                                          void *callbackData,
                                                                          int enableCallbacks,
                                                                          int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvnts2RegOnNewWindow3 (CAObjHandle serverObject,
                                                          DWebBrwsrEvnts2RegOnNewWindow3_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnBeforeNavigate (CAObjHandle serverObject,
                                                             DWebBrwsrEvntsRegOnBeforeNavigate_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnNavigateComplete (CAObjHandle serverObject,
                                                               DWebBrwsrEvntsRegOnNavigateComplete_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnStatusTextChange (CAObjHandle serverObject,
                                                               DWebBrwsrEvntsRegOnStatusTextChange_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnProgressChange (CAObjHandle serverObject,
                                                             DWebBrwsrEvntsRegOnProgressChange_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnDownloadComplete (CAObjHandle serverObject,
                                                               DWebBrwsrEvntsRegOnDownloadComplete_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnCommandStateChange (CAObjHandle serverObject,
                                                                 DWebBrwsrEvntsRegOnCommandStateChange_CallbackType callbackFunction,
                                                                 void *callbackData,
                                                                 int enableCallbacks,
                                                                 int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnDownloadBegin (CAObjHandle serverObject,
                                                            DWebBrwsrEvntsRegOnDownloadBegin_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnNewWindow (CAObjHandle serverObject,
                                                        DWebBrwsrEvntsRegOnNewWindow_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnTitleChange (CAObjHandle serverObject,
                                                          DWebBrwsrEvntsRegOnTitleChange_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnFrameBeforeNavigate (CAObjHandle serverObject,
                                                                  DWebBrwsrEvntsRegOnFrameBeforeNavigate_CallbackType callbackFunction,
                                                                  void *callbackData,
                                                                  int enableCallbacks,
                                                                  int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnFrameNavigateComplete (CAObjHandle serverObject,
                                                                    DWebBrwsrEvntsRegOnFrameNavigateComplete_CallbackType callbackFunction,
                                                                    void *callbackData,
                                                                    int enableCallbacks,
                                                                    int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnFrameNewWindow (CAObjHandle serverObject,
                                                             DWebBrwsrEvntsRegOnFrameNewWindow_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnQuit (CAObjHandle serverObject,
                                                   DWebBrwsrEvntsRegOnQuit_CallbackType callbackFunction,
                                                   void *callbackData,
                                                   int enableCallbacks,
                                                   int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnWindowMove (CAObjHandle serverObject,
                                                         DWebBrwsrEvntsRegOnWindowMove_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnWindowResize (CAObjHandle serverObject,
                                                           DWebBrwsrEvntsRegOnWindowResize_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnWindowActivate (CAObjHandle serverObject,
                                                             DWebBrwsrEvntsRegOnWindowActivate_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId);

HRESULT CVIFUNC WebViewer_DWebBrwsrEvntsRegOnPropertyChange (CAObjHandle serverObject,
                                                             DWebBrwsrEvntsRegOnPropertyChange_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId);

HRESULT CVIFUNC WebViewer_DShellWindowsEvntsRegOnWindowRegistered (CAObjHandle serverObject,
                                                                   DShellWindowsEvntsRegOnWindowRegistered_CallbackType callbackFunction,
                                                                   void *callbackData,
                                                                   int enableCallbacks,
                                                                   int *callbackId);

HRESULT CVIFUNC WebViewer_DShellWindowsEvntsRegOnWindowRevoked (CAObjHandle serverObject,
                                                                DShellWindowsEvntsRegOnWindowRevoked_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId);

HRESULT CVIFUNC WebViewer_DShellNameSpaceEventsRegOnFavoritesSelectionChange (CAObjHandle serverObject,
                                                                              DShellNameSpaceEventsRegOnFavoritesSelectionChange_CallbackType callbackFunction,
                                                                              void *callbackData,
                                                                              int enableCallbacks,
                                                                              int *callbackId);

HRESULT CVIFUNC WebViewer_DShellNameSpaceEventsRegOnSelectionChange (CAObjHandle serverObject,
                                                                     DShellNameSpaceEventsRegOnSelectionChange_CallbackType callbackFunction,
                                                                     void *callbackData,
                                                                     int enableCallbacks,
                                                                     int *callbackId);

HRESULT CVIFUNC WebViewer_DShellNameSpaceEventsRegOnDoubleClick (CAObjHandle serverObject,
                                                                 DShellNameSpaceEventsRegOnDoubleClick_CallbackType callbackFunction,
                                                                 void *callbackData,
                                                                 int enableCallbacks,
                                                                 int *callbackId);

HRESULT CVIFUNC WebViewer_DShellNameSpaceEventsRegOnInitialized (CAObjHandle serverObject,
                                                                 DShellNameSpaceEventsRegOnInitialized_CallbackType callbackFunction,
                                                                 void *callbackData,
                                                                 int enableCallbacks,
                                                                 int *callbackId);
#ifdef __cplusplus
    }
#endif
#endif /* _WEBVIEWER_H */
