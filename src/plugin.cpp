﻿#include "common.hpp"
#include "plugin.hpp"
#include <functional>
#include <array>
#include <vector>


#if defined(USE_VERSION_DLL_HOOK_PLUGIN) && USE_VERSION_DLL_HOOK_PLUGIN
namespace {
    static std::vector<HMODULE> hModules;

    struct Path : std::array<wchar_t, MAX_PATH+1> {
        Path() : Path {L""} {}

        Path(const wchar_t* s) { wcsncpy_s(data(), size(), s, size()); }

        operator const wchar_t*() const { return data(); }

        static Path make(const wchar_t* fmt, ...) {
            va_list args;
            va_start(args, fmt);
            Path path;
            vswprintf_s(path.data(), path.size(), fmt, args);
            return path;
        }
    };

    Path getModuleFilenameW(HMODULE h) {
        std::array<wchar_t, MAX_PATH> moduleName;
        GetModuleFileNameW(h, moduleName.data(), static_cast<UINT>(moduleName.size()));
        return Path(moduleName.data());
    }

    void recursiveFileEnumerator(const wchar_t* path, const std::function<void(const wchar_t*)>& func) {
        WIN32_FIND_DATAW wfd;
        HANDLE h = FindFirstFileW(Path::make(L"%s\\*.*", path), &wfd);
        if(h == INVALID_HANDLE_VALUE) {
            return;
        }
        do {
            const auto a = wfd.dwFileAttributes;
            const auto newPath = Path::make(L"%s\\%s", path, wfd.cFileName);
            if(a & FILE_ATTRIBUTE_DIRECTORY) {
                if(wfd.cFileName[0] != L'.') {
                    recursiveFileEnumerator(newPath, func);
                }
            } else if((a & (FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_NORMAL)) != 0) {
                if(wfd.cFileName[0] != L'.') {
                    func(newPath);
                }
            }
        } while(FindNextFileW(h, &wfd));
    }
}


namespace plugin {
void loadPluginDlls() {
    DEBUG_TRACE(L"loadPluginDlls : begin");
    unloadPluginDlls();

    Path basePath;
    GetModuleFileNameW(nullptr, basePath.data(), static_cast<DWORD>(basePath.size()));

    Path pluginsPath = Path::make(L"%s%s", basePath.data(), L".plugins");
    DEBUG_TRACE(L"loadPluginDlls : pluginsPath=[%s]", pluginsPath.data());

    std::vector<Path> pluginFilenames;
    recursiveFileEnumerator(pluginsPath, [&](const wchar_t* path) {
        const wchar_t* ext = L".dll";
        const auto extLen = wcslen(ext);
        const auto pathLen = wcslen(path);
        if(pathLen < extLen) {
            return;
        }
        if(_wcsicmp(path + pathLen - extLen, ext) != 0) {
            return;
        }
        pluginFilenames.push_back(path);
    });
    std::sort(pluginFilenames.begin(), pluginFilenames.end());

    for(const auto& pluginFilename : pluginFilenames) {
        auto* filename = static_cast<const wchar_t*>(pluginFilename);
        auto hm = LoadLibraryW(filename);
        if(hm == nullptr) {
            DEBUG_TRACE(L"loadPluginDlls : FAILED TO LOAD %s", filename);
        } else {
            hModules.push_back(hm);
            DEBUG_TRACE(L"loadPluginDlls : load %s (hModule=0x%p)", filename, hm);
        }
    }
    DEBUG_TRACE(L"loadPluginDlls : end");
}


void unloadPluginDlls() {
    DEBUG_TRACE(L"unloadPluginDlls : begin");
    for(auto it = hModules.rbegin(); it != hModules.rend(); ++it) {
        const auto h = *it;
        if(h != nullptr) {
            DEBUG_TRACE(L"unloadPluginDlls : unload %s (hModules=0x%p)", getModuleFilenameW(h).data(), h);
            FreeLibrary(h);
        }
    }
    hModules.clear();
    DEBUG_TRACE(L"unloadPluginDlls : end");
}

} // plugin

#else

namespace plugin {
void loadPluginDlls() {}
void unloadPluginDlls() {}
}

#endif
