#pragma once
namespace hid_dll {
    bool checkFname(const wchar_t* name);
    void loadGenuineDll(const wchar_t* systemDirectory);
    void unloadGenuineDll();
}
