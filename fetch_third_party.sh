#!/bin/sh

DIR=$(cd $(dirname $0) && pwd)

pushd $DIR/third_party

echo "================================"
echo "Fetching grpc ...\c"
if [ ! -d grpc ]; then
    echo
    git clone --depth 1 --shallow-submodules --single-branch --recurse-submodules \
        -b v1.40.0 https://github.com/grpc/grpc
else
    echo exists
fi

echo "================================"
echo "Fetching boost ...\c"
if [ ! -d boost ]; then
    echo
    curl -L https://boostorg.jfrog.io/artifactory/main/release/1.77.0/source/boost_1_77_0.tar.bz2 -o boost_1_77_0.tar.bz2
    tar -jxvf boost_1_77_0.tar.bz2
    mv boost_1_77_0 boost
    rm -rf zxvf boost_1_77_0.tar.bz2
else
    echo exists
fi

popd
