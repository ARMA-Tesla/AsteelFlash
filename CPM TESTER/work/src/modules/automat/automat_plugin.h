#include <plugins/el_plugin.h>
#include <guid.h>

#if defined(WINDOWS)
	#include <windows.h>
#endif

DEFINE_PLUGIN_EXPORTS();

DEFINE_PLUGIN_INFO( 2,									/*!< The build number. */
					0,									/*!< The major version (e.g. 1.xx).	*/
					00,									/*!< The minor version (e.g. 0.10). */
					TRUE,								/*!< Does this plugin show its arguments to the public? */
					"Automat",			  				/*!< The plugin's name. */
					"ELCOM, a.s.",						/*!< The plugin's vendor. */
					"Automat",							/*!< The plugin's general description. */
					"",									/*!< The plugin's additional description. */
					"http://dvi.elcom.cz",		      	/*!< The plugin vendor's homepage. */
					"zdenek.rykala@elcom.cz",			/*!< The plugin vendor's email address. */
					PLUGIN_AUTOMAT,                     /*!< The plugin's UUID. */
					""									/*!< Target */
                    );				            
