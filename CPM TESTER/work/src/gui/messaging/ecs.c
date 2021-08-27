#pragma pack(4)
typedef struct {char *name; void *address; unsigned long isFunction:1; unsigned long reserved:31;} ExeSymbol;
int __cdecl OnBox (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnError (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnErrorExit (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnPrinter (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnSAP (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnSAPErrorTimer (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnTimer (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnTimerBox (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnTimerTick (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __UICallbackSymbolCount = 9;
ExeSymbol __UICallbackSymbols [9] =
{
 {"_OnBox", (void*)OnBox, 1, 0},
 {"_OnError", (void*)OnError, 1, 0},
 {"_OnErrorExit", (void*)OnErrorExit, 1, 0},
 {"_OnPrinter", (void*)OnPrinter, 1, 0},
 {"_OnSAP", (void*)OnSAP, 1, 0},
 {"_OnSAPErrorTimer", (void*)OnSAPErrorTimer, 1, 0},
 {"_OnTimer", (void*)OnTimer, 1, 0},
 {"_OnTimerBox", (void*)OnTimerBox, 1, 0},
 {"_OnTimerTick", (void*)OnTimerTick, 1, 0}
};