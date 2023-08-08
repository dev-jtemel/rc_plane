#!/bin/bash

# DO NOT CALL DIRECTLY

ROOT_DIR="$1"

run_tests() {
    ./build/common/test/rcplane_common_tests
    [ -d coverage ] && rm -rf coverage
    mkdir coverage && pushd coverage
    gcovr -r .. -e "../som" -e "../common/test/" -e "../third_party"  --html-details -o coverage.html
    popd
}

echo "***************************"
echo "Compiling and running tests"
echo "***************************"
bash $ROOT_DIR/scripts/compile_som.sh "$ROOT_DIR" "True"
run_tests
echo "****"
echo "Done"
echo "****"
