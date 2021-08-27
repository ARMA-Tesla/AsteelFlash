#pragma pack(4)
typedef struct {char *name; void *address; unsigned long isFunction:1; unsigned long reserved:31;} ExeSymbol;
int __cdecl OnDielectric (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __UICallbackSymbolCount = 1;
ExeSymbol __UICallbackSymbols [1] =
{
 {"_OnDielectric", (void*)OnDielectric, 1, 0}
};