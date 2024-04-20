#!/bin/bash

#Regen 'newlinetest' file so i dont have to manually repair it to its original state

filename="testgrounds/newlinetest"

rm -f $filename

for ((i=1; i<=10; i++))
do
    echo "$i" >> "$filename"
done
