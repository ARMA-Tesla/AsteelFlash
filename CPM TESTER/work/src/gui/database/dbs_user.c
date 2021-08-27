#include <bklib/bklib2.h>
#include "dbs_global.h"
#include <stdio.h>
#include "dbs_user.h"
#include "dbs_user_edit.h" 
#include "dbs_product.h"
#include "ring_control.h"

extern SGlobalPtr		global;
extern int				gs_panel[];

#define BKTREE_HEADER _("root|User ID^w100¦name|Name^w180¦passw|Password^w60¦ibutton|iButton^w60¦rights1|Dbs Rights^w400¦rights2|Tester Rights^w400\n")

#define BKTREE_MENU_ROOT_NEW_USER		0

#define BKTREE_MENU_USER_NEW_RIGHTS		0
#define BKTREE_MENU_USER_EDIT			1
#define BKTREE_MENU_USER_DEL			2     

#define BKTREE_MENU_RIGHTS_EDIT			0 
#define BKTREE_MENU_RIGHTS_DEL			1 

static SElExceptionPtr	UserList(void);
char* DbsRightsToString(int32_t rights);
char* TesterRightsToString(int32_t rights); 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TreeUserIni"
SElExceptionPtr TreeUserIni(void)
{
	int32_t    		error = 0;
	SElExceptionPtr pexception = NULL;
	HRESULT			hr; 
	int				imglistHandle;
	short int		AXvalue;

	global->editMode = EDIT_USER;   
   
   	EXCCHECKCVI( GetObjHandleFromActiveXCtrl (gs_panel[DBS], DBS_IMAGELIST4, &imglistHandle));
	EXCCHECKCOM( BKLib2__DBKTreeSetImageList (global->treeHandle, NULL, imglistHandle));
	EXCCHECKCOM( BKLib2__DBKTreeSetAutoImages (global->treeHandle, NULL, VFALSE));
	EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu (global->treeHandle, NULL, NULL, &AXvalue)); 
	
	EXCCHECK( PUSER->UserRead(PUSER) ); 
	EXCCHECK( UserList() );   
	
	EXCCHECK (RingGroupAttribute(RING_GROUPS_HIDE)); 

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TreeUserClose"
SElExceptionPtr TreeUserClose(void)
{
	SElExceptionPtr pexception = NULL;   
   	int32_t    		error = 0;
   	
/* Error: */							   
   EXCRETHROW( pexception);     
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "UserReload"
SElExceptionPtr UserReload(void)
{
   HRESULT			hr;
   SElExceptionPtr 	pexception = NULL;
   char*			keyNode = NULL;
   
   EXCCHECKCOM( BKLib2__DBKTreeGetSelectedItem (global->treeHandle, NULL, &keyNode));  
   EXCCHECK( PUSER->UserRead(PUSER) );
   EXCCHECK( UserList() );          
   EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (global->treeHandle, NULL, keyNode)); 
   
Error:
   if(keyNode) CA_FreeMemory(keyNode);
   EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "BkTreeUserNodeClick"
SElExceptionPtr BkTreeUserNodeClick (char *keyNode)
{
	SElExceptionPtr pexception = NULL;     
	HRESULT			hr;
	short int		AXvalue; 
	char* 			parentNode;

	if (0==strcmp(keyNode, "root") )
	{
		EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu (global->treeHandle, NULL, _("New User"), &AXvalue)); 
	}
	else
	{
		EXCCHECKCOM( BKLib2__DBKTreeGetParentNode (global->treeHandle, NULL, keyNode, &parentNode));
		
		if (0==strcmp(parentNode, "root") ) 
			EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu (global->treeHandle, NULL, _("New Rights|Edit User|Delete User"), &AXvalue)); 
		else
			EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu (global->treeHandle, NULL, _("Edit Rights|Delete Rights"), &AXvalue));
		
		CA_FreeMemory( parentNode );  
	}

Error:
	EXCRETHROW( pexception); 	
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "BkTreeUserPopupMenuCommand"
SElExceptionPtr BkTreeUserPopupMenuCommand (char *keyNode,char *keyCol,long  menuItem)
{
	SElExceptionPtr pexception = NULL;   
	HRESULT			hr;
	char*			parentNode = NULL;
	SDBSUser		user = {0};
	long			index;
	
	if (0==strcmp(keyNode, "root") )
	{
		switch(menuItem)
		{
			case BKTREE_MENU_ROOT_NEW_USER:
				CHECK_RIGHTS_USER_NEW( EXCCHECK( NewUserDialog(user) ) );
				break;
		}
	}
	else
	{
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeData (global->treeHandle, NULL, keyNode, &index));
		if(index)
		{
			index -= 1;
			user = PUSER->User[index];
		}
		
		EXCCHECKCOM( BKLib2__DBKTreeGetParentNode (global->treeHandle, NULL, keyNode, &parentNode));
		
		if (0==strcmp(parentNode, "root") ) 
		{
			switch(menuItem)
			{
				case BKTREE_MENU_USER_NEW_RIGHTS:
					CHECK_RIGHTS_USER_NEW( EXCCHECK( NewUserRightsDialog(user) ) );
					break;
				case BKTREE_MENU_USER_EDIT:
					CHECK_RIGHTS_USER_EDIT( user, EditUserDialog(user) );
					break;
				case BKTREE_MENU_USER_DEL:
					CHECK_RIGHTS_USER_EDIT( user, DelUserDialog(user) );   
					break;
			}
		}
		else
		{
			int32_t 	value1, value2;
			
			sscanf(keyNode, "%d_%d", &value1, &value2); 
			
			switch(menuItem)
			{
				case BKTREE_MENU_RIGHTS_EDIT:
					CHECK_RIGHTS_USER_EDIT_RIGHTS( user.Rights[value2].tester_id, 
												   EXCCHECK( EditUserRightsDialog(user, value2)) );
					break;
				case BKTREE_MENU_RIGHTS_DEL:
					CHECK_RIGHTS_USER_EDIT_RIGHTS( user.Rights[value2].tester_id,
												   EXCCHECK( DelUserRightsDialog(user, value2)) );
					break;
			}
		}
	}
 
 Error:
	if(parentNode) CA_FreeMemory(parentNode);
	EXCRETHROW( pexception); 	
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ViewUser"
static SElExceptionPtr ViewUser(void)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	char*			rights = NULL;
	int32_t			tester_id;

	EXCCHECKCVI( SetCtrlVal( gs_panel[DBS], DBS_USERNAME, global->activeUser.name));
	
	rights = DbsRightsToString(global->activeUser.rights_dbs);
	tester_id = global->activeUser.tester_id;
	
	if(tester_id)
	{
		STesterItemPtr		ptester = NULL;

		EXCCHECK( PTESTER->TesterGetSelected(PTESTER, tester_id, &ptester) );
		if(ptester)
			EXCCHECKCVI( SetCtrlVal(gs_panel[DBS], DBS_USERPRODUCT, ptester->description)); 
	}
	else
	{
		EXCCHECKCVI( SetCtrlVal(gs_panel[DBS], DBS_USERPRODUCT, "root")); 
	}

	EXCCHECKCVI( ResetTextBox (gs_panel[DBS], DBS_USERRIGHTS, ""));
	
	if(rights!=NULL)
		EXCCHECKCVI( SetCtrlVal( gs_panel[DBS], DBS_USERRIGHTS, rights));  
	
Error:
	if(rights) free(rights);
	EXCRETHROW( pexception); 	  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ActiveUserSet"
SElExceptionPtr ActiveUserSet(SUserListPtr puserlist)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	SUserPtr		puser = NULL;
	
	if(puserlist)
	{
		EXCCHECK( puserlist->GetLoggedUser(puserlist, &puser));
		
		if(puser)
		{
			global->activeUser.user_id = puser->_user_id;		
			global->activeUser.tester_id = puser->_tester_id;
			global->activeUser.rights_tester = puser->_rights_tester;
			global->activeUser.rights_dbs = puser->_rights_dbs;
			strcpy(global->activeUser.name, puser->_name);
		}
		else
		{
			EXCCHECK( PUSER->UserGetAnonymous(PUSER, &global->activeUser)); 
		}
	}
	else
	{
		EXCCHECK( PUSER->UserGetAnonymous(PUSER, &global->activeUser));  
	}
	
	EXCCHECK( ViewUser() ); /* show user info (name, tester, rights) */
	
Error:
	EXCRETHROW(pexception);          
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "UserList"
static SElExceptionPtr	UserList(void)
{
	int32_t  		error = 0;
	SElExceptionPtr pexception = NULL; 
	HRESULT			hr;
	SELStringPtr 	pelstring = global->pelstring;
	STesterItemPtr	ptester = NULL;
	short int		AXvalue;
	int				i, j;
	char*			rights_tester = NULL;
	char*			rights_dbs = NULL;
	
	EXCCHECK( pelstring->Empty(pelstring) );    
	EXCCHECK( pelstring->Append(pelstring, BKTREE_HEADER) );
	
	EXCCHECK( pelstring->Append(pelstring, "|root|Users^i0\n") );		   
	
	for(i=0; i<PUSER->UserSize; i++)
	{
		if( (PUSER->User[i].property&PROPERTY_VALID)>0 )
		{
			EXCCHECK( pelstring->Format(pelstring, "root|%d|%d^i1^d%d¦%s¦%s¦%s\n",
									PUSER->User[i].user_id,
									PUSER->User[i].user_id,
									(long)(i+1),
									PUSER->User[i].name,
									(0==strlen(PUSER->User[i].password))? "NO":"YES",
									(0==strlen(PUSER->User[i].ibutton))? "NO":"YES"
									));
		
			for(j=0; j<PUSER->User[i].RightsSize; j++)    
			{
				if( (PUSER->User[i].Rights[j].property&PROPERTY_VALID)>0 )
				{
					char	tester_name[255];

					if(PUSER->User[i].Rights[j].tester_id==0)
					{
						sprintf(tester_name, "root");
					}
					else
					{
						EXCCHECK( PTESTER->TesterGetSelected(PTESTER, PUSER->User[i].Rights[j].tester_id, &ptester) );
						sprintf(tester_name, "%s", (ptester && ptester->description)? ptester->description : "unknown" );
					}
					
					rights_dbs = DbsRightsToString(PUSER->User[i].Rights[j].rights_dbs);
					rights_tester = TesterRightsToString(PUSER->User[i].Rights[j].rights_tester);
					
					EXCCHECK( pelstring->Format(pelstring, "%d|%d_%d|^i2^d%d¦Product: %s¦¦¦%s¦%s\n",
										PUSER->User[i].user_id,
										PUSER->User[i].user_id,
										j,
										(long)(i+1),
										tester_name,
										rights_dbs,   
										rights_tester  
										) );
					
					free(rights_dbs);
					free(rights_tester);
				}
			}
		}
	}
	
	EXCCHECKCOM( BKLib2__DBKTreeFromString (global->treeHandle, NULL, pelstring->GetPtr(pelstring), "¦", "\n", &AXvalue));
	EXCCHECKCOM( BKLib2__DBKTreeSetEnableDragAndDrop (global->treeHandle, NULL, VFALSE));
	EXCCHECKCOM( BKLib2__DBKTreeExpandLevel (global->treeHandle, NULL, 2, &AXvalue));
	
	EXCCHECK( pelstring->Empty(pelstring) );    
	
Error:
	EXCRETHROW( pexception); 	   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "DbsRightsToString"
char* DbsRightsToString(int32_t rights)
{
	int32_t		errror = 0;
	char*		srights = NULL;
	char*		pdata = NULL;
	
	srights = calloc(256, sizeof(char));
	
	if((rights&USER_RIGHTS_VISIBLE_DATA)>0)  
	{
		strcpy(srights, _("Edit "));									  
		
		if((rights&USER_RIGHTS_EDIT_USER)>0)  
			strcat(srights, _("users, "));
		if((rights&USER_RIGHTS_EDIT_PRODUCT)>0)  						   
			strcat(srights, _("products, "));
		if((rights&USER_RIGHTS_EDIT_TEST)>0)  						   
			strcat(srights, _("tests, "));
		if((rights&USER_RIGHTS_EDIT_PARAMETER)>0)
			strcat(srights, _("parameters, ")); 
		
		if( 0==strcmp(srights, _("Edit ")) ) /* only USER_RIGHTS_VISIBLE_DATA */
		{
			strcpy(srights, _("View data")); 
		}
		else
		{
			pdata = srights+strlen(srights)-2; 
			strcpy(pdata, "");	
		}
	}
	else
	{
		strcpy(srights, _("None"));    
	}
	
/* Error: */
	return srights;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TesterRightsToString"
char* TesterRightsToString(int32_t rights)
{
	int32_t		errror = 0;
	char*		srights = NULL;
	char*		pdata = NULL;
	
	srights = calloc(256, sizeof(char));
	
	if(rights!=0)
	{
		if((rights&TESTER_USER_RIGHTS_TEAMLEADER)>0)  
			strcat(srights, _("Team Leader, "));
		if((rights&TESTER_USER_RIGHTS_OPERATOR)>0)  
			strcat(srights, _("Operator, "));
		if((rights&TESTER_USER_RIGHTS_QUALITY)>0)  
			strcat(srights, _("Quality, "));
		if((rights&TESTER_USER_RIGHTS_MAINTENANCE)>0)  
			strcat(srights, _("Maintenance, "));
		if((rights&TESTER_USER_RIGHTS_EXPERTISE)>0)  
			strcat(srights, _("Expertise, "));
		if((rights&TESTER_USER_RIGHTS_GO_MODE)>0)  
			strcat(srights, _("Go Mode, "));
		
		pdata = srights+strlen(srights)-2; 
		strcpy(pdata, "");	
	}
	else
	{
		strcpy(srights, _("None"));    
	}
	
/* Error: */
	return srights;
}


