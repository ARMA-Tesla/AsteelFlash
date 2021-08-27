#if !defined(__DBS_TEST_PARAM_EDIT_H__)
#define __DBS_TEST_PARAM_EDIT_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

SElExceptionPtr EditParameterDialog2(         
   STestParameter   	parameter
); 
SElExceptionPtr DelParameterDialog2(
   STestParameter   	parameter,
   const char* 			keyNode,
   const char* 			keyCol
);  
SElExceptionPtr InsertParameterDialog2(
   STestParameter		parameter
);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_TEST_PARAM_EDIT_H__)
