        ??  ??                  ?       ?? ??     0 	        <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0" />?      ?? ??     0 	        ?4   V S _ V E R S I O N _ I N F O     ???                 ?                        ?   S t r i n g F i l e I n f o   ?   0 4 0 9 0 4 b 0        C o m p a n y N a m e     N   F i l e D e s c r i p t i o n     c o g n e x   ( D e b u g   x 8 6 )     (   F i l e V e r s i o n     1 . 0   .   I n t e r n a l N a m e   c o g n e x     H   L e g a l C o p y r i g h t   C o p y r i g h t   ?     2 0 2 0   >   O r i g i n a l F i l e n a m e   c o g n e x . d l l     0   P r o d u c t N a m e       c o g n e x   ,   P r o d u c t V e r s i o n   1 . 0   D    V a r F i l e I n f o     $    T r a n s l a t i o n     	??  X   C V I D L L P R O T O T Y P E S   C V I D L L E X P O R T S         0 	        int Cognex_Get_Value_Integer(char *Colonne, int Ligne);
struct _SElException *Cognex_Job(struct _Camera *me, const char *JobID) __declspec(dllexport);
struct _SElException *Cognex_Offline(struct _Camera *me) __declspec(dllexport);
struct _SElException *Cognex_Online(struct _Camera *me) __declspec(dllexport);
struct _SElException *Cognex_Read(struct _Camera *me, char *data) __declspec(dllexport);
int Cognex_Set_Value_Integer(char *Colonne, int Ligne, int Value);
struct _SElException *Cognex_Trigger(struct _Camera *me) __declspec(dllexport);
struct _SElException *Cognex_act_job(struct _Camera *me, int *actual_job) __declspec(dllexport);
struct _SElException *Cognex_disconnect(struct _Camera *me) __declspec(dllexport);
struct _SElException *Cognex_login(struct _Camera *me) __declspec(dllexport);
struct _SElException *Cognex_set_number_boutton(struct _Camera *me, int phase) __declspec(dllexport);
struct _SElException *Cognex_set_param(struct _Camera *me, int phase) __declspec(dllexport);
struct _SElException *Cognex_set_parameters(struct _Camera *me, char *Colonne, int index_pico) __declspec(dllexport);
int ReadResponseTelnet(int Length, char *String_To_Read);
int ServerTCPCB(unsigned int handle, int event, int error, void *callbackData);
void elexception_delete(struct _SElException **pException);
void elexception_log_set(void (*fn_log)(struct _SElException *));
struct _SElException *elexception_new(int errorCode, const char *messsage, const char *file, long line);
void excdisplay(struct _SElException *pException);
void excdispstr(struct _SElException *pException, char *str, int str_size);
struct _SElException *openTCPserver(struct _Camera *me) __declspec(dllexport);
        X   C V I D L L P R O T O T Y P E S   C V I D L L E X P O R T S 2       0 	                ?? ??     0	                                        