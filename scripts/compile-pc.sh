#!/bin/bash

# DO NOT CALL DIRECTLY

ROOT_DIR="$1"
BUILD_DIR="$ROOT_DIR/build_pc"

compile_pc() {
  [ ! -d $BUILD_DIR ] && mkdir $BUILD_DIR
  cd $BUILD_DIR
  cmake .. -DPC_BUILD=True
  make
}

echo "*************************"
echo "Compiling PC"
echo "*************************"
compile_pc
echo "****"
echo "Done"
echo "****"
