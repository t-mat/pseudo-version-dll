#include "common.hpp"
#if defined(EXPORT_LZ32_DLL_FUNCTIONS) && (EXPORT_LZ32_DLL_FUNCTIONS)
#include "lz32_dll.hpp"

///////////////////////////////////////////////////////////////
static HMODULE hModuleDll = nullptr;
static const wchar_t dllFname[] = L"lz32";

namespace lz32_dll {

bool checkFname(const wchar_t* fname) {
    return _wcsicmp(fname, dllFname) == 0;
}

void loadGenuineDll(const wchar_t* systemDirectory) {
    DEBUG_TRACE(L"loadGenuineDll : begin, dllFname=%s", dllFname);
    unloadGenuineDll();

    // systemDirectory : "C:\Windows\System32"
    // fullpathDllName : "C:\Windows\System32\<dllFname>.dll"
    wchar_t fullpathDllName[MAX_PATH];
	swprintf_s(fullpathDllName, std::size(fullpathDllName), L"%s\\%s.dll", systemDirectory, dllFname);
    DEBUG_TRACE(L"loadGenuineDll : fullpathDllName = %s", fullpathDllName);

    // Load "genuine" DLL
    hModuleDll = LoadLibraryW(fullpathDllName);
    DEBUG_TRACE(L"hModuleDll(%s) = 0x%p", fullpathDllName, hModuleDll);

    DEBUG_TRACE(L"loadGenuineDll : end");
}

void unloadGenuineDll() {
    if(hModuleDll == nullptr) {
        return;
    }
    FreeLibrary(hModuleDll);
    hModuleDll = nullptr;
}

} // namespace lz32_dll


///////////////////////////////////////////////////////////////
template<typename T>
void setup(T*& funcPtr, const char* funcName) {
    if(funcPtr != nullptr) {
        return;
    }
    funcPtr = reinterpret_cast<T*>(GetProcAddress(hModuleDll, funcName));
}

#define D(funcname, ...)            \
    static decltype(funcname)* p;   \
    setup(p, #funcname);            \
    return p(__VA_ARGS__);          \
    __pragma(comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__))


extern "C" INT  WINAPI LZStart(VOID) {
    D(LZStart);
}

extern "C" VOID WINAPI LZDone(VOID) {
    D(LZDone);
}

extern "C" LONG WINAPI CopyLZFile(INT hfSource, INT hfDest) {
    D(CopyLZFile, hfSource, hfDest);
}

extern "C" LONG WINAPI LZCopy(INT hfSource, INT hfDest) {
    D(LZCopy, hfSource, hfDest);
}

extern "C" INT  WINAPI LZInit(INT hfSource) {
    D(LZInit, hfSource);
}

extern "C" INT  WINAPI GetExpandedNameA(LPSTR lpszSource, LPSTR lpszBuffer) {
    D(GetExpandedNameA, lpszSource, lpszBuffer);
}

extern "C" INT  WINAPI GetExpandedNameW(LPWSTR lpszSource, LPWSTR lpszBuffer) {
    D(GetExpandedNameW, lpszSource, lpszBuffer);
}

extern "C" INT  WINAPI LZOpenFileA(LPSTR lpFileName, LPOFSTRUCT lpReOpenBuf, WORD wStyle) {
    D(LZOpenFileA, lpFileName, lpReOpenBuf, wStyle);
}

extern "C" INT  WINAPI LZOpenFileW(LPWSTR lpFileName, LPOFSTRUCT lpReOpenBuf, WORD wStyle) {
    D(LZOpenFileW, lpFileName, lpReOpenBuf, wStyle);
}

extern "C" LONG WINAPI LZSeek(INT hFile, LONG lOffset, INT iOrigin) {
    D(LZSeek, hFile, lOffset, iOrigin);
}

extern "C" INT APIENTRY LZRead(INT hFile, CHAR* lpBuffer, INT cbRead) {
    D(LZRead, hFile, lpBuffer, cbRead);
}

extern "C" VOID APIENTRY LZClose(INT hFile) {
    D(LZClose,hFile);
}
#else // EXPORT_LZ32_DLL_FUNCTIONS
namespace lz32_dll {
    bool checkFname(const wchar_t*) { return false; }
    void loadGenuineDll(const wchar_t*) {}
    void unloadGenuineDll() {}
}
#endif // EXPORT_LZ32_DLL_FUNCTIONS
