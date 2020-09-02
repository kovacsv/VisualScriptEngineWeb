pushd %~dp0

pushd  BuildEm\Result
call npm install
popd

popd

call node BuildEm\Result\server.js
