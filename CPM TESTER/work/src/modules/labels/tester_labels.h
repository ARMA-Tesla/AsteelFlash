#if !defined(__TESTER_LABELS_H__)
#define __TESTER_LABELS_H__

#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif

#include <eltypes.h>
#include <exception/Exception.h>
#include <printer/class/Printers.h>

#define LABEL_ERROR_INVALID_LABEL_ID		(-5000L)
#define LABEL_MAX_TEXT_RESULT				256

typedef struct _SLabelList {
	void*	LabelList;
	void*	pDBS;
	int32_t	reloadLabels;
	char*	parameters;
	
	SElExceptionPtr (*ResetLabels)(struct _SLabelList*	me);
	SElExceptionPtr (*ParseLabelToBuffer)(struct _SLabelList*	me,
										  const char*		labelDefinition,
										  int32_t			productId,
										  const char*		parameters,
										  char*				labelBuffer,
										  int32_t			bufferSize);
	SElExceptionPtr (*GetLabel)(struct _SLabelList*	me,
								const char*			labelId,
								int32_t				productId,
								const char*			parameters,
								char*				labelBuffer,
								int32_t				bufferSize);
	SElExceptionPtr (*PrintLabel)(struct _SLabelList*	me,
								  SPrinterPtr			printer,
								  const char*			labelId, /* parameter name with a label definition from dbs */
								  int32_t				productId,
								  const char*			parameters);
	SElExceptionPtr (*PrintLabel2)(struct _SLabelList*	me,
								  SPrinterPtr			printer,
								  const char*			labelId, /* parameter name with a label definition from dbs */
								  int32_t				productId,
								  const char*			parameters);
	
	SElExceptionPtr (*CheckBarCode)(struct _SLabelList*	me,
									const char*			barcodeId, /* parameter name from dbs */
									int32_t				productId,
									const char*			barCode,
									int32_t*			isCorrect,
									const char*			parameters);
	SElExceptionPtr (*GetBarCodeStatus)(struct _SLabelList*	me,
										const char*			barCode, 
										bool_t*				tested,
										bool_t*				failed);
	SElExceptionPtr (*GetMasterBarCodeStatus)(struct _SLabelList*	me,
											  const char*			barCode, 
											  bool_t*				valid,
											  int32_t*				masterId);

} SLabelList, *SLabelListPtr;

SElExceptionPtr labels_new(
	SLabelListPtr*	pLabel,
	const char*		parameters
);
SElExceptionPtr labels_delete(SLabelListPtr* pLabels);

#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif

#endif /* __TESTER_LABELS_H__ */
