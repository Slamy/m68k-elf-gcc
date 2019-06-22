#!/bin/bash
PROJECT=example

set -e

rm -rf ../${PROJECT}_build

mkdir ../${PROJECT}_build
cd ../${PROJECT}_build
cmake -G "Eclipse CDT4 - Unix Makefiles" $* ../$PROJECT
make -j4
