#include "common.hpp"

void outputDebugString(const wchar_t* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    wchar_t buf[1024];
    vswprintf_s(buf, sizeof(buf)/sizeof(buf[0]), fmt, args);
    OutputDebugStringW(buf);
    va_end(args);
}

void outputDebugString(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buf[1024];
    vsprintf_s(buf, sizeof(buf)/sizeof(buf[0]), fmt, args);
    OutputDebugStringA(buf);
    va_end(args);
}
