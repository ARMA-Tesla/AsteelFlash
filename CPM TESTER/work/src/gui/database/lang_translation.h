#if !defined(__LANG_TRANSLATION_H__)
#define __LANG_TRANSLATION_H__

#include "database.h"

/* language support pattern */
static SLanguageItem DBSlang[] = {
   	P(DBS,                  	_noop("Base des Données")),
	L(DBS_USERNAME,				_noop("Utilisateur:")),
	L(DBS_USERPRODUCT,			_noop("Chemain:")),
	L(DBS_USERRIGHTS,			_noop("Droits:")),
	V(DBS_RIGHTS_TXT,           _noop("You have not required rights for browsing or editing database!")),
   	{0 , NULL, 0}
};

static SLanguageItem TESTlang[] = { 
	P(TEST,					_noop("Test")),
	L(TEST_STEP,			_noop("Step")),
	L(TEST_NAME,			_noop("Name")),
	L(TEST_DESCRIPT,		_noop("Description")),
   	{0 , NULL, 0}
};

static SLanguageItem PARAMlang[] = { 
	P(PARAM,				_noop("Parameter")),
	L(PARAM_NB,				_noop("Parameter")),
	L(PARAM_NAME,			_noop("Nom")),
	L(PARAM_DESCRIPT,		_noop("Description")),
	L(PARAM_VALUE,			_noop("Value")),
	L(PARAM_VTYPE,			_noop("VType")),
   	{0 , NULL, 0}
};

static SLanguageItem PRODUCTlang[] = { 
	P(PRODUCT,				_noop("Produit")),
	L(PRODUCT_PARENT,		_noop("Parent")),
	L(PRODUCT_NAME,			_noop("Name")),
	L(PRODUCT_DESCRIPT,		_noop("Description")),
	L(PRODUCT_SETTINGS,		_noop("Visible")),
   	{0 , NULL, 0}
};

static SLanguageItem PRODUCT_CPlang[] = { 
	P(PRODUCT_CP,               _noop("Copy Struct")), 		
	V(PRODUCT_CP_SETTING_TXT, 	_noop("Copy Settings")), 
	L(PRODUCT_CP_SOURCE, 		_noop("Select Source")),
	L(PRODUCT_CP_LISTBOX, 		_noop("Selected Products")),
	L(PRODUCT_CP_SET_PROD_PARAM, 	_noop("Copy Product Parameters")),
	L(PRODUCT_CP_SET_PROD_MASTER, 	_noop("Copy Product Master")), 
	L(PRODUCT_CP_SET_TEST, 			_noop("Copy Tests")),
	L(PRODUCT_CP_SET_TEST_PARAM, 	_noop("Copy Test Parameters")),
	L(PRODUCT_CP_SET_LINE_PROCESS, 	_noop("Copy Line Processes")),
	L(PRODUCT_CP_SET_LINE_STEP, 	_noop("Copy Line Steps")),
	L(PRODUCT_CP_SET_LINE_PARAM, 	_noop("Copy Line Parameters")),
   	{0 , NULL, 0}
};

static SLanguageItem MASTERlang[] = {   
   P(MASTER,                  	_noop("Master Panel")),
   L(MASTER_MASTER_ID,			_noop("Master")),
   L(MASTER_MASTER_CODE,		_noop("Master Code")),
   {0 , NULL, 0}
};

static SLanguageItem DLG_USERlang[] = {   
   P(DLG_USER,                  _noop("User Dialogue")),
   V(DLG_USER_DBS_TXT,     		_noop("Droits")),  
   V(DLG_USER_TESTER_TXT,  		_noop("Droits")),  
   L(DLG_USER_NAME, 			_noop("__Nom")),
   L(DLG_USER_PASSW1, 			_noop("__Mot de passe")), 
   L(DLG_USER_PASSW2, 			_noop("Retaper mot de passe")),
   L(DLG_USER_RIGHTS,			_noop("Droits")),
   L(DLG_USER_DRIGHTS_1, 		_noop("voir Data")), 
   L(DLG_USER_DRIGHTS_2, 		_noop("Editer parametres")),
   L(DLG_USER_DRIGHTS_3, 		_noop("Editer tests")),
   L(DLG_USER_DRIGHTS_4, 		_noop("Editer produits")),
   L(DLG_USER_DRIGHTS_5, 		_noop("Editer utilisateurs")),
   L(DLG_USER_TRIGHTS_1, 		_noop("Team Leader")),
   L(DLG_USER_TRIGHTS_2, 		_noop("Opérateur")),  
   L(DLG_USER_TRIGHTS_3, 		_noop("Opérateur")),  
   L(DLG_USER_TRIGHTS_4, 		_noop("Mode Maintenance")),  
   L(DLG_USER_TRIGHTS_5, 		_noop("Méthode")), 
   {0 , NULL, 0}
};

#endif    
