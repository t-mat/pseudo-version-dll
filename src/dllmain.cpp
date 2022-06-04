#include "common.hpp"
#include "version_dll.hpp"
#include "lz32_dll.hpp"
#include "hid_dll.hpp"
#include "msimg32_dll.hpp"
#include "plugin.hpp"
#include "minhook_api.hpp"
#include <mutex>            // std::{once_flag, call_once}

namespace {
    bool isWin64() {
    #if defined(_WIN64)
        DEBUG_TRACE(L"isWin64 : _WIN64");
        return true;
    #else
        DEBUG_TRACE(L"isWin64 : _WIN32");
        BOOL wow64Process = FALSE;
        return (IsWow64Process(GetCurrentProcess(), &wow64Process) != 0) && (wow64Process != 0);
    #endif
    }

    DllType determineDllType(const wchar_t* dllFilename) {
        if(version_dll::checkFname(dllFilename)) { return DllType::Version; }
        if(lz32_dll::checkFname(dllFilename))    { return DllType::Lz32; }
        if(hid_dll::checkFname(dllFilename))     { return DllType::Hid; }
        if(msimg32_dll::checkFname(dllFilename)) { return DllType::Msimg32; }
        return DllType::Unknown;
    }

    void loadGenuineDll(DllType dllType, const wchar_t* systemDirectory) {
        switch(dllType) {
        default: break;
        case DllType::Version:  version_dll::loadGenuineDll(systemDirectory);   break;
        case DllType::Lz32:     lz32_dll::loadGenuineDll(systemDirectory);      break;
        case DllType::Hid:      hid_dll::loadGenuineDll(systemDirectory);       break;
        case DllType::Msimg32:  msimg32_dll::loadGenuineDll(systemDirectory);   break;
        }
    }

    void unloadGenuineDll(DllType dllType) {
        switch(dllType) {
        default: break;
        case DllType::Version:  version_dll::unloadGenuineDll();    break;
        case DllType::Lz32:     lz32_dll::unloadGenuineDll();       break;
        case DllType::Hid:      hid_dll::unloadGenuineDll();        break;
        case DllType::Msimg32:  msimg32_dll::unloadGenuineDll();    break;
        }
    }
}


namespace {
    DllType dllType = DllType::Unknown;

    void init(HMODULE hModule) {
        DEBUG_TRACE(L"init : begin");
        minhook_api::init();

        wchar_t systemDirectory[MAX_PATH + 1];
        {
            const auto w64 = isWin64();
            DEBUG_TRACE(L"init : isWin64=%d", w64);
            if(w64) {
                GetSystemDirectoryW(systemDirectory, static_cast<UINT>(std::size(systemDirectory)));
            } else {
                GetSystemWow64DirectoryW(systemDirectory, static_cast<UINT>(std::size(systemDirectory)));
            }
            DEBUG_TRACE(L"init : systemDirectory=[%s]", systemDirectory);
        }

        {
            wchar_t moduleFullpathFilename[MAX_PATH + 1];
            {
                GetModuleFileNameW(hModule, moduleFullpathFilename, static_cast<UINT>(std::size(moduleFullpathFilename)));
                SetEnvironmentVariableW(L"VERSION_DLL_PLUGIN_PROVIDER", moduleFullpathFilename);
                DEBUG_TRACE(L"init : moduleFullpathFilename=[%s]", moduleFullpathFilename);
            }

            wchar_t fname[_MAX_FNAME+1];
            {
                wchar_t drive[_MAX_DRIVE+1];
                wchar_t dir[_MAX_DIR+1];
                wchar_t ext[_MAX_EXT+1];
                _wsplitpath_s(moduleFullpathFilename, drive, dir, fname, ext);
                DEBUG_TRACE(L"init : fname=[%s]", fname);
            }

            dllType = determineDllType(fname);
            DEBUG_TRACE(L"init : dllType=[%d]", dllType);
        }

        loadGenuineDll(dllType, systemDirectory);
        plugin::loadPluginDlls();
        {
            //
            // *** You can put your own init code here ***
            //
        }
        DEBUG_TRACE(L"init : end");
    }

    void cleanup() {
        DEBUG_TRACE(L"cleanup : begin");
        {
            //
            // *** You can put your own cleanup code here ***
            //
        }
        plugin::unloadPluginDlls();
        unloadGenuineDll(dllType);
        minhook_api::cleanup();
        DEBUG_TRACE(L"cleanup : end");
    }
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID) {
    static std::once_flag initFlag;
    static std::once_flag cleanupFlag;

    switch(ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DEBUG_TRACE(L"DLL_PROCESS_ATTACH (hModule=%p) : begin", hModule);
        std::call_once(initFlag, [&]() { init(hModule); });
        DEBUG_TRACE(L"DLL_PROCESS_ATTACH (hModule=%p) : end", hModule);
        break;

    case DLL_PROCESS_DETACH:
        DEBUG_TRACE(L"DLL_PROCESS_DETACH (hModule=%p) : begin", hModule);
        std::call_once(cleanupFlag, [&]() { cleanup(); });
        DEBUG_TRACE(L"DLL_PROCESS_DETACH (hModule=%p) : end", hModule);
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
        break;
    }

    return TRUE;
}
