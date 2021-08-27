#if !defined(__DBS_USERLIST_H__)
#define __DBS_USERLIST_H__

#include "dbs_define.h"
#include <exception/exception.h> 

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

#define TETSER_USER_RIGHTS_NONE			0	
#define TESTER_USER_RIGHTS_TEAMLEADER	(1<<0)
#define TESTER_USER_RIGHTS_OPERATOR		(1<<1)
#define TESTER_USER_RIGHTS_QUALITY		(1<<2)
#define TESTER_USER_RIGHTS_MAINTENANCE	(1<<3)
#define TESTER_USER_RIGHTS_EXPERTISE	(1<<4)
#define TESTER_USER_RIGHTS_GO_MODE		(1<<5)

#define USER_RIGHTS_NONE				0
#define USER_RIGHTS_VISIBLE_DATA        (1<<0)
#define USER_RIGHTS_EDIT_PARAMETER      (1<<1) 
#define USER_RIGHTS_EDIT_TEST           (1<<2)   
#define USER_RIGHTS_EDIT_PRODUCT        (1<<3)      
#define USER_RIGHTS_EDIT_USER           (1<<4)

typedef struct _SDBSUserRights
{
	void*		pdbs;

	int32_t		user_id;
	int32_t     tester_id;
	int32_t     rights_tester; 
	int32_t     rights_dbs;      
	char        time[DBS_RECORD_LENGHT_TIME+1];
	int32_t     admin_id; 
	int32_t     property;  
   
} SDBSUserRights, *SDBSUserRightsPtr;   /* Database struct. USER_RIGHTS.DBO */

SElExceptionPtr dbsrights_new(SDBSUserRightsPtr* pDBSUserRightsPtr, int32_t count, void* pDBS);
SElExceptionPtr dbsrights_delete(SDBSUserRightsPtr* pDBSUserRightsPtr);

typedef struct _SDBSUser
{
   void*				pdbs;
   SDBSUserRightsPtr	Rights;

   int32_t      RightsSize;

   int32_t      rights_active;

   int32_t      user_id;
   char			name[DBS_RECORD_LENGHT_NAME+1]; 
   char			password[DBS_RECORD_LENGHT+1];
   char			ibutton[DBS_RECORD_LENGHT_IBUTTON+1];    
   char			time[DBS_RECORD_LENGHT_TIME+1];
   int32_t		admin_id; 
   int32_t		property;  

   /* Users Edit Fnc */
   SElExceptionPtr (*UserRightsNew)(struct _SDBSUser* me, SDBSUserRights rights);
   SElExceptionPtr (*UserRightsEdit)(struct _SDBSUser* me, SDBSUserRights rights);  
   SElExceptionPtr (*UserRightsDel)(struct _SDBSUser* me, SDBSUserRights rights); 
   SElExceptionPtr (*UserRightsSetActive)(struct _SDBSUser* me, int32_t tester_id);

} SDBSUser, *SDBSUserPtr;               /* Database struct. USER.DBO and USER_PARAMETER.DBO */

SElExceptionPtr dbsuser_new(SDBSUserPtr* pDBSUserPtr, int32_t count, void* pDBS);
SElExceptionPtr dbsuser_delete(SDBSUserPtr* pDBSUserPtr);

/* export users for tester */ 
typedef   struct _SEUser
{
	int32_t		tester_id;
	int32_t 	user_id;   
	char		name[DBS_RECORD_LENGHT_NAME+1];
	char		password[DBS_RECORD_LENGHT+1]; 
	char		ibutton[DBS_RECORD_LENGHT_IBUTTON+1];
	int32_t		rights_tester;
	int32_t     rights_dbs; 
} SEUser, *SEUserPtr;

typedef struct _SDBSUserList           
{
   void*         pdbs;
   SDBSUserPtr   User;         

   int32_t   UserSize; 

   /* Database Connections Fnc */
   SElExceptionPtr (*UserRead)(struct _SDBSUserList* me); 

   /* Users Edit Fnc */
   SElExceptionPtr (*UserNew)(struct _SDBSUserList* me, SDBSUser user, SDBSUserRights rights);
   SElExceptionPtr (*UserEdit)(struct _SDBSUserList* me, SDBSUser user);  
   SElExceptionPtr (*UserDel)(struct _SDBSUserList* me, SDBSUser user);  

   SElExceptionPtr (*UserGetFromId)(struct _SDBSUserList* me, int32_t user_id, SDBSUserPtr* user);  
   
   /* export fnc */
   SElExceptionPtr (*UserGetTesterList)(struct _SDBSUserList* me, int32_t tester_id, SEUserPtr* user, int32_t* userSize);
   SElExceptionPtr (*UserGetList)(struct _SDBSUserList* me, SEUserPtr* user, int32_t* userSize);
   SElExceptionPtr (*UserGetFromIbutton)(struct _SDBSUserList* me, char* ibutton, SEUserPtr user);
   SElExceptionPtr (*UserGetFromIdent)(struct _SDBSUserList* me, int32_t user_id, SEUserPtr user); 
   SElExceptionPtr (*UserGetAnonymous)(struct _SDBSUserList* me, SEUserPtr user);  
   
} SDBSUserList, *SDBSUserListPtr;

SElExceptionPtr dbsuserlist_new(SDBSUserListPtr* pDBSUserListPtr, void* pDBS);
SElExceptionPtr dbsuserlist_delete(SDBSUserListPtr* pDBSUserListPtr);

/* DBS_USER_ERROR */
#define DBS_USER_ERROR                    -1000000

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_USERLIST_H__)
