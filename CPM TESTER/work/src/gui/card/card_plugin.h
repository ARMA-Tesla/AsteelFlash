#include <plugins/el_plugin.h>
#include <guid.h>

#if defined(WINDOWS)
	#include <windows.h>
#endif

DEFINE_PLUGIN_EXPORTS();


DEFINE_PLUGIN_INFO( 1,									/*!< The build number. */
					1,									/*!< The major version (e.g. 1.xx).	*/
					0,									/*!< The minor version (e.g. 0.10). */
					TRUE,								/*!< Does this plugin show its arguments to the public? */
					"Cards",		  					/*!< The plugin's name. */
					"ARMA, S.A.R.L",					/*!< The plugin's vendor. */
					"PLC driver",						/*!< The plugin's general description. */
					"",									/*!< The plugin's additional description. */
					"www.arma-consulting.com",		   	/*!< The plugin vendor's homepage. */
					"m.araibia@arma-consulting.com",	/*!< The plugin vendor's email address. */
					PLUGIN_CARD,                     	/*!< The plugin's UUID. */
					PLUGIN_MAINTENANCE					/*!< Target */
                    );				            
