        ??  ??                  ?       ?? ??     0 	        <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0" />?      ?? ??     0 	        ?4   V S _ V E R S I O N _ I N F O     ???                 ?                         ?   S t r i n g F i l e I n f o   ?   0 4 0 9 0 4 b 0        C o m p a n y N a m e     P   F i l e D e s c r i p t i o n     R S P R O   ( R e l e a s e   x 8 6 )   (   F i l e V e r s i o n     1 . 0   ,   I n t e r n a l N a m e   R S P R O   H   L e g a l C o p y r i g h t   C o p y r i g h t   ?     2 0 2 1   < 
  O r i g i n a l F i l e n a m e   R S P R O . d l l   .   P r o d u c t N a m e       R S P R O     ,   P r o d u c t V e r s i o n   1 . 0   D    V a r F i l e I n f o     $    T r a n s l a t i o n     	?   X   C V I D L L P R O T O T Y P E S   C V I D L L E X P O R T S         0 	        struct _SElException *ClearError(struct _Dielectrimeter *me) __declspec(dllexport);
struct _SElException *Connect(struct _Dielectrimeter *me) __declspec(dllexport);
struct _SElException *Disconnect(struct _Dielectrimeter *me) __declspec(dllexport);
struct _SElException *Read(struct _Dielectrimeter *me, const char *read) __declspec(dllexport);
struct _SElException *SetModeManual(struct _Dielectrimeter *me) __declspec(dllexport);
struct _SElException *SetOutput(struct _Dielectrimeter *me, int output) __declspec(dllexport);
struct _SElException *SetStep(struct _Dielectrimeter *me, int step) __declspec(dllexport);
struct _SElException *SystemError(struct _Dielectrimeter *me, char *error) __declspec(dllexport);
struct _SElException *TestResult(struct _Dielectrimeter *me, int *result, double *voltage_kv, double *current_mA) __declspec(dllexport);
struct _SElException *Write(struct _Dielectrimeter *me, const char *command) __declspec(dllexport);
void elexception_delete(struct _SElException **pException);
void elexception_log_set(void (*fn_log)(struct _SElException *));
struct _SElException *elexception_new(int errorCode, const char *messsage, const char *file, long line);
void excdisplay(struct _SElException *pException);
void excdispstr(struct _SElException *pException, char *str, int str_size);
         ?? ??     0	                                        