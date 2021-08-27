#include "dbs.h"
#include "dbs_lineexport.h"
#include <elstring.h>
#include <stdio.h>
#include <odbc/elodbc.h>
#include "msxmldom2.h"
#include "dbs_define_local.h"

#define DBS_NUMBER_OF_GROUPS         	5
	
static SElExceptionPtr export_GetProcessSeq(
	struct _SDBSLineExport* me, 
	int32_t product_id,
	SELineProcessPtr*	seq, 
	int32_t* seqSize
);   
static SElExceptionPtr export_FreeProcessSeq(
	struct _SDBSLineExport* me, 
	SELineProcessPtr* seq, 
	int32_t seqSize
);
static SElExceptionPtr export_GetProcessSeqXML(
	struct _SDBSLineExport* me, 
	int32_t product_id,
	const char* path
);
static SElExceptionPtr export_GetProcessSeqXML2(
	struct _SDBSLineExport* me, 
	int32_t product_id,
	const char* path
);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbslineexport_new"
SElExceptionPtr dbslineexport_new(SDBSLineExportPtr* pDBSLineExportPtr, void* pDBS)
{
	SElExceptionPtr    pexception = NULL; 
	SDBSLineExportPtr	me = NULL;

	me = calloc(1,sizeof(SDBSLineExport));
	EXCCHECKALLOC(me);
	
	if(pDBSLineExportPtr) *pDBSLineExportPtr = me;

	me->GetProcessSeq = export_GetProcessSeq;
	me->FreeProcessSeq = export_FreeProcessSeq; 
	me->GetProcessSeqXML = export_GetProcessSeqXML;
	me->GetProcessSeqXML2 = export_GetProcessSeqXML2;
	
	me->pdbs = pDBS;

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbslineexport_delete"
SElExceptionPtr dbslineexport_delete(SDBSLineExportPtr* pDBSLineExportPtr)
{
	if (pDBSLineExportPtr && *pDBSLineExportPtr)
	{
		free(*pDBSLineExportPtr);
		*pDBSLineExportPtr = NULL;
	}

/* Error: */
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_FreeProcessSeq"
static SElExceptionPtr export_FreeProcessSeq(
	struct _SDBSLineExport* me,
	SELineProcessPtr*	seq, 
	int32_t				seqSize
)
{
	SElExceptionPtr	pexception = NULL;
	
	if(seq && *seq)
	{
		SELineProcessPtr pseq = *seq;
		int i, j;

 		for(i=0; i<seqSize; i++)
		{
			if(pseq[i].step)
			{
				for(j=0; j<pseq[i].stepSize; j++)
				{
					free(pseq[i].step[j].param);
				}
				free(pseq[i].step);
			}
		}
		free(*seq);
		*seq = NULL;
	}

/* Error: */
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_GetParameter"
static SElExceptionPtr export_GetParameter(
	struct _SDBSLineExport* me,
	int32_t			product_id, 
	SELineStepPtr	pstep, 
	int32_t*		pid, 
	int32_t			pidSize
)
{
	SElExceptionPtr	pexception = NULL;    
	int				i, j;
	int				param_count = 0;
	SELineParamPtr	pparam = NULL; 
	
	/* precount parameters */
	for(i=0; i<PLINEPARAM->LineParamSize; i++)   			
	{
		if( (PLINEPARAM->LineParam[i].property&PROPERTY_VALID)>0
			&& pstep->fnc_nb == PLINEPARAM->LineParam[i].fnc_nb)
		{
			for(j=pidSize-1; j>=0; j--)
			{
				if(	pid[j]==PLINEPARAM->LineParam[i].product_id )
				{
					param_count++;
					break;
				}
			}
		}
	}
	
	if(param_count==0)
	{
		if(pstep) pstep->param = NULL;
		if(pstep) pstep->paramSize = 0;
		goto Error;
	}

	pparam = (SELineParamPtr)calloc(param_count, sizeof(SELineParam));
	
	/* list parameters */
	param_count = 0;
	for(i=0; i<PLINEPARAM->LineParamSize; i++)   			
	{
		if( (PLINEPARAM->LineParam[i].property&PROPERTY_VALID)>0
			&& pstep->fnc_nb == PLINEPARAM->LineParam[i].fnc_nb)
		{
			for(j=pidSize-1; j>=0; j--)
			{
				if(pid[j]==PLINEPARAM->LineParam[i].product_id)
				{
					if(param_count>0 && 0==strcmp(PLINEPARAM->LineParam[i].name, pparam[param_count-1].name))
					{
						param_count--;
					}
					strncpy(pparam[param_count].name, PLINEPARAM->LineParam[i].name, DBS_RECORD_LENGHT_NAME);
					strncpy(pparam[param_count].value, PLINEPARAM->LineParam[i].value, DBS_RECORD_LENGHT_VALUE_EXT);
					param_count++;
					break;
				}
			}
		}
	}
	
	if(pstep) pstep->paramSize = param_count;
	if(pstep) pstep->param = pparam;
		
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_GetStep"
static SElExceptionPtr export_GetStep(
	struct _SDBSLineExport* me, 
	int32_t				product_id, 
	SELineProcessPtr	pprocess, 
	int32_t*			pid, 
	int32_t				pidSize
)   
{
	SElExceptionPtr	pexception = NULL;    
	int				i, j;
	int				step_count = 0;
	SELineStepPtr	pstep = NULL;
	
	/* precount steps */ 
	for(i=0; i<PSTEP->LineStepSize; i++)   			
	{
		for(j=pidSize-1; j>=0; j--)
		{
			if( (PSTEP->LineStep[i].property&PROPERTY_VALID)>0
				&& pid[j]==PSTEP->LineStep[i].product_id
				&& pprocess->process_nb == PSTEP->LineStep[i].process_nb)
			{
				step_count++;
				break;
			}
		}
	}

	if(step_count==0)
	{
		if(pprocess) pprocess->step = NULL;
		if(pprocess) pprocess->stepSize = 0;
		goto Error;
	}

	pstep = (SELineStepPtr)calloc(step_count, sizeof(SELineStep));
	
	/* list process */ 
	step_count = 0;
	for(i=0; i<PSTEP->LineStepSize; i++)   			
	{
		for(j=pidSize-1; j>=0; j--)
		{
			if(	(PSTEP->LineStep[i].property&PROPERTY_VALID)>0								    
				&& pid[j]==PSTEP->LineStep[i].product_id
				&& pprocess->process_nb == PSTEP->LineStep[i].process_nb)
			{
				if(	step_count>0
					&& 0==strcmp(PSTEP->LineStep[i].name, "@skip")
					&& PSTEP->LineStep[i].fnc_nb == pstep[step_count-1].fnc_nb)
				{
					step_count--;
				}
				else
				{
					strncpy(pstep[step_count].name, PSTEP->LineStep[i].name, DBS_RECORD_LENGHT_NAME);
					pstep[step_count].fnc_nb = PSTEP->LineStep[i].fnc_nb;
					
					/* TODO: free parameters if there are*/

					EXCCHECK(export_GetParameter(me, product_id, &pstep[step_count], pid, pidSize));
					step_count++; 
				}
				break;
			}
		}
	}
	
	if(pprocess) pprocess->step = pstep;
	if(pprocess) pprocess->stepSize = step_count;
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_GetProcessSeq"
static SElExceptionPtr export_GetProcessSeq(
	struct _SDBSLineExport* me, 
	int32_t		product_id, 
	SELineProcessPtr*	seq, 
	int32_t*	seqSize
)   
{
	SElExceptionPtr	pexception = NULL;    
	int32_t			pid[DBS_NUMBER_OF_GROUPS];
	int32_t			pidSize = DBS_NUMBER_OF_GROUPS;
	int				i, j;
	int				process_count = 0;
	SELineProcessPtr	pprocess = NULL;
	
	/* select specified product tree branch */
	EXCCHECK( PPRODUCT->ProductGetIdTreeUp(PPRODUCT, product_id, pid, &pidSize));
	
	/* precount process */ 
	for(i=0; i<PPROCESS->LineProcessSize; i++)   			
	{
		for(j=pidSize-1; j>=0; j--)
		{
			if( (PPROCESS->LineProcess[i].property&PROPERTY_VALID)>0
				&&pid[j]==PPROCESS->LineProcess[i].product_id)
			{
				process_count++;
				break;
			}
		}
	}

	if(process_count==0)
	{
		if(seq) *seq = NULL;
		if(seqSize) *seqSize = 0;
		goto Error;
	}

	pprocess = (SELineProcessPtr)calloc(process_count, sizeof(SELineProcess));
	
	/* list process */ 
	process_count = 0;
	for(i=0; i<PPROCESS->LineProcessSize; i++)   			
	{
		for(j=pidSize-1; j>=0; j--)
		{
			if(	(PPROCESS->LineProcess[i].property&PROPERTY_VALID)>0								    
				&& pid[j]==PPROCESS->LineProcess[i].product_id )
			{
				if(	process_count>0
					&& 0==strcmp(PPROCESS->LineProcess[i].name, "@skip")
					&& PPROCESS->LineProcess[i].process_nb == pprocess[process_count-1].process_nb)
				{
					process_count--;
				}
				else
				{
					strncpy(pprocess[process_count].name, PPROCESS->LineProcess[i].name, DBS_RECORD_LENGHT_NAME);
					strncpy(pprocess[process_count].description, PPROCESS->LineProcess[i].description, DBS_RECORD_LENGHT_NAME);
					pprocess[process_count].process_nb = PPROCESS->LineProcess[i].process_nb;
					
					/* TODO: free steps if there are*/

					EXCCHECK(export_GetStep(me, product_id, &pprocess[process_count], pid, pidSize));
					process_count++; 
				}
				break;
			}
		}
	}
	
	if(seqSize) *seqSize = process_count;
	if(seq) *seq = pprocess;
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SolveIncludes"
static SElExceptionPtr SolveIncludes(const char* fileName)
{
	SElExceptionPtr	pexception = NULL;  
	int32_t			error = 0;
	HRESULT         hr = 0;
	CAObjHandle     pxml  = 0;
	CAObjHandle     pseq = 0;
	CAObjHandle     pstep = 0;
	CAObjHandle     pstep_new = 0;
	CAObjHandle     pstep_del = 0;
	CAObjHandle     pstep_list = 0;
	CAObjHandle     pprocess = 0;
	CAObjHandle     pprocess_del = 0;
	CAObjHandle     pparameter = 0;   
	const char*     pfile_name = (fileName)? fileName : "c:\\line_seq.xml";
	VBOOL         	is_xml_success;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	VARIANT			node_var = CA_VariantEmpty();
	char*			ptext = NULL;
	char			path[512];
	ERRORINFO		errorInfo = {0};

	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(&pfile_name_var, pfile_name));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//assy_line/seq", &pseq));
	if(pseq)
	{	
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode(pseq, NULL, "process[not(@ws)]/step[@fn='INCLUDE']", &pstep));
		while(pstep)
		{
			/* ref node */
			CA_VariantSetObjHandle(&node_var, pstep, CAVT_UNKNOWN);

			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode(pstep, NULL, "@path", &pparameter));
			if(pparameter)
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext(pparameter, NULL, &ptext));
				if(ptext)
				{
					sprintf(path, "process[@ws='%s']", ptext);
					CA_FreeMemory(ptext); 
					ptext = NULL;
				}
				CA_DiscardObjHandle(pparameter);
				pparameter = 0;
			}
			else
				EXCTHROW(-1, "INCLUDE step without \"path\" specification!");

			/* delete included process */
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode(pseq, &errorInfo, path, &pprocess));
			if(pprocess==0)
			{
				char serror[512];

				sprintf(serror, "INCLUDE step with wrong path specification: \"%s\"", path);
				EXCTHROW(-1, serror);
			}
			EXCCHECKMSXML( MSXML_IXMLDOMNoderemoveChild(pseq, NULL, pprocess, &pprocess_del));
			CA_DiscardObjHandle(pprocess);
			pprocess = 0;

			/* insert steps from deleted process */
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode(pseq, NULL, "process[not(@ws)]", &pprocess));
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectNodes(pprocess_del, NULL, "step", &pstep_list));
			if(pprocess && pstep_list)
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset(pstep_list, NULL));
				EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode(pstep_list, NULL, &pstep_del));
				while(pstep_del)
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeinsertBefore(pprocess, &errorInfo, pstep_del, node_var, &pstep_new));
					CA_DiscardObjHandle(pstep_new);
					pstep_new = 0;
					CA_DiscardObjHandle(pstep_del); 
					pstep_del = 0;
					EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode(pstep_list, NULL, &pstep_del));
				};
				CA_DiscardObjHandle(pstep_list);
				pstep_list = 0;
			}
			CA_DiscardObjHandle(pprocess_del);
			pprocess_del = 0;

			/* remove step INCLUDE */
			EXCCHECKMSXML( MSXML_IXMLDOMNoderemoveChild(pprocess, NULL, pstep, &pstep_new));
			CA_DiscardObjHandle(pstep_new);
			pstep_new = 0;
			CA_DiscardObjHandle(pstep);
			pstep = 0;
			CA_DiscardObjHandle(pprocess);
			pprocess = 0;

			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode(pseq, NULL, "process[not(@ws)]/step[@fn='INCLUDE']", &pstep));
		}
	};

	EXCCHECKMSXML( MSXML_IXMLDOMDocumentsave(pxml, NULL, pfile_name_var));

Error:
	CA_VariantClear(&pfile_name_var);
	CA_VariantClear(&node_var);
	if (pstep) CA_DiscardObjHandle(pstep);
	if (pstep_new) CA_DiscardObjHandle(pstep_new);
	if (pstep_del) CA_DiscardObjHandle(pstep_del);
	if (pstep_list) CA_DiscardObjHandle(pstep_list);
	if (pprocess) CA_DiscardObjHandle(pprocess);
	if (pprocess_del) CA_DiscardObjHandle(pprocess_del);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pseq) CA_DiscardObjHandle(pseq);
	if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_GetProcessSeqXML"
static SElExceptionPtr export_GetProcessSeqXML(
	struct _SDBSLineExport* me, 
	int32_t product_id,
	const char* path
)
{
	SElExceptionPtr	pexception = NULL;   
	SELineProcessPtr	pprocess = NULL, pprocess_org = NULL;
	int32_t			process_size = 0;
	SELStringPtr	elstring = NULL;
	FILE			*file = NULL;
	char			*label = NULL;
	int32_t			i, j, k;
	char			desc[256];

	EXCCHECK( me->GetProcessSeq(me, product_id, &pprocess, &process_size));
	pprocess_org = pprocess;

	EXCCHECK( elstring_new("<?xml version=\"1.0\" encoding=\"windows-1250\"?>\n", &elstring));
	EXCCHECK( elstring->Append(elstring, "<assy_line>\n"));

	EXCCHECK( PPRODUCT->ProductGetIdName(PPRODUCT, product_id, &label));
	EXCCHECK( elstring->Format(elstring, "	<seq product_id=\"%d\" name=\"%s\">\n", product_id, label));

	for(i=0; i<process_size; i++,pprocess++)
	{
		if(strlen(pprocess->description)>0)
			sprintf(desc, "ws=\"%s\"", pprocess->description);
		else
			desc[0] = '\0';

		EXCCHECK( elstring->Format(elstring, "		<%s %s>\n", pprocess->name, desc));
	
		for(j=0; j<pprocess->stepSize; j++)
		{
			EXCCHECK( elstring->Format(elstring, "			<step fn=\"%s\" ", pprocess->step[j].name));
			
			for(k=0; k<pprocess->step[j].paramSize; k++)
			{
				EXCCHECK( elstring->Format(elstring, "%s=\"%s\" ", pprocess->step[j].param[k].name, pprocess->step[j].param[k].value));
			}

			EXCCHECK( elstring->Append(elstring, "/>\n"));
		}

		EXCCHECK( elstring->Format(elstring, "		</%s>\n", pprocess->name));
	}

	EXCCHECK( elstring->Append(elstring, "	</seq>\n"));
	EXCCHECK( elstring->Append(elstring, "</assy_line>\n"));

	if(path=='\0')
		file = fopen("c:\\line_seq.xml","w");
	else
		file = fopen(path,"w");
	
	if(file)
	{
		fwrite(elstring->GetPtr(elstring), 1, elstring->Size(elstring), file);
		fclose(file);
		EXCCHECK( SolveIncludes(path));
	}

Error:
	elstring_delete(&elstring);
	me->FreeProcessSeq(me, &pprocess_org, process_size);
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_GetProcessSeqXML2"
static SElExceptionPtr export_GetProcessSeqXML2(
	struct _SDBSLineExport* me, 
	int32_t product_id,
	const char* path
)
{
	SElExceptionPtr	pexception = NULL;   
	HRESULT			hr = 0;
	int32_t			error = 0;
	VBOOL			is_xml_success;
	ERRORINFO		err_info;
	SELineProcessPtr	pprocess = NULL, pprocess_org = NULL;
	int32_t			process_size = 0;
	SELStringPtr	elstring = NULL;
	int32_t			i, j, k;
	VARIANT			pfile_name_var = CA_VariantEmpty();
	CAObjHandle		pdoc = 0;
	CAObjHandle		pdoc2 = 0;
	CAObjHandle		pseq = 0;
	CAObjHandle		pseq_new = 0;
	CAObjHandle     pnode = 0;
	CAObjHandle		pseq_list = 0;
	CAObjHandle		pseq2 = 0;
	CAObjHandle		pseq2_del = 0;

	EXCCHECK( me->GetProcessSeq(me, product_id, &pprocess, &process_size));
	pprocess_org = pprocess;

	EXCCHECK( elstring_new("<seq>\n", &elstring));
	for(i=0; i<process_size; i++,pprocess++)
	{
		EXCCHECK( elstring->Format(elstring, "		<%s>\n", pprocess->name));
	
		for(j=0; j<pprocess->stepSize; j++)
		{
			EXCCHECK( elstring->Format(elstring, "			<%s ", pprocess->step[j].name));
			
			for(k=0; k<pprocess->step[j].paramSize; k++)
			{
				EXCCHECK( elstring->Format(elstring, "%s=\"%s\" ", pprocess->step[j].param[k].name, pprocess->step[j].param[k].value));
			}
			EXCCHECK( elstring->Append(elstring, "/>\n"));
		}
		EXCCHECK( elstring->Format(elstring, "		</%s>\n", pprocess->name));
	}
	EXCCHECK( elstring->Append(elstring, "</seq>\n"));

	/* open elstring xml doc */
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pdoc));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentloadXML(pdoc, &err_info, elstring->GetPtr(elstring), &is_xml_success));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pdoc, &err_info, "//seq", &pseq));

	/* open line_form.xml to append seq */
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pdoc2));
	EXCCHECKCVI( CA_VariantSetCString(&pfile_name_var, path));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pdoc2, NULL, pfile_name_var, &is_xml_success));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pdoc2, NULL, "//testseq", &pnode));
	if(pnode)
	{
		/* delete old seq if there is some */
		EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectNodes(pdoc2, NULL, "//seq", &pseq_list));
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset(pseq_list, NULL));
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode(pseq_list, NULL, &pseq2));
		while(pseq2)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNoderemoveChild(pnode, NULL, pseq2, &pseq2_del));
			CA_DiscardObjHandle( pseq2_del);
			pseq2_del = 0;
			CA_DiscardObjHandle( pseq2);
			pseq2 = 0;
			EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode(pseq_list, NULL, &pseq2));
		};

		/* append seq to line_form.xml */
		if(pseq)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeappendChild(pnode, NULL, pseq, &pseq_new));	
			EXCCHECKMSXML( MSXML_IXMLDOMDocumentsave(pdoc2, NULL, pfile_name_var));
		}
	}

Error:
	if (pexception)
	{
		FILE*		f;
#if 0
		CAObjHandle parser;
		long		code, line;
		char*		reason;
		
		MSXML_IXMLDOMDocumentGetparseError(pdoc, &err_info, &parser);
		MSXML_IXMLDOMParseErrorGeterrorCode(parser, &err_info, &code);
		MSXML_IXMLDOMParseErrorGetreason(parser, &err_info, &reason);
		MSXML_IXMLDOMParseErrorGetline(parser, &err_info, &line);

		CA_DiscardObjHandle(parser);
		CA_FreeMemory(reason);
#endif
		f = fopen("line_form_err.xml", "w");
		fprintf(f, elstring->GetPtr(elstring));
		fclose(f);
	}
	CA_VariantClear(&pfile_name_var);
	if (pseq_list) CA_DiscardObjHandle(pseq_list);
	if (pseq2) CA_DiscardObjHandle(pseq2);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pseq) CA_DiscardObjHandle(pseq);
	if (pseq_new) CA_DiscardObjHandle(pseq_new);
	if (pdoc) CA_DiscardObjHandle(pdoc);
	if (pdoc2) CA_DiscardObjHandle(pdoc2);
	elstring_delete(&elstring);
	me->FreeProcessSeq(me, &pprocess_org, process_size);
	EXCRETHROW( pexception); 
}