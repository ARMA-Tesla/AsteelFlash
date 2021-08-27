#include <bklib/bklib2.h>
#include <MSComCtrl/MSComCtl2.h>
#include <utility.h>
#include <toolbox.h>
#include "dbs_global.h"
#include "ring_control.h"

extern SGlobalPtr		global;
extern int				gs_panel[];
extern int				gs_panelTab[];

#define	HIS_SHOW_PRODUCT  			(1<<0)  
#define	HIS_SHOW_PRODUCT_PARAMETER  (1<<1) 
#define	HIS_SHOW_PRODUCT_MASTER  	(1<<2)  
#define	HIS_SHOW_TEST				(1<<3)
#define	HIS_SHOW_TEST_PARAMETER   	(1<<4)
#define	HIS_SHOW_USER				(1<<5)       
#define	HIS_SHOW_USER_RIGHTS		(1<<6)   

#define BKTREE_HEADER _("root|Time^w150¦type|Type^w120¦product_id|Product Id^w200¦name|Name^w120¦action|Action^w100¦value|Value^w200¦desc|Description^w160¦user|User^w150\n")

#define HISTORY_MAX_ROWS			200

static int gs_his_filter = HIS_SHOW_PRODUCT_PARAMETER|HIS_SHOW_TEST_PARAMETER;   

HRESULT CVICALLBACK DDTPickerChangeCallback(CAObjHandle caServerObjHandle,
                                            void *caCallbackData);

typedef enum _EType
{
	NONE = 0,
	HIS_REC_PRODUCT,
	HIS_REC_PRODUCT_PARAMETER,
	HIS_REC_PRODUCT_MASTER,
	HIS_REC_TEST,
	HIS_REC_TEST_PARAMETER,
	HIS_REC_USER,
	HIS_REC_USER_RIGHTS
	
} EType;

typedef struct _SHistory
{
	time_t   	record_time;
	EType		record_type;
	void*		precord;
	int32_t		property;
	int32_t		pos;
} SHistory, *SHistoryPtr;

static SHistoryPtr		gs_phistory;    
static int32_t			gs_phistorySize;

static SElExceptionPtr HistoryCreate(void);
static SElExceptionPtr HistoryPage(int32_t page); 
static SElExceptionPtr GetTimeInt(char* time, time_t* record_time);
static SElExceptionPtr HistoryToString(SELStringPtr pelstring, SHistoryPtr phistory, int32_t phistorySize);
static SElExceptionPtr mergeHistoryToString(
	SELStringPtr pelstring, 
	SHistoryPtr phistory, 
	int32_t phistorySize, 
	SHistoryPtr phistory_all, 
	int32_t phistorySize_all
	);  
static SElExceptionPtr HistoryFilterGetTime(time_t* time);     

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnHistList"
int CVICALLBACK OnHistList (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception =  NULL;
	int32_t			error = 0;
	HRESULT			hr;
	int				value;
	int				items;
	
	switch (event)
		{
		case EVENT_COMMIT:

			switch(control)
			{
				case TABPANEL_3_PAGE:
					break;
				case TABPANEL_3_NEXT:
					EXCCHECKCVI( GetNumListItems (panel, TABPANEL_3_PAGE, &items));
					EXCCHECKCVI( GetCtrlVal (panel, TABPANEL_3_PAGE, &value)); 
					if(value==items-1)
						EXCCHECKCVI( SetCtrlIndex (panel, TABPANEL_3_PAGE, 0));
					else
						EXCCHECKCVI( SetCtrlIndex (panel, TABPANEL_3_PAGE, value+1));
					break;
				case TABPANEL_3_PREW:
					EXCCHECKCVI( GetNumListItems (panel, TABPANEL_3_PAGE, &items));
					EXCCHECKCVI( GetCtrlVal (panel, TABPANEL_3_PAGE, &value)); 
					if(value==0)
						EXCCHECKCVI( SetCtrlIndex (panel, TABPANEL_3_PAGE, items-1));
					else
						EXCCHECKCVI( SetCtrlIndex (panel, TABPANEL_3_PAGE, value-1));
					break;
			}
			EXCCHECKCVI( GetCtrlVal (panel, TABPANEL_3_PAGE, &value));
			EXCCHECK( HistoryPage(value) );
			
			EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (global->treeHandle, NULL, "0"));  
			EXCCHECKCOM( BKLib2__DBKTreeEnsureVisible (global->treeHandle, NULL, "0"));
			break;
		}
	
Error:
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::PostDeferredDisplayHistory"
void CVICALLBACK PostDeferredDisplayHistory (void *callbackData)
{
	SElExceptionPtr pexception =  NULL; 
	int32_t			error = 0;
	int				top, left; 
	
	//EXCCHECKCVI( GetCtrlAttribute (gs_panel[DBS], DBS_OUTLINE, ATTR_TOP, &top)); 
	//EXCCHECKCVI( GetCtrlAttribute (gs_panel[DBS], DBS_OUTLINE, ATTR_LEFT, &left));
	
	//EXCCHECKCVI( SetPanelAttribute (gs_panelTab[PAGE_THREE], ATTR_TOP, top));
	//EXCCHECKCVI( SetPanelAttribute (gs_panelTab[PAGE_THREE], ATTR_LEFT, left)); 
	
	EXCCHECKCVI( DisplayPanel (gs_panelTab[PAGE_THREE]));  
	
Error:
	EXCDISPLAY( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::PostDeferredHideHistory"
void CVICALLBACK PostDeferredHideHistory (void *callbackData)
{
	SElExceptionPtr pexception =  NULL; 
	int32_t			error = 0;
	
	EXCCHECKCVI( HidePanel (gs_panelTab[PAGE_THREE]));
	
Error:
	EXCDISPLAY( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TreeHistoryIni"
SElExceptionPtr TreeHistoryIni(void)
{
	SElExceptionPtr pexception =  NULL;  
	int32_t 		error = 0;
	HRESULT			hr;
	double			timer;
	
	PostDeferredCall(PostDeferredDisplayHistory, NULL);

	EXCCHECK( RingGroupAttribute(RING_GROUPS_HIDE));

	TIMER_START;
	SetWaitCursor (1);
	EXCCHECK( HistoryCreate() );
	TIMER_STOP(timer);
	
	EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (global->treeHandle, NULL, "0"));   
	EXCCHECKCOM( BKLib2__DBKTreeEnsureVisible (global->treeHandle, NULL, "0"));
	
Error:
	SetWaitCursor (0);
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TreeHistoryClose"
SElExceptionPtr TreeHistoryClose(void)
{
	SElExceptionPtr pexception =  NULL; 
	int32_t 		error = 0;
	
	PostDeferredCall(PostDeferredHideHistory, NULL);
	
	if(gs_phistory!=NULL)
	{
		free(gs_phistory);
		gs_phistory = NULL;
	}

/* Error: */
	EXCRETHROW(pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "HistoryRefresh"
static SElExceptionPtr	HistoryRefresh(void)
{
	SElExceptionPtr pexception =  NULL;    
	int32_t			error = 0;
	HRESULT			hr;
	double			timer;
	
	TIMER_START;
	SetWaitCursor (1);
	EXCCHECK( HistoryCreate() );
	TIMER_STOP(timer);
	
	EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (global->treeHandle, NULL, "0"));   
	EXCCHECKCOM( BKLib2__DBKTreeEnsureVisible (global->treeHandle, NULL, "0"));
	
Error:
	SetWaitCursor (0);  
	EXCRETHROW(pexception);    
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::DDTPickerChangeCallback"
HRESULT CVICALLBACK DDTPickerChangeCallback(CAObjHandle caServerObjHandle,
                                            void *caCallbackData)
{
	SElExceptionPtr pexception =  NULL;   
	int32_t 		error = 0;

	EXCCHECK( HistoryRefresh() );
	
Error:	
	EXCDISPLAY( pexception);
	return 0;  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "HistoryClear"
SElExceptionPtr HistoryClear(void)
{
	SElExceptionPtr pexception =  NULL;
	int32_t 		error = 0;

	if(gs_phistory!=NULL)
	{
		free(gs_phistory);
		gs_phistory = NULL;
	}

/* Error: */
	EXCRETHROW(pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnHisFillter"
int CVICALLBACK OnHisFillter (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception =  NULL;  
	int32_t			error = 0;
	int 			value;
	
	switch (event)
		{
		case EVENT_COMMIT:
			
			gs_his_filter = 0;
			
			EXCCHECKCVI( GetCtrlVal(gs_panelTab[PAGE_THREE], TABPANEL_3_SWITCH_PROD, &value));  
			if(!value) 
				gs_his_filter |= HIS_SHOW_PRODUCT;
			
			GetCtrlVal(gs_panelTab[PAGE_THREE], TABPANEL_3_SWITCH_PRODPARAM, &value); 
			if(!value)
				gs_his_filter |= HIS_SHOW_PRODUCT_PARAMETER;
			
			GetCtrlVal(gs_panelTab[PAGE_THREE], TABPANEL_3_SWITCH_PRODMASTER, &value); 
			if(!value)
				gs_his_filter |= HIS_SHOW_PRODUCT_MASTER;
			
			GetCtrlVal(gs_panelTab[PAGE_THREE], TABPANEL_3_SWITCH_TEST, &value);  
			if(!value)
				gs_his_filter |= HIS_SHOW_TEST;
			
			GetCtrlVal(gs_panelTab[PAGE_THREE], TABPANEL_3_SWITCH_TESTPARAM, &value);   
			if(!value)
				gs_his_filter |= HIS_SHOW_TEST_PARAMETER;
			
			GetCtrlVal(gs_panelTab[PAGE_THREE], TABPANEL_3_SWITCH_USER, &value);   
			if(!value)
				gs_his_filter |= HIS_SHOW_USER;
		
			GetCtrlVal(gs_panelTab[PAGE_THREE], TABPANEL_3_SWITCH_USERRIGHTS, &value);   
			if(!value)
				gs_his_filter |= HIS_SHOW_USER_RIGHTS;
			
			EXCCHECK( HistoryRefresh() );  
			
			break;
		}
	
Error:
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnHisMerge"
int CVICALLBACK OnHisMerge (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception =  NULL;   
	int32_t			error = 0;
	
	switch (event)
		{
		case EVENT_COMMIT:
		    EXCCHECKCVI( GetCtrlVal(panel, control, &global->HistoryMerge)); 
			
			EXCCHECK( HistoryRefresh() );  
			break;
		}
	
Error:
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "HistoryFilterSetCurrentTime"
SElExceptionPtr	HistoryFilterSetCurrentTime(void)
{
	SElExceptionPtr pexception =  NULL;   
	int32_t			error = 0;
	int				month, day, year, hour, minutes, seconds;
	VARIANT			variant;
	
	EXCCHECKCVI( GetSystemDate (&month, &day, &year));
	EXCCHECKCVI( GetSystemTime (&hour, &minutes, &seconds));
	
	variant = CA_VariantInt (year);
	MSComCtl2_IDTPickerSetYear(global->pickerHandle, NULL, variant);  
	variant = CA_VariantInt (month); 
	MSComCtl2_IDTPickerSetMonth (global->pickerHandle, NULL, variant); 
	variant = CA_VariantInt (day); 
	MSComCtl2_IDTPickerSetDay (global->pickerHandle, NULL, variant);
	
	//database item
	variant = CA_VariantInt (year);
	MSComCtl2_IDTPickerSetYear(global->hpicDayHandle, NULL, variant);  
	variant = CA_VariantInt (month); 
	MSComCtl2_IDTPickerSetMonth (global->hpicDayHandle, NULL, variant);
	variant = CA_VariantInt (day); 
	MSComCtl2_IDTPickerSetDay (global->hpicDayHandle, NULL, variant);
	
	variant = CA_VariantInt (hour);       
	MSComCtl2_IDTPickerSetHour (global->hpicHourHandle, NULL, variant);
	variant = CA_VariantInt (minutes);       
	MSComCtl2_IDTPickerSetMinute (global->hpicHourHandle, NULL, variant); 
	variant = CA_VariantInt (seconds);       
	MSComCtl2_IDTPickerSetSecond (global->hpicHourHandle, NULL, variant);    
	
Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/	
#undef __FUNC__
#define __FUNC__ "HistoryFilterGetTime"
static SElExceptionPtr	HistoryFilterGetTime(time_t* time)
{
	SElExceptionPtr pexception =  NULL;  
	int32_t			error = 0;
	int16_t			value;
	VARIANT			vmonth, vday, vyear;
	struct tm 		date;
	
	MSComCtl2_IDTPickerGetDay (global->pickerHandle, NULL, &vday);
	MSComCtl2_IDTPickerGetMonth (global->pickerHandle, NULL, &vmonth); 
	MSComCtl2_IDTPickerGetYear(global->pickerHandle, NULL, &vyear);  

	CA_VariantGetShort (&vday, &value);
	date.tm_mday = value;
	CA_VariantGetShort (&vmonth, &value);
	date.tm_mon = value;
	CA_VariantGetShort (&vyear, &value);
	date.tm_year = value;
	
	date.tm_year -= 1900;
	date.tm_hour = 0;
	date.tm_min = 0; 
	date.tm_sec = 0;   
	date.tm_isdst = 0;
	date.tm_wday = 0;
	date.tm_yday = 0;
	
	*time = mktime( &date );
	
/* Error: */
	EXCRETHROW(pexception); 
}

/*---------------------------------------------------------------------------*/
int compare_history ( const void* a, const void* b ) 
{
	SHistoryPtr aa, bb;
	
	aa = (SHistoryPtr)a;
	bb = (SHistoryPtr)b;
	
	if(bb->record_time != aa->record_time)
	{
		return (int)(bb->record_time - aa->record_time);
	}
	else
	{
		if(aa->property & PROPERTY_CREATED)
			return -1;
		else if(bb->property & PROPERTY_CREATED)
			return 1;
		else
			return 0;
	}
}		

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "FilePage"
static SElExceptionPtr	FilePage(int32_t phistorySize)
{
	SElExceptionPtr pexception =  NULL;      
	int32_t			error = 0;
	div_t			temp;
	int				i;
	char			name[128];
	
	temp = div(phistorySize, HISTORY_MAX_ROWS);
	
	EXCCHECKCVI( ClearListCtrl (gs_panelTab[PAGE_THREE], TABPANEL_3_PAGE)); 
	
	for(i=0; i<temp.quot+1; i++)
	{
		sprintf(name, "%02d/%02d", i+1, temp.quot+1);
		EXCCHECKCVI( InsertListItem (gs_panelTab[PAGE_THREE], TABPANEL_3_PAGE, -1, name, i));
	}
		
Error:
	EXCRETHROW(pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "HistoryPage"
static SElExceptionPtr HistoryPage(int32_t page)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL; 
	HRESULT			hr;
	SHistoryPtr 	phistory_select = NULL;      
	SELStringPtr   	pelstring = global->pelstring;  
	short int		AXvalue;   
	
	SHistoryPtr		pdata_start = NULL;
	int32_t			copy_block = 0;
	int32_t			rest;
	
	if(gs_phistory!=NULL) 
	{
		rest = gs_phistorySize - ( page*HISTORY_MAX_ROWS ); 
		copy_block = (HISTORY_MAX_ROWS<rest)? HISTORY_MAX_ROWS:rest;
	
		pdata_start = gs_phistory + ( page*HISTORY_MAX_ROWS ); 
		phistory_select = calloc(HISTORY_MAX_ROWS, sizeof(SHistory));
		memcpy(phistory_select, pdata_start, copy_block*sizeof(SHistory)); 
	}
	
	if(global->HistoryMerge)
	{
		EXCCHECK( mergeHistoryToString(pelstring, 
									phistory_select, 
									copy_block,
									gs_phistory,
									gs_phistorySize) 
									); 
	}
	else
	{
		EXCCHECK( HistoryToString(pelstring, 
									phistory_select, 
									copy_block) 
									);       
	}
	
	EXCCHECKCOM( BKLib2__DBKTreeFromString (global->treeHandle, NULL, pelstring->GetPtr(pelstring), "¦", "\n", &AXvalue));
	EXCCHECKCOM( BKLib2__DBKTreeSetEnableDragAndDrop (global->treeHandle, NULL, VFALSE));
	EXCCHECKCOM( BKLib2__DBKTreeExpandLevel (global->treeHandle, NULL, 1, &AXvalue));
	
	EXCCHECK( pelstring->Empty(pelstring) );    
	
Error:
	free(phistory_select); 
	EXCRETHROW(pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "HistoryCreate"
static SElExceptionPtr	HistoryCreate(void)
{
	SElExceptionPtr 		pexception = NULL; 
	int32_t					error = 0;
	int						i, j;
	time_t					time_set, time;
	#ifndef _NO_TEST
	STestStepPtr			ptest = PTEST->Test;
	STestParameterPtr		pparameter = PTESTPARAM->TestParameter;	
	#endif
	SProductParameterPtr 	pprodparameter = PPARAM->ProductParameter;
	#ifndef _NO_MASTER
	SProductMasterPtr		pprodmaster = PMASTER->ProductMaster;
	#endif
	SProductTypesPtr 		pprod = PPRODUCT->Product;
	SDBSUserPtr				puser = PUSER->User;
	
	SHistoryPtr				phistory = NULL;
	int32_t					phistorySize = 0;
	SHistoryPtr				pdata = NULL;
	
	EXCCHECK( HistoryFilterGetTime(&time_set) );
	
	/* precount */
	if((gs_his_filter&HIS_SHOW_PRODUCT)>0) 
		for(i=0; i<PPRODUCT->ProductSize; i++)
		{
			GetTimeInt(pprod[i].time, &time);
			if(time > time_set)
				phistorySize++;	
		}
	if((gs_his_filter&HIS_SHOW_PRODUCT_PARAMETER)>0) 
		for(i=0; i<PPARAM->ProductParameterSize; i++)
		{
			GetTimeInt(pprodparameter[i].time, &time);
			if(time > time_set)
				phistorySize++;	
		}
	#ifndef _NO_MASTER
	if((gs_his_filter&HIS_SHOW_PRODUCT_MASTER)>0) 
		for(i=0; i<PMASTER->ProductMasterSize; i++)
		{
			GetTimeInt(pprodmaster[i].time, &time);
			if(time > time_set)
				phistorySize++;	
		}
	#endif
	
	#ifndef _NO_TEST
	if((gs_his_filter&HIS_SHOW_TEST)>0)
		for(i=0; i<PTEST->TestSize; i++)
		{
			GetTimeInt(ptest[i].time, &time);
			if(time > time_set)
				phistorySize++;	
		}
	if((gs_his_filter&HIS_SHOW_TEST_PARAMETER)>0)   
		for(i=0; i<PTESTPARAM->TestParameterSize; i++)
		{
			GetTimeInt(pparameter[i].time, &time);
			if(time > time_set)
				phistorySize++;	
		}
	#endif
	if((gs_his_filter&HIS_SHOW_USER)>0)   
		for(i=0; i<PUSER->UserSize; i++)
		{
			GetTimeInt(puser[i].time, &time);
			if(time > time_set)
				phistorySize++;	
		}
	if((gs_his_filter&HIS_SHOW_USER_RIGHTS)>0)   
		for(i=0; i<PUSER->UserSize; i++)
		{
			for(j=0; j<puser[i].RightsSize; j++)
			{
				if( (puser[i].property&PROPERTY_VALID)>0 )
				{
					GetTimeInt(puser[i].Rights[j].time, &time);
					if(time > time_set)
						phistorySize++;	
				}
			}
		}

	phistory = calloc(phistorySize, sizeof(SHistory));
	pdata = &phistory[0];
	
	if((gs_his_filter&HIS_SHOW_PRODUCT)>0)    
		for(i=0; i<PPRODUCT->ProductSize; i++)
		{
			GetTimeInt(pprod[i].time, &time);
			if(time > time_set)
			{	
				pdata->record_type = HIS_REC_PRODUCT;
				pdata->precord= (void*)&pprod[i];
				pdata->property = pprod[i].property;
				pdata->record_time = time;
				pdata++;
			}
		}
	if((gs_his_filter&HIS_SHOW_PRODUCT_PARAMETER)>0)   
		for(i=0; i<PPARAM->ProductParameterSize; i++)
		{
			GetTimeInt(pprodparameter[i].time, &time);
			if(time > time_set)
			{	
				pdata->record_type = HIS_REC_PRODUCT_PARAMETER;
				pdata->precord= (void*)&pprodparameter[i];
				pdata->property = pprodparameter[i].property;
				pdata->record_time = time;
				pdata++;
			}
		}
	#ifndef _NO_MASTER
	if((gs_his_filter&HIS_SHOW_PRODUCT_MASTER)>0)   
		for(i=0; i<PMASTER->ProductMasterSize; i++)
		{
			GetTimeInt(pprodmaster[i].time, &time);
			if(time > time_set)
			{	
				pdata->record_type = HIS_REC_PRODUCT_MASTER;
				pdata->precord= (void*)&pprodmaster[i];
				pdata->property = pprodmaster[i].property;
				pdata->record_time = time;
				pdata++;
			}
		}
	#endif
	#ifndef _NO_TEST
	if((gs_his_filter&HIS_SHOW_TEST)>0)   
		for(i=0; i<PTEST->TestSize; i++)
		{
			GetTimeInt(ptest[i].time, &time);
			if(time > time_set)
			{	
				pdata->record_type = HIS_REC_TEST;
				pdata->precord= (void*)&ptest[i];
				pdata->property = ptest[i].property;
				pdata->record_time = time;
				pdata++;
			}
		}
	if((gs_his_filter&HIS_SHOW_TEST_PARAMETER)>0)
		for(i=0; i<PTESTPARAM->TestParameterSize; i++)
		{
			GetTimeInt(pparameter[i].time, &time);
			if(time > time_set)
			{	
				pdata->record_type = HIS_REC_TEST_PARAMETER;
				pdata->precord= (void*)&pparameter[i];
				pdata->property = pparameter[i].property;
				pdata->record_time = time;
				pdata++;
			}
		}
	#endif
	if((gs_his_filter&HIS_SHOW_USER)>0)
		for(i=0; i<PUSER->UserSize; i++)
		{
			GetTimeInt(puser[i].time, &time);
			if(time > time_set)
			{	
				pdata->record_type = HIS_REC_USER;
				pdata->precord= (void*)&puser[i];
				pdata->property = puser[i].property;
				pdata->record_time = time;
				pdata++;
			}
		}
	if((gs_his_filter&HIS_SHOW_USER_RIGHTS)>0)
		for(i=0; i<PUSER->UserSize; i++)
		{
			for(j=0; j<puser[i].RightsSize; j++)
			{
				if( (puser[i].property&PROPERTY_VALID)>0 )
				{
					GetTimeInt(puser[i].Rights[j].time, &time);
					if(time > time_set)
					{	
						pdata->record_type = HIS_REC_USER_RIGHTS;
						pdata->precord= (void*)&puser[i];
						pdata->property = puser[i].property;
						pdata->record_time = time;
						pdata->pos = j;
						pdata++;
					}
				}
			}
		}
	
	/* sort array according to time */
	qsort(phistory, phistorySize, sizeof(SHistory), compare_history);

	if(gs_phistory!=NULL)
	{
		free(gs_phistory);
		gs_phistory = NULL;
	}
	gs_phistory = phistory;
	gs_phistorySize = phistorySize;
	
	EXCCHECK( FilePage(gs_phistorySize) );   
	EXCCHECK( HistoryPage(0) );
	
Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetTimeInt"
static SElExceptionPtr GetTimeInt(char* time, time_t* record_time)
{
	SElExceptionPtr pexception = NULL;   
	int32_t 		error = 0;
	struct tm 		date;
	
	sscanf(time, "%d-%02d-%02d %02d:%02d:%02d", &date.tm_year, &date.tm_mon, &date.tm_mday, &date.tm_hour, &date.tm_min, &date.tm_sec);
	date.tm_year -= 1900;
	date.tm_isdst = 0;
	date.tm_wday = 0;
	date.tm_yday = 0;
	
	*record_time = mktime( &date );
	
/* Error: */
	EXCRETHROW(pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "HistoryToStringTest"
static SElExceptionPtr HistoryToStringTest(
	int32_t			sequence,
	int32_t			sequence2,
	SELStringPtr 	pelstring, 
	STestStepPtr 	ptest 
)
{
	int32_t 		error = 0;
	SElExceptionPtr	pexception = NULL;  
	char	    	path[256] = "";    
	char*			name = NULL;    
	char			action[256];     
	char			row[1024] = ""; 
	SDBSUserPtr		user = NULL;       
	
	EXCCHECK( PUSER->UserGetFromId(PUSER, ptest->user_id, &user) ); 
	EXCDBGSTR( PPRODUCT->ProductGetIdPath(PPRODUCT, ptest->product_id, path, PRODUCT_PATH_LEVEL_2) );
	
	if(0==strcmp(ptest->name, "@skip") )
	{
		EXCCHECK( PTEST->TestGetName(PTEST, ptest->test_nb, &name) );
		strcpy( action, name);
	
		if((ptest->property&PROPERTY_CREATED) >0)
			strcat( action, _("¦Skip"));
		if((ptest->property&PROPERTY_DELETED) >0)
			strcat( action, _("¦Skip Remove"));
	}
	else
	{
		strcpy( action, ptest->name);  
		
		if((ptest->property&PROPERTY_CREATED) >0)
			strcat( action, _("¦Create"));
		if((ptest->property&PROPERTY_EDITED) >0)
			strcat( action, _("¦Edit"));
		if((ptest->property&PROPERTY_DELETED) >0)
			strcat( action, _("¦Del"));
		if((ptest->property&PROPERTY_MOVED) >0)
			strcat( action, _("¦Move"));
	}
	
	if(sequence2==-1)
		sprintf(row, "|%d|%s¦%s¦%s¦%s¦%s¦%s¦%s\n",
								  sequence,
								  ptest->time,
								  "Test",
								  path,
								  action,
								  "",
								  GET_STRING(ptest->description),
								  (user!=NULL)? user->name:"unknown"
								  );
	if(sequence2!=-1)
		sprintf(row, "%d|%d_%d|%s¦%s¦%s¦%s¦%s¦%s¦%s\n",
								  sequence,
								  sequence,
								  sequence2,
								  ptest->time,
								  "Test",
								  path,
								  action,
								  "",
								  GET_STRING(ptest->description),
								  (user!=NULL)? user->name:"unknown" 
								  );
	
	
	EXCCHECK( pelstring->Append( pelstring, row));   
	
Error:
	EXCRETHROW(pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "HistoryToStringTestParameter"
static SElExceptionPtr HistoryToStringTestParameter(
	int32_t			sequence,
	int32_t			sequence2,  
	SELStringPtr 	pelstring, 
	STestParameterPtr	pparameter 
)
{
	int32_t 		error = 0;
	SElExceptionPtr	pexception = NULL;  
	char	    	path[256] = "";    
	char			action[512];     
	char			row[1024]=""; 
	char*			stestnb = NULL; 
	SDBSUserPtr		user = NULL;       
	
	EXCCHECK( PUSER->UserGetFromId(PUSER, pparameter->user_id, &user) ); 
	EXCDBGSTR( PPRODUCT->ProductGetIdPath(PPRODUCT, pparameter->product_id, path, PRODUCT_PATH_LEVEL_2) );
	EXCCHECK( PTEST->TestGetName(PTEST, pparameter->test_nb, &stestnb) ); 
	
	sprintf( action, "%s\\%s", (stestnb)? stestnb:"none", pparameter->name);  
	
	if((pparameter->property&PROPERTY_CREATED) >0)
		strcat( action, _("¦Create"));
	if((pparameter->property&PROPERTY_EDITED) >0)
		strcat( action, _("¦Edit"));
	if((pparameter->property&PROPERTY_DELETED) >0)
		strcat( action, _("¦Del"));
	
	if(sequence2==-1)   
		sprintf(row, "|%d|%s¦%s¦%s¦%s¦%s¦%s¦%s\n",
								  sequence,
								  pparameter->time,
								  "Test Parameter",
								  path,
								  action,
								  pparameter->value,
								  GET_STRING(pparameter->description),
								  (user!=NULL)? user->name:"unknown"
								  );
	if(sequence2!=-1)   
		sprintf(row, "%d|%d_%d|%s¦%s¦%s¦%s¦%s¦%s¦%s\n",
								  sequence,
								  sequence,
								  sequence2,
								  pparameter->time,
								  "Test Parameter",
								  path,
								  action,
								  pparameter->value,
								  GET_STRING(pparameter->description),
								  (user!=NULL)? user->name:"unknown" 
								  );
	
	
	EXCCHECK(pelstring->Append( pelstring, row));

Error:
	EXCRETHROW(pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "HistoryToStringProductParameter"
static SElExceptionPtr HistoryToStringProductParameter(
	int32_t			sequence,
	int32_t			sequence2,  
	SELStringPtr 	pelstring, 
	SProductParameterPtr	pprodparameter 
)
{
	int32_t 		error = 0;
	SElExceptionPtr	pexception = NULL;  
	char	    	path[256] = "";    
	char			action[256];     
	char			row[1024]=""; 
	SDBSUserPtr		user = NULL;       
	
	EXCCHECK( PUSER->UserGetFromId(PUSER, pprodparameter->user_id, &user) ); 	
	EXCDBGSTR( PPRODUCT->ProductGetIdPath(PPRODUCT, pprodparameter->product_id, path, PRODUCT_PATH_LEVEL_2) );   
	
	strcpy( action, pprodparameter->name);    
	
	if(pprodparameter->tester_id == 0)
	{
		if((pprodparameter->property&PROPERTY_CREATED) >0)
			strcat( action, _("¦Create"));
		if((pprodparameter->property&PROPERTY_EDITED) >0)
			strcat( action, _("¦Edit"));
		if((pprodparameter->property&PROPERTY_DELETED) >0)
			strcat( action, _("¦Del"));
	}
	else
	{
		if((pprodparameter->property&PROPERTY_CREATED) >0)
			strcat( action, _("¦Create Specific"));
		if((pprodparameter->property&PROPERTY_EDITED) >0)
			strcat( action, _("¦Edit Specific"));
		if((pprodparameter->property&PROPERTY_DELETED) >0)
			strcat( action, _("¦Del Specific"));
	}
	
	if(sequence2==-1) 
		sprintf(row, "|%d|%s¦%s¦%s¦%s¦%s¦%s¦%s\n",
								  sequence,
								  pprodparameter->time,
								  "Product Parameter",
								  path,
								  action,
								  pprodparameter->value,
								  GET_STRING(pprodparameter->description),
								  (user!=NULL)? user->name:"unknown"
								  );
	if(sequence2!=-1) 
		sprintf(row, "%d|%d_%d|%s¦%s¦%s¦%s¦%s¦%s¦%s\n",
								  sequence,
								  sequence,
								  sequence2,
								  pprodparameter->time,
								  "Product Parameter",
								  path,
								  action,
								  pprodparameter->value,
								  GET_STRING(pprodparameter->description),
								  (user!=NULL)? user->name:"unknown" 
								  );
	
	
	EXCCHECK(pelstring->Append( pelstring, row));

Error:
	EXCRETHROW(pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "HistoryToStringProductMaster"
static SElExceptionPtr HistoryToStringProductMaster(
	int32_t			sequence,
	int32_t			sequence2,  
	SELStringPtr 	pelstring, 
	SProductMasterPtr	pprodmaster 
)
{
	int32_t 		error = 0;
	SElExceptionPtr	pexception = NULL;    
	char	    	path[256] = "";    
	char*			name = "none";  
	char			action[256];     
	char			row[1024]=""; 
	SDBSUserPtr		user = NULL;       
	
	EXCCHECK( PUSER->UserGetFromId(PUSER, pprodmaster->user_id, &user) ); 	
	EXCDBGSTR( PPRODUCT->ProductGetIdPath(PPRODUCT, pprodmaster->product_id, path, PRODUCT_PATH_LEVEL_2) );
	
	if(pprodmaster->master_id)
		EXCCHECK( PPRODUCT->ProductGetIdName(PPRODUCT, pprodmaster->master_id, &name) );  
	
	strcpy( action, name);    
	
	if((pprodmaster->property&PROPERTY_CREATED) >0)
		strcat( action, _("¦Create"));
	if((pprodmaster->property&PROPERTY_EDITED) >0)
		strcat( action, _("¦Edit"));
	if((pprodmaster->property&PROPERTY_DELETED) >0)
		strcat( action, _("¦Del"));
	
	if(sequence2==-1) 
		sprintf(row, "|%d|%s¦%s¦%s¦%s¦%s¦%s¦%s\n",
								  sequence,
								  pprodmaster->time,
								  "Product Master",
								  path,
								  action,
								  (0==strcmp(pprodmaster->master,"@empty"))? _("empty"):pprodmaster->master,
								  "",  //description
								  (user!=NULL)? user->name:"unknown"
								  );
	if(sequence2!=-1) 
		sprintf(row, "%d|%d_%d|%s¦%s¦%s¦%s¦%s¦%s¦%s\n",
								  sequence,
								  sequence,
								  sequence2,
								  pprodmaster->time,
								  "Product Master",
								  path,
								  action,
								  (0==strcmp(pprodmaster->master,"@empty"))? _("empty"):pprodmaster->master,
								  "",  //description   
								  (user!=NULL)? user->name:"unknown" 
								  );
	
	
	EXCCHECK(pelstring->Append( pelstring, row));

Error:
	EXCRETHROW(pexception);    
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "HistoryToStringProduct"
static SElExceptionPtr HistoryToStringProduct(
	int32_t			sequence,
	int32_t			sequence2,  
	SELStringPtr 	pelstring, 
	SProductTypesPtr	pprod 
)
{
	int32_t 		error = 0;
	SElExceptionPtr	pexception = NULL;
	char	    	path[256] = "";    
	char			action[256];     
	char			row[1024]=""; 
	SDBSUserPtr		user = NULL;       
	
	EXCCHECK( PUSER->UserGetFromId(PUSER, pprod->user_id, &user) ); 
	EXCDBGSTR( PPRODUCT->ProductGetIdPath(PPRODUCT, pprod->product_id, path, PRODUCT_PATH_LEVEL_2) );
	
	strcpy( action, pprod->name);  
	
	if((pprod->property&PROPERTY_CREATED) >0)
		strcat( action, _("¦Create"));
	if((pprod->property&PROPERTY_EDITED) >0)
		strcat( action, _("¦Edit"));
	if((pprod->property&PROPERTY_DELETED) >0)
		strcat( action, _("¦Del"));
	
	if(sequence2==-1)   
		sprintf(row, "|%d|%s¦%s¦%s¦%s¦%s¦%s¦%s\n",
								  sequence,
								  pprod->time,
								  "Product",
								  path,
								  action,
								  "",
								  GET_STRING(pprod->description),
								  (user!=NULL)? user->name:"unknown"
								  );
	if(sequence2!=-1)   
		sprintf(row, "%d|%d_%d|%s¦%s¦%s¦%s¦%s¦%s¦%s\n",
								  sequence,
								  sequence,
								  sequence2,
								  pprod->time,
								  "Product",
								  path,
								  action,
								  "",
								  GET_STRING(pprod->description),
								  (user!=NULL)? user->name:"unknown"
								  );
	
	
	EXCCHECK(pelstring->Append( pelstring, row)); 

Error:
	EXCRETHROW(pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "HistoryToStringUser"
static SElExceptionPtr HistoryToStringUser(
	int32_t			sequence,
	int32_t			sequence2,  
	SELStringPtr 	pelstring, 
	SDBSUserPtr		puser 
)
{
	int32_t 		error = 0;
	SElExceptionPtr	pexception = NULL;
	char	    	path[256] = "";    
	char			action[256];     
	char			row[1024]=""; 
	SDBSUserPtr		user = NULL;       
	
	EXCCHECK( PUSER->UserGetFromId(PUSER, puser->admin_id, &user) ); 
	
	strcpy( action, puser->name);  
	
	if((puser->property&PROPERTY_CREATED) >0)
		strcat( action, _("¦Create"));
	if((puser->property&PROPERTY_EDITED) >0)
		strcat( action, _("¦Edit"));
	if((puser->property&PROPERTY_DELETED) >0)
		strcat( action, _("¦Del"));
	
	if(sequence2==-1)   
		sprintf(row, "|%d|%s¦%s¦%s¦%s¦%s¦%s¦%s\n",
								  sequence,
								  puser->time,
								  "User",
								  "",
								  action,
								  "",
								  "",
								  (user!=NULL)? user->name:"unknown"
								  );
	if(sequence2!=-1)   
		sprintf(row, "%d|%d_%d|%s¦%s¦%s¦%s¦%s¦%s¦%s\n",
								  sequence,
								  sequence,
								  sequence2,
								  puser->time,
								  "User",
								  "",
								  action,
								  "",
								  "",
								  (user!=NULL)? user->name:"unknown"
								  );
	
	
	EXCCHECK(pelstring->Append( pelstring, row)); 

Error:
	EXCRETHROW(pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "HistoryToStringUserRights"
static SElExceptionPtr HistoryToStringUserRights(
	int32_t			sequence,
	int32_t			sequence2,  
	SELStringPtr 	pelstring, 
	SDBSUserPtr		puser,
	int32_t			pos
)
{
	int32_t 		error = 0;
	SElExceptionPtr	pexception = NULL;
	char	    	path[256] = "root";    
	char			action[256];     
	char			row[1024]=""; 
	SDBSUserPtr		user = NULL;      
	
	EXCCHECK( PUSER->UserGetFromId(PUSER, puser->Rights[pos].admin_id, &user) ); 
	
	if( puser->Rights[pos].tester_id )
		EXCDBGSTR( PPRODUCT->ProductGetIdPath(PPRODUCT, puser->Rights[pos].tester_id, path, PRODUCT_PATH_LEVEL_1)); 
	
	strcpy( action, puser->name);  
	
	if((puser->Rights[pos].property&PROPERTY_CREATED) >0)
		strcat( action, _("¦Create"));
	if((puser->Rights[pos].property&PROPERTY_EDITED) >0)
		strcat( action, _("¦Edit"));
	if((puser->Rights[pos].property&PROPERTY_DELETED) >0)
		strcat( action, _("¦Del"));
	
	if(sequence2==-1)   
		sprintf(row, "|%d|%s¦%s¦%s¦%s¦%s¦%s¦%s\n",
								  sequence,
								  puser->Rights[pos].time,
								  "User Rights",
								  path,
								  action,
								  "",
								  "",
								  (user!=NULL)? user->name:"unknown"
								  );
	if(sequence2!=-1)   
		sprintf(row, "%d|%d_%d|%s¦%s¦%s¦%s¦%s¦%s¦%s\n",
								  sequence,
								  sequence,
								  sequence2,
								  puser->Rights[pos].time,
								  "User Rights",
								  path,
								  action,
								  "",
								  "",
								  (user!=NULL)? user->name:"unknown"
								  );
	
	
	EXCCHECK(pelstring->Append( pelstring, row)); 

Error:
	EXCRETHROW(pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "HistoryToString"
static SElExceptionPtr HistoryToString(
	SELStringPtr 	pelstring, 
	SHistoryPtr 	phistory, 
	int32_t 		phistorySize
)
{
	int32_t 		error = 0;
	SElExceptionPtr pexception = NULL; 
	int				i;
	
	EXCCHECK( pelstring->Empty(pelstring) );
	EXCCHECK( pelstring->Append(pelstring, BKTREE_HEADER) );
	
	for(i=0; i<phistorySize; i++)
	{
		if( (gs_his_filter&HIS_SHOW_PRODUCT)>0 
			&& phistory[i].record_type==HIS_REC_PRODUCT) 
		{
			SProductTypesPtr	pprod = (SProductTypesPtr)phistory[i].precord; 
			
			EXCCHECK( HistoryToStringProduct(i, -1, pelstring, pprod) );     
		}	
		
		if( (gs_his_filter&HIS_SHOW_PRODUCT_PARAMETER)>0 
			&& phistory[i].record_type==HIS_REC_PRODUCT_PARAMETER) 
		{
			SProductParameterPtr	pprodparameter = (SProductParameterPtr)phistory[i].precord;   
			
			EXCCHECK( HistoryToStringProductParameter(i, -1, pelstring, pprodparameter) );
		}
		
		if( (gs_his_filter&HIS_SHOW_PRODUCT_MASTER)>0 
			&& phistory[i].record_type==HIS_REC_PRODUCT_MASTER) 
		{
			SProductMasterPtr	pprodmaster = (SProductMasterPtr)phistory[i].precord;   
			
			EXCCHECK( HistoryToStringProductMaster(i, -1, pelstring, pprodmaster) );
		}
		
		if( (gs_his_filter&HIS_SHOW_TEST)>0
			&& phistory[i].record_type==HIS_REC_TEST)
		{
			STestStepPtr 	ptest = (STestStepPtr)phistory[i].precord;    
			
			EXCCHECK( HistoryToStringTest(i, -1, pelstring, ptest) ); 
		}
		
		if( (gs_his_filter&HIS_SHOW_TEST_PARAMETER)>0 
			&& phistory[i].record_type==HIS_REC_TEST_PARAMETER) 
		{
			STestParameterPtr	pparameter = (STestParameterPtr)phistory[i].precord;   
			
			EXCCHECK( HistoryToStringTestParameter(i, -1, pelstring, pparameter) );
		}
		
		if( (gs_his_filter&HIS_SHOW_USER)>0 
			&& phistory[i].record_type==HIS_REC_USER) 
		{
			SDBSUserPtr	puser = (SDBSUserPtr)phistory[i].precord;   
			
			EXCCHECK( HistoryToStringUser(i, -1, pelstring, puser) );
		}
		
		if( (gs_his_filter&HIS_SHOW_USER_RIGHTS)>0 
			&& phistory[i].record_type==HIS_REC_USER_RIGHTS) 
		{
			SDBSUserPtr	puser = (SDBSUserPtr)phistory[i].precord;   
			
			EXCCHECK( HistoryToStringUserRights(i, -1, pelstring, puser, phistory[i].pos) );
		}
	}
	
Error:
	EXCRETHROW(pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "mergeHistoryToString"
static SElExceptionPtr mergeHistoryToString(
	SELStringPtr 	pelstring, 
	SHistoryPtr 	phistory, 
	int32_t 		phistorySize,
	SHistoryPtr 	phistory_all, 
	int32_t 		phistorySize_all
	
)
{
	int32_t 		error = 0;
	SElExceptionPtr pexception = NULL; 
	int				i, j;
	
	EXCCHECK( pelstring->Empty(pelstring) );
	EXCCHECK( pelstring->Append(pelstring, BKTREE_HEADER) );
	
	
	for(i=0; i<phistorySize; i++)
	{
		if( (gs_his_filter&HIS_SHOW_PRODUCT)>0 
			&& phistory[i].record_type==HIS_REC_PRODUCT) 
		{
			SProductTypesPtr	pprod = (SProductTypesPtr)phistory[i].precord; 
			
			if( (pprod->property&PROPERTY_VALID)>0 
				|| (pprod->property&PROPERTY_DELETED)>0 )
			{
				EXCCHECK( HistoryToStringProduct(i, -1, pelstring, pprod) ); 				
				
				if( (pprod->property&PROPERTY_CREATED)==0 )
				{
					for(j=0; j<phistorySize_all; j++)
					{
						if( phistory_all[j].record_type==HIS_REC_PRODUCT 
							&& phistory_all[j].record_time < phistory[i].record_time)
						{
							SProductTypesPtr	pprod2 = (SProductTypesPtr)phistory_all[j].precord; 
						
							if( (pprod2->property&PROPERTY_VALID)==0
								&&pprod2->product_id == pprod->product_id )
							{
								EXCCHECK( HistoryToStringProduct(i, j, pelstring, pprod2) ); 
								
								if( (pprod2->property&PROPERTY_CREATED)>0 )
									break;
							}
						}
					}
				}
			}
		}	
		
		if( (gs_his_filter&HIS_SHOW_PRODUCT_PARAMETER)>0 
			&& phistory[i].record_type==HIS_REC_PRODUCT_PARAMETER) 
		{
			SProductParameterPtr	pprodparameter = (SProductParameterPtr)phistory[i].precord;   
			
			if( pprodparameter->tester_id == 0 
				&& ((pprodparameter->property&PROPERTY_VALID)>0
					|| (pprodparameter->property&PROPERTY_DELETED)>0 ))
			{
				EXCCHECK( HistoryToStringProductParameter(i, -1, pelstring, pprodparameter) );   				
				
				if( (pprodparameter->property&PROPERTY_CREATED)==0 )
				{
					for(j=0; j<phistorySize_all; j++)
					{
						if( phistory_all[j].record_type==HIS_REC_PRODUCT_PARAMETER 
							&& phistory_all[j].record_time < phistory[i].record_time)
						{
							SProductParameterPtr	pprodparameter2 = (SProductParameterPtr)phistory_all[j].precord;
						
							if( (pprodparameter2->property&PROPERTY_VALID)==0
								&& pprodparameter2->product_id == pprodparameter->product_id
								&& pprodparameter2->tester_id == 0
								&& 0==strcmp(pprodparameter2->name, pprodparameter->name))
							{
								EXCCHECK( HistoryToStringProductParameter(i, j, pelstring, pprodparameter2) );
								
								if( (pprodparameter2->property&PROPERTY_CREATED)>0 )
									break;
							}
						}
					}
				}
			}
		}
		
		if( (gs_his_filter&HIS_SHOW_PRODUCT_MASTER)>0 
			&& phistory[i].record_type==HIS_REC_PRODUCT_MASTER) 
		{
			SProductMasterPtr	pprodmaster = (SProductMasterPtr)phistory[i].precord;   
			
			if( (pprodmaster->property&PROPERTY_VALID)>0
				|| (pprodmaster->property&PROPERTY_DELETED)>0 )
			{
				EXCCHECK( HistoryToStringProductMaster(i, -1, pelstring, pprodmaster) );   				
				
				if( (pprodmaster->property&PROPERTY_CREATED)==0 )
				{
					for(j=0; j<phistorySize_all; j++)
					{
						if( phistory_all[j].record_type==HIS_REC_PRODUCT_MASTER 
							&& phistory_all[j].record_time < phistory[i].record_time)
						{
							SProductMasterPtr	pprodmaster2 = (SProductMasterPtr)phistory_all[j].precord;
						
							if( (pprodmaster2->property&PROPERTY_VALID)==0
								&& pprodmaster2->product_id == pprodmaster->product_id
								&& pprodmaster2->master_id == pprodmaster->master_id
								&& 0==strcmp(pprodmaster2->master, pprodmaster->master))
							{
								EXCCHECK( HistoryToStringProductMaster(i, j, pelstring, pprodmaster2) );
								
								if( (pprodmaster2->property&PROPERTY_CREATED)>0 )
									break;
							}
						}
					}
				}
			}
		}
		
		if( (gs_his_filter&HIS_SHOW_TEST)>0
			&& phistory[i].record_type==HIS_REC_TEST)
		{
			STestStepPtr 	ptest = (STestStepPtr)phistory[i].precord;    
			
			if( (ptest->property&PROPERTY_VALID)>0 
				|| (ptest->property&PROPERTY_DELETED)>0
				|| (ptest->property&PROPERTY_MOVED)>0)
			{
				EXCCHECK( HistoryToStringTest(i, -1, pelstring, ptest) );
				
				if( (ptest->property&PROPERTY_CREATED)==0 )
				{
					for(j=0; j<phistorySize_all; j++)
					{
						if( phistory_all[j].record_type==HIS_REC_TEST 
							&& phistory_all[j].record_time < phistory[i].record_time )
						{
							STestStepPtr 	ptest2 = (STestStepPtr)phistory_all[j].precord;   
						
							if( (ptest2->property&PROPERTY_VALID)==0
								&& ptest2->product_id == ptest->product_id
								&& ptest2->test_nb == ptest->test_nb)
							{
								EXCCHECK( HistoryToStringTest(i, j, pelstring, ptest2) ); 
								
								if( (ptest2->property&PROPERTY_CREATED)>0 )
									break;
							}
						}
					}
				}
			}
		}
		
		if( (gs_his_filter&HIS_SHOW_TEST_PARAMETER)>0 
			&& phistory[i].record_type==HIS_REC_TEST_PARAMETER) 
		{
			STestParameterPtr	pparameter = (STestParameterPtr)phistory[i].precord;   
			
			if( (pparameter->property&PROPERTY_VALID)>0
				|| (pparameter->property&PROPERTY_DELETED)>0 )
			{
				EXCCHECK( HistoryToStringTestParameter(i, -1, pelstring, pparameter) );  
				
				if( (pparameter->property&PROPERTY_CREATED)==0 )
				{
					for(j=0; j<phistorySize_all; j++)
					{
						if( phistory_all[j].record_type==HIS_REC_TEST_PARAMETER 
							&& phistory_all[j].record_time < phistory[i].record_time)
						{
							STestParameterPtr	pparameter2 = (STestParameterPtr)phistory_all[j].precord; 
						
							if( (pparameter2->property&PROPERTY_VALID)==0
								&& pparameter2->product_id == pparameter->product_id
								&& pparameter2->test_nb == pparameter->test_nb
								&& 0==strcmp(pparameter2->name, pparameter->name))
							{
								EXCCHECK( HistoryToStringTestParameter(i, j, pelstring, pparameter2) );
								
								if( (pparameter2->property&PROPERTY_CREATED)>0 )
									break;
							}
						}
					}
				}
			}
		}
		
		if( (gs_his_filter&HIS_SHOW_USER)>0 
			&& phistory[i].record_type==HIS_REC_USER) 
		{
			SDBSUserPtr	puser = (SDBSUserPtr)phistory[i].precord;   
			
			if( (puser->property&PROPERTY_VALID)>0
				|| (puser->property&PROPERTY_DELETED)>0 )
			{
				EXCCHECK( HistoryToStringUser(i, -1, pelstring, puser) );  
				
				if( (puser->property&PROPERTY_CREATED)==0 )
				{
					for(j=0; j<phistorySize_all; j++)
					{
						if( phistory_all[j].record_type==HIS_REC_USER 
							&& phistory_all[j].record_time < phistory[i].record_time)
						{
							SDBSUserPtr	puser2 = (SDBSUserPtr)phistory_all[j].precord; 
						
							if( (puser2->property&PROPERTY_VALID)==0
								&& puser2->user_id == puser->user_id )
							{
								EXCCHECK( HistoryToStringUser(i, j, pelstring, puser2) );
								
								if( (puser2->property&PROPERTY_CREATED)>0 )
									break;
							}
						}
					}
				}
			}
		}
		
		if( (gs_his_filter&HIS_SHOW_USER_RIGHTS)>0 
			&& phistory[i].record_type==HIS_REC_USER_RIGHTS) 
		{
			SDBSUserPtr	puser = (SDBSUserPtr)phistory[i].precord; 
			int32_t			pos = phistory[i].pos;
			
			if( (puser->Rights[pos].property&PROPERTY_VALID)>0
				|| (puser->Rights[pos].property&PROPERTY_DELETED)>0 )
			{
				EXCCHECK( HistoryToStringUserRights(i, -1, pelstring, puser, pos) );  
				
				if( (puser->Rights[pos].property&PROPERTY_CREATED)==0 )
				{
					for(j=0; j<phistorySize_all; j++)
					{
						if( phistory_all[j].record_type==HIS_REC_USER_RIGHTS 
							&& phistory_all[j].record_time < phistory[i].record_time)
						{
							SDBSUserPtr	puser2 = (SDBSUserPtr)phistory_all[j].precord; 
							int32_t		pos2 = phistory_all[j].pos; 
						
							if( puser2->user_id == puser->user_id
								&& (puser2->Rights[pos2].property&PROPERTY_VALID)==0
								&& puser2->Rights[pos2].user_id == puser->Rights[pos].user_id
								&& puser2->Rights[pos2].tester_id == puser->Rights[pos].tester_id)
							{
								EXCCHECK( HistoryToStringUserRights(i, j, pelstring, puser2, pos2) );
								
								if( (puser2->Rights[pos2].property&PROPERTY_CREATED)>0 )
									break;
							}
						}
					}
				}
			}
		}
		
	}
	
Error:
	EXCRETHROW(pexception);  
}


