        ??  ??                  ?       ?? ??     0 	        <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0" />p      ?? ??     0 	        p4   V S _ V E R S I O N _ I N F O     ???                 ?                         ?   S t r i n g F i l e I n f o   ?   0 4 0 9 0 4 b 0        C o m p a n y N a m e     L   F i l e D e s c r i p t i o n     B S H   ( R e l e a s e   x 8 6 )   (   F i l e V e r s i o n     1 . 0   (   I n t e r n a l N a m e   B S H   H   L e g a l C o p y r i g h t   C o p y r i g h t   ?     2 0 2 0   8   O r i g i n a l F i l e n a m e   B S H . d l l   *   P r o d u c t N a m e       B S H     ,   P r o d u c t V e r s i o n   1 . 0   D    V a r F i l e I n f o     $    T r a n s l a t i o n     	??  X   C V I D L L P R O T O T Y P E S   C V I D L L E X P O R T S         0 	        struct _SElException *Connect(struct _Traca *me) __declspec(dllexport);
struct _SElException *Disconnect(struct _Traca *me) __declspec(dllexport);
struct _SElException *Get_BoardVer_pFin(struct _Traca *me, int id_board_pFin, char *Board_Ver_pFin) __declspec(dllexport);
struct _SElException *Get_BoardVer_sFin_Active(struct _Traca *me, int id_board_sFin, int *valide, int *active) __declspec(dllexport);
struct _SElException *Get_BoardVer_sFin_Valide(struct _Traca *me, char *board_Ver_sFin, int *id_board_sFin, int *valide) __declspec(dllexport);
struct _SElException *Get_FieldLabel(struct _Traca *me, int id_board_pFin, int *count, char FieldName[20][128], char FieldData[20][128]) __declspec(dllexport);
struct _SElException *Get_Last_Mouvement(struct _Traca *me, char *Num_Serie, char *TypeTest, int *result) __declspec(dllexport);
struct _SElException *Get_Reference_Product(struct _Traca *me, int id_board_sFin, int *id_board_pFin, char *ProductRefernce) __declspec(dllexport);
struct _SElException *Get_SMM_SN(struct _Traca *me, char *Num_Serie_sFin, char *Num_Serie_SMM) __declspec(dllexport);
struct _SElException *Set_Association(struct _Traca *me, char *Num_Serie_sFin, char *Num_Serie_pFin, char *Num_Serie_Client, char *date_association) __declspec(dllexport);
struct _SElException *Set_Last_Mouvement(struct _Traca *me, char *Num_Serie, int id_board_pfin, char *DateDebut, char *DateFin, char *Id_Machine, char *TypeTest, int result) __declspec(dllexport);
struct _SElException *Test_SMM(struct _Traca *me, char *Board_Ver_pFin, int *Subassembly) __declspec(dllexport);
void elexception_delete(struct _SElException **pException);
void elexception_log_set(void (*fn_log)(struct _SElException *));
struct _SElException *elexception_new(int errorCode, const char *messsage, const char *file, long line);
void excdisplay(struct _SElException *pException);
void excdispstr(struct _SElException *pException, char *str, int str_size);
         ?? ??     0	                                        