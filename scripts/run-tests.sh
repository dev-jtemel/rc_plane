#!/bin/bash

# DO NOT CALL DIRECTLY

ROOT_DIR="$1"
COMPILE_ONLY="$2"

run_tests() {
    ./build/impl/test/RcplaneTests
    [ -d coverage ] && rm -rf coverage
    mkdir coverage && pushd coverage
    gcovr -r .. -e "../impl/test"  --html-details -o coverage.html 2>/dev/null
    popd
}

echo "***************************"
echo "Compiling and running tests"
echo "***************************"
bash $ROOT_DIR/scripts/compile-som.sh "$ROOT_DIR" "True"

[ "$COMPILE_ONLY" == "True" ]|| run_tests
echo "****"
echo "Done"
echo "****"
