#include "common.hpp"
#include "version_dll.hpp"
#include "plugin.hpp"

///////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID) {
    switch(ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DEBUG_TRACE(L"%s: DLL_PROCESS_ATTACH", APPNAME);
        loadGenuineVersionDll();
        loadPluginDlls();
        {
            //
            // *** You can put your own startup code here ***
            //
        }
        break;

    case DLL_PROCESS_DETACH:
        {
            //
            // *** You can put your own cleanup code here ***
            //
        }
        unloadPluginDlls();
        unloadGenuineVersionDll();
        DEBUG_TRACE("%s: DLL_PROCESS_DETACH", APPNAME);
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
        break;
    }

    return TRUE;
}
