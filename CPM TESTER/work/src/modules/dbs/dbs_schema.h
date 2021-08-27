#if !defined(__DBS_SCHEMA_H__)
#define __DBS_SCHEMA_H__

typedef struct _SDbsColumn
{
	const char*	name;
	bool_t		check;

} DbsColumn;

typedef struct _SDbsTable
{
	const char*	name;
	DbsColumn	column[32];
	bool_t		check;

} SDbsTable; 

typedef struct _SDbsSchema
{
	SDbsTable	table[32];

} SDbsSchema;

SDbsSchema SValeoSchema = 
{
	{
		{
			"line_parameters",
			{
				{"product_id",FALSE},
				{"fnc_nb",FALSE},
				{"name",FALSE},
				{"value",FALSE},
				{"parameter_id",FALSE},
				{"property",FALSE},
				{"time",FALSE},
				{"user_id",FALSE}
			},
			FALSE
		},
		{
			"line_process",
			{
				{"product_id",FALSE},
				{"process_nb",FALSE},
				{"name",FALSE},
				{"description",FALSE},
				{"process_id",FALSE},
				{"property",FALSE},
				{"time",FALSE},
				{"user_id",FALSE}
			},
			FALSE
		},
		{
			"line_steps",
			{
				{"product_id",FALSE},
				{"process_nb",FALSE},
				{"fnc_nb",FALSE},
				{"name",FALSE},
				{"fnc_id",FALSE},
				{"property",FALSE},
				{"time",FALSE},
				{"user_id",FALSE}
			},
			FALSE
		},
		{
			"product",
			{
				{"product_id",FALSE},
				{"parent_id",FALSE},
				{"branch_id",FALSE}
			},
			FALSE
		},
		{
			"product_masters",
			{
				{"product_id",FALSE},
				{"master_id",FALSE},
				{"master",FALSE},
				{"time",FALSE},
				{"user_id",FALSE},
				{"property",FALSE},
				{"parameter_id",FALSE}
			},
			FALSE
		},
		{
			"product_parameters",
			{
				{"product_id",FALSE},
				{"name",FALSE},
				{"description",FALSE},
				{"value",FALSE},
				{"time",FALSE},
				{"user_id",FALSE},
				{"property",FALSE},
				{"vtype",FALSE},
				{"text",FALSE},
				{"parameter_id",FALSE},
				{"tester_id",FALSE}
			},
			FALSE
		},
		{
			"product_status",
			{
				{"time",FALSE},
				{"tester_id",FALSE},
				{"product_id",FALSE},
				{"barcode",FALSE},
				{"duration",FALSE},
				{"failed",FALSE},
				{"mode_id",FALSE}
			},
			FALSE
		},
		{
			"product_types",
			{
				{"product_id",FALSE},
				{"name",FALSE},
				{"description",FALSE},
				{"time",FALSE},
				{"user_id",FALSE},
				{"property",FALSE},
				{"parameter_id",FALSE}
			},
			FALSE
		},
		{
			"status",
			{
				{"time",FALSE},
				{"time_end",FALSE},
				{"tester_id",FALSE},
				{"item_id",FALSE},
				{"id",FALSE},
				{"terminated",FALSE}
			},
			FALSE
		},
		{
			"test_parameters",
			{
				{"product_id",FALSE},
				{"test_nb",FALSE},
				{"name",FALSE},
				{"description",FALSE},
				{"value",FALSE},
				{"time",FALSE},
				{"user_id",FALSE},
				{"property",FALSE},
				{"parameter_id",FALSE},
				{"vtype",FALSE},
				{"tester_id",FALSE}
			},
			FALSE
		},
		{
			"test_status",
			{
				{"time",FALSE},
				{"tester_id",FALSE},
				{"product_id",FALSE},
				{"test_id",FALSE},
				{"parameter_id",FALSE},
				{"vtype",FALSE},
				{"dvalue",FALSE},
				{"ivalue",FALSE},
				{"svalue",FALSE}
			},
			FALSE
		},
		{
			"test_steps",
			{
				{"product_id",FALSE},
				{"process_nb",FALSE},
				{"test_nb",FALSE},
				{"name",FALSE},
				{"description",FALSE},
				{"time",FALSE},
				{"user_id",FALSE},
				{"property",FALSE},
				{"test_id",FALSE}
			},
			FALSE
		},
		{
			"tester",
			{
				{"tester_id",FALSE},
				{"product_id",FALSE},
				{"description",FALSE}
			},
			FALSE
		},
		{
			"user",
			{
				{"user_id",FALSE},
				{"name",FALSE}
			},
			FALSE
		},
		{
			"user_parameters",
			{
				{"user_id",FALSE},
				{"password",FALSE},
				{"ibutton",FALSE},
				{"time",FALSE},
				{"admin_id",FALSE},
				{"property",FALSE}
			},
			FALSE
		},
		{
			"user_rights",
			{
				{"user_id",FALSE},
				{"tester_id",FALSE},
				{"rights_tester",FALSE},
				{"rights_dbs",FALSE},
				{"time",FALSE},
				{"admin_id",FALSE},
				{"property",FALSE}
			},
			FALSE
		}
	}
}, *SValeoSchemaPtr = &SValeoSchema;

#endif // !defined(__DBS_SCHEMA_H__)
