@echo off
echo Building Kai-BF...
gcc -Iinclude src/main.c src/interpreter.c -o kai-bf.exe
if %errorlevel% neq 0 (
    echo Build failed! Make sure GCC is installed.
) else (
    echo Build successful! Created kai-bf.exe
)
pause
