        ??  ??                  ?       ?? ??     0 	        <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0" />l      ?? ??     0 	        l4   V S _ V E R S I O N _ I N F O     ???                 ?                        ?   S t r i n g F i l e I n f o   ?   0 4 0 9 0 4 b 0        C o m p a n y N a m e     H   F i l e D e s c r i p t i o n     A R W   ( D e b u g   x 8 6 )   (   F i l e V e r s i o n     1 . 0   (   I n t e r n a l N a m e   A R W   H   L e g a l C o p y r i g h t   C o p y r i g h t   ?     2 0 2 0   8   O r i g i n a l F i l e n a m e   A R W . d l l   *   P r o d u c t N a m e       A R W     ,   P r o d u c t V e r s i o n   1 . 0   D    V a r F i l e I n f o     $    T r a n s l a t i o n     	?B  X   C V I D L L P R O T O T Y P E S   C V I D L L E X P O R T S         0 	        struct _SElException *Connect(struct _Sonometer *me) __declspec(dllexport);
struct _SElException *Disconnect(struct _Sonometer *me) __declspec(dllexport);
struct _SElException *Read(struct _Sonometer *me, const char *data) __declspec(dllexport);
struct _SElException *SoundLevelMeter(struct _Sonometer *me, int group, float *data, int *count_data, char *log) __declspec(dllexport);
struct _SElException *Write(struct _Sonometer *me, const char *Query) __declspec(dllexport);
void elexception_delete(struct _SElException **pException);
void elexception_log_set(void (*fn_log)(struct _SElException *));
struct _SElException *elexception_new(int errorCode, const char *messsage, const char *file, long line);
void excdisplay(struct _SElException *pException);
void excdispstr(struct _SElException *pException, char *str, int str_size);
       X   C V I D L L P R O T O T Y P E S   C V I D L L E X P O R T S 2       0 	                ?? ??     0	                                        