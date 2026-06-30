@echo off
REM ============================================================
REM  QtGSI 编译脚本 — Qt 6.7.3 MSVC 2019 64-bit
REM  参考: CMake PREFIX_PATH + Ninja + windeployqt 标准流程
REM  用法: 双击运行，或在 x64 Native Tools 终端中运行
REM ============================================================

echo.
echo ============================================================
echo  QtGSI Build Script
echo ============================================================
echo.

REM --- 加载 MSVC 编译器环境 (必须在 CMake 之前) ---
where cl.exe >nul 2>nul
if errorlevel 1 (
    echo [ENV] Loading MSVC x64 environment...
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
)

REM --- 移除 MinGW 路径干扰 (Strawberry Perl 等) ---
set PATH=%PATH:D:\Strawberry\c\bin;=%

REM --- 路径设置 ---
set SRC=d:\Programs\Qt\6.7.3Project\QtGSI
set OUT=d:\Programs\Qt\6.7.3Project\QtGSI\build\Desktop_Qt_6_7_3_MSVC2019_64bit-Release
set QT_KIT=C:/Programs/Qt/Qt6.7.3/6.7.3/msvc2019_64

echo  Source: %SRC%
echo  Output: %OUT%
echo  Qt Kit: %QT_KIT%
echo.

REM --- 首次配置 (缓存不存在时) ---
if not exist %OUT%\CMakeCache.txt (
    echo [1/3] CMake configure (Ninja + MSVC)...
    cmake -S "%SRC%" -B "%OUT%" -G Ninja -DCMAKE_PREFIX_PATH="%QT_KIT%" -DCMAKE_CXX_COMPILER=cl -DCMAKE_C_COMPILER=cl -DCMAKE_BUILD_TYPE=Release
    if errorlevel 1 (
        echo [ERROR] CMake configure failed!
        pause
        exit /b 1
    )
)

REM --- 构建 ---
echo [2/3] Building...
cmake --build "%OUT%" --config Release

if errorlevel 1 (
    echo.
    echo [ERROR] Build failed!
    pause
    exit /b 1
)

REM --- 部署运行时依赖 (取消注释以启用) ---
REM echo [3/3] Deploying Qt dependencies...
REM "%QT_KIT%\bin\windeployqt6.exe" --release "%OUT%\QtGSI.exe"

echo.
echo ============================================================
echo  Build OK!
echo  Executable: %OUT%\QtGSI.exe
echo ============================================================
pause
