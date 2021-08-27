#if !defined(__DBS_PRODUCT_PARAM_EDIT_H__)
#define __DBS_PRODUCT_PARAM_EDIT_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

SElExceptionPtr EditProductParamDialog(         
   	SProductParameter   parameter
); 
SElExceptionPtr DelProductParamDialog(
   	SProductParameter   parameter,
   	const char* 		keyNode,
	const char* 		keyCol
);    
SElExceptionPtr InsertProductParamDialog(
	SProductParameter 	parameter
);
	
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_PRODUCT_PARAM_EDIT_H__)
