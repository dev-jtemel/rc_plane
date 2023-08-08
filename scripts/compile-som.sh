#!/bin/bash

# DO NOT CALL DIRECTLY

ROOT_DIR="$1"
BUILD_DIR="$ROOT_DIR/build"
BUILD_TESTS="$2"

compile_som() {
  [ ! -d $BUILD_DIR ] && mkdir $BUILD_DIR
  cd $BUILD_DIR
  cmake .. -DSOM_BUILD=True -DBUILD_TESTS=${BUILD_TESTS}
  make -j$(nproc)
}

echo "*************************"
echo "Compiling SOM"
echo "*************************"
compile_som
echo "****"
echo "Done"
echo "****"
