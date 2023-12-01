#!/bin/bash

train_images="./input/train_images"
test_images="./input/test_images"
output_prefix="./output/out"
csv_file="./output/out.csv"
config_file="./src/bench.conf"
sizes=(100 500 1000 2000 5000 10000 20000 30000 40000 0)

echo "Metric,LSH,Cube,GNNS,MRNG" > "$csv_file"

make -s benchmark

for size in "${sizes[@]}"; do
    output_file="${output_prefix}${size}.txt"
    ./benchmark -d "$train_images" -q "$test_images" -o "$output_file" -c "$csv_file" -config "$config_file" -size "$size"
done

make clean