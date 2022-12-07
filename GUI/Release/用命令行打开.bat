

set "CURRENT_DIR=%cd%"

cd "%CURRENT_DIR%"

echo Would you like the log to be written in the console("c") or in the file("f")?

set /p opt = :

if /i "%opt%"=="c"  call :1

if /i "%opt%"=="f" call :2

:1
GUI.exe

:2
GUI.exe>latest.log