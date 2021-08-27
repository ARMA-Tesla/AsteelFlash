#include <windows.h>
#include <appIO.h>

/* The two macros below are used as error return codes */
/* in case the DLL does not load, or is missing one or */
/* more functions, respectively.  You must define them */
/* to whatever values are meaningful for your DLL.     */
#define kFailedToLoadDLLError     101
#define kCouldNotFindFunction     102

static HINSTANCE DLLHandle;

/* Declare the variables that hold the addresses of the function   */
/* pointers.                                                       */
static BOOL (__stdcall *IO_Init_Ptr)(WORD wCard, short *status);
static BOOL (__stdcall *IO_Exit_Ptr)(WORD wCard, short *status);
static BOOL (__stdcall *IO_RefreshInput_Ptr)(WORD wCard, short *status);
static BOOL (__stdcall *IO_ReadIBit_Ptr)(WORD wCard, WORD wEquip,
                                         WORD offsetbit, WORD nb,
                                         BYTE *tabBit, short *status);
static BOOL (__stdcall *IO_ReadIByte_Ptr)(WORD wCard, WORD wEquip,
                                          WORD offset, WORD nb,
                                          BYTE *tabByte, short *status);
static BOOL (__stdcall *IO_ReadIWord_Ptr)(WORD wCard, WORD wEquip,
                                          WORD offset, WORD nb,
                                          WORD *tabWord, short *status);
static BOOL (__stdcall *IO_ReadIDWord_Ptr)(WORD wCard, WORD wEquip,
                                           WORD offset, WORD nb,
                                           DWORD *tabDWord, short *status);
static BOOL (__stdcall *IO_RefreshOutput_Ptr)(WORD wCard, short *status);
static BOOL (__stdcall *IO_WriteQBit_Ptr)(WORD wCard, WORD wEquip,
                                          WORD offsetbit, WORD nb,
                                          BYTE *tabBit, short *status);
static BOOL (__stdcall *IO_WriteQByte_Ptr)(WORD wCard, WORD wEquip,
                                           WORD offset, WORD nb,
                                           BYTE *tabByte, short *status);
static BOOL (__stdcall *IO_WriteQWord_Ptr)(WORD wCard, WORD wEquip,
                                           WORD offset, WORD nb,
                                           WORD *tabWord, short *status);
static BOOL (__stdcall *IO_WriteQDWord_Ptr)(WORD wCard, WORD wEquip,
                                            WORD offset, WORD nb,
                                            DWORD *tabDWord, short *status);
static BOOL (__stdcall *IO_GetEquipmentStatus_Ptr)(WORD wCard, WORD wEquip,
                                                   WORD *equipmentStatus,
                                                   short *status);
static BOOL (__stdcall *IO_GetGlobalStatus_Ptr)(WORD wCard,
                                                WORD *globalStatus,
                                                short *status);
static BOOL (__stdcall *IO_ReadDiag_Ptr)(WORD wCard, WORD wEquip,
                                         WORD *equipmentDiag,
                                         short *status);
static BOOL (__stdcall *IO_GetGlobalDiag_Ptr)(WORD wCard, WORD wEquip,
                                              WORD *globalDiag,
                                              short *status);
static BOOL (__stdcall *IO_GetEquipmentList_Ptr)(WORD wCard, WORD *nb,
                                                 WORD *tabEquip,
                                                 short *status);
static BOOL (__stdcall *IO_GetEquipmentInfo_Ptr)(WORD wCard, WORD wEquip,
                                                 WORD *inputSize,
                                                 WORD *outputSize,
                                                 short *status);
static BOOL (__stdcall *IO_GetDigitalInput_Ptr)(WORD wCard, short *status);
static BOOL (__stdcall *IO_SetWatchDog_Ptr)(WORD wCard, WORD timeWD,
                                            short *status);
static BOOL (__stdcall *IO_SetReply_Ptr)(WORD wCard, WORD timeReply,
                                         short *status);


/* Load the DLL and get the addresses of the functions */
static int LoadDLLIfNeeded(void)
{
    if (DLLHandle)
        return 0;

    DLLHandle = LoadLibrary("AppIo.dll");
    if (DLLHandle == NULL) {
        return kFailedToLoadDLLError;
        }

    if (!(IO_Init_Ptr = (void*) GetProcAddress(DLLHandle, "IO_Init")))
        goto FunctionNotFoundError;

    if (!(IO_Exit_Ptr = (void*) GetProcAddress(DLLHandle, "IO_Exit")))
        goto FunctionNotFoundError;

    if (!(IO_RefreshInput_Ptr = (void*) GetProcAddress(DLLHandle, 
         "IO_RefreshInput")))
        goto FunctionNotFoundError;

    if (!(IO_ReadIBit_Ptr = (void*) GetProcAddress(DLLHandle, "IO_ReadIBit")))
        goto FunctionNotFoundError;

    if (!(IO_ReadIByte_Ptr = (void*) GetProcAddress(DLLHandle, "IO_ReadIByte")))
        goto FunctionNotFoundError;

    if (!(IO_ReadIWord_Ptr = (void*) GetProcAddress(DLLHandle, "IO_ReadIWord")))
        goto FunctionNotFoundError;

    if (!(IO_ReadIDWord_Ptr = (void*) GetProcAddress(DLLHandle, "IO_ReadIDWord")))
        goto FunctionNotFoundError;

    if (!(IO_RefreshOutput_Ptr = (void*) GetProcAddress(DLLHandle, 
         "IO_RefreshOutput")))
        goto FunctionNotFoundError;

    if (!(IO_WriteQBit_Ptr = (void*) GetProcAddress(DLLHandle, "IO_WriteQBit")))
        goto FunctionNotFoundError;

    if (!(IO_WriteQByte_Ptr = (void*) GetProcAddress(DLLHandle, "IO_WriteQByte")))
        goto FunctionNotFoundError;

    if (!(IO_WriteQWord_Ptr = (void*) GetProcAddress(DLLHandle, "IO_WriteQWord")))
        goto FunctionNotFoundError;

    if (!(IO_WriteQDWord_Ptr = (void*) GetProcAddress(DLLHandle, 
         "IO_WriteQDWord")))
        goto FunctionNotFoundError;

    if (!(IO_GetEquipmentStatus_Ptr = (void*) GetProcAddress(DLLHandle, 
         "IO_GetEquipmentStatus")))
        goto FunctionNotFoundError;

    if (!(IO_GetGlobalStatus_Ptr = (void*) GetProcAddress(DLLHandle, 
         "IO_GetGlobalStatus")))
        goto FunctionNotFoundError;

    if (!(IO_ReadDiag_Ptr = (void*) GetProcAddress(DLLHandle, "IO_ReadDiag")))
        goto FunctionNotFoundError;

    if (!(IO_GetGlobalDiag_Ptr = (void*) GetProcAddress(DLLHandle, 
         "IO_GetGlobalDiag")))
        goto FunctionNotFoundError;

    if (!(IO_GetEquipmentList_Ptr = (void*) GetProcAddress(DLLHandle, 
         "IO_GetEquipmentList")))
        goto FunctionNotFoundError;

    if (!(IO_GetEquipmentInfo_Ptr = (void*) GetProcAddress(DLLHandle, 
         "IO_GetEquipmentInfo")))
        goto FunctionNotFoundError;

    if (!(IO_GetDigitalInput_Ptr = (void*) GetProcAddress(DLLHandle, 
         "IO_GetDigitalInput")))
        goto FunctionNotFoundError;

    if (!(IO_SetWatchDog_Ptr = (void*) GetProcAddress(DLLHandle, 
         "IO_SetWatchDog")))
        goto FunctionNotFoundError;

    if (!(IO_SetReply_Ptr = (void*) GetProcAddress(DLLHandle, "IO_SetReply")))
        goto FunctionNotFoundError;

    return 0;

FunctionNotFoundError:
    FreeLibrary(DLLHandle);
    DLLHandle = 0;
    return kCouldNotFindFunction;
}


/* Glue Code for each of the DLL functions */



BOOL __stdcall IO_Init(WORD wCard, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_Init_Ptr)(wCard, status);
}


BOOL __stdcall IO_Exit(WORD wCard, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_Exit_Ptr)(wCard, status);
}


BOOL __stdcall IO_RefreshInput(WORD wCard, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_RefreshInput_Ptr)(wCard, status);
}


BOOL __stdcall IO_ReadIBit(WORD wCard, WORD wEquip, WORD offsetbit, WORD nb, 
                           BYTE *tabBit, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_ReadIBit_Ptr)(wCard, wEquip, offsetbit, nb, tabBit, status);
}


BOOL __stdcall IO_ReadIByte(WORD wCard, WORD wEquip, WORD offset, WORD nb, 
                            BYTE *tabByte, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_ReadIByte_Ptr)(wCard, wEquip, offset, nb, tabByte, status);
}


BOOL __stdcall IO_ReadIWord(WORD wCard, WORD wEquip, WORD offset, WORD nb, 
                            WORD *tabWord, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_ReadIWord_Ptr)(wCard, wEquip, offset, nb, tabWord, status);
}


BOOL __stdcall IO_ReadIDWord(WORD wCard, WORD wEquip, WORD offset, WORD nb, 
                             DWORD *tabDWord, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_ReadIDWord_Ptr)(wCard, wEquip, offset, nb, tabDWord, status);
}


BOOL __stdcall IO_RefreshOutput(WORD wCard, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_RefreshOutput_Ptr)(wCard, status);
}


BOOL __stdcall IO_WriteQBit(WORD wCard, WORD wEquip, WORD offsetbit, 
                            WORD nb, BYTE *tabBit, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_WriteQBit_Ptr)(wCard, wEquip, offsetbit, nb, tabBit, status);
}


BOOL __stdcall IO_WriteQByte(WORD wCard, WORD wEquip, WORD offset, WORD nb, 
                             BYTE *tabByte, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_WriteQByte_Ptr)(wCard, wEquip, offset, nb, tabByte, status);
}


BOOL __stdcall IO_WriteQWord(WORD wCard, WORD wEquip, WORD offset, WORD nb, 
                             WORD *tabWord, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_WriteQWord_Ptr)(wCard, wEquip, offset, nb, tabWord, status);
}


BOOL __stdcall IO_WriteQDWord(WORD wCard, WORD wEquip, WORD offset, WORD nb, 
                              DWORD *tabDWord, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_WriteQDWord_Ptr)(wCard, wEquip, offset, nb, tabDWord, 
                                 status);
}


BOOL __stdcall IO_GetEquipmentStatus(WORD wCard, WORD wEquip, 
                                     WORD *equipmentStatus, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_GetEquipmentStatus_Ptr)(wCard, wEquip, equipmentStatus, 
                                        status);
}


BOOL __stdcall IO_GetGlobalStatus(WORD wCard, WORD *globalStatus, 
                                  short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_GetGlobalStatus_Ptr)(wCard, globalStatus, status);
}


BOOL __stdcall IO_ReadDiag(WORD wCard, WORD wEquip, WORD *equipmentDiag, 
                           short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_ReadDiag_Ptr)(wCard, wEquip, equipmentDiag, status);
}


BOOL __stdcall IO_GetGlobalDiag(WORD wCard, WORD wEquip, WORD *globalDiag, 
                                short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_GetGlobalDiag_Ptr)(wCard, wEquip, globalDiag, status);
}


BOOL __stdcall IO_GetEquipmentList(WORD wCard, WORD *nb, WORD *tabEquip, 
                                   short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_GetEquipmentList_Ptr)(wCard, nb, tabEquip, status);
}


BOOL __stdcall IO_GetEquipmentInfo(WORD wCard, WORD wEquip, WORD *inputSize, 
                                   WORD *outputSize, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_GetEquipmentInfo_Ptr)(wCard, wEquip, inputSize, outputSize, 
                                      status);
}


BOOL __stdcall IO_GetDigitalInput(WORD wCard, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_GetDigitalInput_Ptr)(wCard, status);
}


BOOL __stdcall IO_SetWatchDog(WORD wCard, WORD timeWD, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_SetWatchDog_Ptr)(wCard, timeWD, status);
}


BOOL __stdcall IO_SetReply(WORD wCard, WORD timeReply, short *status)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*IO_SetReply_Ptr)(wCard, timeReply, status);
}

