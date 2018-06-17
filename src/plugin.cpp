#include "common.hpp"
#include "plugin.hpp"
#include <algorithm>
#include <functional>
#include <vector>


#if defined(USE_VERSION_DLL_HOOK_PLUGIN) && USE_VERSION_DLL_HOOK_PLUGIN
static std::vector<HMODULE> hModules;


static void recursiveFileEnumerator(const std::wstring& path, const std::function<void(const std::wstring&)>& func) {
    auto wildPath = path + L"\\*.*";
    WIN32_FIND_DATAW wfd;
    auto h = FindFirstFileW(wildPath.c_str(), &wfd);
    if(h == INVALID_HANDLE_VALUE) {
        return;
    }
    do {
        const auto a = wfd.dwFileAttributes;
        if(a & FILE_ATTRIBUTE_DIRECTORY) {
            if(wfd.cFileName[0] != L'.') {
                auto newPath = path + L"\\" + std::wstring(wfd.cFileName);
                recursiveFileEnumerator(newPath, func);
            }
        } else if((a & (FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_NORMAL)) != 0) {
            func(path + L"\\" + wfd.cFileName);
        }
    } while(FindNextFile(h, &wfd));
}


void loadPluginDlls() {
    wchar_t basePath[MAX_PATH];
    {
        // https://stackoverflow.com/a/6924332
        HMODULE h = nullptr;
        if (!GetModuleHandleExW(
              GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT
            , (LPCWSTR) &__FUNCTION__
            , &h)
        ) {
            return;
        }
        GetModuleFileNameW(h, basePath, _countof(basePath));
        DEBUG_TRACE(L"%s: basePath=[%s]", APPNAME, basePath);
    }

    wchar_t pluginsPath[MAX_PATH];
    swprintf_s(pluginsPath, L"%s%s", basePath, L".plugins");
    DEBUG_TRACE(L"%s: pluginsPath=[%s]", APPNAME, pluginsPath);

    std::vector<std::wstring> pluginFilenames;
    recursiveFileEnumerator(pluginsPath, [&](const std::wstring& filePath) {
        pluginFilenames.push_back(filePath);
    });
    std::sort(pluginFilenames.begin(), pluginFilenames.end());

    {
        for(const auto& pluginFilename : pluginFilenames) {
            auto* filename = pluginFilename.c_str();
            DEBUG_TRACE(L"%s: filename %s", APPNAME, filename);
            auto hm = LoadLibrary(filename);
            if(hm == nullptr) {
                DEBUG_TRACE(L"%s: FAILED TO LOAD %s", APPNAME, filename);
            } else {
                hModules.push_back(hm);
                DEBUG_TRACE(L"%s: load %s (%p)", APPNAME, filename, hm);
            }
        }
    }
}


void unloadPluginDlls() {
    for(auto it = hModules.rbegin(); it != hModules.rend(); ++it) {
        const auto h = *it;
        if(h != nullptr) {
            wchar_t moduleName[4096];
            GetModuleFileNameW(h, moduleName, _countof(moduleName));
            DEBUG_TRACE(L"%s: unload %s (%p)", APPNAME, moduleName, h);
            FreeLibrary(h);
        }
    }
}

#else

void loadPluginDlls() {}
void unloadPluginDlls() {}

#endif
