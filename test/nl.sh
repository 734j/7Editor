#!/bin/bash

filename="testgrounds/newlinetest"

rm -f $filename

for ((i=1; i<=10; i++))
do
    echo "$i" >> "$filename"
done

