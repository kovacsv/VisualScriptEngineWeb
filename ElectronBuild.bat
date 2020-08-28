pushd %~dp0
mkdir BuildEl

call EmscriptenBuild.bat %1 BuildEl

xcopy Sources\ElectronApp\*.* BuildEl /s /i /Y > NUL

popd
