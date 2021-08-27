//==============================================================================
//
//
//                  NI-IMAQ for IEEE 1394 Cameras Header File
//
//          Copyright (C) 2004, National Instruments Corporation
//                          http://www.ni.com/
//
//
//
//==============================================================================


#ifdef _MSC_VER
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#endif


#ifndef __NIIMAQ_1394_HEADER__
#define __NIIMAQ_1394_HEADER__


#if (defined(__cplusplus) || defined(__cplusplus__))
extern "C" {
#endif


#ifdef _CVI_
#pragma  EnableLibraryRuntimeChecking
#endif


//==============================================================================
//  Typedefs
//==============================================================================
#if !defined(niimaq1394_types)
    #define niimaq1394_types

    #ifndef _NI_uInt8_DEFINED_
        #define _NI_uInt8_DEFINED_
        typedef unsigned char       uInt8;
    #endif

    #ifndef _NI_uInt16_DEFINED_
        #define _NI_uInt16_DEFINED_
        typedef unsigned short int  uInt16;
    #endif

    #ifndef _NI_uInt32_DEFINED_
        #define _NI_uInt32_DEFINED_
        typedef unsigned long       uInt32;
    #endif

    #ifndef _NI_Int8_DEFINED_
        #define _NI_Int8_DEFINED_
        typedef char                Int8;
    #endif

    #ifndef _NI_Int16_DEFINED_
        #define _NI_Int16_DEFINED_
        typedef short int           Int16;
    #endif

    #ifndef _NI_Int32_DEFINED_
        #define _NI_Int32_DEFINED_
        typedef long                Int32;
    #endif

    #ifndef TRUE
        #define TRUE                1
    #endif

    #ifndef FALSE
        #define FALSE               0
    #endif
    
    #ifndef _NI_GUIDHNDL_DEFINED
        typedef uInt32              GUIHNDL;
    #endif

    #ifndef _NI_FUNC_DEFINED
        #define NI_FUNC             __stdcall
    #endif

    #ifndef _NI_FUNCC_DEFINED
        #define NI_FUNCC            __cdecl
    #endif

#endif


typedef     uInt32  SESSION_ID;
typedef     Int32   IMG_ERR;
typedef     IMG_ERR (NI_FUNCC *IMAQ1394_CALL_BACK_PTR)(SESSION_ID sessionId, uInt32 event, IMG_ERR error, uInt32 cumulBufferIndex, void* userdata);
    

//==============================================================================
//  Defines
//==============================================================================
#if defined(USER_MODE_BUILD)
    #define IMAQ1394_CVI_API extern "C" __declspec(dllexport) IMG_ERR NI_FUNC
#else
    #define IMAQ1394_CVI_API IMG_ERR NI_FUNC
#endif


//==============================================================================
//  Attribute keys
//==============================================================================
#define     IMG1394_ATTR_BRIGHTNESS                     0x0001
#define     IMG1394_ATTR_AUTO_EXPOSURE                  0x0002
#define     IMG1394_ATTR_SHARPNESS                      0x0003
#define     IMG1394_ATTR_WHITE_BALANCE_U_B              0x0004
#define     IMG1394_ATTR_WHITE_BALANCE_V_R              0x0005
#define     IMG1394_ATTR_HUE                            0x0006
#define     IMG1394_ATTR_SATURATION                     0x0007
#define     IMG1394_ATTR_GAMMA                          0x0008
#define     IMG1394_ATTR_SHUTTER                        0x0009
#define     IMG1394_ATTR_GAIN                           0x000A
#define     IMG1394_ATTR_IRIS                           0x000B
#define     IMG1394_ATTR_FOCUS                          0x000C
#define     IMG1394_ATTR_TEMPERATURE                    0x000D
#define     IMG1394_ATTR_TRIGGER                        0x000E
#define     IMG1394_ATTR_ZOOM                           0x000F
#define     IMG1394_ATTR_PAN                            0x0010
#define     IMG1394_ATTR_TILT                           0x0011
#define     IMG1394_ATTR_OPTICAL_FILTER                 0x0012
#define     IMG1394_ATTR_VENDOR_NAME                    0x0013
#define     IMG1394_ATTR_MODEL_NAME                     0x0014
#define     IMG1394_ATTR_SERIAL_NO                      0x0015
#define     IMG1394_ATTR_MODEL_ID                       0x0016
#define     IMG1394_ATTR_VIDEO_FORMAT                   0x0017
#define     IMG1394_ATTR_VIDEO_MODE                     0x0018
#define     IMG1394_ATTR_VIDEO_FRAME_RATE               0x0019
#define     IMG1394_ATTR_IMAGE_REP                      0x001A
#define     IMG1394_ATTR_TIMEOUT                        0x001B
#define     IMG1394_ATTR_FORMAT7_UNIT_WIDTH             0x001C
#define     IMG1394_ATTR_FORMAT7_UNIT_HEIGHT            0x001D
#define     IMG1394_ATTR_UNIQUE_ID_LOW                  0x001E
#define     IMG1394_ATTR_UNIQUE_ID_HIGH                 0x001F
#define     IMG1394_ATTR_LOST_BUFFER_NB                 0x0020
#define     IMG1394_ATTR_FORMAT7_LEFT                   0x0021
#define     IMG1394_ATTR_FORMAT7_TOP                    0x0022
#define     IMG1394_ATTR_FORMAT7_WIDTH                  0x0023
#define     IMG1394_ATTR_FORMAT7_HEIGHT                 0x0024
#define     IMG1394_ATTR_FORMAT7_COLORCODING            0x0025
#define     IMG1394_ATTR_IMAGE_WIDTH                    0x0026
#define     IMG1394_ATTR_IMAGE_HEIGHT                   0x0027
#define     IMG1394_ATTR_BYTES_PER_PIXEL                0x0028
#define     IMG1394_ATTR_FRAME_INTERVAL                 0x0029
#define     IMG1394_ATTR_FORMAT7_BYTES_PER_PACKET       0x002A
#define     IMG1394_ATTR_ABSOLUTE_BRIGHTNESS            0x0101
#define     IMG1394_ATTR_ABSOLUTE_AUTO_EXPOSURE         0x0102
#define     IMG1394_ATTR_ABSOLUTE_SHARPNESS             0x0103
#define     IMG1394_ATTR_ABSOLUTE_WHITE_BALANCE         0x0104
#define     IMG1394_ATTR_ABSOLUTE_HUE                   0x0106
#define     IMG1394_ATTR_ABSOLUTE_SATURATION            0x0107
#define     IMG1394_ATTR_ABSOLUTE_GAMMA                 0x0108
#define     IMG1394_ATTR_ABSOLUTE_SHUTTER               0x0109
#define     IMG1394_ATTR_ABSOLUTE_GAIN                  0x010A
#define     IMG1394_ATTR_ABSOLUTE_IRIS                  0x010B
#define     IMG1394_ATTR_ABSOLUTE_FOCUS                 0x010C
#define     IMG1394_ATTR_ABSOLUTE_TEMPERATURE           0x010D
#define     IMG1394_ATTR_ABSOLUTE_TRIGGER               0x010E
#define     IMG1394_ATTR_ABSOLUTE_ZOOM                  0x010F
#define     IMG1394_ATTR_ABSOLUTE_PAN                   0x0110
#define     IMG1394_ATTR_ABSOLUTE_TILT                  0x0111
#define     IMG1394_ATTR_ABSOLUTE_OPTICAL_FILTER        0x0112
#define     IMG1394_ATTR_ROI_LEFT                       0x011B
#define     IMG1394_ATTR_ROI_TOP                        0x011C
#define     IMG1394_ATTR_ROI_WIDTH                      0x011D
#define     IMG1394_ATTR_ROI_HEIGHT                     0x011E
#define     IMG1394_ATTR_FORMAT7_SPEED                  0x0121
#define     IMG1394_ATTR_LAST_TRANSFERRED_BUFFER_NUM    0x0123
#define     IMG1394_ATTR_FRAME_COUNT                    0x0124
#define     IMG1394_ATTR_ACQ_IN_PROGRESS                0x0125
#define     IMG1394_ATTR_IGNORE_FIRST_FRAME             0x0126
#define     IMG1394_ATTR_SHIFT_PIXEL_BITS               0x0127
#define     IMG1394_ATTR_SWAP_PIXEL_BYTES               0x0128
#define     IMG1394_ATTR_FORMAT7_UNIT_BYTES_PER_PACKET  0x0129
#define     IMG1394_ATTR_FORMAT7_MAX_BYTES_PER_PACKET   0x012A
#define     IMG1394_ATTR_BITS_PER_PIXEL                 0x012C
#define     IMG1394_ATTR_TRIGGER_DELAY                  0x0202
#define     IMG1394_ATTR_WHITE_SHADING_R                0x0203
#define     IMG1394_ATTR_WHITE_SHADING_G                0x0204
#define     IMG1394_ATTR_WHITE_SHADING_B                0x0205
#define     IMG1394_ATTR_FRAME_RATE                     0x0206
#define     IMG1394_ATTR_ABSOLUTE_TRIGGER_DELAY         0x0207
#define     IMG1394_ATTR_ABSOLUTE_FRAME_RATE            0x0208
#define     IMG1394_ATTR_COLOR_FILTER_INQ               0x0209
#define     IMG1394_ATTR_COLOR_FILTER                   0x020A
#define     IMG1394_ATTR_COLOR_FILTER_GAIN_R            0x020B
#define     IMG1394_ATTR_COLOR_FILTER_GAIN_G            0x020C
#define     IMG1394_ATTR_COLOR_FILTER_GAIN_B            0x020D
#define     IMG1394_ATTR_FORMAT7_MAX_SPEED              0x020E
#define     IMG1394_ATTR_FORMAT7_PACKETS_PER_IMAGE      0x021B
#define     IMG1394_ATTR_BASE_ADDRESS                   0x021D


//==============================================================================
//  Attribute limits
//==============================================================================
#define     IMG1394_VIDEO_FORMAT_MIN                    0
#define     IMG1394_VIDEO_FORMAT_MAX                    7
#define     IMG1394_VIDEO_MODE_MIN                      0
#define     IMG1394_VIDEO_MODE_MAX                      7
#define     IMG1394_VIDEO_FRAME_RATE_MIN                0
#define     IMG1394_VIDEO_FRAME_RATE_MAX                7
#define     IMG1394_DWORD_MIN                           0
#define     IMG1394_DWORD_MAX                           0xFFFFFFFF
#define     IMG1394_BITS_PER_PIXEL_MIN                  10
#define     IMG1394_BITS_PER_PIXEL_MAX                  16


//==============================================================================
//  Special keys used for attributes
//==============================================================================
#define     IMG1394_LASTBUFFER                          IMG1394_DWORD_MAX
#define     IMG1394_IMMEDIATEBUFFER                     IMG1394_DWORD_MAX - 1
#define     IMG1394_AUTOMODE                            IMG1394_DWORD_MAX
#define     IMG1394_ONEPUSHMODE                         IMG1394_DWORD_MAX - 1
#define     IMG1394_OFFMODE                             IMG1394_DWORD_MAX - 2
#define     IMG1394_ABSOLUTEMODE                        IMG1394_DWORD_MAX - 3
#define     IMG1394_RELATIVEMODE                        IMG1394_DWORD_MAX - 4
#define     IMG1394_IGNOREMODE                          IMG1394_DWORD_MAX - 5


//==============================================================================
//  Plot flags
//==============================================================================
#define     IMG1394_PLOT_MONO_8                         0x00000000
#define     IMG1394_PLOT_INVERT                         0x00000001
#define     IMG1394_PLOT_COLOR_RGB24                    0x00000002
#define     IMG1394_PLOT_COLOR_RGB32                    0x00000004
#define     IMG1394_PLOT_MONO_10                        0x00000008
#define     IMG1394_PLOT_MONO_12                        0x00000010
#define     IMG1394_PLOT_MONO_14                        0x00000020
#define     IMG1394_PLOT_MONO_16                        0x00000040
#define     IMG1394_PLOT_MONO_32                        0x00000080
#define     IMG1394_PLOT_AUTO                           0x00000100


//============================================================================
//  Error Codes Enumeration
//============================================================================
#define     IMG1394_ERR_GOOD                            0 // success
#define     IMG1394_ERR_EMEM                            0xBFF68000 // Not enough memory
#define     IMG1394_ERR_EDRV                            0xBFF68001 // Cannot load the driver
#define     IMG1394_ERR_TIMO                            0xBFF68002 // Time out
#define     IMG1394_ERR_NIMP                            0xBFF68003 // Function not implemented yet
#define     IMG1394_ERR_INTL                            0xBFF68004 // Internal error
#define     IMG1394_ERR_BMOD                            0xBFF68005 // Invalid combination of format, video mode, and frame rate for this camera
#define     IMG1394_ERR_INIT                            0xBFF68006 // Session not initialized
#define     IMG1394_ERR_BATT                            0xBFF68007 // Bad attribute
#define     IMG1394_ERR_FTNP                            0xBFF68008 // Feature not present in the camera
#define     IMG1394_ERR_ESYS                            0xBFF68009 // System error
#define     IMG1394_ERR_HEAP                            0xBFF6800A // Allocation error
#define     IMG1394_ERR_UNINITIALIZED                   0xBFF6800B // Allocator is not initialized
#define     IMG1394_ERR_ORNG                            0xBFF6800C // Value is out of range
#define     IMG1394_ERR_BCAM                            0xBFF6800D // Bad camera file
#define     IMG1394_ERR_BSID                            0xBFF6800E // Invalid Session ID
#define     IMG1394_ERR_NSUP                            0xBFF6800F // Attribute not supported by the camera
#define     IMG1394_ERR_INVF                            0xBFF68010 // Format is invalid
#define     IMG1394_ERR_INVM                            0xBFF68011 // Video mode is invalid
#define     IMG1394_ERR_INVR                            0xBFF68012 // Frame rate is invalid
#define     IMG1394_ERR_INVC                            0xBFF68013 // Color ID is invalid
#define     IMG1394_ERR_NOAP                            0xBFF68014 // No acquisition in progress
#define     IMG1394_ERR_AOIP                            0xBFF68015 // Acquisition is already in progress
#define     IMG1394_ERR_IRES                            0xBFF68016 // Insufficient resources available for the required video mode
#define     IMG1394_ERR_TBUF                            0xBFF68017 // Too many buffers used
#define     IMG1394_ERR_INVP                            0xBFF68018 // Invalid parameter
#define     IMG1394_ERR_NSAT                            0xBFF68019 // Non-writable attribute
#define     IMG1394_ERR_NGAT                            0xBFF6801A // Non-readable attribute
#define     IMG1394_ERR_CMNF                            0xBFF6801B // Camera not found
#define     IMG1394_ERR_CRMV                            0xBFF6801C // Camera has been removed
#define     IMG1394_ERR_BNRD                            0xBFF6801D // Buffer not ready
#define     IMG1394_ERR_BRST                            0xBFF6801E // A bus reset occured in the middle of a transaction
#define     IMG1394_ERR_NLIC                            0xBFF6801F // Unlicensed copy of NI-IMAQ for IEEE 1394
#define     IMG1394_ERR_NDLL                            0xBFF68020 // CVI only error. DLL could not be found      
#define     IMG1394_ERR_NFNC                            0xBFF68021 // CVI only error. Function not found in DLL
#define     IMG1394_ERR_NOSR                            0xBFF68022 // CVI only error. No resource available
#define     IMG1394_ERR_NCFG                            0xBFF68023 // Session not configured
#define     IMG1394_ERR_IOER                            0xBFF68024 // I/O error
#define     IMG1394_ERR_CAIU                            0xBFF68025 // Camera is already in use
#define     IMG1394_ERR_BAD_POINTER                     0xBFF68026 // Bad pointer. The pointer may be NULL when it should be non-NULL, or it may be non-NULL when it should be NULL.
#define     IMG1394_ERR_EXCEPTION                       0xBFF68027 // An exception has occured. Check the NI-PAL debug log for more information.
#define     IMG1394_ERR_BAD_DEVICE_TYPE                 0xBFF68028 // The type of device is invalid. Unable to create an instance.
#define     IMG1394_ERR_ASYNC_READ                      0xBFF68029 // Unable to perform asychronous register read. The device may be busy or broken.
#define     IMG1394_ERR_ASYNC_WRITE                     0xBFF6802A // Unable to perform asychronous register write. The device may be busy or broken.
#define     IMG1394_ERR_VIDEO_NOT_SUPPORTED             0xBFF6802B // The combination of video format, mode, and rate is not supported for this camera. Please consult your camera documentation.
#define     IMG1394_ERR_BUFFER_INDEX                    0xBFF6802C // The index into the buffer list is incorrect. Reconfigure and try again.
#define     IMG1394_ERR_BAD_USER_ROI                    0xBFF6802D // The camera cannot acquire the user ROI. Resize and try again.
#define     IMG1394_ERR_BUFFER_LIST_ALREADY_LOCKED      0xBFF6802E // The buffer list is already locked. Reconfigure the acquisition and try again.
#define     IMG1394_ERR_BUFFER_LIST_NOT_LOCKED          0xBFF6802F // There is no buffer list. Reconfigure the acquisition and try again.
#define     IMG1394_ERR_RESOURCES_ALREADY_ALLOCATED     0xBFF68030 // The isochronous resources have already been allocated. Reconfigure the acquisition and try again.
#define     IMG1394_ERR_BUFFER_LIST_EMPTY               0xBFF68031 // The buffer list is empty. Add at least one buffer.
#define     IMG1394_ERR_FLAG_1                          0xBFF68032 // For format 7: The combination of speed, image position, image size, and color coding is incorrect.
#define     IMG1394_ERR_BUFFER_NOT_AVAILABLE            0xBFF68033 // The requested buffer is unavailable. The contents of the current buffer has been overwritten by the acquistion.
#define     IMG1394_ERR_IMAGE_REP_NOT_SUPPORTED         0xBFF68034 // The requested image representation is not supported for the current color coding.
#define     IMG1394_ERR_BAD_OCCURRENCE                  0xBFF68035 // The given occurrence is not valid. Unable to complete image acquistion.


//==============================================================================
//  Attribute ranges
//==============================================================================
enum TriggerPolarity {
    IMG1394_TRIG_POLAR_ACTIVEL,
    IMG1394_TRIG_POLAR_ACTIVEH,
    IMG1394_TRIG_POLAR_DEFAULT,
};


enum TriggerMode {
    IMG1394_TRIG_DISABLE,
    IMG1394_TRIG_MODE0,
    IMG1394_TRIG_MODE1,
    IMG1394_TRIG_MODE2,
    IMG1394_TRIG_MODE3,
    IMG1394_TRIG_MODE4,
    IMG1394_TRIG_MODE5,
};


enum ColorCoding {
    IMG1394_COLORID_DEFAULT,
    IMG1394_COLORID_MONO8,
    IMG1394_COLORID_YUV411,
    IMG1394_COLORID_YUV422,
    IMG1394_COLORID_YUV444,
    IMG1394_COLORID_RGB8,
    IMG1394_COLORID_MONO16,
    IMG1394_COLORID_RGB16,
    IMG1394_COLORID_SIGNED_MONO16,
    IMG1394_COLORID_SIGNED_RGB16,
    IMG1394_COLORID_RAW8,
    IMG1394_COLORID_RAW16,
};


enum ColorFilter {
    IMG1394_COLOR_FILTER_NONE,
    IMG1394_COLOR_FILTER_GBGB_RGRG,
    IMG1394_COLOR_FILTER_GRGR_BGBG,
    IMG1394_COLOR_FILTER_BGBG_GRGR,
    IMG1394_COLOR_FILTER_RGRG_GBGB,
};


enum Event {
    IMG1394_EVENT_FRAME_DONE,
    IMG1394_EVENT_CAMERA_ATTACHED,
    IMG1394_EVENT_CAMERA_DETACHED,
    IMG1394_EVENT_ALL
};


enum Speed {
    IMG1394_SPEED_DEFAULT,
    IMG1394_SPEED_100,
    IMG1394_SPEED_200,
    IMG1394_SPEED_400,
    IMG1394_SPEED_800,
    IMG1394_SPEED_1600,
    IMG1394_SPEED_3200,
};


enum ImageRepresentation {
    IMG1394_IMAGEREP_DEFAULT,
    IMG1394_IMAGEREP_RAW,
    IMG1394_IMAGEREP_MONO8,
    IMG1394_IMAGEREP_MONO16,
    IMG1394_IMAGEREP_RGB32,
    IMG1394_IMAGEREP_RGB64,
};


enum OnOverwrite {
    IMG1394_ONOVERWRITE_GET_OLDEST,
    IMG1394_ONOVERWRITE_GET_NEXT_ITERATION,
    IMG1394_ONOVERWRITE_FAIL,
    IMG1394_ONOVERWRITE_GET_NEWEST,
};


enum CameraMode {
    IMG1394_CAMERA_MODE_CONTROLLER,
    IMG1394_CAMERA_MODE_LISTENER,
};


enum InterfacFileFlags {
    IMG1394_INTERFACE_CONNECTED = 0x1,
    IMG1394_INTERFACE_DIRTY = 0x2,
};


//==============================================================================
//  CVI Structures
//==============================================================================
#pragma pack(push, 1)

typedef struct VideoMode_struct {
    uInt32 Format;
    uInt32 Mode;
    uInt32 FrameRate;
    char VideoModeName[64];  // Length fixed to 64
} VideoMode;


typedef struct Feature_struct {
    uInt32 Min;
    uInt32 Max;
    uInt32 AutoMode;
    uInt32 OnePush;
    uInt32 Enable;
    uInt32 Attribute;
    uInt32 Current_Value;
    uInt32 Default_Value;
    uInt32 Readable;
    uInt32 OnOff;
    char FeatureName[64];    // Length fixed to 64
} Feature;


typedef struct Feature2_struct {
    uInt32 Enable;
    uInt32 Absolute;
    uInt32 OnePush;
    uInt32 Readable;
    uInt32 OnOff;
    uInt32 Auto;
    uInt32 Manual;
    uInt32 Relative_Min_Value;
    uInt32 Relative_Max_Value;
    uInt32 Relative_Current_Value;
    uInt32 Relative_Default_Value;
    uInt32 Relative_Attribute;
    double Absolute_Min_Value;
    double Absolute_Max_Value;
    double Absolute_Current_Value;
    double Absolute_Default_Value;
    uInt32 Absolute_Attribute;
    char FeatureName[64];    // Length fixed to 64
} Feature2;


#if !defined(USERINT_HEADER) && !defined(_CVI_RECT_DEFINED)
#define _CVI_RECT_DEFINED
typedef struct Rect_struct {
    int top;   
    int left;  
    int height;
    int width; 
} Rect;
#endif


typedef struct IntefaceFile_struct {
    uInt32 Type;
    uInt32 Version;
    uInt32 Flags;
    uInt32 SerialNumberHi;
    uInt32 SerialNumberLo;
    char InterfaceName[64];
    char VendorName[64];
    char ModelName[64];
    char CameraFileName[64];
} InterfaceFile;


typedef struct CameraFile_struct {
    uInt32 Type;
    uInt32 Version;
    char FileName[64];
} CameraFile;

#pragma pack(pop)


//==============================================================================
//  CVI Basic functions
//==============================================================================
IMAQ1394_CVI_API imaq1394GetInterfaceFiles(InterfaceFile* array, uInt32* count);
IMAQ1394_CVI_API imaq1394CameraOpen2(char* name, uInt32 mode, SESSION_ID* sessionId);
IMAQ1394_CVI_API imaq1394Close(SESSION_ID sessionId);
IMAQ1394_CVI_API imaq1394SetAttribute(SESSION_ID sessionId, uInt32 attribute, uInt32 value);
IMAQ1394_CVI_API imaq1394GetAttribute(SESSION_ID sessionId, uInt32 attribute, void* value);
IMAQ1394_CVI_API imaq1394AttributeInquiry2(SESSION_ID sessionId, uInt32 attribute, Feature2* cameraFeature);
IMAQ1394_CVI_API imaq1394TriggerConfigure(SESSION_ID sessionId, uInt32 polarity, uInt32 timeout, uInt32 mode, uInt32 parameter);
IMAQ1394_CVI_API imaq1394GetFeatures2(SESSION_ID sessionId, Feature2* featureArray, uInt32* featureArraySize);
IMAQ1394_CVI_API imaq1394GetVideoModes(SESSION_ID sessionId, VideoMode* videoModeArray, uInt32* videoModeArraySize, uInt32* currentMode);
IMAQ1394_CVI_API imaq1394ConfigEventMessage(SESSION_ID sessionId, uInt32 event, GUIHNDL hwnd, uInt32 message, void* param);
IMAQ1394_CVI_API imaq1394InstallCallback(SESSION_ID sessionId, uInt32 event, IMAQ1394_CALL_BACK_PTR callbackFnPtr, void* userData);
IMAQ1394_CVI_API imaq1394ShowError(IMG_ERR error, char* errorText, Int32 errorMaxLen);
IMAQ1394_CVI_API imaq1394Plot(GUIHNDL hwnd, void* buffer, uInt32 srcX, uInt32 srcY, uInt32 cols, uInt32 rows, uInt32 dstX, uInt32 dstY, uInt32 flags);
IMAQ1394_CVI_API imaq1394PlotDC(GUIHNDL hdc, void* buffer, uInt32 srcX, uInt32 srcY, uInt32 cols, uInt32 rows, uInt32 dstX, uInt32 dstY, uInt32 flags);
IMAQ1394_CVI_API imaq1394SaveBuffer(SESSION_ID sessionId, void* buffer, char* filename);
IMAQ1394_CVI_API imaq1394ReadQuadlet(SESSION_ID sessionId, uInt32 offset, uInt32* value);
IMAQ1394_CVI_API imaq1394ReadBlock(SESSION_ID sessionId, uInt32 offset, uInt32 count, uInt32* array);
IMAQ1394_CVI_API imaq1394WriteQuadlet(SESSION_ID sessionId, uInt32 offset, uInt32 value);
IMAQ1394_CVI_API imaq1394WriteBlock(SESSION_ID sessionId, uInt32 offset, uInt32 count, uInt32* array);


//==============================================================================
//  CVI Acquisition functions
//==============================================================================
IMAQ1394_CVI_API imaq1394SetupGrab(SESSION_ID sessionId, Rect rectangle);
IMAQ1394_CVI_API imaq1394ConfigureAcquisition(SESSION_ID sessionId, uInt32 continuous, uInt32 bufferCount, Rect rectangle);
IMAQ1394_CVI_API imaq1394StartAcquisition(SESSION_ID sessionId);
IMAQ1394_CVI_API imaq1394StopAcquisition(SESSION_ID sessionId);
IMAQ1394_CVI_API imaq1394ClearAcquisition(SESSION_ID sessionId);


//==============================================================================
//  CVI Acquisition functions that use IMAQ Vision
//==============================================================================
#if defined(NiVision_h)
IMAQ1394_CVI_API imaq1394SnapImage(SESSION_ID sessionId, Image* image, Rect rectangle);
IMAQ1394_CVI_API imaq1394GrabImage2(SESSION_ID sessionId, uInt32 waitForNextBuffer, uInt32* bufferNumberActual, Image* image);
IMAQ1394_CVI_API imaq1394SetupSequenceImage (SESSION_ID sessionId, Image** images, uInt32 imageCount, uInt32 skipCount, Rect rectangle);
IMAQ1394_CVI_API imaq1394GetImage2(SESSION_ID sessionId, uInt32 bufferNumberDesired, uInt32* bufferNumberActual, uInt32 onOverwrite, Image* image);
#endif


//==============================================================================
//  CVI Acquisition functions that do not need IMAQ Vision
//==============================================================================
IMAQ1394_CVI_API imaq1394Snap(SESSION_ID sessionId, void** bufferPtr, Rect rectangle);
IMAQ1394_CVI_API imaq1394Grab2(SESSION_ID sessionId, uInt32 waitForNextBuffer, uInt32* bufferNumberActual, void** bufferPtr);
IMAQ1394_CVI_API imaq1394SetupSequence(SESSION_ID sessionId, void* bufferPtrList[], uInt32 bufferCount, uInt32 skipCount, Rect rectangle);
IMAQ1394_CVI_API imaq1394GetBuffer2(SESSION_ID sessionId, uInt32 bufferNumberDesired, uInt32* bufferNumberActual, uInt32 onOverwrite, void** bufferPtr);
IMAQ1394_CVI_API imaq1394GetImageData(SESSION_ID sessionId, uInt32 bufferNumberDesired, uInt32* bufferNumberActual, uInt32 onOverwrite, void** bufferPtr);


//==============================================================================
//  Obsoleted functions  
//==============================================================================
IMAQ1394_CVI_API imaq1394CameraOpen(char* name, SESSION_ID* sessionId);
IMAQ1394_CVI_API imaq1394AttributeInquiry(SESSION_ID sessionId, uInt32 attribute, uInt32* enable, uInt32* minValue, uInt32* maxValue, uInt32* readable, uInt32* autoMode, uInt32* onePush);
IMAQ1394_CVI_API imaq1394GetFeatures(SESSION_ID sessionId, Feature* featureArray, uInt32* featureArraySize);
#if defined(NiVision_h)
IMAQ1394_CVI_API imaq1394GrabImage(SESSION_ID sessionId, Image* image);
IMAQ1394_CVI_API imaq1394GetImage(SESSION_ID sessionId, uInt32 bufferNumberDesired, Image* image);
#endif
IMAQ1394_CVI_API imaq1394Grab(SESSION_ID sessionId, void** bufferPtr);
IMAQ1394_CVI_API imaq1394GetBuffer(SESSION_ID sessionId, uInt32 bufferNumberDesired, void** bufferPtr);


#if (defined(__cplusplus) || defined(__cplusplus__))
}
#endif


#endif