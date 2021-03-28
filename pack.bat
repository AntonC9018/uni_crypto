@echo off
pushd
mkdir ..\build\crypto_windows
cp ../build/crypto/crypto.exe ../build/crypto_windows/crypto.exe
ldd ../build/crypto/crypto.exe | grep mingw64 | cut -f 1 -d ' ' > libs.txt

@rem F*CK THIS, I'M GOING TO COPY THEM MANUALLY
@rem I have wasted 2 hours and was not able to PREPEND A STRING to the part paths. 
@rem Terminal programming is terribly disguisting.
@REM cat libs.txt | xargs cp -v --target-directory=../build/crypto_windows

popd