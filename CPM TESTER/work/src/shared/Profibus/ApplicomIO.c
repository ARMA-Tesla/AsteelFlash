#include <windows.h>
#include <ApplicomIO.h>

/* The two macros below are used as error return codes */
/* in case the DLL does not load, or is missing one or */
/* more functions, respectively.  You must define them */
/* to whatever values are meaningful for your DLL.     */
#define kFailedToLoadDLLError     101
#define kCouldNotFindFunction     102

static HINSTANCE DLLHandle;

/* Declare the variables that hold the addresses of the function   */
/* pointers.                                                       */
static BOOL (__stdcall *AuInitBus_io_Ptr)(DWORD *pdwStatus);
static BOOL (__stdcall *AuExitBus_io_Ptr)(DWORD *pdwStatus);
static BOOL (__stdcall *AuWriteReadMsg_io_Ptr)(WORD wChan, WORD wNes,
                                               DWORD dwMsgParam,
                                               WORD wNbTx, BYTE *lpbyBufTx,
                                               WORD *pwNbRx,
                                               BYTE *lpbyBufRx,
                                               DWORD *pdwStatus);


/* Load the DLL and get the addresses of the functions */
static int LoadDLLIfNeeded(void)
{
    if (DLLHandle)
        return 0;

    DLLHandle = LoadLibrary("applicomio.dll");
    if (DLLHandle == NULL) {
        return kFailedToLoadDLLError;
        }

    if (!(AuInitBus_io_Ptr = (void*) GetProcAddress(DLLHandle, "AuInitBus_io")))
        goto FunctionNotFoundError;

    if (!(AuExitBus_io_Ptr = (void*) GetProcAddress(DLLHandle, "AuExitBus_io")))
        goto FunctionNotFoundError;

    if (!(AuWriteReadMsg_io_Ptr = (void*) GetProcAddress(DLLHandle, 
         "AuWriteReadMsg_io")))
        goto FunctionNotFoundError;

    return 0;

FunctionNotFoundError:
    FreeLibrary(DLLHandle);
    DLLHandle = 0;
    return kCouldNotFindFunction;
}


/* Glue Code for each of the DLL functions */



BOOL __stdcall AuInitBus_io(DWORD *pdwStatus)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*AuInitBus_io_Ptr)(pdwStatus);
}


BOOL __stdcall AuExitBus_io(DWORD *pdwStatus)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*AuExitBus_io_Ptr)(pdwStatus);
}


BOOL __stdcall AuWriteReadMsg_io(WORD wChan, WORD wNes, DWORD dwMsgParam, 
                                 WORD wNbTx, BYTE *lpbyBufTx, WORD *pwNbRx, 
                                 BYTE *lpbyBufRx, DWORD *pdwStatus)
{
    int dllLoadError;

    if (dllLoadError = LoadDLLIfNeeded())
        return dllLoadError;
    return (*AuWriteReadMsg_io_Ptr)(wChan, wNes, dwMsgParam, wNbTx, 
                                    lpbyBufTx, pwNbRx, lpbyBufRx, pdwStatus);
}

