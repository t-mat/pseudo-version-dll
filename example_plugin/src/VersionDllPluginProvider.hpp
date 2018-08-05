#pragma once
#include "outputdebugstring.hpp"
#include <tchar.h>

namespace VersionDll { namespace PluginProvider {

inline const wchar_t* getEnvironmentVariableName() {
    return L"VERSION_DLL_PLUGIN_PROVIDER";
}

inline HMODULE getDllHmodule() {
    DEBUG_TRACE(L"%s : begin", _T(__FUNCSIG__));

	HMODULE h = nullptr;

    const auto* name = getEnvironmentVariableName();
    const size_t maxLength = MAX_PATH+1;

    wchar_t pluginProvider[maxLength];
    if(0 == GetEnvironmentVariableW(name, pluginProvider, maxLength)) {
        outputDebugString(L"%s : failed to GetEnvironmentVariableW(%s)", _T(__FUNCSIG__), name);
        return nullptr;
    }

    if(! GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, pluginProvider, &h)) {
        outputDebugString(L"%s : failed to GetModuleHandleExW(%s)", _T(__FUNCSIG__), pluginProvider);
        return nullptr;
    }

    DEBUG_TRACE(L"%s : end : h=0x%p", _T(__FUNCSIG__), h);
    return h;
}

}} // namespace VersionDll::PluginProvider
