#include "common.hpp"
#include "version_dll.hpp"
#include "plugin.hpp"
#include "minhook_api.hpp"
#include <mutex>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID) {
    static std::once_flag initFlag;
    static std::once_flag cleanupFlag;

    switch(ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        std::call_once(initFlag, [&]() {
            DEBUG_TRACE(L"DLL_PROCESS_ATTACH (hModule=%p)", hModule);
            minhook_api::init();
            version_dll::loadGenuineVersionDll();
            plugin::loadPluginDlls();
            {
                //
                // *** You can put your own startup code here ***
                //
            }
        });
        break;

    case DLL_PROCESS_DETACH:
        std::call_once(cleanupFlag, [&]() {
            {
                //
                // *** You can put your own cleanup code here ***
                //
            }
            plugin::unloadPluginDlls();
            version_dll::unloadGenuineVersionDll();
            minhook_api::cleanup();
            DEBUG_TRACE(L"DLL_PROCESS_DETACH");
        });
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
        break;
    }

    return TRUE;
}
