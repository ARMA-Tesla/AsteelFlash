#if !defined(__DBS_TEST_H__)
#define __DBS_TEST_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/* editable items parametrs */
#define TREE_EDIT_NO	0
#define TREE_EDIT_DEL	(1 << 0)
#define TREE_EDIT_SKIP	(1 << 1)

#define TREE_EDIT_PARAMETR			(1 << 0) 
#define TREE_EDIT_PARAMETR_INSERT	(1 << 1)
   
SElExceptionPtr TreeStepsIni(void);
SElExceptionPtr TreeStepsClose(void);

/* bktree callback functions */
SElExceptionPtr BkTreeStepsNodeClick (char *keyNode);
SElExceptionPtr BkTreeStepsPopupMenuCommand (char *keyNode,char *keyCol,long  menuItem);
SElExceptionPtr BkTreeStepsInsert(void);
SElExceptionPtr BkTreeStepMove(int32_t key_id, bool_t *activated);

SElExceptionPtr MoveTestMode(STestStep test, int32_t editable, const char* keyNode);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_TEST_H__)
