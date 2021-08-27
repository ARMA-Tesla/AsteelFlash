#include <plugins/el_plugin.h>
#include <guid.h>

#if defined(WINDOWS)
	#include <windows.h>
#endif

DEFINE_PLUGIN_EXPORTS();


DEFINE_PLUGIN_INFO( 1,								/*!< The build number. */
					0,								/*!< The major version (e.g. 1.xx).	*/
					00,								/*!< The minor version (e.g. 0.10). */
					TRUE,							/*!< Does this plugin show its arguments to the public? */
					"Panel",			  			/*!< The plugin's name. */
					"ARMA, s.a.r.l.",				/*!< The plugin's vendor. */
					"Panel",						/*!< The plugin's general description. */
					"",								/*!< The plugin's additional description. */
					"www.arma-consulting.com",		/*!< The plugin vendor's homepage. */
					"m.araibia@arma-consulting.com",/*!< The plugin vendor's email address. */
					PLUGIN_PANEL,         			/*!< The plugin's UUID. */
					""								/*!< Target */
                    );				            
