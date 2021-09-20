#!/bin/sh

DIR=$(cd $(dirname $0) && pwd)

source $DIR/build_config.sh

mkdir -p $DIR/src/_build
pushd $DIR/src/_build
cmake -DGRPC_INSTALL_DIR=$GRPC_INSTALL_DIR \
      -DTHIRD_PARTY_INSTALL_DIR=$THIRD_PARTY_INSTALL_DIR \
      -DCMAKE_INSTALL_PREFIX=$SERVICE_KIT_INSTALL_DIR \
      $CMAKE_EXTRA_FLAGS \
      ..
make -j$CORE_COUNT
make install
popd
