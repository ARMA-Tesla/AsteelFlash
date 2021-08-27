
#include "dbs_global.h"
#include "dbs_user.h"
#include <pwctrl.h>
#include <toolbox.h>

static SDBSUser			gs_user;

extern SGlobalPtr		global;  
extern int				gs_panel[]; 

static int CVICALLBACK OnExitNew (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK OnExitEdit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK OnExitRightsNew (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK OnExitRightsEdit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2);

static SElExceptionPtr GetRightsDbs(int32_t* rights);
static SElExceptionPtr GetRightsTester(int32_t* rights);
static SElExceptionPtr SetRightsDbs(int32_t rights, bool_t dimmed);
static SElExceptionPtr SetRightsTester(int32_t rights, bool_t dimmed);
static SElExceptionPtr FillTesterRing(int32_t tester_id, SDBSUserPtr puser); 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::iButtonReceivedCallback"
SElExceptionPtr _FUNCC iButtonReceivedCallback(struct _SIButton* me, const char* ibutton)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			error = 0;

	if( me->_CallBackFncDisabled)
		EXCCHECKCVI( SetCtrlVal (gs_panel[DLG_USER], DLG_USER_IBUTTON, ibutton)); 

Error:
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "NewUserDialog"
SElExceptionPtr NewUserDialog(SDBSUser user)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			error = 0;
	int32_t			top = 100, left = 100; 

	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_NAME, ""));
	EXCCHECKCVI( PasswordCtrl_SetAttribute (gs_panel[DLG_USER], DLG_USER_PASSW1, ATTR_PASSWORD_VAL, ""));
	EXCCHECKCVI( PasswordCtrl_SetAttribute (gs_panel[DLG_USER], DLG_USER_PASSW2, ATTR_PASSWORD_VAL, "")); 
	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_IBUTTON, ""));  
	
	EXCCHECK( SetRightsDbs(0, TRUE));
	EXCCHECK( SetRightsTester(0, FALSE));
	
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_EXIT_OK, ATTR_CALLBACK_FUNCTION_POINTER, OnExitNew));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_EXIT_CANCEL, ATTR_CALLBACK_FUNCTION_POINTER, OnExitNew));
	
	EXCCHECK( FillTesterRing(-1, NULL)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_RIGHTS, ATTR_DIMMED, FALSE));
	
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_NAME, ATTR_NO_EDIT_TEXT, FALSE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_NAME, ATTR_TEXT_BGCOLOR, VAL_WHITE));  
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW1, ATTR_NO_EDIT_TEXT, FALSE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW1, ATTR_TEXT_BGCOLOR, VAL_WHITE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW2, ATTR_NO_EDIT_TEXT, FALSE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW2, ATTR_TEXT_BGCOLOR, VAL_WHITE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_IBUTTON, ATTR_NO_EDIT_TEXT, FALSE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_IBUTTON, ATTR_TEXT_BGCOLOR, VAL_WHITE)); 
	
	EXCCHECKCVI( SetPanelPos (gs_panel[DLG_USER], top, left)); 
	EXCCHECKCVI( InstallPopup(gs_panel[DLG_USER]));  
	
	if(global->ibutton)
		EXCCHECK( global->ibutton->DisableCallbackFnc(global->ibutton, TRUE));

Error:
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EditUserDialog"
SElExceptionPtr EditUserDialog(SDBSUser user)
{
	SElExceptionPtr pexception = NULL;      
	int32_t			error = 0;
	int32_t			top = 100, left = 100;
	
	gs_user = user;

	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_NAME, user.name));
	EXCCHECKCVI( PasswordCtrl_SetAttribute (gs_panel[DLG_USER], DLG_USER_PASSW1, ATTR_PASSWORD_VAL, user.password));
	EXCCHECKCVI( PasswordCtrl_SetAttribute (gs_panel[DLG_USER], DLG_USER_PASSW2, ATTR_PASSWORD_VAL, user.password)); 
	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_IBUTTON, user.ibutton));
	
	EXCCHECK( SetRightsDbs(0, TRUE));
	EXCCHECK( SetRightsTester(0, TRUE));

	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_EXIT_OK, ATTR_CALLBACK_FUNCTION_POINTER, OnExitEdit));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_EXIT_CANCEL, ATTR_CALLBACK_FUNCTION_POINTER, OnExitEdit));
	
	EXCCHECKCVI( ClearListCtrl (gs_panel[DLG_USER], DLG_USER_RIGHTS));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_RIGHTS, ATTR_DIMMED, TRUE));  
		
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_NAME, ATTR_NO_EDIT_TEXT, TRUE));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_NAME, ATTR_TEXT_BGCOLOR, VAL_LT_GRAY)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW1, ATTR_NO_EDIT_TEXT, FALSE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW1, ATTR_TEXT_BGCOLOR, VAL_WHITE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW2, ATTR_NO_EDIT_TEXT, FALSE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW2, ATTR_TEXT_BGCOLOR, VAL_WHITE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_IBUTTON, ATTR_NO_EDIT_TEXT, FALSE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_IBUTTON, ATTR_TEXT_BGCOLOR, VAL_WHITE));
	
	EXCCHECKCVI( SetPanelPos (gs_panel[DLG_USER], top, left)); 
	EXCCHECKCVI( InstallPopup(gs_panel[DLG_USER]));  
	
	if(global->ibutton)
		EXCCHECK( global->ibutton->DisableCallbackFnc(global->ibutton, TRUE));

Error:
	EXCRETHROW( pexception);     
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "DelUserDialog"
SElExceptionPtr DelUserDialog(SDBSUser user)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	
	if(IDOK==MessageBox((HWND)hWnd, _("Delete current user?"), _("Delete User"), MB_OKCANCEL|MB_ICONQUESTION))
	{
		user.admin_id = global->activeUser.user_id;
		EXCCHECK( PUSER->UserDel(PUSER, user) ); 
		EXCCHECK( UserReload() ); 
	}
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnExitNew"
static int CVICALLBACK OnExitNew (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	char			passw[DBS_RECORD_LENGHT+1] = "";
	SDBSUser		user = {0}; 
	SDBSUserRights	rights = {0};
	int				index, tester_id;
	
	switch (event)
	{
	case EVENT_COMMIT:
		if(control == DLG_USER_EXIT_OK)
		{
			user.admin_id = global->activeUser.user_id; 
			
			EXCCHECKCVI( GetCtrlVal(gs_panel[DLG_USER], DLG_USER_NAME, user.name));
			CHECK_VALUE( user.name);
			
			EXCCHECKCVI( PasswordCtrl_GetAttribute (gs_panel[DLG_USER], DLG_USER_PASSW1, ATTR_PASSWORD_VAL, user.password)); 
			if( 0!=strlen(user.password) )
			{
				if( strlen(user.password)<5 )
				{
					EXCTHROW( -1, _("Minimum password lenght 5 letters"));
				}
					
				EXCCHECKCVI( PasswordCtrl_GetAttribute (gs_panel[DLG_USER], DLG_USER_PASSW2, ATTR_PASSWORD_VAL, passw));
				if( 0!=strcmp(passw, user.password) )
				{
					EXCTHROW( -1, _("Check password"));
				}
			}
			
			EXCCHECKCVI( GetCtrlVal(gs_panel[DLG_USER], DLG_USER_IBUTTON, user.ibutton)); 
			if( 0!=strlen(user.ibutton) && 16!=strlen(user.ibutton) )
			{
				EXCTHROW( -1, _("iButton lenght have to be 16 letters"));
			}
			
			EXCCHECKCVI( GetCtrlIndex (gs_panel[DLG_USER], DLG_USER_RIGHTS, &index));
			if(index==0)
			{
				EXCTHROW( -1, _("Rights were not selected!"));

			}
			EXCCHECKCVI( GetValueFromIndex (gs_panel[DLG_USER], DLG_USER_RIGHTS, index, &tester_id));
			
			rights.tester_id = tester_id;
			EXCCHECK( GetRightsDbs(&rights.rights_dbs));
			EXCCHECK( GetRightsTester(&rights.rights_tester));
			
			EXCCHECK( PUSER->UserNew(PUSER, user, rights) );
			EXCCHECK( UserReload() );
		}
		EXCCHECKCVI( RemovePopup(0)); 

		if(global->ibutton)
			EXCCHECK( global->ibutton->DisableCallbackFnc(global->ibutton, FALSE));

		break;
	}
		
Error:
	EXCDISPLAY( pexception);  
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnExitEdit"
static int CVICALLBACK OnExitEdit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	char			passw[DBS_RECORD_LENGHT+1] = "";
	SDBSUser		user = {0};  
	
	switch (event)
	{
	case EVENT_COMMIT:
		if(control == DLG_USER_EXIT_OK)
		{
			user.admin_id = global->activeUser.user_id; 
			user.user_id = gs_user.user_id;
			
			EXCCHECKCVI( GetCtrlVal(gs_panel[DLG_USER], DLG_USER_NAME, user.name));
			CHECK_VALUE( user.name);
			
			EXCCHECKCVI( PasswordCtrl_GetAttribute (gs_panel[DLG_USER], DLG_USER_PASSW1, ATTR_PASSWORD_VAL, user.password)); 
			if( 0!=strlen(user.password) )
			{
				if( strlen(user.password)<5 )
				{
					EXCTHROW( -1, _("Minimum password lenght 5 letters"));
				}
					
				EXCCHECKCVI( PasswordCtrl_GetAttribute (gs_panel[DLG_USER], DLG_USER_PASSW2, ATTR_PASSWORD_VAL, passw));
				if( 0!=strcmp(passw, user.password) )
				{
					EXCTHROW( -1, _("Check password"));
				}
			}
			
			EXCCHECKCVI( GetCtrlVal(gs_panel[DLG_USER], DLG_USER_IBUTTON, user.ibutton)); 
			if( 0!=strlen(user.ibutton) && 16!=strlen(user.ibutton) )
			{
				EXCTHROW( -1, _("iButton lenght have to be 16 letters"));
			}
			
			EXCCHECK( PUSER->UserEdit(PUSER, user) );
			EXCCHECK( UserReload() );  
		}
		EXCCHECKCVI( RemovePopup(0)); 

		if(global->ibutton)
			EXCCHECK( global->ibutton->DisableCallbackFnc(global->ibutton, FALSE));

		break;
	}
		
Error:
	EXCDISPLAY( pexception);  
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "NewUserRightsDialog"
SElExceptionPtr NewUserRightsDialog(SDBSUser user)
{
	SElExceptionPtr pexception = NULL;      
	int32_t			error = 0;
	int32_t			top = 100, left = 100; 
	
	gs_user = user;
	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_NAME, user.name));
	EXCCHECKCVI( PasswordCtrl_SetAttribute (gs_panel[DLG_USER], DLG_USER_PASSW1, ATTR_PASSWORD_VAL, user.password));
	EXCCHECKCVI( PasswordCtrl_SetAttribute (gs_panel[DLG_USER], DLG_USER_PASSW2, ATTR_PASSWORD_VAL, user.password)); 
	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_IBUTTON, user.ibutton));
	
	EXCCHECK( SetRightsDbs(0, TRUE));
	EXCCHECK( SetRightsTester(0, FALSE));
	
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_EXIT_OK, ATTR_CALLBACK_FUNCTION_POINTER, OnExitRightsNew));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_EXIT_CANCEL, ATTR_CALLBACK_FUNCTION_POINTER, OnExitRightsNew));
	
	EXCCHECK( FillTesterRing(-1, &user));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_RIGHTS, ATTR_DIMMED, FALSE));
	
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_NAME, ATTR_NO_EDIT_TEXT, TRUE));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_NAME, ATTR_TEXT_BGCOLOR, VAL_LT_GRAY)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW1, ATTR_NO_EDIT_TEXT, FALSE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW1, ATTR_TEXT_BGCOLOR, VAL_LT_GRAY)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW2, ATTR_NO_EDIT_TEXT, FALSE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW2, ATTR_TEXT_BGCOLOR, VAL_LT_GRAY)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_IBUTTON, ATTR_NO_EDIT_TEXT, FALSE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_IBUTTON, ATTR_TEXT_BGCOLOR, VAL_LT_GRAY));
	
	EXCCHECKCVI( SetPanelPos (gs_panel[DLG_USER], top, left)); 
	EXCCHECKCVI( InstallPopup(gs_panel[DLG_USER]));
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EditUserRightsDialog"
SElExceptionPtr EditUserRightsDialog(SDBSUser user, int32_t value)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	int32_t			top = 100, left = 100; 
	
	gs_user = user;  
	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_NAME, user.name));
	EXCCHECKCVI( PasswordCtrl_SetAttribute (gs_panel[DLG_USER], DLG_USER_PASSW1, ATTR_PASSWORD_VAL, user.password));
	EXCCHECKCVI( PasswordCtrl_SetAttribute (gs_panel[DLG_USER], DLG_USER_PASSW2, ATTR_PASSWORD_VAL, user.password)); 
	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_IBUTTON, user.ibutton));
	
	EXCCHECK( SetRightsDbs(user.Rights[value].rights_dbs, FALSE));
	EXCCHECK( SetRightsTester(user.Rights[value].rights_tester, FALSE));
	
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_EXIT_OK, ATTR_CALLBACK_FUNCTION_POINTER, OnExitRightsEdit));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_EXIT_CANCEL, ATTR_CALLBACK_FUNCTION_POINTER, OnExitRightsEdit));
	
	EXCCHECK( FillTesterRing(user.Rights[value].tester_id, &user));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_RIGHTS, ATTR_DIMMED, TRUE));
	
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_NAME, ATTR_NO_EDIT_TEXT, TRUE));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_NAME, ATTR_TEXT_BGCOLOR, VAL_LT_GRAY)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW1, ATTR_NO_EDIT_TEXT, FALSE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW1, ATTR_TEXT_BGCOLOR, VAL_LT_GRAY)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW2, ATTR_NO_EDIT_TEXT, FALSE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_PASSW2, ATTR_TEXT_BGCOLOR, VAL_LT_GRAY)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_IBUTTON, ATTR_NO_EDIT_TEXT, FALSE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_IBUTTON, ATTR_TEXT_BGCOLOR, VAL_LT_GRAY)); 
	
	EXCCHECKCVI( SetPanelPos (gs_panel[DLG_USER], top, left)); 
	EXCCHECKCVI( InstallPopup(gs_panel[DLG_USER]));
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "DelUserRightsDialog"
SElExceptionPtr DelUserRightsDialog(SDBSUser user, int32_t value)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	SDBSUserRights	rights = {0};

	if(IDOK==MessageBox((HWND)hWnd, _("Delete current user rights?"), _("Delete User Rights"), MB_OKCANCEL|MB_ICONQUESTION))
	{
		rights.tester_id= user.Rights[value].tester_id;
		rights.rights_tester = user.Rights[value].rights_tester;
		rights.rights_dbs = user.Rights[value].rights_dbs;
				
		rights.admin_id = global->activeUser.user_id; 
		
		EXCCHECK( user.UserRightsDel(&user, rights) );
		EXCCHECK( UserReload() ); 
	}
		
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnExitRightsNew"
static int CVICALLBACK OnExitRightsNew (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	char			passw[DBS_RECORD_LENGHT_NAME+1] = "";
	SDBSUser		user = gs_user;
	SDBSUserRights	rights = {0};
	int				index, tester_id;
	
	switch (event)
	{
		case EVENT_COMMIT:
			if(control == DLG_USER_EXIT_OK)
			{
				rights.admin_id = global->activeUser.user_id; 
							
				EXCCHECKCVI( GetCtrlIndex (gs_panel[DLG_USER], DLG_USER_RIGHTS, &index));
				if(index==0)
				{
					EXCTHROW( -1, _("Rights were not selected!"));
				}
				EXCCHECKCVI( GetValueFromIndex (gs_panel[DLG_USER], DLG_USER_RIGHTS, index, &tester_id));
				
				rights.tester_id = tester_id;
				EXCCHECK( GetRightsDbs(&rights.rights_dbs));
				EXCCHECK( GetRightsTester(&rights.rights_tester));
			
				EXCCHECK( user.UserRightsNew(&user, rights) );
				EXCCHECK( UserReload() );
			}
			EXCCHECKCVI( RemovePopup(0)); 
			break;
	}
		
Error:
	EXCDISPLAY( pexception); 
	return 0;
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnExitRightsEdit"
static int CVICALLBACK OnExitRightsEdit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	char			passw[DBS_RECORD_LENGHT_NAME+1] = "";
	SDBSUser		user = gs_user;
	SDBSUserRights	rights = {0};
	int				index, tester_id;  
	
	switch (event)
	{
		case EVENT_COMMIT:
			if(control == DLG_USER_EXIT_OK)
			{
				rights.admin_id = global->activeUser.user_id; 
								
				EXCCHECKCVI( GetCtrlIndex (gs_panel[DLG_USER], DLG_USER_RIGHTS, &index));
				EXCCHECKCVI( GetValueFromIndex (gs_panel[DLG_USER], DLG_USER_RIGHTS, index, &tester_id));
				
				rights.tester_id = tester_id; 
				EXCCHECK( GetRightsDbs(&rights.rights_dbs));
				EXCCHECK( GetRightsTester(&rights.rights_tester));
			
				EXCCHECK( user.UserRightsEdit(&user, rights) );
				EXCCHECK( UserReload() );
			}
			EXCCHECKCVI( RemovePopup(0));
			break;
	}
		
Error:
	EXCDISPLAY( pexception);  
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetRightsDbs"
static SElExceptionPtr SetRightsDbs(int32_t rights, bool_t dimmed)
{
	SElExceptionPtr pexception = NULL;       
	int32_t			error = 0;

	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_DRIGHTS_1, (rights&USER_RIGHTS_VISIBLE_DATA)>0)); 
	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_DRIGHTS_2, (rights&USER_RIGHTS_EDIT_PARAMETER)>0));  
	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_DRIGHTS_3, (rights&USER_RIGHTS_EDIT_TEST)>0));  
	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_DRIGHTS_4, (rights&USER_RIGHTS_EDIT_PRODUCT)>0));  
	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_DRIGHTS_5, (rights&USER_RIGHTS_EDIT_USER)>0));  

	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_DRIGHTS_1, ATTR_DIMMED, dimmed)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_DRIGHTS_2, ATTR_DIMMED, dimmed));   
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_DRIGHTS_3, ATTR_DIMMED, dimmed));   
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_DRIGHTS_4, ATTR_DIMMED, dimmed));   
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_DRIGHTS_5, ATTR_DIMMED, dimmed));   
	  
Error:
	EXCRETHROW( pexception); 
}	

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetRightsTester"
static SElExceptionPtr SetRightsTester(int32_t rights, bool_t dimmed)
{
	SElExceptionPtr pexception = NULL;       
	int32_t			error = 0;
	
	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_TRIGHTS_1, (rights&TESTER_USER_RIGHTS_TEAMLEADER)>0)); 
	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_TRIGHTS_2, (rights&TESTER_USER_RIGHTS_OPERATOR)>0)); 
	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_TRIGHTS_3, (rights&TESTER_USER_RIGHTS_QUALITY)>0)); 
	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_TRIGHTS_4, (rights&TESTER_USER_RIGHTS_MAINTENANCE)>0)); 
	EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_TRIGHTS_5, (rights&TESTER_USER_RIGHTS_EXPERTISE)>0)); 
	
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_TRIGHTS_1, ATTR_DIMMED, dimmed)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_TRIGHTS_2, ATTR_DIMMED, dimmed));    
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_TRIGHTS_3, ATTR_DIMMED, dimmed));    
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_TRIGHTS_4, ATTR_DIMMED, dimmed));    
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DLG_USER], DLG_USER_TRIGHTS_5, ATTR_DIMMED, dimmed));  
	
Error:
	EXCRETHROW( pexception); 
}	

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetRightsDbs"
static SElExceptionPtr GetRightsDbs(int32_t* rights)
{
	SElExceptionPtr pexception = NULL;       
	int32_t			error = 0;
	bool_t			value;
	int32_t			Rights = 0;
	
	EXCCHECKCVI( GetCtrlVal(gs_panel[DLG_USER], DLG_USER_DRIGHTS_1, &value));
	if(value)
		Rights |= USER_RIGHTS_VISIBLE_DATA;
	
	EXCCHECKCVI( GetCtrlVal(gs_panel[DLG_USER], DLG_USER_DRIGHTS_2, &value)); 
	if(value)
		Rights |= USER_RIGHTS_EDIT_PARAMETER;
	
	EXCCHECKCVI( GetCtrlVal(gs_panel[DLG_USER], DLG_USER_DRIGHTS_3, &value)); 
	if(value)
		Rights |= USER_RIGHTS_EDIT_TEST;
	
	EXCCHECKCVI( GetCtrlVal(gs_panel[DLG_USER], DLG_USER_DRIGHTS_4, &value)); 
	if(value)
		Rights |= USER_RIGHTS_EDIT_PRODUCT;
	
	EXCCHECKCVI( GetCtrlVal(gs_panel[DLG_USER], DLG_USER_DRIGHTS_5, &value)); 
	if(value)
		Rights |= USER_RIGHTS_EDIT_USER;
	
Error:
	if(rights) *rights = Rights;    
	EXCRETHROW( pexception); 
}	

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetRightsTester"
static SElExceptionPtr GetRightsTester(int32_t* rights)
{
	SElExceptionPtr pexception = NULL;       
	int32_t			error = 0;
	bool_t			value;
	int32_t			Rights = 0;
	
	EXCCHECKCVI( GetCtrlVal(gs_panel[DLG_USER], DLG_USER_TRIGHTS_1, &value));
	if(value)
		Rights |= TESTER_USER_RIGHTS_TEAMLEADER;
	
	EXCCHECKCVI( GetCtrlVal(gs_panel[DLG_USER], DLG_USER_TRIGHTS_2, &value));
	if(value)
		Rights |= TESTER_USER_RIGHTS_OPERATOR;
	
	EXCCHECKCVI( GetCtrlVal(gs_panel[DLG_USER], DLG_USER_TRIGHTS_3, &value));
	if(value)
		Rights |= TESTER_USER_RIGHTS_QUALITY;
	
	EXCCHECKCVI( GetCtrlVal(gs_panel[DLG_USER], DLG_USER_TRIGHTS_4, &value));
	if(value)
		Rights |= TESTER_USER_RIGHTS_MAINTENANCE;
	
	EXCCHECKCVI( GetCtrlVal(gs_panel[DLG_USER], DLG_USER_TRIGHTS_5, &value));
	if(value)
		Rights |= TESTER_USER_RIGHTS_EXPERTISE;
	
Error:
	if(rights) *rights = Rights; 
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "FillTesterRing"
static SElExceptionPtr FillTesterRing(int32_t tester_id, SDBSUserPtr puser)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	int				i, j, index = 0;
	STesterItemPtr	ptester = NULL;
  
	EXCCHECKCVI( ClearListCtrl (gs_panel[DLG_USER], DLG_USER_RIGHTS));
	EXCCHECKCVI( InsertListItem (gs_panel[DLG_USER], DLG_USER_RIGHTS, index++, "none", 0));
	
	if(tester_id<0)
	{
		CHECK_RIGHTS_ROOT2(
		/* root user */
		{
			EXCCHECKCVI( InsertListItem(gs_panel[DLG_USER], DLG_USER_RIGHTS, index++, "root", 0));

			for(i=0; i<PTESTER->TesterSize; i++)
			{
				if(puser)
				{
					bool_t 	exist = FALSE;
				
					for(j=0; j<puser->RightsSize; j++)
					{
						if( (puser->Rights[j].property&PROPERTY_VALID)>0
							&& puser->Rights[j].tester_id==PTESTER->Tester[i].tester_id )
						{
							exist = TRUE;
							break;
						}
					}
				
					if(!exist)
						EXCCHECKCVI( InsertListItem (gs_panel[DLG_USER], 
										DLG_USER_RIGHTS, 
										index++, 
										(PTESTER->Tester[i].description)? PTESTER->Tester[i].description : "noname", 
										PTESTER->Tester[i].tester_id));	
				}
				else
				{
					EXCCHECKCVI( InsertListItem (gs_panel[DLG_USER], 
									DLG_USER_RIGHTS, 
									index++, 
									(PTESTER->Tester[i].description)? PTESTER->Tester[i].description : "noname", 
									PTESTER->Tester[i].tester_id));			
				}
			}
		},
		/* not root user */
		{
			EXCCHECK( PTESTER->TesterGetSelected(PTESTER, global->activeUser.tester_id, &ptester) );

			if(ptester && ptester->tester_id>0)
			{
				EXCCHECKCVI( InsertListItem (gs_panel[DLG_USER], 
							DLG_USER_RIGHTS, 
							index, 
							(ptester && ptester->description)? ptester->description : "noname", 
							ptester->tester_id));
			}
		}
		);
	}
	else if(tester_id==0)
	{
		EXCCHECKCVI( InsertListItem (gs_panel[DLG_USER], 
						DLG_USER_RIGHTS, 
						index, 
						"root", 
						tester_id));	
		EXCCHECKCVI( SetCtrlIndex (gs_panel[DLG_USER], DLG_USER_RIGHTS, index));
	}
	else
	{
		EXCCHECK( PTESTER->TesterGetSelected(PTESTER, tester_id, &ptester) );
		
		if(ptester)
		{
			EXCCHECKCVI( InsertListItem (gs_panel[DLG_USER], 
							DLG_USER_RIGHTS, 
							index, 
							(ptester->description)? ptester->description : "noname", 
							tester_id));
			EXCCHECKCVI( SetCtrlIndex (gs_panel[DLG_USER], DLG_USER_RIGHTS, index));
		}
	}
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnRightsSelection"
int CVICALLBACK OnRightsSelection (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	int				value;
	
	switch (event)
		{
		case EVENT_COMMIT:

			EXCCHECKCVI( GetCtrlVal( panel, control, &value));
			
			switch( control)
			{
				case DLG_USER_TRIGHTS_1: /* teamleader */
					if(value)
					{
						EXCCHECK( SetRightsDbs(0, TRUE));
						EXCCHECK( SetRightsTester(TESTER_USER_RIGHTS_TEAMLEADER, FALSE));
						
					}
					break;
				case DLG_USER_TRIGHTS_2: /* operator */
					if(value)
					{
						EXCCHECK( SetRightsDbs(0, TRUE));
						EXCCHECK( SetRightsTester(TESTER_USER_RIGHTS_OPERATOR, FALSE));
						
					}
					break;
				case DLG_USER_TRIGHTS_4: /* maintenance */
					EXCCHECK( SetRightsDbs(0, !value));
				case DLG_USER_TRIGHTS_3: /* quality */
				case DLG_USER_TRIGHTS_5: /* expertise */  
					if(value)
					{
						EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_TRIGHTS_1, 0)); 
						EXCCHECKCVI( SetCtrlVal(gs_panel[DLG_USER], DLG_USER_TRIGHTS_2, 0));
					}
					break;
			}
			
			break;
		}
	
Error:
	EXCDISPLAY( pexception);
	return 0;
}

