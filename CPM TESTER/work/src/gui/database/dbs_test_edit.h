#if !defined(__DBS_TEST_EDIT_H__)
#define __DBS_TEST_EDIT_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

SElExceptionPtr InsertTestDialog(STestStep test, char* name);
SElExceptionPtr EditTestDialog(STestStep test, char* name);
SElExceptionPtr SkipTestDialog(STestStep test, int32_t editable);
SElExceptionPtr DelTestDialog(STestStep test, int32_t editable);
SElExceptionPtr RemoveSkipTestDialog(STestStep test, int32_t editable);

SElExceptionPtr NewParameterDialog(STestStep test, char* name);    
SElExceptionPtr EditParameterDialog(STestParameter parameter, char* name, int32_t editable);
SElExceptionPtr DelParameterDialog(STestParameter parameter, int32_t editable);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_TEST_EDIT_H__)
