/*---------------------------------------------------------------------------*/
/*                                                                           */
/* FILE:    piechart.c                                                       */
/*                                                                           */
/* PURPOSE: This sample instrument driver, used by the piedemo sample, shows */
/*          you how you can take advantage of Canvas controls to implement   */
/*          your own UI controls, and then share that functionality in       */
/*          Instrument Driver form.  This very simple Instrument plots a Pie */
/*          Chart on a panel using a Canvas.                                 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Include files                                                             */
/*---------------------------------------------------------------------------*/
#include <formatio.h>
#include <ansi_c.h>
#include <userint.h>
#include "piechart.h"

/*---------------------------------------------------------------------------*/
/* Internal function prototypes                                              */
/*---------------------------------------------------------------------------*/
void GetArcPoints(Point *arcPoints);

/*---------------------------------------------------------------------------*/
/* Module-globals                                                            */
/*---------------------------------------------------------------------------*/
static int    g_radius;
static double g_angleRad;
static double g_lastAngleRad;

/*---------------------------------------------------------------------------*/
/* Do all the work of creating a pie chart on a Canvas given a set of        */
/* parameters.                                                               */
/*---------------------------------------------------------------------------*/
int CreatePie (int hpanel, pieInfo pieAttr, piePiece piece, int legend)
{
    static int pieID;
    static int i;
    static int memoryForPoints;
    static int j;
    
    int    sumOfPieces     = 0;
    int    otherPiece      = 0;
    int    labelWidth;
    char   otherLegend[50];
    Point* arcPoints;
    Point  origin;

    /* Initialize variables */
    g_angleRad = 0;
    g_lastAngleRad = 0;
    
    /* Sum percentages and return an error if it is > 100 */
    for (i = 0; i < piece.numPieces; i++)
        sumOfPieces += piece.piecePercent[i];
    if (sumOfPieces > 100)
        return -10000;
    else if (sumOfPieces < 100)
        
        /* Create otherPiece to fill out the chart */
        otherPiece = 100 - sumOfPieces;  
    
    /* Create the canvas control */
    if ((pieID = NewCtrl (hpanel, CTRL_CANVAS, pieAttr.title, pieAttr.top,
                          pieAttr.left)) < 0)
        return pieID;
        
    /* Size canvas control depending on whether legend is requested */
    SetCtrlAttribute (hpanel, pieID, ATTR_HEIGHT, pieAttr.height);
    if (legend)
        SetCtrlAttribute (hpanel, pieID, ATTR_WIDTH,
                          pieAttr.width + LEGENDWIDTH);
    else
        SetCtrlAttribute (hpanel, pieID, ATTR_WIDTH, pieAttr.width);
        
    /* Position the Pie Chart's label */
    GetCtrlAttribute (hpanel, pieID, ATTR_LABEL_WIDTH, &labelWidth);
    SetCtrlAttribute (hpanel, pieID, ATTR_LABEL_LEFT,
                      pieAttr.left + (pieAttr.width / 2) - (labelWidth / 2));
    
    /* Determine origin of Pie Chart circle */
    if (pieAttr.height > pieAttr.width)
        {
        origin.x = pieAttr.width / 2;
        origin.y = pieAttr.width / 2;
        }
    else
        {
        origin.x = pieAttr.height / 2;
        origin.y = pieAttr.height / 2;
        }
    g_radius = origin.x;
    
    /* Start canvas batch drawing */
    CanvasStartBatchDraw (hpanel, pieID);
    
    j = -2;
    
    /* For all pieces with percentages > 0, create arc (using GetArcPoints)  */
    /* and draw arc using CanvasDrawPoly().  Create legend if legend was     */
    /* requested.                                                            */
    for (i = 0; i < piece.numPieces; i++)
        {
        if (piece.piecePercent[i] > 0)
            {
            j++;
            g_angleRad += piece.piecePercent[i] * 2 * PI / 100;
            memoryForPoints = (int)sizeof(Point)
                              * ((g_angleRad - g_lastAngleRad)
                                 / (g_radius * DETAILLEVEL) + 3);
            arcPoints = malloc (memoryForPoints);
            GetArcPoints (arcPoints);
            SetCtrlAttribute (hpanel, pieID, ATTR_PEN_FILL_COLOR,
                              piece.colors[i]);
            CanvasDrawPoly (hpanel, pieID, memoryForPoints / sizeof(Point),
                            arcPoints, 1, VAL_DRAW_FRAME_AND_INTERIOR);
            if (legend)
                {
                CanvasDrawRect (hpanel, pieID,  
                                MakeRect (pieAttr.top + (20 * j),
                                          pieAttr.width + 20, 15, 15),
                                VAL_DRAW_FRAME_AND_INTERIOR);
                SetCtrlAttribute (hpanel, pieID, ATTR_PEN_FILL_COLOR,
                                  VAL_TRANSPARENT);
                Fmt (piece.legendInfo[i], "%s[a]< (%d%%)",
                     piece.piecePercent[i]);
                CanvasDrawTextAtPoint (hpanel, pieID, piece.legendInfo[i],
                                       VAL_APP_META_FONT, 
                                       MakePoint (pieAttr.width + 40,
                                                  pieAttr.top + (20 * j)),
                                                  VAL_UPPER_LEFT);
                }
            free (arcPoints);
            }
        }
        
    /* Draw arc for last "other" wedge, if it exists */
    if (otherPiece > 0)
        {
        j++;
        g_angleRad = 2 * PI;
        memoryForPoints = (int)sizeof(Point) * ((g_angleRad - g_lastAngleRad)
                                           / (g_radius * DETAILLEVEL) + 3);
        arcPoints = malloc (memoryForPoints);
        GetArcPoints (arcPoints);
        SetCtrlAttribute (hpanel, pieID, ATTR_PEN_FILL_COLOR, VAL_DK_GRAY);
        CanvasDrawPoly (hpanel, pieID, memoryForPoints / sizeof(Point),
                        arcPoints, 1, VAL_DRAW_FRAME_AND_INTERIOR);
        if (legend)
            {
            CanvasDrawRect (hpanel, pieID, 
                            MakeRect (pieAttr.top + (20 * j),
                                      pieAttr.width + 20, 15, 15),
                            VAL_DRAW_FRAME_AND_INTERIOR);
            SetCtrlAttribute (hpanel, pieID, ATTR_PEN_FILL_COLOR, 
                              VAL_TRANSPARENT);
            Fmt (otherLegend, "%s<Other (%d%%)", otherPiece);
            CanvasDrawTextAtPoint (hpanel, pieID, otherLegend,
                                   VAL_APP_META_FONT, 
                                   MakePoint (pieAttr.width + 40,
                                              pieAttr.top + (20 * j)),
                                              VAL_UPPER_LEFT);
            }
        free (arcPoints);
        }
    
    /* We're finished with the batch drawing */
    CanvasEndBatchDraw (hpanel, pieID);
    return pieID;
}

/*---------------------------------------------------------------------------*/
/* This routine creates points for an arc.                                   */
/*---------------------------------------------------------------------------*/
void GetArcPoints (Point *arcPoints)
{
    int    j = 0;
    double i;
    double x;
    double y;
    
    arcPoints[0] = MakePoint (g_radius, g_radius);
    for (i = g_lastAngleRad; i < g_angleRad; i += g_radius * DETAILLEVEL)
        {
        x = g_radius * cos (i);
        y = g_radius * sin (i);
        arcPoints[++j] = MakePoint (g_radius + x, g_radius - y);
        }
    x = g_radius * cos (g_angleRad);
    y = g_radius * sin (g_angleRad);
    arcPoints[++j] = MakePoint (g_radius + x, g_radius - y);
    g_lastAngleRad = g_angleRad;
}
