#include <bklib/BKLib2.h>
#include <utility.h>
#include <toolbox.h>
#include "dbs_global.h"
#include "dbs_product_param.h"							   
  
extern SGlobalPtr		global;  
extern int				gs_panel[]; 

static int32_t			gs_product_id;

static int CVICALLBACK OnExitNew (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2);

static SElExceptionPtr FillMasterId(int32_t product_id);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "NewProductDialog"
SElExceptionPtr NewProductMasterDialog(SProductMaster master)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL; 
	int32_t			top = 100, left = 30;     
	
	if( 0!=strcmp(master.master, "@newmaster") )
	{
		MessageBox((HWND)hWnd, _("Select empty \"Master Id\" cell for new master panel!"), _("Warning"), MB_OK|MB_ICONWARNING);
	}
	else
	{
		EXCCHECKCVI( SetCtrlVal(gs_panel[MASTER], MASTER_MASTER_CODE, ""));  
	
		gs_product_id = master.product_id;
		
		EXCCHECK( FillMasterId(master.product_id));
	
		EXCCHECKCVI( SetCtrlAttribute (gs_panel[MASTER], MASTER_EXIT_OK, ATTR_CALLBACK_FUNCTION_POINTER, OnExitNew));
		EXCCHECKCVI( SetCtrlAttribute (gs_panel[MASTER], MASTER_EXIT_CANCEL, ATTR_CALLBACK_FUNCTION_POINTER, OnExitNew));
		
		EXCCHECKCVI( SetPanelPos (gs_panel[MASTER], top, left));
		EXCCHECKCVI( InstallPopup(gs_panel[MASTER]));
	}
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "NewProductDialog"
SElExceptionPtr DelProductMasterDialog(	
		SProductMaster	master,
		const char* keyNode,
		const char* keyCol
)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	
	/* check editable parameter */
	if( 0 == strcmp(master.master, "@none")
		|| 0 == strcmp(master.master, "@new")
		|| 0 == strcmp(master.master, "@newmaster") 
		|| 0 == strcmp(master.master, "@empty") )
	{	
		MessageBox((HWND)hWnd, _("Not editable cell"), _("Warning"), MB_OK|MB_ICONWARNING);
	}
	else if( 0==strcmp(keyNode, "1_0") )
	{
		if(IDOK==MessageBox((HWND)hWnd, _("Delete this group of master panels?"), _("Delete Dialog"), MB_OKCANCEL|MB_ICONQUESTION))
		{
			master.user_id = global->activeUser.user_id;
			EXCCHECK( PMASTER->ProductMasterGroupDel(PMASTER, master) );
		
			EXCCHECK( TreeProductParamSet(PRODUCT_PARAM_RELOAD) );	
		}
	}
	else if(IDOK==MessageBox((HWND)hWnd, _("Delete current master panel?"), _("Delete Dialog"), MB_OKCANCEL|MB_ICONQUESTION))    
	{
		short int	AXvalue;
	
		master.user_id = global->activeUser.user_id;
		
		if( 0==strcmp(keyNode, "1_1") )
		{
			EXCCHECK( PMASTER->ProductMasterDel(PMASTER, master) ); 
			EXCCHECK( TreeProductParamSet(PRODUCT_PARAM_RELOAD) );		
		}
		else
		{
			/* disable sorting algorithm */
			PMASTER->sort = FALSE;
	
			EXCCHECK( PMASTER->ProductMasterDel(PMASTER, master) ); 
			
			EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemText (global->treeHandle, NULL, keyNode, keyCol, "", &AXvalue));
			EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemData (global->treeHandle, NULL, keyNode, keyCol, 0));
		}
	}

	
Error:
	PMASTER->sort = TRUE;
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
	int				index, value;
	SProductMaster	master = {0};

	switch (event)
		{
		case EVENT_COMMIT:
			
			if(control == MASTER_EXIT_OK)
			{
				EXCCHECKCVI( GetCtrlIndex (gs_panel[MASTER], MASTER_MASTER_ID, &index));
				EXCCHECKCVI( GetValueFromIndex (gs_panel[MASTER], MASTER_MASTER_ID, index, &value));
				master.product_id = gs_product_id;
				master.master_id = value;
				EXCCHECKCVI( GetCtrlVal(gs_panel[MASTER], MASTER_MASTER_CODE, master.master));
				RemoveSurroundingWhiteSpace(master.master);
				if( 0==strlen(master.master) )
					strcpy(master.master, "@empty");
				master.user_id = global->activeUser.user_id;
				
				EXCCHECK( PMASTER->ProductMasterInsert(PMASTER, master) );
				EXCCHECK( TreeProductParamSet(PRODUCT_PARAM_RELOAD) );
			}
			EXCCHECKCVI( RemovePopup(gs_panel[MASTER]));  
			break;
		}

Error:
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "FillMasterId"
static SElExceptionPtr FillMasterId(int32_t product_id)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL; 
	int32_t			parent_id = 0, index = 0;
	int				i;
	char*			name;
	
	EXCCHECKCVI( ClearListCtrl (gs_panel[MASTER], MASTER_MASTER_ID));	
	
	EXCCHECKCVI( InsertListItem (gs_panel[MASTER], MASTER_MASTER_ID, index++, "none", 0));
	
	for(i=0; i<PPRODUCT->ProductSize; i++)
	{
		if( (PPRODUCT->Product[i].property&PROPERTY_VALID)>0 
			&& PPRODUCT->Product[i].product_id == product_id )
		{
			parent_id = PPRODUCT->Product[i].parent_id;
		}
	}
	
	for(i=0; i<PPRODUCT->ProductSize; i++)
	{
		if( (PPRODUCT->Product[i].property&PROPERTY_VALID)>0 
			&& (PPRODUCT->Product[i].parent_id == parent_id 
			   || PPRODUCT->Product[i].product_id == parent_id) )
		{
			EXCCHECK( PPRODUCT->ProductGetIdName(PPRODUCT, PPRODUCT->Product[i].product_id, &name));
			EXCCHECKCVI( InsertListItem (gs_panel[MASTER], MASTER_MASTER_ID, index++, name, PPRODUCT->Product[i].product_id)); 	
		}
	}
	
Error:
	EXCRETHROW( pexception);
}