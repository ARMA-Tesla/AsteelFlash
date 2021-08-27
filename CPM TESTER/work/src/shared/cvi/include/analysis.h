/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-2005.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       analysis.h                                                    */
/* Purpose:     Include file for LabWindows CVI Advanced Analysis Library     */
/*                                                                            */
/*============================================================================*/


#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#if defined(_CVI_) && !defined(__TPC__)
#pragma EnableLibraryRuntimeChecking
#endif

#include "cvidef.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Error Codes */
typedef enum {
    NoAnlysErr              =      0,   // No Error.
    OutOfMemAnlysErr        = -20001,   // Out of memory error.
    EqSamplesAnlysErr       = -20002,   // Input sequences must be the same size.
    SamplesGTZeroAnlysErr   = -20003,   // Number of samples must be greater than zero.
    SamplesGEZeroAnlysErr   = -20004,   // Number of samples must be greater than or equal to zero.
    SamplesGEOneAnlysErr    = -20005,   // The number of samples must be greater than or equal to one.
    SamplesGETwoAnlysErr    = -20006,   // Number of samples must be greater than or equal to two.
    SamplesGEThreeAnlysErr  = -20007,   // Number of samples must be greater than or equal to three.
    ArraySizeAnlysErr       = -20008,   // Specified conditions on the input arrays have not been met.
    PowerOfTwoAnlysErr      = -20009,   // Size of the input array must be a valid power of two: size = 2m.
    MaxXformSizeAnlysErr    = -20010,   // The maximum allowable transform size has been exceeded.
    DutyCycleAnlysErr       = -20011,   // The duty cycle must meet: 0 <= duty cycle <= 100.
    CyclesAnlysErr          = -20012,   // Number of cycles must meet the condition 0 < cycles <= samples.
    WidthLTSamplesAnlysErr  = -20013,   // The width must meet: 0 < width < samples.
    DelayWidthAnlysErr      = -20014,   // Delay and width must meet the condition 0 <= (delay + width) < samples.
    DtGEZeroAnlysErr        = -20015,   // dt must be greater than or equal to zero.
    DtGTZeroAnlysErr        = -20016,   // dt or dx must be greater than zero.
    IndexLTSamplesAnlysErr  = -20017,   // Index must meet the condition 0 <= index < samples.
    IndexLengthAnlysErr     = -20018,   // The index and length must meet the condition 0 <= (index + length) < samples.
    UpperGELowerAnlysErr    = -20019,   // Upper value must be greater than or equal to the lower value.
    NyquistAnlysErr         = -20020,   // Cut-off frequency fc must meet the condition 0 <= fc <= (fs/2).
    OrderGTZeroAnlysErr     = -20021,   // Order must be greater than zero.
    DecFactAnlysErr         = -20022,   // Decimating factor must meet the condition 0 < decimating factor <= samples.
    BandSpecAnlysErr        = -20023,   // Invalid band specification.
    RippleGTZeroAnlysErr    = -20024,   // Ripple must be greater than zero.
    AttenGTZeroAnlysErr     = -20025,   // Attenuation must be greater than zero.
    WidthGTZeroAnlysErr     = -20026,   // The width must be greater than zero.
    FinalGTZeroAnlysErr     = -20027,   // The final value must be greater than zero.
    AttenGTRippleAnlysErr   = -20028,   // Attenuation must be greater than the ripple amplitude.
    StepSizeAnlysErr        = -20029,   // The step-size parameter mu must meet 0 <= mu <= 0.1.
    LeakAnlysErr            = -20030,   // The leakage coefficient Leak must meet: 0 <= Leak <= mu.
    EqRplDesignAnlysErr     = -20031,   // Filter cannot be designed with the specified input parameters.
    RankAnlysErr            = -20032,   // The rank of the filter must meet 1 <= (2 rank +1) <= size.
    EvenSizeAnlysErr        = -20033,   // Number of coefficients must be odd for this filter.
    OddSizeAnlysErr         = -20034,   // Number of coefficients must be even for this filter.
    StdDevAnlysErr          = -20035,   // The standard deviation is zero. The normalization is impossible.
    MixedSignAnlysErr       = -20036,   // Second array must be all positive or negative and nonzero.
    SizeGTOrderAnlysErr     = -20037,   // Array size must be greater than the order.
    IntervalsAnlysErr       = -20038,   // The number of intervals must be greater than zero.
    MatrixMulAnlysErr       = -20039,   // The specified matrix multiplication cannot be performed.
    SquareMatrixAnlysErr    = -20040,   // Input matrix must be a square matrix.
    SingularMatrixAnlysErr  = -20041,   // Input matrix is singular. The system of equations cannot be solved.
    LevelsAnlysErr          = -20042,   // Number of levels is outside the allowable range.
    FactorAnlysErr          = -20043,   // Level of factor is outside the allowable range.
    ObservationsAnlysErr    = -20044,   // There must be at least one observation.
    DataAnlysErr            = -20045,   // Total number of data points must be equal to product of (levels/ each factor) x (observations/ cell).
    OverflowAnlysErr        = -20046,   // There is an overflow in the calculated F-value(ANOVA Fit).
    BalanceAnlysErr         = -20047,   // Data is unbalanced.
    ModelAnlysErr           = -20048,   // Random Effect model was requested when the Fixed Effect model is required.
    DistinctAnlysErr        = -20049,   // x-values must be distinct.
    PoleAnlysErr            = -20050,   // Interpolating function has a pole at the requested value.
    ColumnAnlysErr          = -20051,   // First column in the X matrix must be all ones.
    FreedomAnlysErr         = -20052,   // The degree of freedom must be greater than zero and less than the length of the input sequence.
    ProbabilityAnlysErr     = -20053,   // Probability must meet the condition 0 < p < 1.
    InvProbAnlysErr         = -20054,   // The probability must meet the condition: 0 <= p < 1.
    CategoryAnlysErr        = -20055,   // Invalid number of categories or samples.
    TableAnlysErr           = -20056,   // Contingency table has a negative number.
    BetaFuncAnlysErr        = -20057,   // Parameter to the beta function must meet the condition 0 < p < 1.
    DimensionAnlysErr       = -20058,   // Invalid number of dimensions or dependent variables.
    NegNumAnlysErr          = -20059,   // Negative number.
    DivByZeroAnlysErr       = -20060,   // Divide by zero.
    InvSelectionAnlysErr    = -20061,   // Invalid selection.
    MaxIterAnlysErr         = -20062,   // Maximum iteration exceeded.
    PolyAnlysErr            = -20063,   // Invalid polynomial.
    InitStateAnlysErr       = -20064,   // Internal memory state not initialized correctly.
    ZeroVectorAnlysErr      = -20065,   // Elements of the vector cannot be all zero.
    IIRFilterInfoAnlysErr   = -20066,   // Information in the IIR filter structure is invalid.
    FFSRInvalidAnlysErr     = -20067,   // The input fundamental frequency or sampling rate is equal to zero.
    MatrixInfNanAnlysErr    = -20068,   // Input parameter has at least one element that is Inf or NaN or DBL_MIN or DBL_MAX.
    InvalidDoubleAnlysErr   = -20068,   // Input parameter has at least one element that is Inf or NaN or DBL_MIN or DBL_MAX.
    SamplesGEFourAnlysErr   = -20069,   // Number of samples must be greater than or equal to four.
    SameSizeAnlysErr        = -20070,   // Matrices must have the same size
    PosDefAnlysErr          = -20071,   // Input to Cholesky Decomposition was not positive definite.
    MatrixLnAnlysErr        = -20072,   // The logarithm of the input matrix can't be computed.
    CloseEgnValAnlysErr     = -20073,   // The eigenvalues can't be reordered because some eigenvalues are too close.
    EgnValChgedAnlysErr     = -20074,   // Reordering eigenvalues changed some complex eigenvalues.
    FltBuffOvflowAnlysErr   = -20075,   // The filter buffer overflows.
    RsmplBydInputAnlysErr   = -20076,   // The resample point can't be calculated with the signal behind that of input.
    TimeNotAscendAnlysErr   = -20077,   // Time points are not in ascending order.
    NotSupportedAnlysErr    = -20078,   // Functionality is not supported on this platform.
    BaseGETopAnlysErr       = -20101,   // Parameter must meet the condition: Top>Base.
    ShiftRangeAnlysErr      = -20102,   // The shifts must meet: |shifts| < samples.
    OrderGEZeroAnlysErr     = -20103,   // The order must be positive.
    InputNaNAnlysErr        = -20104,   // The inputs contain NaN.
    ZeroPolyAnlysErr        = -20111,   // Input Polynomial coefficients are all zero.
    StartIsRootAnlysErr     = -20112,   // Poly real zeros counter: start is a root.
    EndIsRootAnlysErr       = -20113,   // Poly real zeros counter: end is a root.
    StartGTEndAnlysErr      = -20114,   // Poly real zeros counter: start > end.
    FFTSizeGTZeroAnlysErr   = -20115,   // FFT size must be greater than zero.
    NoFeasibleSolAnlysErr   = -20116,   // A feasible solution was not found.
    LossOfSignificAnlysErr  = -20117,   // The computation failed due to loss of significance.
    AbsGTOneAnlysErr        = -20118,   // The input is greater than 1 or less than -1.
    SamplesNEThreeAnlysErr  = -20119,   // The number of samples is not equal to three.
    NegZeroNumAnlysErr      = -20140,   // The number is negtive or zero.
    ZeroNumAnlysErr         = -20141,   // The number is zero.
    ScopeOffsetAnlysErr     = -20307,   // Scope offset error.
    InternalAnlysErr        = -20999,   // Internal analysis library error.
    RankDeficient           =  20001,   // The matrix is rank deficient.
    SamplesGTZeroAnlysWarn  =  20002,   // The number of samples must be greater than zero.
    SingularMatrixAnlysWarn =  20003,   // The matrix is singular.
    SameSizeAnlysWarn       =  20004,   // Matrices or vectors don't have the same size.
    ReorderAnlysWarn        =  20005,   // The reorder procedure cannot be performed.
    AccuracyAnlysWarn       =  20006,   // The computation result might be inaccurate.
    ArraySizeAnlysWarn      =  20010,   // The specified conditions on the sizes of input arrays have not been met.
    InvStdDevAnlysWarn      =  20011,   // The input Standard Deviation is invalid.
    EmptyPolyAnlysWarn      =  20012,   // The input polynomial is empty.
    MatrixInfNanAnlysWarn   =  20030,   // Inf or NaN elements found in the input.

    LibraryNotFoundAnlysErr = -5093,    // The analysis library was not found.
    FunctionNotFoundAnlysErr= -5094,    // The function was not found in the analysis library.

    _unusedAnlysErr         = 0x7fffffff// Unused analysis error code.
} AnalysisLibErrType;

/* Filter types */
typedef enum {
    LOWPASS  = 0,       // Lowpass Filter
    HIGHPASS = 1,       // Highpass Filter
    BANDPASS = 2,       // Bandpass Filter
    BANDSTOP = 3        // Bandstop Filter
} AnalysisLibFilterType;

typedef enum {
    MULTIBAND      = 1, // multiband
    DIFFENTIATOR   = 2, // differen.
    HILBERT_FILTER = 3  // Hilbert
} AnalysisLibFilterType_;

/* Window types */
typedef enum {
    RECTANGLE       = 0,    // Rectangle
    HANNING         = 1,    // Hanning
    HAMMING         = 2,    // Hamming
    BLKHARRIS       = 3,    // Blackman-Harris
    EXBLKMAN        = 4,    // Exact Blackman
    BLKMAN          = 5,    // Blackman
    FLATTOP         = 6,    // Flat Top
    BH4TERM         = 7,    // 4-Term Blackman-Harris
    BH7TERM         = 8,    // 7-Term Blackman-Harris
    LOWSIDELB       = 9,    // Lowside LB
    BLKMANNUTTAL    = 11,   // Blackman-Nuttall
    TRIANGLE        = 30,   // Triangle
    BARTHANN        = 31,   // Barthann
    BOHMAN          = 32,   // Bohman
    PARZEN          = 33,   // Parzen
    WELCH           = 34,   // Welch
    KAISER          = 60,   // Kaiser
    DOLCHEBYSHEV    = 61,   // Dolph-Chebyshev
    GAUSSIAN        = 62    // Gaussian
} AnalysisLibWindowType;

typedef enum {
    RECTANGLE_      = 0,    // Rectangle
    HANNING_        = 1,    // Hanning
    HAMMING_        = 2,    // Hamming
    BLKHARRIS_      = 3,    // Blackman-Harris
    EXBLKMAN_       = 4,    // Exact Blackman
    BLKMAN_         = 5,    // Blackman
    FLATTOP_        = 6,    // Flat Top
    BH4TERM_        = 7,    // 4-Term Blackman-Harris
    BH7TERM_        = 8 // 7-Term Blackman-Harris
} AnalysisLibWindowType_;

typedef enum {
    NO_WINDOW__     = 0,    // None
    HANNING__       = 1,    // Hanning
    HAMMING__       = 2,    // Hamming
    BLKHARRIS__     = 3,    // Blackman-Harris
    EXBLKMAN__      = 4,    // Exact Blackman
    BLKMAN__        = 5,    // Blackman
    FLATTOP__       = 6,    // Flat Top
    BH4TERM__       = 7,    // 4-Term Blackman-Harris
    BH7TERM__       = 8 // 7-Term Blackman-Harris
} AnalysisLibWindowType__;

typedef enum {
    RECTANGLE___    = 0,    // Rectangle
    HANNING___      = 1,    // Hanning
    HAMMING___      = 2,    // Hamming
    BLKHARRIS___    = 3,    // Blackman-Harris
    EXBLKMAN___     = 4,    // Exact Blackman
    BLKMAN___       = 5,    // Blackman
    FLATTOP___      = 6,    // Flat Top
    BH4TERM___      = 7,    // 4-Term Blackman-Harris
    BH7TERM___      = 8,    // 7-Term Blackman-Harris
    LOWSIDELB___    = 9,    // Lowside LB
    BLKMANNUTTAL___ = 11    // Blackman-Nuttall
} AnalysisLibWindowType___;

typedef enum {
    RECTANGLE_FILTER    = 1,    // rectangular
    TRIANGLE_FILTER     = 2,    // triangle
    HANNING_FILTER      = 3,    // Hanning
    HAMMING_FILTER      = 4,    // Hamming
    BLKMAN_FILTER       = 5 // Blackman
} AnalysisLibWindowFilter;

/* Eigenvectors used by EigenVBack */
typedef enum {
    RIGHT_EIGEN     = 0,    // Right Side Eigenvectors
    LEFT_EIGEN      = 1     // Left Side Eigenvectors
} AnalysisLibEigenvectorType;

/* Eigenvalues and eigenvectors */
typedef enum {
    EIGEN_VALUES_ONLY    = 0,   // Only Eigen Values
    EIGEN_VALUES_VECTORS = 1    // Both Eigen Values and Eigen Vectors
} AnalysisLibEigenValueVectors;

/* Pivot used by QREx */
typedef enum {
    NOT_PIVOT       = 0,    // No Pivot
    PIVOT_VECTOR    = 1,    // Pivot as a Vector
    PIVOT_MATRIX    = 2     // Pivot as a Matrix
} AnalysisLibPivot;

/* Options for QREx */
typedef enum {
    FULL_SIZE       = 0,    // Full Size
    ECONOMY_SIZE    = 1     // Economical Size
} AnalysisLibSizeOption;

/* Eigenvalue ordering used by Schur */
typedef enum {
    NO_ORDERING     = 0,    // No ordering
    REAL_ASCENDING  = 1,    // Real Part Ascending
    REAL_DESCENDING = 2,    // Real Part Descending
    MAG_ASCENDING   = 3,    // Magnitude Ascending
    MAG_DESCENDING  = 4     // Magnitude Descending
} AnalysisLibEigenvalueOrdering;

/* Matrix balance */
typedef enum {
    NONE            = 0,    // Neither Permuted nor Scaled
    PERMUTED        = 1,    // Permuted
    SCALED          = 2,    // Scaled
    PERMUTED_SCALED = 3     // Permuted and Scaled
} AnalysisLibMatrixBalance;

/* Special matrix type */
typedef enum {
    IDENTITY        = 0,    // Identity Matrix
    DIAGONAL        = 1,    // Diagonal Matrix
    TOEPLITZ        = 2,    // Toeplitz Matrix
    VANDERMONDE     = 3,    // Vandermonde Matrix
    COMPANION       = 4     // Companion Matrix
} AnalysisLibSpecialMatrixType;

/* Matrix type used by SolveEqs */
typedef enum {
    GENERAL_MATRIX = 0,     // General Matrix
    SYMPOS_MATRIX  = 1,     // Symmetric and Positive Definite Matrix
    LOTRI_MATRIX   = 2,     // Lower Triangular Matrix
    UPTRI_MATRIX   = 3      // Upper Triangular Matrix
} AnalysisLibMatrixType;

/* Matrix type for CxEigenValueVector */
typedef enum {
    COMPLEX_GENERAL_MATRIX   = 0,   // General Matrix
    COMPLEX_HERMITIAN_MATRIX = 1    // Hermitian Matrix
} AnalysisLibComplexMatrixType;

/* Matrix norm type */
typedef enum {
    NORM_TYPE_2         = 0,    // 2-Norm
    NORM_TYPE_1         = 1,    // 1-Norm
    NORM_TYPE_FROBENIUS = 2,    // Frobenius-Norm
    NORM_TYPE_INFINITE  = 3     // Infinite-Norm
} AnalysisLibNormType;

/* Fit method type used by Curve Fitting functions */
typedef enum {
    LEAST_SQUARE            = 0,    // Least square
    LEAST_ABSOLUTE_RESIDUAL = 1,    // Least absolute residual
    BISQUARE                = 2     // Bisquare
} AnalysisLibFitMethod;

/* Interval type used by Curve Fitting functions */
typedef enum {
    CONFIDENCE_INTVL = 0,           // Confidence Interval
    PREDICTION_INTVL = 1            // Prediction Interval
} AnalysisLibIntervalType;

/* Range type used by RemoveOutlier functions */
typedef enum {
    RANGE_OF_Y       = 0,           // Range of Y
    RANGE_OF_X       = 1,           // Range of X
    RANGE_OF_X_AND_Y = 2            // Range of X and Y
} AnalysisLibRangeType;

/* Interpolation type used by ArbitraryWave */
typedef enum {
    LINEAR_INTERPOLATION = 1,       // Linear
    NO_INTERPOLATION     = 0        // None
} AnalysisLibInterpolation;

/* Sort ordering */
typedef enum {
    SORT_DESCENDING = 1,            // descending
    SORT_ASCENDING  = 0             // ascending
} AnalysisLibSortOrdering;

/* Powers of two */
typedef enum {
    POWER_2_1,                      // 2
    POWER_2_2,                      // 4
    POWER_2_3,                      // 8
    POWER_2_4,                      // 16
    POWER_2_5,                      // 32
    POWER_2_6,                      // 64
    POWER_2_7,                      // 128
    POWER_2_8,                      // 256
    POWER_2_9,                      // 512
    POWER_2_10,                     // 1024
    POWER_2_11,                     // 2048
    POWER_2_12,                     // 4096
    POWER_2_13,                     // 8192
    POWER_2_14,                     // 16384
    POWER_2_15,                     // 32768
    POWER_2_16,                     // 65536
    POWER_2_17,                     // 131072
    POWER_2_18,                     // 262144
    POWER_2_19,                     // 524288
    POWER_2_20,                     // 1048576
    POWER_2_21,                     // 2097152
    POWER_2_22,                     // 4194304
    POWER_2_23,                     // 8388608
    POWER_2_24,                     // 16777216
    POWER_2_25,                     // 33554432
    POWER_2_26,                     // 67108864
    POWER_2_27,                     // 134217728
    POWER_2_28                      // 268435456
} AnalysisLibPowerOfTwo;

/* Enable or disable an option */
typedef enum {
    ENABLE_OPTION  = 1,             // On
    DISABLE_OPTION = 0              // Off
} AnalysisLibEnableDisable;

/* True or false */
typedef enum {
    ANALYSIS_TRUE  = 1,             // Yes
    ANALYSIS_FALSE = 0              // No
} AnalysisLibTrueFalse;

/* Polarity */
typedef enum {
    DETECT_PEAKS   = 0,             // Peaks
    DETECT_VALLEYS = 1              // Valleys
} AnalysisLibPeakDetection;

/* Spectrum type */
typedef enum {
    SPECTRUM_POWER     = 0,         // power
    SPECTRUM_AMPLITUDE = 1,         // amplitude
    SPECTRUM_GAIN      = 2          // gain
} AnalysisLibSpectrumType;

/* Spectrum type */
typedef enum {
    SCALING_MODE_LINEAR   = 0,      // linear
    SCALING_MODE_DECIBELS = 1,      // dB
    SCALING_MODE_DBM      = 2       // dBm
} AnalysisLibScalingMode;

/* Display unit */
typedef enum {
    DISPLAY_UNIT_VRMS     = 0,      // rms
    DISPLAY_UNIT_VPK      = 1,      // pk
    DISPLAY_UNIT_VRMS2    = 2,      // rms^2
    DISPLAY_UNIT_VPK2     = 3,      // pk^2
    DISPLAY_UNIT_VRMSHZ   = 4,      // rms/rtHz
    DISPLAY_UNIT_VPKHZ    = 5,      // pk/rtHz
    DISPLAY_UNIT_VRMS2HZ  = 6,      // rms^2/rtHz
    DISPLAY_UNIT_VPK2HZ   = 7       // pk^2/rtHz
} AnalysisLibDisplayUnit;

/* Algorithm used to solve the multiple linear regression model */
typedef enum {
    ALGORITHM_SVD         = 0,      // SVD
    ALGORITHM_GIVENS      = 1,      // Givens
    ALGORITHM_GIVENS2     = 2,      // Givens2
    ALGORITHM_HOUSEHOLD   = 3,      // Householder
    ALGORITHM_LU_DECOMP   = 4,      // LU decomposition
    ALGORITHM_CHOLESKY    = 5       // Cholesky
} AnalysisLibGenLSFitAlgorithm;

/* Algorithm used in QR factorization */
typedef enum {
    ALGORITHM_QR_HOUSEHOLDER = 0,       // Householder
    ALGORITHM_QR_GIVENS      = 1,       // Givens
    ALGORITHM_QR_FAST_GIVENS = 2        // Fast Givens
} AnalysisLibQRAlgorithm;

/* Integration method */
typedef enum {
    TRAPEZOIDAL_RULE = 0,       // Trapezoidal Rule
    SIMPSONS_RULE    = 1,       // Simpson's Rule
    SIMPSONS_38_RULE = 2,       // Simpson's 3/8 Rule
    BODE_RULE        = 3        // Bode Rule
} AnalysisLibIntegrationMethod;

/* Algorithm used in Polynomial Fit Ex */
typedef enum {
    ALGORITHM_POLYFIT_SVD      = 0, // SVD
    ALGORITHM_POLYFIT_QR       = 1, // QR
    ALGORITHM_POLYFIT_LU       = 2, // LU
    ALGORITHM_POLYFIT_CHOLESKY = 3  // Cholesky
} AnalysisLibPolyFitExAlgorithm;

/* Algorithm used in Chirp Z transform and complex Chirp Z transform */
typedef enum {
    ALGORITHM_CHIRP_DIRECT      = 0,    // Direct
    ALGORITHM_CHIRP_FREQ_DOMAIN = 1     // Frequency Domain
} AnalysisLibChirpAlgorithm;

/*- Typedefs -----------------------------------------------------------------*/

/* Window constants used by measurement functions */
typedef struct {
    double enbw;
    double coherentgain;
} WindowConst;

/* Model function used by NonLinearFit() */
typedef double CVICALLBACK ModelFun(double x, double *coef, int ncoef);

/* Filter information used by IIR filter functions */
typedef struct {
    int type;       /* filter type (LOWPASS, etc.) */
    int order;      /* filter order */
    int reset;
    int na;
    double *a;      /* a coefficients */
    int nb;
    double *b;      /* b coefficients */
    int ns;
    double *s;      /* internal state */
} *IIRFilterPtr;


/* Define datatype NIComplexNumber */
#if !defined(_NIComplexNumber)
typedef struct NIComplexNumber_struct {
    double real;
    double imaginary;
} NIComplexNumber;
#define _NIComplexNumber
#endif
#define ComplexNum NIComplexNumber

/* FFT table used by FFT, spectrum related functions */
typedef void* PFFTTable;


/*- Signal Generation --------------------------------------------------------*/

AnalysisLibErrType CVIFUNC Impulse      (int n, double amp, int index, double x[]);
AnalysisLibErrType CVIFUNC Pulse        (int n, double amp, int delay,
                                         int width, double pulsePattern[]);
AnalysisLibErrType CVIFUNC Ramp         (int n, double first, double last,
                                         double rampvals[]);
AnalysisLibErrType CVIFUNC Triangle     (int n, double amp, double tri[]);
AnalysisLibErrType CVIFUNC SinePattern  (int n, double amp, double phase,
                                         double cycles, double sine[]);
AnalysisLibErrType CVIFUNC Uniform      (int n, int seed, double x[]);
AnalysisLibErrType CVIFUNC WhiteNoise   (int n, double amp, int seed,
                                         double noise[]);
AnalysisLibErrType CVIFUNC GaussNoise   (int n, double sDev, int seed,
                                         double noise[]);
AnalysisLibErrType CVIFUNC ArbitraryWave(int n, double amp, double f,
                                         double *phase, const double wavetable[],
                                         int wavesize, int wavetype, double x[]);
AnalysisLibErrType CVIFUNC Chirp        (int n, double amp, double f1,
                                         double f2, double x[]);
AnalysisLibErrType CVIFUNC SawtoothWave (int n, double amp, double f,
                                         double *phase, double x[]);
AnalysisLibErrType CVIFUNC Sinc         (int n, double amp, double delay,
                                         double dt, double x[]);
AnalysisLibErrType CVIFUNC SineWave     (int n, double amp, double f,
                                         double *phase, double x[]);
AnalysisLibErrType CVIFUNC SquareWave   (int n, double amp, double f,
                                         double *phase, double duty, double x[]);
AnalysisLibErrType CVIFUNC TriangleWave (int n, double amp, double f,
                                         double *phase, double x[]);
AnalysisLibErrType CVIFUNC GaussMonopulse (int n, double amp, double delay,
                       double dt, double fc, double x[]);
AnalysisLibErrType CVIFUNC GaussModSinePattern (int n, double amp,
                        double delay, double dt,
                        double fc, double bandwidth,
                        double attenuation, double x[]);
AnalysisLibErrType CVIFUNC PeriodicSinc (int n, double amp, double delay,
                     double dt, int c, double x[]);
AnalysisLibErrType CVIFUNC UnsymmetricTriangle (int n, double amp,
                        double delay, double dt,
                        double width, double asy,
                        double x[]);


/*- 1-D Array Operations -----------------------------------------------------*/

AnalysisLibErrType CVIFUNC Clear1D  (double x[], int n);
AnalysisLibErrType CVIFUNC Set1D    (double x[], int n, double a);
AnalysisLibErrType CVIFUNC Copy1D   (const double x[], int n, double y[]);
AnalysisLibErrType CVIFUNC Add1D    (const double x[], const double y[], int n,
                                     double z[]);
AnalysisLibErrType CVIFUNC Sub1D    (const double x[], const double y[], int n,
                                     double z[]);
AnalysisLibErrType CVIFUNC Mul1D    (const double x[], const double y[], int n,
                                     double z[]);
AnalysisLibErrType CVIFUNC Div1D    (const double x[], const double y[], int n,
                                     double z[]);
AnalysisLibErrType CVIFUNC Abs1D    (const double x[], int n, double y[]);
AnalysisLibErrType CVIFUNC Neg1D    (const double x[], int n, double y[]);
AnalysisLibErrType CVIFUNC LinEv1D  (const double x[], int n, double a,
                                     double b, double y[]);
AnalysisLibErrType CVIFUNC PolyEv1D (const double x[], int n,
                                     const double coef[], int k, double y[]);
AnalysisLibErrType CVIFUNC Scale1D  (const double x[], int n, double y[],
                                     double *offset, double *scale);
AnalysisLibErrType CVIFUNC QScale1D (const double x[], int n, double y[],
                                     double *scale);
AnalysisLibErrType CVIFUNC MaxMin1D (const double x[], int n, double *max,
                                     int *imax, double *min, int *imin);
AnalysisLibErrType CVIFUNC Sum1D    (const double x[], int n, double *sum);
AnalysisLibErrType CVIFUNC Prod1D   (const double x[], int n, double *prod);
AnalysisLibErrType CVIFUNC Subset1D (const double x[], int n, int index,
                                     int length, double y[]);
AnalysisLibErrType CVIFUNC Normal1D (const double x[], int n, double y[],
                                     double *ave, double *sDev);
AnalysisLibErrType CVIFUNC Reverse  (const double x[], int n, double y[]);
AnalysisLibErrType CVIFUNC Shift    (const double x[], int n, int shifts,
                                     double y[]);
AnalysisLibErrType CVIFUNC Clip     (const double x[], int n, double upper,
                                     double lower, double y[]);
AnalysisLibErrType CVIFUNC Sort     (const double x[], int n, int direction,
                                     double y[]);


/*- 2-D Array Operations -----------------------------------------------------*/

AnalysisLibErrType CVIFUNC Add2D    (const void *x, const void *y, int n, int m,
                                     void *z);
AnalysisLibErrType CVIFUNC Sub2D    (const void *x, const void *y, int n, int m,
                                     void *z);
AnalysisLibErrType CVIFUNC Mul2D    (const void *x, const void *y, int n, int m,
                                     void *z);
AnalysisLibErrType CVIFUNC Div2D    (const void *x, const void *y, int n, int m,
                                     void *z);
AnalysisLibErrType CVIFUNC LinEv2D  (const void *x, int n, int m, double a,
                                     double b, void *y);
AnalysisLibErrType CVIFUNC PolyEv2D (const void *x, int n, int m, double coef[],
                                     int k, void *y);
AnalysisLibErrType CVIFUNC Scale2D  (const void *x, int n, int m, void *y,
                                     double *offset, double *scale);
AnalysisLibErrType CVIFUNC QScale2D (const void *x, int n, int m, void *y,
                                     double *scale);
AnalysisLibErrType CVIFUNC MaxMin2D (const void *x, int n, int m, double *max,
                                     int *imax, int *jmax, double *min,
                                     int *imin, int *jmin);
AnalysisLibErrType CVIFUNC Sum2D    (const void *x, int n, int m, double *sum);
AnalysisLibErrType CVIFUNC Normal2D (const void *x, int n, int m, void *y,
                                     double *ave, double *sDev);


/*- Complex Number Operations ------------------------------------------------*/

AnalysisLibErrType CVIFUNC CxAdd   (double xr, double xi, double yr, double yi,
                                    double *zr, double *zi);
AnalysisLibErrType CVIFUNC CxSub   (double xr, double xi, double yr, double yi,
                                    double *zr, double *zi);
AnalysisLibErrType CVIFUNC CxMul   (double xr, double xi, double yr, double yi,
                                    double *zr, double *zi);
AnalysisLibErrType CVIFUNC CxDiv   (double xr, double xi, double yr, double yi,
                                    double *zr, double *zi);
AnalysisLibErrType CVIFUNC CxRecip (double xr, double xi, double *yr, double *yi);
AnalysisLibErrType CVIFUNC CxSqrt  (double xr, double xi, double *yr, double *yi);
AnalysisLibErrType CVIFUNC CxLog   (double xr, double xi, double *yr, double *yi);
AnalysisLibErrType CVIFUNC CxLn    (double xr, double xi, double *yr, double *yi);
AnalysisLibErrType CVIFUNC CxPow   (double xr, double xi, double a, double *yr,
                                    double *yi);
AnalysisLibErrType CVIFUNC CxExp   (double xr, double xi, double *yr, double *yi);
AnalysisLibErrType CVIFUNC ToPolar (double x, double y, double *mag,
                                    double *phase);
AnalysisLibErrType CVIFUNC ToRect  (double mag, double phase, double *x,
                                    double *y);


/*- 1-D Complex Array Operations ---------------------------------------------*/

AnalysisLibErrType CVIFUNC CxAdd1D    (const double xr[], const double xi[],
                                       const double yr[], const double yi[],
                                       int n, double zr[], double zi[]);


AnalysisLibErrType CVIFUNC CxSub1D    (const double xr[], const double xi[],
                                       const double yr[], const double yi[],
                                       int n, double zr[], double zi[]);

AnalysisLibErrType CVIFUNC CxMul1D    (const double xr[], const double xi[],
                                       const double yr[], const double yi[],
                                       int n, double zr[], double zi[]);

AnalysisLibErrType CVIFUNC CxDiv1D    (const double xr[], const double xi[],
                                       const double yr[], const double yi[],
                                       int n, double zr[], double zi[]);

AnalysisLibErrType CVIFUNC CxLinEv1D  (const double xr[], const double xi[],
                                       int n, double ar, double ai, double br,
                                       double bi, double yr[], double yi[]);

AnalysisLibErrType CVIFUNC ToPolar1D  (const double x[], const double y[],
                                       int n, double mag[], double phase[]);

AnalysisLibErrType CVIFUNC ToRect1D   (const double mag[], const double phase[],
                                       int n, double x[], double y[]);


/*- Frequency Domain Analysis ------------------------------------------------*/

AnalysisLibErrType CVIFUNC FFT      (double x[], double y[], int n);
AnalysisLibErrType CVIFUNC InvFFT   (double x[], double y[], int n);
AnalysisLibErrType CVIFUNC ReFFT    (double x[], double y[], int n);
AnalysisLibErrType CVIFUNC ReInvFFT (double x[], double y[], int n);
AnalysisLibErrType CVIFUNC Spectrum (double x[], int n);
AnalysisLibErrType CVIFUNC FHT      (double x[], int n);
AnalysisLibErrType CVIFUNC InvFHT   (double x[], int n);

AnalysisLibErrType CVIFUNC CrossSpectrum(const double x[], const double y[],
                                         int n, double zr[], double zi[]);

AnalysisLibErrType CVIFUNC FastHilbertTransform(double x[], int n);
AnalysisLibErrType CVIFUNC InvFastHilbertTransform(double x[], int n);

AnalysisLibErrType CVIFUNC FFT2D      (const void *a, int rowA, int colA,
                                       int fftSizeX, int fftSizeY, int shift, void *b);
AnalysisLibErrType CVIFUNC InvFFT2D   (const void *a, int rowA, int colA,
                                       int shifted, void *b);
AnalysisLibErrType CVIFUNC CxFFT2D    (const void *a, int rowA, int colA,
                                       int fftSizeX,int fftSizeY, int shift, void *b);
AnalysisLibErrType CVIFUNC CxInvFFT2D (const void *a, int rowA, int colA,
                                       int shifted, void *b);

AnalysisLibErrType CVIFUNC DCT        (const double x[], int n, int DCTSize, double y[]);
AnalysisLibErrType CVIFUNC InvDCT     (const double y[], int n,  double x[]);
AnalysisLibErrType CVIFUNC DST        (const double x[], int n, int DSTSize, double y[]);
AnalysisLibErrType CVIFUNC InvDST     (const double y[], int n,  double x[]);
AnalysisLibErrType CVIFUNC DCT2D      (const void *x, int rows, int cols, void *y);
AnalysisLibErrType CVIFUNC InvDCT2D   (const void *y, int rows, int cols, void *x);
AnalysisLibErrType CVIFUNC DST2D      (const void *x, int rows, int cols, void *y);
AnalysisLibErrType CVIFUNC InvDST2D   (const void *y, int rows, int cols, void *x);
AnalysisLibErrType CVIFUNC ChirpZT    (const double x[], int n,
                                       NIComplexNumber startingPoint,
                                       NIComplexNumber increment, int algorithm,
                                       int m, NIComplexNumber y[]);
AnalysisLibErrType CVIFUNC CxChirpZT  (const NIComplexNumber x[], int n,
                                       NIComplexNumber startingPoint,
                                       NIComplexNumber increment, int algorithm,
                                       int m, NIComplexNumber y[]);
AnalysisLibErrType CVIFUNC InvChirpZT (const NIComplexNumber x[], int m,
                       NIComplexNumber startPoint,
                       NIComplexNumber increment,
                       NIComplexNumber y[], int n);


/*- FFT Tables ----------------*/

PFFTTable CVIFUNC CreateFFTTable  (int n);
void      CVIFUNC DestroyFFTTable (PFFTTable table);

AnalysisLibErrType CVIFUNC FFTEx      (const double tmSig[], int nTmSig,
                                       int fftSize, PFFTTable fftTable,
                                       int shift, NIComplexNumber fft[]);
AnalysisLibErrType CVIFUNC InvFFTEx   (const NIComplexNumber fft[], int fftSize,
                                       PFFTTable fftTable, int shifted,
                                       double tmSig[]);
AnalysisLibErrType CVIFUNC CxFFTEx    (const NIComplexNumber tmSig[], int nTmSig,
                                       int fftSize, PFFTTable fftTable,
                                       int shift, NIComplexNumber fft[]);
AnalysisLibErrType CVIFUNC CxInvFFTEx (const NIComplexNumber fft[], int fftSize,
                                       PFFTTable fftTable, int shifted,
                                       NIComplexNumber tmSig[]);


/*- Time Domain Analysis -----------------------------------------------------*/

AnalysisLibErrType CVIFUNC Convolve   (const double x[], int n, const double y[],
                                       int m, double cxy[]);
AnalysisLibErrType CVIFUNC Correlate  (const double x[], int n, const double y[],
                                       int m, double rxy[]);
AnalysisLibErrType CVIFUNC Integrate  (const double x[], int n, double dt,
                                       double xInit, double xFinal, double y[]);
AnalysisLibErrType CVIFUNC Difference (const double x[], int n, double dt,
                                       double xInit, double xFinal, double y[]);
AnalysisLibErrType CVIFUNC PulseParam (const double pulsePattern[], int n,
                                       double *amp, double *amp90,
                                       double *amp50, double *amp10,
                                       double *top, double *base,
                                       double *topOvershoot,
                                       double *baseOvershoot, int *delay,
                                       int *width, int *riseTime, int *fallTime,
                                       double *slewRate);
AnalysisLibErrType CVIFUNC Decimate   (const double x[], int n, int D, int avg,
                                       double y[]);
AnalysisLibErrType CVIFUNC Deconvolve (const double y[], int ny,
                                       const double x[], int nx, double h[]);
AnalysisLibErrType CVIFUNC UnWrap1D   (double phase[], int n);
AnalysisLibErrType CVIFUNC CxDecimate (const NIComplexNumber x[], int n, int D,
                       int avg, NIComplexNumber y[]);


/*- IIR Digital Filters ------------------------------------------------------*/

AnalysisLibErrType CVIFUNC Bessel_CascadeCoef(double fs, double fl, double fh,
                                              IIRFilterPtr filterInfo);

AnalysisLibErrType CVIFUNC Bw_CascadeCoef    (double fs, double fl, double fh,
                                              IIRFilterPtr filterInfo);

AnalysisLibErrType CVIFUNC Ch_CascadeCoef    (double fs, double fl, double fh,
                                              double r, IIRFilterPtr filterInfo);

AnalysisLibErrType CVIFUNC InvCh_CascadeCoef (double fs, double fl, double fh,
                                              double atten, IIRFilterPtr filterInfo);

AnalysisLibErrType CVIFUNC Elp_CascadeCoef   (double fs, double fl, double fh,
                                              double ripple, double atten,
                                              IIRFilterPtr filterInfo);

AnalysisLibErrType CVIFUNC IIRCascadeFiltering(const double x[], int n,
                                               IIRFilterPtr filterInfo, double y[]);

IIRFilterPtr       CVIFUNC AllocIIRFilterPtr(int type, int order);

void               CVIFUNC ResetIIRFilter   (IIRFilterPtr filterInfo);

void               CVIFUNC FreeIIRFilterPtr (IIRFilterPtr filterInfo);

AnalysisLibErrType CVIFUNC CascadeToDirectCoef(IIRFilterPtr filterInfo, double a[],
                                               int na, double b[], int nb);

AnalysisLibErrType CVIFUNC Bw_LPF     (const double x[], int n, double fs,
                                       double fc, int order, double y[]);

AnalysisLibErrType CVIFUNC Bw_HPF     (const double x[], int n, double fs,
                                       double fc, int order, double y[]);

AnalysisLibErrType CVIFUNC Bw_BPF     (const double x[], int n, double fs,
                                       double fl, double fh, int order,
                                       double y[]);

AnalysisLibErrType CVIFUNC Bw_BSF     (const double x[], int n, double fs,
                                       double fl, double fh, int order,
                                       double y[]);

AnalysisLibErrType CVIFUNC Ch_LPF     (const double x[], int n, double fs,
                                       double fc, double ripple,
                                       int order, double y[]);

AnalysisLibErrType CVIFUNC Ch_HPF     (const double x[], int n, double fs,
                                       double fc, double ripple,
                                       int order, double y[]);

AnalysisLibErrType CVIFUNC Ch_BPF     (const double x[], int n, double fs,
                                       double fl, double fh,
                                       double ripple, int order, double y[]);

AnalysisLibErrType CVIFUNC Ch_BSF     (const double x[], int n, double fs,
                                       double fl, double fh,
                                       double ripple, int order, double y[]);

AnalysisLibErrType CVIFUNC InvCh_LPF  (const double x[], int n, double fs,
                                       double fc, double atten, int order,
                                       double y[]);

AnalysisLibErrType CVIFUNC InvCh_HPF  (const double x[], int n, double fs,
                                       double fc, double atten, int order,
                                       double y[]);

AnalysisLibErrType CVIFUNC InvCh_BPF  (const double x[], int n, double fs,
                                       double fl, double fh, double atten,
                                       int order, double y[]);

AnalysisLibErrType CVIFUNC InvCh_BSF  (const double x[], int n, double fs,
                                       double fl, double fh, double atten,
                                       int order, double y[]);

AnalysisLibErrType CVIFUNC Elp_LPF    (const double x[], int n, double fs,
                                       double fc, double ripple, double atten,
                                       int order, double y[]);

AnalysisLibErrType CVIFUNC Elp_HPF    (const double x[], int n, double fs,
                                       double fc, double ripple, double atten,
                                       int order, double y[]);

AnalysisLibErrType CVIFUNC Elp_BPF    (const double x[], int n, double fs,
                                       double fl, double fh, double ripple,
                                       double atten, int order, double y[]);

AnalysisLibErrType CVIFUNC Elp_BSF    (const double x[], int n, double fs,
                                       double fl, double fh, double ripple,
                                       double atten, int order, double y[]);

/*- Old-style IIR Filter Functions -*/

AnalysisLibErrType CVIFUNC Bessel_Coef(int type, int order, double fs,
                                       double fl, double fh, double a[],
                                       int na, double b[], int nb);

AnalysisLibErrType CVIFUNC Bw_Coef    (int type, int order, double fs,
                                       double fl, double fh, double a[],
                                       int na, double b[], int nb);

AnalysisLibErrType CVIFUNC Ch_Coef    (int type, int order, double fs,
                                       double fl, double fh, double r,
                                       double a[], int na, double b[], int nb);

AnalysisLibErrType CVIFUNC InvCh_Coef (int type, int order, double fs,
                                       double fl, double fh, double fr,
                                       double a[], int na, double b[], int nb);

AnalysisLibErrType CVIFUNC Elp_Coef   (int type, int order, double fs,
                                       double fl, double fh, double r1,
                                       double r2, double a[], int na,
                                       double b[], int nb);

AnalysisLibErrType CVIFUNC IIRFiltering(const double x[], int n,
                                        const double a[], double y1[], int na,
                                        const double b[], double x1[], int nb,
                                        double y[]);
AnalysisLibErrType CVIFUNC IIRFiltering_CxInput(const NIComplexNumber x[],
                        int n, const double a[],
                        NIComplexNumber y1[],
                        int na, const double b[],
                        NIComplexNumber x1[],
                        int nb, NIComplexNumber y[]);


/*- FIR Digital Filters ------------------------------------------------------*/

AnalysisLibErrType CVIFUNC Wind_LPF    (double fs, double fc, int n,
                                        double coef[], int windType);
AnalysisLibErrType CVIFUNC Wind_HPF    (double fs, double fc, int n,
                                        double coef[], int windType);
AnalysisLibErrType CVIFUNC Wind_BPF    (double fs, double fl, double fh, int n,
                                        double coef[], int windType);
AnalysisLibErrType CVIFUNC Wind_BSF    (double fs, double fl, double fh, int n,
                                        double coef[], int windType);
AnalysisLibErrType CVIFUNC Ksr_LPF     (double fs, double fc, int n,
                                        double coef[], double beta);
AnalysisLibErrType CVIFUNC Ksr_HPF     (double fs, double fc, int n,
                                        double coef[], double beta);
AnalysisLibErrType CVIFUNC Ksr_BPF     (double fs, double fl, double fh, int n,
                                        double coef[], double beta);
AnalysisLibErrType CVIFUNC Ksr_BSF     (double fs, double fl, double fh, int n,
                                        double coef[], double beta);
AnalysisLibErrType CVIFUNC Equi_Ripple (int bands, const double A[],
                                        const double wts[], double fs,
                                        const double cutoffs[], int type, int n,
                                        double coef[], double *delta);
AnalysisLibErrType CVIFUNC EquiRpl_LPF (double fs, double f1, double f2, int n,
                                        double coef[], double *delta);
AnalysisLibErrType CVIFUNC EquiRpl_HPF (double fs, double f1, double f2, int n,
                                        double coef[], double *delta);
AnalysisLibErrType CVIFUNC EquiRpl_BPF (double fs, double f1, double f2,
                                        double f3, double f4, int n,
                                        double coef[], double *delta);
AnalysisLibErrType CVIFUNC EquiRpl_BSF (double fs, double f1, double f2,
                                        double f3, double f4, int n,
                                        double coef[], double *delta);
AnalysisLibErrType CVIFUNC FIR_Coef    (int type, double fs, double fl,
                                        double fh, int taps, double *coef);
AnalysisLibErrType CVIFUNC WindFIR_Filter (int filterType, double fs, double fl,
                                           double fh, int taps, int winType,
                                           double winPara, double coef[]);
AnalysisLibErrType CVIFUNC FIRFiltering (const double x[], int n,
                     const double b[], double x1[],
                     int nb, double y[]);
AnalysisLibErrType CVIFUNC FIRFiltering_CxInput (const NIComplexNumber x[],
                         int n, const double b[],
                         NIComplexNumber x1[], int nb,
                         NIComplexNumber y[]);
AnalysisLibErrType CVIFUNC SavitzkyGolayCoef (int polyOrder, int nSidePoints,
                          double weight[], int needDiff,
                          double filterCoef[],
                          double diffCoef[]);
AnalysisLibErrType CVIFUNC SavitzkyGolayFiltering (double x[], int nx,
                           int polyOrder,
                           int nSidePoints,
                           double weight[], double y[]); 
AnalysisLibErrType CVIFUNC SavitzkyGolayFiltering_CxInput (NIComplexNumber x[],
                               int nx,
                               int polyOrder,
                               int nSidePoints,
                               double weight[],
                               NIComplexNumber y[]);


/*- Windows ------------------------------------------------------------------*/

AnalysisLibErrType CVIFUNC TriWin           (double x[], int n);
AnalysisLibErrType CVIFUNC HanWin           (double x[], int n);
AnalysisLibErrType CVIFUNC HamWin           (double x[], int n);
AnalysisLibErrType CVIFUNC BkmanWin         (double x[], int n);
AnalysisLibErrType CVIFUNC KsrWin           (double x[], int n, double beta);
AnalysisLibErrType CVIFUNC BlkHarrisWin     (double x[], int n);
AnalysisLibErrType CVIFUNC CosTaperedWin    (double x[], int n);
AnalysisLibErrType CVIFUNC CosTaperedWinEx  (double x[], int n, double r);
AnalysisLibErrType CVIFUNC ExBkmanWin       (double x[], int n);
AnalysisLibErrType CVIFUNC ExpWin           (double x[], int n, double final);
AnalysisLibErrType CVIFUNC FlatTopWin       (double x[], int n);
AnalysisLibErrType CVIFUNC ForceWin         (double x[], int n, double duty);
AnalysisLibErrType CVIFUNC GenCosWin        (double x[], int n, const double a[], int m);
AnalysisLibErrType CVIFUNC ChebWin          (double x[], int n, double s);
AnalysisLibErrType CVIFUNC GaussWin         (double x[], int n, double dev);
AnalysisLibErrType CVIFUNC SymWin           (double x[], int n, int winType, double para);
AnalysisLibErrType CVIFUNC BlkmanNuttallWin (double x[], int n);

AnalysisLibErrType CVIFUNC GetWinProperties    (int winType, WindowConst *windowconstants);
AnalysisLibErrType CVIFUNC NumericWinProperties(const double coef[], int nCoef,
                                                WindowConst *windowconstants);
AnalysisLibErrType CVIFUNC ScaledWindowEx (double x[], int n, int winType,
                                              double para, WindowConst *windowconstants);
AnalysisLibErrType CVIFUNC ParzenWin (double x[], int n);
AnalysisLibErrType CVIFUNC WelchWin (double x[], int n);
AnalysisLibErrType CVIFUNC BartHannWin (double x[], int n);
AnalysisLibErrType CVIFUNC BohmanWin (double x[], int n);


/*- Measurement --------------------------------------------------------------*/

AnalysisLibErrType CVIFUNC ACDCEstimator     (const double x[], int n,
                                              double *acestimate,
                                              double *dcestimate);
AnalysisLibErrType CVIFUNC AmpPhaseSpectrum  (const double x[], int n,
                                              int unwrap, double dt,
                                              double ampspectrum[],
                                              double phasespectrum[],
                                              double *df);
AnalysisLibErrType CVIFUNC AutoPowerSpectrum (const double x[], int n,
                                              double dt, double autospectrum[],
                                              double *df);
AnalysisLibErrType CVIFUNC CrossPowerSpectrum(const double x[], const double y[],
                                              int n, double dt, double magsxy[],
                                              double phasesxy[], double *df);
AnalysisLibErrType CVIFUNC ImpulseResponse   (const double stimulus[],
                                              const double response[], int n,
                                              double impulse[]);
AnalysisLibErrType CVIFUNC NetworkFunctions  (void *stimulus, void *response,
                                              int n, int numframes, double dt,
                                              double magsxy[],
                                              double phasesxy[],
                                              double magtransfer[],
                                              double phasetransfer[],
                                              double coherence[],
                                              double impulse[], double *df);
AnalysisLibErrType CVIFUNC PowerFrequencyEstimate (const double autospectrum[],
                                                   int n, double searchfreq,
                                                   WindowConst windowconstants,
                                                   double df, int span,
                                                   double *freqpeak,
                                                   double *powerpeak);
AnalysisLibErrType CVIFUNC ScaledWindow      (double x[], int n, int window,
                                              WindowConst *windowconstants);
AnalysisLibErrType CVIFUNC SpectrumUnitConversion (const double spectrum[],
                                                   int ns, int type, int loglin,
                                                   int unitselector, double df,
                                                   WindowConst windowconstants,
                                                   double convertedspectrum[],
                                                   char *unitstring);
AnalysisLibErrType CVIFUNC TransferFunction  (const double stimulus[],
                                              const double response[], int n,
                                              double dt, double magtransfer[],
                                              double phasetransfer[],
                                              double *df);
AnalysisLibErrType CVIFUNC HarmonicAnalyzer (const double *x, int n, int frame_size, int number_of_harmonics,
                                             int window_type, double sampling_rate, double fundamental_frequency,
                                             double harmonic_amplitudes[], double harmonic_frequencies[],
                                             double *thd, double *thdnoise);


/*- General Statistics -------------------------------------------------------*/

AnalysisLibErrType CVIFUNC Mean      (const double x[], int n, double *mean);
AnalysisLibErrType CVIFUNC StdDev    (const double x[], int n, double *mean,
                                      double *sDev);
AnalysisLibErrType CVIFUNC Variance  (const double x[], int n, double *mean,
                                      double *var);
AnalysisLibErrType CVIFUNC RMS       (const double x[], int n, double *rms);
AnalysisLibErrType CVIFUNC Moment    (const double x[], int n, int order,
                                      double *moment);
AnalysisLibErrType CVIFUNC Median    (const double x[], int n, double *median);
AnalysisLibErrType CVIFUNC Mode      (const double x[], int n, double base,
                                      double top, int intervals, double *mode);
AnalysisLibErrType CVIFUNC ModeEx    (const double x[], int n, int intervals,
                                      double *modes, int *m);
AnalysisLibErrType CVIFUNC Histogram (const double x[], int n, double base,
                                      double top, int hist[], double axis[],
                                      int intervals);


/*- Probability Distribution -------------------------------------------------*/

AnalysisLibErrType CVIFUNC N_Dist     (double x, double *p);
AnalysisLibErrType CVIFUNC T_Dist     (double t, int n, double *p);
AnalysisLibErrType CVIFUNC F_Dist     (double f, int n, int m, double *p);
AnalysisLibErrType CVIFUNC XX_Dist    (double x, int n, double *p);
AnalysisLibErrType CVIFUNC InvN_Dist  (double p, double *x);
AnalysisLibErrType CVIFUNC InvT_Dist  (double p, int n, double *t);
AnalysisLibErrType CVIFUNC InvF_Dist  (double p, int n, int m, double *f);
AnalysisLibErrType CVIFUNC InvXX_Dist (double p, int n, double *x);


/*- Analysis of Variance -----------------------------------------------------*/

AnalysisLibErrType CVIFUNC ANOVA1Way (const double y[], const int level[],
                                      int n, int k, double *ssa, double *msa,
                                      double *f, double *sig, double *sse,
                                      double *mse, double *tss);
AnalysisLibErrType CVIFUNC ANOVA2Way (const double y[], const int levelA[],
                                      const int levelB[], int N, int L,
                                      int a, int b, double info[4][4],
                                      double *sigA, double *sigB, double *sigAB);
AnalysisLibErrType CVIFUNC ANOVA3Way (const double y[], const int levelA[],
                                      const int levelB[], const int levelC[],
                                      int N, int L, int a, int b, int c,
                                      double info[8][4], double *sigA,
                                      double *sigB, double *sigC, double *sigAB,
                                      double *sigAC, double *sigBC,
                                      double *sigABC);


/*- Nonparametric Statistics -------------------------------------------------*/

AnalysisLibErrType CVIFUNC Contingency_Table (int s, int k, const void *y,
                                              double *Test_Stat, double *Sig);

AnalysisLibErrType CVIFUNC Erf (double x, double *erfx);
AnalysisLibErrType CVIFUNC Erfc (double x, double *erfcx);


/*- Curve Fitting ------------------------------------------------------------*/

AnalysisLibErrType CVIFUNC LinFit       (const double x[], const double y[],
                                         int n, double z[], double *slope,
                                         double *intercept, double *mse);

AnalysisLibErrType CVIFUNC ExpFit       (const double x[], const double y[],
                                         int n, double z[], double *a,
                                         double *b, double *mse);

AnalysisLibErrType CVIFUNC PolyFit      (const double x[], const double y[],
                                         int n, int order, double z[],
                                         double coef[], double *mse);

AnalysisLibErrType CVIFUNC GenLSFit     (const void *H, int row, int col,
                                         const double y[], const double stddev[],
                                         int algorithm,
                                         double z[], double coef[],
                                         void *covar, double *mse);

AnalysisLibErrType CVIFUNC NonLinearFit (const double x[], const double y[],
                                         double z[], int n, ModelFun *func,
                                         double coef[], int m, double *mse);

AnalysisLibErrType CVIFUNC NonLinearFitWithMaxIters (const double x[], const double y[],
                                                    const double z[], int n,
                                                    int maxiters, ModelFun *func,
                                                    double coef[], int m, double *mse);

AnalysisLibErrType CVIFUNC GaussFit      (const double x[], double y[], const double weight[],
                                          int n, int fitMethod, double tolerance,
                                          double initCoef[], double fittedData[],
                                          double *amplitude, double *center,
                                          double *stdDev, double *residue);
AnalysisLibErrType CVIFUNC LogFit        (const double x[], double y[], const double weight[],
                                          int n, double logBase, int fitMethod,
                                          double tolerance, double fittedData[],
                                          double *amplitude, double *scale, double *residue);
AnalysisLibErrType CVIFUNC PowerFit      (const double x[], double y[], const double weight[],
                                          int n, int fitMethod, double tolerance,
                                          double fittedData[], double *amplitude,
                                          double *power, double *residue);

AnalysisLibErrType CVIFUNC LinearFitEx   (const double x[], double y[], const double weight[],
                                          int n, int fitMethod, double tolerance,
                                          double fittedData[], double *slope,
                                          double *intercept, double *residue);
AnalysisLibErrType CVIFUNC ExpFitEx      (const double x[], double y[], const double weight[],
                                          int n, int fitMethod, double tolerance,
                                          double fittedData[], double *amplitude,
                                          double *damping, double *residue);
AnalysisLibErrType CVIFUNC PolyFitEx     (double x[], const double y[], int n, int order,
                                          const int specOrder[], const double specCoef[], int nSpec,
                                          int algorithm,
                                          double fittedData[], double coef[], double *mse);
AnalysisLibErrType CVIFUNC PolyFitWithWeight (double x[], const double y[],
                          const double weight[], int n,
                          int order, const int specOrder[],
                          const double specCoef[], int nSpec,
                          int algorithm,
                          double fittedData[], double coef[], double *mse);
AnalysisLibErrType CVIFUNC GoodnessOfFit (const double y[], const double fittedData[],
                                          const double weight[], int n, int degOfFree,
                                          double *SSE, double *rSquare, double *RMSE);
AnalysisLibErrType CVIFUNC CubicSplineFit(const double x[], double y[], const double weight[],
                                          int n, double balance, const double smoothness[], double fittedData[]);

AnalysisLibErrType CVIFUNC LinearFitInterval(const double x[], const double y[],
                                             const double weight[], int n,
                                             int intervalType, double confidenceLevel,
                                             double slope, double intercept,
                                             double upperBound[], double lowerBound[],
                                             double *deltaSlope, double *deltaIntcpt);
AnalysisLibErrType CVIFUNC ExpFitInterval   (const double x[], const double y[],
                                             const double weight[], int n,
                                             int intervalType, double confidenceLevel,
                                             double amplitude, double damping,
                                             double upperBound[], double lowerBound[],
                                             double *deltaAmp, double *deltaDamp);
AnalysisLibErrType CVIFUNC GaussFitInterval (const double x[], const double y[],
                                             const double weight[], int n,
                                             int intervalType, double confidenceLevel,
                                             double amplitude, double center,
                                             double stdDev, double upperBound[],
                                             double lowerBound[], double *deltaAmp,
                                             double *deltaCnt, double *deltaDev);
AnalysisLibErrType CVIFUNC LogFitInterval   (const double x[], const double y[],
                                             const double weight[], int n,
                                             double logBase, int intervalType,
                                             double confidenceLevel, double amplitude,
                                             double scale, double upperBound[],
                                             double lowerBound[], double *deltaAmp,
                                             double *deltaScl);
AnalysisLibErrType CVIFUNC PowerFitInterval (const double x[], const double y[],
                                             const double weight[], int n,
                                             int intervalType, double confidenceLevel,
                                             double amplitude, double power,
                                             double upperBound[], double lowerBound[],
                                             double *deltaAmp, double *deltaPow);

AnalysisLibErrType CVIFUNC RemoveOutlierByIndex (double x[], double y[],
                                                 double weight[], int *n,
                                                 const int index[], int nIndex);
AnalysisLibErrType CVIFUNC RemoveOutlierByRange (double x[], double y[],
                                                 double weight[], int *n,
                                                 int rangeType, const double range[],
                                                 int nRange, int outlierIndex[],
                                                 int *nOutlier);

/*- Old-style Curve Fitting Function (replaced by GenLSFit) -*/

AnalysisLibErrType CVIFUNC GenLSFitCoef (const void *H, int row, int col,
                                         const double y[], double coef[],
                                         int algorithm);


/*- Interpolation ------------------------------------------------------------*/

AnalysisLibErrType CVIFUNC PolyInterp (const double x[], const double y[],
                                       int n, double x_val, double *Interp_Val,
                                       double *Error);
AnalysisLibErrType CVIFUNC RatInterp  (const double x[], const double y[],
                                       int n, double x_val, double *Interp_Val,
                                       double *Error);
AnalysisLibErrType CVIFUNC SpInterp   (const double x[], const double y[],
                                       const double y2[], int n, double x_val,
                                       double *Interp_Val);
AnalysisLibErrType CVIFUNC Spline     (const double x[], const double y[],
                                       int n, double b1, double b2, double y2[]);


/*- Vector and Matrix Algebra ------------------------------------------------*/
/*- Real Matrices ----------------*/

AnalysisLibErrType CVIFUNC SpecialMatrix (int type, int m, const double x[], int nx,
                                          const double y[], int ny, void *z);
AnalysisLibErrType CVIFUNC DotProduct    (const double x[], const double y[],
                                          int n, double *dotProd);
AnalysisLibErrType CVIFUNC Transpose     (const void *x, int n, int m, void *y);
AnalysisLibErrType CVIFUNC Determinant   (const void *x, int n, double *determinant);
AnalysisLibErrType CVIFUNC GenDeterminant(void *a, int n, int type, double *determinant);
AnalysisLibErrType CVIFUNC Trace         (const void *x, int n, double *trace);
AnalysisLibErrType CVIFUNC InvMatrix     (const void *x, int n, void *y);
AnalysisLibErrType CVIFUNC GenInvMatrix  (void *a, int n, int type, void *mi);
AnalysisLibErrType CVIFUNC LinEqs        (const void *A, const double y[],
                                          int n, double x[]);
AnalysisLibErrType CVIFUNC GenLinEqs     (void *a, int n, int m, const double y[], int type,
                                          double x[]);
AnalysisLibErrType CVIFUNC MatrixMul     (const void *x, const void *y, int n,
                                          int k, int m, void *z);
AnalysisLibErrType CVIFUNC OuterProduct  (const double vectorX[], int nx,
                                          const double vectorY[], int ny,
                                          void *z);
AnalysisLibErrType CVIFUNC MatrixRank    (void *a, int n, int m, double tolerance, int *rank);
AnalysisLibErrType CVIFUNC MatrixNorm    (void *a, int n, int m, int normType, double *norm);
AnalysisLibErrType CVIFUNC ConditionNumber     (void *a, int n, int m, int normType,
                                                double *conditionNumber);
AnalysisLibErrType CVIFUNC SymEigenValueVector (void *a, int n, int outputChoice,
                                                double eigenValues[], void *eigenVectors);
AnalysisLibErrType CVIFUNC GenEigenValueVector (void *a, int n, int outputChoice,
                                                NIComplexNumber eigenValues[], void *eigenVectors);

AnalysisLibErrType CVIFUNC SVDS          (void *a, int n, int m, double s[]);
AnalysisLibErrType CVIFUNC SVD           (void *a, int n, int m, void *u, double s[], void *v);
AnalysisLibErrType CVIFUNC QR            (void *a, int n, int m, int algorithm,
                                          void *q, void *r);
AnalysisLibErrType CVIFUNC Cholesky      (void *a, int n, void *r);
AnalysisLibErrType CVIFUNC PseudoInverse (void *a, int n, int m, double tolerance,
                                          void *mpi);
AnalysisLibErrType CVIFUNC CheckPosDef   (void *a, int n, int *positiveDefinite);
AnalysisLibErrType CVIFUNC LU            (void *a, int n, int p[], int *sign);
AnalysisLibErrType CVIFUNC ForwSub       (const void *a, const double y[],
                                          int n, double x[], const int p[]);
AnalysisLibErrType CVIFUNC BackSub       (const void *a, const double y[],
                                          int n, double x[]);

AnalysisLibErrType CVIFUNC SVDEx         (const void *a, int n, int m,
                                          int sizeOption, void *u, double s[], void *v);
AnalysisLibErrType CVIFUNC QREx          (void *a, int n, int m, int pvt,
                                          int sizeOption, void *pvtInfo, void *q, void *r);
AnalysisLibErrType CVIFUNC MatrixBalance (void *a, int n, int method, int *ilo,
                                          int *ihi, double scale[], void *b);
AnalysisLibErrType CVIFUNC SolveEqs            (const void *a, int n, int m,
                                                int typeA, void *y, int ny, void *x);
AnalysisLibErrType CVIFUNC EigenVBack          (void *inV, int n, int side, int method,
                                                int ilo, int ihi, const double scale[],
                                                void *outV);
AnalysisLibErrType CVIFUNC GenEigenAB          (const void *a, const void *b, int n,
                                                NIComplexNumber eigenvalues[], void *leftVec,
                                                void *rightVec);
AnalysisLibErrType CVIFUNC Hess                (void *a, int n, void *h, void *q);
AnalysisLibErrType CVIFUNC QZ                  (const void *a, const void *b, int n, int order,
                                                void *aa, void *bb, void *q, void *z,
                                                NIComplexNumber alpha[], double beta[],
                                                void *leftVec, void *rightVec);
AnalysisLibErrType CVIFUNC Schur               (const void *a, int n, int order,
                                                void *q, void *s, NIComplexNumber eigenvalues[]);
AnalysisLibErrType CVIFUNC KroneckerProd (const void *x, const void *y,
                      int n, int m, int k, int l,
                      void *KronProd);


/*- Complex Matrices ---------------------*/

AnalysisLibErrType CVIFUNC CxSpecialMatrix     (int matrixType, int m, const NIComplexNumber x[], int nx,
                                                const NIComplexNumber y[], int ny, void *z);
AnalysisLibErrType CVIFUNC CxDotProduct        (const NIComplexNumber x[], const NIComplexNumber y[],
                                                int n, NIComplexNumber *dotProduct);
AnalysisLibErrType CVIFUNC CxTranspose         (void *x, int n, int m, void *y);
AnalysisLibErrType CVIFUNC CxDeterminant       (void *x, int n, int matrixType, NIComplexNumber *determinant);
AnalysisLibErrType CVIFUNC CxTrace             (void *x, int n, int reserved, NIComplexNumber *trace);
AnalysisLibErrType CVIFUNC CxGenInvMatrix      (void *a, int n, int matrixType, void *mi);
AnalysisLibErrType CVIFUNC CxGenLinEqs         (void *a, int n, int m, const NIComplexNumber y[], int matrixType,
                                                NIComplexNumber x[]);
AnalysisLibErrType CVIFUNC CxMatrixMul         (const void *x, const void *y, int n,
                                                int k, int m, void *z);
AnalysisLibErrType CVIFUNC CxOuterProduct      (const NIComplexNumber vectorX[], int nx,
                                                const NIComplexNumber vectorY[], int ny,
                                                void *outerProduct);
AnalysisLibErrType CVIFUNC CxMatrixRank        (void *a, int n, int m, double tolerance, int *rank);
AnalysisLibErrType CVIFUNC CxMatrixNorm        (void *a, int n, int m, int normType, double *norm);
AnalysisLibErrType CVIFUNC CxConditionNumber   (void *a, int n, int m, int normType,
                                                double *conditionNumber);
AnalysisLibErrType CVIFUNC CxEigenValueVector  (void *a, int n, int matrixType, int outputChoice,
                                                NIComplexNumber eigenValues[], void *eigenVectors);
AnalysisLibErrType CVIFUNC CxSVDS              (void *a, int n, int m, NIComplexNumber s[]);
AnalysisLibErrType CVIFUNC CxSVD               (void *a, int n, int m, void *u, NIComplexNumber s[], void *v);
AnalysisLibErrType CVIFUNC CxQR                (void *a, int n, int m, void *q, void *r);
AnalysisLibErrType CVIFUNC CxCholesky          (void *a, int n, void *r);
AnalysisLibErrType CVIFUNC CxPseudoInverse     (void *a, int n, int m, double tolerance,
                                                void *mpi);
AnalysisLibErrType CVIFUNC CxCheckPosDef       (void *a, int n, int *positiveDefinite);
AnalysisLibErrType CVIFUNC CxLU                (void *a, int n, int p[], int *sign);

AnalysisLibErrType CVIFUNC CxSVDEx             (const void *a, int n, int m,
                                                int sizeOption, void *u, double s[], void *v);
AnalysisLibErrType CVIFUNC CxQREx              (void *a, int n, int m,
                                                int pvt, int sizeOption, void *pvtInfo,
                                                void *q, void *r);
AnalysisLibErrType CVIFUNC CxMatrixBalance     (void *a, int n, int method, int *ilo,
                                                int *ihi, double scale[], void *b);
AnalysisLibErrType CVIFUNC CxSolveEqs          (const void *a, int n, int m,
                                                int matrixType, void *y, int ny, void *x);
AnalysisLibErrType CVIFUNC CxEigenVBack        (void *inV, int n, int side, int method,
                                                int ilo, int ihi, const double scale[],
                                                void *outV);
AnalysisLibErrType CVIFUNC CxGenEigenAB        (const void *a, const void *b, int n,
                                                NIComplexNumber eigenvalues[], void *leftVec,
                                                void *rightVec);
AnalysisLibErrType CVIFUNC CxHess              (void *a, int n, void *h, void *q);
AnalysisLibErrType CVIFUNC CxQZ                (const void *a, const void *b, int n, int order,
                                                void *aa, void *bb, void *q, void *z,
                                                NIComplexNumber alpha[], NIComplexNumber beta[],
                                                void *leftVec, void *rightVec);
AnalysisLibErrType CVIFUNC CxSchur             (const void *a, int n, int order,
                                                void *q, void *s, NIComplexNumber eigenvalues[]);
AnalysisLibErrType CVIFUNC CxKroneckerProd (const void *x, const void *y,
                  int n, int m, int k, int l, void *KronProd);


/*- Additional Numeric Functions ---------------------------------------------*/

AnalysisLibErrType CVIFUNC PolyRootsEx  (const double     x[], int n, NIComplexNumber y[]);
AnalysisLibErrType CVIFUNC CxPolyRoots  (const double     x[], int n, NIComplexNumber y[]);
AnalysisLibErrType CVIFUNC CxPolyRootsEx(const NIComplexNumber x[], int n, NIComplexNumber y[]);

AnalysisLibErrType CVIFUNC NumericIntegration (const double x[], int n, double dt,
                                               int integrationMethod, double *ir);
AnalysisLibErrType CVIFUNC PeakDetector (const double x[], int n, double threshold, int width,
                                         int polarity, int initialize, int endOfData, int *count,
                                         double **pL, double **pA, double **pSD);
AnalysisLibErrType CVIFUNC ThresholdPeakDetector (const double x[], int n, double threshold,
                                                  int width, int peakIndices[], int *count);

/* Special Functions */

void   CVIFUNC Airy             (double x, double *ai, double *bi);
double CVIFUNC Bessel1st        (double r, double x);
double CVIFUNC Bessel2nd        (double r, double x);
double CVIFUNC SphBessel1st     (int n, double x);
double CVIFUNC SphBessel2nd     (int n, double x);
double CVIFUNC ModBessel1st     (double r, double x);
double CVIFUNC ModBessel2nd     (int n, double x);
double CVIFUNC Beta             (double x, double y, double *a);
double CVIFUNC Fact             (int n);
double CVIFUNC Gamma            (double x, double *a);
double CVIFUNC GammaC           (double x, double a);
double CVIFUNC LnFact           (int n);
double CVIFUNC LnGamma          (double x);
double CVIFUNC Psi              (double x);
double CVIFUNC Stirling         (double x);
double CVIFUNC GaussHG          (double x, double a, double b, double c);
double CVIFUNC Kummer           (double x, double a, double b);
double CVIFUNC Tricomi          (double x, double a, double b);
double CVIFUNC Elliptic1st      (double k, double *a);
double CVIFUNC Elliptic2nd      (double k, double *a);
double CVIFUNC Dawson           (double x);
void   CVIFUNC FresnelIntegrals (double x, double *fSinI, double *fCosI);
double CVIFUNC ExpIntegral      (double x, int n);
double CVIFUNC CosIntegral      (double x);
double CVIFUNC SinIntegral      (double x);
double CVIFUNC CoshIntegral     (double x);
double CVIFUNC SinhIntegral     (double x);
void   CVIFUNC JacobiEllipticI  (double u, double k, double *cn, double *sn,
                                 double *dn, double *phi);
void   CVIFUNC Kelvin1st        (double x, int n, NIComplexNumber *be);
void   CVIFUNC Kelvin2nd        (double x, int n, NIComplexNumber *ke);
double CVIFUNC Dilogarithm      (double x);
double CVIFUNC ParabolicCylinder(double x, double v);
double CVIFUNC Struve           (double x, double v);
double CVIFUNC Zeta             (double x);


/*- Error String -------------------------------------------------------------*/

void CVIFUNC FreeAnalysisMem (void *pointer);

char * CVIFUNC GetAnalysisErrorString(int errorNum);

/*- The End ------------------------------------------------------------------*/

#ifdef __cplusplus
 }
#endif

#endif /* !defined(_ANALYSIS_H) */
