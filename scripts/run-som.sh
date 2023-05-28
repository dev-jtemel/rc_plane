#!/bin/bash

# DO NOT CALL DIRECTLY

ROOT_DIR="$1"
BUILD_DIR="$ROOT_DIR/build_som"

run_som() {
  $BUILD_DIR/som/som-controller
}

echo "*************************"
echo "Running SOM"
echo "*************************"
run_som
echo "****"
echo "Done"
echo "****"
