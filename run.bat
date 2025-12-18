@echo off
setlocal

set SEARCH_DIR=build
pushd "%SEARCH_DIR%" || ( echo Failed to enter "%SEARCH_DIR%" & exit /b 1 )

rem Find the first .exe (alphabetically) in this directory
for /f "usebackq delims=" %%F in (`dir /b /a:-d /o:n *.exe 2^>nul`) do (
    set "FIRST_EXE=%%~fF"
    goto :found
)

echo No .exe files found in "%SEARCH_DIR%".
popd
exit /b 1

:found
echo Launching "%FIRST_EXE%"...
"%FIRST_EXE%"
set "RC=%ERRORLEVEL%"
popd
exit /b %RC%