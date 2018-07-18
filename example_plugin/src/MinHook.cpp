#include "MinHook.h"
#include <stdio.h>

namespace {
    static const char* PluginManagerDll = "version.dll";

    static HMODULE GetVersionDllHmodule() {
        HMODULE h = nullptr;
        if(GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, PluginManagerDll, &h)) {
            return h;
        }
        return nullptr;
    }
}


namespace MinHookApi {
//  decltype(&Decl::MH_Initialize       ) MH_Initialize         = nullptr;
//  decltype(&Decl::MH_Uninitialize     ) MH_Uninitialize       = nullptr;
    decltype(&Decl::MH_CreateHook       ) MH_CreateHook         = nullptr;
    decltype(&Decl::MH_CreateHookApi    ) MH_CreateHookApi      = nullptr;
    decltype(&Decl::MH_CreateHookApiEx  ) MH_CreateHookApiEx    = nullptr;
    decltype(&Decl::MH_RemoveHook       ) MH_RemoveHook         = nullptr;
    decltype(&Decl::MH_EnableHook       ) MH_EnableHook         = nullptr;
    decltype(&Decl::MH_DisableHook      ) MH_DisableHook        = nullptr;
    decltype(&Decl::MH_QueueEnableHook  ) MH_QueueEnableHook    = nullptr;
    decltype(&Decl::MH_QueueDisableHook ) MH_QueueDisableHook   = nullptr;
    decltype(&Decl::MH_ApplyQueued      ) MH_ApplyQueued        = nullptr;
    decltype(&Decl::MH_StatusToString   ) MH_StatusToString     = nullptr;

    MH_STATUS MH_Initialize() {
        HMODULE h = GetVersionDllHmodule();
        if(h == nullptr) {
            OutputDebugStringA("MH_Initialize : Can't load host DLL");
            return MH_ERROR_NOT_INITIALIZED;
        }

//      * reinterpret_cast<void**>(&MH_Initialize       ) = GetProcAddress(h, "_MH_Initialize");
//      * reinterpret_cast<void**>(&MH_Uninitialize     ) = GetProcAddress(h, "_MH_Uninitialize");
        * reinterpret_cast<void**>(&MH_CreateHook       ) = GetProcAddress(h, "_MH_CreateHook");
        * reinterpret_cast<void**>(&MH_CreateHookApi    ) = GetProcAddress(h, "_MH_CreateHookApi");
        * reinterpret_cast<void**>(&MH_CreateHookApiEx  ) = GetProcAddress(h, "_MH_CreateHookApiEx");
        * reinterpret_cast<void**>(&MH_RemoveHook       ) = GetProcAddress(h, "_MH_RemoveHook");
        * reinterpret_cast<void**>(&MH_EnableHook       ) = GetProcAddress(h, "_MH_EnableHook");
        * reinterpret_cast<void**>(&MH_DisableHook      ) = GetProcAddress(h, "_MH_DisableHook");
        * reinterpret_cast<void**>(&MH_QueueEnableHook  ) = GetProcAddress(h, "_MH_QueueEnableHook");
        * reinterpret_cast<void**>(&MH_QueueDisableHook ) = GetProcAddress(h, "_MH_QueueDisableHook");
        * reinterpret_cast<void**>(&MH_ApplyQueued      ) = GetProcAddress(h, "_MH_ApplyQueued");
        * reinterpret_cast<void**>(&MH_StatusToString   ) = GetProcAddress(h, "_MH_StatusToString");
        return MH_OK;
    }

    MH_STATUS MH_Uninitialize() {
        return MH_OK;
    }
}
