        ??  ??                  ?       ?? ??     0 	        <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0" />?      ?? ??     0 	        ?4   V S _ V E R S I O N _ I N F O     ???                 ?                        ?   S t r i n g F i l e I n f o   ?   0 4 0 9 0 4 b 0        C o m p a n y N a m e     P   F i l e D e s c r i p t i o n     d a t a m a n   ( D e b u g   x 8 6 )   (   F i l e V e r s i o n     1 . 0   0   I n t e r n a l N a m e   d a t a m a n   H   L e g a l C o p y r i g h t   C o p y r i g h t   ?     2 0 2 0   @   O r i g i n a l F i l e n a m e   d a t a m a n . d l l   2 	  P r o d u c t N a m e       d a t a m a n     ,   P r o d u c t V e r s i o n   1 . 0   D    V a r F i l e I n f o     $    T r a n s l a t i o n     	?#  X   C V I D L L P R O T O T Y P E S   C V I D L L E X P O R T S         0 	        int DllMain(struct HINSTANCE__ *hinstDLL, unsigned long fdwReason, void *lpvReserved);
int EventFactoryEventThreadFunc(void *pArguments);
struct _SElException *ThreadStart(unsigned long (*pThreadFnc)(void *) __attribute__((stdcall)), void *pData, HANDLE *hThread, HANDLE *hStopEvent);
struct _SElException *ThreadStatus(HANDLE hThread, unsigned int timeout, int *pStatus);
struct _SElException *ThreadStop(HANDLE *hThread, HANDLE *hStopEvent);
struct _SElException *bcr_delete(struct _BarCodeReader **pBcr) __declspec(dllexport);
struct _SElException *bcr_new(struct _BarCodeReader **pBcr) __declspec(dllexport);
struct _SElException *eleventdata_delete(struct _SElEventData **pEventData);
struct _SElException *eleventdata_new(struct _SElEventData **pEventData);
struct _SElException *eleventfactory_delete(struct _SElEventFactory **pEventFactory);
struct _SElException *eleventfactory_new(struct _SElEventFactory **pEventFactory);
void elexception_delete(struct _SElException **pException);
void elexception_log_set(void (*fn_log)(struct _SElException *));
struct _SElException *elexception_new(int errorCode, const char *messsage, const char *file, long line);
struct _SElException *elstring_delete(struct _SELString **pString);
struct _SElException *elstring_new(const char *pStr, struct _SELString **pString);
void excdisplay(struct _SElException *pException);
void excdispstr(struct _SElException *pException, char *str, int str_size);
struct _SElException *objlist_delete(struct _SObjList **pList);
struct _SElException *objlist_new(struct _SObjList **pObjList);
      X   C V I D L L P R O T O T Y P E S   C V I D L L E X P O R T S 2       0 	                ?? ??     0	                                        