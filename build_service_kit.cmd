@echo off

set DIR=%~dp0

call %DIR%build_config.cmd

mkdir %DIR%src\_build
pushd %DIR%src\_build
cmake -DGRPC_INSTALL_DIR=%GRPC_INSTALL_DIR% ^
      -DTHIRD_PARTY_INSTALL_DIR=%THIRD_PARTY_INSTALL_DIR% ^
      -DCMAKE_INSTALL_PREFIX=%SERVICE_KIT_INSTALL_DIR% ^
      -DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE% ^
      %CMAKE_EXTRA_FLAGS% ^
      ..
cmake --build . --config %CMAKE_BUILD_TYPE% --parallel %CORE_COUNT%
cmake --install . --config %CMAKE_BUILD_TYPE%
popd
