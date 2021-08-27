#if !defined(__DBS_USER_H__)
#define __DBS_USER_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

SElExceptionPtr ActiveUserSet(SUserListPtr puserlist);   

SElExceptionPtr TreeUserIni(void);
SElExceptionPtr UserReload(void); 
SElExceptionPtr TreeUserClose(void);
/* bktree callback functions */
SElExceptionPtr BkTreeUserNodeClick(char *keyNode);
SElExceptionPtr BkTreeUserPopupMenuCommand(char *keyNode, char *keyCol, long  menuItem);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_USER_H__)
