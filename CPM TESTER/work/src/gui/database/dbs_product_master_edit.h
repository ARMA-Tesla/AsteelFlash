#if !defined(__DBS_PRODUCT_MASTER_EDIT_H__)
#define __DBS_PRODUCT_MASTER_EDIT_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

SElExceptionPtr NewProductMasterDialog(         
   	SProductMaster   	master
); 
SElExceptionPtr DelProductMasterDialog(
   	SProductMaster   	master,
   	const char* 		keyNode,
	const char* 		keyCol
); 

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_PRODUCT_MASTER_EDIT_H__)
