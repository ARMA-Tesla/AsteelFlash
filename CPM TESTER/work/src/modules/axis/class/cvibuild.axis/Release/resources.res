        ��  ��                  �       �� ��     0 	        <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0" />|      �� ��     0 	        |4   V S _ V E R S I O N _ I N F O     ���                 ?                         �   S t r i n g F i l e I n f o   �   0 4 0 9 0 4 b 0        C o m p a n y N a m e     N   F i l e D e s c r i p t i o n     a x i s   ( R e l e a s e   x 8 6 )     (   F i l e V e r s i o n     1 . 0   *   I n t e r n a l N a m e   a x i s     H   L e g a l C o p y r i g h t   C o p y r i g h t   �     2 0 2 0   : 	  O r i g i n a l F i l e n a m e   a x i s . d l l     ,   P r o d u c t N a m e       a x i s   ,   P r o d u c t V e r s i o n   1 . 0   D    V a r F i l e I n f o     $    T r a n s l a t i o n     	��  X   C V I D L L P R O T O T Y P E S   C V I D L L E X P O R T S         0 	        int DllMain(struct HINSTANCE__ *hinstDLL, unsigned long fdwReason, void *lpvReserved);
struct _SElException *GetModuleAttribute(unsigned int attrID, unsigned int size, void *value) __declspec(dllexport);
struct _SElException *InitializeElcomModule(void *pInstance, const char *configuration, struct _SPluginArgs *pParam) __declspec(dllexport);
struct _SElException *LoadElcomModule(const char *configuration, void **pInstance, struct _SPluginArgs *pParam) __declspec(dllexport);
unsigned long Thread_axis(void *lpParameter);
struct _SElException *UnloadElcomModule(void *pInstance) __declspec(dllexport);
struct _SElException *axis_delete(struct _axis **axis);
struct _SElException *axis_new(struct _axis **axis, const char *name, const char *optionString);
struct _SElException *axiss_delete(struct _axiss **axiss);
struct _SElException *axiss_new(struct _axiss **axiss);
void elexception_delete(struct _SElException **pException);
void elexception_log_set(void (*fn_log)(struct _SElException *));
struct _SElException *elexception_new(int errorCode, const char *messsage, const char *file, long line);
void excdisplay(struct _SElException *pException);
void excdispstr(struct _SElException *pException, char *str, int str_size);
         �� ��     0	                                        