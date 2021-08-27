#include <plugins/el_plugin.h>
#include <guid.h>

#if defined(WINDOWS)
	#include <windows.h>
#endif

DEFINE_PLUGIN_EXPORTS();


DEFINE_PLUGIN_INFO( 1,									/*!< The build number. */
					1,									/*!< The major version (e.g. 1.xx).	*/
					10,									/*!< The minor version (e.g. 0.10). */
					TRUE,								/*!< Does this plugin show its arguments to the public? */
					"Report",			  				/*!< The plugin's name. */
					"ARMA",								/*!< The plugin's vendor. */
					"Defautspanel",						/*!< The plugin's general description. */
					"",									/*!< The plugin's additional description. */
					"",		     		 				/*!< The plugin vendor's homepage. */
					"Araibia Mohamed",					/*!< The plugin vendor's email address. */
					PLUGIN_DEFAUTS,                    	/*!< The plugin's UUID. */
					""									/*!< Target */
                    );				            
