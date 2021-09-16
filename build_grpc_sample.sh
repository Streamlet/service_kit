#!/bin/sh

DIR=$(cd $(dirname $0) && pwd)
CMAKE_EXTRA_FLAGS="-DCMAKE_CXX_STANDARD=17"
GRPC_INSTALL_DIR=$DIR/third_party/grpc/built

echo "================================"
echo "Building grpc cpp sample ..."
mkdir -p $DIR/third_party/grpc/examples/cpp/helloworld/cmake/build
cd $DIR/third_party/grpc/examples/cpp/helloworld/cmake/build
cmake -DCMAKE_PREFIX_PATH=$GRPC_INSTALL_DIR $CMAKE_EXTRA_FLAGS ../..
make -j
