#include <msxmldom.h>
#include <guid.h>
#include <userint.h>
#include <language/language.h>
#include <utility.h>
#include <formatio.h>
#include <stdio.h>
#include "panel_fnc.h"




	
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LoadConfiguration"
SElExceptionPtr LoadConfiguration(
   const char*			fileName,
   const char*			spec,
   struct _SPanelItem* 	spanel,
   int 					item_number
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	HRESULT         hr = 0;
	CAObjHandle		pxml = 0;
	CAObjHandle     pnode = 0, 
					pnode2 = 0, 
					pnode2old = 0;
	CAObjHandle     pparameter = 0;
	const char*     pfile_name = (fileName) ? fileName : "c:\\application\\plugins\\panel.xml";
	VBOOL         	is_xml_success;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char*         	ptext= NULL;
	char			path[256], name_pict[256];
	int				i, index;  
	
	if(0!=strcmp(spanel[0].panel, spec))
	{
		EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
		EXCCHECKCVI( CA_VariantSetCString(&pfile_name_var, pfile_name));
		EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
		EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//plugin[@id='"PLUGIN_PANEL"']", &pnode));   
		
		for(i=1; i<item_number+1; i++)
		{
		
			strcpy(spanel[i-1].panel, spec);
		
			/* name */
			sprintf(path, "%s/item[@position='%d']/name", spec, i); 
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
			if(pparameter)
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
				if(ptext)
				{
					spanel[i-1].valid = TRUE;
					strcpy(spanel[i-1].name, gettext(ptext));
				}
		
				EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
				pparameter = 0;
				CA_FreeMemory(ptext);
				ptext=NULL;
			}	
			else
			{
				spanel[i-1].valid = FALSE;
				strcpy(spanel[i-1].name, "");
			}
			
			/* picture */
			sprintf(path, "%s/item[@position='%d']/pict", spec, i);
	        EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
			if ( pparameter )
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
				if(ptext)
				{	
					long size;

					sprintf(spanel[i-1].spict, PANEL_PICT, ptext);
					//sprintf(spanel[i-1].spict, "icons\\%s", name_pict);
					
				}
		
				EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
				pparameter = 0;
				CA_FreeMemory(ptext);
				ptext=NULL;
			}
			else
			{
				sprintf(spanel[i-1].spict, "");
			}
			
		/* save positon */
			sprintf(path, "%s/item[@position='%d']/save_position", spec, i);
	        EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
			if ( pparameter )
			{
				spanel[i-1].save_position = TRUE;
		
				EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
				pparameter = 0;
			}	
			else
			{
				spanel[i-1].save_position = FALSE; 	
			}
			
		/* rights */
			sprintf(path, "%s/item[@position='%d']/rights/@id", spec, i);
	        EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
			if ( pparameter )
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
				if(ptext)
				{	
					strcpy(spanel[i-1].rights, ptext);  		
				}
		
				EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
				pparameter = 0;
				CA_FreeMemory(ptext);
				ptext=NULL;
			}	
			else
			{
				strcpy(spanel[i-1].rights, "");    
			}
			
		/* events */
			sprintf(path, "%s/item[@position='%d']/event", spec, i);
		    EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pnode2));
			index = 0;
		
			while(pnode2!=0)
			{
		        EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@id", &pparameter));
				if ( pparameter )
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));

					if(ptext)
					{	
						strncpy(spanel[i-1].event[index], ptext, ELEVENT_MAX_NAME_LENGTH);	
					}

					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					CA_FreeMemory(ptext);
					ptext=NULL;
				}
				else
				{
					strcpy(spanel[i-1].event[index], "");
				}
			 	/* events data */
		        EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@data", &pparameter));
				if ( pparameter )
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));

					if(ptext)
					{	
						strncpy(spanel[i-1].data[index], ptext, MAX_EVENT_DATA_LENGTH);	
					}

					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					CA_FreeMemory(ptext);
					ptext=NULL;
				}
				else
				{
					strcpy(spanel[i-1].data[index], "");
				}
			
				pnode2old = pnode2;
				EXCCHECKMSXML( MSXML_IXMLDOMNodeGetnextSibling (pnode2old, NULL, &pnode2));
				EXCCHECKCVI( CA_DiscardObjHandle(pnode2old));
				pnode2old = 0;   
			
				index++;
			}
			spanel[i-1].event_nb = index; 
		}
	}
		
Error:
	CA_FreeMemory(ptext);
	CA_VariantClear(&pfile_name_var);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception);
} 
