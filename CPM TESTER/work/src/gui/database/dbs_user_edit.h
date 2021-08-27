#if !defined(__DBS_USER_EDIT_H__)
#define __DBS_USER_EDIT_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

SElExceptionPtr NewUserDialog(SDBSUser user);
SElExceptionPtr EditUserDialog(SDBSUser user); 
SElExceptionPtr DelUserDialog(SDBSUser user);

SElExceptionPtr NewUserRightsDialog(SDBSUser user);
SElExceptionPtr EditUserRightsDialog(SDBSUser user, int32_t value); 
SElExceptionPtr DelUserRightsDialog(SDBSUser user, int32_t value);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_USER_EDIT_H__)
