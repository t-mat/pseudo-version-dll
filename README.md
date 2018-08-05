# pseudo-version-dll

## What's this?

This is a Visual Studio project which generates (pseudo) `version.dll`.  Since it passes all arguments to genuine `version.dll`, virtually both of them have same functionality as `version.dll`.

Since almost all Windows executables load `version.dll`, you can use this project to add functionalities/hooks/hacks to target executable.
For hooking purpose, you may also interest in API hook library [minhook](https://github.com/TsudaKageyu/minhook).


## How to build

- Open `version.sln` with Visual Studio 2017
- Build `x64/Debug` or `x64/Release`
- `x64/Debug/version.dll` or `x64/Release/version.dll` will be produced.


## Test

- Copy `C:\Windows\notepad.exe` to `C:\mytest\notepad.exe`
- Create directory `C:\mytest\notepad.exe.plugins`
- Open and release build `version.sln`.
    - Copy `x64\Release\version.dll` to `C:\mytest\version.dll`
- Open and release build `example_plugin\example_plugin.sln`.
    - Copy `example_plugin\x64\Release\example_plugin.dll` to `C:\mytest\notepad.exe.plugins\example_plugin.dll`
- Open [DebugView](https://docs.microsoft.com/en-us/sysinternals/downloads/debugview)
- Run `C:\mytest\notepad.exe` and open or save some text file.
    - DebugView shows `CreateFileW()` activities which is hooked by `example_plugin.dll`.


## Plugin loading rules

- `version.dll` loads all DLLs which are placed in `YOUR-TARGET.exe-PATH/YOUR-TARGET.exe.plugins/`.
    - DLLs are loaded from mutiple directories recursively.
        - If directory's or file's name is started with `.`, they're ignored. e.g. `.my-private-dir/`, `.my-private-file.dll`, etc.
    - DLL loading order is decided by `std::sort()` for `std::wstring` paths of DLLs.
        - e.g. `YOUR-PATH\01\*.dll` is loaded before `YOUR-PATH\99\*.dll` 
