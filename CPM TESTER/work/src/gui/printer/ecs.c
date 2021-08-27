#pragma pack(4)
typedef struct {char *name; void *address; unsigned long isFunction:1; unsigned long reserved:31;} ExeSymbol;
int __cdecl OnConnect (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnDisconnect (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnPrint (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnReset (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnStatus (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __UICallbackSymbolCount = 5;
ExeSymbol __UICallbackSymbols [5] =
{
 {"_OnConnect", (void*)OnConnect, 1, 0},
 {"_OnDisconnect", (void*)OnDisconnect, 1, 0},
 {"_OnPrint", (void*)OnPrint, 1, 0},
 {"_OnReset", (void*)OnReset, 1, 0},
 {"_OnStatus", (void*)OnStatus, 1, 0}
};