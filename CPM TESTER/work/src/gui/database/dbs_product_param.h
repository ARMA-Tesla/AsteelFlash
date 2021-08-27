#if !defined(__DBS_PRODUCT_PARAM_H__)
#define __DBS_PRODUCT_PARAM_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

#define PRODUCT_PARAM_RELOAD               0   
#define PRODUCT_PARAM_SET_ACTIVE_NODE       1  
   
SElExceptionPtr TreeProductParamIni (void);
SElExceptionPtr TreeProductParamSet(int32_t mode);
SElExceptionPtr TreeProductParamClose(void);

/* bktree callback functions */
SElExceptionPtr BkTreeProductParamNodeClick (char *keyNode);
SElExceptionPtr BkTreeProductParamPopupMenuCommand (char *keyNode,char *keyCol, long  menuItem);
SElExceptionPtr BkTreeProductParamModify(char *keyNode, char *keyCol, long modifyType);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_PRODUCT_PARAM_H__)
