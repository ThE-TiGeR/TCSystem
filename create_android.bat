@echo off
call D:\Development\Android\env.bat
set INSTALL_DIR=D:/Development/Android/installed
set BOOST_ROOT=%INSTALL_DIR%/include/boost-1.48.0
mkdir build_android
pushd build_android
call android_cmake -DBUILD_SHARED_LIBS=1 -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR% ..
cmake --build .
popd
pause
