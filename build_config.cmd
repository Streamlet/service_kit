@echo off

set DIR=%~dp0

if not defined GRPC_INSTALL_DIR (
    set GRPC_INSTALL_DIR=%DIR%.local
    echo GRPC_INSTALL_DIR not defined, default to %GRPC_INSTALL_DIR%
)

if not defined THIRD_PARTY_INSTALL_DIR (
    set THIRD_PARTY_INSTALL_DIR=%DIR%.local
    echo THIRD_PARTY_INSTALL_DIR not defined, default to %THIRD_PARTY_INSTALL_DIR%
)

if not defined SERVICE_KIT_INSTALL_DIR (
    set SERVICE_KIT_INSTALL_DIR=%DIR%.service_kit
    echo SERVICE_KIT_INSTALL_DIR not defined, default to %SERVICE_KIT_INSTALL_DIR%
)

if "%DEBUG_BUILD%" == "1" (
    set CMAKE_BUILD_TYPE=Debug
    set BOOST_VARIANT=debug
    echo Will performance a Debug build.
) else (
    set CMAKE_BUILD_TYPE=Release
    set BOOST_VARIANT=release
    echo Will performance a Release build.
)

set CMAKE_EXTRA_FLAGS="-DCMAKE_CXX_STANDARD=17"
set CORE_COUNT=%NUMBER_OF_PROCESSORS%
