#pragma pack(4)
typedef struct {char *name; void *address; unsigned long isFunction:1; unsigned long reserved:31;} ExeSymbol;
int __cdecl OnCalDisplay (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnFileList (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnMode (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnMonthChange (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __UICallbackSymbolCount = 4;
ExeSymbol __UICallbackSymbols [4] =
{
 {"_OnCalDisplay", OnCalDisplay, 1, 0},
 {"_OnFileList", OnFileList, 1, 0},
 {"_OnMode", OnMode, 1, 0},
 {"_OnMonthChange", OnMonthChange, 1, 0}
};
