#pragma pack(4)
typedef struct {char *name; void *address; unsigned long isFunction:1; unsigned long reserved:31;} ExeSymbol;
int __cdecl OnConnection (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnFieldLabel (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnMvt (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnSMM (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnTraca (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __UICallbackSymbolCount = 5;
ExeSymbol __UICallbackSymbols [5] =
{
 {"_OnConnection", (void*)OnConnection, 1, 0},
 {"_OnFieldLabel", (void*)OnFieldLabel, 1, 0},
 {"_OnMvt", (void*)OnMvt, 1, 0},
 {"_OnSMM", (void*)OnSMM, 1, 0},
 {"_OnTraca", (void*)OnTraca, 1, 0}
};