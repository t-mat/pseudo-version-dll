#include "common.hpp"
#if defined(EXPORT_MSIMG32_DLL_FUNCTIONS) && (EXPORT_MSIMG32_DLL_FUNCTIONS)
#include "msimg32_dll.hpp"

// note : the following command unveils everything about MSIMG32.dll.
// dumpbin C:\WINDOWS\SYSTEM32\MSIMG32.dll /disasm /exports

extern "C" {

typedef struct {
    BYTE BlendOp;
    BYTE BlendFlags;
    BYTE SourceConstantAlpha;
    BYTE AlphaFormat;
} BLENDFUNCTION;

typedef USHORT COLOR16;

typedef struct {
    LONG    x;
    LONG    y;
    COLOR16 Red;
    COLOR16 Green;
    COLOR16 Blue;
    COLOR16 Alpha;
} TRIVERTEX, *PTRIVERTEX;

// Empty function.
void WINAPI vSetDdrawflag(void);

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-alphablend
BOOL WINAPI AlphaBlend(
    HDC           hdcDest,
    int           xoriginDest,
    int           yoriginDest,
    int           wDest,
    int           hDest,
    HDC           hdcSrc,
    int           xoriginSrc,
    int           yoriginSrc,
    int           wSrc,
    int           hSrc,
    BLENDFUNCTION ftn
);

// Entry point
// https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain
BOOL WINAPI DllInitialize(
    HINSTANCE hinstDLL,
    DWORD     fdwReason,
    LPVOID    lpvReserved
);

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-gradientfill
BOOL WINAPI GradientFill(
    HDC        hdc,
    PTRIVERTEX pVertex,
    ULONG      nVertex,
    PVOID      pMesh,
    ULONG      nMesh,
    ULONG      ulMode
);

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-transparentblt
BOOL WINAPI TransparentBlt(
    HDC  hdcDest,
    int  xoriginDest,
    int  yoriginDest,
    int  wDest,
    int  hDest,
    HDC  hdcSrc,
    int  xoriginSrc,
    int  yoriginSrc,
    int  wSrc,
    int  hSrc,
    UINT crTransparent
);

} // end of extern "C"


///////////////////////////////////////////////////////////////
static HMODULE hModuleDll = nullptr;
static const wchar_t dllFname[] = L"msimg32";

namespace msimg32_dll {

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

} // namespace msimg32_dll


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


extern "C" void WINAPI vSetDdrawflag(void) {
    D(vSetDdrawflag);
}

extern "C" BOOL WINAPI AlphaBlend(
    HDC           hdcDest,
    int           xoriginDest,
    int           yoriginDest,
    int           wDest,
    int           hDest,
    HDC           hdcSrc,
    int           xoriginSrc,
    int           yoriginSrc,
    int           wSrc,
    int           hSrc,
    BLENDFUNCTION ftn
) {
    D(AlphaBlend, hdcDest, xoriginDest, yoriginDest, wDest, hDest, hdcSrc, xoriginSrc, yoriginSrc, wSrc, hSrc, ftn);
}

extern "C" BOOL WINAPI DllInitialize(
    HINSTANCE hinstDLL,
    DWORD     fdwReason,
    LPVOID    lpvReserved
) {
    (void) hinstDLL;
    (void) fdwReason;
    (void) lpvReserved;
//  DllMain(hinstDLL, fdwReason, lpvReserved);
//  D(DllInitialize, hinstDLL, fdwReason, lpvReserved);
    return TRUE;
}

extern "C" BOOL WINAPI GradientFill(
    HDC        hdc,
    PTRIVERTEX pVertex,
    ULONG      nVertex,
    PVOID      pMesh,
    ULONG      nMesh,
    ULONG      ulMode
) {
    D(GradientFill, hdc, pVertex, nVertex, pMesh, nMesh, ulMode);
}

extern "C" BOOL WINAPI TransparentBlt(
    HDC  hdcDest,
    int  xoriginDest,
    int  yoriginDest,
    int  wDest,
    int  hDest,
    HDC  hdcSrc,
    int  xoriginSrc,
    int  yoriginSrc,
    int  wSrc,
    int  hSrc,
    UINT crTransparent
) {
    D(TransparentBlt, hdcDest, xoriginDest, yoriginDest, wDest, hDest, hdcSrc, xoriginSrc, yoriginSrc, wSrc, hSrc, crTransparent);
}
#else // EXPORT_MSIMG32_DLL_FUNCTIONS
namespace msimg32_dll {
    bool checkFname(const wchar_t*) { return false; }
    void loadGenuineDll(const wchar_t*) {}
    void unloadGenuineDll() {}
}
#endif // EXPORT_MSIMG32_DLL_FUNCTIONS
