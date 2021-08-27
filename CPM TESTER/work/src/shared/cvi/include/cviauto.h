#ifndef AUTOSUPP_H
#define AUTOSUPP_H

#if defined(INCLUDE_AFTER_WINDOWS_H) && !defined(_INC_WINDOWS)
#error  This header must be included before utility.h and formatio.h
#error  because this header includes Windows SDK headers.
#endif /* INCLUDE_AFTER_WINDOWS_H */

#ifndef _WIN32_DCOM
/* DCOM is required by various features. */
#define _WIN32_DCOM
#endif /* ndef _WIN32_DCOM */

#include <windows.h>
#include <winuser.h>
#include <ole2.h>
#include <winnt.h>
#include <stdarg.h>
#include <cvidef.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push)
#pragma pack(8)

#if defined(_CVI_) && !defined(__TPC__)
#pragma EnableLibraryRuntimeChecking
#endif

#define MAX_PATHNAME_LEN  260    /* includes nul byte */
#define MAX_DRIVENAME_LEN   3    /* includes nul byte */
#define MAX_DIRNAME_LEN   256    /* includes nul byte */
#define MAX_FILENAME_LEN  256    /* includes nul byte */

#define CVIAUTOAPI  HRESULT     CVIFUNC
#define CVIAUTOAPIV HRESULT     CVIFUNC_C
#define CVIAUTOAPI_(retType)    retType CVIFUNC

#ifndef CAOBJHANDLE_DEFINED
#define CAOBJHANDLE_DEFINED
typedef int          CAObjHandle;
#endif /* CAOBJHANDLE_DEFINED */

typedef VARIANT_BOOL VBOOL;     /* VARIANT_BOOL is just too long */

#define VTRUE   VARIANT_TRUE
#define VFALSE  VARIANT_FALSE

/* Use this macro to access elements of a 2D C array                    */
/* array is a pointer to the 2D array                                   */
/* xSize is the number of elements in the 1st dimension. It is not used */
/* ySize is the number of elements in the 2nd dimension                 */
/* xIndex and yIndex are the 1st and 2nd indices                        */
/* datatype is the C data type. For example, double                     */
#define CA_Get2DArrayElement(array, xSize, ySize, xIndex, yIndex, datatype)  (*(datatype*)(((char*)array)+((xIndex*ySize)+yIndex)*sizeof(datatype)))

/* Use this macro to check return values of functions that return       */
/* HRESULTs.  To use this macro in a function there must be a local     */
/* variable named "__result" of type HRESULT and an "Error:" label      */
/* which preceeds the error recovery code at the end of the function.   */
#define __caErrChk(fCall)\
\
if (FAILED(__result = (fCall))) {\
    goto Error;\
} else

/* CVI libraries based on the Automation Library will use the following  */
/* base value for their error codes.                                     */
#define E_CVIAUTO_USER_ERROR_BASE ((HRESULT)0x80040500)

/* Error Codes: The Automation library functions may return either these */
/*              error codes or any of the error codes in winerror.h      */
#define     E_CVIAUTO_NO_ERROR                  0x0
#define     E_CVIAUTO_INVALID_TYPE_DESC         /* 0x80040201 */    MAKE_HRESULT(1, FACILITY_ITF, 0x201)
#define     E_CVIAUTO_INVALID_RETURN_TYPE       /* 0x80040202 */    MAKE_HRESULT(1, FACILITY_ITF, 0x202)
#define     E_CVIAUTO_USE_CAVT_TYPE_DESC        /* 0x80040203 */    MAKE_HRESULT(1, FACILITY_ITF, 0x203)
#define     E_CVIAUTO_INVALID_NUM_DIM           /* 0x80040204 */    MAKE_HRESULT(1, FACILITY_ITF, 0x204)
#define     E_CVIAUTO_DIFF_SAFEARRAY_TYPE       /* 0x80040205 */    MAKE_HRESULT(1, FACILITY_ITF, 0x205)
#define     E_CVIAUTO_VARIANT_NOT_SAFEARRAY     /* 0x80040206 */    MAKE_HRESULT(1, FACILITY_ITF, 0x206)
#define     E_CVIAUTO_NULL_RET_VAL_PARAM        /* 0x80040207 */    MAKE_HRESULT(1, FACILITY_ITF, 0x207)
#define     E_CVIAUTO_DLL_LOAD_FAILED           /* 0x80040208 */    MAKE_HRESULT(1, FACILITY_ITF, 0x208)
#define     E_CVIAUTO_BAD_DLL_VERSION           /* 0x80040209 */    MAKE_HRESULT(1, FACILITY_ITF, 0x209)
#define     E_CVIAUTO_COULD_NOT_CREATE_MUTEX    /* 0x8004020a */    MAKE_HRESULT(1, FACILITY_ITF, 0x20a)
#define     E_CVIAUTO_WRONG_THREAD              /* 0x8004020b */    MAKE_HRESULT(1, FACILITY_ITF, 0x20b)
#define     E_CVIAUTO_NO_CALLBACKS              /* 0x8004020c */    MAKE_HRESULT(1, FACILITY_ITF, 0x20c)
#define     E_CVIAUTO_INVALID_CALLBACKID        /* 0x8004020d */    MAKE_HRESULT(1, FACILITY_ITF, 0x20d)
#define     E_CVIAUTO_CALLBACK_REGISTERED       /* 0x8004020e */    MAKE_HRESULT(1, FACILITY_ITF, 0x20e)
#define     E_CVIAUTO_CALLBACK_NOT_REGISTERED   /* 0x8004020f */    MAKE_HRESULT(1, FACILITY_ITF, 0x20f)
#define     E_CVIAUTO_INVALID_METHOD_INDEX      /* 0x80040210 */    MAKE_HRESULT(1, FACILITY_ITF, 0x210)
#define     E_CVIAUTO_INVALID_INTERFACE_PTR     /* 0x80040211 */    MAKE_HRESULT(1, FACILITY_ITF, 0x211)
#define     E_CVIAUTO_SERVER_NOT_INITIALIZED    /* 0x80040212 */    MAKE_HRESULT(1, FACILITY_ITF, 0x212)
#define     E_CVIAUTO_SERVER_INVALID_TYPE       /* 0x80040213 */    MAKE_HRESULT(1, FACILITY_ITF, 0x213)
#define     E_CVIAUTO_SERVER_INPROC_ONLY        /* 0x80040214 */    MAKE_HRESULT(1, FACILITY_ITF, 0x214)
#define     E_CVIAUTO_SERVER_NOT_AUTOMATION     /* 0x80040215 */    MAKE_HRESULT(1, FACILITY_ITF, 0x215)
#define     E_CVIAUTO_SERVER_INVALID_AGGREG     /* 0x80040216 */    MAKE_HRESULT(1, FACILITY_ITF, 0x216)
#define     E_CVIAUTO_SERVER_IFACE_NOT_AGGREG   /* 0x80040217 */    MAKE_HRESULT(1, FACILITY_ITF, 0x217)
#define     E_CVIAUTO_SERVER_INITIALIZATION     /* 0x80040218 */    MAKE_HRESULT(1, FACILITY_ITF, 0x218)
#define     E_CVIAUTO_SERVER_CLASS_NOT_REG      /* 0x80040219 */    MAKE_HRESULT(1, FACILITY_ITF, 0x219)
#define     E_CVIAUTO_CVI_UI_ERROR              /* 0x80040220 */    MAKE_HRESULT(1, FACILITY_ITF, 0x220)
#define     E_CVIAUTO_SERVER_OBJ_NO_ERRORINFO   /* 0x80040221 */    MAKE_HRESULT(1, FACILITY_ITF, 0x221)
#define     E_CVIAUTO_SERVER_ITF_NO_ERRORINFO   /* 0x80040222 */    MAKE_HRESULT(1, FACILITY_ITF, 0x222)
#define     E_CVIAUTO_SERVER_IN_CALLBACK        /* 0x80040223 */    MAKE_HRESULT(1, FACILITY_ITF, 0x223)

/* Use these to specify the type of a function parameter, function return */
/* value, and a property.                                                 */
enum {
    CAVT_EMPTY = 100,
    CAVT_NULL,
    CAVT_SHORT,
    CAVT_LONG,
    CAVT_INT = CAVT_LONG,
    CAVT_FLOAT,
    CAVT_DOUBLE,
    CAVT_CY,
    CAVT_DATE,
    CAVT_BSTR,
    CAVT_DISPATCH,
    CAVT_ERROR,
    CAVT_BOOL,
    CAVT_VARIANT,
    CAVT_UNKNOWN,
    CAVT_UCHAR,
    CAVT_CSTRING,       /* null terminated ansi (multi-byte) string */
    CAVT_OBJHANDLE,
    CAVT_CHAR,
    CAVT_USHORT,
    CAVT_ULONG,
    CAVT_UINT,
    CAVT_BASICTYPE_MASK =   0x0fff,
    CAVT_MODIFIER_MASK =    0xf000,
    CAVT_ARRAY =            0x1000,
    CAVT_BYREF =            0x2000,
    CAVT_IN =               0x4000,
    CAVT_OUT =              0x8000,
    CAVT_AUTO_CLEAR =       0x0800      /* valid only for VARIANT arguments. VariantClear is called on the variant after the function call */
};

/* Used to specify whether a parameter is an input, output, or both input */
/* and output parameter                                                   */
#define CAVT_BYREFI (CAVT_BYREF | CAVT_IN)
#define CAVT_BYREFO (CAVT_BYREF | CAVT_OUT)
#define CAVT_BYREFIO    (CAVT_BYREF | CAVT_IN | CAVT_OUT)


/* Extended Error Structure */
#define ERRORINFO_SRC_BUF_SIZE      256     // including NULL
#define ERRORINFO_DESC_BUF_SIZE     1024    // including NULL

/* Option flags for CA_SafeArrayTo1DArrayEx, CA_SafeArrayTo2DArrayEx,   */
/* CA_SafeArrayTo1DArrayBufEx, CA_SafeArrayTo2DArrayBufEx.              */
#define CVIAUTO_RETAIN_SAFEARRAY            0x00000001      /* Don't free SafeArray and its contents when converting. */

typedef struct {
    unsigned short wCode;                       // An error code describing the error.
    SCODE sCode;                                // A return value describing the error.
    char source[ERRORINFO_SRC_BUF_SIZE];        // Source of the exception.
    char description[ERRORINFO_DESC_BUF_SIZE];  // Textual description of the error.
    char helpFile[MAX_PATHNAME_LEN];            // Help file path.
    unsigned long helpContext;                  // Help context ID.
    int errorParamPos;                          // 1 based position of invalid parameter
} ERRORINFO;


VARIANT CVIFUNC CA_VariantLong(long val);
VARIANT CVIFUNC CA_VariantInt(int val);
VARIANT CVIFUNC CA_VariantShort(short val);
VARIANT CVIFUNC CA_VariantBool(VBOOL val);
VARIANT CVIFUNC CA_VariantFloat(float val);
VARIANT CVIFUNC CA_VariantDouble(double val);
VARIANT CVIFUNC CA_VariantBSTR(BSTR val);
VARIANT CVIFUNC CA_VariantEmpty(void);
VARIANT CVIFUNC CA_VariantNULL(void);
VARIANT CVIFUNC CA_VariantCurrency(CURRENCY val);
VARIANT CVIFUNC CA_VariantDate(DATE val);
VARIANT CVIFUNC CA_VariantDispatch(LPDISPATCH val);
VARIANT CVIFUNC CA_VariantIUnknown(LPUNKNOWN val);
VARIANT CVIFUNC CA_VariantError(SCODE val);
VARIANT CVIFUNC CA_VariantUChar(unsigned char val);
VARIANT CVIFUNC CA_VariantChar(char val);
VARIANT CVIFUNC CA_VariantUShort(unsigned short val);
VARIANT CVIFUNC CA_VariantULong(unsigned long val);
VARIANT CVIFUNC CA_VariantUInt(unsigned int val);

/* default value for optional parameters */
#define CA_DEFAULT_VAL   CA_DefaultValueVariant()
VARIANT CVIFUNC CA_DefaultValueVariant(void);


/* Variant Query Functions */
HRESULT CVIFUNC CA_VariantHasArray(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasPtr(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasLong(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasInt(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasShort(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasBool(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasFloat(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasDouble(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasCString(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasBSTR(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasNull(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasCurrency(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasDate(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasIUnknown(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasDispatch(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasObjHandle(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasUChar(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasChar(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasUShort(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasULong(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasUInt(const VARIANT *v);
HRESULT CVIFUNC CA_VariantHasError(const VARIANT *v);
HRESULT CVIFUNC CA_VariantIsEmpty(const VARIANT *v);


/* Setting and Getting Values from Variants */
HRESULT CVIFUNC CA_VariantSetLong(VARIANT *v, long val);
HRESULT CVIFUNC CA_VariantSetInt(VARIANT *v, int val);
HRESULT CVIFUNC CA_VariantSetShort(VARIANT *v, short val);
HRESULT CVIFUNC CA_VariantSetBool(VARIANT *v, VBOOL val);
HRESULT CVIFUNC CA_VariantSetFloat(VARIANT *v, float val);
HRESULT CVIFUNC CA_VariantSetDouble(VARIANT *v, double val);
HRESULT CVIFUNC CA_VariantSetSafeArray(VARIANT *v, unsigned int cavt, LPSAFEARRAY val);
HRESULT CVIFUNC CA_VariantSet1DArray(VARIANT *v, unsigned int cavt, unsigned int numElements, const void *array);
HRESULT CVIFUNC CA_VariantSet2DArray(VARIANT *v, unsigned int cavt, unsigned int dim1Size, unsigned int dim2Size, const void *array);
HRESULT CVIFUNC CA_VariantSetBSTR(VARIANT *v, BSTR val);
HRESULT CVIFUNC CA_VariantSetEmpty(VARIANT *v);
HRESULT CVIFUNC CA_VariantSetNULL(VARIANT *v);
HRESULT CVIFUNC CA_VariantSetCurrency(VARIANT *v, CURRENCY val);
HRESULT CVIFUNC CA_VariantSetDate(VARIANT *v, DATE val);
HRESULT CVIFUNC CA_VariantSetDispatch(VARIANT *v, LPDISPATCH val);
HRESULT CVIFUNC CA_VariantSetIUnknown(VARIANT *v, LPUNKNOWN val);
HRESULT CVIFUNC CA_VariantSetError(VARIANT *v, SCODE val);
HRESULT CVIFUNC CA_VariantSetUChar(VARIANT *v, unsigned char val);
HRESULT CVIFUNC CA_VariantSetChar(VARIANT *v, char val);
HRESULT CVIFUNC CA_VariantSetUShort(VARIANT *v, unsigned short val);
HRESULT CVIFUNC CA_VariantSetULong(VARIANT *v, unsigned long val);
HRESULT CVIFUNC CA_VariantSetUInt(VARIANT *v, unsigned int val);
HRESULT CVIFUNC CA_VariantSetObjHandle(VARIANT *v, CAObjHandle objHandle, unsigned int cavt);
HRESULT CVIFUNC CA_VariantSetCString(VARIANT *v, const char *charString);
HRESULT CVIFUNC CA_VariantSetLongPtr(VARIANT *v, long *val);
HRESULT CVIFUNC CA_VariantSetIntPtr(VARIANT *v, int *val);
HRESULT CVIFUNC CA_VariantSetShortPtr(VARIANT *v, short *val);
HRESULT CVIFUNC CA_VariantSetBoolPtr(VARIANT *v, VBOOL *val);
HRESULT CVIFUNC CA_VariantSetFloatPtr(VARIANT *v, float *val);
HRESULT CVIFUNC CA_VariantSetDoublePtr(VARIANT *v, double *val);
HRESULT CVIFUNC CA_VariantSetSafeArrayPtr(VARIANT *v, unsigned int cavt, LPSAFEARRAY *val);
HRESULT CVIFUNC CA_VariantSetBSTRPtr(VARIANT *v, BSTR *val);
HRESULT CVIFUNC CA_VariantSetCurrencyPtr(VARIANT *v, CURRENCY *val);
HRESULT CVIFUNC CA_VariantSetDatePtr(VARIANT *v, DATE *val);
HRESULT CVIFUNC CA_VariantSetDispatchPtr(VARIANT *v, LPDISPATCH *val);
HRESULT CVIFUNC CA_VariantSetIUnknownPtr(VARIANT *v, LPUNKNOWN *val);
HRESULT CVIFUNC CA_VariantSetErrorPtr(VARIANT *v, SCODE *val);
HRESULT CVIFUNC CA_VariantSetUCharPtr(VARIANT *v, unsigned char *val);
HRESULT CVIFUNC CA_VariantSetVariantPtr(VARIANT *v, VARIANT *val);
HRESULT CVIFUNC CA_VariantSetCharPtr(VARIANT *v, char *val);
HRESULT CVIFUNC CA_VariantSetUShortPtr(VARIANT *v, unsigned short *val);
HRESULT CVIFUNC CA_VariantSetULongPtr(VARIANT *v, unsigned long *val);
HRESULT CVIFUNC CA_VariantSetUIntPtr(VARIANT *v, unsigned int *val);


/* CA_VariantGetType returns 0 if the variant does not contain */
/* a valid value, otherwise it returs a CAVT_ constant         */
unsigned int CVIFUNC CA_VariantGetType(const VARIANT *variant);

HRESULT CVIFUNC CA_VariantGetLong(const VARIANT *v, long *longPtr);
HRESULT CVIFUNC CA_VariantGetInt(const VARIANT *v, int *intPtr);
HRESULT CVIFUNC CA_VariantGetShort(const VARIANT *v, short *shortPtr);
HRESULT CVIFUNC CA_VariantGetBool(const VARIANT *v, VBOOL *boolPtr);
HRESULT CVIFUNC CA_VariantGetFloat(const VARIANT *v, float *floatPtr);
HRESULT CVIFUNC CA_VariantGetDouble(const VARIANT *v, double *doublePtr);
HRESULT CVIFUNC CA_VariantGetSafeArray(VARIANT *v, unsigned int cavt, LPSAFEARRAY *arrayPtr);
HRESULT CVIFUNC CA_VariantGet1DArray(VARIANT *v, unsigned int cavt, void *array, unsigned int *dimSz);
HRESULT CVIFUNC CA_VariantGet2DArray(VARIANT *v, unsigned int cavt, void *array, unsigned int *dim1Sz, unsigned int *dim2Sz);
HRESULT CVIFUNC CA_VariantGet1DArrayBuf(VARIANT *v, unsigned int cavt, void *buf,
                                    unsigned int bufSizeInBytes, unsigned int *numElements);
HRESULT CVIFUNC CA_VariantGet2DArrayBuf(VARIANT *v, unsigned int cavt, void *buf,
                                    unsigned int bufSizeInBytes, unsigned int *dim1NumElements,
                                    unsigned int *dim2NumElements);
HRESULT CVIFUNC CA_VariantGetArrayNumDims(const VARIANT *v, unsigned int *numDimensions);

HRESULT CVIFUNC CA_VariantGet1DArraySize(const VARIANT *v, unsigned int *numElements);
HRESULT CVIFUNC CA_VariantGet2DArraySize(const VARIANT *v, unsigned int *dim1NumElements, 
                               unsigned int *dim2NumElements);

HRESULT CVIFUNC CA_VariantGetBSTR(VARIANT *v, BSTR *bstrPtr);
HRESULT CVIFUNC CA_VariantGetCurrency(const VARIANT *v, CURRENCY *cyPtr);
HRESULT CVIFUNC CA_VariantGetDate(const VARIANT *v, DATE *datePtr);
HRESULT CVIFUNC CA_VariantGetDispatch(VARIANT *v, LPDISPATCH *dispPtr);
HRESULT CVIFUNC CA_VariantGetIUnknown(VARIANT *v, LPUNKNOWN *unkPtr);
HRESULT CVIFUNC CA_VariantGetError(const VARIANT *v, SCODE *errPtr);
HRESULT CVIFUNC CA_VariantGetUChar(const VARIANT *v, unsigned char *uCharPtr);
HRESULT CVIFUNC CA_VariantGetChar(const VARIANT *v, char *charPtr);
HRESULT CVIFUNC CA_VariantGetUShort(const VARIANT *v, unsigned short *uShortPtr);
HRESULT CVIFUNC CA_VariantGetULong(const VARIANT *v, unsigned long *uLongPtr);
HRESULT CVIFUNC CA_VariantGetUInt(const VARIANT *v, unsigned int *uIntPtr);
HRESULT CVIFUNC CA_VariantGetObjHandle(VARIANT *v, CAObjHandle *objHandle);
HRESULT CVIFUNC CA_VariantGetCStringLen(const VARIANT *v, unsigned long *len);
/* CA_VariantGetCStringBuf puts converted C string into buffer; */
/* the BSTR is freed if the function is successful              */
HRESULT CVIFUNC CA_VariantGetCStringBuf(VARIANT *v, char *stringBuf, unsigned long bufSize);
/* CA_VariantGetCString allocates storage for the C string; */
/* the BSTR is freed if the function is successful          */
HRESULT CVIFUNC CA_VariantGetCString(VARIANT *v, char**stringPtr);  
HRESULT CVIFUNC CA_VariantGetLongPtr(const VARIANT *v, long **ppLong);
HRESULT CVIFUNC CA_VariantGetIntPtr(const VARIANT *v, int **ppInt);
HRESULT CVIFUNC CA_VariantGetShortPtr(const VARIANT *v, short **ppShort);
HRESULT CVIFUNC CA_VariantGetBoolPtr(const VARIANT *v, VBOOL **ppBool);
HRESULT CVIFUNC CA_VariantGetFloatPtr(const VARIANT *v, float **ppFloat);
HRESULT CVIFUNC CA_VariantGetDoublePtr(const VARIANT *v, double **ppDbl);
HRESULT CVIFUNC CA_VariantGetSafeArrayPtr(const VARIANT *v, unsigned int cavt, LPSAFEARRAY **ppArray);
HRESULT CVIFUNC CA_VariantGetBSTRPtr(const VARIANT *v, BSTR **ppBstr);
HRESULT CVIFUNC CA_VariantGetCurrencyPtr(const VARIANT *v, CURRENCY **ppCy);
HRESULT CVIFUNC CA_VariantGetDatePtr(const VARIANT *v, DATE **ppDate);
HRESULT CVIFUNC CA_VariantGetDispatchPtr(const VARIANT *v, LPDISPATCH **ppDisp);
HRESULT CVIFUNC CA_VariantGetIUnknownPtr(const VARIANT *v, LPUNKNOWN **ppUnk);
HRESULT CVIFUNC CA_VariantGetErrorPtr(const VARIANT *v, SCODE **ppErr);
HRESULT CVIFUNC CA_VariantGetUCharPtr(const VARIANT *v, unsigned char **ppUChar);
HRESULT CVIFUNC CA_VariantGetVariantPtr(const VARIANT *v, VARIANT **pVar);
HRESULT CVIFUNC CA_VariantGetCharPtr(const VARIANT *v, char **ppChar);
HRESULT CVIFUNC CA_VariantGetUShortPtr(const VARIANT *v, unsigned short **ppUShort);
HRESULT CVIFUNC CA_VariantGetULongPtr(const VARIANT *v, unsigned long **ppULong);
HRESULT CVIFUNC CA_VariantGetUIntPtr(const VARIANT *v, unsigned int **ppUInt);



HRESULT CVIFUNC CA_VariantConvertToType(VARIANT *variant, unsigned int cavt, void *ptrToResult);
HRESULT CVIFUNC CA_VariantCopy(const VARIANT *source, VARIANT *destination);

HRESULT CVIFUNC CA_VariantClear(VARIANT *variant);




/* for C strings converted from BSTRs   */
/*  output char** function params,      */
/*  char* function return values        */ 
/*  CA_VariantGetCString                */
/*  CA_BSTRGetCString                   */
/*  CA_VariantConvertToType             */
/* for arrays and string elements of    */
/* arrays allocated with                */
/*  CA_SafeArrayTo2DArray               */
/*  CA_SafeArrayTo1DArray               */
void CVIFUNC    CA_FreeMemory(void *mem);
void *CVIFUNC   CA_AllocMemory(unsigned int numBytes);
void CVIFUNC    CA_FreeBSTR(BSTR bstr);


/* Array Conversion functions */
HRESULT CVIFUNC CA_Array1DToSafeArray(const void *array, unsigned int cavt, 
                                      unsigned int numElements, 
                                      LPSAFEARRAY *ppSafeArray);
HRESULT CVIFUNC CA_Array2DToSafeArray(const void *array, unsigned int cavt, 
                                      unsigned int dim1Size, unsigned int dim2Size,  
                                      LPSAFEARRAY *ppSafeArray);
HRESULT CVIFUNC CA_SafeArrayTo1DArray(LPSAFEARRAY *ppSafeArray, unsigned int cavt, 
                               void *array, unsigned int *dimSz);
HRESULT CVIFUNC CA_SafeArrayTo2DArray(LPSAFEARRAY *ppSafeArray, unsigned int cavt, void *array, 
                               unsigned int *dim1Sz, unsigned int *dim2Sz);
HRESULT CVIFUNC CA_SafeArrayTo1DArrayBuf(LPSAFEARRAY *ppSafeArray, unsigned int cavt, void *buf, 
                                  unsigned int bufSizeInBytes, unsigned int *numElements);
HRESULT CVIFUNC CA_SafeArrayTo2DArrayBuf(LPSAFEARRAY *ppSafeArray, unsigned int cavt, void *buf,
                                        unsigned int bufSizeInBytes, unsigned int *dim1NumElements,
                                        unsigned int *dim2NumElements);
HRESULT CVIFUNC CA_SafeArrayTo1DArrayEx(LPSAFEARRAY *ppSafeArray, unsigned int cavt, unsigned int options, 
                                        void *array, unsigned int *dimSz);
HRESULT CVIFUNC CA_SafeArrayTo2DArrayEx(LPSAFEARRAY *ppSafeArray, unsigned int cavt, unsigned int options, 
                                        void *array, unsigned int *dim1Sz, unsigned int *dim2Sz);
HRESULT CVIFUNC CA_SafeArrayTo1DArrayBufEx(LPSAFEARRAY *ppSafeArray, unsigned int cavt, unsigned int options, 
                                           void *buf, unsigned int bufSizeInBytes, unsigned int *numElements);
HRESULT CVIFUNC CA_SafeArrayTo2DArrayBufEx(LPSAFEARRAY *ppSafeArray, unsigned int cavt, unsigned int options, 
                                           void *buf, unsigned int bufSizeInBytes, unsigned int *dim1NumElements,
                                           unsigned int *dim2NumElements);
HRESULT CVIFUNC CA_SafeArrayGetNumDims(LPSAFEARRAY pSafeArray, unsigned int *numDimensions);

HRESULT CVIFUNC CA_SafeArrayGet1DSize(LPSAFEARRAY pSafeArray, unsigned int *numElements);
HRESULT CVIFUNC CA_SafeArrayGet2DSize(LPSAFEARRAY pSafeArray, unsigned int *dim1NumElements, 
                               unsigned int *dim2NumElements);
HRESULT CVIFUNC CA_SafeArrayDestroy(LPSAFEARRAY  pSafeArray);


/* Gets the underlying IDispatch Pointer */
HRESULT CVIFUNC CA_GetDispatchFromObjHandle(CAObjHandle objHandle, LPDISPATCH *pdispatchPtr);
HRESULT CVIFUNC CA_GetInterfaceFromObjHandle(CAObjHandle objHandle, const IID *pIID, int forceAddRef, 
                                             void *pinterfacePtr, int *didAddRef);


/* Error Display Helpers */
HRESULT CVIFUNC CA_DisplayErrorInfo(CAObjHandle objHandle, const char *title, 
                                    HRESULT status, const ERRORINFO *errorInfo);
void CVIFUNC    CA_GetAutomationErrorString(HRESULT error, char buf[], 
                                            unsigned int bufSize);
HRESULT CVIFUNC CA_FillErrorInfo(HRESULT status, ERRORINFO *errInfo, int *errInfoPresent);
HRESULT CVIFUNC CA_FillErrorInfoEx(CAObjHandle objHandle, const IID *pIID, HRESULT status, 
                                   ERRORINFO *errInfo, int *errInfoPresent);
                                            

HRESULT CVIFUNC CA_CStringToBSTR(const char *cString, BSTR *bstrPtr);
/* CA_BSTRGetCString does not free bstr */
HRESULT CVIFUNC CA_BSTRGetCString(BSTR bstr, char**stringPtr);
/* CA_BSTRGetCStringBuf does not free bstr */
HRESULT CVIFUNC CA_BSTRGetCStringBuf(BSTR bstr, char *stringBuf, unsigned long bufSize);
HRESULT CVIFUNC CA_BSTRGetCStringLen(BSTR bstr, unsigned long *len);


/* Using Locales */
HRESULT CVIFUNC CA_SetLocale(CAObjHandle objHandle, LCID localeId);
HRESULT CVIFUNC CA_GetLocale(CAObjHandle objHandle, LCID *localeId);

/* GIT */
HRESULT CVIFUNC CA_GetSupportForMultithreading(CAObjHandle objHandle, int *supportMultithreading);
HRESULT CVIFUNC CA_SetSupportForMultithreading(CAObjHandle objHandle, int supportMultithreading);

/* Creating ObjHandles */
HRESULT CVIFUNC CA_GetActiveObjectByClassId(const GUID *classId, const char *server, 
                                  CAObjHandle *objHandle);
HRESULT CVIFUNC CA_GetActiveObjectByClassIdEx(const GUID *classId, const char *server, const IID *iid,
                                              int supportMultithreading, LCID locale, int reserved, 
                                              CAObjHandle *objHandle);
HRESULT CVIFUNC CA_GetActiveObjectByProgId(const char *progId, const char *server, 
                                           CAObjHandle *objHandle);
HRESULT CVIFUNC CA_GetActiveObjectByProgIdEx(const char *progId, const char *server, const IID *iid,
                                             int supportMultithreading, LCID locale, int reserved, 
                                             CAObjHandle *objHandle);
HRESULT CVIFUNC CA_CreateObjectByClassId(const GUID *classId, const char *server, 
                                         CAObjHandle *objHandle);
HRESULT CVIFUNC CA_CreateObjectByClassIdEx(const GUID *classId, const char *server, const IID *iid,
                                           int supportMultithreading, LCID locale, int reserved, 
                                           CAObjHandle *objHandle);
HRESULT CVIFUNC CA_CreateObjectByProgId(const char *progId, const char *server, 
                                        CAObjHandle *objHandle);
HRESULT CVIFUNC CA_CreateObjectByProgIdEx(const char *progId, const char *server, const IID *iid,
                                          int supportMultithreading, LCID locale, int reserved, 
                                          CAObjHandle *objHandle);
HRESULT CVIFUNC CA_LoadObjectFromFile(const char *fileName, const char *server, 
                                      CAObjHandle *objHandle);
HRESULT CVIFUNC CA_LoadObjectFromFileEx(const char *fileName, const char *server, const IID *iid,
                                        int supportMultithreading, LCID locale, int reserved, 
                                        CAObjHandle *objHandle);
HRESULT CVIFUNC CA_LoadObjectFromFileByClassId(const char *fileName, const GUID *classId, 
                                               const char *server, CAObjHandle *objHandle);
HRESULT CVIFUNC CA_LoadObjectFromFileByClassIdEx(const char *fileName, const GUID *classId, const char *server, 
                                                 const IID *iid, int supportMultithreading, LCID locale, 
                                                 int reserved, CAObjHandle *objHandle);
HRESULT CVIFUNC CA_LoadObjectFromFileByProgId(const char *fileName, const char *progId, 
                                              const char *server, CAObjHandle *objHandle);
HRESULT CVIFUNC CA_LoadObjectFromFileByProgIdEx(const char *fileName, const char *progId, const char *server, 
                                                const IID *iid, int supportMultithreading, LCID locale, 
                                                int reserved, CAObjHandle *objHandle);
HRESULT CVIFUNC CA_CreateObjHandleFromIDispatch(LPDISPATCH input, int addRef, 
                                                CAObjHandle *objHandle);  
HRESULT CVIFUNC CA_CreateObjHandleFromInterface(void *input, const IID *iid, int supportMultithreading, 
                                                LCID locale, int reserved, int addRef, 
                                                CAObjHandle *objHandle);
HRESULT CVIFUNC CA_DuplicateObjHandle(CAObjHandle objHandle, const IID *pIID, CAObjHandle *newObjHandle);
HRESULT CVIFUNC CA_DiscardObjHandle(CAObjHandle objHandle);

void    CVIFUNC CA_FreeUnusedServers(void);

/* Calling Methods and Getting/Setting Properties */
HRESULT CVIFUNC_C CA_MethodInvoke(CAObjHandle objHandle, ERRORINFO *exceptInfo, 
                         int methodId, unsigned int returnType, void *retval, 
                         int paramCount, unsigned int paramTypes[], ... /* actual args */);
HRESULT CVIFUNC CA_MethodInvokeV(CAObjHandle objHandle, ERRORINFO *exceptInfo, 
                          int methodId, unsigned int returnType, void *retval, 
                          int paramCount, unsigned int paramTypes[], va_list args);
HRESULT CVIFUNC CA_PropertyGet(CAObjHandle objHandle, ERRORINFO *errInfo, int propId, 
                        unsigned int propType, void *value);
HRESULT CVIFUNC_C CA_PropertySet(CAObjHandle objHandle, ERRORINFO *errInfo, int propId, 
                        unsigned int propType, ...);
HRESULT CVIFUNC CA_PropertySetV(CAObjHandle objHandle, ERRORINFO *errInfo, int propId, 
                         unsigned int propType, va_list args);
HRESULT CVIFUNC_C CA_PropertySetByRef(CAObjHandle objHandle, ERRORINFO *errInfo, 
                             int propId, unsigned int propType, ...);
HRESULT CVIFUNC CA_PropertySetByRefV(CAObjHandle objHandle, ERRORINFO *errInfo, 
                              int propId, unsigned int propType, va_list args);
HRESULT CVIFUNC CA_InvokeHelperV(CAObjHandle objHandle, ERRORINFO *exceptInfo, 
                          int methodOrPropId, int mode /* put, get, putbyref, method */, 
                          unsigned int returnType, void *retval, 
                          int paramCount, unsigned int paramTypes[], va_list args);
HRESULT CVIFUNC_C CA_InvokeHelper(CAObjHandle objHandle, ERRORINFO *exceptInfo, 
                         int methodOrPropId, int mode /* put, get, putbyref, method */, 
                         unsigned int returnType, void *retval, 
                         int paramCount, unsigned int paramTypes[], ... /* actual args */);

HRESULT CVIFUNC_C CA_MethodInvokeEx(CAObjHandle objHandle, ERRORINFO *exceptInfo, const IID *pIID, 
                                    int methodId, unsigned int returnType, void *retval, 
                                    int paramCount, unsigned int paramTypes[], ... /* actual args */);
HRESULT CVIFUNC CA_MethodInvokeVEx(CAObjHandle objHandle, ERRORINFO *exceptInfo, const IID *pIID, 
                                   int methodId, unsigned int returnType, void *retval, 
                                   int paramCount, unsigned int paramTypes[], va_list args);
HRESULT CVIFUNC CA_PropertyGetEx(CAObjHandle objHandle, ERRORINFO *errInfo, const IID *pIID, 
                                 int propId, unsigned int propType, void *value);
HRESULT CVIFUNC_C CA_PropertySetEx(CAObjHandle objHandle, ERRORINFO *errInfo, const IID *pIID, 
                                   int propId, unsigned int propType, ...);
HRESULT CVIFUNC CA_PropertySetVEx(CAObjHandle objHandle, ERRORINFO *errInfo, const IID *pIID, 
                                  int propId, unsigned int propType, va_list args);
HRESULT CVIFUNC_C CA_PropertySetByRefEx(CAObjHandle objHandle, ERRORINFO *errInfo, const IID *pIID, 
                                        int propId, unsigned int propType, ...);
HRESULT CVIFUNC CA_PropertySetByRefVEx(CAObjHandle objHandle, ERRORINFO *errInfo, const IID *pIID, 
                                       int propId, unsigned int propType, va_list args);
HRESULT CVIFUNC CA_InvokeHelperVEx(CAObjHandle objHandle, ERRORINFO *exceptInfo, const IID *pIID, 
                                   int methodOrPropId, int mode /* put, get, putbyref, method */, 
                                   unsigned int returnType, void *retval, 
                                   int paramCount, unsigned int paramTypes[], va_list args);
HRESULT CVIFUNC_C CA_InvokeHelperEx(CAObjHandle objHandle, ERRORINFO *exceptInfo, const IID *pIID, 
                                    int methodOrPropId, int mode /* put, get, putbyref, method */, 
                                    unsigned int returnType, void *retval, 
                                    int paramCount, unsigned int paramTypes[], ... /* actual args */);

/* Configuration */
HRESULT CVIFUNC CA_InitActiveXThreadStyleForCurrentThread (void *reserved, unsigned int threadStyle);

/* Events. */
typedef struct {
    char *parameterName;            // The name of the parameter.
    VARTYPE vt;                     // The type of the parameter.
} CA_PARAMDATA;

typedef struct {
    char *methodName;               // The name of the method. 
    CA_PARAMDATA *parameterData;    // Pointer to an array of CA_PARAMDATA structures. 
    DISPID dispid;                  // The Member ID.
    unsigned int methodIndex;       // The index of the method in the EventVtbl (0-based).
    CALLCONV callingConvention;     // The calling convention.
    unsigned int numParams;         // The number of parameters in the method.
    unsigned short invokeKind;      // DISPATCH_METHOD, DISPATCH_PROPERTYGET, DISPATCH_PROPERTYPUT, or DISPATCH_PROPERTYPUTREF.
    VARTYPE vtReturn;               // The method's return type.
} CA_METHODDATA;

typedef struct {
    CA_METHODDATA *methodData;      // Pointer to an array of CA_METHODDATA structures.
    unsigned int numMethods;        // The number of methods in the interface.
} CA_INTERFACEDATA;

typedef struct {
    int structSize;                     // The sizeof() this structure.
    const IID *eventClassIID;           // IID for the event object interface.
    void *methodTable;                  // The EventVTable for this interface.
    CA_INTERFACEDATA *interfaceData;    // The interface data for this interface.
    LCID locale;                        // Locale for this interface.
} CAEventClassDefn;

HRESULT CVIFUNC CA_RegisterEventCallback (CAObjHandle serverObjHandle, const CAEventClassDefn *eventClassDefn,
                                          int methodTableIndex, void *callbackFunction, 
                                          void *callbackData, int enableCallbacks, int *callbackId);
HRESULT CVIFUNC CA_UnregisterEventCallback (int callbackId);
HRESULT CVIFUNC CA_UnregisterAllEventCallbacks (CAObjHandle serverObjHandle, const IID *eventClassIID);
HRESULT CVIFUNC CA_EnableEventsForServerObject (CAObjHandle serverObjHandle, const IID *eventClassIID);
HRESULT CVIFUNC CA_GetEventCallback (void *eventClassInterface, int methodTableIndex, void *callbackFunction, 
                                     void *callbackData, CAObjHandle *serverObjHandle);

/* Automation Server Support */ 

/* Events that trigger ActiveX Server Module and Object Callback functions */
#define CA_SERVER_EVENT_NONE                    0
#define CA_SERVER_EVENT_MODULE_EXIT             1
#define CA_SERVER_EVENT_OBJECT_CREATE           2
#define CA_SERVER_EVENT_OBJECT_DESTROY          3

/* Maximum number of connections supported by connection point objects */
/* used in ActiveX servers created with CVI */
#define CA_SERVER_MAX_EVENT_CONNECTIONS 64

/* Type for handle to user created ActiveX server objects */
typedef int CAServerObjHandle;

/* Type for callback functions for ActiveX server modules */
/* On CA_SERVER_EVENT_MODULE_EXIT event, should return 0 to shutdown user interface loop */
/* Internally QuitUserInterface(0) will be called and WM_QUIT posted to the current thread */
typedef int (CVIFUNC *CAServerModuleCallbackPtr) (int event);

/* Type for callback functions for ActiveX server objects */
/* On CA_SERVER_EVENT_OBJECT_CREATE event, callbackData will be the object's IUnknown* */
/* When creating the object, return S_OK to indicate success, and a failure code, */
/* like E_OUTOFMEMORY, or E_UNEXPECTED, etc to indicate the particular failure that */
/* occurred. When you return a failure code, the object will not be created. */
/* For CA_SERVER_EVENT_OBJECT_DESTROY event, callbackData will be NULL. */
typedef HRESULT (CVIFUNC *CAServerObjCallbackPtr) (CAServerObjHandle objHandle, const CLSID *clsid, 
                                                   int event, void *callbackData);

/*  Enum to indicate the type of interface */
typedef enum {
    IfaceType_kIUnknownDerived,     // Interface derived from IUnknown
    IfaceType_kIDispatchDerived,    // Interface derived from IDispatch
    IfaceType_kEvent = 0x100        // Outgoing event interface (always dispinterfaces)
} IfaceType;

/* Type for describing the type information for IDispatch based interfaces */
/* This is needed to implement the IDispatch methods */
typedef struct {
    LCID                    lcid;               // Locale Identifier
    const GUID              *tlbGUID;           // GUID of Type Library containing the type information
    unsigned short          tlbMajorVersion;    // Major Version of Type Library
    unsigned short          tlbMinorVersion;    // Minor Version of Type Library
    int                     isObjDefaultIface;  // Is this the object's default Dispatch interface
} CAServerDispatchDesc;

/* Type for virtual table of user functions in an interface */
typedef struct {
    int numFunctions;   // Number of functions in the vTable
    void **fnPtrArray;  // Pointer to array of functions, i.e. &fnPtrArray, when, void *fnPtrArray[] = { func1, ...};
} CAServerUserVtbl;

/* Type for describing an interface */
typedef struct {
    IfaceType               ifaceType;          // Type of interface: IUnknown or IDispatch derived
    CAServerUserVtbl        *vtblPtr;           // Pointer to the interface's virtual table
    const IID               *ifaceId;           // IID of the interface
    CAServerDispatchDesc    *specificIfaceDesc; // Additonal Information for Dispatch interfaces
} CAServerIfaceDesc;

/* Enum to indicate type of object */
typedef enum {
    ServerObjType_SingleDocApplication, // Single document object application; ignored for DLL servers
    ServerObjType_MultiDocApplication,  // Multi document object application; ignored for DLL servers
    ServerObjType_Document,             // Document object. Ignored for InProc server.
    ServerObjType_Custom                // Custom object
} ServerObjType;

/* Enum to indicate start-up options for EXE servers; ignored for DLL servers */
typedef enum {
    ServerStartupOptions_CreateObj                       = (1 << 0), // Create instance at server start-up
    ServerStartupOptions_PreventSubsequentObjCreation    = (1 << 1)  // Do not register class factory with COM
} ServerStartupOptions;

/* Enum to indicate other object characteristics */
typedef enum {
    ServerObjDescFlags_DoNotSupportAggregation  = (1 << 0), // Object cannot be aggregated
    ServerObjDescFlags_NonTopLevel              = (1 << 1), // Object is not a top-level object
                                                            // Application objects should be top-level objects
    ServerObjDescFlags_ServeSingleClient        = (1 << 2)  // Ignored for DLL servers
} ServerObjDescFlags;

/* Type for describing an ActiveX Object */
typedef struct {
    char                        *progID;                    // ProgID of the object
    unsigned int                objectVersion;              // Version of the object; for versioned ProgID
    const CLSID                 *clsid;                     // CLSID of the object
    char                        *description;               // Description of the object (for registry)
    ServerObjDescFlags          flags;                      // Object description flags
    ServerObjType               objType;                    // Type of object
    int                         registerAsActive;           // Register object in Running Object Table (ROT)
    ServerStartupOptions        defaultStartupOptions;      // If custom object and cmd line is empty
    ServerStartupOptions        automationStartupOptions;   // If custom object and /Automation is in cmd line
    CAServerObjCallbackPtr      objCallback;                // Callback function for the object
    int                         numIfaces;                  // Number of interfaces the object implements
    CAServerIfaceDesc           **ifaceList;                // Pointer to array of interface descriptions
} CAServerObjDesc;

/* Enum to indicate the server's threading model */
typedef enum {
    ThreadingModel_Single,
    ThreadingModel_Apartment,
    ThreadingModel_Multi
} ThreadingModel;

/* Enum to indicate a DLL server's multi-threading model */
typedef enum {
    DllMultiThreadingModel_None,
    DllMultiThreadingModel_SyncNeutral,
    DllMultiThreadingModel_AsyncNeutral,
    DllMultiThreadingModel_Free,
    DllMultiThreadingModel_Both,
    DllMultiThreadingModel_BothWithFreeThreadedMarshaler
} DllMultiThreadingModel;

/* Type to describe the type-library (if any) embedded in the server (for registration) */
typedef struct {
    const GUID              *tlbGUID;           // GUID of the embedded type library
    unsigned short          tlbMajorVersion;    // Major Version of type library
    unsigned short          tlbMinorVersion;    // Minor Version of type library
    LCID                    lcid;               // Locale Identifier
    SYSKIND                 syskind;            // Target Operating System (only SYS_WIN32 for now)
} CAServerTypeLibDesc;

/* Type for describing the ActiveX Server Module */
typedef struct {
    ThreadingModel              threadingModel;                 // Threading Model of server
    DllMultiThreadingModel      dllMultiThreadingModel;         // Multi-threading model of DLL server 
    int                         requireAutomationCmdLineSwitch; // if true, EXE won't run without /Automation
    CAServerModuleCallbackPtr   moduleCallback;                 // Callback function for the server
    int                         numObjects;                     // number of objects in the server
    CAServerObjDesc             **objList;                      // Pointer to array of object descriptions
    CAServerTypeLibDesc         *embeddedTlb;                   // Description of type-lib embedded in server
} CAServerModuleDesc;

/* Enum to indicate type of server to Initialization function */
typedef enum {
    kServerModuleType_Dll,      // DLL or in process server
    kServerModuleType_Exe       // EXE or out of process server
} ServerModuleType;

/* Server library functions */
/* Initialization, close, and global data functions */
HRESULT CVIFUNC CA_InitActiveXServer (HINSTANCE serverHInstance, ServerModuleType moduleType, 
                                      const char *cmdLine, CAServerModuleDesc *serverModuleDesc, 
                                      int *runServer, char *errStrBuf, int bufSize);
HRESULT CVIFUNC CA_CloseActiveXServer (HINSTANCE serverHInstance);
HRESULT CVIFUNC CA_ServerSetGlobalData (HINSTANCE serverHInstance, void *moduleData);
HRESULT CVIFUNC CA_ServerGetGlobalData (HINSTANCE serverHInstance, void *moduleData);
HRESULT CVIFUNC CA_ServerReleaseGlobalData (HINSTANCE serverHInstance);

/* Object functions */
HRESULT CVIFUNC CA_ServerCreateObject (const CLSID *clsid, IUnknown *outerIUnknown, const IID *iid,
                                       void *ifacePtr);
HRESULT CVIFUNC CA_ServerAggregateObject (CAServerObjHandle objHandle, const IID *iidToAggregate, 
                                          IUnknown *innerObjUnknown);
HRESULT CVIFUNC CA_ServerSetObjData (CAServerObjHandle objHandle, void *objData);
HRESULT CVIFUNC CA_ServerGetObjData (CAServerObjHandle objHandle, void *objData);
HRESULT CVIFUNC CA_ServerReleaseObjData (CAServerObjHandle objHandle);
HRESULT CVIFUNC CA_ServerGetObjHandleFromIface (void *thisIfacePtr, CAServerObjHandle *objHandle);
HRESULT CVIFUNC CA_ServerGetIfaceFromObjHandle (CAServerObjHandle objHandle,  const IID *iid, 
                                                void *thisIfacePtr);

/* Object helper functions */
/* Locks an object, and thus prevents it from being destroyed by COM. */
/* Should be called when a UI object that is active is made visible. */
/*(e.g. Excel.Application.Visible = true) */
HRESULT CVIFUNC CA_ServerLockActiveObject (CAServerObjHandle objHandle);
/* Unlocks an object, and allows the object to be destroyed by COM when appropriate. */
/* Should be called to unlock a UI object that has been made visible and locked. */
HRESULT CVIFUNC CA_ServerUnlockActiveObject (CAServerObjHandle objHandle);
/* Should only be called when a UI object that is visible (and so is locked) */
/* is shut down from the user interface (i.e., not through ActiveX). */
HRESULT CVIFUNC CA_ServerDestroyActiveObject (CAServerObjHandle objHandle);
/* Sets an ErrorInfo object with the input error information (and the Server's progID). */
/* The helpFile input must be the file name (not path), and the file should be located */
/* in the same folder as the server module (DLL/EXE). */
HRESULT CVIFUNC CA_ServerSetErrorInfo (CAServerObjHandle objHandle, const IID *iid, 
                                       const char *desc, const char *helpFile, int helpContext);
/* Gets all the client object handles registered to receive events on the specified */
/* server object through the specified event interface. On success, you must call the */
/* CA_ServerReleaseEventObjHandles from the same thread that called this function. */
HRESULT CVIFUNC CA_ServerGetEventObjHandles (CAServerObjHandle objHandle, 
                                             const IID *eventInterfaceId, 
                                             int supportMultithreading, LCID locale, 
                                             CAObjHandle eventObjHandles[],
                                             unsigned int *numEventObjHandles);
/* Releases client sink objects that user obtained from CA_ServerGetEventObjHandles */
HRESULT CVIFUNC CA_ServerReleaseEventObjHandles (CAServerObjHandle objHandle, 
                                                 const IID *eventInterfaceId, 
                                                 CAObjHandle eventObjHandles[]);

/* Dll server entry points */
HRESULT CVIFUNC CA_ServerDllGetClassObject (HINSTANCE serverHInstance, const CLSID *clsid, 
                                            const IID *iid, void *ifacePtr);
HRESULT CVIFUNC CA_ServerDllCanUnloadNow (HINSTANCE serverHInstance);
HRESULT CVIFUNC CA_ServerDllRegisterServer (HINSTANCE serverHInstance);
HRESULT CVIFUNC CA_ServerDllUnregisterServer (HINSTANCE serverHInstance);

/* IUnknown implementation */
HRESULT CVIFUNC CA_ServerQueryInterface (IUnknown *thisIfacePtr, const IID *iid, void *newIfacePtr);
unsigned long CVIFUNC CA_ServerAddRef (IUnknown *thisIfacePtr);
unsigned long CVIFUNC CA_ServerRelease (IUnknown *thisIfacePtr);

/* IDispatch implementation */
HRESULT CVIFUNC CA_ServerGetTypeInfoCount (IDispatch *thisIfacePtr, unsigned long *countPtr);
HRESULT CVIFUNC CA_ServerGetTypeInfo (IDispatch *thisIfacePtr, unsigned long typeInfoIndex, 
                                      LCID lcid, ITypeInfo **typeInfoPtr);
HRESULT CVIFUNC CA_ServerGetIDsOfNames (IDispatch *thisIfacePtr, const IID *iid, 
                                        LPOLESTR *namesArray, unsigned int numNames, LCID lcid, 
                                        long *idsArray);
HRESULT CVIFUNC CA_ServerInvoke (IDispatch *thisIfacePtr, int memberId, const IID *iid, LCID lcid, 
                                 unsigned short flags, DISPPARAMS *dispParams, VARIANT *vtResult, 
                                 EXCEPINFO *exceptionInfo, unsigned int *argErrorIndex);

/* Obsolete Names */
#define CA_GetActiveObjectByDispId      CA_GetActiveObjectByClassId
#define CA_CreateObjectByDispId         CA_CreateObjectByClassId
#define CA_LoadObjectFromFileByDispId   CA_LoadObjectFromFileByClassId

#if defined(_CVI_) && !defined(__TPC__)
#pragma PassOrigPtrInfoForVoidPtrParam  CA_VariantGet1DArray
#pragma PassOrigPtrInfoForVoidPtrParam  CA_VariantGet2DArray
#pragma PassOrigPtrInfoForVoidPtrParam  CA_SafeArrayTo1DArray
#pragma PassOrigPtrInfoForVoidPtrParam  CA_SafeArrayTo2DArray
#pragma PassOrigPtrInfoForVoidPtrParam  CA_SafeArrayTo1DArrayEx
#pragma PassOrigPtrInfoForVoidPtrParam  CA_SafeArrayTo2DArrayEx
#endif

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* AUTOSUPP_H */
