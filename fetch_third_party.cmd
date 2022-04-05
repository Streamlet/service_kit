@echo off

set DIR=%~dp0

mkdir %DIR%third_party
pushd %DIR%third_party

echo ================================
echo Fetching grpc ...
set GRPC_VERSION=v1.45.1
if not exist grpc (
    echo .
    git clone --depth 1 --shallow-submodules --single-branch --recurse-submodules ^
        -b %GRPC_VERSION% https://github.com/grpc/grpc
) else (
    echo exists
    pushd grpc
    git submodule update --recursive
    popd
)

echo ================================
echo Fetching boost ...
set BOOST_VERSION=1.78.0
if not exist boost (
    echo .
    curl -L https://boostorg.jfrog.io/artifactory/main/release/%BOOST_VERSION%/source/boost_%BOOST_VERSION:.=_%.tar.gz -o boost_%BOOST_VERSION:.=_%.tar.gz
    tar -zxvf boost_%BOOST_VERSION:.=_%.tar.gz
    rename boost_%BOOST_VERSION:.=_% boost
    del /s /q boost_%BOOST_VERSION:.=_%.tar.gz
) else (
    echo exists
)

popd
