#include <windows.h> 
#include <utility.h>
#include "toolbox.h"
#include "graph_gui.h"

#define checkErr( fnc) \
	if(error = (fnc), error<0) \
		goto Error; \
	else

#define GRAPH_FILE	"\\graph.bmp.tmp"

static int 		g_color[] =
{
	VAL_RED,
	VAL_GREEN,
	VAL_BLUE,
	VAL_CYAN, 
	VAL_MAGENTA, 
	VAL_YELLOW, 
	VAL_DK_RED,
	VAL_DK_BLUE, 
	VAL_DK_GREEN, 
	VAL_DK_CYAN, 
	VAL_DK_MAGENTA, 
	VAL_DK_YELLOW, 
	VAL_LT_GRAY, 
	VAL_DK_GRAY, 
	VAL_BLACK
};

static int		g_panel = 0;
static int		g_colorPos = 0;

int __stdcall DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH:
			if (InitCVIRTE (hinstDLL, 0, 0) == 0)
				return 0;	  /* out of memory */
			break;
		case DLL_PROCESS_DETACH:
			CloseCVIRTE ();
			break;
		}
	
	return 1;
}

int __stdcall DllEntryPoint (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	/* Included for compatibility with Borland */

	return DllMain (hinstDLL, fdwReason, lpvReserved);
}


int __cdecl PlotGraph(double* x_array, double* y_array, int size)
{
	int		error = 0;
	
	if(!g_panel)
		g_panel = LoadPanelEx (0, "graph_gui.uir", PANEL, __CVIUserHInst);
	
	checkErr( PlotXY (g_panel, PANEL_GRAPH, x_array, y_array, size, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, g_color[g_colorPos++]));

	if(g_colorPos>(sizeof(g_color)/sizeof(g_color[0])))
		g_colorPos--;
		
Error:
	return error;
}

int __cdecl SaveGraph (const char* file_name)
{
	int		error = 0;
	int		bitmap_id;
	char	path[MAX_PATHNAME_LEN];
	
	if(!g_panel)
		g_panel = LoadPanelEx (0, "graph_gui.uir", PANEL, __CVIUserHInst);
	
	checkErr( GetModuleDir(__CVIUserHInst, path));	
	strcat(path, GRAPH_FILE);
	checkErr( SavePanelDisplayToFile (g_panel, 0, VAL_ENTIRE_OBJECT, -1, -1, path));
	checkErr( GetBitmapFromFile (path, &bitmap_id));
	checkErr( SaveBitmapToPNGFile (bitmap_id, file_name));
	DeleteFile(path);
	
Error:
	if(bitmap_id)
		DiscardBitmap(bitmap_id);
	
	if(g_panel)
	{
		DiscardPanel(g_panel);
		g_panel = 0;
		g_colorPos = 0;
	}
	
	return error;
}
