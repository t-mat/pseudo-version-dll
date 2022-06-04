# pseudo-version-dll

## What's this?

This is a Visual Studio 2022 project which generates (pseudo) `version.dll`.  Since it passes all arguments to genuine `version.dll`, virtually both of them have same functionality as `version.dll`.

Since many Windows executables load `version.dll`, you can use this project to add functionalities/hooks/hacks to target executable.
This project also provides same functionality for `lz32.dll`, `hid.dll` and `msimg32.dll`.

For hooking purpose, you may also interest in API hook library [minhook](https://github.com/TsudaKageyu/minhook).


## How to build

- Run build-vs2022.bat
- Artifacts will be put under artifacts/
  - If you need, you can rename `version.dll` to `lz32.dll`, `hid.dll` or `msimg32.dll`.


## Test

- Check your target executable by [`Dependencies`](https://github.com/lucasg/Dependencies).
  - If your target doesn't use `version.dll` or supported DLLs, you can't use this project.
- Put `artifacts/version.dll` (or renamed one) into your tareget's directory.  Executable and DLL must be put on the same directory.
  - If your target is `<PATH>/<TO>/<YOUR>/<TARGET>.exe` you must put `version.dll` in `<PATH>/<TO>/<YOUR>/version.dll`
- Put `artifacts/plugins/example_plugin.dll` to `<PATH>/<TO>/<YOUR>/<TARGET>.exe.plugins/example_plugin.dll`
- Open [DebugView](https://docs.microsoft.com/en-us/sysinternals/downloads/debugview)
- Run your`<TARGET>.exe`
  - DebugView shows `CreateFileW()` activities which is hooked by `example_plugin.dll`.


## Plugin loading rules

- `version.dll` loads all DLLs which are placed in `YOUR-TARGET.exe-PATH/YOUR-TARGET.exe.plugins/`.
    - DLLs are loaded from mutiple directories recursively.
        - If directory's or file's name is started with `.`, they're ignored. e.g. `.my-private-dir/`, `.my-private-file.dll`, etc.
    - DLL loading order is decided by `std::sort()` for `std::wstring` paths of DLLs.
        - e.g. `YOUR-PATH\01\*.dll` is loaded before `YOUR-PATH\99\*.dll` 
