#include "common.hpp"
#include "minhook_api.hpp"
#include "minhook.h"

namespace minhook_api {

#if defined(USE_MINHOOK) && (USE_MINHOOK == 1)
void init() {
    if(MH_Initialize() != MH_OK) {
        DEBUG_TRACE("MH_Initialize : failed\n");
    }
}

void cleanup() {
    MH_Uninitialize();
}

#define DllExport extern "C" __declspec(dllexport)

DllExport MH_STATUS WINAPI _MH_Initialize(void) {
                     return MH_Initialize();
}

DllExport MH_STATUS WINAPI _MH_Uninitialize(void) {
                     return MH_Uninitialize();
}

DllExport MH_STATUS WINAPI _MH_CreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID *ppOriginal) {
                     return MH_CreateHook(pTarget, pDetour, ppOriginal);
}

DllExport MH_STATUS WINAPI _MH_CreateHookApi(LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, LPVOID *ppOriginal) {
                     return MH_CreateHookApi(pszModule, pszProcName, pDetour, ppOriginal);
}

DllExport MH_STATUS WINAPI _MH_CreateHookApiEx(LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, LPVOID *ppOriginal, LPVOID *ppTarget) {
                     return MH_CreateHookApiEx(pszModule, pszProcName, pDetour, ppOriginal, ppTarget);
}

DllExport MH_STATUS WINAPI _MH_RemoveHook(LPVOID pTarget) {
                     return MH_RemoveHook(pTarget);
}

DllExport MH_STATUS WINAPI _MH_EnableHook(LPVOID pTarget) {
                     return MH_EnableHook(pTarget);
}

DllExport MH_STATUS WINAPI _MH_DisableHook(LPVOID pTarget) {
                     return MH_DisableHook(pTarget);
}

DllExport MH_STATUS WINAPI _MH_QueueEnableHook(LPVOID pTarget) {
                     return MH_QueueEnableHook(pTarget);
}

DllExport MH_STATUS WINAPI _MH_QueueDisableHook(LPVOID pTarget) {
                     return MH_QueueDisableHook(pTarget);
}

DllExport MH_STATUS WINAPI _MH_ApplyQueued(void) {
                     return MH_ApplyQueued();
}

DllExport const char * WINAPI _MH_StatusToString(MH_STATUS status) {
                        return MH_StatusToString(status);
}

#else

void init() {}
void cleanup() {}

#endif
} // namespace minhook
