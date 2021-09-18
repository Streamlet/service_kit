#!/bin/sh

DIR=$(cd $(dirname $0) && pwd)
CMAKE_EXTRA_FLAGS="-DCMAKE_CXX_STANDARD=17"


pushd $DIR/third_party

echo "================================"
echo "Building grpc ..."
GRPC_INSTALL_DIR=$DIR/third_party/grpc/built
mkdir -p $THIRD_PARTY_INSTALL_DIR
mkdir -p grpc/cmake/build
pushd grpc/cmake/build
cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$GRPC_INSTALL_DIR \
      $CMAKE_EXTRA_FLAGS \
      ../..
make -j
make install
popd

echo "================================"
echo "Building abseil ..."

mkdir -p grpc/third_party/abseil-cpp/cmake/build
pushd grpc/third_party/abseil-cpp/cmake/build
cmake -DCMAKE_INSTALL_PREFIX=$GRPC_INSTALL_DIR \
      -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE \
      $CMAKE_EXTRA_FLAGS \
      ../..
make -j
make install
popd

echo "================================"
echo "Building boost ..."
BOOST_INSTALL_DIR=$DIR/third_party/boost/built

pushd boost
./bootstrap.sh --prefix=$BOOST_INSTALL_DIR --with-program_options
./b2 install
popd


popd
