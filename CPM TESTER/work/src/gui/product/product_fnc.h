#ifndef _PRODUCT_FNC_H_
#define _PRODUCT_FNC_H_

#include <dbs/dbs.h>
#include <exception/exception.h>

SElExceptionPtr SetProductPath(SDBSProductListPtr pProductList, int32_t product_id);
SElExceptionPtr SetProductPicture(SDBSProductListPtr pProductList, int32_t product_id);
SElExceptionPtr SetProductList(SDBSProductListPtr pProductList, int32_t product_id);

SElExceptionPtr ChildExist(SDBSProductListPtr pProductList, int32_t product_id, bool_t* child, int32_t *parent_id);

SElExceptionPtr SetProductLevelBackward(SDBSProductListPtr pProductList, int32_t product_id);
SElExceptionPtr SetProductLevelForward(SDBSProductListPtr pProductList, int32_t product_id);

SElExceptionPtr GetPartDescription(SDBSProductListPtr pProductList, int32_t product_id, char* PartDescription, char* ProductPartNumber);
SElExceptionPtr CleanPartDescription(SDBSProductListPtr pProductList);

SElExceptionPtr LoadConfiguration(const char* fileName);

#endif