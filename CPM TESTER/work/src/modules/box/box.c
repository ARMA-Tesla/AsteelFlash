#include "box.h"
#include "msxmldom.h"
#include <ansi_c.h>
#include <guid.h>
#include <time.h>

#define TIMEOUT_INIT() 	clock_t	_start_clock = clock();

#define TIMEOUT_CHECK(timeout) \
	if ( timeout>0 && ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > (timeout/1000.0) ) { EXCTHROW( BOX_ERROR_TIMEOUT, "Timeout"); } else

#define STATE_BOX_UNLOCK	0
#define STATE_BOX_LOCK		1

static SElExceptionPtr Initialize(struct _SBox*	me, const char*	fileName);
static SElExceptionPtr RegisterSensorCallbacks(struct _SBox* me);
static SElExceptionPtr AddPanelToBox(struct _SBox* me);
static SElExceptionPtr SetPanelsInBox(struct _SBox* me, uint32_t nb);
static SElExceptionPtr SetBoxState(struct _SBox* me, uint32_t state);

static SElExceptionPtr CallbackFncBoxInserted(const char* name, uint32_t state, void* pData);
static SElExceptionPtr SetBoxState(struct _SBox* me, uint32_t state);

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "box_new"
 SElExceptionPtr box_new(SBoxPtr* pBox)
{
	SElExceptionPtr    	pexception = NULL;
	int32_t				error = 0;

	*pBox = calloc( 1, sizeof(SBox));
	EXCCHECKALLOC( *pBox);

	(*pBox)->Initialize = Initialize;
	(*pBox)->RegisterSensorCallbacks = RegisterSensorCallbacks;
	(*pBox)->AddPanelToBox = AddPanelToBox;
	(*pBox)->SetPanelsInBox = SetPanelsInBox;

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "box_delete"
SElExceptionPtr	box_delete(SBoxPtr* pBox)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int32_t			status;

	if(pBox && *pBox)
	{
		(*pBox)->eventAPI = NULL;

		free( *pBox);
		*pBox = NULL;
	}

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"Box::RegisterSensorCallbacks"
static SElExceptionPtr RegisterSensorCallbacks( struct _SBox* me)
{
	SElExceptionPtr	pexception = NULL;
	STesterPtr		pTester = me->testerAPI;

	EXCCHECK( pTester->SetSensorCallback(pTester, "BOX_INSERTED", CallbackFncBoxInserted, me));

Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"Box::Initialize"
static SElExceptionPtr Initialize
(
	struct _SBox*	me,
	const char*		fileName
)
{
	SElExceptionPtr	pexception = NULL;
	CAObjHandle		xml_doc = 0;
	CAObjHandle		pnode = 0;
	VBOOL			is_xml_success = 1;
	const char*     pfile_name = (fileName && *fileName)? fileName : "modules\\box.xml";
	char			*ptext = NULL;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	int32_t			error = 0;
	HRESULT			hr;

	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &xml_doc));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, pfile_name));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (xml_doc, NULL, pfile_name_var, &is_xml_success));
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (xml_doc, NULL, "//module[@id='"MODULE_BOX"']/panel_nb", &pnode));
	if(pnode)
	{
		EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (pnode, NULL, &ptext));
		if(ptext && *ptext)
		{
			me->_PanelsInBox = strtoul(ptext, NULL, 10);
			CA_FreeMemory(ptext); 
			ptext = NULL;
		}
		CA_DiscardObjHandle(pnode);
		pnode = 0;
	}

	EXCCHECK( SetBoxState(me, STATE_BOX_UNLOCK));

Error:
	if (pnode) CA_DiscardObjHandle(pnode);
	if (xml_doc) CA_DiscardObjHandle(xml_doc);
	CA_VariantClear(&pfile_name_var);	
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"SetBoxState"
static SElExceptionPtr SetBoxState(struct _SBox* me, uint32_t state)
{
	SElExceptionPtr	pexception = NULL;
	STesterPtr		pTester = me->testerAPI;
	uint32_t		is_locked,
					is_unlocked;
	TIMEOUT_INIT();

	EXCCHECK( pTester->SetSensor(pTester, "BOX_LOCK_UP", state));
	EXCCHECK( pTester->SetSensor(pTester, "BOX_LOCK_DOWN", !state));

	if(state)
		me->_PanelsActual = 0;

	me->_IsBoxInserted = (state==STATE_BOX_LOCK);

	/* wait for sensor */
	do
	{
		EXCCHECK( pTester->GetSensor(pTester, "BOX_LOCKED", &is_locked));
		EXCCHECK( pTester->GetSensor(pTester, "BOX_UNLOCKED", &is_unlocked));

		if(is_locked==state && is_unlocked==!state) 
			break;

		TIMEOUT_CHECK(2000);
	}while(TRUE);

Error:
	EXCDBGSTR(pexception);
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"CallbackFncBoxInserted"
static SElExceptionPtr CallbackFncBoxInserted(const char* name, uint32_t state, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	SBoxPtr			me = (SBoxPtr)pData;

	if(state==1 && me->eventAPI)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_BOX_INSERTED, NULL));

	EXCCHECK( SetBoxState(me, state));

	if(me->_fncBoxCallback)
		EXCCHECK(me->_fncBoxCallback(me));

Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"Box::AddPanelToBox"
static SElExceptionPtr AddPanelToBox(struct _SBox* me)
{
	SElExceptionPtr	pexception = NULL;

	me->_PanelsActual++;

	if(me->_PanelsActual>=me->_PanelsInBox)
	{
		EXCCHECK( SetBoxState(me, STATE_BOX_UNLOCK));

		if(me->eventAPI)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_BOX_CHANGE, NULL));
	}

	if(me->_fncBoxCallback)
		EXCCHECK(me->_fncBoxCallback(me));

Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"Box::SetPanelsInBox"
static SElExceptionPtr SetPanelsInBox(struct _SBox* me, uint32_t nb)
{
	SElExceptionPtr	pexception = NULL;

	me->_PanelsActual = nb;

	if(me->_PanelsActual>=me->_PanelsInBox)
	{
		EXCCHECK( SetBoxState(me, STATE_BOX_UNLOCK));

		if(me->eventAPI)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_BOX_CHANGE, NULL));
	}

	if(me->_fncBoxCallback)
		EXCCHECK(me->_fncBoxCallback(me));

Error:
	EXCRETHROW(pexception);
}
