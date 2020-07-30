@echo %1

pushd %~dp0

mkdir EmBuild

pushd EmBuild
call %1\emsdk_env.bat
call emmake mingw32-make
popd

copy Sources\WebApp\index.html %2\index.html >NUL
copy EmBuild\Release\WebApp.js %2\WebApp.js >NUL
copy EmBuild\Release\WebApp.wasm %2\WebApp.wasm >NUL
copy EmBuild\Release\WebApp.data %2\WebApp.data >NUL

popd
