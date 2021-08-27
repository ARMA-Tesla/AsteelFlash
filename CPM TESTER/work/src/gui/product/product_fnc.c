#include <msxmldom2.h>
#include "product_fnc.h"
#include "product.h"
#include <dbs/dbs.h>
#include <utility.h>
#include <formatio.h>
#include <stdio.h>
#include <guid.h>

#define PRODUCT_PICTURE_PATH			"%s\\images\\"
#define PRODUCT_PICTURE_PATH_JPG   		PRODUCT_PICTURE_PATH"%s_%s.jpg" 
#define PRODUCT_PICTURE_PATH_PCX   		PRODUCT_PICTURE_PATH"%s_%s.pcx" 

extern int		gs_panel[];
extern int32_t	gs_ActiveProductId;

#define BRANCH_ID_SIZE		8

static int32_t	gs_BranchId[BRANCH_ID_SIZE] = {0,0,0,0,0,0,0,0};

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LoadConfiguration"
SElExceptionPtr LoadConfiguration(const char* fileName)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	CAObjHandle     pnode = 0;
	const char*     pfile_name = (fileName) ? fileName : "/plugins/product.xml";
	char         	buffer[256];
	char			*pdata;
	int32_t			i = 0;
	char			path[MAX_PATHNAME_LEN];

	EXCCHECKCVI( GetProjectDir(path));
	strcat(path, pfile_name); 
	
	XML_GET_DOC_NODE(path, "//plugin[@id='"PLUGIN_PRODUCT"']", pnode);
	XML_GET_NODE_STRING(pnode, "branch_id", buffer, sizeof(buffer));

	pdata = strtok(buffer, ";");
	while(pdata)
	{
		gs_BranchId[i++] = strtol(pdata, NULL, 10);

		pdata = strtok(NULL, ";");
	}

Error:
	if (pnode) CA_DiscardObjHandle(pnode);
	EXCRETHROW( pexception);
} 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetProductPath"
SElExceptionPtr SetProductPath(SDBSProductListPtr pProductList, int32_t product_id)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error;
	int32_t     	pid[32];
	int32_t     	pidSize = 32;
	int         	i;
	char*      		name;
	char      		product_path[512] = "No Database Connection";

	if( pProductList)
	{
		EXCCHECK( pProductList->ProductGetIdTreeUp( pProductList, product_id, pid, &pidSize) );

		sprintf(product_path, "");
		for(i=pidSize-1; i>=0; i--)
		{
			EXCCHECK( pProductList->ProductGetIdName( pProductList, pid[i], &name) );
			strcat( product_path, " \\ ");
			strcat( product_path, name);  
		}
	}

	EXCCHECKCVI( SetCtrlVal(gs_panel[PRODUCT], PRODUCT_PATH, product_path));
   
Error:
   	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetProductPicture"
SElExceptionPtr SetProductPicture(SDBSProductListPtr pProductList, int32_t product_id)
{
	int32_t			error = 0; 
	SElExceptionPtr	pexception = NULL; 
	char			projectDir[MAX_PATHNAME_LEN] = "";
	char			pictPath[MAX_PATHNAME_LEN];	
	char			ProductPicture[256] = "";
	long			size = 0;

	if(pProductList)
	{
		SDBSPtr pdbs = (SDBSPtr)pProductList->pdbs;

		EXCCHECKCVI( GetProjectDir(projectDir));

		/* Get picture name from dbs */
		EXCCHECK( pdbs->Export->GetSpecParameter(pdbs->Export, product_id, "ProductPicture", ProductPicture, 255));
		sprintf(pictPath, PRODUCT_PICTURE_PATH, projectDir);
		strcat(pictPath, ProductPicture);

		if(strlen(ProductPicture) && GetFileInfo (pictPath, &size))
		{
			EXCCHECKCVI( DisplayImageFile(gs_panel[PRODUCT], PRODUCT_PICT, pictPath));
		}
		else
		{
			int32_t pid[32];
			int32_t pidSize = 32;

   			EXCCHECK( pProductList->ProductGetIdTreeUp(pProductList, product_id, pid, &pidSize));
			if(pidSize)
   			{
				char* Tester = NULL;
				char* PanelVersion = NULL; 

				EXCCHECK( pProductList->ProductGetIdName(pProductList, pid[pidSize-1], &Tester));
				EXCCHECK( pProductList->ProductGetIdName(pProductList, pid[0], &PanelVersion)); 

				sprintf(pictPath, PRODUCT_PICTURE_PATH_JPG, projectDir, Tester, PanelVersion);

				if(GetFileInfo(pictPath, &size))
				{
					EXCCHECKCVI( DisplayImageFile( gs_panel[PRODUCT], PRODUCT_PICT, pictPath));
				}
				else 
				{
					sprintf(pictPath, PRODUCT_PICTURE_PATH_PCX, projectDir, Tester, PanelVersion);

					if(GetFileInfo(pictPath, &size))
					{					
						EXCCHECKCVI( DisplayImageFile(gs_panel[PRODUCT], PRODUCT_PICT, pictPath));
					}
					else
					{
						EXCCHECKCVI( DeleteImage (gs_panel[PRODUCT], PRODUCT_PICT));
					}
				}
   			}
		}
	}

Error:
     EXCRETHROW( pexception);
}

static SEPartDescriptPtr	gs_PartDesc = NULL;
static int32_t				gs_PartDescSize = 0; 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetPartDescription"
SElExceptionPtr GetPartDescription(
	SDBSProductListPtr pProductList, 
	int32_t product_id, 
	char* PartDescription,
	char* ProductPartNumber
)
{
	int32_t      		error = 0;
	SElExceptionPtr		pexception = NULL;    
	int					i;  
	SDBSExportPtr		pExport = ((SDBSPtr)pProductList->pdbs)->Export;

	if( pExport && gs_PartDesc==NULL)
		EXCCHECK( pExport->GetProductDescription(pExport, &gs_PartDesc, &gs_PartDescSize) );

	for(i=0;i<gs_PartDescSize;i++)
	{
		if(gs_PartDesc[i].product_id==product_id)
		{
			if( PartDescription)
			{
				sprintf( PartDescription, "%s - %s - %s", gs_PartDesc[i].ProductOrder,
														  gs_PartDesc[i].ProductPartNumber,
														  gs_PartDesc[i].ProductName);
			}
			if(ProductPartNumber)
			{
				strcpy(ProductPartNumber, gs_PartDesc[i].ProductPartNumber);
			}
			break;
		}
	}
      
	if (i == gs_PartDescSize )
	{
		if ( PartDescription )
			sprintf(PartDescription, "");
	}
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "CleanPartDescription"
SElExceptionPtr CleanPartDescription(
	SDBSProductListPtr pProductList
)
{
	SElExceptionPtr		pexception = NULL;    

	EXCCHECK( ((SDBSPtr)pProductList->pdbs)->Free( gs_PartDesc));
	gs_PartDesc = NULL;

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetProductList"
SElExceptionPtr SetProductList(SDBSProductListPtr pProductList, int32_t product_id)
{
	int32_t      		error = 0;
	SElExceptionPtr		pexception = NULL;        
	bool_t      		child;
	int32_t      		parent_id;
	int         		i, j, index=0, item_index=0;
	int32_t      		product, active_product, branch_id;
	char				description[1024];   
	char				partdesc[1024];

	branch_id = ((SDBSPtr)pProductList->pdbs)->_branch_id;

	EXCCHECKCVI( DeleteListItem (gs_panel[PRODUCT], PRODUCT_LIST, 0, -1) );
	EXCCHECKCVI( DeleteListItem (gs_panel[PRODUCT], PRODUCT_RING, 0, -1) );
	EXCCHECK( ChildExist(pProductList, product_id, &child, &parent_id) );

	if(child)
	{
		product = product_id;
		active_product = gs_ActiveProductId;
	}
	else
	{
		product = parent_id;
		active_product = product_id;
	}

	EXCCHECKCVI( InsertListItem (gs_panel[PRODUCT], PRODUCT_LIST, -1, 
									(product_id || parent_id)? "..":".", product));
   
	for(i=0; i<pProductList->ProductSize; i++)
	{
		for(j=0; j<BRANCH_ID_SIZE; j++)
		{
			if( (pProductList->Product[i].property&PROPERTY_VALID)>0
				 && (pProductList->Product[i].property&PROPERTY_VISIBLE)>0
				 && pProductList->Product[i].parent_id == product
				 && (product!=0 || (product==0 && 
					(branch_id==pProductList->Product[i].product_id || gs_BranchId[j]==pProductList->Product[i].product_id))))
			{
				EXCCHECK( GetPartDescription( pProductList, pProductList->Product[i].product_id, partdesc, NULL));

				sprintf(description, "%s", (strlen(partdesc))? partdesc:pProductList->Product[i].name);
				EXCCHECKCVI( InsertListItem (gs_panel[PRODUCT], PRODUCT_LIST, -1, description, pProductList->Product[i].product_id));
				EXCCHECKCVI( InsertListItem (gs_panel[PRODUCT], PRODUCT_RING, -1, description, pProductList->Product[i].product_id));

				item_index++;

				if( pProductList->Product[i].product_id == active_product)
					index = item_index;

				break;
  			}	
		}
	}

	EXCCHECKCVI( SetCtrlIndex (gs_panel[PRODUCT], PRODUCT_LIST, index)); 

Error:
	CleanPartDescription(pProductList);
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ChildExist"
SElExceptionPtr ChildExist(SDBSProductListPtr pProductList, int32_t product_id, bool_t* child, int32_t *parent_id)
{
   int32_t			error = 0;
   SElExceptionPtr	pexception = NULL; 
   int				i;
   
   if( child) *child = FALSE;
   
	if( pProductList)
	{
		if( product_id==0)
		{
			if( parent_id) *parent_id = 0;
			if( child) *child = TRUE;
		}
		else
		{
			for(i=0; i<pProductList->ProductSize; i++)
			{
				if( (pProductList->Product[i].property&PROPERTY_VALID)>0
					&& pProductList->Product[i].parent_id == product_id
					&& child)
					*child = TRUE;

				if( pProductList->Product[i].product_id == product_id
					&& parent_id)
					*parent_id = pProductList->Product[i].parent_id;
			}
		}
	}

Error:
   EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetProductLevelBackward"
SElExceptionPtr SetProductLevelBackward(SDBSProductListPtr pProductList, int32_t product_id)
{
	int32_t      	error = 0;
	SElExceptionPtr	pexception = NULL; 
	int32_t      	parent_id = -1;
	int         	i, j;
	bool_t     		run = TRUE, is_visible = FALSE;
	int32_t      	product_id_act = product_id;
	int32_t      	count;
   
   	if(product_id==0)
      	goto Error;
   
	do
	{   
		/* determine parent_id */
		for(i=0; i<pProductList->ProductSize; i++)
		{
			if( (pProductList->Product[i].property&PROPERTY_VALID)>0
				&& pProductList->Product[i].product_id==product_id_act)
			{
				parent_id = pProductList->Product[i].parent_id;
				is_visible = ((pProductList->Product[i].property&PROPERTY_VISIBLE)>0);
				break;
			}
		}

		/* selected product_id is not visible or valid */
		if(parent_id==-1)
			break;

		/* determine number of childs */
		count = 0;
		for(i=0; i<pProductList->ProductSize; i++)
		{
			for(j=0; j<BRANCH_ID_SIZE; j++)
			{
				if( (pProductList->Product[i].property&PROPERTY_VALID)>0
					&& (pProductList->Product[i].property&PROPERTY_VISIBLE)>0
					&& pProductList->Product[i].parent_id == parent_id
					&& (parent_id!=0 || (parent_id==0 && pProductList->Product[i].product_id==gs_BranchId[j])) )
				{
					count++;
					break;
				}
			}
		}

		if(count>1 || (count==1 && !is_visible))
		{
			gs_ActiveProductId = product_id_act;
			EXCCHECK( SetProductPath(pProductList, product_id_act));
			EXCCHECK( SetProductList(pProductList, parent_id));
			break;
		}
		else
		{
			if(product_id_act==parent_id) /* root or unvisible parent */
				break;      
			else
				product_id_act = parent_id;
		}
   	}while(run);
   
Error:
   EXCRETHROW( pexception); 
}


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetProductLevelForward"
SElExceptionPtr SetProductLevelForward(SDBSProductListPtr pProductList, int32_t product_id)
{
	int32_t      	error = 0;
	SElExceptionPtr	pexception = NULL;
	bool_t      	run = TRUE;
	int32_t      	parent_id_act = product_id;
	int32_t      	product_id_act = -1;
	int         	i;
	int32_t      	count;

	if( pProductList)
	{
		do
		{
			count = 0;

			for(i=0; i<pProductList->ProductSize; i++)
			{
				if( (pProductList->Product[i].property&PROPERTY_VALID)>0
					&& (pProductList->Product[i].property&PROPERTY_VISIBLE)>0
					&& pProductList->Product[i].parent_id == parent_id_act)
				{
					product_id_act = pProductList->Product[i].product_id;
					count++;
				}
			}

			if(count==1)   /* if there is only one item, go to the next level */
			{
				parent_id_act = product_id_act;
				continue;
			}
			else         /* if there is none or more then one item, set this level */
			{
				gs_ActiveProductId = parent_id_act;
				EXCCHECK( SetProductPath( pProductList, parent_id_act ) );
				EXCCHECK( SetProductPicture( pProductList, parent_id_act ) );
				EXCCHECK( SetProductList( pProductList, parent_id_act ) );
				run = FALSE;
				continue;
			}
		}while(run);
	}
   
Error:
   EXCRETHROW( pexception); 
}
