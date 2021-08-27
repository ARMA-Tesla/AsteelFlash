#include <dbs/dbs_userlist.h>  
#include "user.h"

#define USER_NAME		"@user"
#define OPERATOR_LOG	"operator_id:%d"

static SElExceptionPtr	user_new( SUserPtr* pUser);	
static SElExceptionPtr	user_delete( SUserPtr* pUser);

static bool_t GetTesterRights(struct _SUser* me, const char* rights_id);    

static SElExceptionPtr	LoginUser(
	struct _SUserList* me,
	const char* userResource
);
static bool_t IsUserLogged(struct _SUserList* me);  
static bool_t IsOperatorLogged(struct _SUserList* me, int32_t user_id);
static SElExceptionPtr LogoutUser(struct _SUserList* me); 
static SElExceptionPtr GetLoggedUser(struct _SUserList* me, SUserPtr* puser); 

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__  "user_new"
static SElExceptionPtr	user_new( SUserPtr* pUser)
{
	SElExceptionPtr    pexception = NULL;
	
	*pUser = (SUserPtr)calloc( 1, sizeof(SUser));
	EXCCHECKALLOC( *pUser);

	(*pUser)->GetTesterRights = GetTesterRights;
	
Error:
	if ( pexception )
	{
		user_delete( pUser);
	}
	EXCRETHROW( pexception); 
} /* user_new */


/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__  "user_delete"
static SElExceptionPtr	user_delete( SUserPtr* pUser)
{
	if ( pUser && *pUser )
	{
		free(*pUser);
		*pUser = NULL;
	}
	
/* Error: */
	return NULL;
} /* user_delete */

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__  "GetTesterRights"
static bool_t GetTesterRights(struct _SUser* me, const char* rights_id)
{
	
	if(0==strcmp(RIGHTS_QUALITY, rights_id))
	{
		if((me->_rights_tester&TESTER_USER_RIGHTS_QUALITY)>0)
			return TRUE;
	}
	else if(0==strcmp(RIGHTS_MAINTENANCE, rights_id)) 
	{
		if((me->_rights_tester&TESTER_USER_RIGHTS_MAINTENANCE)>0)
			return TRUE;
	}
	else if(0==strcmp(RIGHTS_EXPERTISE, rights_id)) 
	{
		if((me->_rights_tester&TESTER_USER_RIGHTS_EXPERTISE)>0)
			return TRUE;
	}	
	else if(0==strcmp(RIGHTS_OPERATOR, rights_id)) 
	{
		if((me->_rights_tester&TESTER_USER_RIGHTS_OPERATOR)>0)
			return TRUE;
	}	
	else if(0==strcmp(RIGHTS_TEAMLEADER, rights_id)) 
	{
		if((me->_rights_tester&TESTER_USER_RIGHTS_TEAMLEADER)>0)
			return TRUE;
	}	
	
	return FALSE;
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__  "userlist_new"
SElExceptionPtr	userlist_new( SUserListPtr* pUserList)
{
	SElExceptionPtr    pexception = NULL;

	*pUserList = (SUserListPtr)calloc( 1, sizeof(SUserList));
	EXCCHECKALLOC( *pUserList);
	
	(*pUserList)->LoginUser = LoginUser;
	(*pUserList)->LogoutUser = LogoutUser;
	(*pUserList)->IsUserLogged = IsUserLogged;
	(*pUserList)->IsOperatorLogged = IsOperatorLogged;
	(*pUserList)->GetLoggedUser = GetLoggedUser;
	
	EXCCHECK( objlist_new(&(*pUserList)->_pList));
	EXCCHECK( ((*pUserList)->_pList)->RegisterDestroy(
			   (*pUserList)->_pList,
			   user_delete));

	InitializeCriticalSection( &(*pUserList)->_Lock);

Error:
	if ( pexception )
	{
		userlist_delete( pUserList);
	}
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__  "userlist_delete"
SElExceptionPtr	userlist_delete( SUserListPtr* pUserList)
{
	SElExceptionPtr    pexception = NULL;

	if ( pUserList && *pUserList )
	{
		EXCCHECK( (*pUserList)->_pList->Clear((*pUserList)->_pList)); 
		
		EXCCHECK( objlist_delete(&(*pUserList)->_pList));
		(*pUserList)->_pList = NULL;

		DeleteCriticalSection(&((*pUserList)->_Lock));  

		free(*pUserList);
		*pUserList = NULL;
	}
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Lock"
static SElExceptionPtr Lock(struct _SUserList* me)
{
	EnterCriticalSection( &me->_Lock);    
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Unlock"
static SElExceptionPtr Unlock(struct _SUserList* me)
{
	LeaveCriticalSection( &me->_Lock);   
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__  "LoginUser"
static SElExceptionPtr	LoginUser(
	struct _SUserList* me,
	const char* userResource
)
{
	SElExceptionPtr pexception = NULL;
	SUserPtr 		puser = NULL;
	SUserPtr 		puser_logged = NULL;
	char			resource[1024];
	int				param_count = 0;
	char*			pdata;
	
	Lock(me);

	EXCCHECK( user_new(&puser));
	EXCCHECKALLOC( puser);
	
	strcpy(resource, userResource);
	
	/* user_id;name;tester_id;rights_tester;rights_dbs */
	pdata = strtok( resource, ";" );
	while(pdata!=NULL)
	{
	   	if(param_count==0)
		{
			puser->_user_id = strtol(pdata, NULL, 10);	
		}
		else if(param_count==1)
		{
			strncpy(puser->_name, pdata, USER_MAX_FULL_NAME);	
		}
		else if(param_count==2)
		{
			puser->_tester_id = strtol(pdata, NULL, 10);	
		}
		else if(param_count==3)
		{
			puser->_rights_tester = strtol(pdata, NULL, 10);	
		}
		else if(param_count==4)
		{
			puser->_rights_dbs = strtol(pdata, NULL, 10);	
		}
	
		param_count++;
		pdata = strtok( NULL, ";" );   
	}
	
	if( param_count==5 
		&& puser->_user_id
		&& puser->_name[0]!='\0') /* check validity of the resource string */
	{	
		/* check user */
		if( puser->GetTesterRights(puser, RIGHTS_QUALITY)
			|| puser->GetTesterRights(puser, RIGHTS_MAINTENANCE) 
			|| puser->GetTesterRights(puser, RIGHTS_EXPERTISE) )
		{
			if(me->IsUserLogged(me))
			{
				puser_logged = (SUserPtr)me->_pList->Get(me->_pList, USER_NAME);

				if( puser_logged && puser_logged->_user_id==puser->_user_id)
				{
					EXCCHECK( me->_pList->Remove(me->_pList, USER_NAME));
					if(me->_UserLogout) 
						EXCCHECK(me->_UserLogout());
					EXCCHECK( user_delete(&puser)); 
				}
				else
				{
					EXCCHECK( me->_pList->Remove(me->_pList, USER_NAME));
					EXCCHECK( me->_pList->Add(me->_pList, USER_NAME, puser));						
					if(me->_UserLogin) 
						EXCCHECK(me->_UserLogin());
				}	
			}
			else
			{
				EXCCHECK( me->_pList->Add(me->_pList, USER_NAME, puser));						
				if(me->_UserLogin) 
					EXCCHECK(me->_UserLogin());
			}
		}
		else if( puser->GetTesterRights(puser, RIGHTS_TEAMLEADER)
				 || puser->GetTesterRights(puser, RIGHTS_OPERATOR)) 
		{
			char operator_id[32];

			sprintf(operator_id, OPERATOR_LOG, puser->_user_id);

			if(me->IsOperatorLogged(me, puser->_user_id))
			{
				EXCCHECK( me->_pList->Remove(me->_pList, operator_id));
				if(me->_OperatorLogout) 
					EXCCHECK(me->_OperatorLogout(puser->_user_id));
				EXCCHECK( user_delete(&puser)); 
			}
			else
			{
				EXCCHECK( me->_pList->Add(me->_pList, operator_id, puser));
				if(me->_OperatorLogin) 
					EXCCHECK(me->_OperatorLogin(puser->_user_id));
			}
		}
		else
		{
			EXCCHECK( user_delete(&puser)); 
		}
	}

Error:
	Unlock(me);
	EXCRETHROW( pexception); 
} /* LoginUser */

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__  "LogoutUser"
static SElExceptionPtr LogoutUser(struct _SUserList* me)
{
	SElExceptionPtr pexception = NULL;
	bool_t			is_logged = FALSE;
	SUserPtr		puser = NULL;
	int32_t			user_id = 0;

	Lock(me);

	is_logged = me->IsUserLogged(me);
	
	if(is_logged)
	{
		/* logout user */
		EXCCHECK( me->GetLoggedUser(me, &puser));
		user_id = puser->_user_id;

		EXCCHECK( me->_pList->Remove(me->_pList, USER_NAME));	

		if(me->_UserLogout) EXCCHECK(me->_UserLogout());
	}
	
Error:
	Unlock(me);
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__  "GetLoggedUser"
static SElExceptionPtr GetLoggedUser(struct _SUserList* me, SUserPtr* puser)
{
	SElExceptionPtr pexception = NULL;
	
	Lock(me);

	if(puser) *puser = NULL;
	
	if( me->IsUserLogged(me))
	{
		 if(puser) *puser = (SUserPtr)(me->_pList->Get(me->_pList, USER_NAME));	
	}
	
Error:
	Unlock(me);
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__  "IsUserLogged"
static bool_t IsUserLogged(struct _SUserList* me)  
{
	bool_t is_logged = FALSE;

	is_logged = ( NULL != (SUserPtr)(me->_pList->Get(me->_pList, USER_NAME)))? TRUE : FALSE;

	return is_logged; 
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__  "IsOperatorLogged"
static bool_t IsOperatorLogged(struct _SUserList* me, int32_t user_id)
{
	bool_t is_logged = FALSE;

	if(user_id)
	{
		char buffer[32];

		sprintf(buffer, OPERATOR_LOG, user_id);
		is_logged = ( NULL != (SUserPtr)(me->_pList->Get(me->_pList, buffer)))? TRUE : FALSE;
	}

	return is_logged; 
}

