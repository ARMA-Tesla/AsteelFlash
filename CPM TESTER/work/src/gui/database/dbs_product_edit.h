#if !defined(__DBS_PRODUCT_EDIT_H__)
#define __DBS_PRODUCT_EDIT_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

SElExceptionPtr NewProductDialog(SProductTypes   product);
SElExceptionPtr CopyProductDialog(SProductTypes  product);
SElExceptionPtr EditProductDialog(SProductTypes  product); 
SElExceptionPtr DelProductDialog(SProductTypes   product);    

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_PRODUCT_EDIT_H__)
