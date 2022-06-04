#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <array>

void outputDebugString(const wchar_t* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::array<wchar_t, 1024> buf;
    vswprintf_s(buf.data(), buf.size(), fmt, args);
    OutputDebugStringW(buf.data());
    va_end(args);
}

void outputDebugString(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::array<char, 1024> buf;
    vsprintf_s(buf.data(), buf.size(), fmt, args);
    OutputDebugStringA(buf.data());
    va_end(args);
}
