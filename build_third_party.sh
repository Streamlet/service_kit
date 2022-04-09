#!/bin/sh

DIR=$(cd $(dirname $0) && pwd)

source $DIR/build_config.sh


echo ================================
echo Building grpc ...
_GRPC_BUILD_DIR=$DIR/third_party/grpc/_build
mkdir -p $_GRPC_BUILD_DIR
pushd $_GRPC_BUILD_DIR
cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$GRPC_INSTALL_DIR \
      -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
      $CMAKE_EXTRA_FLAGS \
      ..
cmake --build . --parallel $CORE_COUNT
cmake --install .
popd


echo ================================
echo Building abseil ...
_ABSEIL_BUILD_DIR=$DIR/third_party/grpc/third_party/abseil-cpp/_build
mkdir -p $_ABSEIL_BUILD_DIR
pushd $_ABSEIL_BUILD_DIR
cmake -DCMAKE_INSTALL_PREFIX=$GRPC_INSTALL_DIR \
      -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
      -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE \
      $CMAKE_EXTRA_FLAGS \
      ..
cmake --build . --parallel $CORE_COUNT
cmake --install .
popd


echo ================================
echo Building boost ...

_BOOST_BUILD_DIR=$DIR/third_party/boost/_build
pushd $DIR/third_party/boost
if [ ! -f b2 ]; then
    ./bootstrap.sh
fi

if [ "$CORE_COUNT" != "" ]; then
    _J_CORE_COUNT="-j $CORE_COUNT"
else
    _J_CORE_COUNT=''
fi

./b2 install \
     link=static \
     cxxflags="-std=c++17" \
     variant=$BOOST_VARIANT \
     $_J_CORE_COUNT \
     --layout=system \
     --build-dir=$_BOOST_BUILD_DIR \
     --prefix=$THIRD_PARTY_INSTALL_DIR \
     --with-program_options \
     --with-log
popd
