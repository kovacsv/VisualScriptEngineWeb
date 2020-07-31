@echo %1

pushd %~dp0

mkdir EmBuild

pushd EmBuild
call %1\emsdk_env.bat
call emcmake cmake -G "Unix Makefiles" -DEMSCRIPTEN=1 -DCMAKE_MAKE_PROGRAM=mingw32-make -DCMAKE_BUILD_TYPE=Release ..
call emmake mingw32-make
popd

copy Sources\WebSite\index.html %2\index.html > NUL
copy Sources\WebSite\style.css %2\style.css > NUL
copy Sources\WebSite\folder_opened.png %2\folder_opened.png > NUL
copy Sources\WebSite\folder_closed.png %2\folder_closed.png > NUL
copy Sources\WebSite\plus.png %2\plus.png > NUL
copy EmBuild\Release\WebApp.js %2\WebApp.js > NUL
copy EmBuild\Release\WebApp.wasm %2\WebApp.wasm > NUL
copy EmBuild\Release\WebApp.data %2\WebApp.data > NUL

popd
