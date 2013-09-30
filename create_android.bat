rem @echo off
call D:\Development\Android\env.bat
mkdir build_android
pushd build_android
call android_cmake ..
cmake --build .
popd
pause
