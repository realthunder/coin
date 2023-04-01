mkdir -p build
cd build
set -x

declare -a CMAKE_PLATFORM_FLAGS

if [[ ${HOST} =~ .*linux.* ]] ; then
    if test ${CONDA_BUILD_CROSS_COMPILATION}; then
        CMAKE_PLATFORM_FLAGS+=(-DCMAKE_TOOLCHAIN_FILE="${RECIPE_DIR}/cross-linux.cmake")
    fi
fi

cmake --debug-find --debug-output -G "Ninja" \
      -D CMAKE_INSTALL_PREFIX=$PREFIX \
      -D CMAKE_PREFIX_PATH=$PREFIX \
      -D CMAKE_BUILD_TYPE="Release" \
      -D SOQT_USE_QT5=ON \
      -D SOQT_BUILD_DOCUMENTATION:BOOL=NO \
      ${CMAKE_PLATFORM_FLAGS[@]} \
      ..

ninja install
