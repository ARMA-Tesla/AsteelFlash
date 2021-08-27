#include "dbs.h"
#include "dbs_userlist.h"
#include <stdio.h>
#include <odbc/elodbc.h>
#include "dbs_define_local.h"

#define LOCK_STR		"user"

static SElExceptionPtr user_ReadUser(SDBSUserListPtr me);	

static SElExceptionPtr user_UserNew(SDBSUserListPtr me, SDBSUser user, SDBSUserRights rights);
static SElExceptionPtr user_UserEdit(SDBSUserListPtr me, SDBSUser user);  
static SElExceptionPtr user_UserDel(SDBSUserListPtr me, SDBSUser user);  

static SElExceptionPtr user_UserGetFromId(SDBSUserListPtr me, int32_t user_id, SDBSUserPtr* user);

static SElExceptionPtr user_UserGetFromIbutton(SDBSUserListPtr me, char* ibutton, SEUserPtr user); 
static SElExceptionPtr user_UserGetFromIdent(SDBSUserListPtr me, int32_t user_id, SEUserPtr user); 
static SElExceptionPtr user_UserGetAnonymous(SDBSUserListPtr me, SEUserPtr user); 
static SElExceptionPtr user_UserGetTesterList(SDBSUserListPtr me, int32_t tester_id, SEUserPtr* user, int32_t* userSize);
static SElExceptionPtr user_UserGetList(SDBSUserListPtr me, SEUserPtr* user, int32_t* userSize);

static SElExceptionPtr userrights_UserRightsNew(SDBSUserPtr me, SDBSUserRights rights);
static SElExceptionPtr userrights_UserRightsEdit(SDBSUserPtr me, SDBSUserRights rights);  
static SElExceptionPtr userrights_UserRightsDel(SDBSUserPtr me, SDBSUserRights rights);
static SElExceptionPtr userrights_UserRightsSetActive(SDBSUserPtr me, int32_t tester_id);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsrights_new"
SElExceptionPtr dbsrights_new(SDBSUserRightsPtr* pDBSUserRightsPtr, int32_t count, void* pDBS)
{
	SElExceptionPtr    	pexception = NULL;
	SDBSUserRightsPtr	me = NULL;

	me = calloc(count ,sizeof(SDBSUserRights));
	EXCCHECKALLOC( me );

	*pDBSUserRightsPtr = me;
	
	me->pdbs = pDBS;

Error:
	EXCRETHROW( pexception); 
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsrights_delete"
SElExceptionPtr dbsrights_delete(SDBSUserRightsPtr* pDBSUserRightsPtr)
{	
	if (pDBSUserRightsPtr && *pDBSUserRightsPtr)
	{
		free(*pDBSUserRightsPtr);
		*pDBSUserRightsPtr = NULL;
	}

/* Error: */
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsuser_new"
SElExceptionPtr dbsuser_new(SDBSUserPtr* pDBSUserPtr, int32_t count, void* pDBS)
{
	SElExceptionPtr	pexception = NULL; 
	SDBSUserPtr		me = NULL;
	int				i;

	me = calloc(count ,sizeof(SDBSUser));
	EXCCHECKALLOC(me);
	
	*pDBSUserPtr = me;

	for(i=0; i<count; i++)
	{
		me[i].UserRightsDel		= userrights_UserRightsDel;
		me[i].UserRightsNew		= userrights_UserRightsNew;
		me[i].UserRightsEdit	= userrights_UserRightsEdit;
		me[i].UserRightsSetActive = userrights_UserRightsSetActive;
		me[i].pdbs = pDBS; 
	}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsuser_delete"
SElExceptionPtr dbsuser_delete(SDBSUserPtr* pDBSUserPtr)
{	
	if (pDBSUserPtr && *pDBSUserPtr)
	{
		free(*pDBSUserPtr);
		*pDBSUserPtr = NULL;
	}

/* Error: */
 	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsuserlist_new"
SElExceptionPtr dbsuserlist_new(SDBSUserListPtr* pDBSUserListPtr, void* pDBS)
{
	SElExceptionPtr	pexception = NULL;
	SDBSUserListPtr	me = NULL;

	me = calloc(1,sizeof(SDBSUserList));
	EXCCHECKALLOC( me );
	
	*pDBSUserListPtr = me;

	me->UserRead			= user_ReadUser;
	me->UserNew 			= user_UserNew;
	me->UserEdit 			= user_UserEdit;  
	me->UserDel 			= user_UserDel;  
	me->UserGetFromIbutton  = user_UserGetFromIbutton;
	me->UserGetAnonymous 	= user_UserGetAnonymous;
	me->UserGetFromId 		= user_UserGetFromId;
	me->UserGetFromIdent 	= user_UserGetFromIdent; 
	me->UserGetTesterList	= user_UserGetTesterList;  
	me->UserGetList			= user_UserGetList;

	me->pdbs = pDBS;

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsuserlist_delete"
SElExceptionPtr dbsuserlist_delete(SDBSUserListPtr* pDBSUserListPtr)
{
	SElExceptionPtr	pexception = NULL;  
	int				i;
	
	if(pDBSUserListPtr && *pDBSUserListPtr)
	{
		SDBSUserListPtr	me = *pDBSUserListPtr;

		for(i=0; i<me->UserSize; i++)
			dbsrights_delete(&me->User[i].Rights);
			
		EXCCHECK( dbsuser_delete( &me->User));

		free(*pDBSUserListPtr);
		*pDBSUserListPtr = NULL;
	}

Error:
 	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
/* USER SECTION **************************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "user_ReadUser"
static SElExceptionPtr user_ReadUser(SDBSUserListPtr me)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;  
	int32_t			count, count2, index, i, j;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;

	LOCK; 

	/* lock database tester.users */
	PDBS_LOCK(LOCK_STR);

	/* clear old data */
	if(me->User)
	{
		for(i=0; i<me->UserSize; i++)
			dbsrights_delete(&me->User[i].Rights);

		dbsuser_delete( &me->User);
	}
	
	/* select items */
	sprintf(sql, "SELECT u.user_id,u.name,ENCODE(up.password,'elcom'),up.ibutton, up.time, up.admin_id, up.property "
				 "FROM tester.user u INNER JOIN tester.user_parameters up "
				 "ON u.user_id = up.user_id "
				 "ORDER BY user_id" );
	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->GetRows(podbc, &count));

	/* memory allocation */
	EXCCHECK( dbsuser_new(&me->User, count, me->pdbs));
	me->UserSize = count;

	CHECKERR( podbc->Fetch( podbc, TRUE));
	
	for(index=0;index<count;index++)
	{
		me->User[index].user_id = *((short*)podbc->GetFieldValuePtr(podbc, 1));
		strncpy(me->User[index].name, (char*)podbc->GetFieldValuePtr(podbc, 2), DBS_RECORD_LENGHT_NAME);
		strncpy(me->User[index].password, (char*)podbc->GetFieldValuePtr(podbc, 3), DBS_RECORD_LENGHT);
		strncpy(me->User[index].ibutton, (char*)podbc->GetFieldValuePtr(podbc, 4), DBS_RECORD_LENGHT_IBUTTON);
		strncpy(me->User[index].time, (char*)podbc->GetFieldValuePtr(podbc, 5), DBS_RECORD_LENGHT_TIME);
		me->User[index].admin_id 	= *((short*)podbc->GetFieldValuePtr(podbc, 6));
		me->User[index].property	= *((short*)podbc->GetFieldValuePtr(podbc, 7));

		CHECKERR( podbc->MoveNext(podbc));
	}
	
	EXCCHECK( podbc->Cancel(podbc));
	
	/* product_id rights for users */
	for(index=0;index<count;index++)
	{	
		/* select items */ 
		sprintf(sql, "SELECT * "
					 "FROM tester.user_rights "
					 "WHERE user_id=%d", 
					 me->User[index].user_id);
		EXCCHECK( podbc->ExecSQL( podbc, sql));
		EXCCHECK( podbc->GetRows(podbc, &count2));

		/* memory allocation */
		EXCCHECK( dbsrights_new(&me->User[index].Rights, count2, me->pdbs));
		me->User[index].RightsSize = count2;

		CHECKERR( podbc->Fetch( podbc, TRUE));
		
		for(j=0;j<count2;j++)
		{
			me->User[index].Rights[j].tester_id = *((short*)podbc->GetFieldValuePtr(podbc, 2)); 
			me->User[index].Rights[j].rights_tester = *((short*)podbc->GetFieldValuePtr(podbc, 3)); 
			me->User[index].Rights[j].rights_dbs = *((short*)podbc->GetFieldValuePtr(podbc, 4));  
			strncpy(me->User[index].Rights[j].time, (char*)podbc->GetFieldValuePtr(podbc, 5), DBS_RECORD_LENGHT_TIME);
			me->User[index].Rights[j].admin_id 	= *((short*)podbc->GetFieldValuePtr(podbc, 6));
			me->User[index].Rights[j].property	= *((short*)podbc->GetFieldValuePtr(podbc, 7));
			
			CHECKERR( podbc->MoveNext(podbc));   
		}
		
		EXCCHECK( podbc->Cancel(podbc));
	}

Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "user_UserNew"
static SElExceptionPtr user_UserNew(SDBSUserListPtr me, SDBSUser user, SDBSUserRights rights)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL; 
	SOdbcPtr		podbc = PDBS->podbc;     
	char			sql[SQL_COMMAND_LENGHT] = "";  
	int				last_user_id;

	if(0==user.admin_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
	
	LOCK;

	/* lock database tester.users */
	PDBS_LOCK(LOCK_STR);

	sprintf(sql, "SELECT MAX(user_id) FROM tester.user");
	EXCCHECK( podbc->ExecSQL( podbc, sql));
	CHECKERR( podbc->Fetch( podbc, TRUE));
	last_user_id = *((short*)podbc->GetFieldValuePtr(podbc, 1));
	EXCCHECK( podbc->Cancel(podbc));
	
	user.user_id = last_user_id + 1;
	
	sprintf(sql, "INSERT INTO tester.user "  
				 "VALUES (%d,_UTF8'%s')", 					
				 user.user_id,
				 user.name);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql)); 
	EXCCHECK( podbc->Cancel(podbc));
	
	user.property = PROPERTY_VALID|PROPERTY_CREATED;
	DATETIME_TO_STRING(user.time); 
	
	sprintf(sql, "INSERT INTO tester.user_parameters "  
				 "VALUES (%d,DECODE('%s','elcom'),'%s','%s',%d,%d)", 						
				 user.user_id,
				 user.password,
				 user.ibutton,
				 user.time,
				 user.admin_id,
				 user.property);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql)); 
	EXCCHECK( podbc->Cancel(podbc));
	
	sprintf(sql, "INSERT INTO tester.user_rights "  
				 "VALUES (%d,%d,%d,%d,'%s',%d,%d)", 
				 user.user_id,
				 rights.tester_id,
				 rights.rights_tester,
				 rights.rights_dbs,
				 user.time,
				 user.admin_id,
				 user.property);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql)); 
	EXCCHECK( podbc->Cancel(podbc));
			
Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "user_UserEdit"
static SElExceptionPtr user_UserEdit(SDBSUserListPtr me, SDBSUser user)
{
	SElExceptionPtr	pexception = NULL; 
	SOdbcPtr		podbc = PDBS->podbc;     
	char			sql[SQL_COMMAND_LENGHT] = "";    
	
	if(0==user.admin_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
	
	LOCK;

	/* lock database tester.users */
	PDBS_LOCK(LOCK_STR);

	sprintf(sql, "UPDATE tester.user_parameters "
				 "SET property=(property^%d) "
				 "WHERE user_id=%d AND property&%d>0", 
				 PROPERTY_VALID,
				 user.user_id,
				 PROPERTY_VALID);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql)); 
	EXCCHECK( podbc->Cancel(podbc));

	/* set record */ 
	user.property = PROPERTY_VALID|PROPERTY_EDITED;  
	DATETIME_TO_STRING(user.time);
	
	sprintf(sql, "INSERT INTO tester.user_parameters "
				 "VALUES (%d,DECODE('%s','elcom'),'%s','%s',%d,%d)", 
				 user.user_id,
				 user.password,
				 user.ibutton,
				 user.time,
				 user.admin_id,
				 user.property);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql)); 
	EXCCHECK( podbc->Cancel(podbc));
	
Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception);   
}															 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "user_UserDel"
static SElExceptionPtr user_UserDel(SDBSUserListPtr me, SDBSUser user)
{
	SElExceptionPtr	pexception = NULL; 
	SOdbcPtr		podbc = PDBS->podbc;     
	char			sql[SQL_COMMAND_LENGHT] = "";    

	if(0==user.admin_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
	
	LOCK;

	/* lock database tester.users */
	PDBS_LOCK(LOCK_STR);

	sprintf(sql, "UPDATE tester.user_parameters "
				 "SET property=(property^%d) "
				 "WHERE user_id=%d AND property&%d>0", 
				 PROPERTY_VALID,
				 user.user_id,
				 PROPERTY_VALID );
	
	EXCCHECK( podbc->ExecSQL( podbc, sql)); 
	EXCCHECK( podbc->Cancel(podbc));
	
	/* set record */ 
	user.property = PROPERTY_DELETED;  
	DATETIME_TO_STRING(user.time);
	
	sprintf(sql, "INSERT INTO tester.user_parameters "
				 "VALUES (%d,DECODE('%s','elcom'),'%s','%s',%d,%d)", 
				 user.user_id,
				 user.password,
				 user.ibutton,
				 user.time,
				 user.admin_id,
				 user.property);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql)); 
	EXCCHECK( podbc->Cancel(podbc));	

	sprintf(sql, "UPDATE tester.user_rights "
				 "SET property=(property^%d) "
				 "WHERE user_id=%d AND property&%d>0", 
				 PROPERTY_VALID,
				 user.user_id,
				 PROPERTY_VALID);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql)); 
	EXCCHECK( podbc->Cancel(podbc));
	
Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "userrights_UserRightsNew"
static SElExceptionPtr userrights_UserRightsNew(SDBSUserPtr me, SDBSUserRights rights)
{
	SElExceptionPtr	pexception = NULL; 
	SOdbcPtr		podbc = PDBS->podbc;     
	char			sql[SQL_COMMAND_LENGHT] = "";      
	
	if(0==rights.admin_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
	
	LOCK;

	/* lock database tester.users */
	PDBS_LOCK(LOCK_STR);

	/* set record */ 
	rights.property = PROPERTY_VALID|PROPERTY_CREATED;  
	DATETIME_TO_STRING(rights.time);
	
	sprintf(sql, "INSERT INTO tester.user_rights "  
				 "VALUES (%d,%d,%d,%d,'%s',%d,%d)", 
				 me->user_id,
				 rights.tester_id,
				 rights.rights_tester,
				 rights.rights_dbs,
				 rights.time,
				 rights.admin_id,
				 rights.property);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql)); 
	EXCCHECK( podbc->Cancel(podbc));
	
Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "userrights_UserRightsEdit"
static SElExceptionPtr userrights_UserRightsEdit(SDBSUserPtr me, SDBSUserRights rights)
{
	SElExceptionPtr    pexception = NULL; 
	SOdbcPtr	podbc = PDBS->podbc;     
	char		sql[SQL_COMMAND_LENGHT] = ""; 
	
	if(0==rights.admin_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
	
	LOCK;

	/* lock database tester.users */
	PDBS_LOCK(LOCK_STR);

	sprintf(sql, "UPDATE tester.user_rights "
				 "SET property=(property^%d) "
				 "WHERE user_id=%d AND tester_id=%d AND property&%d>0", 
				 PROPERTY_VALID,
				 me->user_id,
				 rights.tester_id,
				 PROPERTY_VALID);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql)); 
	EXCCHECK( podbc->Cancel(podbc));
	
	/* set record */ 
	rights.property = PROPERTY_VALID|PROPERTY_EDITED;
	DATETIME_TO_STRING(rights.time); 
	
	sprintf(sql, "INSERT INTO tester.user_rights "  
				 "VALUES (%d,%d,%d,%d,'%s',%d,%d)", 
				 me->user_id,
				 rights.tester_id,
				 rights.rights_tester,
				 rights.rights_dbs,
				 rights.time,
				 rights.admin_id,
				 rights.property);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql)); 
	EXCCHECK( podbc->Cancel(podbc));
	
Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "userrights_UserRightsDel"
static SElExceptionPtr userrights_UserRightsDel(SDBSUserPtr me, SDBSUserRights rights)
{
	SElExceptionPtr    pexception = NULL; 
	SOdbcPtr	podbc = PDBS->podbc;     
	char		sql[SQL_COMMAND_LENGHT] = "";  
	
	if(0==rights.admin_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
	
	LOCK;

	/* lock database tester.users */
	PDBS_LOCK(LOCK_STR);

	sprintf(sql, "UPDATE tester.user_rights "
				 "SET property=(property^%d) "
				 "WHERE user_id=%d AND tester_id=%d AND property&%d>0", 
				 PROPERTY_VALID,
				 me->user_id,
				 rights.tester_id,
				 PROPERTY_VALID);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql)); 
	EXCCHECK( podbc->Cancel(podbc));
	
	/* set record */ 
	rights.property = PROPERTY_DELETED;
	DATETIME_TO_STRING(rights.time); 
	
	sprintf(sql, "INSERT INTO tester.user_rights "  
				 "VALUES (%d,%d,%d,%d,'%s',%d,%d)", 
				 me->user_id,
				 rights.tester_id,
				 rights.rights_tester,
				 rights.rights_dbs,
				 rights.time,
				 rights.admin_id,
				 rights.property);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql)); 
	EXCCHECK( podbc->Cancel(podbc));	
	
Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "user_UserGetFromId"
static SElExceptionPtr user_UserGetFromId(SDBSUserListPtr me, int32_t user_id, SDBSUserPtr* user)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
	
	for(i=0; i<me->UserSize; i++)	   
	{
		if( (me->User[i].property&PROPERTY_VALID)>0
			&& me->User[i].user_id == user_id )
		{
			if(user) *user = &me->User[i];
			break;
		}
	}
	
	if(i==me->UserSize)
	{
		for(i=0; i<me->UserSize; i++)	   
		{
			if( (me->User[i].property&PROPERTY_DELETED)>0
				&& me->User[i].user_id == user_id )
			{
				if(user) *user = &me->User[i];
				break;
			}
		}
	}
	
	/* not found */
	if( i==me->UserSize )
		if(user) *user = NULL;
	
/* Error: */
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "userrights_UserRightsSetActive"
static SElExceptionPtr userrights_UserRightsSetActive(SDBSUserPtr me, int32_t tester_id)
{
	SElExceptionPtr	pexception = NULL;    
	int				i;
	
	for(i=0; i<me->RightsSize; i++)
	{
		if( (me->Rights[i].property&PROPERTY_VALID)>0
			&& me->Rights[i].tester_id == tester_id )	
		{
			me->rights_active = i;
			break;
		}
	}

	if(i==me->RightsSize)
		me->rights_active = -1;
	
/* Error: */
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "user_UserGetTesterList"
static SElExceptionPtr user_UserGetTesterList(SDBSUserListPtr me, int32_t tester_id, SEUserPtr* user, int32_t* userSize)
{
	SElExceptionPtr	pexception = NULL;    
	int				i, j, count=0, index=0;
	SEUserPtr		testuser = NULL;
	
	for(i=0; i<me->UserSize; i++)
	{
		if( (me->User[i].property&PROPERTY_VALID)>0
			&& 0!=strlen(me->User[i].ibutton) )	
		{
			for(j=0; j<me->User[i].RightsSize; j++)
			{
			 	if( (me->User[i].Rights[j].property&PROPERTY_VALID)>0
					&& me->User[i].Rights[j].tester_id == tester_id ) //tester
				{
					count++;
					break;
				}
			}
		}
	}
	
	if(count==0)
	{
		if(user) *user = NULL;
		if(userSize) *userSize = 0;
		goto Error;
	}
	
	testuser = calloc(count, sizeof(SEUser));
	
	for(i=0; i<me->UserSize; i++)
	{
		if( (me->User[i].property&PROPERTY_VALID)>0
			&& 0!=strlen(me->User[i].ibutton) )	
		{
			for(j=0; j<me->User[i].RightsSize; j++)
			{
			 	if( (me->User[i].Rights[j].property&PROPERTY_VALID)>0 
					&& me->User[i].Rights[j].tester_id == tester_id ) // tester
				{
					testuser[index].user_id = me->User[i].user_id;
					strncpy(testuser[index].name, me->User[i].name, DBS_RECORD_LENGHT_NAME);
					strncpy(testuser[index].password, me->User[i].password, DBS_RECORD_LENGHT);
					strncpy(testuser[index].ibutton, me->User[i].ibutton, DBS_RECORD_LENGHT_IBUTTON);
					testuser[index].rights_tester = me->User[i].Rights[j].rights_tester;
					testuser[index].rights_dbs = me->User[i].Rights[j].rights_dbs;
					testuser[index].tester_id = me->User[i].Rights[j].tester_id; 
					index++;
					break;	
				}
			}
		}
	}
	
	if(user) *user = testuser;
	if(userSize) *userSize = index;
		
Error:
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "user_UserGetList"
static SElExceptionPtr user_UserGetList(SDBSUserListPtr me, SEUserPtr* user, int32_t* userSize)
{
	SElExceptionPtr	pexception = NULL;    
	int				i, j, count=0, index=0;
	SEUserPtr		testuser = NULL;
	
	for(i=0; i<me->UserSize; i++)
	{
		if( (me->User[i].property&PROPERTY_VALID)>0)	
		{
			for(j=0; j<me->User[i].RightsSize; j++)
			{
			 	if( (me->User[i].Rights[j].property&PROPERTY_VALID)>0
					&& me->User[i].Rights[j].tester_id == PDBS_TESTER_ID ) //tester
					   
				{
					count++;
					break;
				}
			}
			if(j==me->User[i].RightsSize)
			{
				for(j=0; j<me->User[i].RightsSize; j++)
				{
				 	if( (me->User[i].Rights[j].property&PROPERTY_VALID)>0
						&& me->User[i].Rights[j].tester_id == 0 ) //root
					   
					{
						count++;
						break;
					}
				}
			}
		}
	}
	
	if(count==0)
	{
		if(user) *user = NULL;
		if(userSize) *userSize = 0;
		goto Error;
	}
	
	testuser = calloc(count, sizeof(SEUser));
	
	for(i=0; i<me->UserSize; i++)
	{
		if( (me->User[i].property&PROPERTY_VALID)>0)	
		{
			for(j=0; j<me->User[i].RightsSize; j++)
			{
			 	if( (me->User[i].Rights[j].property&PROPERTY_VALID)>0 
					&& me->User[i].Rights[j].tester_id == PDBS_TESTER_ID ) // tester
				{
					testuser[index].user_id = me->User[i].user_id;
					strncpy(testuser[index].name, me->User[i].name, DBS_RECORD_LENGHT_NAME);
					strncpy(testuser[index].password, me->User[i].password, DBS_RECORD_LENGHT);
					strncpy(testuser[index].ibutton, me->User[i].ibutton, DBS_RECORD_LENGHT_IBUTTON);
					testuser[index].rights_tester = me->User[i].Rights[j].rights_tester;
					testuser[index].rights_dbs = me->User[i].Rights[j].rights_dbs;
					testuser[index].tester_id = me->User[i].Rights[j].tester_id; 
					index++;
					break;	
				}
			}
			if(j==me->User[i].RightsSize)
			{
				for(j=0; j<me->User[i].RightsSize; j++)
				{
				 	if( (me->User[i].Rights[j].property&PROPERTY_VALID)>0
						&& me->User[i].Rights[j].tester_id == 0 ) //root
					   
					{
						testuser[index].user_id = me->User[i].user_id;
						strncpy(testuser[index].name, me->User[i].name, DBS_RECORD_LENGHT_NAME);
						strncpy(testuser[index].password, me->User[i].password, DBS_RECORD_LENGHT);
						strncpy(testuser[index].ibutton, me->User[i].ibutton, DBS_RECORD_LENGHT_IBUTTON);
						testuser[index].rights_tester = me->User[i].Rights[j].rights_tester;
						testuser[index].rights_dbs = me->User[i].Rights[j].rights_dbs;
						testuser[index].tester_id = me->User[i].Rights[j].tester_id; 
						index++;
						break;
					}
				}
			}
		}
	}
	
	if(user) *user = testuser;
	if(userSize) *userSize = index;
		
Error:
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "user_UserGetFromIbutton"
static SElExceptionPtr user_UserGetFromIbutton(SDBSUserListPtr me, char* ibutton, SEUserPtr user)  
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
	SDBSUserPtr		puser = NULL;
	
	memset(user, 0, sizeof(SEUser));
	
	for(i=0; i<me->UserSize; i++)		 
	{
		if( (me->User[i].property&PROPERTY_VALID)>0
			&& 0==strcmp(ibutton, me->User[i].ibutton) )
		{
			puser = &me->User[i];   
			
			EXCCHECK( puser->UserRightsSetActive(puser, PDBS_TESTER_ID) );  
			
			if(puser->rights_active!=-1)
			{
				user->user_id = puser->user_id;
				strncpy( user->name, puser->name, DBS_RECORD_LENGHT_NAME);
				strncpy( user->password, puser->password, DBS_RECORD_LENGHT); 
				strncpy( user->ibutton, puser->ibutton, DBS_RECORD_LENGHT_IBUTTON);
				user->rights_tester = puser->Rights[puser->rights_active].rights_tester;
				user->rights_dbs = puser->Rights[puser->rights_active].rights_dbs; 
				user->tester_id = puser->Rights[puser->rights_active].tester_id;   
			}
			break;
		}
	}
	
Error:
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "user_UserGetFromIdent"
static SElExceptionPtr user_UserGetFromIdent(SDBSUserListPtr me, int32_t user_id, SEUserPtr user)
{
	SElExceptionPtr	pexception = NULL;
	int				i;
	SDBSUserPtr		puser = NULL;
	
	memset(user, 0, sizeof(SEUser));
	
	for(i=0; i<me->UserSize; i++)		 
	{
		if( (me->User[i].property&PROPERTY_VALID)>0
			&& me->User[i].user_id == user_id )
		{
			puser = &me->User[i];   
			
			EXCCHECK( puser->UserRightsSetActive(puser, PDBS_TESTER_ID) );  
			
			if(puser->rights_active!=-1)
			{
				user->user_id = puser->user_id;
				strncpy( user->name, puser->name, DBS_RECORD_LENGHT_NAME);
				strncpy( user->password, puser->password, DBS_RECORD_LENGHT); 
				strncpy( user->ibutton, puser->ibutton, DBS_RECORD_LENGHT_IBUTTON);
				user->rights_tester = puser->Rights[puser->rights_active].rights_tester;
				user->rights_dbs = puser->Rights[puser->rights_active].rights_dbs; 
				user->tester_id = puser->Rights[puser->rights_active].tester_id;   
			}
			break;
		}
	}
			
Error:
	EXCRETHROW( pexception);   
}	

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "user_UserGetAnonymous"
static SElExceptionPtr user_UserGetAnonymous(SDBSUserListPtr me, SEUserPtr user)
{
	SElExceptionPtr	pexception = NULL; 
		
	user->user_id = 1;
	strcpy( user->name, "Anonymous");
	strcpy( user->password, ""); 
	strcpy( user->ibutton, "");
	user->rights_tester = TETSER_USER_RIGHTS_NONE;
	user->rights_dbs = USER_RIGHTS_VISIBLE_DATA; 
	user->tester_id = PDBS_TESTER_ID;   
			
/* Error: */
	EXCRETHROW( pexception);   
}	


