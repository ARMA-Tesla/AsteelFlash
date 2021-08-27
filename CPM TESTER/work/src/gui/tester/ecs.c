#pragma pack(4)
typedef struct {char *name; void *address; unsigned long isFunction:1; unsigned long reserved:31;} ExeSymbol;
int __cdecl Cb_ChangeTab (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnFncStation1 (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnFncStation2 (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnFncStation3 (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnFncStation4 (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnFncStation5 (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnFncStation6 (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnFncTester (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __UICallbackSymbolCount = 8;
ExeSymbol __UICallbackSymbols [8] =
{
 {"_Cb_ChangeTab", (void*)Cb_ChangeTab, 1, 0},
 {"_OnFncStation1", (void*)OnFncStation1, 1, 0},
 {"_OnFncStation2", (void*)OnFncStation2, 1, 0},
 {"_OnFncStation3", (void*)OnFncStation3, 1, 0},
 {"_OnFncStation4", (void*)OnFncStation4, 1, 0},
 {"_OnFncStation5", (void*)OnFncStation5, 1, 0},
 {"_OnFncStation6", (void*)OnFncStation6, 1, 0},
 {"_OnFncTester", (void*)OnFncTester, 1, 0}
};