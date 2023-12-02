#!/bin/bash

train_images="./input/train_images"
test_images="./input/test_images"
output_prefix="./output/out"
csv_file="./output/out.csv"
config_file="./src/bench.conf"
sizes=(100 500 1000 2000 5000 10000 20000 30000 40000 60000)

gnns_prefix="./graphs/graph_gnns_"
mrng_prefix="./graphs/graph_mrng_"

echo "Metric,LSH,Cube,GNNS,MRNG" > "$csv_file"

make -s benchmark

for size in "${sizes[@]}"; do
    output_file="${output_prefix}${size}.txt"
    gnns_file="${gnns_prefix}${size}.csv"
    mrng_file="${mrng_prefix}${size}.csv"

    if [ -e "$gnns_file" ]; then
        gnns="-gnns_load"
    else
        gnns="-gnns_save"
    fi

    if [ -e "$mrng_file" ]; then
        mrng="-mrng_load"
    else
        mrng="-mrng_save"
    fi

    ./benchmark \
        -d "$train_images" \
        -q "$test_images" \
        -o "$output_file" \
        -c "$csv_file" \
        -config "$config_file" \
        -size "$size" \
        "$gnns" "$gnns_file" \
        "$mrng" "$mrng_file" 
done

make clean

python ./output/plot.py