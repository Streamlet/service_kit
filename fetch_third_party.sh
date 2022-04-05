#!/bin/sh

DIR=$(cd $(dirname $0) && pwd)

mkdir $DIR/third_party
pushd $DIR/third_party

echo ================================
echo Fetching grpc ...
GRPC_VERSION=v1.45.1
if [ ! -d grpc ]; then
    git clone --depth 1 --shallow-submodules --single-branch --recurse-submodules \
        -b $GRPC_VERSION https://github.com/grpc/grpc
else
    echo exists
    pushd grpc
    git submodule update --recursive
    popd
fi

echo ================================
echo Fetching boost ...
BOOST_VERSION=1.78.0
if [ ! -d boost ]; then
    curl -L https://boostorg.jfrog.io/artifactory/main/release/$BOOST_VERSION/source/boost_${BOOST_VERSION//./_}.tar.gz -o boost_${BOOST_VERSION//./_}.tar.gz
    tar -zxvf boost_${BOOST_VERSION//./_}.tar.gz
    mv boost_${BOOST_VERSION//./_} boost
    rm -rf boost_${BOOST_VERSION//./_}.tar.gz
else
    echo exists
fi

popd
