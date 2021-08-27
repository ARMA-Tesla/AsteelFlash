#pragma pack(4)
typedef struct {char *name; void *address; unsigned long isFunction:1; unsigned long reserved:31;} ExeSymbol;
int __cdecl OnLoginList (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnPassword (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __UICallbackSymbolCount = 2;
ExeSymbol __UICallbackSymbols [2] =
{
 {"_OnLoginList", (void*)OnLoginList, 1, 0},
 {"_OnPassword", (void*)OnPassword, 1, 0}
};