#pragma once
void outputDebugString(const wchar_t* fmt, ...);
void outputDebugString(const char* fmt, ...);

#ifdef _DEBUG
#  define DEBUG_TRACE(...) outputDebugString(__VA_ARGS__)
#else
#  define DEBUG_TRACE(...)
#endif
