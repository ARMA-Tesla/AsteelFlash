#if !defined(__DBS_HISTORY_H__)
#define __DBS_HISTORY_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

SElExceptionPtr	TreeHistoryIni(void);
SElExceptionPtr	TreeHistoryClose(void); 
SElExceptionPtr HistoryClear(void);
	
SElExceptionPtr HistoryFilterSetCurrentTime(void);      

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_HISTORY_H__)
