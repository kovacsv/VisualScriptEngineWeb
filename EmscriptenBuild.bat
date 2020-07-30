@echo %1

pushd %~dp0

mkdir EmBuild

pushd EmBuild
call %1\emsdk_env.bat
call emcmake cmake -G "Unix Makefiles" -DEMSCRIPTEN=1 -DCMAKE_MAKE_PROGRAM=mingw32-make -DCMAKE_BUILD_TYPE=Release ..
call emmake mingw32-make
popd

copy Sources\DemoApp\main.html %2\main.html >NUL
copy EmBuild\Release\DemoApp.js %2\DemoApp.js >NUL
copy EmBuild\Release\DemoApp.wasm %2\DemoApp.wasm >NUL
copy EmBuild\Release\DemoApp.data %2\DemoApp.data >NUL

popd
