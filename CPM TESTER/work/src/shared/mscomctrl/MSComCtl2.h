#ifndef _MSCOMCTL2_H
#define _MSCOMCTL2_H

#if defined(INCLUDE_AFTER_WINDOWS_H) && !defined(_INC_WINDOWS)
#error  This header must be included before utility.h and formatio.h
#error  because it includes cviauto.h which includes Windows SDK headers.
#endif /* INCLUDE_AFTER_WINDOWS_H */

#include <cviauto.h>

#ifdef __cplusplus
    extern "C" {
#endif
/* NICDBLD_BEGIN> Type Library Specific Types */

enum MSComCtl2Enum_BackStyleConstants
{
	MSComCtl2Const_cc2BackstyleTransparent = 0,
	MSComCtl2Const_cc2BackstyleOpaque = 1,
	_MSComCtl2_BackStyleConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_OLEDropConstants
{
	MSComCtl2Const_cc2OLEDropNone = 0,
	MSComCtl2Const_cc2OLEDropManual = 1,
	_MSComCtl2_OLEDropConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_AlignmentConstants
{
	MSComCtl2Const_cc2AlignmentLeft = 0,
	MSComCtl2Const_cc2AlignmentRight = 1,
	_MSComCtl2_AlignmentConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_OrientationConstants
{
	MSComCtl2Const_cc2OrientationVertical = 0,
	MSComCtl2Const_cc2OrientationHorizontal = 1,
	_MSComCtl2_OrientationConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_AppearanceConstants
{
	MSComCtl2Const_cc2Flat = 0,
	MSComCtl2Const_cc23D = 1,
	_MSComCtl2_AppearanceConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_BorderStyleConstants
{
	MSComCtl2Const_cc2None = 0,
	MSComCtl2Const_cc2FixedSingle = 1,
	_MSComCtl2_BorderStyleConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_DayConstants
{
	MSComCtl2Const_mvwSunday = 1,
	MSComCtl2Const_mvwMonday = 2,
	MSComCtl2Const_mvwTuesday = 3,
	MSComCtl2Const_mvwWednesday = 4,
	MSComCtl2Const_mvwThursday = 5,
	MSComCtl2Const_mvwFriday = 6,
	MSComCtl2Const_mvwSaturday = 7,
	_MSComCtl2_DayConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_MonthConstants
{
	MSComCtl2Const_mvwJanuary = 1,
	MSComCtl2Const_mvwFebruary = 2,
	MSComCtl2Const_mvwMarch = 3,
	MSComCtl2Const_mvwApril = 4,
	MSComCtl2Const_mvwMay = 5,
	MSComCtl2Const_mvwJune = 6,
	MSComCtl2Const_mvwJuly = 7,
	MSComCtl2Const_mvwAugust = 8,
	MSComCtl2Const_mvwSeptember = 9,
	MSComCtl2Const_mvwOctober = 10,
	MSComCtl2Const_mvwNovember = 11,
	MSComCtl2Const_mvwDecember = 12,
	_MSComCtl2_MonthConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_MousePointerConstants
{
	MSComCtl2Const_cc2Default = 0,
	MSComCtl2Const_cc2Arrow = 1,
	MSComCtl2Const_cc2Cross = 2,
	MSComCtl2Const_cc2IBeam = 3,
	MSComCtl2Const_cc2Icon = 4,
	MSComCtl2Const_cc2Size = 5,
	MSComCtl2Const_cc2SizeNESW = 6,
	MSComCtl2Const_cc2SizeNS = 7,
	MSComCtl2Const_cc2SizeNWSE = 8,
	MSComCtl2Const_cc2SizeEW = 9,
	MSComCtl2Const_cc2UpArrow = 10,
	MSComCtl2Const_cc2Hourglass = 11,
	MSComCtl2Const_cc2NoDrop = 12,
	MSComCtl2Const_cc2ArrowHourglass = 13,
	MSComCtl2Const_cc2ArrowQuestion = 14,
	MSComCtl2Const_cc2SizeAll = 15,
	MSComCtl2Const_cc2Custom = 99,
	_MSComCtl2_MousePointerConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_MonthViewHitTestAreas
{
	MSComCtl2Const_mvwCalendarBack = 0,
	MSComCtl2Const_mvwCalendarDate = 1,
	MSComCtl2Const_mvwCalendarDateNext = 2,
	MSComCtl2Const_mvwCalendarDatePrev = 3,
	MSComCtl2Const_mvwCalendarDay = 4,
	MSComCtl2Const_mvwCalendarWeekNum = 5,
	MSComCtl2Const_mvwNoWhere = 6,
	MSComCtl2Const_mvwTitleBack = 7,
	MSComCtl2Const_mvwTitleBtnNext = 8,
	MSComCtl2Const_mvwTitleBtnPrev = 9,
	MSComCtl2Const_mvwTitleMonth = 10,
	MSComCtl2Const_mvwTitleYear = 11,
	MSComCtl2Const_mvwTodayLink = 12,
	_MSComCtl2_MonthViewHitTestAreasForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_FormatConstants
{
	MSComCtl2Const_dtpLongDate = 0,
	MSComCtl2Const_dtpShortDate = 1,
	MSComCtl2Const_dtpTime = 2,
	MSComCtl2Const_dtpCustom = 3,
	_MSComCtl2_FormatConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_FlatScrollBarAppearanceConstants
{
	MSComCtl2Const_fsb3D = 0,
	MSComCtl2Const_fsbFlat = 1,
	MSComCtl2Const_fsbTrack3D = 2,
	_MSComCtl2_FlatScrollBarAppearanceConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_ArrowsConstants
{
	MSComCtl2Const_cc2Both = 0,
	MSComCtl2Const_cc2LeftUp = 1,
	MSComCtl2Const_cc2RightDown = 2,
	_MSComCtl2_ArrowsConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_DragOverConstants
{
	MSComCtl2Const_cc2Enter = 0,
	MSComCtl2Const_cc2Leave = 1,
	MSComCtl2Const_cc2Over = 2,
	_MSComCtl2_DragOverConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_ClipBoardConstants
{
	MSComCtl2Const_cc2CFText = 1,
	MSComCtl2Const_cc2CFBitmap = 2,
	MSComCtl2Const_cc2CFMetafile = 3,
	MSComCtl2Const_cc2CFDIB = 8,
	MSComCtl2Const_cc2CFPalette = 9,
	MSComCtl2Const_cc2CFEMetafile = 14,
	MSComCtl2Const_cc2CFFiles = 15,
	MSComCtl2Const_cc2CFRTF = -16639,
	_MSComCtl2_ClipBoardConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_OLEDropEffectConstants
{
	MSComCtl2Const_cc2OLEDropEffectNone = 0,
	MSComCtl2Const_cc2OLEDropEffectCopy = 1,
	MSComCtl2Const_cc2OLEDropEffectMove = 2,
	MSComCtl2Const_cc2OLEDropEffectScroll = -2147483648,
	_MSComCtl2_OLEDropEffectConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
enum MSComCtl2Enum_ErrorConstants
{
	MSComCtl2Const_cc2BadFileNameOrNumber = 52,
	MSComCtl2Const_cc2FileNotFound = 53,
	MSComCtl2Const_cc2InvalidPropertyValue = 380,
	MSComCtl2Const_cc2SetNotSupportedAtRuntime = 382,
	MSComCtl2Const_cc2SetNotSupported = 383,
	MSComCtl2Const_cc2GetNotSupported = 394,
	MSComCtl2Const_cc2DataObjectLocked = 672,
	MSComCtl2Const_cc2ExpectedAnArgument = 673,
	MSComCtl2Const_cc2InvalidProcedureCall = 5,
	MSComCtl2Const_cc2InvalidObjectUse = 425,
	MSComCtl2Const_cc2WrongClipboardFormat = 461,
	MSComCtl2Const_cc2RecursiveOleDrag = 674,
	MSComCtl2Const_cc2FormatNotByteArray = 675,
	MSComCtl2Const_cc2DataNotSetForFormat = 676,
	MSComCtl2Const_cc2InconsistentObject = 35750,
	MSComCtl2Const_cc2ErrorDuringSet = 35751,
	MSComCtl2Const_cc2ErrorOpeningVideo = 35752,
	MSComCtl2Const_cc2ErrorPlayingVideo = 35753,
	MSComCtl2Const_cc2VideoNotOpen = 35755,
	MSComCtl2Const_cc2ErrorStoppingVideo = 35757,
	MSComCtl2Const_cc2ErrorClosingVideo = 35758,
	MSComCtl2Const_cc2CantStopAutoPlay = 35759,
	MSComCtl2Const_cc2NoValidBuddyCtl = 35754,
	MSComCtl2Const_cc2AutoBuddyNotSet = 35756,
	MSComCtl2Const_cc2BuddyNotASibling = 35760,
	MSComCtl2Const_cc2NoUpDownAsBuddy = 35761,
	MSComCtl2Const_cc2InvalidRange = 35770,
	MSComCtl2Const_cc2InvalidMaxDate = 35771,
	MSComCtl2Const_cc2InvalidMinDate = 35772,
	MSComCtl2Const_cc2DateOutOfRange = 35773,
	MSComCtl2Const_cc2InvalidMaxDateMin = 35774,
	MSComCtl2Const_cc2InvalidMinDateMax = 35775,
	MSComCtl2Const_cc2InvalidRowColValue = 35776,
	MSComCtl2Const_cc2InvalidRowColTotal = 35777,
	MSComCtl2Const_cc2MonthViewError = 35778,
	MSComCtl2Const_cc2SetDayMultiSelectOn = 35779,
	MSComCtl2Const_cc2SetDayOfWeekMultiSelectOn = 35780,
	MSComCtl2Const_cc2SetMonthMultiSelectOn = 35781,
	MSComCtl2Const_cc2SetWeekMultiSelectOn = 35782,
	MSComCtl2Const_cc2SetYearMultiSelectOn = 35783,
	MSComCtl2Const_cc2SetMaxSelCountMultiSelectOff = 35784,
	MSComCtl2Const_cc2SetSelEndMultiSelectOff = 35785,
	MSComCtl2Const_cc2SetSelStartMultiSelectOff = 35786,
	MSComCtl2Const_cc2NullValueNotAllowed = 35787,
	MSComCtl2Const_cc2DTPickerError = 35788,
	MSComCtl2Const_cc2ScrollValueOutOfRange = 35789,
	_MSComCtl2_ErrorConstantsForceSizeToFourBytes = 0xFFFFFFFF
};
typedef CAObjHandle MSComCtl2Obj_IVBDataObjectFiles;
typedef long MSComCtl2Type_OLE_HANDLE;
typedef unsigned long MSComCtl2Type_OLE_COLOR;
typedef long MSComCtl2Type_OLE_XPOS_PIXELS;
typedef long MSComCtl2Type_OLE_YPOS_PIXELS;
typedef CAObjHandle MSComCtl2Obj_IVBDataObject;
typedef CAObjHandle MSComCtl2Obj_Font;
typedef CAObjHandle MSComCtl2Obj_IFontDisp;
typedef CAObjHandle MSComCtl2Obj_Picture;
typedef CAObjHandle MSComCtl2Obj_IPictureDisp;
typedef HRESULT (CVICALLBACK *DAnimationEventsRegOnClick_CallbackType) (CAObjHandle caServerObjHandle,
                                                                        void *caCallbackData);
typedef HRESULT (CVICALLBACK *DAnimationEventsRegOnDblClick_CallbackType) (CAObjHandle caServerObjHandle,
                                                                           void *caCallbackData);
typedef HRESULT (CVICALLBACK *DAnimationEventsRegOnMouseDown_CallbackType) (CAObjHandle caServerObjHandle,
                                                                            void *caCallbackData,
                                                                            short  button,
                                                                            short  shift,
                                                                            MSComCtl2Type_OLE_XPOS_PIXELS  x,
                                                                            MSComCtl2Type_OLE_YPOS_PIXELS  y);
typedef HRESULT (CVICALLBACK *DAnimationEventsRegOnMouseMove_CallbackType) (CAObjHandle caServerObjHandle,
                                                                            void *caCallbackData,
                                                                            short  button,
                                                                            short  shift,
                                                                            MSComCtl2Type_OLE_XPOS_PIXELS  x,
                                                                            MSComCtl2Type_OLE_YPOS_PIXELS  y);
typedef HRESULT (CVICALLBACK *DAnimationEventsRegOnMouseUp_CallbackType) (CAObjHandle caServerObjHandle,
                                                                          void *caCallbackData,
                                                                          short  button,
                                                                          short  shift,
                                                                          MSComCtl2Type_OLE_XPOS_PIXELS  x,
                                                                          MSComCtl2Type_OLE_YPOS_PIXELS  y);
typedef HRESULT (CVICALLBACK *DAnimationEventsRegOnOLEStartDrag_CallbackType) (CAObjHandle caServerObjHandle,
                                                                               void *caCallbackData,
                                                                               MSComCtl2Obj_IVBDataObject *data,
                                                                               long *allowedEffects);
typedef HRESULT (CVICALLBACK *DAnimationEventsRegOnOLEGiveFeedback_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                  void *caCallbackData,
                                                                                  long *effect,
                                                                                  VBOOL *defaultCursors);
typedef HRESULT (CVICALLBACK *DAnimationEventsRegOnOLESetData_CallbackType) (CAObjHandle caServerObjHandle,
                                                                             void *caCallbackData,
                                                                             MSComCtl2Obj_IVBDataObject *data,
                                                                             short *dataFormat);
typedef HRESULT (CVICALLBACK *DAnimationEventsRegOnOLECompleteDrag_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                  void *caCallbackData,
                                                                                  long *effect);
typedef HRESULT (CVICALLBACK *DAnimationEventsRegOnOLEDragOver_CallbackType) (CAObjHandle caServerObjHandle,
                                                                              void *caCallbackData,
                                                                              MSComCtl2Obj_IVBDataObject *data,
                                                                              long *effect,
                                                                              short *button,
                                                                              short *shift,
                                                                              float *x,
                                                                              float *y,
                                                                              short *state);
typedef HRESULT (CVICALLBACK *DAnimationEventsRegOnOLEDragDrop_CallbackType) (CAObjHandle caServerObjHandle,
                                                                              void *caCallbackData,
                                                                              MSComCtl2Obj_IVBDataObject *data,
                                                                              long *effect,
                                                                              short *button,
                                                                              short *shift,
                                                                              float *x,
                                                                              float *y);
typedef HRESULT (CVICALLBACK *DUpDownEventsRegOnChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                      void *caCallbackData);
typedef HRESULT (CVICALLBACK *DUpDownEventsRegOnDownClick_CallbackType) (CAObjHandle caServerObjHandle,
                                                                         void *caCallbackData);
typedef HRESULT (CVICALLBACK *DUpDownEventsRegOnUpClick_CallbackType) (CAObjHandle caServerObjHandle,
                                                                       void *caCallbackData);
typedef HRESULT (CVICALLBACK *DUpDownEventsRegOnMouseDown_CallbackType) (CAObjHandle caServerObjHandle,
                                                                         void *caCallbackData,
                                                                         short  button,
                                                                         short  shift,
                                                                         MSComCtl2Type_OLE_XPOS_PIXELS  x,
                                                                         MSComCtl2Type_OLE_YPOS_PIXELS  y);
typedef HRESULT (CVICALLBACK *DUpDownEventsRegOnMouseMove_CallbackType) (CAObjHandle caServerObjHandle,
                                                                         void *caCallbackData,
                                                                         short  button,
                                                                         short  shift,
                                                                         MSComCtl2Type_OLE_XPOS_PIXELS  x,
                                                                         MSComCtl2Type_OLE_YPOS_PIXELS  y);
typedef HRESULT (CVICALLBACK *DUpDownEventsRegOnMouseUp_CallbackType) (CAObjHandle caServerObjHandle,
                                                                       void *caCallbackData,
                                                                       short  button,
                                                                       short  shift,
                                                                       MSComCtl2Type_OLE_XPOS_PIXELS  x,
                                                                       MSComCtl2Type_OLE_YPOS_PIXELS  y);
typedef HRESULT (CVICALLBACK *DUpDownEventsRegOnOLEStartDrag_CallbackType) (CAObjHandle caServerObjHandle,
                                                                            void *caCallbackData,
                                                                            MSComCtl2Obj_IVBDataObject *data,
                                                                            long *allowedEffects);
typedef HRESULT (CVICALLBACK *DUpDownEventsRegOnOLEGiveFeedback_CallbackType) (CAObjHandle caServerObjHandle,
                                                                               void *caCallbackData,
                                                                               long *effect,
                                                                               VBOOL *defaultCursors);
typedef HRESULT (CVICALLBACK *DUpDownEventsRegOnOLESetData_CallbackType) (CAObjHandle caServerObjHandle,
                                                                          void *caCallbackData,
                                                                          MSComCtl2Obj_IVBDataObject *data,
                                                                          short *dataFormat);
typedef HRESULT (CVICALLBACK *DUpDownEventsRegOnOLECompleteDrag_CallbackType) (CAObjHandle caServerObjHandle,
                                                                               void *caCallbackData,
                                                                               long *effect);
typedef HRESULT (CVICALLBACK *DUpDownEventsRegOnOLEDragOver_CallbackType) (CAObjHandle caServerObjHandle,
                                                                           void *caCallbackData,
                                                                           MSComCtl2Obj_IVBDataObject *data,
                                                                           long *effect,
                                                                           short *button,
                                                                           short *shift,
                                                                           float *x,
                                                                           float *y,
                                                                           short *state);
typedef HRESULT (CVICALLBACK *DUpDownEventsRegOnOLEDragDrop_CallbackType) (CAObjHandle caServerObjHandle,
                                                                           void *caCallbackData,
                                                                           MSComCtl2Obj_IVBDataObject *data,
                                                                           long *effect,
                                                                           short *button,
                                                                           short *shift,
                                                                           float *x,
                                                                           float *y);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnDateClick_CallbackType) (CAObjHandle caServerObjHandle,
                                                                            void *caCallbackData,
                                                                            DATE  dateClicked);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnDateDblClick_CallbackType) (CAObjHandle caServerObjHandle,
                                                                               void *caCallbackData,
                                                                               DATE  dateDblClicked);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnGetDayBold_CallbackType) (CAObjHandle caServerObjHandle,
                                                                             void *caCallbackData,
                                                                             DATE  startDate,
                                                                             short  count,
                                                                             SAFEARRAY **state);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnSelChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                            void *caCallbackData,
                                                                            DATE  startDate,
                                                                            DATE  endDate,
                                                                            VBOOL *cancel);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnClick_CallbackType) (CAObjHandle caServerObjHandle,
                                                                        void *caCallbackData);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnDblClick_CallbackType) (CAObjHandle caServerObjHandle,
                                                                           void *caCallbackData);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnKeyDown_CallbackType) (CAObjHandle caServerObjHandle,
                                                                          void *caCallbackData,
                                                                          short *keyCode,
                                                                          short  shift);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnKeyUp_CallbackType) (CAObjHandle caServerObjHandle,
                                                                        void *caCallbackData,
                                                                        short *keyCode,
                                                                        short  shift);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnKeyPress_CallbackType) (CAObjHandle caServerObjHandle,
                                                                           void *caCallbackData,
                                                                           short *keyAscii);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnMouseDown_CallbackType) (CAObjHandle caServerObjHandle,
                                                                            void *caCallbackData,
                                                                            short  button,
                                                                            short  shift,
                                                                            MSComCtl2Type_OLE_XPOS_PIXELS  x,
                                                                            MSComCtl2Type_OLE_YPOS_PIXELS  y);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnMouseMove_CallbackType) (CAObjHandle caServerObjHandle,
                                                                            void *caCallbackData,
                                                                            short  button,
                                                                            short  shift,
                                                                            MSComCtl2Type_OLE_XPOS_PIXELS  x,
                                                                            MSComCtl2Type_OLE_YPOS_PIXELS  y);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnMouseUp_CallbackType) (CAObjHandle caServerObjHandle,
                                                                          void *caCallbackData,
                                                                          short  button,
                                                                          short  shift,
                                                                          MSComCtl2Type_OLE_XPOS_PIXELS  x,
                                                                          MSComCtl2Type_OLE_YPOS_PIXELS  y);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnOLEStartDrag_CallbackType) (CAObjHandle caServerObjHandle,
                                                                               void *caCallbackData,
                                                                               MSComCtl2Obj_IVBDataObject *data,
                                                                               long *allowedEffects);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnOLEGiveFeedback_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                  void *caCallbackData,
                                                                                  long *effect,
                                                                                  VBOOL *defaultCursors);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnOLESetData_CallbackType) (CAObjHandle caServerObjHandle,
                                                                             void *caCallbackData,
                                                                             MSComCtl2Obj_IVBDataObject *data,
                                                                             short *dataFormat);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnOLECompleteDrag_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                  void *caCallbackData,
                                                                                  long *effect);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnOLEDragOver_CallbackType) (CAObjHandle caServerObjHandle,
                                                                              void *caCallbackData,
                                                                              MSComCtl2Obj_IVBDataObject *data,
                                                                              long *effect,
                                                                              short *button,
                                                                              short *shift,
                                                                              float *x,
                                                                              float *y,
                                                                              short *state);
typedef HRESULT (CVICALLBACK *DMonthViewEventsRegOnOLEDragDrop_CallbackType) (CAObjHandle caServerObjHandle,
                                                                              void *caCallbackData,
                                                                              MSComCtl2Obj_IVBDataObject *data,
                                                                              long *effect,
                                                                              short *button,
                                                                              short *shift,
                                                                              float *x,
                                                                              float *y);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnCallbackKeyDown_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                 void *caCallbackData,
                                                                                 short  keyCode,
                                                                                 short  shift,
                                                                                 char *callbackField,
                                                                                 DATE *callbackDate);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                        void *caCallbackData);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnCloseUp_CallbackType) (CAObjHandle caServerObjHandle,
                                                                         void *caCallbackData);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnDropDown_CallbackType) (CAObjHandle caServerObjHandle,
                                                                          void *caCallbackData);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnFormat_CallbackType) (CAObjHandle caServerObjHandle,
                                                                        void *caCallbackData,
                                                                        char *callbackField,
                                                                        char **formattedString);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnFormatSize_CallbackType) (CAObjHandle caServerObjHandle,
                                                                            void *caCallbackData,
                                                                            char *callbackField,
                                                                            short *size);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnClick_CallbackType) (CAObjHandle caServerObjHandle,
                                                                       void *caCallbackData);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnDblClick_CallbackType) (CAObjHandle caServerObjHandle,
                                                                          void *caCallbackData);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnKeyDown_CallbackType) (CAObjHandle caServerObjHandle,
                                                                         void *caCallbackData,
                                                                         short *keyCode,
                                                                         short  shift);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnKeyUp_CallbackType) (CAObjHandle caServerObjHandle,
                                                                       void *caCallbackData,
                                                                       short *keyCode,
                                                                       short  shift);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnKeyPress_CallbackType) (CAObjHandle caServerObjHandle,
                                                                          void *caCallbackData,
                                                                          short *keyAscii);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnMouseDown_CallbackType) (CAObjHandle caServerObjHandle,
                                                                           void *caCallbackData,
                                                                           short  button,
                                                                           short  shift,
                                                                           MSComCtl2Type_OLE_XPOS_PIXELS  x,
                                                                           MSComCtl2Type_OLE_YPOS_PIXELS  y);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnMouseMove_CallbackType) (CAObjHandle caServerObjHandle,
                                                                           void *caCallbackData,
                                                                           short  button,
                                                                           short  shift,
                                                                           MSComCtl2Type_OLE_XPOS_PIXELS  x,
                                                                           MSComCtl2Type_OLE_YPOS_PIXELS  y);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnMouseUp_CallbackType) (CAObjHandle caServerObjHandle,
                                                                         void *caCallbackData,
                                                                         short  button,
                                                                         short  shift,
                                                                         MSComCtl2Type_OLE_XPOS_PIXELS  x,
                                                                         MSComCtl2Type_OLE_YPOS_PIXELS  y);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnOLEStartDrag_CallbackType) (CAObjHandle caServerObjHandle,
                                                                              void *caCallbackData,
                                                                              MSComCtl2Obj_IVBDataObject *data,
                                                                              long *allowedEffects);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnOLEGiveFeedback_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                 void *caCallbackData,
                                                                                 long *effect,
                                                                                 VBOOL *defaultCursors);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnOLESetData_CallbackType) (CAObjHandle caServerObjHandle,
                                                                            void *caCallbackData,
                                                                            MSComCtl2Obj_IVBDataObject *data,
                                                                            short *dataFormat);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnOLECompleteDrag_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                 void *caCallbackData,
                                                                                 long *effect);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnOLEDragOver_CallbackType) (CAObjHandle caServerObjHandle,
                                                                             void *caCallbackData,
                                                                             MSComCtl2Obj_IVBDataObject *data,
                                                                             long *effect,
                                                                             short *button,
                                                                             short *shift,
                                                                             float *x,
                                                                             float *y,
                                                                             short *state);
typedef HRESULT (CVICALLBACK *DDTPickerEventsRegOnOLEDragDrop_CallbackType) (CAObjHandle caServerObjHandle,
                                                                             void *caCallbackData,
                                                                             MSComCtl2Obj_IVBDataObject *data,
                                                                             long *effect,
                                                                             short *button,
                                                                             short *shift,
                                                                             float *x,
                                                                             float *y);
typedef HRESULT (CVICALLBACK *DFlatSBEventsRegOnChange_CallbackType) (CAObjHandle caServerObjHandle,
                                                                      void *caCallbackData);
typedef HRESULT (CVICALLBACK *DFlatSBEventsRegOnScroll_CallbackType) (CAObjHandle caServerObjHandle,
                                                                      void *caCallbackData);
/* NICDBLD_END> Type Library Specific Types */

extern const IID MSComCtl2_IID_IAnimation;
extern const IID MSComCtl2_IID_IUpDown;
extern const IID MSComCtl2_IID_IMonthView;
extern const IID MSComCtl2_IID_IDTPicker;
extern const IID MSComCtl2_IID_IFlatSB;
extern const IID MSComCtl2_IID_DAnimationEvents;
extern const IID MSComCtl2_IID_DUpDownEvents;
extern const IID MSComCtl2_IID_DMonthViewEvents;
extern const IID MSComCtl2_IID_DDTPickerEvents;
extern const IID MSComCtl2_IID_DFlatSBEvents;
extern const IID MSComCtl2_IID_IVBDataObject;
extern const IID MSComCtl2_IID_IVBDataObjectFiles;

HRESULT CVIFUNC MSComCtl2_IVBDataObjectClear (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_IVBDataObjectGetData (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                short sFormat, VARIANT *pvData);

HRESULT CVIFUNC MSComCtl2_IVBDataObjectGetFormat (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  short sFormat,
                                                  VBOOL *pbFormatSupported);

HRESULT CVIFUNC MSComCtl2_IVBDataObjectSetData (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VARIANT vValue, VARIANT vFormat);

HRESULT CVIFUNC MSComCtl2_IVBDataObjectGetFiles (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSComCtl2Obj_IVBDataObjectFiles *pFiles);

HRESULT CVIFUNC MSComCtl2_IVBDataObjectFilesGetItem (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     long lIndex,
                                                     char **bstrItem);

HRESULT CVIFUNC MSComCtl2_IVBDataObjectFilesGetCount (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      long *plCount);

HRESULT CVIFUNC MSComCtl2_IVBDataObjectFilesAdd (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *bstrFilename,
                                                 VARIANT vIndex);

HRESULT CVIFUNC MSComCtl2_IVBDataObjectFilesClear (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_IVBDataObjectFilesRemove (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VARIANT vIndex);

HRESULT CVIFUNC MSComCtl2_IVBDataObjectFiles_NewEnum (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      LPUNKNOWN *ppUnk);

HRESULT CVIFUNC MSComCtl2_NewIAnimation (int panel, const char *label, int top,
                                         int left, int *controlID, int *UILError);

HRESULT CVIFUNC MSComCtl2_IAnimationSetAutoPlay (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VBOOL pfAutoPlay);

HRESULT CVIFUNC MSComCtl2_IAnimationGetAutoPlay (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VBOOL *pfAutoPlay);

HRESULT CVIFUNC MSComCtl2_IAnimationSetBackStyle (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  enum MSComCtl2Enum_BackStyleConstants penumBackStyle);

HRESULT CVIFUNC MSComCtl2_IAnimationGetBackStyle (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  enum MSComCtl2Enum_BackStyleConstants *penumBackStyle);

HRESULT CVIFUNC MSComCtl2_IAnimationSetCenter (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL pfCenter);

HRESULT CVIFUNC MSComCtl2_IAnimationGetCenter (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *pfCenter);

HRESULT CVIFUNC MSComCtl2_IAnimationSetEnabled (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL pfEnable);

HRESULT CVIFUNC MSComCtl2_IAnimationGetEnabled (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL *pfEnable);

HRESULT CVIFUNC MSComCtl2_IAnimationGethWnd (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             MSComCtl2Type_OLE_HANDLE *phWnd);

HRESULT CVIFUNC MSComCtl2_IAnimationSetBackColor (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Type_OLE_COLOR pocBackColor);

HRESULT CVIFUNC MSComCtl2_IAnimationGetBackColor (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Type_OLE_COLOR *pocBackColor);

HRESULT CVIFUNC MSComCtl2_IAnimationGetOLEDropMode (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_OLEDropConstants *psOLEDropMode);

HRESULT CVIFUNC MSComCtl2_IAnimationSetOLEDropMode (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_OLEDropConstants psOLEDropMode);

HRESULT CVIFUNC MSComCtl2_IAnimationClose (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_IAnimationOpen (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          const char *bstrFilename);

HRESULT CVIFUNC MSComCtl2_IAnimationPlay (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          VARIANT varRepeatCount,
                                          VARIANT varStartFrame,
                                          VARIANT varEndFrame);

HRESULT CVIFUNC MSComCtl2_IAnimationStop (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_IAnimationAboutBox (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_IAnimationOLEDrag (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_NewIUpDown (int panel, const char *label, int top,
                                      int left, int *controlID, int *UILError);

HRESULT CVIFUNC MSComCtl2_IUpDownSetValue (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, long plValue);

HRESULT CVIFUNC MSComCtl2_IUpDownGetValue (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, long *plValue);

HRESULT CVIFUNC MSComCtl2_IUpDownSetAcceleration (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  short psAcceleration);

HRESULT CVIFUNC MSComCtl2_IUpDownGetAcceleration (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  short *psAcceleration);

HRESULT CVIFUNC MSComCtl2_IUpDownSetAlignment (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               enum MSComCtl2Enum_AlignmentConstants penumAlignment);

HRESULT CVIFUNC MSComCtl2_IUpDownGetAlignment (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               enum MSComCtl2Enum_AlignmentConstants *penumAlignment);

HRESULT CVIFUNC MSComCtl2_IUpDownSetAutoBuddy (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL pfAutoBuddy);

HRESULT CVIFUNC MSComCtl2_IUpDownGetAutoBuddy (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *pfAutoBuddy);

HRESULT CVIFUNC MSComCtl2_IUpDownSetBuddyControl (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VARIANT pvarBuddyCtl);

HRESULT CVIFUNC MSComCtl2_IUpDownSetByRefBuddyControl (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VARIANT pvarBuddyCtl);

HRESULT CVIFUNC MSComCtl2_IUpDownGetBuddyControl (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VARIANT *pvarBuddyCtl);

HRESULT CVIFUNC MSComCtl2_IUpDownSetIncrement (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               long plIncrement);

HRESULT CVIFUNC MSComCtl2_IUpDownGetIncrement (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               long *plIncrement);

HRESULT CVIFUNC MSComCtl2_IUpDownSetMax (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, long plMax);

HRESULT CVIFUNC MSComCtl2_IUpDownGetMax (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, long *plMax);

HRESULT CVIFUNC MSComCtl2_IUpDownSetMin (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, long plMin);

HRESULT CVIFUNC MSComCtl2_IUpDownGetMin (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, long *plMin);

HRESULT CVIFUNC MSComCtl2_IUpDownSetOrientation (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 enum MSComCtl2Enum_OrientationConstants penumOrientation);

HRESULT CVIFUNC MSComCtl2_IUpDownGetOrientation (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 enum MSComCtl2Enum_OrientationConstants *penumOrientation);

HRESULT CVIFUNC MSComCtl2_IUpDownSetSyncBuddy (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL pfSyncBuddy);

HRESULT CVIFUNC MSComCtl2_IUpDownGetSyncBuddy (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *pfSyncBuddy);

HRESULT CVIFUNC MSComCtl2_IUpDownSetWrap (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, VBOOL pfWrap);

HRESULT CVIFUNC MSComCtl2_IUpDownGetWrap (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, VBOOL *pfWrap);

HRESULT CVIFUNC MSComCtl2_IUpDownSetBuddyProperty (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VARIANT pvarDispidBuddyProperty);

HRESULT CVIFUNC MSComCtl2_IUpDownGetBuddyProperty (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VARIANT *pvarDispidBuddyProperty);

HRESULT CVIFUNC MSComCtl2_IUpDownSetEnabled (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VBOOL pfEnable);

HRESULT CVIFUNC MSComCtl2_IUpDownGetEnabled (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VBOOL *pfEnable);

HRESULT CVIFUNC MSComCtl2_IUpDownGethWnd (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          MSComCtl2Type_OLE_HANDLE *phWnd);

HRESULT CVIFUNC MSComCtl2_IUpDownGetOLEDropMode (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 enum MSComCtl2Enum_OLEDropConstants *psOLEDropMode);

HRESULT CVIFUNC MSComCtl2_IUpDownSetOLEDropMode (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 enum MSComCtl2Enum_OLEDropConstants psOLEDropMode);

HRESULT CVIFUNC MSComCtl2_IUpDownAboutBox (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_IUpDownOLEDrag (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_NewIMonthView (int panel, const char *label, int top,
                                         int left, int *controlID, int *UILError);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetAppearance (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   enum MSComCtl2Enum_AppearanceConstants *penumAppearances);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetAppearance (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   enum MSComCtl2Enum_AppearanceConstants penumAppearances);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetBackColor (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Type_OLE_COLOR *pclrBackColor);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetBackColor (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Type_OLE_COLOR pclrBackColor);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetBorderStyle (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_BorderStyleConstants *psBorderStyle);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetBorderStyle (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_BorderStyleConstants psBorderStyle);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetDay (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, short *psDay);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetDay (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, short psDay);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetDayBold (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                DATE dateIndex, VBOOL *pbDayBold);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetDayBold (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                DATE dateIndex, VBOOL pbDayBold);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetDayOfWeek (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  enum MSComCtl2Enum_DayConstants *penumDayOfWeek);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetDayOfWeek (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  enum MSComCtl2Enum_DayConstants penumDayOfWeek);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetEnabled (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL *pbEnabled);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetEnabled (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL pbEnabled);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetFont (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             MSComCtl2Obj_IFontDisp *ppFont);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetFont (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             MSComCtl2Obj_IFontDisp ppFont);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetByRefFont (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Obj_IFontDisp ppFont);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetForeColor (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Type_OLE_COLOR *pclrForeColor);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetForeColor (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Type_OLE_COLOR pclrForeColor);

HRESULT CVIFUNC MSComCtl2_IMonthViewGethWnd (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             MSComCtl2Type_OLE_HANDLE *phWnd);

HRESULT CVIFUNC MSComCtl2_IMonthViewSethWnd (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             MSComCtl2Type_OLE_HANDLE phWnd);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMaxDate (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                DATE *pdateMaxDate);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMaxDate (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                DATE pdateMaxDate);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMaxSelCount (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    short *psMaxSelCount);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMaxSelCount (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    short psMaxSelCount);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMinDate (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                DATE *pdateMinDate);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMinDate (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                DATE pdateMinDate);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMonth (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              enum MSComCtl2Enum_MonthConstants *psMonth);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMonth (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              enum MSComCtl2Enum_MonthConstants psMonth);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMonthBackColor (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSComCtl2Type_OLE_COLOR *pclrMonthBackColor);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMonthBackColor (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSComCtl2Type_OLE_COLOR pclrMonthBackColor);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMonthColumns (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     short *psMonthColumns);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMonthColumns (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     short psMonthColumns);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMonthRows (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  short *psMonthRows);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMonthRows (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  short psMonthRows);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMouseIcon (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Obj_IPictureDisp *ppMouseIcon);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMouseIcon (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  MSComCtl2Obj_IPictureDisp ppMouseIcon);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetByRefMouseIcon (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSComCtl2Obj_IPictureDisp ppMouseIcon);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMousePointer (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     enum MSComCtl2Enum_MousePointerConstants *psMousePointer);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMousePointer (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     enum MSComCtl2Enum_MousePointerConstants psMousePointer);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetMultiSelect (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL *pbMultiSelect);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetMultiSelect (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    VBOOL pbMultiSelect);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetOLEDropMode (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_OLEDropConstants *psOLEDropMode);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetOLEDropMode (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_OLEDropConstants psOLEDropMode);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetScrollRate (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   short *psScrollRate);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetScrollRate (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   short psScrollRate);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetSelEnd (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE *pdateSelEnd);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetSelEnd (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE pdateSelEnd);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetSelStart (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 DATE *pdateSelStart);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetSelStart (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 DATE pdateSelStart);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetShowToday (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL *pbShowToday);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetShowToday (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL pbShowToday);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetShowWeekNumbers (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL *pbShowWeekNumbers);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetShowWeekNumbers (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL pbShowWeekNumbers);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetStartOfWeek (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_DayConstants *penumStartOfWeek);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetStartOfWeek (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_DayConstants penumStartOfWeek);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetTitleBackColor (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSComCtl2Type_OLE_COLOR *pclrTitleBackColor);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetTitleBackColor (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSComCtl2Type_OLE_COLOR pclrTitleBackColor);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetTitleForeColor (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSComCtl2Type_OLE_COLOR *pclrTitleForeColor);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetTitleForeColor (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       MSComCtl2Type_OLE_COLOR pclrTitleForeColor);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetTrailingForeColor (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          MSComCtl2Type_OLE_COLOR *pclrTrailingForeColor);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetTrailingForeColor (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          MSComCtl2Type_OLE_COLOR pclrTrailingForeColor);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetValue (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              DATE *pdateValue);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetValue (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              DATE pdateValue);

HRESULT CVIFUNC MSComCtl2_IMonthViewGet_Value (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE *pdateValue);

HRESULT CVIFUNC MSComCtl2_IMonthViewSet_Value (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE pdateValue);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetVisibleDays (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    short sIndex,
                                                    DATE *pdateVisibleDays);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetVisibleDays (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    short sIndex,
                                                    DATE pdateVisibleDays);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetWeek (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, short *psWeek);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetWeek (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, short psWeek);

HRESULT CVIFUNC MSComCtl2_IMonthViewGetYear (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, short *psYear);

HRESULT CVIFUNC MSComCtl2_IMonthViewSetYear (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, short psYear);

HRESULT CVIFUNC MSComCtl2_IMonthViewAboutBox (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_IMonthViewComputeControlSize (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        short rows,
                                                        short columns,
                                                        float *width,
                                                        float *height);

HRESULT CVIFUNC MSComCtl2_IMonthViewHitTest (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, long x,
                                             long y, DATE *date,
                                             enum MSComCtl2Enum_MonthViewHitTestAreas *enumHitTestArea);

HRESULT CVIFUNC MSComCtl2_IMonthViewOLEDrag (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_IMonthViewRefresh (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_NewIDTPicker (int panel, const char *label, int top,
                                        int left, int *controlID, int *UILError);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetFormat (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              enum MSComCtl2Enum_FormatConstants *penumFormat);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetFormat (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              enum MSComCtl2Enum_FormatConstants penumFormat);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetCalendarBackColor (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSComCtl2Type_OLE_COLOR *pclrCalendarBackColor);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetCalendarBackColor (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSComCtl2Type_OLE_COLOR pclrCalendarBackColor);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetCalendarForeColor (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSComCtl2Type_OLE_COLOR *pclrCalendarForeColor);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetCalendarForeColor (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         MSComCtl2Type_OLE_COLOR pclrCalendarForeColor);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetCalendarTitleBackColor (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              MSComCtl2Type_OLE_COLOR *pclrTitleBackColor);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetCalendarTitleBackColor (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              MSComCtl2Type_OLE_COLOR pclrTitleBackColor);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetCalendarTitleForeColor (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              MSComCtl2Type_OLE_COLOR *pclrTitleForeColor);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetCalendarTitleForeColor (CAObjHandle objectHandle,
                                                              ERRORINFO *errorInfo,
                                                              MSComCtl2Type_OLE_COLOR pclrTitleForeColor);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetCalendarTrailingForeColor (CAObjHandle objectHandle,
                                                                 ERRORINFO *errorInfo,
                                                                 MSComCtl2Type_OLE_COLOR *pclrTrailingForeColor);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetCalendarTrailingForeColor (CAObjHandle objectHandle,
                                                                 ERRORINFO *errorInfo,
                                                                 MSComCtl2Type_OLE_COLOR pclrTrailingForeColor);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetCheckBox (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL *pbEnabled);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetCheckBox (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                VBOOL pbEnabled);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetCustomFormat (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    char **pbstrCustomFormat);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetCustomFormat (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    const char *pbstrCustomFormat);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetDay (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, VARIANT *pvDay);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetDay (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, VARIANT pvDay);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetDayOfWeek (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VARIANT *pvDayOfWeek);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetDayOfWeek (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VARIANT pvDayOfWeek);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetEnabled (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL *pbEnabled);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetEnabled (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               VBOOL pbEnabled);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetFont (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            MSComCtl2Obj_IFontDisp *ppFont);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetFont (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            MSComCtl2Obj_IFontDisp ppFont);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetByRefFont (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSComCtl2Obj_IFontDisp ppFont);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetHour (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            VARIANT *pvHour);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetHour (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, VARIANT pvHour);

HRESULT CVIFUNC MSComCtl2_IDTPickerGethWnd (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            MSComCtl2Type_OLE_HANDLE *phWnd);

HRESULT CVIFUNC MSComCtl2_IDTPickerSethWnd (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            MSComCtl2Type_OLE_HANDLE phWnd);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetMaxDate (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE *pdateMaxDate);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetMaxDate (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE pdateMaxDate);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetMinDate (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE *pdateMinDate);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetMinDate (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               DATE pdateMinDate);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetMinute (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VARIANT *pvMinute);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetMinute (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VARIANT pvMinute);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetMonth (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VARIANT *pvMonth);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetMonth (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VARIANT pvMonth);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetMouseIcon (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSComCtl2Obj_IPictureDisp *ppMouseIcon);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetMouseIcon (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 MSComCtl2Obj_IPictureDisp ppMouseIcon);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetByRefMouseIcon (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      MSComCtl2Obj_IPictureDisp ppMouseIcon);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetMousePointer (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_MousePointerConstants *psMousePointer);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetMousePointer (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    enum MSComCtl2Enum_MousePointerConstants psMousePointer);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetOLEDropMode (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   enum MSComCtl2Enum_OLEDropConstants *psOLEDropMode);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetOLEDropMode (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   enum MSComCtl2Enum_OLEDropConstants psOLEDropMode);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetSecond (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VARIANT *pvSecond);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetSecond (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VARIANT pvSecond);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetUpDown (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VBOOL *pbUpDown);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetUpDown (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VBOOL pbUpDown);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetValue (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VARIANT *pvdateValue);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetValue (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VARIANT pvdateValue);

HRESULT CVIFUNC MSComCtl2_IDTPickerGet_Value (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VARIANT *pvdateValue);

HRESULT CVIFUNC MSComCtl2_IDTPickerSet_Value (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VARIANT pvdateValue);

HRESULT CVIFUNC MSComCtl2_IDTPickerGetYear (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            VARIANT *pvYear);

HRESULT CVIFUNC MSComCtl2_IDTPickerSetYear (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, VARIANT pvYear);

HRESULT CVIFUNC MSComCtl2_IDTPickerAboutBox (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_IDTPickerOLEDrag (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_IDTPickerRefresh (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_NewIFlatSB (int panel, const char *label, int top,
                                      int left, int *controlID, int *UILError);

HRESULT CVIFUNC MSComCtl2_IFlatSBGetAppearance (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                enum MSComCtl2Enum_FlatScrollBarAppearanceConstants *penumAppearance);

HRESULT CVIFUNC MSComCtl2_IFlatSBSetAppearance (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                enum MSComCtl2Enum_FlatScrollBarAppearanceConstants penumAppearance);

HRESULT CVIFUNC MSComCtl2_IFlatSBGetArrows (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            enum MSComCtl2Enum_ArrowsConstants *penumArrows);

HRESULT CVIFUNC MSComCtl2_IFlatSBSetArrows (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            enum MSComCtl2Enum_ArrowsConstants penumArrows);

HRESULT CVIFUNC MSComCtl2_IFlatSBGetEnabled (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VBOOL *pbEnabled);

HRESULT CVIFUNC MSComCtl2_IFlatSBSetEnabled (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             VBOOL pbEnabled);

HRESULT CVIFUNC MSComCtl2_IFlatSBGethWnd (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          MSComCtl2Type_OLE_HANDLE *phWnd);

HRESULT CVIFUNC MSComCtl2_IFlatSBSethWnd (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          MSComCtl2Type_OLE_HANDLE phWnd);

HRESULT CVIFUNC MSComCtl2_IFlatSBGetLargeChange (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 short *psLargeChange);

HRESULT CVIFUNC MSComCtl2_IFlatSBSetLargeChange (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 short psLargeChange);

HRESULT CVIFUNC MSComCtl2_IFlatSBGetMin (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, short *psMin);

HRESULT CVIFUNC MSComCtl2_IFlatSBSetMin (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, short psMin);

HRESULT CVIFUNC MSComCtl2_IFlatSBGetMax (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, short *psMax);

HRESULT CVIFUNC MSComCtl2_IFlatSBSetMax (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, short psMax);

HRESULT CVIFUNC MSComCtl2_IFlatSBGetMouseIcon (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               MSComCtl2Obj_IPictureDisp *ppMouseIcon);

HRESULT CVIFUNC MSComCtl2_IFlatSBSetMouseIcon (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               MSComCtl2Obj_IPictureDisp ppMouseIcon);

HRESULT CVIFUNC MSComCtl2_IFlatSBSetByRefMouseIcon (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    MSComCtl2Obj_IPictureDisp ppMouseIcon);

HRESULT CVIFUNC MSComCtl2_IFlatSBGetMousePointer (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  enum MSComCtl2Enum_MousePointerConstants *psMousePointer);

HRESULT CVIFUNC MSComCtl2_IFlatSBSetMousePointer (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  enum MSComCtl2Enum_MousePointerConstants psMousePointer);

HRESULT CVIFUNC MSComCtl2_IFlatSBGetOrientation (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 enum MSComCtl2Enum_OrientationConstants *penumOrientation);

HRESULT CVIFUNC MSComCtl2_IFlatSBSetOrientation (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 enum MSComCtl2Enum_OrientationConstants penumOrientation);

HRESULT CVIFUNC MSComCtl2_IFlatSBGetSmallChange (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 short *psSmallChange);

HRESULT CVIFUNC MSComCtl2_IFlatSBSetSmallChange (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 short psSmallChange);

HRESULT CVIFUNC MSComCtl2_IFlatSBGetValue (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, short *psValue);

HRESULT CVIFUNC MSComCtl2_IFlatSBSetValue (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, short psValue);

HRESULT CVIFUNC MSComCtl2_IFlatSBGet_Value (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, short *psValue);

HRESULT CVIFUNC MSComCtl2_IFlatSBSet_Value (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, short psValue);

HRESULT CVIFUNC MSComCtl2_IFlatSBAboutBox (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_IFlatSBRefresh (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo);

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnClick (CAObjHandle serverObject,
                                                      DAnimationEventsRegOnClick_CallbackType callbackFunction,
                                                      void *callbackData,
                                                      int enableCallbacks,
                                                      int *callbackId);

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnDblClick (CAObjHandle serverObject,
                                                         DAnimationEventsRegOnDblClick_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId);

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnMouseDown (CAObjHandle serverObject,
                                                          DAnimationEventsRegOnMouseDown_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId);

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnMouseMove (CAObjHandle serverObject,
                                                          DAnimationEventsRegOnMouseMove_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId);

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnMouseUp (CAObjHandle serverObject,
                                                        DAnimationEventsRegOnMouseUp_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId);

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnOLEStartDrag (CAObjHandle serverObject,
                                                             DAnimationEventsRegOnOLEStartDrag_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId);

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnOLEGiveFeedback (CAObjHandle serverObject,
                                                                DAnimationEventsRegOnOLEGiveFeedback_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId);

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnOLESetData (CAObjHandle serverObject,
                                                           DAnimationEventsRegOnOLESetData_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId);

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnOLECompleteDrag (CAObjHandle serverObject,
                                                                DAnimationEventsRegOnOLECompleteDrag_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId);

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnOLEDragOver (CAObjHandle serverObject,
                                                            DAnimationEventsRegOnOLEDragOver_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId);

HRESULT CVIFUNC MSComCtl2_DAnimationEventsRegOnOLEDragDrop (CAObjHandle serverObject,
                                                            DAnimationEventsRegOnOLEDragDrop_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId);

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnChange (CAObjHandle serverObject,
                                                    DUpDownEventsRegOnChange_CallbackType callbackFunction,
                                                    void *callbackData,
                                                    int enableCallbacks,
                                                    int *callbackId);

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnDownClick (CAObjHandle serverObject,
                                                       DUpDownEventsRegOnDownClick_CallbackType callbackFunction,
                                                       void *callbackData,
                                                       int enableCallbacks,
                                                       int *callbackId);

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnUpClick (CAObjHandle serverObject,
                                                     DUpDownEventsRegOnUpClick_CallbackType callbackFunction,
                                                     void *callbackData,
                                                     int enableCallbacks,
                                                     int *callbackId);

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnMouseDown (CAObjHandle serverObject,
                                                       DUpDownEventsRegOnMouseDown_CallbackType callbackFunction,
                                                       void *callbackData,
                                                       int enableCallbacks,
                                                       int *callbackId);

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnMouseMove (CAObjHandle serverObject,
                                                       DUpDownEventsRegOnMouseMove_CallbackType callbackFunction,
                                                       void *callbackData,
                                                       int enableCallbacks,
                                                       int *callbackId);

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnMouseUp (CAObjHandle serverObject,
                                                     DUpDownEventsRegOnMouseUp_CallbackType callbackFunction,
                                                     void *callbackData,
                                                     int enableCallbacks,
                                                     int *callbackId);

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnOLEStartDrag (CAObjHandle serverObject,
                                                          DUpDownEventsRegOnOLEStartDrag_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId);

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnOLEGiveFeedback (CAObjHandle serverObject,
                                                             DUpDownEventsRegOnOLEGiveFeedback_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId);

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnOLESetData (CAObjHandle serverObject,
                                                        DUpDownEventsRegOnOLESetData_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId);

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnOLECompleteDrag (CAObjHandle serverObject,
                                                             DUpDownEventsRegOnOLECompleteDrag_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId);

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnOLEDragOver (CAObjHandle serverObject,
                                                         DUpDownEventsRegOnOLEDragOver_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId);

HRESULT CVIFUNC MSComCtl2_DUpDownEventsRegOnOLEDragDrop (CAObjHandle serverObject,
                                                         DUpDownEventsRegOnOLEDragDrop_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnDateClick (CAObjHandle serverObject,
                                                          DMonthViewEventsRegOnDateClick_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnDateDblClick (CAObjHandle serverObject,
                                                             DMonthViewEventsRegOnDateDblClick_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnGetDayBold (CAObjHandle serverObject,
                                                           DMonthViewEventsRegOnGetDayBold_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnSelChange (CAObjHandle serverObject,
                                                          DMonthViewEventsRegOnSelChange_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnClick (CAObjHandle serverObject,
                                                      DMonthViewEventsRegOnClick_CallbackType callbackFunction,
                                                      void *callbackData,
                                                      int enableCallbacks,
                                                      int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnDblClick (CAObjHandle serverObject,
                                                         DMonthViewEventsRegOnDblClick_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnKeyDown (CAObjHandle serverObject,
                                                        DMonthViewEventsRegOnKeyDown_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnKeyUp (CAObjHandle serverObject,
                                                      DMonthViewEventsRegOnKeyUp_CallbackType callbackFunction,
                                                      void *callbackData,
                                                      int enableCallbacks,
                                                      int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnKeyPress (CAObjHandle serverObject,
                                                         DMonthViewEventsRegOnKeyPress_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnMouseDown (CAObjHandle serverObject,
                                                          DMonthViewEventsRegOnMouseDown_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnMouseMove (CAObjHandle serverObject,
                                                          DMonthViewEventsRegOnMouseMove_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnMouseUp (CAObjHandle serverObject,
                                                        DMonthViewEventsRegOnMouseUp_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnOLEStartDrag (CAObjHandle serverObject,
                                                             DMonthViewEventsRegOnOLEStartDrag_CallbackType callbackFunction,
                                                             void *callbackData,
                                                             int enableCallbacks,
                                                             int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnOLEGiveFeedback (CAObjHandle serverObject,
                                                                DMonthViewEventsRegOnOLEGiveFeedback_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnOLESetData (CAObjHandle serverObject,
                                                           DMonthViewEventsRegOnOLESetData_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnOLECompleteDrag (CAObjHandle serverObject,
                                                                DMonthViewEventsRegOnOLECompleteDrag_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnOLEDragOver (CAObjHandle serverObject,
                                                            DMonthViewEventsRegOnOLEDragOver_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId);

HRESULT CVIFUNC MSComCtl2_DMonthViewEventsRegOnOLEDragDrop (CAObjHandle serverObject,
                                                            DMonthViewEventsRegOnOLEDragDrop_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnCallbackKeyDown (CAObjHandle serverObject,
                                                               DDTPickerEventsRegOnCallbackKeyDown_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnChange (CAObjHandle serverObject,
                                                      DDTPickerEventsRegOnChange_CallbackType callbackFunction,
                                                      void *callbackData,
                                                      int enableCallbacks,
                                                      int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnCloseUp (CAObjHandle serverObject,
                                                       DDTPickerEventsRegOnCloseUp_CallbackType callbackFunction,
                                                       void *callbackData,
                                                       int enableCallbacks,
                                                       int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnDropDown (CAObjHandle serverObject,
                                                        DDTPickerEventsRegOnDropDown_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnFormat (CAObjHandle serverObject,
                                                      DDTPickerEventsRegOnFormat_CallbackType callbackFunction,
                                                      void *callbackData,
                                                      int enableCallbacks,
                                                      int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnFormatSize (CAObjHandle serverObject,
                                                          DDTPickerEventsRegOnFormatSize_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnClick (CAObjHandle serverObject,
                                                     DDTPickerEventsRegOnClick_CallbackType callbackFunction,
                                                     void *callbackData,
                                                     int enableCallbacks,
                                                     int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnDblClick (CAObjHandle serverObject,
                                                        DDTPickerEventsRegOnDblClick_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnKeyDown (CAObjHandle serverObject,
                                                       DDTPickerEventsRegOnKeyDown_CallbackType callbackFunction,
                                                       void *callbackData,
                                                       int enableCallbacks,
                                                       int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnKeyUp (CAObjHandle serverObject,
                                                     DDTPickerEventsRegOnKeyUp_CallbackType callbackFunction,
                                                     void *callbackData,
                                                     int enableCallbacks,
                                                     int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnKeyPress (CAObjHandle serverObject,
                                                        DDTPickerEventsRegOnKeyPress_CallbackType callbackFunction,
                                                        void *callbackData,
                                                        int enableCallbacks,
                                                        int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnMouseDown (CAObjHandle serverObject,
                                                         DDTPickerEventsRegOnMouseDown_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnMouseMove (CAObjHandle serverObject,
                                                         DDTPickerEventsRegOnMouseMove_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnMouseUp (CAObjHandle serverObject,
                                                       DDTPickerEventsRegOnMouseUp_CallbackType callbackFunction,
                                                       void *callbackData,
                                                       int enableCallbacks,
                                                       int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnOLEStartDrag (CAObjHandle serverObject,
                                                            DDTPickerEventsRegOnOLEStartDrag_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnOLEGiveFeedback (CAObjHandle serverObject,
                                                               DDTPickerEventsRegOnOLEGiveFeedback_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnOLESetData (CAObjHandle serverObject,
                                                          DDTPickerEventsRegOnOLESetData_CallbackType callbackFunction,
                                                          void *callbackData,
                                                          int enableCallbacks,
                                                          int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnOLECompleteDrag (CAObjHandle serverObject,
                                                               DDTPickerEventsRegOnOLECompleteDrag_CallbackType callbackFunction,
                                                               void *callbackData,
                                                               int enableCallbacks,
                                                               int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnOLEDragOver (CAObjHandle serverObject,
                                                           DDTPickerEventsRegOnOLEDragOver_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId);

HRESULT CVIFUNC MSComCtl2_DDTPickerEventsRegOnOLEDragDrop (CAObjHandle serverObject,
                                                           DDTPickerEventsRegOnOLEDragDrop_CallbackType callbackFunction,
                                                           void *callbackData,
                                                           int enableCallbacks,
                                                           int *callbackId);

HRESULT CVIFUNC MSComCtl2_DFlatSBEventsRegOnChange (CAObjHandle serverObject,
                                                    DFlatSBEventsRegOnChange_CallbackType callbackFunction,
                                                    void *callbackData,
                                                    int enableCallbacks,
                                                    int *callbackId);

HRESULT CVIFUNC MSComCtl2_DFlatSBEventsRegOnScroll (CAObjHandle serverObject,
                                                    DFlatSBEventsRegOnScroll_CallbackType callbackFunction,
                                                    void *callbackData,
                                                    int enableCallbacks,
                                                    int *callbackId);
#ifdef __cplusplus
    }
#endif
#endif /* _MSCOMCTL2_H */
