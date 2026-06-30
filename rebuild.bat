@echo off
REM ============================================================
REM  QtGSI 全量重新编译脚本 — Qt 6.7.3 MSVC 2019 64-bit
REM  用法: 双击运行 (清除后重新编译所有文件)
REM ============================================================

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>nul
set PATH=%PATH:D:\Strawberry\c\bin;=%

set BUILD=d:\Programs\Qt\6.7.3Project\QtGSI\build\Desktop_Qt_6_7_3_MSVC2019_64bit-Release

echo [Build] Full rebuild (clean + build)...
cmake --build "%BUILD%" --config Release --clean-first
if errorlevel 1 ( pause & exit /b 1 )

echo [OK] Rebuild succeeded.
timeout /t 3 >nul
