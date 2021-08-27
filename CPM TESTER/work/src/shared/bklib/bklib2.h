#ifndef _BKLIB2_H
#define _BKLIB2_H

#if defined(INCLUDE_AFTER_WINDOWS_H) && !defined(_INC_WINDOWS)
#error  This header must be included before utility.h and formatio.h
#error  because it includes cviauto.h which includes Windows SDK headers.
#endif /* INCLUDE_AFTER_WINDOWS_H */

#include <cviauto.h>

#ifdef __cplusplus
    extern "C" {
#endif
/* NICDBLD_BEGIN> Type Library Specific Types */

typedef unsigned long BKLib2Type_OLE_COLOR;
typedef CAObjHandle BKLib2Obj_Font;
typedef HRESULT (CVICALLBACK *_DBKTreeEventsRegOnNodeClick_CallbackType) (CAObjHandle caServerObjHandle,
                                                                          void *caCallbackData,
                                                                          char *keyNode);
typedef HRESULT (CVICALLBACK *_DBKTreeEventsRegOnExpand_CallbackType) (CAObjHandle caServerObjHandle,
                                                                       void *caCallbackData,
                                                                       char *keyNode);
typedef HRESULT (CVICALLBACK *_DBKTreeEventsRegOnCollapse_CallbackType) (CAObjHandle caServerObjHandle,
                                                                         void *caCallbackData,
                                                                         char *keyNode);
typedef HRESULT (CVICALLBACK *_DBKTreeEventsRegOnPopupMenuCommand_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                 void *caCallbackData,
                                                                                 char *keyNode,
                                                                                 char *keyCol,
                                                                                 long  menuItem);
typedef HRESULT (CVICALLBACK *_DBKTreeEventsRegOnMouseOverCell_CallbackType) (CAObjHandle caServerObjHandle,
                                                                              void *caCallbackData,
                                                                              char *keyNode,
                                                                              char *keyCol);
typedef HRESULT (CVICALLBACK *_DBKTreeEventsRegOnModify_CallbackType) (CAObjHandle caServerObjHandle,
                                                                       void *caCallbackData,
                                                                       char *keyNode,
                                                                       char *keyCol,
                                                                       long  modifyType);
typedef HRESULT (CVICALLBACK *_DBKTreeEventsRegOnPrePopupMenuCallback_CallbackType) (CAObjHandle caServerObjHandle,
                                                                                     void *caCallbackData,
                                                                                     char *keyNode,
                                                                                     char *keyCol,
                                                                                     char **pMenuItem);
typedef HRESULT (CVICALLBACK *_DBKTreeEventsRegOnDragOver_CallbackType) (CAObjHandle caServerObjHandle,
                                                                         void *caCallbackData,
                                                                         char *keyNode,
                                                                         short  nPosition,
                                                                         short  state);
typedef HRESULT (CVICALLBACK *_DBKTreeEventsRegOnDragDrop_CallbackType) (CAObjHandle caServerObjHandle,
                                                                         void *caCallbackData,
                                                                         char *keyNode,
                                                                         short  nPosition);
/* NICDBLD_END> Type Library Specific Types */

extern const IID BKLib2_IID__DBKTree;
extern const IID BKLib2_IID__DBKGrid;
extern const IID BKLib2_IID__DBKTreeEvents;

HRESULT CVIFUNC BKLib2_New_DBKTree (int panel, const char *label, int top,
                                    int left, int *controlID, int *UILError);

HRESULT CVIFUNC BKLib2_Open_DBKTree (const char *fileName, int panel,
                                     const char *label, int top, int left,
                                     int *controlID, int *UILError);

HRESULT CVIFUNC BKLib2__DBKTreeAddColumn (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          const char *keyCol, const char *name,
                                          short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnName (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *keyCol,
                                              char **returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnCount (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeRemoveColumn (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             const char *keyCol,
                                             short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeResetContent (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetColumnName (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *keyCol,
                                              const char *name,
                                              short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeAddNode (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo,
                                        const char *keyParent,
                                        const char *keyNode, const char *text,
                                        short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeRemoveNode (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo,
                                           const char *keyNode,
                                           short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemText (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *keyNode,
                                                const char *keyCol,
                                                const char *text,
                                                short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemText (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *keyNode,
                                                const char *keyCol,
                                                char **returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemTextEx (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *keyNode,
                                                  const char *keyCol,
                                                  char **returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetColumnWidth (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               const char *keyCol, long nWidth,
                                               short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemImage (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *keyNode,
                                                 const char *keyCol,
                                                 short nImage);

HRESULT CVIFUNC BKLib2__DBKTreeSetRedraw (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, short bRedraw,
                                          short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeExpand (CAObjHandle objectHandle,
                                       ERRORINFO *errorInfo, const char *keyNode,
                                       short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeCollapse (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo,
                                         const char *keyNode, short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeExpandLevel (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, long nLevel,
                                            short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemFormat (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *keyNode,
                                                  const char *keyCol,
                                                  short nodeFormat);

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemFormat (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *keyNode,
                                                  const char *keyCol,
                                                  short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeFontStyle (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *keyNode,
                                                 short nodeStyle);

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeFontStyle (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *keyNode,
                                                 short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetParentNode (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *keyNode,
                                              char **returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetScrollBarVisible (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    short nBar, short nNewValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetScrollBarVisible (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    short nBar,
                                                    short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemFontStyle (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *keyNode,
                                                     const char *keyCol,
                                                     short nodeStyle);

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemFontStyle (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *keyNode,
                                                     const char *keyCol,
                                                     short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemTextColor (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *keyNode,
                                                     const char *keyCol,
                                                     BKLib2Type_OLE_COLOR nNewValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemTextColor (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *keyNode,
                                                     const char *keyCol,
                                                     BKLib2Type_OLE_COLOR *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemBackColor (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *keyNode,
                                                     const char *keyCol,
                                                     BKLib2Type_OLE_COLOR nNewValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemBackColor (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     const char *keyNode,
                                                     const char *keyCol,
                                                     BKLib2Type_OLE_COLOR *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeIsExpanded (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo,
                                           const char *keyNode,
                                           short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeMoveNode (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo,
                                         const char *keyNode,
                                         const char *keyTargetNode,
                                         short nPosition, short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetPopupMenu (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             const char *menuItems,
                                             short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemEditable (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    const char *keyNode,
                                                    const char *keyCol,
                                                    short bEditable);

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemEditable (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    const char *keyNode,
                                                    const char *keyCol,
                                                    short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeFromArray (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, VARIANT *vArray,
                                          short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeToArray (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo,
                                        VARIANT *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeFromString (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo,
                                           const char *strString,
                                           const char *chSeparator,
                                           const char *chLine,
                                           short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeToString (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo,
                                         const char *chSeparator,
                                         const char *chLine, char **returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeToString2 (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          const char *chSeparator,
                                          const char *chLine, char **returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnNext (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *keyCol,
                                              char **returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeNext (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            const char *keyNode, short position,
                                            char **returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeHasChildren (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   const char *keyNode,
                                                   short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemImage (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *keyNode,
                                                 const char *keyCol,
                                                 short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemTextEx (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  const char *keyNode,
                                                  const char *keyCol,
                                                  const char *text,
                                                  short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeEnsureVisible (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *keyNode);

HRESULT CVIFUNC BKLib2__DBKTreeRenameNodeKey (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *keyNode,
                                              const char *keyNew,
                                              short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeIsNodeExist (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            const char *keyNode,
                                            short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetColumnHeadFontStyle (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       const char *keyCol,
                                                       short headStyle);

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnHeadFontStyle (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       const char *keyCol,
                                                       short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetColumnHeadFormat (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    const char *keyCol,
                                                    short headFormat);

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnHeadFormat (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    const char *keyCol,
                                                    short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetVersion (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo,
                                           char **returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetFrameBorder (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               long nNewStyle, long nNewStyleEx);

HRESULT CVIFUNC BKLib2__DBKTreeGetFrameBorder (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               long *pStyle, long *pStyleEx);

HRESULT CVIFUNC BKLib2__DBKTreeFromXML (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo, const char *strXML,
                                        short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeToXML (CAObjHandle objectHandle,
                                      ERRORINFO *errorInfo, char **returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeData (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            const char *keyNode, long lData);

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeData (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            const char *keyNode,
                                            long *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemData (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *keyNode,
                                                const char *keyCol, long lData);

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemData (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *keyNode,
                                                const char *keyCol,
                                                long *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnWidth (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               const char *keyCol,
                                               long *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetNodeItemTipTextEnable (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         const char *keyNode,
                                                         const char *keyCol,
                                                         short bEnable);

HRESULT CVIFUNC BKLib2__DBKTreeGetNodeItemTipTextEnable (CAObjHandle objectHandle,
                                                         ERRORINFO *errorInfo,
                                                         const char *keyNode,
                                                         const char *keyCol,
                                                         short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeRefresh (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo);

HRESULT CVIFUNC BKLib2__DBKTreeAboutBox (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo);

HRESULT CVIFUNC BKLib2__DBKTreeGetSelectedItem (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                char **returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetSelectedItem (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefSelectedItem (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     char **newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetBackColor (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             BKLib2Type_OLE_COLOR *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetBackColor (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             BKLib2Type_OLE_COLOR newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefBackColor (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  BKLib2Type_OLE_COLOR *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetBackTextColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetBackTextColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefBackTextColor (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      BKLib2Type_OLE_COLOR *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetForeTextColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetForeTextColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefForeTextColor (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      BKLib2Type_OLE_COLOR *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnHeadingVisible (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetColumnHeadingVisible (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefColumnHeadingVisible (CAObjHandle objectHandle,
                                                             ERRORINFO *errorInfo,
                                                             VBOOL *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetGridLines (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetGridLines (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             short newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefGridLines (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  short *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetGridLineColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetGridLineColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefGridLineColor (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      BKLib2Type_OLE_COLOR *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetImageList (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             CAObjHandle *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetImageList (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             CAObjHandle newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefImageList (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  CAObjHandle *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetFont (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo,
                                        BKLib2Obj_Font *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetFont (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo,
                                        BKLib2Obj_Font newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefFont (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             BKLib2Obj_Font *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetColumnResizing (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetColumnResizing (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  VBOOL newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefColumnResizing (CAObjHandle objectHandle,
                                                       ERRORINFO *errorInfo,
                                                       VBOOL *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetAutoImages (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VBOOL *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetAutoImages (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              VBOOL newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefAutoImages (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VBOOL *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetTreeLineColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetTreeLineColor (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 BKLib2Type_OLE_COLOR newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefTreeLineColor (CAObjHandle objectHandle,
                                                      ERRORINFO *errorInfo,
                                                      BKLib2Type_OLE_COLOR *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetTreeLines (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetTreeLines (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             short newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefTreeLines (CAObjHandle objectHandle,
                                                  ERRORINFO *errorInfo,
                                                  short *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetEnableEventMouseOverCell (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VBOOL *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetEnableEventMouseOverCell (CAObjHandle objectHandle,
                                                            ERRORINFO *errorInfo,
                                                            VBOOL newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefEnableEventMouseOverCell (CAObjHandle objectHandle,
                                                                 ERRORINFO *errorInfo,
                                                                 VBOOL *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetEnableEventModify (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetEnableEventModify (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefEnableEventModify (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          VBOOL *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetEditable (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            VBOOL *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetEditable (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, VBOOL newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefEditable (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VBOOL *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetEnableDragAndDrop (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetEnableDragAndDrop (CAObjHandle objectHandle,
                                                     ERRORINFO *errorInfo,
                                                     VBOOL newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefEnableDragAndDrop (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          VBOOL *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetDragMode (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetDragMode (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, short newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefDragMode (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 short *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetDragIcon (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            short *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetDragIcon (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, short newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefDragIcon (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 short *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetEnableSelection (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VBOOL *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetEnableSelection (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VBOOL newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefEnableSelection (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetEnableFocusCell (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VBOOL *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetEnableFocusCell (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   VBOOL newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefEnableFocusCell (CAObjHandle objectHandle,
                                                        ERRORINFO *errorInfo,
                                                        VBOOL *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetEnableTreeKeyDirection (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          VBOOL *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetEnableTreeKeyDirection (CAObjHandle objectHandle,
                                                          ERRORINFO *errorInfo,
                                                          VBOOL newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefEnableTreeKeyDirection (CAObjHandle objectHandle,
                                                               ERRORINFO *errorInfo,
                                                               VBOOL *newValue);

HRESULT CVIFUNC BKLib2__DBKTreeGetListMode (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo,
                                            VBOOL *returnValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetListMode (CAObjHandle objectHandle,
                                            ERRORINFO *errorInfo, VBOOL newValue);

HRESULT CVIFUNC BKLib2__DBKTreeSetByRefListMode (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 VBOOL *newValue);

HRESULT CVIFUNC BKLib2_New_DBKGrid (int panel, const char *label, int top,
                                    int left, int *controlID, int *UILError);

HRESULT CVIFUNC BKLib2_Open_DBKGrid (const char *fileName, int panel,
                                     const char *label, int top, int left,
                                     int *controlID, int *UILError);

HRESULT CVIFUNC BKLib2__DBKGridRefresh (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo);

HRESULT CVIFUNC BKLib2__DBKGridAboutBox (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo);

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnNodeClick (CAObjHandle serverObject,
                                                     _DBKTreeEventsRegOnNodeClick_CallbackType callbackFunction,
                                                     void *callbackData,
                                                     int enableCallbacks,
                                                     int *callbackId);

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnExpand (CAObjHandle serverObject,
                                                  _DBKTreeEventsRegOnExpand_CallbackType callbackFunction,
                                                  void *callbackData,
                                                  int enableCallbacks,
                                                  int *callbackId);

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnCollapse (CAObjHandle serverObject,
                                                    _DBKTreeEventsRegOnCollapse_CallbackType callbackFunction,
                                                    void *callbackData,
                                                    int enableCallbacks,
                                                    int *callbackId);

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnPopupMenuCommand (CAObjHandle serverObject,
                                                            _DBKTreeEventsRegOnPopupMenuCommand_CallbackType callbackFunction,
                                                            void *callbackData,
                                                            int enableCallbacks,
                                                            int *callbackId);

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnMouseOverCell (CAObjHandle serverObject,
                                                         _DBKTreeEventsRegOnMouseOverCell_CallbackType callbackFunction,
                                                         void *callbackData,
                                                         int enableCallbacks,
                                                         int *callbackId);

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnModify (CAObjHandle serverObject,
                                                  _DBKTreeEventsRegOnModify_CallbackType callbackFunction,
                                                  void *callbackData,
                                                  int enableCallbacks,
                                                  int *callbackId);

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnPrePopupMenuCallback (CAObjHandle serverObject,
                                                                _DBKTreeEventsRegOnPrePopupMenuCallback_CallbackType callbackFunction,
                                                                void *callbackData,
                                                                int enableCallbacks,
                                                                int *callbackId);

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnDragOver (CAObjHandle serverObject,
                                                    _DBKTreeEventsRegOnDragOver_CallbackType callbackFunction,
                                                    void *callbackData,
                                                    int enableCallbacks,
                                                    int *callbackId);

HRESULT CVIFUNC BKLib2__DBKTreeEventsRegOnDragDrop (CAObjHandle serverObject,
                                                    _DBKTreeEventsRegOnDragDrop_CallbackType callbackFunction,
                                                    void *callbackData,
                                                    int enableCallbacks,
                                                    int *callbackId);
#ifdef __cplusplus
    }
#endif
#endif /* _BKLIB2_H */
