/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       userint.h                                                     */
/* Purpose:     Include file for LabWindows/CVI User Interface Library        */
/*                                                                            */
/*============================================================================*/

#ifndef USERINT_HEADER
#define USERINT_HEADER

#include "cvidef.h"
#include "cvirte.h"

#ifdef __cplusplus
    extern "C" {
#endif

#if defined(_NI_mswin16_)
#define MAX_PATHNAME_LEN   80    /* includes nul byte */
#define MAX_DRIVENAME_LEN   3    /* includes nul byte */
#define MAX_DIRNAME_LEN    66    /* includes nul byte */
#define MAX_FILENAME_LEN   13    /* includes nul byte */

#elif defined(_NI_mswin32_)
#define MAX_PATHNAME_LEN  260    /* includes nul byte */
#define MAX_DRIVENAME_LEN   3    /* includes nul byte */
#define MAX_DIRNAME_LEN   256    /* includes nul byte */
#define MAX_FILENAME_LEN  256    /* includes nul byte */

#elif defined(_NI_linux_)
#define MAX_PATHNAME_LEN 1024    /* recommended buffer size for paths */
#define MAX_DRIVENAME_LEN   1    /* includes nul byte */
#define MAX_DIRNAME_LEN   256    /* includes nul byte */
#define MAX_FILENAME_LEN  256    /* recommended buffer size for file names */

#elif defined(_NI_unix_) || defined(_NI_sparc_)
#define MAX_PATHNAME_LEN  256    /* recommended buffer size for paths */
#define MAX_DRIVENAME_LEN   1    /* includes nul byte */
#define MAX_DIRNAME_LEN   256    /* includes nul byte */
#define MAX_FILENAME_LEN  256    /* recommended buffer size for file names */

#else
#error Undefined Platform. You need to add one of the
#error following to your compiler defines:
#error     Platform                      Preprocessor directive
#error Microsoft Windows 3.1           #define _NI_mswin16_
#error Windows 95/NT                   #define _NI_mswin32_
#error Solaris 1                       #define _NI_sparc_       1
#error Solaris 2                       #define _NI_sparc_       2
#error HP-UX                           #define _NI_hpux_
#error
#error _NI_i386_ has been replaced with _NI_mswin16_.
#error See Programmers Reference Manual for more information.

#endif

#define MAX_TREE_ITEM_TAG_LEN   32      /* includes nul byte */


/* */
/* CONTROL STYLES: (for use in the NewCtrl() function)  */
/* */


    /*** numerics: ***/
#define CTRL_NUMERIC                    100
#define CTRL_NUMERIC_THERMOMETER        101
#define CTRL_NUMERIC_TANK               102
#define CTRL_NUMERIC_GAUGE              103
#define CTRL_NUMERIC_METER              104
#define CTRL_NUMERIC_KNOB               105
#define CTRL_NUMERIC_DIAL               106
#define CTRL_NUMERIC_VSLIDE             107
#define CTRL_NUMERIC_HSLIDE             108
#define CTRL_NUMERIC_FLAT_VSLIDE        109
#define CTRL_NUMERIC_FLAT_HSLIDE        110
#define CTRL_NUMERIC_LEVEL_VSLIDE       111
#define CTRL_NUMERIC_LEVEL_HSLIDE       112
#define CTRL_NUMERIC_POINTER_VSLIDE     113
#define CTRL_NUMERIC_POINTER_HSLIDE     114
#define CTRL_NUMERIC_LS                 115
#define CTRL_NUMERIC_THERMOMETER_LS     116
#define CTRL_NUMERIC_TANK_LS            117
#define CTRL_NUMERIC_GAUGE_LS           118
#define CTRL_NUMERIC_METER_LS           119
#define CTRL_NUMERIC_KNOB_LS            120
#define CTRL_NUMERIC_DIAL_LS            121
#define CTRL_NUMERIC_LEVEL_VSLIDE_LS    126
#define CTRL_NUMERIC_LEVEL_HSLIDE_LS    127
#define CTRL_NUMERIC_POINTER_VSLIDE_LS  128
#define CTRL_NUMERIC_POINTER_HSLIDE_LS  129

#define CTRL_COLOR_NUMERIC              130
#define CTRL_COLOR_NUMERIC_LS           131

#define CTRL_STRING                     150
#define CTRL_STRING_LS                  151

#define CTRL_TEXT_MSG                   160

#define CTRL_TEXT_BOX                   170
#define CTRL_TEXT_BOX_LS                171

    /*** command buttons ***/
#define CTRL_SQUARE_COMMAND_BUTTON      200
#define CTRL_OBLONG_COMMAND_BUTTON      201
#define CTRL_ROUND_COMMAND_BUTTON       202
#define CTRL_ROUNDED_COMMAND_BUTTON     203
#define CTRL_PICTURE_COMMAND_BUTTON     204
#define CTRL_SQUARE_COMMAND_BUTTON_LS   205
#define CTRL_PICTURE_COMMAND_BUTTON_LS  206

    /*** buttons ***/
#define CTRL_ROUND_BUTTON               220
#define CTRL_SQUARE_BUTTON              221
#define CTRL_ROUND_FLAT_BUTTON          222
#define CTRL_SQUARE_FLAT_BUTTON         223
#define CTRL_ROUND_RADIO_BUTTON         224
#define CTRL_SQUARE_RADIO_BUTTON        225
#define CTRL_CHECK_BOX                  226
#define CTRL_ROUND_PUSH_BUTTON          227
#define CTRL_SQUARE_PUSH_BUTTON         228
#define CTRL_ROUND_PUSH_BUTTON2         229
#define CTRL_SQUARE_PUSH_BUTTON2        230
#define CTRL_SQUARE_TEXT_BUTTON         231
#define CTRL_OBLONG_TEXT_BUTTON         232
#define CTRL_ROUND_TEXT_BUTTON          233
#define CTRL_ROUNDED_TEXT_BUTTON        234
#define CTRL_PICTURE_TOGGLE_BUTTON      235
#define CTRL_SQUARE_BUTTON_LS           240
#define CTRL_PICTURE_TOGGLE_BUTTON_LS   241
#define CTRL_SQUARE_PUSH_BUTTON_LS      242
#define CTRL_SQUARE_TEXT_BUTTON_LS      243

    /*** LED's ***/
#define CTRL_ROUND_LIGHT                260
#define CTRL_SQUARE_LIGHT               261
#define CTRL_ROUND_LED                  262
#define CTRL_SQUARE_LED                 263
#define CTRL_ROUND_LED_LS               264
#define CTRL_SQUARE_LED_LS              265

    /*** binary switches ***/
#define CTRL_HSWITCH                    280
#define CTRL_VSWITCH                    281
#define CTRL_GROOVED_HSWITCH            282
#define CTRL_GROOVED_VSWITCH            283
#define CTRL_TOGGLE_HSWITCH             284
#define CTRL_TOGGLE_VSWITCH             285
#define CTRL_TOGGLE_HSWITCH_LS          288
#define CTRL_TOGGLE_VSWITCH_LS          289

    /*** rings ***/
#define CTRL_RING                       300
#define CTRL_RECESSED_MENU_RING         301
#define CTRL_MENU_RING                  302
#define CTRL_POPUP_MENU_RING            303
#define CTRL_RING_VSLIDE                304
#define CTRL_RING_HSLIDE                305
#define CTRL_RING_FLAT_VSLIDE           306
#define CTRL_RING_FLAT_HSLIDE           307
#define CTRL_RING_LEVEL_VSLIDE          308
#define CTRL_RING_LEVEL_HSLIDE          309
#define CTRL_RING_POINTER_VSLIDE        310
#define CTRL_RING_POINTER_HSLIDE        311
#define CTRL_RING_THERMOMETER           312
#define CTRL_RING_TANK                  313
#define CTRL_RING_GAUGE                 314
#define CTRL_RING_METER                 315
#define CTRL_RING_KNOB                  316
#define CTRL_RING_DIAL                  317
#define CTRL_PICTURE_RING               318
#define CTRL_RING_LS                    319
#define CTRL_RECESSED_MENU_RING_LS      320
#define CTRL_MENU_RING_LS               321
#define CTRL_POPUP_MENU_RING_LS         322
#define CTRL_RING_LEVEL_VSLIDE_LS       327
#define CTRL_RING_LEVEL_HSLIDE_LS       328
#define CTRL_RING_POINTER_VSLIDE_LS     329
#define CTRL_RING_POINTER_HSLIDE_LS     330
#define CTRL_RING_THERMOMETER_LS        331
#define CTRL_RING_TANK_LS               332
#define CTRL_RING_GAUGE_LS              333
#define CTRL_RING_METER_LS              334
#define CTRL_RING_KNOB_LS               335
#define CTRL_RING_DIAL_LS               336
#define CTRL_PICTURE_RING_LS            337

#define CTRL_LIST                       340
#define CTRL_LIST_LS                    341

    /*** decorations */
#define CTRL_RAISED_BOX                 380
#define CTRL_RECESSED_BOX               381
#define CTRL_FLAT_BOX                   382
#define CTRL_RAISED_CIRCLE              383
#define CTRL_RECESSED_CIRCLE            384
#define CTRL_FLAT_CIRCLE                385
#define CTRL_RAISED_FRAME               386
#define CTRL_RECESSED_FRAME             387
#define CTRL_FLAT_FRAME                 388
#define CTRL_RAISED_ROUND_FRAME         389
#define CTRL_RECESSED_ROUND_FRAME       390
#define CTRL_FLAT_ROUND_FRAME           391
#define CTRL_RAISED_ROUNDED_BOX         392
#define CTRL_RECESSED_ROUNDED_BOX       393
#define CTRL_FLAT_ROUNDED_BOX           394
#define CTRL_RAISED_BOX_LS              395
#define CTRL_RECESSED_BOX_LS            396
#define CTRL_SMOOTH_VERTICAL_BOX_LS     397
#define CTRL_SMOOTH_HORIZONTAL_BOX_LS   398
#define CTRL_RECESSED_NARROW_FRAME      410

#define CTRL_GRAPH                      440
#define CTRL_GRAPH_LS                   441
#define CTRL_DIGITAL_GRAPH              450
#define CTRL_DIGITAL_GRAPH_LS           451
#define CTRL_STRIP_CHART                460
#define CTRL_STRIP_CHART_LS             461

#define CTRL_PICTURE                    480
#define CTRL_PICTURE_LS                 481

#define CTRL_TIMER                      490

#define CTRL_CANVAS                     500

#define CTRL_ACTIVEX                    503

#define CTRL_TABLE                      510
#define CTRL_TABLE_LS                   511

#define CTRL_TREE                       512
#define CTRL_TREE_LS                    513

#define CTRL_HORIZONTAL_SPLITTER        520
#define CTRL_VERTICAL_SPLITTER          521
#define CTRL_HORIZONTAL_SPLITTER_LS     522
#define CTRL_VERTICAL_SPLITTER_LS       523

#define CTRL_TABS                       540

/* */
/* ATTRIBUTES: (for use in the set and get attribute functions) */
/* Note that not all attributes apply to all objects. */
/* To find out which attributes are valid for a given object, */
/* open the function panel for the Set... or Get... function */
/* that corresponds to that object, and then click on its */
/* attribute selection control */
/* */

#define ATTR_DIMMED                     500         /* int (boolean) */
#define ATTR_CONSTANT_NAME              501         /* (char *) (not settable) */
#define ATTR_CONSTANT_NAME_LENGTH       502         /* int (not settable) */
#define ATTR_CALLBACK_DATA              510         /* (void *) */
#define ATTR_CALLBACK_NAME              511         /* (char *) (not settable) */
#define ATTR_CALLBACK_NAME_LENGTH       512         /* int (not settable) */
#define ATTR_CALLBACK_FUNCTION_POINTER  513         /* (void *) */
#define ATTR_VISIBLE                    530         /* int (boolean) */
#define ATTR_LEFT                       531         /* int (-32768 - 32767) */
#define ATTR_TOP                        532         /* int (-32768 - 32767) */
#define ATTR_WIDTH                      533         /* int (0-32767) */
#define ATTR_HEIGHT                     540         /* int (0-32767) */
#define ATTR_FRAME_COLOR                550         /* int (rgb value) */
#define ATTR_SCROLL_BARS                551         /* int: VAL_NO_SCROLL_BARS, ... */
#define ATTR_SCROLL_BAR_COLOR           552         /* int (rgb value) */
#define ATTR_HSCROLL_OFFSET             560         /* int (in pixels) */
#define ATTR_HSCROLL_OFFSET_MAX         561         /* int (in pixels) */
#define ATTR_BACKCOLOR                  570         /* int (rgb value) */
#define ATTR_TITLEBAR_VISIBLE           571         /* int (boolean) */
#define ATTR_TITLEBAR_THICKNESS         572         /* int */
#define ATTR_TITLE                      573         /* (char *) */
#define ATTR_TITLE_LENGTH               574         /* int (not settable) */
#define ATTR_TITLE_COLOR                575         /* int (rgb value) */
#define ATTR_TITLE_BACKCOLOR            576         /* int (rgb value) */
#define ATTR_FRAME_STYLE                577         /* int: VAL_OUTLINED_FRAME, ... */
#define ATTR_FRAME_THICKNESS            578         /* int */
#define ATTR_MENU_HEIGHT                579         /* int */
#define ATTR_SIZABLE                    580         /* int (boolean) */
#define ATTR_MOVABLE                    581         /* int (boolean) */
#define ATTR_NUM_CTRLS                  582         /* int (not settable) */
#define ATTR_MOUSE_CURSOR               583         /* int: VAL_DEFAULT_CURSOR, ... */
#define ATTR_TITLE_FONT                 584         /* (char *) */
#define ATTR_TITLE_ITALIC               585         /* int (boolean) */
#define ATTR_TITLE_UNDERLINE            586         /* int (boolean) */
#define ATTR_TITLE_STRIKEOUT            587         /* int (boolean) */
#define ATTR_TITLE_POINT_SIZE           588         /* int (1-255) */
#define ATTR_TITLE_BOLD                 589         /* int (boolean) */
#define ATTR_TITLE_FONT_NAME_LENGTH     590         /* int (not settable) */
#define ATTR_MENU_BAR_VISIBLE           591         /* int (boolean) */
#define ATTR_CLOSE_CTRL                 592         /* int (ctrl Id) */
#define ATTR_VSCROLL_OFFSET             593         /* int (in pixels) */
#define ATTR_VSCROLL_OFFSET_MAX         594         /* int (in pixels) */
#define ATTR_PARENT_SHARES_SHORTCUT_KEYS 595        /* int (boolean) */
#define ATTR_ACTIVATE_WHEN_CLICKED_ON   596         /* int (boolean) */
#define ATTR_WINDOW_ZOOM                597         /* int: VAL_MINIMIZE, ... */
#define ATTR_SYSTEM_WINDOW_HANDLE       598         /* int (HWND, or X-Window) */
#define ATTR_CAN_MINIMIZE               599         /* int (boolean) */
#define ATTR_CAN_MAXIMIZE               600         /* int (boolean) */
#define ATTR_CLOSE_ITEM_VISIBLE         601         /* int (boolean) */
#define ATTR_SYSTEM_MENU_VISIBLE        602         /* int (boolean) */
#define ATTR_PANEL_MENU_BAR_CONSTANT    603         /* (char *) */
#define ATTR_PANEL_MENU_BAR_CONSTANT_LENGTH 604     /* int (not settable) */
#define ATTR_TITLE_SIZE_TO_FONT         605         /* int (boolean) */
#define ATTR_PANEL_PARENT               606         /* int (not settable) */
#define ATTR_NUM_CHILDREN               607         /* int (not settable) */
#define ATTR_FIRST_CHILD                608         /* int (not settable) */
#define ATTR_NEXT_PANEL                 609         /* int (not settable) */
#define ATTR_ZPLANE_POSITION            610         /* int (0 - numObjects-1) */
#define ATTR_CTRL_STYLE                 611         /* int: CTRL_NUMERIC, ... (not settable) */
#define ATTR_CTRL_TAB_POSITION          612         /* int */
#define ATTR_PANEL_FIRST_CTRL           612         /* int (not settable) */
#define ATTR_NEXT_CTRL                  613         /* int (not settable) */
#define ATTR_ACTIVE                     614         /* int (not settable) */
#define ATTR_FLOATING                   615         /* int VAL_FLOAT_NEVER, ... */
#define ATTR_TL_ACTIVATE_WHEN_CLICKED_ON 616        /* int (boolean) */
#define ATTR_CONFORM_TO_SYSTEM          617         /* int (boolean) */
#define ATTR_SCALE_CONTENTS_ON_RESIZE   618         /* int (boolean) */
#define ATTR_MIN_HEIGHT_FOR_SCALING     619         /* int (0-32767) */
#define ATTR_CTRL_MODE                  620         /* int: VAL_HOT, ... */
#define ATTR_MIN_WIDTH_FOR_SCALING      621         /* int (0-32767) */
#define ATTR_HAS_TASKBAR_BUTTON         622         /* int (boolean) */
#define ATTR_OWNER_THREAD_ID            623         /* int (not settable) */
#define ATTR_SCROLL_BAR_STYLE           624         /* int: VAL_CLASSIC, ... */
#define ATTR_CTRL_VAL                   630         /* same datatype as ctrl */
#define ATTR_LABEL_COLOR                640         /* int (rgb value) */
#define ATTR_LABEL_TEXT                 641         /* (char *) */
#define ATTR_LABEL_FONT                 642         /* (char *) */
#define ATTR_LABEL_ITALIC               643         /* int (boolean) */
#define ATTR_LABEL_UNDERLINE            644         /* int (boolean) */
#define ATTR_LABEL_STRIKEOUT            645         /* int (boolean) */
#define ATTR_LABEL_POINT_SIZE           646         /* int (1-255) */
#define ATTR_LABEL_BOLD                 647         /* int (boolean) */
#define ATTR_LABEL_TEXT_LENGTH          648         /* int (not settable) */
#define ATTR_LABEL_SIZE_TO_TEXT         649         /* int (boolean) */
#define ATTR_LABEL_FONT_NAME_LENGTH     650         /* int (not settable) */
#define ATTR_LABEL_VISIBLE              660         /* int (boolean) */
#define ATTR_LABEL_LEFT                 670         /* int (-32768 - 32767) */
#define ATTR_LABEL_TOP                  671         /* int (-32768 - 32767) */
#define ATTR_LABEL_WIDTH                672         /* int (0-32767) */
#define ATTR_LABEL_HEIGHT               673         /* int (0-32767) */
#define ATTR_LABEL_BGCOLOR              674         /* int (rgb value) */
#define ATTR_LABEL_JUSTIFY              675         /* int: VAL_LEFT_JUSTIFIED, ... */
#define ATTR_LABEL_RAISED               676         /* int (boolean) */
#define ATTR_TEXT_COLOR                 680         /* int (rgb value) */
#define ATTR_TEXT_FONT_NAME_LENGTH      681         /* int (not settable) */
#define ATTR_TEXT_FONT                  682         /* (char *) */
#define ATTR_TEXT_ITALIC                683         /* int (boolean) */
#define ATTR_TEXT_UNDERLINE             684         /* int (boolean) */
#define ATTR_TEXT_STRIKEOUT             685         /* int (boolean) */
#define ATTR_TEXT_POINT_SIZE            686         /* int (1-255) */
#define ATTR_TEXT_BOLD                  687         /* int (boolean) */
#define ATTR_TEXT_BGCOLOR               690         /* int (rgb value) */
#define ATTR_TEXT_JUSTIFY               700         /* int: VAL_LEFT_JUSTIFIED, ... */
#define ATTR_DATA_TYPE                  710         /* int: VAL_CHAR, ... */
#define ATTR_CTRL_INDEX                 720         /* int: (0 to numItems-1) */
#define ATTR_DFLT_INDEX                 721         /* int: (0 to numItems-1) */
#define ATTR_MAX_VALUE                  730         /* same datatype as ctrl */
#define ATTR_MIN_VALUE                  731         /* same datatype as ctrl */
#define ATTR_DFLT_VALUE                 732         /* same datatype as ctrl */
#define ATTR_INCR_VALUE                 733         /* same datatype as ctrl */
#define ATTR_FORMAT                     734         /* int: VAL_HEX_FORMAT, ... */
#define ATTR_PRECISION                  735         /* int (0 - 15) */
#define ATTR_SHOW_RADIX                 736         /* int (boolean) */
#define ATTR_SHOW_INCDEC_ARROWS         737         /* int (boolean) */
#define ATTR_CHECK_RANGE                738         /* int: VAL_COERCE, ... */
#define ATTR_PADDING                    739         /* int: (0 - 64) */
#define ATTR_DFLT_VALUE_LENGTH          740         /* int (not settable) */
#define ATTR_DISABLE_CHECK_MARK         755         /* int (boolean) */
#define ATTR_MAX_ENTRY_LENGTH           760         /* int (-1 means no limit) */
#define ATTR_MAX_ENTRY_CHARS            761         /* int (-1 means no limit) */
#define ATTR_TEXT_SELECTION_START       762         /* int */
#define ATTR_TEXT_SELECTION_LENGTH      763         /* int */
#define ATTR_STRING_TEXT_LENGTH         770         /* int (not settable) */
#define ATTR_FIRST_VISIBLE_LINE         790         /* int */
#define ATTR_WRAP_MODE                  791         /* int: VAL_CHAR_WRAP, ... */
#define ATTR_EXTRA_LINES                793         /* int (-1 means no limit) */
#define ATTR_TOTAL_LINES                794         /* int (not settable) */
#define ATTR_ENTER_IS_NEWLINE           795         /* int (boolean) */
#define ATTR_SCROLL_BAR_SIZE            820         /* int: VAL_SMALL_SCROLL_BARS, ... (not settable for panels ) */
#define ATTR_VISIBLE_LINES              821         /* int (0 - 500) */
#define ATTR_NO_EDIT_TEXT               840         /* int (boolean) */
#define ATTR_TEXT_RAISED                860         /* int (boolean) */
#define ATTR_SIZE_TO_TEXT               861         /* int (boolean) */
#define ATTR_CMD_BUTTON_COLOR           880         /* int (rgb value) */
#define ATTR_AUTO_SIZING                881         /* int: VAL_ALWAYS_AUTO_SIZE, ... */
#define ATTR_ON_VALUE                   900         /* same datatype as ctrl */
#define ATTR_OFF_VALUE                  901         /* same datatype as ctrl */
#define ATTR_ON_VALUE_LENGTH            902         /* int (only if string value) (not settable) */
#define ATTR_OFF_VALUE_LENGTH           903         /* int (only if string value) (not settable) */
#define ATTR_BINARY_SWITCH_COLOR        904         /* int (rgb value) */
#define ATTR_ON_COLOR                   920         /* int (rgb value) */
#define ATTR_OFF_COLOR                  921         /* int (rgb value) */
#define ATTR_ON_TEXT                    940         /* (char *) */
#define ATTR_OFF_TEXT                   941         /* (char *) */
#define ATTR_ON_TEXT_LENGTH             942         /* int (not settable) */
#define ATTR_OFF_TEXT_LENGTH            943         /* int (not settable) */
#define ATTR_DIG_DISP_TOP               970         /* int (-32768 - 32767) */
#define ATTR_DIG_DISP_LEFT              971         /* int (-32768 - 32767) */
#define ATTR_DIG_DISP_WIDTH             972         /* int (0-32767) */
#define ATTR_DIG_DISP_HEIGHT            973         /* int (0-32767) */
#define ATTR_SHOW_DIG_DISP              974         /* int (boolean) */
#define ATTR_SLIDER_COLOR               980         /* int (rgb value) */
#define ATTR_NEEDLE_COLOR               ATTR_SLIDER_COLOR
#define ATTR_FILL_HOUSING_COLOR         981         /* int (rgb value) */
#define ATTR_MARKER_STYLE               982         /* int: VAL_NO_MARKERS, ... */
#define ATTR_TICK_STYLE                 983         /* int: VAL_NO_TICKS, ... */
#define ATTR_FILL_COLOR                 985         /* int (rgb value) */
#define ATTR_FILL_OPTION                986         /* int: VAL_NO_FILL, ... */
#define ATTR_MARKER_START_ANGLE         990         /* int (0-359) */
#define ATTR_MARKER_END_ANGLE           991         /* int (0-359) */
#define ATTR_SLIDER_WIDTH               992         /* int (0-32767) */
#define ATTR_SLIDER_HEIGHT              993         /* int (0-32767) */
#define ATTR_SHOW_MORE_BUTTON           995         /* int (boolean) */
#define ATTR_SHOW_TRANSPARENT           996         /* int (boolean) */
#define ATTR_SLIDER_LEFT                997         /* int (0-32767) */
#define ATTR_SLIDER_TOP                 998         /* int (0-32767) */
#define ATTR_NUM_DIVISIONS              999         /* int (1-100, or VAL_AUTO) */
#define ATTR_MENU_ARROW_COLOR           1000        /* int (rgb value) */
#define ATTR_MENU_BAR_POINT_SIZE        1010        /* int (1-255) */
#define ATTR_MENU_BAR_BOLD              1011        /* int (boolean) */
#define ATTR_MENU_BAR_ITALIC            1012        /* int (boolean) */
#define ATTR_MENU_BAR_UNDERLINE         1013        /* int (boolean) */
#define ATTR_MENU_BAR_STRIKEOUT         1014        /* int (boolean) */
#define ATTR_MENU_BAR_FONT              1015        /* (char *) */
#define ATTR_MENU_BAR_FONT_NAME_LENGTH  1016        /* int (not settable) */
#define ATTR_MENU_IMAGE_BACKGROUND_COLOR    1017    /* int (rgb value) */
#define ATTR_MENU_BAR_IMAGE_SIZE        1018        /* int (not settable) */
#define ATTR_SHORTCUT_KEY               1020        /* int */
#define ATTR_CHECKED                    1040        /* int (boolean) */
#define ATTR_IS_SEPARATOR               1041        /* int (boolean) (not settable) */
#define ATTR_ITEM_NAME                  1042        /* (char *) */
#define ATTR_ITEM_NAME_LENGTH           1043        /* int (not settable) */
#define ATTR_SUBMENU_ID                 1044        /* int (not settable) */
#define ATTR_NEXT_ITEM_ID               1045        /* int (not settable) */
#define ATTR_BOLD                       1046        /* int (boolean) */
#define ATTR_IMAGE_FILE                 1050        /* (char *) */
#define ATTR_IMAGE_FILE_LENGTH          1051        /* int (not settable) */
#define ATTR_FAST_DRAW_BUTTON           1052        /* int (boolean) */
#define ATTR_USE_SUBIMAGE               1053        /* int (boolean) */
#define ATTR_SUBIMAGE_TOP               1054        /* int (positive or zero) */
#define ATTR_SUBIMAGE_LEFT              1055        /* int (positive or zero) */
#define ATTR_SUBIMAGE_WIDTH             1056        /* int (positive or zero) */
#define ATTR_SUBIMAGE_HEIGHT            1057        /* int (positive or zero) */
#define ATTR_MENU_NAME                  1060        /* (char *) */
#define ATTR_MENU_NAME_LENGTH           1061        /* int (not settable) */
#define ATTR_NUM_MENU_ITEMS             1062        /* int (not settable) */
#define ATTR_NEXT_MENU_ID               1063        /* int (not settable) */
#define ATTR_FIRST_ITEM_ID              1064        /* int (not settable) */
#define ATTR_NUM_MENUS                  1070        /* int (not settable) */
#define ATTR_DRAW_LIGHT_BEVEL           1071        /* int (boolean) */
#define ATTR_DIMMER_CALLBACK            1072        /* (int) (really a MenuDimmerCallbackPtr) */
#define ATTR_FIRST_MENU_ID              1073        /* int (not settable) */
#define ATTR_SEND_DIMMER_EVENTS_FOR_ALL_KEYS 1074   /* int (boolean) */
#define ATTR_GRID_COLOR                 1080        /* int (rgb value) */
#define ATTR_PLOT_BGCOLOR               1081        /* int (rgb value) */
#define ATTR_XYNAME_FONT                1082        /* (char *) */
#define ATTR_XYNAME_COLOR               1083        /* int (rgb value) */
#define ATTR_XYLABEL_FONT               1084        /* (char *) */
#define ATTR_XYLABEL_COLOR              1085        /* int (rgb value) */
#define ATTR_XNAME                      1086        /* (char *) */
#define ATTR_XGRID_VISIBLE              1087        /* int (boolean) */
#define ATTR_XLABEL_VISIBLE             1088        /* int (boolean) */
#define ATTR_XFORMAT                    1089        /* int: VAL_ENGINEERING... */
#define ATTR_XDIVISIONS                 1090        /* int (1-100, or VAL_AUTO) */
#define ATTR_XPRECISION                 1091        /* int (0-15, or VAL_AUTO) */
#define ATTR_XENG_UNITS                 1092        /* int (-308 to 308) */
#define ATTR_YNAME                      1093        /* (char *) */
#define ATTR_YGRID_VISIBLE              1094        /* int (boolean) */
#define ATTR_YLABEL_VISIBLE             1095        /* int (boolean) */
#define ATTR_YMAP_MODE                  1096        /* int: VAL_LINEAR or VAL_LOG */
#define ATTR_YFORMAT                    1097        /* int: VAL_ENGINEERING... */
#define ATTR_YDIVISIONS                 1098        /* int (1-100, or VAL_AUTO) */
#define ATTR_YPRECISION                 1099        /* int (0-15, or VAL_AUTO) */
#define ATTR_YENG_UNITS                 1100        /* int (-308 to 308) */
#define ATTR_EDGE_STYLE                 1101        /* int: VAL_RAISED_EDGE, ...*/
#define ATTR_BORDER_VISIBLE             1102        /* int (boolean) */
#define ATTR_XYNAME_BOLD                1103        /* int (boolean) */
#define ATTR_XYNAME_ITALIC              1104        /* int (boolean) */
#define ATTR_XYNAME_UNDERLINE           1105        /* int (boolean) */
#define ATTR_XYNAME_STRIKEOUT           1106        /* int (boolean) */
#define ATTR_XYNAME_POINT_SIZE          1107        /* int (1-255) */
#define ATTR_XNAME_LENGTH               1108        /* int (not settable) */
#define ATTR_YNAME_LENGTH               1109        /* int (not settable) */
#define ATTR_XYNAME_FONT_NAME_LENGTH    1110        /* int (not settable) */
#define ATTR_XYLABEL_BOLD               1111        /* int (boolean) */
#define ATTR_XYLABEL_ITALIC             1112        /* int (boolean) */
#define ATTR_XYLABEL_UNDERLINE          1113        /* int (boolean) */
#define ATTR_XYLABEL_STRIKEOUT          1114        /* int (boolean) */
#define ATTR_XYLABEL_POINT_SIZE         1115        /* int (1-255) */
#define ATTR_XYLABEL_FONT_NAME_LENGTH   1116        /* int (not settable) */
#define ATTR_GRAPH_BGCOLOR              1117        /* int (rgb value) */
#define ATTR_PLOT_AREA_WIDTH            1118        /* int (not settable) */
#define ATTR_PLOT_AREA_HEIGHT           1119        /* int (not settable) */
#define ATTR_INNER_MARKERS_VISIBLE      1120        /* int (boolean) */
#define ATTR_YREVERSE                   1121        /* int (boolean) */
#define ATTR_XUSE_LABEL_STRINGS         1122        /* int (boolean) */
#define ATTR_YUSE_LABEL_STRINGS         1123        /* int (boolean) */
#define ATTR_XAXIS_GAIN                 1124        /* double */
#define ATTR_YAXIS_GAIN                 1125        /* double */
#define ATTR_XAXIS_OFFSET               1126        /* double */
#define ATTR_YAXIS_OFFSET               1127        /* double */
#define ATTR_PLOT_AREA_TOP              1128        /* int (not settable) */
#define ATTR_PLOT_AREA_LEFT             1129        /* int (not settable) */
#define ATTR_XPADDING                   1130        /* int (0 to 64) */
#define ATTR_YPADDING                   1131        /* int (0 to 64) */
#define ATTR_ACTUAL_XDIVISIONS          1132        /* int (not settable) */
#define ATTR_ACTUAL_YDIVISIONS          1133        /* int (not settable) */
#define ATTR_ACTUAL_XPRECISION          1134        /* int (not settable) */
#define ATTR_ACTUAL_YPRECISION          1135        /* int (not settable) */
#define ATTR_XMINORGRID_VISIBLE         1136        /* int (boolean) */
#define ATTR_YMINORGRID_VISIBLE         1137        /* int (boolean) */
#define ATTR_NUM_CURSORS                1140        /* int (0-10) */
#define ATTR_XMAP_MODE                  1141        /* int: VAL_LINEAR or VAL_LOG */
#define ATTR_DATA_MODE                  1142        /* int: VAL_RETAIN, ... */
#define ATTR_COPY_ORIGINAL_DATA         1143        /* int (boolean) */
#define ATTR_XMARK_ORIGIN               1144        /* int (boolean) */
#define ATTR_YMARK_ORIGIN               1145        /* int (boolean) */
#define ATTR_SMOOTH_UPDATE              1146        /* int (boolean) */
#define ATTR_REFRESH_GRAPH              1147        /* int (boolean) */
#define ATTR_SHIFT_TEXT_PLOTS           1148        /* int (boolean) */
#define ATTR_ACTIVE_YAXIS               1149        /* int: VAL_LEFT_YAXIS or VAL_RIGHT_YAXIS */
#define ATTR_XREVERSE                   1150        /* int (boolean) */
#define ATTR_ENABLE_ZOOM_AND_PAN        1151        /* int (boolean) */
#define ATTR_XLOOSE_FIT_AUTOSCALING     1152        /* int (boolean) */
#define ATTR_YLOOSE_FIT_AUTOSCALING     1153        /* int (boolean) */
#define ATTR_XLOOSE_FIT_AUTOSCALING_UNIT 1154       /* int (-308 to 308) */
#define ATTR_YLOOSE_FIT_AUTOSCALING_UNIT 1155       /* int (-308 to 308) */
#define ATTR_ANTI_ALIASED_PLOTS         1156        /* int (boolean) */
#define ATTR_LEGEND_VISIBLE             1157        /* int (boolean) */
#define ATTR_LEGEND_TOP                 1158        /* int (-32768 - 32767) */
#define ATTR_LEGEND_LEFT                1159        /* int (-32768 - 32767) */
#define ATTR_LEGEND_WIDTH               1160        /* int (-32768 - 32767) */
#define ATTR_LEGEND_HEIGHT              1161        /* int (-32768 - 32767) */
#define ATTR_LEGEND_FRAME_COLOR         1162        /* int (rgb value) */
#define ATTR_LEGEND_PLOT_BGCOLOR        1163        /* int (rgb value) */
#define ATTR_LEGEND_SHOW_SAMPLES        1164        /* int (boolean) */
#define ATTR_LEGEND_AUTO_SIZE           1165        /* int (boolean) */
#define ATTR_LEGEND_AUTO_DISPLAY        1166        /* int (boolean) */
#define ATTR_LEGEND_NUM_VISIBLE_ITEMS   1167        /* int (0-500) */
#define ATTR_ACTIVE_XAXIS               1168        /* int: VAL_BOTTOM_XAXIS or VAL_TOP_XAXIS */
#define ATTR_LEGEND_INTERACTIVE         1169        /* int (boolean) */
#define ATTR_NUM_TRACES                 1170        /* int (1-64) */
#define ATTR_POINTS_PER_SCREEN          1171        /* int (3-10000) */
#define ATTR_SCROLL_MODE                1172        /* int: VAL_SWEEP, ... */
#define ATTR_CURSOR_MODE                1200        /* int: VAL_FREE_FORM, ... */
#define ATTR_CURSOR_POINT_STYLE         1201        /* int: VAL_EMPTY_SQUARE, ... */
#define ATTR_CROSS_HAIR_STYLE           1202        /* int: VAL_LONG_CROSS, ... */
#define ATTR_CURSOR_COLOR               1203        /* int (rgb value) */
#define ATTR_CURSOR_YAXIS               1204        /* int: VAL_LEFT_YAXIS or VAL_RIGHT_YAXIS */
#define ATTR_CURSOR_ENABLED             1205        /* int (boolean) */
#define ATTR_CURSOR_XAXIS               1206        /* int: VAL_BOTTOM_XAXIS or VAL_TOP_XAXIS */
#define ATTR_TRACE_COLOR                1230        /* int (rgb value) */
#define ATTR_PLOT_STYLE                 1231        /* int: VAL_THIN_LINE, ... */
#define ATTR_TRACE_POINT_STYLE          1232        /* int: VAL_EMPTY_SQUARE, ... */
#define ATTR_LINE_STYLE                 1233        /* int: VAL_SOLID, ... */
#define ATTR_TRACE_VISIBLE              1234        /* int (boolean) */
#define ATTR_TRACE_BGCOLOR              1240        /* int (rgb value) */
#define ATTR_PLOT_FONT                  1241        /* (char *) */
#define ATTR_PLOT_FONT_NAME_LENGTH      1242        /* int (not settable) */
#define ATTR_INTERPOLATE_PIXELS         1243        /* int (boolean) */
#define ATTR_PLOT_ZPLANE_POSITION       1244        /* int (0 - numPlots - 1) */
#define ATTR_NUM_POINTS                 1245        /* int (not settable) */
#define ATTR_PLOT_XDATA                 1246        /* (void *) (not settable) */
#define ATTR_PLOT_YDATA                 1247        /* (void *) (not settable) */
#define ATTR_PLOT_ZDATA                 1248        /* (void *) (not settable) */
#define ATTR_PLOT_XDATA_TYPE            1249        /* int (not settable) */
#define ATTR_PLOT_YDATA_TYPE            1250        /* int (not settable) */
#define ATTR_PLOT_ZDATA_TYPE            1251        /* int (not settable) */
#define ATTR_PLOT_XDATA_SIZE            1252        /* int (not settable) */
#define ATTR_PLOT_YDATA_SIZE            1253        /* int (not settable) */
#define ATTR_PLOT_ZDATA_SIZE            1254        /* int (not settable) */
#define ATTR_PLOT_YAXIS                 1255        /* int: VAL_LEFT_YAXIS or VAL_RIGHT_YAXIS */
#define ATTR_PLOT_SNAPPABLE             1256        /* int (boolean) */
#define ATTR_PLOT_ORIGIN                1257        /* int: VAL_LOWER_LEFT,... */
#define ATTR_PLOT_THICKNESS             1258        /* int: 1-32 (only for solid lines) */
#define ATTR_PLOT_XAXIS                 1259        /* int: VAL_BOTTOM_XAXIS or VAL_TOP_XAXIS */
#define ATTR_CHECK_MODE                 1260        /* int (boolean) */
#define ATTR_CHECK_STYLE                1261        /* int: VAL_CHECK_MARK, ... */
#define ATTR_TEXT_CLICK_TOGGLES_CHECK   1262        /* int: (boolean) */
#define ATTR_HILITE_CURRENT_ITEM        1263        /* int (boolean) */
#define ATTR_ALLOW_ROOM_FOR_IMAGES      1264        /* int (boolean) */
#define ATTR_DRAGGABLE_MARKS            1265        /* int (boolean) */
#define ATTR_INTERVAL                   1270        /* double (seconds) */
#define ATTR_ENABLED                    1271        /* int (boolean) */
#define ATTR_PLOT_LG_VISIBLE            1275        /* int (boolean) */
#define ATTR_PLOT_LG_TEXT               1276        /* (char *) */
#define ATTR_PLOT_LG_TEXT_LENGTH        1277        /* int (not settable) */
#define ATTR_PLOT_LG_TEXT_COLOR         1278        /* int (rgb value) */
#define ATTR_PLOT_LG_FONT               1280        /* (char *) */
#define ATTR_PLOT_LG_FONT_NAME_LENGTH   1281        /* int (not settable) */
#define ATTR_FRAME_VISIBLE              1290        /* int (boolean) */
#define ATTR_PICT_BGCOLOR               1291        /* int (rgb value) */
#define ATTR_FIT_MODE                   1292        /* int: VAL_SIZE_TO_IMAGE, ... */
#define ATTR_ORIENTATION                1300        /* int: VAL_PORTRAIT, ... */
#define ATTR_PRINT_AREA_HEIGHT          1301        /* int (millimeter/10), VAL_USE... */
#define ATTR_PRINT_AREA_WIDTH           1302        /* int (millimeter/10), VAL_USE... */
#define ATTR_NUMCOPIES                  1303        /* int (1-100) */
#define ATTR_XRESOLUTION                1304        /* int (dpi), ... */
#define ATTR_YRESOLUTION                1305        /* int (dpi), ... */
#define ATTR_XOFFSET                    1306        /* int (millimeter/10), VAL_CENTER... */
#define ATTR_YOFFSET                    1307        /* int (millimeter/10), VAL_CENTER... */
#define ATTR_COLOR_MODE                 1308        /* int: VAL_BW, ... */
#define ATTR_DUPLEX                     1309        /* int: VAL_SIMPLEX, ... */
#define ATTR_EJECT_AFTER                1310        /* int (boolean) */
#define ATTR_TEXT_WRAP                  1311        /* int (boolean) */
#define ATTR_TAB_INTERVAL               1312        /* int (1-80) */
#define ATTR_SHOW_PAGE_NUMBERS          1313        /* int (boolean) */
#define ATTR_SHOW_LINE_NUMBERS          1314        /* int (boolean) */
#define ATTR_SHOW_FILE_NAME             1315        /* int (boolean) */
#define ATTR_SHOW_DATE                  1316        /* int (boolean) */
#define ATTR_SHOW_TIME                  1317        /* int (boolean) */
#define ATTR_PRINT_FONT_NAME            1318        /* (char *) */
#define ATTR_PRINT_ITALIC               1319        /* int (boolean) */
#define ATTR_PRINT_UNDERLINE            1320        /* int (boolean) */
#define ATTR_PRINT_STRIKEOUT            1321        /* int (boolean) */
#define ATTR_PRINT_POINT_SIZE           1322        /* int  */
#define ATTR_PRINT_BOLD                 1323        /* int  */
#define ATTR_PRINT_FONT_NAME_LENGTH     1324        /* int (not settable) */
#define ATTR_PRINTER_NAME               1325        /* (char *) */
#define ATTR_PRINTER_NAME_LENGTH        1326        /* int (not settable) */
#define ATTR_BITMAP_PRINTING            1327        /* int  */
#define ATTR_SYSTEM_PRINT_DIALOG_ONLY   1328        /* int (boolean) */
#define ATTR_CHARS_PER_LINE             1329        /* int */
#define ATTR_LINES_PER_PAGE             1330        /* int */
#define ATTR_PEN_COLOR                  1350        /* int (rgb value) */
#define ATTR_PEN_FILL_COLOR             1351        /* int (rgb value) */
#define ATTR_PEN_MODE                   1354        /* int: VAL_COPY_MODE, ... */
#define ATTR_PEN_WIDTH                  1355        /* int: (1-255) */
#define ATTR_PEN_PATTERN                1356        /* unsigned char[8] */
#define ATTR_PEN_STYLE                  1357        /* int: VAL_SOLID, ... */
#define ATTR_DRAW_POLICY                1370        /* int: VAL_UPDATE_IMMEDIATELY, ... */
#define ATTR_OVERLAPPED                 1372        /* int (not settable) */
#define ATTR_OVERLAPPED_POLICY          1373        /* int: VAL_DRAW_ON_TOP or VAL_DEFER_DRAWING */
#define ATTR_XCOORD_AT_ORIGIN           1374        /* double */
#define ATTR_YCOORD_AT_ORIGIN           1375        /* double */
#define ATTR_XSCALING                   1376        /* double */
#define ATTR_YSCALING                   1377        /* double */
#define ATTR_ALLOW_UNSAFE_TIMER_EVENTS  1400        /* int (boolean) */    
#define ATTR_REPORT_LOAD_FAILURE        1405        /* int (boolean) */    
#define ATTR_ALLOW_MISSING_CALLBACKS    1410        /* int (boolean) */    
#define ATTR_SUPPRESS_EVENT_PROCESSING  1415        /* int (boolean) */
#define ATTR_TASKBAR_BUTTON_VISIBLE     1420        /* int (boolean) */
#define ATTR_TASKBAR_BUTTON_TEXT        1425        /* (char *)      */
#define ATTR_DEFAULT_MONITOR            1430        /* int */
#define ATTR_PRIMARY_MONITOR            1435        /* int */
#define ATTR_NUM_MONITORS               1440        /* int */
#define ATTR_FIRST_MONITOR              1445        /* int */
#define ATTR_DISABLE_PROG_PANEL_SIZE_EVENTS 1450    /* int (boolean) */
#define ATTR_USE_LOCALIZED_DECIMAL_SYMBOL 1451      /* int: VAL_USE_PERIOD, VAL_USE_SYSTEM_SETTING */
#define ATTR_LOCALIZED_DECIMAL_SYMBOL   1452        /* (char) */
#define ATTR_RESOLUTION_ADJUSTMENT      1500        /* int: VAL_USE_PANEL_SETTING, 0-100 */
#define ATTR_UPPER_LEFT_CORNER_COLOR    1510        /* int (rgb value) */
#define ATTR_ROW_LABELS_COLOR           1511        /* int (rgb value) */
#define ATTR_COLUMN_LABELS_COLOR        1512        /* int (rgb value) */
#define ATTR_TABLE_BGCOLOR              1513        /* int (rgb value) */
#define ATTR_TABLE_MODE                 1514        /* int: VAL_COLUMN, ... */
#define ATTR_ROW_LABELS_VISIBLE         1515        /* int (boolean) */
#define ATTR_COLUMN_LABELS_VISIBLE      1516        /* int (boolean) */
#define ATTR_ROW_LABELS_WIDTH           1517        /* int (0-32767) */
#define ATTR_COLUMN_LABELS_HEIGHT       1518        /* int (0-32767) */
#define ATTR_FIRST_VISIBLE_ROW          1519        /* int */
#define ATTR_FIRST_VISIBLE_COLUMN       1520        /* int */
#define ATTR_NUM_VISIBLE_ROWS           1521        /* int */
#define ATTR_NUM_VISIBLE_COLUMNS        1522        /* int */
#define ATTR_ENABLE_ROW_SIZING          1523        /* int (boolean) */
#define ATTR_ENABLE_COLUMN_SIZING       1524        /* int (boolean) */
#define ATTR_ENABLE_POPUP_MENU          1525        /* int (boolean) */
#define ATTR_GRID_AREA_TOP              1526        /* int (not settable) */
#define ATTR_GRID_AREA_LEFT             1527        /* int (not settable) */
#define ATTR_GRID_AREA_WIDTH            1528        /* int (not settable) */
#define ATTR_GRID_AREA_HEIGHT           1529        /* int (not settable) */
#define ATTR_TABLE_RUN_STATE            1530        /* int: VAL_SELECT_STATE, ... */
#define ATTR_AUTO_EDIT                  1531        /* int (boolean) */
#define ATTR_CELL_TYPE                  1580        /* int: VAL_CELL_NUMERIC, ... */
#define ATTR_CELL_DIMMED                1581        /* int (boolean) */
#define ATTR_CELL_MODE                  1582        /* int: VAL_HOT, ... */
#define ATTR_HORIZONTAL_GRID_COLOR      1583        /* int (rgb value) */
#define ATTR_VERTICAL_GRID_COLOR        1584        /* int (rgb value) */
#define ATTR_HORIZONTAL_GRID_VISIBLE    1585        /* int (boolean) */
#define ATTR_VERTICAL_GRID_VISIBLE      1586        /* int (boolean) */
#define ATTR_MIN_NUM_LINES_VISIBLE      1587        /* int */
#define ATTR_INCDEC_ARROW_COLOR         1588        /* int (rgb value) */
#define ATTR_NUM_CELL_DFLT_VALUE        1589        /* same datatype as cell */
#define ATTR_STR_CELL_DFLT_VALUE        1590        /* (char *) */
#define ATTR_STR_CELL_DFLT_VALUE_LENGTH 1591        /* int (not settable) */
#define ATTR_CELL_JUSTIFY               1592        /* int: VAL_TOP_LEFT_JUSTIFIED, ... */
#define ATTR_STR_CELL_NUM_LINES         1593        /* int (not settable) */
#define ATTR_CELL_FRAME_COLOR           1594        /* int (rgb value) */
#define ATTR_SHOW_RING_ARROW            1595        /* int (boolean) */
#define ATTR_RING_ARROW_LOCATION        1596        /* int: VAL_LEFT_ANCHOR, VAL_RIGHT_ANCHOR */
#define ATTR_RING_ITEMS_UNIQUE          1597        /* int (boolean) */
#define ATTR_CASE_SENSITIVE_COMPARE     1598        /* int (boolean) */
#define ATTR_CELL_SHORTCUT_KEY          1599        /* int */
#define ATTR_USE_LABEL_TEXT             1620        /* int (boolean) */
#define ATTR_SIZE_MODE                  1621        /* int: VAL_USE_EXPLICIT_SIZE, ... */
#define ATTR_LABEL_WRAP_MODE            1622        /* int: VAL_CHAR_WRAP, ... */
#define ATTR_ROW_HEIGHT                 1650        /* int */
#define ATTR_ROW_ACTUAL_HEIGHT          1651        /* int (not settable) */
#define ATTR_COLUMN_WIDTH               1700        /* int */
#define ATTR_COLUMN_ACTUAL_WIDTH        1701        /* int (not settable) */
#define ATTR_COLUMN_VISIBLE             1702        /* int (boolean) */
#define ATTR_DATASOCKET_ENABLED         1750        /* int (boolean) */
#define ATTR_DS_BIND_PLOT_STYLE         1770        /* int: VAL_THIN_LINE, ... */
#define ATTR_DS_BIND_POINT_STYLE        1771        /* int: VAL_EMPTY_SQUARE, ... */
#define ATTR_DS_BIND_LINE_STYLE         1772        /* int: VAL_SOLID, ... */
#define ATTR_DS_BIND_PLOT_COLOR         1773        /* int (rgb value) */
#define ATTR_DATASOCKET_SOURCE          1791        /* (char *) (not settable) */
#define ATTR_DATASOCKET_SOURCE_LENGTH   1792        /* int (not settable) */
#define ATTR_DATASOCKET_MODE            1793        /* int (not settable) */
#define ATTR_COLOR_DEPTH                1800        /* int */
#define ATTR_SYSTEM_MONITOR_HANDLE      1801        /* HMONITOR */
#define ATTR_NEXT_MONITOR               1802        /* int */
#define ATTR_POPUP_STYLE                1850        /* int: VAL_CLASSIC, ... */
#define ATTR_TREE_BGCOLOR               1900        /* int (rgb value) */
#define ATTR_SHOW_CONNECTION_LINES      1901        /* int (boolean) */
#define ATTR_SHOW_PLUS_MINUS            1902        /* int (boolean) */
#define ATTR_SHOW_MARKS                 1903        /* int (boolean) */
#define ATTR_SHOW_IMAGES                1904        /* int (boolean) */
#define ATTR_MARK_REFLECT               1905        /* int (boolean) */
#define ATTR_AUTO_EXPAND                1906        /* int (boolean) */
#define ATTR_AUTO_HSCROLL               1907        /* int (boolean) */
#define ATTR_FULL_ROW_SELECT            1908        /* int (boolean) */
#define ATTR_INDENT_OFFSET              1909        /* int (in pixels) */
#define ATTR_RADIO_SIBLING_ALWAYS_MARKED 1910       /* int (boolean) */
#define ATTR_TEXT_CLICK_TOGGLES_MARK    1911        /* int (boolean) */
#define ATTR_HIDE_ACTIVE_ITEM           1912        /* int (boolean) */
#define ATTR_SELECTION_MODE             1913        /* int: VAL_SELECTION_MULTIPLE, ... */
#define ATTR_TREE_RUN_STATE             1914        /* int: VAL_SELECT_STATE, ... */
#define ATTR_ENABLE_DRAG_DROP           1915        /* int (boolean) */
#define ATTR_EXPANDED_IMAGE_INDEX       1916        /* int */
#define ATTR_DISABLE_TOOLTIPS           1917        /* int (boolean) */
#define ATTR_COLLAPSED_IMAGE_INDEX      1931        /* int */
#define ATTR_MARK_TYPE                  1950        /* int: VAL_MARK_NONE, ... */
#define ATTR_MARK_STATE                 1951        /* int: VAL_MARK_OFF, ... */
#define ATTR_SELECTED                   1952        /* int (boolean) */
#define ATTR_COLLAPSED                  1953        /* int (boolean) */
#define ATTR_NO_EDIT_LABEL              1954        /* int (boolean) */
#define ATTR_ITEM_HEIGHT                1955        /* int */
#define ATTR_ITEM_ACTUAL_HEIGHT         1956        /* int */
#define ATTR_ENABLE_DRAG                1957        /* int (boolean) */
#define ATTR_ENABLE_DROP                1958        /* int (boolean) */
#define ATTR_IMAGE_INDEX                1959        /* int */
#define ATTR_HILITE_ONLY_WHEN_PANEL_ACTIVE 1990     /* int (boolean) */
#define ATTR_TITLEBAR_STYLE             1991        /* int: VAL_CLASSIC, ... */
#define ATTR_ITEM_BITMAP                1992        /* int (bitmap id) */
#define ATTR_TOP_RANGE                  2000        /* int (-32768 - 32767, VAL_AUTO_RANGE, VAL_NO_RANGE_LIMIT) */
#define ATTR_BOTTOM_RANGE               2001        /* int (-32768 - 32767, VAL_AUTO_RANGE, VAL_NO_RANGE_LIMIT) */
#define ATTR_LEFT_RANGE                 2002        /* int (-32768 - 32767, VAL_AUTO_RANGE, VAL_NO_RANGE_LIMIT) */
#define ATTR_RIGHT_RANGE                2003        /* int (-32768 - 32767, VAL_AUTO_RANGE, VAL_NO_RANGE_LIMIT) */
#define ATTR_SHOW_CONTENTS_WHILE_DRAGGING 2004      /* int (boolean) */
#define ATTR_SPAN_PANEL                 2005        /* int (boolean) */
#define ATTR_OPERABLE_AS_INDICATOR      2006        /* int (boolean) */
#define ATTR_TOP_ACTUAL_RANGE           2007        /* int (not settable) */
#define ATTR_BOTTOM_ACTUAL_RANGE        2008        /* int (not settable) */
#define ATTR_LEFT_ACTUAL_RANGE          2009        /* int (not settable) */
#define ATTR_RIGHT_ACTUAL_RANGE         2010        /* int (not settable) */
#define ATTR_DIGWAVEFORM_AUTOSIZE       2100        /* int (boolean) */
#define ATTR_DIGWAVEFORM_LINE_LABEL     2101        /* (char *) */
#define ATTR_DIGWAVEFORM_LINE_LABEL_LENGTH 2102     /* int (not settable) */
#define ATTR_DIGWAVEFORM_BUS_LABEL      2103        /* (char *) */
#define ATTR_DIGWAVEFORM_BUS_LABEL_LENGTH 2104      /* int (not settable) */
#define ATTR_DIGWAVEFORM_FONT           2105        /* (char *) */
#define ATTR_DIGWAVEFORM_FONT_NAME_LENGTH 2106      /* int (not settable) */
#define ATTR_DIGWAVEFORM_SHOW_STATE_LABEL 2107      /* int (boolean) */
#define ATTR_DIGWAVEFORM_EXPAND_BUSES   2108        /* int (boolean) */
#define ATTR_DIGWAVEFORM_PLOT_COLOR     2109        /* int (rgb value) */
#define ATTR_TABS_FIT_MODE              2200        /* int: VAL_SINGLE_ROW, ... */
#define ATTR_TABS_LOCATION              2201        /* int: VAL_LEFT_TOP, ... */
#define ATTR_TABS_VISIBLE               2202        /* int (boolean) */
#define ATTR_TABS_START_OFFSET          2203        /* int (-32768 - 32767) */
#define ATTR_TABS_END_OFFSET            2204        /* int (-32768 - 32767) */
#define ATTR_NUM_ANNOTATIONS            2300        /* int (0-10) */
#define ATTR_ANNOTATION_CAPTION         2301        /* (char *) */
#define ATTR_ANNOTATION_CAPTION_LENGTH  2302        /* int (not settable) */
#define ATTR_ANNOTATION_CAPTION_FONT    2303        /* (char *) */
#define ATTR_ANNOTATION_CAPTION_FONT_NAME_LENGTH 2304   /* int (not settable) */
#define ATTR_ANNOTATION_CAPTION_COLOR   2305        /* int (rgb value) */
#define ATTR_ANNOTATION_CAPTION_BGCOLOR 2306        /* int (rgb value) */
#define ATTR_ANNOTATION_CAPTION_BOLD    2307        /* int (boolean) */
#define ATTR_ANNOTATION_CAPTION_ITALIC  2308        /* int (boolean) */
#define ATTR_ANNOTATION_CAPTION_UNDERLINE 2309      /* int (boolean) */
#define ATTR_ANNOTATION_CAPTION_STRIKEOUT 2310      /* int (boolean) */
#define ATTR_ANNOTATION_CAPTION_POINT_SIZE 2311     /* int (1-255) */
#define ATTR_ANNOTATION_GLYPH_STYLE     2320        /* int: VAL_EMPTY_SQUARE, ... */
#define ATTR_ANNOTATION_GLYPH_COLOR     2321        /* int (rgb value) */
#define ATTR_ANNOTATION_LINE_STYLE      2325        /* int: VAL_THIN_LINE, ... */
#define ATTR_ANNOTATION_LINE_COLOR      2326        /* int (rgb value) */
#define ATTR_ANNOTATION_ARROW_STYLE     2327        /* int: VAL_ARROW, ... */
#define ATTR_ANNOTATION_XVALUE          2330        /* double */
#define ATTR_ANNOTATION_YVALUE          2331        /* double */
#define ATTR_ANNOTATION_XOFFSET         2332        /* int */
#define ATTR_ANNOTATION_YOFFSET         2333        /* int */
#define ATTR_ANNOTATION_LOCKED          2340        /* int (boolean) */
#define ATTR_ANNOTATION_XAXIS           2341        /* int: VAL_BOTTOM_XAXIS or VAL_TOP_XAXIS */
#define ATTR_ANNOTATION_YAXIS           2342        /* int: VAL_LEFT_YAXIS or VAL_RIGHT_YAXIS */
#define ATTR_ANNOTATION_VISIBLE         2343        /* int (boolean) */
#define ATTR_ANNOTATION_HIDE_CAPTION    2344        /* int (boolean) */
#define ATTR_ANNOTATION_CAPTION_ALWAYS_IN_VIEW 2345 /* int (boolean) */
#define ATTR_ANNOTATION_OFFSET_MODE     2350        /* int: VAL_GLYPH_OFFSET or VAL_TOP_LEFT_OFFSET */

/* */
/* ATTRIBUTE VALUES: (primarily for use in the set and get attribute functions) */
/* */

    /*** control data types (for ATTR_DATA_TYPE): ***/
#define VAL_CHAR                        0L
#define VAL_INTEGER                     1L
#define VAL_SHORT_INTEGER               2L
#define VAL_FLOAT                       3L
#define VAL_DOUBLE                      4L
#define VAL_STRING                      5L
#define VAL_UNSIGNED_SHORT_INTEGER      6L
#define VAL_UNSIGNED_INTEGER            7L
#define VAL_UNSIGNED_CHAR               8L
#define VAL_NO_TYPE                     9L    /* for internal use only */

    /*** special position values: ***/
#define VAL_AUTO_CENTER         0x80000000L /* for ATTR_LABEL_TOP, ATTR_, ... */
                                            /* SetPanelPos, NewPanel, ... */
#define VAL_RIGHT_ANCHOR        0x8000000FL /* for ATTR_LABEL_LEFT, ATTR_RING_ARROW_LOCATION, AddCtrlToSplitter... */
#define VAL_LEFT_ANCHOR         0x8000000EL /* for ATTR_LABEL_LEFT, ATTR_RING_ARROW_LOCATION, AddCtrlToSplitter... */
#define VAL_BOTTOM_ANCHOR       0x8000000DL /* for ATTR_LEGEND_TOP, AddCtrlToSplitter... */
#define VAL_TOP_ANCHOR          0x8000000CL /* for ATTR_LEGEND_TOP, AddCtrlToSplitter... */
#define VAL_AUTO_RANGE          0x8000000BL /* for ATTR_TOP_RANGE, ... */
#define VAL_NO_RANGE_LIMIT      0x8000000AL /* for ATTR_TOP_RANGE, ... */
#define VAL_KEEP_SAME_POSITION  0x80000001L /* for DuplicateCtrl */
                                            /* and DuplicatePanel */

    /*** justification values (for ATTR_LABEL_JUSTIFY and ATTR_CELL_JUSTIFY): ***/
#define VAL_TOP_LEFT_JUSTIFIED          0x00L
#define VAL_TOP_RIGHT_JUSTIFIED         0x10L
#define VAL_TOP_CENTER_JUSTIFIED        0x20L
#define VAL_BOTTOM_LEFT_JUSTIFIED       0x40L
#define VAL_BOTTOM_RIGHT_JUSTIFIED      0x50L
#define VAL_BOTTOM_CENTER_JUSTIFIED     0x60L
#define VAL_CENTER_LEFT_JUSTIFIED       0x80L
#define VAL_CENTER_RIGHT_JUSTIFIED      0x90L
#define VAL_CENTER_CENTER_JUSTIFIED     0xA0L

    /*** justification values (for ATTR_LABEL_JUSTIFY and ATTR_TEXT_JUSTIFY): ***/
#define VAL_LEFT_JUSTIFIED              VAL_TOP_LEFT_JUSTIFIED
#define VAL_RIGHT_JUSTIFIED             VAL_TOP_RIGHT_JUSTIFIED
#define VAL_CENTER_JUSTIFIED            VAL_TOP_CENTER_JUSTIFIED

    /***   RGB colors (for all color attributes): ***/
#define VAL_RED                         0xFF0000L /* 16 standard colors */
#define VAL_GREEN                       0x00FF00L
#define VAL_BLUE                        0x0000FFL
#define VAL_CYAN                        0x00FFFFL
#define VAL_MAGENTA                     0xFF00FFL
#define VAL_YELLOW                      0xFFFF00L
#define VAL_DK_RED                      0x800000L
#define VAL_DK_BLUE                     0x000080L
#define VAL_DK_GREEN                    0x008000L
#define VAL_DK_CYAN                     0x008080L
#define VAL_DK_MAGENTA                  0x800080L
#define VAL_DK_YELLOW                   0x808000L
#define VAL_LT_GRAY                     0xC0C0C0L
#define VAL_DK_GRAY                     0x808080L
#define VAL_BLACK                       0x000000L
#define VAL_WHITE                       0xFFFFFFL

#define VAL_PANEL_GRAY                  VAL_LT_GRAY
#define VAL_GRAY                        0xA0A0A0L
#define VAL_OFFWHITE                    0xE0E0E0L
#define VAL_TRANSPARENT                 0x1000000L

    /*** platform independent fonts: ***/
#define VAL_MENU_FONT                    "NIMenu"
#define VAL_DIALOG_FONT                  "NIDialog"
#define VAL_EDITOR_FONT                  "NIEditor"
#define VAL_APP_FONT                     "NIApp"
#define VAL_MESSAGE_BOX_FONT             "NIMessageBox"

    /*** platform independent meta fonts: ***/
#define VAL_MENU_META_FONT               "NIMenuMetaFont"
#define VAL_DIALOG_META_FONT             "NIDialogMetaFont"
#define VAL_EDITOR_META_FONT             "NIEditorMetaFont"
#define VAL_APP_META_FONT                "NIAppMetaFont"
#define VAL_MESSAGE_BOX_META_FONT        "NIMessageBoxMetaFont"

    /*** CVI supplied meta fonts: ***/
#define VAL_7SEG_META_FONT               "NI7SegMetaFont"
#define VAL_SYSTEM_META_FONT             "NISystemMetaFont"

    /*** wrap modes (for ATTR_WRAP_MODE and ATTR_LABEL_WRAP_MODE): ***/
#define VAL_CHAR_WRAP                    1L
#define VAL_LINE_WRAP                    2L
#define VAL_WORD_WRAP                    4L

    /*** numeric display formats: ***/
#define VAL_FLOATING_PT_FORMAT          0xFFF0
#define VAL_SCIENTIFIC_FORMAT           0xFFF1
#define VAL_ENGINEERING_FORMAT          0xFFF2
#define VAL_ABSOLUTE_TIME_FORMAT        0xFFF3
#define VAL_RELATIVE_TIME_FORMAT        0xFFF4
#define VAL_DECIMAL_FORMAT               64L    /* not valid for graph axis format */
#define VAL_HEX_FORMAT                   66L    /* not valid for graph axis format */
#define VAL_OCTAL_FORMAT                 67L    /* not valid for graph axis format */
#define VAL_BINARY_FORMAT                68L    /* not valid for graph axis format */

    /*** key masks:
         either the ascii field or the virtual key field will be all zeros.
    ***/
#define VAL_ASCII_KEY_MASK              (0xFF)
#define VAL_VKEY_MASK                   (0xFF << 8)
#define VAL_MODIFIER_KEY_MASK           (0xFF << 16)

    /*** key modifiers: ***/
#define VAL_SHIFT_MODIFIER               (1L << 16)
#define VAL_UNDERLINE_MODIFIER           (1L << 17)  /* the Alt key under Windows */
#define VAL_MENUKEY_MODIFIER             (1L << 18)  /*  the Ctrl key under Windows - the command accelerator, which is used in shortkey keys */
#define VAL_SHIFT_AND_MENUKEY            (VAL_SHIFT_MODIFIER | VAL_MENUKEY_MODIFIER)

    /*** virtual key codes: ***/
#define VAL_FWD_DELETE_VKEY              (1L << 8)  /* not available on SPARC */
#define VAL_BACKSPACE_VKEY               (2L << 8)  /* the delete key on SPARC */
#define VAL_ESC_VKEY                     (3L << 8)
#define VAL_TAB_VKEY                     (4L << 8)
#define VAL_ENTER_VKEY                   (5L << 8)
#define VAL_UP_ARROW_VKEY                (6L << 8)
#define VAL_DOWN_ARROW_VKEY              (7L << 8)
#define VAL_LEFT_ARROW_VKEY              (8L << 8)
#define VAL_RIGHT_ARROW_VKEY             (9L << 8)
#define VAL_INSERT_VKEY                  (10L << 8)
#define VAL_HOME_VKEY                    (11L << 8)
#define VAL_END_VKEY                     (12L << 8)
#define VAL_PAGE_UP_VKEY                 (13L << 8)
#define VAL_PAGE_DOWN_VKEY               (14L << 8)
#define VAL_F1_VKEY                      (15L << 8)
#define VAL_F2_VKEY                      (16L << 8)
#define VAL_F3_VKEY                      (17L << 8)
#define VAL_F4_VKEY                      (18L << 8)
#define VAL_F5_VKEY                      (19L << 8)
#define VAL_F6_VKEY                      (20L << 8)
#define VAL_F7_VKEY                      (21L << 8)
#define VAL_F8_VKEY                      (22L << 8)
#define VAL_F9_VKEY                      (23L << 8)
#define VAL_F10_VKEY                     (24L << 8)
#define VAL_F11_VKEY                     (25L << 8)
#define VAL_F12_VKEY                     (26L << 8)
#define VAL_POPUP_MENUBAR_VKEY           (37L << 8) /* for fake key stroke only */
#define VAL_POPUP_MENU_VKEY              (39L << 8)

    /*** range checking (for ATTR_CHECK_RANGE): ***/
#define VAL_COERCE                       0L
#define VAL_IGNORE                       1L
#define VAL_NOTIFY                       2L

    /*** fill options (for ATTR_FILL_OPTION):  ***/
#define VAL_NO_FILL                      5L
#define VAL_TOP_FILL                     2L
#define VAL_BOTTOM_FILL                  1L
#define VAL_RIGHT_FILL                  VAL_TOP_FILL
#define VAL_LEFT_FILL                   VAL_BOTTOM_FILL

    /*** marker styles (for ATTR_MARKER_STYLE): ***/
#define VAL_NO_MARKERS                   3L
#define VAL_NO_INNER_MARKERS             2L
#define VAL_FULL_MARKERS                 1L

    /*** tick styles (for ATTR_TICK_STYLE): ***/
#define VAL_NO_TICKS                     3L
#define VAL_NO_MINOR_TICKS               0L
#define VAL_FULL_TICKS                   6L

    /*** control mode values (for ATTR_CTRL_MODE): ***/
#define VAL_INDICATOR                   0L
#define VAL_HOT                         1L
#define VAL_NORMAL                      2L
#define VAL_VALIDATE                    3L

    /*** panel frame styles (for ATTR_FRAME_STYLE): ***/
#define VAL_OUTLINED_FRAME              1L
#define VAL_BEVELLED_FRAME              2L
#define VAL_RAISED_FRAME                3L
#define VAL_HIDDEN_FRAME                4L
#define VAL_STEP_FRAME                  5L
#define VAL_RAISED_OUTLINED_FRAME       6L

    /*** top level panel zoom states (for ATTR_WINDOW_ZOOM) ***/
#define VAL_NO_ZOOM                     0L
#define VAL_MINIMIZE                    1L
#define VAL_MAXIMIZE                    2L

    /*** top level panel floating states (for ATTR_FLOATING) ***/
#define VAL_FLOAT_NEVER                 0L
#define VAL_FLOAT_APP_ACTIVE            1L
#define VAL_FLOAT_ALWAYS                -50L

    /*** graph border styles (for ATTR_EDGE_STYLE): ***/
#define VAL_RAISED_EDGE                 1L
#define VAL_FLAT_EDGE                   2L
#define VAL_RECESSED_EDGE               3L

    /*** images on list box items (for SetListItemImage): ***/
#define VAL_NO_IMAGE                     0L
#define VAL_FOLDER                       1L
#define VAL_OPEN_FOLDER                  2L
#define VAL_CURRENT_FOLDER               3L
#define VAL_PLUS                         4L
#define VAL_MINUS                        5L
#define VAL_BLANK_IMAGE                  6L

    /*** list box check styles (for ATTR_CHECK_STYLE): ***/
#define VAL_CHECK_MARK                   0L
#define VAL_CHECK_BOX                    1L

    /*** graph, strip chart and graph cursor point styles: ***/
    /*** (for ATTR_CURSOR_POINT_STYLE, ATTR_TRACE_POINT_STYLE and ATTR_ANNOTATION_GLYPH_STYLE) ***/
#define VAL_EMPTY_SQUARE                0L
#define VAL_SOLID_SQUARE                1L
#define VAL_ASTERISK                    2L
#define VAL_DOTTED_EMPTY_SQUARE         3L
#define VAL_DOTTED_SOLID_SQUARE         4L
#define VAL_SOLID_DIAMOND               5L
#define VAL_EMPTY_SQUARE_WITH_X         6L
#define VAL_EMPTY_SQUARE_WITH_CROSS     7L
#define VAL_BOLD_X                      8L
#define VAL_SMALL_SOLID_SQUARE          9L
#define VAL_SIMPLE_DOT                  10L
#define VAL_EMPTY_CIRCLE                11L
#define VAL_SOLID_CIRCLE                12L
#define VAL_DOTTED_SOLID_CIRCLE         13L
#define VAL_DOTTED_EMPTY_CIRCLE         14L
#define VAL_BOLD_CROSS                  15L
#define VAL_CROSS                       16L
#define VAL_SMALL_CROSS                 17L
#define VAL_X                           18L
#define VAL_SMALL_X                     19L
#define VAL_DOTTED_SOLID_DIAMOND        20L
#define VAL_EMPTY_DIAMOND               21L
#define VAL_DOTTED_EMPTY_DIAMOND        22L
#define VAL_SMALL_EMPTY_SQUARE          23L
#define VAL_NO_POINT                    24L

    /*** graph plot styles (for ATTR_PLOT_STYLE): ***/
#define VAL_THIN_LINE                   0L
#define VAL_CONNECTED_POINTS            1L
#define VAL_SCATTER                     2L
#define VAL_VERTICAL_BAR                3L  /* not valid for strip charts */
#define VAL_HORIZONTAL_BAR              4L  /* not valid for strip charts */
#define VAL_FAT_LINE                    5L
#define VAL_THIN_STEP                   6L
#define VAL_FAT_STEP                    7L
#define VAL_BASE_ZERO_VERTICAL_BAR      8L  /* not valid for strip charts */
#define VAL_BASE_ZERO_HORIZONTAL_BAR    9L  /* not valid for strip charts */

    /*** graph, strip chart, and canvas line styles (for ATTR_LINE_STYLE and ATTR_PEN_STYLE): ***/
#define VAL_SOLID                       0L
#define VAL_DASH                        1L
#define VAL_DOT                         2L
#define VAL_DASH_DOT                    3L
#define VAL_DASH_DOT_DOT                4L

    /*** text and bitmap plot origins (for ATTR_PLOT_ORIGIN, CanvasDrawText(), ...): ***/
#define VAL_LOWER_LEFT                  1L
#define VAL_CENTER_LEFT                 2L
#define VAL_UPPER_LEFT                  3L
#define VAL_LOWER_CENTER                4L
#define VAL_CENTER                      5L
#define VAL_UPPER_CENTER                6L
#define VAL_LOWER_RIGHT                 7L
#define VAL_CENTER_RIGHT                8L
#define VAL_UPPER_RIGHT                 9L

    /*** graph and strip chart scale modes:         ***/
    /*** (for ATTR_XMAP_MODE or ATTR_YMAP_MODE) ***/
#define VAL_LINEAR                      0L
#define VAL_LOG                         1L

    /*** graph and strip chart auto divisions and auto precision: ***/
    /*** (for ATTR_XDIVISIONS, ATTR_XPRECISION, ATTR_YDIVISIONS, ATTR_YPRECISION) ***/
#define VAL_AUTO                        -1L

    /*** graph data modes (for ATTR_DATA_MODE): ***/
#define VAL_RETAIN                      0L
#define VAL_DISCARD                     1L

    /*** graph cursor modes (for ATTR_CURSOR_MODE): ***/
#define VAL_FREE_FORM                   0L
#define VAL_SNAP_TO_POINT               1L

    /*** graph annotation arrow head styles (for ATTR_ANNOTATION_ARROW_STYLE): ***/
#define VAL_NONE                        0L
#define VAL_ARROW                       1L
#define VAL_EMPTY_TRIANGLE              2L
#define VAL_FILLED_TRIANGLE             3L
#define VAL_LONG_ARROW                  4L
#define VAL_LONG_EMPTY_TRIANGLE         5L
#define VAL_LONG_FILLED_TRIANGLE        6L

    /*** graph annotation offset modes (for ATTR_ANNOTATION_OFFSET_MODE): ***/
#define VAL_GLYPH_OFFSET                0L
#define VAL_TOP_LEFT_OFFSET             1L

/*** digital graph data formats (for PlotPackedDigitalLinesMultiBus and PlotDigitalLinesMultiBus) ***/
#define VAL_INTERLEAVED                 0L
#define VAL_NON_INTERLEAVED             1L

    /*** digital graph bus Array values ***/
#define VAL_SINGLE_LINE                 0L  /* only one line per bus */

    /*** graph cursor cross hair styles (for ATTR_CROSS_HAIR_STYLE): ***/
#define VAL_LONG_CROSS                  0L
#define VAL_VERTICAL_LINE               1L
#define VAL_HORIZONTAL_LINE             2L
#define VAL_NO_CROSS                    3L
#define VAL_SHORT_CROSS                 4L

    /*** strip chart scroll modes (for ATTR_SCROLL_MODE): ***/
#define VAL_SWEEP                       0L
#define VAL_CONTINUOUS                  1L
#define VAL_BLOCK                       2L

    /*** graph and strip chart scaling modes: ***/
#define VAL_NO_CHANGE                  -1L
#define VAL_MANUAL                      0L
#define VAL_AUTOSCALE                   1L
#define VAL_LOCK                        2L

    /*** plot deletion options: ***/
#define VAL_DELAYED_DRAW                0L
#define VAL_IMMEDIATE_DRAW              1L
#define VAL_NO_DRAW                     2L

    /*** mouse cursors (for ATTR_MOUSE_CURSOR, SetMouseCursor): ***/
#define VAL_DEFAULT_CURSOR              -1L
#define VAL_CHECK_CURSOR                11L
#define VAL_CROSS_HAIR_CURSOR           12L
#define VAL_BOX_CURSOR                  13L
#define VAL_POINTING_FINGER_CURSOR      14L
#define VAL_OPEN_HAND_CURSOR            15L
#define VAL_QUESTION_MARK_CURSOR        16L
#define VAL_HOUR_GLASS_CURSOR           17L
#define VAL_HIDDEN_CURSOR               18L
#define VAL_SIZE_EW_CURSOR              19L
#define VAL_SIZE_NS_CURSOR              20L
#define VAL_SIZE_NW_SE_CURSOR           21L
#define VAL_SIZE_NE_SW_CURSOR           22L
#define VAL_CLOSED_HAND_CURSOR          23L
#define VAL_SIZE_EW_CURSOR_2            26L
#define VAL_SIZE_NS_CURSOR_2            27L
#define VAL_MOVE_CURSOR                 28L


    /*** scroll bar options (for ATTR_SCROLL_BARS): ***/
#define VAL_NO_SCROLL_BARS               0L
#define VAL_HORIZ_SCROLL_BAR             1L
#define VAL_VERT_SCROLL_BAR              2L
#define VAL_BOTH_SCROLL_BARS            (2L|1L)

    /*** scroll bar sizes (for ATTR_SCROLL_BAR_SIZE): ***/
#define VAL_SMALL_SCROLL_BARS            8L
#define VAL_MEDIUM_SCROLL_BARS          12L
#define VAL_LARGE_SCROLL_BARS           16L

    /*** command button autosizing (for ATTR_AUTO_SIZING): ***/
#define VAL_ALWAYS_AUTO_SIZE             0L
#define VAL_GROW_ONLY                    1L
#define VAL_SHRINK_ONLY                  2L
#define VAL_NEVER_AUTO_SIZE              3L

    /*** print report masks (returned by PrintPanel() and PrintCtrl()): ***/
#define VAL_TOO_MANY_COPIES              (1<<0)
#define VAL_NO_MULTIPLE_COPIES           (1<<1)
#define VAL_NO_DUPLEX                    (1<<2)
#define VAL_NO_LANDSCAPE                 (1<<3)
#define VAL_CANT_FORCE_MONO              (1<<4)
#define VAL_NO_SUCH_XRESOLUTION          (1<<5)
#define VAL_NO_MULTIPLE_XRESOLUTIONS     (1<<6)
#define VAL_NO_SUCH_YRESOLUTION          (1<<7)
#define VAL_NO_MULTIPLE_YRESOLUTIONS     (1<<8)
#define VAL_NO_SEPARATE_YRESOLUTION      (1<<9)
#define VAL_USER_CANCEL                  (1<<10)

    /*** ctrl Id's for the generic popup (used in GenericMessagePopup()): ***/
#define VAL_GENERIC_POPUP_NO_CTRL       0
#define VAL_GENERIC_POPUP_BTN1          1
#define VAL_GENERIC_POPUP_BTN2          2
#define VAL_GENERIC_POPUP_BTN3          3
#define VAL_GENERIC_POPUP_INPUT_STRING  4

    /*** control style values for the system popups ***/
    /*** scroll bar types (for ATTR_SCROLL_BAR_STYLE, ...): ***/
#define VAL_CLASSIC                     0
#define VAL_LAB_STYLE                   1   /* not valid for ATTR_TITLEBAR_STYLE */
#define VAL_WINDOWS_STYLE               2   /* valid only for ATTR_TITLEBAR_STYLE */

    /*** picture fit modes (for ATTR_FIT_MODE): ***/
#define VAL_SIZE_TO_IMAGE                0L
#define VAL_SIZE_TO_PICTURE              1L
#define VAL_PICT_CORNER                  2L
#define VAL_PICT_CENTER                  3L
#define VAL_PICT_TILE                    4L

    /*** printing scope options (passed to PrintPanel()): ***/
#define VAL_FULL_PANEL                  0L
#define VAL_VISIBLE_AREA                1L

    /*** file dialog button labels: ***/
#define VAL_OK_BUTTON                   0
#define VAL_SAVE_BUTTON                 1
#define VAL_SELECT_BUTTON               2
#define VAL_LOAD_BUTTON                 3

    /*** FileSelectPopup return values: ***/
#define VAL_NO_FILE_SELECTED            0
#define VAL_EXISTING_FILE_SELECTED      1
#define VAL_NEW_FILE_SELECTED           2

    /*** DirSelectPopup return values:  ***/
#define VAL_NO_DIRECTORY_SELECTED       0
#define VAL_DIRECTORY_SELECTED          1

    /*** orientation modes (for ATTR_ORIENTATION): ***/
#define VAL_PORTRAIT                     1
#define VAL_LANDSCAPE                    2

    /*** color modes (for ATTR_COLOR_MODE): ***/
#define VAL_BW                           0
#define VAL_GRAYSCALE                    1
#define VAL_COLOR                        2

    /*** duplex modes (for ATTR_DUPLEX): ***/
#define VAL_SIMPLEX                      1
#define VAL_VERTDUPLEX                   2
#define VAL_HORIZDUPLEX                  3

    /*** special values for ATTR_PRINT_AREA_HEIGHT and ATTR_PRINT_AREA_WIDTH: ***/
#define VAL_USE_ENTIRE_PAPER            -1L
#define VAL_INTEGRAL_SCALE               0L

    /*** special value for ATTR_XOFFSET and ATTR_YOFFSET: ***/
#define VAL_CENTER_ON_PAPER             -1L

    /*** special value for ATTR_[X,Y]RESOLUTION,          ***/
    /*** ATTR_ORIENTATION, ATTR_NUMCOPIES and ATTR_DUPLEX: ***/
#define VAL_USE_PRINTER_SETTING         -1L

    /*** SetSleepPolicy() parameters ***/
#define VAL_SLEEP_NONE  1
#define VAL_SLEEP_SOME  2
#define VAL_SLEEP_MORE  3

    
    /*** graph and chart axes (for Axis functions and ATTR_..._YAXIS && ATTR_..._XAXIS): ***/
#define VAL_BOTTOM_XAXIS    0               /* for graphs only */
#define VAL_LEFT_YAXIS      1               /* for graphs and strip charts */
#define VAL_RIGHT_YAXIS     2               /* for graphs only */
#define VAL_TOP_XAXIS       3               /* for graphs only */
    
    /*** pen modes for ATTR_PEN_MODE ***/   /* R = Result, P = Pen, S = Screen */
#define VAL_COPY_MODE           1       /* R = P */
#define VAL_OR_MODE             2       /* R = P | S */
#define VAL_XOR_MODE            3       /* R = P ^ S */
#define VAL_AND_NOT_MODE        4       /* R = ~P & S */
#define VAL_NOT_COPY_MODE       5       /* R = ~P */
#define VAL_OR_NOT_MODE         6       /* R = ~P | S */
#define VAL_NOT_XOR_MODE        7       /* R = ~(P ^ S) */
#define VAL_AND_MODE            8       /* R = P & S */

    /*** values for the canvas control's ATTR_OVERLAPPED_POLICY ***/
#define VAL_DRAW_ON_TOP         1
#define VAL_DEFER_DRAWING       2

    /*** values for the canvas control's ATTR_DRAW_POLICY ***/
#define VAL_UPDATE_IMMEDIATELY  1
#define VAL_MARK_FOR_UPDATE     2
#define VAL_DIRECT_TO_SCREEN    3

/*** values for the drawMode parameter for canvas control drawing functions ***/
#define VAL_DRAW_FRAME                  1
#define VAL_DRAW_INTERIOR               2
#define VAL_DRAW_FRAME_AND_INTERIOR     3


/*** special values for use in rectangle heights and widths ***/
#define VAL_TO_EDGE                         -1
#define VAL_KEEP_SAME_SIZE                  -2

#define VAL_EMPTY_RECT                      MakeRect(0,0,0,0)
#define VAL_ENTIRE_OBJECT                   MakeRect(0,0,VAL_TO_EDGE, VAL_TO_EDGE)

#define VAL_TABLE_ROW_RANGE(r)              MakeRect((r),1,1,VAL_TO_EDGE)
#define VAL_TABLE_COLUMN_RANGE(c)           MakeRect(1,(c),VAL_TO_EDGE,1)
#define VAL_TABLE_ENTIRE_RANGE              MakeRect(1,1,VAL_TO_EDGE,VAL_TO_EDGE)
#define VAL_TABLE_SINGLE_CELL_RANGE(r,c)    MakeRect((r),(c),1,1)

/*** special value for mask array parameter to SetBitmapData and SetBitmapDataEx ***/
#define REMOVE_TRANSPARENCY_MASK  ((unsigned char *)(-1))
/*** special value for alpha array parameter to SetBitmapDataEx ***/
#define REMOVE_ALPHA_CHANNEL      ((unsigned char *)(-1))

/*** values for the system attribute ATTR_RESOLUTION_ADJUSTMENT ***/

#define VAL_USE_PANEL_SETTING     -1

/*** values for table modes **/

#define VAL_GRID                  0
#define VAL_COLUMN                1
#define VAL_ROW                   2

/*** values for table and tree run states **/

#define VAL_SELECT_STATE          0
#define VAL_EDIT_STATE            1

/*** values for table cell types **/

#define VAL_CELL_NUMERIC          0
#define VAL_CELL_STRING           1
#define VAL_CELL_PICTURE          2
#define VAL_CELL_RING             3
#define VAL_CELL_COMBO_BOX        4
#define VAL_CELL_BUTTON           5

#define VAL_USE_MASTER_CELL_TYPE -1     /* used in InsertTableRows and InsertTableColumns only */
#define VAL_TEXT_CELL_TYPE       -2     /* used in GetTableCellFromValue only */

/*** values for table search directions **/

#define VAL_ROW_MAJOR             0
#define VAL_COLUMN_MAJOR          1

/*** values for table row, table column, and tree item size modes **/

#define VAL_USE_EXPLICIT_SIZE           0
#define VAL_SIZE_TO_CELL_TEXT           1
#define VAL_SIZE_TO_CELL_IMAGE          2
#define VAL_SIZE_TO_CELL_IMAGE_AND_TEXT 3
#define VAL_SIZE_TO_CELL_FONT           VAL_SIZE_TO_CELL_TEXT
#define VAL_SIZE_TO_CELL_IMAGE_AND_FONT VAL_SIZE_TO_CELL_IMAGE_AND_TEXT

/*** values for tree control functions **/

#define VAL_NEXT                        -1
#define VAL_NEXT_PLUS_SELF              -2
#define VAL_PREV                        -3
#define VAL_PREV_PLUS_SELF              -4
#define VAL_FIRST                       -5
#define VAL_LAST                        -6
#define VAL_SIBLING                     -7
#define VAL_CHILD                       -8
#define VAL_ALL                         -9
#define VAL_DESCENDENT                  -10
#define VAL_ANCESTOR                    -11

/*** values for tree item mark type **/
#define VAL_MARK_NONE                   0
#define VAL_MARK_CHECK                  1
#define VAL_MARK_RADIO                  2

/*** values for tree item mark state **/
#define VAL_MARK_OFF                    0
#define VAL_MARK_ON                     1
#define VAL_MARK_PARTIAL                2

/*** values for tree selection mode **/
#define VAL_SELECTION_NONE              0
#define VAL_SELECTION_SINGLE            1
#define VAL_SELECTION_MULTIPLE          2

/*** values for the selctionEffect parameter of SetActiveTreeItem **/
#define VAL_NO_SELECTION_EFFECT             0
#define VAL_TOGGLE_SELECTION_OF_ITEM        1
#define VAL_ADD_ITEM_TO_SELECTION           2
#define VAL_ADD_INTERVAL_TO_SELECTION       3
#define VAL_REPLACE_SELECTION_WITH_ITEM     4

/*** state criteria masks for tree items **/
#define VAL_SELECTED                    (1<<0)
#define VAL_UNSELECTED                  (1<<1)
#define VAL_EXPOSED                     (1<<2)
#define VAL_UNEXPOSED                   (1<<3)
#define VAL_MARKED                      (1<<4)
#define VAL_UNMARKED                    (1<<5)
#define VAL_PARTIALLY_MARKED            (1<<6)
#define VAL_EXPANDED                    (1<<7)
#define VAL_COLLAPSED                   (1<<8)
#define VAL_LEAF                        (1<<9)

/*** values for the area parameter of GetIndexFromPoint and GetTabPageFromPoint **/
#define VAL_OTHER_AREA                       0
#define VAL_ITEM_PLUS_MINUS_AREA             1
#define VAL_ITEM_MARK_AREA                   2
#define VAL_ITEM_IMAGE_AREA                  3
#define VAL_ITEM_LABEL_AREA                  4
#define VAL_COLUMN_LABEL_AREA                5
#define VAL_CELL_AREA                        6
#define VAL_COLUMN_SIZE_AREA                 7
#define VAL_GRID_COLUMN_SIZE_AREA            8
#define VAL_VSCROLL_AREA                     9
#define VAL_HSCROLL_AREA                     10
#define VAL_TAB_AREA                         11
#define VAL_TAB_PAGE_AREA                    12
#define VAL_TAB_SCROLL_NEXT_AREA             13
#define VAL_TAB_SCROLL_PREV_AREA             14
#define VAL_BEFORE_FIRST_TAB_AREA            15
#define VAL_AFTER_LAST_TAB_AREA              16

/*** values for tabs fit mode **/
#define VAL_SINGLE_ROW                  0
#define VAL_SINGLE_ROW_JUSTIFY          1
#define VAL_SINGLE_ROW_SCROLL           2
#define VAL_MULTIPLE_ROWS               3

/*** values for tabs location **/
#define VAL_TOP_LEFT                    0
#define VAL_TOP_RIGHT                   1
#define VAL_BOTTOM_LEFT                 2
#define VAL_BOTTOM_RIGHT                3
#define VAL_RIGHT_TOP                   4
#define VAL_RIGHT_BOTTOM                5
#define VAL_LEFT_TOP                    6
#define VAL_LEFT_BOTTOM                 7

/*** values for built-in control menu ids */
#define VAL_GOTO                        -2      /* table */         
#define VAL_SEARCH                      -3      /* table, tree */
#define VAL_SORT                        -4      /* table, tree */
#define VAL_EXPAND_SUBTREE              -5      /* tree */
#define VAL_COLLAPSE_SUBTREE            -6      /* tree */
#define VAL_EXPAND_ALL                  -7      /* tree */
#define VAL_COLLAPSE_ALL                -8      /* tree */

/*** values for localized decimal symbol **/

#define VAL_USE_PERIOD                  0
#define VAL_USE_SYSTEM_SETTING          1

/*** for numeric, graph, text controls ***/
#define VAL_DS_WRITE                    1
#define VAL_DS_READ                     2

/*** for set attribute functions ***/
#define VAL_ALL_OBJECTS                 -1
#define VAL_DFLT_FOR_NEW_OBJECTS        -2

/*** JPEG options - used for the option parameter of SaveBitmapToJPEGFile ***/
#define JPEG_INTERLACE          0x40000000
#define JPEG_PROGRESSIVE        0x40000000
/* Note: only one of the two following may be used. If more than */
/* one is applied, DCTFAST will be used. The default is DCTFLOAT.*/
#define JPEG_DCTFAST            0x20000000
#define JPEG_DCTSLOW            0x10000000
#define JPEG_DCTFLOAT           0x00000000

/* */
/* EVENTS: (for use in callback functions)  */
/* Event numbers 1000 to 10000 are reserved for use by user programs (see QueueUserEvent()) */
/* Note: for callback functions: return 1 (true) to swallow mouse and key events. */
/* */

#define EVENT_NONE                      0
#define EVENT_COMMIT                    1   /* for menu selection events calling the main callback: */
                                                /* eventData1 = panel for menu selections */
                                                /* eventData2 = (int)(callbackData for menuItem) */
                                            /* for table events calling the control callback: */
                                                /* eventData1 = row of cell where event was generated; if 0, event affected multiple cells */
                                                /* eventData2 = column of cell where event was generated; if 0, event affected multiple cells  */
                                            /* for tree events calling the control callback: */
                                                /* eventData1 = MARK_STATE_CHANGE, ... */
                                                /* eventData2 = depends on eventData1 */
#define EVENT_VAL_CHANGED               2   /* for table events calling the control callback: */
                                                /* eventData1 = row of cell where event was generated; if 0, event affected multiple cells  */
                                                /* eventData2 = column of cell where event was generated; if 0, event affected multiple cells  */
                                            /* for tree events calling the control callback: */
                                                /* eventData1 = ACTIVE_ITEM_CHANGE, ... */
                                                /* eventData2 = depends on eventData1 */
#define EVENT_LEFT_CLICK                3   /* eventData1 = mouse vertical pos., */
                                            /* eventData2 = mouse horizontal pos. */
#define EVENT_LEFT_DOUBLE_CLICK         4   /* eventData1 = mouse vertical pos., */
                                            /* eventData2 = mouse horizontal pos. */
#define EVENT_RIGHT_CLICK               5   /* eventData1 = mouse vertical pos., */
                                            /* eventData2 = mouse horizontal pos. */
#define EVENT_RIGHT_DOUBLE_CLICK        6   /* eventData1 = mouse vertical pos., */
                                            /* eventData2 = mouse horizontal pos. */
#define EVENT_KEYPRESS                  7   /* eventData1 = keypressed, */
                                            /* eventData2 = pointer to keypressed */
#define EVENT_GOT_FOCUS                 8   /* eventData1 = old control with focus */
#define EVENT_LOST_FOCUS                9   /* eventData1 = new control with focus */
#define EVENT_IDLE                      10
#define EVENT_CLOSE                     11
#define EVENT_PANEL_SIZE                12
#define EVENT_PANEL_MOVE                13
#define EVENT_END_TASK                  14  /* sent to the MainCallback function when the system wants to quit */
                                            /* return  0L to quit,  1L to abort the termination */
#define EVENT_TIMER_TICK                15  /* eventData1 = ptr to current time (*double) */
                                            /* eventData2 = ptr to time since the control last
                                                        received EVENT_TIMER_TICK (*double) */
#define EVENT_DISCARD                   16  /* sent to panels and controls when they are discarded */
#define EVENT_EXPAND                    17
#define EVENT_COLLAPSE                  18
#define EVENT_DRAG                      19
#define EVENT_DROP                      20
#define EVENT_DROPPED                   21
#define EVENT_SORT                      22
#define EVENT_SELECTION_CHANGE          23
#define EVENT_HSCROLL                   24
#define EVENT_VSCROLL                   25
#define EVENT_MARK_STATE_CHANGE         26
#define EVENT_COMBO_BOX_INSERT          27  /* eventData1 = row of cell where event was generated */
                                            /* eventData2 = column of cell where event was generated */
#define EVENT_ACTIVE_CELL_CHANGE        28
#define EVENT_ROW_SIZE_CHANGE           29  /* eventData1 = row that was sized */
                                            /* eventData2 = 0 */
#define EVENT_COLUMN_SIZE_CHANGE        30  /* eventData1 = 0 */
                                            /* eventData2 = column that was sized */
#define EVENT_ACTIVE_TAB_CHANGE         31  /* eventData1 = old active tab */
                                            /* eventData2 = new active tab */
#define EVENT_EDIT_MODE_STATE_CHANGE    32  /* eventData1 = new edit state */
                                            /* eventData2 = index */

    /*** Event specifiers ***/
#define MARK_STATE_CHANGE           0
#define LABEL_CHANGE                1
#define LEFT_DOUBLE_CLICK           2
#define ENTER_KEY                   3
#define ACTIVE_ITEM_CHANGE          4
#define COLUMN_LABEL_CLICK          5

#define LoWord(x)                   ((short)(x))
#define HiWord(x)                   ((short)((int)(x)>>16))

    /*** Obsolete macros: ***/

#define ATTR_PAPER_HEIGHT               ATTR_PRINT_AREA_HEIGHT
#define ATTR_PAPER_WIDTH                ATTR_PRINT_AREA_WIDTH
#define ATTR_INNER_LOG_MARKERS_VISIBLE  ATTR_INNER_MARKERS_VISIBLE
#define VAL_USE_PRINTER_DEFAULT         -1L /* replaced by VAL_USE_ENTIRE_PAPER, VAL_CENTER_ON_PAPER or VAL_USE_PRINTER_SETTING */
#define VAL_XAXIS                       0   /* replaced by VAL_BOTTOM_XAXIS */

#ifdef WIN32
    #pragma pack(push)
    #pragma pack(4)
#endif

    /*** color map used by intensity plots ***/
#if !defined(_CVI_COLOR_MAP_ENTRY_DEFINED)
typedef struct
{
    union
    {
        char            valChar;
        int             valInt;
        short           valShort;
        float           valFloat;
        double          valDouble;
        unsigned char   valUChar;
        unsigned long   valULong;
        unsigned short  valUShort;
    } dataValue;
    int color;
} ColorMapEntry;
#define _CVI_COLOR_MAP_ENTRY_DEFINED
#endif

    /*** Rect and Point types used by Canvas Controls ***/
#if !defined(_CVI_RECT_DEFINED)
typedef struct 
    {
    int top;
    int left;
    int height;
    int width;
    } Rect;
#define _CVI_RECT_DEFINED
#endif

#if !defined(_CVI_POINT_DEFINED)
typedef  struct
    {
    int x;
    int y;
    } Point;
#define _CVI_POINT_DEFINED
#endif

    /*** CLSID used by NewActiveXCtrl ***/
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct  _GUID
    {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
    }   GUID;
#endif /* !GUID_DEFINED */

#if !defined( __IID_DEFINED__ )
#define __IID_DEFINED__
typedef GUID IID;

typedef IID *LPIID;

#define IID_NULL            GUID_NULL
#define IsEqualIID(riid1, riid2) IsEqualGUID(riid1, riid2)
typedef GUID CLSID;

typedef CLSID *LPCLSID;

#define CLSID_NULL          GUID_NULL
#define IsEqualCLSID(rclsid1, rclsid2) IsEqualGUID(rclsid1, rclsid2)
typedef GUID FMTID;

typedef FMTID *LPFMTID;

#define FMTID_NULL          GUID_NULL
#define IsEqualFMTID(rfmtid1, rfmtid2) IsEqualGUID(rfmtid1, rfmtid2)
#if defined(__cplusplus)
#ifndef _REFGUID_DEFINED
#define _REFGUID_DEFINED
#define REFGUID             const GUID &
#endif /* !_REFGUID_DEFINED */
#ifndef _REFIID_DEFINED
#define _REFIID_DEFINED
#define REFIID              const IID &
#endif /* !_REFIID_DEFINED */
#ifndef _REFCLSID_DEFINED
#define _REFCLSID_DEFINED
#define REFCLSID            const CLSID &
#endif /* !_REFCLSID_DEFINED */
#ifndef _REFFMTID_DEFINED
#define _REFFMTID_DEFINED
#define REFFMTID            const FMTID &
#endif /* !_REFFMTID_DEFINED */
#else /* !__cplusplus */
#ifndef _REFGUID_DEFINED
#define _REFGUID_DEFINED
#define REFGUID             const GUID * const
#endif /* !_REFGUID_DEFINED */
#ifndef _REFIID_DEFINED
#define _REFIID_DEFINED
#define REFIID              const IID * const
#endif /* !_REFIID_DEFINED */
#ifndef _REFCLSID_DEFINED
#define _REFCLSID_DEFINED
#define REFCLSID            const CLSID * const
#endif /* !_REFCLSID_DEFINED */
#ifndef _REFFMTID_DEFINED
#define _REFFMTID_DEFINED
#define REFFMTID            const FMTID * const
#endif /* !_REFFMTID_DEFINED */
#endif /* !__cplusplus */
#endif /* !__IID_DEFINED__ */

#ifndef CAOBJHANDLE_DEFINED
#define CAOBJHANDLE_DEFINED
typedef int CAObjHandle;
#endif /* CAOBJHANDLE_DEFINED */

#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef long HRESULT;
#endif /* !_HRESULT_DEFINED */

#ifdef WIN32
    #pragma pack(pop)
#endif

    /*** errors returned by User Interface Library functions: ***/
typedef enum
{
    UIENoError                                                  = 0,
    UIEManagerCouldNotOpen                                      = -1,
    UIESystemFontCouldNotBeLoaded                               = -2,
    UIEOperationInvalidDuringPopUp                              = -3,
    UIEHandleInvalid                                            = -4,
    UIEAttemptToMovePanelToInvalidLocation                      = -5,
    UIEAttemptedToMakeInoperableCtrlActive                      = -6,
    UIEOperationRequirePanelBeLoaded                            = -7,
    UIEOperationRequiresPopUpBeActive                           = -8,
    UIEOperationRequiresMenuBeLoaded                            = -9,
    UIEControlNotTypeExpectedByFunction                         = -10,
    UIEInvalidMenuItemId                                        = -11,
    UIEOutOfMemory                                              = -12,
    UIEInvalidControlId                                         = -13,
    UIEValueIsInvalidOrOutOfRange                               = -14,
    UIEFileIsNotUserInterfaceFileOrIsBad                        = -15,
    UIEFileFormatIsOutOfDate                                    = -16,
    UIEImageIsBadOrNotSupported                                 = -17,
    UIENoUserEventPossibleWithCurrentSetup                      = -18,
    UIEUnableToOpenFile                                         = -19,
    UIEErrorReadingFile                                         = -20,
    UIEErrorWritingFile                                         = -21,
    UIEErrorClosingFile                                         = -22,
    UIEPanelStateFileHasInvalidFormat                           = -23,
    UIEInvalidIDinResourceFile                                  = -24,
    UIEHardCopyError                                            = -25,
    UIEInvalidDefaultDirSelectedInFSPopup                       = -26,
    UIEOperationIsInvalidForSpecifiedObject                     = -27,
    UIECantFindMenuString                                       = -28,
    UIEPalleteItemsMustBeAddedAtEnd                             = -29,
    UIEMaxNumberOfMenusPerBarExceeded                           = -30,
    UIECantSetSeparatorCheckMark                                = -31,
    UIESeparatorCantHaveSubMenu                                 = -32,
    UIEItemMustBeASeparator                                     = -33,
    UIEItemCantBeASeparator                                     = -34,
    UIEItemAlreadyHasSubMenu                                    = -35,
    UIEItemDoesNotHaveASubMenu                                  = -36,
    UIECtrlIdIsNotAMenuOrMenuItemIdOrNull                       = -37,
    UIECtrlIdIsNotAMenuOrMenuItemId                             = -38,
    UIENotASubMenuId                                            = -39,
    UIEInvalidMenuId                                            = -40,
    UIENotAMenuBarHandle                                        = -41,
    UIENotAPanelHandle                                          = -42,
    UIEOperationInvalidWhileThisPopupIsActive                   = -43,
    UIEAttributeNotAppropriateForSpecifiedControlOrPanelOrMenu  = -44,
    UIEInvalidControlType                                       = -45,
    UIEInvalidAttribute                                         = -46,
    UIESlideMustHaveFillAboveOrBelow                            = -47,
    UIEScaleMustHaveFillAboveOrBelow                            = -48,
    UIESlideMustBeLinear                                        = -49,
    UIEScaleMustBeLinear                                        = -50,
    UIESlideDoesntHaveIncDecArrows                              = -51,
    UIEScaleDoesntHaveIncDecArrows                              = -52,
    UIEInvalidDataType                                          = -53,
    UIEInvalidAttrForDataType                                   = -54,
    UIEIndexOutOfRange                                          = -55,
    UIENoItemsInList                                            = -56,
    UIEBufTooSmall                                              = -57,
    UIECtrlDoesNotHaveAValue                                    = -58,
    UIEValueNotInListCtrl                                       = -59,
    UIENotListCtrl                                              = -60,
    UIENotListCtrlOrBinary                                      = -61,
    UIENotStringDataType                                        = -62,
    UIEAttributeNotSettable                                     = -63,
    UIEInvalidCtrlMode                                          = -64,
    UIENullPointerPassed                                        = -65,
    UIECantSetGetTextBGColorOfMenuRing                          = -66,
    UIERingMustBeMenuStyle                                      = -67,
    UIECantColorTextTransparent                                 = -68,
    UIEInvalidValueInListControl                                = -69,
    UIEInvalidTabbingOrderPosition                              = -70,
    UIECantSetTabOrderOfAnIndicator                             = -71,
    UIENotAValidNumber                                          = -72,
    UIEPanelDoesntHaveAMenuBar                                  = -73,
    UIENotTextBox                                               = -74,
    UIEInvalidChartStyle                                        = -75,
    UIEInvalidImageType                                         = -76,
    UIENotATopLevelPanelAttribute                               = -77,
    UIEListCtrlNotInCheckMode                                   = -78,
    UIEPanelDataDoesNotFitIntoPanel                             = -79,
    UIEAxisRangeError                                           = -80,
    UIENonExistantCursor                                        = -81,
    UIENotAValidPlot                                            = -82,
    UIEOutsidePlotArea                                          = -83,
    UIEStringTooLarge                                           = -84,
    UIECallbackFuncIsWrongType                                  = -85,
    UIECantFindCallbackFunc                                     = -86,
    UIEZeroCursors                                              = -87,
    UIEInvalidChartScaling                                      = -88,
    UIEFontNotInFontTable                                       = -89,
    UIEBadAttributeValue                                        = -90,
    UIETooManyFilesOpen                                         = -91,
    UIEfEOF                                                     = -92,
    UIEIOError                                                  = -93,
    UIEFileWasNotFound                                          = -94,
    UIEAccessDenied                                             = -95,
    UIEFileAccessNotEnabled                                     = -96,
    UIEDiskFull                                                 = -97,
    UIEFileAlreadyExists                                        = -98,
    UIEFileAlreadyOpen                                          = -99,
    UIEBadPathnameMsg                                           = -100,
    UIEResourceNotFound                                         = -101,
    UIEOldUnsupportedUIResourceFormat                           = -102,
    UIEFileIsCorrupted                                          = -103,
    UIEOperationFailed                                          = -104,
    UIESlideMustBeRound                                         = -105,
    UIEScaleMustBeRound                                         = -106,
    UIECountOutOfRange                                          = -107,
    UIENotAValidKeycode                                         = -108,
    UIESlideDoesntHaveFrame                                     = -109,
    UIECantColorPanelBGTransparent                              = -110,
    UIECantColorTitleBGTransparent                              = -111,
    UIEMemPrintError                                            = -112,
    UIEReservedHotKey                                           = -113,
    UIEFileFormatIsTooNew                                       = -114,
    UIESystemPrintError                                         = -115,
    UIEDriverPrintError                                         = -116,
    UIEDefCallBackQueueFull                                     = -117,
    UIEInvalidMouseCursor                                       = -118,
    UIEPrintReentranceError                                     = -119,
    UIEOutOfGDISpace                                            = -120,
    UIEHiddenPanelError                                         = -121,
    UIEHiddenCtrlError                                          = -122,
    UIEInvalidAttrForPlot                                       = -123,
    UIECantColorPlotTransparent                                 = -124,
    UIEInvalidColor                                             = -125,
    UIECallbackNameIsAmbiguous                                  = -126,
    UIEInvalidBitmap                                            = -127,
    UIENoImagePresent                                           = -128,
    UIEMustBeInTopLevelPanelThread                              = -129,
    UIEPanelNotInTUIFile                                        = -130,
    UIEMenuBarNotInTUIFile                                      = -131,
    UIECtrlStyleNotInTUIFile                                    = -132,
    UIETagOrValueMissingInTUIFile                               = -133,
    UIENoPrintersInstalled                                      = -135,
    UIEInitialCellOutOfRange                                    = -136,
    UIEInvalidCellType                                          = -137,
    UIECellTypeMismatch                                         = -138,
    UIECtrlDoesNotHaveMenu                                      = -139,
    UIECannotDiscardBuiltInCtrlMenuItem                         = -140,
    UIECantSetSeparatorBold                                     = -141,
    UIEMustPassEventData2                                       = -142,
    UIEActiveXError                                             = -143,
    UIEObjHandleDoesNotReferToActiveXCtrl                       = -144,
    UIEActiveXCtrlNotRegistered                                 = -145,
    UIEActiveXPersistenceError                                  = -146,
    UIEInvalidMenuButtonId                                      = -147,
    UIECannotModifyBuiltInCtrlMenuItem                          = -148,
    UIEDataSocketError                                          = -149,
    UIECtrlAlreadyBound                                         = -150,
    UIECtrlNeedsActiveBinding                                   = -151,
    UIEAttachedPanelMustBeChild                                 = -152,
    UIEItemIsAttached                                           = -153,
    UIEItemIsNotAttached                                        = -154,
    UIECannotSizeCtrl                                           = -155,
    UIECannotSelfAttach                                         = -156,
    UIEOperationInvalidOnMaskedBitmap                           = -157,
    UIEOperationInvalidOnAlphaBitmap                            = -158,
    UIEOperationRequiresMaskedBitmap                            = -159,
    UIEOperationRequiresAlphaBitmap                             = -160,
    UIENonExistantAnnotation                                    = -161,
    UIEOperationInvalidOnTabPanel                               = -162,
    UIEAttrValidOnMenubarOnly                                   = -163,
    UIEAttrValidOnMenuItemOnly                                  = -164,
    UIEAttrValidOnMenuAndSubmenusOnly                           = -165,
    UIEErrorLimit                                               = -166
} UIError;


    /* */
    /* User Interface Library Functions */
    /* */

    /*** for panels: ***/
int CVIFUNC LoadPanel(int parentPanel, const char *fileName, int panelResourceId);   /* returns a panel */
int CVIFUNC LoadPanelEx(int parentPanel, const char *fileName, int panelResourceId, 
                        void *hInstance);         /* returns a panel */
int CVIFUNC NewPanel(int parentPanel, const char *panelTitle, int top, int left,
                     int height, int width);      /* returns a panel */
int CVIFUNC DuplicatePanel(int destinationParentPanel, int panel, const char *dupName,
                           int top, int left);    /* returns a panel */
int CVIFUNC DuplicatePanelTree(int destinationParentPanel, int panel, const char *dupName,
                           int top, int left);    /* returns a panel */
int CVIFUNC DiscardPanel(int panel);
int CVIFUNC DisplayPanel(int panel);
int CVIFUNC HidePanel(int panel);
int CVIFUNC GetActivePanel(void);                 /* returns a panel */
int CVIFUNC SetActivePanel(int panel);
int CVIFUNC ValidatePanel(int panel, int *valid);
int CVIFUNC DefaultPanel(int panel);
int CVIFUNC SavePanelState(int panel, const char *fileName, int index);
int CVIFUNC RecallPanelState(int panel, const char *fileName, int index);
int CVIFUNC SetPanelPos(int panel, int top, int left);
int CVIFUNC SetPanelSize(int panel, int height, int width);

    /*** for controls: ***/
int CVIFUNC NewCtrl(int panel, int style, const char *ctrlLabel, int top, int left); /* returns a ctrl */
int CVIFUNC NewActiveXCtrl(int panel, const char *ctrlLabel, int top, int left, 
                           const CLSID *clsid, const IID *iidForObjHandle, 
                           const char *licenseString, HRESULT *activeXError);        /* returns a ctrl */
int CVIFUNC NewActiveXCtrlFromFile(int panel, const char *ctrlLabel, int top, int left, 
                                   const char *filename, const IID *iidForObjHandle, 
                                   HRESULT *activeXError);                           /* returns a ctrl */
int CVIFUNC GetObjHandleFromActiveXCtrl(int panel, int control, CAObjHandle *objHandle);
int CVIFUNC GetActiveXCtrlFromObjHandle(CAObjHandle objHandle, int *panel, int *control);

int CVIFUNC DuplicateCtrl(int sourcePanel, int control, int destinationPanel,
                          const char *labelOfDuplicate, int top, int left);          /* returns a ctrl */
int CVIFUNC DiscardCtrl(int panel, int control);
int CVIFUNC SetActiveCtrl(int panel, int control);
int CVIFUNC GetActiveCtrl(int panel);     /* returns ctrl */
int CVIFUNC GetCtrlBoundingRect(int panel, int control, int *top, int *left,
                                int *height, int *width);
int CVIFUNC_C SetCtrlVal(int panel, int control, ...);
int CVIFUNC GetCtrlVal(int panel, int control, void *value );
int CVIFUNC DefaultCtrl(int panel, int control);

    /*** for list controls: (list controls are rings and list boxes) ***/
int CVIFUNC_C ReplaceListItem(int panel,  int  control, int index, const char *itemLabel, ...);
int CVIFUNC_C InsertListItem(int panel, int control,  int  index, const char *itemLabel, ...);
int CVIFUNC DeleteListItem(int panel, int control, int index, int count);
int CVIFUNC GetValueFromIndex(int panel, int control, int index, void *itemValue);
int CVIFUNC GetValueLengthFromIndex(int panel, int control, int index, int *length);
int CVIFUNC_C GetIndexFromValue(int panel, int control,  int  *index, ...);
int CVIFUNC GetCtrlIndex(int panel, int control, int *index);
int CVIFUNC SetCtrlIndex(int panel, int control, int index);
int CVIFUNC ClearListCtrl(int panel, int control);
int CVIFUNC GetNumListItems(int panel, int control, int *count);
int CVIFUNC GetLabelFromIndex(int panel, int control, int index, char *itemLabel);
int CVIFUNC GetLabelLengthFromIndex(int panel, int control, int index, int *length);
int CVIFUNC GetIndexFromPoint (int panel, int control, Point coordinate, int *index, 
                               int *area, int *column);

    /*** for list boxes: ***/
int CVIFUNC SetListItemImage(int panel, int control, int index, int image);
int CVIFUNC GetListItemImage(int panel, int control, int index, int *image);
int CVIFUNC IsListItemChecked(int panel, int control, int index, int *checked);
int CVIFUNC CheckListItem(int panel, int control, int index, int check);
int CVIFUNC GetNumCheckedItems(int panel, int control, int *numItems);

    /*** for text boxes: ***/
int CVIFUNC InsertTextBoxLine(int panel, int control, int index, const char *text);
int CVIFUNC ReplaceTextBoxLine(int panel, int control, int index, const char *text);
int CVIFUNC DeleteTextBoxLine(int panel, int control, int index);
int CVIFUNC DeleteTextBoxLines(int panel, int control, int index, int numLines);
int CVIFUNC GetNumTextBoxLines(int panel, int control, int *count);
int CVIFUNC ResetTextBox(int panel, int control, const char *text);
int CVIFUNC GetTextBoxLineLength(int panel, int control, int lineIndex, int *length);
int CVIFUNC GetTextBoxLineOffset(int panel, int control, int lineIndex, int *byteOffset);
int CVIFUNC GetTextBoxLine(int panel, int control, int lineIndex, char *destBuffer);
int CVIFUNC GetTextBoxLineIndexFromOffset(int panel, int control, int offset, int *lineIndex);

    /*** attribute functions: ***/
int CVIFUNC_C SetCtrlAttribute(int panel, int control, int attribute, ...);
int CVIFUNC_C SetMenuBarAttribute(int menuBar, int control, int attribute, ...);
int CVIFUNC_C SetPanelAttribute(int panel, int attribute, ...);
int CVIFUNC_C SetPrintAttributeEx(int attribute, ...);
#define SetPrintAttribute   SetPrintAttributeEx
int CVIFUNC_C SetSystemPopupsAttribute(int attribute, ...);   
int CVIFUNC_C SetSystemAttribute(int attribute, ...);   
int CVIFUNC GetCtrlAttribute(int panel, int control, int attribute, void *value);
int CVIFUNC GetMenuBarAttribute(int menuBar, int control, int attribute, void *value);
int CVIFUNC GetPanelAttribute(int panel, int attribute, void *value);
int CVIFUNC GetPrintAttribute(int attribute, void *value);
int CVIFUNC GetSystemPopupsAttribute(int attribute, void *value);
int CVIFUNC GetSystemAttribute(int attribute, void *value);
int CVIFUNC GetMonitorAttribute(int monitorId, int attribute, void *attributeValue);

    /*** for monitors: ***/
int CVIFUNC GetMonitorFromPoint (Point p, int *monitorId);
int CVIFUNC GetMonitorFromRect (Rect r, int *monitorId);
int CVIFUNC GetMonitorFromPanel (int panel, int *monitorId);


    /*** for strip charts: ***/
int CVIFUNC SetTraceAttribute(int panel, int control, int traceNum,
                              int attribute, int value);
int CVIFUNC GetTraceAttribute(int panel, int control, int traceNum,
                              int attribute, int *value);

    /*** for graphs: ***/
int CVIFUNC_C SetPlotAttribute(int panel, int control, int plotNum, int attribute,
                               ...);
int CVIFUNC GetPlotAttribute(int panel, int control, int plotNum, int attribute,
                             void *value);
int CVIFUNC SetCursorAttribute(int panel, int control, int cursorNum,
                               int attribute, int value);
int CVIFUNC GetCursorAttribute(int panel, int control, int cursorNum,
                               int attribute, int *value);
int CVIFUNC GetGraphCoordsFromPoint (int panel, int control, Point point,
                                     double *x, double *y);

    /*** printer functions: ***/
int CVIFUNC PrintPanel(int panel, const char *fileName, int fullSize, int scope,
                       int dialog);
int CVIFUNC PrintCtrl(int panel, int control, const char *fileName, int fullSize,
                      int dialog);
int CVIFUNC PrintTextFile (const char *fileName, const char *fileDest);
int CVIFUNC PrintTextBuffer (const char *buffer, const char *fileDest);

    /*** callback function pointer prototypes: ***/
typedef  int (CVICALLBACK * PanelCallbackPtr)(int panel, int event, void *callbackData,
                                              int eventData1, int eventData2);
typedef  int (CVICALLBACK * CtrlCallbackPtr)(int panel, int control, int event,
                                             void *callbackData, int eventData1, int eventData2);
typedef void (CVICALLBACK * MenuCallbackPtr)(int menubar, int menuItem, void *callbackData,
                                             int panel);
typedef void (CVICALLBACK * MenuDimmerCallbackPtr)(int menuBar, int panel);
typedef  int (CVICALLBACK * MainCallbackPtr)(int panelOrMenuBar, int controlOrMenuItem,
                                             int event, void *callbackData, int eventData1, int eventData2);
typedef void (CVICALLBACK * DeferredCallbackPtr)(void *callbackData);
typedef  int (CVICALLBACK * CellCompareCallbackPtr)(int panel, int control, Point item1, Point item2,
                                                    void *callbackData);
typedef  int (CVICALLBACK * TreeCellCompareCallbackPtr)(int panel, int control, int item1, 
                                                        int item2, int keyCol, void *callbackData);
typedef void (CVICALLBACK * CtrlMenuCallbackPtr)(int panel, int control, int menuItem,
                                                 void *callbackData);

#if defined(_NI_mswin16_)
 typedef unsigned short WinMsgWParam;
 typedef unsigned long  WinMsgLParam;
#else
 typedef unsigned       WinMsgWParam;
 typedef unsigned long  WinMsgLParam;
#endif
typedef void (CVICALLBACK * WinMsgCallbackPtr)(WinMsgWParam wParam, WinMsgLParam lParam, void *callbackData);

    /*** for installing callback functions: ***/
int CVIFUNC InstallMainCallback(MainCallbackPtr eventFunction, void *callbackData,
                                int getIdleEvents);
int CVIFUNC InstallCtrlCallback(int panel, int control, CtrlCallbackPtr eventFunction,
                                void *callbackData);
int CVIFUNC InstallPanelCallback(int panel, PanelCallbackPtr eventFunction,
                                 void *callbackData);
int CVIFUNC InstallMenuCallback(int menuBar, int menuOrMenuItem,
                                MenuCallbackPtr eventFunction, void *callbackData);
int CVIFUNC InstallMenuDimmerCallback(int menuBar, MenuDimmerCallbackPtr dimmerFunction);
int CVIFUNC PostDeferredCall(DeferredCallbackPtr funcPtr, void *callbackData);  /* may be called asynchronously */

int CVIFUNC PostDeferredCallToThread(DeferredCallbackPtr funcPtr, void *callbackData, 
                                     unsigned long targetThreadId);  /* may be called asynchronously */

#if defined(_NI_mswin16_) || defined(_NI_mswin32_)
int CVIFUNC RegisterWinMsgCallback(WinMsgCallbackPtr callbackFunc, const char *messageIdentifier,
                                   void *callbackData, int dataSize, int *callbackId,
                                   int deleteWhenUserProgramStops);
int CVIFUNC UnRegisterWinMsgCallback(int callbackId);
int CVIFUNC GetCVIWindowHandle(void);
int CVIFUNC GetCVIWindowHandleForCurrThread(void);
#endif
#if defined(_NI_mswin16_) 
int CVIFUNC GetCVITaskHandle(void);
#endif

    /*** for user interface management: ***/
int CVIFUNC RunUserInterface(void);
int CVIFUNC QuitUserInterface(int returnCode);
int CVIFUNC ProcessSystemEvents(void);
int CVIFUNC ProcessDrawEvents(void);
int CVIFUNC QueueUserEvent(int eventNumber, int panel, int control);

int CVIFUNC GetUserEvent(int wait, int *panel, int *control);     /* returns an event */
int CVIFUNC SetInputMode(int handle, int control, int mode);
int CVIFUNC SetIdleEventRate(int minIntervalInMillisecs);
int CVIFUNC SetSleepPolicy(int sleepPolicy);
int CVIFUNC GetSleepPolicy(void);

    /*** for timer controls ***/
int CVIFUNC ResetTimer(int panel, int control);
int CVIFUNC ResumeTimerCallbacks(void);
int CVIFUNC SuspendTimerCallbacks(void);

    /*** time related functions ***/
int CVIFUNC MakeDateTime(int hour, int minute, double second, int month,
                         int day, int year, double *dateTime);
int CVIFUNC FormatDateTimeString(double dateTime, const char *format, char *buf, int bufLen);
int CVIFUNC GetCurrentDateTime(double *dateTime);
int CVIFUNC GetDateTimeElements(double dateTime, int *hour, int *minute, double *second,
                                                 int *month, int *day, int *year);

    /*** for menu bars: ***/
int CVIFUNC LoadMenuBar(int panel, const char *fileName, int menuBarResourceId); /* returns a menu bar */
int CVIFUNC LoadMenuBarEx(int panel, const char *fileName, int menuBarResourceId, 
                          void *hInstance); /* returns a menu bar */
int CVIFUNC SetPanelMenuBar(int panel,int menuBar);
int CVIFUNC GetPanelMenuBar(int panel);
int CVIFUNC NewMenuBar(int panel);                              /* returns a menu bar */
int CVIFUNC DiscardMenuBar(int menuBar);
int CVIFUNC EmptyMenuBar(int menuBar);
int CVIFUNC GetSharedMenuBarEventPanel(void);                           /* returns a panel */

    /*** for menus: ***/
int CVIFUNC NewMenu(int menuBar, const char *menuTitle, int beforeMenu); /* returns a menu */
int CVIFUNC DiscardMenu(int menuBar, int menu);
int CVIFUNC EmptyMenu(int menuBar, int menu);
int CVIFUNC NewSubMenu(int menuBar, int menuItem);                 /* returns a menu  */
int CVIFUNC DiscardSubMenu(int menuBar, int subMenu);
int CVIFUNC RunPopupMenu(int menuBar, int menu, int panel, int top, int left,
                         int pinTop, int pinLeft, int pinHeight, int pinWidth);

    /*** for menu items: ***/
int CVIFUNC NewMenuItem(int menuBar, int menu, const char *itemString, int beforeMenuItem,
                        int shortCutKey, MenuCallbackPtr eventFunction, void *callbackData);        /* returns a menu item */
int CVIFUNC DiscardMenuItem(int menuBar, int menuItem);
int CVIFUNC InsertSeparator(int menuBar, int menu, int beforeMenuItem);    /* menu item */

    /*** for graph plots: ***/
int CVIFUNC PlotX(int panel, int control, void *xArray, int numPoints,
                  int xDType, int plotStyle, int pointStyle, int lineStyle,
                  int pointFreq, int color);
int CVIFUNC PlotY(int panel, int control, void *yArray, int numPoints,
                  int yDType, int plotStyle, int pointStyle, int lineStyle,
                  int pointFreq, int color);
int CVIFUNC PlotXY(int panel, int control, void *xArray, void *yArray,
                   int numPoints, int xDType, int yDType, int plotStyle,
                   int pointStyle, int lineStyle, int pointFreq, int color);
int CVIFUNC PlotWaveform(int panel, int control, void *wfmArray,
                         int numPoints, int wfmDType, double yGain, double yOffset,
                         double xInit, double xInc, int plotStyle, int pointStyle,
                         int lineStyle, int pointFreq, int color);
int CVIFUNC PlotPoint(int panel, int control, double x, double y, int pointStyle,
                      int color);
int CVIFUNC PlotText(int panel, int control, double x, double y, const char *theText,
                     const char *fontName, int fgColor, int bgColor);
int CVIFUNC PlotLine(int panel, int control, double x1, double y1, double x2,
                     double y2, int color);
int CVIFUNC PlotRectangle(int panel, int control, double x1, double y1, double x2,
                          double y2, int frameColor, int fillColor);
int CVIFUNC PlotPolygon(int panel, int control,  void *xArray,  void *yArray,
                        int numPoints, int xDType, int yDType, int frameColor,
                        int fillColor);
int CVIFUNC PlotOval(int panel, int control, double x1, double y1, double x2,
                     double y2, int frameColor, int fillColor);
int CVIFUNC PlotArc(int panel, int control, double x1, double y1, double x2,
                    double y2, int begAngle, int arcAngle, int frameColor,
                    int fillColor);
int CVIFUNC PlotIntensity(int panel, int control,  void *zArray, int numXPoints,
                          int numYPoints, int zDType, ColorMapEntry colors[], int hiColor,
                          int numColors, int interpColors, int interpPixels);
int CVIFUNC PlotScaledIntensity(int panel, int control,  void *zArray,
                                int numXPoints, int numYPoints, int zDType, double yGain,
                                double yOffset, double xGain, double xOffset,
                                ColorMapEntry colors[], int hiColor, int numColors,
                                int interpColors, int interpPixels);
int CVIFUNC PlotBitmap(int panel, int control, double x, double y,
                       double width, double height, const char *fileName);
int CVIFUNC PlotStripChart(int panel, int control, void *yArray,
                           int numPoints, int start, int skip, int yDType);
int CVIFUNC PlotStripChartPoint(int panel, int control, double y);
int CVIFUNC PlotPackedDigitalLines (int panel, int control, void *data, int numElements, 
                                    int dataType, void *mask);
int CVIFUNC PlotDigitalLines (int panel, int control, void *data, int numPoints, 
                              int dataType, unsigned char numLines);
int CVIFUNC PlotPackedDigitalLinesMultiBus (int panel, int control, void *data, int numElements, 
                                            int numBuses, int dataType, int interleaved, void *maskArray);
int CVIFUNC PlotDigitalLinesMultiBus (int panel, int control, void *data, int numPoints, int numBuses, 
                                      int dataType, int interleaved, unsigned char *numPlotLinesArray);
int CVIFUNC ClearDigitalGraph (int panel, int control);
int CVIFUNC DeleteGraphPlot(int panel, int control, int plot, int timing);
int CVIFUNC ClearStripChart(int panel, int control);
int CVIFUNC RefreshGraph(int panel, int control);
int CVIFUNC ClearLegend(int panel, int control);
int CVIFUNC GetNumLegendItems(int panel, int control, int *numItems);

    /*** for graph scales: ***/
int CVIFUNC SetAxisScalingMode(int panel, int control, int axis,
                               int mode, double min, double max);
int CVIFUNC GetAxisScalingMode(int panel, int control, int axis,
                               int *mode, double *min, double *max);
int CVIFUNC ReplaceAxisItem(int panel, int control, int axis, int index,
                            const char *itemLabel, double itemValue);
int CVIFUNC InsertAxisItem(int panel, int control, int axis, int index,
                           const char *itemLabel, double itemValue);
int CVIFUNC DeleteAxisItem(int panel, int control, int axis, int index,
                           int count);
int CVIFUNC ClearAxisItems(int panel, int control, int axis);
int CVIFUNC GetNumAxisItems(int panel, int control, int axis, int *count);
int CVIFUNC GetAxisItem(int panel, int control, int axis, int index,
                        char *itemLabel, double *itemValue);
int CVIFUNC GetAxisItemLabelLength(int panel, int control, int axis,
                                   int index, int *length);
int CVIFUNC SetAxisRange(int panel, int control, int x_mode, double x_min,
                         double x_max, int y_mode, double y_min, double y_max);
int CVIFUNC GetAxisRange(int panel, int control, int *x_mode, double *x_min,
                         double *x_max, int *y_mode, double *y_min, double *y_max);
int CVIFUNC SetAxisTimeFormat(int panel, int control, int axis, int format, const char *formatString);
int CVIFUNC GetAxisTimeFormat(int panel, int control, int axis, int format, char *outputBuffer, int bufferSize);

    /*** for graph cursors: ***/
int CVIFUNC GetGraphCursor(int panel, int control, int cursorNum, double *x,
                           double *y);
int CVIFUNC SetGraphCursor (int panel, int control, int cursorNum, double x,
                            double y);
int CVIFUNC GetActiveGraphCursor(int panel, int control, int *cursorNum);
int CVIFUNC SetActiveGraphCursor(int panel, int control, int cursorNum);
int CVIFUNC GetGraphCursorIndex(int panel, int control, int cursorNum,
                                int *plot, int *index);
int CVIFUNC SetGraphCursorIndex(int panel, int control, int cursorNum,
                                int plot, int index);

    /*** for graph annotations: ***/
int CVIFUNC GetAnnotationAttribute(int panel, int control, int index, int attribute, void *attributeValue);
int CVIFUNC SetAnnotationAttribute(int panel, int control, int index, int attribute, ...);
int CVIFUNC AddGraphAnnotation(int panel, int control, double xVal, double yVal, const char* captionText, 
                               int xOffset, int yOffset);
int CVIFUNC DeleteGraphAnnotation(int panel, int control, int index);
int CVIFUNC GetAnnotationIndexFromCaption(int panel, int control, const char* captionText, 
                                          int begIndex, int *index);

    /*** for pictures: ***/
int CVIFUNC DisplayImageFile(int panel, int control, const char *fileName);
int CVIFUNC DeleteImage(int panel, int control);
int CVIFUNC SetImageBits(int panel, int control, int imageId, int rowBytes,
                         int depth, int width, int height, int colors[],
                         unsigned char *bits,  unsigned char *mask);
int CVIFUNC GetImageBits(int panel, int control, int imageId, int *rowBytes,
                         int *depth, int *width, int *height, int *colors,
                         unsigned char *bits,  unsigned char *mask);
int CVIFUNC GetImageInfo(int panel, int control, int imageId, int *colorSize,
                         int *bitsSize, int *maskSize);
int CVIFUNC AllocImageBits(int panel, int control, int imageId, int **colors,
                           unsigned char **bits,  unsigned char **mask);

    /*** popup functions: ***/
int CVIFUNC FileSelectPopup(const char defaultDir[], const char defaultSpec[], const char fileTypeList[],
                            const char title[], int buttonLabel, int restrictDir, int restrictExt,
                            int allowCancel, int allowMakeDir, char fileSpec[]);
int CVIFUNC MultiFileSelectPopup(const char defaultDir[], const char defaultSpec[], const char fileTypeList[],
                             const char title[], int restrictDir, int restrictExt,
                              int allowCancel, int *numSelected, char ***fileSpecList);
int CVIFUNC DirSelectPopup (const char defaultDir[], const char title[], int allowCancel,
                            int allowMakeDir, char dirChosen[]);
int CVIFUNC AddToFilePopupDirHistory(char pathString[]);
int CVIFUNC GetFilePopupDirHistory(int *numDirs, char ***dirHistory);
int CVIFUNC ClearFilePopupDirHistory(void);
int CVIFUNC InstallPopup(int popupHandle);
int CVIFUNC RemovePopup(int removeAll);
int CVIFUNC MessagePopup(const char *popupTitle, const char *messageString);
int CVIFUNC ConfirmPopup(const char *popupTitle, const char *messageString);
int CVIFUNC PromptPopup(const char *popupTitle, const char *messageString, char *responseBuffer, int maxResponseLength);
int CVIFUNC GenericMessagePopup(const char *popupTitle, const char *messageString, const char *btn1Label, const char *btn2Label,
                                const char *btn3Label, char *responseBuffer, int maxResponseLength,
                                int putBtnsOnSide, int activeControl, int defaultBtn, int cancelBtn);
int CVIFUNC YGraphPopup(const char *popupTitle, void *yArray, int numPoints, int yDType);
int CVIFUNC XGraphPopup(const char *popupTitle, void *xArray, int numPoints, int xDType);
int CVIFUNC XYGraphPopup(const char *popupTitle, void *xArray, void *yArray, int numPoints, int xDType,
                        int yDType);
int CVIFUNC WaveformGraphPopup(const char *popupTitle, void *wFmArray, int numPoints, int wFmDType,
                               double yGain, double yOffset, double xInit, double xInc);
int CVIFUNC FontSelectPopup(const char *popupTitle, const char *sampleString, int monoSpacedOnly, char *typeFace,int *bold,
                            int *underline, int *strikeOut, int *italic, int *justification, int *textColor,
                            int *fontSize, int minFontSize, int maxFontSize, int allowDefaultBtn, int allowMetaFonts);
int CVIFUNC SetFontPopupDefaults(const char *typeFace, int bold, int underline, int strikeOut, int italic,
                                 int justification, int textColor, int fontSize);


    /*** clipboard functions: ***/
int CVIFUNC ClipboardPutBitmap(int bitmap);
int CVIFUNC ClipboardGetBitmap(int *bitmap, int *available);
int CVIFUNC ClipboardPutText(char *text);
int CVIFUNC ClipboardGetText(char **text, int *available);


    /*** keypress handling functions ***/
int CVIFUNC KeyPressEventIsLeadByte (int eventData2);
int CVIFUNC KeyPressEventIsTrailByte (int eventData2);
int CVIFUNC GetKeyPressEventVirtualKey(int eventData2);
int CVIFUNC GetKeyPressEventCharacter (int eventData2);
int CVIFUNC GetKeyPressEventModifiers(int eventData2);
int CVIFUNC SetKeyPressEventKey(int eventData2, int virtualKey, 
                                int character, int modifiers,
                                int isLeadByte, int isTrailByte);



    /*** miscellaneous functions: ***/
int CVIFUNC MakeColor(int red, int green, int blue);            /* returns a rgb value */
int CVIFUNC Get3dBorderColors(int color, int *highlightColor, int *lightColor, int *shadowColor, int *darkShadowColor);
int CVIFUNC SetWaitCursor(int wait);
int CVIFUNC GetWaitCursorState(void);                           /* returns a boolean */
int CVIFUNC SetMouseCursor(int cursor);
int CVIFUNC GetMouseCursor(int *cursor);
int CVIFUNC GetGlobalMouseState(int *panel, int *x, int *y, int *leftButtonDown,
                                int *rightButtonDown, int *keyModifiers);
int CVIFUNC GetRelativeMouseState(int panel, int control, int *x, int *y,
                                  int *leftButtonDown, int *rightButtonDown, int *keyModifiers);
int CVIFUNC GetScreenSize(int *height, int *width);
int CVIFUNC CreateMetaFont(const char *name, const char *fontName, int size, int bold,
                           int italic, int underline, int strikeout);
int CVIFUNC GetTextDisplaySize(const char *ptr, const char *fontString, int *height, int *width);
int CVIFUNC FakeKeystroke(int fakedKey);
int CVIFUNC MakeApplicationActive(void);
int CVIFUNC SetOwnerForNextCreatedWindow(int ownerHwnd);
char * CVIFUNC GetUILErrorString(int errorCode);
#if defined(_NI_mswin32_)
void CVIFUNC MinimizeAllWindows(void);
#endif
int CVIFUNC GetFontTypefaceName(char *fontName, char *typefaceName);
int CVIFUNC GetFontTypefaceNameLength(char *fontName, int *typefaceNameLength);


    /*** functions provided for LW DOS compatibility: ***/
int CVIFUNC DisplayPCXFile(const char *fileName, int x, int y);
int CVIFUNC ConfigurePrinter(const char *printFile, int orientation, int width,
                             int height, int ejectPage);
int CVIFUNC DOSColorToRGB (int color);
int CVIFUNC DOSCompatWindow(void);

    /*************************************/
    /*** functions for Canvas Controls ***/
    /*************************************/

    /** Drawing functions for use with the canvas control **/
int     CVIFUNC CanvasStartBatchDraw(int panel, int control);
int     CVIFUNC CanvasEndBatchDraw(int panel, int control);

int     CVIFUNC CanvasDrawPoint(int panel, int control, Point p);
int     CVIFUNC CanvasDrawLine(int panel, int control, Point start, Point end);
int     CVIFUNC CanvasDrawLineTo(int panel, int control, Point end);
int     CVIFUNC CanvasDrawRect(int panel, int control, Rect r, int drawMode);
int     CVIFUNC CanvasDimRect(int panel, int control, Rect r);
int     CVIFUNC CanvasDrawRoundedRect (int panel, int control, Rect r, int ovalHeight, int ovalWidth, int drawMode);
int     CVIFUNC CanvasDrawOval(int panel, int control, Rect r, int drawMode);
int     CVIFUNC CanvasDrawArc(int panel, int control, Rect r, int begAngle, int arcAngle, int drawMode);
int     CVIFUNC CanvasDrawPoly(int panel, int control, int numPts, const Point points[], int wrap, int drawMode);
int     CVIFUNC CanvasDrawText(int panel, int control, const char text[], const char metaFont[], Rect bounds, int alignment);
int     CVIFUNC CanvasDrawTextAtPoint(int panel, int control, const char text[], const char metaFont[], Point anchor, int alignment);
int     CVIFUNC CanvasDrawBitmap(int panel, int control, int bitmap, Rect source, Rect destination);    
int     CVIFUNC CanvasScroll(int panel, int control, Rect r, int dx, int dy);
int     CVIFUNC CanvasInvertRect(int panel, int control, Rect r);
int     CVIFUNC CanvasClear(int panel, int canvas, Rect r);

    /** Pen functions for use with the canvas control **/
int     CVIFUNC CanvasGetPenPosition(int panel, int control, Point *penPosition);
int     CVIFUNC CanvasSetPenPosition(int panel, int control, Point penPosition);
int     CVIFUNC CanvasDefaultPen(int panel, int control);

    /** Update functions for use with the canvas control **/
int     CVIFUNC CanvasUpdate(int panel, int control, Rect updateArea);

    /** Clipping functions for use with the canvas control **/
int     CVIFUNC CanvasSetClipRect(int panel, int control, Rect clipRect);
int     CVIFUNC CanvasGetClipRect(int panel, int control, Rect *clipRect);

    /** Bitmap functions **/                                                            
int     CVIFUNC NewBitmap(int rowBytes, int depth, int width, int height, int colors[],
                          unsigned char bits[], unsigned char mask[], int *bitmap);
int     CVIFUNC NewBitmapEx(int rowBytes, int depth, int width, int height, int colors[],
                          unsigned char bits[], unsigned char mask[], unsigned char alpha[], int *bitmap);
int     CVIFUNC GetBitmapFromFile(const char fileName[], int *bitmap);
int     CVIFUNC SaveBitmapToBMPFile(int bitmap, const char fileName[]);
int     CVIFUNC SaveBitmapToJPEGFile(int bitmap, const char fileName[], unsigned long options, int quality);
int     CVIFUNC SaveBitmapToPNGFile(int bitmap, const char fileName[]);
int     CVIFUNC GetCtrlBitmap(int panel, int control, int imageId, int *bitmap);
int     CVIFUNC DiscardBitmap(int bitmap);
int     CVIFUNC DuplicateBitmap(int sourceBitmap, int *destinationBitmap);
int     CVIFUNC GetCtrlDisplayBitmap(int panel, int control, int includeLabel, int *bitmap);
int     CVIFUNC GetScaledCtrlDisplayBitmap(int panel, int control, int includeLabel, int height, int width, int *bitmap);
int     CVIFUNC GetPanelDisplayBitmap(int panel, int scope, Rect area, int *bitmap);
int     CVIFUNC GetScaledPanelDisplayBitmap(int panel, int scope, Rect area, int height, int width, int *bitmap);
int     CVIFUNC SetCtrlBitmap(int panel, int control, int imageId, int bitmap);
int     CVIFUNC SetBitmapData (int bitmap, int rowBytes, int depth, int colors[],
                               unsigned char bits[], unsigned char mask[]);
int     CVIFUNC SetBitmapDataEx (int bitmap, int rowBytes, int depth, int colors[],
                               unsigned char bits[], unsigned char mask[], unsigned char alpha[]);
int     CVIFUNC GetBitmapData(int bitmap, int *rowBytes, int *depth, int *width,
                              int *height, int *colors, unsigned char *bits, unsigned char *mask);
int     CVIFUNC GetBitmapDataEx(int bitmap, int *rowBytes, int *depth, int *width,
                              int *height, int *colors, unsigned char *bits, unsigned char *mask,
                unsigned char *alpha);
int     CVIFUNC GetBitmapInfo(int bitmap, int *colorSize, int *bitsSize,
                              int *maskSize);
int     CVIFUNC GetBitmapInfoEx(int bitmap, int *colorSize, int *bitsSize,
                              int *maskSize, int *alphaSize);
int     CVIFUNC AllocBitmapData(int bitmap, int **colors, unsigned char **bits,
                                unsigned char **mask);
int     CVIFUNC AllocBitmapDataEx(int bitmap, int **colors, unsigned char **bits,
                                unsigned char **mask, unsigned char **alpha);

    /** Pixel value functions for use with the canvas control ***/  
int     CVIFUNC CanvasGetPixel(int panel, int control, Point p, int *pixel);
int     CVIFUNC CanvasGetPixels(int panel, int control, Rect r, int pixels[]);
    
    /*** Rectangle and Point functions ***/

        /*** Creating and modifying Rectangles and Points ***/
Rect    CVIFUNC MakeRect(int top, int left, int height, int width);
void    CVIFUNC RectSet(Rect *r, int top, int left, int height, int width);
void    CVIFUNC RectSetFromPoints(Rect *r, Point p1, Point p2); 
void    CVIFUNC RectSetBottom(Rect *r, int bottom);
void    CVIFUNC RectSetRight(Rect *r, int right); 
void    CVIFUNC RectSetCenter(Rect *r, Point centerPoint);
void    CVIFUNC RectOffset(Rect *r, int dx, int dy);
void    CVIFUNC RectMove(Rect *r, Point p);
void    CVIFUNC RectGrow(Rect *r, int dx, int dy);
Point   CVIFUNC MakePoint(int x, int y);
void    CVIFUNC PointSet(Point *p, int x, int y);

        /*** Retrieving and comparing Rect and Point values ***/
int     CVIFUNC RectBottom(Rect r);
int     CVIFUNC RectRight(Rect r);
void    CVIFUNC RectCenter(Rect r, Point *centerPoint);
int     CVIFUNC RectEqual(Rect r1, Rect r2);
int     CVIFUNC RectEmpty(Rect r);
int     CVIFUNC RectContainsPoint(Rect r, Point p);
int     CVIFUNC RectContainsRect(Rect container, Rect containee);
int     CVIFUNC RectSameSize(Rect r1, Rect r2);
void    CVIFUNC RectUnion(Rect r1, Rect r2, Rect *unionRect);
int     CVIFUNC RectIntersection(Rect r1, Rect r2, Rect *intersectionRect); 
int     CVIFUNC PointEqual(Point p1, Point p2);
void    CVIFUNC PointPinnedToRect(Point p, Rect r, Point *pinnedPoint);

    /************************************/
    /*** functions for table controls ***/
    /************************************/

int     CVIFUNC GetNumTableRows (int panel, int control, int *numRows);
int     CVIFUNC GetNumTableColumns (int panel, int control, int *numColumns);
int     CVIFUNC_C SetTableRowAttribute (int panel, int control, int rowIndex, int attribute, ...);
int     CVIFUNC_C SetTableColumnAttribute (int panel, int control, int columnIndex, int attribute,
                                         ...);
int     CVIFUNC_C SetTableCellAttribute (int panel, int control, Point cell, int attribute, ...);
int     CVIFUNC_C SetTableCellRangeAttribute (int panel, int control, Rect cellRange, int attribute,
                                            ...);
int     CVIFUNC GetTableRowAttribute (int panel, int control, int rowIndex, int attribute,
                                      void *value);
int     CVIFUNC GetTableColumnAttribute (int panel, int control, int columnIndex, int attribute,
                                         void *value);
int     CVIFUNC GetTableCellAttribute (int panel, int control, Point cell, int attribute,
                                       void *value);
int     CVIFUNC InsertTableRows (int panel, int control, int rowIndex, int numRows, int cellType);
int     CVIFUNC InsertTableColumns (int panel, int control, int columnIndex, int numColumns,
                                    int cellType);
int     CVIFUNC DeleteTableRows (int panel, int control, int rowIndex, int numRows);
int     CVIFUNC DeleteTableColumns (int panel, int control, int columnIndex, int numColumns);
int     CVIFUNC_C GetTableCellFromValue (int panel, int control, Point begCell, Rect cellRange,
                                       Point *cell, int direction, int cellType, int dataType, ...);
int     CVIFUNC GetTableRowFromLabel (int panel, int control, int begIndex, int *rowIndex,
                                      const char *label, int visibleLabelsOnly);
int     CVIFUNC GetTableColumnFromLabel (int panel, int control, int begIndex, int *columnIndex,
                                         const char *label, int visibleLabelsOnly);
int     CVIFUNC_C SetTableCellVal (int panel, int control, Point cell, ...);
int     CVIFUNC SetTableCellValFromIndex (int panel, int control, Point cell, int index);
int     CVIFUNC SetTableCellRangeVals (int panel, int control, Rect cellRange, void *valArray,
                                       int direction);
int     CVIFUNC_C FillTableCellRange (int panel, int control, Rect cellRange, ...);
int     CVIFUNC GetTableCellVal (int panel, int control, Point cell, void *value);
int     CVIFUNC GetTableCellRangeVals (int panel, int control, Rect cellRange, void *valArray,
                                       int direction);
int     CVIFUNC GetTableCellValLength (int panel, int control, Point cell, int *length);
int     CVIFUNC FreeTableValStrings (void *valArray, int count);
int     CVIFUNC GetTableCellFromPoint (int panel, int control, Point coordinate, Point *cell);
int     CVIFUNC GetTableCellRangeRect (int panel, int control, Rect cellRange, Rect *coordinates);
int     CVIFUNC GetActiveTableCell (int panel, int control, Point *cell);
int     CVIFUNC SetActiveTableCell (int panel, int control, Point cell);
int     CVIFUNC GetTableSelection (int panel, int control, Rect *cellRange);
int     CVIFUNC SetTableSelection (int panel, int control, Rect cellRange);
int     CVIFUNC SortTableCells (int panel, int control, Rect cellRange, int direction, int keyIndex,
                                int descending, CellCompareCallbackPtr comparisonFunction,
                                void *callbackData);
int     CVIFUNC ClipboardPutTableVals (int panel, int control, Rect cellRange);
int     CVIFUNC ClipboardGetTableVals (int panel, int control, Rect cellRange, int *available);
int     CVIFUNC GetNumTableCellRingItems (int panel, int control, Point cell, int *numItems);
int     CVIFUNC InsertTableCellRingItem (int panel, int control, Point cell, int index,
                                         const char *value);
int     CVIFUNC InsertTableCellRangeRingItem (int panel, int control, Rect cellRange, int index,
                                              const char *value);
int     CVIFUNC DeleteTableCellRingItems (int panel, int control, Point cell, int index, int numItems);
int     CVIFUNC DeleteTableCellRangeRingItems (int panel, int control, Rect cellRange, int index,
                                               int numItems);
int     CVIFUNC GetTableCellRingIndexFromValue (int panel, int control, int begIndex, Point cell,
                                                int *index, const char* value); 
int     CVIFUNC GetTableCellRingValueFromIndex (int panel, int control, Point cell, int index,
                                                char *value); 
int     CVIFUNC GetTableCellRingValueLengthFromIndex (int panel, int control, Point cell, int index,
                                                      int *length);



    /***********************************/
    /*** functions for tree controls ***/
    /***********************************/
int     CVIFUNC_C InsertTreeItem (int panel, int control, int relation, int relativeIndex, int position,
                                const char *label, const char *tagBase, char *tag, ...);
int     CVIFUNC GetTreeItemTag (int panel, int control, int index, char *tag);
int     CVIFUNC GetNumTreeItems (int panel, int control, int relation, int relativeIndex, 
                                    int begIndex, int direction, int stateCriteria, int *numItems);
int     CVIFUNC SetActiveTreeItem (int panel, int control, int index, int selectionEffect);
int     CVIFUNC GetActiveTreeItem (int panel, int control, int *index);
int     CVIFUNC IsTreeItemExposed (int panel, int control, int item, int *exposed);
int     CVIFUNC_C SetTreeItemAttribute (int panel, int control, int index, int attribute, ...);
int     CVIFUNC GetTreeItemAttribute (int panel, int control, int index, int attribute, void *value);
int     CVIFUNC AddTreeImage (int panel, int control, int bitmap);
int     CVIFUNC MoveTreeItem (int panel, int control, int index, int relation, int relativeIndex, int position);
int     CVIFUNC CopyTreeItem (int panelSrc, int controlSrc, int indexSrc, int panelDst, 
                              int controlDst, int relationDst, int relativeIndexDst, int positionDst);
int     CVIFUNC MakeTreeItemVisible (int panel, int control, int item);
int     CVIFUNC AreTreeItemsRelated (int panel, int control, int item, int relation, int relativeIndex, 
                                     int *related);
int     CVIFUNC GetTreeItem (int panel, int control, int relation, int relativeIndex, int begIndex, int direction, 
                             int stateCriteria, int *index);
int     CVIFUNC GetTreeItemFromLabel (int panel, int control, int relation, int relativeIndex, int begIndex, 
                                      int direction, int stateCriteria, const char *label, int *index);
int     CVIFUNC_C GetTreeItemFromValue (int panel, int control, int relation, int relativeIndex, int begIndex, 
                                      int direction, int stateCriteria, int *index, ...);
int     CVIFUNC GetTreeItemFromTag (int panel, int control, const char *tag, int *index);
int     CVIFUNC GetTreeItemParent (int panel, int control, int index, int *parentIndex);
int     CVIFUNC GetTreeItemNumChildren (int panel, int control, int index, int *numChildren);
int     CVIFUNC GetTreeItemNumDescendents (int panel, int control, int index, int *numDescendents);
int     CVIFUNC GetTreeItemLevel (int panel, int control, int index, int *level);
int     CVIFUNC InsertTreeColumn (int panel, int control, int columnIndex, const char *label);
int     CVIFUNC DeleteTreeColumn (int panel, int control, int columnIndex);
int     CVIFUNC GetNumTreeColumns (int panel, int control, int *numColumns);
int     CVIFUNC GetTreeCellAttribute (int panel, int control, int item, int column, 
                                      int attribute, void *value);
int     CVIFUNC_C SetTreeCellAttribute (int panel, int control, int item, int column, 
                                      int attribute, ...);
int     CVIFUNC GetTreeColumnAttribute (int panel, int control, int column, 
                                      int attribute, void *value);
int     CVIFUNC_C SetTreeColumnAttribute (int panel, int control, int column, 
                                      int attribute, ...);
int     CVIFUNC SetColumnWidthToWidestCellContents (int panel, int control, int columnIndex);

int     CVIFUNC GetTreeItemPathLength (int panel, int control, int index, const char *separator, int *length);
int     CVIFUNC GetTreeItemPath (int panel, int control, int index, const char *separator, char *path);
int     CVIFUNC SortTreeItems (int panel, int control, int siblingIndex, int keyCol, int descending, int sortSubtrees,
                               TreeCellCompareCallbackPtr comparisonFunction, void *callbackData);

    /***************************************/
    /*** functions for splitter controls ***/
    /***************************************/
int     CVIFUNC AddCtrlToSplitter (int panel, int control, int anchor, int attachedControl,
                                   int snapOnAttach, int move);
int     CVIFUNC AddPanelToSplitter (int panel, int control, int anchor, int attachedPanel,
                                    int snapOnAttach, int move);
int     CVIFUNC RemoveCtrlFromSplitter (int panel, int control, int attachedControl);
int     CVIFUNC RemovePanelFromSplitter (int panel, int control, int attachedPanel);
int     CVIFUNC OperateSplitter (int panel, int control, int delta);

    /**********************************/
    /*** functions for tab controls ***/
    /**********************************/
int     CVIFUNC InsertTabPage(int panel, int control, int index, const char *label);
int     CVIFUNC InsertPanelAsTabPage(int panel, int control, int index, int panelToInsert);
int     CVIFUNC CopyTabPage(int panelSrc, int controlSrc, int indexSrc, 
                            int panelDst, int controlDst, int indexDst);
int     CVIFUNC MoveTabPage(int panel, int control, int index, int indexDst);
int     CVIFUNC DeleteTabPage(int panel, int control, int index, int count);
int     CVIFUNC GetPanelHandleFromTabPage(int panel, int control, int index, int *tabPanel);
int     CVIFUNC GetNumTabPages(int panel, int control, int *numPages);
int     CVIFUNC SetActiveTabPage(int panel, int control, int index);
int     CVIFUNC GetActiveTabPage(int panel, int control, int *index);
int     CVIFUNC_C SetTabPageAttribute(int panel, int control, int index, int attribute, ...);
int     CVIFUNC GetTabPageAttribute(int panel, int control, int index, int attribute, void *value);
int     CVIFUNC GetTabPageFromPoint(int panel, int control, Point coordinate, int *index, int *area);

    /***********************************/
    /***  functions for databinding  ***/
    /***********************************/
typedef int DSHandle;

int     CVIFUNC DSBindCtrl (int panel, int control, char *URL, int accessMode, 
                            DSHandle *dsHandle, HRESULT *dsError);
int     CVIFUNC DSBindPlot(int panel, int control, int plotId, char *URL, 
                           DSHandle *dsHandle, HRESULT *dsError);
int     CVIFUNC DSUnbind(int panel, int control, HRESULT *dsError);
int     CVIFUNC DSGetBoundPlotID(int panel, int control, int *plot);
int     CVIFUNC DSBindTableCellRange(int panel, int control, Rect cellRange, char *URL, 
                                     int accessMode, DSHandle *dsHandle, HRESULT *dsError);


    /***********************************/
    /*** functions for control menus ***/
    /***********************************/
int     CVIFUNC NewCtrlMenuItem (int panel, int control, const char *itemLabel, int beforeMenuItem,
                                 CtrlMenuCallbackPtr eventFunction, void *callbackData);
int     CVIFUNC NewCtrlMenuSeparator (int panel, int control, int beforeMenuItem);
int     CVIFUNC DiscardCtrlMenuItem (int panel, int control, int menuItem);
int     CVIFUNC HideBuiltInCtrlMenuItem (int panel, int control, int menuItem);
int     CVIFUNC ShowBuiltInCtrlMenuItem (int panel, int control, int menuItem, int beforeMenuItem);
int     CVIFUNC_C SetCtrlMenuAttribute (int panel, int control, int menuItem, int attribute, ...);
int     CVIFUNC GetCtrlMenuAttribute (int panel, int control, int menuItem, int attribute,
                                          void *value);

extern void *__CVIUserHInst;


#ifdef __cplusplus
    }
#endif

#endif  /* USERINT_HEADER */

/******************************************************************************/
