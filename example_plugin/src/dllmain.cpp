#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifndef _WIN64
#  error This code must be compiled with x64 configuration
#endif
#pragma comment(linker, "/DLL")
#include "minhook.h"
#include <stdio.h>


namespace Original {
    decltype(&CreateFileW) CreateFileW {};
}


namespace Detour {
    HANDLE WINAPI CreateFileW(
        LPCWSTR               lpFileName,
        DWORD                 dwDesiredAccess,
        DWORD                 dwShareMode,
        LPSECURITY_ATTRIBUTES lpSecurityAttributes,
        DWORD                 dwCreationDisposition,
        DWORD                 dwFlagsAndAttributes,
        HANDLE                hTemplateFile
    ) {
        wchar_t buf[256];
        swprintf_s(buf, L"Example Plugin Hook: CreateFileW(%s, %08x)", lpFileName, dwDesiredAccess);
        OutputDebugStringW(buf);
        return Original::CreateFileW(
            lpFileName,
            dwDesiredAccess,
            dwShareMode,
            lpSecurityAttributes,
            dwCreationDisposition,
            dwFlagsAndAttributes,
            hTemplateFile
        );
    }
}


void attach() {
    using namespace MinHookApi;
OutputDebugStringA("ATTACH: 1");
    MH_Initialize();
OutputDebugStringA("ATTACH: 2");
	{
        wchar_t buf[256];
        swprintf_s(buf, L"MH_CreateHook=%p\n", MH_CreateHook);
        OutputDebugStringW(buf);
	}
    MH_CreateHook(&CreateFileW, &Detour::CreateFileW, reinterpret_cast<void**>(&Original::CreateFileW));
OutputDebugStringA("ATTACH: 3");
    MH_EnableHook(&CreateFileW);
OutputDebugStringA("ATTACH: 4");
}


void detach() {
    using namespace MinHookApi;
    MH_DisableHook(&MessageBoxW);
}


BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID) {
    switch(ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        attach();
        break;

    case DLL_PROCESS_DETACH:
        detach();
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
        break;
    }

    return TRUE;
}
