        ��  ��                  �       �� ��     0 	        <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0" />�      �� ��     0 	        �4   V S _ V E R S I O N _ I N F O     ���                 ?                            S t r i n g F i l e I n f o   �   0 4 0 9 0 4 b 0        C o m p a n y N a m e     \   F i l e D e s c r i p t i o n     m a i n t e n a n c e   ( R e l e a s e   x 8 6 )   (   F i l e V e r s i o n     1 . 0   8   I n t e r n a l N a m e   m a i n t e n a n c e   H   L e g a l C o p y r i g h t   C o p y r i g h t   �     2 0 2 0   H   O r i g i n a l F i l e n a m e   m a i n t e n a n c e . d l l   :   P r o d u c t N a m e       m a i n t e n a n c e     ,   P r o d u c t V e r s i o n   1 . 0   D    V a r F i l e I n f o     $    T r a n s l a t i o n     	�P  P   U I R F I L E T Y P E   M A I N T E N A N C E . U I R       0 	        RSRC
           �  �      d   uI20      DuI23
                             x�    4                   ���     �  �i[   $      H   9 ��    H�K��H���   �   � ��pANl  @  8      |     �             ���      �  H  �     ���        ���                                                                                          PANEL           maintenance NIDialogMetaFont       (    AcLSt2LSt��� n                      pS18          pS18                              �         ����    ����                                                                                                               RSRC
           �  �      d    �x       4  �   
uIRf       �uIRe       �uIRr       �DLG3       �pANl       �cLSt       �pVAl       lCHk      cDVl      (pXTr      <pXT2      P               $�              �             T,�   ����       d@�      .       �0�   ����      tD�   ����      �L�   ����      �P�   ����      �X�   ����      �H�   ����      �T�UIR VersionUIR VersionUIR Screen ResolutionPANEL�  X   C V I D L L P R O T O T Y P E S   C V I D L L E X P O R T S         0 	        void DiscardUIObjectsForDLL();
int DllEntryPoint(struct HINSTANCE__ *hinstDLL, unsigned long fdwReason, void *lpvReserved);
int DllMain(struct HINSTANCE__ *hinstDLL, unsigned long fdwReason, void *lpvReserved);
struct _SElException *PLUGIN_SHUTDOWN_NAME_CODE(struct _SPluginArgs *a_pPluginArgs) __declspec(dllexport);
void elexception_delete(struct _SElException **pException);
void elexception_log_set(void (*fn_log)(struct _SElException *));
struct _SElException *elexception_new(int errorCode, const char *messsage, const char *file, long line);
void excdisplay(struct _SElException *pException);
void excdispstr(struct _SElException *pException, char *str, int str_size);
struct _SElException *pluginGetAttribute(unsigned int attrID, unsigned int size, void *value) __declspec(dllexport);
struct _SElException *pluginGetInfo(struct _SPluginInfo **a_pInfo) __declspec(dllexport);
struct _SElException *pluginInitialize(struct _SPluginArgs *a_pPluginArgs) __declspec(dllexport);
struct _SElException *pluginLeave(struct _SPluginArgs *a_pPluginArgs, const char *uuid) __declspec(dllexport);
struct _SElException *pluginRun(struct _SPluginArgs *a_pPluginArgs, const char *uuid) __declspec(dllexport);
           �� ��     0	                                        