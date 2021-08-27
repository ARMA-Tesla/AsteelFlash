#pragma pack(4)
typedef struct {char *name; void *address; unsigned long isFunction:1; unsigned long reserved:31;} ExeSymbol;
int __cdecl OnAppPanel (int panel, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnInfoExit (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __UICallbackSymbolCount = 2;
ExeSymbol __UICallbackSymbols [2] =
{
 {"_OnAppPanel", (void*)OnAppPanel, 1, 0},
 {"_OnInfoExit", (void*)OnInfoExit, 1, 0}
};