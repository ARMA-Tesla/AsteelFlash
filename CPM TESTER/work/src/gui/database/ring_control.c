#include "dbs_global.h"
#include "dbs_product.h"
#include "dbs_product_param.h" 
#include "dbs_test.h"
#include "dbs_test_param.h"
#include "dbs_process.h"
#include "ring_control.h"

extern SGlobalPtr		global;
extern int				gs_panel[];

typedef struct _SRingGroup
{
   int32_t      panelId;
   int32_t      pictureId;         /* picture - background */ 
   int32_t      controlId;         /* control ring */
   int32_t      productId;         /* active product id for selected group */
   int32_t      productsIdSize;      /* ring items size */
   int32_t      productsId[DBS_NUMBER_GROUP_ITEMS];
   char*        productsName[DBS_NUMBER_GROUP_ITEMS];
} SRingGroup, *SRingGroupPtr;               /* group control structure */

static SRingGroup   gs_Group[DBS_NUMBER_OF_GROUPS];    /* groups control */   
static int32_t  	gs_GroupActive; 

static SElExceptionPtr RingGroupSet(int32_t control, int32_t value);
static SElExceptionPtr RingGroupFill(int32_t group, int32_t parent_id);
static SElExceptionPtr SetRingValue(int32_t panel,int32_t control, int32_t product_id);
static SElExceptionPtr ReloadActiveRingGroup(int32_t productId); 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "RingGroupIni"
SElExceptionPtr RingGroupIni(void) 
{
	SElExceptionPtr pexception = NULL;         
	int32_t			error = 0;

	gs_Group[0].panelId = gs_panel[DBS];
	gs_Group[1].panelId = gs_panel[DBS];
	gs_Group[2].panelId = gs_panel[DBS];
	gs_Group[3].panelId = gs_panel[DBS];
	gs_Group[4].panelId = gs_panel[DBS];
    
    gs_Group[0].controlId = DBS_GROUP1;
    gs_Group[1].controlId = DBS_GROUP2;
    gs_Group[2].controlId = DBS_GROUP3;
    gs_Group[3].controlId = DBS_GROUP4;
    gs_Group[4].controlId = DBS_GROUP5; 
    
    gs_Group[0].pictureId = DBS_PICTURE1;
    gs_Group[1].pictureId = DBS_PICTURE2;
    gs_Group[2].pictureId = DBS_PICTURE3;
    gs_Group[3].pictureId = DBS_PICTURE4;
    gs_Group[4].pictureId = DBS_PICTURE5;
	
	EXCCHECK( RingGroupFill(DBS_GROUP_1, 0) );
    
Error:
	EXCRETHROW(pexception); 
}


/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "RingGroupSet"
static SElExceptionPtr RingGroupSet(int32_t control, int32_t value)
{
	SElExceptionPtr pexception = NULL;    
	int32_t			error = 0;
	int				i;
	int32_t			actual_group = 0;
	int32_t			group = 0;
	
	
	switch(control)
	{
	 	case DBS_GROUP1:
	 		actual_group = 	DBS_GROUP_1;
	 		group = 		DBS_GROUP_2;
	 		break;					 
	 	case DBS_GROUP2:
	 		actual_group = 	DBS_GROUP_2;
	 		group =			DBS_GROUP_3;
	 		break;
	 	case DBS_GROUP3:
	 		actual_group = 	DBS_GROUP_3;
	 		group = 		DBS_GROUP_4;
	 		break;
	 	case DBS_GROUP4:
	 		actual_group = 	DBS_GROUP_4;
	 		group = 		DBS_GROUP_5;
	 		break;
	 	case DBS_GROUP5:
	 		actual_group = 	DBS_GROUP_5;
	 		group = 		0;
	 		break;
	}

	/* set ProductId_Tree */
	if(value)
	{	
		gs_Group[actual_group].productId = gs_Group[actual_group].productsId[value-1];
	
		for (i=actual_group+1; i<DBS_NUMBER_OF_GROUPS; i++)
		{
			gs_Group[i].productId = 0;
		}
	}
	else
	{
		for (i=actual_group; i<DBS_NUMBER_OF_GROUPS; i++)
		{
			gs_Group[i].productId = 0;
		}
	}
	/* position of active productId in ProductId_Tree */
	gs_GroupActive = 0;
	for(i=DBS_NUMBER_OF_GROUPS-1; i>=0; i--)
	{	
		if( gs_Group[i].productId != 0 )
		{
			gs_GroupActive = i;
			global->product_id = gs_Group[i].productId;
			break;
		}
		if( i == 0 )
			global->product_id = 0;	
	}
	/* fill and visible/unvisible groups items */
	if ( group > 0 )
	{
		if (value)
		{
			EXCCHECK( RingGroupFill(group, gs_Group[group-1].productId) ); 
		
			for(i=group+1; i<DBS_NUMBER_OF_GROUPS; i++)
			{
				EXCCHECKCVI( SetCtrlAttribute (gs_Group[i].panelId, gs_Group[i].controlId, ATTR_VISIBLE, 0));
				EXCCHECKCVI( SetCtrlAttribute (gs_Group[i].panelId, gs_Group[i].pictureId, ATTR_VISIBLE, 0));
				EXCCHECKCVI( SetCtrlVal (gs_Group[i].panelId, gs_Group[i].controlId, 0)); 
			}
		}
		else
		{
			for(i=group; i<DBS_NUMBER_OF_GROUPS; i++)
			{
				EXCCHECKCVI( SetCtrlAttribute (gs_Group[i].panelId, gs_Group[i].controlId, ATTR_VISIBLE, 0));
				EXCCHECKCVI( SetCtrlAttribute (gs_Group[i].panelId, gs_Group[i].pictureId, ATTR_VISIBLE, 0));
				EXCCHECKCVI( SetCtrlVal (gs_Group[i].panelId, gs_Group[i].controlId, 0)); 
			}
		}
	}
		
Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "RingGroupProcessSet"
SElExceptionPtr RingGroupProcessSet(int32_t control, int32_t value)
{
	SElExceptionPtr pexception = NULL; 
	
	EXCCHECK( RingGroupSet(control, value));
	
	/* reload process tree */
	EXCCHECK( TreeProcessIni());
	
Error:
	EXCRETHROW(pexception);    
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "RingGroupStepSet"
SElExceptionPtr RingGroupStepSet(int32_t control, int32_t value)
{
	SElExceptionPtr pexception = NULL; 
	
	EXCCHECK( RingGroupSet(control, value));
	
	/* reload test tree */
	EXCCHECK( TreeStepsIni());
	
Error:
	EXCRETHROW(pexception);    
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "RingGroupProductSet"
SElExceptionPtr RingGroupProductSet(int32_t control, int32_t value)
{
	SElExceptionPtr pexception = NULL;   
	int32_t			error = 0;
	
	EXCCHECK( RingGroupSet(control, value));
	
	/* reload test tree */
	EXCCHECK( TreeProductIni());
	
Error:
	EXCRETHROW(pexception);  
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "RingGroupFill"
static SElExceptionPtr RingGroupFill(int32_t group, int32_t parent_id)
{
    int32_t				error = 0;
	SElExceptionPtr 	pexception = NULL;
    int					index, i, count = 0; 
    SProductTypesPtr	pproduct = PPRODUCT->Product;
	STesterItemPtr		ptester = NULL;
	int32_t				branch_id =	(global->branch_id)? global->branch_id:PDBS->_branch_id;
   	
	if(global->activeUser.tester_id!=0)
		EXCCHECK( PTESTER->TesterGetSelected(PTESTER, global->activeUser.tester_id, &ptester));

	for(index=0; index<PPRODUCT->ProductSize; index++)
	{
		if( (pproduct[index].property&PROPERTY_VALID)>0  
			&& pproduct[index].parent_id == parent_id
			&& ( (parent_id==0 && (global->activeUser.tester_id==0 /* root user */
			|| (ptester && (ptester->product_id==pproduct[index].product_id || branch_id==pproduct[index].product_id)))) /* common user */  
			|| (parent_id!=0) ) )
		{
			gs_Group[group].productsId[count] = pproduct[index].product_id;
			gs_Group[group].productsName[count++] = pproduct[index].name;
		}
	}
	
	EXCCHECKCVI( ClearListCtrl (gs_Group[group].panelId, gs_Group[group].controlId));
	EXCCHECKCVI( InsertListItem (gs_Group[group].panelId, gs_Group[group].controlId, 0, "- select -", 0));
	
	for(index=0;index<count;index++)
	{
		EXCCHECKCVI( InsertListItem (gs_Group[group].panelId, 
						gs_Group[group].controlId, 
						index+1, 
						gs_Group[group].productsName[index], 
						index+1));
	}
	
	gs_Group[group].productsIdSize = index;
	
	/* visible control and background picture */
	EXCCHECKCVI( SetCtrlAttribute (gs_Group[group].panelId, gs_Group[group].controlId, ATTR_VISIBLE, (gs_Group[group].productsIdSize!=0) ));
	EXCCHECKCVI( SetCtrlAttribute (gs_Group[group].panelId, gs_Group[group].pictureId, ATTR_VISIBLE, (gs_Group[group].productsIdSize!=0) ));

	/* unvisible rest of group */
	for(i=group+1; i<DBS_NUMBER_OF_GROUPS; i++)
	{
		EXCCHECKCVI( SetCtrlAttribute (gs_Group[i].panelId, gs_Group[i].controlId, ATTR_VISIBLE, 0));
		EXCCHECKCVI( SetCtrlAttribute (gs_Group[i].panelId, gs_Group[i].pictureId, ATTR_VISIBLE, 0));
		EXCCHECKCVI( SetCtrlVal (gs_Group[i].panelId, gs_Group[i].controlId, 0)); 
	}
	
Error:
	EXCRETHROW(pexception);  
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "RingGroupAttribute"
SElExceptionPtr RingGroupAttribute(int32_t mode)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	int				i;
	int32_t			pid[32];
	int32_t			pidSize = 32;
	int32_t			control = 0;
	static int		visible_level = 0;
	int 			level;
	int				visible; 

	switch(mode)
	{
		case RING_GROUPS_LOCK:
			for(i=0; i<DBS_NUMBER_OF_GROUPS; i++)
				EXCCHECKCVI( SetCtrlAttribute (gs_panel[DBS], 
								  gs_Group[i].controlId, 
								  ATTR_CTRL_MODE, 
								  VAL_INDICATOR));
			break;
		
		case RING_GROUPS_UNLOCK:
			for(i=0; i<DBS_NUMBER_OF_GROUPS; i++)
				EXCCHECKCVI( SetCtrlAttribute (gs_panel[DBS], 
								  gs_Group[i].controlId, 
								  ATTR_CTRL_MODE, 
								  VAL_HOT));
			break;
		case RING_GROUPS_HIDE:
			
			for(level=0; level<DBS_NUMBER_OF_GROUPS; level++)
			{
				EXCCHECKCVI( GetCtrlAttribute (gs_panel[DBS], 
								  gs_Group[level].controlId, 
								  ATTR_VISIBLE, 
								  &visible));
				if(!visible)
				 	break;
			}
			
			if(level!=0)
				visible_level = level;	
			
			for(i=0; i<DBS_NUMBER_OF_GROUPS; i++)
			{
				EXCCHECKCVI( SetCtrlAttribute (gs_panel[DBS], 
								  gs_Group[i].controlId, 
								  ATTR_VISIBLE, 
								  FALSE));
				EXCCHECKCVI( SetCtrlAttribute (gs_panel[DBS], 
								  gs_Group[i].pictureId, 
								  ATTR_VISIBLE, 
								  FALSE));
			}
			break;
		case RING_GROUPS_VISIBLE:
			for(i=0; i<visible_level; i++)
			{	
				EXCCHECKCVI( SetCtrlAttribute (gs_panel[DBS], 
								  gs_Group[i].controlId, 
								  ATTR_VISIBLE, 
								  TRUE));
				EXCCHECKCVI( SetCtrlAttribute (gs_panel[DBS], 
								  gs_Group[i].pictureId, 
								  ATTR_VISIBLE, 
								  TRUE));
			}
			break;
		case RING_GROUPS_SET_PATH:
			EXCCHECK( PPRODUCT->ProductGetIdTreeUp(PPRODUCT, global->set_product_id, pid, &pidSize) );
			for(i=pidSize-1; i>=0; i--)
			{
				EXCCHECK( SetRingValue(gs_panel[DBS], control++, pid[i])); 
			}
			break;
		case RING_GROUPS_SET_INI_PATH:
			EXCCHECK( SetRingValue(gs_panel[DBS], control, 0));
			break;
		case RING_GROUPS_RELOAD:
			EXCCHECK( ReloadActiveRingGroup(global->product_id));
			break;
	}
	
Error:
	EXCRETHROW(pexception);    
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetRingValue"
static SElExceptionPtr SetRingValue(int32_t panel,int32_t control, int32_t product_id)
{
	SElExceptionPtr pexception = NULL; 
	int32_t 		error = 0;
	int32_t     	controlID = -1;
	int32_t     	index;
	
	/* ini */
	if(product_id == 0)
	{
		EXCCHECK( RingGroupSet(DBS_GROUP1, 0) );
		EXCCHECKCVI( SetCtrlVal(panel, DBS_GROUP1, 0));
	}
	else
	{
		for(index=0; index<gs_Group[control].productsIdSize; index++)
		{
			if(gs_Group[control].productsId[index] == product_id)
				break;
		}
	
		/* product_id not found */
		if(index == gs_Group[control].productsIdSize)
			EXCTHROW(-1, "PRODUCT_ID_NOT_FOUND");
	
		switch(control)
		{
			case DBS_GROUP_1:
				controlID = DBS_GROUP1;
				break;
			case DBS_GROUP_2:
				controlID = DBS_GROUP2;
				break;
			case DBS_GROUP_3:
				controlID = DBS_GROUP3;
				break;
			case DBS_GROUP_4:
				controlID = DBS_GROUP4;
				break;
			case DBS_GROUP_5:
				controlID = DBS_GROUP5;
				break;
			default:
				CHECKERR(-1); 
		}
	
		EXCCHECK( RingGroupSet(controlID, ++index) );
		EXCCHECKCVI( SetCtrlVal(panel, controlID, index));
	}
	
Error:
	EXCRETHROW(pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ReloadActiveRingGroup"
static SElExceptionPtr ReloadActiveRingGroup(int32_t productId)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	int32_t			pid[128];
	int32_t			pidSize = 128;
	
	EXCCHECK( PPRODUCT->ProductGetIdTreeUp(PPRODUCT, productId, pid, &pidSize ));
	
	if(productId==0 || pidSize==1)
	{
		EXCCHECK( RingGroupFill(DBS_GROUP_1, 0) );
	}
	else if(pidSize>1)
	{
		EXCCHECK( RingGroupFill(pidSize-1, pid[1]) );
	}
	
	global->set_product_id = productId;
	EXCCHECK( RingGroupAttribute(RING_GROUPS_SET_PATH) );
	
Error:
	EXCRETHROW(pexception);  
}
