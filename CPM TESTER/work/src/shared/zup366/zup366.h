/****************************************************************************
 *                       Keysight/Agilent N6700 DC Power Supply
 *---------------------------------------------------------------------------
 *   2004-2015, National Instruments, Corporation.  All Rights Reserved.
 *---------------------------------------------------------------------------
 *
 * Title:    zup366.h
 * Purpose:  Keysight/Agilent N6700 DC Power Supply
 *           instrument driver declarations.
 *
 ****************************************************************************/

#ifndef __ZUP366_HEADER
#define __ZUP366_HEADER

#include <ivi.h>
#include <ividcpwr.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/****************************************************************************
 *----------------- Instrument Driver Revision Information -----------------*
 ****************************************************************************/
#define ZUP366_MAJOR_VERSION                  3  /* Instrument driver major version   */
#define ZUP366_MINOR_VERSION                  2  /* Instrument driver minor version   */

#define ZUP366_CLASS_SPEC_MAJOR_VERSION       3  /* Class specification major version */
#define ZUP366_CLASS_SPEC_MINOR_VERSION       0  /* Class specification minor version */

#define ZUP366_SUPPORTED_INSTRUMENT_MODELS    "N6700 Series"
#define ZUP366_DRIVER_VENDOR                  "National Instruments"
#ifdef  _IVI_mswin64_
#define ZUP366_DRIVER_DESCRIPTION             "Keysight/Agilent N6700 Modular Power System [Compiled for 64-bit.]"
#else
#define ZUP366_DRIVER_DESCRIPTION             "Keysight/Agilent N6700 Modular Power System"
#endif

/****************************************************************************
 *------------------------------ Useful Macros -----------------------------*
 ****************************************************************************/

/*- Defined values for rangeType parameter of function -----------------*/
/*- ConfigureOutputRange -----------------------------------------------*/
#define ZUP366_VAL_RANGE_CURRENT    IVIDCPWR_VAL_RANGE_CURRENT
#define ZUP366_VAL_RANGE_VOLTAGE    IVIDCPWR_VAL_RANGE_VOLTAGE


/*- Defined values for outputState parameter of function ---------------*/
/*- QueryOutputState ---------------------------------------------------*/
#define ZUP366_VAL_OUTPUT_CONSTANT_VOLTAGE    IVIDCPWR_VAL_OUTPUT_CONSTANT_VOLTAGE
#define ZUP366_VAL_OUTPUT_CONSTANT_CURRENT    IVIDCPWR_VAL_OUTPUT_CONSTANT_CURRENT
#define ZUP366_VAL_OUTPUT_OVER_VOLTAGE        IVIDCPWR_VAL_OUTPUT_OVER_VOLTAGE
#define ZUP366_VAL_OUTPUT_OVER_CURRENT        IVIDCPWR_VAL_OUTPUT_OVER_CURRENT
#define ZUP366_VAL_OUTPUT_UNREGULATED         IVIDCPWR_VAL_OUTPUT_UNREGULATED
#define ZUP366_VAL_OUTPUT_OVER_TEMPERATURE    (IVIDCPWR_VAL_OUTPUT_STATE_SPECIFIC_EXT_BASE + 1L)

/*- Defined values for measurementType parameter of function -----------*/
/*- Measure ------------------------------------------------------------*/
/* ZUP366_ATTR_MEASUREMENT_FUNCTION -----------------------------------*/
#define ZUP366_VAL_MEASURE_CURRENT    IVIDCPWR_VAL_MEASURE_CURRENT
#define ZUP366_VAL_MEASURE_VOLTAGE    IVIDCPWR_VAL_MEASURE_VOLTAGE
#define ZUP366_VAL_MEASURE_POWER      (IVIDCPWR_VAL_MEASURE_SPECIFIC_EXT_BASE + 1L)


/* Defined values for maxTime parameter to the measure functions */
#define ZUP366_VAL_MAX_TIME_INFINITE     IVI_VAL_MAX_TIME_INFINITE
#define ZUP366_VAL_MAX_TIME_IMMEDIATE    IVI_VAL_MAX_TIME_IMMEDIATE

/****************************************************************************
 *---------------------------- Attribute Defines ---------------------------*
 ****************************************************************************/

/*- IVI Inherent Instrument Attributes ---------------------------------*/

/* User Options */
#define ZUP366_ATTR_RANGE_CHECK                                 IVI_ATTR_RANGE_CHECK             /* ViBoolean */
#define ZUP366_ATTR_QUERY_INSTRUMENT_STATUS                     IVI_ATTR_QUERY_INSTRUMENT_STATUS /* ViBoolean */
#define ZUP366_ATTR_CACHE                                       IVI_ATTR_CACHE                   /* ViBoolean */
#define ZUP366_ATTR_SIMULATE                                    IVI_ATTR_SIMULATE                /* ViBoolean */
#define ZUP366_ATTR_RECORD_COERCIONS                            IVI_ATTR_RECORD_COERCIONS        /* ViBoolean */
#define ZUP366_ATTR_INTERCHANGE_CHECK                           IVI_ATTR_INTERCHANGE_CHECK       /* ViBoolean */

/* Driver Information  */
#define ZUP366_ATTR_SPECIFIC_DRIVER_PREFIX                      IVI_ATTR_SPECIFIC_DRIVER_PREFIX                   /* ViString, read-only  */
#define ZUP366_ATTR_SUPPORTED_INSTRUMENT_MODELS                 IVI_ATTR_SUPPORTED_INSTRUMENT_MODELS              /* ViString, read-only  */
#define ZUP366_ATTR_GROUP_CAPABILITIES                          IVI_ATTR_GROUP_CAPABILITIES                       /* ViString, read-only  */
#define ZUP366_ATTR_INSTRUMENT_MANUFACTURER                     IVI_ATTR_INSTRUMENT_MANUFACTURER                  /* ViString, read-only  */
#define ZUP366_ATTR_INSTRUMENT_MODEL                            IVI_ATTR_INSTRUMENT_MODEL                         /* ViString, read-only  */
#define ZUP366_ATTR_INSTRUMENT_FIRMWARE_REVISION                IVI_ATTR_INSTRUMENT_FIRMWARE_REVISION             /* ViString, read-only  */
#define ZUP366_ATTR_SPECIFIC_DRIVER_REVISION                    IVI_ATTR_SPECIFIC_DRIVER_REVISION                 /* ViString, read-only  */
#define ZUP366_ATTR_SPECIFIC_DRIVER_VENDOR                      IVI_ATTR_SPECIFIC_DRIVER_VENDOR                   /* ViString, read-only  */
#define ZUP366_ATTR_SPECIFIC_DRIVER_DESCRIPTION                 IVI_ATTR_SPECIFIC_DRIVER_DESCRIPTION              /* ViString, read-only  */
#define ZUP366_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION    IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION /* ViInt32, read-only */
#define ZUP366_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION    IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION /* ViInt32, read-only */

/* Advanced Session Information */
#define ZUP366_ATTR_LOGICAL_NAME                                IVI_ATTR_LOGICAL_NAME           /* ViString, read-only  */
#define ZUP366_ATTR_IO_RESOURCE_DESCRIPTOR                      IVI_ATTR_IO_RESOURCE_DESCRIPTOR /* ViString, read-only  */
#define ZUP366_ATTR_DRIVER_SETUP                                IVI_ATTR_DRIVER_SETUP           /* ViString, read-only  */
/*- Instrument-Specific Attributes -------------------------------------*/
/*- Basic Instrument Operations ----------------------------------------*/
#define ZUP366_ATTR_VOLTAGE_LEVEL                               IVIDCPWR_ATTR_VOLTAGE_LEVEL          /* ViReal64,  multi-channel, volts */
#define ZUP366_ATTR_CHANNEL_COUNT                               IVI_ATTR_CHANNEL_COUNT               /* ViInt32,  read-only  */
#define ZUP366_ATTR_OVP_ENABLED                                 IVIDCPWR_ATTR_OVP_ENABLED            /* ViBoolean, multi-channel        */
#define ZUP366_ATTR_OVP_LIMIT                                   IVIDCPWR_ATTR_OVP_LIMIT              /* ViReal64,  multi-channel, volts */
#define ZUP366_ATTR_CURRENT_LIMIT_BEHAVIOR                      IVIDCPWR_ATTR_CURRENT_LIMIT_BEHAVIOR /* ViInt32,   multi-channel        */
#define ZUP366_ATTR_CURRENT_LIMIT                               IVIDCPWR_ATTR_CURRENT_LIMIT          /* ViReal64,  multi-channel, amps  */
#define ZUP366_ATTR_OUTPUT_ENABLED                              IVIDCPWR_ATTR_OUTPUT_ENABLED         /* ViBoolean, multi-channel        */

/*- Trigger Attributes -------------------------------------------------*/
#define ZUP366_ATTR_TRIGGER_SOURCE                              IVIDCPWR_ATTR_TRIGGER_SOURCE          /* ViInt32,   multi-channel        */
#define ZUP366_ATTR_TRIGGERED_CURRENT_LIMIT                     IVIDCPWR_ATTR_TRIGGERED_CURRENT_LIMIT /* ViReal64,  multi-channel, amps  */
#define ZUP366_ATTR_TRIGGERED_VOLTAGE_LEVEL                     IVIDCPWR_ATTR_TRIGGERED_VOLTAGE_LEVEL /* ViReal64,  multi-channel, volts */

#define ZUP366_ATTR_ID_QUERY_RESPONSE                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1L)  /* ViString (Read Only) */

/*- List Attributes ----------------------------------------------------*/
#define ZUP366_ATTR_LIST_COUNT                                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2L)   /* ViInt32 */
#define ZUP366_ATTR_LIST_STEP                                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3L)   /* ViInt32,   multi-channel */
#define ZUP366_ATTR_LIST_TERMINATE_LAST                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 4L)   /* ViBoolean, multi-channel */
#define ZUP366_ATTR_LIST_VOLTAGE_POINTS                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 5L)   /* ViInt32,   multi-channel, read-only */
#define ZUP366_ATTR_LIST_CURRENT_POINTS                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 6L)   /* ViInt32,   multi-channel, read-only */
#define ZUP366_ATTR_LIST_DWELL_POINTS                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7L)   /* ViInt32,   multi-channel, read-only */
#define ZUP366_ATTR_LIST_TRIGGER_OUTPUT_BOST_POINTS             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8L)   /* ViInt32,   multi-channel, read-only */
#define ZUP366_ATTR_LIST_TRIGGER_OUTPUT_EOST_POINTS             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9L)   /* ViInt32,   multi-channel, read-only */

#define ZUP366_ATTR_LIST_TRIGGER_OUTPUT_ENABLED                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10L)  /* ViBoolean, multi-channel */

#define ZUP366_ATTR_TRIGGER_CURRENT_MODE                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 11L)  /* ViInt32, multi-channel  */
#define ZUP366_ATTR_TRIGGER_VOLTAGE_MODE                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 12L)  /* ViInt32, multi-channel  */

#define ZUP366_ATTR_OUTPUT_PROTECTION_DELAY                     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13L)  /* ViReal64, multi-channel */

#define ZUP366_ATTR_SWEEP_POINTS                                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 14L)  /* ViInt32, multi-channel  */
#define ZUP366_ATTR_SWEEP_TIME_INTERVAL                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 15L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_MEASUREMENT_WINDOW                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 16L)  /* ViInt32, multi-channel  */
#define ZUP366_ATTR_SWEEP_OFFSET_POINTS                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 17L)  /* ViInt32, multi-channel  */

#define ZUP366_ATTR_MEASUREMENT_FUNCTION                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19L)  /* ViInt32, multi-channel */
#define ZUP366_ATTR_OUTPUT_PROTECTION_COUPLED                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 18L)  /* ViBoolean */

#define ZUP366_ATTR_ACQUISITION_TRIGGER_SOURCE                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 20L)  /* ViInt32, multi-channel */

#define ZUP366_ATTR_OUTPUT_DELAY_FALL                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 21L)  /* ViInt32, multi-channel */
#define ZUP366_ATTR_OUTPUT_DELAY_RISE                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 22L)  /* ViInt32, multi-channel */
#define ZUP366_ATTR_OUTPUT_INHIBIT_MODE                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 23L)  /* ViInt32 */

#define ZUP366_ATTR_DIGITAL_INPUT_DATA                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 24L)  /* ViInt32 */
#define ZUP366_ATTR_DIGITAL_OUTPUT_DATA                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 25L)  /* ViInt32 */
#define ZUP366_ATTR_DIGITAL_PIN1_FUNCTION                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 26L)  /* ViInt32 */
#define ZUP366_ATTR_DIGITAL_PIN2_FUNCTION                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 27L)  /* ViInt32 */
#define ZUP366_ATTR_DIGITAL_PIN3_FUNCTION                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 28L)  /* ViInt32 */
#define ZUP366_ATTR_DIGITAL_PIN1_POLARITY                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 29L)  /* ViInt32 */
#define ZUP366_ATTR_DIGITAL_PIN2_POLARITY                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 30L)  /* ViInt32 */
#define ZUP366_ATTR_DIGITAL_PIN3_POLARITY                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 31L)  /* ViInt32 */
#define ZUP366_ATTR_DIGITAL_PIN4_FUNCTION                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 132L) /* ViInt32 */
#define ZUP366_ATTR_DIGITAL_PIN5_FUNCTION                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 133L) /* ViInt32 */
#define ZUP366_ATTR_DIGITAL_PIN6_FUNCTION                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 134L) /* ViInt32 */
#define ZUP366_ATTR_DIGITAL_PIN7_FUNCTION                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 135L) /* ViInt32 */
#define ZUP366_ATTR_DIGITAL_PIN4_POLARITY                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 136L) /* ViInt32 */
#define ZUP366_ATTR_DIGITAL_PIN5_POLARITY                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 137L) /* ViInt32 */
#define ZUP366_ATTR_DIGITAL_PIN6_POLARITY                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 138L) /* ViInt32 */
#define ZUP366_ATTR_DIGITAL_PIN7_POLARITY                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 139L) /* ViInt32 */

#define ZUP366_ATTR_INITIATE_CONTINUOUS                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 32L)  /* ViBoolean */

#define ZUP366_ATTR_GROUP_CHANNELS                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 40L)  /* ViString */
#define ZUP366_ATTR_UNGROUP_CHANNEL                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 41L)  /* ViString */
#define ZUP366_ATTR_RELAY_POLARITY                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 42L)  /* ViInt32 */
#define ZUP366_ATTR_POWER_LIMIT                                 (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 43L)  /* ViReal64, multi-channel */


#define ZUP366_ATTR_WAVEFORM_SHAPE                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 44L)  /* ViInt32,   multi-channel */ /* Waveform Attributes */
#define ZUP366_ATTR_WAVEFORM_MEASUREMENT                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 45L)  /* ViInt32,   multi-channel */
#define ZUP366_ATTR_WAVEFORM_COUNT                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 46L)  /* ViInt32,   multi-channel */
#define ZUP366_ATTR_WAVEFORM_TERMINATION                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 47L)  /* ViBoolean, multi-channel */
#define ZUP366_ATTR_SEQUENCE_STEP_NUMBER                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 53L)  /* ViInt32 */

#define ZUP366_ATTR_EXP_TOTAL_TIME                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 48L)  /* ViReal64,  multi-channel */ /*Exponential Waveform Attributes*/
#define ZUP366_ATTR_EXP_TIME_CONSTANT                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 49L)  /* ViReal64,  multi-channel */
#define ZUP366_ATTR_EXP_START_TIME                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 50L)  /* ViReal64,  multi-channel */
#define ZUP366_ATTR_EXP_END_LEVEL                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 51L)  /* ViReal64,  multi-channel */
#define ZUP366_ATTR_EXP_START_LEVEL                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 52L)  /* ViReal64,  multi-channel */

#define ZUP366_ATTR_PULS_START_LEVEL                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 54L)  /* ViReal64, multi-channel */   /*Pulse Waveform Attributes*/
#define ZUP366_ATTR_PULS_AMPL_TLEV                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 55L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_PULS_START_TIME                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 56L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_PULS_WIDTH_TTIME                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 57L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_PULS_END_TIME                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 58L)  /* ViReal64, multi-channel */

#define ZUP366_ATTR_RAMP_START_LEVEL                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 59L)  /* ViReal64, multi-channel */   /*Ramp Waveform Attributes*/
#define ZUP366_ATTR_RAMP_END_LEVEL                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 60L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_RAMP_START_TIME                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 61L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_RAMP_END_TIME                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 62L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_RAMP_RISE_TIME                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 63L)  /* ViReal64, multi-channel */

#define ZUP366_ATTR_AMPLITUDE                                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 64L)  /* ViReal64, multi-channel */   /*Sinusoid Waveform Attributes*/
#define ZUP366_ATTR_OFFSET                                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 65L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_FREQUENCY                                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 66L)  /* ViReal64, multi-channel */

#define ZUP366_ATTR_STAIR_START_LEVEL                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 67L)  /* ViReal64, multi-channel */   /*Stair Waveform Attributes*/
#define ZUP366_ATTR_STAIR_END_LEVEL                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 68L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_STAIR_START_TIME                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 69L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_STAIR_END_TIME                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 70L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_STAIR_TOTAL_TIME                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 71L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_STAIR_NSTEPS                                (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 72L)  /* ViInt32, multi-channel  */

#define ZUP366_ATTR_STEP_START_LEVEL                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 73L)  /* ViReal64, multi-channel */   /*Step Waveform Attributes*/
#define ZUP366_ATTR_STEP_END_LEVEL                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 74L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_STEP_START_TIME                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 75L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_STEP_END_TIME                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 76L)  /* ViReal64, multi-channel */

#define ZUP366_ATTR_TRAP_START_LEVEL                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 77L)  /* ViReal64, multi-channel */   /*Trapezoid Waveform Attributes*/
#define ZUP366_ATTR_TRAP_PEAK_TLEV                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 78L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_TRAP_START_TIME                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 79L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_TRAP_END_TIME                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 80L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_TRAP_WIDTH_TTIME                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 81L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_TRAP_FALL_TIME                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 82L)  /* ViReal64, multi-channel */
#define ZUP366_ATTR_TRAP_RISE_TIME                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 83L)  /* ViReal64, multi-channel */

#define ZUP366_ATTR_SEQUENCE_SHAPE                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 84L)  /* ViInt32, multi-channel  */    /*Sequence Attributes*/
#define ZUP366_ATTR_SEQUENCE_MEASUREMENT                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 85L)  /* ViInt32, multi-channel  */
#define ZUP366_ATTR_SEQUENCE_PACING                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 86L)  /* ViInt32, multi-channel  */
#define ZUP366_ATTR_SEQUENCE_STEP_COUNT                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 87L)  /* ViInt32, multi-channel  */
#define ZUP366_ATTR_SEQUENCE_COUNT                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 88L)  /* ViInt32, multi-channel  */
#define ZUP366_ATTR_SEQUENCE_TERMINATION                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 89L)  /* ViBoolean, multi-channel */

#define ZUP366_ATTR_DATALOG_ENABLED                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 90L)  /* ViBoolean, multi-channel */    /* Datalog Attributes*/
#define ZUP366_ATTR_DATALOG_PERIOD                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 91L)  /* ViReal64, multi-channel  */
#define ZUP366_ATTR_DATALOG_DURATION                            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 92L)  /* ViReal64, multi-channel  */
#define ZUP366_ATTR_DATALOG_AUTO_RANGE                          (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 93L)  /* ViBoolean, multi-channel */
#define ZUP366_ATTR_DATALOG_RANGE                               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 94L)  /* ViReal64, multi-channel  */
#define ZUP366_ATTR_DATALOG_MARKER1                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 95L)  /* ViReal64, multi-channel  */
#define ZUP366_ATTR_DATALOG_MARKER2                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 96L)  /* ViReal64, multi-channel  */
#define ZUP366_ATTR_HISTOGRAM_ENABLED                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 97L)  /* ViBoolean, multi-channel */
#define ZUP366_ATTR_HISTOGRAM_AUTO_RANGE                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 98L)  /* ViBoolean, multi-channel */
#define ZUP366_ATTR_HISTOGRAM_RANGE                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 99L)  /* ViReal64, multi-channel  */

#define ZUP366_ATTR_TRIGGER_WFM_SRC                             (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 100L) /* ViInt32, multi-channel  */    /* Wfm, Datalog, Histogram  Triggering Attributes*/
#define ZUP366_ATTR_DATALOG_TRIGGER_SRC                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 102L) /* ViInt32, multi-channel  */
#define ZUP366_ATTR_HISTOGRAM_TRIGGER_SRC                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 106L) /* ViInt32, multi-channel  */
#define ZUP366_ATTR_DATALOG_OFFSET                              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 101L) /* ViInt32, multi-channel   */
#define ZUP366_ATTR_DATALOG_TRIGGER_LEVEL                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 103L) /* ViReal64, multi-channel  */
#define ZUP366_ATTR_DATALOG_TRIGGER_SLOPE                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 104L) /* ViInt32, multi-channel   */
#define ZUP366_ATTR_DATALOG_MEASUREMENT                         (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 105L) /* ViInt32, multi-channel   */

#define ZUP366_ATTR_SMU_OUTPUT_IMPEDANCE                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 107L) /* ViInt32, multi-channel   */   /* SMU and Application specific module attributes*/
#define ZUP366_ATTR_SMU_ENABLE_OSCILLATION                      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 108L) /* ViBoolean, multi-channel */
#define ZUP366_ATTR_SMU_VOLTAGE_BANDWIDTH                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 109L) /* ViInt32, multi-channel   */
#define ZUP366_ATTR_SMU_ENABLE_RESISTANCE                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 110L) /* ViInt32, multi-channel   */
#define ZUP366_ATTR_SMU_RESISTANCE_LEVEL                        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 111L) /* ViReal64, multi-channel  */
#define ZUP366_ATTR_SMU_VOLTAGE_LIMIT_TRACKING                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 112L) /* ViBoolean, multi-channel */
#define ZUP366_ATTR_SMU_POSITIVE_VOLTAGE_LIMIT                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 113L) /* ViReal64, multi-channel  */
#define ZUP366_ATTR_SMU_NEGATIVE_VOLTAGE_LIMIT                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 114L) /* ViReal64, multi-channel  */
#define ZUP366_ATTR_SMU_CURRENT_LIMIT_TRACKING                  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 115L) /* ViReal64, multi-channel  */
#define ZUP366_ATTR_SMU_ASM_POSTIVE_CURRENT_LIMIT               (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 116L) /* ViReal64, multi-channel  */
#define ZUP366_ATTR_SMU_ASM_NEGATIVE_CURRENT_LIMIT              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 117L) /* ViReal64, multi-channel  */
#define ZUP366_ATTR_SMU_POSITIVE_OVRP                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 118L) /* ViInt32, multi-channel   */
#define ZUP366_ATTR_SMU_NEGATIVE_OVRP                           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 119L) /* ViInt32, multi-channel   */
#define ZUP366_ATTR_ASM_OVER_PROTECTION_DELAY                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 120L) /* ViReal64, multi-channel  */
#define ZUP366_ATTR_SMU_CURRENT_SLEW_RATE                       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 121L) /* ViReal64, multi-channel  */
#define ZUP366_ATTR_SMU_OUTPUT_REGULATION_PRIORITY              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 123L) /* ViInt32, multi-channel   */
#define ZUP366_ATTR_ENABLE_SIMULTANEOUS_CURR_MEASUREMENT        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 124L) /* ViBoolean, multi-channel */
#define ZUP366_ATTR_ENABLE_SIMULTANEOUS_VOLT_MEASUREMENT        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 125L) /* ViBoolean, multi-channel */


/*- External Datalog Group Attributes ---------------*/
#define ZUP366_ATTR_AUTO_ELOG_CURRENT_RANGE       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1001L)      /* ViBoolean	*/
#define ZUP366_ATTR_AUTO_ELOG_VOLTAGE_RANGE       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1002L)      /* ViBoolean	*/
#define ZUP366_ATTR_ELOG_CURRENT_RANGE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1003L)      /* ViReal64		*/
#define ZUP366_ATTR_ELOG_VOLTAGE_RANGE            (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1004L)      /* ViReal64		*/
#define ZUP366_ATTR_ELOG_TRIGGER_SOURCE           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1005L)      /* ViInt32		*/
#define ZUP366_ATTR_MEASUREMENT_RESOLUTION        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1006L)      /* ViInt32		*/
#define ZUP366_ATTR_ENABLE_CURRENT_ELOG           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1007L)      /* ViBoolean	*/
#define ZUP366_ATTR_ENABLE_VOLTAGE_ELOG           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1008L)      /* ViBoolean	*/
#define ZUP366_ATTR_ENABLE_CURRENT_MINMAX_ELOG    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1009L)      /* ViBoolean	*/
#define ZUP366_ATTR_ENABLE_VOLTAGE_MINMAX_ELOG    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1010L)      /* ViBoolean	*/
#define ZUP366_ATTR_ELOG_PERIOD                   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1011L)      /* ViReal64		*/
	
	
#define ZUP366_ATTR_OUTPUT_POWER				   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1012L)	    /* ViReal64		*/

/*- Obsolete Inherent Instrument Attributes ----------------------------*/
/* Driver Information  */
#define ZUP366_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION    IVI_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION /* ViInt32,  read-only */
#define ZUP366_ATTR_SPECIFIC_DRIVER_MINOR_VERSION    IVI_ATTR_SPECIFIC_DRIVER_MINOR_VERSION /* ViInt32,  read-only */

/* Error Info */
#define ZUP366_ATTR_PRIMARY_ERROR                    IVI_ATTR_PRIMARY_ERROR               /* ViInt32 */
#define ZUP366_ATTR_SECONDARY_ERROR                  IVI_ATTR_SECONDARY_ERROR             /* ViInt32 */
#define ZUP366_ATTR_ERROR_ELABORATION                IVI_ATTR_ERROR_ELABORATION           /* ViString*/

/* Advanced Session Information */
#define ZUP366_ATTR_IO_SESSION_TYPE                  IVI_ATTR_IO_SESSION_TYPE             /* ViString, read-only  */

/* Obsolete Attribute Identifiers for Renamed Attributes */
#define ZUP366_ATTR_NUM_CHANNELS                     IVI_ATTR_CHANNEL_COUNT
#define ZUP366_ATTR_QUERY_INSTR_STATUS               ZUP366_ATTR_QUERY_INSTRUMENT_STATUS
#define ZUP366_ATTR_RESOURCE_DESCRIPTOR              ZUP366_ATTR_IO_RESOURCE_DESCRIPTOR

/****************************************************************************
 *------------------------ Attribute Value Defines -------------------------*
 ****************************************************************************/
/*- Defined values for attribute ZUP366_ATTR_CURRENT_LIMIT_BEHAVIOR ---*/
#define ZUP366_VAL_CURRENT_REGULATE    IVIDCPWR_VAL_CURRENT_REGULATE
#define ZUP366_VAL_CURRENT_TRIP        IVIDCPWR_VAL_CURRENT_TRIP

/*- Defined values for attribute ZUP366_ATTR_TRIGGER_SOURCE -----------*/
#define ZUP366_VAL_TRIG_EXTERNAL       IVIDCPWR_VAL_TRIG_EXTERNAL
#define ZUP366_VAL_SOFTWARE_TRIG       IVIDCPWR_VAL_SOFTWARE_TRIG
#define ZUP366_VAL_TRIG_IMMEDIATE      IVIDCPWR_VAL_TRIG_IMMEDIATE
#define ZUP366_VAL_TRIG_PIN1           (IVIDCPWR_VAL_TRIG_SRC_SPECIFIC_EXT_BASE + 1L)
#define ZUP366_VAL_TRIG_PIN2           (IVIDCPWR_VAL_TRIG_SRC_SPECIFIC_EXT_BASE + 2L)
#define ZUP366_VAL_TRIG_PIN3           (IVIDCPWR_VAL_TRIG_SRC_SPECIFIC_EXT_BASE + 3L)
#define ZUP366_VAL_TRIG_TRAN1          (IVIDCPWR_VAL_TRIG_SRC_SPECIFIC_EXT_BASE + 4L)
#define ZUP366_VAL_TRIG_TRAN2          (IVIDCPWR_VAL_TRIG_SRC_SPECIFIC_EXT_BASE + 5L)
#define ZUP366_VAL_TRIG_TRAN3          (IVIDCPWR_VAL_TRIG_SRC_SPECIFIC_EXT_BASE + 6L)
#define ZUP366_VAL_TRIG_TRAN4          (IVIDCPWR_VAL_TRIG_SRC_SPECIFIC_EXT_BASE + 7L)
#define ZUP366_VAL_TRIG_BUS            (IVIDCPWR_VAL_TRIG_SRC_SPECIFIC_EXT_BASE + 8L)
#define ZUP366_VAL_TRIG_PIN4           (IVIDCPWR_VAL_TRIG_SRC_SPECIFIC_EXT_BASE + 9L)
#define ZUP366_VAL_TRIG_PIN5           (IVIDCPWR_VAL_TRIG_SRC_SPECIFIC_EXT_BASE + 10L)
#define ZUP366_VAL_TRIG_PIN6           (IVIDCPWR_VAL_TRIG_SRC_SPECIFIC_EXT_BASE + 11L)
#define ZUP366_VAL_TRIG_PIN7           (IVIDCPWR_VAL_TRIG_SRC_SPECIFIC_EXT_BASE + 12L)

/* Instrument specific attribute value definitions */
/*- Defined values for attribute ZUP366_ATTR_LIST_COUNT ---------------*/
#define ZUP366_VAL_LIST_COUNT_INFINITY               (-1)

/*- Defined values for attribute ZUP366_ATTR_LIST_STEP ----------------*/
#define ZUP366_VAL_LIST_STEP_ONCE                    (0)
#define ZUP366_VAL_LIST_STEP_AUTO                    (1)

/*- Defined values for attribute ZUP366_ATTR_TRIGGER_CURRENT_MODE------*/
#define ZUP366_VAL_TRIGGER_CURRENT_MODE_FIXED        (0)
#define ZUP366_VAL_TRIGGER_CURRENT_MODE_STEP         (1)
#define ZUP366_VAL_TRIGGER_CURRENT_MODE_LIST         (2)
#define ZUP366_VAL_TRIGGER_CURRENT_MODE_ARB          (3)

/*- Defined values for attribute ZUP366_ATTR_TRIGGER_VOLTAGE_MODE------*/
#define ZUP366_VAL_TRIGGER_VOLTAGE_MODE_FIXED        (0)
#define ZUP366_VAL_TRIGGER_VOLTAGE_MODE_STEP         (1)
#define ZUP366_VAL_TRIGGER_VOLTAGE_MODE_LIST         (2)
#define ZUP366_VAL_TRIGGER_VOLTAGE_MODE_ARB          (3)

/*- Defined values for attribute ZUP366_ATTR_MEASUREMENT_WINDOW -------*/
#define ZUP366_VAL_MEASUREMENT_WINDOW_HANNING        (1)
#define ZUP366_VAL_MEASUREMENT_WINDOW_RECTANGULAR    (2)

/*- Defined values for attribute ZUP366_ATTR_OUTPUT_INHIBIT_MODE ------*/
#define ZUP366_VAL_OUTPUT_INHIBIT_MODE_OFF           (0)
#define ZUP366_VAL_OUTPUT_INHIBIT_MODE_LIVE          (1)
#define ZUP366_VAL_OUTPUT_INHIBIT_MODE_LATCHING      (2)

/*- Defined values for attribute ZUP366_ATTR_DIGITAL_PIN1_FUNCTION ----*/
/*- Defined values for attribute ZUP366_ATTR_DIGITAL_PIN2_FUNCTION ----*/
/*- Defined values for attribute ZUP366_ATTR_DIGITAL_PIN3_FUNCTION ----*/
/*- Defined values for attribute ZUP366_ATTR_DIGITAL_PIN4_FUNCTION ----*/
/*- Defined values for attribute ZUP366_ATTR_DIGITAL_PIN5_FUNCTION ----*/
/*- Defined values for attribute ZUP366_ATTR_DIGITAL_PIN6_FUNCTION ----*/
/*- Defined values for attribute ZUP366_ATTR_DIGITAL_PIN7_FUNCTION ----*/
#define ZUP366_VAL_DIGITAL_PIN_FUNCTION_DIO          (0)
#define ZUP366_VAL_DIGITAL_PIN_FUNCTION_DINPUT       (1)
#define ZUP366_VAL_DIGITAL_PIN_FUNCTION_TOUTPUT      (2)
#define ZUP366_VAL_DIGITAL_PIN_FUNCTION_TINPUT       (3)
#define ZUP366_VAL_DIGITAL_PIN_FUNCTION_FAULT        (4)
#define ZUP366_VAL_DIGITAL_PIN_FUNCTION_INHIBIT      (5)
#define ZUP366_VAL_DIGITAL_PIN_FUNCTION_ONCOUPLE     (6)
#define ZUP366_VAL_DIGITAL_PIN_FUNCTION_OFFCOUPLE    (7)


/*- Defined values for attribute ZUP366_ATTR_DIGITAL_PIN1_POLARITY ----*/
/*- Defined values for attribute ZUP366_ATTR_DIGITAL_PIN2_POLARITY ----*/
/*- Defined values for attribute ZUP366_ATTR_DIGITAL_PIN3_POLARITY ----*/
/*- Defined values for attribute ZUP366_ATTR_DIGITAL_PIN4_POLARITY ----*/
/*- Defined values for attribute ZUP366_ATTR_DIGITAL_PIN5_POLARITY ----*/
/*- Defined values for attribute ZUP366_ATTR_DIGITAL_PIN6_POLARITY ----*/
/*- Defined values for attribute ZUP366_ATTR_DIGITAL_PIN7_POLARITY ----*/
#define ZUP366_VAL_DIGITAL_PIN_POLARITY_POSITIVE     (0)
#define ZUP366_VAL_DIGITAL_PIN_POLARITY_NEGATIVE     (1)

/*- Defined values for attribute ZUP366_ATTR_RELAY_POLARITY -----------*/
#define ZUP366_VAL_RELAY_POLARITY_NORMAL             (0)
#define ZUP366_VAL_RELAY_POLARITY_REVERSE            (1)

/*- Defined range table values for (attribute ZUP366_ATTR_WAVEFORM_SHAPE) Waveform Shape */
#define ZUP366_VAL_WAVEFORM_NONE                     0
#define ZUP366_VAL_WAVEFORM_CONSTANTDWELL            1
#define ZUP366_VAL_WAVEFORM_EXPONENTIAL              2
#define ZUP366_VAL_WAVEFORM_PULSE                    3
#define ZUP366_VAL_WAVEFORM_RAMP                     4
#define ZUP366_VAL_WAVEFORM_SEQUENCE                 5
#define ZUP366_VAL_WAVEFORM_SINSUOID                 6
#define ZUP366_VAL_WAVEFORM_STAIRCASE                7
#define ZUP366_VAL_WAVEFORM_STEP                     8
#define ZUP366_VAL_WAVEFORM_TRAPEZOID                9
#define ZUP366_VAL_WAVEFORM_UDEFINED                 10

/*- Defined range table values for (attribute ZUP366_ATTR_WAVEFORM_MEASUREMENT) Waveform Measurement Type*/
#define ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE             0
#define ZUP366_VAL_WFM_MEAS_TYPE_CURRENT             1

/*- Defined values for attribute ZUP366_ATTR_WAVEFORM_COUNT ---------------*/
#define ZUP366_VAL_WAVEFORM_COUNT_INFINITY           (-1)

/*- Defined values for attribute ZUP366_ATTR_SEQUENCE_COUNT ---------------*/
#define ZUP366_VAL_SEQUENCE_STEP_COUNT_INFINITY      (-1)

/*- Defined values for attribute ZUP366_VAL_SEQUENCE_COUNT_INFINITY ---------------*/
#define ZUP366_VAL_SEQUENCE_COUNT_INFINITY           (-1)

/*- Defined values for attribute ZUP366_ATTR_CURRENT_SLEW_RATE_COUNT ---------------*/
#define ZUP366_VAL_SLEW_RATE_COUNT_INFINITY          (-1)

/*- Defined range table values for (attribute ZUP366_ATTR_SEQUENCE_MEASUREMENT) Sequence Shape*/
#define ZUP366_VAL_SEQUENCE_EXPONENTIAL              0
#define ZUP366_VAL_SEQUENCE_PULSE                    1
#define ZUP366_VAL_SEQUENCE_RAMP                     2
#define ZUP366_VAL_SEQUENCE_SINSUOID                 3
#define ZUP366_VAL_SEQUENCE_STAIRCASE                4
#define ZUP366_VAL_SEQUENCE_STEP                     5
#define ZUP366_VAL_SEQUENCE_TRAPEZOID                6
#define ZUP366_VAL_SEQUENCE_UDEFINED                 7

/*- Defined range table values for Sequence Measurement Type */
#define ZUP366_VAL_SEQ_MEAS_TYPE_VOLTAGE             0
#define ZUP366_VAL_SEQ_MEAS_TYPE_CURRENT             1

/*- Defined range table values for (attribute ZUP366_ATTR_) Sequence Pacing*/
#define ZUP366_VAL_SEQUENCE_PACING_TYPE_DWELL        0
#define ZUP366_VAL_SEQUENCE_PACING_TYPE_TRIGGERED    1

/*- Defined range table values for (attribute ZUP366_ATTR_) Waveform Trigger Source*/
#define ZUP366_VAL_TRIG_WFM_IMMEDIATE                0
#define ZUP366_VAL_TRIG_WFM_EXTERNAL                 1
#define ZUP366_VAL_TRIG_WFM_BUS_SW                   2

/*- Defined range table values for (attribute ZUP366_ATTR_SEQUENCE_PACING) Datalog Trigger Source*/
#define ZUP366_VAL_TRIG_DATALOG_IMMEDIATE            0
#define ZUP366_VAL_TRIG_DATALOG_EXTERNAL             1
#define ZUP366_VAL_TRIG_DATALOG_BUS_SW               2
#define ZUP366_VAL_TRIG_DATALOG_CURRENT1             3
#define ZUP366_VAL_TRIG_DATALOG_CURRENT2             4
#define ZUP366_VAL_TRIG_DATALOG_CURRENT3             5
#define ZUP366_VAL_TRIG_DATALOG_CURRENT4             6
#define ZUP366_VAL_TRIG_DATALOG_VOLTAGE1             7
#define ZUP366_VAL_TRIG_DATALOG_VOLTAGE2             8
#define ZUP366_VAL_TRIG_DATALOG_VOLTAGE3             9
#define ZUP366_VAL_TRIG_DATALOG_VOLTAGE4             10

/*- Defined range table values for (attribute ZUP366_ATTR_DATALOG_MEASUREMENT) Datalog Measurement Type*/
#define ZUP366_VAL_DL_MEAS_TYPE_VOLTAGE              0
#define ZUP366_VAL_DL_MEAS_TYPE_CURRENT              1

/*- Defined range table values for (attribute ZUP366_ATTR_DATALOG_TRIGGER_SLOPE) Datalog Slope*/
#define ZUP366_VAL_DATALOG_SLOPE_POSITIVE            0
#define ZUP366_VAL_DATALOG_SLOPE_NEGATIVE            1

/*- Defined range table values for (attribute ZUP366_ATTR_HISTOGRAM_TRIGGER_SRC) Histogram Trigger Source*/
#define ZUP366_VAL_HISTOGRAM_IMMEDIATE               0
#define ZUP366_VAL_HISTOGRAM_EXTERNAL                1
#define ZUP366_VAL_HISTOGRAM_BUS_TRIG                2
#define ZUP366_VAL_HISTOGRAM_TRIG_PIN1               3
#define ZUP366_VAL_HISTOGRAM_TRIG_PIN2               4
#define ZUP366_VAL_HISTOGRAM_TRIG_PIN3               5

/*- Defined range table values for (attribute ZUP366_ATTR_SMU_OUTPUT_IMPEDANCE) SMU Output Impedance*/
#define ZUP366_VAL_SMU_HIGH_IMPEDANCE                0
#define ZUP366_VAL_SMU_LOW_IMPEDANCE                 1

/*- Defined range table values for (attribute ZUP366_ATTR_SMU_VOLTAGE_BANDWIDTH) SMU Bandwidth*/
#define ZUP366_VAL_SMU_LOW_BANDWIDTH                 0
#define ZUP366_VAL_SMU_HIGH1_BANDWIDTH               1
#define ZUP366_VAL_SMU_HIGH2_BANDWIDTH               3
#define ZUP366_VAL_SMU_HIGH3_BANDWIDTH               4

/*- Defined range table values for (attribute ZUP366_ATTR_SMU_OUTPUT_REGULATION_PRIORITY) Output Regulation Priority*/
#define ZUP366_VAL_OUTPUT_REGULATION_VOLTAGE         0
#define ZUP366_VAL_OUTPUT_REGULATION_CURRENT         1

/*- Defined values for Waveform Sequence Actions */
#define ZUP366_VAL_SEQUENCE_ACTION_SAVE              0
#define ZUP366_VAL_SEQUENCE_ACTION_RECALL            1

/*- Defined values for File Management Actions */
#define ZUP366_VAL_FILE_OPERATION_DELETE             0
#define ZUP366_VAL_FILE_OPERATION_EXPORT             1


/*- Defined values for attribute ZUP366_ATTR_MEASUREMENT_RESOLUTION -*/
#define ZUP366_VAL_RESOLUTION_20MS      (1)
#define ZUP366_VAL_RESOLUTION_40MS      (2)

#define ZUP366_VAL_ELOG_TYPE_VOLTAGE    (1)
#define ZUP366_VAL_ELOG_TYPE_CURRENT    (2)

/****************************************************************************
 *---------------- Instrument Driver Function Declarations -----------------*
 ****************************************************************************/

/*- Init and Close Functions -------------------------------------------*/
ViStatus _VI_FUNC  zup366_init (ViRsrc resourceName,
                                 ViBoolean IDQuery,
                                 ViBoolean resetDevice,
                                 ViSession   *vi);
ViStatus _VI_FUNC  zup366_InitWithOptions (ViRsrc resourceName,
                                            ViBoolean IDQuery,
                                            ViBoolean resetDevice,
                                            ViConstString optionString,
                                            ViSession       *newVi);
ViStatus _VI_FUNC  zup366_close (ViSession vi);

/*- Coercion Info Functions --------------------------------------------*/
ViStatus _VI_FUNC  zup366_GetNextCoercionRecord (ViSession vi,
                                                  ViInt32 bufferSize,
                                                  ViChar record[]);

/*- Locking Functions --------------------------------------------------*/
ViStatus _VI_FUNC  zup366_LockSession (ViSession vi,
                                        ViBoolean*  callerHasLock);
ViStatus _VI_FUNC  zup366_UnlockSession (ViSession vi,
                                          ViBoolean*  callerHasLock);

/*- Channel Info Functions ---------------------------------------------*/
ViStatus _VI_FUNC zup366_GetChannelName (ViSession vi,
                                          ViInt32 index,
                                          ViInt32 bufferSize,
                                          ViChar name[]);

/*- Basic Instrument Operation -----------------------------------------*/
ViStatus _VI_FUNC zup366_ConfigureOutputEnabled (ViSession vi,
                                                  ViConstString channelName,
                                                  ViBoolean enabled);
ViStatus _VI_FUNC zup366_ConfigureOutputRange (ViSession vi,
                                                ViConstString channelName,
                                                ViInt32 rangeType,
                                                ViReal64 range);
ViStatus _VI_FUNC zup366_ConfigureCurrentLimit (ViSession vi,
                                                 ViConstString channelName,
                                                 ViInt32 behavior,
                                                 ViReal64 limit);
ViStatus _VI_FUNC zup366_ConfigureOVP (ViSession vi,
                                        ViConstString channelName,
                                        ViBoolean enabled,
                                        ViReal64 limit);
ViStatus _VI_FUNC zup366_ConfigureVoltageLevel (ViSession vi,
                                                 ViConstString channelName,
                                                 ViReal64 level);
ViStatus _VI_FUNC zup366_ConfigureRelayPolarity (ViSession vi,
                                                  ViConstString channelName,
                                                  ViInt32 relayPolarity);
ViStatus _VI_FUNC zup366_LimitChannelPower (ViSession vi,
                                             ViConstString channelName,
                                             ViReal64 powerLimit);

ViStatus _VI_FUNC zup366_QueryOutputState (ViSession vi,
                                            ViConstString channelName,
                                            ViInt32 outputState,
                                            ViBoolean*      inState);
ViStatus _VI_FUNC zup366_QueryMaxCurrentLimit (ViSession vi,
                                                ViConstString channelName,
                                                ViReal64 voltageLevel,
                                                ViReal64*       maxCurrentLimit);
ViStatus _VI_FUNC zup366_QueryMaxVoltageLevel (ViSession vi,
                                                ViConstString channelName,
                                                ViReal64 currentLimit,
                                                ViReal64*       maxVoltageLevel);
ViStatus _VI_FUNC zup366_ResetOutputProtection (ViSession vi,
                                                 ViConstString ChannelName);
ViStatus _VI_FUNC zup366_ChannelGroupingStatus (ViSession vi,
                                                 ViChar groupedChannels[]);


/*- Triggering ---------------------------------------------------------*/
ViStatus _VI_FUNC zup366_ConfigureTriggerSource (ViSession vi,
                                                  ViConstString channelName,
                                                  ViInt32 source);
ViStatus _VI_FUNC zup366_ConfigureTriggeredVoltageLevel (ViSession vi,
                                                          ViConstString channelName,
                                                          ViReal64 level);
ViStatus _VI_FUNC zup366_ConfigureTriggeredCurrentLimit (ViSession vi,
                                                          ViConstString channelName,
                                                          ViReal64 limit);

ViStatus _VI_FUNC zup366_ConfigureTriggerWaveformSource (ViSession instrumentHandle,
                                                          ViInt32 waveformTriggerSource);

ViStatus _VI_FUNC zup366_ConfigureDatalogTriggerSource (ViSession instrumentHandle,
                                                         ViInt32 datalogTriggerSource);

ViStatus _VI_FUNC zup366_ConfigureDatalogCurrVoltTrigger (ViSession instrumentHandle,
                                                           ViChar channelName[],
                                                           ViReal64 datalogLevel,
                                                           ViInt32 datalogSlope);

ViStatus _VI_FUNC zup366_ConfigureHistogramTriggerSource (ViSession instrumentHandle,
                                                           ViChar channelName[],
                                                           ViInt32 histogramTriggerSource);

ViStatus _VI_FUNC zup366_ImmediateHistogramTrigger (ViSession instrumentHandle, ViChar channelName[]);

ViStatus _VI_FUNC zup366_ImmediateDatalogTrigger (ViSession instrumentHandle);


/*- List ---------------------------------------------------------------*/
ViStatus _VI_FUNC zup366_ConfigureList (ViSession vi,
                                         ViConstString channelName,
                                         ViInt32 listCount,
                                         ViInt32 listStep,
                                         ViBoolean terminateLast);

ViStatus _VI_FUNC zup366_ConfigureDwellList (ViSession vi,
                                              ViConstString channelName,
                                              ViInt32 listSize,
                                              ViReal64 dwellTime[]);

ViStatus _VI_FUNC zup366_ConfigureVoltageList (ViSession vi,
                                                ViConstString channelName,
                                                ViInt32 listSize,
                                                ViReal64 voltageList[]);

ViStatus _VI_FUNC zup366_ConfigureCurrentList (ViSession vi,
                                                ViConstString channelName,
                                                ViInt32 listSize,
                                                ViReal64 currentList[]);

/*- Trigger Output -----------------------------------------------------*/
ViStatus _VI_FUNC zup366_ConfigureBOSTList (ViSession vi,
                                             ViConstString channelName,
                                             ViInt32 listSize,
                                             ViBoolean bostList[]);

ViStatus _VI_FUNC zup366_ConfigureEOSTList (ViSession vi,
                                             ViConstString channelName,
                                             ViInt32 listSize,
                                             ViBoolean eostList[]);

/*- Acquisition --------------------------------------------------------*/
ViStatus _VI_FUNC zup366_ConfigureMeasurement (ViSession vi,
                                                ViConstString channelName,
                                                ViInt32 measurementType,
                                                ViInt32 triggerOffsetPoints,
                                                ViInt32 points,
                                                ViReal64 sampleInterval);
ViStatus _VI_FUNC zup366_ConfigureMeasurementRange (ViSession vi,
                                                     ViConstString channelName,
                                                     ViInt32 rangeType,
                                                     ViReal64 range);

ViStatus _VI_FUNC zup366_ConfigureTriggeredVoltageMode (ViSession vi,
                                                         ViConstString channelName,
                                                         ViInt32 mode);
ViStatus _VI_FUNC zup366_ConfigureTriggeredCurrentMode (ViSession vi,
                                                         ViConstString channelName,
                                                         ViInt32 mode);

ViStatus _VI_FUNC zup366_ConfigureSimulatanMeasurement (ViSession instrumentHandle,
                                                         ViChar channelName[],
                                                         ViBoolean enableCurrentMeasurement,
                                                         ViBoolean enableVoltageMeasurement);

ViStatus _VI_FUNC zup366_ConfigurePin (ViSession vi,
                                        ViConstString pinName,
                                        ViInt32 pinFunction,
                                        ViInt32 polarity);
ViStatus _VI_FUNC zup366_WriteToOuputPort (ViSession vi,
                                            ViInt32 data);
ViStatus _VI_FUNC zup366_ReadInputPort (ViSession vi,
                                         ViInt32*    data);

ViStatus _VI_FUNC zup366_Abort (ViSession vi);

ViStatus _VI_FUNC zup366_AbortDatalog (ViSession instrumentHandle);

ViStatus _VI_FUNC zup366_AbortHistogram (ViSession instrumentHandle, ViChar channelName[]);

ViStatus _VI_FUNC zup366_InitiateDatalog (ViSession instrumentHandle,
                                           ViString datalogFilePath);

ViStatus _VI_FUNC zup366_InitiateHistogram (ViSession instrumentHandle, ViChar channelName[]);

ViStatus _VI_FUNC zup366_FetchDatalog (ViSession instrumentHandle, ViChar channelName[], ViReal64 *peakToPeak);

ViStatus _VI_FUNC zup366_FetchDatalogMeasurement (ViSession instrumentHandle, ViChar channelName[], ViReal64 *datalogMeasurement);

ViStatus _VI_FUNC zup366_FetchHistogram (ViSession instrumentHandle,
                                          ViChar channelName[],
                                          ViReal64 binRange,
                                          ViReal64 *binCurrentData);


ViStatus _VI_FUNC zup366_Initiate (ViSession vi);

ViStatus _VI_FUNC zup366_InitiateAcquisition (ViSession vi,
                                               ViConstString channelList);
ViStatus _VI_FUNC zup366_AbortAcquisition (ViSession vi,
                                            ViConstString channelList);
ViStatus _VI_FUNC zup366_ImmediateAcquisitionTrigger (ViSession vi,
                                                       ViConstString channelList);
ViStatus _VI_FUNC zup366_ImmediateTransientTrigger (ViSession vi,
                                                     ViConstString channelList);

ViStatus _VI_FUNC zup366_ReadHistogram (ViSession instrumentHandle, ViChar channelName[],
                                         ViReal64 binRange, ViReal64 *binCurrentData);

/*- Software Triggering ------------------------------------------------*/
ViStatus _VI_FUNC zup366_SendSoftwareTrigger (ViSession vi);


/*- Measuring ----------------------------------------------------------*/
ViStatus _VI_FUNC zup366_Measure (ViSession vi,
                                   ViConstString channelName,
                                   ViInt32 measurementType,
                                   ViReal64*       measurement);
ViStatus _VI_FUNC zup366_MeasureArray (ViSession vi,
                                        ViConstString channelName,
                                        ViInt32 measurementType,
                                        ViInt32 maximumTime_ms,
                                        ViInt32 arraySize,
                                        ViReal64 _VI_FAR measurementArray[],
                                        ViInt32*        actualPoints);
ViStatus _VI_FUNC zup366_Fetch (ViSession vi,
                                 ViConstString channelName,
                                 ViInt32 measurementType,
                                 ViReal64*       measurement);
ViStatus _VI_FUNC zup366_FetchArray (ViSession vi,
                                      ViConstString channelName,
                                      ViInt32 measurementType,
                                      ViInt32 arraySize,
                                      ViReal64 _VI_FAR measurementArray[],
                                      ViInt32*        actualPoints);

/*- Error Functions ----------------------------------------------------*/
ViStatus _VI_FUNC  zup366_error_query (ViSession vi, ViInt32 *errorCode,
                                        ViChar errorMessage[]);

ViStatus _VI_FUNC  zup366_GetError (ViSession vi, ViStatus *code,
                                     ViInt32 bufferSize, ViChar description[]);
ViStatus _VI_FUNC  zup366_ClearError (ViSession vi);

ViStatus _VI_FUNC zup366_error_message (ViSession vi, ViStatus errorCode,
                                         ViChar errorMessage[256]);

/*- Obsolete Error Handling Functions ----------------------------*/
ViStatus _VI_FUNC zup366_GetErrorInfo (
    ViSession vi,
    ViStatus * primaryError,
    ViStatus * secondaryError,
    ViChar errorElaboration[256]
    );
ViStatus _VI_FUNC  zup366_ClearErrorInfo (ViSession vi);

/*- Interchangeability Checking Functions ------------------------------*/
ViStatus _VI_FUNC zup366_GetNextInterchangeWarning (ViSession vi,
                                                     ViInt32 bufferSize,
                                                     ViChar warnString[]);
ViStatus _VI_FUNC zup366_ResetInterchangeCheck (ViSession vi);
ViStatus _VI_FUNC zup366_ClearInterchangeWarnings (ViSession vi);

/*- Utility Functions --------------------------------------------------*/
ViStatus _VI_FUNC zup366_InvalidateAllAttributes (ViSession vi);
ViStatus _VI_FUNC zup366_reset (ViSession vi);
ViStatus _VI_FUNC zup366_ResetWithDefaults (ViSession vi);
ViStatus _VI_FUNC zup366_self_test (ViSession vi, ViInt16 *selfTestResult,
                                     ViChar selfTestMessage[]);
ViStatus _VI_FUNC zup366_revision_query (ViSession vi,
                                          ViChar instrumentDriverRevision[],
                                          ViChar firmwareRevision[]);
ViStatus _VI_FUNC zup366_Disable (ViSession vi);

ViStatus _VI_FUNC zup366_viWrite (ViSession vi, ViByte buffer[], ViInt64 count, ViInt64 *returnCount);
ViStatus _VI_FUNC zup366_viRead (ViSession vi, ViInt64 bufferSize, ViByte buffer[], ViInt64 *returnCount);
/* Obsolete Direct IO Functions. */
ViStatus _VI_FUNC zup366_WriteInstrData (ViSession vi, ViConstString writeBuffer);
ViStatus _VI_FUNC zup366_ReadInstrData (ViSession vi, ViInt32 numBytes,
                                         ViChar rdBuf[], ViInt32 *bytesRead);

ViStatus _VI_FUNC zup366_ResetSequence (ViSession instrumentHandle, ViChar channelName[]);

ViStatus _VI_FUNC zup366_SaveRecallSequence (ViSession instrumentHandle, ViChar channelName[],
                                              ViInt32 sequenceAction, ViString sequenceFilePath);
ViStatus _VI_FUNC zup366_ConvertWaveformToList (ViSession instrumentHandle, ViChar channelName[]);
ViStatus _VI_FUNC zup366_FileManagement (ViSession instrumentHandle,
                                          ViInt32 operation, ViString filePath);

/*- Group Functions --------------------------------------------------*/
ViStatus _VI_FUNC zup366_ConfigureChannelGrouping (ViSession vi, ViConstString channelName);
ViStatus _VI_FUNC zup366_UngroupChannels (ViSession vi, ViConstString channelName, ViBoolean ungroupAll);

/*- Set, Get, and Check Attribute Functions ----------------------------*/
ViStatus _VI_FUNC zup366_GetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 *value);
ViStatus _VI_FUNC zup366_GetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 *value);
ViStatus _VI_FUNC zup366_GetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 bufSize, ViChar value[]);
ViStatus _VI_FUNC zup366_GetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession *value);
ViStatus _VI_FUNC zup366_GetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean *value);

ViStatus _VI_FUNC zup366_ConfigureWaveformParameters (ViSession instrumentHandle, ViChar channelName[],
                                                       ViInt32 waveformShape, ViInt32 waveformMeasurementType,
                                                       ViInt32 waveformCount, ViBoolean enableWaveformTermination);

ViStatus _VI_FUNC zup366_ConfigureConstantDwellWaveform (ViSession instrumentHandle, ViChar channelName[],
                                                          ViInt32 constantDwellLevelWfmSize, ViReal64 constantDwellLevel[],
                                                          ViReal64 constantDwellTime);


ViStatus _VI_FUNC zup366_ConfigureExponentialWfmSeq (ViSession instrumentHandle, ViChar channelName[],
                                                      ViInt32 sequenceStepNumber, ViReal64 exponentialStartLevel, ViReal64 exponentialEndLevel,
                                                      ViReal64 exponentialStartTime, ViReal64 exponentialTimeConstant,
                                                      ViReal64 exponentialTotalTime);

ViStatus _VI_FUNC zup366_ConfigurePulseWfmSeq (ViSession instrumentHandle, ViChar channelName[],
                                                ViInt32 sequenceStepNumber, ViReal64 pulseStartLevel, ViReal64 pulseAmplitude,
                                                ViReal64 pulseStartTime, ViReal64 pulseWidth, ViReal64 pulseEndTime);

ViStatus _VI_FUNC zup366_ConfigureRampWfmSeq (ViSession instrumentHandle, ViChar channelName[],
                                               ViInt32 sequenceStepNumber, ViReal64 rampStartLevel, ViReal64 rampEndLevel,
                                               ViReal64 rampStartTime, ViReal64 rampEndTime, ViReal64 rampRiseTime);

ViStatus _VI_FUNC zup366_ConfigureSinusoidWfmSeq (ViSession instrumentHandle, ViChar channelName[],
                                                   ViInt32 sequenceStepNumber, ViReal64 amplitude, ViReal64 offset, ViReal64 frequency);

ViStatus _VI_FUNC zup366_ConfigureStaircaseWfmSeq (ViSession instrumentHandle, ViChar channelName[],
                                                    ViInt32 sequenceStepNumber, ViReal64 staircaseStartLevel, ViReal64 staircaseEndLevel,
                                                    ViReal64 staircaseStartTime, ViReal64 staircaseEndTime, ViReal64 staircaseTotalTime,
                                                    ViInt32 staircaseNSteps);

ViStatus _VI_FUNC zup366_ConfigureStepWfmSeq (ViSession instrumentHandle, ViChar channelName[],
                                               ViInt32 sequenceStepNumber, ViReal64 stepStartLevel, ViReal64 stepEndLevel,
                                               ViReal64 stepStartTime, ViReal64 stepEndTime);


ViStatus _VI_FUNC zup366_ConfigureTrapezoidWfmSeq (ViSession instrumentHandle, ViChar channelName[],
                                                    ViInt32 sequenceStepNumber, ViReal64 trapezoidStartLevel, ViReal64 trapezoidPeak,
                                                    ViReal64 trapezoidStartTime, ViReal64 trapezoidRiseTime,
                                                    ViReal64 trapezoidWidth, ViReal64 trapezoidFallTime, ViReal64 trapezoidEndTime);

ViStatus _VI_FUNC zup366_ConfigureUserDefinedWaveform (ViSession instrumentHandle, ViChar channelName[],
                                                        ViInt32 userDefinedBOSTSize, ViInt32 userDefinedBeginningStep[],
                                                        ViInt32 userDefinedDwellTimeSize, ViReal64 userDefinedDwellTime[],
                                                        ViInt32 userDefinedLevelSize, ViReal64 userDefinedLevel[]);

ViStatus _VI_FUNC zup366_ConfigureSequenceParameters (ViSession instrumentHandle, ViChar channelName[],
                                                       ViInt32 sequenceMeasurementType, ViInt32 sequenceStepNumber,
                                                       ViInt32 sequenceShape, ViInt32 pacing, ViInt32 sequenceStepCount,
                                                       ViInt32 sequenceCount, ViBoolean enableSequenceTermination);

ViStatus _VI_FUNC zup366_ConfigureUserDefinedSequence (ViSession instrumentHandle,
                                                        ViChar channelName[],
                                                        ViInt32 sequenceUDefBOSTSize,
                                                        ViInt32 sequenceUDefBeginningStep[],
                                                        ViInt32 sequenceUDefDwellTimeSize,
                                                        ViReal64 sequenceUDefDwellTime[],
                                                        ViInt32 sequenceUDefDwellLevelSize,
                                                        ViReal64 sequenceUDefDwellLevel[],
                                                        ViInt32 sequenceStepNumber);

ViStatus _VI_FUNC zup366_ConfigureDatalog (ViSession instrumentHandle,
                                            ViChar channelName[],
                                            ViInt32 datalogMeasurementType,
                                            ViBoolean datalogEnabled,
                                            ViReal64 datalogPeriod,
                                            ViReal64 datalogDuration,
                                            ViInt32 datalogOffset);

ViStatus _VI_FUNC zup366_ConfigureDatalogMeasurement (ViSession instrumentHandle,
                                                       ViChar channelName[],
                                                       ViBoolean datalogAutoRange,
                                                       ViReal64 datalogManualRange);

ViStatus _VI_FUNC zup366_ConfigureDatalogMarkers (ViSession instrumentHandle,
                                                   ViReal64 datalogMarker1,
                                                   ViReal64 datalogMarker2);

ViStatus _VI_FUNC zup366_ConfigureHistogram (ViSession instrumentHandle,
                                              ViChar channelName[],
                                              ViBoolean enableHistorgram,
                                              ViBoolean histogramAutoRange,
                                              ViReal64 histogramRange);

ViStatus _VI_FUNC zup366_ConfigureSMUOutputImpedance (ViSession instrumentHandle,
                                                       ViChar channelName[],
                                                       ViInt32 outputImpedance);

ViStatus _VI_FUNC zup366_ConfigureSMUBWOscillation (ViSession instrumentHandle,
                                                     ViChar channelName[],
                                                     ViInt32 voltageBandwidth,
                                                     ViBoolean enableOscillationProtection);

ViStatus _VI_FUNC zup366_ConfigureSMUResistance (ViSession instrumentHandle,
                                                  ViChar channelName[],
                                                  ViBoolean enableResistanceOutput,
                                                  ViReal64 resistanceLevel);

ViStatus _VI_FUNC zup366_ConfigureSMUVoltageLimit (ViSession instrumentHandle,
                                                    ViChar channelName[],
                                                    ViReal64 positiveVoltageLimit,
                                                    ViReal64 negativeVoltageLimit,
                                                    ViBoolean enableVoltageLimitTracking);

ViStatus _VI_FUNC zup366_ConfigureSMUCurrentLimit (ViSession instrumentHandle,
                                                    ViChar channelName[],
                                                    ViBoolean enableCurrentLimitTracking,
                                                    ViReal64 positiveCurrentLimit,
                                                    ViReal64 negativeCurrentLimit);

ViStatus _VI_FUNC zup366_ConfigureSMUOVRPosProtectection (ViSession instrumentHandle,
                                                           ViChar channelName[],
                                                           ViInt32 positiveOVRP);

ViStatus _VI_FUNC zup366_ConfigureSMUOVRNegProtectection (ViSession instrumentHandle,
                                                           ViChar channelName[],
                                                           ViInt32 negativeOVRP);

ViStatus _VI_FUNC zup366_ConfigureSMUOVRemoteDelay (ViSession instrumentHandle,
                                                     ViChar channelName[],
                                                     ViReal64 overProtectionDelay);


ViStatus _VI_FUNC zup366_ConfigureSMUCurrentSlewRate (ViSession instrumentHandle,
                                                       ViChar channelName[],
                                                       ViReal64 currentSlewRate);

ViStatus _VI_FUNC zup366_ConfigureSMUOutputRegulation (ViSession instrumentHandle,
                                                        ViChar channelName[],
                                                        ViInt32 outputRegulation);

ViStatus _VI_FUNC zup366_QueryWaveformQuality (ViSession instrumentHandle,
                                                ViChar channelName[],
                                                ViInt32 *waveformQuality);

ViStatus _VI_FUNC zup366_QueryHistogram (ViSession instrumentHandle,
                                          ViChar channelName[],
                                          ViReal64 binRange,
                                          ViReal64 *currentBinGain,
                                          ViReal64 *currentBinOffset,
                                          ViReal64 *currentBinRange);

ViStatus _VI_FUNC zup366_AbortExternalDatalog (ViSession vi,
                                                ViChar ChannelName[]);
ViStatus _VI_FUNC zup366_FetchExternalDatalog (ViSession vi,
                                                ViChar ChannelName[],
                                                ViInt32 MaxRecords,
                                                ViChar ExternalLog[]);
ViStatus _VI_FUNC zup366_InitiateExternalDatalog (ViSession vi,
                                                   ViChar ChannelName[]);
ViStatus _VI_FUNC zup366_ImmediateExternalDatalogTrigger (ViSession vi,
                                                           ViChar ChannelName[]);
ViStatus _VI_FUNC zup366_ConfigureExternalDatalogTriggerSource (ViSession vi,
                                                                 ViChar ChannelName[],
                                                                 ViInt32 ExternalLoggerTriggerSource);
ViStatus _VI_FUNC zup366_ConfigureMeasurementResolution (ViSession vi,
                                                          ViInt32 MeasurementResolution);
ViStatus _VI_FUNC zup366_ConfigureExternalDatalog (ViSession vi,
                                                    ViChar ChannelName[],
                                                    ViInt32 ExternalDataLogType,
                                                    ViBoolean AutoRange,
                                                    ViReal64 MeasurementRange,
                                                    ViBoolean EnableExternalDataLog,
                                                    ViBoolean EnableMinMaxLog,
                                                    ViReal64 ExternalDataLogPeriod);


ViStatus _VI_FUNC  zup366_SetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
ViStatus _VI_FUNC  zup366_SetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
ViStatus _VI_FUNC  zup366_SetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value);
ViStatus _VI_FUNC  zup366_SetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
ViStatus _VI_FUNC  zup366_SetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

ViStatus _VI_FUNC  zup366_CheckAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
ViStatus _VI_FUNC  zup366_CheckAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
ViStatus _VI_FUNC  zup366_CheckAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value);
ViStatus _VI_FUNC  zup366_CheckAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
ViStatus _VI_FUNC  zup366_CheckAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

/*********************************************************
 *  Functions reserved for class driver use only.
 *  End-users should not call these functions.
 *********************************************************/
ViStatus _VI_FUNC  zup366_IviInit (ViRsrc resourceName, ViBoolean IDQuery,
                                    ViBoolean reset, ViSession vi);
ViStatus _VI_FUNC  zup366_IviClose (ViSession vi);

/****************************************************************************
 *------------------------ Error And Completion Codes ----------------------*
 ****************************************************************************/
#define ZUP366_ERROR_TRIGGER_NOT_SOFTWARE    IVIDCPWR_ERROR_TRIGGER_NOT_SOFTWARE

/* instrument-specific error codes */
#define ZUP366_ERROR_MAX_TIME_EXCEEDED       (IVI_SPECIFIC_ERROR_BASE + 1)


/****************************************************************************
 *---------------------------- End Include File ----------------------------*
 ****************************************************************************/
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
#endif /* __ZUP366_HEADER */



