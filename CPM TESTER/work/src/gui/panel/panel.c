#include <msxmldom.h>
#include <utility.h>
#include <users/user.h>
#include <guid.h>
#include <windows.h>
#include <userint.h>
#include <stdio.h>
#include <language/language.h>
#include "panel.h" 
#include "panel_fnc.h"
#include "panel_plugin.h"
																				  
const char* HISTORY = \
"[2008-01-08] zR 1.21\n"
" * Removed toolbox from the project.\n"
"[2008-01-07] rT 1.20\n"
" * Function SetPanelTable was edited because of application freezing.\n"
"[2007-12-13] rT 1.10\n"
" * Functions OnPanel, OnMenu, OnSubmenu were edited because of algorithm speed.\n";


#define PANEL_ITEM_NUMBER		12
#define MENU_ITEM_NUMBER 		8
#define SUBMENU_ITEM_NUMBER 	8 
#define SHORTCUT_ITEM_NUMBER 	8        

/* panels positions */
static int32_t					gv_PanelTop = 70;
static int32_t					gv_PanelLeft = 0;
#define PANEL_TOP				gv_PanelTop
#define PANEL_LEFT				gv_PanelLeft

static int32_t					gv_MenuTop = 70 + 742;
static int32_t					gv_MenuLeft = 9 + 2;
static int32_t					gv_MenuWidth = 467;
static int32_t					gv_MenuHeight = 741;
static int32_t					gv_MenuItemHeight = 82;
#define MENU_TOP				gv_MenuTop
#define MENU_LEFT				gv_MenuLeft
#define MENU_WIDTH				gv_MenuWidth
#define MENU_HEIGHT				gv_MenuHeight
#define MENU_ITEM_HEIGHT		gv_MenuItemHeight

static int32_t					gv_SubMenuLeft = 9 + 2 + 467;
#define SUBMENU_LEFT			gv_SubMenuLeft
#define SUBMENU_ITEM_HEIGHT		60

/* tables offsets */
#define PANEL_CELL_X_OFFSET		80
#define PANEL_CELL_X_SHIFT		140

#define MENU_CELL_Y_OFFSET		40
#define MENU_CELL_Y_SHIFT		80

#define SUBMENU_CELL_Y_OFFSET	30
#define SUBMENU_CELL_Y_SHIFT	60
	
#define COLOR_TEXT  VAL_WHITE 

#define NB_OF_PANEL 		2
static int					gs_panel[NB_OF_PANEL + 1];
static SElEventFactoryPtr	gs_EventFactory; 
static SObjListPtr          gs_Modules;
SLanguagePtr				gs_pLanguage = NULL; 
static Point				gs_lastMenuCell;
static Point				gs_lastSubMenuCell;
static Point				gs_lastPanelCell;
static Point				gs_lastPanelIcon;
static SPanelItem 			SPanel[PANEL_ITEM_NUMBER]; 
static bool_t				gs_is_locked = FALSE;
HINSTANCE					g_hInstance = NULL;
	
#define TIMEOUT_HIDE	    10
static int					gs_position_menu_hide;
static bool_t               gs_Hide;
int 						gs_time;
int                         gs_selected;

static SLastPositionPtr		gs_lastSavedPosition;

static SElExceptionPtr SetPanelTableHighlight(int position);

CRITICAL_SECTION   _Lock;


static int Decoration_Menu [PANEL_ITEM_NUMBER] = 
	{
		PANEL_DECORATION_MENU_1, 	
		PANEL_DECORATION_MENU_2, 	
		PANEL_DECORATION_MENU_3, 	
		PANEL_DECORATION_MENU_4,
		PANEL_DECORATION_MENU_5, 	
		PANEL_DECORATION_MENU_6, 	
		PANEL_DECORATION_MENU_7, 	
		PANEL_DECORATION_MENU_8,
		PANEL_DECORATION_MENU_9, 	
		PANEL_DECORATION_MENU_10, 	
		PANEL_DECORATION_MENU_11, 	
		PANEL_DECORATION_MENU_12,
	};
static int Menu [PANEL_ITEM_NUMBER] = 
	{
		PANEL_MENU_1, 	
		PANEL_MENU_2, 	
		PANEL_MENU_3, 	
		PANEL_MENU_4,
		PANEL_MENU_5, 	
		PANEL_MENU_6, 	
		PANEL_MENU_7, 	
		PANEL_MENU_8,
		PANEL_MENU_9, 	
		PANEL_MENU_10, 	
		PANEL_MENU_11, 	
		PANEL_MENU_12,
	};

static int LabelMenu [PANEL_ITEM_NUMBER] = 
	{
		PANEL_LABEL_MENU_1, 	
		PANEL_LABEL_MENU_2, 	
		PANEL_LABEL_MENU_3, 	
		PANEL_LABEL_MENU_4,
		PANEL_LABEL_MENU_5, 	
		PANEL_LABEL_MENU_6, 	
		PANEL_LABEL_MENU_7, 	
		PANEL_LABEL_MENU_8,
		PANEL_LABEL_MENU_9, 	
		PANEL_LABEL_MENU_10, 	
		PANEL_LABEL_MENU_11, 	
		PANEL_LABEL_MENU_12,
	};

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "fncLock"
static SElExceptionPtr fncLock(void)
{
	EnterCriticalSection(&_Lock);
	return NULL;
} 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "fncUnlock"
static SElExceptionPtr fncUnlock(void)
{
	LeaveCriticalSection(&_Lock);
	return NULL;
} 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SaveLastPosition"
static SElExceptionPtr SaveLastPosition(
   int	position,
   struct _SPanelItem spanel
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0; 
	bool_t			found = FALSE;
	int				i = 0;
	
	if(spanel.save_position)
	{
		if(gs_lastSavedPosition)
		{
			/* search */
			while(i<gs_lastSavedPosition->allocated && !found)
			{
				if(strlen(gs_lastSavedPosition->pSavedPos[i].panel)==0)
					break;
				
				if(0==strcmp(gs_lastSavedPosition->pSavedPos[i].panel, spanel.panel))
				{
					gs_lastSavedPosition->pSavedPos[i].position = position;
					found = TRUE;	
					continue;
				}
				
				i++;
			}
			
			if(!found && i<gs_lastSavedPosition->allocated)
			{
				gs_lastSavedPosition->pSavedPos[i].position = position;	
				strcpy(gs_lastSavedPosition->pSavedPos[i].panel, spanel.panel);
			}
			else if(!found && i==gs_lastSavedPosition->allocated) 
			{
				gs_lastSavedPosition->pSavedPos = realloc( gs_lastSavedPosition->pSavedPos, 
														   (gs_lastSavedPosition->allocated + 10)*sizeof(SLastPositionItem)); 
				EXCCHECKALLOC(gs_lastSavedPosition->pSavedPos); 
				gs_lastSavedPosition->allocated += 10; 
				
				gs_lastSavedPosition->pSavedPos[i].position = position;	
				strcpy(gs_lastSavedPosition->pSavedPos[i].panel, spanel.panel);  
			}
		}
		else
		{
			gs_lastSavedPosition = calloc(1, sizeof(SLastPosition));
			EXCCHECKALLOC(gs_lastSavedPosition);
			
			gs_lastSavedPosition->pSavedPos = calloc(10, sizeof(SLastPositionItem)); 
			EXCCHECKALLOC(gs_lastSavedPosition->pSavedPos);  
			gs_lastSavedPosition->allocated = 10;
			
			gs_lastSavedPosition->pSavedPos[0].position = position;
			strcpy(gs_lastSavedPosition->pSavedPos[0].panel, spanel.panel);
		}
	}
	
Error:
	EXCRETHROW( pexception);
}	

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetLastPosition"
static SElExceptionPtr GetLastPosition(
   const char* panel,
   int*	position
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0; 
	bool_t			found = FALSE;
	int				i = 0;
	
	if(position) *position = 0;
		
	if(gs_lastSavedPosition)
	{
		/* search */
		while(i<gs_lastSavedPosition->allocated && !found)
		{
			if(strlen(gs_lastSavedPosition->pSavedPos[i].panel)==0)
				break;
			
			if(0==strcmp(gs_lastSavedPosition->pSavedPos[i].panel, panel))
			{
				if(position) *position = gs_lastSavedPosition->pSavedPos[i].position;
				found = TRUE;				
				continue;
			}
			
			i++;
		}
	}

Error:
	EXCRETHROW( pexception);
}	

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ProcessEvents"
static SElExceptionPtr ProcessEvents(
   struct _SPanelItem 	spanel
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	SElEventDataPtr	pevent_data = NULL; 
	int				i;	           
	
	if ( gs_EventFactory && spanel.valid && !spanel.dimmed)
	{
		EXCCHECK( eleventdata_new(&pevent_data));
		
		for(i=0; i<spanel.event_nb; i++)
		{
			EXCCHECKCVI( ProcessSystemEvents ());
			
			if(strlen(spanel.data[i])>0)
			{
				EXCCHECK( pevent_data->SetStringPtr(pevent_data, spanel.data[i])); 
				
				if(strlen(spanel.event[i])>0)
					EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, spanel.event[i], pevent_data));
			}
			else if(strlen(spanel.event[i])>0)
			{
				EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, spanel.event[i], NULL)); 
			}
		}
	}
	
Error:
	eleventdata_delete(&pevent_data); 
	EXCRETHROW(pexception);
}
/*---------------------------------------------------------------------------*/
/*#undef __FUNC__
#define __FUNC__ "OnPanel"
int CVICALLBACK OnPanel (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;
	bool_t			is_locked = FALSE;

	if(gs_is_locked)
		goto Error;

	switch (event)
	{
		case EVENT_LEFT_CLICK:
			{
				int32_t error = 0;
				Point point, cell, Table_cell;
				bool_t valid_cell;

				EXCCHECK( fncLock()); 
				is_locked = TRUE;

				point.y = eventData1;
				point.x = eventData2;
				
				EXCCHECKCVI( GetTableCellFromPoint (panel, control, point, &cell));
				
				cell.x = (cell.x+1)/2;
			
				valid_cell = (cell.x && cell.y && SPanel[cell.x - 1].valid && !SPanel[cell.x - 1].dimmed);
				if(valid_cell)
				{   Table_cell.y = cell.y;
					Table_cell.x = cell.x*2;
					PANEL_HIGHLIGHT_SET( Table_cell);
					
					Table_cell.x =cell.x*2-1;
					PANEL_HIGHLIGHT_ICON( Table_cell);
					EXCCHECK( SaveLastPosition(cell.x, SPanel[cell.x - 1]));
					EXCCHECK( ProcessEvents(SPanel[cell.x - 1]));  
				}
			}
			break;
	}
	
Error:
	if(is_locked)
		fncUnlock();
	EXCDISPLAY( pexception);
	return 0;
}*/

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Highlight_Buttom"
static SElExceptionPtr  Highlight_Buttom (int panel, int control) 
{ 
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int 			i;
	
	for (i=0; i<PANEL_ITEM_NUMBER;i++)
	{
		if (Menu[i] == control)
		{
			gs_selected = i;
				
			if ( SPanel[i].valid) 
			{
				EXCCHECKCVI( SetCtrlAttribute (panel, Menu[i], ATTR_PICT_BGCOLOR, 0x797979));
				EXCCHECKCVI( SetCtrlAttribute (panel, Decoration_Menu[i], ATTR_VISIBLE, TRUE));
			}

			else 
			{
				EXCCHECKCVI( SetCtrlAttribute (panel, Menu[i], ATTR_PICT_BGCOLOR, 0x686868));
				EXCCHECKCVI( SetCtrlAttribute (panel, Decoration_Menu[i], ATTR_VISIBLE, FALSE));
			}
		}
	
		else 
		{
			EXCCHECKCVI( SetCtrlAttribute (panel, Menu[i], ATTR_PICT_BGCOLOR, 0x686868));
			EXCCHECKCVI( SetCtrlAttribute (panel, Decoration_Menu[i], ATTR_VISIBLE, FALSE));
		}
	}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Highlight_Buttom2"
static SElExceptionPtr  Highlight_Buttom2 (int panel, int Selected) 
{ 
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int 			i;
	
	for (i=0; i<PANEL_ITEM_NUMBER;i++)
	{
		EXCCHECKCVI( SetCtrlAttribute (panel, Decoration_Menu[i], ATTR_VISIBLE, FALSE));
	}
	
	if (gs_selected <PANEL_ITEM_NUMBER)
	EXCCHECKCVI( SetCtrlAttribute (panel, Decoration_Menu[gs_selected], ATTR_VISIBLE, TRUE));
   	
	gs_selected = Selected;

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetMenuID"
static SElExceptionPtr  GetMenuID (int panel, int control, int *id) 
{ 
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int 			i;
	
	for (i=0; i<PANEL_ITEM_NUMBER;i++)
	{
		if(Menu[i] == control)
		{
		 *id = i;
		 break;
		}
		else *id = -1;
		
	}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnPanel"
int CVICALLBACK OnPanel (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;
	bool_t			is_locked = FALSE;
	int       id;

	gs_time = 0;
	
	if(gs_is_locked)
		goto Error;
	
	switch (event)
	{
		case EVENT_MOUSE_POINTER_MOVE :
		 Highlight_Buttom(panel, control);
		break;
		
		case EVENT_LEFT_CLICK:
			{
				int32_t error = 0;
				bool_t valid_cell;

				EXCCHECK( fncLock()); 
				is_locked = TRUE;
				
				EXCCHECK( GetMenuID (panel, control, &id));
				
				valid_cell = ((id+1) && SPanel[id].valid && !SPanel[id].dimmed);
				
				if(valid_cell)
				{
					//PANEL_HIGHLIGHT_SET( id);	
					EXCCHECK( SaveLastPosition(id, SPanel[id]));
					EXCCHECK( ProcessEvents(SPanel[id]));  
				}
			}
			break;
	}
	
Error:
	if(is_locked)
		fncUnlock();
	EXCDISPLAY( pexception);
	return 0;
}

int CVICALLBACK OnPanelCallback (int panel, int event, 
		void *callbackData, int eventData1, int eventData2)
{
    int  virtual = 0;
    char msg[128];
	
	if (event == EVENT_KEYPRESS)
	{
		if (!KeyPressEventIsLeadByte (eventData2))
			virtual   = GetKeyPressEventVirtualKey (eventData2);

		//sprintf (msg, "%d", gs_selected);
		//SetCtrlVal (panel, PANEL_TEXT, msg);
		
		switch (virtual)
		{
			case VAL_LEFT_ARROW_VKEY :
				if (gs_selected>0) 
				{
				Highlight_Buttom2(panel, gs_selected - 1);
				}
				else if (gs_selected == 0) 
				{
				gs_selected = 15; 
				Highlight_Buttom2(panel, gs_selected);
				}
			break;
		
			case VAL_RIGHT_ARROW_VKEY :
			if (gs_selected<PANEL_ITEM_NUMBER) 
				{
				Highlight_Buttom2(panel, gs_selected + 1);
				}
				else if (gs_selected == PANEL_ITEM_NUMBER) 
				{
				gs_selected = 0; 
				Highlight_Buttom2(panel, gs_selected);
				}
			break;

#if 0    /* formerly excluded lines */
		
			case VAL_UP_ARROW_VKEY :
				if (selected>4) selected = gs_selected - 4;
				Highlight_Buttom2(panel, selected);
			break;
				
			case VAL_DOWN_ARROW_VKEY :	  
				if (selected<12) selected = gs_selected + 4;
				Highlight_Buttom2(panel, selected);
			break;
#endif   /* formerly excluded lines */
		}
	}			
	return 0;
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "CheckRights"
static SElExceptionPtr CheckRights(
   	struct _SPanelItem*	ppanel,
	int item_number
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	int				i;
	SUserListPtr	puserlist = NULL;
	
	if(gs_Modules)
		puserlist = (SUserListPtr)gs_Modules->Get(gs_Modules, MODULE_USER); 

	for(i=0; i<item_number; i++)
	{
		if(ppanel[i].valid)
		{
			ppanel[i].dimmed = FALSE; 
	
			if(strlen(ppanel[i].rights)>0)
			{
				SUserPtr puser = NULL; 
				
				ppanel[i].dimmed = TRUE;  
				
				if(puserlist)
				{
					EXCCHECK( puserlist->GetLoggedUser(puserlist, &puser)); 
					if(puser && puser->GetTesterRights(puser, ppanel[i].rights))
					{
						ppanel[i].dimmed = FALSE; 
					}
				}
			}
		}
	}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetPanelTableHighlight"
static SElExceptionPtr SetPanelTableHighlight(
	int position
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	Point			cell;
	
	/* storno selection */
#if 0    /* formerly excluded lines */
	PANEL_HIGHLIGHT_RESET();
#endif   /* formerly excluded lines */
	
	if(position==0)  
		EXCCHECK( GetLastPosition(SPanel[0].panel, &position));	
	
	if( position>0)
	{
		cell.y = 1;
		cell.x = position;
			
#if 0    /* formerly excluded lines */
		PANEL_HIGHLIGHT_SET( cell);		
#endif   /* formerly excluded lines */
		EXCCHECK( SaveLastPosition(cell.x, SPanel[cell.x - 1]));    
	}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetPanelTable"
static SElExceptionPtr SetPanelTable(
	int position
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	int				i;
	Point			cell;     

	EXCCHECK( CheckRights(SPanel, PANEL_ITEM_NUMBER));
		
	for(i=0; i<PANEL_ITEM_NUMBER; i++)
	{
			
		EXCCHECKCVI( SetCtrlVal (gs_panel[PANEL], LabelMenu[i], SPanel[i].name));

		EXCCHECKCVI( SetCtrlAttribute (gs_panel[PANEL], LabelMenu[i], ATTR_DIMMED, SPanel[i].dimmed));
		EXCCHECKCVI( SetCtrlAttribute (gs_panel[PANEL], Menu[i], ATTR_DIMMED, SPanel[i].dimmed));
		EXCCHECKCVI( SetCtrlAttribute (gs_panel[PANEL], Decoration_Menu[i], ATTR_DIMMED, SPanel[i].dimmed));    
		if (strlen(SPanel[i].spict))
		{
   	        EXCCHECKCVI( InsertListItem (gs_panel[PANEL], Menu[i], 0, SPanel[i].spict, 0));
      	    SetCtrlIndex (gs_panel[PANEL], Menu[i], 0);
		}
		else 
		{
			EXCCHECKCVI( DeleteListItem (gs_panel[PANEL], Menu[i], 0, -1));
		}
	}
	
	EXCCHECK( SetPanelTableHighlight(position)); //FINISH
	
Error:
	EXCRETHROW( pexception);
}

void SetPanelTransparent(int ParentPanel, int Panel, double transparency)
{
	int hwnd_Parent;
	int hwnd;
    double alpha;
	
/* Get the window handle (HWND) of the panel, using the ATTR_SYSTEM_WINDOW_HANDLE panel attribute: */
GetPanelAttribute (ParentPanel, ATTR_SYSTEM_WINDOW_HANDLE, &hwnd_Parent);
GetPanelAttribute (Panel, ATTR_SYSTEM_WINDOW_HANDLE, &hwnd);

/*Configure windows layering:*/ 
//SetWindowLong ((HWND)hwnd, GWL_EXSTYLE, GetWindowLong((HWND)hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
SetWindowLong ((HWND)hwnd, GWL_EXSTYLE, GetWindowLong((HWND)hwnd, GWL_HWNDPARENT) | WS_EX_LAYERED);

// Set the alpha for transparency: 0 is transparent and 255 is opaque.
alpha = transparency * (255.0 / 100);
SetLayeredWindowAttributes ((HWND)hwnd, 0, alpha, LWA_ALPHA);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PostDeferredDisplayPanel"
static void CVICALLBACK PostDeferredDisplayPanel (void *callbackData) 
{ 
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int i;
	
	for (i=0; i<960;)
	{
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PANEL], ATTR_LEFT, gs_position_menu_hide+i));
	i = i + 40;
	//Sleep(1);
	SetActivePanel (gs_panel[PANEL]);
	ProcessDrawEvents ();
	}
	
	gs_Hide = FALSE;

Error:
	EXCDISPLAY( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PostDeferredHidePanel"
static void CVICALLBACK PostDeferredHidePanel (void *callbackData) 
{ 
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int i, position_menu;
	

	EXCCHECKCVI( GetPanelAttribute (gs_panel[PANEL], ATTR_LEFT, &position_menu));

	for (i=position_menu; i>gs_position_menu_hide;) 
	{
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PANEL], ATTR_LEFT, i));
	i = i - 50;
	//Sleep(1);
	ProcessDrawEvents ();
	}
	
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PANEL], ATTR_LEFT, gs_position_menu_hide));
	ProcessDrawEvents ();
	
	gs_Hide = TRUE; 
	
Error:
	EXCDISPLAY( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PostDeferredDisplayMenu"
static void CVICALLBACK PostDeferredDisplayMenu (void *callbackData) 
{ 
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	
	EXCCHECKCVI( DisplayPanel(gs_panel[PANEL])); 
	//EXCCHECKCVI( SetActivePanel( gs_panel[MENU]));
	
Error:
	EXCDISPLAY( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PostDeferredHideMenu"
static void CVICALLBACK PostDeferredHideMenu (void *callbackData) 
{ 
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	
	EXCCHECKCVI( HidePanel(gs_panel[PANEL])); 
	
Error:
	EXCDISPLAY( pexception);
}

 /*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EventFunctionCallback"
static SElExceptionPtr EventFunctionCallback( const char* eventName, void* pData)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	bool_t			rights = FALSE;	
	char			mode[64] = "";
	char*			pdata;
	int				position;
	char			path[MAX_PATHNAME_LEN ];			  
	
	EXCCHECK( fncLock());  
	
	EXCCHECKCVI(GetModuleDir (__CVIUserHInst, path));
	strcat(path, PANEL_XML);

	if(0==strcmp(eventName, "EVNT_GUI_PANEL"))
	{
		if(pData)
		{
			const char* pmode = ((SElEventDataPtr)pData)->GetStringPtr(pData);   
			
			if(strlen(pmode)>0)
			{
				strcpy(mode, pmode);
				pdata = strtok(mode, ";");
				EXCCHECK( LoadConfiguration(path, pdata, SPanel, PANEL_ITEM_NUMBER));
				
				pdata = strtok(NULL, ";");
				if(pdata==NULL)
					position = 0;
				else
					position = strtol(pdata, NULL, 10);
				
				EXCCHECK( SetPanelTable(position));
			}
		}
	}

	else if(0==strcmp(eventName, "EVNT_GUI_PANEL_LOCK"))
	{
		gs_is_locked = TRUE;
	}

	else if(0==strcmp(eventName, "EVNT_GUI_PANEL_UNLOCK"))
	{
		gs_is_locked = FALSE;
	}
	
	else if(0==strcmp(eventName, "EVNT_USER_LOGIN_VALIDATED"))
	{
		EXCCHECK( SetPanelTable(0));
		
		EXCCHECK( CheckRights(SPanel, PANEL_ITEM_NUMBER));
	}

	else if(0==strcmp(eventName, "EVNT_USER_LOGOUT_VALIDATED"))
	{
		EXCCHECK( SetPanelTable(0));
		
		EXCCHECK( CheckRights(SPanel, PANEL_ITEM_NUMBER));
	}
	
	else if(0==strcmp(eventName, "EVNT_GUI_MENU_HIDE"))
	{
		EXCCHECKCVI( PostDeferredCall( PostDeferredHidePanel, NULL));
	}
	else if(0==strcmp(eventName, "EVNT_GUI_MENU_DISPLAY"))
	{
		EXCCHECKCVI( PostDeferredCall( PostDeferredDisplayPanel, NULL));
	}
	
	
		
Error:  
	fncUnlock();
	EXCRETHROW( pexception);
}

static SElExceptionPtr ButtonEventFunctionCallback( const char* eventName, void* pData)
{
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "RegisterEventFunction"
static SElExceptionPtr RegisterEventFunction(void)
{
	int				error = 0;
	SElExceptionPtr	pexception = NULL;
	
	if ( gs_EventFactory )
	{
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_PANEL",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_PANEL_LOCK",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_PANEL_UNLOCK",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_MENU",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_SUBMENU",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_MENU_HIDE",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_MENU_DISPLAY",
														 EventFunctionCallback));	
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_USER_LOGIN_VALIDATED",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_USER_LOGOUT_VALIDATED",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_KEY_PRESSED",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_TESTER_BUTTON_PRESSED_OK",
														 ButtonEventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_TESTER_BUTTON_PRESSED_NOK",
														 ButtonEventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_TESTER_BUTTON_PRESSED_MENU",
														 ButtonEventFunctionCallback));
	}

Error:
	EXCRETHROW(pexception);
}	/* RegisterEventFunction */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Cb_Timer"
int CVICALLBACK Cb_Timer (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;
	bool_t			is_locked = FALSE;
	int32_t			error = 0;

	if(gs_is_locked)
		goto Error;

	switch (event)
	{
		case EVENT_TIMER_TICK:

			 if (gs_Hide == FALSE)
			 {
				 if (gs_time > TIMEOUT_HIDE)
				 {
				 EXCCHECKCVI( PostDeferredCall( PostDeferredHidePanel, NULL));
				 EXCCHECKCVI( PostDeferredCall( PostDeferredDisplayMenu, NULL));
				 gs_Hide = TRUE;
				 gs_time = 0;	 
				 }
			 	else 
			 	{
			 	gs_time++;	 
			 	}
			 }
			
	break;
	}
	
Error:
	if(is_locked)
		fncUnlock();
	EXCDISPLAY( pexception);
	return 0;
}


/*---------------------------------------------------------------------------*/
#if defined(_CVI_DLL_) || \
	defined(_WINDLL)
void DiscardUIObjectsForDLL (void)
{
	/* Discard the panels loaded in InitUIForDLL */
}

int __stdcall DllMain(
	HINSTANCE hinstDLL,
	DWORD fdwReason,
	LPVOID lpvReserved
)
{
	switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH:
#if (!defined(_CVI_)&&defined(_DEBUG))
			//_CrtSetBreakAlloc(79);
#endif
			if (InitCVIRTE (hinstDLL, 0, 0) == 0)		
				return 0;	/* out of memory */
			g_hInstance = hinstDLL;
			break;
		case DLL_PROCESS_DETACH:
			if (!CVIRTEHasBeenDetached())		/* Do not call CVI functions if cvirte.dll has already been detached. */
			{
				DiscardUIObjectsForDLL ();		/* Discard the panels loaded in InitUIForDLL */
				CloseCVIRTE ();		
			}
#if !defined(_CVI_) && defined(_DEBUG) && defined(DUMP_MEMORY_LEAKS)
			_CrtDumpMemoryLeaks();
#endif
			break;
		}

	return 1;
}

int __stdcall DllEntryPoint(
	HINSTANCE hinstDLL,
	DWORD fdwReason,
	LPVOID lpvReserved
)
{
	/* Included for compatibility with Borland */

	 return DllMain (hinstDLL, fdwReason, lpvReserved);
}

#undef __FUNC__
#define __FUNC__ "{Panel Plugin}::Initialize"
PLUGIN_API SElExceptionPtr PLUGIN_INIT_NAME_CODE( SPluginArgsPtr a_pPluginArgs )
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int				main_panel = 0;
	const char*		pconfiguration_file = NULL;
	int				monitor_id, 
					left, 
					top,
					height, 
					width,
					i,
					table_height, 
					panel_main_height, 
					panel_height, 
					panel_main_width;
	
	int             icon_size, menu_size, size;
	
	
	InitializeCriticalSection( &_Lock);
	
	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "MainPanel",
									 sizeof(main_panel),
									 &main_panel));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "WindowHandle",
									 sizeof(hWnd),
									 &hWnd));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        "Modules",
	                        sizeof(SObjListPtr),
	                        &gs_Modules));

	/* gettext */
	if(gs_Modules)
	{
		gs_pLanguage = (SLanguagePtr)gs_Modules->Get(gs_Modules, MODULE_LANGUAGE);  
		if(gs_pLanguage)
		{
			char pathname[MAX_PATHNAME_LEN];
			EXCCHECKCVI( GetProjectDir( pathname));
			strcat(pathname, "\\lang");
			LANGUAGE_PTR->bindtextdomain (LANGUAGE_PTR, PACKAGE, pathname);
			LANGUAGE_PTR->textdomain (LANGUAGE_PTR, PACKAGE);
		}
	}
	
	EXCCHECKCVI( gs_panel[PANEL] = LoadPanelEx (main_panel, "panel.uir", PANEL, __CVIUserHInst));
	EXCCHECKCVI( gs_panel[PANEL_2] = LoadPanelEx (main_panel, "panel.uir", PANEL_2, __CVIUserHInst));

	//SetPanelTransparent (main_panel, gs_panel[PANEL], 30);

	EXCCHECKCVI( GetPanelAttribute (main_panel, ATTR_TOP, &top));
	EXCCHECKCVI( GetPanelAttribute (main_panel, ATTR_HEIGHT, &height));
	
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PANEL], ATTR_TOP, 105));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PANEL], ATTR_LEFT, -920));
	
	//EXCCHECKCVI( SetPanelAttribute (gs_panel[PANEL_2], ATTR_TOP, height-65));
	//EXCCHECKCVI( SetPanelAttribute (gs_panel[PANEL_2], ATTR_LEFT, 0));
	
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PANEL_2], ATTR_TOP, height-60));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PANEL_2], ATTR_LEFT, 0));
	
	EXCCHECKCVI(SetPanelAttribute (gs_panel[PANEL], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
	EXCCHECKCVI(SetPanelAttribute (gs_panel[PANEL_2], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));

	EXCCHECKCVI( GetPanelAttribute (gs_panel[PANEL], ATTR_LEFT, &gs_position_menu_hide));
		
	gs_Hide = TRUE;
	
	for (i=0; i<PANEL_ITEM_NUMBER;i++)
		EXCCHECKCVI( SetCtrlAttribute (gs_panel[PANEL], Decoration_Menu[i], ATTR_VISIBLE, FALSE));
		
	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "EventFactory",
									 sizeof(SElEventFactoryPtr),
									 &gs_EventFactory));

	
	EXCCHECK( LoadConfiguration(NULL, "panel", SPanel, PANEL_ITEM_NUMBER));

	EXCCHECK( SetPanelTable(1));
	
	EXCCHECK( RegisterEventFunction());

	EXCCHECKCVI( DisplayPanel(gs_panel[PANEL]));
   	EXCCHECKCVI( DisplayPanel(gs_panel[PANEL_2]));


	gs_time = 0;

Error:	
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Panel Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;

Error:	
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Panel Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	
Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Panel Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int32_t			i;

	if(gs_lastSavedPosition)
	{
		if(gs_lastSavedPosition->pSavedPos)
		{
			free(gs_lastSavedPosition->pSavedPos);
			gs_lastSavedPosition->pSavedPos = NULL;
		}
		
		free(gs_lastSavedPosition);
		gs_lastSavedPosition = NULL;
	}

#if 0    /* formerly excluded lines */
	/* clear menu pictures */
	for(i=0; i<MENU_ITEM_NUMBER; i++)
	{
		if(SMenu[i].pict!=0)
		{
			DiscardBitmap(SMenu[i].pict);
			SMenu[i].pict = 0;
		}
	}

	EXCCHECK( elzip_delete(&gs_pZip));
#endif   /* formerly excluded lines */
	
	if(gs_panel[PANEL])
		DiscardPanel(gs_panel[PANEL]);
	
	DeleteCriticalSection( &_Lock);  
	
Error:	
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Panel Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
	SElExceptionPtr	pexception = NULL;  
	
	if(a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Panel Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
	return NULL;
}

#endif 

int CVICALLBACK Cb_Expand_Menu (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int   active_panel;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetPanelAttribute (gs_panel[PANEL], ATTR_ACTIVE, &active_panel);

			if (gs_Hide == TRUE)
				PostDeferredCall( PostDeferredDisplayPanel, NULL);
			
			else
					PostDeferredCall( PostDeferredHidePanel, NULL);
			
#if 0    /* formerly excluded lines */
			else
			{
				 if (active_panel == 0)   //caché
				 {
					PostDeferredCall( PostDeferredHidePanel, NULL);
					PostDeferredCall( PostDeferredDisplayPanel, NULL);
				 }
				 
				 else 
				 {
					SetActivePanel (gs_panel[PANEL]);  //non caché
					PostDeferredCall( PostDeferredHidePanel, NULL);
				 }
			}
#endif   /* formerly excluded lines */
			
		break;
	}
	return 0;
}
