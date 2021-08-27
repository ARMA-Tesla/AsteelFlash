#if !defined(__DBS_GLOBAL_H__)
#define __DBS_GLOBAL_H__

#include <exception/exception.h>
#include <dbs/dbs.h>
#include <language/language.h>
#include <elstring.h>
#include <timer/eltimer.h> 
#include <users/user.h>
#include <ibutton/ibutton.h>
#include <test/testseq.h>
#include "database.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

#define PDBS		global->pdbs
#define PUSER		global->pdbs->UserList
#define PPRODUCT	global->pdbs->ProductList
#define PPARAM		global->pdbs->ProductParameterList
#define PMASTER		global->pdbs->ProductMasterList
#define PTEST		global->pdbs->TestList
#define PTESTPARAM	global->pdbs->TestParameterList
#define PEXPORT		global->pdbs->Export
#define PTESTER		global->pdbs->TesterList
#define PPROCESS	global->pdbs->LineProcessList
#define PSTEP		global->pdbs->LineStepList
#define PLINEPARAM	global->pdbs->LineParamList
#define PLINEEXPORT global->pdbs->LineExport

#define PDBS_FREE(me)			global->pdbs->Free(me)
#define PDBS_ALLOC(me, size)	global->pdbs->CharAlloc(me, size)
#define PDBS_COPY(me)			global->pdbs->CharCopy(me)
	
#define PARAMETER_DIALOG_HIGH			55
#define PARAMETER_DIALOG_HIGH_WITH_TEXT	355
#define DBS_NUMBER_GROUP_ITEMS         	128     /* number of items in one group */

#define DBS_GROUP_1                  	0
#define DBS_GROUP_2                  	1
#define DBS_GROUP_3                  	2
#define DBS_GROUP_4                  	3
#define DBS_GROUP_5                  	4  
#define DBS_NUMBER_OF_GROUPS         	5

#define PAGE_ONE 						0
#define PAGE_TWO 						1
#define PAGE_THREE 						2
#define PAGE_FOUR  						3
	
/* BKLib2__DBKTreeMoveNode */
#define TCTV_FIRST              0x01 // První uzel ve vìtvi. Je možno kombinovat s TCTV_CHILD.
#define TCTV_LAST               0x02 // Poslední uzel ve vìtvi. Je možno kombinovat s TCTV_CHILD.
#define TCTV_PREV               0x04 // Pøedchozí uzel ve vìtvi.
#define TCTV_NEXT               0x08 // Následující uzel ve vìtvi.
#define TCTV_CHILD              0x10 // V kombinaci s TCTV_FIRST a TCTV_LAST se bude jednat o první nebo poslední uzel v podøízené vìtvi.

#define CHECK_RIGHTS_PARAMETER(fnc)	if((global->activeUser.rights_dbs&USER_RIGHTS_EDIT_PARAMETER)>0) \
										{ fnc; } \
									else \
										{ MessagePopup( _("Message"), _("You have not required rights for editing parameters from database!")); }

#define CHECK_RIGHTS_PARAMETER2(fnc1, fnc2)	((global->activeUser.rights_dbs&USER_RIGHTS_EDIT_PARAMETER)>0)? (fnc1):(fnc2)

#define CHECK_RIGHTS_TEST(fnc)	if((global->activeUser.rights_dbs&USER_RIGHTS_EDIT_TEST)>0) \
									{ fnc; } \
								else \
									{ MessagePopup( _("Message"), _("You have not required rights for editing steps from database!")); }

#define CHECK_RIGHTS_PRODUCT(fnc)	if((global->activeUser.rights_dbs&USER_RIGHTS_EDIT_PRODUCT)>0) \
										{ fnc; } \
									else \
										{ MessagePopup( _("Message"), _("You have not required rights for editing products from database!")); }
	
#define CHECK_RIGHTS_USER_NEW(fnc)	if((global->activeUser.rights_dbs&USER_RIGHTS_EDIT_USER)>0) \
										{ fnc; } \
									else \
										{ MessagePopup( _("Message"), _("You have not required rights for editing users from database!")); }

#define CHECK_RIGHTS_USER_EDIT(xuser, fnc)	{ \
	if( global->activeUser.tester_id == 0 ) \
		{ fnc; } \
	else \
		{ \
			int xi; \
			for(xi=0; xi<xuser.RightsSize; xi++) \
			{ \
				if( (xuser.Rights[xi].property&PROPERTY_VALID)>0 \
				  	&& xuser.Rights[xi].tester_id == 0 ) \
				{ break; } \
			} \
			if(xi<xuser.RightsSize) \
				{ MessagePopup( _("Message"), _("You have not required rights for editing this item!")); } \
			else { \
				for(xi=0; xi<xuser.RightsSize; xi++) \
				{ \
					if( (xuser.Rights[xi].property&PROPERTY_VALID)>0 \
					  	&& xuser.Rights[xi].tester_id == global->activeUser.tester_id ) \
					{ fnc; break; } \
				} \
				if(xi==xuser.RightsSize) \
					{ MessagePopup( _("Message"), _("You have not required rights for editing this item!")); } \
			} \
		} \
	}

#define CHECK_RIGHTS_USER_EDIT_RIGHTS(xtester_id, fnc) if( global->activeUser.tester_id == 0 \
															|| global->activeUser.tester_id == xtester_id) \
															{ fnc; } \
														else \
															{ MessagePopup( _("Message"), _("You have not required rights for editing this item!")); }

#define CHECK_RIGHTS_ROOT(fnc) 	if(global->activeUser.tester_id == 0) \
									{ fnc; } \
								else \
									{ MessagePopup( _("Message"), _("You have not root rights!")); }
	
#define CHECK_RIGHTS_ROOT2(fnc1, fnc2)	if(global->activeUser.tester_id == 0) {fnc1;} else {fnc2;}

#define CHECK_VALUE( value) \
	RemoveSurroundingWhiteSpace(value); \
    if(0==strlen(value)) { \
		MessagePopup("Warning", "Empty string value!"); \
		CHECKERR( -1); }

#define TIMER_START       	global->ptimer->Start(global->ptimer)
#define TIMER_STOP(value)   value = global->ptimer->GetTimeDiff(global->ptimer); \
                     		SetCtrlVal(gs_panel[DBS], DBS_TIMER, value)

#define USED_MEMORY(value)	PDBS->UsedMemory(PDBS, &value); \
							SetCtrlVal(gs_panel[DBS], DBS_MEMORY, value)      
	
#define BKTREE_MAX_SELECTED_ITEM		25

#define CONFIG_XML			global->configxml
#define CONFIG_XML_PATH		"\\maintenance\\database.xml"

typedef enum _EProgMode
{
   PROG_TREE 		= 0,
   PROG_INFO
} EProgMode;                     /* program mode */	
	
typedef enum _EEditMode
{
   EDIT_PRODUCT 		= 1,
   EDIT_PRODUCT_PARAM,
   EDIT_TEST,
   EDIT_TEST_PARAM,
   EDIT_LINE,
   EDIT_USER,
   DBS_HISTORY,
   DBS_INFO,
   EDIT_MODE					 /* size of EEditMode */
} EEditMode;                     /* bktree edit mode */

typedef struct _SGlobal               /* GLOBAL */
{
   SELStringPtr   	pelstring;
   STimerPtr      	ptimer; 
   SDBSPtr			pdbs;
   SIButtonPtr		ibutton;
   STestSequencePtr	ptestseq;
  
   int32_t			process_nb;								/* actual process number */
   char				process_name[128];						/* actual process name */ 
					
   int32_t  		set_product_id;                    		/* product id for setting */ 
   int32_t			product_id;								/* actual product id */
   int32_t			branch_id;								/* default product branch*/
   
   int32_t  		test_nb;                        		/* active test_nb */
   int32_t			test_nb_ins[BKTREE_MAX_SELECTED_ITEM];  /* inserted items in test step view */
   int32_t			test_nb_insSize;						/* number of inserted items */
   
   char     		keyNode[EDIT_MODE][32];		       		/* active node */
   
   bool_t			HistoryMerge;
   
   CAObjHandle		pickerHandle;
   CAObjHandle		hpicDayHandle;
   CAObjHandle		hpicHourHandle;
   CAObjHandle 		treeHandle;                        		/* bktree handle */
   
   EProgMode		progMode;								/* program mode */	
   EEditMode    	editMode;                         		/* bktree edit mode */
   
   SEUser			activeUser;

   HINSTANCE		hInstance;
   char				pathIOxml[512];
   char				configxml[512];

} SGlobal, *SGlobalPtr;

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_GLOBAL_H__)
