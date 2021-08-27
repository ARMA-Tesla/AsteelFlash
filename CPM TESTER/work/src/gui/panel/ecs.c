#pragma pack(4)
typedef struct {char *name; void *address; unsigned long isFunction:1; unsigned long reserved:31;} ExeSymbol;
int __cdecl Cb_Expand_Menu (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnPanel (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnPanelCallback (int panel, int event, void *callbackData, int eventData1, int eventData2);
int __UICallbackSymbolCount = 3;
ExeSymbol __UICallbackSymbols [3] =
{
 {"_Cb_Expand_Menu", (void*)Cb_Expand_Menu, 1, 0},
 {"_OnPanel", (void*)OnPanel, 1, 0},
 {"_OnPanelCallback", (void*)OnPanelCallback, 1, 0}
};