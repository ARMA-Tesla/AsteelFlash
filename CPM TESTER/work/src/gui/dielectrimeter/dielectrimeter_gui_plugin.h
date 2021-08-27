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
					"Dielectrimeter",			  		/*!< The plugin's name. */
					"ARMA",								/*!< The plugin's vendor. */
					"Dielectrimeter maintenance",		/*!< The plugin's general description. */
					"",									/*!< The plugin's additional description. */
					"",		      						/*!< The plugin vendor's homepage. */
					"",									/*!< The plugin vendor's email address. */
					PLUGIN_DIELECTRIMETER,				/*!< The plugin's UUID. */
					PLUGIN_CARD							/*!< Target */
                    );				            
