s??         
?     
b   D   ?   ????                                           Pie Chart                                        ? ??piePiece     ? ??pieInfo   Q    This function automatically creates a canvas control to display a pie chart on.     ,    Handle of the panel for the new pie chart.    @    Pie Attributes is a structure containing the following:

struct pieInfoStruct {
    int top;
    int left;
    int width;
    int height;
    char *title;
    };

top, left specifies the position of the top left corner of the canvas control with respect to the top left corner of the panel.

width, height specifies the width and height of the canvas control.  If the pie chart legend is turned on, the width of the canvas control is increased by LEGENDWIDTH (defined in piechart.h and defaults to 200).

title is a character string that will be used to label the pie chart.    ?    The pie wedges in the pie chart are defined in a structure containing the following:

struct piePieceStruct {
    int numPieces;
    int *piecePercent;
    int *colors;
    char **legendInfo;
    };

numPieces specifies the number of wedges that you will be defining.

piecePercent is an integer array that specifies the percent of the pie that each wedge will occupy.  The size of this array will vary depending on the value of numPieces (the number of wedges).  If the piecePercent elements do not add up to 100, the remaining percent will be displayed in another wedge labeled "Other".

colors is an integer array that specifies the color of the associated wedge.  For example, piecePercent[4]'s color would be defined by colors[4].

legendInfo is an array of strings that specifies the label given to each pie wedge.  For example, legendInfo[3] would contain a string that defines what piecePercent[3] is.     ?    Determines whether a legend will be created on the canvas control for the pie chart.  If a legend is created, the canvas control's width is automatically increased by LEGENDWIDTH pixels (default to 200) defined in piechart.h.        If the return value is less than zero, an error has occurred in the creating of the canvas control.  Check the return code errors from NewCtrl for an explanation.

If the return value is -10000, the sum of the percentages in the piePiece structure is greater than 100 percent.      4 O           Panel Handle                      4 4 ? ?  i    Pie Attributes                    | 4p ?  i    Pie Wedges                         x          Legend?                           ????         Pie Control ID                               "  On VAL_LEGENDON Off VAL_LEGENDOFF    	         ????          ?  	     K.    CreatePie                           ????                                     DCreate Pie Chart                