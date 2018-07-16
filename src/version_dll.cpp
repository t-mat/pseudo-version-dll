#include "common.hpp"
#include "version_dll.hpp"
#include <array>

///////////////////////////////////////////////////////////////
static HMODULE hModuleVersionDll = nullptr;

namespace version_dll {

void loadGenuineVersionDll() {
    unloadGenuineVersionDll();

    // systemDirectory : "C:\Windows\System32"
    std::array<wchar_t, MAX_PATH> systemDirectory;
    GetSystemDirectoryW(systemDirectory.data(), static_cast<UINT>(systemDirectory.size()));

    // fullpathDllName : "C:\Windows\System32\version.dll"
    std::array<wchar_t, MAX_PATH> fullpathDllName;
    swprintf_s(fullpathDllName.data(), fullpathDllName.size(), L"%s\\%s", systemDirectory.data(), L"version.dll");

    // Load "genuine" version.dll
    hModuleVersionDll = LoadLibraryW(fullpathDllName.data());
}

void unloadGenuineVersionDll() {
    if(hModuleVersionDll == nullptr) {
        return;
    }
    FreeLibrary(hModuleVersionDll);
    hModuleVersionDll = nullptr;
}

} // namespace version_dll


///////////////////////////////////////////////////////////////
template<typename T>
void setup(T*& funcPtr, const char* funcName) {
    if(funcPtr != nullptr) {
        return;
    }
    funcPtr = reinterpret_cast<T*>(GetProcAddress(hModuleVersionDll, funcName));
}

#define D(funcname, ...)            \
    static decltype(funcname)* p;   \
    setup(p, #funcname);            \
    return p(__VA_ARGS__);          \


// Function Name     : GetFileVersionInfoA
// Ordinal           : 1 (0x1)
extern "C"    BOOL WINAPI  GetFileVersionInfoA (LPCSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData) {
    D(GetFileVersionInfoA, lptstrFilename, dwHandle, dwLen, lpData);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : GetFileVersionInfoByHandle
// Ordinal           : 2 (0x2)
extern "C" int WINAPI  GetFileVersionInfoByHandle (int hMem, LPCWSTR lpFileName, int v2, int v3) {
    D(GetFileVersionInfoByHandle, hMem, lpFileName, v2, v3);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : GetFileVersionInfoExA
// Ordinal           : 3 (0x3)
extern "C" BOOL WINAPI  GetFileVersionInfoExA (DWORD dwFlags, LPCSTR lpwstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData) {
    D(GetFileVersionInfoExA, dwFlags, lpwstrFilename, dwHandle, dwLen, lpData);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : GetFileVersionInfoExW
// Ordinal           : 4 (0x4)
extern "C" BOOL WINAPI  GetFileVersionInfoExW (DWORD dwFlags, LPCWSTR lpwstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData) {
    D(GetFileVersionInfoExW, dwFlags, lpwstrFilename, dwHandle, dwLen, lpData);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : GetFileVersionInfoSizeA
// Ordinal           : 5 (0x5)
extern "C" DWORD WINAPI  GetFileVersionInfoSizeA (LPCSTR lptstrFilename, LPDWORD lpdwHandle) {
    D(GetFileVersionInfoSizeA, lptstrFilename, lpdwHandle);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : GetFileVersionInfoSizeExA
// Ordinal           : 6 (0x6)
extern "C" DWORD WINAPI  GetFileVersionInfoSizeExA (DWORD dwFlags, LPCSTR lpwstrFilename, LPDWORD lpdwHandle) {
    D(GetFileVersionInfoSizeExA, dwFlags, lpwstrFilename, lpdwHandle);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : GetFileVersionInfoSizeExW
// Ordinal           : 7 (0x7)
extern "C" DWORD WINAPI  GetFileVersionInfoSizeExW (DWORD dwFlags, LPCWSTR lpwstrFilename, LPDWORD lpdwHandle) {
    D(GetFileVersionInfoSizeExW, dwFlags, lpwstrFilename, lpdwHandle);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : GetFileVersionInfoSizeW
// Ordinal           : 8 (0x8)
extern "C" DWORD WINAPI   GetFileVersionInfoSizeW (LPCWSTR lptstrFilename, LPDWORD lpdwHandle) {
    D(GetFileVersionInfoSizeW, lptstrFilename, lpdwHandle);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : GetFileVersionInfoW
// Ordinal           : 9 (0x9)
extern "C" BOOL WINAPI  GetFileVersionInfoW (LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData) {
    D(GetFileVersionInfoW, lptstrFilename, dwHandle, dwLen, lpData);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerFindFileA
// Ordinal           : 10 (0xa)
extern "C" DWORD WINAPI  VerFindFileA (DWORD uFlags, LPCSTR szFileName, LPCSTR szWinDir, LPCSTR szAppDir, LPSTR szCurDir, PUINT lpuCurDirLen, LPSTR szDestDir, PUINT lpuDestDirLen) {
    D(VerFindFileA, uFlags, szFileName, szWinDir, szAppDir, szCurDir, lpuCurDirLen, szDestDir, lpuDestDirLen);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerFindFileW
// Ordinal           : 11 (0xb)
extern "C" DWORD WINAPI  VerFindFileW (DWORD uFlags, LPCWSTR szFileName, LPCWSTR szWinDir, LPCWSTR szAppDir, LPWSTR szCurDir, PUINT lpuCurDirLen, LPWSTR szDestDir, PUINT lpuDestDirLen) {
    D(VerFindFileW, uFlags, szFileName, szWinDir, szAppDir, szCurDir, lpuCurDirLen, szDestDir, lpuDestDirLen);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerInstallFileA
// Ordinal           : 12 (0xc)
extern "C" DWORD WINAPI  VerInstallFileA (DWORD uFlags, LPCSTR szSrcFileName, LPCSTR szDestFileName, LPCSTR szSrcDir, LPCSTR szDestDir, LPCSTR szCurDir, LPSTR szTmpFile, PUINT lpuTmpFileLen) {
    D(VerInstallFileA, uFlags, szSrcFileName, szDestFileName, szSrcDir, szDestDir, szCurDir, szTmpFile, lpuTmpFileLen);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerInstallFileW
// Ordinal           : 13 (0xd)
extern "C" DWORD WINAPI  VerInstallFileW (DWORD uFlags, LPCWSTR szSrcFileName, LPCWSTR szDestFileName, LPCWSTR szSrcDir, LPCWSTR szDestDir, LPCWSTR szCurDir, LPWSTR szTmpFile, PUINT lpuTmpFileLen) {
    D(VerInstallFileW, uFlags, szSrcFileName, szDestFileName, szSrcDir, szDestDir, szCurDir, szTmpFile, lpuTmpFileLen);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerLanguageNameA
// Ordinal           : 14 (0xe)
extern "C" DWORD WINAPI  VerLanguageNameA (DWORD wLang, LPSTR szLang, DWORD cchLang) {
    D(VerLanguageNameA, wLang, szLang, cchLang);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerLanguageNameW
// Ordinal           : 15 (0xf)
extern "C" DWORD WINAPI  VerLanguageNameW (DWORD wLang, LPWSTR szLang, DWORD cchLang) {
    D(VerLanguageNameW, wLang, szLang, cchLang);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerQueryValueA
// Ordinal           : 16 (0x10)
extern "C" BOOL WINAPI  VerQueryValueA (LPCVOID pBlock, LPCSTR lpSubBlock, LPVOID * lplpBuffer, PUINT puLen) {
    D(VerQueryValueA, pBlock, lpSubBlock, lplpBuffer, puLen);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}


// Function Name     : VerQueryValueW
// Ordinal           : 17 (0x11)
extern "C" BOOL WINAPI  VerQueryValueW (LPCVOID pBlock, LPCWSTR lpSubBlock, LPVOID * lplpBuffer, PUINT puLen) {
    D(VerQueryValueW, pBlock, lpSubBlock, lplpBuffer, puLen);
    #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
}
