#pragma pack(4)
typedef struct {char *name; void *address; unsigned long isFunction:1; unsigned long reserved:31;} ExeSymbol;
int __cdecl Cb_ChangeTab (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnBKTREE (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnCP (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnCopySourceSelect (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnDbDeleteHist (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnDbInsert (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnDbRead (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnGroup (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnHisFillter (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnHisMerge (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnHistList (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnImportCsvFile (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnIniLimits (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnOpenCsvFile (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnRightsSelection (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnTimer (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __cdecl OnVtype (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int __UICallbackSymbolCount = 17;
ExeSymbol __UICallbackSymbols [17] =
{
 {"_Cb_ChangeTab", Cb_ChangeTab, 1, 0},
 {"_OnBKTREE", OnBKTREE, 1, 0},
 {"_OnCP", OnCP, 1, 0},
 {"_OnCopySourceSelect", OnCopySourceSelect, 1, 0},
 {"_OnDbDeleteHist", OnDbDeleteHist, 1, 0},
 {"_OnDbInsert", OnDbInsert, 1, 0},
 {"_OnDbRead", OnDbRead, 1, 0},
 {"_OnGroup", OnGroup, 1, 0},
 {"_OnHisFillter", OnHisFillter, 1, 0},
 {"_OnHisMerge", OnHisMerge, 1, 0},
 {"_OnHistList", OnHistList, 1, 0},
 {"_OnImportCsvFile", OnImportCsvFile, 1, 0},
 {"_OnIniLimits", OnIniLimits, 1, 0},
 {"_OnOpenCsvFile", OnOpenCsvFile, 1, 0},
 {"_OnRightsSelection", OnRightsSelection, 1, 0},
 {"_OnTimer", OnTimer, 1, 0},
 {"_OnVtype", OnVtype, 1, 0}
};
