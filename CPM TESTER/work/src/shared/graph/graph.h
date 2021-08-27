#if !defined(__GRAPH_FNC_H__)
#define __GRAPH_FNC_H__

#include <windows.h>

#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif

int __cdecl PlotGraph(double* x_array, double* y_array, int size);
int __cdecl SaveGraph (const char* file_name);


#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif

#endif  /* __GRAPH_FNC_H__ */
