#!/bin/bash

make -s benchmark

./benchmark -d ./input/train_images -q ./input/test_images -o ./output/out1.txt   -config ./src/bench.conf -size 100
./benchmark -d ./input/train_images -q ./input/test_images -o ./output/out2.txt   -config ./src/bench.conf -size 500
./benchmark -d ./input/train_images -q ./input/test_images -o ./output/out3.txt   -config ./src/bench.conf -size 1000
./benchmark -d ./input/train_images -q ./input/test_images -o ./output/out4.txt   -config ./src/bench.conf -size 2000
./benchmark -d ./input/train_images -q ./input/test_images -o ./output/out5.txt   -config ./src/bench.conf -size 5000
./benchmark -d ./input/train_images -q ./input/test_images -o ./output/out6.txt   -config ./src/bench.conf -size 10000
./benchmark -d ./input/train_images -q ./input/test_images -o ./output/out7.txt   -config ./src/bench.conf -size 20000
./benchmark -d ./input/train_images -q ./input/test_images -o ./output/out8.txt   -config ./src/bench.conf -size 30000
./benchmark -d ./input/train_images -q ./input/test_images -o ./output/out9.txt   -config ./src/bench.conf -size 40000
./benchmark -d ./input/train_images -q ./input/test_images -o ./output/out10.txt  -config ./src/bench.conf -size 0

make clean