#!/bin/bash

# DO NOT CALL DIRECTLY

ROOT_DIR="$1"
BUILD_DIR="$ROOT_DIR/build_som"

run_sim() {
  $BUILD_DIR/som/simulator
}

echo "*************************"
echo "Running Simulator"
echo "*************************"
run_sim
echo "****"
echo "Done"
echo "****"
