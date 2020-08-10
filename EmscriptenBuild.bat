@echo %1

pushd %~dp0
mkdir EmBuild

pushd EmBuild
call %1\emsdk_env.bat || goto :error
call emcmake cmake -G "Unix Makefiles" -DEMSCRIPTEN=1 -DCMAKE_MAKE_PROGRAM=mingw32-make -DCMAKE_BUILD_TYPE=Release .. || goto :error
call emmake mingw32-make || goto :error
popd
echo Build Succeeded.


xcopy Sources\WebSite\*.* %2 /s /i /Y > NUL
copy EmBuild\Release\WebApp.js %2\WebApp.js > NUL
copy EmBuild\Release\WebApp.wasm %2\WebApp.wasm > NUL
copy EmBuild\Release\WebApp.data %2\WebApp.data > NUL

popd
goto :EOF

:error
echo Build Failed with Error %errorlevel%.
popd
popd
exit /b %errorlevel%
