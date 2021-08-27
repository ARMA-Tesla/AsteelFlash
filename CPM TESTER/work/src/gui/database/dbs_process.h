#if !defined(__DBS_PROCESS_H__)
#define __DBS_PROCESS_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif
   
#include <exception/Exception.h>

SElExceptionPtr TreeProcessIni(void);
SElExceptionPtr TreeProcessClose(void);

/* bktree callback functions */
SElExceptionPtr BkTreeProcessNodeClick (char *keyNode);
SElExceptionPtr BkTreeProcessPopupMenuCommand (char *keyNode, char *keyCol, long menuItem);
SElExceptionPtr BkTreeProcessModify (char *keyNode, char *keyCol, long  modifyType);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_PROCESS_H__)
