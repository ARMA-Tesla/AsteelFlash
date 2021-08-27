#pragma pack(4)
typedef struct {char *name; void *address; unsigned long isFunction:1; unsigned long reserved:31;} ExeSymbol;
int __cdecl OnGen (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __UICallbackSymbolCount = 1;
ExeSymbol __UICallbackSymbols [1] =
{
 {"_OnGen", (void*)OnGen, 1, 0}
};