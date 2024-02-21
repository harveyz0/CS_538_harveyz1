#!/bin/sh
#
PROJ_DIR="/home/zac/Documents/classes/CS538/CS_538_harveyz1"
rm -rf $PROJ_DIR/build
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -S$PROJ_DIR -B$PROJ_DIR/build -G Unix\ Makefiles
cd $PROJ_DIR/build
make
cd -
