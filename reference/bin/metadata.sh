#! /usr/bin/env bash

input=$1;
#echo "Processing $input";
klvoutput=${input%.*}.klv;
hexoutput=${input%.*}.hex;
ffmpeg -i $input -map data-re -codec copy -f data $klvoutput;
hexdump -C $klvoutput > $hexoutput;

