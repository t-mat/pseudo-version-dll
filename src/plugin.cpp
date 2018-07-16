#include "common.hpp"
#include "plugin.hpp"
#include <functional>
#include <array>
#include <vector>


#if defined(USE_VERSION_DLL_HOOK_PLUGIN) && USE_VERSION_DLL_HOOK_PLUGIN
static std::vector<HMODULE> hModules;

struct Path : std::array<wchar_t, MAX_PATH+1> {
    Path() : Path{L""} {}
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

static Path getModuleFilenameW(HMODULE h) {
    std::array<wchar_t, MAX_PATH> moduleName;
    GetModuleFileNameW(h, moduleName.data(), static_cast<UINT>(moduleName.size()));
	return Path(moduleName.data());
}

static void recursiveFileEnumerator(const wchar_t* path, const std::function<void(const wchar_t*)>& func) {
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
            func(newPath);
        }
    } while(FindNextFile(h, &wfd));
}


namespace plugin {
void loadPluginDlls() {
    unloadPluginDlls();

    std::array<wchar_t,MAX_PATH> basePath;
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
        GetModuleFileNameW(h, basePath.data(), static_cast<UINT>(basePath.size()));
        DEBUG_TRACE(L"basePath=[%s]", basePath.data());
    }

    Path pluginsPath = Path::make(L"%s%s", basePath.data(), L".plugins");
    DEBUG_TRACE(L"pluginsPath=[%s]", pluginsPath);

    std::vector<Path> pluginFilenames;
    recursiveFileEnumerator(pluginsPath, [&](const wchar_t* path) {
        pluginFilenames.push_back(path);
    });
    std::sort(pluginFilenames.begin(), pluginFilenames.end());

    for(const auto& pluginFilename : pluginFilenames) {
        auto* filename = static_cast<const wchar_t*>(pluginFilename);
        auto hm = LoadLibraryW(filename);
        if(hm == nullptr) {
            DEBUG_TRACE(L"FAILED TO LOAD %s", filename);
        } else {
            hModules.push_back(hm);
            DEBUG_TRACE(L"load %s (%p)", filename, hm);
        }
    }
}


void unloadPluginDlls() {
    for(auto it = hModules.rbegin(); it != hModules.rend(); ++it) {
        const auto h = *it;
        if(h != nullptr) {
            DEBUG_TRACE(L"unload %s (%p)", getModuleFilenameW(h).data(), h);
            FreeLibrary(h);
        }
    }
    hModules.clear();
}

} // plugin

#else

namespace plugin {
void loadPluginDlls() {}
void unloadPluginDlls() {}
}

#endif
