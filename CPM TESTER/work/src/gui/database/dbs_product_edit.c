#include <bklib/BKLib2.h>
#include <toolbox.h>
#include "dbs_global.h"
#include "dbs_product.h"	
#include "ring_control.h"

#define DEFAULT_COPY_MODE		PRODUCT_CP_PRODUCT \
								|PRODUCT_CP_PRODUCT_PARAMETER \
								|PRODUCT_CP_PRODUCT_MASTER \
								|PRODUCT_CP_TEST \
								|PRODUCT_CP_TEST_PARAMETER \
								|PRODUCT_CP_LINE_PROCESS \
								|PRODUCT_CP_LINE_STEP \
								|PRODUCT_CP_LINE_PARAMETER

extern SGlobalPtr		global;  
extern int				gs_panel[];
static int32_t			gs_product_id = 0;
static int32_t			gs_parent_id = 0;
static int32_t			gs_copy_mode = DEFAULT_COPY_MODE;

static int CVICALLBACK OnExitNew (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK OnExitEdit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK OnExitCopy (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "NewProductDialog"
SElExceptionPtr NewProductDialog(SProductTypes	product)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	int32_t			top = 100, left = 30;   
	int32_t			pid[32];
	int32_t			pidSize = 32;
	
	EXCCHECK( PPRODUCT->ProductGetIdTreeUp(PPRODUCT, product.product_id, pid, &pidSize) );
	
	if( pidSize < DBS_NUMBER_OF_GROUPS)
	{
		gs_product_id = product.product_id;
		gs_parent_id  = product.parent_id;
		
		EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT], PRODUCT_PARENT, product.name)); 
		EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT], PRODUCT_NAME, "")); 
		EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT], PRODUCT_DESCRIPT, ""));
		EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT], PRODUCT_SETTINGS, 0)); 
	
		EXCCHECKCVI( SetPanelAttribute (gs_panel[PRODUCT], ATTR_TITLE, _("Add New Product")));
		EXCCHECKCVI( SetCtrlAttribute (gs_panel[PRODUCT], PRODUCT_EXIT_OK, ATTR_CALLBACK_FUNCTION_POINTER, OnExitNew));
		EXCCHECKCVI( SetCtrlAttribute (gs_panel[PRODUCT], PRODUCT_EXIT_CANCEL, ATTR_CALLBACK_FUNCTION_POINTER, OnExitNew));
	
		EXCCHECKCVI( SetPanelPos (gs_panel[PRODUCT], top, left));  
		EXCCHECKCVI( InstallPopup(gs_panel[PRODUCT]));  
	
		EXCCHECKCVI( SetActiveCtrl (gs_panel[PRODUCT], PRODUCT_NAME));
	}
	else
	{
		MessageBox((HWND)hWnd, _("Can not create next item in that tree level!"), _("Warning"), MB_OK|MB_ICONWARNING);
	}
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "CopyProductDialog"
SElExceptionPtr CopyProductDialog(SProductTypes   product)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t			error = 0;
	int				i;
	int32_t* 		pid = NULL;
	int32_t			pidSize = 0;
	int32_t			top = 100, left = 30;          
	
	/* precount */
	for(i=0; i<PPRODUCT->ProductSize; i++)
	{
		if( (PPRODUCT->Product[i].property&PROPERTY_VALID)>0
			&& product.parent_id == PPRODUCT->Product[i].parent_id 
			&& product.product_id != PPRODUCT->Product[i].product_id )
		{
			pidSize++;
		}
	}
	
	if(pidSize)
	{
		pid = calloc(pidSize, sizeof(int32_t));
		
		gs_product_id = product.product_id;
		gs_parent_id  = product.parent_id;
		
		EXCCHECKCVI( ClearListCtrl (gs_panel[PRODUCT_CP], PRODUCT_CP_SOURCE));
		EXCCHECKCVI( InsertListItem (gs_panel[PRODUCT_CP], PRODUCT_CP_SOURCE, -1, "- select -", 0));
		EXCCHECKCVI( ResetTextBox (gs_panel[PRODUCT_CP], PRODUCT_CP_LISTBOX, ""));              
		
		/* list of sources */
		pidSize = 0;
		for(i=0; i<PPRODUCT->ProductSize; i++)
		{
			if( (PPRODUCT->Product[i].property&PROPERTY_VALID)>0
				&& product.parent_id == PPRODUCT->Product[i].parent_id 
				&& product.product_id != PPRODUCT->Product[i].product_id )
			{
				pid[pidSize++] = PPRODUCT->Product[i].product_id;
				EXCCHECKCVI( InsertListItem (gs_panel[PRODUCT_CP], PRODUCT_CP_SOURCE, -1, PPRODUCT->Product[i].name, PPRODUCT->Product[i].product_id));
			}
		}														 
	
		EXCCHECKCVI( SetPanelAttribute (gs_panel[PRODUCT_CP], ATTR_TITLE, _("Copy Structure")));   
		EXCCHECKCVI( SetCtrlAttribute (gs_panel[PRODUCT_CP], PRODUCT_CP_EXIT_OK, ATTR_CALLBACK_FUNCTION_POINTER, OnExitCopy));
		EXCCHECKCVI( SetCtrlAttribute (gs_panel[PRODUCT_CP], PRODUCT_CP_EXIT_CANCEL, ATTR_CALLBACK_FUNCTION_POINTER, OnExitCopy));
	
		gs_copy_mode = DEFAULT_COPY_MODE;
		
		EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_PROD_PARAM, 1));
		EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_PROD_MASTER, 1));   
		EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_TEST, 1)); 
		EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_TEST_PARAM, 1));
		EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_LINE_PROCESS, 1)); 
		EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_LINE_STEP, 1)); 
		EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_LINE_PARAM, 1)); 
		
		EXCCHECKCVI( SetPanelPos (gs_panel[PRODUCT_CP], top, left));  
		EXCCHECKCVI( InstallPopup(gs_panel[PRODUCT_CP]));  
	}
	else
	{
		MessageBox((HWND)hWnd, _("No source exist!"), _("Warning"), MB_OK|MB_ICONWARNING);
	}
	
Error:
	if(pid) free(pid);
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EditProductDialog"
SElExceptionPtr EditProductDialog(SProductTypes	product)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	int32_t			top = 100, left = 30; 
	char*			parent_name;
	
	gs_product_id = product.product_id;  
	gs_parent_id  = product.parent_id; 
	
	EXCCHECK( PPRODUCT->ProductGetIdName(PPRODUCT, product.parent_id, &parent_name) );
	EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT], PRODUCT_PARENT, parent_name)); 
	EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT], PRODUCT_NAME, product.name)); 
	EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT], PRODUCT_DESCRIPT, GET_STRING(product.description)));
	EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT], PRODUCT_SETTINGS, ((product.property&PROPERTY_VISIBLE)>0)));

	EXCCHECKCVI( SetPanelAttribute (gs_panel[PRODUCT], ATTR_TITLE, _("Edit Product")));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PRODUCT], PRODUCT_EXIT_OK, ATTR_CALLBACK_FUNCTION_POINTER, OnExitEdit));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PRODUCT], PRODUCT_EXIT_CANCEL, ATTR_CALLBACK_FUNCTION_POINTER, OnExitEdit));
	
	EXCCHECKCVI( SetPanelPos (gs_panel[PRODUCT], top, left));  
	EXCCHECKCVI( InstallPopup(gs_panel[PRODUCT]));  
	
	EXCCHECKCVI( SetActiveCtrl (gs_panel[PRODUCT], PRODUCT_NAME));
	
Error:
	EXCRETHROW( pexception); 
}


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "DelProductDialog"
SElExceptionPtr DelProductDialog(SProductTypes	product)
{
	SElExceptionPtr	pexception = NULL;
	
	if(IDOK==MessageBox((HWND)hWnd, _("Delete selected item?\n - child items will be removed\n - product parameters will be removed\n - product steps will be removed\n - step parameters will be removed"), _("Delete Product"), MB_OKCANCEL|MB_ICONQUESTION))
	{
		product.user_id = global->activeUser.user_id;
		
		EXCCHECK( PPRODUCT->ProductDel(PPRODUCT, product));

		global->product_id = product.parent_id;
		EXCCHECK( TreeProductIni());
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
	SElExceptionPtr	pexception = NULL;
	SProductTypes	product = {0};
	int32_t			value;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			if(control == PRODUCT_EXIT_OK)
			{	
				product.parent_id = gs_product_id;
				EXCCHECKCVI( GetCtrlVal(gs_panel[PRODUCT], PRODUCT_NAME, product.name));
				CHECK_VALUE(product.name);
				EXCCHECKCVI( GetCtrlVal(gs_panel[PRODUCT], PRODUCT_DESCRIPT, PDBS_ALLOC(&product.description, DBS_RECORD_LENGHT_DESCRIPTION)));
				
				EXCCHECKCVI( GetCtrlVal(gs_panel[PRODUCT], PRODUCT_SETTINGS, &value));
				if(value) product.property |= PROPERTY_VISIBLE;

				product.user_id = global->activeUser.user_id;
				
				EXCCHECK( PPRODUCT->ProductNew(PPRODUCT, &product)); 
				
				global->product_id = product.product_id;
				EXCCHECK( TreeProductIni());
			}
			EXCCHECKCVI( RemovePopup(gs_panel[PRODUCT])); 
			break;
	}
		
Error:
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnExitCopy"
static int CVICALLBACK OnExitCopy (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{	
	int32_t			error = 0; 
	SElExceptionPtr pexception = NULL;
	int				index, product_id, count;
	
	switch (event)
	{
		case EVENT_COMMIT:
			if(control == PRODUCT_CP_EXIT_OK)
			{
				EXCCHECKCVI( GetCtrlIndex (gs_panel[PRODUCT_CP], PRODUCT_CP_SOURCE, &index));
				EXCCHECKCVI( GetValueFromIndex (gs_panel[PRODUCT_CP], PRODUCT_CP_SOURCE, index, &product_id));
				EXCCHECKCVI( GetNumTextBoxLines (gs_panel[PRODUCT_CP], PRODUCT_CP_LISTBOX, &count));
				if(!count)
				{
					MessageBox((HWND)hWnd, _("There is nothing to copy"), _("Warning"), MB_OK|MB_ICONWARNING); 
					goto Error;
				}

				if(product_id&&count)
				{
					SProductTypes	source = {0};  	
					SProductTypes	target = {0};  	
					
					source.product_id = product_id;
					target.product_id = gs_product_id;
					target.user_id = global->activeUser.user_id;
					
					EXCCHECK( PPRODUCT->ProductCopy(PPRODUCT, source, target, gs_copy_mode));

					global->product_id = gs_product_id;
					EXCCHECK( TreeProductIni());    
				}
			}
			
			EXCCHECKCVI( RemovePopup(gs_panel[PRODUCT_CP]));  
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
	SProductTypes	product = {0};
	int32_t			value;

	switch (event)
	{
		case EVENT_COMMIT:
			if(control == PRODUCT_EXIT_OK)
			{
				product.product_id = gs_product_id;
				product.parent_id  = gs_parent_id;
				EXCCHECKCVI( GetCtrlVal(gs_panel[PRODUCT], PRODUCT_NAME, product.name));
				CHECK_VALUE(product.name); 

				EXCCHECKCVI( GetCtrlVal(gs_panel[PRODUCT], PRODUCT_DESCRIPT, PDBS_ALLOC(&product.description, DBS_RECORD_LENGHT_DESCRIPTION)));
				
				EXCCHECKCVI( GetCtrlVal(gs_panel[PRODUCT], PRODUCT_SETTINGS, &value));
				if(value) product.property |= PROPERTY_VISIBLE;

				product.user_id = global->activeUser.user_id;
				
				EXCCHECK( PPRODUCT->ProductEdit(PPRODUCT, product));  

				global->product_id = gs_product_id;
				EXCCHECK( TreeProductIni());
			}
			EXCCHECKCVI( RemovePopup(gs_panel[PRODUCT]));  
			break;
	}

Error:
	EXCDISPLAY(pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnCopySourceSelect"
int CVICALLBACK OnCopySourceSelect (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			error =0;
	SElExceptionPtr pexception = NULL;
	int 			index, i, j;
	int32_t			product_id;
	int32_t			pidSize = 512;
	int32_t 		pid[512];
	
	switch (event)
	{
		case EVENT_COMMIT:
			EXCCHECKCVI( GetCtrlIndex (gs_panel[PRODUCT_CP], PRODUCT_CP_SOURCE, &index));
			EXCCHECKCVI( GetValueFromIndex (gs_panel[PRODUCT_CP], PRODUCT_CP_SOURCE, index, &product_id));
			
			EXCCHECKCVI( ResetTextBox (gs_panel[PRODUCT_CP], PRODUCT_CP_LISTBOX, ""));
			
			if(product_id != 0)
			{
				EXCCHECK( PPRODUCT->ProductGetIdTreeDown(PPRODUCT, product_id, pid, &pidSize) );
			
				for(i=0; i<pidSize; i++)
				{
					for(j=0; j<PPRODUCT->ProductSize; j++)
					{
						if( PPRODUCT->Product[j].product_id == pid[i] 
							&& PPRODUCT->Product[j].product_id != product_id 
							&& (PPRODUCT->Product[j].property&PROPERTY_VALID)>0 )
						{
							EXCCHECKCVI( InsertTextBoxLine (gs_panel[PRODUCT_CP], PRODUCT_CP_LISTBOX, -1, PPRODUCT->Product[j].name));
						}
					}
				}
			}

			break;
	}
	
Error:
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnCP"
int CVICALLBACK OnCP (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			error =0;
	SElExceptionPtr pexception = NULL;
	int				value1, value2, value3, value4, value5, value6, value7;
	
	switch (event)
	{
		case EVENT_COMMIT:
			EXCCHECKCVI( GetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_PROD_PARAM, &value1));
			EXCCHECKCVI( GetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_TEST, &value2)); 
			EXCCHECKCVI( GetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_TEST_PARAM, &value3));
			EXCCHECKCVI( GetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_PROD_MASTER, &value4)); 
			EXCCHECKCVI( GetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_LINE_PROCESS, &value5)); 
			EXCCHECKCVI( GetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_LINE_STEP, &value6)); 
			EXCCHECKCVI( GetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_LINE_PARAM, &value7)); 
			
			if(value2==0)
			{
				value3 = 0;
				EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_TEST_PARAM, value3)); 	
			}
			if(value5==0)
			{
				value6 = 0;
				EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_LINE_STEP, value6));  
			}
			if(value6==0)
			{
				value7 = 0;
				EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT_CP], PRODUCT_CP_SET_LINE_PARAM, value7)); 
			}
			
			gs_copy_mode = PRODUCT_CP_PRODUCT;
			
			if(value1)
				gs_copy_mode |= PRODUCT_CP_PRODUCT_PARAMETER;
			if(value2)
				gs_copy_mode |= PRODUCT_CP_TEST;
			if(value3)
				gs_copy_mode |= PRODUCT_CP_TEST_PARAMETER;
			if(value4)
				gs_copy_mode |= PRODUCT_CP_PRODUCT_MASTER;
			if(value5)
				gs_copy_mode |= PRODUCT_CP_LINE_PROCESS;
			if(value6)
				gs_copy_mode |= PRODUCT_CP_LINE_STEP;
			if(value7)
				gs_copy_mode |= PRODUCT_CP_LINE_PARAMETER;

			break;
	}
	
Error:
	EXCDISPLAY( pexception);
	return 0;
}
