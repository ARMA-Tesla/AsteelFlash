#if !defined(__EL_USER_H__)
#define __EL_USER_H__

#include <windows.h>
#include <exception/Exception.h>
#include <objlist/objlist.h> 

/* tester rights_id */
#define RIGHTS_OPERATOR		"RIGHTS_OPERATOR"
#define RIGHTS_TEAMLEADER	"RIGHTS_TEAMLEADER"
#define RIGHTS_QUALITY		"RIGHTS_QUALITY"
#define RIGHTS_MAINTENANCE	"RIGHTS_MAINTENANCE"
#define RIGHTS_EXPERTISE	"RIGHTS_EXPERTISE"

#define USER_MAX_FULL_NAME			46

typedef struct _SUser
{
	/***************************************************************************
     * Public Members
     **************************************************************************/      
	bool_t (*GetTesterRights)(struct _SUser* me, const char* rights_id);
	
	/**************************************************************************
     * Private Members
     **************************************************************************/
	char		_name[USER_MAX_FULL_NAME+1]; 
	int32_t 	_user_id;    
	int32_t		_tester_id;    
	int32_t		_rights_tester;
	int32_t     _rights_dbs; 
	
} SUser, *SUserPtr;
	
typedef struct _SUserList
{
	bool_t (*IsUserLogged)(struct _SUserList* me);
	bool_t (*IsOperatorLogged)(struct _SUserList* me, int32_t user_id);

	/* Resource: user_id;name;tester_id;rights_tester;rights_dbs */
	SElExceptionPtr	(*LoginUser)(struct _SUserList* me, const char* userResource);

	/* Logout user with rights quality, maintenance, expertise */
	SElExceptionPtr	(*LogoutUser)(struct _SUserList* me); 

	/* Get user with rights quality, maintenance, expertise */
	SElExceptionPtr (*GetLoggedUser)(struct _SUserList* me, SUserPtr* puser);
	
	SObjListPtr		_pList;

	/* callbacks functions */
	SElExceptionPtr	(*_UserLogin)(void);
	SElExceptionPtr	(*_UserLogout)(void);
	SElExceptionPtr	(*_OperatorLogin)(int32_t user_id);
	SElExceptionPtr	(*_OperatorLogout)(int32_t user_id);

	CRITICAL_SECTION   _Lock;

} SUserList, *SUserListPtr;

SElExceptionPtr	userlist_new( SUserListPtr* pUser);	
SElExceptionPtr	userlist_delete( SUserListPtr* pUser);

#endif /* __EL_USER_H__ */

