#!/bin/bash

for i in `seq 1 10`; do
	plantri $i -a -m1 -c1 -p >> planar_graphs.txt
	echo $i - vertices
done

