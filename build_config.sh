#!/bin/sh

DIR=$(cd $(dirname $0) && pwd)

if [ -z $GRPC_INSTALL_DIR ]; then
    GRPC_INSTALL_DIR=$DIR/.local
    echo GRPC_INSTALL_DIR not defined, default to $GRPC_INSTALL_DIR
fi

if [ -z $THIRD_PARTY_INSTALL_DIR ]; then
    THIRD_PARTY_INSTALL_DIR=$DIR/.local
    echo THIRD_PARTY_INSTALL_DIR not defined, default to $THIRD_PARTY_INSTALL_DIR
fi

if [ -z $SERVICE_KIT_INSTALL_DIR ]; then
    SERVICE_KIT_INSTALL_DIR=$DIR/.service_kit
    echo SERVICE_KIT_INSTALL_DIR not defined, default to $SERVICE_KIT_INSTALL_DIR
fi

if [ "$DEBUG_BUILD" == "1" ]; then
    CMAKE_BUILD_TYPE=Debug
    BOOST_VARIANT=debug
    echo Will performance a Debug build.
else
    CMAKE_BUILD_TYPE=Release
    BOOST_VARIANT=release
    echo Will performance a Release build.
fi

CMAKE_EXTRA_FLAGS="-DCMAKE_CXX_STANDARD=17"
CORE_COUNT=$(sysctl -a | grep machdep.cpu.core_count | awk '{ print $2 }')
