#if !defined(__DBS_DEFINE_LOCAL_H__)
#define __DBS_DEFINE_LOCAL_H__

#define PDBS		((SDBSPtr)(me->pdbs))

/* tester_id for storage */
#define PDBS_TESTER_ID	((int32_t)(PDBS->_tester_id)) 

#define PTESTER		((SDBSTesterListPtr)(PDBS->TesterList))
#define PUSER		((SDBSUserListPtr)(PDBS->UserList))

#define PPRODUCT	((SDBSProductListPtr)(PDBS->ProductList))   
#define PPARAM		((SDBSParamListPtr)(PDBS->ProductParameterList))
#define PMASTER		((SDBSMasterListPtr)(PDBS->ProductMasterList))

#define PTEST		((SDBSTestListPtr)(PDBS->TestList))
#define PTESTPARAM	((SDBSTestParamListPtr)(PDBS->TestParameterList))

#define PEXPORT		((SDBSExportPtr)(PDBS->Export))
#define PLINEEXPORT	((SDBSLineExportPtr)(PDBS->LineExport))


#define PKOSU		((SDBSKOSUPtr)(PDBS->Kosu))   
#define PSTORAGE	((SDBSStoragePtr)(PDBS->Storage))

#define PPROCESS	((SDBSLineProcessListPtr)(PDBS->LineProcessList))
#define PSTEP		((SDBSLineStepListPtr)(PDBS->LineStepList))
#define PLINEPARAM	((SDBSLineParamListPtr)(PDBS->LineParamList))

#define PDBS_FREE(me)	PDBS->Free(me)
#define PDBS_ALLOC(me)	PDBS->CharAlloc(me)
#define PDBS_COPY(me)	PDBS->CharCopy(me)

#define LOCK			EXCCHECK( PDBS->fncLock(PDBS))
#define UNLOCK			PDBS->fncUnLock(PDBS)

#define PDBS_LOCK(name)		EXCCHECK( PDBS->Lock(PDBS, name))
#define PDBS_UNLOCK(name)	PDBS->Unlock(PDBS, name)

#define DATETIME_TO_STRING( str)	\
	{ \
		SYSTEMTIME SystemTime; \
		GetLocalTime(&SystemTime); \
		sprintf(str, "%hd-%02hd-%02hd %02hd:%02hd:%02hd", SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond); \
	}

#define DATETIME_FROM_STRING(str, SystemTime)	\
		sscanf(str, "%hd-%02hd-%02hd %02hd:%02hd:%02hd", &SystemTime.wYear, &SystemTime.wMonth, &SystemTime.wDay, &SystemTime.wHour, &SystemTime.wMinute, &SystemTime.wSecond)

#endif // !defined(__DBS_DEFINE_LOCAL_H__)
