        ��  ��                  �       �� ��     0 	        <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0" />�      �� ��     0 	        �4   V S _ V E R S I O N _ I N F O     ���                 ?                         �   S t r i n g F i l e I n f o   �   0 4 0 9 0 4 b 0        C o m p a n y N a m e     T   F i l e D e s c r i p t i o n     p r o d u c t   ( R e l e a s e   x 8 6 )   (   F i l e V e r s i o n     1 . 0   0   I n t e r n a l N a m e   p r o d u c t   H   L e g a l C o p y r i g h t   C o p y r i g h t   �     2 0 2 0   @   O r i g i n a l F i l e n a m e   p r o d u c t . d l l   2 	  P r o d u c t N a m e       p r o d u c t     ,   P r o d u c t V e r s i o n   1 . 0   D    V a r F i l e I n f o     $    T r a n s l a t i o n     	�>  H   U I R F I L E T Y P E   P R O D U C T . U I R       0 	        RSRC
           �  �      d   uI17      DuI20Recherche des produits...   8           " + ��$       Recherch     �   
   �     �  �
��   �������������<��"�����������888888�����������J���*�� ����a��� ��*����-����  �?�Ϡ����������<��*���������-������888��������������������������
�888������������"������������888�������,����*�����a�wa�|�����StandardPicture0DDOD��
��	��	��HLablji.x7��������,Cosm��Pm	�������,Cosm_	�~Mp	�������F�StdRingDispHDDOD@��
��
����	��	��	��HLablj	FO���������LLablJ
)�8̳��������
0\MLbl 2	�'HM	"�������NIDialog��,Cosm 2/,8	%<	,q,Cosm 2,	�'	"M	0������,Cosm	%FO	2������NSNUM@,	StdString0DDOD0�
��
��	��HLablj	���;���D��������tLabl2	�3m��̙��������#Trebuchet MS��gXXX \ YYY \ ZZZ01,Cosm	5oN4	StdString0DDOD0�
��
��	��HLablJ	"�<4�F���������|Labl2	�9fl������������#Trebuchet MS��gRecherche des produits...,Cosm	7hnNStandardList0DDOD��
��	��	��HLablj	����������,Cosm2	�o�j��̙��,CosmW	�m�lN0SLST�������Trebuchet MS��p�StandardPicture0DDOD��
��	��	��HLablj6?��������,Cosm�;i������,Cosm_	�	>l������F     �   � ��pANl  @  8     p     �             ���   -   �    w     ���       ���                                                                                          PRODUCT         Product Dialog NIDialogMetaFont        9   - �-cTRl����                        ��- RING       ;   / �/cTRl����                        ��&� 	 PICT_2     9   - S-cTRl����                        ��&� 	 PICT       9   - �-cTRl����                        �� �  PATH       ;   / w/cTRl����                        �� � SEARCH     ?   3 
3cTRl����                        ��TOnList LIST     ,   *                                           ,   *                                           W                                                                                          W                                                                                          @   4 �4cLSt2LSt��� 
                                  8  8pS15     T      `      (2LSt��� 
                           ,cLSt��� 
          .. ZZZ01 ZZZ02         �      Recherche des produits...      �      XXX \ YYY \ ZZZ01     �   	            �   	            -      D����   2LSt��� 
                   cLSt��� 
                      pS15             h               Recherche des produits...         XXX \ YYY \ ZZZ01                       ����                      �         ����    ����                                                                                                                RSRC
           �  �      d�p��@p       4  �   uIRf       �uIRe       �uIRr       �DLG3       �pANl       �cTRl     sTRn     �dBNd     �cLSt      �pVAl      �lCHk       cDVl      pXTr      (pXT2      <               �~%             x~%             T�~%   ����       d~%      .      8�~%   ����      $L~%   ����      dH~%   ����      �\~%   ����      �|~%   ����      $~%   ����      d�~%   ����      ��~%   ����      � ~%   ����      T~%   ����      d~%   ����      �@~%   ����      	<~%   ����      
@P~%   ����      
X,~%   ����      
�`~%   ����      
� ~%UIR VersionUIR VersionUIR Screen ResolutionPRODUCT  
  X   C V I D L L P R O T O T Y P E S   C V I D L L E X P O R T S         0 	        struct _SElException *ChildExist(struct _SDBSProductList *pProductList, int product_id, int *child, int *parent_id);
struct _SElException *CleanPartDescription(struct _SDBSProductList *pProductList);
void DiscardUIObjectsForDLL();
int DllEntryPoint(struct HINSTANCE__ *hinstDLL, unsigned long fdwReason, void *lpvReserved);
int DllMain(struct HINSTANCE__ *hinstDLL, unsigned long fdwReason, void *lpvReserved);
struct _SElException *EventFunctionCallback(const char *eventName, void *pData);
struct _SElException *GetPartDescription(struct _SDBSProductList *pProductList, int product_id, char *PartDescription, char *ProductPartNumber);
struct _SElException *LoadConfiguration(const char *fileName);
int OnList(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
struct _SElException *PLUGIN_SHUTDOWN_NAME_CODE(struct _SPluginArgs *a_pPluginArgs) __declspec(dllexport);
struct _SElException *SetProductLevelBackward(struct _SDBSProductList *pProductList, int product_id);
struct _SElException *SetProductLevelForward(struct _SDBSProductList *pProductList, int product_id);
struct _SElException *SetProductList(struct _SDBSProductList *pProductList, int product_id);
struct _SElException *SetProductPath(struct _SDBSProductList *pProductList, int product_id);
struct _SElException *SetProductPicture(struct _SDBSProductList *pProductList, int product_id);
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
            �� ��     0	                                        