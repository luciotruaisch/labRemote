#!/bin/bash

config=$1
dir=$2
load=$3
GPIB=$4

i=0

while [ $i -lt 100 ]
do
./bin/pbv3_long_test -c $config $dir $load $GPIB
i=$[$i+1]
done 
