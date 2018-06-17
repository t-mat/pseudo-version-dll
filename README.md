# pseudo-version-dll

## What's this?

This is a Visual Studio project which generates (pseudo) version.dll which mimics genuine version.dll which is shipped with every Windows OS.
The purpose is hooking/hacking target executable.  You may also interest in [minhook](https://github.com/TsudaKageyu/minhook) which is framework for hooking WIN32 APIs.


## How to build

- Open `version.sln` with Visual Studio 2017
- Build x64/Debug or x64/Release
- x64/Debug/version.dll or x64/Release/version.dll will be produced.


## How to use

- Put your `version.dll` to same directory of your target executable.
- This version.dll loads all DLLs which is placed in `YOUR-version.dll-PATH/version.dll.plugins` by `LoadLibrary()`.

For example, if your target executable is `C:\foo\bar\baz.exe`,
- Copy your `version.dll` as `C:\foo\bar\version.dll`
- Create directory `C:\foo\bar\version.dll.plugins\`
- Put your plugin DLLs to `C:\foo\bar\version.dll.plugins\`.  (e.g. `C:\foo\bar\version.dll.plugins\myplugin.dll`
