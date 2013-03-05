mkdir build_win32
set INSTALL_DIR=%~dp0\..\installed
cd build_win32
cmake -G "Visual Studio 11 Win64" -DBUILD_SHARED_LIBS=true -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR% ..
pause