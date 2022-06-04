@setlocal enabledelayedexpansion
@echo off
pushd "%~dp0"
set /a errorno=1

rem Set Visual C++ environment for Windows, desktop, x64.
rem https://github.com/Microsoft/vswhere
rem https://github.com/microsoft/vswhere/wiki/Find-VC#batch

set "vswhere=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%vswhere%" (
  echo Can't find "vswhere.exe".   Please install the latest version of Visual Studio.
  set /a errorno=1
  goto :ERROR
)

set "InstallDir="
for /f "usebackq tokens=*" %%i in (`"%vswhere%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
  set "InstallDir=%%i"
)
if "%InstallDir%" == "" (
  echo Can't find Visual C++.   Please install the latest version of Visual C++.
  set /a errorno=1
  goto :ERROR
)

rem Set -no_logo
rem For details, see %InstallDir%\Common7\Tools\vsdevcmd\core\parse_cmd.bat
set __VSCMD_ARG_NO_LOGO=1
echo call "%InstallDir%\VC\Auxiliary\Build\vcvars64.bat"
     call "%InstallDir%\VC\Auxiliary\Build\vcvars64.bat" || goto :ERROR


rem Build .sln file by msbuild

pushd "projects\vs2022\version"
set "SlnFile=version.sln"
echo msbuild "%SlnFile%" /p:Platform=x64   /p:Configuration=Release
     msbuild "%SlnFile%" /p:Platform=x64   /p:Configuration=Release /nologo /v:quiet /m /t:Clean,Build || goto :ERROR
echo msbuild "%SlnFile%" /p:Platform=Win32 /p:Configuration=Release
     msbuild "%SlnFile%" /p:Platform=Win32 /p:Configuration=Release /nologo /v:quiet /m /t:Clean,Build || goto :ERROR
echo msbuild "%SlnFile%" /p:Platform=x64   /p:Configuration=Debug
     msbuild "%SlnFile%" /p:Platform=x64   /p:Configuration=Debug   /nologo /v:quiet /m /t:Clean,Build || goto :ERROR
echo msbuild "%SlnFile%" /p:Platform=Win32 /p:Configuration=Debug
     msbuild "%SlnFile%" /p:Platform=Win32 /p:Configuration=Debug   /nologo /v:quiet /m /t:Clean,Build || goto :ERROR
popd

pushd "projects\vs2022\example_plugin"
set "SlnFile=example_plugin.sln"
echo msbuild "%SlnFile%" /p:Platform=x64   /p:Configuration=Release
     msbuild "%SlnFile%" /p:Platform=x64   /p:Configuration=Release /nologo /v:quiet /m /t:Clean,Build || goto :ERROR
echo msbuild "%SlnFile%" /p:Platform=Win32 /p:Configuration=Release
     msbuild "%SlnFile%" /p:Platform=Win32 /p:Configuration=Release /nologo /v:quiet /m /t:Clean,Build || goto :ERROR
echo msbuild "%SlnFile%" /p:Platform=x64   /p:Configuration=Debug
     msbuild "%SlnFile%" /p:Platform=x64   /p:Configuration=Debug   /nologo /v:quiet /m /t:Clean,Build || goto :ERROR
echo msbuild "%SlnFile%" /p:Platform=Win32 /p:Configuration=Debug
     msbuild "%SlnFile%" /p:Platform=Win32 /p:Configuration=Debug   /nologo /v:quiet /m /t:Clean,Build || goto :ERROR
popd


if not exist "%~dp0artifacts\Win32\Debug" (
       mkdir "%~dp0artifacts\Win32\Debug"           2>nul || goto :ERROR
)
if not exist "%~dp0artifacts\Win32\Debug\Plugins" (
       mkdir "%~dp0artifacts\Win32\Debug\Plugins"   2>nul || goto :ERROR
)
if not exist "%~dp0artifacts\Win32\Release" (
       mkdir "%~dp0artifacts\Win32\Release"         2>nul || goto :ERROR
)
if not exist "%~dp0artifacts\Win32\Release\Plugins" (
       mkdir "%~dp0artifacts\Win32\Release\Plugins" 2>nul || goto :ERROR
)
if not exist "%~dp0artifacts\x64\Debug" (
       mkdir "%~dp0artifacts\x64\Debug"             2>nul || goto :ERROR
)
if not exist "%~dp0artifacts\x64\Debug\Plugins" (
       mkdir "%~dp0artifacts\x64\Debug\Plugins"     2>nul || goto :ERROR
)
if not exist "%~dp0artifacts\x64\Release" (
       mkdir "%~dp0artifacts\x64\Release"           2>nul || goto :ERROR
)
if not exist "%~dp0artifacts\x64\Release\Plugins" (
       mkdir "%~dp0artifacts\x64\Release\Plugins"   2>nul || goto :ERROR
)

copy projects\vs2022\version\Win32\Debug\*.dll          "%~dp0artifacts\Win32\Debug"           || goto :ERROR
copy projects\vs2022\version\Win32\Release\*.dll        "%~dp0artifacts\Win32\Release"         || goto :ERROR
copy projects\vs2022\version\x64\Debug\*.dll            "%~dp0artifacts\x64\Debug"             || goto :ERROR
copy projects\vs2022\version\x64\Release\*.dll          "%~dp0artifacts\x64\Release"           || goto :ERROR
copy projects\vs2022\example_plugin\Win32\Debug\*.dll   "%~dp0artifacts\Win32\Debug\Plugins"   || goto :ERROR
copy projects\vs2022\example_plugin\Win32\Release\*.dll "%~dp0artifacts\Win32\Release\Plugins" || goto :ERROR
copy projects\vs2022\example_plugin\x64\Debug\*.dll     "%~dp0artifacts\x64\Debug\Plugins"     || goto :ERROR
copy projects\vs2022\example_plugin\x64\Release\*.dll   "%~dp0artifacts\x64\Release\Plugins"   || goto :ERROR

echo.
echo "%~dp0artifacts" contains the following artifacts.
echo.
echo tree /a /f "%~dp0artifacts"
     tree /a /f "%~dp0artifacts"
echo.

echo Build Status - SUCCEEDED
set /a errorno=0
goto :END

:ERROR
echo Abort by error.
echo Build Status - ERROR

:END
popd
exit /B %errorno%
