#!/bin/bash
n=1
while (($n<=256))
do
        (/usr/bin/time --format '%E %U %S ' python2.7 test_th_cpp.py $n) &>> out_th_cpp
	wait
	n=$((n*2))
done
