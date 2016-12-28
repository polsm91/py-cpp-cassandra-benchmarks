#!/bin/bash
for f in *.py
do
	echo "Executing test $f" &>> out_pytests
	n=1
	while (($n<=256))
	do
	        (/usr/bin/time --format '%E %U %S ' python $f $n) &>> out_pytests
		wait
		n=$((n*2))
	done
done
