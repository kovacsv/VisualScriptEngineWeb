@echo %1

pushd %~dp0

mkdir EmBuild

pushd EmBuild
call %1\emsdk_env.bat
call emcmake cmake -G "Unix Makefiles" -DEMSCRIPTEN=1 -DCMAKE_MAKE_PROGRAM=mingw32-make ..
call emmake mingw32-make
popd

copy Sources\DemoApp\main.html %2\main.html >NUL
copy EmBuild\DemoApp.js %2\DemoApp.js >NUL
copy EmBuild\DemoApp.wasm %2\DemoApp.wasm >NUL

popd
