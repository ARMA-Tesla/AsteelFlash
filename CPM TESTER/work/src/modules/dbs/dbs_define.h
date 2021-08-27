#if !defined(__DBS_DEFINE_H__)
#define __DBS_DEFINE_H__

#define GET_VTYPE_STRING(vtype) (vtype==E_DVT_INT32)? "int32_t":((vtype==E_DVT_REAL64)? "real64":((vtype==E_DVT_STRING)? "string":((vtype==E_DVT_TEXT)? "text":"unknown")) )
#define GET_STRING(label) (label!=NULL)? label:""

#define DBS_RECORD_LENGHT            	45
#define DBS_RECORD_LENGHT_NAME			45
#define DBS_RECORD_LENGHT_DESCRIPTION   255
#define DBS_RECORD_LENGHT_VALUE         45 
#define DBS_RECORD_LENGHT_VALUE_EXT		127
#define DBS_RECORD_LENGHT_TIME         	19   
#define DBS_RECORD_LENGHT_IBUTTON		16

#define SQL_COMMAND_LENGHT      		4048 
#define SQL_COMMAND_NB_OF_INSERTED_ROW	50
#define MEMORY_ALLOCATION_RESERVE		100
#define MEMORY_ALLOCATION_MIN			3

#define PROPERTY_CREATED				(1<<1)
#define PROPERTY_EDITED					(1<<2)
#define PROPERTY_DELETED				(1<<3)
#define PROPERTY_VALID					(1<<4)
#define PROPERTY_VISIBLE				(1<<5)
#define PROPERTY_MOVED					(1<<6)

#endif // !defined(__DBS_DEFINE_H__)
