#pragma pack(4)
typedef struct {char *name; void *address; unsigned long isFunction:1; unsigned long reserved:31;} ExeSymbol;
int __UICallbackSymbolCount = 0;
ExeSymbol *__UICallbackSymbols = (ExeSymbol *)0;