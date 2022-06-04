#pragma once
//
// If you want to use "plugin" system, define the following macro USE_VERSION_DLL_HOOK_PLUGIN.
// When you define this macro, pseudo version.dll will load DLLs from the following path (recursively, dictionary order).
//    <PATH-OF-YOUR-version.dll>\<YOUR-TARGET-executable>.exe.plugins\**.dll
// All native DLLs are loaded by LoadLibrary(), and unloaded by FreeLibrary().
//
// Unless, comment out the following line.  All plugin system completely removed.
//
#define USE_VERSION_DLL_HOOK_PLUGIN 1


//
// If you want to use MinHook, define the following macro USE_MINHOOK.
//
#define USE_MINHOOK 1


//
// If you define and set '1' to EXPORT_VERSION_DLL_FUNCTIONS, DLL exports version.dll functions
//
#define EXPORT_VERSION_DLL_FUNCTIONS 1


//
// If you define and set '1' to EXPORT_LZ32_DLL_FUNCTIONS, DLL exports lz32.dll functions
//
#define EXPORT_LZ32_DLL_FUNCTIONS 1


//
// If you define and set '1' to EXPORT_HID_DLL_FUNCTIONS, DLL exports hid.dll functions
//
#define EXPORT_HID_DLL_FUNCTIONS 1


//
// If you define and set '1' to EXPORT_MSIMG32_DLL_FUNCTIONS, DLL exports msimg32.dll functions
//
#define EXPORT_MSIMG32_DLL_FUNCTIONS 1


#if defined(_DEBUG)
#define USE_OUTPUT_DEBUG_STRING 1
#define USE_DEBUG_TRACE 1
#else
#define USE_OUTPUT_DEBUG_STRING 0
#define USE_DEBUG_TRACE 0
#endif
