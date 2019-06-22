#!/bin/bash -e

# Build for AmigaDOS
./build.sh -DAMIGACROSS=1 -DBUILD_FOR_AMIGADOS=1

#Build for NDOS
./build.sh -DAMIGACROSS=1 -DBUILD_FOR_AMIGADOS=0

#Build for Linux
./build.sh -DAMIGACROSS=0

echo "Everything went fine"
