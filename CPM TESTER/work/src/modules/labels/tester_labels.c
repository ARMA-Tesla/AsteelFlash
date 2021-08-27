
#include "tester_labels.h"
#include "msxmldom.h"
#include "toolbox.h"
#include <formatio.h>
#include <userint.h>
#include <dbs/dbs.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#pragma warning( push)
#pragma warning( disable: 4100)

typedef struct _SLabel {
	int32_t			productId;
	SEParamPtr		parameters;
	int32_t			paramSize;
	struct _SLabel*	next;
} SLabel, *SLabelPtr;

#define LBL_VAR_CONFIG				"config"
#define LBL_VAR_LOW					"LowLimit"
#define LBL_VAR_HIGH				"HighLimit"
#define LBL_VAR_UNIT				"Unit"
#define LBL_VAR_FN					"FunctionName"
#define LBL_VAR_MV					"MeasuredValue"
#define LBL_VAR_RTS					"TestResultString"
#define LBL_VAR_DESC				"TestDescription"		
#define LBL_VAR_SHIFT_ID			"ShiftId"
#define LBL_VAR_PALETT_ID			"PalettId"

#define LBL_VAR_TIME				"TIME"
#define LBL_VAR_HH					"HH"
#define LBL_VAR_NN					"NN"
#define LBL_VAR_SS					"SS"

#define LBL_VAR_Y					"Y"
#define LBL_VAR_MM					"MM"
#define LBL_VAR_DD					"DD"

#define LBL_VAR_SAPMN				"SAPMN"
#define LBL_VAR_HW_IDENTIFIER		"HW_Identifier"
#define LBL_VAR_HWID				"HWID"
#define LBL_VAR_SAP_SUPPLIER		"SAP_Supplier"
#define LBL_VAR_SUPPLIER_SERIES		"Supplier_Series"
#define LBL_VAR_VDE_VERSION			"VDE_Version"
#define LBL_VAR_TEMPERATURE			"Temperature"
#define LBL_VAR_NOMINAL_VOLTAGE		"Nominal_Voltage"
#define LBL_VAR_NOMINAL_POWER		"Nominal_Power"
#define LBL_VAR_SMM					"SMM"
#define LBL_VAR_NUM_SERIE_PFIN		"NUM_SERIE_PFIN"	


#define LBL_VAR_DY36				"DY36"
#define LBL_VAR_SERIAL				"SERIAL"

#define LBL_VAR_SOH					"SOH"
#define LBL_VAR_STX					"STX"
#define LBL_VAR_ESC					"ESC"

#define LBL_PRM_SERIAL_FMT			"FMT"
#define LBL_PRM_CODE36				"CODE36"
#define LBL_PRM_YEAR_SIZE			"SIZE"
#define LBL_PRM_YEAR_CODE			"CODE"

#define LBL_CONST_SOH				0x01
#define LBL_CONST_STX				0x02
#define LBL_CONST_ACK				0x06
#define LBL_CONST_LF				0x0A
#define LBL_CONST_CR				0x0D
#define LBL_CONST_XON				0x11
#define LBL_CONST_XOFF				0x13
#define LBL_CONST_NAK				0x15
#define LBL_CONST_ESC				0x1B

static SElExceptionPtr labels_initialize(
	SLabelListPtr*	pLabels,
	const char*		parameters
);
static SElExceptionPtr labels_checkBarCode(
	SLabelListPtr	me,
	const char*		barcodeId,
	int32_t			productId,
	const char*		barCode,
	int32_t*		isCorrect,
	const char*		parameters
);
static SElExceptionPtr labels_GetBarCodeStatus(
	struct _SLabelList*	me,
	const char*			barCode, 
	bool_t*				tested,
	bool_t*				failed
);
static  SElExceptionPtr labels_GetMasterBarCodeStatus(
	struct _SLabelList*	me,
	const char*			barCode, 
	bool_t*				valid,
	int32_t*			masterId
);
static SElExceptionPtr labels_getResetLabels(SLabelListPtr me);
static SElExceptionPtr labels_getLabel(
	SLabelListPtr	me,
	const char*		labelId,
	int32_t			productId,
	const char*		parameters,
	char*			labelBuffer,
	int32_t			bufferSize
);
static SElExceptionPtr labels_parseLabel(
	SLabelListPtr	me,
	const char*		labelDefinition,
	int32_t			productId,
	const char*		parameters,
	char*			labelBuffer,
	int32_t			bufferSize
);
static SElExceptionPtr labels_printLabel(
	SLabelListPtr	me,
	SPrinterPtr		printer,
	const char*		labelId,
	int32_t			productId,
	const char*		parameters
);
static char* ParseLabel(
	SLabelListPtr	me,
	const char*		labelString,
	SLabelPtr		productLabel,
	CAObjHandle		parameters
);
static char* BuildOutputString(
	const char*	firstPart,
	int32_t		sizeOfFirstPart,
	const char*	secondPart,
	const char*	lastPart
);
static char* ReplaceVariable(
	SLabelListPtr	me,
	const char*		variableName,
	SLabelPtr		productParams,
	CAObjHandle		parameters
);
static int32_t FindParameterIndexByName(
	SEParamPtr	parameters,
	int32_t		paramSize,
	const char*	paramName
);
static SEParamPtr FindParameterByName(
	SEParamPtr	parameters,
	int32_t		paramSize,
	const char*	paramName
);

static void FindLabelID(
	const char*	path,
    char*	Label_Text
);

/* Numéro jour dans l'année **************************************************/
int NumeroJour (int mois, int jours, int annee)
{
	int nbJours=0;
  	
	switch (mois) 
	{
   		case 1 : nbJours = jours; break;
   		case 2 : nbJours = 31 + jours; break;
   		case 3 : nbJours = 31 + 28 + jours; break;
   		case 4 : nbJours = 31 + 28 + 31 + jours; break;
   		case 5 : nbJours = 31 + 28 + 31 + 30 + jours; break;
   		case 6 : nbJours = 31 + 28 + 31 + 30 + 31 + jours; break;
   		case 7 : nbJours = 31 + 28 + 31 + 30 + 31 + 30 + jours; break;
   		case 8 : nbJours = 31 + 28 + 31 + 30 + 31 + 30 + 31 + jours; break;
   		case 9 : nbJours = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + jours; break;
   		case 10: nbJours = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + jours; break;
   		case 11: nbJours = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + jours; break;
   		case 12: nbJours = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + jours;
	};

	/* Si année bissextile */
	if (((annee % 4) == 0) && (mois > 2))
		nbJours ++;

	return nbJours;	
}

/*****************************************************************************
 *****************************************************************************/
#undef __FUNC__
#define __FUNC__	"labels_new"
SElExceptionPtr labels_new(
	SLabelListPtr*	pLabel,
	const char*		parameters
)
{
	SElExceptionPtr	pexception = NULL;
	SLabelListPtr	plabels = NULL;
	
	EXCCHECK( labels_initialize(&plabels, parameters));
	
Error:
	if (pLabel) *pLabel = plabels;
	
	EXCRETHROW(pexception);
}

#undef  __FUNC__
#define __FUNC__	"labels_delete"
SElExceptionPtr labels_delete(SLabelListPtr* pLabels)
{
	SElExceptionPtr	pexception = NULL;
	
	if (*pLabels)
	{
		if ((*pLabels)->parameters != NULL)
		{
			free((*pLabels)->parameters);
			(*pLabels)->parameters = NULL;
		}
		
		EXCCHECK( (*pLabels)->ResetLabels(*pLabels));
		
		free(*pLabels);
		*pLabels = NULL;
	}
	
Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__	"labels_initialize"
static SElExceptionPtr labels_initialize(
	SLabelListPtr*	pLabels,
	const char*		parameters
)
{
	SElExceptionPtr	pexception = NULL;
	SLabelListPtr	plabels = NULL;

	plabels = (SLabelListPtr) calloc(1, sizeof(SLabelList));
	EXCCHECKALLOC( plabels);

	plabels->CheckBarCode		= labels_checkBarCode;
	plabels->GetBarCodeStatus	= labels_GetBarCodeStatus;
	plabels->GetMasterBarCodeStatus	= labels_GetMasterBarCodeStatus;
	plabels->ResetLabels		= labels_getResetLabels;
	plabels->GetLabel			= labels_getLabel;
	plabels->ParseLabelToBuffer	= labels_parseLabel;
	plabels->PrintLabel			= labels_printLabel;
	
	
	plabels->LabelList = NULL;
	if (parameters)
	{
		int32_t	size = strlen(parameters) + 1;

		plabels->parameters = (char*) malloc(size * sizeof(char));
		strcpy_s(plabels->parameters, size, parameters);
	}
	else
	{
		plabels->parameters = NULL;
	}
	
	if (pLabels != NULL)
		*pLabels = plabels;
	
Error:
	EXCRETHROW(pexception);
} /* labels_initialize */

/*****************************************************************************
 *****************************************************************************
 *****************************************************************************/
/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__	"labels_checkBarCode"
static SElExceptionPtr labels_checkBarCode(
	SLabelListPtr	me,
	const char*		barcodeId,
	int32_t			productId,
	const char*		barCode,
	int32_t*		isCorrect,
	const char*		parameters
)
{
	SElExceptionPtr	pexception = NULL;
	char			buffer[256] = "";
	char			*pdata = NULL,
					*pdata2 = NULL;
	uint32_t		i;
	
	if (isCorrect) *isCorrect = FALSE;

	if(productId<0 && me->pDBS)
		productId = ((SDBSPtr)me->pDBS)->TesterProductId;

	EXCCHECK( me->GetLabel(me, barcodeId,  productId, parameters, buffer, sizeof(buffer)));

	if(strlen(buffer)==strlen(barCode))
	{
		pdata = buffer;
		pdata2 = (char*)barCode;
		
		/* check barcode */
		for( i = 0; i < strlen(buffer); i++)
		{
			if(*pdata == 0x23) /* # */
			{
				pdata++;
				pdata2++;
				continue;
			}

			if(*pdata != *pdata2)
				break;

			pdata++;
			pdata2++;
		}
		if(i == strlen(buffer) && isCorrect)
			*isCorrect = TRUE;
	}
	
Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__	"labels_GetBarCodeStatus"
static SElExceptionPtr labels_GetBarCodeStatus(
	struct _SLabelList*	me,
	const char*			barCode, 
	bool_t*				tested,
	bool_t*				failed
)
{
	SElExceptionPtr	pexception = NULL;

	if(me->pDBS)
		EXCCHECK( ((SDBSPtr)me->pDBS)->Storage->GetBarCodeStatus(((SDBSPtr)me->pDBS)->Storage, barCode, tested, failed));

Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__	"labels_GetMasterBarCodeStatus"
static  SElExceptionPtr labels_GetMasterBarCodeStatus(
	struct _SLabelList*	me,
	const char*			barCode, 
	bool_t*				valid,
	int32_t*			masterId
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			master_id = 0;

	if(valid) *valid = FALSE;
	if(masterId) *masterId = 0;

	if(me->pDBS)
		EXCCHECK( ((SDBSPtr)me->pDBS)->ProductMasterList->ProductMasterGetId(
											((SDBSPtr)me->pDBS)->ProductMasterList,
											((SDBSPtr)me->pDBS)->TesterProductId,
											barCode,
											&master_id));

	if(master_id > 0)
	{
		if(valid) *valid = TRUE;
		if(masterId) *masterId = master_id;
	}

Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__	"labels_getResetLabels"
static SElExceptionPtr labels_getResetLabels(SLabelListPtr me)
{
	SElExceptionPtr	pexception = NULL;
	SLabelPtr		p2labels = me->LabelList;
	
	while (p2labels)
	{
		SLabelPtr	p2next = p2labels->next;
		
		if (p2labels->parameters)
		{
			EXCCHECK( ((SDBSPtr)me->pDBS)->Export->FreeParameter(((SDBSPtr)me->pDBS)->Export, 
																&p2labels->parameters,
																p2labels->paramSize));
		}
		
		free(p2labels);
		p2labels = p2next;
	}
	
	me->LabelList = NULL;
	
Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__	"labels_parseLabel"
static SElExceptionPtr labels_parseLabel(
	SLabelListPtr	me,
	const char*		labelDefinition,
	int32_t			productId,
	const char*		parameters,
	char*			labelBuffer,
	int32_t			bufferSize
)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr = S_OK;
	VARIANT_BOOL	is_xml_success = 1;
	SLabelPtr		p2label = (SLabelPtr)me->LabelList;	
	CAObjHandle		document = 0;
	char*			print_label = NULL;
	int32_t			label_size;
	int32_t			copy_size, error = 0;
	
	
	if(parameters!=NULL)
	{
		EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &document));
		EXCCHECKMSXML( MSXML_IXMLDOMDocumentloadXML (document, NULL, parameters, &is_xml_success));
	}
	
	print_label = ParseLabel(me, labelDefinition, p2label, document);
	
	label_size = (int32_t) strlen(print_label);
	if (labelBuffer != NULL && bufferSize != 0)
	{
		if (bufferSize < label_size)
		{
			copy_size = bufferSize;
			error = label_size;
		}
		else
		{
			copy_size = label_size;
		}
#ifndef _CVI_
		strncpy_s(labelBuffer, bufferSize, print_label, copy_size - 1);
#else
		strncpy(labelBuffer, print_label, copy_size - 1);
#endif
		labelBuffer[copy_size - 1] = '\0';
	}
	else
	{
		error = label_size;
	}
	if ( error > 0 )
	{
		EXCTHROW( error, "Small buffer for parselabel");
	}

Error:
	if (document)
		CA_DiscardObjHandle(document);
	
	if (print_label)
		free(print_label);
	
	EXCRETHROW(pexception);	
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__	"labels_getLabel"
static SElExceptionPtr labels_getLabel(
	SLabelListPtr	me,
	const char*		labelId,
	int32_t			productId,
	const char*		parameters,
	char*			labelBuffer,
	int32_t			bufferSize
)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr = S_OK;
	VARIANT_BOOL	is_xml_success = 1;
	SLabelPtr		p2label = (SLabelPtr)me->LabelList;
	SLabelPtr		new_label = NULL;
	SEParamPtr		p2label_def = NULL;
	CAObjHandle		document = 0;
	char*			print_label = NULL;
	int32_t			label_size = 0,
					copy_size = 0;
	
	while (p2label != NULL)
	{
		if (p2label->productId == productId)
			break;

		p2label = p2label->next;
	}
	
	if (p2label == NULL)
	{
		new_label = (SLabelPtr) calloc(1, sizeof(SLabel));
		EXCCHECKALLOC( new_label);
		new_label->next = NULL;
		new_label->productId = productId;
				
		/* Get new parameters from database */
		if(productId > 0)
		{	
			EXCCHECK( ((SDBSPtr)me->pDBS)->Export->GetParameter(((SDBSPtr)me->pDBS)->Export, 
																productId, 0,
																&(new_label->parameters),
																&(new_label->paramSize)));
		}
		
		if (me->LabelList == NULL)
		{
			me->LabelList = (void*)new_label;
		}
		else
		{
			p2label = (SLabelPtr)me->LabelList;
			while (p2label->next)
			{
				p2label = p2label->next;
			}
			
			p2label->next = new_label;
		}
		p2label = new_label;
	}
	
	p2label_def = FindParameterByName(p2label->parameters, p2label->paramSize, labelId);
	
	if (p2label_def == NULL)
		EXCTHROW(-1, "Label Definition Not Found");
	
	/* parse label for print and return buffer */
	if ( parameters != NULL )
	{
		EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &document));
		EXCCHECKMSXML( MSXML_IXMLDOMDocumentloadXML (document, NULL, parameters, &is_xml_success));
	}
	print_label = ParseLabel(me, p2label_def->value, p2label, document);
	
	label_size = (int32_t) strlen(print_label);
	if (labelBuffer != NULL && bufferSize != 0)
	{
		if (bufferSize < label_size)
		{
			copy_size = bufferSize;
		}
		else
		{
			copy_size = label_size;
		}
#ifndef _CVI_
		strncpy_s(labelBuffer, bufferSize, print_label, copy_size);
#else
		strncpy(labelBuffer, print_label, copy_size);
#endif
		labelBuffer[copy_size] = '\0';
	}
	else
	{
		;
	}
	
Error:
	if (document)
		CA_DiscardObjHandle(document);
	
	if (print_label)
		free(print_label);
	
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__	"labels_printLabel"
static SElExceptionPtr labels_printLabel(
	SLabelListPtr	me,
	SPrinterPtr		printer,
	const char*		labelId,
	int32_t			productId,
	const char*		parameters
)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr = S_OK;
	VARIANT_BOOL	is_xml_success = 1;
	SLabelPtr		p2label = (SLabelPtr)me->LabelList;
	SLabelPtr		new_label = NULL;
	SEParamPtr		p2label_def = NULL;
	CAObjHandle		document = 0;
	char*			print_label = NULL;
    char     	    Tex_Label[3000]="";

	
	if (printer == NULL)
	{
		/* TODO: Change it */
		EXCTHROW(-1, "Printer Not Specified");
	}
		
	/* Find correct label */
	while (p2label != NULL)
	{
		if (p2label->productId == productId)
			break;

		p2label = p2label->next;
	}
	
	if (p2label == NULL)
	{
		new_label = (SLabelPtr) calloc(1, sizeof(SLabel));
		EXCCHECKALLOC( new_label);
		new_label->next = NULL;
		new_label->productId = productId;
				
		/* Get new parameters from database */
		if(productId > 0)
		{
			EXCCHECK( ((SDBSPtr)me->pDBS)->Export->GetParameter(((SDBSPtr)me->pDBS)->Export, 
																productId, 0,
																&(new_label->parameters),
																&(new_label->paramSize)));
		}
		
		if (me->LabelList == NULL)
		{
			me->LabelList = (void*)new_label;
		}
		else
		{
			p2label = (SLabelPtr)me->LabelList;
			while (p2label->next)
			{
				p2label = p2label->next;
			}
			
			p2label->next = new_label;
		}
		p2label = new_label;
	}
	
	p2label_def = FindParameterByName(p2label->parameters, p2label->paramSize, labelId);
	
	if (p2label_def==NULL 
		|| p2label_def->value==NULL 
		|| 0==strlen(p2label_def->value))
	{
		EXCTHROW(-1, "Definition étiquette dans la base des données n'est pas trouvé");
	}
	
	/* extraie fichier label de repertoire */
	FindLabelID(p2label_def->value, Tex_Label);

	if (strlen(Tex_Label) == 0) 
	{
		EXCTHROW(-1, "Fichier étiquette n'est pas trouvé");
	}
	
	/* parse label for print and print */
	if(parameters!=NULL)
	{
		EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &document));
		EXCCHECKMSXML( MSXML_IXMLDOMDocumentloadXML (document, NULL, parameters, &is_xml_success)); //convert String to XML
	}
	print_label = ParseLabel(me, Tex_Label, p2label, document);

	//printf (print_label);
	//printf ("\r\n");

	EXCCHECK( printer->Write(printer, print_label));
	
Error:
	if (document)
		CA_DiscardObjHandle(document);
	
	if (print_label)
		free(print_label);
	
	EXCRETHROW(pexception);
}

/*****************************************************************************
 *****************************************************************************
 *****************************************************************************/
/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__	"ParseLabel"
static char* ParseLabel(
	SLabelListPtr	me,
	const char*		labelString,
	SLabelPtr		productParams,
	CAObjHandle		parameters
)
{
	char*		output = NULL;
	char*		tmp_output = NULL;
	char*		p2variable = NULL;
	char*		p2name = NULL;
	char*		name = NULL;
	char*		replace = NULL;
	int32_t		buffer_size = 0;
	
	if (labelString == NULL)
		return NULL;
	
	if ((p2variable = strpbrk(labelString, "@%")) == NULL)
	{
		int32_t	size = strlen(labelString) + 1;

		/* None variable or function found */
		output = malloc(size);
		strcpy_s(output, size, labelString);
	}
	else
	{
		buffer_size = (int32_t)(p2variable - labelString);
		if (*p2variable == '@')
		{
			if (*(p2variable + 1) == '@')
			{
				/* Copy only one '@' character */
				buffer_size++;
				p2variable += 2;
			}
			else
			{
				if (*(p2variable + 1) != '[')
				{
					/* TODO: log invalid variable used */
					/* only copy invalid string to label */
					buffer_size += 2;
					p2variable += 2;
				}
				else
				{
					/* Variable found */
					p2variable += 2;
					p2name = p2variable;
					if ((p2variable = strchr(p2variable, ']')) == NULL)
					{
						/* TODO: log invalid variable used */
						/* only copy invalid string to label */
						buffer_size += 2;
						p2variable = p2name;
					}
					else
					{
						/* TODO: Get variable name @[name] */
						int32_t	name_size = (int32_t)(p2variable - p2name);
						
						name = (char*) malloc((name_size + 1) * sizeof(char));
						
						/* Copy variable name string */
#ifndef _CVI_
						strncpy_s(name, name_size + 1, p2name, name_size);
#else
						strncpy(name, p2name, name_size);
#endif
						name[name_size] = '\0';
						
						/* TODO: Replace variable string */
						replace = ReplaceVariable(me, name, productParams, parameters);
						
						free(name);
						p2variable++;
					}
				}
			}
		}
		else if (*p2variable == '%' && *(p2variable + 1) != '%')
		{
			char*	p2parameters = NULL;
			char*	p2end_param = NULL;
			
			/* Function found */
			p2variable++;
			if ((p2parameters = strchr(p2variable, '(')) != NULL)
			{
				p2parameters++;
				if ((p2end_param = strchr(p2parameters, ')')) != NULL)
				{
					*p2end_param = '\0';
					replace = ParseLabel(me, p2parameters, productParams, parameters);
					*p2end_param = ')';
					p2variable = p2end_param + 1;
				}
				else
				{
					buffer_size++;
					/* TODO: invalid function definition */
				}
			}
			else
			{
				buffer_size++;
				/* TODO: invalid function definition */
			}
		}
		else
		{
			p2variable++;
			buffer_size++;
		}
		
		/* call recursive function */
		tmp_output = ParseLabel(me, p2variable, productParams, parameters);
		
		/* copy output label */
		output = BuildOutputString(labelString, buffer_size, replace, tmp_output);
		free(replace);
		free(tmp_output);
	}
	return output;
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__	"BuildOutputString"
static char* BuildOutputString(
	const char*	firstPart,
	int32_t		sizeOfFirstPart,
	const char*	secondPart,
	const char*	lastPart
)
{
	int32_t	second_size = 0,
			last_size = 0,
			output_size = 0;
	char*	output = NULL;
	char*	p2output = NULL;
	
	if (secondPart != NULL)
		second_size = (int32_t)strlen(secondPart);
	
	if (lastPart != NULL)
		last_size = (int32_t)strlen(lastPart);
	
	output_size = sizeOfFirstPart + second_size + last_size + 1;
	
	output = (char*) malloc(output_size * sizeof(char));
	memset(output, 0, output_size);
	p2output = output;
	
	if (firstPart != NULL)
	{
#ifndef _CVI_
		strncpy_s(p2output, output_size, firstPart, sizeOfFirstPart);
#else
		strncpy(p2output, firstPart, sizeOfFirstPart);
#endif
		p2output += sizeOfFirstPart;
		output_size -= sizeOfFirstPart;
	}
	
	if (secondPart != NULL)
	{
#ifndef _CVI_
		strcpy_s(p2output, output_size, secondPart);
#else
		strcpy(p2output, secondPart);
#endif
		p2output += second_size;
		output_size -= second_size;
	}
	
	if (lastPart != NULL)
	{
#ifndef _CVI_
		strcpy_s(p2output, output_size, lastPart);
#else
		strcpy(p2output, lastPart);
#endif
	}

	return output;
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__	"ReplaceVariable"
static char* ReplaceVariable(
	SLabelListPtr	me,
	const char*		variableName,
	SLabelPtr		productParams,
	CAObjHandle		parameters
)
{
	int32_t			i = 0,
					size = 0;
	HRESULT			hr = S_OK;
	SElExceptionPtr	pexception = NULL;
    char*			new_value = NULL;
    char*			p2param = NULL;
	CAObjHandle		xml_param = 0;
	CAObjHandle		xml_param2 = 0;
    char*			xml_value = NULL;
	
	if ((p2param = strchr(variableName, ':')) != NULL)
	{
		*p2param = '\0';
		p2param++;
	}
	
	
	if (!strcmp(variableName, LBL_VAR_SOH))
	{
		new_value = malloc(2 * sizeof(char));
		*new_value = LBL_CONST_SOH;
		*(new_value + 1) = '\0';
	}
	else if (!strcmp(variableName, LBL_VAR_STX))
	{
		new_value = malloc(2 * sizeof(char));
		*new_value = LBL_CONST_STX;
		*(new_value + 1) = '\0';
	}
	else if (!strcmp(variableName, LBL_VAR_ESC))
	{
		new_value = malloc(2 * sizeof(char));
		*new_value = LBL_CONST_ESC;
		*(new_value + 1) = '\0';
	}
	else if (!strcmp(variableName, "ACK"))
	{
		new_value = malloc(2 * sizeof(char));
		*new_value = LBL_CONST_ACK;
		*(new_value + 1) = '\0';
	}
	else if (!strcmp(variableName, "LF"))
	{
		new_value = malloc(2 * sizeof(char));
		*new_value = LBL_CONST_LF;
		*(new_value + 1) = '\0';
	}
	else if (!strcmp(variableName, "CR"))
	{
		new_value = malloc(2 * sizeof(char));
		*new_value = LBL_CONST_CR;
		*(new_value + 1) = '\0';
	}
	else if (!strcmp(variableName, "XON"))
	{
		new_value = malloc(2 * sizeof(char));
		*new_value = LBL_CONST_XON;
		*(new_value + 1) = '\0';
	}
	else if (!strcmp(variableName, "XOFF"))
	{
		new_value = malloc(2 * sizeof(char));
		*new_value = LBL_CONST_XOFF;
		*(new_value + 1) = '\0';
	}
	else if (!strcmp(variableName, "NAK"))
	{
		new_value = malloc(2 * sizeof(char));
		*new_value = LBL_CONST_NAK;
		*(new_value + 1) = '\0';
	}
	/* Get information from parameters string */
	else if (!strcmp(variableName, LBL_VAR_CONFIG) ||
			 !strcmp(variableName, LBL_VAR_LOW) ||
			 !strcmp(variableName, LBL_VAR_HIGH) ||
			 !strcmp(variableName, LBL_VAR_UNIT) ||
			 !strcmp(variableName, LBL_VAR_FN) ||
			 !strcmp(variableName, LBL_VAR_MV) ||
			 !strcmp(variableName, LBL_VAR_RTS) ||
			 !strcmp(variableName, LBL_VAR_DESC) ||
			 !strcmp(variableName, LBL_VAR_PALETT_ID) ||
			 !strcmp(variableName, LBL_VAR_SHIFT_ID))
	{
		char	xpath[512] = "";
		
#ifndef _CVI_
		sprintf_s(xpath, sizeof(xpath), "//LabelParameters/%s", variableName);
#else
		sprintf(xpath, "//LabelParameters/%s", variableName);
#endif
		
		EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, xpath, &xml_param));
		if (xml_param)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
		
			size = strlen(xml_value) + 1;
			new_value = malloc(size * sizeof(char));
#ifndef _CVI_
			strcpy_s(new_value, size, xml_value);
#else
			strcpy(new_value, xml_value);
#endif
		}
	}
else if (!strcmp(variableName, LBL_VAR_SAPMN))
	{
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/SAPMN", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				CA_DiscardObjHandle(xml_param);
				xml_param = 0;
					
				size = strlen(xml_value) + 1;
				new_value = malloc(size * sizeof(char));
				strcpy(new_value, xml_value);
			}
			
		}
	}
	
	else if (!strcmp(variableName, LBL_VAR_HW_IDENTIFIER))
	{
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/HW_Identifier", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				CA_DiscardObjHandle(xml_param);
				xml_param = 0;
					
				size = strlen(xml_value) + 1;
				new_value = malloc(size * sizeof(char));
				strcpy(new_value, xml_value);
			}
			
		}
	}
	else if (!strcmp(variableName, LBL_VAR_HWID))
	{
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/HWID", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				CA_DiscardObjHandle(xml_param);
				xml_param = 0;
					
				size = strlen(xml_value) + 1;
				new_value = malloc(size * sizeof(char));
				strcpy(new_value, xml_value);
			}
			
		}
	}
	
	else if (!strcmp(variableName, LBL_VAR_SAP_SUPPLIER))
	{
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/SAP_Supplier", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				CA_DiscardObjHandle(xml_param);
				xml_param = 0;
					
				size = strlen(xml_value) + 1;
				new_value = malloc(size * sizeof(char));
				strcpy(new_value, xml_value);
			}
			
		}
	}
		
	else if (!strcmp(variableName, LBL_VAR_SUPPLIER_SERIES))
	{
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/Supplier_Series", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				CA_DiscardObjHandle(xml_param);
				xml_param = 0;	
				size = strlen(xml_value) + 1;
				new_value = malloc(size * sizeof(char));
				strcpy(new_value, xml_value);
			}
			
		}
	}
	
	else if (!strcmp(variableName, LBL_VAR_VDE_VERSION))
	{
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/VDE_Version", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				CA_DiscardObjHandle(xml_param);
				xml_param = 0;	
				size = strlen(xml_value) + 1;
				new_value = malloc(size * sizeof(char));
				strcpy(new_value, xml_value);
			}
			
		}
	}
	
	else if (!strcmp(variableName, LBL_VAR_TEMPERATURE))
	{
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/Temperature", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				CA_DiscardObjHandle(xml_param);
				xml_param = 0;		
				size = strlen(xml_value) + 1;
				new_value = malloc(size * sizeof(char));
				strcpy(new_value, xml_value);
			}
			
		}
	}
	
	else if (!strcmp(variableName, LBL_VAR_NOMINAL_VOLTAGE))
	{
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/Nominal_Voltage", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				CA_DiscardObjHandle(xml_param);
				xml_param = 0;	
				size = strlen(xml_value) + 1;
				new_value = malloc(size * sizeof(char));
				strcpy(new_value, xml_value);
			}
			
		}
	}
	
	else if (!strcmp(variableName, LBL_VAR_NOMINAL_POWER))
	{
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/Nominal_Power", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				CA_DiscardObjHandle(xml_param);
				xml_param = 0;		
				size = strlen(xml_value) + 1;
				new_value = malloc(size * sizeof(char));
				strcpy(new_value, xml_value);
			}
			
		}
	}
	
	else if (!strcmp(variableName, LBL_VAR_SMM))
	{
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/SMM", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				CA_DiscardObjHandle(xml_param);
				xml_param = 0;
					
				size = strlen(xml_value) + 1;
				new_value = malloc(size * sizeof(char));
				strcpy(new_value, xml_value);
			}
			
		}
	}
	
	else if (!strcmp(variableName, LBL_VAR_NUM_SERIE_PFIN))
	{

		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/NUM_SERIE_PFIN", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				CA_DiscardObjHandle(xml_param);
				xml_param = 0;
					
				size = strlen(xml_value) + 1;
				new_value = malloc(size * sizeof(char));
				strcpy(new_value, xml_value);
			}
			
		}
	}
	
	else if (!strcmp(variableName, LBL_VAR_TIME))
	{
		char		time_string[65] = "";
		time_t		curr_time;
		struct tm*	ptime;
		struct tm	time_str;
		
		time(&curr_time);
#ifndef _CVI_
		localtime_s(&time_str, &curr_time);
		ptime = &time_str;
#else
		ptime = localtime(&curr_time);
#endif
		
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/Time", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode (xml_param, NULL, "Hours", &xml_param2));
				if (xml_param2)
				{
					EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
					CA_DiscardObjHandle(xml_param2);
					xml_param2 = 0;
				
					ptime->tm_hour = atoi(xml_value);
					CA_FreeMemory(xml_value);
					xml_value = NULL;
				}
				EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode (xml_param, NULL, "Minutes", &xml_param2));
				if (xml_param2)
				{
					EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
					CA_DiscardObjHandle(xml_param2);
					xml_param2 = 0;
				
					ptime->tm_min = atoi(xml_value);
					CA_FreeMemory(xml_value);
					xml_value = NULL;
				}
				EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode (xml_param, NULL, "Seconds", &xml_param2));
				if (xml_param2)
				{
					EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
					CA_DiscardObjHandle(xml_param2);
					xml_param2 = 0;
				
					ptime->tm_sec = atoi(xml_value);
					CA_FreeMemory(xml_value);
					xml_value = NULL;
				}
			}
		}
		
#ifndef _CVI_
		sprintf_s(time_string, sizeof(time_string), "%02d:%02d:%02d", ptime->tm_hour, ptime->tm_min, ptime->tm_sec);
#else
		sprintf(time_string, "%02d:%02d:%02d", ptime->tm_hour, ptime->tm_min, ptime->tm_sec);
#endif
		size = strlen(time_string) + 1;
		new_value = malloc(size * sizeof(char));
#ifndef _CVI_
		strcpy_s(new_value, size, time_string);
#else
		strcpy(new_value, time_string);
#endif
	}
	else if (!strcmp(variableName, LBL_VAR_HH))
	{
		char		time_string[65] = "";
		time_t		curr_time;
		struct tm*	ptime;
		struct tm	time_str;
		
		time(&curr_time);
#ifndef _CVI_
		localtime_s(&time_str, &curr_time);
		ptime = &time_str;
#else
		ptime = localtime(&curr_time);
#endif
		
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/Time/Hours", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				ptime->tm_hour = atoi(xml_value);
			}
		}
		
#ifndef _CVI_
		sprintf_s(time_string, sizeof(time_string), "%02d", ptime->tm_hour);
#else
		sprintf(time_string, "%02d", ptime->tm_hour);
#endif
		size = strlen(time_string) + 1;
		new_value = malloc(size * sizeof(char));
#ifndef _CVI_
		strcpy_s(new_value, size, time_string);
#else
		strcpy(new_value, time_string);
#endif
	}
	else if (!strcmp(variableName, LBL_VAR_NN))
	{
		char		time_string[65] = "";
		time_t		curr_time;
		struct tm*	ptime;
		struct tm	time_str;
		
		time(&curr_time);
#ifndef _CVI_
		localtime_s(&time_str, &curr_time);
		ptime = &time_str;
#else
		ptime = localtime(&curr_time);
#endif
		
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/Time/Minutes", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				ptime->tm_min = atoi(xml_value);
			}
		}
		
#ifndef _CVI_
		sprintf_s(time_string, sizeof(time_string), "%02d", ptime->tm_min);
#else
		sprintf(time_string, "%02d", ptime->tm_min);
#endif
		size = strlen(time_string) + 1;
		new_value = malloc(size * sizeof(char));
#ifndef _CVI_
		strcpy_s(new_value, size, time_string);
#else
		strcpy(new_value, time_string);
#endif
	}
	else if (!strcmp(variableName, LBL_VAR_SS))
	{
		char		time_string[65] = "";
		time_t		curr_time;
		struct tm*	ptime;
		struct tm	time_str;
		
		time(&curr_time);
#ifndef _CVI_
		localtime_s(&time_str, &curr_time);
		ptime = &time_str;
#else
		ptime = localtime(&curr_time);
#endif
		
		if ( parameters )
		{
		EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/Time/Seconds", &xml_param));
		if (xml_param)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
			ptime->tm_sec = atoi(xml_value);
		}
		}
		
#ifndef _CVI_
		sprintf_s(time_string, sizeof(time_string), "%02d", ptime->tm_sec);
#else
		sprintf(time_string, "%02d", ptime->tm_sec);
#endif
		size = strlen(time_string) + 1;
		new_value = malloc(size * sizeof(char));
#ifndef _CVI_
		strcpy_s(new_value, size, time_string);
#else
		strcpy(new_value, time_string);
#endif
	}
	else if (!strcmp(variableName, LBL_VAR_Y))
	{
		char		year_string[20] = "";
		char*		p2next_prm = NULL;
		char*		p2prm_value = NULL;
		time_t		curr_time;
		struct tm*	ptime;
		struct tm	time_str;
		
		time(&curr_time);
#ifndef _CVI_
		localtime_s(&time_str, &curr_time);
		ptime = &time_str;
#else
		ptime = localtime(&curr_time);
#endif
		
		if ( parameters )
		{
		EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/Date/Year", &xml_param));
		if (xml_param)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
			ptime->tm_year = atoi(xml_value) - 1900;
		}
		}
		
		/* Default year format */
#ifndef _CVI_
		sprintf_s(year_string, sizeof(year_string), "%04d", ptime->tm_year + 1900);
#else
		sprintf(year_string, "%04d", ptime->tm_year + 1900);
#endif
		
		/* parse params */
		while (p2param != NULL)
		{
			if ((p2next_prm = strpbrk(p2param, ",;")) != NULL)
			{
				*p2next_prm = '\0';
				p2next_prm++;
			}
			
			if ((p2prm_value = strchr(p2param, '=')) != NULL)
			{
				*p2prm_value = '\0';
				p2prm_value++;
			}
			
			if (!strcmp(p2param, LBL_PRM_YEAR_SIZE))
			{
				if (p2prm_value != NULL)
				{
					int32_t	fmt_size = atoi(p2prm_value);
					
					switch (fmt_size)
					{
					case 1:
#ifndef _CVI_
						sprintf_s(year_string, sizeof(year_string), "%d", (ptime->tm_year + 1900) % 10);
#else
						sprintf(year_string, "%d", (ptime->tm_year + 1900) % 10);
#endif
						break;
					
					case 2:
#ifndef _CVI_
						sprintf_s(year_string, sizeof(year_string), "%02d", (ptime->tm_year + 1900) % 100);
#else
						sprintf(year_string, "%02d", (ptime->tm_year + 1900) % 100);
#endif
						break;
					}
				}
			}
			else if (!strcmp(p2param, LBL_PRM_CODE36))
			{
				int32_t	rest = (((ptime->tm_year + 1900) % 100) % 36);
				
#ifndef _CVI_
				sprintf_s(year_string, sizeof(year_string), "%c", (rest < 10) ? (rest + '0') : ((rest - 10) + 'A'));
#else
				sprintf(year_string, "%c", (rest < 10) ? (rest + '0') : ((rest - 10) + 'A'));
#endif
			}
			else if (!strcmp(p2param, LBL_PRM_YEAR_CODE))
			{
				if (p2prm_value != NULL)
				{
					char*	p2code = NULL;
					int32_t	first_year;
					
					if ((p2code = strchr(p2prm_value, '-')) != NULL)
					{
						*p2code = '\0';
						p2code++;
						first_year = atoi(p2prm_value);
#ifndef _CVI_
						sprintf_s(year_string, sizeof(year_string), "%c", ((ptime->tm_year + 1900) - first_year) + *p2code);
#else
						sprintf(year_string, "%c", ((ptime->tm_year + 1900) - first_year) + *p2code);
#endif
					}
					else
					{
						/* TODO: invalid coding */
					}
				}
				else
				{
					/* TODO: missing value */
				}
			}
			else
			{
				/* TODO: unknown parameter */
			}
			
			p2param = p2next_prm;
		}

		size = strlen(year_string) + 1;
		new_value = malloc(size * sizeof(char));
		strcpy_s(new_value, size, year_string);
	}
	else if (!strcmp(variableName, LBL_VAR_MM))
	{
		char		month_string[20] = "";
		time_t		curr_time;
		struct tm*	ptime;
		struct tm	time_str;
		
		time(&curr_time);
#ifndef _CVI_
		localtime_s(&time_str, &curr_time);
		ptime = &time_str;
#else
		ptime = localtime(&curr_time);
#endif
		
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/Date/Month", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				ptime->tm_mon = atoi(xml_value) - 1;
			}
		}
		
#ifndef _CVI_
		sprintf_s(month_string, sizeof(month_string), "%02d", ptime->tm_mon + 1);
#else
		sprintf(month_string, "%02d", ptime->tm_mon + 1);
#endif
		size = strlen(month_string) + 1;
		new_value = malloc(size * sizeof(char));
		strcpy_s(new_value, size, month_string);
	}
	else if (!strcmp(variableName, LBL_VAR_DD))
	{
		char		day_string[20] = "";
		time_t		curr_time;
		struct tm*	ptime;
		struct tm	time_str;
		
		time(&curr_time);
#ifndef _CVI_
		localtime_s(&time_str, &curr_time);
		ptime = &time_str;
#else
		ptime = localtime(&curr_time);
#endif
		
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/Date/Day", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				ptime->tm_mday = atoi(xml_value);
			}
		}
		
#ifndef _CVI_
		sprintf_s(day_string, sizeof(day_string), "%02d", ptime->tm_mday);
#else
		sprintf(day_string, "%02d", ptime->tm_mday);
#endif
		size = strlen(day_string) + 1;
		new_value = malloc(size * sizeof(char));
		strcpy_s(new_value, size, day_string);
	}
	

	
	else if (!strcmp(variableName, LBL_VAR_DY36))
	{
		char		day[20] = "";
		struct tm*	ptime;
		struct tm	time_str;
		time_t		curr_time;
		int32_t		yday = 0,
					rest = 0;
		
		time(&curr_time);
		ptime = localtime(&curr_time);
		
		
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/Date/Day", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				ptime->tm_mday = atoi(xml_value);
			}
		}
	
		if ( parameters )
		{
			EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/Date/Month", &xml_param));
			if (xml_param)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
				ptime->tm_mon = atoi(xml_value) - 1;
			}
		}
			
		if ( parameters )
		{
		EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/Date/Year", &xml_param));
		if (xml_param)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
			ptime->tm_year = atoi(xml_value) - 1900;
		}
		}
				
		
		yday = NumeroJour(ptime->tm_mon+1, ptime->tm_mday, ptime->tm_year + 1900);
		sprintf (day, "%03d", yday);
		size = strlen(day) + 1;
		new_value = malloc(size * sizeof(char));
		strcpy_s(new_value, size, day);
	}
	else if (!strcmp(variableName, LBL_VAR_SERIAL))
	{
		char	serial_string[20] = "";
		char	format_string[20] = "%09d";
		char*	p2next_prm = NULL;
		char*	p2prm_value = NULL;
		int32_t	serial = 0;
		
		EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (parameters, NULL, "//LabelParameters/SerialNumber", &xml_param));
		if (xml_param)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (xml_param, NULL, &xml_value));
			serial = atol(xml_value);
		}
		
		/* parse params */
		while (p2param != NULL)
		{
			if ((p2next_prm = strpbrk(p2param, ",;")) != NULL)
			{
				*p2next_prm = '\0';
				p2next_prm++;
			}
			
			if ((p2prm_value = strchr(p2param, '=')) != NULL)
			{
				*p2prm_value = '\0';
				p2prm_value++;
			}
			
			if (!strcmp(p2param, LBL_PRM_SERIAL_FMT))
			{
				if (p2prm_value != NULL)
#ifndef _CVI_
					sprintf_s(format_string, sizeof(format_string), "%%%sd", p2prm_value);
#else
					sprintf(format_string, "%%%sd", p2prm_value);
#endif
				else
				{
					/* TODO: missing value */
				}
			}
			else if (!strcmp(p2param, LBL_PRM_CODE36))
			{
				int32_t		coding_size = 3,
							number = serial,
							rest = 0;
				
				if (p2prm_value != NULL)
				{
					coding_size = atol(p2prm_value);
				}
				
				memset(format_string, 0, 20);
				for (i = coding_size - 1; i >= 0; i--)
				{
					rest = number % 36;
					number /= 36;
					format_string[i] = (char) ((rest < 10) ? (rest + '0') : ((rest - 10) + 'A'));
				}
			}
			else
			{
				/* TODO: unknown parameter */
			}
			
			p2param = p2next_prm;
		}
		
		if (strchr(format_string, '%'))
#ifndef _CVI_
			sprintf_s(serial_string, sizeof(serial_string), format_string, serial);
#else
			sprintf(serial_string, format_string, serial);
#endif
		else
#ifndef _CVI_
			sprintf_s(serial_string, sizeof(serial_string), format_string);
#else
			sprintf(serial_string, format_string);
#endif
		size = strlen(serial_string) + 1;
		new_value = malloc(size * sizeof(char));
#ifndef _CVI_
		strcpy_s(new_value, size, serial_string);
#else
		strcpy(new_value, serial_string);
#endif
	}
	else  //from database!
	{
		/* Rest of label definition variables */
		SEParamPtr	p2param = NULL;
		
		if (productParams != NULL && productParams->parameters != NULL && productParams->paramSize != 0)
			p2param = FindParameterByName(productParams->parameters, productParams->paramSize, variableName);
		
		if (p2param == NULL)
		{
			new_value = NULL;
		}
		else
		{
			size = strlen(p2param->value) + 1;
			new_value = malloc(size * sizeof(char));
			
			memset(new_value, 0x00, size);
#if 0    /* formerly excluded lines */
			
		   if (!strcmp(variableName, "PSA_FPN"))
		   {
 			   CopyBytes(new_value,0, p2param->value, 0, 8);
		   	   sprintf (new_value, "%sZD", new_value);
		   }
		   
		   else 
#endif   /* formerly excluded lines */
			strcpy_s(new_value, size, p2param->value);
		}
	}

Error:
	if (xml_param)
		CA_DiscardObjHandle(xml_param);
	
	if (xml_param2)
		CA_DiscardObjHandle(xml_param2);
	
	if (xml_value)
		CA_FreeMemory(xml_value);
	
	return new_value;
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__	"FindParameterIndexByName"
static int32_t FindParameterIndexByName(
	SEParamPtr	parameters,
	int32_t		paramSize,
	const char*	paramName
)
{
	int32_t	index = 0;
	
	for (index = 0; index < paramSize; index++)
	{
		if (!strcmp(parameters[index].name, paramName))
			return index;
	}
	
	return -1;
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__	"FindParameterByName"
static SEParamPtr FindParameterByName(
	SEParamPtr	parameters,
	int32_t		paramSize,
	const char*	paramName
)
{
	int32_t		index = 0;
	SEParamPtr	p2param = parameters;
	
	for (index = 0; index < paramSize; index++, p2param++)
	{
		if (!strcmp(p2param->name, paramName))
			return p2param;
	}
	
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__	"FindLabelID"
static void FindLabelID(
	const char*	path,
    char*	Label_Text
)
{
   	int File_Handle;
	char Ligne_Lue [2000 + 1];


	File_Handle = OpenFile (path, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);
	
	while (ReadLine (File_Handle, Ligne_Lue, -1) >= 0)
	{
		RemoveSurroundingWhiteSpace (Ligne_Lue);
		strcat (Label_Text, Ligne_Lue);
	}

	
	CloseFile (File_Handle);

}

#pragma warning( pop)
