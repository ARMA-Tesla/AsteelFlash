#if !defined(__BOX_H__)
#define __BOX_H__

#include <eltypes.h>
#include <exception/Exception.h>
#include <windows.h>
#include <tester/tester.h>
#include <eventfactory/EventFactory.h>

#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif

#define EVNT_BOX_CHANGE		"EVNT_BOX_CHANGE"
#define EVNT_BOX_INSERTED	"EVNT_BOX_INSERTED"

typedef SElExceptionPtr (*BoxCallback_t)(struct _SBox* me);

typedef struct _SBox
{
	SElExceptionPtr (*Initialize)(struct _SBox*	me, const char*	fileXmlConfig);
	SElExceptionPtr (*RegisterSensorCallbacks)( struct _SBox* me);

	SElExceptionPtr (*AddPanelToBox)( struct _SBox* me);
	SElExceptionPtr (*SetPanelsInBox)( struct _SBox* me, uint32_t nb);

/*******************************************
 * PRIVATE
 *******************************************/
	STesterPtr			testerAPI;
	SElEventFactoryPtr	eventAPI;
	uint32_t			_PanelsInBox;
	uint32_t			_PanelsActual;
	bool_t				_IsBoxInserted;
	BoxCallback_t		_fncBoxCallback;
} SBox, *SBoxPtr;

static SElExceptionPtr CallbackFncBoxInserted(const char* name, uint32_t state, void* pData);
static SElExceptionPtr SetBoxState(struct _SBox* me, uint32_t state);

SElExceptionPtr	box_new(SBoxPtr* pBox);
SElExceptionPtr	box_delete(SBoxPtr* pTepBoxster);

#define BOX_ERROR_INVALID_PARAMETER		-10001
#define BOX_ERROR_TIMEOUT				-10002

#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif

#endif /* __BOX_H__*/ 
