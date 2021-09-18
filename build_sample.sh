#!/bin/sh

DIR=$(cd $(dirname $0) && pwd)
CMAKE_EXTRA_FLAGS="-DCMAKE_CXX_STANDARD=17"
GRPC_INSTALL_DIR=$DIR/third_party/grpc/built

echo "================================"
echo "Building registry center ..."
mkdir -p $DIR/registry_center/cmake/build
pushd $DIR/registry_center/cmake/build
cmake -DCMAKE_PREFIX_PATH=$GRPC_INSTALL_DIR $CMAKE_EXTRA_FLAGS ../..
make -j
popd

echo "================================"
echo "Building sample ..."
mkdir -p $DIR/sample/helloworld/cmake/build
pushd $DIR/sample/helloworld/cmake/build
cmake -DCMAKE_PREFIX_PATH=$GRPC_INSTALL_DIR $CMAKE_EXTRA_FLAGS ../..
make -j
popd
