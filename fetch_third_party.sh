#!/bin/sh

DIR=$(cd $(dirname $0) && pwd)

mkdir $DIR/third_party
pushd $DIR/third_party

echo "================================"
echo "Fetching grpc ...\c"
GRPC_VERSION=v1.45.1
if [ ! -d grpc ]; then
    echo
    git clone --depth 1 --shallow-submodules --single-branch --recurse-submodules \
        -b $GRPC_VERSION https://github.com/grpc/grpc
else
    echo exists
    pushd grpc
    git submodule update --recursive
    popd
fi

echo "================================"
echo "Fetching boost ...\c"
BOOST_VERSION=1.78.0
if [ ! -d boost ]; then
    echo
    curl -L https://boostorg.jfrog.io/artifactory/main/release/$BOOST_VERSION/source/boost_${BOOST_VERSION//./_}.tar.bz2 -o boost_${BOOST_VERSION//./_}.tar.bz2
    tar -jxvf boost_${BOOST_VERSION//./_}.tar.bz2
    mv boost_${BOOST_VERSION//./_} boost
    rm -rf boost_${BOOST_VERSION//./_}.tar.bz2
else
    echo exists
fi

popd
