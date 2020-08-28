pushd %~dp0
mkdir BuildEl

call EmscriptenBuild.bat %1 BuildEl

xcopy Sources\ElectronApp\*.* BuildEl /s /i /Y > NUL
pushd BuildEl
REM call npm install -g electron
REM call npm install -g electron-forge
REM call npm install -g npx
REM call npm install -g make
call npx @electron-forge/cli import
call npm run make
popd

popd
