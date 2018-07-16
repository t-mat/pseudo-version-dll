#pragma once
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(linker, "/DLL")
//
// If you want to use "plugin" system, define the following macro USE_VERSION_DLL_HOOK_PLUGIN.
// When you define this macro, pseudo version.dll will load DLLs from the following path (recursively, dictionary order).
//    <PATH-OF-YOUR-version.dll>\version.dll.plugins\**.dll
// All native DLLs are loaded by LoadLibrary(), and unloaded by FreeLibrary().
//
// Unless, comment out the following line.  All plugin system completely removed.
//
#define USE_VERSION_DLL_HOOK_PLUGIN 1
#define USE_DEBUG_TRACE

#include "outputdebugstring.hpp"
#define APPNAME L"PSEUDO-version.dll"
