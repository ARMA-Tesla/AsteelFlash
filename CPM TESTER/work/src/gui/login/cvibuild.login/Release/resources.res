        ??  ??                  ?       ?? ??     0 	        <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0" />?      ?? ??     0 	        ?4   V S _ V E R S I O N _ I N F O     ???                 ?                         ?   S t r i n g F i l e I n f o   ?   0 4 0 9 0 4 b 0        C o m p a n y N a m e     P   F i l e D e s c r i p t i o n     l o g i n   ( R e l e a s e   x 8 6 )   (   F i l e V e r s i o n     1 . 0   ,   I n t e r n a l N a m e   l o g i n   H   L e g a l C o p y r i g h t   C o p y r i g h t   ?     2 0 2 0   < 
  O r i g i n a l F i l e n a m e   l o g i n . d l l   .   P r o d u c t N a m e       l o g i n     ,   P r o d u c t V e r s i o n   1 . 0   D    V a r F i l e I n f o     $    T r a n s l a t i o n     	??  D   U I R F I L E T Y P E   L O G I N . U I R       0 	        RSRC
           ?  ?      ?   uI23      DuI23??? n          ??e?i/o/?o/?]/    연           L??     ?  8in    J     	?  { ??   ?????888?????????????-???                ???Ϡ???????<?Ϫ???????????? ???                ?????88888??????????J???*?? ???a??? ???????                ??  ???????????                ?????8?.??????a???                ?????8?.??????a???                ?????F6???PPP?	StdString0DDODD0?
??
??	??HLabln	m)???D`Labl2	?o\? ?????????Trebuchet MS??d,Cosm	mZ?"??????N?TTip??	StdString0DDOD@0?
??
??	??HLabln	?? ???D`Labl2	??Y?7?????????Trebuchet MS??d,Cosm	?W?9N?TTip?LStdRingDisp@DDODD??
??
????	??	??HLabln	GY&???DLLablJ
I?[?????????
0\MLbl 2	?G\]??????
NIDialog??,Cosm02/	OUL,Cosm	EZ_"??????F?TTip?SNUM@,	StdString DDODb!??
??dLabl#'IPPPPPP??????NIDialog??Log in?TTip?0	StdString DDODb!??
??hLabl#G]RPPPPPP??????NIDialog??utilisateur?TTip?(	StdString DDODb!??
??`Labl#o?1PPPPPP??????NIDialog??Pass?TTip?     ?   ? r?pANl  @  8     x     ?             ???      ?   ?  F     ???        ???                                                                                          PANEL           Login Dialog NIDialogMetaFont          >   2 	i2cTRl????                        ?? ?  TEXTMSG_2      >   2 	i2cTRl????                        ?? ?  TEXTMSG_3      <   0 ?0cTRl????                        ?? ?  TEXTMSG    I   = v=cTRl????                        ??@OnLoginList LOGINLIST       G   ; 	g;cTRl                            ?? ?OnPassword PASSWORD     :   . \.cTRl                            ?? ? LOGIN      ,   *                                           ,   *                                           W                                                                                          W                                                                                          W                                                                                          W                                                                                          W                                                                                          @   4 b4cLSt2LSt??? n                                   ?   ?pS20      ?             ?            @      D????   2LSt??? n                   cLSt??? n                   ?      Log in     ?      Pass       ?      utilisateur       pS20       X       ?           H      ????      Log in        Pass          utilisateur                       ?         ????    ????                                                                                                               RSRC
           ?  ?      ?    \`        4  ?   uIRf       ?uIRe       ?uIRr       ?DLG3       ?pANl       ?cTRl     sTRn     ?dBNd     ?cLSt      pVAl      (lCHk      <cDVl      PpXTr      dpXT2      x               ?`/             ?^/             T`g/   ????       d?!/      .      ?p^/   ????      ?@g/   ????      ?4g/   ????      (!/   ????      h<!/   ????      ?g/   ????      x!/   ????      D?f/   ????      t\h/   ????      ?Xg/   ????       ?!/   ????      \?f/   ????      ?g/   ????      	|g/   ????      	p0!/   ????      	?Hg/   ????      
t(f/   ????      
?a/   ????      
?Pg/   ????      
?a/UIR VersionUIR VersionUIR Screen ResolutionPANEL  X   C V I D L L P R O T O T Y P E S   C V I D L L E X P O R T S         0 	        void DiscardUIObjectsForDLL();
int DllEntryPoint(struct HINSTANCE__ *hinstDLL, unsigned long fdwReason, void *lpvReserved);
int DllMain(struct HINSTANCE__ *hinstDLL, unsigned long fdwReason, void *lpvReserved);
struct _SElException *LoadPanelExWithLanguage(int parentPanel, const char *path, int panelResourceId, struct _SLanguageItem table[], int *pPanelHandle);
struct _SElException *LoadPanelWithLanguage(int parentPanel, const char *path, int panelResourceId, struct _SLanguageItem table[], int *pPanelHandle);
int OnLoginList(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int OnPassword(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
struct _SElException *PLUGIN_SHUTDOWN_NAME_CODE(struct _SPluginArgs *a_pPluginArgs) __declspec(dllexport);
void PostDeferredHidePanel(void *callbackData);
struct _SElException *eleventdata_delete(struct _SElEventData **pEventData);
struct _SElException *eleventdata_new(struct _SElEventData **pEventData);
void elexception_delete(struct _SElException **pException);
void elexception_log_set(void (*fn_log)(struct _SElException *));
struct _SElException *elexception_new(int errorCode, const char *messsage, const char *file, long line);
struct _SElException *elstring_delete(struct _SELString **pString);
struct _SElException *elstring_new(const char *pStr, struct _SELString **pString);
void excdisplay(struct _SElException *pException);
void excdispstr(struct _SElException *pException, char *str, int str_size);
struct _SElException *pluginGetAttribute(unsigned int attrID, unsigned int size, void *value) __declspec(dllexport);
struct _SElException *pluginGetInfo(struct _SPluginInfo **a_pInfo) __declspec(dllexport);
struct _SElException *pluginInitialize(struct _SPluginArgs *a_pPluginArgs) __declspec(dllexport);
struct _SElException *pluginLeave(struct _SPluginArgs *a_pPluginArgs, const char *uuid) __declspec(dllexport);
struct _SElException *pluginRun(struct _SPluginArgs *a_pPluginArgs, const char *uuid) __declspec(dllexport);
          ?? ??     0	                                        