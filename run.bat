@echo off

@rem Ninja needs to be in path.
@rem Maybe It is possible to give the path to cmake, but I couldn't find how.
where ninja
IF %ERRORLEVEL% EQU 1   SET PATH=%PATH%;D:\Qt\Tools\Ninja

cmake -S D:/Coding/C++/crypto -B D:/Coding/C++/build/crypto -GNinja "-DCMAKE_BUILD_TYPE:String=Debug" "-DCMAKE_PREFIX_PATH:STRING=D:\msys64\mingw64" "-DCMAKE_C_COMPILER:STRING=D:/msys64/mingw64/bin/gcc.exe" "-DCMAKE_CXX_COMPILER:STRING=D:/msys64/mingw64/bin/g++.exe"

IF %ERRORLEVEL% NEQ 0 GOTO build_failed

@rem Build the project to get an exe.
cmake --build D:/Coding/C++/build/crypto

IF %ERRORLEVEL% NEQ 0 GOTO build_failed

@rem Call the exe.
D:/Coding/C++/build/crypto/crypto.exe

GOTO end_script

:build_failed
echo ======================= Build failed ===========================
popd
exit /b 1

:end_script