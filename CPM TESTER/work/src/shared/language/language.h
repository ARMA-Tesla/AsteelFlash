#if !defined(__LANGUAGE_H__)
#define __LANGUAGE_H__

#include <exception/exception.h>
#include <language/lang_support.h>

#define LANGUAGE_PTR	gs_pLanguage

extern SLanguagePtr     LANGUAGE_PTR;

typedef enum _ELangTableItem
{
   LANG_TABLE_PANEL_ITEM,
   LANG_TABLE_CONTROL_ITEM
} ELangTableItem;

#define V(id, str) {id, str, ATTR_CTRL_VAL, LANG_TABLE_CONTROL_ITEM}
#define L(id, str) {id, str, ATTR_LABEL_TEXT, LANG_TABLE_CONTROL_ITEM}
#define D(id, str) {id, str, ATTR_DFLT_VALUE, LANG_TABLE_CONTROL_ITEM}
#define P(id, str) {id, str, ATTR_TITLE, LANG_TABLE_PANEL_ITEM}       /* panel title */

typedef struct _SLanguageItem{
	uint32_t        ID;
    const char*     text;
    int32_t         attrType;
	ELangTableItem	item_id;
} SLanguageItem, *SLanguageItemPtr;

SElExceptionPtr LoadPanelWithLanguage(
   int          	parentPanel,
   const char*      path,
   int              panelResourceId,
   SLanguageItem    table[],
   int*            	pPanelHandle
);

SElExceptionPtr LoadPanelExWithLanguage(
   int          	parentPanel,
   const char*      path,
   int              panelResourceId,
   SLanguageItem    table[],
   int*            	pPanelHandle
);

#endif
