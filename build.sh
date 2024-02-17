#!/bin/sh
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -S/home/zac/Documents/classes/CS538/CS_538_harveyz1 -B/home/zac/Documents/classes/CS538/CS_538_harveyz1/build -G Unix\ Makefiles
