#if !defined(__DBS_TEST_PARAM_H__)
#define __DBS_TEST_PARAM_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

#define TEST_PARAM_RELOAD            0   
#define TEST_PARAM_SET_ACTIVE_NODE       1  
   
SElExceptionPtr TreeParametersIni (void); //(STestParameterPtr parameter, char* keyNode);
SElExceptionPtr TreeParametersSet(int32_t mode);
SElExceptionPtr TreeParametersClose(void);
/* bktree callback functions */
SElExceptionPtr BkTreeParametersNodeClick (char *keyNode);
SElExceptionPtr BkTreeParametersPopupMenuCommand (char *keyNode,char *keyCol,long  menuItem);
SElExceptionPtr BkTreeParametersModify(char *keyNode, char *keyCol, long modifyType);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_TEST_PARAM_H__)
