#!/bin/sh

DIR=$(cd $(dirname $0) && pwd)

source $DIR/build_config.sh

mkdir -p $DIR/sample/helloworld/_build
pushd $DIR/sample/helloworld/_build
cmake -DGRPC_INSTALL_DIR=$GRPC_INSTALL_DIR \
      -DBOOST_INSTALL_DIR=$BOOST_INSTALL_DIR \
      -DSERVICE_KIT_INSTALL_DIR=$SERVICE_KIT_INSTALL_DIR \
      -DCMAKE_INSTALL_PREFIX=$DIR/sample \
      $CMAKE_EXTRA_FLAGS \
      ..
make -j$CORE_COUNT
make install
popd
