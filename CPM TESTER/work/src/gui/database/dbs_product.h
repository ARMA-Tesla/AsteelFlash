#if !defined(__DBS_PRODUCT_H__)
#define __DBS_PRODUCT_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif
   
SElExceptionPtr TreeProductIni(void);
SElExceptionPtr TreeProductClose(void);

/* bktree callback functions */
SElExceptionPtr BkTreeProductNodeClick(char *keyNode);
SElExceptionPtr BkTreeProductPopupMenuCommand(char *keyNode, char *keyCol, long  menuItem);

SElExceptionPtr ChangeMode (int32_t mode);          


#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_PRODUCT_H__)
