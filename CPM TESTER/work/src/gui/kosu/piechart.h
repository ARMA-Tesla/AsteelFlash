/*---------------------------------------------------------------------------*/
/*                                                                           */
/* FILE:    piechart.h                                                       */
/*                                                                           */
/* PURPOSE: This is the header file for the PIECHART sample Instrument       */
/*          See piechart.c or piedemo.c for more details.                    */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Defines                                                                   */
/*---------------------------------------------------------------------------*/
#define VAL_LEGENDON 1
#define VAL_LEGENDOFF 0
#define PI 3.1415926535897932384626433832795028841971
#define DETAILLEVEL .0005
#define LEGENDWIDTH 200

/*---------------------------------------------------------------------------*/
/* Types                                                                     */
/*---------------------------------------------------------------------------*/
struct piePieceStruct
{
    int numPieces;
    int *piecePercent;
    int *colors;
    char **legendInfo;
};
struct pieInfoStruct
{
    int top;
    int left;
    int width;
    int height;
    char *title;
};
typedef struct pieInfoStruct pieInfo;
typedef struct piePieceStruct piePiece;

/*---------------------------------------------------------------------------*/
/* Exported function prototypes                                              */
/*---------------------------------------------------------------------------*/
int CreatePie(int pnlHandle, pieInfo pieAttr, piePiece piece, int legend);     
