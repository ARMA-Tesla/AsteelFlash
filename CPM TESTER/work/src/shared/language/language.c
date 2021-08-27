#include "language.h"
#include <userint.h>
#include <utility.h>
	
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LoadPanelWithLanguage"
/*!
 * This function loads specified panel by panelResourceId from specified file
 * to memory and set all langueage strings on the panel.
 *
 * \param   path      Specifies full UIR file path name, where is panel with
 *                      specified \p panelResourceId.
 * \param   panelResourceId   Defined constant that you assigned to the panel
 *                      in the User Interface Editor.
 * \param   table      Table with default language strings for all controls.
 *
 * \param   pPanelHandle Returns handle of the loaded panel.
 *
 * \return On success, LoadPanelWithLanguage() returns a zero (0).
 *         On failure it returns a negative error.
 */
SElExceptionPtr LoadPanelWithLanguage(
   int          	parentPanel,  
   const char*      path,
   int              panelResourceId,
   SLanguageItem    table[],
   int*            	pPanelHandle
)
{
	int32_t    		error;
	SElExceptionPtr	pexception = NULL;
	int    			i = 0;

	EXCCHECKCVI( *pPanelHandle = LoadPanel (parentPanel, path, panelResourceId));

	/* set panel and controls attribute */
	while ( table[i].text ) 
	{
		if(LANGUAGE_PTR!=NULL)
		{
			if(table[i].item_id == LANG_TABLE_PANEL_ITEM)
			{
			SetPanelAttribute( *pPanelHandle, table[i].attrType, gettext(table[i].text));
			}
			else if(table[i].item_id == LANG_TABLE_CONTROL_ITEM)
			{
			SetCtrlAttribute( *pPanelHandle, table[i].ID, table[i].attrType, gettext(table[i].text));
			}
		}
		else
		{
			if(table[i].item_id == LANG_TABLE_PANEL_ITEM)
			{
			SetPanelAttribute( *pPanelHandle, table[i].attrType, table[i].text);
			}
			else if(table[i].item_id == LANG_TABLE_CONTROL_ITEM)
			{
			SetCtrlAttribute( *pPanelHandle, table[i].ID, table[i].attrType, table[i].text);
			}
		}
		i++;
	}

Error:
   EXCRETHROW( pexception);
} /* LoadPanelWithLanguage */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LoadPanelExWithLanguage"
/*!
 * This function loads specified panel by panelResourceId from specified file
 * to memory and set all langueage strings on the panel.
 *
 * \param   path      Specifies full UIR file path name, where is panel with
 *                      specified \p panelResourceId.
 * \param   panelResourceId   Defined constant that you assigned to the panel
 *                      in the User Interface Editor.
 * \param   table      Table with default language strings for all controls.
 *
 * \param   pPanelHandle Returns handle of the loaded panel.
 *
 * \return On success, LoadPanelWithLanguage() returns a zero (0).
 *         On failure it returns a negative error.
 */
SElExceptionPtr LoadPanelExWithLanguage(
   int          	parentPanel,  
   const char*      path,
   int              panelResourceId,
   SLanguageItem    table[],
   int*            	pPanelHandle
)
{
	int32_t 		error;
	SElExceptionPtr	pexception = NULL; 
	int    			i = 0;

	EXCCHECKCVI( *pPanelHandle = LoadPanelEx (parentPanel, path, panelResourceId, __CVIUserHInst));

	/* set panel and controls attribute */
	while ( table[i].text ) 
	{
		if(LANGUAGE_PTR!=NULL)
		{
			if(table[i].item_id == LANG_TABLE_PANEL_ITEM)
			{
				EXCCHECKCVI( SetPanelAttribute( *pPanelHandle, table[i].attrType, gettext(table[i].text)));
			}
			else if(table[i].item_id == LANG_TABLE_CONTROL_ITEM)
			{
				EXCCHECKCVI( SetCtrlAttribute( *pPanelHandle, table[i].ID, table[i].attrType, gettext(table[i].text)));
			}
		}
		else
		{
			if(table[i].item_id == LANG_TABLE_PANEL_ITEM)
			{
				EXCCHECKCVI( SetPanelAttribute( *pPanelHandle, table[i].attrType, table[i].text));
			}
			else if(table[i].item_id == LANG_TABLE_CONTROL_ITEM)
			{
				EXCCHECKCVI( SetCtrlAttribute( *pPanelHandle, table[i].ID, table[i].attrType, table[i].text));
			}
		}
		i++; 
	}

Error:
   EXCRETHROW( pexception); 
} /* LoadPanelExWithLanguage */



