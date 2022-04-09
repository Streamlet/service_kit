#!/bin/sh

DIR=$(cd $(dirname $0) && pwd)

source $DIR/build_config.sh

mkdir -p $DIR/sample/helloworld/_build
pushd $DIR/sample/helloworld/_build
cmake -DGRPC_INSTALL_DIR=$GRPC_INSTALL_DIR \
      -DTHIRD_PARTY_INSTALL_DIR=$THIRD_PARTY_INSTALL_DIR \
      -DSERVICE_KIT_INSTALL_DIR=$SERVICE_KIT_INSTALL_DIR \
      -DCMAKE_INSTALL_PREFIX=$DIR/sample \
      -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
      $CMAKE_EXTRA_FLAGS \
      ..
cmake --build . --parallel $CORE_COUNT
cmake --install .
popd
