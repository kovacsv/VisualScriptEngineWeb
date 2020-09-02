pushd %~dp0
mkdir BuildEm

pushd BuildEm
call %1\emsdk_env.bat || goto :error
call emcmake cmake -G "Unix Makefiles" -DEMSCRIPTEN=1 -DCMAKE_MAKE_PROGRAM=mingw32-make -DCMAKE_BUILD_TYPE=Release .. || goto :error
call emmake mingw32-make || goto :error
popd
echo Build Succeeded.

set resultFolder=BuildEm\Result
xcopy Sources\WebServer\*.* %resultFolder% /s /i /Y > NUL
xcopy Sources\WebSite\*.* %resultFolder%\public /s /i /Y > NUL
copy BuildEm\Release\WebApp.js BuildEm\Result\public\WebApp.js > NUL
copy BuildEm\Release\WebApp.wasm BuildEm\Result\public\WebApp.wasm > NUL
copy BuildEm\Release\WebApp.data BuildEm\Result\public\WebApp.data > NUL

popd
exit /b 0

:error
echo Build Failed with Error %errorlevel%.
popd
popd
exit /b %errorlevel%
