#pragma pack(4)
typedef struct {char *name; void *address; unsigned long isFunction:1; unsigned long reserved:31;} ExeSymbol;
int __cdecl OnPausePanel (int panel, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnPauseSelect (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnTimerTick (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __UICallbackSymbolCount = 3;
ExeSymbol __UICallbackSymbols [3] =
{
 {"_OnPausePanel", OnPausePanel, 1, 0},
 {"_OnPauseSelect", OnPauseSelect, 1, 0},
 {"_OnTimerTick", OnTimerTick, 1, 0}
};
