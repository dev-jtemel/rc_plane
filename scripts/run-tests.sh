#!/bin/bash

# DO NOT CALL DIRECTLY

ROOT_DIR="$1"
COMPILE_ONLY="$2"

run_tests() {
    # DISABLED
    # ./build/common/test/rcplane_common_tests
    #[ -d coverage ] && rm -rf coverage
    #mkdir coverage && pushd coverage
    #gcovr -r .. -e "../som" -e "../common/test/" -e "../third_party"  --html-details -o coverage.html
    #popd
    echo "DISABLED"
}

echo "***************************"
echo "Compiling and running tests"
echo "***************************"
bash $ROOT_DIR/scripts/compile-som.sh "$ROOT_DIR" "True"

[ $COMPILE_ONLY == "True" ] || run_tests
echo "****"
echo "Done"
echo "****"
