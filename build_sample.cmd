@echo off

set DIR=%~dp0

call %DIR%build_config.cmd

mkdir -p %DIR%sample\helloworld\_build
pushd %DIR%sample\helloworld\_build
cmake -DGRPC_INSTALL_DIR=%GRPC_INSTALL_DIR% ^
      -DBOOST_INSTALL_DIR=%BOOST_INSTALL_DIR% ^
      -DSERVICE_KIT_INSTALL_DIR=%SERVICE_KIT_INSTALL_DIR% ^
      -DCMAKE_INSTALL_PREFIX=%DIR%sample ^
      -DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE% ^
      %CMAKE_EXTRA_FLAGS% ^
      ..
cmake --build . --config %CMAKE_BUILD_TYPE% --parallel %CORE_COUNT%
cmake --install . --config %CMAKE_BUILD_TYPE%
popd
