@echo %1

pushd %~dp0

mkdir EmBuild

pushd EmBuild
call %1\emsdk_env.bat
call emcmake cmake -G "Unix Makefiles" -DEMSCRIPTEN=1 -DCMAKE_MAKE_PROGRAM=mingw32-make -DCMAKE_BUILD_TYPE=Release ..
call emmake mingw32-make
popd

xcopy Sources\WebSite\*.* %2 /s /i /Y > NUL
copy EmBuild\Release\WebApp.js %2\WebApp.js > NUL
copy EmBuild\Release\WebApp.wasm %2\WebApp.wasm > NUL
copy EmBuild\Release\WebApp.data %2\WebApp.data > NUL

popd
