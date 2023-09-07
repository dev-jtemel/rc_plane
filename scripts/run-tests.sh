#!/bin/bash

# DO NOT CALL DIRECTLY

ROOT_DIR="$1"
COMPILE_ONLY="$2"

run_tests() {
    socat -d -d pty,raw,echo=0,link=/tmp/rcplane_test_tty_dev_read pty,raw,echo=0,link=/tmp/rcplane_test_tty_dev_write &
    tty=$!
    res=$?
    if [ ! $res -eq 0 ]
    then
      echo "Failed to start socat!"
      exit 1
    fi
    ./build/impl/test/RcplaneTests
    [ -d coverage ] && rm -rf coverage
    mkdir coverage && pushd coverage
    gcovr -r .. -e "../impl/test" -e "../third_party" -e "../impl/src/som/main.cpp" -e "../impl/src/pc/main.cpp" --html-details -o coverage.html 2>/dev/null
    popd
    kill -9 $tty
}

echo "***************************"
echo "Compiling and running tests"
echo "***************************"
bash $ROOT_DIR/scripts/compile-som.sh "$ROOT_DIR" "True"

[ "$COMPILE_ONLY" == "True" ] || run_tests
echo "****"
echo "Done"
echo "****"
