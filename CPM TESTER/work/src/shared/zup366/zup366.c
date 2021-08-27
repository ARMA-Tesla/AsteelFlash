/*****************************************************************************
 *  2004-2015, National Instruments, Corporation.  All Rights Reserved.    *
 *****************************************************************************/

/*****************************************************************************
 *  Agilent N6700 DC Power Supply Instrument Driver
 *  LabWindows/CVI Instrument Driver
 *  Original Release: 8. March 2004
 *  By: Petr Marcalik, ELCOM, a.s.
 *      PH. +420 597 326 930   Fax +420 597 326 921
 *
 *  Modification History:
 *
 *  2015-07-25 [ZSU] Revision 3.2
 *					 - Added support for new modules (N6785A, N6786A).
 *					 - Added support for 8 pin digital IO port in zup366_ConfigurePin.
 *					 - Added new attributes:
 *					   ZUP366_ATTR_ELOG_PERIOD
 *					   ZUP366_ATTR_ENABLE_VOLTAGE_MINMAX_ELOG
 *					   ZUP366_ATTR_ENABLE_CURRENT_MINMAX_ELOG
 *					   ZUP366_ATTR_ENABLE_VOLTAGE_ELOG
 *					   ZUP366_ATTR_ENABLE_CURRENT_ELOG
 *					   ZUP366_ATTR_MEASUREMENT_RESOLUTION
 *					   ZUP366_ATTR_ELOG_TRIGGER_SOURCE
 *					   ZUP366_ATTR_ELOG_VOLTAGE_RANGE
 *					   ZUP366_ATTR_ELOG_CURRENT_RANGE
 *					   ZUP366_ATTR_AUTO_ELOG_VOLTAGE_RANGE
 *					   ZUP366_ATTR_AUTO_ELOG_CURRENT_RANGE
 *					 - Added new functions:    
 *					   zup366_AbortExternalDatalog
 *					   zup366_FetchExternalDatalog
 *					   zup366_InitiateExternalDatalog
 *					   zup366_ImmediateExternalDatalogTrigger
 *					   zup366_ConfigureExternalDatalogTriggerSource
 *					   zup366_ConfigureMeasurementResolution
 *					   zup366_ConfigureExternalDatalog
 *
 *  2015-03-19 [Yuchen Zhang] Revision 3.1
 *					Updated driver to comply with IVI-2014 generation.
 *
 *	2012-06-25 [JP] Revision 3.0
 *					Add support for new N6705B model and modules N6755A,
 *					N6756A, N6763A, N6764A, N6765A, N6766A, N6777A,
 *					N6781A, N6782A, N6783A-BAT, and N6784A.
 *					New functionality added for Waveform, Sequence, SMU,
 *					Datalog, and Histogram.
 *					Fixed issues in Abort, Abort Acquisition, and
 *					Initiate when double modules are installed.
 *					For the N6705B now supports empty slots modifications
 *					also made to zup366_IviInit and
 *					zup366_GetInstrumentSetupOption to support this feature.
 *
 *  2011-07-08 [hfeng] Revision 2.11
 *				 - Fix a bug: infinite loop in attribute
 *				   ZUP366_ATTR_INSTR_MODULE_OPTIONS
 *
 *  2010-05-12 [Joe Ju] Revision 2.10
 *				 - Port to 64-bit
 *
 *  2008-07-07 [KP] Revision 2.9
 *               - Fixed bug in zup366AttrVoltageLevel_CoerceCallback
 *				   coercedValue does not set when value is correct
 *
 *  2008-04-14 [Joe Ju] Revision 2.8
 *               - Coerced voltage level to the minimum value if user inputs a
 *                 value smaller than the minimum value
 *
 *  2008-04-14 [EZ] Revision 2.7
 *               - Coerced the channel name before getting channel_index in
 *                 zup366_QueryMaxCurrentLimit
 *                 zup366_QueryMaxVoltageLevel
 *                 zup366_ConfigureVoltageList
 *                 zup366_ConfigureCurrentList
 *
 *  2008-03-12 [Joe Ju] Revision 2.6
 *			     - Changed ESupportedModules to zup366_ESupportedModules.
 *
 *  2008-01-08 [Joe Ju] Revision 2.5
 *               - Removed Range Check in ConfigureOutputRange.
 *               - Modified coerce logics of ZUP366_ATTR_INSTR_VOLTAGE_RANGE
 *                 and ZUP366_ATTR_INSTR_CURRENT_RANGE.
 *
 *  2008-01-08 [Joe Ju] Revision 2.4
 *			     - Added support for double width modules (N6753A, N6754A).
 *
 *  2007-04-05 [EZ] Revision 2.2
 *               - Driver supports new Agilent modules: N6753A, N6754A
 *               - Added zup366_ConfigureRelayPolarity
 *               - Added zup366_LimitChannelPower
 *
 *	2005-12-12 [LH] Revision 1.5
 *			     - Driver supports new Agilent models: N6773A, N6774A, N6775A, N6776A.
 *				 - Added new functions for group/ungroup channels.
 *
 *	2005-11-11 [LH] Revision 1.41
 *				 Fixed channel numbering in initialization.
 *
 *	2005-10-27 [LH] Revision 1.4
 *				 Fixed initialization when any module slots in the instrument are empty.
 *
 *  2005-05-16 [LH] Revision 1.3
 *			     - Driver supports Agilent N6700B series.
 *             Fixed:
 *               - zup366_IviInit: Default installed_modules fixed.
 *               - max current range for N6745.
 *             Made a slight modification in this functions:
 *               - zup366_ConfigureOutputRange
 *               - zup366_ConfigureVoltageList
 *               - zup366_ConfigureCurrentList
 *               - zup366_ConfigureMeasurementRange
 *             Updated Help and some default values.
 *
 *	2005-04-20 [VJ] Revision 1.2 Fixed IDQuery.
 *
 *	2004-07-13 [ZR] Revision 1.1
 *             Fixed:
 *               zup366_IviInit: Simulation fixed.
 *
 *  2004-03-05 [ZR] Instrument Driver Created.
 *
 *****************************************************************************/

//#include <utility.h>
#include <utility.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "zup366.h"

/*****************************************************************************
 *--------------------- Hidden Attribute Declarations -----------------------*
 *****************************************************************************/

#define ZUP366_ATTR_OPC_TIMEOUT                        (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 1L)  /* ViInt32 */
#define ZUP366_ATTR_VISA_RM_SESSION                    (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 5L)  /* ViSession */

#define ZUP366_ATTR_OPC_CALLBACK                       (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 6L)  /* ViAddr */
#define ZUP366_ATTR_CHECK_STATUS_CALLBACK              (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 7L)  /* ViAddr */
#define ZUP366_ATTR_USER_INTERCHANGE_CHECK_CALLBACK    (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 8L)  /* ViAddr */
#define ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES    (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 10L) /* ViAddr */

#define ZUP366_ATTR_INSTR_VOLTAGE_RANGE                (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 11L) /* ViReal64 */
#define ZUP366_ATTR_INSTR_CURRENT_RANGE                (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 12L) /* ViReal64 */
#define ZUP366_ATTR_INSTR_VOLTAGE_MEASUREMENT_RANGE    (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 13L) /* ViReal64 */
#define ZUP366_ATTR_INSTR_CURRENT_MEASUREMENT_RANGE    (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 14L) /* ViReal64 */

#define ZUP366_ATTR_INSTR_MODULE_OPTIONS               (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 15L) /* ViInt32 */
#define ZUP366_ATTR_INSTR_CHANNEL_COUNT                (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 16L) /* ViInt32 */

#define MAX_MODULE_COUNT                                (4)

/**
 * Define supported instrument modules.
 */
enum
{
    NOT_SUPPORTED = -1,
    N6731A        = 6731,
    N6732A,
    N6733A,
    N6734A,
    N6735A,
    N6742A = 6742,
    N6743A,
    N6744A,
    N6745A,
    N6751A = 6751,
    N6752A,
    N6753A,
    N6754A,
    N6755A,
    N6756A,
    N6761A = 6761,
    N6762A,
    N6763A,
    N6764A,
    N6765A,
    N6766A,
    N6773A = 6773,
    N6774A,
    N6775A,
    N6776A,
    N6777A,
    N6781A = 6781,
    N6782A,
    N6783A,
    N6784A,
    N6785A,
    N6786A,

    N6731B = 67310,
    N6732B,
    N6733B,
    N6734B,
    N6735B,
    N6736B,
    N6741B = 67410,
    N6742B,
    N6743B,
    N6744B,
    N6745B,
    ZUP366
} zup366_ESupportedModules;

/*** Define supported instrument options. ***/
enum
{
    OPT_054 = 0,   /* 0 - bit (High Speed Test Extensions)*/
    OPT_761,       /* 1 - bit (Output Disconnect/Polarity Reversal Relays)*/
    OPT_760 = 2,   /* 2 - bit (Output Disconnect Relays, Standard for SMU Modules)*/
} ESupportedOptions;

/**
 * Define table of the supported instrument modules and their corresponding strings
 */
static IviStringValueTable zup366_ModuleTable =
{
    { N6731A,  "N6731A" },
    { N6732A,  "N6732A" },
    { N6733A,  "N6733A" },
    { N6734A,  "N6734A" },
    { N6735A,  "N6735A" },
    { N6742A,  "N6742A" },
    { N6743A,  "N6743A" },
    { N6744A,  "N6744A" },
    { N6745A,  "N6745A" },
    { N6751A,  "N6751A" },
    { N6752A,  "N6752A" },
    { N6753A,  "N6753A" },
    { N6754A,  "N6754A" },
    { N6755A,  "N6755A" },
    { N6756A,  "N6756A" },
    { N6761A,  "N6761A" },
    { N6762A,  "N6762A" },
    { N6763A,  "N6763A" },
    { N6764A,  "N6764A" },
    { N6765A,  "N6765A" },
    { N6766A,  "N6766A" },
    { N6773A,  "N6773A" },
    { N6774A,  "N6774A" },
    { N6775A,  "N6775A" },
    { N6776A,  "N6776A" },
    { N6777A,  "N6777A" },
    { N6781A,  "N6781A" },
    { N6782A,  "N6782A" },
    { N6783A,  "N6783A" },
    { N6784A,  "N6784A" },
    { N6731B,  "N6731B" },
    { N6732B,  "N6732B" },
    { N6733B,  "N6733B" },
    { N6734B,  "N6734B" },
    { N6735B,  "N6735B" },
    { N6736B,  "N6736B" },
    { N6741B,  "N6741B" },
    { N6742B,  "N6742B" },
    { N6743B,  "N6743B" },
    { N6744B,  "N6744B" },
    { N6745B,  "N6745B" },
    { N6785A,  "N6785A" },
    { ZUP366,  "ZUP366" },
    { VI_NULL, VI_NULL  }
};

static IviStringValueTable zup366_OptionTable =
{
    { OPT_761, "Opt 761" },
    { OPT_054, "Opt 054" },
    { OPT_760, "Opt 760" },
    { VI_NULL, VI_NULL   }
};

#define IS_OPT_054(options) \
    ((options & (1L << OPT_054)) != 0)

#define IS_OPT_760(options) \
    ((options & (1L << OPT_760)) != 0)

#define MAX_NB_RANGES    2

/**
 * Define structure with ranges for instrument modules.
 */
typedef struct
{
    ViReal64 MinCurrent[MAX_NB_RANGES];
    ViReal64 MaxCurrent[MAX_NB_RANGES];
    ViReal64 MinVoltage[MAX_NB_RANGES];
    ViReal64 MaxVoltage[MAX_NB_RANGES];
    ViReal64 MaxPower;
    ViReal64 MaxOVP;
    ViInt32  ModelType;
} SInstrumentRanges, *SInstrumentRangesPtr;

static SInstrumentRanges gs_N6732Ranges =
{
    {  0.01,  0.01 }, /* Min Current */
    { 6.375, 6.375 }, /* Max Current */
    {  0.02,  0.02 }, /* Min Voltage */
    {  8.16,  8.16 }, /* Max Voltage */
    52.02,            /* Max Power   */
    10.0,             /* Max OVP     */
    N6732A,           /* Model Type  */
};

static SInstrumentRanges gs_N6733Ranges =
{
    { 0.01, 0.01 }, /* Min Current */
    { 2.55, 2.55 }, /* Max Current */
    { 0.02, 0.02 }, /* Min Voltage */
    { 20.4, 20.4 }, /* Max Voltage */
    52.02,          /* Max Power     */
    22.0,           /* Max OVP     */
    N6733A,         /* Model Type  */
};

static SInstrumentRanges gs_N6734Ranges =
{
    { 0.01, 0.01 }, /* Min Current */
    { 1.53, 1.53 }, /* Max Current */
    { 0.02, 0.02 }, /* Min Voltage */
    { 35.7, 35.7 }, /* Max Voltage */
    54.621,         /* Max Power   */
    38.5,           /* Max OVP	 */
    N6734A,         /* Model Type  */
};

static SInstrumentRanges gs_N6735Ranges =
{
    {  0.01,  0.01 }, /* Min Current */
    { 0.816, 0.816 }, /* Max Current */
    {  0.02,  0.02 }, /* Min Voltage */
    {  51.0,  51.0 }, /* Max Voltage */
    41.616,           /* Max Power   */
    55.0,             /* Max OVP     */
    N6735A,           /* Model Type  */
};

static SInstrumentRanges gs_N6742Ranges =
{
    { 0.01, 0.01 }, /* Min Current */
    { 10.2, 10.2 }, /* Max Current */
    { 0.02, 0.02 }, /* Min Voltage */
    { 8.16, 8.16 }, /* Max Voltage */
    83.232,         /* Max Power   */
    10.0,           /* Max OVP     */
    N6742A,         /* Model Type  */
};

static SInstrumentRanges gs_N6743Ranges =
{
    { 0.01, 0.01 }, /* Min Current */
    {  5.1,  5.1 }, /* Max Current */
    { 0.02, 0.02 }, /* Min Voltage */
    { 20.4, 20.4 }, /* Max Voltage */
    104.04,         /* Max Power   */
    22.0,           /* Max OVP     */
    N6743A,         /* Model Type  */
};

static SInstrumentRanges gs_N6744Ranges =
{
    { 0.01, 0.01 }, /* Min Current */
    { 3.06, 3.06 }, /* Max Current */
    { 0.02, 0.02 }, /* Min Voltage */
    { 35.7, 35.7 }, /* Max Voltage */
    109.242,        /* Max Power   */
    38.5,           /* Max OVP     */
    N6744A,         /* Model Type  */
};

static SInstrumentRanges gs_N6745Ranges =
{
    { 0.01, 0.01 }, /* Min Current */
    { 1.63, 1.63 }, /* Max Current */
    { 0.02, 0.02 }, /* Min Voltage */
    { 51.0, 51.0 }, /* Max Voltage */
    83.232,         /* Max Power   */
    55.0,           /* Max OVP     */
    N6745A,         /* Model Type  */
};

static SInstrumentRanges gs_N6751Ranges =
{
    { 0.01, 0.01 }, /* Min Current */
    {  5.1,  5.1 }, /* Max Current */
    { 0.02, 0.02 }, /* Min Voltage */
    { 51.0, 51.0 }, /* Max Voltage */
    50.0,           /* Max Power   */
    55.0,           /* Max OVP     */
    N6751A,         /* Model Type  */
};

static SInstrumentRanges gs_N6752Ranges =
{
    { 0.01, 0.01 }, /* Min Current */
    { 10.2, 10.2 }, /* Max Current */
    { 0.02, 0.02 }, /* Min Voltage */
    { 51.0, 51.0 }, /* Max Voltage */
    100.0,          /* Max Power   */
    55.0,           /* Max OVP     */
    N6752A,         /* Model Type  */
};

static SInstrumentRanges gs_N6753Ranges =
{
    { 0.05, 0.05 }, /* Min Current */
    { 51.0, 51.0 }, /* Max Current */
    { 0.01, 0.01 }, /* Min Voltage */
    { 20.4, 20.4 }, /* Max Voltage */
    300.0,          /* Max Power   */
    22.0,           /* Max OVP     */
    N6753A,         /* Model Type  */
};

static SInstrumentRanges gs_N6754Ranges =
{
    {  0.02,  0.02 }, /* Min Current */
    {  20.4,  20.4 }, /* Max Current */
    { 0.025, 0.025 }, /* Min Voltage */
    {  61.2,  61.2 }, /* Max Voltage */
    300.0,            /* Max Power   */
    66.0,             /* Max OVP     */
    N6754A,           /* Model Type  */
};

static SInstrumentRanges gs_N6755Ranges =
{
    { 0.05, 0.05 }, /* Min Current */
    { 51.0, 51.0 }, /* Max Current */
    { 0.01, 0.01 }, /* Min Voltage */
    { 20.4, 20.4 }, /* Max Voltage */
    500.0,          /* Max Power   */
    22.0,           /* Max OVP     */
    N6755A,         /* Model Type  */
};

static SInstrumentRanges gs_N6756Ranges =
{
    {  0.02,  0.02 }, /* Min Current */
    {  17.3,  17.3 }, /* Max Current */
    { 0.025, 0.025 }, /* Min Voltage */
    {  61.2,  61.2 }, /* Max Voltage */
    500.0,            /* Max Power   */
    66.0,             /* Max OVP     */
    N6756A,           /* Model Type  */
};

static SInstrumentRanges gs_N6761Ranges =
{
    { 0.1e-3, 0.001 }, /* Min Current */
    {    0.1,  1.53 }, /* Max Current */
    {  0.012, 0.015 }, /* Min Voltage */
    {    5.5,  51.0 }, /* Max Voltage */
    78.03,             /* Max Power   */
    55.0,              /* Max OVP     */
    N6761A,            /* Model Type  */
};

static SInstrumentRanges gs_N6762Ranges =
{
    { 0.1e-3, 0.001 }, /* Min Current */
    {    0.1,  3.06 }, /* Max Current */
    {  0.012, 0.015 }, /* Min Voltage */
    {    5.5,  51.0 }, /* Max Voltage */
    100.0,             /* Max Power   */
    55.0,              /* Max OVP     */
    N6762A,            /* Model Type  */
};

static SInstrumentRanges gs_N6763Ranges =
{
    { 0.05, 0.05 }, /* Min Current */
    { 51.0, 51.0 }, /* Max Current */
    { 0.01, 0.01 }, /* Min Voltage */
    { 20.4, 20.4 }, /* Max Voltage */
    300.0,          /* Max Power   */
    22.0,           /* Max OVP     */
    N6763A,         /* Model Type  */
};

static SInstrumentRanges gs_N6764Ranges =
{
    {  0.02,  0.02 }, /* Min Current */
    {  20.4,  20.4 }, /* Max Current */
    { 0.025, 0.025 }, /* Min Voltage */
    {  61.2,  61.2 }, /* Max Voltage */
    300.0,            /* Max Power   */
    66.0,             /* Max OVP     */
    N6764A,           /* Model Type  */
};

static SInstrumentRanges gs_N6765Ranges =
{
    { 0.05, 0.05 }, /* Min Current */
    { 51.0, 51.0 }, /* Max Current */
    { 0.01, 0.01 }, /* Min Voltage */
    { 20.4, 20.4 }, /* Max Voltage */
    500.0,          /* Max Power   */
    22.0,           /* Max OVP     */
    N6765A,         /* Model Type  */
};

static SInstrumentRanges gs_N6766Ranges =
{
    {  0.02,  0.02 }, /* Min Current */
    {  17.3,  17.3 }, /* Max Current */
    { 0.025, 0.025 }, /* Min Voltage */
    {  61.2,  61.2 }, /* Max Voltage */
    500.0,            /* Max Power   */
    66.0,             /* Max OVP     */
    N6766A,           /* Model Type  */
};

static SInstrumentRanges gs_N6773Ranges =
{
    { 0.03, 0.03 },  /* Min Current */
    { 15.3, 15.3 },  /* Max Current */
    { 0.03, 0.03 },  /* Min Voltage */
    { 20.4, 20.4 },  /* Max Voltage */
    300.0,           /* Max Power   */
    22.0,            /* Max OVP     */
    N6773A,          /* Model Type  */
};

static SInstrumentRanges gs_N6774Ranges =
{
    { 0.015, 0.015 }, /* Min Current */
    {  8.67,  8.67 }, /* Max Current */
    {  0.04,  0.04 }, /* Min Voltage */
    {  35.7,  35.7 }, /* Max Voltage */
    300.0,            /* Max Power   */
    38.5,             /* Max OVP     */
    N6774A,           /* Model Type  */
};

static SInstrumentRanges gs_N6775Ranges =
{
    { 7.5e-3, 7.5e-3 }, /* Min Current */
    {    5.1,    5.1 }, /* Max Current */
    {   0.07,   0.07 }, /* Min Voltage */
    {   61.2,   61.2 }, /* Max Voltage */
    300.0,              /* Max Power   */
    66.0,               /* Max OVP     */
    N6775A,             /* Model Type  */
};

static SInstrumentRanges gs_N6776Ranges =
{
    { 4.5e-3, 4.5e-3 }, /* Min Current */
    {   3.06,   3.06 }, /* Max Current */
    {    0.1,    0.1 }, /* Min Voltage */
    {  102.0,  102.0 }, /* Max Voltage */
    300.0,              /* Max Power   */
    110.0,              /* Max OVP     */
    N6776A,             /* Model Type  */
};

static SInstrumentRanges gs_N6777Ranges =
{
    {   0.0,   0.0 }, /* Min Current */
    {   2.0,   2.0 }, /* Max Current */
    {   0.0,   0.0 }, /* Min Voltage */
    { 150.0, 150.0 }, /* Max Voltage */
    300.0,            /* Max Power   */
    165.0,            /* Max OVP     */
    N6777A,           /* Model Type  */
};

static SInstrumentRanges gs_N6781Ranges =
{
    { -3.06,  0.0 }, /* Min Current */
    {  3.06, 3.06 }, /* Max Current */
    {   0.0,  0.0 }, /* Min Voltage */
    {  6.12, 20.4 }, /* Max Voltage */
    20.0,            /* Max Power   */
    24.0,            /* Max OVP     */
    N6781A,          /* Model Type  */
};

static SInstrumentRanges gs_N6782Ranges =
{
    { -3.06,  0.0 }, /* Min Current */
    {  3.06, 3.06 }, /* Max Current */
    {   0.0,  0.0 }, /* Min Voltage */
    {  6.12, 20.4 }, /* Max Voltage */
    20.0,            /* Max Power   */
    24.0,            /* Max OVP     */
    N6782A,          /* Model Type  */
};

static SInstrumentRanges gs_N6783BATRanges =
{
    { 0.0, 0.0 }, /* Min Current */
    { 3.0, 3.0 }, /* Max Current */
    { 0.0, 0.0 }, /* Min Voltage */
    { 6.0, 8.0 }, /* Max Voltage */
    24.0,         /* Max Power   */
    10.0,         /* Max OVP     */
    N6783A,       /* Model Type  */
};

static SInstrumentRanges gs_N6784Ranges =
{
    { -3.06,  0.0 }, /* Min Current */
    {  3.06, 3.06 }, /* Max Current */
    { -20.4,  0.0 }, /* Min Voltage */
    {  6.12, 20.4 }, /* Max Voltage */
    20.0,            /* Max Power   */
    24.0,            /* Max OVP     */
    N6784A,          /* Model Type  */
};

static SInstrumentRanges gs_N6785Ranges =
{
    { -8.0, -8.0 },  /* Min Current */
    {  8.0,  8.0 },  /* Max Current */
    {  0.0,  0.0 },  /* Min Voltage */
    { 20.0, 20.0 },  /* Max Voltage */
    80.0,            /* Max Power   */
    24.0,            /* Max OVP     */
    N6785A,          /* Model Type  */
};

static SInstrumentRanges gs_N6786Ranges =
{
    { -8.0, -8.0 },  /* Min Current */
    {  8.0,  8.0 },  /* Max Current */
    {  0.0,  0.0 },  /* Min Voltage */
    { 20.0, 20.0 },  /* Max Voltage */
    80.0,            /* Max Power   */
    24.0,            /* Max OVP     */
    N6786A,          /* Model Type  */
};

static SInstrumentRanges gs_N6731BRanges =
{
    {  0.06,  0.06 }, /* Min Current */
    {  10.2,  10.2 }, /* Max Current */
    { 0.015, 0.015 }, /* Min Voltage */
    {   5.1,   5.1 }, /* Max Voltage */
    50.0,             /* Max Power   */
    7.5,              /* Max OVP     */
    N6731B,           /* Model Type  */
};

static SInstrumentRanges gs_N6732BRanges =
{
    {  0.04,  0.04 }, /* Min Current */
    { 6.375, 6.375 }, /* Max Current */
    { 0.015, 0.015 }, /* Min Voltage */
    {  8.16,  8.16 }, /* Max Voltage */
    50.0,             /* Max Power   */
    10.0,             /* Max OVP     */
    N6732B,           /* Model Type  */
};

static SInstrumentRanges gs_N6733BRanges =
{
    { 0.01, 0.01 },  /* Min Current */
    { 2.55, 2.55 },  /* Max Current */
    { 0.03, 0.03 },  /* Min Voltage */
    { 20.4, 20.4 },  /* Max Voltage */
    50.0,            /* Max Power   */
    22.0,            /* Max OVP     */
    N6733B,          /* Model Type  */
};

static SInstrumentRanges gs_N6734BRanges =
{
    { 0.005, 0.005 }, /* Min Current */
    {  1.53,  1.53 }, /* Max Current */
    {  0.04,  0.04 }, /* Min Voltage */
    {  35.7,  35.7 }, /* Max Voltage */
    52.5,             /* Max Power   */
    38.5,             /* Max OVP     */
    N6734B,           /* Model Type  */
};

static SInstrumentRanges gs_N6735BRanges =
{
    { 0.0025, 0.0025 }, /* Min Current */
    {   0.85,   0.85 }, /* Max Current */
    {   0.07,   0.07 }, /* Min Voltage */
    {   61.2,   61.2 }, /* Max Voltage */
    50.0,               /* Max Power   */
    66.0,               /* Max OVP     */
    N6735B,             /* Model Type  */
};

static SInstrumentRanges gs_N6736BRanges =
{
    { 0.0015, 0.0015 }, /* Min Current */
    {   0.51,   0.51 }, /* Max Current */
    {    0.1,    0.1 }, /* Min Voltage */
    {  102.0,  102.0 }, /* Max Voltage */
    50.0,               /* Max Power   */
    110.0,              /* Max OVP     */
    N6736B,             /* Model Type  */
};

static SInstrumentRanges gs_N6741BRanges =
{
    {  0.06,  0.06 }, /* Min Current */
    {  20.4,  20.4 }, /* Max Current */
    { 0.015, 0.015 }, /* Min Voltage */
    {   5.1,   5.1 }, /* Max Voltage */
    100.0,            /* Max Power   */
    7.5,              /* Max OVP     */
    N6741B,           /* Model Type  */
};

static SInstrumentRanges gs_N6742BRanges =
{
    {  0.04,  0.04 }, /* Min Current */
    { 12.75, 12.75 }, /* Max Current */
    { 0.015, 0.015 }, /* Min Voltage */
    {  8.16,  8.16 }, /* Max Voltage */
    100.0,            /* Max Power   */
    10.0,             /* Max OVP     */
    N6742B,           /* Model Type  */
};

static SInstrumentRanges gs_N6743BRanges =
{
    { 0.01, 0.01 },  /* Min Current */
    {  5.1,  5.1 },  /* Max Current */
    { 0.03, 0.03 },  /* Min Voltage */
    { 20.4, 20.4 },  /* Max Voltage */
    100.0,           /* Max Power   */
    22.0,            /* Max OVP     */
    N6743B,          /* Model Type  */
};

static SInstrumentRanges gs_N6744BRanges =
{
    { 0.005, 0.005 }, /* Min Current */
    {  3.06,  3.06 }, /* Max Current */
    {  0.04,  0.04 }, /* Min Voltage */
    {  35.7,  35.7 }, /* Max Voltage */
    105.0,            /* Max Power   */
    38.5,             /* Max OVP     */
    N6744B,           /* Model Type  */
};

static SInstrumentRanges gs_N6745BRanges =
{
    { 0.0025, 0.0025 }, /* Min Current */
    {    1.7,    1.7 }, /* Max Current */
    {   0.07,   0.07 }, /* Min Voltage */
    {   61.2,   61.2 }, /* Max Voltage */
    100.0,              /* Max Power   */
    66.0,               /* Max OVP     */
    N6745B,             /* Model Type  */
};

static SInstrumentRanges gs_zup366Ranges =
{
    { 0.0015, 0.0015 }, /* Min Current */
    {   1.02,   1.02 }, /* Max Current */
    {    0.1,    0.1 }, /* Min Voltage */
    {  102.0,  102.0 }, /* Max Voltage */
    100.0,              /* Max Power   */
    110.0,              /* Max OVP     */
    ZUP366,             /* Model Type  */
};


/*****************************************************************************
 *---------------------------- Useful Macros --------------------------------*
 *****************************************************************************/

/*- I/O buffer size -----------------------------------------------------*/
#define BUFFER_SIZE                512L
#define ZUP366_IO_SESSION_TYPE    IVI_VAL_VISA_SESSION_TYPE


/*- 488.2 Event Status Register (ESR) Bits ------------------------------*/
#define IEEE_488_2_ERROR_MASK        0x3C

/*- List of default modules. You must define in order 1
 *   or 1,2 or 1,2,3 or 1,2,3,4. One default module must be defined. ----*/
#define DEFAUL_MODULE_1              N6762A
#define DEFAUL_MODULE_2              N6752A
#define DEFAUL_MODULE_3              N6742A
#define DEFAUL_MODULE_4              N6745A

#define MAX_HEADER_COMMAND_LENGTH    25
#define MAX_DOUBLE_FORMAT_LENGTH     15
#define MAX_BOOL_FORMAT_LENGTH       4

static IviStringValueTable zup366ViBoolean_commands =
{
    { ZUP366_ATTR_OUTPUT_ENABLED,                       ":OUT"               	   },
    { ZUP366_ATTR_OVP_ENABLED,                          ":SOUR:VOLT:PROT:STAT"     },
    { ZUP366_ATTR_LIST_TERMINATE_LAST,                  ":SOUR:LIST:TERM:LAST"     },
    { ZUP366_ATTR_LIST_TRIGGER_OUTPUT_ENABLED,          ":STEP:TOUT"               },
    { ZUP366_ATTR_OUTPUT_PROTECTION_COUPLED,            ":OUTP:PROT:COUP"          },
    { ZUP366_ATTR_INITIATE_CONTINUOUS,                  ":INIT:CONT:TRAN"          },
    { ZUP366_ATTR_WAVEFORM_TERMINATION,                 ":SOUR:ARB:TERM:LAST"      },
    { ZUP366_ATTR_SEQUENCE_TERMINATION,                 ":SOUR:ARB:SEQ:TERM:LAST"  },
    { ZUP366_ATTR_SMU_ENABLE_OSCILLATION,               ":OUTP:PROT:OSC"           },
    { ZUP366_ATTR_SMU_ENABLE_RESISTANCE,                ":SOUR:RES:STAT"           },
    { ZUP366_ATTR_SMU_VOLTAGE_LIMIT_TRACKING,           ":SOUR:VOLT:LIM:COUP"      },
    { ZUP366_ATTR_SMU_CURRENT_LIMIT_TRACKING,           ":SOUR:CURR:LIM:COUP"      },
    { ZUP366_ATTR_ENABLE_SIMULTANEOUS_VOLT_MEASUREMENT, ":SENS:FUNC:VOLT"          },
    { ZUP366_ATTR_ENABLE_SIMULTANEOUS_CURR_MEASUREMENT, ":SENS:FUNC:CURR"          },
    { ZUP366_ATTR_AUTO_ELOG_CURRENT_RANGE,              ":SENS:ELOG:CURR:RANG:AUTO"},
    { ZUP366_ATTR_AUTO_ELOG_VOLTAGE_RANGE,              ":SENS:ELOG:VOLT:RANG:AUTO"},
    { ZUP366_ATTR_ENABLE_CURRENT_ELOG,                  ":SENS:ELOG:FUNC:CURR"     },
    { ZUP366_ATTR_ENABLE_VOLTAGE_ELOG,                  ":SENS:ELOG:FUNC:VOLT"     },
    { ZUP366_ATTR_ENABLE_CURRENT_MINMAX_ELOG,           ":SENS:ELOG:FUNC:CURR:MINM"},
    { ZUP366_ATTR_ENABLE_VOLTAGE_MINMAX_ELOG,           ":SENS:ELOG:FUNC:VOLT:MINM"},
    { VI_NULL,                                           VI_NULL                    }
};

static IviStringValueTable zup366ViReal64_commands =
{
    { ZUP366_ATTR_OVP_LIMIT,                       ":SOUR:VOLT:PROT:LEV"      },
    { ZUP366_ATTR_CURRENT_LIMIT,                   ":CUR"  },
    { ZUP366_ATTR_VOLTAGE_LEVEL,                   ":VOL"           },
    { ZUP366_ATTR_TRIGGERED_VOLTAGE_LEVEL,         ":SOUR:VOLT:LEV:TRIG:AMPL" },
    { ZUP366_ATTR_TRIGGERED_CURRENT_LIMIT,         ":SOUR:CURR:LEV:TRIG:AMPL" },
    { ZUP366_ATTR_INSTR_VOLTAGE_RANGE,             ":VOL"          },
    { ZUP366_ATTR_INSTR_CURRENT_RANGE,             ":SOUR:CURR:RANG"          },
    { ZUP366_ATTR_INSTR_VOLTAGE_MEASUREMENT_RANGE, ":SENS:VOLT:RANG"          },
    { ZUP366_ATTR_INSTR_CURRENT_MEASUREMENT_RANGE, ":SENS:CURR:RANG"          },
    { ZUP366_ATTR_SWEEP_TIME_INTERVAL,             ":SENS:SWE:TINT"           },
    { ZUP366_ATTR_OUTPUT_PROTECTION_DELAY,         ":OUTP:PROT:DELAY"         },
    { ZUP366_ATTR_POWER_LIMIT,                     ":SOUR:POW:LIM"            },
    { ZUP366_ATTR_DATALOG_PERIOD,                  ":SENS:DLOG:PER"           },
    { ZUP366_ATTR_DATALOG_DURATION,                ":SENS:DLOG:TIME"          },
    { ZUP366_ATTR_DATALOG_MARKER1,                 ":SENS:DLOG:MARK1:POIN"    },
    { ZUP366_ATTR_DATALOG_MARKER2,                 ":SENS:DLOG:MARK2:POIN"    },
    { ZUP366_ATTR_SMU_RESISTANCE_LEVEL,            ":SOUR:RES:LEV"            },
    { ZUP366_ATTR_HISTOGRAM_RANGE,                 ":SENS:HIST:CURR:RANG"     },
    { ZUP366_ATTR_SMU_POSITIVE_VOLTAGE_LIMIT,      ":SOUR:VOLT:LIM:POS"       },
    { ZUP366_ATTR_SMU_NEGATIVE_VOLTAGE_LIMIT,      ":SOUR:VOLT:LIM:NEG"       },
    { ZUP366_ATTR_SMU_ASM_POSTIVE_CURRENT_LIMIT,   ":SOUR:CURR:LIM:POS"       },
    { ZUP366_ATTR_SMU_ASM_NEGATIVE_CURRENT_LIMIT,  ":SOUR:CURR:LIM:NEG"       },
    { ZUP366_ATTR_ASM_OVER_PROTECTION_DELAY,       ":SOUR:VOLT:PROT:DEL"      },
    { ZUP366_ATTR_ELOG_CURRENT_RANGE,              ":SENS:ELOG:CURR:RANG"     },
    { ZUP366_ATTR_ELOG_VOLTAGE_RANGE,              ":SENS:ELOG:VOLT:RANG"     },
    { ZUP366_ATTR_ELOG_PERIOD,                     ":SENS:ELOG:PER"           },
    { VI_NULL,                                      VI_NULL                    }
};

static IviStringValueTable zup366ViInt32_commands =
{
    { ZUP366_ATTR_LIST_VOLTAGE_POINTS,             ":SOUR:LIST:VOLT:POIN"      },
    { ZUP366_ATTR_LIST_CURRENT_POINTS,             ":SOUR:LIST:CURR:POIN"      },
    { ZUP366_ATTR_LIST_DWELL_POINTS,               ":SOUR:LIST:DWEL:POIN"      },
    { ZUP366_ATTR_LIST_TRIGGER_OUTPUT_BOST_POINTS, ":SOUR:LIST:TOUT:BOST:POIN" },
    { ZUP366_ATTR_LIST_TRIGGER_OUTPUT_EOST_POINTS, ":SOUR:LIST:TOUT:EOST:POIN" },
    { ZUP366_ATTR_SWEEP_POINTS,                    ":SENS:SWE:POIN"            },
    { ZUP366_ATTR_SWEEP_OFFSET_POINTS,             ":SENS:SWE:OFFS:POIN"       },
    { ZUP366_ATTR_INSTR_CHANNEL_COUNT,             ":SYST:CHAN:COUN"           },
    { ZUP366_ATTR_DIGITAL_INPUT_DATA,              ":DIG:INP:DATA"             },
    { ZUP366_ATTR_DIGITAL_OUTPUT_DATA,             ":DIG:OUTP:DATA"            },
    { ZUP366_ATTR_DATALOG_OFFSET,                  ":SENS:DLOG:OFFS"           },
    { ZUP366_ATTR_SMU_POSITIVE_OVRP,               ":SOUR:VOLT:PROT:REM:POS"   },
    { ZUP366_ATTR_SMU_NEGATIVE_OVRP,               ":SOUR:VOLT:PROT:REM:NEG"   },
    { VI_NULL,                                      VI_NULL                     }
};

static IviStringValueTable zup366Enum_commands =
{
    { ZUP366_ATTR_CURRENT_LIMIT_BEHAVIOR,         ":SOUR:CURR:PROT:STAT" },
    { ZUP366_ATTR_TRIGGER_SOURCE,                 ":TRIG:TRAN:SOUR"      },
    { ZUP366_ATTR_ACQUISITION_TRIGGER_SOURCE,     ":TRIG:ACQ:SOUR"       },
    { ZUP366_ATTR_LIST_STEP,                      ":SOUR:LIST:STEP"      },
    { ZUP366_ATTR_TRIGGER_VOLTAGE_MODE,           ":SOUR:VOLT:MODE"      },
    { ZUP366_ATTR_TRIGGER_CURRENT_MODE,           ":SOUR:CURR:MODE"      },
    { ZUP366_ATTR_MEASUREMENT_WINDOW,             ":SENS:WIND"           },
    { ZUP366_ATTR_OUTPUT_INHIBIT_MODE,            ":OUTP:INH:MODE"       },
    { ZUP366_ATTR_DIGITAL_PIN1_FUNCTION,          ":DIG:PIN1:FUNC"       },
    { ZUP366_ATTR_DIGITAL_PIN2_FUNCTION,          ":DIG:PIN2:FUNC"       },
    { ZUP366_ATTR_DIGITAL_PIN3_FUNCTION,          ":DIG:PIN3:FUNC"       },
    { ZUP366_ATTR_DIGITAL_PIN4_FUNCTION,          ":DIG:PIN4:FUNC"       },
    { ZUP366_ATTR_DIGITAL_PIN5_FUNCTION,          ":DIG:PIN5:FUNC"       },
    { ZUP366_ATTR_DIGITAL_PIN6_FUNCTION,          ":DIG:PIN6:FUNC"       },
    { ZUP366_ATTR_DIGITAL_PIN7_FUNCTION,          ":DIG:PIN7:FUNC"       },
    { ZUP366_ATTR_DIGITAL_PIN1_POLARITY,          ":DIG:PIN1:POL"        },
    { ZUP366_ATTR_DIGITAL_PIN2_POLARITY,          ":DIG:PIN2:POL"        },
    { ZUP366_ATTR_DIGITAL_PIN3_POLARITY,          ":DIG:PIN3:POL"        },
    { ZUP366_ATTR_DIGITAL_PIN4_POLARITY,          ":DIG:PIN4:POL"        },
    { ZUP366_ATTR_DIGITAL_PIN5_POLARITY,          ":DIG:PIN5:POL"        },
    { ZUP366_ATTR_DIGITAL_PIN6_POLARITY,          ":DIG:PIN6:POL"        },
    { ZUP366_ATTR_DIGITAL_PIN7_POLARITY,          ":DIG:PIN7:POL"        },
    { ZUP366_ATTR_RELAY_POLARITY,                 ":OUTP:REL:POL"        },
    { ZUP366_ATTR_WAVEFORM_SHAPE,                 ":SOUR:ARB:FUNC:SHAP"  },
    { ZUP366_ATTR_WAVEFORM_MEASUREMENT,           ":SOUR:ARB:FUNC:TYPE"  },
    { ZUP366_ATTR_TRIGGER_WFM_SRC,                ":TRIG:ARB:SOUR"       },
    { ZUP366_ATTR_DATALOG_TRIGGER_SRC,            ":TRIG:DLOG:SOUR"      },
    { ZUP366_ATTR_HISTOGRAM_TRIGGER_SRC,          ":TRIG:HIST:SOUR"      },
    { ZUP366_ATTR_SMU_OUTPUT_IMPEDANCE,           ":OUTP:TMOD"           },
    { ZUP366_ATTR_SMU_VOLTAGE_BANDWIDTH,          ":SOUR:VOLT:BWID"      },
    { ZUP366_ATTR_SMU_OUTPUT_REGULATION_PRIORITY, ":SOUR:FUNC"           },
    { ZUP366_ATTR_ELOG_TRIGGER_SOURCE,            ":TRIG:ELOG:SOUR"      },
    { ZUP366_ATTR_MEASUREMENT_RESOLUTION,         ":SENS:SWE:TINT:RES"   },
    { VI_NULL,                                     VI_NULL                }
};

static IviStringValueTable zup366ViString_commands =
{
    { ZUP366_ATTR_GROUP_CHANNELS,  ":SYST:GRO:DEF" },
    { ZUP366_ATTR_UNGROUP_CHANNEL, ":SYST:GRO:DEL" },
    { VI_NULL,                      VI_NULL         }
};

/* - ZUP366_ATTR_WAVEFORM_MEASUREMENT -*/
static IviRangeTableEntry attrWaveformMeasurementRangeTableEntries[] =
{
    { ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE, 0, 0, "VOLT", 0 },
    { ZUP366_VAL_WFM_MEAS_TYPE_CURRENT, 0, 0, "CURR", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrWaveformMeasurementRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrWaveformMeasurementRangeTableEntries,
};

/* - ZUP366_ATTR_WAVEFORM_SHAPE -*/
static IviRangeTableEntry attrWaveformShapeRangeTableEntries[] =
{
    { ZUP366_VAL_WAVEFORM_NONE,          0, 0, "NONE", 0 },
    { ZUP366_VAL_WAVEFORM_CONSTANTDWELL, 0, 0, "CDW",  0 },
    { ZUP366_VAL_WAVEFORM_EXPONENTIAL,   0, 0, "EXP",  0 },
    { ZUP366_VAL_WAVEFORM_PULSE,         0, 0, "PULS", 0 },
    { ZUP366_VAL_WAVEFORM_RAMP,          0, 0, "RAMP", 0 },
    { ZUP366_VAL_WAVEFORM_SEQUENCE,      0, 0, "SEQ",  0 },
    { ZUP366_VAL_WAVEFORM_SINSUOID,      0, 0, "SIN",  0 },
    { ZUP366_VAL_WAVEFORM_STAIRCASE,     0, 0, "STA",  0 },
    { ZUP366_VAL_WAVEFORM_STEP,          0, 0, "STEP", 0 },
    { ZUP366_VAL_WAVEFORM_TRAPEZOID,     0, 0, "TRAP", 0 },
    { ZUP366_VAL_WAVEFORM_UDEFINED,      0, 0, "UDEF", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrWaveformShapeRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrWaveformShapeRangeTableEntries,
};

/* - ZUP366_ATTR_DATALOG_MEASUREMENT -*/
static IviRangeTableEntry attrDatalogMeasurementRangeTableEntries[] =
{
    { ZUP366_VAL_DL_MEAS_TYPE_VOLTAGE, 0, 0, "VOLT", 0 },
    { ZUP366_VAL_DL_MEAS_TYPE_CURRENT, 0, 0, "CURR", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrDatalogMeasurementRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrDatalogMeasurementRangeTableEntries,
};

static IviRangeTableEntry attrElogTriggerSourceRangeTableEntries[] =
{
    { ZUP366_VAL_TRIG_BUS,       0, 0, "BUS",  0 },
    { ZUP366_VAL_TRIG_EXTERNAL,  0, 0, "EXT",  0 },
    { ZUP366_VAL_TRIG_IMMEDIATE, 0, 0, "IMM",  0 },
    { ZUP366_VAL_TRIG_PIN1,      0, 0, "PIN1", 0 },
    { ZUP366_VAL_TRIG_PIN2,      0, 0, "PIN2", 0 },
    { ZUP366_VAL_TRIG_PIN3,      0, 0, "PIN3", 0 },
    { ZUP366_VAL_TRIG_PIN4,      0, 0, "PIN4", 0 },
    { ZUP366_VAL_TRIG_PIN5,      0, 0, "PIN5", 0 },
    { ZUP366_VAL_TRIG_PIN6,      0, 0, "PIN6", 0 },
    { ZUP366_VAL_TRIG_PIN7,      0, 0, "PIN7", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrElogTriggerSourceRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrElogTriggerSourceRangeTableEntries,
};

static IviRangeTableEntry attrExternalLogPeriodRangeTableEntries[] =
{
    { 0.0001024, 60, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrExternalLogPeriodRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrExternalLogPeriodRangeTableEntries,
};

static IviRangeTableEntry attrMeasurementResolutionRangeTableEntries[] =
{
    { ZUP366_VAL_RESOLUTION_20MS, 0, 0, "RES20", 0 },
    { ZUP366_VAL_RESOLUTION_40MS, 0, 0, "RES40", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrMeasurementResolutionRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrMeasurementResolutionRangeTableEntries,
};

/*****************************************************************************
 *-------------- Utility Function Declarations (Non-Exported) ---------------*
 *****************************************************************************/
static ViStatus zup366_InitAttributes (ViSession vi,
                                        ViInt32 pInstaledModules[MAX_MODULE_COUNT + 1]);
static ViStatus zup366_GetIDNInformation (ViSession vi,
                                           ViAttr attributeId,
                                           ViConstString formatString);
static ViStatus zup366_DefaultInstrSetup (ViSession vi);
static ViStatus zup366_CheckStatus (ViSession vi);
static void RemoveSurroundingWhiteSpace (ViChar string[]);
static void RemoveSurroundingDoubleQuotes (ViChar string[]);
static ViStatus zup366_GetDriverSetupOption (ViSession vi,
                                              ViInt32 pInstaledModules[MAX_MODULE_COUNT + 1]);
static ViStatus zup366_GetInstrumentSetupOption (ViSession io,
                                                  ViInt32 pInstaledModules[MAX_MODULE_COUNT + 1], ViSession vi);
static ViStatus zup366_CheckListFunctionality (ViSession vi,
                                                ViConstString channelName,
                                                ViStatus generatedError);
static ViStatus zup366_CheckReversePolarityFunctionality (ViSession vi,
                                                           ViConstString channelName,
                                                           ViStatus generatedError);
static ViStatus zup366_GetModuleType (ViSession vi,
                                       ViConstString channelName,
                                       ViInt32*        moduleType);
static ViStatus zup366_MakeChannelList (ViSession vi,
                                         ViConstString channelList,
                                         ViChar coercedChannelList[]);
static ViStatus zup366_ReadUnknownLengthData (ViSession vi,
                                               ViSession io,
                                               ViString*           pData,
                                               ViInt32*            pDataCount);
static ViStatus zup366_array_atof (ViReal64 measurementArray[],
                                    ViInt32 arraySize,
                                    ViChar*     pdata,
                                    ViInt32*    pActualPoints);

static void isDoubleWideModule (ViInt32 module_type, ViBoolean* value);

/*
 * static ViStatus zup366_GetStringFromTable(
 *  zup366StringValueTable table,
 *  ViInt32				  value,
 *  ViConstString*		  string
 * );
 */

/*****************************************************************************
 *----------------- Callback Declarations (Non-Exported) --------------------*
 *****************************************************************************/

/*- Global Session Callbacks --------------------------------------------*/

static ViStatus _VI_FUNC zup366_CheckStatusCallback (ViSession vi, ViSession io);

static ViStatus _VI_FUNC zup366ViBoolean_ReadCallback (ViSession vi,
                                                        ViSession io,
                                                        ViConstString channelName,
                                                        ViAttr attributeId,
                                                        ViBoolean*      value);
static ViStatus _VI_FUNC zup366ViBoolean_WriteCallback (ViSession vi,
                                                         ViSession io,
                                                         ViConstString channelName,
                                                         ViAttr attributeId,
                                                         ViBoolean value);
static ViStatus _VI_FUNC zup366ViReal64_WriteCallback (ViSession vi,
                                                        ViSession io,
                                                        ViConstString channelName,
                                                        ViAttr attributeId,
                                                        ViReal64 value);
static ViStatus _VI_FUNC zup366ViReal64_ReadCallback (ViSession vi,
                                                       ViSession io,
                                                       ViConstString channelName,
                                                       ViAttr attributeId,
                                                       ViReal64*       value);
static ViStatus _VI_FUNC zup366ViInt32_WriteCallback (ViSession vi,
                                                       ViSession io,
                                                       ViConstString channelName,
                                                       ViAttr attributeId,
                                                       ViInt32 value);
static ViStatus _VI_FUNC zup366ViInt32_ReadCallback (ViSession vi,
                                                      ViSession io,
                                                      ViConstString channelName,
                                                      ViAttr attributeId,
                                                      ViInt32*       value);
static ViStatus _VI_FUNC zup366Enum_WriteCallback (ViSession vi,
                                                    ViSession io,
                                                    ViConstString channelName,
                                                    ViAttr attributeId,
                                                    ViInt32 value);
static ViStatus _VI_FUNC zup366Enum_ReadCallback (ViSession vi,
                                                   ViSession io,
                                                   ViConstString channelName,
                                                   ViAttr attributeId,
                                                   ViInt32*       value);
static ViStatus _VI_FUNC zup366ViString_WriteCallback (ViSession vi,
                                                        ViSession io,
                                                        ViConstString channelName,
                                                        ViAttr attributeId,
                                                        ViConstString value);
static ViStatus _VI_FUNC zup366ViString_ReadCallback (ViSession vi,
                                                       ViSession io,
                                                       ViConstString channelName,
                                                       ViAttr attributeId,
                                                       const ViConstString cacheValue);

/*- Attribute callbacks -------------------------------------------------*/
static ViStatus _VI_FUNC zup366AttrDriverRevision_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 const ViConstString cacheValue);
static ViStatus _VI_FUNC zup366AttrInstrumentManufacturer_ReadCallback (ViSession vi,
                                                                         ViSession io,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         const ViConstString cacheValue);
static ViStatus _VI_FUNC zup366AttrInstrumentModel_ReadCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  const ViConstString cacheValue);
static ViStatus _VI_FUNC zup366AttrFirmwareRevision_ReadCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   const ViConstString cacheValue);
static ViStatus _VI_FUNC zup366AttrIdQueryResponse_ReadCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  const ViConstString cacheValue);

static ViStatus _VI_FUNC zup366AttrVoltageLevel_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);
static ViStatus _VI_FUNC zup366AttrOvpLimit_CheckCallback (ViSession vi,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 value);
static ViStatus _VI_FUNC zup366AttrRelayPolarity_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value);
static ViStatus _VI_FUNC zup366AttrPowerLimit_CheckCallback (ViSession vi,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 value);
static ViStatus _VI_FUNC zup366AttrCurrentLimit_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);
static ViStatus _VI_FUNC zup366AttrTrigVoltageLevel_CheckCallback (ViSession vi,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViReal64 value);
static ViStatus _VI_FUNC zup366AttrTrigCurrentLimit_CheckCallback (ViSession vi,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViReal64 value);
static ViStatus _VI_FUNC zup366AttrInstrVoltageRange_CoerceCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViReal64 value,
                                                                      ViReal64        *coercedValue);
static ViStatus _VI_FUNC zup366AttrInstrCurrentRange_CoerceCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViReal64 value,
                                                                      ViReal64        *coercedValue);
static ViStatus _VI_FUNC zup366AttrListViInt32_CheckCallback (ViSession vi,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 value);
static ViStatus _VI_FUNC zup366AttrListViReal64_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);
static ViStatus _VI_FUNC zup366AttrListViBoolean_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViBoolean value);
static ViStatus _VI_FUNC zup366AttrListCount_CheckCallback (ViSession vi,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViInt32 value);
static ViStatus _VI_FUNC zup366AttrListCount_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViInt32 value);
static ViStatus _VI_FUNC zup366AttrListCount_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViInt32         *value);
static ViStatus _VI_FUNC zup366AttrListViInt32_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32         *value);
static ViStatus _VI_FUNC zup366AttrInstrModuleOptions_ReadCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32         *value);
static ViStatus _VI_FUNC zup366AttrInstrMeasurementRange_WriteCallback (ViSession vi,
                                                                         ViSession io,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         ViReal64 value);
static ViStatus _VI_FUNC zup366AttrInstrMeasurementRange_ReadCallback (ViSession vi,
                                                                        ViSession io,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        ViReal64*       value);
static ViStatus _VI_FUNC zup366AttrMeasurementfunction_ReadCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViInt32         *value);
static ViStatus _VI_FUNC zup366AttrMeasurementfunction_WriteCallback (ViSession vi,
                                                                       ViSession io,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       ViInt32 value);
static ViStatus _VI_FUNC zup366AttrOutputDelay_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32         *value);
static ViStatus _VI_FUNC zup366AttrOutputDelay_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 value);
static ViStatus _VI_FUNC zup366AttrOutputInhibitMode_CheckCallback (ViSession vi,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value);

static ViStatus _VI_FUNC zup366AttrVoltageLevel_CoerceCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value,
                                                                 ViReal64 *coercedValue);


static ViStatus _VI_FUNC zup366AttrWaveformCount_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 *value);

static ViStatus _VI_FUNC zup366AttrWaveformCount_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value);

static ViStatus _VI_FUNC zup366AttrWaveformCount_RangeTableCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      IviRangeTablePtr *rangeTablePtr);

static ViStatus _VI_FUNC zup366AttrExpTimeConstant_ReadCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrExpTimeConstant_WriteCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value);

static ViStatus _VI_FUNC zup366AttrExpTotalTime_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrExpTotalTime_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);


static ViStatus _VI_FUNC zup366AttrExpStartTime_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrExpStartTime_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);



static ViStatus _VI_FUNC zup366AttrExpEndLevel_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrExpEndLevel_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 value);

static ViStatus _VI_FUNC zup366AttrExpEndLevel_CheckCallback (ViSession vi,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 value);

static ViStatus _VI_FUNC zup366AttrExpStartLevel_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);

static ViStatus _VI_FUNC zup366AttrExpStartLevel_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrExpStartLevel_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);

static ViStatus _VI_FUNC zup366AttrPulsStartLevel_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);

static ViStatus _VI_FUNC zup366AttrPulsStartLevel_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrPulsStartLevel_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);

static ViStatus _VI_FUNC zup366AttrPulsAmplTlev_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);

static ViStatus _VI_FUNC zup366AttrPulsAmplTlev_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrPulsAmplTlev_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);


static ViStatus _VI_FUNC zup366AttrPulsStartTime_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrPulsStartTime_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);

static ViStatus _VI_FUNC zup366AttrPulsWidthTtime_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrPulsWidthTtime_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);



static ViStatus _VI_FUNC zup366AttrPulsEndTime_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrPulsEndTime_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 value);

static ViStatus _VI_FUNC zup366AttrSequenceShape_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 *value);

static ViStatus _VI_FUNC zup366AttrSequenceShape_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value);

static ViStatus _VI_FUNC zup366AttrSequencePacing_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 *value);

static ViStatus _VI_FUNC zup366AttrSequencePacing_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViInt32 value);



static ViStatus _VI_FUNC zup366AttrSequenceStepCount_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViInt32 *value);

static ViStatus _VI_FUNC zup366AttrSequenceStepCount_WriteCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value);


static ViStatus _VI_FUNC zup366AttrSequenceCount_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 *value);

static ViStatus _VI_FUNC zup366AttrSequenceCount_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value);

static ViStatus _VI_FUNC zup366AttrRampStartLevel_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);

static ViStatus _VI_FUNC zup366AttrRampStartLevel_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrRampStartLevel_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);

static ViStatus _VI_FUNC zup366AttrRampEndLevel_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);

static ViStatus _VI_FUNC zup366AttrRampEndLevel_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrRampEndLevel_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);


static ViStatus _VI_FUNC zup366AttrRampStartTime_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrRampStartTime_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);

static ViStatus _VI_FUNC zup366AttrRampEndTime_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrRampEndTime_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 value);



static ViStatus _VI_FUNC zup366AttrRampRiseTime_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrRampRiseTime_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);

static ViStatus _VI_FUNC zup366AttrAmplitude_CheckCallback (ViSession vi,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViReal64 value);

static ViStatus _VI_FUNC zup366AttrAmplitude_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrAmplitude_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViReal64 value);


static ViStatus _VI_FUNC zup366AttrFrequency_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrFrequency_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViReal64 value);

static ViStatus _VI_FUNC zup366AttrOffset_CheckCallback (ViSession vi,
                                                          ViConstString channelName,
                                                          ViAttr attributeId,
                                                          ViReal64 value);

static ViStatus _VI_FUNC zup366AttrOffset_ReadCallback (ViSession vi,
                                                         ViSession io,
                                                         ViConstString channelName,
                                                         ViAttr attributeId,
                                                         ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrOffset_WriteCallback (ViSession vi,
                                                          ViSession io,
                                                          ViConstString channelName,
                                                          ViAttr attributeId,
                                                          ViReal64 value);

static ViStatus _VI_FUNC zup366AttrStairStartLevel_CheckCallback (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value);

static ViStatus _VI_FUNC zup366AttrStairStartLevel_ReadCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrStairStartLevel_WriteCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value);

static ViStatus _VI_FUNC zup366AttrStairEndLevel_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);

static ViStatus _VI_FUNC zup366AttrStairEndLevel_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrStairEndLevel_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);


static ViStatus _VI_FUNC zup366AttrStairStartTime_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrStairStartTime_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);

static ViStatus _VI_FUNC zup366AttrStairTotalTime_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrStairTotalTime_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);



static ViStatus _VI_FUNC zup366AttrStairEndTime_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrStairEndTime_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);


static ViStatus _VI_FUNC zup366AttrStairNsteps_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 *value);

static ViStatus _VI_FUNC zup366AttrStairNsteps_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 value);

static ViStatus _VI_FUNC zup366AttrStepStartLevel_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);

static ViStatus _VI_FUNC zup366AttrStepStartLevel_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrStepStartLevel_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);

static ViStatus _VI_FUNC zup366AttrStepEndLevel_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);

static ViStatus _VI_FUNC zup366AttrStepEndLevel_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrStepEndLevel_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);

static ViStatus _VI_FUNC zup366AttrStepEndTime_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrStepEndTime_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 value);


static ViStatus _VI_FUNC zup366AttrStepStartTime_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrStepStartTime_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);


static ViStatus _VI_FUNC zup366AttrTrapPeakTlev_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);

static ViStatus _VI_FUNC zup366AttrTrapPeakTlev_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrTrapPeakTlev_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);

static ViStatus _VI_FUNC zup366AttrTrapStartLevel_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);

static ViStatus _VI_FUNC zup366AttrTrapStartLevel_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrTrapStartLevel_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);


static ViStatus _VI_FUNC zup366AttrTrapEndTime_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrTrapEndTime_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 value);


static ViStatus _VI_FUNC zup366AttrTrapStartTime_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrTrapStartTime_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);


static ViStatus _VI_FUNC zup366AttrTrapWidthTtime_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrTrapWidthTtime_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);


static ViStatus _VI_FUNC zup366AttrTrapFallTime_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrTrapFallTime_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);


static ViStatus _VI_FUNC zup366AttrTrapRiseTime_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrTrapRiseTime_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);

static ViStatus _VI_FUNC zup366AttrDatalogEnabled_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViBoolean *value);

static ViStatus _VI_FUNC zup366AttrDatalogEnabled_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViBoolean value);

static ViStatus _VI_FUNC zup366AttrDatalogDuration_CheckCallback (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value);

static ViStatus _VI_FUNC zup366AttrDatalogPeriod_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value);

static ViStatus _VI_FUNC zup366AttrDatalogAutoRange_ReadCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViBoolean *value);

static ViStatus _VI_FUNC zup366AttrDatalogAutoRange_WriteCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViBoolean value);

static ViStatus _VI_FUNC zup366AttrDatalogRange_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);

static ViStatus _VI_FUNC zup366AttrDatalogRange_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrDatalogRange_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);



static ViStatus _VI_FUNC zup366AttrHistogramAutoRange_ReadCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViBoolean *value);

static ViStatus _VI_FUNC zup366AttrHistogramAutoRange_WriteCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViBoolean value);

static ViStatus _VI_FUNC zup366AttrHistogramEnabled_ReadCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViBoolean *value);

static ViStatus _VI_FUNC zup366AttrHistogramEnabled_WriteCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViBoolean value);

static ViStatus _VI_FUNC zup366AttrHistogramRange_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value);

static ViStatus _VI_FUNC zup366AttrDatalogOffset_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value);

static ViStatus _VI_FUNC zup366AttrDatalogTriggerLevel_CheckCallback (ViSession vi,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       ViReal64 value);


static ViStatus _VI_FUNC zup366AttrSmuAsmNegativeCurrentLimit_RangeTableCallback (ViSession vi,
                                                                                   ViConstString channelName,
                                                                                   ViAttr attributeId,
                                                                                   IviRangeTablePtr *rangeTablePtr);






static ViStatus _VI_FUNC zup366AttrSmuPositiveVoltageLimit_CheckCallback (ViSession vi,
                                                                           ViConstString channelName,
                                                                           ViAttr attributeId,
                                                                           ViReal64 value);

static ViStatus _VI_FUNC zup366AttrDatalogTriggerLevel_ReadCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrDatalogTriggerLevel_WriteCallback (ViSession vi,
                                                                       ViSession io,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       ViReal64 value);

static ViStatus _VI_FUNC zup366AttrDatalogTriggerSlope_ReadCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViInt32 *value);

static ViStatus _VI_FUNC zup366AttrDatalogTriggerSlope_WriteCallback (ViSession vi,
                                                                       ViSession io,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       ViInt32 value);







static ViStatus _VI_FUNC zup366AttrSmuCurrentSlewRate_CheckCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViReal64 value);

static ViStatus _VI_FUNC zup366AttrSmuCurrentSlewRate_ReadCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViReal64 *value);

static ViStatus _VI_FUNC zup366AttrSmuCurrentSlewRate_WriteCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViReal64 value);

static ViStatus _VI_FUNC zup366ElogCurrentRange_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);
static ViStatus _VI_FUNC zup366ElogVoltageRange_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value);


/*****************************************************************************
 *------------ User-Callable Functions (Exportable Functions) ---------------*
 *****************************************************************************/

/*****************************************************************************
 * Function: zup366_init
 * Purpose:  VXIplug&play required function.  Calls the
 *           zup366_InitWithOptions function.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_init (ViRsrc resourceName,
                                ViBoolean IDQuery,
                                ViBoolean resetDevice,
                                ViSession*  newVi)
{
    ViStatus error = VI_SUCCESS;

    if (newVi == VI_NULL)
    {
        Ivi_SetErrorInfo (VI_NULL, VI_FALSE, IVI_ERROR_INVALID_PARAMETER,
                          VI_ERROR_PARAMETER4, "Null address for Instrument Handle");
        checkErr (IVI_ERROR_INVALID_PARAMETER);
    }

    checkErr (zup366_InitWithOptions (resourceName, IDQuery, resetDevice,
                                       "", newVi));

Error:
    return error;
}

/*****************************************************************************
 * Function: zup366_InitWithOptions
 * Purpose:  This function creates a new IVI session and calls the
 *           IviInit function.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_InitWithOptions (ViRsrc resourceName,
                                           ViBoolean IDQuery,
                                           ViBoolean resetDevice,
                                           ViConstString optionString,
                                           ViSession      *newVi)
{
    ViStatus  error = VI_SUCCESS;
    ViSession vi    = VI_NULL;
    ViChar    newResourceName[IVI_MAX_MESSAGE_BUF_SIZE];
    ViChar    newOptionString[IVI_MAX_MESSAGE_BUF_SIZE];
    ViBoolean isLogicalName;

    if (newVi == VI_NULL)
    {
        Ivi_SetErrorInfo (VI_NULL, VI_FALSE, IVI_ERROR_INVALID_PARAMETER,
                          VI_ERROR_PARAMETER5, "Null address for Instrument Handle");
        checkErr (IVI_ERROR_INVALID_PARAMETER);
    }

    *newVi = VI_NULL;

    checkErr (Ivi_GetInfoFromResourceName (resourceName, (ViString) optionString,
                                           newResourceName,
                                           newOptionString,
                                           &isLogicalName));

    /* create a new interchangeable driver */
    checkErr (Ivi_SpecificDriverNew ("zup366", newOptionString, &vi));
    if (!isLogicalName)
    {
        ViInt32 oldFlag = 0;

        checkErr (Ivi_GetAttributeFlags (vi, IVI_ATTR_IO_RESOURCE_DESCRIPTOR, &oldFlag));
        checkErr (Ivi_SetAttributeFlags (vi, IVI_ATTR_IO_RESOURCE_DESCRIPTOR, oldFlag & 0xfffb | 0x0010));
        checkErr (Ivi_SetAttributeViString (vi, "", IVI_ATTR_IO_RESOURCE_DESCRIPTOR, 0, newResourceName));
    }
    /* init the driver */
    checkErr (zup366_IviInit (newResourceName, IDQuery, resetDevice, vi));
    if (isLogicalName)
        checkErr (Ivi_ApplyDefaultSetup (vi));
    *newVi = vi;

Error:
    if (error < VI_SUCCESS)
        Ivi_Dispose (vi);

    return error;
}

/*****************************************************************************
 * Function: zup366_IviInit
 * Purpose:  This function is called by zup366_InitWithOptions
 *           or by an IVI class driver.  This function initializes the I/O
 *           interface, optionally resets the device, optionally performs an
 *           ID query, and sends a default setup to the instrument.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_IviInit (ViRsrc resourceName,
                                   ViBoolean IDQuery,
                                   ViBoolean reset,
                                   ViSession vi)
{
    ViStatus  error = VI_SUCCESS;
    ViSession io    = VI_NULL;
    ViInt32   installed_modules[MAX_MODULE_COUNT + 1];
    ViInt32   i, channel_count = MAX_MODULE_COUNT;
    ViChar    channel_list[BUFFER_SIZE];
    ViChar    * pchannel_list = channel_list;


    checkErr (zup366_GetDriverSetupOption (vi, installed_modules));

    if (!Ivi_Simulating (vi))
    {
        ViSession rmSession = VI_NULL;

        /* Open instrument session */
        checkErr (Ivi_GetAttributeViSession (vi, VI_NULL, IVI_ATTR_VISA_RM_SESSION, 0,
                                             &rmSession));
        viCheckErr (viOpen (rmSession, resourceName, VI_NULL, VI_NULL, &io));
		
        /* io session owned by driver now */
        checkErr (Ivi_SetAttributeViSession (vi, VI_NULL, IVI_ATTR_IO_SESSION, 0, io));

        /* Configure VISA Formatted I/O */
        viCheckErr (viSetAttribute (io, VI_ATTR_TMO_VALUE, 5000));
        viCheckErr (viSetBuf (io, VI_READ_BUF | VI_WRITE_BUF, 4000));
        viCheckErr (viSetAttribute (io, VI_ATTR_WR_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS));
        viCheckErr (viSetAttribute (io, VI_ATTR_RD_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS));
    }

    for (i = 1; i <= MAX_MODULE_COUNT; i++)
    {
        if (installed_modules[i] == NOT_SUPPORTED)
            channel_count--;
        else
            pchannel_list += sprintf (pchannel_list, "%ld,", i);
    }


    if (channel_count == 0)
    {
        /*
         * Simulation and user does not specify any module
         */
        channel_count = 0;
#if defined(DEFAUL_MODULE_1)
        installed_modules[1] = DEFAUL_MODULE_1;
        channel_count++;
        pchannel_list += sprintf (pchannel_list, "1,");
#endif
#if defined(DEFAUL_MODULE_2)
        installed_modules[2] = DEFAUL_MODULE_2;
        channel_count++;
        pchannel_list += sprintf (pchannel_list, "2,");
#endif
#if defined(DEFAUL_MODULE_3)
        installed_modules[3] = DEFAUL_MODULE_3;
        channel_count++;
        pchannel_list += sprintf (pchannel_list, "3,");
#endif
#if defined(DEFAUL_MODULE_4)
        installed_modules[4] = DEFAUL_MODULE_4;
        channel_count++;
        pchannel_list += sprintf (pchannel_list, "4,");
#endif
    }

    *--pchannel_list = '\0';

    checkErr (Ivi_BuildChannelTable (vi, channel_list, VI_FALSE, VI_NULL));

    checkErr (zup366_GetDriverSetupOption (vi, installed_modules));

    if (!Ivi_Simulating (vi))
    {   /* Code moved from above in order to support empty slots */
        if (installed_modules[1] == NOT_SUPPORTED)
        {
			  viCheckErr (viPrintf (io, ":DCL;"));
			  Delay(0.05);
			  viCheckErr (viPrintf (io, ":ADR01;"));
			  Delay(0.05);
			  viCheckErr (viPrintf (io, ":RMT2;"));
			  Delay(0.05);
        }
    }
	
	
	
	/* Add attributes */
    checkErr (zup366_InitAttributes (vi, installed_modules));

#if 0    /* formerly excluded lines */

    /*- Reset instrument ----------------------------------------------------*/
    if (reset)
        error = zup366_reset (vi);
    else  /*- Send Default Instrument Setup ---------------------------------*/
        error = zup366_DefaultInstrSetup (vi);
    checkErr (error);
	
	
	
    /*- Identification Query ------------------------------------------------*/
    if (IDQuery)
    {
        ViChar rdBuffer[BUFFER_SIZE];

        #define VALID_RESPONSE_STRING_START             "Agilent Technologies,N67"
        #define VALID_RESPONSE_STRING_START_KEYSIGHT    "Keysight Technologies,N67"

        checkErr (Ivi_GetAttributeViString (vi, VI_NULL, ZUP366_ATTR_ID_QUERY_RESPONSE,
                                            0, BUFFER_SIZE, rdBuffer));

        if ((strncmp (rdBuffer, VALID_RESPONSE_STRING_START,
                      strlen (VALID_RESPONSE_STRING_START)) != 0) &&
            (strncmp (rdBuffer, VALID_RESPONSE_STRING_START_KEYSIGHT,
                      strlen (VALID_RESPONSE_STRING_START_KEYSIGHT)) != 0))
        {
            viCheckErr (VI_ERROR_FAIL_ID_QUERY);
        }
    }

	
#endif   /* formerly excluded lines */

	
    checkErr (zup366_CheckStatus (vi));

Error:
    if (error < VI_SUCCESS)
    {
        if (!Ivi_Simulating (vi) && io)
            viClose (io);
    }
    return error;
}

/*****************************************************************************
 * Function: zup366_close
 * Purpose:  This function closes the instrument.
 *
 *           Note:  This function must unlock the session before calling
 *           Ivi_Dispose.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_close (ViSession vi)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_IviClose (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    Ivi_Dispose (vi);

    return error;
}

/*****************************************************************************
 * Function: zup366_IviClose
 * Purpose:  This function performs all of the drivers clean-up operations
 *           except for closing the IVI session.  This function is called by
 *           zup366_close or by an IVI class driver.
 *
 *           Note:  This function must close the I/O session and set
 *           IVI_ATTR_IO_SESSION to 0.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_IviClose (ViSession vi)
{
    ViStatus  error = VI_SUCCESS;
    ViSession io    = VI_NULL;

    /* Do not lock here.  The caller manages the lock. */

    checkErr (Ivi_GetAttributeViSession (vi, VI_NULL, IVI_ATTR_IO_SESSION, 0, &io));

Error:
    Ivi_SetAttributeViSession (vi, VI_NULL, IVI_ATTR_IO_SESSION, 0, VI_NULL);
    if (io)
    {
        viClose (io);
    }
    return error;
}

/*****************************************************************************
 * Function: zup366_reset
 * Purpose:  This function resets the instrument.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_reset (ViSession vi)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (!Ivi_Simulating (vi))               /* call only when locked */
    {
        ViSession io = Ivi_IOSession (vi);  /* call only when locked */

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, "*RST"));
    }

    checkErr (zup366_DefaultInstrSetup (vi));

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ResetWithDefaults
 * Purpose:  This function resets the instrument and applies default settings
 *           from the IVI Configuration Store based on the logical name
 *           from which the session was created.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ResetWithDefaults (ViSession vi)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));
    checkErr (zup366_reset (vi));
    checkErr (Ivi_ApplyDefaultSetup (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/****************************************************************************
 *  Function: zup366_Disable
 *  Purpose:  This function places the instrument in a quiescent state as
 *            quickly as possible.
 *
 *   Note: This function is not supported.
 ****************************************************************************/
ViStatus _VI_FUNC zup366_Disable (ViSession vi)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_self_test
 * Purpose:  This function executes the instrument self-test and returns the
 *           result.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_self_test (ViSession vi, ViInt16 *testResult,
                                     ViChar testMessage[])
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (testResult == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Test Result");
    if (testMessage == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Test Message");

    if (!Ivi_Simulating (vi))               /* call only when locked */
    {
        ViSession io = Ivi_IOSession (vi);  /* call only when locked */

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, "*TST?"));

        viCheckErr (viScanf (io, "%hd", testResult));

        if (testMessage)
            sprintf (testMessage, "Self-Test %s.",
                     (!(*testResult)) ? "Passed" : "Failed");
    }
    else
    {
        /* Simulate Self Test */
        *testResult = 0;
        strcpy (testMessage, "No error.");
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_revision_query
 * Purpose:  This function returns the driver and instrument revisions.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_revision_query (ViSession vi,
                                          ViChar driverRev[],
                                          ViChar instrRev[])
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (driverRev == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Driver Revision");
    if (instrRev == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Instrument Revision");

    checkErr (Ivi_GetAttributeViString (vi, VI_NULL, ZUP366_ATTR_SPECIFIC_DRIVER_REVISION,
                                        0, 256, driverRev));
    checkErr (Ivi_GetAttributeViString (vi, "", ZUP366_ATTR_INSTRUMENT_FIRMWARE_REVISION,
                                        0, 256, instrRev));
    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_error_query
 * Purpose:  This function queries the instrument error queue and returns
 *           the result.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_error_query (ViSession vi,
                                       ViInt32     *errCode,
                                       ViChar errMessage[])
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (errCode == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Error Code");
    if (errMessage == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Error Message");

    if (!Ivi_Simulating (vi))               /* call only when locked */
    {
        ViSession io = Ivi_IOSession (vi);  /* call only when locked */

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, ":SYST:ERR?"));
        viCheckErr (viScanf (io, "%ld,\"%256[^\"]", errCode, errMessage));
    }
    else
    {
        /* Simulate Error Query */
        *errCode = 0;
        strcpy (errMessage, "No error.");
    }

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_error_message
 * Purpose:  This function translates the error codes returned by this
 *           instrument driver into user-readable strings.
 *
 *           Note:  The caller can pass VI_NULL for the vi parameter.  This
 *           is useful if one of the init functions fail.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_error_message (ViSession vi,
                                         ViStatus errorCode,
                                         ViChar errorMessage[256])
{
    ViStatus                   error = VI_SUCCESS;

    static IviStringValueTable errorTable =
    {
        IVIDCPWR_ERROR_CODES_AND_MSGS,
        { VI_NULL,                    VI_NULL}
    };

    if (vi)
        Ivi_LockSession (vi, VI_NULL);

    /* all VISA and IVI error codes are handled as well as codes in the table */
    if (errorMessage == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Error Message");

    checkErr (Ivi_GetSpecificDriverStatusDesc (vi, errorCode, errorMessage, errorTable));

Error:
    if (vi)
        Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_InvalidateAllAttributes
 * Purpose:  This function invalidates the cached value of all attributes.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_InvalidateAllAttributes (ViSession vi)
{
    return Ivi_InvalidateAllAttributes (vi);
}

/* OBSOLETE INHERENT FUNCTIONS */
/*- Obsolete Error Handling Functions ----------------------------*/
/*****************************************************************************
 * Function: zup366_GetErrorInfo and zup366_ClearErrorInfo Functions
 * Purpose:  These functions enable the instrument driver user to
 *           get or clear the error information the driver associates with the
 *           IVI session.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_GetErrorInfo (ViSession vi, ViStatus   *primaryError,
                                        ViStatus *secondaryError,
                                        ViChar errorElaboration[256])
{
    return Ivi_GetErrorInfo (vi, primaryError, secondaryError, errorElaboration);
}
ViStatus _VI_FUNC zup366_ClearErrorInfo (ViSession vi)
{
    return Ivi_ClearErrorInfo (vi);
}

/*****************************************************************************
 * Function: zup366_GetError and zup366_ClearError Functions
 * Purpose:  These functions enable the instrument driver user to
 *           get or clear the error information the driver associates with the
 *           IVI session.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_GetError (ViSession vi,
                                    ViStatus    *errorCode,
                                    ViInt32 bufferSize,
                                    ViChar description[])
{
    ViStatus  error              = VI_SUCCESS;
    ViStatus  primary            = VI_SUCCESS,
              secondary          = VI_SUCCESS;
    ViChar    elaboration[256]   = "";
    ViChar    errorMessage[1024] = "";
    ViChar    *appendPoint       = errorMessage;
    ViInt32   actualSize         = 0;
    ViBoolean locked             = VI_FALSE;

    /* Lock Session */
    if (vi != 0)
    {
        checkErr (Ivi_LockSession (vi, &locked));
    }

    /* Test for nulls and acquire error data */
    if (bufferSize != 0)
    {
        if (errorCode == VI_NULL)
        {
            viCheckParm (IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Error");
        }
        if (description == VI_NULL)
        {
            viCheckParm (IVI_ERROR_INVALID_PARAMETER, 4, "Null address for Description");
        }
        checkErr (Ivi_GetErrorInfo (vi, &primary, &secondary, elaboration));
    }

    else
    {
        checkErr (Ivi_GetAttributeViString (vi, VI_NULL, IVI_ATTR_ERROR_ELABORATION, 0, 256, elaboration));
        checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, IVI_ATTR_SECONDARY_ERROR, 0, &secondary));
        checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, IVI_ATTR_PRIMARY_ERROR, 0, &primary));
    }

    /* Format data */
    if (primary != VI_SUCCESS)
    {
        ViChar msg[256] = "";
        checkErr (zup366_error_message (vi, primary, msg));
        appendPoint += sprintf (appendPoint, "Primary Error: (Hex 0x%08X) %s\n", primary, msg);
    }

    if (secondary != VI_SUCCESS)
    {
        ViChar msg[256] = "";
        checkErr (zup366_error_message (vi, secondary, msg));
        appendPoint += sprintf (appendPoint, "Secondary Error: (Hex 0x%08X) %s\n", secondary, msg);
    }

    if (elaboration[0])
    {
        sprintf (appendPoint, "Elaboration: %s", elaboration);
    }

    actualSize = (ViInt32) strlen (errorMessage) + 1;

    /* Prepare return values */
    if (bufferSize == 0)
    {
        error = actualSize;
    }
    else
    {
        if (bufferSize > 0)
        {
            if (actualSize > bufferSize)
            {
                error      = actualSize;
                actualSize = bufferSize;
            }
        }
        memcpy (description, errorMessage, actualSize - 1);
        description[actualSize - 1] = '\0';
    }

    if (errorCode)
    {
        *errorCode = primary;
    }

Error:
    /* Unlock Session */
    Ivi_UnlockSession (vi, &locked);
    return error;
}

ViStatus _VI_FUNC zup366_ClearError (ViSession vi)
{
    return Ivi_ClearErrorInfo (vi);
}

/*****************************************************************************
 * Function: zup366_GetNextCoercionRecord
 * Purpose:  This function enables the instrument driver user to obtain
 *           the coercion information associated with the IVI session.
 *           This function retrieves and clears the oldest instance in which
 *           the instrument driver coerced a value the instrument driver user
 *           specified to another value.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_GetNextCoercionRecord (ViSession vi,
                                                 ViInt32 bufferSize,
                                                 ViChar record[])
{
    return Ivi_GetNextCoercionString (vi, bufferSize, record);
}

/****************************************************************************
 *  Function: zup366_GetNextInterchangeWarning,
 *            zup366_ResetInterchangeCheck, and
 *            zup366_ClearInterchangeWarnings
 *  Purpose:  These functions allow the user to retrieve interchangeability
 *            warnings, reset the driver's interchangeability checking
 *            state, and clear all previously logged interchangeability warnings.
 ****************************************************************************/
ViStatus _VI_FUNC zup366_GetNextInterchangeWarning (ViSession vi,
                                                     ViInt32 bufferSize,
                                                     ViChar warnString[])
{
    return Ivi_GetNextInterchangeCheckString (vi, bufferSize, warnString);
}

ViStatus _VI_FUNC zup366_ResetInterchangeCheck (ViSession vi)
{
    return Ivi_ResetInterchangeCheck (vi);
}

ViStatus _VI_FUNC zup366_ClearInterchangeWarnings (ViSession vi)
{
    return Ivi_ClearInterchangeWarnings (vi);
}

/*****************************************************************************
 * Function: zup366_LockSession and zup366_UnlockSession Functions
 * Purpose:  These functions enable the instrument driver user to lock the
 *           session around a sequence of driver calls during which other
 *           execution threads must not disturb the instrument state.
 *
 *           NOTE:  The callerHasLock parameter must be a local variable
 *           initialized to VI_FALSE and passed by reference, or you can pass
 *           VI_NULL.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_LockSession (ViSession vi, ViBoolean *callerHasLock)
{
    return Ivi_LockSession (vi, callerHasLock);
}
ViStatus _VI_FUNC zup366_UnlockSession (ViSession vi, ViBoolean *callerHasLock)
{
    return Ivi_UnlockSession (vi, callerHasLock);
}

/****************************************************************************
 *  Function: zup366_GetChannelName
 *  Purpose:  This function returns the highest-level channel name that
 *            corresponds to the specific driver channel string that is in
 *            the channel table at an index you specify.
 ****************************************************************************/
ViStatus _VI_FUNC zup366_GetChannelName (ViSession vi,
                                          ViInt32 index,
                                          ViInt32 bufferSize,
                                          ViChar name[])
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channelName;
    ViInt32       actualSize = 0;
    ViInt32       channelCount;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    /* Check parameters */
    if (bufferSize != 0)
    {
        if (name == VI_NULL)
        {
            viCheckParm (IVI_ERROR_INVALID_PARAMETER, 4, "Null address for Channel Name");
        }
    }

    checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, IVI_ATTR_CHANNEL_COUNT, 0, &channelCount));
    if ((index < 1) || (index > channelCount))
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 2, "Channel Index");

    /* Get channel name */
    viCheckParm (Ivi_GetNthChannelString (vi, index, &channelName), 2, "Index");
    viCheckParm ((channelName == NULL) ? IVI_ERROR_INVALID_PARAMETER : VI_SUCCESS, 2, "Index");

    actualSize = (ViInt32) strlen (channelName) + 1;

    /* Prepare return values */
    if (bufferSize == 0)
    {
        error = actualSize;
    }
    else
    {
        if (bufferSize > 0)
        {
            if (actualSize > bufferSize)
            {
                error      = actualSize;
                actualSize = bufferSize;
            }
        }
        memcpy (name, channelName, actualSize - 1);
        name[actualSize - 1] = '\0';
    }

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureOutputEnabled
 * Purpose:  This function controls whether or not the signal the power supply
 *           generates is present at the output terminal.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureOutputEnabled (ViSession vi,
                                                  ViConstString channelName,
                                                  ViBoolean enabled)
{
    return Ivi_SetAttributeViBoolean (vi, channelName,
                                      ZUP366_ATTR_OUTPUT_ENABLED,
                                      IVI_VAL_DIRECT_USER_CALL,
                                      enabled);
}

/*****************************************************************************
 * Function: zup366_ConfigureOutputRange
 * Purpose:  This function determines the output range in which the power
 *           supply operates.
 *****************************************************************************/
static IviRangeTableEntry rangeTypeRangeTableEntries[] =
	{
		{ZUP366_VAL_RANGE_CURRENT, 0, 0, "CURR", ZUP366_ATTR_CURRENT_LIMIT},
		{ZUP366_VAL_RANGE_VOLTAGE, 0, 0, "VOLT", ZUP366_ATTR_VOLTAGE_LEVEL},
		{IVI_RANGE_TABLE_LAST_ENTRY}
	};
static IviRangeTable rangeTypeRangeTable =
	{
		IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        VI_NULL,
        rangeTypeRangeTableEntries,
	};

ViStatus _VI_FUNC zup366_ConfigureOutputRange (ViSession vi,
                                                ViConstString channelName,
                                                ViInt32 rangeType,
                                                ViReal64 range)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (Ivi_SetAttributeViReal64 (vi,
                                        channelName,
                                        (rangeType == ZUP366_VAL_RANGE_VOLTAGE) ?
                                        ZUP366_ATTR_INSTR_VOLTAGE_RANGE :
                                        ZUP366_ATTR_INSTR_CURRENT_RANGE,
                                        0, range));

    checkErr (Ivi_InvalidateAttribute (vi, channelName, ZUP366_ATTR_CURRENT_LIMIT));
    checkErr (Ivi_InvalidateAttribute (vi, channelName, ZUP366_ATTR_VOLTAGE_LEVEL));

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureCurrentLimit
 * Purpose:  This function configures the behavior and level of the
 *           current limit.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureCurrentLimit (ViSession vi,
                                                 ViConstString channelName,
                                                 ViInt32 behavior,
                                                 ViReal64 limit)
{
    ViStatus  error         = VI_SUCCESS;
    ViBoolean behaviorFirst = VI_FALSE;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    /*
     * If you are setting the instrument to trip, set the limit and then the behavior
     * to prevent a premature over-current condition.
     */

    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_CURRENT_LIMIT,
                                           0, limit), 4,
                 "Current Limit");

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureOVP
 * Purpose:  This function enables or disables over-voltage protection and
 *           configures the over-voltage limit.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureOVP (ViSession vi,
                                        ViConstString channelName,
                                        ViBoolean enabled,
                                        ViReal64 limit)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));


    /*
     * If you are enabling over-voltage protection, set the limit first
     * to avoid a premature over-voltage condition.
     */
    if (enabled)
    {
        viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                               ZUP366_ATTR_OVP_LIMIT,
                                               0, limit), 4, "OVP Limit");
    }

    /* The following attribute is provided to be compatible with IVI specification,
     * setting this attribute has no influence on the connected instrument - N6700
     * does not support disable OVP */
    viCheckParm (Ivi_SetAttributeViBoolean (vi, channelName,
                                            ZUP366_ATTR_OVP_ENABLED,
                                            0, enabled), 3, "OVP Enabled");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureVoltageLevel
 * Purpose:  This function configures the voltage level the power supply
 *           attempts to generate.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureVoltageLevel (ViSession vi,
                                                 ViConstString channelName,
                                                 ViReal64 level)
{
	if (level>14.0)
	return -1;
    return Ivi_SetAttributeViReal64 (vi, channelName,
                                     ZUP366_ATTR_VOLTAGE_LEVEL,
                                     IVI_VAL_DIRECT_USER_CALL,
                                     level);

}

/*****************************************************************************
 * Function: zup366_ConfigureRelayPolarity
 * Purpose:  This function configures the output relay polarity.
 *
 * Notes:
 *	Option 760 (Polarity Reversal) available on models N6742B and N6773A
 *  Required option for N673xB, N674xB, N675xA, N676xA, N677xA
 * **	Not available for N6741B, N6751A, N6752A, N6761A, N6762A, and N678xA.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureRelayPolarity (ViSession vi,
                                                  ViConstString channelName,
                                                  ViInt32 relayPolarity)
{
    return Ivi_SetAttributeViInt32 (vi,
                                    channelName,
                                    ZUP366_ATTR_RELAY_POLARITY,
                                    IVI_VAL_DIRECT_USER_CALL,
                                    relayPolarity);
}

/*****************************************************************************
 * Function: zup366_LimitChannelPower
 * Purpose:  This function configures the output power limit.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_LimitChannelPower (ViSession vi,
                                             ViConstString channelName,
                                             ViReal64 powerLimit)
{
    return Ivi_SetAttributeViReal64 (vi,
                                     channelName,
                                     ZUP366_ATTR_POWER_LIMIT,
                                     0, powerLimit);
}

/*****************************************************************************
 * Function: zup366_QueryOutputState
 * Purpose:  This function returns a value indicating if the power supply
 *           is in a particular output state.
 *****************************************************************************/
static IviRangeTableEntry outputStateRangeTableEntries[] =
	{
		{ZUP366_VAL_OUTPUT_CONSTANT_VOLTAGE, 0, 0, "OPER", 0},
		{ZUP366_VAL_OUTPUT_CONSTANT_CURRENT, 0, 0, "OPER", 1},
		{ZUP366_VAL_OUTPUT_OVER_VOLTAGE, 0, 0, "QUES", 0},
		{ZUP366_VAL_OUTPUT_OVER_CURRENT, 0, 0, "QUES", 1},
		{ZUP366_VAL_OUTPUT_OVER_TEMPERATURE, 0, 0, "QUES", 4},
		{ZUP366_VAL_OUTPUT_UNREGULATED, 0, 0, "QUES", 10},
		{IVI_RANGE_TABLE_LAST_ENTRY}
	};
static IviRangeTable outputStateRangeTable =
	{
		IVI_VAL_DISCRETE,
        VI_FALSE,
        VI_FALSE,
        VI_NULL,
        outputStateRangeTableEntries,
	};

ViStatus _VI_FUNC zup366_QueryOutputState (ViSession vi,
                                            ViConstString channelName,
                                            ViInt32 outputState,
                                            ViBoolean*      inState)
{
    ViStatus      error = VI_SUCCESS;
    ViString      cmd;
    ViConstString coerceedChannelName;
    ViBoolean     in_state = VI_FALSE;
    ViInt32       bit, value = 1;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (inState == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 4, "Null value for inState.");

    viCheckParm (Ivi_CoerceChannelName (vi, channelName, &coerceedChannelName),
                 2, "Channel Name");

    viCheckParm (Ivi_GetViInt32EntryFromValue (outputState,
                                               &outputStateRangeTable,
                                               VI_NULL, VI_NULL, VI_NULL,
                                               VI_NULL, &cmd, &bit),
                 3, "Output State");

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viQueryf (io, ":STAT:%s:COND? (@%s)", "%ld",
                              cmd,
                              coerceedChannelName,
                              &value));
    }


    in_state = ((value & (1 << bit)) != 0);
    checkErr (zup366_CheckStatus (vi));

Error:
    if (inState)
        *inState = in_state;
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_QueryMaxCurrentLimit
 * Purpose:  This function returns the maximum programmable current limit
 *           that the power supply accepts for a particular voltage level on
 *           a channel for the output range to which the power supply is
 *           currently configured.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_QueryMaxCurrentLimit (ViSession vi,
                                                ViConstString channelName,
                                                ViReal64 voltageLevel,
                                                ViReal64*       maxCurrentLimit)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViReal64            value                 = 0.0;
    ViReal64            voltage_range;
    ViInt32             range_index;
    ViInt32             channel_index = 0;
    ViInt32             result        = 0;
    ViReal64            minimum;
    ViReal64            maximum;
    ViConstString       channel;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (maxCurrentLimit == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 4, "Null value for maxCurrentLimit.");

    if (Ivi_RangeChecking (vi))
    {
        viCheckParm (Ivi_CoerceChannelName (vi, channelName, &channel),
                     2, "Channel Name");

        checkErr (Ivi_GetAttributeViAddr (vi,
                                          VI_NULL,
                                          ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                          0, (ViAddr *) &pinstrument_modules));
        channel_index = (channel[0] - '0');


        checkErr (Ivi_GetAttributeViReal64 (vi,
                                            channelName, ZUP366_ATTR_INSTR_VOLTAGE_RANGE, 0,
                                            &voltage_range));
        for (range_index = 0; range_index < MAX_NB_RANGES; range_index++)
        {
            viCheckErr (Ivi_CompareWithPrecision (0, pinstrument_modules[channel_index]->MaxVoltage[range_index],
                                                  voltage_range,
                                                  &result));
            if (result == 0)
                break;
        }

        /*
         * For sure, check that range_index is less as MAX_NB_RANGES
         */
        if (range_index >= MAX_NB_RANGES)
            range_index = (MAX_NB_RANGES - 1);

        minimum = pinstrument_modules[channel_index]->MinVoltage[range_index];
        maximum = voltage_range;

        if (voltageLevel < minimum || voltageLevel > maximum)
            viCheckParm (IVI_ERROR_INVALID_VALUE, 3, "Voltage Level");
    }

    /* Change: Determine the max current for the given voltage level. */
    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viQueryf (io, "SOUR:CURR:LEV:IMM:AMPL? MAX,(@%s)", "%Le", channelName, &value));
    }
    else
    {
        viCheckParm (Ivi_CoerceChannelName (vi, channelName, &channel),
                     2, "Channel Name");

        checkErr (Ivi_GetAttributeViAddr (vi,
                                          VI_NULL,
                                          ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                          0, (ViAddr *) &pinstrument_modules));
        channel_index = (channel[0] - '0');


        checkErr (Ivi_GetAttributeViReal64 (vi,
                                            channelName, ZUP366_ATTR_INSTR_VOLTAGE_RANGE, 0,
                                            &voltage_range));
        for (range_index = 0; range_index < MAX_NB_RANGES; range_index++)
        {
            viCheckErr (Ivi_CompareWithPrecision (0, pinstrument_modules[channel_index]->MaxVoltage[range_index],
                                                  voltage_range,
                                                  &result));
            if (result == 0)
                break;
        }

        /*
         * For sure, check that range_index is less as MAX_NB_RANGES
         */
        if (range_index >= MAX_NB_RANGES)
            range_index = (MAX_NB_RANGES - 1);

        value = pinstrument_modules[channel_index]->MaxCurrent[range_index];
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (maxCurrentLimit)
        *maxCurrentLimit = value;
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_QueryMaxVoltageLevel
 * Purpose:  This function returns the maximum programmable voltage level
 *           that the power supply accepts for a particular current limit on
 *           a channel for the output range to which the power supply is
 *           currently configured.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_QueryMaxVoltageLevel (ViSession vi,
                                                ViConstString channelName,
                                                ViReal64 currentLimit,
                                                ViReal64*       maxVoltageLevel)
{
    ViStatus            error                 = VI_SUCCESS;
    ViReal64            value                 = 0.0;
    ViConstString       pchannel_name         = NULL;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViInt32             channel_index         = 0;
    ViReal64            minimum;
    ViReal64            maximum;
    ViInt32             result = 0;
    ViReal64            current_range;
    ViInt32             range_index;
    ViConstString       channel;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (maxVoltageLevel == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 4, "Null value for maxVoltageLevel.");

    if (Ivi_RangeChecking (vi))
    {
        viCheckParm (Ivi_CoerceChannelName (vi, channelName, &channel),
                     2, "Channel Name");

        checkErr (Ivi_GetAttributeViAddr (vi,
                                          VI_NULL,
                                          ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                          0, (ViAddr *) &pinstrument_modules));


        channel_index = (channel[0] - '0');


        checkErr (Ivi_GetAttributeViReal64 (vi,
                                            channelName, ZUP366_ATTR_INSTR_CURRENT_RANGE, 0,
                                            &current_range));

        for (range_index = 0; range_index < MAX_NB_RANGES; range_index++)
        {
            viCheckErr (Ivi_CompareWithPrecision (0, pinstrument_modules[channel_index]->MaxCurrent[range_index],
                                                  current_range,
                                                  &result));
            if (result == 0)
                break;
        }
        /*
         * For sure, check that range_index is less as MAX_NB_RANGES
         */
        if (range_index >= MAX_NB_RANGES)
            range_index = (MAX_NB_RANGES - 1);

        minimum = pinstrument_modules[channel_index]->MinCurrent[range_index];
        maximum = current_range;

        if (currentLimit < minimum || currentLimit > maximum)
            viCheckParm (IVI_ERROR_INVALID_VALUE, 3, "Current Limit");
    }

    viCheckParm (Ivi_CoerceChannelName (vi, channelName, &pchannel_name),
                 2, "Channel Name");

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viQueryf (io, "SOUR:VOLT:LEV:IMM:AMPL? MAX,(@%s)", "%Le", pchannel_name, &value));
    }
    else
    {
        viCheckParm (Ivi_CoerceChannelName (vi, channelName, &channel),
                     2, "Channel Name");

        checkErr (Ivi_GetAttributeViAddr (vi,
                                          VI_NULL,
                                          ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                          0, (ViAddr *) &pinstrument_modules));


        channel_index = (channel[0] - '0');


        checkErr (Ivi_GetAttributeViReal64 (vi,
                                            channelName, ZUP366_ATTR_INSTR_CURRENT_RANGE, 0,
                                            &current_range));

        for (range_index = 0; range_index < MAX_NB_RANGES; range_index++)
        {
            viCheckErr (Ivi_CompareWithPrecision (0, pinstrument_modules[channel_index]->MaxCurrent[range_index],
                                                  current_range,
                                                  &result));
            if (result == 0)
                break;
        }
        /*
         * For sure, check that range_index is less as MAX_NB_RANGES
         */
        if (range_index >= MAX_NB_RANGES)
            range_index = (MAX_NB_RANGES - 1);
        value = pinstrument_modules[channel_index]->MaxVoltage[range_index];
    }



    checkErr (zup366_CheckStatus (vi));

Error:
    if (maxVoltageLevel)
        *maxVoltageLevel = value;
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ResetOutputProtection
 * Purpose:  This function resets the power supply's output protection.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ResetOutputProtection (ViSession vi,
                                                 ViConstString channelName)
{
    ViStatus      error = VI_SUCCESS;
    ViConstString coerced_channel_name;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_CoerceChannelName (vi, channelName, &coerced_channel_name),
                 2, "Channel Name");

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, "OUTP:PROT:CLE (@%s)", coerced_channel_name));
    }
    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ChannelGroupingStatus
 * Purpose:  This function returns a list of output channels which are
 *			 currently grouped together.
 *
 * Note: This command does NOT apply to models N678xA SMU.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ChannelGroupingStatus (ViSession vi,
                                                 ViChar groupedChannels[])
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));


    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        viCheckErr (viQueryf (io, ":SYST:GRO:CAT?", "%s", groupedChannels));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureTriggerSource
 * Purpose:  This function configures the trigger source.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureTriggerSource (ViSession vi,
                                                  ViConstString channelName,
                                                  ViInt32 source)
{
    return Ivi_SetAttributeViInt32 (vi,
                                    channelName,
                                    ZUP366_ATTR_TRIGGER_SOURCE,
                                    IVI_VAL_DIRECT_USER_CALL, source);
}

/*****************************************************************************
 * Function: zup366_ConfigureTriggeredVoltageMode
 * Purpose:  This function configures the trigger voltage mode.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureTriggeredVoltageMode (ViSession vi,
                                                         ViConstString channelName,
                                                         ViInt32 mode)
{
    return Ivi_SetAttributeViInt32 (vi,
                                    channelName,
                                    ZUP366_ATTR_TRIGGER_VOLTAGE_MODE,
                                    IVI_VAL_DIRECT_USER_CALL, mode);
}

/*****************************************************************************
 * Function: zup366_ConfigureTriggeredCurrentMode
 * Purpose:  This function configures the trigger current mode.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureTriggeredCurrentMode (ViSession vi,
                                                         ViConstString channelName,
                                                         ViInt32 mode)
{
    return Ivi_SetAttributeViInt32 (vi,
                                    channelName,
                                    ZUP366_ATTR_TRIGGER_CURRENT_MODE,
                                    IVI_VAL_DIRECT_USER_CALL, mode);
}

/*****************************************************************************
 * Function: zup366_ConfigureTriggeredVoltageLevel
 * Purpose:  This function configures the voltage level the power supply
 *           attempts to generate after it receives a trigger.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureTriggeredVoltageLevel (ViSession vi,
                                                          ViConstString channelName,
                                                          ViReal64 level)
{
    return Ivi_SetAttributeViReal64 (vi, channelName,
                                     ZUP366_ATTR_TRIGGERED_VOLTAGE_LEVEL,
                                     IVI_VAL_DIRECT_USER_CALL, level);
}

/*****************************************************************************
 * Function: zup366_ConfigureTriggeredCurrentLimit
 * Purpose:  This function configures the current limit the power supply
 *           attempts to generate after it receives a trigger.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureTriggeredCurrentLimit (ViSession vi,
                                                          ViConstString channelName,
                                                          ViReal64 limit)
{
    return Ivi_SetAttributeViReal64 (vi, channelName,
                                     ZUP366_ATTR_TRIGGERED_CURRENT_LIMIT,
                                     IVI_VAL_DIRECT_USER_CALL, limit);
}

/*****************************************************************************
 * Function: zup366_Abort
 * Purpose:  This function causes the instrument to no longer wait for a trigger.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_Abort (ViSession vi)
{
    ViStatus error         = VI_SUCCESS;
    ViInt32  channel_count = 4, i, j;
    ViChar   read_buffer[BUFFER_SIZE], channel_buffer[BUFFER_SIZE] = "";

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (Ivi_GetAttributeViInt32 (vi,
                                       VI_NULL,
                                       ZUP366_ATTR_CHANNEL_COUNT,
                                       0, &channel_count));

    checkErr (Ivi_InvalidateAttribute (vi, IVI_VAL_ALL_CHANNELS,
                                       ZUP366_ATTR_TRIGGERED_VOLTAGE_LEVEL));
    checkErr (Ivi_InvalidateAttribute (vi, IVI_VAL_ALL_CHANNELS,
                                       ZUP366_ATTR_TRIGGERED_CURRENT_LIMIT));
    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        /* Query occupied slots */
        viCheckErr (viQueryf (io, ":SYST:GRO:CAT?", "%s", read_buffer));
        RemoveSurroundingDoubleQuotes (read_buffer);

        for (i = 0, j = 0; i < strlen (read_buffer); ++i)
        {
            const char ch = read_buffer[i];

            if (isdigit (ch) || ch == ',')
            {
                channel_buffer[j++] = ch;
            }
        }

        if (channel_count > 1)
            viCheckErr (viPrintf (io, "ABOR:TRAN (@%s)", channel_buffer));
        else
            viCheckErr (viPrintf (io, "ABOR:TRAN (@1)"));
    }

    /*
     *  Do not invoke the zup366_CheckStatus function here.  It
     *  is invoked by the high-level driver functions when this function is
     *  used internally.  After the user calls this function, the user can check
     *  for errors by calling the zup366_error_query function.
     */

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function: zup366_AbortAcquisition
 * Purpose:  This function causes the instrument to no longer acquire
 *           measurement
 *****************************************************************************/
ViStatus _VI_FUNC zup366_AbortAcquisition (ViSession vi,
                                            ViConstString channelList)
{
    ViStatus error                     = VI_SUCCESS;
    ViChar   channel_list[BUFFER_SIZE] = "";

    checkErr (Ivi_LockSession (vi, VI_NULL));


    checkErr (zup366_MakeChannelList (vi,
                                       channelList,
                                       channel_list));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, "ABOR:ACQ (@%s)", channel_list));
    }

    /*
     *  Do not invoke the zup366_CheckStatus function here.  It
     *  is invoked by the high-level driver functions when this function is
     *  used internally.  After the user calls this function, the user can check
     *  for errors by calling the zup366_error_query function.
     */

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_Initiate
 * Purpose:  This function causes the instrument to wait for a trigger from
 *           the trigger source.  After it receives a trigger, it updates the
 *           triggered current limit and triggered voltage level.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_Initiate (ViSession vi)
{
    ViStatus error         = VI_SUCCESS;
    ViInt32  channel_count = 4, i, j;
    ViChar   read_buffer[BUFFER_SIZE], channel_buffer[BUFFER_SIZE] = "";

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL,
                                       ZUP366_ATTR_CHANNEL_COUNT,
                                       0, &channel_count));

    checkErr (Ivi_InvalidateAttribute (vi, IVI_VAL_ALL_CHANNELS,
                                       ZUP366_ATTR_VOLTAGE_LEVEL));
    checkErr (Ivi_InvalidateAttribute (vi, IVI_VAL_ALL_CHANNELS,
                                       ZUP366_ATTR_CURRENT_LIMIT));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        /* Query occupied slots */
        viCheckErr (viQueryf (io, ":SYST:GRO:CAT?", "%s", read_buffer));
        RemoveSurroundingDoubleQuotes (read_buffer);

        for (i = 0, j = 0; i < strlen (read_buffer); ++i)
        {
            const char ch = read_buffer[i];

            if (isdigit (ch) || ch == ',')
            {
                channel_buffer[j++] = ch;
            }
        }

        if (channel_count > 1)
            viCheckErr (viPrintf (io, "INIT:IMM:TRAN (@%s)", channel_buffer));
        else
            viCheckErr (viPrintf (io, "INIT:IMM:TRAN (@1)"));
    }

    /*
     *  Do not invoke the zup366_CheckStatus function here.  It
     *  is invoked by the high-level driver functions when this function is
     *  used internally.  After the user calls this function, the user can check
     *  for errors by calling the zup366_error_query function.
     */

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_InitiateAcqusition
 * Purpose:  This function causes the instrument to start acqusition.
 *
 * Note:On mainframe N6700
 *		This command is used only with models N676xA, N678xA SMU,
 *       or models that have Option 054 (High Speed Test Extensions).
 *****************************************************************************/
ViStatus _VI_FUNC zup366_InitiateAcquisition (ViSession vi,
                                               ViConstString channelList)
{
    ViStatus error                     = VI_SUCCESS;
    ViChar   channel_list[BUFFER_SIZE] = "";

    checkErr (Ivi_LockSession (vi, VI_NULL));


    checkErr (zup366_MakeChannelList (vi,
                                       channelList,
                                       channel_list));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, "INIT:IMM:ACQ (@%s)", channel_list));
    }

    /*
     *  Do not invoke the zup366_CheckStatus function here.  It
     *  is invoked by the high-level driver functions when this function is
     *  used internally.  After the user calls this function, the user can check
     *  for errors by calling the zup366_error_query function.
     */


Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ImmediateAcqusitionTrigger
 * Purpose:  This function sends immediate trigger to the instrument
 *
 * Note: On mainframe N6700
 *		 This command is used only with models N676xA, N678xA SMU, or models
 *		 that have Option 054 (High Speed Test Extensions).
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ImmediateAcquisitionTrigger (ViSession vi,
                                                       ViConstString channelList)
{
    ViStatus error                     = VI_SUCCESS;
    ViChar   channel_list[BUFFER_SIZE] = "";

    checkErr (Ivi_LockSession (vi, VI_NULL));


    checkErr (zup366_MakeChannelList (vi,
                                       channelList,
                                       channel_list));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, ":TRIG:ACQ (@%s)", channel_list));
    }

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ImmediateTransientTrigger
 * Purpose:  This function sends immediate trigger to the instrument
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ImmediateTransientTrigger (ViSession vi,
                                                     ViConstString channelList)
{
    ViStatus error                     = VI_SUCCESS;
    ViChar   channel_list[BUFFER_SIZE] = "";

    checkErr (Ivi_LockSession (vi, VI_NULL));


    checkErr (zup366_MakeChannelList (vi,
                                       channelList,
                                       channel_list));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, ":TRIG:TRAN (@%s)", channel_list));
    }

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_SendSoftwareTrigger
 * Purpose:  This function sends a software trigger to the instrument.  If the
 *           instrument is not configured to receive a software trigger, this
 *           function returns an error.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_SendSoftwareTrigger (ViSession vi)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (Ivi_RangeChecking (vi))
    {
        ViBoolean isSoftware = VI_FALSE;
        ViInt32   numChans;
        ViString  channel;
        ViInt32   i;
        ViInt32   source;

        checkErr (Ivi_GetAttributeViInt32 (vi, VI_NULL,
                                           ZUP366_ATTR_CHANNEL_COUNT,
                                           0, &numChans));
        for (i = 1; i <= numChans && !isSoftware; i++)
        {
            checkErr (Ivi_GetNthChannelString (vi, i, &channel));
            checkErr (Ivi_GetAttributeViInt32 (vi, channel,
                                               ZUP366_ATTR_TRIGGER_SOURCE,
                                               0, &source));
            isSoftware = (source == ZUP366_VAL_SOFTWARE_TRIG);
        }
        if (!isSoftware)
        {
            viCheckErr (ZUP366_ERROR_TRIGGER_NOT_SOFTWARE);
        }
    }

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, "*TRG"));
    }


    /*
     *  Do not invoke the zup366_CheckStatus function here.  It
     *  is invoked by the high-level driver functions when this function is
     *  used internally.  After the user calls this function, the user can check
     *  for errors by calling the zup366_error_query function.
     */

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/**********************************************************************************************************
 * Function: zup366_Measure
 * Purpose:  This function measures either the current or voltage that the
 *           power supply is generating.
 *
 * Note:     The time interval, number of points, and windowing function can be programmed on models N676xA,
 *           N678xA SMU, or models that have Option 054 (High Speed Test Extensions).
 ***********************************************************************************************************/
static IviRangeTableEntry measurementTypeRangeTableEntries[] =
{
    { ZUP366_VAL_MEASURE_CURRENT, 0, 0, "CURR", ZUP366_ATTR_CURRENT_LIMIT },
    { ZUP366_VAL_MEASURE_VOLTAGE, 0, 0, "VOLT", ZUP366_ATTR_VOLTAGE_LEVEL },
	{ ZUP366_VAL_MEASURE_POWER,   0, 0, "POW",  ZUP366_ATTR_OUTPUT_POWER },  
    { IVI_RANGE_TABLE_LAST_ENTRY }
};
static IviRangeTable      measurementTypeRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    measurementTypeRangeTableEntries
};


ViStatus _VI_FUNC zup366_Measure (ViSession vi,
                                   ViConstString channelName,
                                   ViInt32 measurementType,
                                   ViReal64*       measurement)
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;
    ViReal64      value = 0.0;
    ViAttr        attrId;
    ViString      cmd;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (measurement == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 4, "Null value for measurement.");

    viCheckParm (Ivi_CoerceChannelName (vi,
                                        channelName,
                                        &channel_name_coerced),
                 2, "Channel Name");

    viCheckParm (Ivi_GetViInt32EntryFromValue (measurementType,
                                               &measurementTypeRangeTable,
                                               VI_NULL, VI_NULL, VI_NULL,
                                               VI_NULL, &cmd, &attrId),
                 3, "Measurement Type");
    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viQueryf (io, ":MEAS:SCAL:%s? (@%s)", "%Le", cmd, channel_name_coerced, &value));
    }
    else
    {
		if (measurementType != ZUP366_ATTR_OUTPUT_POWER)
		{
        	checkErr (Ivi_GetAttributeViReal64 (vi, channelName, attrId, 0, &value));
        	value *= 0.95;
		}
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (measurement)
        *measurement = value;
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/**********************************************************************************************************
 * Function: zup366_Fetch
 * Purpose: This query returns the DC output current in amperes. The data returned is the result of the
 *          last measurement command or triggered acquisition. The data is valid until the next MEASure
 *          or INITiate command occurs.
 *
 * Note:	 This command is used only with models N676xA, N678xA SMU,
 *           or models that have Option 054 (High Speed Test Extensions).
 ***********************************************************************************************************/
ViStatus _VI_FUNC zup366_Fetch (ViSession vi,
                                 ViConstString channelName,
                                 ViInt32 measurementType,
                                 ViReal64*       measurement)
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;
    ViReal64      value = 0.0;
    ViAttr        attrId;
    ViString      cmd;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (measurement == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 4, "Null value for measurement.");

    viCheckParm (Ivi_CoerceChannelName (vi,
                                        channelName,
                                        &channel_name_coerced),
                 2, "Channel Name");

    viCheckParm (Ivi_GetViInt32EntryFromValue (measurementType,
                                               &measurementTypeRangeTable,
                                               VI_NULL, VI_NULL, VI_NULL,
                                               VI_NULL, &cmd, &attrId),
                 3, "Measurement Type");
    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viQueryf (io, ":FETC:SCAL:%s? (@%s)", "%Le", cmd, channel_name_coerced, &value));
    }
    else
    {
		if (measurementType != ZUP366_ATTR_OUTPUT_POWER)
		{
        	checkErr (Ivi_GetAttributeViReal64 (vi, channelName, attrId, 0, &value));
        	value *= 0.95;
		}
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (measurement)
        *measurement = value;
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_MeasureArray
 * Purpose:  This function measures either the current or voltage that the
 *           power supply is generating.
 *
 * Note:	On mainframe N6700
 *			This command is used only with models N676xA, N678xA SMU,
 *          or models that have Option 054 (High Speed Test Extensions).
 *****************************************************************************/
ViStatus _VI_FUNC zup366_MeasureArray (ViSession vi,
                                        ViConstString channelName,
                                        ViInt32 measurementType,
                                        ViInt32 maximumTime,
                                        ViInt32 arraySize,
                                        ViReal64*       measurementArray,
                                        ViInt32*        actualPoints)
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;
    ViAttr        attrId;
    ViString      cmd;
    ViString      pdata = NULL;


    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (arraySize < 1)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 5, "Array Size is too small.");

    if (measurementArray == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 6, "Null value for measurement.");

    viCheckParm (Ivi_CoerceChannelName (vi,
                                        channelName,
                                        &channel_name_coerced),
                 2, "Channel Name");

    viCheckParm (Ivi_GetViInt32EntryFromValue (measurementType,
                                               &measurementTypeRangeTable,
                                               VI_NULL, VI_NULL, VI_NULL,
                                               VI_NULL, &cmd, &attrId),
                 3, "Measurement Type");
    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, ":MEAS:ARR:%s? (@%s)", cmd, channel_name_coerced));
        checkErr (zup366_ReadUnknownLengthData (vi, io, &pdata, NULL));
        viCheckErr (zup366_array_atof (measurementArray, arraySize, pdata, actualPoints));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (pdata)
        free (pdata);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_FetchArray
 * Purpose:  This function reads either the current or voltage that the
 *           power supply is generating.
 *
 * Note:	 This command is used only with models N676xA, N678xA SMU,
 *			 or models that have Option 054 (High Speed Test Extensions).
 *****************************************************************************/
ViStatus _VI_FUNC zup366_FetchArray (ViSession vi,
                                      ViConstString channelName,
                                      ViInt32 measurementType,
                                      ViInt32 arraySize,
                                      ViReal64*       measurementArray,
                                      ViInt32*        actualPoints)
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;
    ViAttr        attrId;
    ViString      cmd;
    ViString      pdata = NULL;


    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (arraySize < 1)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 5, "Array Size is too small.");

    if (measurementArray == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 6, "Null value for measurement.");

    viCheckParm (Ivi_CoerceChannelName (vi,
                                        channelName,
                                        &channel_name_coerced),
                 2, "Channel Name");

    viCheckParm (Ivi_GetViInt32EntryFromValue (measurementType,
                                               &measurementTypeRangeTable,
                                               VI_NULL, VI_NULL, VI_NULL,
                                               VI_NULL, &cmd, &attrId),
                 3, "Measurement Type");
    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, ":FETC:ARR:%s? (@%s)", cmd, channel_name_coerced));
        checkErr (zup366_ReadUnknownLengthData (vi, io, &pdata, NULL));
        viCheckErr (zup366_array_atof (measurementArray, arraySize, pdata, actualPoints));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (pdata)
        free (pdata);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureListCount
 * Purpose:  This function
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureListCount (ViSession vi,
                                              ViConstString channelName,
                                              ViInt32 listCount)
{
    return Ivi_SetAttributeViInt32 (vi, channelName,
                                    ZUP366_ATTR_LIST_COUNT,
                                    IVI_VAL_DIRECT_USER_CALL,
                                    listCount);
}

/*****************************************************************************
 * Function: zup366_ConfigureList
 * Purpose:  This function configures list properties.
 *
 * Note: List mode is only available with models N676xA, N678xA SMU,
 *		 or models that have Option 054 (High Speed Test Extensions).
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureList (ViSession vi,
                                         ViConstString channelName,
                                         ViInt32 listCount,
                                         ViInt32 listStep,
                                         ViBoolean terminateLast)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));


    if (Ivi_RangeChecking (vi))
    {
        viCheckParm (Ivi_CoerceChannelName (vi,
                                            channelName,
                                            VI_NULL),
                     2, "Channel Name");

        checkErr (zup366_CheckListFunctionality (vi,
                                                  channelName,
                                                  IVI_ERROR_FUNCTION_NOT_SUPPORTED));
    }

    viCheckParm (Ivi_SetAttributeViInt32 (vi,
                                          channelName,
                                          ZUP366_ATTR_LIST_COUNT,
                                          0,
                                          listCount),
                 3, "List Count");
    viCheckParm (Ivi_SetAttributeViInt32 (vi,
                                          channelName,
                                          ZUP366_ATTR_LIST_STEP,
                                          0,
                                          listStep),
                 4, "List Step");

    viCheckParm (Ivi_SetAttributeViBoolean (vi,
                                            channelName,
                                            ZUP366_ATTR_LIST_TERMINATE_LAST,
                                            0,
                                            terminateLast),
                 5, "Terminate Last");

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function: zup366_ConfigureDwellList
 * Purpose:  This function configures the dwell time for each list step.
 *
 * Note: List mode is only available with models N676xA, N678xA SMU, or models
 *       that have Option 054 (High Speed Test Extensions).
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureDwellList (ViSession vi,
                                              ViConstString channelName,
                                              ViInt32 listSize,
                                              ViReal64 dwellTime[])
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;
    ViInt32       i;
    ViChar        *pbuffer     = VI_NULL;
    ViChar        *pbuffer_end = VI_NULL;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_CoerceChannelName (vi,
                                        channelName,
                                        &channel_name_coerced),
                 2, "Channel Name");
    if (Ivi_RangeChecking (vi))
    {
        checkErr (zup366_CheckListFunctionality (vi,
                                                  channelName,
                                                  IVI_ERROR_FUNCTION_NOT_SUPPORTED));

        if (listSize < 1 || listSize > 512)
            viCheckParm (IVI_ERROR_INVALID_VALUE, 3, "List Size");

        for (i = 0; i < listSize; i++)
        {
            if (dwellTime[i] < 0.0 || dwellTime[i] > 262.143)
                viCheckParm (IVI_ERROR_INVALID_VALUE, 4, "Dwell Time");
        }
    }

    /*
     * Allocated buffer for command
     */
    viCheckErr (Ivi_Alloc (vi,
                           MAX_HEADER_COMMAND_LENGTH + listSize * (MAX_DOUBLE_FORMAT_LENGTH + 1),
                           (ViAddr *) &pbuffer));

    pbuffer_end = pbuffer;

    /*
     * Format command
     */
    pbuffer_end += sprintf (pbuffer_end, ":LIST:DWEL ");
    for (i = 0; i < listSize; i++)
    {
        pbuffer_end += sprintf (pbuffer_end, "%le,", dwellTime[i]);
    }
    pbuffer_end += sprintf (pbuffer_end, " (@%s)", channel_name_coerced);


    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, pbuffer));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (pbuffer)
        Ivi_Free (vi, pbuffer);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureVoltageList
 * Purpose:  This function configures the voltage setting for each list step
 *           in volts.
 * Note: List mode is only available with models N676xA, N678xA SMU, or models
 *       that have Option 054 (High Speed Test Extensions).
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureVoltageList (ViSession vi,
                                                ViConstString channelName,
                                                ViInt32 listSize,
                                                ViReal64 voltageList[])
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;
    ViInt32       i;
    ViChar        *pbuffer     = VI_NULL;
    ViChar        *pbuffer_end = VI_NULL;
    ViConstString channel;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_CoerceChannelName (vi,
                                        channelName,
                                        &channel_name_coerced),
                 2, "Channel Name");
    if (Ivi_RangeChecking (vi))
    {
        ViReal64            max_voltage           = 262.143;
        SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
        ViInt32             channel_index;
        ViReal64            minimum;

        viCheckParm (Ivi_CoerceChannelName (vi, channelName, &channel),
                     2, "Channel Name");

        checkErr (zup366_CheckListFunctionality (vi,
                                                  channelName,
                                                  IVI_ERROR_FUNCTION_NOT_SUPPORTED));

        if (listSize < 1 || listSize > 512)
            viCheckParm (IVI_ERROR_INVALID_VALUE, 3, "List Size");

        checkErr (Ivi_GetAttributeViReal64 (vi,
                                            channelName,
                                            ZUP366_ATTR_INSTR_VOLTAGE_RANGE,
                                            0,
                                            &max_voltage));

        checkErr (Ivi_GetAttributeViAddr (vi,
                                          VI_NULL,
                                          ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                          0, (ViAddr *) &pinstrument_modules));
        channel_index = (channel[0] - '0');

        for (i = 0; i < listSize; i++)
        {
            minimum = pinstrument_modules[channel_index]->MinVoltage[MAX_NB_RANGES - 2];
            if (voltageList[i] < minimum || voltageList[i] > max_voltage)
                viCheckParm (IVI_ERROR_INVALID_VALUE, 4, "Voltage");
        }
    }

    /*
     * Allocated buffer for command
     */
    viCheckErr (Ivi_Alloc (vi,
                           MAX_HEADER_COMMAND_LENGTH + listSize * (MAX_DOUBLE_FORMAT_LENGTH + 1),
                           (ViAddr *) &pbuffer));

    pbuffer_end = pbuffer;

    /*
     * Format command
     */
    pbuffer_end += sprintf (pbuffer_end, ":LIST:VOLT ");
    for (i = 0; i < listSize; i++)
    {
        pbuffer_end += sprintf (pbuffer_end, "%le,", voltageList[i]);
    }
    pbuffer_end += sprintf (pbuffer_end, "(@%s)", channel_name_coerced);


    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, pbuffer));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (pbuffer)
        Ivi_Free (vi, pbuffer);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureCurrentList
 * Purpose:  This function configures the current setting for each list step
 *           in amperes.
 * Note: List mode is only available with models N676xA, N678xA SMU, or models
 *       that have Option 054 (High Speed Test Extensions).
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureCurrentList (ViSession vi,
                                                ViConstString channelName,
                                                ViInt32 listSize,
                                                ViReal64 currentList[])
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;
    ViInt32       i;
    ViChar        *pbuffer     = VI_NULL;
    ViChar        *pbuffer_end = VI_NULL;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_CoerceChannelName (vi,
                                        channelName,
                                        &channel_name_coerced),
                 2, "Channel Name");
    if (Ivi_RangeChecking (vi))
    {
        ViReal64            max_current           = 10.0;
        SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
        ViInt32             channel_index;
        ViReal64            minimum;

        checkErr (zup366_CheckListFunctionality (vi,
                                                  channelName,
                                                  IVI_ERROR_FUNCTION_NOT_SUPPORTED));

        if (listSize < 1 || listSize > 512)
            viCheckParm (IVI_ERROR_INVALID_VALUE, 3, "List Size");

        checkErr (Ivi_GetAttributeViReal64 (vi,
                                            channelName,
                                            ZUP366_ATTR_INSTR_CURRENT_RANGE,
                                            0,
                                            &max_current));

        checkErr (Ivi_GetAttributeViAddr (vi,
                                          VI_NULL,
                                          ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                          0, (ViAddr *) &pinstrument_modules));
        channel_index = (channel_name_coerced[0] - '0');

        for (i = 0; i < listSize; i++)
        {
            minimum = pinstrument_modules[channel_index]->MinCurrent[MAX_NB_RANGES - 2];
            if (currentList[i] < minimum || currentList[i] > max_current)
                viCheckParm (IVI_ERROR_INVALID_VALUE, 4, "Current");
        }
    }

    /*
     * Allocated buffer for command
     */
    viCheckErr (Ivi_Alloc (vi,
                           MAX_HEADER_COMMAND_LENGTH + listSize * (MAX_DOUBLE_FORMAT_LENGTH + 1),
                           (ViAddr *) &pbuffer));

    pbuffer_end = pbuffer;

    /*
     * Format command
     */
    pbuffer_end += sprintf (pbuffer_end, ":LIST:CURR ");
    for (i = 0; i < listSize; i++)
    {
        pbuffer_end += sprintf (pbuffer_end, "%le,", currentList[i]);
    }
    pbuffer_end += sprintf (pbuffer_end, " (@%s)", channel_name_coerced);


    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, pbuffer));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (pbuffer)
        Ivi_Free (vi, pbuffer);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureBOSTList
 * Purpose:  This function configures which list steps generate a trigger out
 *           signal at the beginning of the list step.
 *
 * Note: List mode is only available with models N676xA, N678xA SMU, or models
 *       that have Option 054 (High Speed Test Extensions).
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureBOSTList (ViSession vi,
                                             ViConstString channelName,
                                             ViInt32 listSize,
                                             ViBoolean bostList[])
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;
    ViInt32       i;
    ViChar        *pbuffer     = VI_NULL;
    ViChar        *pbuffer_end = VI_NULL;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_CoerceChannelName (vi,
                                        channelName,
                                        &channel_name_coerced),
                 2, "Channel Name");
    if (Ivi_RangeChecking (vi))
    {
        checkErr (zup366_CheckListFunctionality (vi,
                                                  channelName,
                                                  IVI_ERROR_FUNCTION_NOT_SUPPORTED));

        if (listSize < 1 || listSize > 512)
            viCheckParm (IVI_ERROR_INVALID_VALUE, 3, "List Size");
    }

    /*
     * Allocated buffer for command
     */
    viCheckErr (Ivi_Alloc (vi,
                           MAX_HEADER_COMMAND_LENGTH + listSize * (MAX_BOOL_FORMAT_LENGTH + 1),
                           (ViAddr *) &pbuffer));

    pbuffer_end = pbuffer;

    /*
     * Format command
     */
    pbuffer_end += sprintf (pbuffer_end, ":LIST:TOUT:BOST ");
    for (i = 0; i < listSize; i++)
    {
        pbuffer_end += sprintf (pbuffer_end, (bostList[i]) ? "ON," : "OFF,");
    }
    pbuffer_end += sprintf (pbuffer_end, " (@%s)", channel_name_coerced);


    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, pbuffer));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (pbuffer)
        Ivi_Free (vi, pbuffer);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureEOSTList
 * Purpose:  This function configures which list steps generate a trigger out
 *           signal at the end of the step's dwell time.
 *
 * Note: List mode is only available with models N676xA, N678xA SMU, or models
 * that have Option 054 (High Speed Test Extensions).
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureEOSTList (ViSession vi,
                                             ViConstString channelName,
                                             ViInt32 listSize,
                                             ViBoolean eostList[])
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;
    ViInt32       i;
    ViChar        *pbuffer     = VI_NULL;
    ViChar        *pbuffer_end = VI_NULL;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_CoerceChannelName (vi,
                                        channelName,
                                        &channel_name_coerced),
                 2, "Channel Name");
    if (Ivi_RangeChecking (vi))
    {
        checkErr (zup366_CheckListFunctionality (vi,
                                                  channelName,
                                                  IVI_ERROR_FUNCTION_NOT_SUPPORTED));

        if (listSize < 1 || listSize > 512)
            viCheckParm (IVI_ERROR_INVALID_VALUE, 3, "List Size");
    }

    /*
     * Allocated buffer for command
     */
    viCheckErr (Ivi_Alloc (vi,
                           MAX_HEADER_COMMAND_LENGTH + listSize * (MAX_BOOL_FORMAT_LENGTH + 1),
                           (ViAddr *) &pbuffer));

    pbuffer_end = pbuffer;

    /*
     * Format command
     */
    pbuffer_end += sprintf (pbuffer_end, ":LIST:TOUT:EOST ");
    for (i = 0; i < listSize; i++)
    {
        pbuffer_end += sprintf (pbuffer_end, (eostList[i]) ? "ON," : "OFF,");
    }
    pbuffer_end += sprintf (pbuffer_end, " (@%s)", channel_name_coerced);


    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, pbuffer));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (pbuffer)
        Ivi_Free (vi, pbuffer);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureMeasurement
 * Purpose:  This function configures the power supply's measurement range on
 *           a channel. You specify whether you want to configure the voltage
 *           or current range, and the value to which to set the range.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureMeasurement (ViSession vi,
                                                ViConstString channelName,
                                                ViInt32 measurementType,
                                                ViInt32 offsetPoints,
                                                ViInt32 points,
                                                ViReal64 sampleInterval)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (Ivi_RangeChecking (vi))
    {
        viCheckParm (Ivi_CoerceChannelName (vi,
                                            channelName,
                                            NULL),
                     2, "Channel Name");
    }

    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    /* The measurement command is ignored when programming modules N6761A, N6762A and SMU (N6781A, N6782A, and n6784A) modules.
     *  This is because these modules measure both voltage and current simultaneously. */
    if ((model_type != N6761A) || (model_type != N6762A) || (model_type != N6781A) || (model_type != N6782A) || (model_type != N6784A))
    {
        viCheckParm (Ivi_SetAttributeViInt32 (vi,
                                              channelName,
                                              ZUP366_ATTR_MEASUREMENT_FUNCTION,
                                              0, measurementType), 3, "Measurement Type");
    }

    /* The attributes below are valid only with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).*/
    viCheckParm (Ivi_SetAttributeViInt32 (vi,
                                          channelName,
                                          ZUP366_ATTR_SWEEP_OFFSET_POINTS,
                                          0, offsetPoints), 4, "Trigger Offset Points");

    viCheckParm (Ivi_SetAttributeViInt32 (vi,
                                          channelName,
                                          ZUP366_ATTR_SWEEP_POINTS,
                                          0, points), 5, "Points");

    viCheckParm (Ivi_SetAttributeViReal64 (vi,
                                           channelName,
                                           ZUP366_ATTR_SWEEP_TIME_INTERVAL,
                                           0, sampleInterval), 6, "Sample Interval");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureMeasurementRange
 * Purpose:  This function configures the power supply's measurement range on
 *           a channel. You specify whether you want to configure the voltage
 *           or current range, and the value to which to set the range.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureMeasurementRange (ViSession vi,
                                                     ViConstString channelName,
                                                     ViInt32 rangeType,
                                                     ViReal64 range)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (Ivi_RangeChecking (vi))
    {
        SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
        ViConstString       pchannel_name;
        ViInt32             channel_index = 0;
        ViReal64            maximum;
        ViReal64            minimum;

        viCheckParm (Ivi_CoerceChannelName (vi, channelName, &pchannel_name),
                     2, "Channel Name");

        viCheckParm (Ivi_GetViInt32EntryFromValue (rangeType,
                                                   &rangeTypeRangeTable,
                                                   VI_NULL, VI_NULL, VI_NULL,
                                                   VI_NULL, VI_NULL, VI_NULL),
                     3, "Range Type");

        checkErr (Ivi_GetAttributeViAddr (vi,
                                          VI_NULL,
                                          ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                          0, (ViAddr *) &pinstrument_modules));
        channel_index = (pchannel_name[0] - '0');



        if (rangeType == ZUP366_VAL_RANGE_VOLTAGE)
        {
            minimum = pinstrument_modules[channel_index]->MinVoltage[MAX_NB_RANGES - 2];
            maximum = pinstrument_modules[channel_index]->MaxVoltage[MAX_NB_RANGES - 1];
        }
        else
        {
            minimum = pinstrument_modules[channel_index]->MinCurrent[MAX_NB_RANGES - 2];
            maximum = pinstrument_modules[channel_index]->MaxCurrent[MAX_NB_RANGES - 1];
        }

        if (range < minimum || range > maximum)
        {
            viCheckParm (IVI_ERROR_INVALID_VALUE, 4, "Range");
        }
    }

    checkErr (Ivi_SetAttributeViReal64 (vi,
                                        channelName,
                                        (rangeType == ZUP366_VAL_RANGE_VOLTAGE) ?
                                        ZUP366_ATTR_INSTR_VOLTAGE_MEASUREMENT_RANGE :
                                        ZUP366_ATTR_INSTR_CURRENT_MEASUREMENT_RANGE,
                                        0, range));

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigurePin
 * Purpose:  This function configures digital port pin functions and
 *           polarity.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigurePin (ViSession vi,
                                        ViConstString pinName,
                                        ViInt32 pinFunction,
                                        ViInt32 pinPolarity)
{
    ViStatus error        = VI_SUCCESS;
    ViInt32  pin          = 1;
    ViAttr   attributeId1 = NULL, attributeId2 = NULL;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (pinName && (strncmp (pinName, "PIN", 3) == 0) && strlen (pinName) == 4)
    {
        pin = *(pinName + 3) - '0';
        if (pin < 1 || pin > 7)
            error = IVI_ERROR_INVALID_VALUE;
    }
    else
        error = IVI_ERROR_INVALID_VALUE;
    viCheckParm (error, 2, "Pin Name");



    switch (pin)
    {
    case 1:
        attributeId1 = ZUP366_ATTR_DIGITAL_PIN1_FUNCTION;
        attributeId2 = ZUP366_ATTR_DIGITAL_PIN1_POLARITY;
        break;

    case 3:
        attributeId1 = ZUP366_ATTR_DIGITAL_PIN3_FUNCTION;
        attributeId2 = ZUP366_ATTR_DIGITAL_PIN3_POLARITY;
        break;

    case 4:
        attributeId1 = ZUP366_ATTR_DIGITAL_PIN4_FUNCTION;
        attributeId2 = ZUP366_ATTR_DIGITAL_PIN4_POLARITY;
        break;

    case 5:
        attributeId1 = ZUP366_ATTR_DIGITAL_PIN5_FUNCTION;
        attributeId2 = ZUP366_ATTR_DIGITAL_PIN5_POLARITY;
        break;

    case 6:
        attributeId1 = ZUP366_ATTR_DIGITAL_PIN6_FUNCTION;
        attributeId2 = ZUP366_ATTR_DIGITAL_PIN6_POLARITY;
        break;

    case 7:
        attributeId1 = ZUP366_ATTR_DIGITAL_PIN7_FUNCTION;
        attributeId2 = ZUP366_ATTR_DIGITAL_PIN7_POLARITY;
        break;

    case 2:
    default:
        attributeId1 = ZUP366_ATTR_DIGITAL_PIN2_FUNCTION;
        attributeId2 = ZUP366_ATTR_DIGITAL_PIN2_POLARITY;
        break;
    }

    viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, attributeId1, 0, pinFunction), 3, "Pin Function");
    viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, attributeId2, 0, pinPolarity), 4, "Pin Polarity");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function: zup366_WriteToOuputPort
 * Purpose:  This function sets the output data on the digital control port
 *           when that port is configured for Digital I/O operation.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_WriteToOuputPort (ViSession vi,
                                            ViInt32 data)
{
    return Ivi_SetAttributeViInt32 (vi,
                                    VI_NULL,
                                    ZUP366_ATTR_DIGITAL_OUTPUT_DATA,
                                    IVI_VAL_DIRECT_USER_CALL,
                                    data);
}


/*****************************************************************************
 * Function: zup366_ReadInputPort
 * Purpose:  This function returns the state of the digital control port.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ReadInputPort (ViSession vi,
                                         ViInt32*        data)
{
    return Ivi_GetAttributeViInt32 (vi,
                                    VI_NULL,
                                    ZUP366_ATTR_DIGITAL_INPUT_DATA,
                                    IVI_VAL_DIRECT_USER_CALL,
                                    data);
}

/*****************************************************************************
 * Function: zup366_ConfigureChannelGrouping
 * Purpose:  This function defines a list of output channels as a paralleled
 *			 group.
 *
 * Note: This command does NOT apply to models N678xA SMU.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureChannelGrouping (ViSession vi,
                                                    ViConstString channelName)
{
    ViStatus error              = VI_SUCCESS;
    ViChar   group[BUFFER_SIZE] = "";

    checkErr (Ivi_LockSession (vi, VI_NULL));

    sprintf (group, "(@%s)", channelName);

    checkErr (Ivi_SetAttributeViString (vi,
                                        VI_NULL,
                                        ZUP366_ATTR_GROUP_CHANNELS,
                                        0,
                                        group));

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_UngroupChannels
 * Purpose:  This function restores a group of channels that have been
 *           grouped with zup366_ConfigureChannelGrouping back to an
 *			 ungrouped state.
 *
 * Note: This command does NOT apply to models N678xA SMU.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_UngroupChannels (ViSession vi,
                                           ViConstString channelName,
                                           ViBoolean ungroupAll)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (ungroupAll)
    {
        if (!Ivi_Simulating (vi))
        {
            ViSession io = Ivi_IOSession (vi);  /* call only when locked */

            viCheckErr (viPrintf (io, ":SYST:GRO:DEL:ALL"));
        }
    }
    else
    {
        checkErr (Ivi_SetAttributeViString (vi,
                                            VI_NULL,
                                            ZUP366_ATTR_UNGROUP_CHANNEL,
                                            0,
                                            channelName));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureWaveformParameters
 * Purpose:  Configures parameters for Waveforms.
 *           Sets the waveform type, shape, count and the output value when
 *			 the waveform terminates.
 *
 * Notes:   1)Waveform capability is only available on the N6705B DC Power Analyzer
 *			2)If ZUP366_VAL_WAVEFORM_SEQUENCE is selected as a Waveform
 *		    Shape the ConfigureSequenceParameters function needs run to
 *		    configure the Sequence Parameters.
 *
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureWaveformParameters (ViSession vi,
                                                       ViChar channelName[],
                                                       ViInt32 waveformShape,
                                                       ViInt32 waveformMeasurementType,
                                                       ViInt32 waveformCount,
                                                       ViBoolean enableWaveformTermination)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (Ivi_RangeChecking (vi))
    {
        viCheckParm (Ivi_CoerceChannelName (vi, channelName, NULL), 2, "Channel Name");
    }

    viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                          ZUP366_ATTR_WAVEFORM_SHAPE, 0, waveformShape),
                 3, "Waveform Shape");
    viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                          ZUP366_ATTR_WAVEFORM_MEASUREMENT, 0, waveformMeasurementType),
                 4, "Waveform Measurement Type");
    viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                          ZUP366_ATTR_WAVEFORM_COUNT, 0, waveformCount),
                 5, "Waveform Count");
    viCheckParm (Ivi_SetAttributeViBoolean (vi, channelName,
                                            ZUP366_ATTR_WAVEFORM_TERMINATION,
                                            0, enableWaveformTermination),
                 6, "Enable Waveform Termination");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*************************************************************************************
 * Function: zup366_ConfigureSequenceParameters
 * Purpose:  Configures parameters for Sequences.
 *			 Sets the shape, pacing, step number, step repeat count,
 *			 the sequence count and the output value when the sequence terminates.
 *
 * Notes :   1) Sequence capability is only available on the N6705B DC Power Analyzer.
 *			 2) zup366_ConfigureWaveformParameters needs to be set to
 *			 ZUP366_VAL_WAVEFORM_SEQUENCE before running this function.
 *
 **************************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureSequenceParameters (ViSession vi,
                                                       ViChar channelName[],
                                                       ViInt32 sequenceMeasurementType,
                                                       ViInt32 sequenceStepNumber,
                                                       ViInt32 sequenceShape,
                                                       ViInt32 pacing,
                                                       ViInt32 sequenceStepCount,
                                                       ViInt32 sequenceCount,
                                                       ViBoolean enableSequenceTermination)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));
    viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                          ZUP366_ATTR_SEQUENCE_MEASUREMENT, 0, sequenceMeasurementType),
                 3, "Sequence Measurement Type");
    viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                          ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, sequenceStepNumber),
                 4, "Sequence Step Number");
    viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                          ZUP366_ATTR_SEQUENCE_SHAPE, 0, sequenceShape),
                 5, "Sequence Shape");
    viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                          ZUP366_ATTR_SEQUENCE_PACING, 0, pacing),
                 6, "Pacing");
    viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                          ZUP366_ATTR_SEQUENCE_STEP_COUNT, 0, sequenceStepCount),
                 7, "Sequence Step Count");
    viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                          ZUP366_ATTR_SEQUENCE_COUNT, 0, sequenceCount),
                 8, "Sequence Count");
    viCheckParm (Ivi_SetAttributeViBoolean (vi, channelName,
                                            ZUP366_ATTR_SEQUENCE_TERMINATION, 0, enableSequenceTermination),
                 9, "Enable Sequence Termination");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************************************
 * Function: zup366_ConfigureExponentialWfmSeq
 * Purpose: Configures the exponential waveform or sequence.
 * Note: The Waveform and Sequence capability is only available on the N6705B DC Power Analyzer.
 *
 *****************************************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureExponentialWfmSeq (ViSession vi,
                                                      ViChar channelName[],
                                                      ViInt32 sequenceStepNumber,
                                                      ViReal64 exponentialStartLevel,
                                                      ViReal64 exponentialEndLevel,
                                                      ViReal64 exponentialStartTime,
                                                      ViReal64 exponentialTimeConstant,
                                                      ViReal64 exponentialTotalTime)

{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureSeqShapeValue;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_SHAPE, 0, &measureSeqShapeValue));

    if (measureSeqShapeValue == ZUP366_VAL_SEQUENCE_EXPONENTIAL)
        viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                              ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, sequenceStepNumber),
                     3, "Sequence Step Number");

    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_EXP_START_LEVEL, 0, exponentialStartLevel),
                 4, "Exponential Start Level");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_EXP_END_LEVEL, 0, exponentialEndLevel),
                 5, "Exponential End Level");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_EXP_START_TIME, 0, exponentialStartTime),
                 6, "Exponential Start Time");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_EXP_TIME_CONSTANT, 0, exponentialTimeConstant),
                 7, "Exponential Time Constant");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_EXP_TOTAL_TIME, 0, exponentialTotalTime),
                 8, "Exponential Total Time");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/************************************************************************************************
 * Function: zup366_ConfigurePulseWfmSeq
 * Purpose: Configures the pulse waveform or sequence.
 * Note: The Waveform and Sequence capability is only available on the N6705B DC Power Analyzer.
 *
 ************************************************************************************************/
ViStatus _VI_FUNC zup366_ConfigurePulseWfmSeq (ViSession vi,
                                                ViChar channelName[],
                                                ViInt32 sequenceStepNumber,
                                                ViReal64 pulseStartLevel,
                                                ViReal64 pulseAmplitude,
                                                ViReal64 pulseStartTime,
                                                ViReal64 pulseWidth,
                                                ViReal64 pulseEndTime)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureSeqShapeValue;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_SHAPE, 0, &measureSeqShapeValue));

    if (measureSeqShapeValue == ZUP366_VAL_SEQUENCE_PULSE)
        viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                              ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, sequenceStepNumber),
                     3, "Sequence Step Number");

    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_PULS_START_LEVEL, 0, pulseStartLevel),
                 4, "Pulse Start Level");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_PULS_AMPL_TLEV, 0, pulseAmplitude),
                 5, "Pulse Amplitude");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_PULS_START_TIME, 0, pulseStartTime),
                 6, "Pulse Start Time");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_PULS_WIDTH_TTIME, 0, pulseWidth),
                 7, "Pulse Width");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_PULS_END_TIME, 0, pulseEndTime),
                 8, "Pulse End Time");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/************************************************************************************************
 * Function: zup366_ConfigureRampWfmSeq
 * Purpose: Configures the ramp waveform or sequence.
 * Note: The Waveform and Sequence capability is only available on the N6705B DC Power Analyzer.
 *
 ************************************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureRampWfmSeq (ViSession vi,
                                               ViChar channelName[],
                                               ViInt32 sequenceStepNumber,
                                               ViReal64 rampStartLevel,
                                               ViReal64 rampEndLevel,
                                               ViReal64 rampStartTime,
                                               ViReal64 rampEndTime,
                                               ViReal64 rampRiseTime)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureSeqShapeValue;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_SHAPE, 0, &measureSeqShapeValue));

    if (measureSeqShapeValue == ZUP366_VAL_SEQUENCE_RAMP)
        viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                              ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, sequenceStepNumber),
                     3, "Sequence Step Number");

    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_RAMP_START_LEVEL, 0, rampStartLevel),
                 4, "Ramp Start Level");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_RAMP_END_LEVEL, 0, rampEndLevel),
                 5, "Ramp End Level");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_RAMP_START_TIME, 0, rampStartTime),
                 6, "Ramp Start Time");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_RAMP_END_TIME, 0, rampEndTime),
                 7, "Ramp End Time");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_RAMP_RISE_TIME, 0, rampRiseTime),
                 8, "Ramp Rise Time");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/***********************************************************************************************
 * Function: zup366_ConfigureSinusoidWfmSeq
 * Purpose: Configures the sine waveform or sequence.
 * Note: The Waveform and Sequence capability is only available on the N6705B DC Power Analyzer.
 *
 ***********************************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureSinusoidWfmSeq (ViSession vi,
                                                   ViChar channelName[],
                                                   ViInt32 sequenceStepNumber,
                                                   ViReal64 amplitude,
                                                   ViReal64 offset,
                                                   ViReal64 frequency)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureSeqShapeValue;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_SHAPE, 0, &measureSeqShapeValue));

    if (measureSeqShapeValue == ZUP366_VAL_SEQUENCE_SINSUOID)
        viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                              ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, sequenceStepNumber),
                     3, "Sequence Step Number");

    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_AMPLITUDE, 0, amplitude),
                 4, "Amplitude");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_FREQUENCY, 0, frequency),
                 5, "Frequency");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_OFFSET, 0, offset),
                 6, "Offset");

    checkErr (zup366_CheckStatus (vi));


Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*************************************************************************************************
 * Function: zup366_ConfigureStaircaseWfmSeq
 * Purpose: Configures the staircase waveform or sequence.
 * Note: The Waveform and Sequence capability is only available on the N6705B DC Power Analyzer.
 *
 *************************************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureStaircaseWfmSeq (ViSession vi,
                                                    ViChar channelName[],
                                                    ViInt32 sequenceStepNumber,
                                                    ViReal64 staircaseStartLevel,
                                                    ViReal64 staircaseEndLevel,
                                                    ViReal64 staircaseStartTime,
                                                    ViReal64 staircaseEndTime,
                                                    ViReal64 staircaseTotalTime,
                                                    ViInt32 staircaseNSteps)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureSeqShapeValue;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_SHAPE, 0, &measureSeqShapeValue));

    if (measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STAIRCASE)
        viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                              ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, sequenceStepNumber),
                     3, "Sequence Step Number");

    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_STAIR_START_LEVEL, 0, staircaseStartLevel),
                 4, "Staircase Start Level");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_STAIR_END_LEVEL, 0, staircaseEndLevel),
                 5, "Staircase End Level");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_STAIR_START_TIME, 0, staircaseStartTime),
                 6, "Staircase Start Time");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_STAIR_END_TIME, 0, staircaseEndTime),
                 7, "Staircase End Time");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_STAIR_TOTAL_TIME, 0, staircaseTotalTime),
                 8, "Staircase Total Time");
    viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                          ZUP366_ATTR_STAIR_NSTEPS, 0, staircaseNSteps),
                 9, "Staircase N Steps");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/***********************************************************************************************
 * Function: zup366_ConfigureStepWfmSeq
 * Purpose:  Configures the step waveform or sequence.
 * Note: The Waveform and Sequence capability is only available on the N6705B DC Power Analyzer.
 *
 ***********************************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureStepWfmSeq (ViSession vi,
                                               ViChar channelName[],
                                               ViInt32 sequenceStepNumber,
                                               ViReal64 stepStartLevel,
                                               ViReal64 stepEndLevel,
                                               ViReal64 stepStartTime,
                                               ViReal64 stepEndTime)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureSeqShapeValue;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_SHAPE, 0, &measureSeqShapeValue));

    if (measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STEP)
        viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                              ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, sequenceStepNumber),
                     3, "Sequence Step Number");

    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_STEP_START_LEVEL, 0, stepStartLevel),
                 4, "Step Start Level");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_STEP_END_LEVEL, 0, stepEndLevel),
                 5, "Step End Level");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_STEP_START_TIME, 0, stepStartTime),
                 6, "Step Start Time");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_STEP_END_TIME, 0, stepEndTime),
                 7, "Step End Time");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/**************************************************************************************************
 * Function: zup366_ConfigureTrapezoidWfmSeq
 * Purpose: Configures the trapezoid waveform or sequence.
 * Note: The Waveform and Sequence capability is only available on the N6705B DC Power Analyzer.
 *
 ***************************************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureTrapezoidWfmSeq (ViSession vi,
                                                    ViChar channelName[],
                                                    ViInt32 sequenceStepNumber,
                                                    ViReal64 trapezoidStartLevel,
                                                    ViReal64 trapezoidPeak,
                                                    ViReal64 trapezoidStartTime,
                                                    ViReal64 trapezoidRiseTime,
                                                    ViReal64 trapezoidWidth,
                                                    ViReal64 trapezoidFallTime,
                                                    ViReal64 trapezoidEndTime)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureSeqShapeValue;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_SHAPE, 0, &measureSeqShapeValue));

    if (measureSeqShapeValue == ZUP366_VAL_SEQUENCE_TRAPEZOID)
        viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                              ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, sequenceStepNumber),
                     3, "Sequence Step Number");

    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_TRAP_START_LEVEL, 0, trapezoidStartLevel),
                 4, "Trapezoid Start Level");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_TRAP_PEAK_TLEV, 0, trapezoidPeak),
                 5, "Trapezoid Peak");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_TRAP_START_TIME, 0, trapezoidStartTime),
                 6, "Trapezoid Start Time");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_TRAP_RISE_TIME, 0, trapezoidRiseTime),
                 7, "Trapezoid Rise Time");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_TRAP_WIDTH_TTIME, 0, trapezoidWidth),
                 8, "Trapezoid Width");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_TRAP_FALL_TIME, 0, trapezoidFallTime),
                 9, "Trapezoid Fall Time");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_TRAP_END_TIME, 0, trapezoidEndTime),
                 10, "Trapezoid End Time");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*********************************************************************************
 * Function: zup366_ConfigureConstantDwellWaveform
 * Purpose:  Configures the constant dwell waveform.
 * Note: Waveform capability is only available on the N6705B DC Power Analyzer.
 *
 **********************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureConstantDwellWaveform (ViSession vi,
                                                          ViChar channelName[],
                                                          ViInt32 constantDwellLevelWfmSize,
                                                          ViReal64 constantDwellLevel[],
                                                          ViReal64 constantDwellTime)
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;
    ViInt32       iCDWDLCmd, measureTypeValue, measureShapeValue;
    ViChar        *pCDWDLbuffer        = VI_NULL, *pCDWDLbuffer_end = VI_NULL;
    ViChar        *pCDWDwellTimebuffer = VI_NULL, *pCDWDwellTimebuffer_end = VI_NULL;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_CoerceChannelName (vi, channelName, &channel_name_coerced), 2, "Channel Name");
    if (Ivi_RangeChecking (vi))
    {
        if (constantDwellLevelWfmSize < 1 || constantDwellLevelWfmSize > 65535)
            viCheckParm (IVI_ERROR_INVALID_VALUE, 3, "Constant Dwell Level Wfm Size");

        if (constantDwellTime < 0.00001024 || constantDwellTime > 0.30)
            viCheckParm (IVI_ERROR_INVALID_VALUE, 5, "Constant Dwell Time");
    }

    /*Waveform*/
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_MEASUREMENT, 0, &measureTypeValue));

    /* Allocated buffer for CDW Dwell Level command for constant dwell waveform */
    viCheckErr (Ivi_Alloc (vi, MAX_HEADER_COMMAND_LENGTH + constantDwellLevelWfmSize * (MAX_DOUBLE_FORMAT_LENGTH + 1), (ViAddr *) &pCDWDLbuffer));
    pCDWDLbuffer_end = pCDWDLbuffer;

    /* Format CDW Dwell Level command for constant dwell waveform */
    if (measureShapeValue == ZUP366_VAL_WAVEFORM_CONSTANTDWELL && measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE)
        pCDWDLbuffer_end += sprintf (pCDWDLbuffer_end, ":SOUR:ARB:VOLT:CDW ");
    else if (measureShapeValue == ZUP366_VAL_WAVEFORM_CONSTANTDWELL && measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_CURRENT)
        pCDWDLbuffer_end += sprintf (pCDWDLbuffer_end, ":SOUR:ARB:CURR:CDW ");

    /* CDW Dwell Level array formatting*/
    for (iCDWDLCmd = 0; iCDWDLCmd < constantDwellLevelWfmSize; iCDWDLCmd++)
    {
        pCDWDLbuffer_end += sprintf (pCDWDLbuffer_end, "%le,", constantDwellLevel[iCDWDLCmd]);
    }
    pCDWDLbuffer_end += sprintf (pCDWDLbuffer_end, " (@%s)", channel_name_coerced);

    /* Allocated buffer for User Defined Dwell Level command for user defined waveform */
    viCheckErr (Ivi_Alloc (vi, MAX_HEADER_COMMAND_LENGTH + (MAX_DOUBLE_FORMAT_LENGTH + 1), (ViAddr *) &pCDWDwellTimebuffer));
    pCDWDwellTimebuffer_end = pCDWDwellTimebuffer;

    /* Format Dwell Time command for constant dwell waveform */
    if (measureShapeValue == ZUP366_VAL_WAVEFORM_CONSTANTDWELL && measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE)
        pCDWDwellTimebuffer_end += sprintf (pCDWDwellTimebuffer_end, ":SOUR:ARB:VOLT:CDW:DWEL %Le,(@%s)", constantDwellTime, channelName);
    else if (measureShapeValue == ZUP366_VAL_WAVEFORM_CONSTANTDWELL && measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_CURRENT)
        pCDWDwellTimebuffer_end += sprintf (pCDWDwellTimebuffer_end, ":SOUR:ARB:CURR:CDW:DWEL %Le,(@%s)", constantDwellTime, channelName);

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, pCDWDLbuffer));
        viCheckErr (viPrintf (io, pCDWDwellTimebuffer));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (pCDWDLbuffer)
        Ivi_Free (vi, pCDWDLbuffer);
    if (pCDWDwellTimebuffer)
        Ivi_Free (vi, pCDWDwellTimebuffer);

    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*******************************************************************************
 * Function: zup366_ConfigureUserDefinedWaveform
 * Purpose:  Configures the user defined waveform.
 * Note: Waveform capability is only available on the N6705B DC Power Analyzer.
 *
 ********************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureUserDefinedWaveform (ViSession vi,
                                                        ViChar channelName[],
                                                        ViInt32 userDefinedBOSTSize,
                                                        ViInt32 userDefinedBeginningStep[],
                                                        ViInt32 userDefinedDwellTimeSize,
                                                        ViReal64 userDefinedDwellTime[],
                                                        ViInt32 userDefinedLevelSize,
                                                        ViReal64 userDefinedLevel[])
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;
    ViInt32       iDTS, iBOSTCmd, iUDDTCmd, iUDDLCmd, measureTypeValue, measureShapeValue;
    ViChar        *pBOSTbuffer     = VI_NULL, *pBOSTbuffer_end = VI_NULL, *pDTimbuffer = VI_NULL;
    ViChar        *pDTimbuffer_end = VI_NULL, *pDLevbuffer = VI_NULL, *pDLevbuffer_end = VI_NULL;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_CoerceChannelName (vi, channelName, &channel_name_coerced), 2, "Channel Name");

    if (Ivi_RangeChecking (vi))
    {
        if (userDefinedBOSTSize < 1 || userDefinedBOSTSize > 512)
            viCheckParm (IVI_ERROR_INVALID_VALUE, 3, "User Defined BOST Size");

        if (userDefinedDwellTimeSize < 1 || userDefinedDwellTimeSize > 512)
            viCheckParm (IVI_ERROR_INVALID_VALUE, 5, "User Defined Dwell Time Size");
        for (iDTS = 0; iDTS < userDefinedDwellTimeSize; iDTS++)
        {
            if (userDefinedDwellTime[iDTS] < 0.0 || userDefinedDwellTime[iDTS] > 262.143)
                viCheckParm (IVI_ERROR_INVALID_VALUE, 6, "User Defined Dwell Time");
        }

        if (userDefinedLevelSize < 1 || userDefinedLevelSize > 512)
            viCheckParm (IVI_ERROR_INVALID_VALUE, 7, "User Defined Level Size");
    }

    /*Waveform*/
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_MEASUREMENT, 0, &measureTypeValue));

    /* Allocated buffer for User Defined Beginning Step command */
    viCheckErr (Ivi_Alloc (vi, MAX_HEADER_COMMAND_LENGTH + userDefinedBOSTSize * (MAX_BOOL_FORMAT_LENGTH + 1), (ViAddr *) &pBOSTbuffer));
    pBOSTbuffer_end = pBOSTbuffer;

    /* Format Beginning Step command for user defined waveform*/
    if (measureShapeValue == ZUP366_VAL_WAVEFORM_UDEFINED && measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE)
        pBOSTbuffer_end += sprintf (pBOSTbuffer_end, ":SOUR:ARB:VOLT:UDEF:BOST ");
    else if (measureShapeValue == ZUP366_VAL_WAVEFORM_UDEFINED && measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_CURRENT)
        pBOSTbuffer_end += sprintf (pBOSTbuffer_end, ":SOUR:ARB:CURR:UDEF:BOST ");

    /*BOST array formatting*/
    for (iBOSTCmd = 0; iBOSTCmd < userDefinedBOSTSize; iBOSTCmd++)
    {
        pBOSTbuffer_end += sprintf (pBOSTbuffer_end, (userDefinedBeginningStep[iBOSTCmd]) ? "ON," : "OFF,");
    }
    pBOSTbuffer_end += sprintf (pBOSTbuffer_end, " (@%s)", channel_name_coerced);

    /* Allocated buffer for User Defined Dwell Time command */
    viCheckErr (Ivi_Alloc (vi, MAX_HEADER_COMMAND_LENGTH + userDefinedDwellTimeSize * (MAX_DOUBLE_FORMAT_LENGTH + 1), (ViAddr *) &pDTimbuffer));
    pDTimbuffer_end = pDTimbuffer;

    /* Format User Defined Dwell Time command for user defined waveform */
    if (measureShapeValue == ZUP366_VAL_WAVEFORM_UDEFINED && measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE)
        pDTimbuffer_end += sprintf (pDTimbuffer_end, ":SOUR:ARB:VOLT:UDEF:DWEL ");
    else if (measureShapeValue == ZUP366_VAL_WAVEFORM_UDEFINED && measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_CURRENT)
        pDTimbuffer_end += sprintf (pDTimbuffer_end, ":SOUR:ARB:CURR:UDEF:DWEL ");

    for (iUDDTCmd = 0; iUDDTCmd < userDefinedDwellTimeSize; iUDDTCmd++)
    {
        pDTimbuffer_end += sprintf (pDTimbuffer_end, "%le,", userDefinedDwellTime[iUDDTCmd]);
    }
    pDTimbuffer_end += sprintf (pDTimbuffer_end, " (@%s)", channel_name_coerced);

    /* Allocated buffer for User Defined Dwell Level command for user defined waveform */
    viCheckErr (Ivi_Alloc (vi, MAX_HEADER_COMMAND_LENGTH + userDefinedLevelSize * (MAX_DOUBLE_FORMAT_LENGTH + 1), (ViAddr *) &pDLevbuffer));
    pDLevbuffer_end = pDLevbuffer;

    /* Format User Defined Dwell Level command for user defined waveform */
    if (measureShapeValue == ZUP366_VAL_WAVEFORM_UDEFINED && measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE)
        pDLevbuffer_end += sprintf (pDLevbuffer_end, ":SOUR:ARB:VOLT:UDEF:LEV ");
    else if (measureShapeValue == ZUP366_VAL_WAVEFORM_UDEFINED && measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_CURRENT)
        pDLevbuffer_end += sprintf (pDLevbuffer_end, ":SOUR:ARB:CURR:UDEF:LEV ");

    for (iUDDLCmd = 0; iUDDLCmd < userDefinedLevelSize; iUDDLCmd++)
    {
        pDLevbuffer_end += sprintf (pDLevbuffer_end, "%le,", userDefinedLevel[iUDDLCmd]);
    }
    pDLevbuffer_end += sprintf (pDLevbuffer_end, " (@%s)", channel_name_coerced);

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, pBOSTbuffer));
        viCheckErr (viPrintf (io, pDTimbuffer));
        viCheckErr (viPrintf (io, pDLevbuffer));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (pBOSTbuffer)
        Ivi_Free (vi, pBOSTbuffer);
    if (pDTimbuffer)
        Ivi_Free (vi, pDTimbuffer);
    if (pDLevbuffer)
        Ivi_Free (vi, pDLevbuffer);

    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureUserDefinedSequence
 * Purpose: Configures the user defined sequence.
 * Note: Sequence capability is only available on the N6705B DC Power Analyzer.
 *
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureUserDefinedSequence (ViSession vi,
                                                        ViChar channelName[],
                                                        ViInt32 sequenceUDefBOSTSize,
                                                        ViInt32 sequenceUDefBeginningStep[],
                                                        ViInt32 sequenceUDefDwellTimeSize,
                                                        ViReal64 sequenceUDefDwellTime[],
                                                        ViInt32 sequenceUDefDwellLevelSize,
                                                        ViReal64 sequenceUDefDwellLevel[],
                                                        ViInt32 sequenceStepNumber)
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;
    ViInt32       iSeqDTS, iSeqBOSTCmd, iSeqUDDTCmd, iSeqUDDLCmd, measureShapeValue;
    ViInt32       measureSeqTypeValue, measureSeqShapeValue, measureStepValue;
    ViChar        *pSeqBOSTbuffer     = VI_NULL, *pSeqBOSTbuffer_end = VI_NULL, *pSeqDTimbuffer = VI_NULL;
    ViChar        *pSeqDTimbuffer_end = VI_NULL, *pSeqDLevbuffer = VI_NULL, *pSeqDLevbuffer_end = VI_NULL;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_CoerceChannelName (vi, channelName, &channel_name_coerced), 2, "Channel Name");

    if (Ivi_RangeChecking (vi))
    {
        if (sequenceUDefBOSTSize < 1 || sequenceUDefBOSTSize > 512)
            viCheckParm (IVI_ERROR_INVALID_VALUE, 3, "Sequence UDef BOST Size");

        if (sequenceUDefDwellTimeSize < 1 || sequenceUDefDwellTimeSize > 512)
            viCheckParm (IVI_ERROR_INVALID_VALUE, 5, "Sequence UDef Dwell Time Size");
        for (iSeqDTS = 0; iSeqDTS < sequenceUDefDwellTimeSize; iSeqDTS++)
        {
            if (sequenceUDefDwellTime[iSeqDTS] < 0.0 || sequenceUDefDwellTime[iSeqDTS] > 262.143)
                viCheckParm (IVI_ERROR_INVALID_VALUE, 6, "Sequence UDef Dwell Time");
        }

        if (sequenceUDefDwellLevelSize < 1 || sequenceUDefDwellLevelSize > 512)
            viCheckParm (IVI_ERROR_INVALID_VALUE, 7, "Sequence UDef Level Size");
    }

    /*Waveform related attribute */
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));
    /*Sequence related attributes */
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_MEASUREMENT, 0, &measureSeqTypeValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_SHAPE, 0, &measureSeqShapeValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, &measureStepValue));

    /* Allocated buffer for User Defined Beginning Step command */
    viCheckErr (Ivi_Alloc (vi, MAX_HEADER_COMMAND_LENGTH + sequenceUDefBOSTSize * (MAX_BOOL_FORMAT_LENGTH + 1), (ViAddr *) &pSeqBOSTbuffer));
    pSeqBOSTbuffer_end = pSeqBOSTbuffer;

    /* Format Beginning Step command for user defined sequence*/
    if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_UDEFINED && measureSeqTypeValue == ZUP366_VAL_SEQ_MEAS_TYPE_VOLTAGE)
        pSeqBOSTbuffer_end += sprintf (pSeqBOSTbuffer_end, ":SOUR:ARB:SEQ:STEP:VOLT:UDEF:BOST ");
    else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_UDEFINED && measureSeqTypeValue == ZUP366_VAL_SEQ_MEAS_TYPE_CURRENT)
        pSeqBOSTbuffer_end += sprintf (pSeqBOSTbuffer_end, ":SOUR:ARB:SEQ:STEP:CURR:UDEF:BOST ");

    /*BOST array formatting*/
    for (iSeqBOSTCmd = 0; iSeqBOSTCmd < sequenceUDefBOSTSize; iSeqBOSTCmd++)
    {
        pSeqBOSTbuffer_end += sprintf (pSeqBOSTbuffer_end, (sequenceUDefBeginningStep[iSeqBOSTCmd]) ? "ON," : "OFF,");
    }
    /* append the step number and channel to BOST array */
    pSeqBOSTbuffer_end += sprintf (pSeqBOSTbuffer_end, " %d, (@%s)", measureStepValue, channel_name_coerced);

    /* Allocated buffer for User Defined Dwell Time command */
    viCheckErr (Ivi_Alloc (vi, MAX_HEADER_COMMAND_LENGTH + sequenceUDefDwellTimeSize * (MAX_DOUBLE_FORMAT_LENGTH + 1), (ViAddr *) &pSeqDTimbuffer));
    pSeqDTimbuffer_end = pSeqDTimbuffer;

    /* Format User Defined Dwell Time command for user defined sequence */
    if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_UDEFINED && measureSeqTypeValue == ZUP366_VAL_SEQ_MEAS_TYPE_VOLTAGE)
        pSeqDTimbuffer_end += sprintf (pSeqDTimbuffer_end, ":SOUR:ARB:SEQ:STEP:VOLT:UDEF:DWEL ");
    else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_UDEFINED && measureSeqTypeValue == ZUP366_VAL_SEQ_MEAS_TYPE_CURRENT)
        pSeqDTimbuffer_end += sprintf (pSeqDTimbuffer_end, ":SOUR:ARB:SEQ:STEP:CURR:UDEF:DWEL ");

    /* User Defined Dwell Time array formatting*/
    for (iSeqUDDTCmd = 0; iSeqUDDTCmd < sequenceUDefDwellTimeSize; iSeqUDDTCmd++)
    {
        pSeqDTimbuffer_end += sprintf (pSeqDTimbuffer_end, "%le,", sequenceUDefDwellTime[iSeqUDDTCmd]);
    }
    /* append the step number and channel to  User Defined Dwell Time array */
    pSeqDTimbuffer_end += sprintf (pSeqDTimbuffer_end, "%d, (@%s)", measureStepValue, channel_name_coerced);

    /* Allocated buffer for User Defined Dwell Level command for user defined waveform */
    viCheckErr (Ivi_Alloc (vi, MAX_HEADER_COMMAND_LENGTH + sequenceUDefDwellLevelSize * (MAX_DOUBLE_FORMAT_LENGTH + 1), (ViAddr *) &pSeqDLevbuffer));
    pSeqDLevbuffer_end = pSeqDLevbuffer;

    /* Format User Defined Dwell Level command for user defined sequence */
    if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_UDEFINED && measureSeqTypeValue == ZUP366_VAL_SEQ_MEAS_TYPE_VOLTAGE)
        pSeqDLevbuffer_end += sprintf (pSeqDLevbuffer_end, ":SOUR:ARB:SEQ:STEP:VOLT:UDEF:LEV ");
    else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_UDEFINED && measureSeqTypeValue == ZUP366_VAL_SEQ_MEAS_TYPE_CURRENT)
        pSeqDLevbuffer_end += sprintf (pSeqDLevbuffer_end, ":SOUR:ARB:SEQ:STEP:CURR:UDEF:LEV ");

    /* User Defined Dwell Level array formatting*/
    for (iSeqUDDLCmd = 0; iSeqUDDLCmd < sequenceUDefDwellLevelSize; iSeqUDDLCmd++)
    {
        pSeqDLevbuffer_end += sprintf (pSeqDLevbuffer_end, "%le,", sequenceUDefDwellLevel[iSeqUDDLCmd]);
    }
    /* append the step number and channel to User Defined Dwell Time array */
    pSeqDLevbuffer_end += sprintf (pSeqDLevbuffer_end, "%d, (@%s)", measureStepValue, channel_name_coerced);

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, pSeqBOSTbuffer));
        viCheckErr (viPrintf (io, pSeqDTimbuffer));
        viCheckErr (viPrintf (io, pSeqDLevbuffer));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (pSeqBOSTbuffer)
        Ivi_Free (vi, pSeqBOSTbuffer);
    if (pSeqDTimbuffer)
        Ivi_Free (vi, pSeqDTimbuffer);
    if (pSeqDLevbuffer)
        Ivi_Free (vi, pSeqDLevbuffer);

    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureDatalog
 * Purpose: Configures the data log type, duration, period, offset and sets
 *          the state of the data log function.
 * Note: This function only applies to the N6705B DC Power Analyzer.
 *
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureDatalog (ViSession vi,
                                            ViChar channelName[],
                                            ViInt32 datalogMeasurementType,
                                            ViBoolean datalogEnabled,
                                            ViReal64 datalogPeriod,
                                            ViReal64 datalogDuration,
                                            ViInt32 datalogOffset)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                          ZUP366_ATTR_DATALOG_MEASUREMENT, 0, datalogMeasurementType),
                 3, "Datalog Measurement Type");

    viCheckParm (Ivi_SetAttributeViBoolean (vi, channelName,
                                            ZUP366_ATTR_DATALOG_ENABLED, 0, datalogEnabled),
                 4, "Datalog Enabled");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, VI_NULL,
                                           ZUP366_ATTR_DATALOG_PERIOD, 0, datalogPeriod),
                 5, "Datalog Period");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, VI_NULL,
                                           ZUP366_ATTR_DATALOG_DURATION, 0, datalogDuration),
                 6, "Datalog Duration");
    viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL,
                                          ZUP366_ATTR_DATALOG_OFFSET, 0, datalogOffset),
                 7, "Datalog Offset");
    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureDatalogMeasurement
 * Purpose: Configures the measurement range of the data logger.
 * Note: This function only applies to the N6705B DC Power Analyzer.
 *
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureDatalogMeasurement (ViSession vi,
                                                       ViChar channelName[],
                                                       ViBoolean datalogAutoRange,
                                                       ViReal64 datalogManualRange)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (Ivi_LockSession (vi, VI_NULL));
    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    if ((model_type == N6781A) || (model_type == N6782A))
        viCheckParm (Ivi_SetAttributeViBoolean (vi, channelName, ZUP366_ATTR_DATALOG_AUTO_RANGE, 0, datalogAutoRange),
                     3, "Datalog Auto Range");

    if (!datalogAutoRange)
        viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName, ZUP366_ATTR_DATALOG_RANGE, 0, datalogManualRange),
                     4, "Datalog Manual Range");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureDatalogMarkers
 * Purpose: Sets the position of the data logger markers.
 * Note: This function only applies to the N6705B DC Power Analyzer.
 *
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureDatalogMarkers (ViSession vi,
                                                   ViReal64 datalogMarker1,
                                                   ViReal64 datalogMarker2)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_SetAttributeViReal64 (vi, VI_NULL,
                                           ZUP366_ATTR_DATALOG_MARKER1, 0, datalogMarker1),
                 2, "Datalog Marker 1");
    viCheckParm (Ivi_SetAttributeViReal64 (vi, VI_NULL,
                                           ZUP366_ATTR_DATALOG_MARKER2, 0, datalogMarker2),
                 3, "Datalog Marker 2");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureHistogram
 * Purpose:  Configures the histogram function of the power analyzer.
 *
 * Notes:  Only applies to models N6781A and N6782A that have the Data Logger installed.
 *         The histogram function only applies to the N6705B DC Power Analyzer.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureHistogram (ViSession vi,
                                              ViChar channelName[],
                                              ViBoolean enableHistorgram,
                                              ViBoolean histogramAutoRange,
                                              ViReal64 histogramRange)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_SetAttributeViBoolean (vi,
                                            channelName,
                                            ZUP366_ATTR_HISTOGRAM_ENABLED,
                                            0, enableHistorgram), 3, "Enable Histogram");
    viCheckParm (Ivi_SetAttributeViBoolean (vi,
                                            channelName,
                                            ZUP366_ATTR_HISTOGRAM_AUTO_RANGE,
                                            0, histogramAutoRange), 4, "Histogram Autorange");
    if (!histogramAutoRange)
        viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                               ZUP366_ATTR_HISTOGRAM_RANGE, 0, histogramRange),
                     5, "Historgram Range");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureTriggerWaveformSource
 * Purpose:  Configures the trigger source of the waveform function.
 * Note: Waveform capability is only available on the N6705B DC Power Analyzer.
 *
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureTriggerWaveformSource (ViSession vi,
                                                          ViInt32 waveformTriggerSource)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL,
                                          ZUP366_ATTR_TRIGGER_WFM_SRC, 0, waveformTriggerSource),
                 2, "Waveform Trigger Source");
    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureDatalogTriggerSource
 * Purpose:  Configures the trigger source of the data logger. When the power
 *          supply receives a trigger signal on the source you specify, it
 *          runs the data logger.
 * Note: This function only applies to the N6705B DC Power Analyzer.
 *
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureDatalogTriggerSource (ViSession vi,
                                                         ViInt32 datalogTriggerSource)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL,
                                          ZUP366_ATTR_DATALOG_TRIGGER_SRC, 0, datalogTriggerSource),
                 2, "Datalog Trigger Source");
    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureDatalogCurrVoltTrigger
 * Purpose: Configures the trigger for the data logger.
 * Note: This function only applies to the N6705B DC Power Analyzer.
 *
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureDatalogCurrVoltTrigger (ViSession vi,
                                                           ViChar channelName[],
                                                           ViReal64 datalogLevel,
                                                           ViInt32 datalogSlope)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                           ZUP366_ATTR_DATALOG_TRIGGER_LEVEL, 0, datalogLevel),
                 3, "Datalog Level");
    viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                          ZUP366_ATTR_DATALOG_TRIGGER_SLOPE, 0, datalogSlope),
                 4, "Datalog Slope");
    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureHistogramTriggerSource
 * Purpose: Configures the trigger source of the histogram function. When
 *			the power supply receives a trigger signal on the source you
 *			specify, it runs the histogram.
 *
 *	Notes: This function only applies to model N6705B and modules N6781A
 *		   and N6782A that have the Data Logger function installed.
 *
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureHistogramTriggerSource (ViSession vi,
                                                           ViChar channelName[],
                                                           ViInt32 histogramTriggerSource)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    if ((model_type == N6781A) || (model_type == N6782A))
        viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                              ZUP366_ATTR_HISTOGRAM_TRIGGER_SRC, 0, histogramTriggerSource),
                     3, "Histogram Source");
    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureSMUOutputImpedance
 * Purpose:  Specifies the output impedance mode at turn-off.
 * Notes: This function only applies to N678xA SMU models.
 *
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureSMUOutputImpedance (ViSession vi,
                                                       ViChar channelName[],
                                                       ViInt32 outputImpedance)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    if (model_type == N6781A || model_type == N6782A || model_type == N6784A)
        viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                              ZUP366_ATTR_SMU_OUTPUT_IMPEDANCE, 0, outputImpedance),
                     3, "Output Impedance");
    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureSMUBWOscillation
 * Purpose:  Sets the voltage bandwidth and the oscillation state.
 * Note: This funtion only applies to the N678xA SMU models.
 *
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureSMUBWOscillation (ViSession vi,
                                                     ViChar channelName[],
                                                     ViInt32 voltageBandwidth,
                                                     ViBoolean enableOscillationProtection)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    if (model_type == N6781A || model_type == N6782A || model_type == N6784A)
    {
        viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                              ZUP366_ATTR_SMU_VOLTAGE_BANDWIDTH, 0, voltageBandwidth),
                     3, "Voltage Bandwidth");
        viCheckParm (Ivi_SetAttributeViBoolean (vi, channelName,
                                                ZUP366_ATTR_SMU_ENABLE_OSCILLATION,
                                                0, enableOscillationProtection), 4, "Enable Oscillation Protection");
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureSMUResistance
 * Purpose: Sets the state of the output resistance and resistance level on
 *			the specified channels.
 * Note: This function only applies to the N6781A model.
 *
 *****************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureSMUResistance (ViSession vi,
                                                  ViChar channelName[],
                                                  ViBoolean enableResistanceOutput,
                                                  ViReal64 resistanceLevel)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    if ((model_type == N6781A))
    {
        viCheckParm (Ivi_SetAttributeViBoolean (vi, channelName,
                                                ZUP366_ATTR_SMU_ENABLE_RESISTANCE,
                                                0, enableResistanceOutput), 3, "Enable Resistance Output");
        viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                               ZUP366_ATTR_SMU_RESISTANCE_LEVEL, 0, resistanceLevel),
                     4, "Resistance Level");
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureSMUOutputRegulation
 * Purpose: Selects whether the output regulation is in voltage or current
 *			priority mode.
 * Note: This function only applies to models N678xA SMU.
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureSMUOutputRegulation (ViSession vi,
                                                        ViChar channelName[],
                                                        ViInt32 outputRegulation)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    if ((model_type == N6781A) || (model_type == N6782A) || (model_type == N6784A))
        viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                              ZUP366_ATTR_SMU_OUTPUT_REGULATION_PRIORITY, 0, outputRegulation),
                     3, "Output Regulation");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************************
 * Function: zup366_ConfigureSMUVoltageLimit
 * Purpose: Configures the Positive Voltage Limit, Negative Voltage Limit, and
 *			Voltage Limit Tracking.
 * Notes:
 * 1) ZUP366_ATTR_SMU_OUTPUT_REGULATION_PRIORITY needs to be set to
 *	ZUP366_VAL_OUTPUT_REGULATION_CURRENT in the ConfigureSMUOutputRegulation function.
 * 2) This function applies to N678xA SMU and N6784 models.
 *
 *******************************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureSMUVoltageLimit (ViSession vi,
                                                    ViChar channelName[],
                                                    ViReal64 positiveVoltageLimit,
                                                    ViReal64 negativeVoltageLimit,
                                                    ViBoolean enableVoltageLimitTracking)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    if (model_type == N6781A || model_type == N6782A || model_type == N6784A)
        viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                               ZUP366_ATTR_SMU_POSITIVE_VOLTAGE_LIMIT, 0, positiveVoltageLimit),
                     3, "Positive Voltage Limit");

    if ((model_type == N6784A))
    {
        viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                               ZUP366_ATTR_SMU_NEGATIVE_VOLTAGE_LIMIT, 0, negativeVoltageLimit),
                     4, "NegativeVoltageLimit");
        viCheckParm (Ivi_SetAttributeViBoolean (vi, channelName,
                                                ZUP366_ATTR_SMU_VOLTAGE_LIMIT_TRACKING,
                                                0, enableVoltageLimitTracking), 5, "Enable Voltage Limit Tracking");
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureSMUCurrentLimit
 * Purpose: Configures the Positive Current Limit, Negative Current Limit,
 *			and Current Limit Tracking settings.
 *
 *	Notes:
 *	1) ZUP366_ATTR_SMU_OUTPUT_REGULATION_PRIORITY needs to be set
 *	to ZUP366_VAL_OUTPUT_REGULATION_VOLTAGE in ConfigureSMUOutputRegulation.
 *	2) This function applies to N678xA SMU and N6783 models.
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureSMUCurrentLimit (ViSession vi,
                                                    ViChar channelName[],
                                                    ViBoolean enableCurrentLimitTracking,
                                                    ViReal64 positiveCurrentLimit,
                                                    ViReal64 negativeCurrentLimit)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    if ((model_type == N6781A) || (model_type == N6782A) || (model_type == N6784A))
        viCheckParm (Ivi_SetAttributeViBoolean (vi, channelName,
                                                ZUP366_ATTR_SMU_CURRENT_LIMIT_TRACKING,
                                                0, enableCurrentLimitTracking), 3, "Enable Current Limit Tracking");

    if ((model_type == N6781A) || (model_type == N6782A) || (model_type == N6784A) || (model_type == N6783A))
    {
        viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                               ZUP366_ATTR_SMU_ASM_POSTIVE_CURRENT_LIMIT, 0, positiveCurrentLimit),
                     4, "Positive Curren Limit");

        viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                               ZUP366_ATTR_SMU_ASM_NEGATIVE_CURRENT_LIMIT, 0, negativeCurrentLimit),
                     5, "NegativeCurrentLimit");
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureSMUOVRPosProtectection
 * Purpose: Sets the positive over-voltage remote protection (OVP) level of
 *			the output channel.
 * Note: This function only applies to the N678xA SMU models.
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureSMUOVRPosProtectection (ViSession vi,
                                                           ViChar channelName[],
                                                           ViInt32 positiveOVRP)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    if ((model_type == N6781A) || (model_type == N6782A) || (model_type == N6784A))
        viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                              ZUP366_ATTR_SMU_POSITIVE_OVRP, 0, positiveOVRP),
                     3, "Positive Over Voltage Remote Protection");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ConfigureSMUOVRNegProtectection
 * Purpose: Sets the negative over-voltage remote protection (OVP) level of
 *          the output channel.
 * Note: This function only applies to models N6784A.
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureSMUOVRNegProtectection (ViSession vi,
                                                           ViChar channelName[],
                                                           ViInt32 negativeOVRP)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    if ((model_type == N6784A))
        viCheckParm (Ivi_SetAttributeViInt32 (vi, channelName,
                                              ZUP366_ATTR_SMU_NEGATIVE_OVRP, 0, negativeOVRP),
                     3, "Negative Over Voltage Remote Protection");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function: zup366_ConfigureSMUOVRemoteDelay
 * Purpose: Sets the over-voltage protection delay.
 * Note: This function only applies to N6783A.
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureSMUOVRemoteDelay (ViSession vi,
                                                     ViChar channelName[],
                                                     ViReal64 overProtectionDelay)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    if (model_type == N6783A)
        viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                               ZUP366_ATTR_ASM_OVER_PROTECTION_DELAY, 0, overProtectionDelay),
                     3, "Over Protection Delay");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/***********************************************************************************************************
 * Function: zup366_ConfigureSMUCurrentSlewRate
 * Purpose:  Sets the current slew rate.
 * Note: This functin only applies to the N678xA SMU models.
 *
 *************************************************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureSMUCurrentSlewRate (ViSession vi,
                                                       ViChar channelName[],
                                                       ViReal64 currentSlewRate)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    if ((model_type == N6781A) || (model_type == N6782A) || (model_type == N6784A))
    {
        /* Since the [SOURce:]CURRent:SLEW:MAX command is coupled to the [SOURce:]CURRent:SLEW command.
         * the ZUP366_ATTR_SMU_CURRENT_SLEW_RATE attribute sets both commands */
        viCheckParm (Ivi_SetAttributeViReal64 (vi, channelName,
                                               ZUP366_ATTR_SMU_CURRENT_SLEW_RATE, 0, currentSlewRate),
                     3, "Current Slew Rate");
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/***************************************************************************************************************
 * Function: zup366_ConfigureSimulatanMeasurement
 * Purpose: Configures simultaneous voltage and current measurement.
 *
 * Notes: This command replaces the previous SENSe:FUNCtion command and should be used
 *		  in new applications. SENSe:FUNCtion is still available for backward compatibility.
 *       Simultaneous voltage and current measurement capabilities are only available on N676xA, and N678x SMU.
 *****************************************************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureSimulatanMeasurement (ViSession vi,
                                                         ViChar channelName[],
                                                         ViBoolean enableSimCurrentMeasurement,
                                                         ViBoolean enableSimVoltageMeasurement)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    if (model_type == N6781A || model_type == N6782A || model_type == N6784A || model_type != N6761A || model_type != N6762A)

    {
        viCheckParm (Ivi_SetAttributeViBoolean (vi, channelName,
                                                ZUP366_ATTR_ENABLE_SIMULTANEOUS_VOLT_MEASUREMENT, 0, enableSimCurrentMeasurement),
                     3, "Enable Current Measurement");

        viCheckParm (Ivi_SetAttributeViBoolean (vi, channelName,
                                                ZUP366_ATTR_ENABLE_SIMULTANEOUS_CURR_MEASUREMENT, 0, enableSimVoltageMeasurement),
                     4, "Enable Voltage Measurement");
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_QueryWaveformQuality
 * Purpose:  Queries the quality of the waveforms in the waveform sequence.
 * Note:  Sequence capability is only available on the N6705B DC Power Analyzer
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_QueryWaveformQuality (ViSession vi,
                                                ViChar channelName[],
                                                ViInt32 *waveformQuality)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:QUAL? (@%s)", "%ld", channelName, waveformQuality));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_QueryHistogram
 * Purpose:  Queries least significant bit (LSB) weight of a histogram,
 *			 historgram offset, and current bin ranges.
 * Notes:
 *	1) This function only applies to models N6781A and N6782A that
 *	have the Data Logger function installed.
 *	2) The histogram function only applies to the N6705B DC Power Analyzer.
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_QueryHistogram (ViSession vi,
                                          ViChar channelName[],
                                          ViReal64 binRange,
                                          ViReal64 *currentBinGain,
                                          ViReal64 *currentBinOffset,
                                          ViReal64 *currentBinRange)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));


        viCheckErr (viQueryf (io, ":SENS:HIST:CURR:BIN:GAIN? %Le,(@%s)", "%Le", binRange, channelName, &currentBinGain));
        viCheckErr (viQueryf (io, ":SENS:HIST:CURR:BIN:OFFS? %Le,(@%s)", "%Le", binRange, channelName, &currentBinOffset));
        viCheckErr (viQueryf (io, ":SENS:HIST:CURR:BIN:RANG? (@%s)", "%Le", channelName, &currentBinRange));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function: zup366_ImmediateDatalogTrigger
 * Purpose: Sends an immediate trigger signal to the data logger. This will
 *			trigger the data log regardless of the selected trigger source.
 *
 * Notes:
 *	1) The data logger must be initiated before triggered.
 *	2) This function only applies to the N6705B DC Power Analyzer.
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_ImmediateDatalogTrigger (ViSession vi)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));


    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, ":TRIG:DLOG:IMM"));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ImmediateHistogramTrigger
 * Purpose: Sends an immediate trigger signal to the histogram current
 *			measurement data logger.
 * Notes:
 *	1) The histogram function only applies to the N6705B Analyzer.
 *	2) This command only applies to models N6781A and N6782A that
 *	   have the Data Logger function installed.
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_ImmediateHistogramTrigger (ViSession vi, ViChar channelName[])
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, ":TRIG:HIST:IMM (@%s)", channelName));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_AbortDatalog
 * Purpose:  Stops the currently running internal data log and returns the
 *           trigger system back to the Idle state.
 * Note:This function only applies to the N6705B DC Power Analyzer.
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_AbortDatalog (ViSession vi)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));


    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, ":ABORt:DLOG"));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_AbortHistogram
 * Purpose:  Abort histogram current measurement.
 *
 * Notes:
 *	1) This function only applies to models N678xA SMU that have the
 *	Data Logger function installed.
 *	2) The histogram function only applies to the N6705B DC Power Analyzer.
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_AbortHistogram (ViSession vi, ViChar channelName[])

{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, ":ABOR:HIST (@%s)", channelName));
    }

    /*
     *  Do not invoke the zup366_CheckStatus function here.  It
     *  is invoked by the high-level driver functions when this function is
     *  used internally.  After the user calls this function, the user can check
     *  for errors by calling the zup366_error_query function.
     */


Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*********************************************************************************
 * Function: zup366_InitiateDatalog
 * Purpose:  This command initiates the data logger. When the data logger is
 *			initiated, an event on a selected data log trigger source starts a datalog.
 * Note: This function only applies to the N6705B DC Power Analyzer.
 ******************************************************************************/
ViStatus _VI_FUNC zup366_InitiateDatalog (ViSession vi,
                                           ViString datalogFilePath)
{
    ViStatus error            = VI_SUCCESS;
    ViChar   *pDlogBuffer     = VI_NULL;
    ViChar   *pDlogBuffer_end = VI_NULL;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    /* Allocated buffer */
    viCheckErr (Ivi_Alloc (vi, MAX_HEADER_COMMAND_LENGTH * 2 + MAX_DOUBLE_FORMAT_LENGTH, (ViAddr *) &pDlogBuffer));
    pDlogBuffer_end = pDlogBuffer;

    /* Format command */
    pDlogBuffer_end += sprintf (pDlogBuffer_end, ":INIT:DLOG ");
    pDlogBuffer_end += sprintf (pDlogBuffer_end, "\"%s\"", datalogFilePath);

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, pDlogBuffer));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (pDlogBuffer)
        Ivi_Free (vi, pDlogBuffer);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/***************************************************************************************************
 * Function: zup366_InitiateHistogram
 * Purpose:  Initiates a histogram current measurement.
 * Note: This command only applies to models N678xA SMU that have the Data Logger function installed.
 *
 *****************************************************************************************************/
ViStatus _VI_FUNC zup366_InitiateHistogram (ViSession vi, ViChar channelName[])

{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, ":INIT:IMM:HIST (@%s)", channelName));
    }

    /*
     *  Do not invoke the zup366_CheckStatus function here.  It
     *  is invoked by the high-level driver functions when this function is
     *  used internally.  After the user calls this function, the user can check
     *  for errors by calling the zup366_error_query function.
     */

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_FetchDatalog
 * Purpose: Returns the peak to peak value of the data logger between marker
 *			1 and marker 2.
 *
 * Note: This function only applies to the N6705B DC Power Analyzer.
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_FetchDatalog (ViSession vi, ViChar channelName[],
                                        ViReal64 *peakToPeak)
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;
    ViInt32       dlogMeasureTypeValue, model_type;
    ViString      dlogMeasureType;
    ViBoolean     doubleModuleSup;

    checkErr (Ivi_LockSession (vi, VI_NULL));
    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    viCheckParm (Ivi_CoerceChannelName (vi, channelName, &channel_name_coerced), 2, "Channel Name");

    isDoubleWideModule (model_type, &doubleModuleSup);

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_DATALOG_MEASUREMENT, 0, &dlogMeasureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (dlogMeasureTypeValue, &attrDatalogMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &dlogMeasureType, VI_NULL));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        if (!doubleModuleSup)
            viCheckErr (viQueryf (io, ":FETC:DLOG:%s:PTP? (@%s)", "%Le", dlogMeasureType, channel_name_coerced, peakToPeak));
        else
            viCheckErr (viQueryf (io, ":FETC:DLOG:%s:PTP? (@%s)", "%Le", dlogMeasureType, channelName, peakToPeak));
    }

    /*
     *  Do not invoke the zup366_CheckStatus function here.  It
     *  is invoked by the high-level driver functions when this function is
     *  used internally.  After the user calls this function, the user can check
     *  for errors by calling the zup366_error_query function.
     */


Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_FetchDatalogMeasurement
 * Purpose:  Returns the DC output current or voltage average on the selected
 *			 data log trace.
 *
 * Note: This function only applies to the N6705B DC Power Analyzer.
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_FetchDatalogMeasurement (ViSession vi, ViChar channelName[],
                                                   ViReal64 *datalogMeasurement)
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;
    ViInt32       dlogMeasureTypeValue, model_type;
    ViString      dlogMeasureType;
    ViBoolean     doubleModuleSup;

    checkErr (Ivi_LockSession (vi, VI_NULL));
    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    viCheckParm (Ivi_CoerceChannelName (vi, channelName, &channel_name_coerced), 2, "Channel Name");

    isDoubleWideModule (model_type, &doubleModuleSup);

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_DATALOG_MEASUREMENT, 0, &dlogMeasureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (dlogMeasureTypeValue, &attrDatalogMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &dlogMeasureType, VI_NULL));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        if (!doubleModuleSup)
            viCheckErr (viQueryf (io, ":FETC:DLOG:%s? (@%s)", "%Le", dlogMeasureType, channel_name_coerced, datalogMeasurement));
        else
            viCheckErr (viQueryf (io, ":FETC:DLOG:%s? (@%s)", "%Le", dlogMeasureType, channelName, datalogMeasurement));
    }

    /*
     *  Do not invoke the zup366_CheckStatus function here.  It
     *  is invoked by the high-level driver functions when this function is
     *  used internally.  After the user calls this function, the user can check
     *  for errors by calling the zup366_error_query function.
     */


Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function: zup366_FetchHistogram
 * Purpose:  This query fetches the histogram current bin data.
 * Note: This command only applies to models N678xA SMU that have the
 *		 Data Logger function installed.
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_FetchHistogram (ViSession vi,
                                          ViChar channelName[],
                                          ViReal64 binRange,
                                          ViReal64 *binCurrentData)
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    viCheckParm (Ivi_CoerceChannelName (vi, channelName, &channel_name_coerced), 2, "Channel Name");
    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viQueryf (io, ":FETC:HIST:CURR? %Le, (@%s)", "%Le", binRange, channel_name_coerced, binCurrentData));
    }

    /*
     *  Do not invoke the zup366_CheckStatus function here.  It
     *  is invoked by the high-level driver functions when this function is
     *  used internally.  After the user calls this function, the user can check
     *  for errors by calling the zup366_error_query function.
     */

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/********************************************************************************************************
 * Function: zup366_ReadHistogram
 * Purpose: Reads and returns the bin data in the histogram function.
 * Notes:
 *	1) This function only applies to models N678xA SMU that have the Data Logger function installed.
 *	2) The histogram function only applies to the N6705B DC Power Analyzer.
 *
 ***********************************************************************************************************/
ViStatus _VI_FUNC zup366_ReadHistogram (ViSession vi,
                                         ViChar channelName[],
                                         ViReal64 binRange,
                                         ViReal64 *binCurrentData)

{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (binCurrentData == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Measurement");

    checkErr (zup366_InitiateHistogram (vi, channelName));
    checkErr (zup366_ImmediateHistogramTrigger (vi, channelName));
    checkErr (zup366_FetchHistogram (vi, channelName, binRange, binCurrentData));

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_ResetSequence
 * Purpose: Resets the sequence to its power-on default setting.
 *
 * Notes:
 *	1) This function only applies to models N678xA SMU that have the
 *	   Data Logger function installed.
 *	2) Sequence capability is only available on the N6705B DC Power Analyzer
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_ResetSequence (ViSession vi,
                                         ViChar channelName[])
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:RES (@%s)", channelName));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/**********************************************************************************************************
 * Function: zup366_ConvertWaveformToList
 * Purpose: This function converts the specified  waveform to a list of
 *			points in the user-defined current or voltage waveform format.
 * Notes:
 *	1)Waveform capability is only available on the N6705B DC Power Analyzer.
 *	2) To use this command you must specify a Shape and measurement (Configure Waveform Parameters).
 *	3) If the Waveform Shape of the specified channel is already set to a User-Defined waveform,
 *	   the conversion is not performed.
 *	4) If the Waveform Shape of the specified channel is set to "NONE", the conversion is not performed.
 *
 **************************************************************************************************************/
ViStatus _VI_FUNC zup366_ConvertWaveformToList (ViSession vi,
                                                 ViChar channelName[])
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureShapeValue, measureTypeValue;
    ViString measureType;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

        viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));

        viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_MEASUREMENT, 0, &measureTypeValue));
        viCheckErr (Ivi_GetViInt32EntryFromValue (measureTypeValue, &attrWaveformMeasurementRangeTable, VI_NULL, VI_NULL,
                                                  VI_NULL, VI_NULL, &measureType, VI_NULL));

        if (measureShapeValue != ZUP366_VAL_WAVEFORM_CONSTANTDWELL || measureShapeValue != ZUP366_VAL_WAVEFORM_SEQUENCE ||
            measureShapeValue != ZUP366_VAL_WAVEFORM_UDEFINED)
        {
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:CONV (@%s)", measureType, channelName));
        }
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function: zup366_SaveRecallSequence
 * Purpose:  Recalls or saves the sequence settings on the N6705B model.
 * Note: Sequence capability is only available on the N6705B DC Power Analyzer.
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_SaveRecallSequence (ViSession vi,
                                              ViChar channelName[],
                                              ViInt32 sequenceAction,
                                              ViString sequenceFilePath)
{
    ViStatus      error = VI_SUCCESS;
    ViConstString channel_name_coerced;
    ViChar        *pSequenceBuffer     = VI_NULL;
    ViChar        *pSequenceBuffer_end = VI_NULL;


    checkErr (Ivi_LockSession (vi, VI_NULL));
    viCheckParm (Ivi_CoerceChannelName (vi, channelName, &channel_name_coerced), 2, "Channel Name");

    /* Allocated buffer */
    viCheckErr (Ivi_Alloc (vi, MAX_HEADER_COMMAND_LENGTH * 2 + MAX_DOUBLE_FORMAT_LENGTH, (ViAddr *) &pSequenceBuffer));
    pSequenceBuffer_end = pSequenceBuffer;

    /* Format command */
    if (sequenceAction == ZUP366_VAL_SEQUENCE_ACTION_SAVE)
        pSequenceBuffer_end += sprintf (pSequenceBuffer_end, ":MMEM:STOR:ARB:SEQ ");
    else
        pSequenceBuffer_end += sprintf (pSequenceBuffer_end, ":MMEM:LOAD:ARB:SEQ ");

    pSequenceBuffer_end += sprintf (pSequenceBuffer_end, "\"%s\",(@%s)", sequenceFilePath, channel_name_coerced);

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, pSequenceBuffer));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (pSequenceBuffer)
        Ivi_Free (vi, pSequenceBuffer);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: zup366_zup366_FileManagement
 * Purpose:  Exports a datalog file or deletes a specified file.
 * Note: This function only applies to the N6705B DC Power Analyzer.
 *
 ******************************************************************************/
ViStatus _VI_FUNC zup366_FileManagement (ViSession vi,
                                          ViInt32 operation, ViString filePath)
{
    ViStatus error               = VI_SUCCESS;
    ViChar   *pFileMgmBuffer     = VI_NULL;
    ViChar   *pFileMgmBuffer_end = VI_NULL;


    checkErr (Ivi_LockSession (vi, VI_NULL));

    /* Allocated buffer */
    viCheckErr (Ivi_Alloc (vi, MAX_HEADER_COMMAND_LENGTH * 2 + MAX_DOUBLE_FORMAT_LENGTH, (ViAddr *) &pFileMgmBuffer));
    pFileMgmBuffer_end = pFileMgmBuffer;

    /* Format command */
    if (operation == ZUP366_VAL_FILE_OPERATION_EXPORT)
        pFileMgmBuffer_end += sprintf (pFileMgmBuffer_end, ":MMEM:EXP:DLOG ");
    else
        pFileMgmBuffer_end += sprintf (pFileMgmBuffer_end, ":MMEM:DEL ");

    pFileMgmBuffer_end += sprintf (pFileMgmBuffer_end, "\"%s\"", filePath);

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, pFileMgmBuffer));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    if (pFileMgmBuffer)
        Ivi_Free (vi, pFileMgmBuffer);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*******************************************************************************
 * Function:  zup366_AbortExternalDatalog
 * Purpose:   Stops the external data log and returns the trigger system back
 *            to the Idle state.
 *******************************************************************************/
ViStatus _VI_FUNC zup366_AbortExternalDatalog (ViSession vi, 
												ViChar ChannelName[])
{
    ViStatus error                     = VI_SUCCESS;
    ViChar   channel_list[BUFFER_SIZE] = "";

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_MakeChannelList (vi,
                                       ChannelName,
                                       channel_list));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, "ABOR:ELOG (@%s)", channel_list));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*******************************************************************************
 * Function:  zup366_FetchExternalDatalog
 * Purpose:   Returns external datalog data from the instrument's buffer.
 *******************************************************************************/
ViStatus _VI_FUNC zup366_FetchExternalDatalog (ViSession vi,
                                                ViChar ChannelName[],
                                                ViInt32 MaxRecords,
                                                ViChar ExternalLog[])
{
    ViStatus error                     = VI_SUCCESS;
    ViChar   channel_list[BUFFER_SIZE] = "";

    checkErr (Ivi_LockSession (vi, VI_NULL));
	
	if (ExternalLog == VI_NULL)
        viCheckParm (IVI_ERROR_INVALID_PARAMETER, 4, "Null address for ExternalLog");
	
    checkErr (zup366_MakeChannelList (vi,
                                       ChannelName,
                                       channel_list));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viQueryf (io, ":FETC:ELOG? %d,(@%s)", "%s", MaxRecords, channel_list, ExternalLog));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*******************************************************************************
 * Function:  zup366_InitiateExternalDatalog
 * Purpose:   Initiates an external data log. When the external data log is
 *            initiated, an event on a selected external data log trigger source
 *            starts the data log. If the external data log is not initiated, all
 *            triggers are ignored.
 *
 *            Notes:
 *            1) The external data log function is not accessible from the
 *            instrument's front panel.
 *            2) All external data log functions are controlled using SCPI commands.
 *******************************************************************************/
ViStatus _VI_FUNC zup366_InitiateExternalDatalog (ViSession vi,
                                                   ViChar ChannelName[])
{
    ViStatus error                     = VI_SUCCESS;
    ViChar   channel_list[BUFFER_SIZE] = "";

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_MakeChannelList (vi,
                                       ChannelName,
                                       channel_list));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, "INIT:ELOG (@%s)", channel_list));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*******************************************************************************
 * Function:  zup366_ImmediateExternalDatalogTrigger
 * Purpose:   Sends an immediate trigger signal to the external data logger.
 *            This will trigger the external data log regardless of the selected
 *            trigger source.
 *
 *            Note:
 *            The external data logger must be initiated before triggered.
 *******************************************************************************/
ViStatus _VI_FUNC zup366_ImmediateExternalDatalogTrigger (ViSession vi,
                                                           ViChar ChannelName[])
{
    ViStatus error                     = VI_SUCCESS;
    ViChar   channel_list[BUFFER_SIZE] = "";

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_MakeChannelList (vi,
                                       ChannelName,
                                       channel_list));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, "TRIG:ELOG (@%s)", channel_list));
    }

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*******************************************************************************
 * Function:  zup366_ConfigureExternalDatalogTriggerSource
 * Purpose:   Configures the trigger source for the external data logger.
 *******************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureExternalDatalogTriggerSource (ViSession vi,
                                                                 ViChar ChannelName[],
                                                                 ViInt32 ExternalLoggerTriggerSource)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));


    viCheckParm (Ivi_SetAttributeViInt32 (vi, ChannelName, ZUP366_ATTR_ELOG_TRIGGER_SOURCE,
                                          0, ExternalLoggerTriggerSource), 3, "External Logger Trigger Source");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*******************************************************************************
 * Function:  zup366_ConfigureMeasurementResolution
 * Purpose:   Configures the resolution used in setting the time interval between
 *            digitized measurements for time intervals above 20.48 microseconds.
 *
 *            Note:
 *            This VI is used only with models N676xA, N678xA SMU, or models that
 *            have Option 054 (High Speed Test Extensions).
 *******************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureMeasurementResolution (ViSession vi,
                                                          ViInt32 MeasurementResolution)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_LockSession (vi, VI_NULL));


    viCheckParm (Ivi_SetAttributeViInt32 (vi, VI_NULL, ZUP366_ATTR_MEASUREMENT_RESOLUTION,
                                          0, MeasurementResolution), 2, "Measurement Resolution");

    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*******************************************************************************
 * Function:  zup366_ConfigureExternalDatalog
 * Purpose:   Configures the external data log.
 *******************************************************************************/
ViStatus _VI_FUNC zup366_ConfigureExternalDatalog (ViSession vi,
                                                    ViChar ChannelName[],
                                                    ViInt32 ExternalDataLogType,
                                                    ViBoolean AutoRange,
                                                    ViReal64 MeasurementRange,
                                                    ViBoolean EnableExternalDataLog,
                                                    ViBoolean EnableMinMaxLog, 
													ViReal64 ExternalDataLogPeriod)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (Ivi_LockSession (vi, VI_NULL));

    checkErr (zup366_GetModuleType (vi, ChannelName, &model_type));

    if (ExternalDataLogType == ZUP366_VAL_ELOG_TYPE_CURRENT)
    {
        if (model_type == N6781A || model_type == N6782A || model_type == N6785A || model_type == N6786A)
        {
            viCheckParm (Ivi_SetAttributeViBoolean (vi, ChannelName, ZUP366_ATTR_AUTO_ELOG_CURRENT_RANGE,
                                                    0, AutoRange), 4, "Auto Range");
            if (AutoRange == VI_FALSE)
            {
                viCheckParm (Ivi_SetAttributeViReal64 (vi, ChannelName, ZUP366_ATTR_ELOG_CURRENT_RANGE,
                                                       0, MeasurementRange), 5, "Measurement  Range");
            }
        }
        else
        {
            viCheckParm (Ivi_SetAttributeViReal64 (vi, ChannelName, ZUP366_ATTR_ELOG_CURRENT_RANGE,
                                                   0, MeasurementRange), 5, "Measurement  Range");
        }
        viCheckParm (Ivi_SetAttributeViBoolean (vi, ChannelName, ZUP366_ATTR_ENABLE_CURRENT_ELOG,
                                                0, EnableExternalDataLog), 6, "Enable External Data Log");
        viCheckParm (Ivi_SetAttributeViBoolean (vi, ChannelName, ZUP366_ATTR_ENABLE_CURRENT_MINMAX_ELOG,
                                                0, EnableMinMaxLog), 7, "Enable Min Max Log");
    }
    else
    {
        if (model_type == N6781A || model_type == N6782A || model_type == N6785A || model_type == N6786A)
        {
            viCheckParm (Ivi_SetAttributeViBoolean (vi, ChannelName, ZUP366_ATTR_AUTO_ELOG_VOLTAGE_RANGE,
                                                    0, AutoRange), 4, "Auto Range");
            if (AutoRange == VI_FALSE)
            {
                viCheckParm (Ivi_SetAttributeViReal64 (vi, ChannelName, ZUP366_ATTR_ELOG_VOLTAGE_RANGE,
                                                       0, MeasurementRange), 5, "Measurement  Range");
            }
        }
        else
        {
            viCheckParm (Ivi_SetAttributeViReal64 (vi, ChannelName, ZUP366_ATTR_ELOG_VOLTAGE_RANGE,
                                                   0, MeasurementRange), 5, "Measurement  Range");
        }
        viCheckParm (Ivi_SetAttributeViBoolean (vi, ChannelName, ZUP366_ATTR_ENABLE_VOLTAGE_ELOG,
                                                0, EnableExternalDataLog), 6, "Enable External Data Log");
        viCheckParm (Ivi_SetAttributeViBoolean (vi, ChannelName, ZUP366_ATTR_ENABLE_VOLTAGE_MINMAX_ELOG,
                                                0, EnableMinMaxLog), 7, "Enable Min Max Log");
    }

    viCheckParm (Ivi_SetAttributeViReal64 (vi, ChannelName, ZUP366_ATTR_ELOG_PERIOD,
                                           0, ExternalDataLogPeriod), 8, "External Data Log Period");


    checkErr (zup366_CheckStatus (vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function: zup366_GetAttribute<type> Functions
 * Purpose:  These functions enable the instrument driver user to get
 *           attribute values directly.  There are typesafe versions for
 *           ViInt32, ViReal64, ViString, ViBoolean, and ViSession attributes.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_GetAttributeViInt32 (ViSession vi,
                                               ViConstString channelName,
                                               ViAttr attributeId,
                                               ViInt32         *value)
{
    return Ivi_GetAttributeViInt32 (vi,
                                    channelName,
                                    attributeId,
                                    IVI_VAL_DIRECT_USER_CALL,
                                    value);
}
ViStatus _VI_FUNC zup366_GetAttributeViReal64 (ViSession vi,
                                                ViConstString channelName,
                                                ViAttr attributeId,
                                                ViReal64        *value)
{
    return Ivi_GetAttributeViReal64 (vi,
                                     channelName,
                                     attributeId,
                                     IVI_VAL_DIRECT_USER_CALL,
                                     value);
}
ViStatus _VI_FUNC zup366_GetAttributeViString (ViSession vi,
                                                ViConstString channelName,
                                                ViAttr attributeId,
                                                ViInt32 bufSize,
                                                ViChar value[])
{
    return Ivi_GetAttributeViString (vi,
                                     channelName,
                                     attributeId,
                                     IVI_VAL_DIRECT_USER_CALL,
                                     bufSize,
                                     value);
}
ViStatus _VI_FUNC zup366_GetAttributeViBoolean (ViSession vi,
                                                 ViConstString channelName,
                                                 ViAttr attributeId,
                                                 ViBoolean       *value)
{
    return Ivi_GetAttributeViBoolean (vi,
                                      channelName,
                                      attributeId,
                                      IVI_VAL_DIRECT_USER_CALL,
                                      value);
}
ViStatus _VI_FUNC zup366_GetAttributeViSession (ViSession vi,
                                                 ViConstString channelName,
                                                 ViAttr attributeId,
                                                 ViSession       *value)
{
    return Ivi_GetAttributeViSession (vi,
                                      channelName,
                                      attributeId,
                                      IVI_VAL_DIRECT_USER_CALL,
                                      value);
}

/*****************************************************************************
 * Function: zup366_SetAttribute<type> Functions
 * Purpose:  These functions enable the instrument driver user to set
 *           attribute values directly.  There are typesafe versions for
 *           ViInt32, ViReal64, ViString, ViBoolean, and ViSession datatypes.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_SetAttributeViInt32 (ViSession vi, ViConstString channelName,
                                               ViAttr attributeId, ViInt32 value)
{
    return Ivi_SetAttributeViInt32 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL,
                                    value);
}
ViStatus _VI_FUNC zup366_SetAttributeViReal64 (ViSession vi, ViConstString channelName,
                                                ViAttr attributeId, ViReal64 value)
{
    return Ivi_SetAttributeViReal64 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL,
                                     value);
}
ViStatus _VI_FUNC zup366_SetAttributeViString (ViSession vi, ViConstString channelName,
                                                ViAttr attributeId, ViConstString value)
{
    return Ivi_SetAttributeViString (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL,
                                     value);
}
ViStatus _VI_FUNC zup366_SetAttributeViBoolean (ViSession vi, ViConstString channelName,
                                                 ViAttr attributeId, ViBoolean value)
{
    return Ivi_SetAttributeViBoolean (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL,
                                      value);
}
ViStatus _VI_FUNC zup366_SetAttributeViSession (ViSession vi, ViConstString channelName,
                                                 ViAttr attributeId, ViSession value)
{
    return Ivi_SetAttributeViSession (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL,
                                      value);
}

/*****************************************************************************
 * Function: zup366_CheckAttribute<type> Functions
 * Purpose:  These functions enable the instrument driver user to check
 *           attribute values directly.  These functions check the value you
 *           specify even if you set the ZUP366_ATTR_RANGE_CHECK
 *           attribute to VI_FALSE.  There are typesafe versions for ViInt32,
 *           ViReal64, ViString, ViBoolean, and ViSession datatypes.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_CheckAttributeViInt32 (ViSession vi,
                                                 ViConstString channelName,
                                                 ViAttr attributeId,
                                                 ViInt32 value)
{
    return Ivi_CheckAttributeViInt32 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL,
                                      value);
}
ViStatus _VI_FUNC zup366_CheckAttributeViReal64 (ViSession vi,
                                                  ViConstString channelName,
                                                  ViAttr attributeId,
                                                  ViReal64 value)
{
    return Ivi_CheckAttributeViReal64 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL,
                                       value);
}
ViStatus _VI_FUNC zup366_CheckAttributeViString (ViSession vi,
                                                  ViConstString channelName,
                                                  ViAttr attributeId,
                                                  ViConstString value)
{
    return Ivi_CheckAttributeViString (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL,
                                       value);
}
ViStatus _VI_FUNC zup366_CheckAttributeViBoolean (ViSession vi,
                                                   ViConstString channelName,
                                                   ViAttr attributeId,
                                                   ViBoolean value)
{
    return Ivi_CheckAttributeViBoolean (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL,
                                        value);
}
ViStatus _VI_FUNC zup366_CheckAttributeViSession (ViSession vi,
                                                   ViConstString channelName,
                                                   ViAttr attributeId,
                                                   ViSession value)
{
    return Ivi_CheckAttributeViSession (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL,
                                        value);
}

/*****************************************************************************
 * Function: viWrite and viRead Functions
 * Purpose:  These functions enable the instrument driver user to
 *           write and read commands directly to and from the instrument.
 *
 *           Notes: 1) These functions bypass the IVI attribute state caching.
 *                  viWrite invalidates the cached values for all
 *                  attributes.
 *                  2) These functions are supported with the IVI Compliance
 *                  Package (ICP) 4.6.1 and later. If you are using an
 *                  earlier version of ICP, you can still use the zup366_WriteInstrData
 *                  and zup366_ReadInstrData functions under the Obsolete folder.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_viWrite (ViSession vi, ViByte buffer[], ViInt64 count, ViInt64 *returnCount)
{
    return Ivi_viWrite (vi, buffer, count, returnCount);
}
ViStatus _VI_FUNC zup366_viRead (ViSession vi, ViInt64 bufferSize, ViByte buffer[], ViInt64 *returnCount)
{
    return Ivi_viRead (vi, bufferSize, buffer, returnCount);
}


/*****************************************************************************
 * Function: WriteInstrData and ReadInstrData Functions
 * Purpose:  These functions enable the instrument driver user to
 *           write and read commands directly to and from the instrument.
 *
 *           Notes: 1) These functions bypass the IVI attribute state caching.
 *                  WriteInstrData invalidates the cached values for all
 *                  attributes.
 *                  2) These functions are obsolete since IVI Compliance
 *                  Package (ICP) 4.6.1. You can use zup366_viWrite
 *                  and zup366_viRead instead.
 *****************************************************************************/
ViStatus _VI_FUNC zup366_WriteInstrData (ViSession vi,
                                          ViConstString writeBuffer)
{
    return Ivi_WriteInstrData (vi, writeBuffer);
}
ViStatus _VI_FUNC zup366_ReadInstrData (ViSession vi,
                                         ViInt32 numBytes,
                                         ViChar rdBuf[],
                                         ViInt32         *bytesRead)
{
    return Ivi_ReadInstrData (vi, numBytes, rdBuf, bytesRead);
}

/*****************************************************************************
 *-------------------- Utility Functions (Not Exported) ---------------------*
 *****************************************************************************/
/*****************************************************************************
 * Function: RemoveSurroundingWhiteSpace
 * Purpose:  This function removes surrounding white space around a string
 *****************************************************************************/
static void RemoveSurroundingWhiteSpace (ViChar string[])
{
    ViChar *start = string,
    *end;
    size_t lengthWithoutLeadingWS;

    if (start)
    {
        while (*start && isspace ((unsigned char) *start))
            start++;

        lengthWithoutLeadingWS = strlen (start);
        memmove (string, start, lengthWithoutLeadingWS);
        end = string + lengthWithoutLeadingWS;
        while (end > string && isspace ((unsigned char) *(end - 1)))
            end--;

        *end = '\0';
    }
}

/*****************************************************************************
 * Function: RemoveSurroundingDoubleQuotes
 * Purpose:  This function removes surrounding double quotes around a string
 *****************************************************************************/
static void RemoveSurroundingDoubleQuotes (ViChar string[])
{
    ViChar *start = string,
    *end;
    size_t lengthWithoutLeadingDQ;

    if (start)
    {
        while (*start && (isspace ((unsigned char) *start) || *start == '\"'))
            start++;

        lengthWithoutLeadingDQ = strlen (start);
        memmove (string, start, lengthWithoutLeadingDQ);
        end = string + lengthWithoutLeadingDQ;
        while (end > string && (isspace ((unsigned char) *(end - 1)) || *(end - 1) == '\"'))
            end--;

        *end = '\0';
    }
}

/*****************************************************************************
 *  Function: zup366_GetDriverSetupOption
 *  Purpose:  Get the value of a DriverSetup option. The driver setup string
 *            is part of the option string the user passes to the
 *            agesa_InitWithOptions function. Specifically it is the part
 *            that follows "DriverSetup=". This function assumes the
 *            DriverSetup string has the following format:
 *              "Module1 : N6731A ; Module2 : N6732A ;"
 *            And Module1 stands for the first physical channel,
 *            Module2 stands for the second physical channel.
 *
 *****************************************************************************/
static ViStatus zup366_GetDriverSetupOption (ViSession vi,                                  /* Session */
                                              ViInt32 pInstaledModules[MAX_MODULE_COUNT + 1] /* Returns Model Type */
                                              )
{
    ViStatus error = VI_SUCCESS;
    ViChar   * ptag;
    ViChar   setup[BUFFER_SIZE],
             model_buffer[256],
             module_tag[256];
    ViInt32   i,
              module_type;
    ViBoolean value;

    /* Initialize output variables */
    for (i = 0; i < (MAX_MODULE_COUNT + 1); i++)
        pInstaledModules[i] = NOT_SUPPORTED;

    checkErr (Ivi_GetAttributeViString (vi,
                                        VI_NULL,
                                        IVI_ATTR_DRIVER_SETUP,
                                        0, BUFFER_SIZE,
                                        setup));

    for (i = 1; i <= MAX_MODULE_COUNT; i++)
    {
        sprintf (module_tag, "Module%d", i);
        module_type = NOT_SUPPORTED;

        if ((ptag = strstr (setup, module_tag)) != VI_NULL)
        {
            sscanf (ptag, "%*[^:]:%256[^;]", model_buffer);
            RemoveSurroundingWhiteSpace (model_buffer);
            if (Ivi_GetValueFromTable (zup366_ModuleTable, model_buffer, &module_type) < VI_SUCCESS)
            {
                ViChar message[BUFFER_SIZE];

                sprintf (message, "Bad value for %s option in DriverSetup: %s",
                         module_tag,
                         model_buffer);

                viCheckErrElab (IVI_ERROR_BAD_OPTION_VALUE, message);
            }
        }

        pInstaledModules[i] = module_type;

        isDoubleWideModule (module_type, &value);
        if (value == VI_TRUE) /* Doule Wide Module */
        {
            ++i;              /* skip one slot */
        }
    }

Error:
    return error;
}

/*****************************************************************************
 *  Function: zup366_GetInstrumentSetupOption
 *  Purpose:  Reads information from instrument, which contains installed
 *            modules.
 *****************************************************************************/
static ViStatus zup366_GetInstrumentSetupOption (ViSession io,
                                                  ViInt32 pInstaledModules[MAX_MODULE_COUNT + 1] /* Returns Model Type */,
                                                  ViSession vi)
{
    ViStatus error              = VI_SUCCESS;
    ViChar   module_buffer[256] = "";
    ViInt32  i, module_type, channel_count = 4;
    ViChar   queryReturn [BUFFER_SIZE] = "";
    ViChar   aStr[50]                  = "";
    ViString channelStr                = aStr;

    /* Initialize output variables */
    for (i = 0; i < (MAX_MODULE_COUNT + 1); i++)
        pInstaledModules[i] = NOT_SUPPORTED;

    /* Queries number of output channels */
    checkErr (viQueryf (io, ":SYST:CHAN:COUN?", "%ld", &channel_count));

    /* Query occupied slots */
    checkErr (viQueryf (io, ":SYST:GRO:CAT?", "%s", queryReturn));
    RemoveSurroundingDoubleQuotes (queryReturn);

    for (i = 1; i <= MAX_MODULE_COUNT; i++)
    {
        ViString chStr;
        viCheckErr (Ivi_GetNthChannelString (vi, i, &chStr));

        sprintf (channelStr, "%s", chStr);

        if (strstr (queryReturn, channelStr) == VI_NULL)
        {
            /*added to skip empty slots*/
        }
        else
        {
            checkErr (viQueryf (io, ":SYST:CHAN:MOD? (@%d)", "%256[^\n]", i, module_buffer));

            if (Ivi_GetValueFromTable (zup366_ModuleTable, module_buffer, &module_type) == VI_SUCCESS)
            {
                ViBoolean value;
                pInstaledModules[i] = module_type;
                isDoubleWideModule (module_type, &value);
                if (value == VI_TRUE)
                {        /* Doule Wide Module */
                    ++i; /* skip one slot */
                    ++channel_count;
                }
            }
        }
    }

Error:
    return error;
}

/*****************************************************************************
 *  Function: zup366_CheckListFunctionality
 *  Purpose:  Checks all requirements for List functionality:
 *
 *            if the following conditions are valid, functions returns
 *            VI_SUCCESS, otherwise generatedError.
 *
 *  Notes: Only Models N676xA, N6781A, N6782A, N6784A and Models N675xA,
 *             with Text Extension 054.
 *
 *****************************************************************************/
static ViStatus zup366_CheckListFunctionality (ViSession vi,
                                                ViConstString channelName,
                                                ViStatus generatedError)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  model_type;

    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    if ((model_type != N6761A) &&
        (model_type != N6762A) &&
        (model_type != N6763A) &&
        (model_type != N6764A) &&
        (model_type != N6765A) &&
        (model_type != N6766A) &&
        (model_type != N6751A) &&
        (model_type != N6752A) &&
        (model_type != N6753A) &&
        (model_type != N6754A) &&
        (model_type != N6755A) &&
        (model_type != N6756A) &&
        (model_type != N6762A) &&
        (model_type != N6781A) &&
        (model_type != N6782A) &&
        (model_type != N6784A))
    {
        viCheckErr (generatedError);
    }


    /*
     * Models N675xA requires option 054
     */
    if ((model_type == N6751A) ||
        (model_type == N6752A) ||
        (model_type == N6753A) ||
        (model_type == N6754A) ||
        (model_type == N6755A) ||
        (model_type == N6756A))
    {
        ViInt32 options;

        checkErr (Ivi_GetAttributeViInt32 (vi,
                                           channelName,
                                           ZUP366_ATTR_INSTR_MODULE_OPTIONS,
                                           0, &options));
        if (!IS_OPT_054 (options))
        {
            viCheckErr (generatedError);
        }
    }


Error:
    return error;
}

/*****************************************************************************
 *  Function: zup366_CheckRelayPolarityFunctionality
 *  Purpose:  Checks all requirements for Relay Polarity functionslity:
 *
 *            if the following conditions are valid, functions returns
 *            VI_SUCCESS, otherwise generatedError.
 *
 *  Notes: Only with Option 760
 *	Option 760 (Polarity Reversal) available on models N6742B and N6773A
 *  Required option for N673xB, N674xB**, N675xA**, N676xA**, N677xA*
 * **	Not available for N6741B, N6751A, N6752A, N6761A, N6762A, and N678xA.
 *
 *****************************************************************************/
static ViStatus zup366_CheckRelayPolarityFunctionality (ViSession vi,
                                                         ViConstString channelName,
                                                         ViStatus generatedError)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  options;

    checkErr (Ivi_GetAttributeViInt32 (vi,
                                       channelName,
                                       ZUP366_ATTR_INSTR_MODULE_OPTIONS,
                                       0, &options));
    if (!IS_OPT_760 (options))
    {
        viCheckErr (generatedError);
    }


Error:
    return error;
}

/*****************************************************************************
 *  Function: zup366_GetModuleType
 *  Purpose:  Returns module type on the specified channel
 *
 *****************************************************************************/
static ViStatus zup366_GetModuleType (ViSession vi,
                                       ViConstString channelName,
                                       ViInt32*        pModuleType)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViConstString       pchannel_name;
    ViInt32             channel_index = 0;

    checkErr (Ivi_CoerceChannelName (vi, channelName, &pchannel_name));
    checkErr (Ivi_GetAttributeViAddr (vi,
                                      VI_NULL,
                                      ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                      0, (ViAddr *) &pinstrument_modules));
    channel_index = (pchannel_name[0] - '0');
    if (pinstrument_modules[channel_index])
        *pModuleType = pinstrument_modules[channel_index]->ModelType;
    else
        viCheckErr (IVI_ERROR_BAD_CHANNEL_NAME);
Error:
    return error;
}

/*****************************************************************************
 * Function: zup366_MakeChannelList
 * Purpose:  This function create instrument dependent channel list.
 *****************************************************************************/
static ViStatus zup366_MakeChannelList (ViSession vi,
                                         ViConstString channelList,
                                         ViChar coercedChannelList[])
{
    ViStatus error           = VI_SUCCESS;
    ViChar   * pchannel_list = coercedChannelList;

    if (channelList == NULL || strlen (channelList) == 0)
    {
        ViInt32 channel_count = 4;

        checkErr (Ivi_GetAttributeViInt32 (vi,
                                           VI_NULL,
                                           ZUP366_ATTR_CHANNEL_COUNT,
                                           0, &channel_count));
        if (channel_count == 1)
            pchannel_list += sprintf (pchannel_list, "1");
        else
            pchannel_list += sprintf (pchannel_list, "1:%ld", channel_count);
    }
    else
    {
        ViConstString pcoerced_channel;
        ViChar        buffer[BUFFER_SIZE] = "";
        ViChar        * ptok;
        ViChar        * pend;

        strncpy (buffer, channelList, sizeof (buffer) - 1);
        ptok = pend = buffer;
        do
        {
            pend = strchr (pend, ',');
            if (pend != NULL)
                *pend++ = '\0';

            checkErr (Ivi_CoerceChannelName (vi, ptok, &pcoerced_channel));

            pchannel_list += sprintf (pchannel_list, "%s,", pcoerced_channel);
            ptok           = pend;
        } while (pend != NULL);

        *--pchannel_list = '\0';
    }


Error:
    return error;
}

/*****************************************************************************
 * Function: zup366_ReadUnknownLengthData
 * Purpose:  This function reads data from the instrument. This functions
 *           reads all data from the instrument independent on the buffer size.
 *
 * WARNING: 1) DON'T USE FOR BINARY TRANSFER (we should improve this function
 *                                            for binary transfers)
 *          2) USE free() function FOR pData pointer in function, which call
 *             this function.
 *****************************************************************************/
static ViStatus zup366_ReadUnknownLengthData (ViSession vi,
                                               ViSession io,
                                               ViString*           pData,
                                               ViInt32*            pDataCount)
{
    ViStatus error   = VI_SUCCESS;
    ViChar   * pdata = VI_NULL;
    ViChar   * pend  = VI_NULL;
    ViUInt32 count   = 0;

    if (!Ivi_Simulating (vi))
    {
        ViChar   buffer[BUFFER_SIZE];
        ViChar   * pbuffer       = VI_NULL;
        ViUInt32 allocated_count = 0;
        ViUInt32 readed_count;

        while ((error = viRead (io, buffer, BUFFER_SIZE, &readed_count)) == VI_SUCCESS_MAX_CNT)
        {
            if (allocated_count)
            {
                if (allocated_count <= count + readed_count)
                {
                    allocated_count += (allocated_count - count) + BUFFER_SIZE;
                    pdata            = realloc (pdata, allocated_count);
                }

                pend = pdata + count;
            }
            else
            {
                allocated_count = 2 * BUFFER_SIZE;
                pdata           = malloc (allocated_count);
                pend            = pdata;
            }

            memcpy (pend, buffer, readed_count);
            count += readed_count;
        }

        if (readed_count > 0)
        {
            /*
             * Check if the last character is alnum.
             * if this character is not alnum we know that it's '\n'.
             * Remove it.
             *
             * WARNING: NOT SAFE FOR BINARY TRANSFERS
             */
            if (!isalnum (buffer[readed_count - 1]))
                readed_count--;
            buffer[readed_count] = '\0';
        }

        /*
         * If function is read more data as BUFFER_SIZE, we must
         * copy last data from static buffer to dynamic buffer
         */
        if (pdata)
        {
            /*
             * For sure, check if the  buffer has enought element.
             * This condition should be FALSE every time.
             */
            if (allocated_count <= count + readed_count)
            {
                allocated_count += (allocated_count - count) + BUFFER_SIZE;
                pdata            = realloc (pdata, allocated_count);
            }
            pend = pdata + count;
            memcpy (pend, buffer, readed_count + 1);
            pbuffer = pdata;
        }
        else
        {
            count = readed_count;
            pdata = malloc (sizeof (ViChar) * (readed_count + 1));
            memcpy (pdata, buffer, readed_count);
            *(pdata + readed_count) = '\0';
        }

        viCheckErr (error);
    }

Error:
    if (pDataCount)
        *pDataCount = (ViInt32) count;

    if (error < VI_SUCCESS && pdata)
    {
        free (pdata);
        pdata = NULL;
        if (pDataCount)
            *pDataCount = 0;
    }

    *pData = pdata;
    return error;
}

/*****************************************************************************
 * Function: zup366_array_atof
 * Purpose:  This function convert data which are formated in the string to
 *           double array.
 *
 * WARNING: Data in the string are modified.
 *****************************************************************************/
static ViStatus zup366_array_atof (ViReal64 measurementArray[],
                                    ViInt32 arraySize,
                                    ViChar*     pdata,
                                    ViInt32*    pActualPoints)
{
    ViInt32 i = 0;
    ViChar  * ptok;
    ViChar  * pend;

    ptok = pend = pdata;
    do
    {
        if (i >= arraySize)
            break;

        pend = strchr (pend, ',');
        if (pend != NULL)
            *pend++ = '\0';

        measurementArray[i++] = atof (ptok);

        ptok = pend;
    } while (pend != NULL);

    if (pActualPoints)
        *pActualPoints = i;

    return VI_SUCCESS;
}

/*****************************************************************************
 * Function: zup366_CheckStatus
 * Purpose:  This function checks the status of the instrument to detect
 *           whether the instrument has encountered an error.  This function
 *           is called at the end of most exported driver functions.  If the
 *           instrument reports an error, this function returns
 *           IVI_ERROR_INSTRUMENT_SPECIFIC.  The user can set the
 *           ZUP366_ATTR_QUERY_INSTRUMENT_STATUS attribute to VI_FALSE to disable this
 *           check and increase execution speed.
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
static ViStatus zup366_CheckStatus (ViSession vi)
{
    ViStatus error = VI_SUCCESS;

    if (Ivi_QueryInstrStatus (vi) && Ivi_NeedToCheckStatus (vi) && !Ivi_Simulating (vi))
    {
        checkErr (zup366_CheckStatusCallback (vi, Ivi_IOSession (vi)));
        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_FALSE));
    }

Error:
    return error;
}

/*****************************************************************************
 * Function: zup366_DefaultInstrSetup
 * Purpose:  This function sends a default setup to the instrument.  The
 *           zup366_reset function calls this function.  The
 *           zup366_IviInit function calls this function when the
 *           user passes VI_FALSE for the reset parameter.  This function is
 *           useful for configuring settings that other instrument driver
 *           functions require.
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
static ViStatus zup366_DefaultInstrSetup (ViSession vi)
{
    ViStatus error = VI_SUCCESS;

    /* Invalidate all attributes */
    checkErr (Ivi_InvalidateAllAttributes (vi));

    if (!Ivi_Simulating (vi))
    {
        ViSession io = Ivi_IOSession (vi);  /* call only when locked */

        checkErr (Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        viCheckErr (viPrintf (io, "*CLS;*ESE 1;*SRE 32"));
    }

Error:
    return error;
}

/*****************************************************************************
 *  Function: zup366_GetIDNInformation
 *  Purpose:  This function scans ID Query and sets string attribute. This
 *            function is used for following attributes:
 *			       ZUP366_ATTR_INSTRUMENT_MODEL
 *                 ZUP366_ATTR_INSTRUMENT_MANUFACTURER
 *                 ZUP366_ATTR_INSTRUMENT_FIRMWARE_REVISION
 *****************************************************************************/
static ViStatus zup366_GetIDNInformation (ViSession vi,
                                           ViAttr attributeId,
                                           ViConstString formatString)
{
    ViStatus error = VI_SUCCESS;
    ViChar   idn_query[256];

    checkErr (Ivi_GetAttributeViString (vi,
                                        VI_NULL,
                                        ZUP366_ATTR_ID_QUERY_RESPONSE,
                                        0, 256,
                                        idn_query));

    sscanf (idn_query, formatString, idn_query);
    RemoveSurroundingWhiteSpace (idn_query);
    checkErr (Ivi_SetValInStringCallback (vi, attributeId, idn_query));

Error:
    return error;
}

/*****************************************************************************
 * Function: isDoubleWideModule
 * Purpose:  This function checks whether the module is double wide or not.
 *
 *
 *****************************************************************************/
static void isDoubleWideModule (ViInt32 module_type, ViBoolean* value)
{
    if ((module_type == N6753A || module_type == N6754A) || (module_type == N6755A || module_type == N6756A) ||
        (module_type == N6763A || module_type == N6764A) || (module_type == N6765A || module_type == N6766A) ||
        (module_type == N6785A || module_type == N6786A))
    {
        *value = VI_TRUE;
    }
    else
    {
        *value = VI_FALSE;
    }
}

/*****************************************************************************
 *------------------------ Global Session Callbacks -------------------------*
 *****************************************************************************/
/*****************************************************************************
 * Function: zup366_CheckStatusCallback
 * Purpose:  This function queries the instrument to determine if it has
 *           encountered an error.  If the instrument has encountered an
 *           error, this function returns the IVI_ERROR_INSTRUMENT_SPECIFIC
 *           error code.  This function is called by the
 *           zup366_CheckStatus utility function.  The
 *           Ivi_SetAttribute and Ivi_GetAttribute functions invoke this
 *           function when the optionFlags parameter includes the
 *           IVI_VAL_DIRECT_USER_CALL flag.
 *
 *           The user can disable calls to this function by setting the
 *           ZUP366_QUERY_INSTRUMENT_STATUS attribute to VI_FALSE.  The driver can
 *           disable the check status callback for a particular attribute by
 *           setting the IVI_VAL_DONT_CHECK_STATUS flag.
 *****************************************************************************/
static ViStatus _VI_FUNC zup366_CheckStatusCallback (ViSession vi,
                                                      ViSession io)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  esr   = 0;

    viCheckErr (viQueryf (io, "*ESR?", "%ld", &esr));
    if (esr & IEEE_488_2_ERROR_MASK)
    {
        viCheckErr (IVI_ERROR_INSTR_SPECIFIC);
    }

Error:
    return error;
}

/*****************************************************************************
 * Function: zup366ViBoolean_WriteCallback
 * Purpose:  This function sets the bool value of the ViBoolean attribute.
 *****************************************************************************/
static ViStatus _VI_FUNC zup366ViBoolean_WriteCallback (ViSession vi,
                                                         ViSession io,
                                                         ViConstString channelName,
                                                         ViAttr attributeId,
                                                         ViBoolean value)
{
    ViStatus error = VI_SUCCESS;
    ViString command;

    viCheckErr (Ivi_GetStringFromTable (zup366ViBoolean_commands,
                                        attributeId,
                                        &command));
    if (!Ivi_Simulating (vi))
    {
            viCheckErr (viPrintf (io, "%s%s;", command, value ? "1" : "0"));
    }

Error:
    return error;
}

/*****************************************************************************
 * Function: zup366ViBoolean_ReadCallback
 * Purpose:  This function reads the bool value of the ViBoolean attribute.
 *****************************************************************************/
static ViStatus _VI_FUNC zup366ViBoolean_ReadCallback (ViSession vi,
                                                        ViSession io,
                                                        ViConstString channelName,
                                                        ViAttr attributeId,
                                                        ViBoolean*      value)
{
    ViStatus error = VI_SUCCESS;
    ViString command;

    viCheckErr (Ivi_GetStringFromTable (zup366ViBoolean_commands, attributeId, &command));

    if (!Ivi_Simulating (vi))
    {
        if (channelName && *channelName != '\0')
        {
            /*
             * channel based attribute
             */
            viCheckErr (viQueryf (io, "%s? (@%s)", "%hd", command, channelName, value));
        }
        else
        {
            viCheckErr (viQueryf (io, "%s?", "%hd", command, value));
        }
    }

Error:
    return error;
}

/*****************************************************************************
 * Function: zup366ViReal64_WriteCallback
 * Purpose:  This function sets the bool value of the ViReal64 attribute.
 *****************************************************************************/
static ViStatus _VI_FUNC zup366ViReal64_WriteCallback (ViSession vi,
                                                        ViSession io,
                                                        ViConstString channelName,
                                                        ViAttr attributeId,
                                                        ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    ViString command;
	char  Chaine[128];
	
    viCheckErr (Ivi_GetStringFromTable (zup366ViReal64_commands, attributeId, &command));

    if (!Ivi_Simulating (vi))
    {
        if (channelName && *channelName != '\0')
        {
            /*
             * channel based attribute
             */
	         viCheckErr (viPrintf (io, "%s%05.2f;", command, value));
		}
        else
        {
            viCheckErr (viPrintf (io, "%s %Le", command, value));
        }
    }

Error:
    return error;
}

/*****************************************************************************
 * Function: zup366ViReal64_ReadCallback
 * Purpose:  This function reads the bool value of the ViReal64 attribute.
 *****************************************************************************/
static ViStatus _VI_FUNC zup366ViReal64_ReadCallback (ViSession vi,
                                                       ViSession io,
                                                       ViConstString channelName,
                                                       ViAttr attributeId,
                                                       ViReal64*       value)
{
    ViStatus error = VI_SUCCESS;
    ViString command;

    viCheckErr (Ivi_GetStringFromTable (zup366ViReal64_commands, attributeId, &command));

    if (!Ivi_Simulating (vi))
    {
        if (channelName && *channelName != '\0')
        {
            /*
             * channel based attribute
             */
            //viCheckErr (viQueryf (io, "%s? (@%s)", "%Le", command, channelName, value));
        }
        else
        {
            viCheckErr (viQueryf (io, "%s?", "%Le", command, value));
        }
    }

Error:
    return error;
}

/*****************************************************************************
 * Function: zup366ViInt32_WriteCallback
 * Purpose:  This function sets the value of the ViInt32 attribute.
 *****************************************************************************/
static ViStatus _VI_FUNC zup366ViInt32_WriteCallback (ViSession vi,
                                                       ViSession io,
                                                       ViConstString channelName,
                                                       ViAttr attributeId,
                                                       ViInt32 value)
{
    ViStatus error = VI_SUCCESS;
    ViString command;
	char Chaine [50];

    viCheckErr (Ivi_GetStringFromTable (zup366ViInt32_commands, attributeId, &command));

    if (!Ivi_Simulating (vi))
    {
        if (channelName && *channelName != '\0')
        {
            /*
             * channel based attribute
             */
			 sprintf (Chaine,"%s%05.2f;", command, value);
			 viCheckErr (viBufWrite (io, Chaine, strlen(Chaine), 0));
            //viCheckErr (viPrintf (io, "%s %Ld,(@%s)", command, value, channelName));
        }
        else
        {
            viCheckErr (viPrintf (io, "%s %Ld", command, value));
        }
    }

Error:
    return error;
}

/*****************************************************************************
 * Function: zup366ViInt32_ReadCallback
 * Purpose:  This function reads the value of the ViInt32 attribute.
 *****************************************************************************/
static ViStatus _VI_FUNC zup366ViInt32_ReadCallback (ViSession vi,
                                                      ViSession io,
                                                      ViConstString channelName,
                                                      ViAttr attributeId,
                                                      ViInt32 *value)
{
    ViStatus error = VI_SUCCESS;
    ViString command;

    viCheckErr (Ivi_GetStringFromTable (zup366ViInt32_commands, attributeId, &command));

    if (!Ivi_Simulating (vi))
    {
        if (channelName && *channelName != '\0')
        {
            /*
             * channel based attribute
             */
            viCheckErr (viQueryf (io, "%s? (@%s)", "%Ld", command, channelName, value));
        }
        else
        {
            viCheckErr (viQueryf (io, "%s?", "%Ld", command, value));
        }
    }

Error:
    return error;
}

/*****************************************************************************
 * Function: zup366Enum_WriteCallback
 * Purpose:  This function sets the enum value of the ViInt32 attribute.
 *****************************************************************************/
static ViStatus _VI_FUNC zup366Enum_WriteCallback (ViSession vi,
                                                    ViSession io,
                                                    ViConstString channelName,
                                                    ViAttr attributeId,
                                                    ViInt32 value)
{
    ViStatus         error = VI_SUCCESS;
    ViString         command,
                     commandValue;
    IviRangeTablePtr table;

    viCheckErr (Ivi_GetStringFromTable (zup366Enum_commands, attributeId, &command));
    checkErr (Ivi_GetAttrRangeTable (vi, channelName, attributeId, &table));
    viCheckErr (Ivi_GetViInt32EntryFromValue (value, table, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &commandValue,
                                              VI_NULL));

    if (!Ivi_Simulating (vi))
    {
        if (channelName && *channelName != '\0')
        {
            /*
             * channel based attribute
             */
            viCheckErr (viPrintf (io, "%s %s,(@%s)", command, commandValue, channelName));
        }
        else
        {
            viCheckErr (viPrintf (io, "%s %s", command, commandValue));
        }
    }


Error:
    return error;
}

/*****************************************************************************
 * Function: zup366Enum_ReadCallback
 * Purpose:  This function reads the enum value of the ViInt32 attribute.
 *****************************************************************************/
static ViStatus _VI_FUNC zup366Enum_ReadCallback (ViSession vi,
                                                   ViSession io,
                                                   ViConstString channelName,
                                                   ViAttr attributeId,
                                                   ViInt32 *value)
{
    ViStatus         error = VI_SUCCESS;
    ViString         command;
    IviRangeTablePtr table;

    viCheckErr (Ivi_GetStringFromTable (zup366Enum_commands, attributeId, &command));
    checkErr (Ivi_GetAttrRangeTable (vi, channelName, attributeId, &table));

    if (!Ivi_Simulating (vi))
    {
        ViChar buffer[BUFFER_SIZE];

        if (channelName && *channelName != '\0')
        {
            /*
             * channel based attribute
             */
            viCheckErr (viQueryf (io, "%s? (@%s)", "%s", command, channelName, buffer));
        }
        else
        {
            viCheckErr (viQueryf (io, "%s?", "%s", command, buffer));
        }
        viCheckErr (Ivi_GetViInt32EntryFromString (buffer, table, value, VI_NULL,
                                                   VI_NULL, VI_NULL, VI_NULL));
    }
Error:
    return error;
}

/*****************************************************************************
 * Function: zup366ViString_WriteCallback
 * Purpose:  This function sets the string value of the ViString attribute.
 *****************************************************************************/
static ViStatus _VI_FUNC zup366ViString_WriteCallback (ViSession vi,
                                                        ViSession io,
                                                        ViConstString channelName,
                                                        ViAttr attributeId,
                                                        ViConstString value)
{
    ViStatus error = VI_SUCCESS;
    ViString command;

    viCheckErr (Ivi_GetStringFromTable (zup366ViString_commands, attributeId, &command));
    if (!Ivi_Simulating (vi))
    {
        if (channelName != VI_NULL)
        {
            ViChar buffer[BUFFER_SIZE];
            sprintf (buffer, command, channelName);
            error = viPrintf (io, "%s %s", buffer, value);
        }
        else
        {
            error = viPrintf (io, "%s %s", command, value);
        }
        viCheckErr (error);
    }

Error:
    return error;
}

/*****************************************************************************
 *----------------- Attribute Range Tables and Callbacks --------------------*
 *****************************************************************************/

/*- ZUP366_ATTR_ID_QUERY_RESPONSE -*/
static ViStatus _VI_FUNC zup366AttrIdQueryResponse_ReadCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  const ViConstString cacheValue)
{
    ViStatus error = VI_SUCCESS;
    ViChar   rdBuffer[BUFFER_SIZE];
    ViUInt32 retCnt;

    viCheckErr (viPrintf (io, "*IDN?"));
    viCheckErr (viRead (io, rdBuffer, BUFFER_SIZE - 1, &retCnt));
    rdBuffer[retCnt] = 0;

    checkErr (Ivi_SetValInStringCallback (vi, attributeId, rdBuffer));

Error:
    return error;
}

/*- ZUP366_ATTR_SPECIFIC_DRIVER_REVISION -*/
static ViStatus _VI_FUNC zup366AttrDriverRevision_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 const ViConstString cacheValue)
{
    ViStatus error = VI_SUCCESS;
    ViChar   driverRevision[256];

    sprintf (driverRevision,
             "Driver: zup366 %d.%d, Compiler: %s %3.2f, "
             "Components: IVIEngine %.2f, VISA-Spec %.2f",
             ZUP366_MAJOR_VERSION, ZUP366_MINOR_VERSION,
             IVI_COMPILER_NAME, IVI_COMPILER_VER_NUM,
             IVI_ENGINE_MAJOR_VERSION + IVI_ENGINE_MINOR_VERSION / 1000.0,
             Ivi_ConvertVISAVer (VI_SPEC_VERSION));

    checkErr (Ivi_SetValInStringCallback (vi, attributeId, driverRevision));
Error:
    return error;
}

/*- ZUP366_ATTR_INSTRUMENT_FIRMWARE_REVISION -*/
static ViStatus _VI_FUNC zup366AttrFirmwareRevision_ReadCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   const ViConstString cacheValue)
{
    return zup366_GetIDNInformation (vi, attributeId, "%*[^,],%*[^,],%*[^,],%256[^\n]");
}

/*- ZUP366_ATTR_INSTRUMENT_MANUFACTURER -*/
static ViStatus _VI_FUNC zup366AttrInstrumentManufacturer_ReadCallback (ViSession vi,
                                                                         ViSession io,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         const ViConstString cacheValue)
{
    return zup366_GetIDNInformation (vi, attributeId, "%256[^,]");
}


/*- ZUP366_ATTR_INSTRUMENT_MODEL -*/
static ViStatus _VI_FUNC zup366AttrInstrumentModel_ReadCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  const ViConstString cacheValue)
{
    return zup366_GetIDNInformation (vi, attributeId, "%*[^,],%256[^,]");
}

/*- ZUP366_ATTR_OUTPUT_ENABLED -*/
/*- Shared Write callback zup366ViBoolean_WriteCallback -*/
/*- Shared Read callback zup366ViBoolean_ReadCallback -*/

/*- ZUP366_ATTR_OUTPUT_PROTECTION_DELAY -*/
/*- Shared Write callback zup366ViReal64_WriteCallback -*/
/*- Shared Read callback zup366ViReal64_ReadCallback -*/
static IviRangeTableEntry attrOutputProtectionDelayRangeTableEntries[] =
{
    { 0.0, 255.0e-3, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY },
};
static IviRangeTable      attrOutputProtectionDelayRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrOutputProtectionDelayRangeTableEntries,
};

/*- ZUP366_ATTR_VOLTAGE_LEVEL -*/
/*- Shared Write callback zup366ViReal64_WriteCallback -*/
/*- Shared Read callback zup366ViReal64_ReadCallback -*/
static ViStatus _VI_FUNC zup366AttrVoltageLevel_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViInt32             channel_index         = 0;
    ViInt32             result                = 0;
    ViReal64            minimum;
    ViReal64            maximum;
    ViReal64            voltage_range;
    ViInt32             range_index;

    checkErr (Ivi_GetAttributeViAddr (vi,
                                      VI_NULL,
                                      ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                      0, (ViAddr *) &pinstrument_modules));
    channel_index = (channelName[0] - '0');


    checkErr (Ivi_GetAttributeViReal64 (vi,
                                        channelName, ZUP366_ATTR_INSTR_VOLTAGE_RANGE, 0,
                                        &voltage_range));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrVoltageLevel_CoerceCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value,
                                                                 ViReal64 *coercedValue)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViInt32             channel_index         = 0;
    ViInt32             result                = 0;
    ViReal64            minimum;
    ViReal64            maximum;
    ViReal64            voltage_range;
    ViInt32             range_index;

	*coercedValue = value;

Error:
    return error;
}

/*- ZUP366_ATTR_OVP_ENABLED -*/
/*- Shared Write callback zup366ViBoolean_WriteCallback -*/
/*- Shared Read callback zup366ViBoolean_ReadCallback -*/

/*- ZUP366_ATTR_OVP_LIMIT -*/
/*- Shared Write callback zup366ViReal64_WriteCallback -*/
/*- Shared Read callback zup366ViReal64_ReadCallback -*/
static ViStatus _VI_FUNC zup366AttrOvpLimit_CheckCallback (ViSession vi,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 value)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViInt32             channel_index         = 0;
    ViReal64            maximum;
    ViReal64            minimum = 0.0;

    checkErr (Ivi_GetAttributeViAddr (vi,
                                      VI_NULL,
                                      ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                      0, (ViAddr *) &pinstrument_modules));
    channel_index = (channelName[0] - '0');

    maximum = pinstrument_modules[channel_index]->MaxOVP;

    if (value < minimum || value > maximum)
        viCheckErr (IVI_ERROR_INVALID_VALUE);

Error:
    return error;
}

/*- ZUP366_ATTR_RELAY_POLARITY -*/
/*- Shared Write callback zup366ViEnum_WriteCallback -*/
static ViStatus _VI_FUNC zup366AttrRelayPolarity_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_DefaultCheckCallbackViInt32 (vi,
                                               channelName,
                                               attributeId,
                                               value));
    checkErr (zup366_CheckRelayPolarityFunctionality (vi,
                                                       channelName,
                                                       IVI_ERROR_ATTRIBUTE_NOT_SUPPORTED));

Error:
    return error;
}

/*- ZUP366_ATTR_POWER_LIMIT -*/
/*- Shared Write callback zup366ViReal64_WriteCallback -*/
static ViStatus _VI_FUNC zup366AttrPowerLimit_CheckCallback (ViSession vi,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 value)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViInt32             channel_index         = 0;
    ViReal64            maximum;
    ViReal64            minimum = 0.0;

    checkErr (Ivi_GetAttributeViAddr (vi,
                                      VI_NULL,
                                      ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                      0, (ViAddr *) &pinstrument_modules));
    channel_index = (channelName[0] - '0');

    maximum = pinstrument_modules[channel_index]->MaxPower;

    if (value < minimum || value > maximum)
        viCheckErr (IVI_ERROR_INVALID_VALUE);

Error:
    return error;
}

/*- ZUP366_ATTR_CURRENT_LIMIT_BEHAVIOR -*/
static IviRangeTableEntry attrCurrentLimitBehaviorRangeTableEntries[] =
{
    { ZUP366_VAL_CURRENT_REGULATE, 0, 0, "0", 0 },
    { ZUP366_VAL_CURRENT_TRIP,     0, 0, "1", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY },
};
static IviRangeTable      attrCurrentLimitBehaviorRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrCurrentLimitBehaviorRangeTableEntries,
};
/*- Shared Write callback zup366Enum_WriteCallback -*/
/*- Shared Read callback zup366Enum_ReadCallback -*/

/*- ZUP366_ATTR_CURRENT_LIMIT -*/
/*- Shared Write callback zup366ViReal64_WriteCallback -*/
/*- Shared Read callback zup366ViReal64_ReadCallback -*/
static ViStatus _VI_FUNC zup366AttrCurrentLimit_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViInt32             channel_index         = 0;
    ViReal64            minimum;
    ViReal64            maximum;
    ViInt32             result = 0;
    ViReal64            current_range;
    ViInt32             range_index;


    checkErr (Ivi_GetAttributeViAddr (vi,
                                      VI_NULL,
                                      ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                      0, (ViAddr *) &pinstrument_modules));


    channel_index = (channelName[0] - '0');


    checkErr (Ivi_GetAttributeViReal64 (vi,
                                        channelName, ZUP366_ATTR_INSTR_CURRENT_RANGE, 0,
                                        &current_range));

Error:
    return error;
}

/*- ZUP366_ATTR_TRIGGER_SOURCE -*/
/*- ZUP366_ATTR_ACQUSITION_TRIGGER_SOURCE -*/
/*- Shared Write callback zup366ViEnum_WriteCallback -*/
/*- Shared Read callback zup366ViEnum_ReadCallback -*/
static IviRangeTableEntry attrTriggerSourceRangeTableEntries[] =
{
    { ZUP366_VAL_TRIG_EXTERNAL, 0, 0, "EXT",   0 },
    { ZUP366_VAL_SOFTWARE_TRIG, 0, 0, "BUS",   0 },
    { ZUP366_VAL_TRIG_PIN1,     0, 0, "PIN1",  0 },
    { ZUP366_VAL_TRIG_PIN2,     0, 0, "PIN2",  0 },
    { ZUP366_VAL_TRIG_PIN3,     0, 0, "PIN3",  0 },
    { ZUP366_VAL_TRIG_TRAN1,    0, 0, "TRAN1", 0 },
    { ZUP366_VAL_TRIG_TRAN2,    0, 0, "TRAN2", 0 },
    { ZUP366_VAL_TRIG_TRAN3,    0, 0, "TRAN3", 0 },
    { ZUP366_VAL_TRIG_TRAN4,    0, 0, "TRAN4", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY },
};
static IviRangeTable      attrTriggerSourceRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrTriggerSourceRangeTableEntries,
};

/*- ZUP366_ATTR_TRIGGERED_VOLTAGE_LEVEL -*/
/*- Shared Write callback zup366ViReal64_WriteCallback -*/
/*- Shared Read callback zup366ViReal64_ReadCallback -*/

static ViStatus _VI_FUNC zup366AttrTrigVoltageLevel_CheckCallback (ViSession vi,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViReal64 value)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViInt32             channel_index         = 0;
    ViInt32             range_index           = 0;
    ViReal64            minimum;
    ViReal64            maximum;
    ViReal64            voltage_range;
    ViInt32             result;

    checkErr (Ivi_GetAttributeViAddr (vi,
                                      VI_NULL,
                                      ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                      0, (ViAddr *) &pinstrument_modules));
    channel_index = (channelName[0] - '0');

    checkErr (Ivi_GetAttributeViReal64 (vi,
                                        channelName, ZUP366_ATTR_INSTR_VOLTAGE_RANGE, 0,
                                        &voltage_range));
    for (range_index = 0; range_index < MAX_NB_RANGES; range_index++)
    {
        viCheckErr (Ivi_CompareWithPrecision (0, pinstrument_modules[channel_index]->MaxVoltage[range_index],
                                              voltage_range,
                                              &result));
        if (result == 0)
            break;
    }
    /*
     * For sure, check that range_index is less as MAX_NB_RANGES
     */
    if (range_index >= MAX_NB_RANGES)
        range_index = (MAX_NB_RANGES - 1);

    minimum = pinstrument_modules[channel_index]->MinVoltage[range_index];
    maximum = voltage_range;

    if (value < minimum || value > maximum)
        viCheckErr (IVI_ERROR_INVALID_VALUE);

Error:
    return error;
}

/*- ZUP366_ATTR_TRIGGERED_CURRENT_LIMIT -*/
/*- Shared Write callback zup366ViReal64_WriteCallback -*/
/*- Shared Read callback zup366ViReal64_ReadCallback -*/
static ViStatus _VI_FUNC zup366AttrTrigCurrentLimit_CheckCallback (ViSession vi,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViReal64 value)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViInt32             channel_index         = 0;
    ViInt32             result                = 0;
    ViReal64            current_range;
    ViInt32             range_index;
    ViReal64            minimum;
    ViReal64            maximum;

    checkErr (Ivi_GetAttributeViAddr (vi,
                                      VI_NULL,
                                      ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                      0, (ViAddr *) &pinstrument_modules));
    channel_index = (channelName[0] - '0');

    checkErr (Ivi_GetAttributeViReal64 (vi,
                                        channelName, ZUP366_ATTR_INSTR_CURRENT_RANGE, 0,
                                        &current_range));

    for (range_index = 0; range_index < MAX_NB_RANGES; range_index++)
    {
        viCheckErr (Ivi_CompareWithPrecision (0, pinstrument_modules[channel_index]->MaxCurrent[range_index],
                                              current_range,
                                              &result));
        if (result == 0)
            break;
    }
    /*
     * For sure, check that range_index is less as MAX_NB_RANGES
     */
    if (range_index >= MAX_NB_RANGES)
        range_index = (MAX_NB_RANGES - 1);

    minimum = pinstrument_modules[channel_index]->MinCurrent[range_index];
    maximum = current_range;

    if (value < minimum || value > maximum)
        viCheckErr (IVI_ERROR_INVALID_VALUE);

Error:
    return error;
}

/*- ZUP366_ATTR_INSTR_VOLTAGE_RANGE -*/
/*- Shared Write callback zup366ViReal64_WriteCallback -*/
/*- Shared Read callback zup366ViReal64_ReadCallback -*/
static ViStatus _VI_FUNC zup366AttrInstrVoltageRange_CoerceCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViReal64 value,
                                                                      ViReal64        *coercedValue)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViReal64            new_value             = value;
    ViInt32             channel_index         = 0;
    ViReal64            minimum, maximum;
    ViInt32             result;

    checkErr (Ivi_GetAttributeViAddr (vi,
                                      VI_NULL,
                                      ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                      0, (ViAddr *) &pinstrument_modules));
    channel_index = (channelName[0] - '0');


    minimum = pinstrument_modules[channel_index]->MinVoltage[MAX_NB_RANGES - 2];
    maximum = pinstrument_modules[channel_index]->MaxVoltage[MAX_NB_RANGES - 1];

    viCheckErr (Ivi_CompareWithPrecision (0, minimum, value, &result));
    if (result >= 0)
    {
        new_value = minimum;
    }
    else
    {
        viCheckErr (Ivi_CompareWithPrecision (0, maximum, value, &result));
        if (result <= 0)
        {
            new_value = maximum;
        }
    }

Error:
    *coercedValue = new_value;
    return error;
}

/*- ZUP366_ATTR_INSTR_CURRENT_RANGE -*/
/*- Shared Write callback zup366ViReal64_WriteCallback -*/
/*- Shared Read callback zup366ViReal64_ReadCallback -*/
static ViStatus _VI_FUNC zup366AttrInstrCurrentRange_CoerceCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViReal64 value,
                                                                      ViReal64        *coercedValue)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViReal64            new_value             = value;
    ViInt32             channel_index         = 0;
    ViReal64            minimum, maximum;
    ViInt32             result;

    checkErr (Ivi_GetAttributeViAddr (vi,
                                      VI_NULL,
                                      ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                      0, (ViAddr *) &pinstrument_modules));
    channel_index = (channelName[0] - '0');

    minimum = pinstrument_modules[channel_index]->MinCurrent[MAX_NB_RANGES - 2];
    maximum = pinstrument_modules[channel_index]->MaxCurrent[MAX_NB_RANGES - 1];

    viCheckErr (Ivi_CompareWithPrecision (0, minimum, value, &result));
    if (result >= 0)
    {
        new_value = minimum;
    }
    else
    {
        viCheckErr (Ivi_CompareWithPrecision (0, maximum, value, &result));
        if (result <= 0)
        {
            new_value = maximum;
        }
    }
Error:
    *coercedValue = new_value;
    return error;
}

/*- ZUP366_ATTR_INSTR_VOLTAGE_MEASUREMENT_RANGE -*/
/*- ZUP366_ATTR_INSTR_CURRENT_MEASUREMENT_RANGE -*/
static ViStatus _VI_FUNC zup366AttrInstrMeasurementRange_WriteCallback (ViSession vi,
                                                                         ViSession io,
                                                                         ViConstString channelName,
                                                                         ViAttr attributeId,
                                                                         ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  module_type;

    checkErr (zup366_GetModuleType (vi, channelName, &module_type));

    if ((module_type == N6761A) ||
        (module_type == N6762A))
    {
        checkErr (zup366ViReal64_WriteCallback (vi, io, channelName, attributeId, value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrInstrMeasurementRange_ReadCallback (ViSession vi,
                                                                        ViSession io,
                                                                        ViConstString channelName,
                                                                        ViAttr attributeId,
                                                                        ViReal64*       value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  module_type;

    checkErr (zup366_GetModuleType (vi, channelName, &module_type));

    if ((module_type == N6761A) ||
        (module_type == N6762A))
    {
        checkErr (zup366ViReal64_ReadCallback (vi, io, channelName, attributeId, value));
    }

Error:
    return error;
}

/*- Trigger-List Attributes -------------------------------------------------*/
/*- ZUP366_ATTR_LIST_COUNT -*/
static IviRangeTableEntry attrListCountRangeTableEntries[] =
{
    { 1, 256, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};
static IviRangeTable      attrListCountRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrListCountRangeTableEntries,
};

static ViStatus _VI_FUNC zup366AttrListCount_CheckCallback (ViSession vi,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViInt32 value)
{
    ViStatus error = VI_SUCCESS;

    if (value != ZUP366_VAL_LIST_COUNT_INFINITY)
        checkErr (Ivi_DefaultCheckCallbackViInt32 (vi, channelName, attributeId, value));

    checkErr (zup366_CheckListFunctionality (vi, channelName, IVI_ERROR_ATTRIBUTE_NOT_SUPPORTED));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrListCount_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViInt32 value)
{
    ViStatus error = VI_SUCCESS;

    if (value < 0)
        viCheckErr (viPrintf (io, ":SOUR:LIST:COUN INF,(@%s)", channelName));
    else
        viCheckErr (viPrintf (io, ":SOUR:LIST:COUN %d,(@%s)", value, channelName));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrListCount_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViInt32         *value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  x     = 0;

    viCheckErr (viQueryf (io, ":SOUR:LIST:COUN? (@%s)", "%d", channelName, &x));

    if (x <= 9.9e37)
        *value = x;
    else
        *value = 0;
Error:
    return error;
}

/*- ZUP366_ATTR_LIST_STEP -*/
/*- Shared Write callback zup366Enum_WriteCallback -*/
/*- Shared Read callback zup366Enum_ReadCallback -*/
static IviRangeTableEntry attrListStepRangeTableEntries[] =
{
    { ZUP366_VAL_LIST_STEP_ONCE, 0, 0, "ONCE", 0 },
    { ZUP366_VAL_LIST_STEP_AUTO, 0, 0, "AUTO", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};
static IviRangeTable      attrListStepRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrListStepRangeTableEntries
};

static ViStatus _VI_FUNC zup366AttrListViInt32_CheckCallback (ViSession vi,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_DefaultCheckCallbackViInt32 (vi, channelName, attributeId, value));
    checkErr (zup366_CheckListFunctionality (vi, channelName, IVI_ERROR_ATTRIBUTE_NOT_SUPPORTED));

Error:
    return error;
}

/*- ZUP366_ATTR_TRIGGER_CURRENT_MODE -*/
/*- Shared Write callback zup366Enum_WriteCallback -*/
/*- Shared Read callback zup366Enum_ReadCallback -*/
static IviRangeTableEntry attrCurrentModeRangeTableEntries[] =
{
    { ZUP366_VAL_TRIGGER_CURRENT_MODE_FIXED, 0, 0, "FIX",  0 },
    { ZUP366_VAL_TRIGGER_CURRENT_MODE_STEP,  0, 0, "STEP", 0 },
    { ZUP366_VAL_TRIGGER_CURRENT_MODE_LIST,  0, 0, "LIST", 0 },
    { ZUP366_VAL_TRIGGER_CURRENT_MODE_ARB,   0, 0, "ARB",  0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};
static IviRangeTable      attrCurrentModeRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrCurrentModeRangeTableEntries,
};

/*- ZUP366_ATTR_TRIGGER_VOLTAGE_MODE -*/
/*- Shared Write callback zup366Enum_WriteCallback -*/
/*- Shared Read callback zup366Enum_ReadCallback -*/
static IviRangeTableEntry attrVoltageModeRangeTableEntries[] =
{
    { ZUP366_VAL_TRIGGER_VOLTAGE_MODE_FIXED, 0, 0, "FIX",  0 },
    { ZUP366_VAL_TRIGGER_VOLTAGE_MODE_STEP,  0, 0, "STEP", 0 },
    { ZUP366_VAL_TRIGGER_VOLTAGE_MODE_LIST,  0, 0, "LIST", 0 },
    { ZUP366_VAL_TRIGGER_VOLTAGE_MODE_ARB,   0, 0, "ARB",  0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};
static IviRangeTable      attrVoltageModeRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrVoltageModeRangeTableEntries,
};

/*- ZUP366_ATTR_RELAY_POLARITY -*/
/*- Shared Write callback zup366Enum_WriteCallback -*/
static IviRangeTableEntry attrRelayPolarityRangeTableEntries[] =
{
    { ZUP366_VAL_RELAY_POLARITY_NORMAL,  0, 0, "NORM", 0 },
    { ZUP366_VAL_RELAY_POLARITY_REVERSE, 0, 0, "REV",  0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};
static IviRangeTable      attrRelayPolarityRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrRelayPolarityRangeTableEntries
};

/*- ZUP366_ATTR_LIST_TRIGGER_OUTPUT_ENABLED -*/
/*- ZUP366_ATTR_LIST_TERMINATE_LAST -*/
/*- Shared Write callback zup366ViBoolean_WriteCallback -*/
/*- Shared Read callback zup366ViBoolean_ReadCallback -*/
static ViStatus _VI_FUNC zup366AttrListViBoolean_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViBoolean value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366_CheckListFunctionality (vi, channelName, IVI_ERROR_ATTRIBUTE_NOT_SUPPORTED));

Error:
    return error;
}

/*- ZUP366_ATTR_LIST_VOLTAGE_POINTS -*/
static ViStatus _VI_FUNC zup366AttrListViInt32_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32         *value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366_CheckListFunctionality (vi, channelName, IVI_ERROR_ATTRIBUTE_NOT_SUPPORTED));
    checkErr (zup366ViInt32_ReadCallback (vi, io, channelName, attributeId, value));

Error:
    return error;
}

/*- ZUP366_ATTR_LIST_CURRENT_POINTS -*/
/*- Shared Read callback zup366AttrListViInt32_ReadCallback -*/

/*- ZUP366_ATTR_LIST_DWELL_POINTS -*/
/*- Shared Read callback zup366AttrListViInt32_ReadCallback -*/

/*- ZUP366_ATTR_LIST_TRIGGER_OUTPUT_BOST_POINTS -*/
/*- Shared Read callback zup366AttrListViInt32_ReadCallback -*/

/*- ZUP366_ATTR_LIST_TRIGGER_OUTPUT_EOST_POINTS -*/
/*- Shared Read callback zup366AttrListViInt32_ReadCallback -*/

/*- ZUP366_ATTR_SWEEP_POINTS -*/
/*- Shared Read callback zup366ViInt32_ReadCallback -*/
/*- Shared Write callback zup366ViInt32_WriteCallback -*/
/*- Shared Check callback zup366AttrListViInt32_CheckCallback -*/
static IviRangeTableEntry attrSweepPointsRangeTableEntries[] =
{
    { 1, 524288, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};
static IviRangeTable      attrSweepPointsRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrSweepPointsRangeTableEntries,
};

/*- ZUP366_ATTR_SWEEP_OFFSET_POINTS -*/
/*- Shared Read callback zup366ViInt32_ReadCallback -*/
/*- Shared Write callback zup366ViInt32_WriteCallback -*/
/*- Shared Check callback zup366AttrListViInt32_CheckCallback -*/
static IviRangeTableEntry attrSweepOffsetPointsRangeTableEntries[] =
{
    { -524.287, 2000000000, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};
static IviRangeTable      attrSweepOffsetPointsRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrSweepOffsetPointsRangeTableEntries,
};

/*- ZUP366_ATTR_SWEEP_TIME_INTERVAL -*/
/*- Shared Read callback zup366ViInt32_ReadCallback -*/
/*- Shared Write callback zup366ViInt32_WriteCallback -*/
static IviRangeTableEntry attrSweepTimeIntervalRangeTableEntries[] =
{
    { 0.00002048, 40000, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};
static IviRangeTable      attrSweepTimeIntervalRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrSweepTimeIntervalRangeTableEntries
};

static ViStatus _VI_FUNC zup366AttrListViReal64_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_DefaultCheckCallbackViReal64 (vi, channelName, attributeId, value));
    checkErr (zup366_CheckListFunctionality (vi, channelName, IVI_ERROR_ATTRIBUTE_NOT_SUPPORTED));

Error:
    return error;
}

/*- ZUP366_ATTR_MEASUREMENT_WINDOW -*/
/*- Shared Write callback zup366Enum_WriteCallback -*/
/*- Shared Write callback zup366AttrListViInt32_CheckCallback -*/
/*- Shared Read callback zup366Enum_ReadCallback -*/
static IviRangeTableEntry attrMeasurementWindowRangeTableEntries[] =
{
    { ZUP366_VAL_MEASUREMENT_WINDOW_HANNING,     0, 0, "HANN", 0 },
    { ZUP366_VAL_MEASUREMENT_WINDOW_RECTANGULAR, 0, 0, "RECT", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};
static IviRangeTable      attrMeasurementWindowRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrMeasurementWindowRangeTableEntries
};

/*- ZUP366_ATTR_INSTR_MODULE_OPTIONS -*/
static ViStatus _VI_FUNC zup366AttrInstrModuleOptions_ReadCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32         *value)
{
    ViStatus error       = VI_SUCCESS;
    ViInt32  bit         = 0;
    ViChar   buffer[256] = "";
    ViChar   * p2option;
    ViChar   * p2buffer;
    ViInt32  opions      = 0;
    ViInt32  bufRealSize = 0;

    viCheckErr (viQueryf (io, ":SYST:CHAN:OPT? (@%s)",
                          "%256[^\n]",
                          channelName,
                          buffer));

    /*
     * Remove double quotes
     */
    RemoveSurroundingDoubleQuotes (buffer);

    bufRealSize = strlen (buffer);
    p2buffer    = p2option = buffer;
    p2buffer    = strchr (p2buffer, ',');
    if (p2buffer)
        *p2buffer = '\0';
    while (p2option && strlen (p2option) > 0)
    {
        RemoveSurroundingWhiteSpace (p2option);
        if (Ivi_GetValueFromTable (zup366_OptionTable, p2option, &bit) == VI_SUCCESS)
        {
            opions |= (1L << bit);
        }

        /* move the pointer to the next option string*/
        p2option += strlen (p2option) + 1;

        if ((p2option - buffer) >= bufRealSize)
            break;

        p2buffer = p2option;
        p2buffer = strchr (p2buffer, ',');
        if (p2buffer)
            *p2buffer = '\0';
    }

Error:
    *value = opions;
    return error;
}

/*- ZUP366_ATTR_MEASUREMENT_FUNCTION -*/
static ViStatus _VI_FUNC zup366AttrMeasurementfunction_WriteCallback (ViSession vi,
                                                                       ViSession io,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       ViInt32 value)
{
    ViStatus error = VI_SUCCESS;

    viCheckErr (viPrintf (io, ":SENS:FUNC \"%s\",(@%s)",
                          (value == ZUP366_VAL_MEASURE_VOLTAGE) ? "VOLT" : "CURR",
                          channelName));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrMeasurementfunction_ReadCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViInt32         *value)
{
    ViStatus error               = VI_SUCCESS;
    ViChar   buffer[BUFFER_SIZE] = "";
    size_t   buffer_size         = sizeof (buffer);

    viCheckErr (viQueryf (io, ":SENS:FUNC? (@%s)", "%#s",
                          channelName,
                          &buffer_size,
                          buffer));

    RemoveSurroundingDoubleQuotes (buffer);

    viCheckErr (Ivi_GetViInt32EntryFromString (buffer, &measurementTypeRangeTable, value,
                                               VI_NULL, VI_NULL, VI_NULL, VI_NULL));

Error:
    return error;
}


/*- ZUP366_ATTR_OUTPUT_DELAY_FALL -*/
/*- ZUP366_ATTR_OUTPUT_DELAY_RISE -*/
static IviRangeTableEntry attrOutputDelayRangeTableEntries[] =
{
    { 0, 1023, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY },
};
static IviRangeTable      attrOutputDelayRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrOutputDelayRangeTableEntries,
};

static ViStatus _VI_FUNC zup366AttrOutputDelay_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32         *value)
{
    ViStatus error = VI_SUCCESS;
    ViReal64 r64value;

    viCheckErr (viQueryf (io, (attributeId == ZUP366_ATTR_OUTPUT_DELAY_FALL) ?
                          ":OUTP:DEL:FALL? (@%s)" :
                          ":OUTP:DEL:RISE? (@%s)", "%Le",
                          channelName,
                          &r64value));

    *value = (ViInt32) floor (r64value * 1000.0);

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrOutputDelay_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 value)
{
    ViStatus error    = VI_SUCCESS;
    ViReal64 r64value = ((double) value / 1000.0);

    viCheckErr (viPrintf (io, (attributeId == ZUP366_ATTR_OUTPUT_DELAY_FALL) ?
                          ":OUTP:DEL:FALL %Le,(@%s)" :
                          ":OUTP:DEL:RISE %Le,(@%s)",
                          r64value,
                          channelName));
Error:
    return error;
}

/*- ZUP366_ATTR_OUTPUT_INHIBIT_MODE -*/
/*- Shared Write callback zup366ViEnum_WriteCallback -*/
/*- Shared Read callback zup366ViEnum_ReadCallback -*/
static IviRangeTableEntry attrOutputInhibitModeRangeTableEntries[] =
{
    { ZUP366_VAL_OUTPUT_INHIBIT_MODE_OFF,      0, 0, "OFF",  0 },
    { ZUP366_VAL_OUTPUT_INHIBIT_MODE_LIVE,     0, 0, "LIVE", 0 },
    { ZUP366_VAL_OUTPUT_INHIBIT_MODE_LATCHING, 0, 0, "LATC", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY },
};
static IviRangeTable      attrOutputInhibitModeRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrOutputInhibitModeRangeTableEntries,
};

/*- ZUP366_ATTR_DIGITAL_PIN1_FUNCTION -*/
/*- ZUP366_ATTR_DIGITAL_PIN2_FUNCTION -*/
/*- ZUP366_ATTR_DIGITAL_PIN3_FUNCTION -*/
/*- ZUP366_ATTR_DIGITAL_PIN4_FUNCTION -*/
/*- ZUP366_ATTR_DIGITAL_PIN5_FUNCTION -*/
/*- ZUP366_ATTR_DIGITAL_PIN6_FUNCTION -*/
/*- ZUP366_ATTR_DIGITAL_PIN7_FUNCTION -*/
/*- Shared Write callback zup366ViEnum_WriteCallback -*/
/*- Shared Read callback zup366ViEnum_ReadCallback -*/
static IviRangeTableEntry attrDigitalPinFunctionRangeTableEntries[] =
{
    { ZUP366_VAL_DIGITAL_PIN_FUNCTION_DIO,       0, 0, "DIO",  0 },
    { ZUP366_VAL_DIGITAL_PIN_FUNCTION_DINPUT,    0, 0, "DINP", 0 },
    { ZUP366_VAL_DIGITAL_PIN_FUNCTION_TOUTPUT,   0, 0, "TOUT", 0 },
    { ZUP366_VAL_DIGITAL_PIN_FUNCTION_TINPUT,    0, 0, "TINP", 0 },
    { ZUP366_VAL_DIGITAL_PIN_FUNCTION_FAULT,     0, 0, "FAUL", 0 },
    { ZUP366_VAL_DIGITAL_PIN_FUNCTION_INHIBIT,   0, 0, "INH",  0 },
    { ZUP366_VAL_DIGITAL_PIN_FUNCTION_ONCOUPLE,  0, 0, "ONC",  0 },
    { ZUP366_VAL_DIGITAL_PIN_FUNCTION_OFFCOUPLE, 0, 0, "OFFC", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY },
};
static IviRangeTable      attrDigitalPinFunctionRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrDigitalPinFunctionRangeTableEntries,
};

static ViStatus _VI_FUNC zup366AttrOutputInhibitMode_CheckCallback (ViSession vi,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_DefaultCheckCallbackViInt32 (vi, channelName, attributeId, value));

    switch (value)
    {
    case ZUP366_VAL_DIGITAL_PIN_FUNCTION_FAULT:
        if (attributeId != ZUP366_ATTR_DIGITAL_PIN1_FUNCTION)
            error = IVI_ERROR_INVALID_VALUE;
        break;

    case ZUP366_VAL_DIGITAL_PIN_FUNCTION_INHIBIT:
        if (attributeId != ZUP366_ATTR_DIGITAL_PIN3_FUNCTION)
            error = IVI_ERROR_INVALID_VALUE;
        break;

    case ZUP366_VAL_DIGITAL_PIN_FUNCTION_ONCOUPLE:
        if (attributeId == ZUP366_ATTR_DIGITAL_PIN1_FUNCTION || attributeId == ZUP366_ATTR_DIGITAL_PIN2_FUNCTION || attributeId == ZUP366_ATTR_DIGITAL_PIN3_FUNCTION)
            error = IVI_ERROR_INVALID_VALUE;
        break;

    case ZUP366_VAL_DIGITAL_PIN_FUNCTION_OFFCOUPLE:
        if (attributeId == ZUP366_ATTR_DIGITAL_PIN1_FUNCTION || attributeId == ZUP366_ATTR_DIGITAL_PIN2_FUNCTION || attributeId == ZUP366_ATTR_DIGITAL_PIN3_FUNCTION)
            error = IVI_ERROR_INVALID_VALUE;
        break;

    default:
        break;
    }

    viCheckErr (error);


Error:
    return error;
}

/*- ZUP366_ATTR_DIGITAL_PIN1_POLARITY -*/
/*- ZUP366_ATTR_DIGITAL_PIN2_POLARITY -*/
/*- ZUP366_ATTR_DIGITAL_PIN3_POLARITY -*/
/*- ZUP366_ATTR_DIGITAL_PIN4_POLARITY -*/
/*- ZUP366_ATTR_DIGITAL_PIN5_POLARITY -*/
/*- ZUP366_ATTR_DIGITAL_PIN6_POLARITY -*/
/*- ZUP366_ATTR_DIGITAL_PIN7_POLARITY -*/
/*- Shared Write callback zup366ViEnum_WriteCallback -*/
/*- Shared Read callback zup366ViEnum_ReadCallback -*/
static IviRangeTableEntry attrDigitalPinPolarityRangeTableEntries[] =
{
    { ZUP366_VAL_DIGITAL_PIN_POLARITY_POSITIVE, 0, 0, "POS", 0 },
    { ZUP366_VAL_DIGITAL_PIN_POLARITY_NEGATIVE, 0, 0, "NEG", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY },
};
static IviRangeTable      attrDigitalPinPolarityRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrDigitalPinPolarityRangeTableEntries,
};

/*- ZUP366_ATTR_SMU_OUTPUT_REGULATION_PRIORITY -*/
static IviRangeTableEntry attrOutputRegulationRangeTableEntries[] =
{
    { ZUP366_VAL_OUTPUT_REGULATION_VOLTAGE, 0, 0, "VOLT", 0 },
    { ZUP366_VAL_OUTPUT_REGULATION_CURRENT, 0, 0, "CURR", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};
static IviRangeTable      attrOutputRegulationRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrOutputRegulationRangeTableEntries,
};

/*- ZUP366_ATTR_SMU_ENABLE_RESISTANCE -*/
static IviRangeTableEntry attrResistanceRangeTableEntries[] =
{
    { -0.04, 1, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrResistanceRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrResistanceRangeTableEntries,
};

/* - ZUP366_ATTR_SMU_VOLTAGE_BANDWIDTH -*/
static IviRangeTableEntry attrSMUBandwidthRangeTableEntries[] =
{
    { ZUP366_VAL_SMU_LOW_BANDWIDTH,   0, 0, "LOW",   0 },
    { ZUP366_VAL_SMU_HIGH1_BANDWIDTH, 0, 0, "HIGH1", 0 },
    { ZUP366_VAL_SMU_HIGH2_BANDWIDTH, 0, 0, "HIGH2", 0 },
    { ZUP366_VAL_SMU_HIGH3_BANDWIDTH, 0, 0, "HIGH3", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrSMUBandwidthRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrSMUBandwidthRangeTableEntries,
};

/* -  ZUP366_ATTR_SMU_OUTPUT_IMPEDANCE -*/
static IviRangeTableEntry attrSMUOutputImpedanceRangeTableEntries[] =
{
    { ZUP366_VAL_SMU_HIGH_IMPEDANCE, 0, 0, "HIGHZ", 0 },
    { ZUP366_VAL_SMU_LOW_IMPEDANCE,  0, 0, "LOWZ",  0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrSMUOutputImpedanceRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrSMUOutputImpedanceRangeTableEntries,
};

/* - ZUP366_ATTR_TRIGGER_WFM_SRC -*/
static IviRangeTableEntry attrTriggerWFMSourceRangeTableEntries[] =
{
    { ZUP366_VAL_TRIG_WFM_IMMEDIATE, 0, 0, "IMM", 0 },
    { ZUP366_VAL_TRIG_WFM_EXTERNAL,  0, 0, "EXT", 0 },
    { ZUP366_VAL_TRIG_WFM_BUS_SW,    0, 0, "BUS", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrTriggerWFMSourceRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrTriggerWFMSourceRangeTableEntries,
};

/* - ZUP366_ATTR_DATALOG_TRIGGER_SRC -*/
static IviRangeTableEntry attrDatalogTriggerSourceRangeTableEntries[] =
{
    { ZUP366_VAL_TRIG_DATALOG_IMMEDIATE, 0, 0, "IMM",   0 },
    { ZUP366_VAL_TRIG_DATALOG_EXTERNAL,  0, 0, "EXT",   0 },
    { ZUP366_VAL_TRIG_DATALOG_BUS_SW,    0, 0, "BUS",   0 },
    { ZUP366_VAL_TRIG_DATALOG_CURRENT1,  0, 0, "CURR1", 0 },
    { ZUP366_VAL_TRIG_DATALOG_CURRENT2,  0, 0, "CURR2", 0 },
    { ZUP366_VAL_TRIG_DATALOG_CURRENT3,  0, 0, "CURR3", 0 },
    { ZUP366_VAL_TRIG_DATALOG_CURRENT4,  0, 0, "CURR4", 0 },
    { ZUP366_VAL_TRIG_DATALOG_VOLTAGE1,  0, 0, "VOLT1", 0 },
    { ZUP366_VAL_TRIG_DATALOG_VOLTAGE2,  0, 0, "VOLT2", 0 },
    { ZUP366_VAL_TRIG_DATALOG_VOLTAGE3,  0, 0, "VOLT3", 0 },
    { ZUP366_VAL_TRIG_DATALOG_VOLTAGE4,  0, 0, "VOLT4", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrDatalogTriggerSourceRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrDatalogTriggerSourceRangeTableEntries,
};

/* - ZUP366_ATTR_HISTOGRAM_TRIGGER_SRC -*/
static IviRangeTableEntry attrHistogramTriggerSourceRangeTableEntries[] =
{
    { ZUP366_VAL_HISTOGRAM_IMMEDIATE, 0, 0, "IMM",  0 },
    { ZUP366_VAL_HISTOGRAM_EXTERNAL,  0, 0, "EXT",  0 },
    { ZUP366_VAL_HISTOGRAM_BUS_TRIG,  0, 0, "BUS",  0 },
    { ZUP366_VAL_HISTOGRAM_TRIG_PIN1, 0, 0, "PIN1", 0 },
    { ZUP366_VAL_HISTOGRAM_TRIG_PIN2, 0, 0, "PIN2", 0 },
    { ZUP366_VAL_HISTOGRAM_TRIG_PIN3, 0, 0, "PIN3", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrHistogramTriggerSourceRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrHistogramTriggerSourceRangeTableEntries,
};

/* - ZUP366_ATTR_SEQUENCE_MEASUREMENT -*/
static IviRangeTableEntry attrSequenceMeasurementRangeTableEntries[] =
{
    { ZUP366_VAL_SEQ_MEAS_TYPE_VOLTAGE, 0, 0, "VOLT", 0 },
    { ZUP366_VAL_SEQ_MEAS_TYPE_CURRENT, 0, 0, "CURR", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrSequenceMeasurementRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrSequenceMeasurementRangeTableEntries,
};



/********************************************************************/

static ViStatus ag_CreateModuleTable (ViSession vi,
                                      ViInt32 pInstaledModules[MAX_MODULE_COUNT + 1],
                                      SInstrumentRangesPtr** pInstrumentModules)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = NULL;
    ViInt32             i                     = 0;

    checkErr (Ivi_Alloc (vi, (ViInt32) sizeof (SInstrumentRangesPtr) * (MAX_MODULE_COUNT + 1), (ViAddr *) &pinstrument_modules));
    memset (pinstrument_modules, 0, sizeof (SInstrumentRangesPtr) * (MAX_MODULE_COUNT + 1));

    for (i = 1; i <= MAX_MODULE_COUNT; i++)
    {
        switch (pInstaledModules[i])
        {
        case ZUP366:
            pinstrument_modules[i] = &gs_zup366Ranges;
            break;

        case N6732A:
            pinstrument_modules[i] = &gs_N6732Ranges;
            break;

        case N6733A:
            pinstrument_modules[i] = &gs_N6733Ranges;
            break;

        case N6734A:
            pinstrument_modules[i] = &gs_N6734Ranges;
            break;

        case N6735A:
            pinstrument_modules[i] = &gs_N6735Ranges;
            break;

        case N6742A:
            pinstrument_modules[i] = &gs_N6742Ranges;
            break;

        case N6743A:
            pinstrument_modules[i] = &gs_N6743Ranges;
            break;

        case N6744A:
            pinstrument_modules[i] = &gs_N6744Ranges;
            break;

        case N6745A:
            pinstrument_modules[i] = &gs_N6745Ranges;
            break;

        case N6751A:
            pinstrument_modules[i] = &gs_N6751Ranges;
            break;

        case N6752A:
            pinstrument_modules[i] = &gs_N6752Ranges;
            break;

        case N6753A:
            pinstrument_modules[i] = &gs_N6753Ranges;
            break;

        case N6754A:
            pinstrument_modules[i] = &gs_N6754Ranges;
            break;

        case N6755A:
            pinstrument_modules[i] = &gs_N6755Ranges;
            break;

        case N6756A:
            pinstrument_modules[i] = &gs_N6756Ranges;
            break;

        case N6761A:
            pinstrument_modules[i] = &gs_N6761Ranges;
            break;

        case N6762A:
            pinstrument_modules[i] = &gs_N6762Ranges;
            break;

        case N6763A:
            pinstrument_modules[i] = &gs_N6763Ranges;
            break;

        case N6764A:
            pinstrument_modules[i] = &gs_N6764Ranges;
            break;

        case N6765A:
            pinstrument_modules[i] = &gs_N6765Ranges;
            break;

        case N6766A:
            pinstrument_modules[i] = &gs_N6766Ranges;
            break;

        case N6773A:
            pinstrument_modules[i] = &gs_N6773Ranges;
            break;

        case N6774A:
            pinstrument_modules[i] = &gs_N6774Ranges;
            break;

        case N6775A:
            pinstrument_modules[i] = &gs_N6775Ranges;
            break;

        case N6776A:
            pinstrument_modules[i] = &gs_N6776Ranges;
            break;

        case N6777A:
            pinstrument_modules[i] = &gs_N6777Ranges;
            break;

        case N6781A:
            pinstrument_modules[i] = &gs_N6781Ranges;
            break;

        case N6782A:
            pinstrument_modules[i] = &gs_N6782Ranges;
            break;

        case N6783A:
            pinstrument_modules[i] = &gs_N6783BATRanges;
            break;

        case N6784A:
            pinstrument_modules[i] = &gs_N6784Ranges;
            break;

        case N6785A:
            pinstrument_modules[i] = &gs_N6785Ranges;
            break;

        case N6786A:
            pinstrument_modules[i] = &gs_N6786Ranges;
            break;


        case N6731B:
            pinstrument_modules[i] = &gs_N6731BRanges;
            break;

        case N6732B:
            pinstrument_modules[i] = &gs_N6732BRanges;
            break;

        case N6733B:
            pinstrument_modules[i] = &gs_N6733BRanges;
            break;

        case N6734B:
            pinstrument_modules[i] = &gs_N6734BRanges;
            break;

        case N6735B:
            pinstrument_modules[i] = &gs_N6735BRanges;
            break;

        case N6736B:
            pinstrument_modules[i] = &gs_N6736BRanges;
            break;

        case N6741B:
            pinstrument_modules[i] = &gs_N6741BRanges;
            break;

        case N6742B:
            pinstrument_modules[i] = &gs_N6742BRanges;
            break;

        case N6743B:
            pinstrument_modules[i] = &gs_N6743BRanges;
            break;

        case N6744B:
            pinstrument_modules[i] = &gs_N6744BRanges;
            break;

        case N6745B:
            pinstrument_modules[i] = &gs_N6745BRanges;
            break;
        }
    }

Error:
    *pInstrumentModules = pinstrument_modules;
    return error;
}

/***Configure Waveform Parameters function callbacks***/
/*- ZUP366_ATTR_WAVEFORM_COUNT -*/
static IviRangeTableEntry attrWaveformCountRangeTableEntries[] =
{
    { 1, 16777216, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrWaveformCountRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrWaveformCountRangeTableEntries,
};

static IviRangeTableEntry attrWaveformCountCDRangeTableEntries[] =
{
    { 1, 256, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrWaveformCountCDRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrWaveformCountCDRangeTableEntries,
};

static ViStatus _VI_FUNC zup366AttrWaveformCount_RangeTableCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      IviRangeTablePtr *rangeTablePtr)
{
    ViStatus         error  = VI_SUCCESS;
    IviRangeTablePtr tblPtr = VI_NULL;

    ViInt32          wfmShape;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &wfmShape));

    if (wfmShape != ZUP366_VAL_WAVEFORM_CONSTANTDWELL)
        tblPtr = &attrWaveformCountRangeTable;
    else
        tblPtr = &attrWaveformCountCDRangeTable;

Error:
    *rangeTablePtr = tblPtr;
    return error;
}

static ViStatus _VI_FUNC zup366AttrWaveformCount_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 *value)
{
    ViStatus error       = VI_SUCCESS;
    ViInt32  repeatCount = 0;

    if (!Ivi_Simulating (vi))
        viCheckErr (viQueryf (io, ":SOUR:ARB:COUN? (@%s)", "%d", channelName, &repeatCount));

    if (repeatCount <= 9.9e37)
        *value = repeatCount;
    else
        *value = 0;

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrWaveformCount_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  max   = 16777216;

    if (!Ivi_Simulating (vi))
    {
        if (value < 0 || value > max)
            viCheckErr (viPrintf (io, ":SOUR:ARB:COUN INF,(@%s)", channelName));
        else
            viCheckErr (viPrintf (io, ":SOUR:ARB:COUN %d,(@%s)", value, channelName));
    }

Error:
    return error;
}

/***Configure Sequence Parameters function Callbacks***/
/* - ZUP366_ATTR_SEQUENCE_STEP_NUMBER-*/
static IviRangeTableEntry attrSequenceStepNumberRangeTableEntries[] =
{
    { 0, 100, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrSequenceStepNumberRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrSequenceStepNumberRangeTableEntries,
};



/*- ZUP366_ATTR_SEQUENCE_SHAPE -*/
static IviRangeTableEntry attrSequenceShapeRangeTableEntries[] =
{
    { ZUP366_VAL_SEQUENCE_EXPONENTIAL, 0, 0, "EXP",  0 },
    { ZUP366_VAL_SEQUENCE_PULSE,       0, 0, "PULS", 0 },
    { ZUP366_VAL_SEQUENCE_RAMP,        0, 0, "RAMP", 0 },
    { ZUP366_VAL_SEQUENCE_SINSUOID,    0, 0, "SIN",  0 },
    { ZUP366_VAL_SEQUENCE_STAIRCASE,   0, 0, "STA",  0 },
    { ZUP366_VAL_SEQUENCE_STEP,        0, 0, "STEP", 0 },
    { ZUP366_VAL_SEQUENCE_TRAPEZOID,   0, 0, "TRAP", 0 },
    { ZUP366_VAL_SEQUENCE_UDEFINED,    0, 0, "UDEF", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrSequenceShapeRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrSequenceShapeRangeTableEntries,
};

static ViStatus _VI_FUNC zup366AttrSequenceShape_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 *value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureShapeValue, measureStepValue;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, &measureStepValue));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:FUNC:SHAP? %d, (@%s)", "%s", measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrSequenceShape_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureShapeValue, measureStepValue;
    ViString seqShapeType;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, &measureStepValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (value, &attrSequenceShapeRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &seqShapeType, VI_NULL));


    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:FUNC:SHAP %s, %d, (@%s)", seqShapeType, measureStepValue, channelName));
    }

Error:
    return error;
}

/*-ZUP366_ATTR_SEQUENCE_PACING-*/
static IviRangeTableEntry attrSequencePacingRangeTableEntries[] =
{
    { ZUP366_VAL_SEQUENCE_PACING_TYPE_DWELL,     0, 0, "DWEL", 0 },
    { ZUP366_VAL_SEQUENCE_PACING_TYPE_TRIGGERED, 0, 0, "TRIG", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrSequencePacingRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrSequencePacingRangeTableEntries,
};

static ViStatus _VI_FUNC zup366AttrSequencePacing_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 *value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureShapeValue, measureStepValue;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, &measureStepValue));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:PAC? %d, (@%s)", "%s", measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrSequencePacing_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViInt32 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureShapeValue, measureStepValue;
    ViString seqPacingType;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, &measureStepValue));

    viCheckErr (Ivi_GetViInt32EntryFromValue (value, &attrSequencePacingRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &seqPacingType, VI_NULL));


    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:PAC %s, %d, (@%s)", seqPacingType, measureStepValue, channelName));
    }

Error:
    return error;
}

/*ZUP366_ATTR_SEQUENCE_STEP_COUNT*/
static IviRangeTableEntry attrSequenceStepCountRangeTableEntries[] =
{
    { 1, 16777216, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrSequenceStepCountRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrSequenceStepCountRangeTableEntries,
};

static ViStatus _VI_FUNC zup366AttrSequenceStepCount_ReadCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViInt32 *value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureShapeValue, measureStepValue;
    ViInt32  xSeqStepCnt = 0;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, &measureStepValue));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:COUN? (@%s)", "%d", channelName, measureStepValue, &xSeqStepCnt));
    }

    if (xSeqStepCnt <= 9.9e37)
        *value = xSeqStepCnt;
    else
        *value = 0;

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrSequenceStepCount_WriteCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViInt32 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureShapeValue, measureStepValue;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, &measureStepValue));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE)
        {
            if (value < 0)
                viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:COUN INF, %d, (@%s)", measureStepValue, channelName));
            else
                viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:COUN %d, %d, (@%s)", value, measureStepValue, channelName));
        }
    }

Error:
    return error;
}

/* - ZUP366_ATTR_SEQUENCE_COUNT - */
static IviRangeTableEntry attrSequenceCountRangeTableEntries[] =
{
    { 1, 4096, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrSequenceCountRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrSequenceCountRangeTableEntries,
};


static ViStatus _VI_FUNC zup366AttrSequenceCount_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViInt32 *value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureShapeValue, xSeqCnt = 0;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));
    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:COUN? (@%s)", "%d", channelName, &xSeqCnt));
    }

    if (xSeqCnt <= 9.9e37)
        *value = xSeqCnt;
    else
        *value = 0;

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrSequenceCount_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureShapeValue;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE)
        {
            if (value < 0)
                viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:COUN INF, (@%s)", channelName));
            else
                viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:COUN %d, (@%s)", value, channelName));
        }
    }

Error:
    return error;
}

/* Common CheckCallback for Start Level, End Level, Top Level, Amplitude, and Offset */
static ViStatus _VI_FUNC zup366CommonCheckCallback (ViSession vi, ViConstString channelName,
                                                     ViAttr attributeId, ViReal64 value)

{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViInt32             channel_index         = 0, measureType, measureSeqType;
    ViReal64            maximum, minimum;

    checkErr (Ivi_GetAttributeViAddr (vi, VI_NULL, ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES, 0, (ViAddr *) &pinstrument_modules));
    channel_index = (channelName[0] - '0');

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_MEASUREMENT, 0, &measureType));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_MEASUREMENT, 0, &measureSeqType));

    if (measureType == ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE || measureSeqType == ZUP366_VAL_SEQ_MEAS_TYPE_VOLTAGE)
    {
        minimum = pinstrument_modules[channel_index]->MinVoltage[MAX_NB_RANGES - 2];
        maximum = pinstrument_modules[channel_index]->MaxVoltage[MAX_NB_RANGES - 1];
    }
    else
    {
        minimum = pinstrument_modules[channel_index]->MinCurrent[MAX_NB_RANGES - 2];
        maximum = pinstrument_modules[channel_index]->MaxCurrent[MAX_NB_RANGES - 1];
    }

    if (value < minimum || value > maximum)
        viCheckErr (IVI_ERROR_INVALID_VALUE);
Error:
    return error;
}

/***Configure Exponential function Callbacks***/
/* - ZUP366_ATTR_EXP_START_LEVEL - */
static ViStatus _VI_FUNC zup366AttrExpStartLevel_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366CommonCheckCallback (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366WaveformSequenceCheck (ViSession vi, ViConstString channelName, ViAttr attributeId,
                                                       ViReal64 *value, ViInt32 measureShapeValue, ViInt32 measureSeqShapeValue,
                                                       ViString measureType, ViString measureSeqType)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureTypeValue, measureSeqTypeValue, measureStepValue;

    /*Waveform*/
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_MEASUREMENT, 0, &measureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (measureTypeValue, &attrWaveformMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &measureType, VI_NULL));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));

    /*Sequence*/
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_SHAPE, 0, &measureSeqShapeValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_MEASUREMENT, 0, &measureSeqTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (measureSeqTypeValue, &attrSequenceMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &measureSeqType, VI_NULL));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, &measureStepValue));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrExpStartLevel_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_EXPONENTIAL)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:EXP:STAR? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_EXPONENTIAL)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:EXP:STAR? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrExpStartLevel_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureTypeValue, measureShapeValue, measureSeqTypeValue, measureSeqShapeValue, measureStepValue;
    ViString measureType, measureSeqType;

    /*Waveform*/
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_MEASUREMENT, 0, &measureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (measureTypeValue, &attrWaveformMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &measureType, VI_NULL));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));

    /*Sequence*/
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_SHAPE, 0, &measureSeqShapeValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_MEASUREMENT, 0, &measureSeqTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (measureSeqTypeValue, &attrSequenceMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &measureSeqType, VI_NULL));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, &measureStepValue));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_EXPONENTIAL)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:EXP:STAR %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_EXPONENTIAL)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:EXP:STAR %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - ZUP366_ATTR_EXP_END_LEVEL - */
static ViStatus _VI_FUNC zup366AttrExpEndLevel_CheckCallback (ViSession vi,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366CommonCheckCallback (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrExpEndLevel_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_EXPONENTIAL)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:EXP:END? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_EXPONENTIAL)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:EXP:END? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrExpEndLevel_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_EXPONENTIAL)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:EXP:END %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_EXPONENTIAL)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:EXP:END %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - attrStartTime Range Table shared with Exponential, Pulse, Ramp, Stair, Step, and Trapezoid Start Time Callbacks- */
static IviRangeTableEntry attrStartTimeRangeTableEntries[] =
{
    { 0, 262.144, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrStartTimeRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrStartTimeRangeTableEntries,
};

/* - ZUP366_ATTR_EXP_START_TIME - */
static ViStatus _VI_FUNC zup366AttrExpStartTime_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_EXPONENTIAL)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:EXP:STAR:TIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_EXPONENTIAL)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:EXP:STAR:TIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrExpStartTime_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_EXPONENTIAL)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:EXP:STAR:TIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_EXPONENTIAL)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:EXP:STAR:TIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - ZUP366_ATTR_EXPONENTIAL_TIME_CONSTANT - */
static IviRangeTableEntry attrExponentialTimeConstantRangeTableEntries[] =
{
    { 1, 262.144, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrExponentialTimeConstantRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrExponentialTimeConstantRangeTableEntries,
};


static ViStatus _VI_FUNC zup366AttrExpTimeConstant_ReadCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";


    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_EXPONENTIAL)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:EXP:TCON? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_EXPONENTIAL)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:EXP:TCON? %d, (@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrExpTimeConstant_WriteCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_EXPONENTIAL)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:EXP:TCON %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_EXPONENTIAL)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:EXP:TIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - attrTotalTime Range Table shared with Exponential and Stair Total Time Callbacks- */
static IviRangeTableEntry attrTotalTimeRangeTableEntries[] =
{
    { 1, 262.144, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrTotalTimeRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrTotalTimeRangeTableEntries,
};

/* - ZUP366_ATTR_EXP_TOTAL_TIME -*/

static ViStatus _VI_FUNC zup366AttrExpTotalTime_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));


    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_EXPONENTIAL)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:EXP:TIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_EXPONENTIAL)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:EXP:TIM? %d, (@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrExpTotalTime_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_EXPONENTIAL)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:EXP:TIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_EXPONENTIAL)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:EXP:TIM %Le, %d, (@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/***Configure Pulse function Callbacks***/
/*- ZUP366_ATTR_PULS_START_LEVEL -*/
static ViStatus _VI_FUNC zup366AttrPulsStartLevel_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366CommonCheckCallback (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrPulsStartLevel_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_PULSE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:PULS:STAR? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_PULSE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:PULS:STAR? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrPulsStartLevel_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_PULSE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:PULS:STAR %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_PULSE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:PULS:STAR %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/*- ZUP366_ATTR_PULS_AMPL_TLEV -*/
static ViStatus _VI_FUNC zup366AttrPulsAmplTlev_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366CommonCheckCallback (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrPulsAmplTlev_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_PULSE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:PULS:TOP? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_PULSE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:PULS:TOP? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrPulsAmplTlev_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_PULSE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:PULS:TOP %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_PULSE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:PULS:TOP %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - attrStartTime Range Table shared with Exponential, Pulse, Ramp, Stair, Step, and Trapezoid Start Time Callbacks- */
/*- ZUP366_ATTR_PULS_START_TIME -*/

static ViStatus _VI_FUNC zup366AttrPulsStartTime_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_PULSE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:PULS:STAR:TIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_PULSE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:PULS:STAR:TIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrPulsStartTime_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_PULSE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:PULS:STAR:TIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_PULSE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:PULS:STAR:TIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - attrWidth Range Table shared with Pulse and Trapezoid Width Time Callbacks- */
static IviRangeTableEntry attrWidthRangeTableEntries[] =
{
    { 1, 262.144, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrWidthRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrWidthRangeTableEntries,
};

/*- ZUP366_ATTR_PULS_WIDTH_TTIME -*/

static ViStatus _VI_FUNC zup366AttrPulsWidthTtime_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_PULSE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:PULS:TOP:TIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_PULSE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:PULS:TOP:TIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrPulsWidthTtime_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_PULSE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:PULS:TOP:TIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_PULSE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:PULS:TOP:TIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - attrEndTime Range Table shared with Pulse, Ramp, Stair, Step, and Trapezoid End Time Callbacks- */
static IviRangeTableEntry attrEndTimeRangeTableEntries[] =
{
    { 0, 262.144, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrEndTimeRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrEndTimeRangeTableEntries,
};

/* - ZUP366_ATTR_PULS_END_TIME -*/

static ViStatus _VI_FUNC zup366AttrPulsEndTime_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_PULSE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:PULS:END:TIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_PULSE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:PULS:END:TIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrPulsEndTime_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));
    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_PULSE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:PULS:END:TIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_PULSE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:PULS:END:TIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/***Configure Ramp function Callbacks***/
/*- ZUP366_ATTR_RAMP_START_LEVEL - */
static ViStatus _VI_FUNC zup366AttrRampStartLevel_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366CommonCheckCallback (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrRampStartLevel_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_RAMP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:RAMP:STAR? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_RAMP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:RAMP:STAR? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrRampStartLevel_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));
    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_RAMP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:RAMP:STAR %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_RAMP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:RAMP:STAR %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - ZUP366_ATTR_RAMP_END_LEVEL -*/
static ViStatus _VI_FUNC zup366AttrRampEndLevel_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366CommonCheckCallback (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrRampEndLevel_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));
    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_RAMP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:RAMP:END? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_RAMP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:RAMP:END? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrRampEndLevel_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));
    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_RAMP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:RAMP:END %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_RAMP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:RAMP:END %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - attrStartTime Range Table shared with Exponential, Pulse, Ramp, Stair, Step, and Trapezoid Start Time Callbacks- */

static ViStatus _VI_FUNC zup366AttrRampStartTime_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_RAMP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:RAMP:STAR:TIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_RAMP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:RAMP:STAR:TIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrRampStartTime_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";


    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_RAMP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:RAMP:STAR:TIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_RAMP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:RAMP:STAR:TIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - attrEndTime Range Table shared with Pulse, Ramp, Stair, Step, and Trapezoid End Time Callbacks- */
/* - ZUP366_ATTR_RAMP_END_TIME - */
static ViStatus _VI_FUNC zup366AttrRampEndTime_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_RAMP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:RAMP:END:TIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_RAMP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:RAMP:END:TIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrRampEndTime_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_RAMP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:RAMP:END:TIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_RAMP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:RAMP:END:TIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }


Error:
    return error;
}

/* - attrRampRiseTime Range Table shared with Ramp and Trapezoid Rise Time Callbacks- */
static IviRangeTableEntry attrRiseTimeRangeTableEntries[] =
{
    { 1, 262.144, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrRiseTimeRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrRiseTimeRangeTableEntries,
};

/* - ZUP366_ATTR_RAMP_RISE_TIME - */
static ViStatus _VI_FUNC zup366AttrRampRiseTime_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_PULSE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:RAMP:RTIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_RAMP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:RAMP:RTIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }


Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrRampRiseTime_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_RAMP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:RAMP:RTIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_RAMP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:RAMP:RTIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/***ConfigureSinusoid Function Callbacks***/
/* - ZUP366_ATTR_AMPLITUDE - */
static ViStatus _VI_FUNC zup366AttrAmplitude_CheckCallback (ViSession vi,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366CommonCheckCallback (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrAmplitude_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_SINSUOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:SIN:AMPL? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_SINSUOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:SIN:AMPL? %d, (@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrAmplitude_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_SINSUOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:SIN:AMPL %Le, (@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_SINSUOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:SIN:AMPL %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }


Error:
    return error;
}

/* - ZUP366_ATTR_FREQUENCY - */
static IviRangeTableEntry attrFrequencyRangeTableEntries[] =
{
    { 3.8147E-5, 1.0E4, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrFrequencyRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrFrequencyRangeTableEntries,
};

static ViStatus _VI_FUNC zup366AttrFrequency_ReadCallback (ViSession vi,
                                                            ViSession io,
                                                            ViConstString channelName,
                                                            ViAttr attributeId,
                                                            ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_SINSUOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:SIN:FREQ? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_SINSUOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:SIN:FREQ? %d, (@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrFrequency_WriteCallback (ViSession vi,
                                                             ViSession io,
                                                             ViConstString channelName,
                                                             ViAttr attributeId,
                                                             ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_SINSUOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:SIN:FREQ %Le, (@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_SINSUOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:SIN:FREQ %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* -  ZUP366_ATTR_OFFSET - */
static ViStatus _VI_FUNC zup366AttrOffset_CheckCallback (ViSession vi,
                                                          ViConstString channelName,
                                                          ViAttr attributeId,
                                                          ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366CommonCheckCallback (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrOffset_ReadCallback (ViSession vi,
                                                         ViSession io,
                                                         ViConstString channelName,
                                                         ViAttr attributeId,
                                                         ViReal64 *value)
{
    ViStatus error            = VI_SUCCESS;
    ViInt32  measureTypeValue = 0, measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0, model_type;
    ViString measureType      = "", measureSeqType = "";

    ViReal64 ampValue;

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    /* For only specific modules (N6784-Voltage) or (N678xA-Current) can output negative voltages/currents
     * other modules output (Voltage or Current) can not be less than the amplitude of the sine wave */
    checkErr (zup366_GetModuleType (vi, channelName, &model_type));
    viCheckErr (Ivi_GetAttributeViReal64 (vi, channelName, ZUP366_ATTR_AMPLITUDE, 0, &ampValue));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_SINSUOID && measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE && model_type == N6784A)
            viCheckErr (viQueryf (io, ":SOUR:ARB:VOLT:SIN:OFFS? (@%s)", "%Le", channelName, &value));                                                   /*voltage - waveform - n6784*/
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SINSUOID && measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE && model_type != N6784A)
            viCheckErr (viQueryf (io, ":SOUR:ARB:VOLT:SIN:OFFS? (@%s)", "%Le", channelName, &ampValue));                                                /*voltage - waveform !n6784*/
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SINSUOID && measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_CURRENT &&
                 model_type == N6781A || model_type == N6782A || model_type == N6784A)
            viCheckErr (viQueryf (io, ":SOUR:ARB:CURR:SIN:OFFS? (@%s)", "%Le", channelName, &value));                                                   /*current - waveform - SMU*/
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SINSUOID && measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_CURRENT &&
                 model_type != N6781A || model_type != N6782A || model_type != N6784A)
            viCheckErr (viQueryf (io, ":SOUR:ARB:CURR:SIN:OFFS? (@%s)", "%Le", channelName, &ampValue));                                                /*current - waveform - !SMU*/
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_SINSUOID &&
                 measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE && model_type == N6784A)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:VOLT:SIN:OFFS? %d, (@%s)", "%Le", measureStepValue, channelName, &value));                    /*voltage - sequence - n6784*/
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_SINSUOID &&
                 measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE && model_type != N6784A)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:VOLT:SIN:OFFS? %d, (@%s)", "%Le", measureStepValue, channelName, &ampValue));                 /*voltage - sequence - !n6784*/
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_SINSUOID &&
                 measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_CURRENT && model_type == N6781A || model_type == N6782A || model_type == N6784A)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:CURR:SIN:OFFS? %d, (@%s)", "%Le", measureStepValue, channelName, &value));                    /*current - sequence - SMU*/
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_SINSUOID &&
                 measureTypeValue == ZUP366_VAL_WFM_MEAS_TYPE_CURRENT && model_type != N6781A || model_type != N6782A || model_type != N6784A)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:CURR:SIN:OFFS? %d, (@%s)", "%Le", measureStepValue, channelName, &ampValue));                 /*current - sequence - !SMU*/
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrOffset_WriteCallback (ViSession vi,
                                                          ViSession io,
                                                          ViConstString channelName,
                                                          ViAttr attributeId,
                                                          ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_SINSUOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:SIN:OFFS %Le, (@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_SINSUOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:SIN:OFFS %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/*** Configure Staircase fucntion Callbacks ***/
/* - ZUP366_ATTR_STAIR_START_LEVEL - */
static ViStatus _VI_FUNC zup366AttrStairStartLevel_CheckCallback (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366CommonCheckCallback (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrStairStartLevel_ReadCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STAIRCASE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:STA:STAR? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STAIRCASE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:STA:STAR? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrStairStartLevel_WriteCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STAIRCASE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:STA:STAR %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STAIRCASE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:STA:STAR %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - ZUP366_ATTR_STAIR_END_LEVEL - */
static ViStatus _VI_FUNC zup366AttrStairEndLevel_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366CommonCheckCallback (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrStairEndLevel_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));
    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STAIRCASE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:STA:END? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STAIRCASE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:STA:END? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrStairEndLevel_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STAIRCASE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:STA:END %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STAIRCASE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:STA:END %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - attrStartTime Range Table shared with Exponential, Pulse, Ramp, Stair, Step, and Trapezoid Start Time Callbacks- */
/* - ZUP366_ATTR_STAIR_START_TIME -*/
static ViStatus _VI_FUNC zup366AttrStairStartTime_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));
    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STAIRCASE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:STA:STAR:TIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STAIRCASE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:STA:STAR:TIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrStairStartTime_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));
    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STAIRCASE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:STA:STAR:TIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STAIRCASE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:STA:STAR:TIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - attrEndTime Range Table shared with Pulse, Ramp, Stair, Step, and Trapezoid End Time Callbacks- */
/* - ZUP366_ATTR_STAIR_END_TIME -*/
static ViStatus _VI_FUNC zup366AttrStairEndTime_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));
    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STAIRCASE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:STA:END:TIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STAIRCASE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:STA:END:TIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrStairEndTime_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));
    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STAIRCASE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:STA:END:TIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STAIRCASE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:STA:END:TIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }


Error:
    return error;
}

/* - attrTotalTime Range Table shared with Exponential and Stair Total Time Callbacks- */
/* - ZUP366_ATTR_STAIR_TOTAL_TIME -*/
static ViStatus _VI_FUNC zup366AttrStairTotalTime_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STAIRCASE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:STA:TIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STAIRCASE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:STA:TIM? %d, (@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrStairTotalTime_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STAIRCASE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:STA:TIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STAIRCASE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:STA:TIM %Le, %d, (@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - ZUP366_ATTR_STAIR_NSTEPS - */
static IviRangeTableEntry attrStairNstepsRangeTableEntries[] =
{
    { 1, 500, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrStairNstepsRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrStairNstepsRangeTableEntries,
};


static ViStatus _VI_FUNC zup366AttrStairNsteps_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViInt32 *value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureTypeValue, measureShapeValue, measureSeqTypeValue, measureSeqShapeValue, measureStepValue;
    ViString measureType, measureSeqType;

    /*Waveform*/
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_MEASUREMENT, 0, &measureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (measureTypeValue, &attrWaveformMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &measureType, VI_NULL));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));

    /*Sequence*/
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_MEASUREMENT, 0, &measureSeqTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (measureSeqTypeValue, &attrSequenceMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &measureSeqType, VI_NULL));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_SHAPE, 0, &measureSeqShapeValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, &measureStepValue));


    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STAIRCASE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:STA:NST? (@%s)", "%d", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STAIRCASE)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:STA:NST? %d, (@%s)", "%d", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrStairNsteps_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViInt32 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  measureTypeValue, measureShapeValue, measureSeqTypeValue, measureSeqShapeValue, measureStepValue;
    ViString measureType, measureSeqType;

    /*Waveform*/
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_MEASUREMENT, 0, &measureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (measureTypeValue, &attrWaveformMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &measureType, VI_NULL));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_SHAPE, 0, &measureShapeValue));

    /*Sequence*/
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_MEASUREMENT, 0, &measureSeqTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (measureSeqTypeValue, &attrSequenceMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &measureSeqType, VI_NULL));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_SHAPE, 0, &measureSeqShapeValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_STEP_NUMBER, 0, &measureStepValue));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STAIRCASE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:STA:NST %d,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STAIRCASE)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:STA:NST %d, %d, (@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/***Configure Step function Callbacks ***/
/* - ZUP366_ATTR_STEP_START_LEVEL - */
static ViStatus _VI_FUNC zup366AttrStepStartLevel_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366CommonCheckCallback (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrStepStartLevel_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STEP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:STEP:STAR? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STEP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:STEP:STAR? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrStepStartLevel_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STEP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:STEP:STAR %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STEP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:STEP:STAR %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - ZUP366_ATTR_STEP_END_LEVEL - */
static ViStatus _VI_FUNC zup366AttrStepEndLevel_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366CommonCheckCallback (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrStepEndLevel_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STEP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:STEP:END? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STEP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:STEP:END? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrStepEndLevel_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STEP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:STEP:END %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STEP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:STEP:END %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - attrStartTime Range Table shared with Exponential, Pulse, Ramp, Stair, Step, and Trapezoid Start Time Callbacks- */
/* - ZUP366_ATTR_STEP_START_TIME - */

static ViStatus _VI_FUNC zup366AttrStepStartTime_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STEP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:STEP:STAR:TIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STEP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:STEP:STAR:TIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrStepStartTime_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));
    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STEP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:STEP:STAR:TIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STEP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:STEP:STAR:TIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - attrEndTime Range Table shared with Pulse, Ramp, Stair, Step, and Trapezoid End Time Callbacks- */
/* - ZUP366_ATTR_STEP_END_TIME - */

static ViStatus _VI_FUNC zup366AttrStepEndTime_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));
    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STEP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:STEP:END:TIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STEP)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:STEP:END:TIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrStepEndTime_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_STEP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:STEP:END:TIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_STEP)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:STEP:END:TIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/***Configure Trapezoid function Callbacks ***/
/* - ZUP366_ATTR_TRAP_START_LEVEL - */
static ViStatus _VI_FUNC zup366AttrTrapStartLevel_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366CommonCheckCallback (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrTrapStartLevel_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));
    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_TRAPEZOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:TRAP:STAR? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_TRAPEZOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:TRAP:STAR? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrTrapStartLevel_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));
    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_TRAPEZOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:TRAP:STAR %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_TRAPEZOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:TRAP:STAR %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/*- ZUP366_ATTR_TRAP_PEAK_TLEV - */
static ViStatus _VI_FUNC zup366AttrTrapPeakTlev_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (zup366CommonCheckCallback (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrTrapPeakTlev_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_TRAPEZOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:TRAP:TOP? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_TRAPEZOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:TRAP:TOP? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrTrapPeakTlev_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_TRAPEZOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:TRAP:TOP %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_TRAPEZOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:TRAP:TOP %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }


Error:
    return error;
}

/* - attrStartTime Range Table shared with Exponential, Pulse, Ramp, Stair, Step, and Trapezoid Start Time Callbacks- */
/* - ZUP366_ATTR_TRAP_START_TIME - */
static ViStatus _VI_FUNC zup366AttrTrapStartTime_ReadCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_TRAPEZOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:TRAP:STAR:TIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_TRAPEZOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:TRAP:STAR:TIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrTrapStartTime_WriteCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_TRAPEZOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:TRAP:STAR:TIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_TRAPEZOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:TRAP:STAR:TIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - attrEndTime Range Table shared with Pulse, Ramp, Stair, Step, and Trapezoid End Time Callbacks- */
/* - ZUP366_ATTR_TRAP_END_TIME - */
static ViStatus _VI_FUNC zup366AttrTrapEndTime_ReadCallback (ViSession vi,
                                                              ViSession io,
                                                              ViConstString channelName,
                                                              ViAttr attributeId,
                                                              ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_TRAPEZOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:TRAP:END:TIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_TRAPEZOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:TRAP:END:TIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrTrapEndTime_WriteCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_TRAPEZOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:TRAP:END:TIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_TRAPEZOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:TRAP:END:TIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }


Error:
    return error;
}

/* - attrWidth Range Table shared with Pulse and Trapezoid Width Time Callbacks- */
/* - ZUP366_ATTR_TRAP_WIDTH_TTIME - */
static ViStatus _VI_FUNC zup366AttrTrapWidthTtime_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_TRAPEZOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:TRAP:TOP:TIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_TRAPEZOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:TRAP:TOP:TIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrTrapWidthTtime_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_TRAPEZOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:TRAP:TOP:TIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_TRAPEZOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:TRAP:TOP:TIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - ZUP366_ATTR_TRAP_FALL_TIME - */
static IviRangeTableEntry attrTrapezoidFallTimeRangeTableEntries[] =
{
    { 0, 262.144, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrTrapezoidFallTimeRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrTrapezoidFallTimeRangeTableEntries,
};


static ViStatus _VI_FUNC zup366AttrTrapFallTime_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_TRAPEZOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:TRAP:FTIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_TRAPEZOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:TRAP:FTIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrTrapFallTime_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_TRAPEZOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:TRAP:FTIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_TRAPEZOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:TRAP:FTIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - attrRampRiseTime Range Table shared with Ramp and Trapezoid Rise Time Callbacks- */
/* - ZUP366_ATTR_TRAP_RISE_TIME - */
static ViStatus _VI_FUNC zup366AttrTrapRiseTime_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_TRAPEZOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:%s:TRAP:RTIM? (@%s)", "%Le", measureType, channelName, &value));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_TRAPEZOID)
            viCheckErr (viQueryf (io, ":SOUR:ARB:SEQ:STEP:%s:TRAP:RTIM? %d,(@%s)", "%Le", measureSeqType, measureStepValue, channelName, &value));
    }


Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrTrapRiseTime_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error             = VI_SUCCESS;
    ViInt32  measureShapeValue = 0, measureSeqShapeValue = 0, measureStepValue = 0;
    ViString measureType       = "", measureSeqType = "";

    viCheckErr (zup366WaveformSequenceCheck (vi, channelName, attributeId, &value, measureShapeValue, measureSeqShapeValue, measureType, measureSeqType));

    if (!Ivi_Simulating (vi))
    {
        if (measureShapeValue == ZUP366_VAL_WAVEFORM_TRAPEZOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:%s:TRAP:RTIM %Le,(@%s)", measureType, value, channelName));
        else if (measureShapeValue == ZUP366_VAL_WAVEFORM_SEQUENCE && measureSeqShapeValue == ZUP366_VAL_SEQUENCE_TRAPEZOID)
            viCheckErr (viPrintf (io, ":SOUR:ARB:SEQ:STEP:%s:TRAP:RTIM %Le, %d,(@%s)", measureSeqType, value, measureStepValue, channelName));
    }

Error:
    return error;
}

/* - ZUP366_ATTR_DATALOG_ENABLED - */
static ViStatus _VI_FUNC zup366AttrDatalogEnabled_ReadCallback (ViSession vi,
                                                                 ViSession io,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViBoolean *value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  dlogMeasureTypeValue;
    ViString dlogMeasureType;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_DATALOG_MEASUREMENT, 0, &dlogMeasureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (dlogMeasureTypeValue, &attrDatalogMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &dlogMeasureType, VI_NULL));

    if (!Ivi_Simulating (vi))
        viCheckErr (viQueryf (io, "SENS:DLOG:FUNC:%s? (@%s)", "%s", dlogMeasureType, channelName, &value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrDatalogEnabled_WriteCallback (ViSession vi,
                                                                  ViSession io,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViBoolean value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  dlogMeasureTypeValue;
    ViString dlogMeasureType;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_DATALOG_MEASUREMENT, 0, &dlogMeasureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (dlogMeasureTypeValue, &attrDatalogMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &dlogMeasureType, VI_NULL));

    if (!Ivi_Simulating (vi))
        viCheckErr (viPrintf (io, "SENS:DLOG:FUNC:%s %s, (@%s)", dlogMeasureType, (value ? "ON" : "OFF"), channelName));

Error:
    return error;
}

/* - ZUP366_ATTR_DATALOG_PERIOD - */
static IviRangeTableEntry attrDatalogPeriodRangeTableEntries[] =
{
    { 0.000020, 60, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrDatalogPeriodRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrDatalogPeriodRangeTableEntries,
};

static ViStatus _VI_FUNC zup366AttrDatalogDuration_CheckCallback (ViSession vi,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_DefaultCheckCallbackViReal64 (vi, VI_NULL, attributeId, value));

Error:
    return error;
}

/* - ZUP366_ATTR_DATALOG_DURATION - */
static IviRangeTableEntry attrDatalogDurationRangeTableEntries[] =
{
    { 1, 3.599964e+9, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrDatalogDurationRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrDatalogDurationRangeTableEntries,
};

static ViStatus _VI_FUNC zup366AttrDatalogPeriod_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_DefaultCheckCallbackViReal64 (vi, VI_NULL, attributeId, value));

Error:
    return error;
}

/* - ZUP366_ATTR_DATALOG_OFFSET - */
static IviRangeTableEntry attrDatalogOffsetRangeTableEntries[] =
{
    { 0, 100, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrDatalogOffsetRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrDatalogOffsetRangeTableEntries,
};

static ViStatus _VI_FUNC zup366AttrDatalogOffset_CheckCallback (ViSession vi,
                                                                 ViConstString channelName,
                                                                 ViAttr attributeId,
                                                                 ViInt32 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_DefaultCheckCallbackViInt32 (vi, VI_NULL, attributeId, value));

Error:
    return error;
}

/* - ZUP366_ATTR_DATALOG_AUTO_RANGE - */
static ViStatus _VI_FUNC zup366AttrDatalogAutoRange_ReadCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViBoolean *value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  dlogMeasureTypeValue;
    ViString dlogMeasureType;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_DATALOG_MEASUREMENT, 0, &dlogMeasureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (dlogMeasureTypeValue, &attrDatalogMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &dlogMeasureType, VI_NULL));

    if (!Ivi_Simulating (vi))
        viCheckErr (viQueryf (io, "SENS:DLOG:%s:RANG:AUTO? (@%s)", "%s", dlogMeasureType, channelName, &value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrDatalogAutoRange_WriteCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViBoolean value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  dlogMeasureTypeValue;
    ViString dlogMeasureType;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_DATALOG_MEASUREMENT, 0, &dlogMeasureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (dlogMeasureTypeValue, &attrDatalogMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &dlogMeasureType, VI_NULL));
    if (!Ivi_Simulating (vi))
        viCheckErr (viPrintf (io, "SENS:DLOG:%s:RANG:AUTO %s, (@%s)", dlogMeasureType, (value ? "ON" : "OFF"), channelName));

Error:
    return error;
}

/* - ZUP366_ATTR_DATALOG_RANGE -*/
static ViStatus _VI_FUNC zup366AttrDatalogRange_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViInt32             channel_index         = 0, measureType, measureSeqType;
    ViReal64            maximum, minimum;

    checkErr (Ivi_GetAttributeViAddr (vi, VI_NULL, ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES, 0, (ViAddr *) &pinstrument_modules));
    channel_index = (channelName[0] - '0');

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_MEASUREMENT, 0, &measureType));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_MEASUREMENT, 0, &measureSeqType));

    if (measureType == ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE || measureSeqType == ZUP366_VAL_SEQ_MEAS_TYPE_VOLTAGE)
    {
        minimum = pinstrument_modules[channel_index]->MinVoltage[MAX_NB_RANGES - 2];
        maximum = pinstrument_modules[channel_index]->MaxVoltage[MAX_NB_RANGES - 1];
    }
    else
    {
        minimum = pinstrument_modules[channel_index]->MinCurrent[MAX_NB_RANGES - 2];
        maximum = pinstrument_modules[channel_index]->MaxCurrent[MAX_NB_RANGES - 1];
    }

    if (value < minimum || value > maximum)
        viCheckErr (IVI_ERROR_INVALID_VALUE);


Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrDatalogRange_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViReal64 *value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  dlogMeasureTypeValue;
    ViString dlogMeasureType;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_DATALOG_MEASUREMENT, 0, &dlogMeasureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (dlogMeasureTypeValue, &attrDatalogMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &dlogMeasureType, VI_NULL));

    if (!Ivi_Simulating (vi))
        viCheckErr (viQueryf (io, "SENS:DLOG:%s:RANG? (@%s)", "%Le", dlogMeasureType, channelName, &value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrDatalogRange_WriteCallback (ViSession vi,
                                                                ViSession io,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  dlogMeasureTypeValue;
    ViString dlogMeasureType;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_DATALOG_MEASUREMENT, 0, &dlogMeasureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (dlogMeasureTypeValue, &attrDatalogMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &dlogMeasureType, VI_NULL));

    if (!Ivi_Simulating (vi))
        viCheckErr (viPrintf (io, "SENS:DLOG:%s:RANG %Le, (@%s)", dlogMeasureType, value, channelName));

Error:
    return error;
}

/* - ZUP366_ATTR_DATALOG_MARKER1 - */
static IviRangeTableEntry attrDatalogMarker1PointsRangeTableEntries[] =
{
    { -3.599964e+9, 3.599964e+9, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrDatalogMarker1PointsRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrDatalogMarker1PointsRangeTableEntries,
};


/* - ZUP366_ATTR_DATALOG_MARKER2 - */
static IviRangeTableEntry attrDatalogMarker2PointsRangeTableEntries[] =
{
    { -3.599964e+9, 3.599964e+9, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrDatalogMarker2PointsRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrDatalogMarker2PointsRangeTableEntries,
};


/* - ZUP366_ATTR_HISTOGRAM_ENABLED -*/
static ViStatus _VI_FUNC zup366AttrHistogramEnabled_ReadCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViBoolean *value)
{
    ViStatus error = VI_SUCCESS;

    if (!Ivi_Simulating (vi))
        viCheckErr (viQueryf (io, "SENS:HIST:FUNC:CURR? (@%s)", "%s", channelName, &value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrHistogramEnabled_WriteCallback (ViSession vi,
                                                                    ViSession io,
                                                                    ViConstString channelName,
                                                                    ViAttr attributeId,
                                                                    ViBoolean value)
{
    ViStatus error = VI_SUCCESS;

    if (!Ivi_Simulating (vi))
        viCheckErr (viPrintf (io, "SENS:HIST:FUNC:CURR %s,(@%s)", (value ? "ON" : "OFF"), channelName));

Error:
    return error;
}

/* - ZUP366_ATTR_HISTOGRAM_AUTO_RANGE - */
static ViStatus _VI_FUNC zup366AttrHistogramAutoRange_ReadCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViBoolean *value)
{
    ViStatus error = VI_SUCCESS;

    if (!Ivi_Simulating (vi))
        viCheckErr (viQueryf (io, "SENS:HIST:CURR:RANG:AUTO? (@%s)", "%s", channelName, &value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrHistogramAutoRange_WriteCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViBoolean value)
{
    ViStatus error = VI_SUCCESS;

    if (!Ivi_Simulating (vi))
        viCheckErr (viPrintf (io, "SENS:HIST:CURR:RANG:AUTO %s,(@%s)", (value ? "ON" : "OFF"), channelName));

Error:
    return error;
}

/* - ZUP366_ATTR_HISTOGRAM_RANGE -*/
static ViStatus _VI_FUNC zup366AttrHistogramRange_CheckCallback (ViSession vi,
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId,
                                                                  ViReal64 value)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViInt32             channel_index         = 0;
    ViReal64            minimum, maximum;

    checkErr (Ivi_GetAttributeViAddr (vi, VI_NULL, ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                      0, (ViAddr *) &pinstrument_modules));

    channel_index = (channelName[0] - '0');

    minimum = pinstrument_modules[channel_index]->MinCurrent[MAX_NB_RANGES - 2];
    maximum = pinstrument_modules[channel_index]->MaxCurrent[MAX_NB_RANGES - 1];

    if (value < minimum || value > maximum)
        viCheckErr (IVI_ERROR_INVALID_VALUE);
Error:
    return error;
}

/* - ZUP366_ATTR_DATALOG_TRIGGER_LEVEL - */
static ViStatus _VI_FUNC zup366AttrDatalogTriggerLevel_CheckCallback (ViSession vi,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       ViReal64 value)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViInt32             channel_index         = 0, measureType, measureSeqType;
    ViReal64            maximum, minimum;

    checkErr (Ivi_GetAttributeViAddr (vi, VI_NULL, ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                      0, (ViAddr *) &pinstrument_modules));
    channel_index = (channelName[0] - '0');

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_WAVEFORM_MEASUREMENT, 0, &measureType));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_SEQUENCE_MEASUREMENT, 0, &measureSeqType));

    if (measureType == ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE || measureSeqType == ZUP366_VAL_SEQ_MEAS_TYPE_VOLTAGE)
    {
        minimum = pinstrument_modules[channel_index]->MinVoltage[MAX_NB_RANGES - 2];
        maximum = pinstrument_modules[channel_index]->MaxVoltage[MAX_NB_RANGES - 1];
    }
    else
    {
        minimum = pinstrument_modules[channel_index]->MinCurrent[MAX_NB_RANGES - 2];
        maximum = pinstrument_modules[channel_index]->MaxCurrent[MAX_NB_RANGES - 1];
    }

    if (value < minimum || value > maximum)
        viCheckErr (IVI_ERROR_INVALID_VALUE);

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrDatalogTriggerLevel_ReadCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViReal64 *value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  dlogTriggerSrcValue, dlogMeasureTypeValue;
    ViString dlogMeasureType;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_DATALOG_TRIGGER_SRC, 0, &dlogTriggerSrcValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_DATALOG_MEASUREMENT, 0, &dlogMeasureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (dlogMeasureTypeValue, &attrDatalogMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &dlogMeasureType, VI_NULL));

    if (!Ivi_Simulating (vi))
    {
        if (dlogTriggerSrcValue != ZUP366_VAL_TRIG_DATALOG_IMMEDIATE || dlogTriggerSrcValue != ZUP366_VAL_TRIG_DATALOG_EXTERNAL
            || dlogTriggerSrcValue != ZUP366_VAL_TRIG_DATALOG_BUS_SW)
        {
            viCheckErr (viQueryf (io, "TRIG:DLOG:%s:LEV? (@%s)", "%Le", dlogMeasureType, channelName, &value));
        }
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrDatalogTriggerLevel_WriteCallback (ViSession vi,
                                                                       ViSession io,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       ViReal64 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  dlogTriggerSrcValue, dlogMeasureTypeValue;
    ViString dlogMeasureType;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, ZUP366_ATTR_DATALOG_TRIGGER_SRC, 0, &dlogTriggerSrcValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_DATALOG_MEASUREMENT, 0, &dlogMeasureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (dlogMeasureTypeValue, &attrDatalogMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &dlogMeasureType, VI_NULL));

    if (!Ivi_Simulating (vi))
    {
        if (dlogTriggerSrcValue != ZUP366_VAL_TRIG_DATALOG_IMMEDIATE || dlogTriggerSrcValue != ZUP366_VAL_TRIG_DATALOG_EXTERNAL
            || dlogTriggerSrcValue != ZUP366_VAL_TRIG_DATALOG_BUS_SW)
        {
            viCheckErr (viPrintf (io, ":TRIG:DLOG:%s:LEV %Le, (@%s)", dlogMeasureType, value, channelName));
        }
    }

Error:
    return error;
}

/* - ZUP366_ATTR_DATALOG_TRIGGER_SLOPE - */
static IviRangeTableEntry attrDatalogSlopeRangeTableEntries[] =
{
    { ZUP366_VAL_DATALOG_SLOPE_POSITIVE, 0, 0, "POS", 0 },
    { ZUP366_VAL_DATALOG_SLOPE_NEGATIVE, 0, 0, "NEG", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrDatalogSlopeRangeTable =
{
    IVI_VAL_DISCRETE,
    VI_FALSE,
    VI_FALSE,
    VI_NULL,
    attrDatalogSlopeRangeTableEntries,
};

static ViStatus _VI_FUNC zup366AttrDatalogTriggerSlope_ReadCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViInt32 *value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  dlogTriggerSrcValue, dlogMeasureTypeValue;
    ViString dlogMeasureType;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_DATALOG_TRIGGER_SRC, 0, &dlogTriggerSrcValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_DATALOG_MEASUREMENT, 0, &dlogMeasureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (dlogMeasureTypeValue, &attrDatalogMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &dlogMeasureType, VI_NULL));

    if (!Ivi_Simulating (vi))
    {
        if (dlogTriggerSrcValue != ZUP366_VAL_TRIG_DATALOG_IMMEDIATE || dlogTriggerSrcValue != ZUP366_VAL_TRIG_DATALOG_EXTERNAL
            || dlogTriggerSrcValue != ZUP366_VAL_TRIG_DATALOG_BUS_SW)
            viCheckErr (viQueryf (io, ":TRIG:DLOG:%s:SLOP? (@%s)", "%s", dlogMeasureType, channelName, &value));
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrDatalogTriggerSlope_WriteCallback (ViSession vi,
                                                                       ViSession io,
                                                                       ViConstString channelName,
                                                                       ViAttr attributeId,
                                                                       ViInt32 value)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  dlogTriggerSrcValue, dlogMeasureTypeValue;
    ViString dlogMeasureType, dlogSlopeType;

    viCheckErr (Ivi_GetAttributeViInt32 (vi, VI_NULL, ZUP366_ATTR_DATALOG_TRIGGER_SRC, 0, &dlogTriggerSrcValue));
    viCheckErr (Ivi_GetAttributeViInt32 (vi, channelName, ZUP366_ATTR_DATALOG_MEASUREMENT, 0, &dlogMeasureTypeValue));
    viCheckErr (Ivi_GetViInt32EntryFromValue (dlogMeasureTypeValue, &attrDatalogMeasurementRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &dlogMeasureType, VI_NULL));
    viCheckErr (Ivi_GetViInt32EntryFromValue (value, &attrDatalogSlopeRangeTable, VI_NULL, VI_NULL,
                                              VI_NULL, VI_NULL, &dlogSlopeType, VI_NULL));

    if (!Ivi_Simulating (vi))
    {
        if (dlogTriggerSrcValue != ZUP366_VAL_TRIG_DATALOG_IMMEDIATE || dlogTriggerSrcValue != ZUP366_VAL_TRIG_DATALOG_EXTERNAL
            || dlogTriggerSrcValue != ZUP366_VAL_TRIG_DATALOG_BUS_SW)
            viCheckErr (viPrintf (io, ":TRIG:DLOG:%s:SLOP %s, (@%s)", dlogMeasureType, dlogSlopeType, channelName));
    }

Error:
    return error;
}

/* - ZUP366_ATTR_SMU_POSITIVE_VOLTAGE_LIMIT - */
static IviRangeTableEntry attrVoltageLimitPositiveRangeTableEntries[] =
{
    { 0, 20.4, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrVoltageLimitPositiveRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrVoltageLimitPositiveRangeTableEntries,
};

static ViStatus _VI_FUNC zup366AttrSmuPositiveVoltageLimit_CheckCallback (ViSession vi,
                                                                           ViConstString channelName,
                                                                           ViAttr attributeId,
                                                                           ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    checkErr (Ivi_DefaultCheckCallbackViReal64 (vi, channelName, attributeId, value));

Error:
    return error;
}

/* - ZUP366_ATTR_SMU_NEGATIVE_VOLTAGE_LIMIT - */
static IviRangeTableEntry attrVoltageLimitNegativeRangeTableEntries[] =
{
    { -20.4, 0, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrVoltageLimitNegativeRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrVoltageLimitNegativeRangeTableEntries,
};


/* - ZUP366_ATTR_SMU_ASM_POSTIVE_CURRENT_LIMIT -*/
static IviRangeTableEntry attrCurrentLimitPositiveRangeTableEntries[] =
{
    { 0, 3.06, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrCurrentLimitPositiveRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrCurrentLimitPositiveRangeTableEntries,
};


/* - ZUP366_ATTR_SMU_NEGATIVE_CURRENT_LIMIT - */
static IviRangeTableEntry attrCurrentLimitNegativeN6783ABATRangeTableEntries[] =
{
    { -2, 0, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrCurrentLimitNegativeN6783ABATRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrCurrentLimitNegativeN6783ABATRangeTableEntries,
};

static IviRangeTableEntry attrCurrentLimitNegativeN678xARangeTableEntries[] =
{
    { -3.06, 0, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrCurrentLimitNegativeN678xARangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrCurrentLimitNegativeN678xARangeTableEntries,
};


static ViStatus _VI_FUNC zup366AttrSmuAsmNegativeCurrentLimit_RangeTableCallback (ViSession vi,
                                                                                   ViConstString channelName,
                                                                                   ViAttr attributeId,
                                                                                   IviRangeTablePtr *rangeTablePtr)
{
    ViStatus         error  = VI_SUCCESS;
    IviRangeTablePtr tblPtr = VI_NULL;
    ViInt32          model_type;

    checkErr (zup366_GetModuleType (vi, channelName, &model_type));

    if (model_type != N6783A)
        tblPtr = &attrCurrentLimitNegativeN678xARangeTable;
    else
        tblPtr = &attrCurrentLimitNegativeN6783ABATRangeTable;

Error:
    *rangeTablePtr = tblPtr;
    return error;
}

/* - ZUP366_ATTR_SMU_POSITIVE_OVRP -*/
static IviRangeTableEntry attrSmuPositiveOvrpRangeTableEntries[] =
{
    { 0, 22, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrSmuPositiveOvrpRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrSmuPositiveOvrpRangeTableEntries,
};


/*- ZUP366_ATTR_NEGATIVE_OVRP -*/
static IviRangeTableEntry attrSmuNegativeOvrpRangeTableEntries[] =
{
    { -22, 0, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrSmuNegativeOvrpRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrSmuNegativeOvrpRangeTableEntries,
};


/* - ZUP366_ATTR_ASM_OVER_PROTECTION_DELAY - */
static IviRangeTableEntry attrOverProtectionDelayRangeTableEntries[] =
{
    { 6.E-05, 0.005, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrOverProtectionDelayRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrOverProtectionDelayRangeTableEntries,
};



/* - ZUP366_ATTR_SMU_CURRENT_SLEW_RATE -*/
static IviRangeTableEntry attrCurrentSlewRateRangeTableEntries[] =
{
    { 0, 9.9E+37, 0, "", 0 },
    { IVI_RANGE_TABLE_LAST_ENTRY }
};

static IviRangeTable attrCurrentSlewRateRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    attrCurrentSlewRateRangeTableEntries,
};

static ViStatus _VI_FUNC zup366AttrSmuCurrentSlewRate_CheckCallback (ViSession vi,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    if (value != ZUP366_VAL_SLEW_RATE_COUNT_INFINITY)
        checkErr (Ivi_DefaultCheckCallbackViReal64 (vi, channelName, attributeId, value));

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrSmuCurrentSlewRate_ReadCallback (ViSession vi,
                                                                     ViSession io,
                                                                     ViConstString channelName,
                                                                     ViAttr attributeId,
                                                                     ViReal64 *value)
{
    ViStatus error         = VI_SUCCESS;
    ViInt32  xCurrSlewRate = 0;

    if (!Ivi_Simulating (vi))
    {
        viCheckErr (viQueryf (io, ":SOUR:CURR:SLEW? (@%s)", "%Le", channelName, &xCurrSlewRate));
    }

    if (xCurrSlewRate <= 9.9e37)
        *value = xCurrSlewRate;
    else
        *value = 0;

Error:
    return error;
}

static ViStatus _VI_FUNC zup366AttrSmuCurrentSlewRate_WriteCallback (ViSession vi,
                                                                      ViSession io,
                                                                      ViConstString channelName,
                                                                      ViAttr attributeId,
                                                                      ViReal64 value)
{
    ViStatus error = VI_SUCCESS;

    if (!Ivi_Simulating (vi))
    {
        if (value < 0)
        {
            viCheckErr (viPrintf (io, ":SOUR:CURR:SLEW INF,(@%s)", channelName));
            viCheckErr (viPrintf (io, ":SOUR:CURR:SLEW:MAX ON,(@%s)", channelName));
        }
        else
        {
            viCheckErr (viPrintf (io, ":SOUR:CURR:SLEW %Le,(@%s)", value, channelName));
            viCheckErr (viPrintf (io, ":SOUR:CURR:SLEW:MAX OFF,(@%s)", channelName));
        }
    }

Error:
    return error;
}

static ViStatus _VI_FUNC zup366ElogCurrentRange_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViInt32             channel_index         = 0;
    ViReal64            minimum, maximum;

    checkErr (Ivi_GetAttributeViAddr (vi, VI_NULL, ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                      0, (ViAddr *) &pinstrument_modules));

    channel_index = (channelName[0] - '0');

    minimum = pinstrument_modules[channel_index]->MinCurrent[MAX_NB_RANGES - 2];
    maximum = pinstrument_modules[channel_index]->MaxCurrent[MAX_NB_RANGES - 1];

    if (value < minimum || value > maximum)
        viCheckErr (IVI_ERROR_INVALID_VALUE);
Error:
    return error;
}

static ViStatus _VI_FUNC zup366ElogVoltageRange_CheckCallback (ViSession vi,
                                                                ViConstString channelName,
                                                                ViAttr attributeId,
                                                                ViReal64 value)
{
    ViStatus            error                 = VI_SUCCESS;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViInt32             channel_index         = 0;
    ViReal64            minimum, maximum;

    checkErr (Ivi_GetAttributeViAddr (vi, VI_NULL, ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                      0, (ViAddr *) &pinstrument_modules));

    channel_index = (channelName[0] - '0');

    minimum = pinstrument_modules[channel_index]->MinVoltage[MAX_NB_RANGES - 2];
    maximum = pinstrument_modules[channel_index]->MaxVoltage[MAX_NB_RANGES - 1];

    if (value < minimum || value > maximum)
        viCheckErr (IVI_ERROR_INVALID_VALUE);
Error:
    return error;
}


/*****************************************************************************
 * Function: zup366_InitAttributes
 * Purpose:  This function adds attributes to the IVI session, initializes
 *           instrument attributes, and sets attribute invalidation
 *           dependencies.
 *****************************************************************************/
static ViStatus zup366_InitAttributes (ViSession vi,
                                        ViInt32 pInstaledModules[MAX_MODULE_COUNT + 1])
{
    ViStatus            error                 = VI_SUCCESS;
    ViInt32             flags                 = 0;
    SInstrumentRangesPtr* pinstrument_modules = VI_NULL;
    ViInt32             channel_count         = MAX_MODULE_COUNT;
    ViInt32             i;


    checkErr (ag_CreateModuleTable (vi, pInstaledModules, &pinstrument_modules));

    for (i = channel_count; i > 0; i--)
    {
        if (pInstaledModules[i] == NOT_SUPPORTED)
            channel_count--;
    }

    /*- Initialize instrument attributes --------------------------------*/
    checkErr (Ivi_SetAttributeViInt32 (vi, "", ZUP366_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION,
                                       0, ZUP366_MAJOR_VERSION));
    checkErr (Ivi_SetAttributeViInt32 (vi, "", ZUP366_ATTR_SPECIFIC_DRIVER_MINOR_VERSION,
                                       0, ZUP366_MINOR_VERSION));
    checkErr (Ivi_SetAttributeViString (vi, "", ZUP366_ATTR_IO_SESSION_TYPE,
                                        0, ZUP366_IO_SESSION_TYPE));

    checkErr (Ivi_SetAttrReadCallbackViString (vi, ZUP366_ATTR_SPECIFIC_DRIVER_REVISION,
                                               zup366AttrDriverRevision_ReadCallback));
    checkErr (Ivi_SetAttributeViInt32 (vi, "", ZUP366_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION,
                                       0, ZUP366_CLASS_SPEC_MAJOR_VERSION));
    checkErr (Ivi_SetAttributeViInt32 (vi, "", ZUP366_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION,
                                       0, ZUP366_CLASS_SPEC_MINOR_VERSION));
    checkErr (Ivi_SetAttributeViString (vi, "", ZUP366_ATTR_SUPPORTED_INSTRUMENT_MODELS,
                                        0, ZUP366_SUPPORTED_INSTRUMENT_MODELS));


    checkErr (Ivi_GetAttributeFlags (vi, ZUP366_ATTR_INSTRUMENT_FIRMWARE_REVISION, &flags));
    checkErr (Ivi_SetAttributeFlags (vi, ZUP366_ATTR_INSTRUMENT_FIRMWARE_REVISION,
                                     flags | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION));
    checkErr (Ivi_SetAttrReadCallbackViString (vi, ZUP366_ATTR_INSTRUMENT_FIRMWARE_REVISION,
                                               zup366AttrFirmwareRevision_ReadCallback));

    checkErr (Ivi_GetAttributeFlags (vi, ZUP366_ATTR_INSTRUMENT_MANUFACTURER, &flags));
    checkErr (Ivi_SetAttributeFlags (vi, ZUP366_ATTR_INSTRUMENT_MANUFACTURER,
                                     flags | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION));
    checkErr (Ivi_SetAttrReadCallbackViString (vi, ZUP366_ATTR_INSTRUMENT_MANUFACTURER,
                                               zup366AttrInstrumentManufacturer_ReadCallback));

    checkErr (Ivi_GetAttributeFlags (vi, ZUP366_ATTR_INSTRUMENT_MODEL, &flags));
    checkErr (Ivi_SetAttributeFlags (vi, ZUP366_ATTR_INSTRUMENT_MODEL,
                                     flags | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION));
    checkErr (Ivi_SetAttrReadCallbackViString (vi, ZUP366_ATTR_INSTRUMENT_MODEL,
                                               zup366AttrInstrumentModel_ReadCallback));

    checkErr (Ivi_SetAttributeViString (vi, "", ZUP366_ATTR_SPECIFIC_DRIVER_VENDOR,
                                        0, ZUP366_DRIVER_VENDOR));
    checkErr (Ivi_SetAttributeViString (vi, "", ZUP366_ATTR_SPECIFIC_DRIVER_DESCRIPTION,
                                        0, ZUP366_DRIVER_DESCRIPTION));
    checkErr (Ivi_SetAttributeViAddr (vi, VI_NULL, IVI_ATTR_OPC_CALLBACK, 0,
                                      VI_NULL));
    checkErr (Ivi_SetAttributeViAddr (vi, VI_NULL, IVI_ATTR_CHECK_STATUS_CALLBACK, 0,
                                      zup366_CheckStatusCallback));

    checkErr (Ivi_SetAttributeViString (vi, "", ZUP366_ATTR_GROUP_CAPABILITIES, 0,
                                        "IviDCPwrBase,"
                                        "IviDCPwrTrigger,"
                                        "IviDCPwrSoftwareTrigger,"
                                        "IviDCPwrMeasurement"));

    /*- Add instrument-specific attributes ------------------------------*/
													   
    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_OUTPUT_ENABLED,
                                         "ZUP366_ATTR_OUTPUT_ENABLED", VI_FALSE,
                                         IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));

    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_OUTPUT_PROTECTION_COUPLED,
                                         "ZUP366_ATTR_OUTPUT_PROTECTION_COUPLED",
                                         VI_FALSE, 0, zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));

    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_OUTPUT_PROTECTION_DELAY,
                                        "ZUP366_ATTR_OUTPUT_PROTECTION_DELAY",
                                        20.0e-3,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback,
                                        &attrOutputProtectionDelayRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_VOLTAGE_LEVEL,
                                        "ZUP366_ATTR_VOLTAGE_LEVEL", 0.02,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback, VI_NULL, 0));
	
    checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi, ZUP366_ATTR_VOLTAGE_LEVEL,
                                                 zup366AttrVoltageLevel_CoerceCallback));


    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_OVP_ENABLED,
                                         "ZUP366_ATTR_OVP_ENABLED", VI_TRUE,
                                         IVI_VAL_MULTI_CHANNEL, VI_NULL, VI_NULL));

    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_OVP_LIMIT,
                                        "ZUP366_ATTR_OVP_LIMIT", 20.0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback, VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi,
                                                ZUP366_ATTR_OVP_LIMIT,
                                                zup366AttrOvpLimit_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_CURRENT_LIMIT_BEHAVIOR,
                                       "ZUP366_ATTR_CURRENT_LIMIT_BEHAVIOR",
                                       ZUP366_VAL_CURRENT_REGULATE,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrCurrentLimitBehaviorRangeTable));

    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_CURRENT_LIMIT,
                                        "ZUP366_ATTR_CURRENT_LIMIT", 0.01,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback, VI_NULL, 0));

    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi,
                                                ZUP366_ATTR_CURRENT_LIMIT,
                                                zup366AttrCurrentLimit_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_TRIGGER_SOURCE,
                                       "ZUP366_ATTR_TRIGGER_SOURCE",
                                       ZUP366_VAL_SOFTWARE_TRIG,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrTriggerSourceRangeTable));


    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_ACQUISITION_TRIGGER_SOURCE,
                                       "ZUP366_ATTR_ACQUISITION_TRIGGER_SOURCE",
                                       ZUP366_VAL_SOFTWARE_TRIG,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrTriggerSourceRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi,
                                               ZUP366_ATTR_ACQUISITION_TRIGGER_SOURCE,
                                               zup366AttrListViInt32_CheckCallback));


    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_TRIGGERED_VOLTAGE_LEVEL,
                                        "ZUP366_ATTR_TRIGGERED_VOLTAGE_LEVEL", 0.02,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback, VI_NULL, 0));

    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi,
                                                ZUP366_ATTR_TRIGGERED_VOLTAGE_LEVEL,
                                                zup366AttrTrigVoltageLevel_CheckCallback));

    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_TRIGGERED_CURRENT_LIMIT,
                                        "ZUP366_ATTR_TRIGGERED_CURRENT_LIMIT", 0.01,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback, VI_NULL, 0));

    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_TRIGGERED_CURRENT_LIMIT,
                                                zup366AttrTrigCurrentLimit_CheckCallback));

    checkErr (Ivi_AddAttributeViString (vi, ZUP366_ATTR_ID_QUERY_RESPONSE,
                                        "ZUP366_ATTR_ID_QUERY_RESPONSE",
                                        "Agilent Technologies,N6700,xxx,xxx",
                                        IVI_VAL_NOT_USER_WRITABLE,
                                        zup366AttrIdQueryResponse_ReadCallback,
                                        VI_NULL));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_OPC_TIMEOUT,
                                       "ZUP366_ATTR_OPC_TIMEOUT",
                                       5000, IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, VI_NULL));
    checkErr (Ivi_AddAttributeViInt32 (vi,
                                       ZUP366_ATTR_VISA_RM_SESSION,
                                       "ZUP366_ATTR_VISA_RM_SESSION",
                                       VI_NULL,
                                       IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, VI_NULL));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_OPC_CALLBACK,
                                       "ZUP366_ATTR_OPC_CALLBACK",
                                       VI_NULL,
                                       IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, VI_NULL));
    checkErr (Ivi_AddAttributeViInt32 (vi,
                                       ZUP366_ATTR_CHECK_STATUS_CALLBACK,
                                       "ZUP366_ATTR_CHECK_STATUS_CALLBACK",
                                       VI_NULL,
                                       IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, VI_NULL));
    checkErr (Ivi_AddAttributeViInt32 (vi,
                                       ZUP366_ATTR_USER_INTERCHANGE_CHECK_CALLBACK,
                                       "ZUP366_ATTR_USER_INTERCHANGE_CHECK_CALLBACK",
                                       VI_NULL,
                                       IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, VI_NULL));

    checkErr (Ivi_AddAttributeViAddr (vi,
                                      ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES,
                                      "ZUP366_ATTR_INSTR_INSTRUMENT_MODULES_RANGES",
                                      pinstrument_modules,
                                      IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                      VI_NULL, VI_NULL));

    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_INSTR_VOLTAGE_RANGE,
                                        "ZUP366_ATTR_INSTR_VOLTAGE_RANGE",
                                        50.00,
                                        IVI_VAL_HIDDEN | IVI_VAL_MULTI_CHANNEL | IVI_VAL_COERCEABLE_ONLY_BY_INSTR,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback,
                                        VI_NULL, 0));

    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_INSTR_CURRENT_RANGE,
                                        "ZUP366_ATTR_INSTR_CURRENT_RANGE", 5.0,
                                        IVI_VAL_HIDDEN | IVI_VAL_MULTI_CHANNEL | IVI_VAL_COERCEABLE_ONLY_BY_INSTR,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback, VI_NULL, 0));
    checkErr (Ivi_SetAttrCoerceCallbackViReal64 (vi,
                                                 ZUP366_ATTR_INSTR_CURRENT_RANGE,
                                                 zup366AttrInstrCurrentRange_CoerceCallback));


    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_INSTR_VOLTAGE_MEASUREMENT_RANGE,
                                        "ZUP366_ATTR_INSTR_VOLTAGE_MEASUREMENT_RANGE",
                                        0.02,
                                        IVI_VAL_HIDDEN | IVI_VAL_MULTI_CHANNEL | IVI_VAL_COERCEABLE_ONLY_BY_INSTR,
                                        zup366AttrInstrMeasurementRange_ReadCallback,
                                        zup366AttrInstrMeasurementRange_WriteCallback,
                                        VI_NULL, 0));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_LIST_COUNT,
                                       "ZUP366_ATTR_LIST_COUNT", 1,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366AttrListCount_ReadCallback,
                                       zup366AttrListCount_WriteCallback,
                                       &attrListCountRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi,
                                               ZUP366_ATTR_LIST_COUNT,
                                               zup366AttrListCount_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_LIST_STEP,
                                       "ZUP366_ATTR_LIST_STEP",
                                       ZUP366_VAL_LIST_STEP_AUTO,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrListStepRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi,
                                               ZUP366_ATTR_LIST_STEP,
                                               zup366AttrListViInt32_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_TRIGGER_CURRENT_MODE,
                                       "ZUP366_ATTR_TRIGGER_CURRENT_MODE",
                                       ZUP366_VAL_TRIGGER_CURRENT_MODE_STEP,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrCurrentModeRangeTable));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_TRIGGER_VOLTAGE_MODE,
                                       "ZUP366_ATTR_TRIGGER_VOLTAGE_MODE",
                                       ZUP366_VAL_TRIGGER_VOLTAGE_MODE_STEP,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrVoltageModeRangeTable));

    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_LIST_TERMINATE_LAST,
                                         "ZUP366_ATTR_LIST_TERMINATE_LAST",
                                         VI_FALSE, IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));
    checkErr (Ivi_SetAttrCheckCallbackViBoolean (vi,
                                                 ZUP366_ATTR_LIST_TERMINATE_LAST,
                                                 zup366AttrListViBoolean_CheckCallback));

    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_LIST_TRIGGER_OUTPUT_ENABLED,
                                         "ZUP366_ATTR_LIST_TRIGGER_OUTPUT_ENABLED",
                                         VI_FALSE,
                                         IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));


    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_LIST_VOLTAGE_POINTS,
                                       "ZUP366_ATTR_LIST_VOLTAGE_POINTS", 0,
                                       IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION,
                                       zup366AttrListViInt32_ReadCallback, VI_NULL, VI_NULL));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_LIST_CURRENT_POINTS,
                                       "ZUP366_ATTR_LIST_CURRENT_POINTS", 0,
                                       IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION,
                                       zup366AttrListViInt32_ReadCallback, VI_NULL, VI_NULL));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_LIST_DWELL_POINTS,
                                       "ZUP366_ATTR_LIST_DWELL_POINTS", 0,
                                       IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION,
                                       zup366AttrListViInt32_ReadCallback, VI_NULL, VI_NULL));


    checkErr (Ivi_AddAttributeViInt32 (vi,
                                       ZUP366_ATTR_LIST_TRIGGER_OUTPUT_BOST_POINTS,
                                       "ZUP366_ATTR_LIST_TRIGGER_OUTPUT_BOST_POINTS", 0,
                                       IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION,
                                       zup366AttrListViInt32_ReadCallback, VI_NULL, VI_NULL));

    checkErr (Ivi_AddAttributeViInt32 (vi,
                                       ZUP366_ATTR_LIST_TRIGGER_OUTPUT_EOST_POINTS,
                                       "ZUP366_ATTR_LIST_TRIGGER_OUTPUT_EOST_POINTS", 0,
                                       IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION,
                                       zup366AttrListViInt32_ReadCallback, VI_NULL, VI_NULL));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_SWEEP_POINTS,
                                       "ZUP366_ATTR_SWEEP_POINTS", 1024,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366ViInt32_ReadCallback,
                                       zup366ViInt32_WriteCallback,
                                       &attrSweepPointsRangeTable));

    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi,
                                               ZUP366_ATTR_SWEEP_POINTS,
                                               zup366AttrListViInt32_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_SWEEP_OFFSET_POINTS,
                                       "ZUP366_ATTR_SWEEP_OFFSET_POINTS", 0,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366ViInt32_ReadCallback,
                                       zup366ViInt32_WriteCallback,
                                       &attrSweepOffsetPointsRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi,
                                               ZUP366_ATTR_SWEEP_OFFSET_POINTS,
                                               zup366AttrListViInt32_CheckCallback));

    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_SWEEP_TIME_INTERVAL,
                                        "ZUP366_ATTR_SWEEP_TIME_INTERVAL", 20.48e-6,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback,
                                        &attrSweepTimeIntervalRangeTable, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi,
                                                ZUP366_ATTR_SWEEP_TIME_INTERVAL,
                                                zup366AttrListViReal64_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_MEASUREMENT_WINDOW,
                                       "ZUP366_ATTR_MEASUREMENT_WINDOW",
                                       ZUP366_VAL_MEASUREMENT_WINDOW_RECTANGULAR,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrMeasurementWindowRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi,
                                               ZUP366_ATTR_MEASUREMENT_WINDOW,
                                               zup366AttrListViInt32_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_INSTR_MODULE_OPTIONS,
                                       "ZUP366_ATTR_INSTR_MODULE_OPTIONS",
                                       0,
                                       IVI_VAL_HIDDEN | IVI_VAL_MULTI_CHANNEL,
                                       zup366AttrInstrModuleOptions_ReadCallback,
                                       VI_NULL, VI_NULL));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_INSTR_CHANNEL_COUNT,
                                       "ZUP366_ATTR_INSTR_CHANNEL_COUNT",
                                       channel_count,
                                       IVI_VAL_HIDDEN | IVI_VAL_ALWAYS_CACHE,
                                       zup366ViInt32_ReadCallback,
                                       VI_NULL, VI_NULL));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_MEASUREMENT_FUNCTION,
                                       "ZUP366_ATTR_MEASUREMENT_FUNCTION",
                                       ZUP366_VAL_MEASURE_VOLTAGE,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366AttrMeasurementfunction_ReadCallback,
                                       zup366AttrMeasurementfunction_WriteCallback,
                                       &measurementTypeRangeTable));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_OUTPUT_DELAY_FALL,
                                       "ZUP366_ATTR_OUTPUT_DELAY_FALL", 0,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366AttrOutputDelay_ReadCallback,
                                       zup366AttrOutputDelay_WriteCallback,
                                       &attrOutputDelayRangeTable));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_OUTPUT_DELAY_RISE,
                                       "ZUP366_ATTR_OUTPUT_DELAY_RISE", 0,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366AttrOutputDelay_ReadCallback,
                                       zup366AttrOutputDelay_WriteCallback,
                                       &attrOutputDelayRangeTable));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_OUTPUT_INHIBIT_MODE,
                                       "ZUP366_ATTR_OUTPUT_INHIBIT_MODE",
                                       ZUP366_VAL_OUTPUT_INHIBIT_MODE_OFF, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrOutputInhibitModeRangeTable));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_INPUT_DATA,
                                       "ZUP366_ATTR_DIGITAL_INPUT_DATA", 0,
                                       IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_NEVER_CACHE,
                                       zup366ViInt32_ReadCallback, VI_NULL,
                                       VI_NULL));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_OUTPUT_DATA,
                                       "ZUP366_ATTR_DIGITAL_OUTPUT_DATA", 7, 0,
                                       zup366ViInt32_ReadCallback,
                                       zup366ViInt32_WriteCallback, VI_NULL));

    checkErr (Ivi_AddAttributeViString (vi, ZUP366_ATTR_GROUP_CHANNELS,
                                        "ZUP366_ATTR_GROUP_CHANNELS", "",
                                        IVI_VAL_NOT_USER_READABLE, VI_NULL,
                                        zup366ViString_WriteCallback));

    checkErr (Ivi_AddAttributeViString (vi, ZUP366_ATTR_UNGROUP_CHANNEL,
                                        "ZUP366_ATTR_UNGROUP_CHANNEL", "",
                                        IVI_VAL_NOT_USER_READABLE, VI_NULL,
                                        zup366ViString_WriteCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_PIN1_FUNCTION,
                                       "ZUP366_ATTR_DIGITAL_PIN1_FUNCTION",
                                       ZUP366_VAL_DIGITAL_PIN_FUNCTION_DINPUT, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrDigitalPinFunctionRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi,
                                               ZUP366_ATTR_DIGITAL_PIN1_FUNCTION,
                                               zup366AttrOutputInhibitMode_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_PIN2_FUNCTION,
                                       "ZUP366_ATTR_DIGITAL_PIN2_FUNCTION",
                                       ZUP366_VAL_DIGITAL_PIN_FUNCTION_DINPUT, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrDigitalPinFunctionRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi,
                                               ZUP366_ATTR_DIGITAL_PIN2_FUNCTION,
                                               zup366AttrOutputInhibitMode_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_PIN3_FUNCTION,
                                       "ZUP366_ATTR_DIGITAL_PIN3_FUNCTION",
                                       ZUP366_VAL_DIGITAL_PIN_FUNCTION_DINPUT, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrDigitalPinFunctionRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi,
                                               ZUP366_ATTR_DIGITAL_PIN3_FUNCTION,
                                               zup366AttrOutputInhibitMode_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_PIN4_FUNCTION,
                                       "ZUP366_ATTR_DIGITAL_PIN4_FUNCTION",
                                       ZUP366_VAL_DIGITAL_PIN_FUNCTION_DINPUT, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrDigitalPinFunctionRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi,
                                               ZUP366_ATTR_DIGITAL_PIN4_FUNCTION,
                                               zup366AttrOutputInhibitMode_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_PIN5_FUNCTION,
                                       "ZUP366_ATTR_DIGITAL_PIN5_FUNCTION",
                                       ZUP366_VAL_DIGITAL_PIN_FUNCTION_DINPUT, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrDigitalPinFunctionRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi,
                                               ZUP366_ATTR_DIGITAL_PIN5_FUNCTION,
                                               zup366AttrOutputInhibitMode_CheckCallback));


    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_PIN6_FUNCTION,
                                       "ZUP366_ATTR_DIGITAL_PIN6_FUNCTION",
                                       ZUP366_VAL_DIGITAL_PIN_FUNCTION_DINPUT, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrDigitalPinFunctionRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi,
                                               ZUP366_ATTR_DIGITAL_PIN6_FUNCTION,
                                               zup366AttrOutputInhibitMode_CheckCallback));


    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_PIN7_FUNCTION,
                                       "ZUP366_ATTR_DIGITAL_PIN7_FUNCTION",
                                       ZUP366_VAL_DIGITAL_PIN_FUNCTION_DINPUT, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrDigitalPinFunctionRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi,
                                               ZUP366_ATTR_DIGITAL_PIN7_FUNCTION,
                                               zup366AttrOutputInhibitMode_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_PIN1_POLARITY,
                                       "ZUP366_ATTR_DIGITAL_PIN1_POLARITY",
                                       ZUP366_VAL_DIGITAL_PIN_POLARITY_POSITIVE, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrDigitalPinPolarityRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_PIN2_POLARITY,
                                       "ZUP366_ATTR_DIGITAL_PIN2_POLARITY",
                                       ZUP366_VAL_DIGITAL_PIN_POLARITY_POSITIVE, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrDigitalPinPolarityRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_PIN3_POLARITY,
                                       "ZUP366_ATTR_DIGITAL_PIN3_POLARITY",
                                       ZUP366_VAL_DIGITAL_PIN_POLARITY_POSITIVE, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrDigitalPinPolarityRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_PIN4_POLARITY,
                                       "ZUP366_ATTR_DIGITAL_PIN4_POLARITY",
                                       ZUP366_VAL_DIGITAL_PIN_POLARITY_POSITIVE, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrDigitalPinPolarityRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_PIN5_POLARITY,
                                       "ZUP366_ATTR_DIGITAL_PIN5_POLARITY",
                                       ZUP366_VAL_DIGITAL_PIN_POLARITY_POSITIVE, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrDigitalPinPolarityRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_PIN6_POLARITY,
                                       "ZUP366_ATTR_DIGITAL_PIN6_POLARITY",
                                       ZUP366_VAL_DIGITAL_PIN_POLARITY_POSITIVE, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrDigitalPinPolarityRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DIGITAL_PIN7_POLARITY,
                                       "ZUP366_ATTR_DIGITAL_PIN7_POLARITY",
                                       ZUP366_VAL_DIGITAL_PIN_POLARITY_POSITIVE, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrDigitalPinPolarityRangeTable));

    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_INITIATE_CONTINUOUS,
                                         "ZUP366_ATTR_INITIATE_CONTINUOUS",
                                         VI_FALSE, 0, zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_RELAY_POLARITY,
                                       "ZUP366_ATTR_RELAY_POLARITY",
                                       ZUP366_VAL_RELAY_POLARITY_NORMAL,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrRelayPolarityRangeTable));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi,
                                               ZUP366_ATTR_RELAY_POLARITY,
                                               zup366AttrRelayPolarity_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_POWER_LIMIT,
                                        "ZUP366_ATTR_POWER_LIMIT", 20.0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback, VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi,
                                                ZUP366_ATTR_POWER_LIMIT,
                                                zup366AttrPowerLimit_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_WAVEFORM_SHAPE,
                                       "ZUP366_ATTR_WAVEFORM_SHAPE",
                                       ZUP366_VAL_WAVEFORM_NONE,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrWaveformShapeRangeTable));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_WAVEFORM_MEASUREMENT,
                                       "ZUP366_ATTR_WAVEFORM_MEASUREMENT",
                                       ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrWaveformMeasurementRangeTable));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_WAVEFORM_COUNT,
                                       "ZUP366_ATTR_WAVEFORM_COUNT", 1,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366AttrWaveformCount_ReadCallback,
                                       zup366AttrWaveformCount_WriteCallback,
                                       VI_NULL));
    checkErr (Ivi_SetAttrRangeTableCallback (vi, ZUP366_ATTR_WAVEFORM_COUNT,
                                             zup366AttrWaveformCount_RangeTableCallback));

    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_WAVEFORM_TERMINATION,
                                         "ZUP366_ATTR_WAVEFORM_TERMINATION",
                                         VI_FALSE, IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));

    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_EXP_TOTAL_TIME,
                                        "ZUP366_ATTR_EXP_TOTAL_TIME", 1,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrExpTotalTime_ReadCallback,
                                        zup366AttrExpTotalTime_WriteCallback,
                                        &attrTotalTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_EXP_TIME_CONSTANT,
                                        "ZUP366_ATTR_EXP_TIME_CONSTANT", 1,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrExpTimeConstant_ReadCallback,
                                        zup366AttrExpTimeConstant_WriteCallback,
                                        &attrExponentialTimeConstantRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_EXP_START_TIME,
                                        "ZUP366_ATTR_EXP_START_TIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrExpStartTime_ReadCallback,
                                        zup366AttrExpStartTime_WriteCallback,
                                        &attrStartTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_EXP_END_LEVEL,
                                        "ZUP366_ATTR_EXP_END_LEVEL", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrExpEndLevel_ReadCallback,
                                        zup366AttrExpEndLevel_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_EXP_END_LEVEL,
                                                zup366AttrExpEndLevel_CheckCallback));

    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_EXP_START_LEVEL,
                                        "ZUP366_ATTR_EXP_START_LEVEL", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrExpStartLevel_ReadCallback,
                                        zup366AttrExpStartLevel_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_EXP_START_LEVEL,
                                                zup366AttrExpStartLevel_CheckCallback));

    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_SEQUENCE_STEP_NUMBER,
                                       "ZUP366_ATTR_SEQUENCE_STEP_NUMBER", 0,
                                       IVI_VAL_MULTI_CHANNEL, VI_NULL, VI_NULL,
                                       &attrSequenceStepNumberRangeTable));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_PULS_START_LEVEL,
                                        "ZUP366_ATTR_PULS_START_LEVEL", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrPulsStartLevel_ReadCallback,
                                        zup366AttrPulsStartLevel_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_PULS_START_LEVEL,
                                                zup366AttrPulsStartLevel_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_PULS_AMPL_TLEV,
                                        "ZUP366_ATTR_PULS_AMPL_TLEV", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrPulsAmplTlev_ReadCallback,
                                        zup366AttrPulsAmplTlev_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_PULS_AMPL_TLEV,
                                                zup366AttrPulsAmplTlev_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_PULS_START_TIME,
                                        "ZUP366_ATTR_PULS_START_TIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrPulsStartTime_ReadCallback,
                                        zup366AttrPulsStartTime_WriteCallback,
                                        &attrStartTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_PULS_WIDTH_TTIME,
                                        "ZUP366_ATTR_PULS_WIDTH_TTIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrPulsWidthTtime_ReadCallback,
                                        zup366AttrPulsWidthTtime_WriteCallback,
                                        &attrWidthRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_PULS_END_TIME,
                                        "ZUP366_ATTR_PULS_END_TIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrPulsEndTime_ReadCallback,
                                        zup366AttrPulsEndTime_WriteCallback,
                                        &attrEndTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_RAMP_START_LEVEL,
                                        "ZUP366_ATTR_RAMP_START_LEVEL", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrRampStartLevel_ReadCallback,
                                        zup366AttrRampStartLevel_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_RAMP_START_LEVEL,
                                                zup366AttrRampStartLevel_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_RAMP_END_LEVEL,
                                        "ZUP366_ATTR_RAMP_END_LEVEL", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrRampEndLevel_ReadCallback,
                                        zup366AttrRampEndLevel_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_RAMP_END_LEVEL,
                                                zup366AttrRampEndLevel_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_RAMP_START_TIME,
                                        "ZUP366_ATTR_RAMP_START_TIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrRampStartTime_ReadCallback,
                                        zup366AttrRampStartTime_WriteCallback,
                                        &attrStartTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_RAMP_END_TIME,
                                        "ZUP366_ATTR_RAMP_END_TIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrRampEndTime_ReadCallback,
                                        zup366AttrRampEndTime_WriteCallback,
                                        &attrEndTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_RAMP_RISE_TIME,
                                        "ZUP366_ATTR_RAMP_RISE_TIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrRampRiseTime_ReadCallback,
                                        zup366AttrRampRiseTime_WriteCallback,
                                        &attrRiseTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_AMPLITUDE,
                                        "ZUP366_ATTR_AMPLITUDE", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrAmplitude_ReadCallback,
                                        zup366AttrAmplitude_WriteCallback, VI_NULL,
                                        0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_AMPLITUDE,
                                                zup366AttrAmplitude_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_OFFSET,
                                        "ZUP366_ATTR_OFFSET", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrOffset_ReadCallback,
                                        zup366AttrOffset_WriteCallback, VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_OFFSET,
                                                zup366AttrOffset_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_FREQUENCY,
                                        "ZUP366_ATTR_FREQUENCY", 1,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrFrequency_ReadCallback,
                                        zup366AttrFrequency_WriteCallback,
                                        &attrFrequencyRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_STAIR_START_LEVEL,
                                        "ZUP366_ATTR_STAIR_START_LEVEL", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrStairStartLevel_ReadCallback,
                                        zup366AttrStairStartLevel_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_STAIR_START_LEVEL,
                                                zup366AttrStairStartLevel_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_STAIR_END_LEVEL,
                                        "ZUP366_ATTR_STAIR_END_LEVEL", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrStairEndLevel_ReadCallback,
                                        zup366AttrStairEndLevel_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_STAIR_END_LEVEL,
                                                zup366AttrStairEndLevel_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_STAIR_START_TIME,
                                        "ZUP366_ATTR_STAIR_START_TIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrStairStartTime_ReadCallback,
                                        zup366AttrStairStartTime_WriteCallback,
                                        &attrStartTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_STAIR_END_TIME,
                                        "ZUP366_ATTR_STAIR_END_TIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrStairEndTime_ReadCallback,
                                        zup366AttrStairEndTime_WriteCallback,
                                        &attrEndTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_STAIR_TOTAL_TIME,
                                        "ZUP366_ATTR_STAIR_TOTAL_TIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrStairTotalTime_ReadCallback,
                                        zup366AttrStairTotalTime_WriteCallback,
                                        &attrTotalTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_STAIR_NSTEPS,
                                       "ZUP366_ATTR_STAIR_NSTEPS", 10,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366AttrStairNsteps_ReadCallback,
                                       zup366AttrStairNsteps_WriteCallback,
                                       &attrStairNstepsRangeTable));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_STEP_START_LEVEL,
                                        "ZUP366_ATTR_STEP_START_LEVEL", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrStepStartLevel_ReadCallback,
                                        zup366AttrStepStartLevel_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_STEP_START_LEVEL,
                                                zup366AttrStepStartLevel_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_STEP_END_LEVEL,
                                        "ZUP366_ATTR_STEP_END_LEVEL", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrStepEndLevel_ReadCallback,
                                        zup366AttrStepEndLevel_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_STEP_END_LEVEL,
                                                zup366AttrStepEndLevel_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_STEP_START_TIME,
                                        "ZUP366_ATTR_STEP_START_TIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrStepStartTime_ReadCallback,
                                        zup366AttrStepStartTime_WriteCallback,
                                        &attrStartTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_STEP_END_TIME,
                                        "ZUP366_ATTR_STEP_END_TIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrStepEndTime_ReadCallback,
                                        zup366AttrStepEndTime_WriteCallback,
                                        &attrEndTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_TRAP_START_LEVEL,
                                        "ZUP366_ATTR_TRAP_START_LEVEL", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrTrapStartLevel_ReadCallback,
                                        zup366AttrTrapStartLevel_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_TRAP_START_LEVEL,
                                                zup366AttrTrapStartLevel_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_TRAP_PEAK_TLEV,
                                        "ZUP366_ATTR_TRAP_PEAK_TLEV", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrTrapPeakTlev_ReadCallback,
                                        zup366AttrTrapPeakTlev_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_TRAP_PEAK_TLEV,
                                                zup366AttrTrapPeakTlev_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_TRAP_START_TIME,
                                        "ZUP366_ATTR_TRAP_START_TIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrTrapStartTime_ReadCallback,
                                        zup366AttrTrapStartTime_WriteCallback,
                                        &attrStartTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_TRAP_END_TIME,
                                        "ZUP366_ATTR_TRAP_END_TIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrTrapEndTime_ReadCallback,
                                        zup366AttrTrapEndTime_WriteCallback,
                                        &attrEndTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_TRAP_WIDTH_TTIME,
                                        "ZUP366_ATTR_TRAP_WIDTH_TTIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrTrapWidthTtime_ReadCallback,
                                        zup366AttrTrapWidthTtime_WriteCallback,
                                        &attrWidthRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_TRAP_FALL_TIME,
                                        "ZUP366_ATTR_TRAP_FALL_TIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrTrapFallTime_ReadCallback,
                                        zup366AttrTrapFallTime_WriteCallback,
                                        &attrTrapezoidFallTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_TRAP_RISE_TIME,
                                        "ZUP366_ATTR_TRAP_RISE_TIME", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrTrapRiseTime_ReadCallback,
                                        zup366AttrTrapRiseTime_WriteCallback,
                                        &attrRiseTimeRangeTable, 0));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_SEQUENCE_SHAPE,
                                       "ZUP366_ATTR_SEQUENCE_SHAPE",
                                       ZUP366_VAL_SEQUENCE_PULSE,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366AttrSequenceShape_ReadCallback,
                                       zup366AttrSequenceShape_WriteCallback,
                                       &attrSequenceShapeRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_SEQUENCE_MEASUREMENT,
                                       "ZUP366_ATTR_SEQUENCE_MEASUREMENT",
                                       ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE,
                                       IVI_VAL_MULTI_CHANNEL, VI_NULL, VI_NULL,
                                       &attrSequenceMeasurementRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_SEQUENCE_PACING,
                                       "ZUP366_ATTR_SEQUENCE_PACING",
                                       ZUP366_VAL_SEQUENCE_PACING_TYPE_DWELL,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366AttrSequencePacing_ReadCallback,
                                       zup366AttrSequencePacing_WriteCallback,
                                       &attrSequencePacingRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_SEQUENCE_STEP_COUNT,
                                       "ZUP366_ATTR_SEQUENCE_STEP_COUNT", 0,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366AttrSequenceStepCount_ReadCallback,
                                       zup366AttrSequenceStepCount_WriteCallback,
                                       &attrSequenceStepCountRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_SEQUENCE_COUNT,
                                       "ZUP366_ATTR_SEQUENCE_COUNT", 0,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366AttrSequenceCount_ReadCallback,
                                       zup366AttrSequenceCount_WriteCallback,
                                       &attrSequenceCountRangeTable));
    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_SEQUENCE_TERMINATION,
                                         "ZUP366_ATTR_SEQUENCE_TERMINATION",
                                         VI_FALSE, IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));
    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_DATALOG_ENABLED,
                                         "ZUP366_ATTR_DATALOG_ENABLED", VI_FALSE,
                                         IVI_VAL_MULTI_CHANNEL,
                                         zup366AttrDatalogEnabled_ReadCallback,
                                         zup366AttrDatalogEnabled_WriteCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_DATALOG_PERIOD,
                                        "ZUP366_ATTR_DATALOG_PERIOD", 0.1, 0,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback,
                                        &attrDatalogPeriodRangeTable, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_DATALOG_PERIOD,
                                                zup366AttrDatalogPeriod_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_DATALOG_DURATION,
                                        "ZUP366_ATTR_DATALOG_DURATION", 30, 0,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback,
                                        &attrDatalogDurationRangeTable, 0));
    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_DATALOG_AUTO_RANGE,
                                         "ZUP366_ATTR_DATALOG_AUTO_RANGE", 0,
                                         IVI_VAL_MULTI_CHANNEL,
                                         zup366AttrDatalogAutoRange_ReadCallback,
                                         zup366AttrDatalogAutoRange_WriteCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_DATALOG_RANGE,
                                        "ZUP366_ATTR_DATALOG_RANGE", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrDatalogRange_ReadCallback,
                                        zup366AttrDatalogRange_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_DATALOG_MARKER1,
                                        "ZUP366_ATTR_DATALOG_MARKER1", 0, 0,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback,
                                        &attrDatalogMarker1PointsRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_DATALOG_MARKER2,
                                        "ZUP366_ATTR_DATALOG_MARKER2", 0, 0,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback,
                                        &attrDatalogMarker2PointsRangeTable, 0));
    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_HISTOGRAM_ENABLED,
                                         "ZUP366_ATTR_HISTOGRAM_ENABLED", VI_FALSE,
                                         IVI_VAL_MULTI_CHANNEL,
                                         zup366AttrHistogramEnabled_ReadCallback,
                                         zup366AttrHistogramEnabled_WriteCallback));
    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_HISTOGRAM_AUTO_RANGE,
                                         "ZUP366_ATTR_HISTOGRAM_AUTO_RANGE", 0,
                                         IVI_VAL_MULTI_CHANNEL,
                                         zup366AttrHistogramAutoRange_ReadCallback,
                                         zup366AttrHistogramAutoRange_WriteCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_HISTOGRAM_RANGE,
                                        "ZUP366_ATTR_HISTOGRAM_RANGE", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback, VI_NULL, 0));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_TRIGGER_WFM_SRC,
                                       "ZUP366_ATTR_TRIGGER_WFM_SRC",
                                       ZUP366_VAL_TRIG_WFM_IMMEDIATE, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrTriggerWFMSourceRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DATALOG_OFFSET,
                                       "ZUP366_ATTR_DATALOG_OFFSET", 0, 0,
                                       zup366ViInt32_ReadCallback,
                                       zup366ViInt32_WriteCallback,
                                       &attrDatalogOffsetRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DATALOG_TRIGGER_SRC,
                                       "ZUP366_ATTR_DATALOG_TRIGGER_SRC",
                                       ZUP366_VAL_TRIG_DATALOG_IMMEDIATE, 0,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrDatalogTriggerSourceRangeTable));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_DATALOG_TRIGGER_LEVEL,
                                        "ZUP366_ATTR_DATALOG_TRIGGER_LEVEL", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrDatalogTriggerLevel_ReadCallback,
                                        zup366AttrDatalogTriggerLevel_WriteCallback,
                                        VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi,
                                                ZUP366_ATTR_DATALOG_TRIGGER_LEVEL,
                                                zup366AttrDatalogTriggerLevel_CheckCallback));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DATALOG_TRIGGER_SLOPE,
                                       "ZUP366_ATTR_DATALOG_TRIGGER_SLOPE",
                                       ZUP366_VAL_DATALOG_SLOPE_POSITIVE,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366AttrDatalogTriggerSlope_ReadCallback,
                                       zup366AttrDatalogTriggerSlope_WriteCallback,
                                       &attrDatalogSlopeRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_DATALOG_MEASUREMENT,
                                       "ZUP366_ATTR_DATALOG_MEASUREMENT", 0,
                                       IVI_VAL_MULTI_CHANNEL, VI_NULL, VI_NULL,
                                       &attrDatalogMeasurementRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_HISTOGRAM_TRIGGER_SRC,
                                       "ZUP366_ATTR_HISTOGRAM_TRIGGER_SRC",
                                       ZUP366_VAL_HISTOGRAM_BUS_TRIG,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrHistogramTriggerSourceRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_SMU_OUTPUT_IMPEDANCE,
                                       "ZUP366_ATTR_SMU_OUTPUT_IMPEDANCE",
                                       ZUP366_VAL_SMU_HIGH_IMPEDANCE,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrSMUOutputImpedanceRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_SMU_VOLTAGE_BANDWIDTH,
                                       "ZUP366_ATTR_SMU_VOLTAGE_BANDWIDTH",
                                       ZUP366_VAL_SMU_LOW_BANDWIDTH,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrSMUBandwidthRangeTable));
    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_SMU_ENABLE_OSCILLATION,
                                         "ZUP366_ATTR_SMU_ENABLE_OSCILLATION", 0,
                                         IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));
    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_SMU_ENABLE_RESISTANCE,
                                         "ZUP366_ATTR_SMU_ENABLE_RESISTANCE", 0,
                                         IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_SMU_RESISTANCE_LEVEL,
                                        "ZUP366_ATTR_SMU_RESISTANCE_LEVEL", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback,
                                        &attrResistanceRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_SMU_POSITIVE_VOLTAGE_LIMIT,
                                        "ZUP366_ATTR_SMU_POSITIVE_VOLTAGE_LIMIT", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback,
                                        &attrVoltageLimitPositiveRangeTable, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi,
                                                ZUP366_ATTR_SMU_POSITIVE_VOLTAGE_LIMIT,
                                                zup366AttrSmuPositiveVoltageLimit_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_SMU_NEGATIVE_VOLTAGE_LIMIT,
                                        "ZUP366_ATTR_SMU_NEGATIVE_VOLTAGE_LIMIT", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback,
                                        &attrVoltageLimitNegativeRangeTable, 0));
    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_SMU_VOLTAGE_LIMIT_TRACKING,
                                         "ZUP366_ATTR_SMU_VOLTAGE_LIMIT_TRACKING",
                                         0, IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));
    ;
    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_SMU_CURRENT_LIMIT_TRACKING,
                                         "ZUP366_ATTR_SMU_CURRENT_LIMIT_TRACKING",
                                         0, IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi,
                                        ZUP366_ATTR_SMU_ASM_POSTIVE_CURRENT_LIMIT,
                                        "ZUP366_ATTR_SMU_ASM_POSTIVE_CURRENT_LIMIT",
                                        0, IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback,
                                        &attrCurrentLimitPositiveRangeTable, 0));
    checkErr (Ivi_AddAttributeViReal64 (vi,
                                        ZUP366_ATTR_SMU_ASM_NEGATIVE_CURRENT_LIMIT,
                                        "ZUP366_ATTR_SMU_ASM_NEGATIVE_CURRENT_LIMIT",
                                        0, IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback, VI_NULL, 0));
    checkErr (Ivi_SetAttrRangeTableCallback (vi,
                                             ZUP366_ATTR_SMU_ASM_NEGATIVE_CURRENT_LIMIT,
                                             zup366AttrSmuAsmNegativeCurrentLimit_RangeTableCallback));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_SMU_POSITIVE_OVRP,
                                       "ZUP366_ATTR_SMU_POSITIVE_OVRP", 0,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366ViInt32_ReadCallback,
                                       zup366ViInt32_WriteCallback,
                                       &attrSmuPositiveOvrpRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_SMU_NEGATIVE_OVRP,
                                       "ZUP366_ATTR_SMU_NEGATIVE_OVRP", 0,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366ViInt32_ReadCallback,
                                       zup366ViInt32_WriteCallback,
                                       &attrSmuNegativeOvrpRangeTable));

    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_ASM_OVER_PROTECTION_DELAY,
                                        "ZUP366_ATTR_ASM_OVER_PROTECTION_DELAY", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback,
                                        &attrOverProtectionDelayRangeTable, 0));

    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_SMU_CURRENT_SLEW_RATE,
                                        "ZUP366_ATTR_SMU_CURRENT_SLEW_RATE",
                                        9.9E+37, IVI_VAL_MULTI_CHANNEL,
                                        zup366AttrSmuCurrentSlewRate_ReadCallback,
                                        zup366AttrSmuCurrentSlewRate_WriteCallback,
                                        &attrCurrentSlewRateRangeTable, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi,
                                                ZUP366_ATTR_SMU_CURRENT_SLEW_RATE,
                                                zup366AttrSmuCurrentSlewRate_CheckCallback));
    checkErr (Ivi_AddAttributeViInt32 (vi,
                                       ZUP366_ATTR_SMU_OUTPUT_REGULATION_PRIORITY,
                                       "ZUP366_ATTR_SMU_OUTPUT_REGULATION_PRIORITY",
                                       0, IVI_VAL_MULTI_CHANNEL,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrOutputRegulationRangeTable));
    checkErr (Ivi_AddAttributeViBoolean (vi,
                                         ZUP366_ATTR_ENABLE_SIMULTANEOUS_CURR_MEASUREMENT,
                                         "ZUP366_ATTR_ENABLE_SIMULTANEOUS_CURR_MEASUREMENT",
                                         VI_FALSE, IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));
    checkErr (Ivi_AddAttributeViBoolean (vi,
                                         ZUP366_ATTR_ENABLE_SIMULTANEOUS_VOLT_MEASUREMENT,
                                         "ZUP366_ATTR_ENABLE_SIMULTANEOUS_VOLT_MEASUREMENT",
                                         VI_FALSE, IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));
    checkErr (Ivi_SetAttrCheckCallbackViInt32 (vi, ZUP366_ATTR_DATALOG_OFFSET,
                                               zup366AttrDatalogOffset_CheckCallback));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_HISTOGRAM_RANGE,
                                                zup366AttrHistogramRange_CheckCallback));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_DATALOG_RANGE,
                                                zup366AttrDatalogRange_CheckCallback));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_DATALOG_DURATION,
                                                zup366AttrDatalogDuration_CheckCallback));

    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_AUTO_ELOG_CURRENT_RANGE,
                                         "ZUP366_ATTR_AUTO_ELOG_CURRENT_RANGE", VI_FALSE,
                                         IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));
    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_AUTO_ELOG_VOLTAGE_RANGE,
                                         "ZUP366_ATTR_AUTO_ELOG_VOLTAGE_RANGE", VI_FALSE,
                                         IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_ELOG_CURRENT_RANGE,
                                        "ZUP366_ATTR_ELOG_CURRENT_RANGE", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback, VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_ELOG_CURRENT_RANGE,
                                                zup366ElogCurrentRange_CheckCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_ELOG_VOLTAGE_RANGE,
                                        "ZUP366_ATTR_ELOG_VOLTAGE_RANGE", 0,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback, VI_NULL, 0));
    checkErr (Ivi_SetAttrCheckCallbackViReal64 (vi, ZUP366_ATTR_ELOG_VOLTAGE_RANGE,
                                                zup366ElogVoltageRange_CheckCallback));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_ELOG_TRIGGER_SOURCE,
                                       "ZUP366_ATTR_ELOG_TRIGGER_SOURCE", ZUP366_VAL_TRIG_BUS,
                                       IVI_VAL_MULTI_CHANNEL,
                                       zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrElogTriggerSourceRangeTable));
    checkErr (Ivi_AddAttributeViInt32 (vi, ZUP366_ATTR_MEASUREMENT_RESOLUTION,
                                       "ZUP366_ATTR_MEASUREMENT_RESOLUTION", ZUP366_VAL_RESOLUTION_20MS,
                                       0, zup366Enum_ReadCallback,
                                       zup366Enum_WriteCallback,
                                       &attrMeasurementResolutionRangeTable));
    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_ENABLE_CURRENT_ELOG,
                                         "ZUP366_ATTR_ENABLE_CURRENT_ELOG", VI_FALSE,
                                         IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));
    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_ENABLE_VOLTAGE_ELOG,
                                         "ZUP366_ATTR_ENABLE_VOLTAGE_ELOG", VI_FALSE,
                                         IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));
    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_ENABLE_CURRENT_MINMAX_ELOG,
                                         "ZUP366_ATTR_ENABLE_CURRENT_MINMAX_ELOG", VI_FALSE,
                                         IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));
    checkErr (Ivi_AddAttributeViBoolean (vi, ZUP366_ATTR_ENABLE_VOLTAGE_MINMAX_ELOG,
                                         "ZUP366_ATTR_ENABLE_VOLTAGE_MINMAX_ELOG", VI_FALSE,
                                         IVI_VAL_MULTI_CHANNEL,
                                         zup366ViBoolean_ReadCallback,
                                         zup366ViBoolean_WriteCallback));
    checkErr (Ivi_AddAttributeViReal64 (vi, ZUP366_ATTR_ELOG_PERIOD,
                                        "ZUP366_ATTR_ELOG_PERIOD", 0.1,
                                        IVI_VAL_MULTI_CHANNEL,
                                        zup366ViReal64_ReadCallback,
                                        zup366ViReal64_WriteCallback,
                                        &attrExternalLogPeriodRangeTable, 0));



    /*- Setup attribute invalidations -----------------------------------*/


Error:
    return error;
}

/*****************************************************************************
 *------------------- End Instrument Driver Source Code ---------------------*
 *****************************************************************************/
