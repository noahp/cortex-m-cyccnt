#!/usr/bin/env bash

# Simple test script to run the tests in docker

# Error on any non-zero command, and print the commands as they're run
set -ex

# Make sure we have the docker utility
if ! command -v docker; then
    echo "🐋 Please install docker first 🐋"
    exit 1
fi

docker run -v $(pwd):/mnt/workspace -t silkeh/clang:10 bash -c "
    clang -Weverything -Werror -c /mnt/workspace/cortexm-cycnt.h -o /dev/null
"
