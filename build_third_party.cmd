@echo off

set DIR=%~dp0

call %DIR%build_config.cmd


echo ================================
echo Building grpc ...
set _GRPC_BUILD_DIR=%DIR%third_party\grpc\_build
mkdir %_GRPC_BUILD_DIR%
pushd %_GRPC_BUILD_DIR%
cmake -DgRPC_INSTALL=ON ^
      -DgRPC_BUILD_TESTS=OFF ^
      -DCMAKE_INSTALL_PREFIX=%GRPC_INSTALL_DIR% ^
      -DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE% ^
      %CMAKE_EXTRA_FLAGS% ^
      ..
cmake --build . --config %CMAKE_BUILD_TYPE% --parallel %CORE_COUNT%
cmake --install . --config %CMAKE_BUILD_TYPE%
popd


echo ================================
echo Building abseil ...
set _ABSEIL_BUILD_DIR=%DIR%third_party\grpc\third_party\abseil-cpp\_build
mkdir %_ABSEIL_BUILD_DIR%
pushd %_ABSEIL_BUILD_DIR%
cmake -DCMAKE_INSTALL_PREFIX=%GRPC_INSTALL_DIR% ^
      -DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE% ^
      -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE ^
      %CMAKE_EXTRA_FLAGS% ^
      ..
cmake --build . --config %CMAKE_BUILD_TYPE% --parallel %CORE_COUNT%
cmake --install . --config %CMAKE_BUILD_TYPE%
popd


echo ================================
echo Building boost ...

set _BOOST_BUILD_DIR=%DIR%third_party\boost\_build
pushd %DIR%third_party\boost
if not exist b2.exe (
    .\bootstrap
)
.\b2 install ^
     link=static ^
     cxxflags="/std:c++17" ^
     address-model=64 ^
     variant=%BOOST_VARIANT% ^
     define=BOOST_ALL_NO_LIB ^
     -j %CORE_COUNT% ^
     --build-dir=%_BOOST_BUILD_DIR% ^
     --prefix=%THIRD_PARTY_INSTALL_DIR% ^
     --layout=system ^
     --with-program_options ^
     --with-log ^
     --with-atomic
popd

pushd %THIRD_PARTY_INSTALL_DIR%\lib
del /S /Q boost_*.lib
rename "libboost_*.lib" "///boost_*.lib"
popd