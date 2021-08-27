#include <cvidef.h>
typedef unsigned short WORD;
typedef unsigned char BYTE;

void _stdcall initbus (short *status);

BOOL _stdcall IO_Init (WORD wCard, short *status);
BOOL _stdcall IO_Exit (WORD wCard, short *status);
BOOL _stdcall IO_RefreshInput (WORD wCard, short *status);
BOOL _stdcall IO_ReadIBit (WORD wCard, WORD wEquip, WORD offsetbit, WORD nb,
                           BYTE *tabBit, short *status);
BOOL _stdcall IO_ReadIByte (WORD wCard, WORD wEquip, WORD offset, WORD nb,
                            BYTE *tabByte, short *status);
BOOL _stdcall IO_ReadIWord (WORD wCard, WORD wEquip, WORD offset, WORD nb,
                            WORD *tabWord, short *status);
BOOL _stdcall IO_ReadIDWord (WORD wCard, WORD wEquip, WORD offset, WORD nb,
                             DWORD *tabDWord, short *status);
BOOL _stdcall IO_RefreshOutput (WORD wCard, short *status);
BOOL _stdcall IO_WriteQBit (WORD wCard, WORD wEquip, WORD offsetbit, WORD nb,
                            BYTE *tabBit, short *status);
BOOL _stdcall IO_WriteQByte (WORD wCard, WORD wEquip, WORD offset, WORD nb,
                             BYTE *tabByte, short *status);
BOOL _stdcall IO_WriteQWord (WORD wCard, WORD wEquip, WORD offset, WORD nb,
                             WORD *tabWord, short *status);
BOOL _stdcall IO_WriteQDWord (WORD wCard, WORD wEquip, WORD offset, WORD nb,
                              DWORD *tabDWord, short *status);
BOOL _stdcall IO_GetEquipmentStatus (WORD wCard, WORD wEquip, WORD *equipmentStatus,
                                     short *status);
BOOL _stdcall IO_GetGlobalStatus (WORD wCard, WORD *globalStatus, short *status);
BOOL _stdcall IO_ReadDiag (WORD wCard, WORD wEquip, WORD *equipmentDiag,
                           short *status);
BOOL _stdcall IO_GetGlobalDiag (WORD wCard, WORD wEquip, WORD *globalDiag,
                                short *status);
BOOL _stdcall IO_GetEquipmentList (WORD wCard, WORD *nb, WORD *tabEquip,
                                   short *status);
BOOL _stdcall IO_GetEquipmentInfo (WORD wCard, WORD wEquip, WORD *inputSize,
                                   WORD *outputSize, short *status);
BOOL _stdcall IO_GetDigitalInput (WORD wCard, short *status);
BOOL _stdcall IO_SetWatchDog (WORD wCard, WORD timeWD, short *status);
BOOL _stdcall IO_SetReply (WORD wCard, WORD timeReply, short *status);

