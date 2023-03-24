mkdir build
cd build

cmake -G "Ninja" ^
      -D CMAKE_PREFIX_PATH:FILEPATH="%PREFIX%" ^
      -D CMAKE_INSTALL_PREFIX:FILEPATH="%LIBRARY_PREFIX%" ^
      -D CMAKE_BUILD_TYPE="Release" ^
      -D SOQT_USE_QT5=ON ^
      -D SOQT_BUILD_DOCUMENTATION:BOOL=NO ^
      ..

if errorlevel 1 exit 1
ninja install
if errorlevel 1 exit 1
